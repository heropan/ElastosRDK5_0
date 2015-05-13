
#include "webkit/WebViewFragment.h"
#include "webkit/CWebView.h"

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::EIID_IFragment;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                   InnerWebViewFragment
//===============================================================
InnerWebViewFragment::InnerWebViewFragment()
{
}

IFRAGMENT_METHODS_IMPL(InnerWebViewFragment, Fragment);

//===============================================================
//                      WebViewFragment
//===============================================================
WebViewFragment::WebViewFragment()
{
}

/**
 * Called to instantiate the view. Creates and returns the WebView.
 */
ECode WebViewFragment::OnCreateView(
    /* [in] */ ILayoutInflater* inflater,
    /* [in] */ IViewGroup* container,
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    if (mWebView != NULL) {
        mWebView->Destroy();
    }
    AutoPtr<IActivity> activity;
    GetActivity((IActivity**)&activity);
    mWebView = NULL;
    CWebView::New(activity, (IWebView**)&mWebView);
    mIsWebViewAvailable = TRUE;
    *view = (IView*)mWebView.Get();
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

/**
 * Called when the fragment is visible to the user and actively running. Resumes the WebView.
 */
ECode WebViewFragment::OnPause()
{
    InnerWebViewFragment::OnPause();
    mWebView->OnPause();
    return NOERROR;
}

/**
 * Called when the fragment is no longer resumed. Pauses the WebView.
 */
ECode WebViewFragment::OnResume()
{
    mWebView->OnResume();
    InnerWebViewFragment::OnResume();
    return NOERROR;
}

/**
 * Called when the WebView has been detached from the fragment.
 * The WebView is no longer available after this time.
 */
ECode WebViewFragment::OnDestroyView()
{
    mIsWebViewAvailable = FALSE;
    InnerWebViewFragment::OnDestroyView();
    return NOERROR;
}

/**
 * Called when the fragment is no longer in use. Destroys the internal state of the WebView.
 */
ECode WebViewFragment::OnDestroy()
{
    if (mWebView != NULL) {
        mWebView->Destroy();
        mWebView = NULL;
    }
    InnerWebViewFragment::OnDestroy();
    return NOERROR;
}

/**
 * Gets the WebView.
 */
AutoPtr<IWebView> WebViewFragment::GetWebView()
{
    return mIsWebViewAvailable ? mWebView : NULL;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
