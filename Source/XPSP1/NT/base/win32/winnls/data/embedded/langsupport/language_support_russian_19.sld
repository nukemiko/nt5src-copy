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
        GroupVSGUID="{b6131f2d-229b-4451-896e-48ea92f67835}"
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

        <DISPLAYNAME>Russian Language Extended Support</DISPLAYNAME>

        <VERSION>5.1.2600</VERSION>

        <DESCRIPTION>This dependency group contains any additional NLS components that support Russian Language.</DESCRIPTION>

        <COPYRIGHT>Copyright © 2001 Microsoft Corporation</COPYRIGHT>

        <VENDOR>Microsoft Corporation</VENDOR>

        <OWNERS>Microsoft</OWNERS>

        <AUTHORS>Microsoft</AUTHORS>

        <DATECREATED>7/26/2002 6:14:59 PM</DATECREATED>

        <DATEREVISED>7/26/2002 6:14:59 PM</DATEREVISED>
      </GROUP>

      <GROUP
        GroupVSGUID="{e644fceb-f513-4ea4-b114-e1f566f497ba}"
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

        <DISPLAYNAME>Russian Locales Group</DISPLAYNAME>

        <VERSION>5.1.2600</VERSION>

        <DESCRIPTION>This dependency group contains user and system locale components that support Russian Language.</DESCRIPTION>

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
        ComponentVSGUID="{CFF9BE81-87E6-40D7-8A69-90B7579AC52E}"
        ComponentVIGUID="{850F896C-3C03-484E-B534-1A24E61248CE}"
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
          >{B6131F2D-229B-4451-896E-48EA92F67835}</PROPERTY>

          <PROPERTY
            Name="cmiLangName"
            Format="String"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >Russian</PROPERTY>

          <PROPERTY
            Name="cmiLangId"
            Format="Integer"
            Context="1"
            PlatformGUID="{00000000-0000-0000-0000-000000000000}"
          >25</PROPERTY>
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
            DependOnGUID="{1486EF9F-9F53-4ADA-9347-1C0779FD5B23}"
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

            <DISPLAYNAME>1251  (ANSI - Cyrillic)</DISPLAYNAME>

            <DESCRIPTION>1251  (ANSI - Cyrillic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{3E41B61F-5EB8-4411-9F1B-B7A52397F7F8}"
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

            <DISPLAYNAME>20866 (Russian - KOI8)</DISPLAYNAME>

            <DESCRIPTION>20866 (Russian - KOI8)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{BA73A63A-13F2-4493-B35C-9652D0EE4ACD}"
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

            <DISPLAYNAME>10007 (MAC - Cyrillic)</DISPLAYNAME>

            <DESCRIPTION>10007 (MAC - Cyrillic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{5BDAC3AA-F872-481A-90A5-3C7BA5AB5819}"
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

            <DISPLAYNAME>20880 (IBM EBCDIC - Cyrillic (Russian))</DISPLAYNAME>

            <DESCRIPTION>20880 (IBM EBCDIC - Cyrillic (Russian))</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{898C35C2-EF63-4962-98A8-F099EDC2D1BA}"
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

            <DISPLAYNAME>21025 (IBM EBCDIC - Cyrillic (Serbian, Bulgarian))</DISPLAYNAME>

            <DESCRIPTION>21025 (IBM EBCDIC - Cyrillic (Serbian, Bulgarian))</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{7CA22C0B-93AB-4D3D-9E00-C790E2D4757B}"
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

            <DISPLAYNAME>28595 (ISO 8859-5 Cyrillic)</DISPLAYNAME>

            <DESCRIPTION>28595 (ISO 8859-5 Cyrillic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{2E33EEB1-F601-4EAF-828B-33D1BAADB9A3}"
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

            <DISPLAYNAME>Cyrillic bitmap fonts</DISPLAYNAME>

            <DESCRIPTION>Cyrillic bitmap fonts</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{A30A2ABE-6342-4AE9-AA2C-0799D9B48FBC}"
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

            <DISPLAYNAME>Russian (Typewriter)</DISPLAYNAME>

            <DESCRIPTION>Russian (Typewriter)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{E6EA22D5-B71C-46A3-B2E2-C84962EA9D0F}"
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

            <DISPLAYNAME>Russian</DISPLAYNAME>

            <DESCRIPTION>Russian</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{86327B17-D6BE-4F4E-8C35-8A7B97A73C2F}"
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

            <DISPLAYNAME>User Locale: 0419</DISPLAYNAME>

            <DESCRIPTION>User Locale: 0419</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{9CB28108-9054-40B5-B98C-048B574B1264}"
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
              >1049</PROPERTY>

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

            <DISPLAYNAME>Russian - 0419</DISPLAYNAME>

            <DESCRIPTION>Russian - 0419</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{96841D32-7F2F-4768-A248-AF17B03B8D30}"
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

            <DISPLAYNAME>866   (OEM - Russian)</DISPLAYNAME>

            <DESCRIPTION>866   (OEM - Russian)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{1C469A72-753A-49C5-8CA7-BB43F86C919D}"
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

            <DISPLAYNAME>855   (OEM - Cyrillic)</DISPLAYNAME>

            <DESCRIPTION>855   (OEM - Cyrillic)</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="AtLeastOne"
            DependOnGUID="{90F57626-1581-46F6-9442-B85177456CE8}"
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

            <DISPLAYNAME>Font Substitute dependency: Fonts_Cyrillic_font_substitute</DISPLAYNAME>

            <DESCRIPTION>This component dependends on font substitute Fonts_Cyrillic_font_substitute</DESCRIPTION>
          </DEPENDENCY>

          <DEPENDENCY
            Class="Include"
            Type="All"
            DependOnGUID="{E644FCEB-F513-4EA4-B114-E1F566F497BA}"
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

        <DISPLAYNAME>Russian Language Support</DISPLAYNAME>

        <VERSION>5.1.2600.1</VERSION>

        <DESCRIPTION>This component contains all necessary modules to support Russian language.</DESCRIPTION>

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
