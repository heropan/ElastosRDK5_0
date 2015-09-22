
#include "wm/AppWindowToken.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::StringBuilder;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

AppWindowToken::AppWindowToken(
    /* [in] */ CWindowManagerService* service,
    /* [in] */ Int32 userId,
    /* [in] */ IApplicationToken* token)
    : WindowToken(service, IBinder::Probe(token), IWindowManagerLayoutParams::TYPE_APPLICATION, TRUE)
    , mUserId(userId)
    , mAppToken(token)
    , mGroupId(-1)
    , mAppFullscreen(FALSE)
    , mRequestedOrientation(IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED)
    , mShowWhenLocked(FALSE)
    , mInputDispatchingTimeoutNanos(0)
    , mLastTransactionSequence(0)
    , mNumInterestingWindows(0)
    , mNumDrawnWindows(0)
    , mInPendingTransaction(FALSE)
    , mAllDrawn(FALSE)
    , mWillBeHidden(FALSE)
    , mHiddenRequested(FALSE)
    , mClientHidden(FALSE)
    , mReportedVisible(FALSE)
    , mReportedDrawn(FALSE)
    , mRemoved(FALSE)
    , mStartingDisplayed(FALSE)
    , mStartingMoved(FALSE)
    , mFirstWindowDrawn(FALSE)
{
    mAppWindowToken = this;
    mInputApplicationHandle = new InputApplicationHandle(this);
    mAnimator = service->mAnimator;
}

ECode AppWindowToken::Init()
{
    mAppAnimator = new AppWindowAnimator(this);
    return NOERROR;
}

void AppWindowToken::SendAppVisibilityToClients()
{
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mAllAppWindows.Begin(); it != mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        if (win == mStartingWindow && mClientHidden) {
            // Don't hide the starting window.
            continue;
        }
        // try {
        // if (CWindowManagerService::DEBUG_VISIBILITY) {
        //     Slogger::V(CWindowManagerService::TAG,
        //         "Setting visibility of %p: %d", win, (Int32)!clientHidden));
        // }
        win->mClient->DispatchAppVisibility(!mClientHidden);
        // } catch (RemoteException e) {
        // }
    }
}

void AppWindowToken::UpdateReportedVisibilityLocked()
{
    if (mAppToken == NULL) {
        return;
    }

    Int32 numInteresting = 0;
    Int32 numVisible = 0;
    Int32 numDrawn = 0;
    Boolean nowGone = TRUE;

    // if (CWindowManagerService::DEBUG_VISIBILITY) {
    //     Slogger::V(CWindowManagerService::TAG, "Update reported visibility: %p", this);
    // }
    List<AutoPtr<WindowState> >::Iterator it;
    for (it = mAllAppWindows.Begin(); it != mAllAppWindows.End(); ++it) {
        AutoPtr<WindowState> win = *it;
        Int32 winType;
        win->mAttrs->GetType(&winType);
        if (win == mStartingWindow || win->mAppFreezing
                || win->mViewVisibility != IView::VISIBLE
                || winType == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING
                || win->mDestroying) {
            continue;
        }
        // if (WindowManagerService.DEBUG_VISIBILITY) {
        //     Slog.v(WindowManagerService.TAG, "Win " + win + ": isDrawn="
        //             + win.isDrawnLw()
        //             + ", isAnimating=" + win.mWinAnimator.isAnimating());
        //     if (!win.isDrawnLw()) {
        //         Slog.v(WindowManagerService.TAG, "Not displayed: s=" + win.mWinAnimator.mSurface
        //                 + " pv=" + win.mPolicyVisibility
        //                 + " mDrawState=" + win.mWinAnimator.mDrawState
        //                 + " ah=" + win.mAttachedHidden
        //                 + " th="
        //                 + (win.mAppToken != null
        //                         ? win.mAppToken.hiddenRequested : false)
        //                 + " a=" + win.mWinAnimator.mAnimating);
        //     }
        // }
        numInteresting++;
        if (win->IsDrawnLw()) {
            numDrawn++;
            if (!win->mWinAnimator->IsAnimating()) {
                numVisible++;
            }
            nowGone = FALSE;
        }
        else if (win->mWinAnimator->IsAnimating()) {
            nowGone = FALSE;
        }
    }

    Boolean nowDrawn = numInteresting > 0 && numDrawn >= numInteresting;
    Boolean nowVisible = numInteresting > 0 && numVisible >= numInteresting;
    if (!nowGone) {
        // If the app is not yet gone, then it can only become visible/drawn.
        if (!nowDrawn) {
            nowDrawn = mReportedDrawn;
        }
        if (!nowVisible) {
            nowVisible = mReportedVisible;
        }
    }
    // if (WindowManagerService.DEBUG_VISIBILITY) Slog.v(WindowManagerService.TAG, "VIS " + this + ": interesting="
    //         + numInteresting + " visible=" + numVisible);
    if (nowDrawn != mReportedDrawn) {
        if (nowDrawn) {
            AutoPtr<IMessage> msg;
            mService->mH->ObtainMessage(CWindowManagerService::H::REPORT_APPLICATION_TOKEN_DRAWN,
                this, (IMessage**)&msg);
            Boolean result;
            mService->mH->SendMessage(msg, &result);
        }
        mReportedDrawn = nowDrawn;
    }
    if (nowVisible != mReportedVisible) {
        // if (WindowManagerService.DEBUG_VISIBILITY) Slog.v(
        //         WindowManagerService.TAG, "Visibility changed in " + this
        //         + ": vis=" + nowVisible);
        mReportedVisible = nowVisible;
        AutoPtr<IMessage> msg;
        mService->mH->ObtainMessage(CWindowManagerService::H::REPORT_APPLICATION_TOKEN_WINDOWS,
            nowVisible ? 1 : 0, nowGone ? 1 : 0, this, (IMessage**)&msg);
        Boolean result;
        mService->mH->SendMessage(msg, &result);
    }
}

AutoPtr<WindowState> AppWindowToken::FindMainWindow()
{
    List<AutoPtr<WindowState> >::ReverseIterator rit = mWindows.RBegin();
    while (rit != mWindows.REnd()) {
        AutoPtr<WindowState> win = *rit;
        Int32 winType;
        win->mAttrs->GetType(&winType);
        if (winType == IWindowManagerLayoutParams::TYPE_BASE_APPLICATION
                || winType == IWindowManagerLayoutParams::TYPE_APPLICATION_STARTING) {
            return win;
        }
        ++rit;
    }
    return NULL;
}

String AppWindowToken::ToString()
{
    if (mStringName.IsNull()) {
        StringBuilder sb;
        sb.Append("AppWindowToken{");
        sb.Append((Int32)this);
        sb.Append(" token=");
        String info;
        mToken->ToString(&info);
        sb.Append(info);
        if (mStartingWindow != NULL) {
            sb.Append(" startingWindow=");
            sb.Append(mStartingWindow->ToString());
        }
        sb.AppendChar('}');
        sb.ToString(&mStringName);
    }
    return mStringName;
}

} // Wm
} // Server
} // Droid
} // Elastos
