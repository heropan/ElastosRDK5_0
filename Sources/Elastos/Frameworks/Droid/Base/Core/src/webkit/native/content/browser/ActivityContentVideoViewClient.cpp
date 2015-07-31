
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

ActivityContentVideoViewClient::ActivityContentVideoViewClient(
    /* [in] */ IActivity* activity)
    : mActivity(activity)
{
}

//@Override
Boolean ActivityContentVideoViewClient::OnShowCustomView(
    /* [in] */ IView* view)
{
    AutoPtr<IWindowManager> wm;
    mActivity->GetWindow((IWindowManager**)&wm);
    AutoPtr<IFrameLayout> decor;
    wm->GetDecorView((IFrameLayout**)&decor);
    AutoPtr<IFrameLayoutLayoutParams> params;
    CFrameLayoutLayoutParams::New(
        IViewGroupLayoutParams::MATCH_PARENT,
        IViewGroupLayoutParams::MATCH_PARENT,
        IGravity::CENTER,
        (IFrameLayoutLayoutParams**)&params);
    decor->AddView(view, 0, params);
    SetSystemUiVisibility(decor, TRUE);
    mView = view;
    return TRUE;
}

//@Override
void ActivityContentVideoViewClient::OnDestroyContentVideoView()
{
    AutoPtr<IWindowManager> wm;
    mActivity->GetWindow((IWindowManager**)&wm);
    AutoPtr<IFrameLayout> decor;
    wm->GetDecorView((IFrameLayout**)&decor);
    decor->RemoveView(mView);
    SetSystemUiVisibility(decor, FALSE);
    mView = NULL;
}

//@Override
AutoPtr<IView> ActivityContentVideoViewClient::GetVideoLoadingProgressView()
{
    return NULL;
}

/**
 * Returns the system ui visibility after entering or exiting fullscreen.
 * @param view The decor view belongs to the activity window
 * @param enterFullscreen True if video is going fullscreen, or false otherwise.
 */
//@SuppressLint("InlinedApi")
void ActivityContentVideoViewClient::SetSystemUiVisibility(
    /* [in] */ IView* view,
    /* [in] */ Boolean enterFullscreen)
{
    AutoPtr<IWindowManager> wm;
    mActivity->GetWindow((IWindowManager**)&wm);
    if (enterFullscreen) {
        wm->SetFlags(
                IWindowManagerLayoutParams::FLAG_FULLSCREEN,
                IWindowManagerLayoutParams::FLAG_FULLSCREEN);
    }
    else {
        wm->ClearFlags(IWindowManagerLayoutParams::FLAG_FULLSCREEN);
    }

    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT) {
        return;
    }

    Int32 systemUiVisibility;
    view->GetSystemUiVisibility(&systemUiVisibility);
    Int32 flags = IView::SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | IView::SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            | IView::SYSTEM_UI_FLAG_HIDE_NAVIGATION
            | IView::SYSTEM_UI_FLAG_FULLSCREEN
            | IView::SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
    if (enterFullscreen) {
        systemUiVisibility |= flags;
    }
    else {
        systemUiVisibility &= ~flags;
    }

    view->SetSystemUiVisibility(systemUiVisibility);
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
