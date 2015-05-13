
#ifndef __CCYCLEINTERPOLATOR_H__
#define __CCYCLEINTERPOLATOR_H__

#include "_CCycleInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CCycleInterpolator)
{
public:
    CARAPI constructor(
        /* [in] */ Float cycles);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mCycles;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__CCYCLEINTERPOLATOR_H__
