#ifndef __ELASTOS_DROID_TEXT_CSPANNABLESTRINGHELPER_H__
#define __ELASTOS_DROID_TEXT_CSPANNABLESTRINGHELPER_H__

#include "_Elastos_Droid_Text_CSpannableStringHelper.h"

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

#endif //__ELASTOS_DROID_TEXT_CSPANNABLESTRINGHELPER_H__

