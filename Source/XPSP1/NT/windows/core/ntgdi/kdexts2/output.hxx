/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    output.hxx

Abstract:

    This header file declares output classes for callbacks,
    control, parsing, and filtering.

Author:

    JasonHa

--*/


#ifndef _OUTPUT_HXX_
#define _OUTPUT_HXX_


class OutputControl
{
public:
    // IUnknown
    virtual ULONG STDMETHODCALLTYPE AddRef(THIS) {
        return ++RefCount;
    }

    virtual ULONG STDMETHODCALLTYPE Release(void) {
        ULONG NewCount = RefCount-1;
        if (NewCount == 0)
        {
            delete this;
        }
        else
        {
            RefCount = NewCount;
        }
        return NewCount;
    }

    OutputControl()
    {
        RefCount = 1;
        OutCtl = DEBUG_OUTCTL_AMBIENT;
        Control = NULL;
    }

    OutputControl(ULONG OutputControl,
                  PDEBUG_CLIENT Client = NULL)
    {
        RefCount = 1;
        OutCtl = DEBUG_OUTCTL_AMBIENT;
        Control = NULL;
        SetControl(OutputControl, Client);
    }

    OutputControl(PDEBUG_CLIENT Client)
    {
        RefCount = 1;
        OutCtl = DEBUG_OUTCTL_AMBIENT;
        Control = NULL;
        SetControl(DEBUG_OUTCTL_AMBIENT, Client);
    }

    ~OutputControl()
    {
        if (RefCount != 1)
        {
            DbgPrint("OutputControl::RefCount != 1.\n");
            DbgBreakPoint();
        }

        if (Control != NULL) Control->Release();
    }

    ULONG GetControl() { return OutCtl; }
    HRESULT SetControl(ULONG OutputControl, PDEBUG_CLIENT Client = NULL);

    HRESULT Output(ULONG Mask, PCSTR Format, ...);
    HRESULT OutputVaList(ULONG Mask, PCSTR Format, va_list Args);

    HRESULT Output(PCSTR Format, ...);
    HRESULT OutErr(PCSTR Format, ...);
    HRESULT OutWarn(PCSTR Format, ...);
    HRESULT OutVerb(PCSTR Format, ...);

    HRESULT GetInterrupt();
    HRESULT SetInterrupt(ULONG Flags);
    HRESULT Evaluate(IN PCSTR Expression, IN ULONG DesiredType, OUT PDEBUG_VALUE Value, OUT OPTIONAL PULONG RemainderIndex);
    HRESULT CoerceValue(IN PDEBUG_VALUE In, IN ULONG OutType, OUT PDEBUG_VALUE Out);
    HRESULT IsPointer64Bit();

private:
    ULONG           RefCount;
    PDEBUG_CONTROL  Control;
    ULONG           OutCtl;
};


class OutputState
{
public:
    OutputState(PDEBUG_CLIENT OrgClient, BOOL SameClient=FALSE);
    ~OutputState();

    HRESULT Setup(ULONG OutMask, PDEBUG_OUTPUT_CALLBACKS OutCallbacks);

    HRESULT Execute(IN PCSTR pszCommand);

    HRESULT OutputType(
        IN BOOL Physical,
        IN ULONG64 Offset,
        IN ULONG64 Module,
        IN ULONG TypeId,
        IN ULONG Flags
    );

    HRESULT OutputType(
        IN BOOL Physical,
        IN ULONG64 Offset,
        IN PCSTR Type,
        IN ULONG Flags
    );

    HRESULT OutputTypePhysical(
        IN ULONG64 Offset,
        IN ULONG64 Module,
        IN ULONG TypeId,
        IN ULONG Flags
    )
    {
        return OutputType(TRUE, Offset, Module, TypeId, Flags);
    }

    HRESULT OutputTypePhysical(
        IN ULONG64 Offset,
        IN PCSTR Type,
        IN ULONG Flags
    )
    {
        return OutputType(TRUE, Offset, Type, Flags);
    }

    HRESULT OutputTypeVirtual(
        IN ULONG64 Offset,
        IN ULONG64 Module,
        IN ULONG TypeId,
        IN ULONG Flags
    )
    {
        return OutputType(FALSE, Offset, Module, TypeId, Flags);
    }

    HRESULT OutputTypeVirtual(
        IN ULONG64 Offset,
        IN PCSTR Type,
        IN ULONG Flags
    )
    {
        return OutputType(FALSE, Offset, Type, Flags);
    }

    void Restore();

public:
    PDEBUG_CLIENT           Client;

private:
    HRESULT                 hrInit;
    PDEBUG_CONTROL          Control;
    PDEBUG_SYMBOLS          Symbols;
    BOOL                    CreatedClient;
    BOOL                    SetCallbacks;
    BOOL                    Saved;
    ULONG                   OrgOutMask;
    PDEBUG_OUTPUT_CALLBACKS OrgOutCallbacks;
};


//----------------------------------------------------------------------------
//
// Default output callbacks implementation, provides IUnknown for
// static classes.
//
//----------------------------------------------------------------------------

class DefOutputCallbacks :
    public IDebugOutputCallbacks
{
public:
    DefOutputCallbacks() { RefCount = 1; }
    ~DefOutputCallbacks()
    {
        if (RefCount != 1)
        {
            DbgPrint("DefOutputCallbacks@0x%p::RefCount(%lu) != 1.\n", this, RefCount);
            DbgBreakPoint();
        }
#if DBG
        RefCount--;
#endif
    }

    // IUnknown.
    STDMETHOD(QueryInterface)(
        THIS_
        IN REFIID InterfaceId,
        OUT PVOID* Interface
        );
    STDMETHOD_(ULONG, AddRef)(
        THIS
        );
    STDMETHOD_(ULONG, Release)(
        THIS
        );

    // IDebugOutputCallbacks.
    STDMETHOD(Output)(
        THIS_
        IN ULONG Mask,
        IN PCSTR Text
        );

protected:
    ULONG   RefCount;
};


//----------------------------------------------------------------------------
//
// DebugOutputCallbacks.
//
//----------------------------------------------------------------------------

class DebugOutputCallbacks : public DefOutputCallbacks
{
public:
    // IDebugOutputCallbacks.
    STDMETHOD(Output)(
        THIS_
        IN ULONG Mask,
        IN PCSTR Text
        );
};


//----------------------------------------------------------------------------
//
// OutputReader
//
// General DebugOutputCallback class to read output.
//
//----------------------------------------------------------------------------

class OutputReader : public DefOutputCallbacks
{
public:
    OutputReader()
    {
        hHeap = NULL;
        Buffer = NULL;
        BufferSize = 0;
        BufferLeft = 0;
    }

    ~OutputReader()
    {
        if (Buffer) HeapFree(hHeap, 0, Buffer);
    }

    // IDebugOutputCallbacks.
    STDMETHOD(Output)(
        THIS_
        IN ULONG Mask,
        IN PCSTR Text
        );

    // Discard any text left unused by Parse
    virtual void DiscardOutput();

    // Get a copy of the output buffer
    HRESULT GetOutputCopy(PSTR *Copy);

    // Free the copy
    void FreeOutputCopy(PSTR Copy)
    {
        if (hHeap != NULL) { HeapFree(hHeap, 0, Copy); }
    }

protected:
    HANDLE  hHeap;
    PSTR    Buffer;
    SIZE_T  BufferSize;
    SIZE_T  BufferLeft;
};

    
//----------------------------------------------------------------------------
//
// OutputParser
//
// General DebugOutputCallback class to parse output.
//
//----------------------------------------------------------------------------

#define PARSE_OUTPUT_DISCARD    0x00000000
#define PARSE_OUTPUT_NO_DISCARD 0x00000001

#define PARSE_OUTPUT_UNPARSED   0x00000000
#define PARSE_OUTPUT_ALL        0x00000002

#define PARSE_OUTPUT_DEFAULT    (PARSE_OUTPUT_DISCARD | PARSE_OUTPUT_UNPARSED)

class OutputParser : public OutputReader
{
public:
    OutputParser() : OutputReader()
    {
        UnparsedIndex = 0;
    }

    // Send all read text through Parse method
    //  Flags:
    //      PARSE_OUTPUT_DISCARD or PARSE_OUTPUT_NO_DISCARD
    //      PARSE_OUTPUT_UNPARSED or PARSE_OUTPUT_ALL
    //      PARSE_OUTPUT_DEFAULT = PARSE_OUTPUT_DISCARD + PARSE_OUTPUT_UNPARSED
    HRESULT ParseOutput(FLONG Flags = PARSE_OUTPUT_DEFAULT);

    // Discard any text left unused by Parse
    void DiscardOutput();

    // Check if ready to look for keys/values
    virtual HRESULT Ready() PURE;

    // Reset progress counter so we may parse more output
    virtual void    Relook() PURE;

    // Parse line of text and optionally return index to unused portion of text
    virtual HRESULT Parse(IN PCSTR Text, OUT OPTIONAL PULONG RemainderIndex) PURE;

    // Check if all keys/values were found during past reads
    virtual HRESULT Complete() PURE;

private:
    DWORD           UnparsedIndex;
};

    
//----------------------------------------------------------------------------
//
// BasicOutputParser
//
// Basic DebugOutputCallback class to parse output looking for 
// string keys and subsequent values.
//
//----------------------------------------------------------------------------

#define PARSER_UNSPECIFIED_RADIX   -1
#define PARSER_DEFAULT_RADIX       0

class BasicOutputParser : public OutputParser
{
    typedef struct {
        PDEBUG_VALUE    Value;
        ULONG           Type;
        ULONG           Radix;
        CHAR            Key[80];
    } LookupEntry;

public:
    BasicOutputParser(PDEBUG_CLIENT OutputClient, ULONG TotalEntries = 4)
    {
        Client = OutputClient;

        Entries = new LookupEntry[TotalEntries];
        if (!Entries) TotalEntries = 0;

        MaxEntries = TotalEntries;
        NumEntries = 0;
        CurEntry = 0;
    }

    ~BasicOutputParser()
    {
        if (Entries) delete[] Entries;
    }

    HRESULT LookFor(OUT PDEBUG_VALUE Value,
                    IN PCSTR Key,
                    IN ULONG Type = DEBUG_VALUE_INVALID,
                    IN ULONG Radix = PARSER_UNSPECIFIED_RADIX);

    // Check if ready to look for keys/values
    HRESULT Ready() { return (CurEntry != NumEntries) ? S_OK : S_FALSE; }

    // Reset progress counter so we may parse more output
    void    Relook() { CurEntry = 0; }

    // Parse line of text and optionally return index to unused portion of text
    HRESULT Parse(IN PCSTR Text, OUT OPTIONAL PULONG RemainderIndex);

    // Check if all keys/values were found during past reads
    HRESULT Complete()
    {
        return (Client != NULL &&
                CurEntry == NumEntries) ?
            S_OK :
            S_FALSE;
    }

private:
    PDEBUG_CLIENT   Client;
    ULONG       MaxEntries;
    ULONG       NumEntries;

    ULONG       CurEntry;

    LookupEntry *Entries;
};


//----------------------------------------------------------------------------
//
// BitFieldParser
//
// DebugOutputCallback class to parse bitfield type output
//
//----------------------------------------------------------------------------

class BitFieldInfo {
public:
    BitFieldInfo() { Valid = FALSE; };
    BitFieldInfo(ULONG InitBitPos, ULONG InitBits) {
        Valid = Compose(InitBitPos, InitBits);
    }

    BOOL Compose(ULONG CBitPos, ULONG CBits)
    {
        BitPos = CBitPos;
        Bits = CBits;
        Mask = (((((ULONG64) 1) << Bits) - 1) << BitPos);
        return TRUE;
    }

    BOOL    Valid;
    ULONG   BitPos;
    ULONG   Bits;
    ULONG64 Mask;
};

class BitFieldParser : public OutputParser
{
public:
    BitFieldParser(PDEBUG_CLIENT Client, BitFieldInfo *BFI);

    // Reset progress counter so we may parse more output
    void    Relook()
    {
        if (BitField != NULL)
        {
            BitField->Valid = FALSE;
            BitField->BitPos = 0;
            BitField->Bits = 0;
            BitField->Mask = 0;
        }
        BitFieldReader.Relook();
    }

    // Check if ready to look for bit fields
    HRESULT Ready()
    {
        return (BitField != NULL) ?
            BitFieldReader.Ready() :
            S_FALSE;
    }

    // Parse line of text and optionally return index to unused portion of text
    HRESULT Parse(IN PCSTR Text, OUT OPTIONAL PULONG RemainderIndex);

    // Check if bit fields were found during past reads
    HRESULT Complete()
    {
        return (BitField != NULL && BitField->Valid) ?
            BitFieldReader.Complete() :
            S_FALSE;
    }

private:
    BitFieldInfo       *BitField;
    DEBUG_VALUE         BitPos;
    DEBUG_VALUE         Bits;
    BasicOutputParser   BitFieldReader;
};


//----------------------------------------------------------------------------
//
// OutputFilter
//
// DebugOutputCallback class to filter output
// by skipping/replacing lines.
//
//----------------------------------------------------------------------------

// Query Flags
#define OUTFILTER_QUERY_EVERY_LINE      0x00000000
#define OUTFILTER_QUERY_ONE_LINE        0x00000001

#define OUTFILTER_QUERY_WHOLE_WORD      0x00000002  // Characters before and after
                                                    // query must not be C symbols
                                                    // [a-z,A-A,0-9,_]

#define OUTFILTER_QUERY_ENABLED         0x00000004
#define OUTFILTER_QUERY_HIT             0x00000008

// Replace Flags
#define OUTFILTER_REPLACE_EVERY         0x00000000
#define OUTFILTER_REPLACE_ONCE          0x00010000

#define OUTFILTER_REPLACE_ALL_INSTANCES     (OUTFILTER_REPLACE_EVERY | OUTFILTER_QUERY_EVERY_LINE)
#define OUTFILTER_REPLACE_ONCE_PER_LINE     (OUTFILTER_REPLACE_ONCE  | OUTFILTER_QUERY_EVERY_LINE)
#define OUTFILTER_REPLACE_EXACTLY_ONCE      (OUTFILTER_REPLACE_ONCE  | OUTFILTER_QUERY_ONE_LINE)

#define OUTFILTER_REPLACE_CONTINUE      0x00000000
#define OUTFILTER_REPLACE_NEXT_LINE     0x00020000  // Stop replacement checks 
                                                    // for current line after
                                                    // this replacement

#define OUTFILTER_REPLACE_BEFORE        0x04000000  // Replace text in line prior to query match
#define OUTFILTER_REPLACE_THIS          0x02000000  // Replace query text
#define OUTFILTER_REPLACE_AFTER         0x01000000  // Replace text following query match

#define OUTFILTER_REPLACE_FROM_START    (OUTFILTER_REPLACE_BEFORE | OUTFILTER_REPLACE_THIS)
#define OUTFILTER_REPLACE_TO_END        (OUTFILTER_REPLACE_THIS | OUTFILTER_REPLACE_AFTER)
#define OUTFILTER_REPLACE_LINE          (OUTFILTER_REPLACE_BEFORE | \
                                         OUTFILTER_REPLACE_THIS |   \
                                         OUTFILTER_REPLACE_AFTER)

#define OUTFILTER_REPLACE_PRIORITY(x)   (((x+8) & 0xF) << 28)  // Higher priority replacement queries
                                                                // are tested before lower.  Priority
                                                                // range: 7 (high) to -7 (low)

#define OUTFILTER_REPLACE_DEFAULT       (OUTFILTER_REPLACE_ALL_INSTANCES |  \
                                         OUTFILTER_REPLACE_CONTINUE |       \
                                         OUTFILTER_REPLACE_THIS |           \
                                         OUTFILTER_REPLACE_PRIORITY(0))

// Skip Flags
#define OUTFILTER_SKIP_DEFAULT          OUTFILTER_QUERY_EVERY_LINE

// FindMatch Flags
#define OUTFILTER_FINDMATCH_ANYWHERE    0
#define OUTFILTER_FINDMATCH_AT_START    1
#define OUTFILTER_FINDMATCH_MARK        0
#define OUTFILTER_FINDMATCH_NO_MARK     2

#define OUTFILTER_FINDMATCH_DEFAULT     (OUTFILTER_FINDMATCH_ANYWHERE | OUTFILTER_FINDMATCH_MARK)

class OutputFilter : public OutputReader
{
public:
    OutputFilter(PDEBUG_CLIENT DbgClient) : OutputReader()
    {
        Client = DbgClient;
        if (Client != NULL) Client->AddRef();
        SkipList = NULL;
        ReplaceList = NULL;
        Outputing = FALSE;
    }
    ~OutputFilter()
    {
        if (Client != NULL) Client->Release();
    }

    // IDebugOutputCallbacks.
    STDMETHOD(Output)(
        THIS_
        IN ULONG Mask,
        IN PCSTR Text
        );

    HRESULT Query(PCSTR Query,
                  PDEBUG_VALUE Value = NULL,
                  ULONG Type = DEBUG_VALUE_INVALID,
                  ULONG Radix = PARSER_UNSPECIFIED_RADIX);

    HRESULT Replace(ULONG Flags, PCSTR Query, PCSTR Replacement);
    HRESULT Skip(ULONG Flags, PCSTR Query);

    HRESULT OutputText(OutputControl *OutCtl = NULL, ULONG Mask = DEBUG_OUTPUT_NORMAL);

protected:
    class QuerySpec
    {
    public:
        QuerySpec(ULONG Flags, PCSTR Query);
        ~QuerySpec()
        {
            if (Query != NULL)
            {
                delete[] Query;
            }
        }

        QuerySpec      *Next;
        ULONG           Flags;
        ULONG           QueryLen;
        PSTR            Query;
    };

    class ReplacementSpec : public QuerySpec
    {
    public:
        ReplacementSpec(ULONG Flags, PCSTR QueryText, PCSTR ReplacementText);
        ~ReplacementSpec()
        {
            if (Replacement != NULL)
            {
                delete[] Replacement;
            }
        }

//        ReplacementSpec *Next;
        ULONG           ReplacementLen;
        PSTR            Replacement;
    };

    QuerySpec **FindPrior(ULONG Flags, PCSTR Query, QuerySpec **List);
    QuerySpec *FindMatch(PCSTR Text,
                         QuerySpec *List,
                         ULONG Start = 0,
                         ULONG Flags = OUTFILTER_FINDMATCH_DEFAULT,
                         PULONG MatchPos = NULL);

    PDEBUG_CLIENT       Client;

    ReplacementSpec    *ReplaceList;
    QuerySpec          *SkipList;

private:
    BOOL                Outputing;
};


#endif  _OUTPUT_HXX_

