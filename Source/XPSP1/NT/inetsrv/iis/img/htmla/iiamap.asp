<%@ LANGUAGE = VBScript %>
<% Option Explicit %>
<!-- #include file="directives.inc" -->

<% if Session("FONTSIZE") = "" then %>
	<!--#include file="iito.inc"-->
<% else %>
	<!--#include file="iiamap.str"-->
<HTML>
<HEAD>
	<TITLE></TITLE>
		<SCRIPT LANGUAGE="JavaScript">

		function loadHead(){
			self.head.location.href = "iiamapmn.asp";		
		}
	</SCRIPT>
</HEAD>
	<FRAMESET ROWS="<%= L_AMAPDLGHEAD_NUM %>,<%= L_AMAPTABLEHEAD_NUM %>,*,<%= L_AMAPTABLEBORDER_NUM %>" BORDER=0 FRAMEBORDER=0 FRAMESPACING=0  onLoad="loadHead();">	
		<FRAME NAME="head" SRC="blank.htm" SCROLLING="no">
		<FRAMESET COLS="<%= L_AMAPTABLEBORDER_NUM %>,<%= L_AMAPTABLEWIDTH_NUM %>,*" BORDER=0 FRAMESPACING=0 FRAMEBORDER=0>				
			<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
			<FRAME NAME="colhds" SRC="iiamaphd.asp" SCROLLING="no" MARGINHEIGHT=0 MARGINWIDTH=0 BORDER=0 FRAMEBORDER=0>
			<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
		</FRAMESET>
		<FRAMESET COLS="<%= L_AMAPTABLEBORDER_NUM %>,<%= L_AMAPTABLEWIDTH_NUM %>,*" BORDER=0 FRAMESPACING=0 FRAMEBORDER=0>				
			<FRAME NAME="pad1" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
			<FRAME NAME="list" SRC="blank.htm" SCROLLING="AUTO" MARGINHEIGHT=0 MARGINWIDTH=0 BORDER=1 FRAMEBORDER=1>
			<FRAME NAME="buttons" SRC="iilist.asp" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>
		</FRAMESET>
		<FRAME NAME="pad3" SRC="blank.htm" SCROLLING="no" MARGINHEIGHT=5 MARGINWIDTH=5 FRAMEBORDER=0>	
	</FRAMESET>
</HTML>

<% end if %>