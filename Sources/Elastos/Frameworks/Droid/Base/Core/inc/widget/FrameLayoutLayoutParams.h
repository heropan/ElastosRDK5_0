
#ifndef __FRAMELAYOUTLAYOUTPARAMS_H__
#define __FRAMELAYOUTLAYOUTPARAMS_H__

#include "ext/frameworkext.h"
#include "view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::View::ViewGroupLayoutParams;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;

namespace Elastos{
namespace Droid{
namespace Widget{


class FrameLayoutLayoutParams : public ViewGroupMarginLayoutParams
{
public:
    FrameLayoutLayoutParams();

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
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
     * @param gravity the gravity
     *
     * @see android.view.Gravity
     */
    FrameLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 gravity);

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    /**
     * {@inheritDoc}
     */
    FrameLayoutLayoutParams(
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
        /* [in] */ Int32 gravity);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI_(Int32) GetGravity();

public:
    /**
     * Gravity for the view associated with these LayoutParams.
     *
     * @see android.view.Gravity
     */
    Int32 mGravity;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __FRAMELAYOUTLAYOUTPARAMS_H__
