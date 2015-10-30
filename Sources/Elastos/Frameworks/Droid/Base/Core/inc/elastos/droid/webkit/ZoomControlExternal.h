
#ifndef __ELASTOS_DROID_WEBKIT_ZOOMCONTROLEXTERNAL_H__
#define __ELASTOS_DROID_WEBKIT_ZOOMCONTROLEXTERNAL_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/Runnable.h"

#include "elastos/droid/webkit/ZoomControlBase.h"
#include "elastos/droid/widget/FrameLayout.h"


using Elastos::Core::IRunnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::IZoomControls;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

class ZoomControlExternal
    : public Object
    , public IZoomControlBase
{
private:
    class ExtendedZoomControls
        : public IFrameLayout
        , public FrameLayout
    {
    public:
        ExtendedZoomControls(
            /* [in] */ IContext* context);

        CAR_INTERFACE_DECL();

        IVIEW_METHODS_DECL();
        IVIEWGROUP_METHODS_DECL();
        IFRAMELAYOUT_METHODS_DECL();

        CARAPI_(void) Show(
            /* [in] */ Boolean showZoom);

        CARAPI_(void) Hide();

        CARAPI_(Boolean) HasFocus();

        CARAPI_(void) SetOnZoomInClickListener(
            /* [in] */ IViewOnClickListener* listener);

        CARAPI_(void) SetOnZoomOutClickListener(
            /* [in] */ IViewOnClickListener* listener);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        CARAPI_(void) Fade(
            /* [in] */ Int32 visibility,
            /* [in] */ Float startAlpha,
            /* [in] */ Float endAlpha);

    private:
        AutoPtr<IZoomControls> mPlusMinusZoomControls;
    };

    class ZoomInClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        ZoomInClickListener(
            /* [in] */ ZoomControlExternal* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IView* v);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        ZoomControlExternal* mOwner;
    };

    class ZoomOutClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        ZoomOutClickListener(
            /* [in] */ ZoomControlExternal* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnClick(
            /* [in] */ IView* v);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        ZoomControlExternal* mOwner;
    };

    class ZoomControlRunnable : public Runnable
    {
    public:
        ZoomControlRunnable(
            /* [in] */ ZoomControlExternal* owner);

        CARAPI Run();

    private:
        ZoomControlExternal* mOwner;
    };

public:
    ZoomControlExternal(
        /* [in] */ CWebViewClassic* webView);

    CAR_INTERFACE_DECL()

    CARAPI Show();

    CARAPI Hide();

    CARAPI IsVisible(
        /* [out] */ Boolean* isVisible);

    CARAPI Update();

    CARAPI_(AutoPtr<ExtendedZoomControls>) GetControls();

private:
    CARAPI_(AutoPtr<ExtendedZoomControls>) CreateZoomControls();

private:
    // The time that the external controls are visible before fading away
    static const Int64 ZOOM_CONTROLS_TIMEOUT;
    // The view containing the external zoom controls
    AutoPtr<ExtendedZoomControls> mZoomControls;
    AutoPtr<IRunnable> mZoomControlRunnable;
    AutoPtr<IHandler> mPrivateHandler;

    CWebViewClassic* mWebView;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_ZOOMCONTROLEXTERNAL_H__
