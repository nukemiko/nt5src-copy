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
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >
      <COMPONENT
        ComponentVSGUID="{ADE763E7-924A-4F8B-9FA6-617600631DB8}"
        ComponentVIGUID="{160BC66F-DD4A-4295-8B1D-33E316762F87}"
        Revision="8"
        PrototypeVIGUID="{9496CB5B-EE1D-438D-8CDC-A82006067E0B}"
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
        <PROPERTIES
          Context="1"
          PlatformGUID="{00000000-0000-0000-0000-000000000000}"
        >
          <PROPERTY
            Name="cmiPnPDevDriverVer"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >07/01/2001,5.1.2535.0</PROPERTY>

          <PROPERTY
            Name="cmiPnPDevId"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >USB\VID_045E&amp;PID_006D</PROPERTY>

          <PROPERTY
            Name="cmiPnPDevInf"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >irbus.inf</PROPERTY>

          <PROPERTY
            Name="cmiPnPDevClassGUID"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >{36FC9E60-C465-11CF-8056-444553540000}</PROPERTY>

          <PROPERTY
            Name="cmiIsCriticalDevice"
            Format="Boolean"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >False</PROPERTY>

          <PROPERTY
            Name="cmiPnPDevSection"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >IrBus</PROPERTY>

          <PROPERTY
            Name="cmiConcordanceID"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >eHome Infrared Receiver:USB\VID_045E&amp;PID_006D</PROPERTY>
        </PROPERTIES>

        <RESOURCES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <RESOURCE
            Name="PnPID(819):&quot;USB\VID_045E&amp;PID_006D&quot;"
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
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{ADE763E7-924A-4F8B-9FA6-617600631DB8}</PROPERTY>

              <PROPERTY
                Name="IdOriginal"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >True</PROPERTY>

              <PROPERTY
                Name="IsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="LowerFilter"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >USB\VID_045E&amp;PID_006D</PROPERTY>

              <PROPERTY
                Name="ServiceName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >IrBus</PROPERTY>

              <PROPERTY
                Name="UpperFilter"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>PnPID(819):&quot;USB\VID_045E&amp;PID_006D&quot;</DISPLAYNAME>

            <DESCRIPTION>PnPID(819):&quot;USB\VID_045E&amp;PID_006D&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%17%&quot;,&quot;irbus.inf&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1001"
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
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{ADE763E7-924A-4F8B-9FA6-617600631DB8}</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >irbus.inf</PROPERTY>

              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%17%</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="Overwrite"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="SrcFileCRC"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="SrcFileSize"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="SrcName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >irbus.inf</PROPERTY>

              <PROPERTY
                Name="SrcPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>File(819):&quot;%17%&quot;,&quot;irbus.inf&quot;</DISPLAYNAME>

            <DESCRIPTION>File(819):&quot;%17%&quot;,&quot;irbus.inf&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%12%&quot;,&quot;IrBus.sys&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1002"
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
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{ADE763E7-924A-4F8B-9FA6-617600631DB8}</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >IrBus.sys</PROPERTY>

              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%12%</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="Overwrite"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="SrcFileCRC"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="SrcFileSize"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="SrcName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >IrBus.sys</PROPERTY>

              <PROPERTY
                Name="SrcPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>File(819):&quot;%12%&quot;,&quot;IrBus.sys&quot;</DISPLAYNAME>

            <DESCRIPTION>File(819):&quot;%12%&quot;,&quot;IrBus.sys&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="Service(819):&quot;IrBus&quot;"
            ResTypeVSGUID="{5C16ED57-3182-4411-8EA7-AC1CE70B96DA}"
            BuildTypeMask="819"
            BuildOrder="1003"
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
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{ADE763E7-924A-4F8B-9FA6-617600631DB8}</PROPERTY>

              <PROPERTY
                Name="Dependencies"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="ErrorControl"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="LoadOrderGroup"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >PNP Filter</PROPERTY>

              <PROPERTY
                Name="Password"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="ServiceBinary"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%12%\IrBus.sys</PROPERTY>

              <PROPERTY
                Name="ServiceDescription"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="ServiceDisplayName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Infrared bus filter driver for eHome remote controls</PROPERTY>

              <PROPERTY
                Name="ServiceName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >IrBus</PROPERTY>

              <PROPERTY
                Name="ServiceType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="StartName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="StartType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Infrared bus filter driver for eHome remote controls</DISPLAYNAME>
          </RESOURCE>

          <RESOURCE
            Name="PnPID(819):&quot;USB\VID_0403&amp;PID_6001&quot;"
            ResTypeVSGUID="{AFC59066-28EA-4279-979B-955C9E8DE82A}"
            BuildTypeMask="819"
            BuildOrder="1004"
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
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{ADE763E7-924A-4F8B-9FA6-617600631DB8}</PROPERTY>

              <PROPERTY
                Name="IdOriginal"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >True</PROPERTY>

              <PROPERTY
                Name="IsCompatibleID"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="LowerFilter"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="PnPID"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >USB\VID_0403&amp;PID_6001</PROPERTY>

              <PROPERTY
                Name="ServiceName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >IrBus</PROPERTY>

              <PROPERTY
                Name="UpperFilter"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>PnPID(819):&quot;USB\VID_0403&amp;PID_6001&quot;</DISPLAYNAME>

            <DESCRIPTION>PnPID(819):&quot;USB\VID_0403&amp;PID_6001&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%12%&quot;,&quot;hidir.sys&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1001"
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
                Name="SrcPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              ></PROPERTY>

              <PROPERTY
                Name="SrcName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >hidir.sys</PROPERTY>

              <PROPERTY
                Name="SrcFileSize"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="SrcFileCRC"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="Overwrite"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>

              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%12%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >hidir.sys</PROPERTY>

              <PROPERTY
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{607DA25F-1C6B-4285-9D16-8F26D2BE5F8B}</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>File(819):&quot;%12%&quot;,&quot;hidir.sys&quot;</DISPLAYNAME>

            <DESCRIPTION>File(819):&quot;%12%&quot;,&quot;hidir.sys&quot;</DESCRIPTION>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;hidusb.sys&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
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
                Name="ComponentVSGUID"
                Format="GUID"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >{00000000-0000-0000-0000-000000000000}</PROPERTY>

              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >hidusb.sys</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Dependency upon &apos;hidusb.sys&apos;</DISPLAYNAME>

            <DESCRIPTION></DESCRIPTION>
          </RESOURCE>
        </RESOURCES>

        <GROUPMEMBERS
        >
          <GROUPMEMBER
            GroupVSGUID="{DE577668-9566-11D4-8E84-00B0D03D27C6}"
          ></GROUPMEMBER>

          <GROUPMEMBER
            GroupVSGUID="{E01B4103-3883-4FE8-992F-10566E7B796C}"
          ></GROUPMEMBER>
        </GROUPMEMBERS>

        <DEPENDENCIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >        </DEPENDENCIES>

        <DISPLAYNAME>eHome Infrared Receiver</DISPLAYNAME>

        <VERSION>1.0</VERSION>

        <DESCRIPTION>eHome Infrared Receiver</DESCRIPTION>

        <COPYRIGHT>2002 Microsoft Corp.</COPYRIGHT>

        <VENDOR>Microsoft Corp.</VENDOR>

        <OWNERS>SachaD</OWNERS>

        <AUTHORS>sachad</AUTHORS>

        <DATECREATED>4/25/2002</DATECREATED>

        <DATEREVISED>5/8/2002 11:04:53 PM</DATEREVISED>
      </COMPONENT>
    </COMPONENTS>

    <RESTYPES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </RESTYPES>
  </DCARRIER>
