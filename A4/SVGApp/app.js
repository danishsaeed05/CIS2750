'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');

const mysql = require('mysql2/promise');


app.use(fileUpload());
app.use(express.static(path.join(__dirname + '/uploads')));


// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
  fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
  if (!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function (err) {
    if (err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
  fs.stat('uploads/' + req.params.name, function (err, stat) {
    if (err == null) {
      res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: ' + err);
      res.send('');
    }
  });
});

//******************** Your code goes here ********************



app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

let sharedLib = ffi.Library('./libsvgparse', {

  'createValidSVGimage': ['pointer', ['string', 'string']],
  'SVGtoJSON': ['string', ['pointer']],
  'test': ['string', ['string']],
  'requestCreateSVG': ['string', ['string', 'string', 'string']],
  'addRectangle': ['void', ['string', 'string', 'float', 'float', 'float', 'float', 'string']],
  'scaleShape': ['void', ['string', 'string', 'string']],
  'getTitle': ['string', ['string']],
  'getDescription': ['string', ['string']],
  'editAttribute': ['void', ['string', 'string', 'string']],
});


app.get('/svg', function (req, res) {

  var r = [];
  var dir;
  let files = fs.readdirSync('./uploads');

  for (var i = 0; i < files.length; i++) {
    dir = "./uploads/" + files[i];
    var syn = fs.statSync(dir);
    var sizeB = syn["size"];
    let sizeK = sizeB / 1024;
    var whole = Math.round(sizeK);
    var fileSize = whole.toString(8) + "KB";


    var svg = sharedLib.createValidSVGimage("./uploads/" + files[i], "svg.xsd");
    var json = sharedLib.SVGtoJSON(svg);

    let jsonObj = JSON.parse(json);
    jsonObj["filename"] = files[i];
    jsonObj["fileSize"] = fileSize;

    r[i] = JSON.stringify(jsonObj);
  }

  console.log("In SVG Log func");
  res.send(r);
});



/* View Panel */
app.get('/panel/:filename', function (req, res) {
  let file = req.params.filename;
  console.log("fileNAME is: " + file);

  var jsonObj = sharedLib.test(file);
  var title = sharedLib.getTitle(file);
  var description = sharedLib.getDescription(file);
  console.log("\n\nJSON Objects: \n" + jsonObj + "\n\nTitle: \n" + title + "\n\nDescription: \n" + description);

  // let p = JSON.parse(jsonObj);
  // console.log(p);

  let viewPanel = {
    title: title,
    description: description,
    json: jsonObj
  }

  //console.log(jsonObj);
  //
  // let jsonData = {
  //   var allListsOfAttrJSON = p;
  // }
  res.send(viewPanel);
});


/* Create SVG Image */
app.get('/createSVG', function (req, res) {
  let file = req.query.file;
  let title = req.query.title;
  let description = req.query.description;

  console.log("\n\n File Name received is: " + file + "\n\n");
  console.log("\n\n Title = " + title + " and description = " + description);

  var jsonObj = sharedLib.requestCreateSVG(file, title, description);
  console.log("\n\n" + jsonObj + "\n\n");


  res.send(jsonObj);
});

/* Add Shape */
app.get('/addShape', function (req, res) {
  let file = req.query.filename;
  console.log("\n\n File Name received is: " + file + "\n\n");

  console.log("shapeType: " + req.query.shapeType);
  console.log("x: " + req.query.x);
  console.log("y: " + req.query.y);
  console.log("rw: " + req.query.rw);
  console.log("h: " + req.query.h);
  console.log("units: " + req.query.units);

  let addShapeExecute = sharedLib.addRectangle(req.query.filename, req.query.shapeType, parseFloat(req.query.x), parseFloat(req.query.y), parseFloat(req.query.rw), parseFloat(req.query.h), req.query.units);

  res.send("Successfully Added");
});


/* Scale Shapes */
app.get('/scaleShape', function (req, res) {
  console.log("Scale Shape Data Received:");
  console.log("\n\n File Name : " + req.query.filename);
  console.log("shapeType: " + req.query.shapeType);
  console.log("scale Factor: " + req.query.scaleFactor);

  let scaleExecute = sharedLib.scaleShape(req.query.filename, req.query.shapeType, String(req.query.scaleFactor));

  res.send("Successfully Added");
});

/* Scale Shapes */
app.get('/editAttribute', function (req, res) {
  console.log("Edit Attribute Data Received:");
  console.log("\n\n title : " + req.query.title);
  console.log("description: " + req.query.description);
  console.log("filename: " + req.query.file);


  let editAttributeExecute = sharedLib.editAttribute(req.query.file, req.query.title, req.query.description);

  res.send("Successfully Added");
});



/************************************************* Assignment 4 ********************************************************/

/* Creating Tables */
let fileQuery = "CREATE TABLE IF NOT EXISTS FILE ( \
  svg_id INT AUTO_INCREMENT, \
  file_name VARCHAR(60) NOT NULL, \
  file_title VARCHAR(256), \
  file_description VARCHAR(256), \
  n_rect INT NOT NULL, \
  n_circ INT NOT NULL, \
  n_path INT NOT NULL, \
  n_group INT NOT NULL, \
  creation_time DATETIME NOT NULL, \
  file_size INT NOT NULL,  \
  PRIMARY KEY (svg_id))";


let changeQuery = "CREATE TABLE IF NOT EXISTS IMG_CHANGE( \
    change_id INT AUTO_INCREMENT, \
    change_type VARCHAR(256) NOT NULL, \
    change_summary VARCHAR(256) NOT NULL, \
    change_time DATETIME NOT NULL, \
    svg_id INT NOT NULL, \
    PRIMARY KEY (change_id), \
    FOREIGN KEY(svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE)";

let downloadQuery = "CREATE TABLE IF NOT EXISTS DOWNLOAD ( \
      download_id INT AUTO_INCREMENT,  \
      d_descr VARCHAR(256), \
      svg_id INT NOT NULL, \
      PRIMARY KEY (download_id), \
      FOREIGN KEY(svg_id) REFERENCES FILE(svg_id) ON DELETE CASCADE)";

/*Credentials to use for later*/
let login = {};
let connection;

/* Loging into Database */
app.get('/dbLogin', async function (req, res) {

  login = {
    host: 'dursley.socs.uoguelph.ca',
    user: req.query.user,
    password: req.query.pass,
    database: req.query.database_name
  };

  var flag;
  try {
    connection = await mysql.createConnection(login);

    await connection.execute(fileQuery);
    await connection.execute(changeQuery);
    await connection.execute(downloadQuery);

    flag = "Success";

  } catch (e) {
    console.log("Error when logging in: " + e);
    flag = "Fail";
  } finally {
    //Close the connection

    if (connection && connection.end) connection.end();
  }

  console.log("Database Log in = " + flag);
  res.send(flag);
});


function retrieveFileName(filename) {
  let dir = "./uploads/" + filename;
  var syn = fs.statSync(dir);
  var sizeB = syn["size"];
  let sizeK = sizeB / 1024;
  var fileSize = Math.round(sizeK);
  return fileSize;
}

/* Storing All Files in Database */
app.get('/DBStoreFiles', async function (req, res) {

  console.log("Login data " + JSON.stringify(login));

  var r = [];
  let files = fs.readdirSync('./uploads');
  var dir;

  try {
    connection = await mysql.createConnection(login);


    for (var i = 0; i < files.length; i++) {

      var fileSize = retrieveFileName(files[i])

      var svg = sharedLib.createValidSVGimage("./uploads/" + files[i], "svg.xsd");
      var json = sharedLib.SVGtoJSON(svg);

      var title = sharedLib.getTitle(files[i]);
      var titleStr = title.toString().replace(/\\"/g, '"').replace(/"/g, '\\"');

      var description = sharedLib.getDescription(files[i]);
      var descriptionStr = description.toString().replace(/\\"/g, '"').replace(/"/g, '\\"');

      let jsonObj = JSON.parse(json);
      jsonObj["filename"] = files[i];
      jsonObj["fileSize"] = fileSize;
      jsonObj["title"] = titleStr;
      jsonObj["description"] = descriptionStr;

      console.log("This is All the INSERT Data for store file: \n");
      console.log(jsonObj);


      let check = "SELECT file_name FROM FILE WHERE file_name LIKE \"%" + files[i] + "%\" LIMIT 1;";
      //let check = "SELECT * FROM FILE WHERE file_name LIKE  '%hello.jpg%' LIMIT 1";

      const [row] = await connection.execute(check);

      var numOfRows = row.length;
      console.log("Check = " + numOfRows);
      if (numOfRows !== 0) {
        console.log("File exists in DB. Update File");
        let updateQuery = "UPDATE FILE SET file_title= \"" + jsonObj["title"] + "\",file_description= \"" + jsonObj["description"] + "\",n_rect =" + jsonObj["numRect"] + ",n_circ= " + jsonObj["numCirc"] + ", n_path= " + jsonObj["numPaths"] + ", n_group= " + jsonObj["numGroups"] + ",file_size=\"" + jsonObj["fileSize"] + "\" WHERE file_name = \"" + files[i] + "\";";
        console.log("Update Query = " + updateQuery);
        await connection.execute(updateQuery);

      }
      else {
        console.log("File does NOT exist in DB");
        let insertFileQuery = "INSERT INTO FILE(file_name, file_title, file_description, n_rect, n_circ, n_path, n_group, creation_time ,file_size) VALUES (\"" + jsonObj["filename"] + "\",\"" + jsonObj["title"] + " \",\" " + jsonObj["description"] + " \", " + jsonObj["numRect"] + " , " + jsonObj["numCirc"] + " , " + jsonObj["numPaths"] + " , " + jsonObj["numGroups"] + " ,NOW(), " + jsonObj["fileSize"] + ");";
        await connection.execute(insertFileQuery);
      }
    }
    res.send("Success");
  } catch (e) {
    console.log("Error when inserting files: " + e);
    res.status(400).send(e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});


/* Clear All Data from Alll Tables */
app.get('/DBClearData', async function (req, res) {
  try {
    connection = await mysql.createConnection(login);
    await connection.execute("DELETE FROM DOWNLOAD;");
    await connection.execute("DELETE FROM IMG_CHANGE;");
    await connection.execute("DELETE FROM FILE;");

    res.send("Success");
  } catch (e) {
    console.log("Error when inserting files: " + e);
    res.status(400).send(e);
  } finally {
    if (connection && connection.end) connection.end();
  }

});


/* Track Downloads */
app.get('/DBStatus', async function (req, res) {

  /* To store the # of records in each table */
  var counter = [];

  try {
    connection = await mysql.createConnection(login);

    console.log("Successfully Connected to DB [DBStatus]");

    let [rowsF, fields1] = await connection.execute('SELECT (SELECT COUNT(*) FROM FILE) AS fileCount;');
    let [rowsC, fields2] = await connection.execute('SELECT (SELECT COUNT(*) FROM IMG_CHANGE) AS changeCount;');
    let [rowsD, fields3] = await connection.execute('SELECT (SELECT COUNT(*) FROM DOWNLOAD) AS downloadCount;');


    for (let row of rowsF) {
      console.log("Files = " + row.fileCount);
      counter[0] = row.fileCount;
    }
    for (let row of rowsC) {
      console.log("Changes = " + row.changeCount);
      counter[1] = row.changeCount;
    }
    for (let row of rowsD) {
      console.log("Downloads = " + row.downloadCount);
      counter[2] = row.downloadCount;
    }
  } catch (err) {
    console.log("Error Retrieving: " + err);
  } finally {
    if (connection && connection.end) connection.end();
  }
  res.send(counter);
});



/* Display All Files */
app.get('/DBQueryFiles', async function (req, res) {

  var files = [];
  var i = 0;
  var sortSelected = req.query.sort;
  var startDate = req.query.startDate;
  var endDate = req.query.endDate;


  /* Testing */
  console.log("Sort = " + sortSelected);
  console.log("Start Date = " + startDate);
  console.log("End Date = " + endDate);
  console.log(typeof startDate);


  let query;

  if (startDate === "false") {
    query = "SELECT * from FILE ORDER BY " + sortSelected + ";";
  } else {
    query = "SELECT * from FILE \
    WHERE creation_time BETWEEN'" + startDate + "' \
    AND '"+ endDate + "' ORDER BY " + sortSelected + ";";
  }
  console.log("Query = " + query);


  try {
    connection = await mysql.createConnection(login);

    console.log("Successfully logged IN");
    const [rows, fields] = await connection.execute(query);

    for (let row of rows) {
      files[i] = {
        "file_name": row.file_name,
        "file_title": row.file_title,
        "file_description": row.file_description,
        "n_rect": row.n_rect,
        "n_circ": row.n_circ,
        "n_path": row.n_path,
        "n_group": row.n_group,
        "creation_time": row.creation_time,
        "file_size": row.file_size
      };
      console.log(files[i]);
      i++;
    }
  } catch (e) {
    console.log("Error Executing Query: " + e);

  } finally {
    if (connection && connection.end) connection.end();
  }
  res.send(files);
});


/* Track Downloads */
app.get('/DBDownload', async function (req, res) {
  try {
    connection = await mysql.createConnection(login);

    //console.log("Desc = " +req.query.desc);

    let svg_id = "SELECT (SELECT svg_id FROM FILE WHERE file_name=\"" + req.query.filename + "\") AS svg;";
    let [rows, fields] = await connection.execute(svg_id);
    console.log("The file_name ID for download is: " + rows[0].svg);

    let download = "INSERT INTO DOWNLOAD(svg_id, d_descr) VALUES(" + rows[0].svg + ", \"User downloaded " + req.query.filename + "\");";

    await connection.execute(download);

    res.send("Success");
  } catch (e) {
    console.log("Error when inserting download: " + e);
    res.status(400).send(e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});



/* Track Creations */
app.get('/DBtrackCreation', async function (req, res) {
  try {
    console.log("In DB TRACT CREATION");

    let file = retrieveFileName(req.query.file);
    connection = await mysql.createConnection(login);

    let check = "SELECT file_name FROM FILE WHERE file_name LIKE \"%" + req.query.file + "%\" LIMIT 1;";
    const [row] = await connection.execute(check);
    var numOfRows = row.length
    console.log("check Creation = " + numOfRows);

    if (numOfRows !== 0) {
      console.log("File exists in DB. Update File and added Changes to CHANGE table");
      let svg_id = "SELECT (SELECT svg_id FROM FILE WHERE file_name=\"" + req.query.file + "\") AS svg;";
      let [rows, fields] = await connection.execute(svg_id);
      let creationQuery = "UPDATE FILE SET file_size=\"" + file + "\" WHERE svg_id = " + rows[0].svg + ";";
      console.log("Query = " + creationQuery);
      await connection.execute(creationQuery);

      /* Add changes to table CHANGE in DB since file already exists but maybe the title/description might be modified */
      let changeType = "change Title/Description";
      let changeSummary = "Description and Title modified. Title is now: " + req.query.title;

      let change = "INSERT INTO IMG_CHANGE(svg_id, change_type, change_summary, change_time) \
      VALUES(" + rows[0].svg + ",\"" + changeType + "\",\"" + changeSummary + "\", NOW());";
      console.log("Change Query for Title/Description = " + change);
      await connection.execute(change);

    }
    else {
      console.log("File does NOT exist in DB");
      let creationQuery = "INSERT INTO FILE (file_name, file_title, file_description, creation_time, file_size) VALUES(\"" + req.query.file + "\",\"" + req.query.title + "\",\"" + req.query.description + "\", NOW(), \"" + file + "\");";
      console.log("Query = " + creationQuery);
      await connection.execute(creationQuery);
    }
    res.send("Success");
  } catch (e) {
    console.log("Error when inserting new file created: " + e);
    res.status(400).send(e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});



/* Track Downloads */
app.get('/DBTrackChanges', async function (req, res) {

  var fileSize = retrieveFileName(req.query.filename);

  try {
    connection = await mysql.createConnection(login);

    let svg_id = "SELECT (SELECT svg_id FROM FILE WHERE file_name=\"" + req.query.filename + "\") AS svg;";
    let [rows, fields] = await connection.execute(svg_id);
    console.log("Summary = " + req.query.changeSummary);

    let change = "INSERT INTO IMG_CHANGE(svg_id, change_type, change_summary, change_time) \
    VALUES(" + rows[0].svg + ",\"" + req.query.changeType + "\",\"" + req.query.changeSummary + "\", NOW());";
    console.log("Change Query = " + change);
    await connection.execute(change);

    let fileSizeUpdate = "UPDATE FILE SET file_size=\"" + fileSize + "\" WHERE svg_id = " + rows[0].svg + ";";
    console.log("Update file Size Query = " + fileSizeUpdate);

    await connection.execute(fileSizeUpdate);


    res.send("Success");
  } catch (e) {
    console.log("Error when inserting changes: " + e);
    res.status(400).send(e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});





/* Query 3 */
app.get('/DBQuery3', async function (req, res) {

  var files = [];
  var i = 0;
  var sortSelected = req.query.sort;
  var startDate = req.query.startDate;
  var endDate = req.query.endDate;


  /* Testing */
  console.log("Sort = " + sortSelected);
  console.log("Start Date = " + startDate);
  console.log("End Date = " + endDate);
  console.log(typeof startDate);

  var type;

  if (sortSelected === "mRecent")
    type = "DESC";
  else
    type = "";

  let query = " \
  select a.svg_id, a.file_name, a.file_size, MAX(b.change_time) as mRecent, COUNT(b.svg_id) as count \
  from FILE a \
  INNER JOIN IMG_CHANGE b \
  WHERE(SELECT max(b.change_time) From IMG_CHANGE \
  WHERE(a.svg_id = b.svg_id) AND(change_time BETWEEN \"" + startDate + "\"AND\"" + endDate + "\")) \
  GROUP BY b.svg_id \
  ORDER BY " + sortSelected + " " + type + ";";

  console.log("Query = " + query);


  try {
    connection = await mysql.createConnection(login);

    console.log("Successfully logged IN");
    const [rows, fields] = await connection.execute(query);

    for (let row of rows) {
      files[i] = {
        "file_name": row.file_name,
        "mRecent": row.mRecent,
        "count": row.count,
        "file_size": row.file_size
      };
      console.log(files[i]);
      i++;
    }
  } catch (e) {
    console.log("Error Executing Query: " + e);

  } finally {
    if (connection && connection.end) connection.end();
  }
  res.send(files);
});



/* Query 4 */
app.get('/DBQuery4', async function (req, res) {

  var files = [];
  var i = 0;
  var sortSelected = req.query.sort;
  var startRange = req.query.startRange;
  var endRange = req.query.endRange;
  var shapeType = req.query.shapeType;


  /* Testing */
  console.log("Sort = " + sortSelected);
  console.log("shape Type = " + shapeType);
  console.log("Start Range = " + startRange);
  console.log("End Range = " + endRange);


  let query = " \
  SELECT file_name, file_size," + shapeType + " AS shapeCount FROM FILE WHERE " + shapeType + " BETWEEN " + startRange + " and " + endRange + " ORDER BY " + sortSelected + ";";


  console.log("Query = " + query);


  try {
    connection = await mysql.createConnection(login);

    console.log("Successfully logged IN");
    const [rows, fields] = await connection.execute(query);

    for (let row of rows) {
      files[i] = {
        "file_name": row.file_name,
        "shapeCount": row.shapeCount,
        "file_size": row.file_size
      };
      console.log(files[i]);
      i++;
    }
  } catch (e) {
    console.log("Error Executing Query: " + e);

  } finally {
    if (connection && connection.end) connection.end();
  }
  res.send(files);
});




/* Query 5 */
app.get('/DBQuery5', async function (req, res) {

  let files = [];
  var i = 0;
  var sortSelected = req.query.sort;
  var nMost = req.query.nMost;

  /* Testing */
  console.log("Sort = " + sortSelected);
  console.log("N = " + nMost);

  let query1 = "SELECT b.file_name, b.file_size, a.d_descr, COUNT(a.svg_id) as count, a.svg_id \
  FROM DOWNLOAD a \
  INNER JOIN FILE b \
  ON a.svg_id = b.svg_id \
  GROUP BY a.svg_id \
  ORDER BY count DESC \
  LIMIT " + nMost + ";";

  let query2 = "SELECT b.file_name, b.file_size, a.d_descr, COUNT(a.svg_id) as count, a.svg_id \
  FROM DOWNLOAD a \
  INNER JOIN FILE b \
  ON a.svg_id = b.svg_id \
  GROUP BY a.svg_id \
  ORDER BY file_name;";

  console.log("Query 1 = " + query1);
  console.log("Query 2 = " + query2);


  let description = "";
  let file_size = 0;
  let numDownloads = 0;
  let ids = "";
  let svg_id_list = "";
  let name = "";

  try {
    if (sortSelected == "file_name") {

      connection = await mysql.createConnection(login);

      console.log("Sory by file_name selected");
      console.log("Successfully logged IN");
      const [myQuery1] = await connection.execute(query1);
      const [myQuery2] = await connection.execute(query2);

      for (let row in myQuery1)
        svg_id_list += myQuery1[row].svg_id + " "


      for (let row of myQuery2) {

        name = row.file_name;
        description = row.d_descr;
        numDownloads = row.count;
        file_size = row.file_size;
        ids = row.svg_id;

        var data = { file_name: name, d_descr: description, numDownloads: numDownloads, file_size: file_size }
        if (svg_id_list.includes(ids)) {
          files.push(data);
        }
      }
      res.send(files);
    }
    else {

      connection = await mysql.createConnection(login);

      console.log("Sort by Download COunt selected");
      const [rows] = await connection.execute(query1);

      for (let row of rows) {
        files[i] = {
          "file_name": row.file_name,
          "d_descr": row.d_descr,
          "numDownloads": row.count,
          "file_size": row.file_size
        };
        console.log(files[i]);
        i++;
      }
      res.send(files);
    }
  } catch (e) {
    console.log("Error Executing Query: " + e);

  } finally {
    if (connection && connection.end) connection.end();
  }
});


/* Query 6 */

/* Get file_name and change_type for dropdowns */
app.get('/DBfileNameDropdown', async function (req, res) {

  let nameArray = [];
  var k = 0;

  try {
    connection = await mysql.createConnection(login);
    console.log("Successfully Connected to DB [DBStatus]");

    let file_nameQuery = "SELECT file_name FROM FILE a INNER JOIN IMG_CHANGE b WHERE (a.svg_id = b.svg_id) GROUP BY file_name;";

    //console.log("file_nameQuery = " + file_nameQuery);

    // const [change_type] = await connection.execute(change_typeQuery);
    const [file_name] = await connection.execute(file_nameQuery);

    /*   console.log("Change Type =  ");
      for (let row of change_type) {
        typeArray[i] = row.change_type;
        console.log(typeArray[i]);
        i++;
      } */
    console.log("\nFile Names =  ");
    for (let row of file_name) {
      nameArray[k] = row.file_name;
      console.log(nameArray[k]);
      k++;
    }

    res.send(nameArray);
  } catch (e) {
    console.log("Error Executing Query: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});




app.get('/DBchangeTypeDropdown', async function (req, res) {

  let typeArray = [];
  var i = 0;
  let fileName = req.query.file_name;

  try {
    connection = await mysql.createConnection(login);
    console.log("Successfully Connected to DB [DBStatus]");

    let change_typeQuery = "SELECT change_type FROM IMG_CHANGE a \
      INNER JOIN FILE b WHERE(a.svg_id = b.svg_id) AND(b.file_name = \"" + fileName +"\") \
      GROUP BY change_type;";

    console.log("change_typeQuery = " + change_typeQuery);

    const [change_type] = await connection.execute(change_typeQuery);
    
    console.log("\nChange Type =  ");
    for (let row of change_type) {
      typeArray[i] = row.change_type;
      console.log(typeArray[i]);
      i++;
    }

    res.send(typeArray);
  } catch (e) {
    console.log("Error Executing Query: " + e);
  } finally {
    if (connection && connection.end) connection.end();
  }
});






/* Query 6 */
app.get('/DBQuery6', async function (req, res) {

  var files = [];
  var i = 0;
  var sortSelected = req.query.sort;
  var startDate = req.query.startDate;
  var endDate = req.query.endDate;
  var fileName = req.query.file_name;
  var changeType = req.query.changeType;

  /* Testing */
  console.log("Sort = " + sortSelected);
  console.log("FileName = " + fileName);
  console.log("Change Type = " + changeType);
  console.log("Start Date = " + startDate);
  console.log("End Date = " + endDate);


  let query = " \
 SELECT b.change_summary, b.change_time \
FROM FILE a \
INNER JOIN IMG_CHANGE b \
WHERE (a.svg_id = b.svg_id) AND (file_name = \"" + fileName + "\") \
AND (b.change_type = \"" + changeType + "\") AND (change_time BETWEEN \"" + startDate + "\" AND \"" + endDate + "\") \
ORDER BY b.change_time " + sortSelected + ";";

  console.log("Query = " + query);

  try {
    connection = await mysql.createConnection(login);

    console.log("Successfully logged IN");
    const [rows] = await connection.execute(query);

    for (let row of rows) {
      files[i] = {
        "change_summary": row.change_summary,
        "change_time": row.change_time
      };
      console.log(files[i]);
      i++;
    }
  } catch (e) {
    console.log("Error Executing Query: " + e);

  } finally {
    if (connection && connection.end) connection.end();
  }
  res.send(files);
});

