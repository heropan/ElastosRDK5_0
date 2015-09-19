
#ifndef __ELASTOS_DROID_WEBKIT_PLUGINFULLSCREENHOLDER_H__
#define __ELASTOS_DROID_WEBKIT_PLUGINFULLSCREENHOLDER_H__

#include "ext/frameworkext.h"
#include "widget/FrameLayout.h"


#include "webkit/WebChromeClient.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::FrameLayout;
using Elastos::Droid::Widget::IFrameLayout;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

class PluginFullScreenHolder : public ElRefBase
{
private:
    class InnerCustomFrameLayout
        : public ElLightRefBase
        , public FrameLayout
        , public IFrameLayout
    {
    public:
        InnerCustomFrameLayout(
            /* [in] */ IContext* context);

        CAR_INTERFACE_DECL();

        IVIEW_METHODS_DECL();
        IVIEWGROUP_METHODS_DECL();
        IFRAMELAYOUT_METHODS_DECL();
    };

    class CustomFrameLayout : public InnerCustomFrameLayout
    {
    public:
        CustomFrameLayout(
            /* [in] */ PluginFullScreenHolder* owner,
            /* [in] */ IContext* context);

        CARAPI OnKeyDown(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnKeyUp(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnTouchEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnTrackballEvent(
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:
        PluginFullScreenHolder* mOwner;
    };

public:
    PluginFullScreenHolder(
        /* [in] */ CWebViewClassic* webView,
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 npp);

    virtual CARAPI_(void) SetContentView(
        /* [in] */ IView* contentView);

    virtual CARAPI_(void) Show();

    virtual CARAPI_(void) Hide();

private:
    AutoPtr<CWebViewClassic> mWebView;
    Int32 mNpp;
    Int32 mOrientation;

    // The container for the plugin view
    static AutoPtr<CustomFrameLayout> mLayout;

    AutoPtr<IView> mContentView;

    AutoPtr<WebChromeClient::CustomViewCallback> mCallback;/* =
        new WebChromeClient.CustomViewCallback() {
            virtual CARAPI_(void) onCustomViewHidden() {

                mWebView.mPrivateHandler.obtainMessage(WebViewClassic.HIDE_FULLSCREEN)
                    .sendToTarget();

                mWebView.getWebViewCore().sendMessage(
                        WebViewCore.EventHub.HIDE_FULLSCREEN, mNpp, 0);

                mLayout.removeView(mContentView);
                mLayout = null;

                // Re enable plugin views.
                mWebView.getViewManager().showAll();
            }
        };*/
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_PLUGINFULLSCREENHOLDER_H__
