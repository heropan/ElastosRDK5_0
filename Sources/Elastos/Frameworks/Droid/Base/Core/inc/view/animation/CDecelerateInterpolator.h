
#ifndef __CDECELERATEINTERPOLATOR_H__
#define __CDECELERATEINTERPOLATOR_H__

#include "_CDecelerateInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CDecelerateInterpolator)
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
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__CDECELERATEINTERPOLATOR_H__
