
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CANTICIPATEOVERSHOOTINTERPOLATOR_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CANTICIPATEOVERSHOOTINTERPOLATOR_H__

#include "_CAnticipateOvershootInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAnticipateOvershootInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float tension);

    CARAPI constructor(
        /* [in] */ Float tension,
        /* [in] */ Float extraTension);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mTension;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_CANTICIPATEOVERSHOOTINTERPOLATOR_H__
