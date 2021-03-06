//-------------------------     Localization variable     -----------------------------------------------------------------
var L_strServerErrMsg_Message	= "You must select an option.";
var L_DHCPWarningMsg_Message 	= "Because one or more other servers already exist, you cannot configure this server as the only server on the network.";
//-------------------------     Localization variable     -----------------------------------------------------------------
	
var srvWiz = new ActiveXObject("ServerAdmin.ConfigureYourServer");

function loadForm()
// --------------------------------------------------------------------------------------
// inital process when loading the page; display wait message, change SZ_ShowStartup=0, en/disable Express path
// --------------------------------------------------------------------------------------
{
	var L_strWait_TEXT = "<BR>Please wait while Configure Your Server detects your network settings (2)...";
	pWait.innerHTML	= L_strWait_TEXT;
	divWait.style.display = "inline";	//make visible "Please Wait" message
	//addDot();							//display a "please wait" message; function wait() below		
	pWait.innerHTML += '. ';
	
	document.body.style.cursor = "wait";
	document.all.L_next_Button.disabled = false;
	document.all.L_next_Button.focus();
	loadFocus();
	
	setReg2(SZ_ShowStartup, 0);			//CYS was run, no reload of CYS unless otherwise specified
	testDHCP();
}

function testDHCP()
{
	divMain2.load("oDataStore");
	var i = noNull(divMain2.getAttribute("P_server"));
	var j = noNull(divMain2.getAttribute("P_serverDC"));
					
	// If DHCP server not found on the network and network adapter(s) is/are not DHCP enabled 
	// or 1 of 2 NICs is DHCPEnabled but is a public adapter, enable express setup, else disable
	//var testDHCP = srvWiz.CheckDhcpServer();
	//var testDHCP = noNull(divMain2.getAttribute("P_testDHCP"));		
	var bolExpressPathAllowed = convertBol(noNull(divMain2.getAttribute("P_ExpressPathAllowed")));	//	alert (bolExpressPathAllowed);		
	//if(((testDHCP != 0)||srvWiz.DsRole(0)||srvWiz.DsRole(1))||(DetectNetWorkAdapterConguration()==0)) 
	if (!bolExpressPathAllowed)
	{
		formServer.rdServerRole[0].checked	= false;
		formServer.rdServerRole[0].disabled = true;	
		labelServerRoleExpress.className = 'disabled';
		labelServerRoleExpress.disabled = true;	
		spanExpress.className = 'disabled';
		spanExpress.disabled = true;
									
		tblDC.className = 'grey';
		formServer.rdServerRole[1].checked	= true;
		formServer.rdDCRole[0].disabled	= false;
		formServer.rdDCRole[0].checked = true;
	}
		
	if(i!='')			// disable DCPromo option if this is a DC or is in middle of upgrade.
	{			
		try
		{
			formServer.rdServerRole[i].checked	= true;
			formServer.rdDCRole[j].checked		= true;
		}
		catch(e){}					
	}
	
	divWait.style.display = "none";	//hide "Please Wait" message			
	divWait.style.visibility ="hidden" ;
	document.body.style.cursor = "default";
	
	dispTbl();		
	loadFocus();	
}
		
function wait()
// --------------------------------------------------------------------------------------
// function displys a "Please wait" message with a progress indication (.)
// --------------------------------------------------------------------------------------
{
	//divWait.style.display = "inline";	//make visible "Please Wait" message
	addDot();
}
	
function addDot()
{
	pWait.innerHTML += ". ";			//add a . for at existing message text to indicate progress
	setTimeout("addDot()",200);			//do the . addition at each 500ms (repeat the fct at 500ms)
}

function dispTbl()
// --------------------------------------------------------------------------------------
// enable/disable radio buttons accordingly 
// --------------------------------------------------------------------------------------
{ 
	
	tblDC.className=((formServer.rdServerRole[1].checked)?"grey":"disabled");
	tblDC.disabled=((formServer.rdServerRole[1].checked)?false:true);
	for(i=0;i<formServer.rdDCRole.length;i++){
		formServer.rdDCRole[i].disabled=((formServer.rdServerRole[1].checked)?false:true);
		if(formServer.rdDCRole[i].disabled) formServer.rdDCRole[i].checked=false;
	}
	if(srvWiz.DsRole(0)||srvWiz.DsRole(1))
	{
		formServer.rdDCRole[1].disabled	= true;
		rdDCRole1Label.disabled = true;
	}
	if (((!formServer.rdDCRole[0].checked)&&(!formServer.rdDCRole[1].checked))&&(!formServer.rdDCRole[0].disabled)) formServer.rdDCRole[0].checked = true;
	
	// enable Next button if correct/complete option selected
	document.all.L_next_Button.disabled=(formServer.rdServerRole[0].checked||formServer.rdDCRole[0].checked||formServer.rdDCRole[1].checked)?false:true;		
}

function checkServer(intPos)
// --------------------------------------------------------------------------------------
// used at OnClick on radio buttons to en/disable option buttons and text
// --------------------------------------------------------------------------------------
{
	if(!formServer.rdServerRole[0].disabled)
	{
		formServer.rdServerRole[intPos].checked=(formServer.rdServerRole[intPos].checked)?false:true;
		dispTbl();		
	}
}

function checkDC(intPos)
// --------------------------------------------------------------------------------------
// used at OnClick on radio buttons labels to en/disable option buttons and text
// --------------------------------------------------------------------------------------
{
	if (intPos==0)
	{
		formServer.rdDCRole[intPos].checked=true;
		dispTbl();		
	}
	else if (!(srvWiz.DsRole(0)||srvWiz.DsRole(1)))
	{
		formServer.rdDCRole[intPos].checked=true;
		dispTbl();		
	}	
}
	
function back()
// --------------------------------------------------------------------------------------
// process when clik on BACK button (persist data and load previous page)
// --------------------------------------------------------------------------------------	
{
	saveForm();
	self.location.href="welcome.htm";
}

function installNext(strServer)
// --------------------------------------------------------------------------------------
// Sets the next member service to install; used in fct next() below
// --------------------------------------------------------------------------------------
{	
	divMain2.setAttribute("P_install",strServer);
	divMain2.save("oDataStore");
}

function next()
// --------------------------------------------------------------------------------------
// process when NEXT button clicked ; proceed to next page accordingly to (server role) selection made 
// --------------------------------------------------------------------------------------	
{
	var next	= "";		
	if(formServer.rdServerRole[1].checked)
	{
		if((formServer.rdDCRole[0].checked)||(formServer.rdDCRole[1].checked))
		{
			next=((formServer.rdDCRole[1].checked)?"dcOnly":"member.htm");
			if(next=="dcOnly")
			{
				installNext("dcOnly");
				next = "confirm.htm";
			}
		}			
	}
	else if(formServer.rdServerRole[0].checked) {next="dc.htm";}
		
	if(next=="") showAlert(L_strServerErrMsg_Message);
	else
	{
		saveForm();
		self.location.href=next;
	}		
}
	
function saveForm()
// --------------------------------------------------------------------------------------
// process before exiting the page; saves the selection made
// --------------------------------------------------------------------------------------
{	
	var radio1	= -1;
	var radio2	= -1;
		
	if(formServer.rdServerRole[0].checked)
	{
		radio1 = 0;
		radio2 =-1;				
	}
	else if(formServer.rdServerRole[1].checked)
	{
		radio1=1;
		//if((formServer.rdDCRole[0].checked)||(formServer.rdDCRole[1].checked)){
			radio2=((formServer.rdDCRole[0].checked)?0:1);
		//}
	}
	divMain2.setAttribute("P_server",radio1);
	divMain2.setAttribute("P_serverDC",radio2);
	divMain2.save("oDataStore");
}