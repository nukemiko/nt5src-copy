var strURL			= "";
var srvWiz			= new ActiveXObject("ServerAdmin.ConfigureYourServer");

//var clustering	= ((srvWiz.IsServiceInstalled("Clustering")==0)||(srvWiz.IsServiceInstalled("Clustering")!=-1))?true:false;
var wizRRAS		= getReg(SZ_RRASConfig);
var wizDHCP		= srvWiz.IsDhcpConfigured();	

var DNS		= (srvWiz.IsServiceInstalled("DNS"))?true:false;						//var DNS = ((srvWiz.IsServiceInstalled("DNS"))&&(wizDNS))?true:false;
var DHCP	= ((srvWiz.IsServiceInstalled("DHCP"))&&(wizDHCP))?true:false;
var WINS	= (srvWiz.IsServiceInstalled("WINS"))?true:false;
var RRAS	= ((srvWiz.IsServiceInstalled("RRAS"))&&(wizRRAS))?true:false;

var IISInstalled = srvWiz.IsServiceInstalled("IIS");
if (IISInstalled)
{
	try
	{
		var srvOWS = new ActiveXObject("SpCyscom.SpCys");

		var OWSInstalled = srvOWS.SPAlreadyInstalled();		// actuall check for "OWS"	
		if (!OWSInstalled)
		{
			// calling the SPAskReplace method; returns true if a file default.htm exist in wwwroot
			var changeHomePage	= srvOWS.SPAskReplace();					//alert (changeHomePage);
		}
	}
	catch (e)
	{
		//alert ("SharePoint missing!");
		var OWSInstalled = true;			// consider SP installed, so NEXT is disabled
	}
}
else
{
	var OWSInstalled = false;	
	var changeHomePage = false;
}

//var OWSInstalled = confirm("Sharepoint Installed ?");	// delete when SP checked in;
//var OWSInstalled = true;

//********************************************    Localization variables    ****************************************************************
var L_strErrMsg_Message	= "You must select one of the network infrastructure services.";
var arrText	= new Array();

// not optimized & TS not installed
var L_T01_TEXT = "<P>Application servers store programs that licensed clients can gain access to and use. If you select this option, Configure Your Server optimizes this server to perform as an application server. You can also install Terminal Server, which sets up this server to process tasks for authorized clients.</P>";
// optimized, but TS not installed
var L_T02_TEXT = "<P>Application servers store programs that licensed clients can gain access to and use. Although this server is already optimized to perform as an application server, you can install Terminal Server to set up this server to process tasks for authorized clients.</P>";
// optimized and TS installed
var L_T03_TEXT = "<P>Application servers store programs that licensed clients can gain access to and use. This server is optimized to perform as an application server and Terminal Server has been installed.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
// not optimized, but TS installed
var L_T04_TEXT = "<P>Application servers store programs that licensed clients can gain access to and use. If you select this option, Configure Your Server optimizes this server to perform as an application server. Terminal Server has been installed.</P>";
// Standard server SKU, optimized
var L_T05_TEXT = "<P>Application servers store programs that licensed clients can gain access to and use. This server is optimized to perform as an application server.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
// Standard server SKU, not optimized
var L_T06_TEXT = "<P>Application servers store programs that licensed clients can gain access to and use. If you select this option, Configure Your Server optimizes this server to perform as an application server.</P>";

var strSKU = srvWiz.GetProductSku();	// 3="Server";4="Advanced Server";5="Datacenter Server";1="Personal";2="Professional";
if (strSKU > 3)
{
	if((getReg(SZ_AppServerSize)==3&&getReg(SZ_AppServerCache)==0)&&(getReg(SZ_TSApplicationMode)==1))
	{	 arrText[0] = L_T03_TEXT;	}		// optimized and TS installed alert("optimized and TS installed");
	else if((getReg(SZ_AppServerSize)==3&&getReg(SZ_AppServerCache)==0)&&(getReg(SZ_TSApplicationMode)!=1))
	{	 arrText[0] = L_T02_TEXT;	}		// optimized, but TS not installed alert("optimized, but TS not installed");
	else if(((getReg(SZ_AppServerSize)!=3)||(getReg(SZ_AppServerCache)!=0))&&(getReg(SZ_TSApplicationMode)!=1))
	{	 arrText[0] = L_T01_TEXT;	}		// not optimized & TS not installed alert("not optimized & TS not installed");
	else if(((getReg(SZ_AppServerSize)!=3)||(getReg(SZ_AppServerCache)!=0))&&(getReg(SZ_TSApplicationMode)==1))
	{	 arrText[0] = L_T04_TEXT;	}		// not optimized, but TS installed alert("not optimized, but TS installed");
}
else
{
	if (getReg(SZ_AppServerSize)==3&&getReg(SZ_AppServerCache)==0)
	{	 arrText[0] = L_T05_TEXT;	}		// Standard server SKU, optimized
	else if ((getReg(SZ_AppServerSize)!=3)||(getReg(SZ_AppServerCache)!=0))
	{	 arrText[0] = L_T06_TEXT;	}		// Standard server SKU, not optimized
}

var L_T11_TEXT = "<P>A cluster is a group of independent servers managed as a single system for higher availability, easier manageability, and greater scalability. Because the Cluster server component is already installed on this server, it is set up to function as a member of a cluster.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
var L_T12_TEXT = "<P>A cluster is a group of independent servers managed as a single system for higher availability, easier manageability, and greater scalability. If you select this option, Configure Your Server installs the Cluster server component. Cluster service provides failover support by automatically detecting the failure of an application or server and quickly restarting it on a surviving server in the cluster.</P>";
//if(srvWiz.IsServiceInstalled("Clustering")){ arrText[1] = L_T11_TEXT ;}
//else{arrText[1] = L_T12_TEXT ; //}

//var L_T21_TEXT = "<P>File servers provide and manage access to files. If you select this option, you can use this wizard to share folders that contain publicly available files. You can also limit disk space usage on this server.</P>";	
//var L_T22_TEXT = "<P>File servers provide and manage access to files. If you select this option, you can use this wizard to share folders that contain publicly available files and publish information about these folders in Active Directory. You can also limit disk space usage on this server.</P>";
//arrText[2] = (bolPublishAllowed != 0)? L_T22_TEXT : L_T21_TEXT ;
var L_T2YesNTFSNoSP_TEXT = "<P>File servers provide and manage access to files. If you select this option, you can limit disk space usage and turn on or off the indexing service on this server.";
var L_T2YesNTFSYesSP_TEXT = "<P>File servers provide and manage access to files. If you select this option, you can use this wizard to limit the amount of disk space allocated to users of this server.";
var L_T2NoNTFSNoSP_TEXT = "<P>File servers provide and manage access to files. If you select this option, you can turn on or off the indexing service on this server.";
var L_T2NoNTFSYesSP_TEXT = "<P>File servers provide and manage access to files. All configurations available through the Configure Your Server wizard for a file server are complete.</P><P>Select another server type, or click Cancel to close the wizard.";

var L_T31_TEXT = "<P>Because Windows codename Whistler networking services are currently installed and configured on this server, it is already a functional networking and communications server.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
var L_T32_TEXT = "<P>If you select this option, Configure Your Server installs the Whistler networking services that you select. DHCP assigns IP addresses to network clients. DNS translates domain names to IP addresses and IP addresses to domain names. WINS maps IP addresses to computer names (NetBIOS names). Routing and Remote Access can route network traffic or set up either dial-up networking or a virtual private network (VPN).</P>";
if(DHCP&&DNS&&WINS&&RRAS){ arrText[3]= L_T31_TEXT; }
else{ arrText[3]= L_T32_TEXT ;}

var L_T4_TEXT = "<P>Print servers provide and manage access to network printers and printer drivers. If you select this option, you can use the Add Printer Wizard and the Add Printer Driver Wizard to install printers and printer drivers on this server.</P>";
arrText[4] = L_T4_TEXT; 

/* delete this when confirmed ok
//var L_T51NoIIS_TEXT = "<P>SharePoint is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions. If you select this option, Configure Your Server installs SharePoint and Internet Information Services (IIS).</P>";
//var L_T51WithIIS_TEXT = "<P>SharePoint is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions. If you select this option, Configure Your Server installs SharePoint.</P>";
//var L_T52_TEXT = "<P>SharePoint is a set of Web server extensions that enable groups within your organization to work together by sharing documents, lists of information, threaded discussions, and more. The SharePoint component is currently installed on this server. To administer SharePoint, click <B>Start</B>, point to <B>Programs</B>, then to <B>Administrative Tools</B>, and click <B>SharePoint Administrator</B>.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
*/

//if OWS installed
var L_T5OWSInstalled_TEXT = "<P>Microsoft’s SharePoint Team Services is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions. Because SharePoint is already installed on this server, it is already a functioning SharePoint server.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
//else if OWS not installed
	// if IIS is installed             
		//If Indexing Service is turned on
		var L_T5IISInstalledIndxOn_TEXT = "<P>Microsoft’s SharePoint Team Services is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions. If you select this option, Configure Your Server installs SharePoint Team Services.</P>";
		//Else if Indexing Service is turned off
		var L_T5IISInstalledIndxOff_TEXT = "<P>Microsoft’s SharePoint Team Services is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions.  If you select this option, Configure Your Server installs SharePoint Team Services and turns Indexing Service on.</P>";
	// else if IIS is not installed
		//If Indexing Service is turned on
		var L_T5NoIISIndxOn_TEXT = "<P>Microsoft’s SharePoint Team Services is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions. If you select this option, Configure Your Server installs SharePoint Team Services and Internet Information Services (IIS).</P>";		
		//Else if Indexing Service is turned off
		var L_T5NoIISIndxOff_TEXT = "<P>Microsoft’s SharePoint Team Services is a set of Web server extensions that enable groups within an organization to work together by sharing information such as documents and threaded discussions. If you select this option, Configure Your Server installs SharePoint Team Services and Internet Information Services (IIS) and turns Indexing Service on.</P>";		
		
if(OWSInstalled)
{	arrText[5] = L_T5OWSInstalled_TEXT; }
else 
{ 
	var objAdminIS 	= new ActiveXObject("Microsoft.ISAdm")
	IndexON = (objAdminIS.IsRunning())? true : false ;
	   
	if (IISInstalled)	// If IIS is installed
	{
		arrText[5] = (IndexON) ? L_T5IISInstalledIndxOn_TEXT : L_T5IISInstalledIndxOff_TEXT;		//  depending if Indexing Service is turned on or not
	}
	else	// else if IIS is not installed
	{
		arrText[5] = (IndexON) ? L_T5NoIISIndxOn_TEXT : L_T5NoIISIndxOff_TEXT;		//  depending if Indexing Service is turned on or not
	}	
}

var L_T61_TEXT = "<P>Streaming media servers store digital multimedia presentations that are transmitted over an intranet or the Internet. Because Windows Media Services is currently installed on this server, it is already a functional streaming media server.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
var L_T62_TEXT = "<P>Streaming media servers store multimedia content that can be transmitted over an intranet or the Internet. If you select this option, Configure Your Server installs Windows Media Services. You can use Windows Media Services to deliver real-time multimedia content or to prepare and stream stored content.</P>";
if(srvWiz.IsServiceInstalled("StreamingMedia")){ arrText[6] = L_T61_TEXT; }
else{ arrText[6] = L_T62_TEXT; }

var L_T71_TEXT = "<P>Web servers host and manage Web sites and share information on a network. Internet Information Services (IIS) is currently installed on this server.</P><P>Select another server type, or click Cancel to close the wizard.</P>";
var L_T72_TEXT = "<P>Web servers host and manage Web sites and share information on a network. If you select this option, Configure Your Server installs Internet Information Services (IIS). You can use IIS to set up a Web or FTP site on your corporate intranet or create sites for the Internet.</P>";
if(IISInstalled){ arrText[7] = L_T71_TEXT; }
else{ arrText[7] = L_T72_TEXT; }


//********************************************    Localization variables  section end  ****************************************************************

function loadForm()
// --------------------------------------------------------------------------------------
// function performed at page load; 
// --------------------------------------------------------------------------------------
{
	divMain2.load("oDataStore");
	var P_select = divMain2.getAttribute("P_select");
	
	/* Removal of second dropdown option, previous Cluster, current File Server
	if(!clustering) form.selectServer.options.remove(1); // Machine does not meet requirements for clustering	
	*/
		
	// detect the processor type to temporary remove the Media Server in case of IA64 (not yet supported)
	// and remove SharePoint also
	var wSHShell 	= new ActiveXObject("WScript.Shell");
	var WshSysEnv = wSHShell.Environment("SYSTEM");
	var processorType = WshSysEnv("PROCESSOR_ARCHITECTURE");
	//var proc = confirm("IA64 ?"); processorType = (proc)?'ia64':'x86';
	if ((processorType == 'ia64')||(processorType == 'IA64')) 
	{ 
		form.selectServer.options.remove(4);	//remove SharePoint		 
		form.selectServer.options.remove(4);	//remove Media Server (5 becomes 4)
	}	
		
	for(i=0;i<form.selectServer.options.length;i++)
	{
		if(P_select==form.selectServer.options[i].value) form.selectServer.selectedIndex=i;
	}		
	show();				// try showMenu()
	loadFocus();		// focus NEXT button 
	enable();			// based on selection and current status en/disable options
}

function show()
// --------------------------------------------------------------------------------------
// showMenu() (fct.below); used in loadForm() and OnChange combobox 
// --------------------------------------------------------------------------------------
{
	try {showMenu();}
	catch(e)
	{
		var FailLog_TEXT = '  Error : ' + e + "\n";
		var varErrorNumber = e.number;
		var hexString = "0x" + varErrorNumber.toString(16); 	// hex is base 16
		FailLog_TEXT	+= '  Error Number: '+ varErrorNumber + ' (' + hexString + ')' + "\n";
		FailLog_TEXT	+= '  Error Description: ' + e.description + "\n";
		alert(FailLog_TEXT);
	}	
}
	
function showMenu()
// --------------------------------------------------------------------------------------
// based on selection and current status en/disable Networking options and displays the correct text
// --------------------------------------------------------------------------------------
{
	strURL		= "";
	var choice	= form.selectServer.selectedIndex;
	var enable	= ((form.selectServer.options[choice].value=="4")?false:true);		
	if (choice>0) choice++;		
		
	p2.className			= ((!enable)?"grey":"disabled");
	p1b.className			= ((!enable)?"grey":"disabled");
	table.className			= ((!enable)?"grey":"disabled");
	form.cbDHCP.disabled 	= enable;
	chkSpanDHCP.disabled	= enable;
	form.cbDNS.disabled 	= enable;
	chkSpanDNS.disabled		= enable;
	form.cbWINS.disabled 	= enable;
	chkSpanWINS.disabled	= enable;
	form.cbRRAS.disabled 	= enable;
	chkSpanRRAS.disabled	= enable;
	
	if ((form.selectServer.options.length==5)&&(choice==5))
	{	
		choice = choice + 2 ;
	}
	span2.innerHTML	= arrText[choice];			// change the displayed text according selection	
		
	if(!enable)
	{		
		form.cbDNS.checked		= DNS;
		form.cbDNS.disabled		= DNS;
		form.cbDHCP.checked		= DHCP;
		form.cbDHCP.disabled	= DHCP;
		form.cbWINS.checked		= WINS;
		form.cbWINS.disabled	= WINS;
		form.cbRRAS.checked		= RRAS;
		form.cbRRAS.disabled	= RRAS;
	}
	else
	{		
		form.cbDHCP.checked	= (convertBol(divMain2.getAttribute("P_selectDHCP"))||DHCP);
		form.cbDNS.checked	= (convertBol(divMain2.getAttribute("P_selectDNS"))||DNS);
		form.cbWINS.checked	= (convertBol(divMain2.getAttribute("P_selectWINS"))||WINS);
		form.cbRRAS.checked	= (convertBol(divMain2.getAttribute("P_selectRRAS"))||RRAS);	
	}
	if(choice!=3)
	{			
		form.cbDHCP.checked	= DHCP;
		form.cbDNS.checked	= DNS;
		form.cbWINS.checked	= WINS;
		form.cbRRAS.checked	= RRAS;
	}
	document.all.L_next_Button.disabled=false;		
	switch(choice)
	{
		case 0: 
			if (strSKU > 3)
			{
				if((getReg(SZ_AppServerSize)==3&&getReg(SZ_AppServerCache)==0)&&(getReg(SZ_TSApplicationMode)==1))
				{	// optimized and TS installed
					document.all.L_next_Button.disabled=true;
					strURL="";	
				}		
				else if((getReg(SZ_AppServerSize)==3&&getReg(SZ_AppServerCache)==0)&&(getReg(SZ_TSApplicationMode)!=1))
				{	// optimized, but TS not installed
					installNext("appServer");
					document.all.L_next_Button.disabled=false;
					strURL="app.htm";	
				}		
				else if(((getReg(SZ_AppServerSize)!=3)||(getReg(SZ_AppServerCache)!=0))&&(getReg(SZ_TSApplicationMode)!=1))
				{	// not optimized & TS not installed
					installNext("appServer");
					document.all.L_next_Button.disabled=false;
					strURL="app.htm";
				}		
				else if(((getReg(SZ_AppServerSize)!=3)||(getReg(SZ_AppServerCache)!=0))&&(getReg(SZ_TSApplicationMode)==1))
				{	// not optimized, but TS installed
					installNext("appServer");
					document.all.L_next_Button.disabled=false;
					strURL="confirm.htm";				
				}
			}
			else
			{
				if (getReg(SZ_AppServerSize)==3&&getReg(SZ_AppServerCache)==0)				// Standard server SKU, optimized
				{
					document.all.L_next_Button.disabled=true;								// button Next disabled
					strURL="";
				}		
				else if ((getReg(SZ_AppServerSize)!=3)||(getReg(SZ_AppServerCache)!=0))		// Standard server SKU, not optimized
				{	
					installNext("appServer");												// Next enabled, goto Completion page
					document.all.L_next_Button.disabled=false;
					strURL="confirm.htm";	
				}		
			}
			break;					
		
		/*
		case 1: 
			//if(srvWiz.IsServiceInstalled("Clustering"))
			//{
			//	document.all.L_next_Button.disabled=true;
			//	strURL="";
			//}
			//else
			//{
				installNext("clusterServer");
				strURL="confirm.htm"
			//}
			break;
		*/
		
		case 2:
			// detect NTFS partitions
			document.body.style.cursor = "wait";
			var bolNTFS	= false;
			var oFS 	= new ActiveXObject("Scripting.FileSystemObject");	
			var oEnum	= new Enumerator(oFS.Drives);	
			for (;!oEnum.atEnd();oEnum.moveNext())		// If an NTFS drive found, 
			{		
				var oDrv=oEnum.item();
				if((oDrv.IsReady)&&(oDrv.FileSystem=="NTFS")&&(oDrv.DriveType==2)) bolNTFS=true;
			}
			document.body.style.cursor = "default";
			
			//var bolNTFS = confirm("NTFS partiton existent ?");		// delete when checked in;
			
			var P_JumpNTFS = (bolNTFS)? false:true;
			divMain2.setAttribute("P_JumpNTFS", P_JumpNTFS);	// store JumpNTFS to know to skip DiskQuotas page	
			
			if (bolNTFS)	//If there is at least one NTFS partition 
			{	
				//If SharePoint is not installed
					// “File servers provide and manage access to files. If you select this option, you can limit disk space usage and turn on or off the indexing service on this server.”					
				// Else if SharePoint is installed
					//“File servers provide and manage access to files. If you select this option, you can limit disk space usage on this server.”			
				span2.innerHTML	=(!OWSInstalled)?L_T2YesNTFSNoSP_TEXT:L_T2YesNTFSYesSP_TEXT;	
				strURL = "file2.htm" ;		// NTFS partition found, advance to disk quotas page
			}
			else			//Else if there is no NTFS partition
			{
				if(!OWSInstalled)		// If SharePoint is not installed
				{
					// “File servers provide and manage access to files. If you select this option, you can turn on or off the indexing service on this server.”
					span2.innerHTML	= L_T2NoNTFSNoSP_TEXT;			// change the text according situation
					strURL = "file3.htm" ;
				}
				else		// Else if SharePoint is installed
				{				
					// “File servers provide and manage access to files. All configurations available through the Configure Your Server wizard for a file server are complete. Select another server type, or click Cancel to close the wizard.”
					span2.innerHTML	= L_T2NoNTFSYesSP_TEXT;			// change the text according situation
					// (The Next button is disabled under this set of conditions.)
					document.all.L_next_Button.disabled=true;
				}
			}
			break;
			
		case 3: 
			form.cbDHCP.checked	= (convertBol(divMain2.getAttribute("P_selectDHCP"))||DHCP);
			form.cbDNS.checked	= (convertBol(divMain2.getAttribute("P_selectDNS"))||DNS);
			form.cbWINS.checked	= (convertBol(divMain2.getAttribute("P_selectWINS"))||WINS);
			form.cbRRAS.checked	= (convertBol(divMain2.getAttribute("P_selectRRAS"))||RRAS);
			installNext("netWorkServer");	
			document.all.L_next_Button.disabled=true;
			strURL="confirm.htm";			
			if(DHCP&&DNS&&WINS&&RRAS) {strURL="";}					
			else if(((!DHCP)&&(form.cbDHCP.checked))||((!DNS)&&(form.cbDNS.checked))||((!WINS)&&(form.cbWINS.checked))||((!RRAS)&&(form.cbRRAS.checked))) 			
			{
				document.all.L_next_Button.disabled=false;
				strURL="confirm.htm";
			}
			else if(((!DHCP)&&(!form.cbDHCP.checked))||((!DNS)&&(!form.cbDNS.checked))||((!WINS)&&(!form.cbWINS.checked))||((!RRAS)&&(!form.cbRRAS.checked)))
			{
				strURL="";
			}			
			else
			{
				document.all.L_next_Button.disabled=false;
				strURL="confirm.htm";
			}			
			break;
			
		case 4: 
			strURL="print.htm";
			break;
		
		/*******************************   modified because of Media Server removal ******************************
		case 5: 
			if(srvWiz.IsServiceInstalled("StreamingMedia"))
			{
				document.all.L_next_Button.disabled=true;
				strURL="";
			}
			else
			{
				installNext("streamingMediaServer");
				strURL="confirm.htm"
			}
			break;
	
		*******************************   modified because of Media Server removal *******************************/
		
		case 5:
			if(!OWSInstalled)   // if SharePoint not installed (OWSInstalled=false)
			{
				if(IISInstalled)		//if IIS is installed
				{					
					// Once the user chooses to install SharePoint on their machine, and it has been determined that the user should be asked
					// if they want their existing homepage replaced (this is determined by calling the SPAskReplace method) they are taken to the following page.   		
					if (changeHomePage)	
					{
						strURL="owshome.htm";	// there is a homepage to replace, go to Change Home Page to SharePoint Home Page (owshome.htm)	
					}
					else
					{
						strURL="confirm.htm";	// no need to replace homepage, go directly to completion Page
						divMain2.setAttribute("P_OWSChangeHomePage", true);  // set SPInstall argument to TRUE; If SPAskReplace returns False, then you should always call SPInstall(True).
					}					
				}
				else		//If IIS is not installed, go to Completion Page
				{
					strURL="confirm.htm";	// no homepage, no need to replace, go directly to completion Page	
					divMain2.setAttribute("P_OWSChangeHomePage", true);  // set SPInstall argument to TRUE; If SPAskReplace returns False, then you should always call SPInstall(True).						
				}
				installNext("officeWebServer");
				document.all.L_next_Button.disabled=false;				
			}
			else	// if SharePoint is installed ==> NEXT disabled
			{
				document.all.L_next_Button.disabled=true;
				strURL="";	
			}	
			break;
			
		case 6:
			if (form.selectServer.value=="7")
			{
				if(srvWiz.IsServiceInstalled("StreamingMedia"))
				{
					document.all.L_next_Button.disabled=true;
					strURL="";
				}
				else
				{
					installNext("streamingMediaServer");
					strURL="confirm.htm"
				}
			}
			if (form.selectServer.value=="8")
			{
				if(IISInstalled)
				{
					document.all.L_next_Button.disabled=true;
					strURL="";		//document.all.L_next_Button.disabled=false;installNext("webServer");strURL="confirm.htm"
				}
				else{
					installNext("webServer");
					strURL="confirm.htm"
				}
			}
			break;
			 
		case 7: 
			if(IISInstalled)
			{
				document.all.L_next_Button.disabled=true;
				strURL="";			//document.all.L_next_Button.disabled=false;installNext("webServer");strURL="confirm.htm"
			}
			else
			{
				installNext("webServer");
				strURL="confirm.htm"
			}
			break;	
				
		default:
			// stub
			break;
	}				
}
	
function enable()
// --------------------------------------------------------------------------------------
// based on selection for Networking options enable NEXT button
// --------------------------------------------------------------------------------------
{	
	if(form.selectServer.options[form.selectServer.selectedIndex].value=="4")
	{		
		document.all.L_next_Button.disabled =((!(DHCP)&&(form.cbDHCP.checked))||(!(DNS)&&(form.cbDNS.checked))||(!(WINS)&&(form.cbWINS.checked))||(!(RRAS)&&(form.cbRRAS.checked)))?false:true;
		strURL=(document.all.L_next_Button.disabled)?"":"confirm.htm";
	}
}

function installNext(strServer)
// --------------------------------------------------------------------------------------
// Sets the next member service to install
// --------------------------------------------------------------------------------------
{
	divMain2.setAttribute("P_JumpIndex",OWSInstalled);	// store JumpIndex as True if OWSInstalled
	divMain2.setAttribute("P_install",strServer); 
	divMain2.save("oDataStore");
}
		
function back()
// --------------------------------------------------------------------------------------
// function when BACK button clicked, loads previous CYS step 
// --------------------------------------------------------------------------------------
{
	saveForm();
	var bolShowServerPage = (convertBol(divMain2.getAttribute("P_ShowServerPage")));
	self.location.href = (bolShowServerPage)? "server.htm" : "welcome.htm";	
}	
	
function next()
// --------------------------------------------------------------------------------------
// function when NEXT button clicked, displays next CYS step 
// --------------------------------------------------------------------------------------
{		
	saveForm();
	if(strURL!="") self.location.href=strURL;	
}

function saveForm()
// --------------------------------------------------------------------------------------
// process before exiting the page; saves the selections made in oDataStore 
// --------------------------------------------------------------------------------------
{
	var choice = form.selectServer.selectedIndex;
	divMain2.setAttribute("P_select",form.selectServer.options[choice].value);
	divMain2.setAttribute("P_selectDHCP",((form.cbDHCP.checked)&&(!form.cbDHCP.disabled)));
	divMain2.setAttribute("P_selectDNS",((form.cbDNS.checked)&&(!form.cbDNS.disabled)));
	divMain2.setAttribute("P_selectWINS",((form.cbWINS.checked)&&(!form.cbWINS.disabled)));
	divMain2.setAttribute("P_selectRRAS",((form.cbRRAS.checked)&&(!form.cbRRAS.disabled)));
	divMain2.save("oDataStore");
}

function foolit()
// --------------------------------------------------------------------------------------
// used on BODY ONMOUSEUP for out of checkbox return to white (197872)
// --------------------------------------------------------------------------------------
{	spanFoolIt.innerHTML = "";	 }	

