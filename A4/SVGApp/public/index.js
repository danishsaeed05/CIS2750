var stat = "Fail";

/* To add data of the download to the Database */
function DBDownloadFunc(filename) {
  var desc;

  if (stat === "Fail") {
    return;
  } else {

    $.ajax({
      type: 'get',
      url: '/DBDownload/',
      data: {
        filename: filename,
      },
      success: function (data) {
        console.log(data);
        alert("Successfully Stored Download Record");
      },
      fail: function (error) {
        alert("Error Storing Download Record");
        console.log(error);
      }
    });
  }
}

$('#DatabaseUI *').prop('disabled',true);

$(document).ready(function () {


  $.ajax({
    type: 'get',
    dataType: 'json',
    url: '/svg',

    success: function (data) {
      console.log("String data" + data);
      console.log("Data Length = " + data.length);
      if (data.length === 0) {
        $('#table').append("<tr><td>No Files</tr><tr>");
      }

      for (var i = 0; i < data.length; i++) {

        let json = JSON.parse(data[i]);
        if ((json["numRect"] === 0) && (json["numCirc"] === 0) && (json["numPaths"] === 0) && (json["numGroups"] === 0)) {
          console.log("File " + json["filename"] + " has no shapes (SKIPPED)");
        } else {
          console.log("This is the data" + data[i]);

          $('#table').append("<tr><td>" + "<a download='" + json["filename"] + "' href='" + json["filename"] + "' title='" + json["filename"] + "'>" + "<img id='logImage' src='" + json["filename"] + "'></a></td><td><a class=\"idDownload\" onclick=DBDownloadFunc(\"" + json["filename"] + "\") download='" + json["filename"] + "' " + "href='" + json["filename"] + "'>" + json["filename"] + "</a></td><td>" + json["fileSize"] + "</td><td>" + json["numRect"] + "</td><td>" + json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td></tr>");

        }
        $('#dropdown').append("<option value ='" + json["filename"] + "'>" + json["filename"] + "</option>");
        $('#dropdown2').append("<option value ='" + json["filename"] + "'>" + json["filename"] + "</option>");
        $('#dropdown3').append("<option value ='" + json["filename"] + "'>" + json["filename"] + "</option>");

        console.log(json);
      }

    },
    fail: function (error) {
      alert('Unable to add ' + json["filename"] + ' to View Panel');
      console.log(error);
    }
  });







  /* SVG View Panel */
  $('#dropdown').change(function () {
    let filename = $("#dropdown option:selected").text();
    $('#mytable').html("")

    console.log("dropdown 1: FileName: " + filename);
    if (filename === "None") {
      $('#viewPanel').css('display', 'none');
      $('#devEditAtt').css('display', 'none');

      return;
    }
    $('#viewPanel').css('display', 'block');

    $('#devEditAtt').css('display', 'block');

    $('#viewPanelImage').attr("src", filename);


    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/panel/' + filename,

      success: function (data2) {
        console.log("in success");
        console.log(data2);
        console.log("FileName is :" + filename);

        $('#mytable').append("<tr><th>Title</th><th>Description</th></tr>");
        $('#mytable').append("<tr><td>" + data2.title + "</td><td>" + data2.description + "</td></tr>");
        $('#mytable').append("<tr><th>Component</th><th>Summary</th><th>Other attributes</th></tr>");

        var title = $('#id1');
        title.innerHTML = "hello";
        $('#editdesc').value = data2.description;


        let data = JSON.parse(data2.json);
        console.log("\n\nJSON PARSED: \n " + data);


        let length = data.length;
        console.log("Length of # of objects of JSON : " + length + "\n\n");

        $('#summary').html("");
        for (var i = 0; i < length; i++) {
          console.log("Loop = " + i);
          console.log("length of data " + i + " is : " + data[i].length);

          if (i == 0) {
            if ((data[i].length) !== 0) {
              for (var f = 0; f < data[i].length; f++) {
                console.log("Path " + (f + 1) + " " + data[i][f]["d"]);
                $("#mytable").append("<tr><td>Path " + (f + 1) + "</td><td>path data = " + data[i][f]["d"] + "</td><td>" + data[i][f]["numAttr"] + "</td></tr>");
              }
            }
            else {
              console.log("Skipped 1");
              $("#summary").html("");
            }
          }
          else if (i == 1) {
            if (data[i].length !== 0) {
              for (var f = 0; f < data[i].length; f++) {
                console.log("Group " + (f + 1) + " " + data[i][f]["children"]);
                $("#mytable").append("<tr><td>Group " + (f + 1) + "</td><td>" + data[i][f]["children"] + " child elements</td><td>" + data[i][f]["numAttr"] + "</td></tr>");
              }
            }
            else {
              console.log("Skipped 2");
            }
          }
          else if (i == 2) {
            if (data[i].length !== 0) {
              for (var f = 0; f < data[i].length; f++) {
                console.log("Rectangle " + (f + 1) + " Upper left corner: x = " + data[i][f]["x"] + "y = " + data[i][f]["y"] + "Width: " + data[i][f]["w"] + ", Height" + data[i][f]["h"]);
                $("#mytable").append("<tr><td>Rectangle " + (f + 1) + "</td><td>Upper left corner: x = " + data[i][f]["x"] + "cm, y = " + data[i][f]["y"] + "cm Width: " + data[i][f]["w"] + "cm, Height: " + data[i][f]["h"] + "cm</td><td>" + data[i][f]["numAttr"] + "</td></tr>");
              }
            }
            else {
              console.log("Skipped 3");
            }
          }
          else if (i == 3) {
            if (data[i].length !== 0) {
              for (var f = 0; f < data[i].length; f++) {
                console.log("Circle " + (f + 1) + " Cenre: x = " + data[i][f]["cx"] + "cm y = " + data[i][f]["cy"] + "cm radius: " + data[i][f]["r"] + "cm");
                $("#mytable").append("<tr><td>Circle " + (f + 1) + "</td><td>Centre: x = " + data[i][f]["cx"] + "cm y = " + data[i][f]["cy"] + "cm radius: " + data[i][f]["r"] + "cm <td>" + data[i][f]["numAttr"] + "</td></tr>");

              }
            }
            else {
              console.log("Skipped 3");
            }
          }
        }
      },
      fail: function (error) {
        alert("Unable to get data for " + filename);
        console.log(error);
      }
    });
  });







  $('#editAttribute').submit(function (data) {
    data.preventDefault();

    let title = $("#edittitle").val();
    let description = $("#editdesc").val();
    let file = $("#dropdown option:selected").text();


    $.ajax({
      type: 'get',
      dataType: 'text',
      url: '/editAttribute/',
      data: {
        file: file,
        title: title,
        description: description
      },
      success: function (data) {
        console.log(data);
        console.log("in success");
        alert("Successfully Added");
        /*Reloads the page to display latest data */
        location.reload();
      },
      fail: function (error) {

        alert("Could not add component to " + filename);
        console.log(error);
      }
    });

  });




  /* Create New SVG Image */
  $('#createSVGimage').submit(function (data) {
    //Prevent it from submitting the form
    data.preventDefault();
    let file = $("#fileName").val() + ".svg";
    console.log("User entered SVG Image Name is: " + file);

    let title = $("#title").val();
    let description = $("#description").val();


    //console.log("\n\n" + svg + "\n\n");

    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/createSVG/',
      data: {
        file: file,
        title: title,
        description: description
      },
      success: function (json) {

        if ((json["numRect"] === 0) && (json["numCirc"] === 0) && (json["numPaths"] === 0) && (json["numGroups"] === 0)) {
          console.log("File " + json["filename"] + " has no shapes. Will NOT add to Log Panel (SKIPPED)");
        } else {
          $('#table').append("<tr><td><img id='logImage' src='" + file + "'></td><td><a href='" + file + "'>" + file + "</a></td><td>" + json["filesize"] + "</td><td>" + json["numRect"] + "</td><td>" + json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td></tr>");
        }
        $('#dropdown').append("<option value ='" + file + "'>" + file + "</option>");
        $('#dropdown2').append("<option value ='" + file + "'>" + file + "</option>");
        $('#dropdown3').append("<option value ='" + file + "'>" + file + "</option>");

        alert("Successfully added file: " + file);

        /* Before reloading page. Add the necessary data needed to DB */
        if (stat !== "Fail") {
          $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/DBtrackCreation/',
            data: {
              file: file,
              title: title,
              description: description
            },
            success: function (data) {
              console.log(data);
            },
            fail: function (error) {
              console.log(error);
              alert("Unsuccessfull");
            }
          });
        }
          /* DB Data send request ends here */
        location.reload();
        console.log(data);
      },
      fail: function (error) {
        alert("Unable to create file: " + file);
        console.log(error);
      }
    });

  });


  /* add Shape selection Modification */
  $(document).ready(function () {
    $('#selectShape').change(
      function () {
        var lblheight = document.getElementById("lblheight");
        var height = document.getElementById("height");
        var lblrwID = document.getElementById("lblrwID");
        var rwID = document.getElementById("rwID");

        var method = $('option:selected', this).text();
        if (method == "Rectangle") {
          height.required = true;
          lblheight.style.display = "block";
          height.style.display = "block";
          lblrwID.innerHTML = "Width";
          rwID.placeholder = "Enter Width: ";
        } else {
          lblheight.style.display = "none";
          height.style.display = "none";
          lblrwID.innerHTML = "Radius";
          rwID.placeholder = "Enter Radius: ";
          height.required = false;
        }
      });
  });


  /* Add shape */
  $('#addShape').submit(function (e) {
    e.preventDefault();

    let filename = $("#dropdown2 option:selected").text();
    console.log(filename);

    let shapeType = $("#selectShape option:selected").text();
    console.log(shapeType);

    let x = $("#xID").val();
    console.log("\n\nx = " + x);

    let y = $("#yID").val();
    console.log("y = " + y);

    let rw = $("#rwID").val();
    console.log("r = " + rw);

    let h = $("#height").val();
    console.log("r = " + h);

    let units = $("#unitsID").val();
    console.log("units = " + units);

    /*Adding changes to Database */
    if (stat !== "Fail") {
      var changeType = "add " + shapeType;
      var changeSummary = "add " + shapeType + " at " + x + "," + y + " with radius/width of " + rw;
      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/DBTrackChanges/',
        data: {
          filename: filename,
          changeType: changeType,
          changeSummary: changeSummary
        },
        success: function (data) {
          console.log(data);
          console.log("in success");
          alert("Successfully added changes to DB");
        },
        fail: function (error) {
          alert("Unsuccessfull");
          console.log(error);
        }
      });
    }



    if (filename !== "None") {
      $.ajax({
        type: 'get',
        dataType: 'text',
        url: '/addShape/',
        data: {
          filename: filename,
          shapeType: shapeType,
          x: x,
          y: y,
          rw: rw,
          h: h,
          units: units
        },
        success: function (data) {
          console.log(data);
          console.log("in success");
          if (stat === "Success")
            alert("Successfully Added to File and Database");
          else 
          alert("Successfully Added");
          /*Reloads the page to display latest data */
          location.reload();
        },
        fail: function (error) {

          alert("Could not add component to " + filename);
          console.log(error);
        }
      });
    }
    else {
      alert("Please select a file");
    }
  });



  $('#scaleShape').submit(function (e) {
    e.preventDefault();
    let filename = $("#dropdown3 option:selected").text();
    console.log(filename);

    let shapeType = $("#selectShapeToScale option:selected").text();
    console.log(shapeType);

    let scaleFactor = $("#scale").val();
    console.log("scale Factor is = " + scaleFactor);


    /*Adding changes to Database */
    if (stat !== "Fail") {
      var changeType = "change attribute";
      var changeSummary = "change " + shapeType + " scale factor by " + scaleFactor + " units";
      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/DBTrackChanges/',
        data: {
          filename: filename,
          changeType: changeType,
          changeSummary: changeSummary
        },
        success: function (data) {
          console.log(data);
          console.log("in success");
        },
        fail: function (error) {
          alert("Unsuccessfull to add to DB");
          console.log(error);
        }
      });
    }


    if (filename !== "None") {
      $.ajax({
        type: 'get',
        dataType: 'text',
        url: '/scaleShape/',
        data: {
          filename: filename,
          shapeType: shapeType,
          scaleFactor: scaleFactor,
        },
        success: function (data) {
          console.log(data);
          console.log("in success");
          

          if (stat === "Success")
          alert("Successfully Scaled Shape and saved to DB");
        else 
          alert("Successfully Scaled Shape");
          /*Reloads the page to display latest data */
          location.reload();
        },
        fail: function (error) {

          alert("Could not add component to " + filename);
          console.log(error);
        }
      });
    }
    else {
      alert("Please select a file");
    }
  });


  /************************************************ Assignment 4 *************************************************************/


  $('#login').submit(function (e) {
    e.preventDefault();

    let user = $("#user").val();
    let pass = $("#pass").val();
    let database_name = $("#database_name").val();

    console.log(user);
    console.log(pass);
    console.log(database_name);

    var val = $('#btn_login').html();
      console.log("Button Value = "+val);

      if(val === "Logout")
      {
        location.reload();
      }

else {
    $.ajax({
      type: 'get',            //Request type
      url: '/dbLogin/',   //The server endpoint we are connecting to
      data: {
        user: user,
        pass: pass,
        database_name: database_name,
      },
      success: function (data) {
        console.log("Login: " + data);

        if (data === "Success") {
          alert("Successfully Signed In");
          stat = "Success";

          $('#btn_login').html("Logout");

          var val = $('#btn_login').html();
          console.log("Button Value = "+val);

          $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/DBStatus/',
            success: function (data) {
              console.log(data);
              alert("Database has " + data[0] + " files, " + data[1] + " changes and " + data[2] + " downloads.");
              $('#status').html("Database has " + data[0] + " files, " + data[1] + " changes and " + data[2] + " downloads.");
              /* Scroll to table */
              $('html,body').animate({
                scrollTop: $("#querries").offset().top},
                'slow');
                /* Enable all elements */
                $('#DatabaseUI *').prop('disabled',false);

            },
            fail: function (error) {
              console.log(error);
              /* Disbale all elements */
              $('#DatabaseUI *').prop('disabled',true);
            }
          });

        }
        else {
          stat = "Fail";
          alert("Wrong Credentials. Please try again");
          /* Disbale all elements */
          $('#DatabaseUI *').prop('disabled',true);
        }

      },
      fail: function (e) {
        console.log("Error is: " + e);
        stat = "Fail";
        alert("Wrong Credentials. Please try again");
        $('#DatabaseUI *').prop('disabled',true);

      }
    });

  }
    $('#DBStoreFiles').click(function (e) {
      e.preventDefault();


      $.ajax({
        type: 'get',
        url: '/DBStoreFiles/',

        success: function (data) {
          console.log(data);
          alert("Successfully Stored All Files");
        },
        fail: function (error) {
          alert("Error Storing Files");
          console.log(error);
        }
      });

    });


    $("#DBClearData").click(function (e) {
      e.preventDefault();


      $.ajax({
        type: 'get',
        url: '/DBClearData/',

        success: function (data) {
          console.log(data);
          alert("Successfully Cleared All Data");
        },
        fail: function (error) {
          alert("Error Clearing Data");
          console.log(error);
        }
      });

    });



    $("#reload").click(function (e) {
      e.preventDefault();

      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/DBStatus/',
        success: function (data) {
          console.log(data);
          $('#status').html("Database has " + data[0] + " files, " + data[1] + " changes and " + data[2] + " downloads.");
        },
        fail: function (error) {
          console.log(error);
        }
      });
    });


    function sortType(select) {
      var sort;
      if (select === "Name")
        sort = "file_name";
      else if (select === "Size")
        sort = "file_size";
      else if (select === "Date")
        sort = "change_time";
      else if (select === "Most Recent Modified")
        sort = "mRecent";
      else if (select === "Path")
        sort = "n_path";
      else if (select === "Group")
        sort = "n_group";
      else if (select === "Circle")
        sort = "n_circ";
      else if (select === "Rectangle")
        sort = "n_rect";
      else if (select === "Shape Count") 
        sort = "shapeCount";
      else if (select === "Download Count") 
        sort = "count";
      else if (select === "Recent Changes First") 
        sort = "DESC";
      else if (select === "Recent Changes Last") 
        sort = "ASC";
      return sort;
    }


    /* Need some functional variables -->Not global since its within a function */
    let option = "None";

    $('#DBSearchOptions').change(function () {
      $('#DBtable tr').empty();

      option = $("#DBSearchOptions option:selected").text();

      console.log("Query = " + option);

      /* First Query */
      if (option === "Q1: Files") {
        $('#dateRange').css('display', 'none');
        $('#shapeRange').css('display', 'none');
        $('#divShapeType').css('display', 'none');
        $('#NdownloadDiv').css('display', 'none');
        $('#divFile_name').css('display', 'none');

        $('#divShapeType').css('display', 'none');
        $('#divChange_type').css('display', 'none');

        $('#DBdropdown').empty();
        $('#DBdropdown').append("<option value ='Name' selected>Name</option><option value ='Size'>Size</option>");

      }
      else if (option === "Q2: Files Date") {
        $('#shapeRange').css('display', 'none');
        $('#dateRange').css('display', 'block');
        $('#divShapeType').css('display', 'none');
        $('#NdownloadDiv').css('display', 'none');
        $('#divFile_name').css('display', 'none');

        $('#divShapeType').css('display', 'none');
        $('#divChange_type').css('display', 'none');

        $('#DBdropdown').empty();
        $('#DBdropdown').append("<option value ='Name' selected>Name</option><option value ='Size'>Size</option><option value ='Date'>Date</option>");
      }
      else if (option === "Q3: Files Modified") {
        $('#dateRange').css('display', 'block');
        $('#shapeRange').css('display', 'none');
        $('#divShapeType').css('display', 'none');
        $('#NdownloadDiv').css('display', 'none');
        $('#divFile_name').css('display', 'none');

        $('#divShapeType').css('display', 'none');
        $('#divChange_type').css('display', 'none');

        $('#DBdropdown').empty();
        $('#DBdropdown').append("<option value ='Name' selected>Name</option><option value ='Size'>Size</option><option value ='Most Recent Modified'>Most Recent Modified</option>");
      }
      else if (option === "Q4: Files Shapes Range"){
        $('#dateRange').css('display', 'none');
        $('#shapeRange').css('display', 'block');
        $('#divShapeType').css('display', 'block');
        $('#NdownloadDiv').css('display', 'none');
        $('#divFile_name').css('display', 'none');

        $('#divShapeType').css('display', 'none');
        $('#divChange_type').css('display', 'none');

        $('#DBdropdown').empty();
        $('#DBdropdown').append("<option value ='Name' selected>Name</option><option value ='Size'>Size</option><option value ='Shape Count'>Shape Count</option>");
      } 
      else if (option === "None") {
        $('#dateRange').css('display', 'none');
        $('#shapeRange').css('display', 'none');
        $('#divShapeType').css('display', 'none');
        $('#NdownloadDiv').css('display', 'none');
        $('#divFile_name').css('display', 'none');

        $('#divShapeType').css('display', 'none');
        $('#divChange_type').css('display', 'none');

      }
      else if (option === "Q5: Frequent Downloads"){
        $('#dateRange').css('display', 'none');
        $('#shapeRange').css('display', 'none');
        $('#divShapeType').css('display', 'none');
        $('#NdownloadDiv').css('display', 'block');
        $('#divFile_name').css('display', 'none');

        $('#divShapeType').css('display', 'none');
        $('#divChange_type').css('display', 'none');


        $('#DBdropdown').empty();
        $('#DBdropdown').append("<option value ='Name' selected>Name</option><option value ='Download Count'>Download Count</option>");
      }
      else if (option === "Q6: Specific Changes"){
        $('#dateRange').css('display', 'none');
        $('#shapeRange').css('display', 'none');
        $('#divShapeType').css('display', 'none');
        $('#NdownloadDiv').css('display', 'none');

        $('#divFile_name').css('display', 'block');
        $('#divChange_type').css('display', 'block');
        $('#dateRange').css('display', 'block');

        $('#DBdropdown').empty();
        $('#DBdropdown').append("<option value ='Recent Changes First'>Recent Changes First</option><option value ='Recent Changes Last'>Recent Changes Last</option>");
        
      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/DBfileNameDropdown/',
        success: function (data) {
          console.log(data);

          $('#file_name').empty();
          $('#change_type').empty();
          $('#file_name').append("<option selected>None</option>");
          for (var i=0; i<data.length;i++)
          $('#file_name').append("<option >"+data[i]+"</option>");

        },
        fail: function (error) {
          console.log(error);
        }
      });

    }
    });

    /* If filename for Query 6 changes, populate change_type accordingly */
    $('#file_name').change(function () {

      fileSelected = $("#file_name option:selected").text();
      if(fileSelected !== "None")
      {
      $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/DBchangeTypeDropdown/',
        data: {
          file_name: fileSelected
        },
        success: function (data) {
          console.log(data);

          $('#change_type').empty();
          for (var i=0; i<data.length;i++)
          $('#change_type').append("<option >"+data[i]+"</option>");

        },
        fail: function (error) {
          console.log(error);
        }
      });
    }
    else
    {
    $('#change_type').empty();
    alert("Please select a File Name from the dropdown List");
    }
    });



    $("#DBExecute").click(function (e) {
      e.preventDefault();
      $('#DBtable tr').empty();

      let select, sort;
      select = $("#DBdropdown option:selected").text();
      sort = sortType(select);

      console.log("Sort Type = " + sort);
     
      if (option === "None") {
        alert("Please choose the Query Search Type");
      }
      else if (option === "Q1: Files") {
        let startDate = false;
        let endDate = false;
        let checkDate = "n";
        Q1AndQ2(checkDate, sort, startDate, endDate)
      }
      else if (option === "Q2: Files Date"){
        let startDate = $("#startDate").val();
        let endDate = $("#endDate").val();
        let checkDate = "y";
        Q1AndQ2(checkDate, sort, startDate, endDate)
      }
      else if (option === "Q3: Files Modified"){
        let startDate = $("#startDate").val();
        let endDate = $("#endDate").val();
        Q3(startDate, endDate, sort);
      }
      else if (option === "Q4: Files Shapes Range"){
        let startRange = $("#startRange").val();
        let endRange = $("#endRange").val();
        let shapeValueType = $("#shapeType option:selected").text();
        let shapeType = sortType(shapeValueType);
        Q4(startRange, endRange, shapeType, sort, shapeValueType);
      }
      else if (option === "Q5: Frequent Downloads"){
        let numDownload = $("#NDownload").val();
        Q5(numDownload, sort);
      }
      else if (option === "Q6: Specific Changes"){
        let file_name = $("#file_name option:selected").text();
        let change_type = $("#change_type option:selected").text();

        let startDate = $("#startDate").val();
        let endDate = $("#endDate").val();

        Q6(sort,file_name, change_type, startDate, endDate);
      }
    });



/* Query 6 */
function Q6(sort, fileName, changeType, startDate, endDate)
{
  if ((startDate !== "") || (endDate !== "")) {
    if ((change_type !== "") || (file_name.val !== "None"))
      {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/DBQuery6/',
      data: {
        sort: sort,
        file_name: fileName,
        changeType: changeType,
        startDate: startDate,
        endDate: endDate
      },
      success: function (data) {
        console.log(data);

        $('#DBtable').empty();
        $("#DBtable").append("<tr> \
          <th>File Name (" + fileName + ")</th> \
          <th>Change Summary</th>\
          <th>Change Date</th>\
        </tr>");

        for (var i = 0; i < data.length; i++) {

          $("#DBtable").append("<tbody><tr> \
            <td></td> \
            <td>" + data[i].change_summary + "</td> \
            <td>" + data[i].change_time + "</td> \
            </tr></tbody>");
        }
        var len = data.length
        console.log("Length = " + len);
        if((len) == 0)
        {
          $("#DBtable").append("tr><td>Nothing to Display<td></tr>");
        }
      },
      fail: function (error) {
        console.log(error);
      }
    });
  } else 
    alert("Please fill in all fields");
}
else 
    alert("Please fill in all fields");
}


    

/* Query 5 */
function Q5(nMost, sort)
{
  if (nMost !== "") {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/DBQuery5/',
      data: {
        sort: sort,
        nMost: nMost,
      },
      success: function (data) {
        console.log(data);

        $('#DBtable').empty();
        $("#DBtable").append("<tr> \
          <th>File Name</th> \
          <th>Description</th>\
          <th># of Downloads</th>\
          <th>File Size</th>\
        </tr>");

        for (var i = 0; i < data.length; i++) {

          $("#DBtable").append("<tbody><tr> \
            <td>" + data[i].file_name + "</td> \
            <td>" + data[i].d_descr + "</td> \
            <td>" + data[i].numDownloads + "</td> \
            <td>" + data[i].file_size + "</td> \
            </tr></tbody>");
        }
      },
      fail: function (error) {
        console.log(error);
      }
    });
  } else {
    alert("Please enter Number of frequent N downloads you want to display");
  }
}




/* Query 4 */
function Q4(startRange, endRange, shapeType, sort, shapeValueType)
{
  if ((startRange !== "") || (endRange !== "")) {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/DBQuery4/',
      data: {
        sort: sort,
        startRange: startRange,
        endRange: endRange,
        shapeType: shapeType
      },
      success: function (data) {
        console.log(data);

        $('#DBtable').empty();
        $("#DBtable").append("<tr> \
          <th>File Name</th> \
          <th>Number of " + shapeValueType + "s</th>\
          <th>File Size (KB)</th>\
        </tr>");

        for (var i = 0; i < data.length; i++) {

          $("#DBtable").append("<tbody><tr> \
            <td>" + data[i].file_name + "</td> \
            <td>" + data[i].shapeCount + "</td> \
            <td>" + data[i].file_size + "</td> \
            </tr></tbody>");
        }
      },
      fail: function (error) {
        console.log(error);
      }
    });
  } else {
    alert("Please enter the Shape Range Values");
  }
}






/* Query 3 */
    function Q3(startDate, endDate, sort)
    {
      if ((startDate !== "") || (endDate !== "")) {
        $.ajax({
          type: 'get',
          dataType: 'json',
          url: '/DBQuery3/',
          data: {
            sort: sort,
            startDate: startDate,
            endDate: endDate,
          },
          success: function (data) {
            console.log(data);

            $('#DBtable').empty();
            $("#DBtable").append("<tr> \
              <th>File Name</th> \
              <th>Number of Changes</th>\
              <th>Most Recent Modified</th>\
              <th>File Size (KB)</th>\
            </tr>");

            for (var i = 0; i < data.length; i++) {

              $("#DBtable").append("<tbody><tr> \
                <td>" + data[i].file_name + "</td> \
                <td>" + data[i].count + "</td> \
                <td>" + data[i].mRecent + "</td> \
                <td>" + data[i].file_size + "</td> \
                </tr></tbody>");
            }
          },
          fail: function (error) {
            console.log(error);
          }
        });
      } else {
        alert("Please enter Date");
      }
    }

  

/* Query 1 and 2 */
    function Q1AndQ2(checkDate, sort, startDate, endDate)
    {
      if ((checkDate !== "y") && (startDate !== "") || (endDate !== "")) {
        $.ajax({
          type: 'get',
          dataType: 'json',
          url: '/DBQueryFiles/',
          data: {
            sort: sort,
            startDate: startDate,
            endDate: endDate,
          },
          success: function (data) {
            console.log(data);

            $('#DBtable').empty();
            $("#DBtable").append("<tr> \
              <th>File Name</th> \
              <th>File Title</th>\
              <th>File Description</th>\
              <th>Rectangle(s)</th>\
              <th>Circles(s)</th>\
              <th>Paths(s)</th>\
              <th>Groups(s)</th>\
              <th>Creation Time</th>\
              <th>File Size (KB)</th>\
            </tr>");

            for (var i = 0; i < data.length; i++) {

              $("#DBtable").append("<tbody><tr> \
                <td>" + data[i].file_name + "</td> \
                <td>" + data[i].file_title + "</td> \
                <td>" + data[i].file_description + "</td> \
                <td>" + data[i].n_rect + "</td> \
                <td>" + data[i].n_circ + "</td> \
                <td>" + data[i].n_path + "</td> \
                <td>" + data[i].n_group + "</td> \
                <td>" + data[i].creation_time + "</td> \
                <td>" + data[i].file_size + "</td> \
                </tr></tbody>");
            }
          },
          fail: function (error) {
            console.log(error);
          }
        });
      } else {
        alert("Please enter Date");
      }
    }

















    /* Login End */
  });






  /* Document End */
});