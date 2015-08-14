
#ifndef __ELASTOS_DROID_WIDGET_CABSLISTVIEWLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CABSLISTVIEWLAYOUTPARAMS_H__

#include "_CAbsListViewLayoutParams.h"
#include "widget/AbsListViewLayoutParams.h"

using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CAbsListViewLayoutParams) , public AbsListViewLayoutParams
{
public :

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 viewType);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI GetViewType(
        /* [out] */ Int32* viewType);

    CARAPI SetViewType(
        /* [in] */ Int32 viewType);

    CARAPI GetRecycledHeaderFooter(
        /* [out] */ Boolean* footer);

    CARAPI SetRecycledHeaderFooter(
        /* [in] */ Boolean footer);

    CARAPI GetForceAdd(
        /* [out] */ Boolean* force);

    CARAPI SetForceAdd(
        /* [in] */ Boolean force);

    CARAPI GetItemId(
        /* [out] */ Int32* itemId);

    CARAPI SetItemId(
        /* [in] */ Int32 itemId);

    CARAPI GetScrappedFromPosition(
        /* [out] */ Int32* scrappedFromPosition);

    CARAPI SetScrappedFromPosition(
        /* [in] */  Int32 scrappedFromPosition);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetLayoutAnimationParameters(
        /* [in] */ IAnimationParameters* ap);

    CARAPI GetLayoutAnimationParameters(
        /* [out] */ IAnimationParameters** ap);

    /**
     * Resolve layout parameters depending on the layout direction. Subclasses that care about
     * layoutDirection changes should override this method. The default implementation does
     * nothing.
     *
     * @param layoutDirection the direction of the layout
     *
     * {@link View#LAYOUT_DIRECTION_LTR}
     * {@link View#LAYOUT_DIRECTION_RTL}
     */
    CARAPI ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    /**
     * Use {@code canvas} to draw suitable debugging annotations for these LayoutParameters.
     *
     * @param view the view that contains these layout parameters
     * @param canvas the canvas on which to draw
     *
     * @hide
     */
    CARAPI OnDebugDraw(
        /* [in] */ IView* view,
        /* [in] */ ICanvas* canvas);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
