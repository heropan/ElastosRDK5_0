
#ifndef __CSPANNABLEFACTORY_H__
#define __CSPANNABLEFACTORY_H__

#include "_CSpannableFactory.h"

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

#endif //__CSPANNABLEFACTORY_H__
