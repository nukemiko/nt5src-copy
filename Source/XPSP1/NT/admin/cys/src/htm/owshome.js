
function loadForm()
// --------------------------------------------------------------------------------------
// function performed at page load; load the values stored in oDataStore
// --------------------------------------------------------------------------------------
{
	divMain2.load("oDataStore");
	form.rd_OWSChangeHomePageYes.checked=(divMain2.getAttribute("P_OWSChangeHomePage")==null)?true:convertBol(divMain2.getAttribute("P_OWSChangeHomePage"));
	form.rd_OWSChangeHomePageNo.checked=(form.rd_OWSChangeHomePageYes.checked)?false:true;
	loadFocus();
	
	var srvOWS	= new ActiveXObject("SpCyscom.SpCys");
	var OWSHomePage = srvOWS.SPNonDefaultHomePage();
  
    p3_1.innerHTML = OWSHomePage.replace(/SharePoint.htm/g, "");	//  remove "SharePoint.htm" from the returned url string for the response "yes, replace my homepage." 
    p3_2.innerHTML = OWSHomePage; 
}
	
function back()
// --------------------------------------------------------------------------------------
// process when click on BACK button (persist data and load previous member.htm page)
// --------------------------------------------------------------------------------------
{	saveForm(); self.location.href="member.htm" }
	
function next()
// --------------------------------------------------------------------------------------
// process when NEXT button clicked ; save entries made and proceed to next page (confirm.htm) 
// --------------------------------------------------------------------------------------
{	saveForm(); self.location.href="confirm.htm" }
	
function saveForm()
// --------------------------------------------------------------------------------------
// process before exiting the page; saves the selection and entries made
// --------------------------------------------------------------------------------------
{
	divMain2.setAttribute("P_OWSChangeHomePage",form.rd_OWSChangeHomePageYes.checked);
	divMain2.save("oDataStore");
}
	
function checkLabel(pos)
// --------------------------------------------------------------------------------------
// function used at OnClick;
// --------------------------------------------------------------------------------------
{	
	if (pos==1) {form.rd_OWSChangeHomePageYes.checked=true; form.rd_OWSChangeHomePageNo.checked=false;} 
	if (pos==2) {form.rd_OWSChangeHomePageYes.checked=false; form.rd_OWSChangeHomePageNo.checked=true;}
}

