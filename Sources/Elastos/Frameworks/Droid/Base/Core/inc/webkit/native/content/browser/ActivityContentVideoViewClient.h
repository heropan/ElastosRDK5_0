
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_ACTIVITYCONTENTVIDEOVIEWCLIENT_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_ACTIVITYCONTENTVIDEOVIEWCLIENT_H__

// import android.annotation.SuppressLint;
// import android.app.Activity;
// import android.os.Build;
// import android.view.Gravity;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.WindowManager;
// import android.widget.FrameLayout;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Uses an existing Activity to handle displaying video in full screen.
 */
class ActivityContentVideoViewClient
    : public Object
    , public ContentVideoViewClient
{
public:
    ActivityContentVideoViewClient(
        /* [in] */ IActivity* activity);

    //@Override
    CARAPI_(Boolean) OnShowCustomView(
        /* [in] */ IView* view);

    //@Override
    CARAPI_(void) OnDestroyContentVideoView();

    //@Override
    CARAPI_(AutoPtr<IView>) GetVideoLoadingProgressView();

private:
    /**
     * Returns the system ui visibility after entering or exiting fullscreen.
     * @param view The decor view belongs to the activity window
     * @param enterFullscreen True if video is going fullscreen, or false otherwise.
     */
    //@SuppressLint("InlinedApi")
    CARAPI_(void) SetSystemUiVisibility(
        /* [in] */ IView* view,
        /* [in] */ Boolean enterFullscreen);

private:
    const AutoPtr<IActivity> mActivity;
    AutoPtr<IView> mView;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_ACTIVITYCONTENTVIDEOVIEWCLIENT_H__
