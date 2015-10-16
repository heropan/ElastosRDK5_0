
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_SIZEADAPTIVELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_SIZEADAPTIVELAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/view/ViewGroupLayoutParams.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::ViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

/**
 * Per-child layout information associated with ViewSizeAdaptiveLayout.
 *
 * TODO extend to width and height
 *
 * @attr ref android.R.styleable#SizeAdaptiveLayout_Layout_layout_minHeight
 * @attr ref android.R.styleable#SizeAdaptiveLayout_Layout_layout_maxHeight
 */
class SizeAdaptiveLayoutLayoutParams : public ViewGroupLayoutParams
{
public:
    /**
     * {@inheritDoc}
     */
    SizeAdaptiveLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Creates a new set of layout parameters with the specified width, height
     * and valid height bounds.
     *
     * @param width the width, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param height the height, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param minHeight the minimum height of this child
     * @param maxHeight the maximum height of this child
     *        or {@link #UNBOUNDED} if the child can grow forever
     */
    SizeAdaptiveLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minHeight,
        /* [in] */ Int32 maxHeight);

    /**
     * {@inheritDoc}
     */
    SizeAdaptiveLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Constructs a new LayoutParams with default values as defined in {@link LayoutParams}.
     */
    SizeAdaptiveLayoutLayoutParams();

    /**
     * {@inheritDoc}
     */
    SizeAdaptiveLayoutLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI_(String) Debug(
        /* [in] */ const String& output);

protected:

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minHeight,
        /* [in] */ Int32 maxHeight);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI InitImpl(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI InitImpl(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minHeight,
        /* [in] */ Int32 maxHeight);

    CARAPI InitImpl(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI InitImpl();

    CARAPI InitImpl(
        /* [in] */ IViewGroupLayoutParams* p);

public:
    /**
     * Indicates the minimum valid height for the child.
     */

    Int32 mMinHeight;

    /**
     * Indicates the maximum valid height for the child.
     */
    Int32 mMaxHeight;

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_SIZEADAPTIVELAYOUTLAYOUTPARAMS_H__
