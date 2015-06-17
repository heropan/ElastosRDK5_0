
#include <sys/types.h>
#include <eltypes.h>
#include <elsharedbuf.h>
#include <elstring.h>
#include <stdlib.h>
#include <ctype.h>

#define _atoi64 atol

_ELASTOS_NAMESPACE_USING

EXTERN_C SharedBuffer* gElEmptyStringBuf = NULL;
EXTERN_C char* gElEmptyString = NULL;

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

#ifdef  __cplusplus
extern "C" {
#endif

Char32 _getChar32LowerCase(Char32 ch);
Char32 _getChar32UpperCase(Char32 ch);

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

Int32 __cdecl _String_GetLength(const char *str, Int32 maxLen)
{
    if (!str || maxLen < -1) return -1;
    if (maxLen == 0) return 0;
    if (maxLen == -1)  return strlen(str);

    const char *p = str;
    int i = 0;
    while (*p) {
        if (i >= maxLen) return -1;
        p++;
        i++;
    }
    return (p - str);
}

Char32 __cdecl _String_Char32_ToLowerCase(Char32 ch)
{
    return _getChar32LowerCase(ch);
}

Char32 __cdecl _String_Char32_ToUpperCase(_ELASTOS Char32 ch)
{
    return _getChar32UpperCase(ch);
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

#ifdef  __cplusplus
}
#endif
