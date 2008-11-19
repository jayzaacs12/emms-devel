{javascript}
function updateBG(id,name) {
  top.window.opener.document.societyForm.parent_id.value = id;
  top.window.opener.document.societyForm.parent.value = name;
  top.window.close();
  }