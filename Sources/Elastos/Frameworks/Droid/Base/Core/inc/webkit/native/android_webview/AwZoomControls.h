
#ifndef __AWZOOMCONTROLS_H__
#define __AWZOOMCONTROLS_H__

// import android.view.Gravity;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.FrameLayout;
// import android.widget.ZoomButtonsController;

// import org.chromium.content.browser.ContentViewCore.ZoomControlsDelegate;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

class AwZoomControls : public ZoomControlsDelegate
{
private:
    private class ZoomListener
        : public Object
        , public IOnZoomListener
    {
    public:
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
        /* [in] */ AwContents* awContents);

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

    AutoPtr<AwContents> mAwContents;
    // It is advised to use getZoomController() where possible.
    AutoPtr<IZoomButtonsController> mZoomButtonsController;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWZOOMCONTROLS_H__
