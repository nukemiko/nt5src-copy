<%@ LANGUAGE = JScript %>

<!*************************
This sample is provided for educational purposes only. It is not intended to be 
used in a production environment, has not been tested in a production environment, 
and Microsoft will not provide technical support for it. 
*************************>


<HTML>
    <HEAD>
        <TITLE>Tools Component</TITLE>
    </HEAD>

    <BODY BGCOLOR="White" TOPMARGIN="10" LEFTMARGIN="10">


        <!-- Display header. -->

        <FONT SIZE="4" FACE="ARIAL, HELVETICA">
        <B>Tools Component</B></FONT><BR>
      
        <HR SIZE="1" COLOR="#000000">

		<%
			var objTools;
			var FoundFile, blnFileExists, blnFileDoesNotExist, NotFoundFile;
			var intRand, intRandPos, intRandBelow;

			//Create the Tools Object
			objTools = Server.CreateObject("MSWC.Tools");
		%>
	

		<H3>FileExists Example</H3>
	
		<%
			FoundFile = "Tools_VBScript.asp";
			NotFoundFile = "blah.asp";

	
			//Check if file exists.
	
			blnFileExists = objTools.FileExists(FoundFile);


			//Output response appropriately.

			if (blnFileExists)
			{ 
				Response.Write("The File " + FoundFile + " Exists<BR>");
			}
			else
			{ 
				Response.Write("The File " + FoundFile + " Does Not Exist<BR>");
			}


			//Check if file exists.
			
			blnFileDoesNotExist = objTools.FileExists(NotFoundFile);


			//Output response appropriately.
		
			if (blnFileDoesNotExist)
			{ 
				Response.Write("The File " + NotFoundFile + " Exists");
			}
			else
			{ 
				Response.Write("The File " + NotFoundFile + " Does Not Exist");
			}
		%>
	

		<H3>Random Integer Example</H3>
	
		<%
			//Random integer.
	
			intRand = objTools.Random();
			Response.Write("Random integer: ");
			Response.Write(intRand);
			Response.Write("<BR>");

	
			//Positive random integer.
			
			intRandPos = Math.abs( objTools.Random() );
			Response.Write("Positive random integer: ");
			Response.Write(intRandPos);
			Response.Write("<BR>");

			
			//Positive random integer between 0 and 100.
			
			intRandBelow =  Math.abs( objTools.Random() ) % 100;
			Response.Write("Random integer below 100: ");
			Response.Write(intRandBelow);
			Response.Write("<BR>");
		%>
    </BODY>
</HTML>
