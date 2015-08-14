
#ifndef __ELASTOS_DROID_VIEW_CSURFACEVIEW_H__
#define __ELASTOS_DROID_VIEW_CSURFACEVIEW_H__

#include "_CSurfaceView.h"
#include "view/SurfaceView.h"


namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSurfaceView), public SurfaceView
{
public:
    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pContext,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetHolder(
        /* [out] */ ISurfaceHolder** ppHolder);

    CARAPI SetZOrderMediaOverlay(
        /* [in] */ Boolean isMediaOverlay);

    CARAPI SetZOrderOnTop(
        /* [in] */ Boolean onTop);

    CARAPI SetSecure(
        /* [in] */ Boolean isSecure);

    CARAPI SetWindowType(
        /* [in] */ Int32 type);

    CARAPI IsFixedSize(
        /* [out] */ Boolean * pSize);

    //todo: for gecko using input method
    CARAPI SetCreateInputConnectionDelegate(
        /* [in] */ IView* view);

    //todo: for gecko using input method
    CARAPI SetKeyEventCallbackDelegate(
        /* [in] */IKeyEventCallback* cb);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_VIEW_CSURFACEVIEW_H__
