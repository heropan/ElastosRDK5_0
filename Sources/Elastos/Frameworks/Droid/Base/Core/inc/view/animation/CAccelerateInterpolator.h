
#ifndef __CACCELERATEINTERPOLATOR_H__
#define __CACCELERATEINTERPOLATOR_H__

#include "_CAccelerateInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAccelerateInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float factor);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mFactor;
    Double mDoubleFactor;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__CACCELERATEINTERPOLATOR_H__
