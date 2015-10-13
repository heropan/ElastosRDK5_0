#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWZOOMCONTROLS_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWZOOMCONTROLS_H__
#include "ext/frameworkext.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewCore.h"
//TODO #include "elastos/droid/webkit/native/android_webview/AwContents.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::IZoomButtonsController;
using Elastos::Droid::Widget::IOnZoomListener;
using Elastos::Droid::Webkit::Content::Browser::ContentViewCore;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

class AwZoomControls
: public ContentViewCore::ZoomControlsDelegate
{
private:
    class ZoomListener
        : public Object
        , public IOnZoomListener
    {
    public:
        CAR_INTERFACE_DECL();
        ZoomListener(
            /* [in] */ AwZoomControls* owner);

        //@Override
        CARAPI OnVisibilityChanged(
            /* [in] */ Boolean visible);

        //@Override
        CARAPI OnZoom(
            /* [in] */ Boolean zoomIn);

    private:
        AwZoomControls* mOwner;
    };

public:
    AwZoomControls(
        /* [in] */ /*TODO AwContents*/IInterface* awContents);

    //@Override
    CARAPI_(void) InvokeZoomPicker();

    //@Override
    CARAPI_(void) DismissZoomPicker();

    //@Override
    CARAPI_(void) UpdateZoomControls();

    // This method is used in tests. It doesn't modify the state of zoom controls.
    CARAPI_(AutoPtr<IView>) GetZoomControlsViewForTest();

private:
    CARAPI_(AutoPtr<IZoomButtonsController>) GetZoomController();

    AutoPtr</*TODO AwContents*/IInterface> mAwContents;
    // It is advised to use getZoomController() where possible.
    AutoPtr<IZoomButtonsController> mZoomButtonsController;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWZOOMCONTROLS_H__
