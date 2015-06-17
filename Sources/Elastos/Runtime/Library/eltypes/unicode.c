
#define _NO_INCLIST
#include <eltypes.h>
#include <limits.h>
#include <_pubcrt.h>

#include "ucase.h"

Char32 _getChar32LowerCase(Char32 ch)
{
    return u_tolower(ch);
}

Char32 _getChar32UpperCase(Char32 ch)
{
    return u_toupper(ch);
}
