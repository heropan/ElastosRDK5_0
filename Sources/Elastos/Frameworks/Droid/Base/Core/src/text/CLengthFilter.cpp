
#include "ext/frameworkext.h"
#include "text/CLengthFilter.h"
#include <elastos/Character.h>

using Elastos::Core::Character;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Text {

CLengthFilter::CLengthFilter()
    : mMax(0)
{}

ECode CLengthFilter::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);

    Int32 len;
    dest->GetLength(&len);
    Int32 keep = mMax - (len - (dend - dstart));

    if (keep <= 0) {
        return CStringWrapper::New(String(""), cs);
    }
    else if (keep >= end - start) {
        *cs = NULL;
        return NOERROR; // keep original
    }
    else {
        keep += start;
        Char32 sourceChar;
        if (Character::IsHighSurrogate((source->GetCharAt(keep - 1, &sourceChar), sourceChar))) {
            --keep;
            if (keep == start) {
                return CStringWrapper::New(String(""), cs);
            }
        }
        return source->SubSequence(start, keep, cs);
    }
}

ECode CLengthFilter::constructor(
    /* [in] */ Int32 max)
{
    mMax = max;
    return NOERROR;
}

}//namespace Text
}//namespace Droid
}//namespace Elastos