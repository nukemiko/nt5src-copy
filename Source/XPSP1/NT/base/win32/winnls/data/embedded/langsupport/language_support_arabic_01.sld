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
        GroupVSGUID="{f8872884-0b0a-4664-ad51-5275fadec241}"
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

        <DISPLAYNAME>Arabic Language Extended Support</DISPLAYNAME>

        <VERSION>5.1.2600</VERSION>

        <DESCRIPTION>This dependency group contains any additional NLS components that support Arabic Language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:57 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:57 PM</DATEREVISED>
      </GROUP>

      <GROUP
        GroupVSGUID="{2cf82785-b9aa-4e3b-a6cb-41594ed2a486}"
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

        <DISPLAYNAME>Arabic Locales Group</DISPLAYNAME>

        <VERSION>5.1.2600</VERSION>

        <DESCRIPTION>This dependency group contains user and system locale components that support Arabic Language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:57 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:57 PM</DATEREVISED>
      </GROUP>
    </GROUPS>

    <COMPONENTS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >
      <COMPONENT
        ComponentVSGUID="{86253199-F1E5-45CB-8C58-FCCC73EDF648}"
        ComponentVIGUID="{0EBAB71B-6EE0-4BCF-A6A3-F3793A46A0C9}"
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
          >{F8872884-0B0A-4664-AD51-5275FADEC241}</PROPERTY>

          <PROPERTY
            Name="cmiLangName"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >Arabic</PROPERTY>

          <PROPERTY
            Name="cmiLangId"
            Format="Integer"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >1</PROPERTY>
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
            DependOnGUID="{95E8EF1E-07A2-48E8-8EEF-91E1F4C7A928}"
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

            <DISPLAYNAME>1256  (ANSI - Arabic)</DISPLAYNAME>

            <DESCRIPTION>1256  (ANSI - Arabic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{6FD76E98-19CD-4A22-891B-13496FAD316E}"
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

            <DISPLAYNAME>10004 (MAC - Arabic)</DISPLAYNAME>

            <DESCRIPTION>10004 (MAC - Arabic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{5E723315-B19B-44FF-8737-6A42C8E28DF1}"
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

            <DISPLAYNAME>20420 (IBM EBCDIC - Arabic)</DISPLAYNAME>

            <DESCRIPTION>20420 (IBM EBCDIC - Arabic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{12DFD29D-D907-434D-BE20-4373411BA16D}"
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

            <DISPLAYNAME>28596 (ISO 8859-6 Arabic)</DISPLAYNAME>

            <DESCRIPTION>28596 (ISO 8859-6 Arabic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{C9A53F0F-F8B6-468A-83D4-18F9D9B2C114}"
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

            <DISPLAYNAME>708   (Arabic - ASMO)</DISPLAYNAME>

            <DESCRIPTION>708   (Arabic - ASMO)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{B7BF9614-DF22-4CCD-8C35-D98415AF0C17}"
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

            <DISPLAYNAME>720   (Arabic - Transparent ASMO)</DISPLAYNAME>

            <DESCRIPTION>720   (Arabic - Transparent ASMO)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{2AEC383D-D038-47A5-A304-40455051E0EA}"
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

            <DISPLAYNAME>Arabic bitmap &amp; TrueType fonts</DISPLAYNAME>

            <DESCRIPTION>Arabic bitmap &amp; TrueType fonts</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{7642041C-1DED-4497-8783-A99A204057BE}"
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

            <DISPLAYNAME>Arabic (101)</DISPLAYNAME>

            <DESCRIPTION>Arabic (101)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{83C7ECB4-60A5-4572-AC67-E27FC3B41B89}"
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

            <DISPLAYNAME>Arabic (102 AZERTY)</DISPLAYNAME>

            <DESCRIPTION>Arabic (102 AZERTY)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{ABF247A6-44F9-40E8-9995-C94B50E6B93F}"
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

            <DISPLAYNAME>Arabic (102)</DISPLAYNAME>

            <DESCRIPTION>Arabic (102)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{DD4A78DB-9EA1-42EC-A18B-AFA114770B8D}"
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

            <DISPLAYNAME>US English Table for IBM Arabic 238_L</DISPLAYNAME>

            <DESCRIPTION>US English Table for IBM Arabic 238_L</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{1A04E8F7-DEEE-4B51-8FB2-508E7B858374}"
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

            <DISPLAYNAME>User Locale: 0401</DISPLAYNAME>

            <DESCRIPTION>User Locale: 0401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{BDD83924-6106-47F7-86F0-77E804FFEF2E}"
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

            <DISPLAYNAME>User Locale: 1401</DISPLAYNAME>

            <DESCRIPTION>User Locale: 1401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{586B0425-F3F0-454C-ADBC-8CEB6D0895EA}"
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

            <DISPLAYNAME>User Locale: 3c01</DISPLAYNAME>

            <DESCRIPTION>User Locale: 3c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{BA1CFFFF-62CD-4C03-8B63-6DFC59CC03DD}"
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

            <DISPLAYNAME>User Locale: 0c01</DISPLAYNAME>

            <DESCRIPTION>User Locale: 0c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{83F30DAA-05AF-40AB-9F73-3493E9DF71DA}"
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

            <DISPLAYNAME>User Locale: 0801</DISPLAYNAME>

            <DESCRIPTION>User Locale: 0801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{1A69439E-ADC8-458C-ABD7-0D392B0FB869}"
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

            <DISPLAYNAME>User Locale: 2c01</DISPLAYNAME>

            <DESCRIPTION>User Locale: 2c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{D8ABD35C-8F8F-4D0F-97EC-449C1C4E892F}"
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

            <DISPLAYNAME>User Locale: 3401</DISPLAYNAME>

            <DESCRIPTION>User Locale: 3401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{D64B3664-BB63-4948-AD8E-14AE76B38D3B}"
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

            <DISPLAYNAME>User Locale: 3001</DISPLAYNAME>

            <DESCRIPTION>User Locale: 3001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{ECBDE577-0FB0-444E-8192-00E1B437738D}"
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

            <DISPLAYNAME>User Locale: 1001</DISPLAYNAME>

            <DESCRIPTION>User Locale: 1001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{D2E2CB09-A989-49E8-80EE-190A0236C1F2}"
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

            <DISPLAYNAME>User Locale: 1801</DISPLAYNAME>

            <DESCRIPTION>User Locale: 1801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{D322B53A-979A-4ACC-BD36-BC4C09FC2E6B}"
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

            <DISPLAYNAME>User Locale: 2001</DISPLAYNAME>

            <DESCRIPTION>User Locale: 2001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{B14CFCCF-2FD0-4142-B8FA-0400BDE260E7}"
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

            <DISPLAYNAME>User Locale: 4001</DISPLAYNAME>

            <DESCRIPTION>User Locale: 4001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{5025A053-CD6B-44DC-B18A-88706986AB19}"
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

            <DISPLAYNAME>User Locale: 2801</DISPLAYNAME>

            <DESCRIPTION>User Locale: 2801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{783CF296-1E42-48D3-9E98-248A52D33213}"
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

            <DISPLAYNAME>User Locale: 1c01</DISPLAYNAME>

            <DESCRIPTION>User Locale: 1c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{49681D8A-7A91-4188-BB69-07FF6BC48095}"
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

            <DISPLAYNAME>User Locale: 3801</DISPLAYNAME>

            <DESCRIPTION>User Locale: 3801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{3702491F-B502-49A9-B2E0-0C830F7C374A}"
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

            <DISPLAYNAME>User Locale: 2401</DISPLAYNAME>

            <DESCRIPTION>User Locale: 2401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{DBE1DCA2-8ECF-4B4E-BDFD-1E1947432B89}"
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
              >5121</PROPERTY>

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

            <DISPLAYNAME>Arabic (Algeria) - 1401</DISPLAYNAME>

            <DESCRIPTION>Arabic (Algeria) - 1401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{0DA841A1-730F-42EF-A4F2-4131F6B3A473}"
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
              >15361</PROPERTY>

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

            <DISPLAYNAME>Arabic (Bahrain) - 3c01</DISPLAYNAME>

            <DESCRIPTION>Arabic (Bahrain) - 3c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{07711587-21C7-4412-A425-2D12111670A8}"
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
              >3073</PROPERTY>

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

            <DISPLAYNAME>Arabic (Egypt) - 0c01</DISPLAYNAME>

            <DESCRIPTION>Arabic (Egypt) - 0c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{D5D0F88F-4B7A-480C-BE89-17C65AB33205}"
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
              >2049</PROPERTY>

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

            <DISPLAYNAME>Arabic (Iraq) - 0801</DISPLAYNAME>

            <DESCRIPTION>Arabic (Iraq) - 0801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{27234E18-263B-42E6-A440-4F66BEC38186}"
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
              >11265</PROPERTY>

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

            <DISPLAYNAME>Arabic (Jordan) - 2c01</DISPLAYNAME>

            <DESCRIPTION>Arabic (Jordan) - 2c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{1B47A82D-4290-4D46-843A-56C7DA16071D}"
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
              >13313</PROPERTY>

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

            <DISPLAYNAME>Arabic (Kuwait) - 3401</DISPLAYNAME>

            <DESCRIPTION>Arabic (Kuwait) - 3401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{0046B847-8D49-4B16-9937-853E19434F3A}"
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
              >12289</PROPERTY>

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

            <DISPLAYNAME>Arabic (Lebanon) - 3001</DISPLAYNAME>

            <DESCRIPTION>Arabic (Lebanon) - 3001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{1FE6B92A-45D7-4379-A35E-7D5930381003}"
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
              >4097</PROPERTY>

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

            <DISPLAYNAME>Arabic (Libya) - 1001</DISPLAYNAME>

            <DESCRIPTION>Arabic (Libya) - 1001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{2773D28B-2A9C-4160-B2B9-CECC2B89DB44}"
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
              >6145</PROPERTY>

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

            <DISPLAYNAME>Arabic (Morocco) - 1801</DISPLAYNAME>

            <DESCRIPTION>Arabic (Morocco) - 1801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{661F9F2B-5FB8-48FF-8A5A-94AD487CB0BB}"
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
              >8193</PROPERTY>

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

            <DISPLAYNAME>Arabic (Oman) - 2001</DISPLAYNAME>

            <DESCRIPTION>Arabic (Oman) - 2001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{C8D4098D-6B9C-44A1-A587-A7B405FF2F22}"
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
              >16385</PROPERTY>

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

            <DISPLAYNAME>Arabic (Qatar) - 4001</DISPLAYNAME>

            <DESCRIPTION>Arabic (Qatar) - 4001</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{E6C8CA09-58F5-47A3-81B3-7A2148A5D6B8}"
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
              >1025</PROPERTY>

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

            <DISPLAYNAME>Arabic (Saudi Arabia) - 0401</DISPLAYNAME>

            <DESCRIPTION>Arabic (Saudi Arabia) - 0401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{52A49B30-E209-46EC-8FA2-29DF7CDC69E7}"
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
              >10241</PROPERTY>

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

            <DISPLAYNAME>Arabic (Syria) - 2801</DISPLAYNAME>

            <DESCRIPTION>Arabic (Syria) - 2801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{9FCD89CD-9D89-4B52-B2DB-C7C58AAE4508}"
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
              >7169</PROPERTY>

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

            <DISPLAYNAME>Arabic (Tunisia) - 1c01</DISPLAYNAME>

            <DESCRIPTION>Arabic (Tunisia) - 1c01</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{7F7EEE17-858E-4E1C-82FE-58E589924EF6}"
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
              >14337</PROPERTY>

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

            <DISPLAYNAME>Arabic (UAE) - 3801</DISPLAYNAME>

            <DESCRIPTION>Arabic (UAE) - 3801</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{CF9A2AA4-F46A-453D-86CA-33219AAE920D}"
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
              >9217</PROPERTY>

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

            <DISPLAYNAME>Arabic (Yemen) - 2401</DISPLAYNAME>

            <DESCRIPTION>Arabic (Yemen) - 2401</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{FB08459B-7E37-4FA5-B336-ADA923EE2533}"
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

            <DISPLAYNAME>864   (OEM - Arabic)</DISPLAYNAME>

            <DESCRIPTION>864   (OEM - Arabic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{75EEA591-3C3E-4691-B27D-0F92DFC8EE3E}"
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

            <DISPLAYNAME>Font Substitute dependency: Fonts_Arabic_font_subsititute_fonts</DISPLAYNAME>

            <DESCRIPTION>This component dependends on font substitute Fonts_Arabic_font_subsititute_fonts</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{2CF82785-B9AA-4E3B-A6CB-41594ED2A486}"
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
        </DEPENDENCIES>

        <SECONDARYPLATFORMS
        >        </SECONDARYPLATFORMS>

        <DISPLAYNAME>Arabic Language Support</DISPLAYNAME>

        <VERSION>5.1.2600.1</VERSION>

        <DESCRIPTION>This component contains all necessary modules to support Arabic language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:57 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:57 PM</DATEREVISED>
      </COMPONENT>
    </COMPONENTS>

    <RESTYPES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </RESTYPES>
  </DCARRIER>
