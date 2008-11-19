{javascript}
function updateSociety(id,name) {
  top.window.opener.document.clientForm.society_id.value = id;
  top.window.opener.document.clientForm.society.value = name;
  top.window.close();
  }