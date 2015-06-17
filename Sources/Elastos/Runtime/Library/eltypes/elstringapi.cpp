
#include <sys/types.h>
#include <eltypes.h>
#include <elsharedbuf.h>
#include <elstring.h>
#include <stdlib.h>
#include <ctype.h>

#include <ucase.h>

#define _atoi64 atol

_ELASTOS_NAMESPACE_USING

extern "C"
{
    SharedBuffer* gElEmptyStringBuf = NULL;
    char* gElEmptyString = NULL;
}

static void InitString()
{
    SharedBuffer* buf = SharedBuffer::Alloc(1);
    char* str = (char*)buf->GetData();
    *str = 0;
    gElEmptyStringBuf = buf;
    gElEmptyString = str;
}

static void UninitString()
{
    SharedBuffer::GetBufferFromData(gElEmptyString)->Release();
    gElEmptyStringBuf = NULL;
    gElEmptyString = NULL;
}

class LibUtilsFirstStatics
{
public:
    LibUtilsFirstStatics()
    {
        InitString();
    }

    ~LibUtilsFirstStatics()
    {
        UninitString();
    }
};

static LibUtilsFirstStatics gFirstStatics;

extern "C" {

char* __cdecl _String_Duplicate(const char *strSource)
{
    size_t len;
    char * copy;

    if (!strSource) {
        return NULL;
    }

    len = strlen(strSource) + 1;
    if ((copy = (char *)malloc(len)) == NULL) return NULL;
    memcpy(copy, strSource, len);
    return(copy);
}

void __cdecl _String_Free(char * str)
{
    if (str) {
        free((void *)str);
    }
}

Char32 __cdecl _String_Char32_ToLowerCase(Char32 ch)
{
    return u_tolower(ch);
}

Char32 __cdecl _String_Char32_ToUpperCase(_ELASTOS Char32 ch)
{
    return u_toupper(ch);
}

//---- HashCode ----
Int32 _String_GetHashCode(const char *string)
{
    Int64 h = 0;

    if (string) {
        for ( ; *string; ++string) {
            h = 5 * h + *string;
        }
    }
    return (Int32)h;
}

}
