#ifndef __ELASTOS_DROID_TEXT_CSPANNEDSTRINGHELPER_H__
#define __ELASTOS_DROID_TEXT_CSPANNEDSTRINGHELPER_H__

#include "_Elastos_Droid_Text_CSpannedStringHelper.h"

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

#endif //__ELASTOS_DROID_TEXT_CSPANNEDSTRINGHELPER_H__

