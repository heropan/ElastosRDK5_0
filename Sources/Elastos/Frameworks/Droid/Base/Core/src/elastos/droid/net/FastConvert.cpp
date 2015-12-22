
#include <Elastos.CoreLibrary.Core.h>
#include "elastos/droid/net/FastConvert.h"

using Elastos::Core::CString;

AutoPtr<ICharSequence> StringToCharSequence(const String& s)
{
    AutoPtr<ICharSequence> rev;
    CString::New(s, (ICharSequence**)&rev);
    return rev;
}