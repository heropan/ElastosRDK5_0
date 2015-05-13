
#ifndef __CACCELERATEDECELERATEINTERPOLATOR_H__
#define __CACCELERATEDECELERATEINTERPOLATOR_H__

#include "_CAccelerateDecelerateInterpolator.h"

using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAccelerateDecelerateInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__CACCELERATEDECELERATEINTERPOLATOR_H__
