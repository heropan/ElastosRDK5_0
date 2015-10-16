
#ifndef __ELASTOS_DROID_WIDGET_ABSOLUTELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_ABSOLUTELAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/view/ViewGroupLayoutParams.h"

using Elastos::Droid::View::ViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {


/**
 * Per-child layout information associated with AbsoluteLayout.
 * See
 * {@link android.R.styleable#AbsoluteLayout_Layout Absolute Layout Attributes}
 * for a list of all child view attributes that this class supports.
 */
class AbsoluteLayoutLayoutParams : public ViewGroupLayoutParams
{
public:
    AbsoluteLayoutLayoutParams();

    /**
     * Creates a new set of layout parameters with the specified width,
     * height and location.
     *
     * @param width the width, either {@link #MATCH_PARENT},
              {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param height the height, either {@link #MATCH_PARENT},
              {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param x the X location of the child
     * @param y the Y location of the child
     */
    AbsoluteLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Creates a new set of layout parameters. The values are extracted from
     * the supplied attributes set and context. The XML attributes mapped
     * to this set of layout parameters are:
     *
     * <ul>
     *   <li><code>layout_x</code>: the X location of the child</li>
     *   <li><code>layout_y</code>: the Y location of the child</li>
     *   <li>All the XML attributes from
     *   {@link android.view.ViewGroup.LayoutParams}</li>
     * </ul>
     *
     * @param c the application environment
     * @param attrs the set of attributes fom which to extract the layout
     *              parameters values
     */
    AbsoluteLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    AbsoluteLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    ~AbsoluteLayoutLayoutParams() {}

    CARAPI SetX(
        /* [in] */ Int32 x);

    CARAPI SetY(
        /* [in] */ Int32 y);

    CARAPI_(Int32) GetX();

    CARAPI_(Int32) GetY();

protected:

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

public:
    /**
     * The horizontal, or X, location of the child within the view group.
     */
    Int32 mX;
    /**
     * The vertical, or Y, location of the child within the view group.
     */
    Int32 mY;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
