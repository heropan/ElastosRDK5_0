
#ifndef __CBOUNCEINTERPOLATOR_H__
#define __CBOUNCEINTERPOLATOR_H__

#include "_CBounceInterpolator.h"

using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CBounceInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);

private:
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__CBOUNCEINTERPOLATOR_H__
