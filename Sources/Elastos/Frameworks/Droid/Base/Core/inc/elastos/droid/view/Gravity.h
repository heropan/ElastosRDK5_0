
#ifndef __ELASTOS_DROID_VIEW_GRAVITY_H__
#define __ELASTOS_DROID_VIEW_GRAVITY_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;
using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace View {

class Gravity
    : public Singleton
    , public IGravity
{
public:
    CARAPI Apply(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* container,
        /* [in] */ IRect* outRect);

    CARAPI Apply(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* container,
        /* [in] */ IRect* outRect,
        /* [in] */ Int32 layoutDirection);

    CARAPI Apply(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* container,
        /* [in] */ Int32 xAdj,
        /* [in] */ Int32 yAdj,
        /* [in] */ IRect* outRect);

    CARAPI Apply(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* container,
        /* [in] */ Int32 xAdj,
        /* [in] */ Int32 yAdj,
        /* [in] */ IRect* outRect,
        /* [in] */ Int32 layoutDirection);

    CARAPI ApplyDisplay(
        /* [in] */ Int32 gravity,
        /* [in] */ IRect* display,
        /* [in] */ IRect* inoutObj);

    CARAPI ApplyDisplay(
        /* [in] */ Int32 gravity,
        /* [in] */ IRect* display,
        /* [in] */ IRect* inoutObj,
        /* [in] */ Int32 layoutDirection);

    CARAPI IsVertical(
        /* [in] */ Int32 gravity,
        /* [out] */ Boolean* rst);

    CARAPI IsHorizontal(
        /* [in] */ Int32 gravity,
        /* [out] */ Boolean* rst);

    CARAPI GetAbsoluteGravity(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 layoutDirection,
        /* [out] */ Int32* rst);

    static CARAPI_(Boolean) IsVertical(
        /* [in] */ Int32 gravity);

    static CARAPI_(Boolean) IsHorizontal(
        /* [in] */ Int32 gravity);

    static CARAPI_(Int32) GetAbsoluteGravity(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 layoutDirection);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_GRAVITY_H__
