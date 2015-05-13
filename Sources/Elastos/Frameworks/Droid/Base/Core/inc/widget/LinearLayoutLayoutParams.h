
#ifndef __LINEARLAYOUTLAYOUTPARAMS_H__
#define __LINEARLAYOUTLAYOUTPARAMS_H__

#include "view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {


class LinearLayoutLayoutParams : public ViewGroupMarginLayoutParams
{
public:
    LinearLayoutLayoutParams();

    /**
     * {@inheritDoc}
     */
    LinearLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    LinearLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Creates a new set of layout parameters with the specified width, height
     * and weight.
     *
     * @param width the width, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param height the height, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param weight the weight
     */
    LinearLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    /**
     * {@inheritDoc}
     */
    LinearLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* p);

    /**
     * {@inheritDoc}
     */
    LinearLayoutLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Float weight);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI SetWeight(
        /* [in] */ Float weight);

    CARAPI_(Float) GetWeight();

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI_(Int32) GetGravity();

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


#endif // __LINEARLAYOUTLAYOUTPARAMS_H__
