$(document).ready(function() {


  $.ajax({
    type: 'get',
    dataType: 'json',
    url: '/svg',

    success: function (data) {
      console.log("String data" +data);
      console.log("Data Length = " +data.length);
      if (data.length === 0)
      {
        $('#table').append("<tr><td>No Files</tr><tr>");
      }

      for(var i = 0; i < data.length; i++)
      {

        let json = JSON.parse(data[i]);
        if ((json["numRect"] === 0) && (json["numCirc"]=== 0) && (json["numPaths"]=== 0) && (json["numGroups"]=== 0))
        {
          console.log("File " + json["filename"] + " has no shapes (SKIPPED)");
        }else{
          console.log("This is the data" +data[i]);

          $('#table').append("<tr><td>"+"<a download='" +json["filename"]+"' href='"+json["filename"]+"' title='"+json["filename"]+"'>"+"<img id='logImage' src='" + json["filename"] + "'></a></td><td><a href='" + json["filename"] + "'>" + json["filename"] +"</a></td><td>" + json["fileSize"] + "</td><td>" + json["numRect"] + "</td><td>"+ json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td></tr>");

        }
        $('#dropdown').append("<option value ='" + json["filename"] + "'>" + json["filename"] + "</option>");
        $('#dropdown2').append("<option value ='" + json["filename"] + "'>" + json["filename"] + "</option>");
        $('#dropdown3').append("<option value ='" + json["filename"] + "'>" + json["filename"] + "</option>");

        /*Get file size */

        // const fsize = json["filename"].item(i).size;
        //        const file = Math.round((fsize / 1024));
        console.log(json);
      }
    },
    fail: function(error) {
      alert('Unable to add ' + json["filename"] + ' to View Panel');
      console.log(error);
    }
  });





  /* SVG View Panel */
  $('#dropdown').change(function(){
    let filename = $("#dropdown option:selected").text();
    $('#mytable').html("")

    console.log("dropdown 1: FileName: " + filename);
    if(filename === "None")
    {
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
        console.log("FileName is :" +filename);

        $('#mytable').append("<tr><th>Title</th><th>Description</th></tr>");
        $('#mytable').append("<tr><td>"+data2.title+"</td><td>"+data2.description+"</td></tr>");
        $('#mytable').append("<tr><th>Component</th><th>Summary</th><th>Other attributes</th></tr>");

        var title = $('#id1');
        title.innerHTML = "hello";
        $('#editdesc').value = data2.description;


        let data = JSON.parse(data2.json);
        console.log("\n\nJSON PARSED: \n " + data);


        let length = data.length;
        console.log("Length of # of objects of JSON : "+length + "\n\n");

        $('#summary').html("");
        for (var i = 0;i<length; i++)
        {
          console.log("Loop = " + i);
          console.log("length of data "+ i + " is : " +data[i].length);

          if (i == 0)
          {
            if((data[i].length) !== 0)
            {
              for (var f = 0; f < data[i].length; f++)
              {
                console.log("Path " + (f+1) + " "+data[i][f]["d"]);
                $("#mytable").append("<tr><td>Path " + (f+1) + "</td><td>path data = "+data[i][f]["d"]+"</td><td>" +data[i][f]["numAttr"]+ "</td></tr>");
              }
            }
            else {
              console.log("Skipped 1");
              $("#summary").html("");
            }
          }
          else if (i == 1)
          {
            if(data[i].length !== 0)
            {
              for (var f = 0; f < data[i].length; f++)
              {
                console.log("Group " + (f+1) + " "+data[i][f]["children"]);
                $("#mytable").append("<tr><td>Group " + (f+1) + "</td><td>"+data[i][f]["children"]+" child elements</td><td>"+data[i][f]["numAttr"] +"</td></tr>");
              }
            }
            else {
              console.log("Skipped 2");
            }
          }
          else if (i == 2)
          {
            if(data[i].length !== 0)
            {
              for (var f = 0; f < data[i].length; f++)
              {
                console.log("Rectangle " + (f+1) + " Upper left corner: x = "+data[i][f]["x"] + "y = " +data[i][f]["y"] + "Width: "+data[i][f]["w"] + ", Height"+data[i][f]["h"]);
                $("#mytable").append("<tr><td>Rectangle " + (f+1) + "</td><td>Upper left corner: x = "+data[i][f]["x"]+  "cm, y = " +data[i][f]["y"] + "cm Width: "+data[i][f]["w"] + "cm, Height: "+data[i][f]["h"]+ "cm</td><td>" + data[i][f]["numAttr"] + "</td></tr>");
              }
            }
            else {
              console.log("Skipped 3");
            }
          }
          else if (i == 3)
          {
            if(data[i].length !== 0)
            {
              for (var f = 0; f < data[i].length; f++)
              {
                console.log("Circle " + (f+1) + " Cenre: x = "+data[i][f]["cx"] + "cm y = " +data[i][f]["cy"] + "cm radius: "+data[i][f]["r"]+"cm");
                $("#mytable").append("<tr><td>Circle " + (f+1) + "</td><td>Centre: x = " + data[i][f]["cx"] + "cm y = " + data[i][f]["cy"] + "cm radius: " + data[i][f]["r"] + "cm <td>" + data[i][f]["numAttr"] + "</td></tr>");

              }
            }
            else {
              console.log("Skipped 3");
            }
          }
        }
      },
      fail: function(error) {
        alert("Unable to get data for " + filename);
        console.log(error);
      }
    });
  });







  $('#editAttribute').submit(function(data) {
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
      fail: function(error) {

        alert("Could not add component to " + filename);
        console.log(error);
      }
    });

  });




  /* Create New SVG Image */
  $('#createSVGimage').submit(function(data) {
    //Prevent it from submitting the form
    data.preventDefault();
    let file = $("#fileName").val() +".svg";
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

        if ((json["numRect"] === 0) && (json["numCirc"]=== 0) && (json["numPaths"]=== 0) && (json["numGroups"]=== 0))
        {
          console.log("File " + json["filename"] + " has no shapes. Will NOT add to Log Panel (SKIPPED)");
        }else{
          $('#table').append("<tr><td><img id='logImage' src='" + file + "'></td><td><a href='" + file + "'>" + file +"</a></td><td>" + json["filesize"] + "</td><td>" + json["numRect"] + "</td><td>"+ json["numCirc"] + "</td><td>" + json["numPaths"] + "</td><td>" + json["numGroups"] + "</td></tr>");
        }
        $('#dropdown').append("<option value ='" + file + "'>" + file + "</option>");
        $('#dropdown2').append("<option value ='" + file + "'>" + file + "</option>");
        $('#dropdown3').append("<option value ='" + file + "'>" + file + "</option>");

        alert("Successfully added file: " + file);
        location.reload();
        console.log(data);
      },
      fail: function(error) {
        alert("Unable to create file: " + file);
        console.log(error);
      }
    });

  });


  /* add Shape selection Modification */
  $(document).ready(function(){
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
    $('#addShape').submit(function(e) {
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

      if(filename !== "None")
      {
        $.ajax({
          type: 'get',            //Request type
          dataType: 'text',       //Data type - we will use JSON for almost everything
          url: '/addShape/',   //The server endpoint we are connecting to
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
            alert("Successfully Added");
            /*Reloads the page to display latest data */
            location.reload();
          },
          fail: function(error) {

            alert("Could not add component to " + filename);
            console.log(error);
          }
        });
      }
      else
      {
        alert("Please select a file");
      }
    });






    $('#scaleShape').submit(function(e) {
      e.preventDefault();
      let filename = $("#dropdown3 option:selected").text();
      console.log(filename);

      let shapeType = $("#selectShapeToScale option:selected").text();
      console.log(shapeType);

      let scaleFactor = $("#scale").val();
      console.log("scale Factor is = " + scaleFactor);

      if(filename !== "None")
      {
        $.ajax({
          type: 'get',            //Request type
          dataType: 'text',       //Data type - we will use JSON for almost everything
          url: '/scaleShape/',   //The server endpoint we are connecting to
          data: {
            filename: filename,
            shapeType: shapeType,
            scaleFactor: scaleFactor,
          },
          success: function (data) {
            console.log(data);
            console.log("in success");
            alert("Successfully Scaled Shape");
            /*Reloads the page to display latest data */
            location.reload();
          },
          fail: function(error) {

            alert("Could not add component to " + filename);
            console.log(error);
          }
        });
      }
      else
      {
        alert("Please select a file");
      }
    });



  });
