
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTVIEWCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTVIEWCLIENT_H__

// import android.content.Context;
// import android.view.KeyEvent;
// import android.view.View;
// import android.webkit.URLUtil;
// import android.webkit.WebChromeClient;
// import android.widget.FrameLayout;

// import org.chromium.base.CommandLine;
// import org.chromium.content.browser.ContentVideoView;
// import org.chromium.content.browser.ContentVideoViewClient;
// import org.chromium.content.browser.ContentViewClient;
// import org.chromium.content.common.ContentSwitches;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * ContentViewClient implementation for WebView
 */
class AwContentViewClient : public ContentViewClient
{
private:
    class AwContentVideoViewClient : public ContentVideoViewClient
    {
    private:
        class InnerWebChromeClientCustomViewCallback
            : public Object
            , public IWebChromeClientCustomViewCallback
        {
        public:
            InnerWebChromeClientCustomViewCallback(
                /* [in] */ AwContentVideoViewClient* owner);

            CARAPI OnCustomViewHidden();

        private:
            AwContentVideoViewClient* mOwner;
        };

        class InnerViewOnAttachStateChangeListener
            : public Object
            , public IViewOnAttachStateChangeListener
        {
        public:
            InnerViewOnAttachStateChangeListener(
                /* [in] */ AwContentVideoViewClient* owner,
                /* [in] */ IFrameLayout* viewGroup);

            //@Override
            CARAPI OnViewDetachedFromWindow(
                /* [in] */ IView* v);

            //@Override
            CARAPI OnViewAttachedToWindow(
                /* [in] */ IView* v);

        private:
            AwContentVideoViewClient* mOwner;
            IFrameLayout* mViewGroup;
        };

    public:
        AwContentVideoViewClient(
            /* [in] */ AwContentViewClient* owner);

        //@Override
        CARAPI_(Boolean) OnShowCustomView(
            /* [in] */ IView* view);

        //@Override
        CARAPI_(void) OnDestroyContentVideoView();

        //@Override
        CARAPI_(AutoPtr<IView>) GetVideoLoadingProgressView();

    private:
        CARAPI_(void) OnShowCustomViewLegacy(
            /* [in] */ IView* view,
            /* [in] */ IWebChromeClientCustomViewCallback* cb);

        CARAPI_(void) OnShowCustomView(
            /* [in] */ IView* view,
            /* [in] */ IWebChromeClientCustomViewCallback* cb);

    private:
        AwContentViewClient* mOwner;
    };

public:
    AwContentViewClient(
        /* [in] */ AwContentsClient* awContentsClient,
        /* [in] */ AwSettings* awSettings,
        /* [in] */ AwContents* awContents,
        /* [in] */ IContext* context);

    //@Override
    CARAPI_(void) OnBackgroundColorChanged(
        /* [in] */ Int32 color);

    //@Override
    CARAPI_(void) OnStartContentIntent(
        /* [in] */ IContext* context,
        /* [in] */ const String& contentUrl);

    //@Override
    CARAPI_(void) OnUpdateTitle(
        /* [in] */ const String& title);

    //@Override
    CARAPI_(Boolean) ShouldOverrideKeyEvent(
        /* [in] */ IKeyEvent* event);

    //@Override
    CARAPI_(AutoPtr<ContentVideoViewClient>) GetContentVideoViewClient();

    //@Override
    CARAPI_(Boolean) ShouldBlockMediaRequest(
        /* [in] */ const String& url);

private:
    static CARAPI_(Boolean) AreHtmlControlsEnabled();

private:
    AutoPtr<AwContentsClient> mAwContentsClient;
    AutoPtr<AwSettings> mAwSettings;
    AutoPtr<AwContents> mAwContents;
    AutoPtr<IContext> mContext;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTVIEWCLIENT_H__
