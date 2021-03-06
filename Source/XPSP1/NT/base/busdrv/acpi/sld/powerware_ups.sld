<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE DCARRIER SYSTEM "Mantis.DTD">

  <DCARRIER
    CarrierRevision="1"
    DTDRevision="16"
  >
    <TASKS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </TASKS>

    <PLATFORMS
      Context="1"
    >    </PLATFORMS>

    <REPOSITORIES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </REPOSITORIES>

    <GROUPS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </GROUPS>

    <COMPONENTS
      Context="0"
      PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
    >
      <COMPONENT
        ComponentVSGUID="{13DD8D78-215D-4C5D-B25F-5B6BBD472073}"
        ComponentVIGUID="{DBD9182F-0822-497F-98C0-858746B51154}"
        Revision="620"
        PrototypeVIGUID="{E65EA663-D0C1-4B65-A3C5-369FB269EB1C}"
        RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}"
        Visibility="1000"
        MultiInstance="False"
        Released="False"
        Editable="True"
        HTMLFinal="False"
        IsMacro="False"
        Opaque="False"
        Context="1"
        PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
      >
        <HELPCONTEXT
          src=".\powerwareups.htm"
        ><![CDATA[<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
<HTML DIR="LTR"><HEAD>
<META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252">
<TITLE>Powerware UPS</TITLE>
<STYLE TYPE="text/css">
<!--
BODY         { font: normal  8pt Tahoma; background-color: #FFFFFF; }
P            { font: normal  8pt Tahoma; }
.showhide    { color: blue; text-decoration: underline; cursor: hand; }
.callout     { font: normal  8pt Tahoma; background-color: #E0E0E0; padding: 10pt; }
.code        { font: normal  8pt Courier New; }
UL           { font: normal  8pt Tahoma; list-style: square outside; margin-left: 0.25in; }
OL           { font: normal  8pt Tahoma; list-style: decimal outside; margin-left: 0.25in; }
H1           { font: bold   12pt Tahoma; margin-bottom: -12pt; }
H2           { font: bold   10pt Tahoma; margin-bottom: -12pt; }
H3           { font: bold    8pt Tahoma; margin-bottom: -12pt; }
H4           { font: italic  8pt Tahoma; margin-bottom: -12pt; }
TABLE        { font: normal  8pt Tahoma; text-align: left; padding: 2px; }
CAPTION      { font: bold    8pt Tahoma; text-align: left; padding: 2px; }
THEAD        { font: bold    8pt Tahoma; text-align: left; padding: 2px; background-color: #F0F0F0 }
TH           { font: bold    8pt Tahoma; text-align: left; padding: 2px; background-color: #F0F0F0 }
TBODY        { font: normal  8pt Tahoma; text-align: left; padding: 2px; }
TD           { font: normal  8pt Tahoma; text-align: left; padding: 2px; }
TR           { font: normal  8pt Tahoma; text-align: left; padding: 2px; }
CODE         { font-family: Courier New; }
TT           { font-family: Courier New; }
KBD          { font-family: Courier New; font-weight: bold; }
VAR          { font-family: Courier New; font-style: italic; }
EM           { font-style: italic; }
I            { font-style: italic; }
STRONG       { font-weight: bold; }
B            { font-weight: bold; }
-->
</STYLE></HEAD>
<BODY TOPMARGIN="0">
<H1><A NAME="powerwareups"></A>Powerware UPS</H1>

<P>The Powerware Uninterruptible Power Supply (UPS) component monitors and configures the proprietary UPS mechanism provided by Powerware.</P>

<P>A UPS provides constant power to a computer system when a power fluctuation or power loss occurs. Built-in electronics constantly monitor line voltages: if the line voltage fluctuates above or below predefined limits, or fails entirely, the UPS supplies power to the computer system from built-in batteries. The UPS converts the direct current (DC) battery voltage into the alternating current (AC) voltage required by the computer system. The change to batteries must take place very rapidly to prevent data loss.</P>

<H2>Component Configuration</H2>

<P>There are no configuration requirements for this component. </P>

<H2>For More Information </H2>

<P>For more information, see the Windows 2000 Resource Kit at this <A HREF="http://www.microsoft.com">Microsoft Web site</A>.</P>

</BODY>
</HTML>
]]></HELPCONTEXT>

        <PROPERTIES
          Context="1"
          PlatformGUID="{00000000-0000-0000-0000-000000000000}"
        >
          <PROPERTY
            Name="cmiPnPDevId"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >SERENUM\DEL0001</PROPERTY>

          <PROPERTY
            Name="cmiPnPDevInf"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >battery.inf</PROPERTY>

          <PROPERTY
            Name="cmiPnPDevClassGUID"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >{72631e54-78a4-11d0-bcf7-00aa00b7b32a}</PROPERTY>

          <PROPERTY
            Name="cmiIsCriticalDevice"
            Format="Boolean"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >False</PROPERTY>
        </PROPERTIES>

        <RESOURCES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\DEL0001&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\DEL0001</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%17%&quot;,&quot;battery.inf&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%17%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >battery.inf</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\EXI0001&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\EXI0001</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\FPS0001&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\FPS0001</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\APC0000&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\APC0000</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\APC0001&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\APC0001</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\APC0002&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\APC0002</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;SERENUM\APC0003&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SERENUM\APC0003</PROPERTY>

              <PROPERTY
                Name="cmiIsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>
        </RESOURCES>

        <GROUPMEMBERS
        >
          <GROUPMEMBER
            GroupVSGUID="{DE577689-9566-11D4-8E84-00B0D03D27C6}"
          ></GROUPMEMBER>
        </GROUPMEMBERS>

        <DEPENDENCIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{7573FB43-D959-47B2-B7C8-0D847CF57104}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >            </PROPERTIES>
          </DEPENDENCY>
        </DEPENDENCIES>

        <DISPLAYNAME>PowerWare UPS</DISPLAYNAME>

        <VERSION>5.1.2517.0</VERSION>

        <DESCRIPTION>PowerWare UPS</DESCRIPTION>

        <VENDOR>Microsoft</VENDOR>

        <OWNERS>matth</OWNERS>

        <AUTHORS>matth</AUTHORS>

        <DATECREATED>7/16/2001</DATECREATED>

        <DATEREVISED>9/19/2001 8:27:50 PM</DATEREVISED>
      </COMPONENT>
    </COMPONENTS>

    <RESTYPES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </RESTYPES>
  </DCARRIER>
