//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <lube.h>
#include <string.h>
#include <ctype.h>

static char * s_elatypes[] = {
    "ECode",    "PVoid",
    "Int8",     "UInt8",    "Byte",     "PInt8",    "PByte",    "PUInt8",
    "Char8",    "Char16",   "Char32",   "PChar8",   "PChar16",  "PChar32",
    "Int16",    "UInt16",   "PInt16",   "PUInt16",
    "Int32",    "UInt32",   "PInt32",   "PUInt32",
    "Int64",    "UInt64",   "PInt64",   "PUInt64",
    "Float",    "Double",   "PFloat",   "PDouble",
    "Flags8",   "Flags16",  "Flags32",  "Flags64",
    "PFlags8",  "PFlags16", "PFlags32", "PFlags64",
    "Handle32", "Handle64",
    "PHandle32", "PHandle64",
    "Boolean",  "PBoolean",
    "CString", "String",
    "StringBuf",

    "CarQuintet",  "BufferOf",      "ArrayOf",

    "MemoryBuf",
    "PMemoryBuf",  "PCarQuintet",

    "StringBuf_",  "MemoryBuf_",   "BufferOf_","ArrayOf_",

    "Encoding",    "SystemEvents",  "MemorySize",   "MemFlags", "EventState",
    "ThreadCreationFlags",          "ModuleLoadFlags",          "MemoryMapFlags",
    "Address",     "SystemTime",    "DateTime",     "Timestamp64",

    "EventHandler",

    "Second32",   "Millisecond32",   "Microsecond32",
    "Second64",   "Millisecond64",   "Microsecond64",
    "Psecond32",  "PMillisecond32",  "PMicrosecond32",
    "PSecond64",  "PMillisecond64",  "PMicrosecond64",

    "EGuid",       "EMuid",         "EIID",        "ClassID",      "InterfaceID",
    "REGuid",      "REMuid",        "REIID",       "RClassID",
};

const unsigned int c_elatypes = sizeof(s_elatypes) / sizeof(s_elatypes[0]);

char * s_replstr = "_ELASTOS ";

static int IsWordBoundary(int c)
{
    return !isalnum(c) && (c != '_');
}

// Search 'substr' in 'instr' and add a prefix to the 'substr'.
// Returns TRUE on success, and the 'outstr' holds the result string.
// Returns FALSE if 'substr' does not appear in 'instr'.
//
static BOOL AddNamespacePrefix(
    /* [in] */ char *instr,
    /* [in] */ char *substr,
    /* [out] */ char * outstr)
{
    assert(instr != NULL);
    assert(substr != NULL);
    assert(outstr != NULL);

    int nsubstr = strlen(substr);
    if (nsubstr == 0) {
        return FALSE;
    }

    char * p = strstr(instr, substr);
    if (p == NULL) {
        return FALSE;
    }

    int n = strlen(instr);
    char * instrend = instr + n;

    unsigned int neplstr = strlen(s_replstr);

    char * q;
    char * pbuf = outstr;
    pbuf[0] = 0;

    while (p != NULL) {
        if (p > instr) {
            n = p - instr;
            strncat(pbuf, instr, n);
            pbuf += n;
        }

        q = p + nsubstr;

        if (((p != instr && IsWordBoundary(*(p - 1))) || (p == instr))
            && (((q < instrend) && IsWordBoundary(*q)) || (q == instrend))) {
            strcat(pbuf, s_replstr);
            pbuf += neplstr;
        }

        strncat(pbuf, p, nsubstr);
        pbuf += nsubstr;
        instr = p + nsubstr;

        p = strstr(instr, substr);
    }

    if (instr != instrend) {
        strcat(pbuf, instr);
    }

    return TRUE;
}

char * ImplNamespaceType(const char * str)
{
    static char buf1[1024];
    static char buf2[1024];

    char * p = buf1;
    char * q = buf2;

    strcpy(p, str);
    char * ret = p;

    for (unsigned int i = 0; i < c_elatypes; i++) {
        if (AddNamespacePrefix(p, s_elatypes[i], q)) {
            ret = q;
            q = p;
            p = ret;
        }
    }
    return ret;
}
