
#ifndef  __CACTIONBARCONTEXTVIEW_H__
#define  __CACTIONBARCONTEXTVIEW_H__

#include "_CActionBarContextView.h"


#include "widget/internal/ActionBarContextView.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CActionBarContextView), public ActionBarContextView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetCustomView(
        /* [in] */ IView* view);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetSubtitle(
        /* [in] */ ICharSequence* subtitle);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI GetSubtitle(
        /* [out] */ ICharSequence** subTitle);

    CARAPI InitForMode(
        /* [in] */ IActionMode* mode);

    CARAPI CloseMode();

    CARAPI KillMode();

    CARAPI SetTitleOptional(
        /* [in] */ Boolean titleOptional);

    CARAPI IsTitleOptional(
        /* [out] */ Boolean* res);

    CARAPI SetSplitActionBar(
        /* [in] */ Boolean split);

    CARAPI SetSplitWhenNarrow(
        /* [in] */ Boolean splitWhenNarrow);

    CARAPI SetContentHeight(
        /* [in] */ Int32 height);

    CARAPI GetContentHeight(
        /* [out] */ Int32* height);

    CARAPI SetSplitView(
        /* [in] */ IActionBarContainer* splitView);

    CARAPI GetAnimatedVisibility(
        /* [out] */ Int32* visibility);

    CARAPI AnimateToVisibility(
        /* [in] */ Int32 visibility);

    CARAPI ShowOverflowMenu(
        /* [out] */ Boolean* result);

    CARAPI PostShowOverflowMenu();

    CARAPI HideOverflowMenu(
        /* [out] */ Boolean* result);

    CARAPI IsOverflowMenuShowing(
        /* [out] */ Boolean* result);

    CARAPI IsOverflowReserved(
        /* [out] */ Boolean* result);

    CARAPI DismissPopupMenus();

    CARAPI OnAnimationStart(
        /* [in] */ IAnimator* animation);

    CARAPI OnAnimationEnd(
        /* [in] */ IAnimator* animation);

    CARAPI OnAnimationCancel(
        /* [in] */ IAnimator* animation);

    CARAPI OnAnimationRepeat(
        /* [in] */ IAnimator* animation);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif  //__CACTIONBARCONTEXTVIEW_H__
