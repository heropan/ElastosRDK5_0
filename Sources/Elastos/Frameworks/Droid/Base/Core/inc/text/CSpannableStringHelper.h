#ifndef __CSPANNABLESTRINGHELPER_H__
#define __CSPANNABLESTRINGHELPER_H__

#include "_CSpannableStringHelper.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSpannableStringHelper)
{
public:
    CARAPI ValueOf(
        /* [in] */ ICharSequence* source,
        /* [out] */ ISpannableString** spannedString);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__CSPANNABLESTRINGHELPER_H__

