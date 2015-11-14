
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CDECELERATEINTERPOLATOR_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CDECELERATEINTERPOLATOR_H__

#include "_Elastos_Droid_View_Animation_CDecelerateInterpolator.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Internal::View::Animation::INativeInterpolatorFactory;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CDecelerateInterpolator)
    , public Object
    , public IDecelerateInterpolator
    , public INativeInterpolatorFactory
    , public IInterpolator
    , public ITimeInterpolator
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CDecelerateInterpolator();

    ~CDecelerateInterpolator();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float factor);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /** @hide */
    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IResourcesTheme* theme,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);

    /** @hide */
    // @Override
    CARAPI CreateNativeInterpolator(
        /* [out] */ Int64* interpolator);

private:
    Float mFactor;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_CDECELERATEINTERPOLATOR_H__
