
#ifndef __ELASTOS_DROID_TEXT_CSPANNABLEFACTORY_H__
#define __ELASTOS_DROID_TEXT_CSPANNABLEFACTORY_H__

#include "_Elastos_Droid_Text_CSpannableFactory.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSpannableFactory)
{
public:
    CARAPI NewSpannable(
        /* [in] */ ICharSequence* source,
        /* [out] */ ISpannable** spannable);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_CSPANNABLEFACTORY_H__
