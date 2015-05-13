
#ifndef __COVERSHOOTINTERPOLATOR_H__
#define __COVERSHOOTINTERPOLATOR_H__

#include "_COvershootInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(COvershootInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float tension);
    
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

#endif //__COVERSHOOTINTERPOLATOR_H__
