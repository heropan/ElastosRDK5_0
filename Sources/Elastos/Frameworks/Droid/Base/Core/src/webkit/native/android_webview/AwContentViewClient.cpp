
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//=======================================================================================================
//         AwContentViewClient::AwContentVideoViewClient::InnerWebChromeClientCustomViewCallback
//=======================================================================================================

AwContentViewClient::AwContentVideoViewClient::InnerWebChromeClientCustomViewCallback::InnerWebChromeClientCustomViewCallback(
    /* [in] */ AwContentVideoViewClient* owner)
    : mOwner(owner)
{
}

ECode AwContentViewClient::AwContentVideoViewClient::InnerWebChromeClientCustomViewCallback::OnCustomViewHidden()
{
    AutoPtr<ContentVideoView> contentVideoView = ContentVideoView::GetContentVideoView();
    if (contentVideoView != NULL)
        contentVideoView->ExitFullscreen(FALSE);

    return NOERROR;
}

//=======================================================================================================
//          AwContentViewClient::AwContentVideoViewClient::InnerViewOnAttachStateChangeListener
//=======================================================================================================

AwContentViewClient::AwContentVideoViewClient::InnerViewOnAttachStateChangeListener::InnerViewOnAttachStateChangeListener(
    /* [in] */ AwContentVideoViewClient* owner,
    /* [in] */ IFrameLayout* viewGroup)
    : mOwner(owner)
    , mViewGroup(viewGroup)
{
}

//@Override
ECode AwContentViewClient::AwContentVideoViewClient::InnerViewOnAttachStateChangeListener::OnViewDetachedFromWindow(
    /* [in] */ IView* v)
{
    // Intentional no-op (see onDestroyContentVideoView).
    return NOERROR;
}

//@Override
ECode AwContentViewClient::AwContentVideoViewClient::InnerViewOnAttachStateChangeListener::OnViewAttachedToWindow(
    /* [in] */ IView* v)
{
    if (mAwContents->IsFullScreen()) {
        return;
    }

    viewGroup->AddView(mAwContents->EnterFullScreen());

    return NOERROR;
}

//===============================================================
//        AwContentViewClient::AwContentVideoViewClient
//===============================================================

AwContentViewClient::AwContentVideoViewClient::AwContentVideoViewClient(
    /* [in] */ AwContentViewClient* owner)
    : mOwner(owner)
{
}

//@Override
Boolean AwContentViewClient::AwContentVideoViewClient::OnShowCustomView(
    /* [in] */ IView* view)
{
    AutoPtr<IWebChromeClientCustomViewCallback> cb = new InnerWebChromeClientCustomViewCallback(this);
    // TODO(igsolla): remove the legacy path (kept as a fallback if things go awry).
    if (!AreHtmlControlsEnabled()) {
        OnShowCustomViewLegacy(view, cb);
    }
    else {
        OnShowCustomView(view, cb);
    }
    return TRUE;
}

void AwContentViewClient::AwContentVideoViewClient::OnShowCustomViewLegacy(
    /* [in] */ IView* view,
    /* [in] */ IWebChromeClientCustomViewCallback* cb)
{
    mAwContentsClient->OnShowCustomView(view, cb);
}

void AwContentViewClient::AwContentVideoViewClient::OnShowCustomView(
    /* [in] */ IView* view,
    /* [in] */ IWebChromeClientCustomViewCallback* cb)
{
    AutoPtr<IFrameLayout> viewGroup;
    CFrameLayout::New(mContext, (IFrameLayout**)&viewGroup);
    viewGroup->AddView(view);
    AutoPtr<IViewOnAttachStateChangeListener> listener = new InnerViewOnAttachStateChangeListener(this, viewGroup);
    viewGroup->AddOnAttachStateChangeListener(listener);
    mAwContentsClient->OnShowCustomView(viewGroup, cb);
}

//@Override
void AwContentViewClient::AwContentVideoViewClient::OnDestroyContentVideoView()
{
    if (AreHtmlControlsEnabled()) {
        mAwContents->ExitFullScreen();
    }
    mAwContentsClient->OnHideCustomView();
}

//@Override
AutoPtr<IView> AwContentViewClient::AwContentVideoViewClient::GetVideoLoadingProgressView()
{
    return mAwContentsClient->GetVideoLoadingProgressView();
}

//===============================================================
//                    AwContentViewClient
//===============================================================

AwContentViewClient::AwContentViewClient(
    /* [in] */ AwContentsClient* awContentsClient,
    /* [in] */ AwSettings* awSettings,
    /* [in] */ AwContents* awContents,
    /* [in] */ IContext* context)
    : mAwContentsClient(awContentsClient)
    , mAwSettings(awSettings)
    , mAwContents(awContents)
    , mContext(context)
{
}

//@Override
void AwContentViewClient::OnBackgroundColorChanged(
    /* [in] */ Int32 color)
{
    mAwContentsClient->OnBackgroundColorChanged(color);
}

//@Override
void AwContentViewClient::OnStartContentIntent(
    /* [in] */ IContext* context,
    /* [in] */ String contentUrl)
{
    //  Callback when detecting a click on a content link.
    mAwContentsClient->ShouldOverrideUrlLoading(contentUrl);
}

//@Override
void AwContentViewClient::OnUpdateTitle(
    /* [in] */ String title)
{
    mAwContentsClient->OnReceivedTitle(title);
}

//@Override
Boolean AwContentViewClient::ShouldOverrideKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return mAwContentsClient->ShouldOverrideKeyEvent(event);
}

//@Override
AutoPtr<ContentVideoViewClient> AwContentViewClient::GetContentVideoViewClient()
{
    AutoPtr<ContentVideoViewClient> cvvc = new AwContentVideoViewClient();
    return cvvc;
}

//@Override
Boolean AwContentViewClient::ShouldBlockMediaRequest(
    /* [in] */ String url)
{
    return mAwSettings != NULL ?
            mAwSettings->GetBlockNetworkLoads() && URLUtil::IsNetworkUrl(url) : TRUE;
}

Boolean AwContentViewClient::AreHtmlControlsEnabled()
{
    return !CommandLine::GetInstance()->HasSwitch(
            ContentSwitches::DISABLE_OVERLAY_FULLSCREEN_VIDEO_SUBTITLE);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos