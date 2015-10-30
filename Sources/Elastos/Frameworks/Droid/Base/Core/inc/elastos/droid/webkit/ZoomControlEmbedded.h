
#ifndef __ELASTOS_DROID_WEBKIT_ZOOMCONTROLEMBEDDED_H__
#define __ELASTOS_DROID_WEBKIT_ZOOMCONTROLEMBEDDED_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/ZoomControlBase.h"

using Elastos::Droid::Widget::IOnZoomListener;
using Elastos::Droid::Widget::IZoomButtonsController;

namespace Elastos {
namespace Droid {
namespace Webkit {

class ZoomManager;
class CWebViewClassic;

class ZoomControlEmbedded
    : public Object
    , public IZoomControlBase
{
private:
    class ZoomListener
        : public Object
        , public IOnZoomListener
    {
    public:
        ZoomListener(
            /* [in] */ ZoomControlEmbedded* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnVisibilityChanged(
            /* [in] */ Boolean visible);

        CARAPI OnZoom(
            /* [in] */ Boolean zoomIn);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        ZoomControlEmbedded* mOwner;
    };

public:
    ZoomControlEmbedded(
        /* [in] */ ZoomManager* zoomManager,
        /* [in] */ CWebViewClassic* webView);

    CAR_INTERFACE_DECL()

    CARAPI Show();

    CARAPI Hide();

    CARAPI IsVisible(
        /* [out] */ Boolean* isVisible);

    CARAPI Update();

    CARAPI ToString(
        /* [out] */ String* info);

private:
    CARAPI_(AutoPtr<IZoomButtonsController>) GetControls();

private:
    ZoomManager* mZoomManager;
    CWebViewClassic* mWebView;
    // The controller is lazily initialized in getControls() for performance.
    AutoPtr<IZoomButtonsController> mZoomButtonsController;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_ZOOMCONTROLEMBEDDED_H__
