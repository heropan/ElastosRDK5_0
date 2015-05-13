#ifndef __CIMAGESWITCHER_H_
#define __CIMAGESWITCHER_H_

#include "_CImageSwitcher.h"
#include "widget/ImageSwitcher.h"
#include "view/ViewMacro.h"
#include "widget/FrameLayoutMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CImageSwitcher), public ImageSwitcher
{
public:
    IVIEW_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()
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

    CARAPI SetImageResource(
        /* [in] */ Int32 resid);

    CARAPI SetImageURI(
        /* [in] */ IUri* uri);

    CARAPI SetImageDrawable(
        /* [in] */ IDrawable* drawable);

    CARAPI SetDisplayedChild(
        /* [in] */ Int32 whichChild);

    CARAPI GetDisplayedChild(
        /* [out] */ Int32* child);

    CARAPI ShowNext();

    CARAPI ShowPrevious();

    CARAPI GetCurrentView(
        /* [out] */ IView** view);

    CARAPI GetInAnimation(
        /* [out] */ IAnimation** animation);

    CARAPI SetInAnimation(
        /* [in] */ IAnimation* inAnimation);

    CARAPI GetOutAnimation(
        /* [out] */ IAnimation** animation);

    CARAPI SetOutAnimation(
        /* [in] */ IAnimation* outAnimation);

    CARAPI SetInAnimationEx(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI SetOutAnimationEx(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI GetAnimateFirstView(
        /* [out] */ Boolean* animate);

    CARAPI SetAnimateFirstView(
        /* [in] */ Boolean animate);

    CARAPI GetNextView(
        /* [out] */ IView** view);

    CARAPI SetFactory(
        /* [in] */ IViewFactory* factory);

    CARAPI Reset();
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CIMAGESWITCHER_H_