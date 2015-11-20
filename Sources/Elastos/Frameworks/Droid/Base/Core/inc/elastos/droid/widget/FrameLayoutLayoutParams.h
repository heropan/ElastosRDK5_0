
#ifndef __ELASTOS_DROID_WIDGET_FRAMELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_FRAMELAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::View::ViewGroupLayoutParams;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

class FrameLayoutLayoutParams
    : public ViewGroupMarginLayoutParams
    , public IFrameLayoutLayoutParams
{
public:
    CAR_INTERFACE_DECL();

    FrameLayoutLayoutParams();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    CARAPI constructor(
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
    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 gravity);

    /**
     * {@inheritDoc}
     */
    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    /**
     * {@inheritDoc}
     */
    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    /**
     * Copy constructor. Clones the width, height, margin values, and
     * gravity of the source.
     *
     * @param source The layout params to copy from.
     */
    CARAPI constructor(
        /* [in] */ IFrameLayoutLayoutParams* source);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

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

#endif // __ELASTOS_DROID_WIDGET_FRAMELAYOUTLAYOUTPARAMS_H__
