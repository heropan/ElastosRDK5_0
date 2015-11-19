
#ifndef __ELASTOS_DROID_WIDGET_LINEARLAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_LINEARLAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

class LinearLayoutLayoutParams
    : public ViewGroupMarginLayoutParams
    , public ILinearLayoutLayoutParams
{
public:
    CAR_INTERFACE_DECL();

    LinearLayoutLayoutParams();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI constructor(
        /* [in] */ ILinearLayoutLayoutParams* source);

    CARAPI SetWeight(
        /* [in] */ Float weight);

    CARAPI GetWeight(
        /* [out] */ Float* weight);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

public:
    /**
     * Indicates how much of the extra space in the LinearLayout will be
     * allocated to the view associated with these LayoutParams. Specify
     * 0 if the view should not be stretched. Otherwise the extra pixels
     * will be pro-rated among all views whose weight is greater than 0.
     */
    Float mWeight;

    /**
     * Gravity for the view associated with these LayoutParams.
     *
     * @see android.view.Gravity
     */
    Int32 mGravity;

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WIDGET_LINEARLAYOUTLAYOUTPARAMS_H__
