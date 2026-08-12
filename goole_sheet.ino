function doGet(e) {
  return handleRequest(e);
}

function doPost(e) {
  return handleRequest(e);
}

function handleRequest(e) {

  var sheet = SpreadsheetApp.openById("1Xa0dFF0y3y_M3-D0qy5nwdKUELwqWtzOPtbdahahgVQ").getActiveSheet();

  var rfid = e.parameter.rfid;

  var studentData = getStudentData(rfid);

  if (studentData == null) {
    return ContentService.createTextOutput("Unknown Card");
  }

  // 🇮🇳 India Time
  var date = Utilities.formatDate(new Date(), "Asia/Kolkata", "yyyy-MM-dd");
  var time = Utilities.formatDate(new Date(), "Asia/Kolkata", "HH:mm:ss");

  // 📊 Add to Sheet
  sheet.appendRow([
    date,
    time,
    studentData.id,
    studentData.firstName,
    studentData.lastName,
    studentData.className,
    studentData.mobile
  ]);

  return ContentService.createTextOutput("Welcome " + studentData.firstName);
}


// 🔑 RFID Student Database (FINAL)
  function getStudentData(rfid) {

  var students = {

    "Card_id": {
      id: "S001",
      firstName: "name",
      lastName: "name",
      className: "required",
      mobile: "something"
    },

    "Card_id": {
      id: "S002",
      firstName: "name",
      lastName: "name",
      className: "required",
      mobile: "something"
    }

  };

  return students[rfid] || null;
}
