# Microsoft Developer Studio Project File - Name="makefile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=makefile - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "makefile.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "makefile.mak" CFG="makefile - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "makefile - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "makefile - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "makefile - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "%TOOLS%\BIN\SMSBUILD /f makefile NO_OPTIM=1"
# PROP Rebuild_Opt "clean"
# PROP Target_File "viewprov.dll"
# PROP Bsc_Name "makefile.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "makefile - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "%TOOLS%\BIN\SMSBUILD /f makefile NO_OPTIM=1"
# PROP Rebuild_Opt "clean"
# PROP Target_File "viewprov.dll"
# PROP Bsc_Name "makefile.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "makefile - Win32 Release"
# Name "makefile - Win32 Debug"

!IF  "$(CFG)" == "makefile - Win32 Release"

!ELSEIF  "$(CFG)" == "makefile - Win32 Debug"

!ENDIF 

# Begin Group "build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ansi.mk
# End Source File
# Begin Source File

SOURCE=.\makefile
# End Source File
# Begin Source File

SOURCE=.\master.mk
# End Source File
# Begin Source File

SOURCE=.\unicode.mk
# End Source File
# Begin Source File

SOURCE=.\viewprov.def
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\maindll.cpp
# End Source File
# Begin Source File

SOURCE=.\vp_core_qengine.cpp
# End Source File
# Begin Source File

SOURCE=.\vpcfac.cpp
# End Source File
# Begin Source File

SOURCE=.\vpget.cpp
# End Source File
# Begin Source File

SOURCE=.\vpmthd.cpp
# End Source File
# Begin Source File

SOURCE=.\vpput.cpp
# End Source File
# Begin Source File

SOURCE=.\vpquals.cpp
# End Source File
# Begin Source File

SOURCE=.\vpquery.cpp
# End Source File
# Begin Source File

SOURCE=.\vpserv.cpp
# End Source File
# Begin Source File

SOURCE=.\vpsinks.cpp
# End Source File
# Begin Source File

SOURCE=.\vptasks.cpp
# End Source File
# Begin Source File

SOURCE=.\vptasksh.cpp
# End Source File
# Begin Source File

SOURCE=.\vptasksj.cpp
# End Source File
# Begin Source File

SOURCE=.\vptasksu.cpp
# End Source File
# End Group
# Begin Group "Header"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\include\vpcfac.h
# End Source File
# Begin Source File

SOURCE=.\include\vpdefs.h
# End Source File
# Begin Source File

SOURCE=.\include\vpquals.h
# End Source File
# Begin Source File

SOURCE=.\include\vpserv.h
# End Source File
# Begin Source File

SOURCE=.\include\vptasks.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\viewtest.mof
# End Source File
# End Target
# End Project
