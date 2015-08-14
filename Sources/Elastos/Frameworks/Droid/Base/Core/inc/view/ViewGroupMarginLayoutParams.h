
#ifndef __ELASTOS_DROID_VIEW_VIEWGROUPMARGINLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_VIEWGROUPMARGINLAYOUTPARAMS_H__

#include "view/ViewGroupLayoutParams.h"

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace View {

class ViewGroupMarginLayoutParams : public ViewGroupLayoutParams
{
public:
    ViewGroupMarginLayoutParams();

    /**
     * Creates a new set of layout parameters. The values are extracted from
     * the supplied attributes set and context.
     *
     * @param c the application environment
     * @param attrs the set of attributes from which to extract the layout
     *              parameters' values
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Copy constructor. Clones the width, height and margin values of the source.
     *
     * @param source The layout params to copy from.
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    /**
     * {@inheritDoc}
     */
    ViewGroupMarginLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    virtual CARAPI GetLeftMargin(
        /* [out] */ Int32* leftMargin);

    virtual CARAPI SetLeftMargin(
        /* [in] */ Int32 leftMargin);

    /**
     * The top margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    virtual CARAPI GetTopMargin(
        /* [out] */ Int32* topMargin);

    virtual CARAPI SetTopMargin(
        /* [in] */ Int32 topMargin);

    /**
     * The right margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    virtual CARAPI GetRightMargin(
        /* [out] */ Int32* rightMargin);

    virtual CARAPI SetRightMargin(
        /* [in] */ Int32 rightMargin);

    /**
     * The bottom margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    virtual CARAPI GetBottomMargin(
        /* [out] */ Int32* bottomMargin);

    virtual CARAPI SetBottomMargin(
        /* [in] */ Int32 bottomMargin);

    /**
     * Sets the margins, in pixels.
     *
     * @param left the left margin size
     * @param top the top margin size
     * @param right the right margin size
     * @param bottom the bottom margin size
     *
     * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginLeft
     * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginTop
     * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginRight
     * @attr ref android.R.styleable#ViewGroup_MarginLayout_layout_marginBottom
     */
    virtual CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI GetMargins(
        /* [out] */ Int32* left,
        /* [out] */ Int32* top,
        /* [out] */ Int32* right,
        /* [out] */ Int32* bottom);

    virtual CARAPI SetMarginsRelative(
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom);

    virtual CARAPI SetMarginStart(
        /* [in] */ Int32 start);

    virtual CARAPI_(Int32) GetMarginStart();

    virtual CARAPI SetMarginEnd(
        /* [in] */ Int32 end);

    virtual CARAPI_(Int32) GetMarginEnd();

    virtual CARAPI_(Boolean) IsMarginRelative();

    virtual CARAPI SetLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI_(Int32) GetLayoutDirection();

    /**
     * This will be called by {@link android.view.View#requestLayout()}. Left and Right margins
     * may be overridden depending on layout direction.
     */
    //@Override
    virtual CARAPI ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    virtual CARAPI OnDebugDraw(
        /* [in] */ IView* view,
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(Boolean) IsLayoutRtl();

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

public:
    /**
     * The left margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    Int32 mLeftMargin;

    /**
     * The top margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    Int32 mTopMargin;

    /**
     * The right margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    Int32 mRightMargin;

    /**
     * The bottom margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    Int32 mBottomMargin;

private:
    /**
     * The start margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    //@ViewDebug.ExportedProperty(category = "layout")
    Int32 mStartMargin;

    /**
     * The end margin in pixels of the child.
     * Call {@link ViewGroup#setLayoutParams(LayoutParams)} after reassigning a new value
     * to this field.
     */
    //@ViewDebug.ExportedProperty(category = "layout")
    Int32 mEndMargin;

    Int32 mInitialLeftMargin;
    Int32 mInitialRightMargin;

    static Int32 LAYOUT_DIRECTION_UNDEFINED;

    // Layout direction undefined by default
    Int32 mLayoutDirection;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_VIEW_VIEWGROUPMARGINLAYOUTPARAMS_H__
