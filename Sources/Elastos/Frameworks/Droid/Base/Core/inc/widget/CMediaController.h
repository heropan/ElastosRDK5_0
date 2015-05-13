#ifndef __CMEDIACONTROLLER_H_
#define __CMEDIACONTROLLER_H_

#include "_CMediaController.h"
#include "widget/MediaController.h"
#include "view/ViewMacro.h"
#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/FrameLayoutMacro.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CMediaController), public MediaController
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean useFastForWard);

public:
    CARAPI SetMediaPlayer(
        /* [in] */ IMediaPlayerControl* player);

    CARAPI SetAnchorView(
        /* [in] */ IView* view);

    CARAPI Show();

    CARAPI ShowEx(
        /* [in] */ Int32 timeout);

    CARAPI IsShowing(
        /* [out] */ Boolean* isShowing);

    CARAPI Hide();

    CARAPI SetPrevNextListeners(
        /* [in] */ IViewOnClickListener* next,
        /* [in] */ IViewOnClickListener* prev);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CMEDIACONTROLLER_H_