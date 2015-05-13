#ifndef __CSPANNEDSTRINGHELPER_H__
#define __CSPANNEDSTRINGHELPER_H__

#include "_CSpannedStringHelper.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSpannedStringHelper)
{
public:
    CARAPI ValueOf(
        /* [in] */ ICharSequence* source,
        /* [out] */ ISpannedString** spannedString);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__CSPANNEDSTRINGHELPER_H__

