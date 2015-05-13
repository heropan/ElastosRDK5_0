#ifndef __CEDGEEFFECT_H_
#define __CEDGEEFFECT_H_

#include "_CEdgeEffect.h"
#include "widget/EdgeEffect.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;


namespace Elastos{
namespace Droid{
namespace Widget{


CarClass(CEdgeEffect), public EdgeEffect
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI IsFinished(
        /* [out] */ Boolean* isFinished);

    CARAPI Finish();

    CARAPI OnPull(
        /* [in] */ Float deltaDistance);

    CARAPI OnRelease();

    CARAPI OnAbsorb(
        /* [in] */ Int32 velocity);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [out] */ Boolean* res);

    CARAPI GetBounds(
        /* [in] */ Boolean reverse,
        /* [out] */ IRect** rect);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CEDGEEFFECT_H_
