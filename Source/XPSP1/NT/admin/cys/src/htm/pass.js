//***************  Localization variable  **********************************************************
var L_strErrMsg_Message = "The confirmation does not match the password. Try typing the password again.";
//***************  Localization variable  **********************************************************

function loadForm(){
	divMain2.load("oDataStore");
	formPassword.txtPassword.value = noNull(divMain2.getAttribute("P_pass"));
	formPassword.txtConfirm.value = formPassword.txtPassword.value;
	formPassword.txtPassword.focus();
	//loadFocus();
}
/*	
function accessKeys(){
	key=event.keyCode;
	if(event.altKey)
	{
		if(key==67) formPassword.txtConfirm.focus();		// Alt + C
		else if(key==80) formPassword.txtPassword.focus();	// Alt + P			
	}			
}
*/	
function checkPassword(){
	varPassword = formPassword.txtPassword.value;
	varConfirm = formPassword.txtConfirm.value;
	if (varPassword != varConfirm) {return false;}
	else {return true;}
}
	
function back(){
	saveForm();
	self.location.href="dc3.htm";		
}
	
function next(){
	if(checkPassword())
	{
		saveForm();
		self.location.href="confirm.htm";
	}
	else showAlert(L_strErrMsg_Message)
}
	
function saveForm(){
	var P_pass = formPassword.txtPassword.value;
	divMain2.setAttribute("P_pass",P_pass);
	divMain2.save("oDataStore");
}
