
#ifndef __ELASTOS_DROID_APP_CACTIONBARLAYOUTPARAMS_H__
#define __ELASTOS_DROID_APP_CACTIONBARLAYOUTPARAMS_H__

#include "_Elastos_Droid_App_CActionBarLayoutParams.h"
#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
namespace Elastos {
namespace Droid {
namespace App {

/**
 * Per-child layout information associated with action bar custom views.
 *
 * @attr ref android.R.styleable#ActionBar_LayoutParams_layout_gravity
 */
CarClass(CActionBarLayoutParams), public ViewGroupMarginLayoutParams
{
public:
    CActionBarLayoutParams();

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 gravity);

    CARAPI constructor(
        /* [in] */ Int32 gravity);

    CARAPI constructor(
        /* [in] */ IActionBarLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);





    CARAPI GetLeftMargin(
        /* [out] */ Int32* leftMargin);

    CARAPI SetLeftMargin(
        /* [in] */ Int32 leftMargin);

    CARAPI GetTopMargin(
        /* [out] */ Int32* topMargin);

    CARAPI SetTopMargin(
        /* [in] */ Int32 topMargin);

    CARAPI GetRightMargin(
        /* [out] */ Int32* rightMargin);

    CARAPI SetRightMargin(
        /* [in] */ Int32 rightMargin);

    CARAPI GetBottomMargin(
        /* [out] */ Int32* bottomMargin);

    CARAPI SetBottomMargin(
        /* [in] */ Int32 bottomMargin);

    CARAPI SetMargins(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    CARAPI GetMargins(
        /* [out] */ Int32* left,
        /* [out] */ Int32* top,
        /* [out] */ Int32* right,
        /* [out] */ Int32* bottom);

    CARAPI SetMarginsRelative(
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom);

    CARAPI SetMarginStart(
        /* [in] */ Int32 start);

    CARAPI GetMarginStart(
        /* [out] */ Int32* startMargin);

    CARAPI SetMarginEnd(
        /* [in] */ Int32 end);

    CARAPI GetMarginEnd(
        /* [out] */ Int32* endMargin);

    CARAPI IsMarginRelative(
        /* [out] */ Boolean* set);

    CARAPI SetLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    CARAPI GetLayoutDirection(
        /* [out] */ Int32* layoutDirection);

    CARAPI IsLayoutRtl(
        /* [out] */ Boolean* rtl);
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

    CARAPI ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    CARAPI OnDebugDraw(
        /* [in] */ IView* view,
        /* [in] */ ICanvas* canvas);

public:
    Int32 mGravity;// = Gravity.NO_GRAVITY;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CACTIONBARLAYOUTPARAMS_H__
