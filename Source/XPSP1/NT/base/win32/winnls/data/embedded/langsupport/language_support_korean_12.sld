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
    >
      <GROUP
        GroupVSGUID="{c7473b19-d4b6-4930-950a-ff90e25fc7d7}"
        Revision="721"
        GroupClass="Dependency"
        DefaultDependencyType="AtLeastOne"
        Context="1"
        PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
      >
        <PROPERTIES
          Context="1"
          PlatformGUID="{00000000-0000-0000-0000-000000000000}"
        >        </PROPERTIES>

        <SECONDARYPLATFORMS
        >        </SECONDARYPLATFORMS>

        <DISPLAYNAME>Korean Language Extended Support</DISPLAYNAME>

        <VERSION>5.1.2600</VERSION>

        <DESCRIPTION>This dependency group contains any additional NLS components that support Korean Language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:59 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:59 PM</DATEREVISED>
      </GROUP>

      <GROUP
        GroupVSGUID="{d360ec72-d76a-4e97-862e-4b69a04ba99c}"
        Revision="721"
        GroupClass="Dependency"
        DefaultDependencyType="All"
        Context="1"
        PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
      >
        <PROPERTIES
          Context="1"
          PlatformGUID="{00000000-0000-0000-0000-000000000000}"
        >        </PROPERTIES>

        <SECONDARYPLATFORMS
        >        </SECONDARYPLATFORMS>

        <DISPLAYNAME>Korean Locales Group</DISPLAYNAME>

        <VERSION>5.1.2600</VERSION>

        <DESCRIPTION>This dependency group contains user and system locale components that support Korean Language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:59 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:59 PM</DATEREVISED>
      </GROUP>
    </GROUPS>

    <COMPONENTS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >
      <COMPONENT
        ComponentVSGUID="{E578DCF2-DA1C-40D2-A01E-2ADB6BB9101D}"
        ComponentVIGUID="{DB90B2CB-5D01-44CC-8946-BE3DEEA3C49E}"
        Revision="721"
        PrototypeVIGUID="{87899248-F1AA-4037-99A7-81B6DD1AFEB4}"
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
            Name="cmiLangEnableAllSystemLocales"
            Format="Boolean"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >True</PROPERTY>

          <PROPERTY
            Name="cmiLangExtendedSupportShared"
            Format="GUID"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >{B463E505-2792-45FA-9EF0-02D364AC234F}</PROPERTY>

          <PROPERTY
            Name="cmiLangExtendedSupport"
            Format="GUID"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >{C7473B19-D4B6-4930-950A-FF90E25FC7D7}</PROPERTY>

          <PROPERTY
            Name="cmiLangName"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >Korean</PROPERTY>

          <PROPERTY
            Name="cmiLangId"
            Format="Integer"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >18</PROPERTY>
        </PROPERTIES>

        <RESOURCES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >        </RESOURCES>

        <GROUPMEMBERS
        >
          <GROUPMEMBER
            GroupVSGUID="{8180C915-788C-473C-80BB-BB6DF8FFA263}"
          ></GROUPMEMBER>

          <GROUPMEMBER
            GroupVSGUID="{A1A56917-46F2-492E-A544-C4EB1B95F61E}"
          ></GROUPMEMBER>

          <GROUPMEMBER
            GroupVSGUID="{719FF4F5-55C2-4A87-A744-19E6C9B3089F}"
          ></GROUPMEMBER>
        </GROUPMEMBERS>

        <DEPENDENCIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{6EB8E22A-9947-4F62-9F5F-9E2A5B89192E}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangRequired"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >True</PROPERTY>

              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>949   (ANSI/OEM - Korean)</DISPLAYNAME>

            <DESCRIPTION>949   (ANSI/OEM - Korean)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{37C6394E-D1C5-46A4-B56B-A6BC8CF30EE4}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>10003 (MAC - Korean)</DISPLAYNAME>

            <DESCRIPTION>10003 (MAC - Korean)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{AA881DB1-8933-473C-8C14-EB8145EFB876}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>1361  (Korean - Johab)</DISPLAYNAME>

            <DESCRIPTION>1361  (Korean - Johab)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{CB875F36-0731-4DAC-BC80-79EA4CB4834A}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>20833 (IBM EBCDIC - Korean Extended)</DISPLAYNAME>

            <DESCRIPTION>20833 (IBM EBCDIC - Korean Extended)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{2D3671F0-77C0-4138-A430-A8E5B9B6749A}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>20949 (Korean Wansung)</DISPLAYNAME>

            <DESCRIPTION>20949 (Korean Wansung)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{736CE8DC-D853-498F-8858-F6D14651FF50}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>50225 (ISO-2022 Korean)</DISPLAYNAME>

            <DESCRIPTION>50225 (ISO-2022 Korean)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{C0D886B0-FC03-4D15-978D-7EF6615D4E76}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>51949 (EUC - Korean)</DISPLAYNAME>

            <DESCRIPTION>51949 (EUC - Korean)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{BA8CEA9C-DF18-4E70-8FCD-ABB54284D68D}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="cmiLangRequired"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >True</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean bitmap &amp; TrueType fonts</DISPLAYNAME>

            <DESCRIPTION>Korean bitmap &amp; TrueType fonts</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{8F917DBC-51E1-401A-8178-A01BB6C0394F}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean</DISPLAYNAME>

            <DESCRIPTION>Korean</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{A3A82FF1-E20D-42EE-93E5-7090A510F52C}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >6</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>User Locale: 0412</DISPLAYNAME>

            <DESCRIPTION>User Locale: 0412</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{0241B46B-16C2-4EFD-99FF-0CA9ACBB4528}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangLocaleId"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1042</PROPERTY>

              <PROPERTY
                Name="cmiLangRequired"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >True</PROPERTY>

              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >7</PROPERTY>

              <PROPERTY
                Name="cmiLangInputProp"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >cmiLangEnableAllSystemLocales</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean - 0412</DISPLAYNAME>

            <DESCRIPTION>Korean - 0412</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{0B32CECF-C01E-4F57-AD15-B5C2C15D6A16}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >5</PROPERTY>

              <PROPERTY
                Name="cmiLangDependOnSystemLocale"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >True</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Font Substitute dependency: Fonts_Korean_font_substitutes</DISPLAYNAME>

            <DESCRIPTION>This component dependends on font substitute Fonts_Korean_font_substitutes</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{D360EC72-D76A-4E97-862E-4B69A04BA99C}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >            </PROPERTIES>

            <DISPLAYNAME>Dependency on user and system locale components</DISPLAYNAME>

            <DESCRIPTION>Dependency on user and system locale components</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{0933204C-D4B9-4358-85F3-4FFD42396F4F}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean 101a</DISPLAYNAME>

            <DESCRIPTION>Korean 101a</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{4298018D-3F49-46F8-9745-54D40783113B}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean 101b</DISPLAYNAME>

            <DESCRIPTION>Korean 101b</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{60F18432-B01C-4274-9DDE-9AB3B69ADF66}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean 101c</DISPLAYNAME>

            <DESCRIPTION>Korean 101c</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{06F5D929-85A9-4E8D-9C28-05DF3C910C28}"
            MinRevision="0"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="cmiLangModuleType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>
            </PROPERTIES>

            <DISPLAYNAME>Korean 103</DISPLAYNAME>

            <DESCRIPTION>Korean 103</DESCRIPTION>
          </DEPENDENCY>
        </DEPENDENCIES>

        <SECONDARYPLATFORMS
        >        </SECONDARYPLATFORMS>

        <DISPLAYNAME>Korean Language Support</DISPLAYNAME>

        <VERSION>5.1.2600.1</VERSION>

        <DESCRIPTION>This component contains all necessary modules to support Korean language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:59 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:59 PM</DATEREVISED>
      </COMPONENT>
    </COMPONENTS>

    <RESTYPES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </RESTYPES>
  </DCARRIER>
