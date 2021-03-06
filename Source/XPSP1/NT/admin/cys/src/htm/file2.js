//***********************    Localization variable    *****************
var L_strErrMsg_TEXT = "Please enter a valid disk space and warning level.";
var L_strAlert_Message = "Invalid input"; 
//***********************    Localization variable    *****************
	
function loadForm()
// --------------------------------------------------------------------------------------
// function performed at page load;
// --------------------------------------------------------------------------------------
{
	// load the values stored in oDataStore, replace Nulls with empty strings and trim the ending ","
	divMain2.load("oDataStore");
	var P_cb_file2		= trim(noNull(divMain2.getAttribute("P_cb_file2")));
	var P_txt_file2		= trim(noNull(divMain2.getAttribute("P_txt_file2")));
	var P_select_file2	= trim(noNull(divMain2.getAttribute("P_select_file2")));
	
	// put in an array the values	
	var arrCB			= P_cb_file2.split(",");
	var arrTxt			= P_txt_file2.split(",");
	var arrSelect		= P_select_file2.split(",");
		
	// en/disable radio buttons 
	for(i=0;i<arrCB.length;i++) { form.cbFile2[i].checked = convertBol(arrCB[i]); }
	
	// restore the previous existent values entered	
	for(j=0;j<arrTxt.length;j++)
	{
		form.txtFile2[j].value	 			= ((arrTxt[j]=="")?"":arrTxt[j]);
		form.select_file2[j].selectedIndex	= ((arrSelect[j]=="")?0:arrSelect[j]);
	}
	setup();		// en/disable options and text accordingly
	loadFocus();	// set focus on NEXT button
}
	
function trim(strVal)
// --------------------------------------------------------------------------------------
// function used in loadForm(); trims the trailing ","
// -------------------------------------------------------------------------------------- 
{	if(strVal.charAt(strVal.length-1)==",")	strVal=strVal.substr(0,strVal.length-1); return(strVal); }
	
function setup()
// --------------------------------------------------------------------------------------
// function used in loadForm() to en/disable options and text accordingly
// --------------------------------------------------------------------------------------
{
	var setup 		= ((form.cbFile2[0].checked)?false:true);
	var className 	= ((!setup)?"grey":"disabled");
	var className2 	= ((!setup)?"white":"disabled");
	document.all.L_next_Button.disabled	= form.cbFile2[0].checked;
		
	for(i=1;i<form.cbFile2.length;i++)
	{
		form.cbFile2[i].disabled	= setup;		
		if (setup) form.cbFile2[i].checked = false;
	}
	
	for(j=0;j<table.length;j++)
	{
		table[j].className				= className;
		form.txtFile2[j].disabled		= setup;
		form.select_file2[j].disabled	= setup;
		form.txtFile2[j].className		= className2;
		form.select_file2[j].className 	= className2;
		if (setup) 
		{
			form.txtFile2[j].value		= "";
			form.select_file2[j].value 	= "";		
		}
	}		
	
	if(form.cbFile2[0].checked) isNum(1);
}
	
function isNum(pos)
// --------------------------------------------------------------------------------------
// function used in setup() to en/disable NEXT button accordingly to options made and OnKeyUp
// --------------------------------------------------------------------------------------
{
	if(form.txtFile2[pos].value=="")
		document.all.L_next_Button.disabled=true;
	else if(!isValid(form.txtFile2[pos].value))
	{
		showAlert(L_strAlert_Message);  // if non-numeric display error msg
		form.txtFile2[pos].value="";
		form.txtFile2[pos].focus();
		document.all.L_next_Button.disabled=true;
	}	
	else if(isValid(form.txtFile2[0].value)&&isValid(form.txtFile2[1].value))		
		document.all.L_next_Button.disabled=false;
}

function isValid(value)
// --------------------------------------------------------------------------------------
// function used in isNum() to check entries non-numerical validity
// --------------------------------------------------------------------------------------
{
	for(i=0;i<value.length;i++)
	{
		if(isNaN(value.charAt(i))) { return false; }
	}		
	return true;
}	
	
function back()
// --------------------------------------------------------------------------------------
// process when click on BACK button (persist data and load previous file.htm page)
// --------------------------------------------------------------------------------------	
{ saveForm(); self.location.href="member.htm"; }
	
function next()
// --------------------------------------------------------------------------------------
// process when NEXT button clicked ; check entries made and proceed to next page (file3.htm) 
// --------------------------------------------------------------------------------------	
{
	divMain2.load("oDataStore");
	var P_JumpIndex	= convertBol(noNull(divMain2.getAttribute("P_JumpIndex")));
	if (P_JumpIndex)
	{
		divMain2.setAttribute("P_file3Index", true);	
		divMain2.setAttribute("P_install","fileServer");
		var strNextPage = "confirm.htm";
	}
	else
	{
		var strNextPage = "file3.htm";
	}
	
	var checkForm = true;		
	if(form.cbFile2[0].checked)
	{
		for(i=0;i<form.txtFile2.length;i++)
		{
			// invalidate entries if non-numeric or zero
			if((isNaN(form.txtFile2[i].value)||form.txtFile2[i].value==0)) {checkForm = false;}
		}
	}
	if(!checkForm)
		showAlert(L_strErrMsg_TEXT);	// display error message if wrong entries
	else if(!checkValues())
	{
		var strWarning = form.txtFile2[1].value + " " + form.select_file2[1].options[form.select_file2[1].selectedIndex].value;
		var strLimit = form.txtFile2[0].value + " " + form.select_file2[0].options[form.select_file2[0].selectedIndex].value;
		// display message if warning level bigger than limit level 
		var L_reset_Message = 'Warning level ("%1") exceeds quota limit ("%2").  Set warning level to "%2"?';
		var resetMessage = L_reset_Message.replace(/%1/g, strWarning);
		resetMessage = resetMessage.replace(/%2/g, strLimit);
		var responseResetMessage	= confirm(resetMessage);
		if(responseResetMessage)		// if user selects to set warning to limit level, do that and move to next page
		{			
			form.txtFile2[1].value = form.txtFile2[0].value;
			form.select_file2[1].selectedIndex	= form.select_file2[0].selectedIndex;
			saveForm()
			self.location.href = strNextPage;
		}
		else { form.txtFile2[1].focus(); }
	}
	else { saveForm(); self.location.href = strNextPage; }
}

function checkValues()
// --------------------------------------------------------------------------------------
// function used in next() to convert to bytes the entries and compare 
// --------------------------------------------------------------------------------------
{
	var limit 	= convertBytes(form.txtFile2[0].value, form.select_file2[0].selectedIndex);
	var warning = convertBytes(form.txtFile2[1].value, form.select_file2[1].selectedIndex);
	if (limit<warning) 
		return false;
	else 
		return true;
}

function convertBytes(val, size)
// --------------------------------------------------------------------------------------
// function used in checkValues() to convert to bytes the entries based on selection made (Kb, Mb, GB) 
// --------------------------------------------------------------------------------------
{
	switch(size)
	{
		case 1:
			val *= 1048576;
			break;
		case 2:
			val *= 1073741824;
			break;
		default:
			val *= 1024;
			break;
	}
	return val;
}
	
function saveForm()
// --------------------------------------------------------------------------------------
// process before exiting the page; saves the selection and entries made
// --------------------------------------------------------------------------------------
{
	var cbFile2			= "";
	var txtFile2		= "";
	var select_file2	= "";
		
	for(i=0;i<form.cbFile2.length;i++)
		cbFile2 += form.cbFile2[i].checked + ",";
	for(j=0;j<form.txtFile2.length;j++)
	{
		txtFile2 += form.txtFile2[j].value + ",";
		select_file2 += form.select_file2[j].selectedIndex + ",";
	}
		
	cbFile2	= trim(cbFile2);
	txtFile2	= trim(txtFile2);
	select_file2 = trim(select_file2);
		
	divMain2.setAttribute("P_cb_file2",cbFile2);
	divMain2.setAttribute("P_txt_file2",txtFile2);
	divMain2.setAttribute("P_select_file2",select_file2);
	divMain2.save("oDataStore");
}
	

function check(pos) 
// --------------------------------------------------------------------------------------
// function used OnClick on checkboxes
// --------------------------------------------------------------------------------------
{ 
	if (!(form.cbFile2[pos].disabled))
		form.cbFile2[pos].checked=(form.cbFile2[pos].checked)?false:true; 
		if (pos==0) setup();
}
	
