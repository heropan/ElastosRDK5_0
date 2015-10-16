#ifndef __ELASTOS_DROID_WIDGET_CFRAMELAYOUT_H__
#define __ELASTOS_DROID_WIDGET_CFRAMELAYOUT_H__

#include "_Elastos_Droid_Widget_CFrameLayout.h"


#include "elastos/droid/widget/FrameLayout.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CFrameLayout), public FrameLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Describes how the foreground is positioned.
     *
     * @return foreground gravity.
     *
     * @see #setForegroundGravity(int)
     *
     * @attr ref android.R.styleable#FrameLayout_foregroundGravity
     */
    CARAPI GetForegroundGravity(
        /* [out] */ Int32* foregroundGravity);

    /**
     * Describes how the foreground is positioned. Defaults to START and TOP.
     *
     * @param foregroundGravity See {@link android.view.Gravity}
     *
     * @see #getForegroundGravity()
     *
     * @attr ref android.R.styleable#FrameLayout_foregroundGravity
     */
    //@android.view.RemotableViewMethod
    CARAPI SetForegroundGravity(
        /* [in] */ Int32 foregroundGravity);

    /**
     * Supply a Drawable that is to be rendered on top of all of the child
     * views in the frame layout.  Any padding in the Drawable will be taken
     * into account by ensuring that the children are inset to be placed
     * inside of the padding area.
     *
     * @param drawable The Drawable to be drawn on top of the children.
     *
     * @attr ref android.R.styleable#FrameLayout_foreground
     */
    CARAPI SetForeground(
        /* [in] */ IDrawable* drawable);

    /**
     * Returns the drawable used as the foreground of this FrameLayout. The
     * foreground drawable, if non-null, is always drawn on top of the children.
     *
     * @return A Drawable or null if no foreground was set.
     */
    CARAPI GetForeground(
        /* [out] */ IDrawable** foreground);

    /**
     * Determines whether all children, or just those in the VISIBLE or
     * INVISIBLE state, are considered when measuring.
     *
     * @return Whether all children are considered when measuring.
     */
    CARAPI GetMeasureAllChildren(
        /* [out] */ Boolean* measureAll);

    /**
     * Sets whether to consider all children, or just those in
     * the VISIBLE or INVISIBLE state, when measuring. Defaults to false.
     *
     * @param measureAll true to consider children marked GONE, false otherwise.
     * Default value is false.
     *
     * @attr ref android.R.styleable#FrameLayout_measureAllChildren
     */
    //@android.view.RemotableViewMethod
    CARAPI SetMeasureAllChildren(
        /* [in] */ Boolean measureAll);

    /**
     * Determines whether all children, or just those in the VISIBLE or
     * INVISIBLE state, are considered when measuring.
     *
     * @return Whether all children are considered when measuring.
     *
     * @deprecated This method is deprecated in favor of
     * {@link #getMeasureAllChildren() getMeasureAllChildren()}, which was
     * renamed for consistency with
     * {@link #setMeasureAllChildren(boolean) setMeasureAllChildren()}.
     */
    //@Deprecated
    CARAPI GetConsiderGoneChildrenWhenMeasuring(
        /* [out] */ Boolean* measureAll);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CFRAMELAYOUT_H__
