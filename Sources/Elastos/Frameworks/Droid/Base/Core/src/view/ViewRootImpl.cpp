
#include "view/ViewRootImpl.h"
#include "view/CViewRootImplW.h"
#include "view/CInputDevice.h"
#include "view/CInputChannel.h"
#include "view/FocusFinder.h"
#include "view/CKeyEvent.h"
#include "view/CWindowManagerGlobal.h"
#include "view/CCompatibilityInfoHolder.h"
#include "view/CAccessibilityInteractionConnection.h"
#include "view/CAccessibilityInteractionController.h"
#include "view/ViewTreeObserver.h"
#include "view/Choreographer.h"
#include "view/ViewGroup.h"
#include "view/CKeyCharacterMap.h"
#include "view/SurfaceView.h"
#include "view/SoundEffectConstants.h"
#include "view/inputmethod/CInputMethodManager.h"
#include "view/animation/CAccelerateDecelerateInterpolator.h"
#include "view/accessibility/CAccessibilityManager.h"
#include "view/accessibility/CAccessibilityNodeInfo.h"
#include "impl/CPolicyManager.h"
#include "graphics/CPixelFormat.h"
#include "graphics/CPaint.h"
#include "graphics/CPoint.h"
#include "graphics/CPointF.h"
#include "content/res/CCompatibilityInfo.h"
#include "content/res/CConfiguration.h"
//#include "widget/Scroller.h"
#include "os/Build.h"
#include "os/SystemClock.h"
#include "os/SystemProperties.h"
#include "os/CSystemProperties.h"
#include "os/Process.h"
#include "os/Binder.h"
#include "os/SomeArgs.h"
#include "util/CTypedValue.h"
#include <elastos/Thread.h>
#include <elastos/Math.h>
#include <elastos/Logger.h>
#include <elastos/Slogger.h>
#include "R.h"
#include <stdio.h>
#include "opengl/CGLES20.h"

#ifndef TRACE_IN_TERMINAL
#define TRACE_IN_TERMINAL          0
#endif

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Droid::Opengl::CGLES20;
using Elastos::Core::CStringWrapper;
using Elastos::Core::Threading::Thread;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Content::Res::CCompatibilityInfo;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::View::Animation::CAccelerateDecelerateInterpolator;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityNodeInfo;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityStateChangeListener;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityInteractionConnection;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::View::InputMethod::EIID_IInputMethodManagerFinishedEventCallback;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::SomeArgs;
using Elastos::Droid::Internal::Policy::IPolicyManager;
using Elastos::Droid::Internal::Policy::CPolicyManager;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Utility::HashSet;
using Elastos::Utility::Vector;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::Animation::IAccelerateDecelerateInterpolator;

namespace Elastos {
namespace Droid {
namespace View {

static void LocalTrace(IView* view, const char* info)
{
#if TRACE_IN_TERMINAL
    Int32 viewId;
    view->GetId(&viewId);
    if (viewId != 0x7f0700a2 /* content_parent */) {
        printf("%s\n", info);
        Logger::D("ViewRootImpl", "%s view:%08x", info, viewId);
    }
#endif
}

extern "C" const InterfaceID EIID_ViewRootImpl =
        {0xcccccccc,0xcccc,0xcccc,{0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc}};

const Int32 ViewRootImpl::QueuedInputEvent::FLAG_DELIVER_POST_IME;

const Int32 ViewRootImpl::MSG_INVALIDATE = 1;
const Int32 ViewRootImpl::MSG_INVALIDATE_RECT = 2;
const Int32 ViewRootImpl::MSG_DIE = 3;
const Int32 ViewRootImpl::MSG_RESIZED = 4;
const Int32 ViewRootImpl::MSG_RESIZED_REPORT = 5;
const Int32 ViewRootImpl::MSG_WINDOW_FOCUS_CHANGED = 6;
const Int32 ViewRootImpl::MSG_DISPATCH_KEY = 7;
const Int32 ViewRootImpl::MSG_DISPATCH_APP_VISIBILITY = 8;
const Int32 ViewRootImpl::MSG_DISPATCH_GET_NEW_SURFACE = 9;
const Int32 ViewRootImpl::MSG_IME_FINISHED_EVENT = 10;
const Int32 ViewRootImpl::MSG_DISPATCH_KEY_FROM_IME = 11;
const Int32 ViewRootImpl::MSG_FINISH_INPUT_CONNECTION = 12;
const Int32 ViewRootImpl::MSG_CHECK_FOCUS = 13;
const Int32 ViewRootImpl::MSG_CLOSE_SYSTEM_DIALOGS = 14;
const Int32 ViewRootImpl::MSG_DISPATCH_DRAG_EVENT = 15;
const Int32 ViewRootImpl::MSG_DISPATCH_DRAG_LOCATION_EVENT = 16;
const Int32 ViewRootImpl::MSG_DISPATCH_SYSTEM_UI_VISIBILITY = 17;
const Int32 ViewRootImpl::MSG_UPDATE_CONFIGURATION = 18;
const Int32 ViewRootImpl::MSG_PROCESS_INPUT_EVENTS = 19;
const Int32 ViewRootImpl::MSG_DISPATCH_SCREEN_STATE = 20;
const Int32 ViewRootImpl::MSG_INVALIDATE_DISPLAY_LIST = 21;
const Int32 ViewRootImpl::MSG_CLEAR_ACCESSIBILITY_FOCUS_HOST = 22;
const Int32 ViewRootImpl::MSG_DISPATCH_DONE_ANIMATING = 23;
const Int32 ViewRootImpl::MSG_INVALIDATE_WORLD = 24;
const Int32 ViewRootImpl::MSG_WINDOW_MOVED = 25;

CAR_INTERFACE_IMPL(ViewRootImpl::SystemUiVisibilityInfo, IInterface)

//=======================================================================================
// ViewRootImpl::ViewRootHandler
//=======================================================================================
ViewRootImpl::ViewRootHandler::ViewRootHandler(
    /* [in] */ ViewRootImpl* host)
{
    host->GetWeakReference((IWeakReference**)&mWeakHost);
}

String ViewRootImpl::ViewRootHandler::GetMessageNameImpl(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch (what) {
        case ViewRootImpl::MSG_INVALIDATE:
            return String("MSG_INVALIDATE");
        case ViewRootImpl::MSG_INVALIDATE_RECT:
            return String("MSG_INVALIDATE_RECT");
        case ViewRootImpl::MSG_DIE:
            return String("MSG_DIE");
        case ViewRootImpl::MSG_RESIZED:
            return String("MSG_RESIZED");
        case ViewRootImpl::MSG_RESIZED_REPORT:
            return String("MSG_RESIZED_REPORT");
        case ViewRootImpl::MSG_WINDOW_FOCUS_CHANGED:
            return String("MSG_WINDOW_FOCUS_CHANGED");
        case ViewRootImpl::MSG_DISPATCH_KEY:
            return String("MSG_DISPATCH_KEY");
        case ViewRootImpl::MSG_DISPATCH_APP_VISIBILITY:
            return String("MSG_DISPATCH_APP_VISIBILITY");
        case ViewRootImpl::MSG_DISPATCH_GET_NEW_SURFACE:
            return String("MSG_DISPATCH_GET_NEW_SURFACE");
        case ViewRootImpl::MSG_IME_FINISHED_EVENT:
            return String("MSG_IME_FINISHED_EVENT");
        case ViewRootImpl::MSG_DISPATCH_KEY_FROM_IME:
            return String("MSG_DISPATCH_KEY_FROM_IME");
        case ViewRootImpl::MSG_FINISH_INPUT_CONNECTION:
            return String("MSG_FINISH_INPUT_CONNECTION");
        case ViewRootImpl::MSG_CHECK_FOCUS:
            return String("MSG_CHECK_FOCUS");
        case ViewRootImpl::MSG_CLOSE_SYSTEM_DIALOGS:
            return String("MSG_CLOSE_SYSTEM_DIALOGS");
        case ViewRootImpl::MSG_DISPATCH_DRAG_EVENT:
            return String("MSG_DISPATCH_DRAG_EVENT");
        case ViewRootImpl::MSG_DISPATCH_DRAG_LOCATION_EVENT:
            return String("MSG_DISPATCH_DRAG_LOCATION_EVENT");
        case ViewRootImpl::MSG_DISPATCH_SYSTEM_UI_VISIBILITY:
            return String("MSG_DISPATCH_SYSTEM_UI_VISIBILITY");
        case ViewRootImpl::MSG_UPDATE_CONFIGURATION:
            return String("MSG_UPDATE_CONFIGURATION");
        case ViewRootImpl::MSG_PROCESS_INPUT_EVENTS:
            return String("MSG_PROCESS_INPUT_EVENTS");
        case ViewRootImpl::MSG_DISPATCH_SCREEN_STATE:
            return String("MSG_DISPATCH_SCREEN_STATE");
        case ViewRootImpl::MSG_INVALIDATE_DISPLAY_LIST:
            return String("MSG_INVALIDATE_DISPLAY_LIST");
        case ViewRootImpl::MSG_CLEAR_ACCESSIBILITY_FOCUS_HOST:
            return String("MSG_CLEAR_ACCESSIBILITY_FOCUS_HOST");
        case ViewRootImpl::MSG_DISPATCH_DONE_ANIMATING:
            return String("MSG_DISPATCH_DONE_ANIMATING");
        case ViewRootImpl::MSG_WINDOW_MOVED:
            return String("MSG_WINDOW_MOVED");
    }
    return HandlerBase::GetMessageNameImpl(msg);
}

ECode ViewRootImpl::ViewRootHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IViewParent> hostObj;
    mWeakHost->Resolve(EIID_IViewParent, (IInterface**)&hostObj);
    if (NULL == hostObj.Get()) {
        if (ViewRootImpl::LOCAL_LOGV) {
            String msgInfo = GetMessageNameImpl(msg);
            Slogger::D(ViewRootImpl::TAG, "ViewRootImpl has been destroyed, ignore message: %s.",
                msgInfo.string());
        }
        return NOERROR;
    }

    AutoPtr<ViewRootImpl> mHost = (ViewRootImpl*)hostObj.Get();

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case ViewRootImpl::MSG_INVALIDATE: {
            IView* view = IView::Probe(obj);
            view->InvalidateEx2();
            break;
        }
        case ViewRootImpl::MSG_INVALIDATE_RECT: {
            View::AttachInfo::InvalidateInfo* info = (View::AttachInfo::InvalidateInfo*)obj.Get();
            info->mTarget->InvalidateEx(info->mLeft, info->mTop, info->mRight, info->mBottom);
            info->ReleaseInfo();
            break;
        }
        case ViewRootImpl::MSG_IME_FINISHED_EVENT: {
            mHost->HandleImeFinishedEvent(arg1, arg2 != 0);
            break;
        }
        case ViewRootImpl::MSG_PROCESS_INPUT_EVENTS: {
            mHost->mProcessInputEventsScheduled = FALSE;
            mHost->DoProcessInputEvents();
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_APP_VISIBILITY: {
            mHost->HandleAppVisibility(arg1 != 0);
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_GET_NEW_SURFACE: {
            mHost->HandleGetNewSurface();
            break;
        }
        case ViewRootImpl::MSG_RESIZED:  {
            // Recycled in the fall through...
            AutoPtr<SomeArgs> args = (SomeArgs*)obj.Get();
            Boolean bval;
            if ((mHost->mWinFrame->Equals(args->mArg1, &bval), bval)
                    && (mHost->mPendingContentInsets->Equals(args->mArg2, &bval), bval)
                    && (mHost->mPendingVisibleInsets->Equals(args->mArg3, &bval), bval)
                    && args->mArg4 == NULL) {
                break;
            }
        } // fall through...
        case ViewRootImpl::MSG_RESIZED_REPORT: {
            if (mHost->mAdded) {
                AutoPtr<SomeArgs> args = (SomeArgs*)obj.Get();

                IConfiguration* config = IConfiguration::Probe(args->mArg4);
                if (config != NULL) {
                    mHost->UpdateConfiguration(config, FALSE);
                }

                mHost->mWinFrame->SetEx(IRect::Probe(args->mArg1));
                mHost->mPendingContentInsets->SetEx(IRect::Probe(args->mArg2));
                mHost->mPendingVisibleInsets->SetEx(IRect::Probe(args->mArg3));

                args->Recycle();

                if (what == MSG_RESIZED_REPORT) {
                    mHost->mReportNextDraw = TRUE;
                }

                if (mHost->mView != NULL) {
                    mHost->ForceLayout(mHost->mView);
                }

                mHost->RequestLayout();
            }
            break;
        }
        case ViewRootImpl::MSG_WINDOW_MOVED: {
            if (mHost->mAdded) {
                Int32 w, h;
                mHost->mWinFrame->GetWidth(&w);
                mHost->mWinFrame->GetHeight(&h);
                Int32 l = arg1;
                Int32 t = arg2;
                mHost->mWinFrame->Set(l, t, l + w, t + h);

                if (mHost->mView != NULL) {
                    mHost->ForceLayout(mHost->mView);
                }
                mHost->RequestLayout();
            }
            break;
        }
        case ViewRootImpl::MSG_WINDOW_FOCUS_CHANGED: {
            Boolean hasWindowFocus = arg1 != 0;
            Boolean inTouchMode = arg2 != 0;
            mHost->HandleWindowFocusChanged(hasWindowFocus, inTouchMode);
            break;
        }
        case ViewRootImpl::MSG_DIE: {
            mHost->DoDie();
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_KEY: {
            IKeyEvent* event = IKeyEvent::Probe(obj);
            mHost->EnqueueInputEvent(event, NULL, 0, TRUE);
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_KEY_FROM_IME: {
            IKeyEvent* event = IKeyEvent::Probe(obj);

            if (ViewRootImpl::LOCAL_LOGV) {
                Logger::V(ViewRootImpl::TAG, "Dispatching key 0x%08x  from IME to 0x%08x",
                    event, mHost->mView.Get());
            }

            Int32 flags;
            event->GetFlags(&flags);
            AutoPtr<IKeyEvent> newEvent;
            if ((flags & IKeyEvent::FLAG_FROM_SYSTEM) != 0) {
                // The IME is trying to say this event is from the
                // system!  Bad bad bad!
                //noinspection UnusedAssignment
                CKeyEvent::ChangeFlags(
                    event, flags & ~IKeyEvent::FLAG_FROM_SYSTEM, (IKeyEvent**)&newEvent);
            }
            else {
                newEvent = event;
            }

            mHost->EnqueueInputEvent(newEvent, NULL, QueuedInputEvent::FLAG_DELIVER_POST_IME, TRUE);
            break;
        }
        case ViewRootImpl::MSG_FINISH_INPUT_CONNECTION: {
            AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL) {
                imm->ReportFinishInputConnection(IInputConnection::Probe(obj));
            }
            break;
        }
        case ViewRootImpl::MSG_CHECK_FOCUS: {
            AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL) {
                imm->CheckFocus();
            }
            break;
        }
        case ViewRootImpl::MSG_CLOSE_SYSTEM_DIALOGS: {
            if (mHost->mView != NULL) {
                ICharSequence* seq = ICharSequence::Probe(obj);
                String info;
                seq->ToString(&info);
                mHost->mView->OnCloseSystemDialogs(info);
            }
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_DRAG_EVENT:
        case ViewRootImpl::MSG_DISPATCH_DRAG_LOCATION_EVENT: {
            IDragEvent* event = IDragEvent::Probe(obj);
            event->SetLocalState(mHost->mLocalDragState);    // only present when this app called startDrag()
            mHost->HandleDragEvent(event);
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_SYSTEM_UI_VISIBILITY: {
            SystemUiVisibilityInfo* info = (SystemUiVisibilityInfo*)obj.Get();
            mHost->HandleDispatchSystemUiVisibilityChanged(info);
            break;
        }
        case ViewRootImpl::MSG_UPDATE_CONFIGURATION: {
            AutoPtr<IConfiguration> config = IConfiguration::Probe(obj);
            Boolean result;
            if (config->IsOtherSeqNewer(mHost->mLastConfiguration, &result), result) {
                config = mHost->mLastConfiguration;
            }
            mHost->UpdateConfiguration(config, FALSE);
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_SCREEN_STATE: {
            if (mHost->mView != NULL) {
                mHost->HandleScreenStateChange(arg1 == 1);
            }
            break;
        }
        case ViewRootImpl::MSG_INVALIDATE_DISPLAY_LIST: {
            mHost->InvalidateDisplayLists();
            break;
        }
        case ViewRootImpl::MSG_CLEAR_ACCESSIBILITY_FOCUS_HOST: {
            mHost->SetAccessibilityFocus(NULL, NULL);
            break;
        }
        case ViewRootImpl::MSG_DISPATCH_DONE_ANIMATING: {
            mHost->HandleDispatchDoneAnimating();
            break;
        }
        case ViewRootImpl::MSG_INVALIDATE_WORLD: {
            if (mHost->mView != NULL) {
                mHost->InvalidateWorld(mHost->mView);
            }
            break;
        }
    }
    return NOERROR;
}

//=======================================================================================
// ViewRootImpl::TraversalRunnable
//=======================================================================================
ViewRootImpl::TraversalRunnable::TraversalRunnable(
    /* [in] */ IWeakReference* host)
    : mHost(host)
{}

ECode ViewRootImpl::TraversalRunnable::Run()
{
    AutoPtr<IInterface> obj;
    mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        AutoPtr<ViewRootImpl> viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
        viewRoot->DoTraversal();
    }

    return NOERROR;
}

//=======================================================================================
// ViewRootImpl::WindowInputEventReceiver
//=======================================================================================
ViewRootImpl::WindowInputEventReceiver::WindowInputEventReceiver(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ ILooper* looper,
    /* [in] */ IWeakReference* viewRootImpl)
    : InputEventReceiver(inputChannel, looper)
    , mHost(viewRootImpl)
{}

ViewRootImpl::WindowInputEventReceiver::OnInputEvent(
    /* [in] */ IInputEvent* event)
{
    if (mHost != NULL) {
        AutoPtr<IInterface> obj;
        mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
        if (obj) {
            AutoPtr<ViewRootImpl> viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
            viewRoot->EnqueueInputEvent(event, this, 0, TRUE);
        }
    }

    return NOERROR;
}

ViewRootImpl::WindowInputEventReceiver::OnBatchedInputEventPending()
{
    if (mHost != NULL) {
        AutoPtr<IInterface> obj;
        mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
        if (obj) {
            AutoPtr<ViewRootImpl> viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
            viewRoot->ScheduleConsumeBatchedInput();
        }
    }

    return NOERROR;
}

ViewRootImpl::WindowInputEventReceiver::Dispose()
{
    if (mHost != NULL) {
        AutoPtr<IInterface> obj;
        mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
        if (obj) {
            AutoPtr<ViewRootImpl> viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
            viewRoot->UnscheduleConsumeBatchedInput();
            mHost = NULL;
        }
    }

    return InputEventReceiver::Dispose();
}

//=======================================================================================
// ViewRootImpl::ConsumeBatchedInputRunnable
//=======================================================================================
ViewRootImpl::ConsumeBatchedInputRunnable::ConsumeBatchedInputRunnable(
    /* [in] */ IWeakReference* host)
    : mHost(host)
{}

ECode ViewRootImpl::ConsumeBatchedInputRunnable::Run()
{
    AutoPtr<IInterface> obj;
    mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        Int64 timeNanos;
        AutoPtr<ViewRootImpl> viewRoot;
        viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
        viewRoot->mChoreographer->GetFrameTimeNanos(&timeNanos);
        viewRoot->DoConsumeBatchedInput(timeNanos);
    }

    return NOERROR;
}

//=======================================================================================
// ViewRootImpl::InvalidateOnAnimationRunnable
//=======================================================================================
ViewRootImpl::InvalidateOnAnimationRunnable::InvalidateOnAnimationRunnable(
    /* [in] */ IWeakReference* host)
    : mPosted(FALSE)
    , mHost(host)
{}

void ViewRootImpl::InvalidateOnAnimationRunnable::AddView(
    /* [in] */ IView* view)
{
    Mutex::Autolock lock(mSelfLock);
    mViews.PushBack(view);
    PostIfNeededLocked();
}

void ViewRootImpl::InvalidateOnAnimationRunnable::AddViewRect(
    /* [in] */ View::AttachInfo::InvalidateInfo* info)
{
    Mutex::Autolock lock(mSelfLock);
    mViewRects.PushBack(info);
    PostIfNeededLocked();
}

void ViewRootImpl::InvalidateOnAnimationRunnable::RemoveView(
    /* [in] */ IView* view)
{
    Mutex::Autolock lock(mSelfLock);
    mViews.Remove(view);

    typedef typename List<AutoPtr<View::AttachInfo::InvalidateInfo> >::ReverseIterator InfoReverseIterator;
    InfoReverseIterator iter;
    for (iter = mViewRects.RBegin(); iter != mViewRects.REnd();) {
        AutoPtr<View::AttachInfo::InvalidateInfo> info = *iter;
        if (info->mTarget.Get() == view) {
            info->ReleaseInfo();
            iter = InfoReverseIterator(mViewRects.Erase(--(iter.GetBase())));
        }
        else {
            ++iter;
        }
    }

    AutoPtr<IInterface> obj;
    mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (mPosted && mViews.IsEmpty() && mViewRects.IsEmpty() && obj != NULL) {
        AutoPtr<ViewRootImpl> viewRoot;
        viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
        viewRoot->mChoreographer->RemoveCallbacks(
            IChoreographer::CALLBACK_ANIMATION, this, NULL);
        mPosted = FALSE;
    }
}

//@Override
ECode ViewRootImpl::InvalidateOnAnimationRunnable::Run()
{
    Int32 viewCount;
    Int32 viewRectCount;
    {
        Mutex::Autolock lock(mSelfLock);
        mPosted = FALSE;

        viewCount = mViews.GetSize();
        if (viewCount != 0) {
            if (mTempViews == NULL || mTempViews->GetLength() < viewCount) {
                mTempViews = ArrayOf<IView*>::Alloc(viewCount);
            }

            List<AutoPtr<IView> >::Iterator iter = mViews.Begin();
            for (Int32 i = 0; iter != mViews.End(); ++iter, ++i) {
                mTempViews->Set(i, iter->Get());
            }
            mViews.Clear();
        }

        viewRectCount = mViewRects.GetSize();
        if (viewRectCount != 0) {
            if (mTempViewRects == NULL || mTempViewRects->GetLength() < viewRectCount) {
                mTempViewRects = ArrayOf<View::AttachInfo::InvalidateInfo*>::Alloc(viewRectCount);
            }

            List<AutoPtr<View::AttachInfo::InvalidateInfo> >::Iterator iter = mViewRects.Begin();
            for (Int32 i = 0; iter != mViewRects.End(); ++iter, ++i) {
                mTempViewRects->Set(i, iter->Get());
            }
            mViewRects.Clear();
        }
    }

    for (Int32 i = 0; i < viewCount; i++) {
        (*mTempViews)[i]->InvalidateEx2();
        mTempViews->Set(i, NULL);
    }

    for (Int32 i = 0; i < viewRectCount; i++) {
        View::AttachInfo::InvalidateInfo* info = (*mTempViewRects)[i];
        info->mTarget->InvalidateEx(info->mLeft, info->mTop, info->mRight, info->mBottom);
        info->ReleaseInfo();
    }

    return NOERROR;
}

void ViewRootImpl::InvalidateOnAnimationRunnable::PostIfNeededLocked()
{
    if (!mPosted) {
        AutoPtr<IInterface> obj;
        mHost->Resolve(EIID_IInterface, (IInterface**)&obj);
        if (mPosted == FALSE && obj != NULL) {
            AutoPtr<ViewRootImpl> viewRoot;
            viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
            viewRoot->mChoreographer->PostCallback(
                IChoreographer::CALLBACK_ANIMATION, this, NULL);
            mPosted = TRUE;
        }
    }
}

//=======================================================================================
// ViewRootImpl::TakenSurfaceHolder
//=======================================================================================
ViewRootImpl::TakenSurfaceHolder::TakenSurfaceHolder(
    /* [in] */ ViewRootImpl* viewRoot) :
    mViewRoot(viewRoot)
{
}

Boolean ViewRootImpl::TakenSurfaceHolder::OnAllowLockCanvas()
{
    return mViewRoot->mDrawingAllowed;
}

void ViewRootImpl::TakenSurfaceHolder::OnRelayoutContainer()
{
    // Not currently interesting -- from changing between fixed and layout size.
}

ECode ViewRootImpl::TakenSurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    assert(mViewRoot->mView.Get());
    IRootViewSurfaceTaker* rvst =
        (IRootViewSurfaceTaker*)(mViewRoot->mView)->Probe(EIID_IRootViewSurfaceTaker);
    assert(rvst);

    return rvst->SetSurfaceFormat(format);
}

ECode ViewRootImpl::TakenSurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    assert(mViewRoot->mView.Get());
    IRootViewSurfaceTaker* rvst =
        (IRootViewSurfaceTaker*)(mViewRoot->mView)->Probe(EIID_IRootViewSurfaceTaker);
    assert(rvst);

    return rvst->SetSurfaceType(type);
}

ECode ViewRootImpl::TakenSurfaceHolder::OnUpdateSurface()
{
    // We take care of format and type changes on our own.
    Logger::E(ViewRootImpl::TAG, "Shouldn't be here");

    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode ViewRootImpl::TakenSurfaceHolder::IsCreating(
    /* [out] */ Boolean* result)
{
    if (result == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *result = mViewRoot->mIsCreating;

    return NOERROR;
}

ECode ViewRootImpl::TakenSurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    Logger::E(ViewRootImpl::TAG, "Currently only support sizing from layout");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ViewRootImpl::TakenSurfaceHolder::SetKeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    assert(mViewRoot->mView.Get());
    IRootViewSurfaceTaker* rvst =
        (IRootViewSurfaceTaker*)(mViewRoot->mView)->Probe(EIID_IRootViewSurfaceTaker);
    assert(rvst);

    return rvst->SetSurfaceKeepScreenOn(screenOn);
}

//=======================================================================================
// ViewRootImpl::InputMethodCallback
//=======================================================================================
CAR_INTERFACE_IMPL(ViewRootImpl::InputMethodCallback, IInputMethodManagerFinishedEventCallback);

ViewRootImpl::InputMethodCallback::InputMethodCallback(
    /* [in] */ IWeakReference* viewAncestor)
    : mViewAncestor(viewAncestor)
{
}

//@Override
ECode ViewRootImpl::InputMethodCallback::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    AutoPtr<ViewRootImpl> viewRoot;
    AutoPtr<IInterface> obj;
    mViewAncestor->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        viewRoot = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
        viewRoot->DispatchImeFinishedEvent(seq, handled);
    }

    return NOERROR;
}

//=======================================================================================
// ViewRootImpl::TrackballAxis
//=======================================================================================
const Float ViewRootImpl::TrackballAxis::MAX_ACCELERATION;
const Float ViewRootImpl::TrackballAxis::ACCEL_MOVE_SCALING_FACTOR;

ViewRootImpl::TrackballAxis::TrackballAxis()
    : mPosition(0)
    , mAbsPosition(0)
    , mAcceleration(1)
    , mLastMoveTime(0)
    , mStep(0)
    , mDir(0)
    , mNonAccelMovement(0)
{
}

/**
 * Add trackball movement into the state.  If the direction of movement
 * has been reversed, the state is reset before adding the
 * movement (so that you don't have to compensate for any previously
 * collected movement before see the result of the movement in the
 * new direction).
 *
 * @return Returns the absolute value of the amount of movement
 * collected so far.
 */
void ViewRootImpl::TrackballAxis::Reset(
    /* [in] */ Int32 _step)
{
    mPosition = 0;
    mAcceleration = 1;
    mLastMoveTime = 0;
    mStep = _step;
    mDir = 0;
}

Float ViewRootImpl::TrackballAxis::Collect(
    /* [in] */ Float off,
    /* [in] */ Int64 time,
    /* [in] */ const char* axis)
{
    Int64 normTime;
    if (off > 0) {
        normTime = (Int64)(off * FAST_MOVE_TIME);
        if (mDir < 0) {
            if (ViewRootImpl::DEBUG_TRACKBALL)
                Logger::D(ViewRootImpl::TAG, "%s reversed to positive!", axis);
            mPosition = 0;
            mStep = 0;
            mAcceleration = 1;
            mLastMoveTime = 0;
        }
        mDir = 1;
    }
    else if (off < 0) {
        normTime = (Int64)((-off) * FAST_MOVE_TIME);
        if (mDir > 0) {
            if (ViewRootImpl::DEBUG_TRACKBALL)
                Logger::D(ViewRootImpl::TAG, "%s reversed to negative!", axis);
            mPosition = 0;
            mStep = 0;
            mAcceleration = 1;
            mLastMoveTime = 0;
        }
        mDir = -1;
    }
    else {
        normTime = 0;
    }

    // The number of milliseconds between each movement that is
    // considered "normal" and will not result in any acceleration
    // or deceleration, scaled by the offset we have here.
    if (normTime > 0) {
        Int64 delta = time - mLastMoveTime;
        mLastMoveTime = time;
        Float acc = mAcceleration;
        if (delta < normTime) {
            // The user is scrolling rapidly, so increase acceleration.
            Float scale = (normTime-delta) * ACCEL_MOVE_SCALING_FACTOR;
            if (scale > 1) acc *= scale;
            if (ViewRootImpl::DEBUG_TRACKBALL) {
                //Logger::D(ViewRootImpl::TAG, StringBuffer(axis) + " accelerate: off="
                //+ off + " normTime=" + normTime + " delta=" + delta
                //+ " scale=" + scale + " acc=" + acc);
            }
            mAcceleration = acc < MAX_ACCELERATION ? acc : MAX_ACCELERATION;
        }
        else {
            // The user is scrolling slowly, so decrease acceleration.
            Float scale = (delta-normTime) * ACCEL_MOVE_SCALING_FACTOR;
            if (scale > 1)
                acc /= scale;

            if (ViewRootImpl::DEBUG_TRACKBALL) {
                //Logger::D(ViewRootImpl::TAG, StringBuffer(axis) + " deccelerate: off="
                //    + off + " normTime=" + normTime + " delta=" + delta
                //    + " scale=" + scale + " acc=" + acc);
            }
            mAcceleration = acc > 1 ? acc : 1;
        }
    }

    mPosition += off;

    return (mAbsPosition = Elastos::Core::Math::Abs(mPosition));
}

/**
 * Generate the number of discrete movement events appropriate for
 * the currently collected trackball movement.
 *
 * @param precision The minimum movement required to generate the
 * first discrete movement.
 *
 * @return Returns the number of discrete movements, either positive
 * or negative, or 0 if there is not enough trackball movement yet
 * for a discrete movement.
 */
Int32 ViewRootImpl::TrackballAxis::Generate(
    /* [in] */ Float precision)
{
    Int32 movement = 0;
    mNonAccelMovement = 0;
    do {
        const Int32 dir = mPosition >= 0 ? 1 : -1;
        switch (mStep) {
            // If we are going to execute the first step, then we want
            // to do this as soon as possible instead of waiting for
            // a full movement, in order to make things look responsive.
        case 0:
            {
                if (mAbsPosition < precision) {
                    return movement;
                }
                movement += dir;
                mNonAccelMovement += dir;
                mStep = 1;
            }
            break;
            // If we have generated the first movement, then we need
            // to wait for the second complete trackball motion before
            // generating the second discrete movement.
        case 1:
            {
                if (mAbsPosition < 2) {
                    return movement;
                }
                movement += dir;
                mNonAccelMovement += dir;
                mPosition += dir > 0 ? -2 : 2;
                mAbsPosition = Elastos::Core::Math::Abs(mPosition);
                mStep = 2;
            }
            break;
            // After the first two, we generate discrete movements
            // consistently with the trackball, applying an acceleration
            // if the trackball is moving quickly.  This is a simple
            // acceleration on top of what we already compute based
            // on how quickly the wheel is being turned, to apply
            // a longer increasing acceleration to continuous movement
            // in one direction.
        default:
            {
                if (mAbsPosition < 1) {
                    return movement;
                }
                movement += dir;
                mPosition += dir >= 0 ? -1 : 1;
                mAbsPosition = Elastos::Core::Math::Abs(mPosition);
                Float acc = mAcceleration;
                acc *= 1.1f;
                mAcceleration = acc < MAX_ACCELERATION ? acc : mAcceleration;
            }
            break;
        }
    } while (TRUE);
}

//=======================================================================================
// ViewRootImpl::CSurfaceHolder
//=======================================================================================
ViewRootImpl::CSurfaceHolder::CSurfaceHolder(
    /* [in] */ ViewRootImpl* viewRoot)
    : mViewRoot(viewRoot)
{}

PInterface ViewRootImpl::CSurfaceHolder::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ISurfaceHolder*)this;
    }
    else if (riid == EIID_ISurfaceHolder) {
        return (ISurfaceHolder*)this;
    }
    return NULL;
}

UInt32 ViewRootImpl::CSurfaceHolder::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewRootImpl::CSurfaceHolder::Release()
{
    return ElRefBase::Release();
}

ECode ViewRootImpl::CSurfaceHolder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ISurfaceHolder*)this) {
        *pIID = EIID_ISurfaceHolder;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::GetSurface(
    /* [out] */ ISurface** surface)
{
    if (surface == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *surface = mViewRoot->mSurface;
    INTERFACE_ADDREF(*surface);

    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::IsCreating(
    /* [out] */ Boolean* result)
{
    if (result == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *result = FALSE;

    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::AddCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::RemoveCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::SetSizeFromLayout()
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::SetKeepScreenOn(
    /* [in] */ Boolean screenOn)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::LockCanvas(
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *canvas = NULL;

    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::LockCanvasEx(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *canvas = NULL;

    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::UnlockCanvasAndPost(
    /* [in] */ ICanvas* canvas)
{
    return NOERROR;
}

ECode ViewRootImpl::CSurfaceHolder::GetSurfaceFrame(
    /* [out] */ IRect** rect)
{
    if (rect == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *rect = NULL;

    return NOERROR;
}

void ViewRootImpl::RunQueue::Post(
    /* [in] */ IRunnable* action)
{
    PostDelayed(action, 0);
}

void ViewRootImpl::RunQueue::PostDelayed(
    /* [in] */ IRunnable* action,
    /* [in] */ Int32 delayMillis)
{
    AutoPtr<HandlerAction> handlerAction = new HandlerAction();
    handlerAction->mAction = action;
    handlerAction->mDelay = delayMillis;

    Mutex::Autolock lock(mLock);

    mActions.PushBack(handlerAction);
}

void ViewRootImpl::RunQueue::RemoveCallbacks(
    /* [in] */ IRunnable* action)
{
    Mutex::Autolock lock(mLock);

    List<AutoPtr<HandlerAction> >::Iterator iter = mActions.Begin();
    while (iter != mActions.End()) {
        if ((*iter)->mAction.Get() == action) {
            iter = mActions.Erase(iter);
        }
        else ++iter;
    }
}

void ViewRootImpl::RunQueue::ExecuteActions(
    /* [in] */ IHandler* handler)
{
    Mutex::Autolock lock(mLock);

    Boolean result;

    List<AutoPtr<HandlerAction> >::Iterator iter;
    for (iter = mActions.Begin(); iter != mActions.End(); ++iter) {
        handler->PostDelayed((*iter)->mAction, (*iter)->mDelay, &result);
    }

    mActions.Clear();
}

CAR_INTERFACE_IMPL(
    ViewRootImpl::AccessibilityInteractionConnectionManager,
    IAccessibilityStateChangeListener);

ViewRootImpl::AccessibilityInteractionConnectionManager::AccessibilityInteractionConnectionManager(
    /* [in] */ ViewRootImpl* viewRootImpl)
    : mViewRootImpl(viewRootImpl)
{
}

ECode ViewRootImpl::AccessibilityInteractionConnectionManager::OnAccessibilityStateChanged(
    /* [in] */ Boolean enabled)
{
    if (enabled) {
        EnsureConnection();
        if (mViewRootImpl->mAttachInfo != NULL &&
            mViewRootImpl->mAttachInfo->mHasWindowFocus) {
            IAccessibilityEventSource::Probe(mViewRootImpl->mView)->SendAccessibilityEvent(
                IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);
            AutoPtr<IView> focusedView;
            mViewRootImpl->mView->FindFocus((IView**)&focusedView);
            if (focusedView != NULL && focusedView != mViewRootImpl->mView) {
                IAccessibilityEventSource::Probe(focusedView)->SendAccessibilityEvent(
                    IAccessibilityEvent::TYPE_VIEW_FOCUSED);
            }
        }
    }
    else {
        EnsureNoConnection();
        Boolean result;
        mViewRootImpl->mHandler->SendEmptyMessage(
            ViewRootImpl::MSG_CLEAR_ACCESSIBILITY_FOCUS_HOST, &result);
    }
    return NOERROR;
}

ECode ViewRootImpl::AccessibilityInteractionConnectionManager::EnsureConnection()
{
    if (mViewRootImpl->mAttachInfo != NULL) {
        Boolean registered =
            mViewRootImpl->mAttachInfo->mAccessibilityWindowId != IAccessibilityNodeInfo::UNDEFINED;
        if (!registered) {
            AutoPtr<IAccessibilityInteractionConnection> connection;
            CAccessibilityInteractionConnection::New(
                (Handle32)mViewRootImpl, (IAccessibilityInteractionConnection**)&connection);
            Int32 status;
            mViewRootImpl->mAttachInfo->mAccessibilityWindowId =
                mViewRootImpl->mAccessibilityManager->AddAccessibilityInteractionConnection(
                    mViewRootImpl->mWindow, connection, &status);
        }
    }
    return NOERROR;
}

ECode ViewRootImpl::AccessibilityInteractionConnectionManager::EnsureNoConnection()
{
    Boolean registered =
        mViewRootImpl->mAttachInfo->mAccessibilityWindowId != IAccessibilityNodeInfo::UNDEFINED;
    if (registered) {
        mViewRootImpl->mAttachInfo->mAccessibilityWindowId = IAccessibilityNodeInfo::UNDEFINED;
        mViewRootImpl->mAccessibilityManager->RemoveAccessibilityInteractionConnection(
            mViewRootImpl->mWindow);
    }
    return NOERROR;
}

ECode ViewRootImpl::SendWindowContentChangedAccessibilityEvent::Run()
{
    IAccessibilityEventSource* aes = IAccessibilityEventSource::Probe(mSource);
    if (aes != NULL) {
        aes->SendAccessibilityEvent(IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED);
        mSource->ResetAccessibilityStateChanged();
        mSource = NULL;
    }

    return NOERROR;
}

ViewRootImpl::RenderProfileRunnable::MyRunnable::MyRunnable(
    /* [in] */ IWeakReference* viewRootImpl)
    : mWeakHost(viewRootImpl)
{
}

ECode ViewRootImpl::RenderProfileRunnable::MyRunnable::Run()
{
    AutoPtr<IInterface> obj;
    mWeakHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        AutoPtr<ViewRootImpl> mViewRootImpl = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));
        mViewRootImpl->mDirty->Set(0, 0, mViewRootImpl->mWidth, mViewRootImpl->mHeight);
        mViewRootImpl->ScheduleTraversals();
    }

    return NOERROR;
}

ViewRootImpl::RenderProfileRunnable::RenderProfileRunnable(
    /* [in] */ IWeakReference* viewRootImpl)
    : mWeakHost(viewRootImpl)
{
}

ECode ViewRootImpl::RenderProfileRunnable::Run()
{
    Logger::D(TAG, "Starting profiling thread");

    AutoPtr<IInterface> obj;
    mWeakHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        AutoPtr<ViewRootImpl> mViewRootImpl = reinterpret_cast<ViewRootImpl*>(obj->Probe(EIID_ViewRootImpl));

        Boolean result;
        while (mViewRootImpl->mRenderProfilingEnabled) {
            AutoPtr<IRunnable> runnable = new MyRunnable(mWeakHost);
            mViewRootImpl->mAttachInfo->mHandler->Post(runnable, &result);

            // TODO: This should use vsync when we get an API
            if (FAILED(Thread::Sleep(15))) {
                Logger::D(TAG, "Exiting profiling thread");
            }
        }
    }
    return NOERROR;
}

const char* ViewRootImpl::TAG = "ViewRootImpl";
const Boolean ViewRootImpl::DBG = FALSE;
const Boolean ViewRootImpl::LOCAL_LOGV = FALSE;
const Boolean ViewRootImpl::DEBUG_DRAW = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_LAYOUT = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_DIALOG = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_INPUT_RESIZE = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_ORIENTATION = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_TRACKBALL = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_IMF = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_CONFIGURATION = FALSE || LOCAL_LOGV;
const Boolean ViewRootImpl::DEBUG_FPS = FALSE;
const Boolean ViewRootImpl::USE_RENDER_THREAD = FALSE;
const Boolean ViewRootImpl::MEASURE_LATENCY = FALSE;
const String ViewRootImpl::PROPERTY_PROFILE_RENDERING("viewancestor.profile_rendering");
const Int32 ViewRootImpl::MAX_TRACKBALL_DELAY = 250;
const Int32 ViewRootImpl::MAX_QUEUED_INPUT_EVENT_POOL_SIZE = 10;

pthread_key_t ViewRootImpl::sKeyRunQueues;
Boolean ViewRootImpl::sKeyRunQueuesInited = FALSE;

List<AutoPtr<IRunnable> > ViewRootImpl::sFirstDrawHandlers;
Mutex ViewRootImpl::sFirstDrawHandlersLock;

Boolean ViewRootImpl::sFirstDrawComplete = FALSE;

List<AutoPtr<IComponentCallbacks> > ViewRootImpl::sConfigCallbacks;
Mutex ViewRootImpl::sConfigCallbacksLock;

Boolean ViewRootImpl::sUseRenderThread = FALSE;
Boolean ViewRootImpl::sRenderThreadQueried = FALSE;
Mutex ViewRootImpl::sRenderThreadQueryLock;

static AutoPtr<IAccelerateDecelerateInterpolator> CreateInterpolator()
{
    AutoPtr<CAccelerateDecelerateInterpolator> interpolator;
    CAccelerateDecelerateInterpolator::NewByFriend((CAccelerateDecelerateInterpolator**)&interpolator);
    return interpolator;
}

AutoPtr<Elastos::Droid::View::Animation::IInterpolator> ViewRootImpl::mResizeInterpolator = CreateInterpolator();

ViewRootImpl::ViewRootImpl(
    /* [in] */ IContext* context,
    /* [in] */ IDisplay* display) :
    mDisplay(display),
    mLastTrackballTime(0),
    mLastJoystickXDirection(0),
    mLastJoystickYDirection(0),
    mLastJoystickXKeyCode(0),
    mLastJoystickYKeyCode(0),
    mSeq(0),
    mViewVisibility(IView::GONE),
    mAppVisible(TRUE),
    mOrigWindowType(-1),
    mStopped(FALSE),
    mLastInCompatMode(FALSE),
    mIsCreating(FALSE),
    mDrawingAllowed(FALSE),
    mWidth(-1),
    mHeight(-1),
    mIsAnimating(FALSE),
    mTraversalScheduled(FALSE),
    mTraversalBarrier(0),
    mWillDrawSoon(FALSE),
    mIsInTraversal(FALSE),
    mFitSystemWindowsRequested(FALSE),
    mLayoutRequested(FALSE),
    mFirst(TRUE), // TRUE for the first time the view is added
    mReportNextDraw(FALSE),
    mFullRedrawNeeded(FALSE),
    mNewSurfaceNeeded(FALSE),
    mHasHadWindowFocus(FALSE),
    mLastWasImTarget(FALSE),
    mWindowsAnimating(FALSE),
    mIsDrawing(FALSE),
    mLastSystemUiVisibility(0),
    mClientWindowLayoutFlags(0),
    mProcessInputEventsScheduled(FALSE),
    mWindowAttributesChanged(FALSE),
    mWindowAttributesChangesFlag(0),
    mAdded(FALSE),
    mAddedTouchMode(FALSE),
    mScrollMayChange(FALSE),
    mScrollY(0),
    mCurScrollY(0),
    mResizeBufferStartTime(0),
    mResizeBufferDuration(0),
    mHardwareYOffset(0),
    mResizeAlpha(0),
    mConsumeBatchedInputScheduled(FALSE),
    mQueuedInputEventPoolSize(0),
    mProfileRendering(FALSE),
    mRenderProfilingEnabled(FALSE),
    mFpsStartTime(-1),
    mFpsPrevTime(-1),
    mFpsNumFrames(0),
    mViewLayoutDirectionInitial(0),
    mProfile(FALSE)
{
    //if (MEASURE_LATENCY && mLt == NULL) {
    //    mLt = new LatencyTimer(100, 1000);
    //}

    mTrackballAxisX = new TrackballAxis();
    mTrackballAxisY = new TrackballAxis();

    ASSERT_SUCCEEDED(CTypedValue::New((ITypedValue**)&mTmpValue));

    mHandler = new ViewRootHandler(this);

    // Initialize the statics when this class is first instantiated. This is
    // done here instead of in the static block because Zygote does not
    // allow the spawning of threads.
    AutoPtr<ILooper> looper;
    context->GetMainLooper((ILooper**)&looper);
    mWindowSession = CWindowManagerGlobal::GetWindowSession(looper);

    display->GetCompatibilityInfo((ICompatibilityInfoHolder**)&mCompatibilityInfo);
    if (mCompatibilityInfo == NULL) {
        CCompatibilityInfoHolder::New((ICompatibilityInfoHolder**)&mCompatibilityInfo);
    }

    mThreadID = pthread_self();
    // mLocation = new WindowLeaked(NULL);
    // mLocation.fillInStackTrace();

    ASSERT_SUCCEEDED(CWindowManagerLayoutParams::NewByFriend(
        (CWindowManagerLayoutParams**)&mWindowAttributes));

    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mDirty));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mCurrentDirty));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mPreviousDirty));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mVisRect));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mWinFrame));

    ASSERT_SUCCEEDED(CViewRootImplW::New((Handle32)this, (IIWindow**)&mWindow));

    ASSERT_SUCCEEDED(CSurface::NewByFriend((CSurface**)&mSurface));

    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);
    appInfo->GetTargetSdkVersion(&mTargetSdkVersion);
    AutoPtr<IWeakReference> weakThis;
    GetWeakReference((IWeakReference**)&weakThis);
    mInputMethodCallback = new InputMethodCallback(weakThis);

    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mPendingVisibleInsets));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mPendingContentInsets));

    mLastGivenInsets = new ViewTreeObserver::InternalInsetsInfo();

    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mFitSystemWindowsInsets));

    ASSERT_SUCCEEDED(CConfiguration::New(
        (IConfiguration**)&mLastConfiguration));
    ASSERT_SUCCEEDED(CConfiguration::New(
        (IConfiguration**)&mPendingConfiguration));

    ASSERT_SUCCEEDED(CPointF::New((IPointF**)&mDragPoint));
    ASSERT_SUCCEEDED(CPointF::New((IPointF**)&mLastTouchPoint));

    ASSERT_SUCCEEDED(CRegion::NewByFriend((CRegion**)&mTransparentRegion));
    ASSERT_SUCCEEDED(CRegion::NewByFriend((CRegion**)&mPreviousTransparentRegion));

    mInputEventConsistencyVerifier =
        InputEventConsistencyVerifier::IsInstrumentationEnabled() ?
            new InputEventConsistencyVerifier(this->Probe(EIID_IInterface), 0) : NULL;

    CAccessibilityManager::GetInstance(context, (IAccessibilityManager**)&mAccessibilityManager);
    mAccessibilityInteractionConnectionManager =
        new AccessibilityInteractionConnectionManager(this);
    Boolean  result;
    mAccessibilityManager->AddAccessibilityStateChangeListener(
        mAccessibilityInteractionConnectionManager, &result);

    mAttachInfo = new View::AttachInfo(
        mWindowSession, (IIWindow*)mWindow->Probe(EIID_IIWindow),
        display, this, mHandler, this);

    mViewConfiguration = CViewConfiguration::Get(context);

    AutoPtr<IResources> rc;
    context->GetResources((IResources**)&rc);
    AutoPtr<IDisplayMetrics> dm;
    rc->GetDisplayMetrics((IDisplayMetrics**)&dm);
    dm->GetDensityDpi(&mDensity);
    dm->GetNoncompatDensityDpi(&mNoncompatDensity);

    AutoPtr<IPolicyManager> pm;
    CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
    pm->MakeNewFallbackEventHandler(
        context, (IFallbackEventHandler**)&mFallbackEventHandler);

    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    String temp;
    sp->GetEx(PROPERTY_PROFILE_RENDERING, String("FALSE"), &temp);
    mProfileRendering = temp.EqualsIgnoreCase("TRUE");
    mChoreographer = Choreographer::GetInstance();

    mAttachInfo->mScreenOn = TRUE;
    // AutoPtr<IPowerManager> powerManager;
    // context->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&powerManager);
    // powerManager->IsScreenOn(&mAttachInfo->mScreenOn);
    LoadSystemProperties();

    mHolder = new CSurfaceHolder(this);

    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mResizePaint));

    mTraversalRunnable = new TraversalRunnable(weakThis);
    mConsumedBatchedInputRunnable = new ConsumeBatchedInputRunnable(weakThis);
    mInvalidateOnAnimationRunnable = new InvalidateOnAnimationRunnable(weakThis);

}

ViewRootImpl::~ViewRootImpl()
{
    // delete mScroller;
    // delete mLastGivenInsets;
}

PInterface ViewRootImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IViewParent*)this;
    }
    else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_ViewRootImpl) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 ViewRootImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewRootImpl::Release()
{
    return ElRefBase::Release();
}

ECode ViewRootImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

static void RunQueueDestructor(void* st)
{
    ViewRootImpl::RunQueue* obj = static_cast<ViewRootImpl::RunQueue*>(st);
    if (obj) {
        obj->Release();
    }
}

AutoPtr<ViewRootImpl::RunQueue> ViewRootImpl::GetRunQueue()
{
    if (!sKeyRunQueuesInited) {
        ASSERT_TRUE(pthread_key_create(&sKeyRunQueues, RunQueueDestructor) == 0);
        sKeyRunQueuesInited = TRUE;
    }
    AutoPtr<RunQueue> runQueue = (RunQueue*)pthread_getspecific(sKeyRunQueues);
    if (runQueue == NULL) {
        runQueue = new RunQueue();
        ASSERT_TRUE(pthread_setspecific(sKeyRunQueues, runQueue.Get()) == 0);
        runQueue->AddRef();
    }
    assert(runQueue);

    return runQueue;
}

Boolean ViewRootImpl::IsRenderThreadRequested(
    /* [in] */ IContext* context)
{
    if (USE_RENDER_THREAD) {
        Mutex::Autolock lock(sRenderThreadQueryLock);

        if (!sRenderThreadQueried) {
            AutoPtr<IPackageManager> packageManager;
            context->GetPackageManager((IPackageManager**)&packageManager);
            AutoPtr<IApplicationInfo> appInfo;
            context->GetApplicationInfo((IApplicationInfo**)&appInfo);
            String packageName;
            appInfo->GetPackageName(&packageName);

            AutoPtr<IApplicationInfo> applicationInfo;
            packageManager->GetApplicationInfo(
                packageName, IPackageManager::GET_META_DATA,
                (IApplicationInfo**)&applicationInfo);

            AutoPtr<IBundle> metaData;
            applicationInfo->GetMetaData((IBundle**)&metaData);
            if (metaData != NULL) {
                metaData->GetBooleanEx(String("android.graphics.renderThread"),
                    FALSE, &sUseRenderThread);
            }

            sRenderThreadQueried = TRUE;
        }
        return sUseRenderThread;
    }
    else {
        return FALSE;
    }
}

void ViewRootImpl::AddFirstDrawHandler(
    /* [in] */ IRunnable* callback)
{
    Mutex::Autolock lock(sFirstDrawHandlersLock);

    if (!sFirstDrawComplete) {
        sFirstDrawHandlers.PushBack(callback);
    }
}

void ViewRootImpl::AddConfigCallback(
    /* [in] */ IComponentCallbacks* callback)
{
    Mutex::Autolock lock(sConfigCallbacksLock);

    sConfigCallbacks.PushBack(callback);
}

/**
* Call this to profile the next traversal call.
* FIXME for perf testing only. Remove eventually
*/
void ViewRootImpl::Profile()
{
    mProfile = TRUE;
}

/**
 * Indicates whether we are in touch mode. Calling this method triggers an IPC
 * call and should be avoided whenever possible.
 *
 * @return True, if the device is in touch mode, FALSE otherwise.
 *
 * @hide
 */
Boolean ViewRootImpl::IsInTouchMode()
{
    AutoPtr<IWindowSession> windowSession = CWindowManagerGlobal::PeekWindowSession();
    Boolean isInTouchMode = FALSE;
    if (windowSession != NULL) {
        windowSession->GetInTouchMode(&isInTouchMode);
    }

    return isInTouchMode;
}

ECode ViewRootImpl::SetView(
    /* [in] */ IView* view,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ IView* panelParentView)
{
    Mutex::Autolock lock(mSyncLock);

    if (mView == NULL) {
        mView = view;
        mView->GetRawLayoutDirection(&mViewLayoutDirectionInitial);
        mFallbackEventHandler->SetView(view);
        Int32 changes;
        mWindowAttributes->CopyFrom(attrs, &changes);
        attrs = mWindowAttributes.Get();

        // Keep track of the actual window flags supplied by the client.
        mClientWindowLayoutFlags = mWindowAttributes->mFlags;

        SetAccessibilityFocus(NULL, NULL);

        IRootViewSurfaceTaker* rootViewST = (IRootViewSurfaceTaker*)view->Probe(EIID_IRootViewSurfaceTaker);
        if (rootViewST) {
            mSurfaceHolderCallback = NULL;
            rootViewST->WillYouTakeTheSurface(
                (ISurfaceHolderCallback2**)&mSurfaceHolderCallback);
            if (mSurfaceHolderCallback != NULL) {
                mSurfaceHolder = new TakenSurfaceHolder(this);
                mSurfaceHolder->SetFormat(IPixelFormat::UNKNOWN);
            }
        }

        AutoPtr<IContext> ctx;
        mView->GetContext((IContext**)&ctx);
        assert(ctx.Get());

        AutoPtr<IResources> resources;
        ctx->GetResources((IResources**)&resources);
        assert(resources.Get());

        AutoPtr<ICompatibilityInfo> compatibilityInfo;
        mCompatibilityInfo->Get((ICompatibilityInfo**)&compatibilityInfo);
        mTranslator = NULL;
        compatibilityInfo->GetTranslator((ICompatibilityInfoTranslator**)&mTranslator);

        // If the application owns the surface, don't enable hardware acceleration
        if (mSurfaceHolder == NULL) {
            EnableHardwareAcceleration(ctx, attrs);
        }

        Boolean restore = FALSE;
        if (mTranslator != NULL) {
            mSurface->SetCompatibilityTranslator(mTranslator);
            restore = TRUE;
            attrs->Backup();
            mTranslator->TranslateWindowLayout(attrs);
        }

        if (DEBUG_LAYOUT) {
            String str;
            mWindowAttributes->ToString(&str);
            Logger::D(TAG, "WindowLayout in setView:%s", str.string());
        }

        Boolean supportsScreen = TRUE;
        compatibilityInfo->SupportsScreen(&supportsScreen);
        if (!supportsScreen) {
            mWindowAttributes->mFlags |= IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW;
            mLastInCompatMode = TRUE;
        }

        mSoftInputMode = mWindowAttributes->mSoftInputMode;
        mWindowAttributesChanged = TRUE;
        mAttachInfo->mRootView = view;
        mAttachInfo->mScalingRequired = mTranslator != NULL;
        mAttachInfo->mApplicationScale = (mTranslator == NULL)
            ? 1.0f : ((CCompatibilityInfo::Translator*)mTranslator.Get())->mApplicationScale;
        if (panelParentView != NULL) {
            mAttachInfo->mPanelParentWindowToken = NULL;
            panelParentView->GetApplicationWindowToken(
                (IBinder**)&mAttachInfo->mPanelParentWindowToken);
        }
        mAdded = TRUE;

        // Schedule the first layout -before- adding to the window
        // manager, to make sure we do the relayout before receiving
        // any other events from the system.
        //
        RequestLayout();

        if ((mWindowAttributes->mInputFeatures
            & IWindowManagerLayoutParams::INPUT_FEATURE_NO_INPUT_CHANNEL) == 0) {
            mInputChannel = NULL;
            ASSERT_SUCCEEDED(CInputChannel::New((IInputChannel**)&mInputChannel));
        }

        mOrigWindowType = mWindowAttributes->mType;
        mAttachInfo->mRecomputeGlobalAttributes = TRUE;
        CollectViewAttributes();

        Int32 displayId;
        mDisplay->GetDisplayId(&displayId);
        AutoPtr<IRect> tempRect;
        AutoPtr<IInputChannel> tempInputChannel;
        Int32 res;
        ECode ec = mWindowSession->AddToDisplay(
            mWindow.Get(), mSeq, mWindowAttributes.Get(),
            GetHostVisibility(), displayId, mAttachInfo->mContentInsets.Get(),
             mInputChannel.Get(), (IRect**)&tempRect,
            (IInputChannel**)&tempInputChannel, &res);

        if (tempRect != NULL) {
            mAttachInfo->mContentInsets->SetEx(tempRect);
        }
        if (tempInputChannel != NULL) {
            tempInputChannel->TransferTo(mInputChannel);
        }

        if (FAILED(ec)) {
            mAdded = FALSE;
            mView = NULL;
            mAttachInfo->mRootView = NULL;
            mInputChannel = NULL;
            mFallbackEventHandler->SetView(NULL);
            UnscheduleTraversals();
            SetAccessibilityFocus(NULL, NULL);

            if (restore) {
                attrs->Restore();
            }

            Logger::E(TAG, "Adding window failed");
            return E_RUNTIME_EXCEPTION;
        }

        if (restore) {
            mWindowAttributes->Restore();
        }

        if (mTranslator != NULL) {
            mTranslator->TranslateRectInScreenToAppWindow(mAttachInfo->mContentInsets);
        }

        mPendingContentInsets->SetEx((IRect*)mAttachInfo->mContentInsets);
        mPendingVisibleInsets->Set(0, 0, 0, 0);

        if (DEBUG_LAYOUT)
            Logger::V(TAG, "Added window %p", mWindow.Get());

        if (res < IWindowManagerGlobal::ADD_OKAY) {
            mAttachInfo->mRootView = NULL;
            mAdded = FALSE;
            mFallbackEventHandler->SetView(NULL);
            UnscheduleTraversals();
            SetAccessibilityFocus(NULL, NULL);

            switch (res) {
                case IWindowManagerGlobal::ADD_BAD_APP_TOKEN:
                case IWindowManagerGlobal::ADD_BAD_SUBWINDOW_TOKEN:
                    Logger::E(TAG, "Unable to add window -- token 0x%08x"
                        "  is not valid; is your activity running?",
                        mWindowAttributes->mToken.Get());
                    return E_BAD_TOKEN_EXCEPTION;

                case IWindowManagerGlobal::ADD_NOT_APP_TOKEN:
                    Logger::E(TAG, "Unable to add window -- token 0x%08x"
                        "  is not for an application",
                        mWindowAttributes->mToken.Get());
                    return E_BAD_TOKEN_EXCEPTION;

                case IWindowManagerGlobal::ADD_APP_EXITING:
                    Logger::E(TAG, "Unable to add window -- app for token 0x%08x"
                        "   is exiting?", mWindowAttributes->mToken.Get());
                    return E_BAD_TOKEN_EXCEPTION;

                case IWindowManagerGlobal::ADD_DUPLICATE_ADD:
                    Logger::E(TAG, "Unable to add window -- window 0x%08x"
                        "  has already been added", mWindow.Get());
                    return E_BAD_TOKEN_EXCEPTION;

                case IWindowManagerGlobal::ADD_STARTING_NOT_NEEDED:
                    // Silently ignore -- we would have just removed it
                    // right away, anyway.
                    return NOERROR;

                case IWindowManagerGlobal::ADD_MULTIPLE_SINGLETON:
                    Logger::E(TAG, "Unable to add window 0x%08x -- another "
                        "window of this type already exists", mWindow.Get());
                    return E_BAD_TOKEN_EXCEPTION;

                case IWindowManagerGlobal::ADD_PERMISSION_DENIED:
                    Logger::E(TAG, "Unable to add window 0x%08x -- permission "
                        "denied for this window type", mWindow.Get());
                    return E_BAD_TOKEN_EXCEPTION;

                case IWindowManagerGlobal::ADD_INVALID_DISPLAY:
                    Logger::E(TAG, "Unable to add window 0x%08x -- the specified "
                        " display can not be found", mWindow.Get());
                    return E_INVALID_DISPLAY_EXCEPTION;

                default:
                    Logger::E(TAG, "Unable to add window -- unknown error code %d", res);
                    return E_RUNTIME_EXCEPTION;
            }
        }

        if (rootViewST) {
            rootViewST->WillYouTakeTheInputQueue((IInputQueueCallback**)&mInputQueueCallback);
        }

        if (mInputChannel != NULL) {
            if (mInputQueueCallback != NULL) {
                CInputQueue::New(mInputChannel.Get(), (IInputQueue**)&mInputQueue);
                mInputQueueCallback->OnInputQueueCreated(mInputQueue);
            }
            else {
                AutoPtr<IWeakReference> weakThis;
                GetWeakReference((IWeakReference**)&weakThis);
                mInputEventReceiver = new WindowInputEventReceiver(
                    mInputChannel, mHandler->mLooper, weakThis);
                assert(mInputEventReceiver != NULL);
            }
        }

        view->AssignParent(THIS_PROBE(IViewParent));
        mAddedTouchMode = (res & IWindowManagerGlobal::ADD_FLAG_IN_TOUCH_MODE) != 0;
        mAppVisible = (res & IWindowManagerGlobal::ADD_FLAG_APP_VISIBLE) != 0;

        Boolean isEnabled = FALSE;
        mAccessibilityManager->IsEnabled(&isEnabled);
        if (isEnabled) {
            mAccessibilityInteractionConnectionManager->EnsureConnection();
        }

        Int32 ia;
        view->GetImportantForAccessibility(&ia);
        if (ia == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
            view->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
        }
    }

    return NOERROR;
}

void ViewRootImpl::DestroyHardwareResources()
{
    // if (mAttachInfo->mHardwareRenderer != NULL) {
    //     if (mAttachInfo->mHardwareRenderer->IsEnabled()) {
    //         mAttachInfo->mHardwareRenderer->DestroyLayers(mView);
    //     }
    //     mAttachInfo->mHardwareRenderer->Destroy(FALSE);
    // }
}

void ViewRootImpl::TerminateHardwareResources()
{
    // if (mAttachInfo->mHardwareRenderer != NULL) {
    //     mAttachInfo->mHardwareRenderer->DestroyHardwareResources(mView);
    //     mAttachInfo->mHardwareRenderer->Destroy(FALSE);
    // }
}

void ViewRootImpl::DestroyHardwareLayers()
{
    // if (mThread != Thread.currentThread()) {
    //     if (mAttachInfo->mHardwareRenderer != NULL &&
    //         mAttachInfo->mHardwareRenderer->IsEnabled()) {
    //         HardwareRenderer::TrimMemory(IComponentCallbacks2::TRIM_MEMORY_MODERATE);
    //     }
    // }
    // else {
    //     if (mAttachInfo->mHardwareRenderer != NULL &&
    //         mAttachInfo->mHardwareRenderer->IsEnabled()) {
    //         mAttachInfo->mHardwareRenderer->DestroyLayers(mView);
    //     }
    // }
}

void ViewRootImpl::PushHardwareLayerUpdate(
    /* [in] */ IHardwareLayer* layer)
{
    // if (mAttachInfo->mHardwareRenderer != NULL && mAttachInfo->mHardwareRenderer->IsEnabled()) {
    //     mAttachInfo->mHardwareRenderer->PushLayerUpdate(layer);
    // }
}

Boolean ViewRootImpl::AttachFunctor(
    /* [in] */ Int32 functor)
{
    // //noinspection SimplifiableIfStatement
    // if (mAttachInfo->mHardwareRenderer != NULL && mAttachInfo->mHardwareRenderer->IsEnabled()) {
    //     return mAttachInfo->mHardwareRenderer->AttachFunctor(mAttachInfo, functor);
    // }
    return FALSE;
}

void ViewRootImpl::DetachFunctor(
    /* [in] */ Int32 functor)
{
    // if (mAttachInfo->mHardwareRenderer != NULL) {
    //     mAttachInfo->mHardwareRenderer->DetachFunctor(functor);
    // }
}

void ViewRootImpl::EnableHardwareAcceleration(
    /* [in] */ IContext* context,
    /* [in] */ IWindowManagerLayoutParams* attrs)
{
    Logger::D("ViewRootImpl", "TODO no impl, EnableHardwareAcceleration,line:%d ,", __LINE__);
    // mAttachInfo->mHardwareAccelerated = FALSE;
    // mAttachInfo->mHardwareAccelerationRequested = FALSE;

    // // Don't enable hardware acceleration when the application is in compatibility mode
    // if (mTranslator != NULL)
    //     return;

    // // Try to enable hardware acceleration if requested
    // Int32 flags;
    // attrs->GetFlags(&flags);
    // const Boolean hardwareAccelerated =
    //         (flags & IWindowManagerLayoutParams::FLAG_HARDWARE_ACCELERATED) != 0;

    /*int index;
    String pckname;
    String  substr = "com.android.cts.acceleration";
    pckname = context.getPackageName();
    index  = pckname.indexOf(substr);
    if(index == -1) {
        hardwareAccelerated = true;
    }*/

    /*if (attrs.type == WindowManager.LayoutParams.FIRST_SUB_WINDOW) {
        hardwareAccelerated = false;
    }*/
    /*
    Log.i(TAG, "attrs:" + attrs);
    Log.i(TAG, "attrs.flags=" + attrs.flags);
    Log.i(TAG, "attrs.type=" + attrs.type);
    */
    // if (hardwareAccelerated) {
    //     if (!HardwareRenderer::IsAvailable()) {
    //         return;
    //     }

    //     // Persistent processes (including the system) should not do
    //     // accelerated rendering on low-end devices.  In that case,
    //     // sRendererDisabled will be set.  In addition, the system process
    //     // itself should never do accelerated rendering.  In that case, both
    //     // sRendererDisabled and sSystemRendererDisabled are set.  When
    //     // sSystemRendererDisabled is set, PRIVATE_FLAG_FORCE_HARDWARE_ACCELERATED
    //     // can be used by code on the system process to escape that and enable
    //     // HW accelerated drawing.  (This is basically for the lock screen.)

    //     Int32 privateFlags;
    //     attrs->GetPrivateFlags(&privateFlags);
    //     const Boolean fakeHwAccelerated = (privateFlags &
    //             IWindowManagerLayoutParams::PRIVATE_FLAG_FAKE_HARDWARE_ACCELERATED) != 0;
    //     const Boolean forceHwAccelerated = (privateFlags &
    //             IWindowManagerLayoutParams::PRIVATE_FLAG_FORCE_HARDWARE_ACCELERATED) != 0;

    //     if (!HardwareRenderer::sRendererDisabled || (HardwareRenderer::sSystemRendererDisabled
    //             && forceHwAccelerated)) {
    //         // Don't enable hardware acceleration when we're not on the main thread
    //         if (!HardwareRenderer::sSystemRendererDisabled &&
    //                 Looper::GetMainLooper() != Looper::MyLooper()) {
    //             Logger::W(HardwareRenderer::LOG_TAG, "Attempting to initialize hardware "
    //                 "acceleration outside of the main thread, aborting");
    //             return;
    //         }

    //         const Boolean renderThread = IsRenderThreadRequested(context);
    //         if (renderThread) {
    //             Logger::I(HardwareRenderer::LOG_TAG, "Render threat initiated");
    //         }

    //         if (mAttachInfo->mHardwareRenderer != NULL) {
    //             mAttachInfo->mHardwareRenderer->Destroy(TRUE);
    //         }

    //         const Boolean translucent = attrs.format != PixelFormat.OPAQUE;
    //         mAttachInfo->mHardwareRenderer = HardwareRenderer::CreateGlRenderer(2, translucent);
    //         mAttachInfo->mHardwareAccelerated = mAttachInfo->mHardwareAccelerationRequested
    //                 = mAttachInfo->mHardwareRenderer != NULL;

    //     }
    //     else if (fakeHwAccelerated) {
    //         // The window had wanted to use hardware acceleration, but this
    //         // is not allowed in its process.  By setting this flag, it can
    //         // still render as if it was accelerated.  This is basically for
    //         // the preview windows the window manager shows for launching
    //         // applications, so they will look more like the app being launched.
    //         mAttachInfo->mHardwareAccelerationRequested = TRUE;
    //     }
    // }
}

AutoPtr<IView> ViewRootImpl::GetView()
{
    return mView;
}

// final WindowLeaked getLocation() {
//     return mLocation;
// }

void ViewRootImpl::SetLayoutParams(
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [in] */ Boolean newView)
{
    Mutex::Autolock lock(mSyncLock);

    Int32 oldSoftInputMode = mWindowAttributes->mSoftInputMode;

    // Keep track of the actual window flags supplied by the client.
    attrs->GetFlags(&mClientWindowLayoutFlags);
    // preserve compatible window flag if exists.
    //
    Int32 compatibleWindowFlag =
        mWindowAttributes->mFlags & IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW;

    // transfer over system UI visibility values as they carry current state.
    attrs->SetSystemUiVisibility(mWindowAttributes->mSystemUiVisibility);
    attrs->SetSubtreeSystemUiVisibility(mWindowAttributes->mSubtreeSystemUiVisibility);
    mWindowAttributes->CopyFrom(attrs, &mWindowAttributesChangesFlag);
    mWindowAttributes->mFlags |= compatibleWindowFlag;

    ApplyKeepScreenOnFlag(mWindowAttributes);

    if (newView) {
        mSoftInputMode = mWindowAttributes->mSoftInputMode;
        RequestLayout();
    }

    // Don't lose the mode we last auto-computed.
    //
    if ((mWindowAttributes->mSoftInputMode & IWindowManagerLayoutParams::SOFT_INPUT_MASK_ADJUST)
        == IWindowManagerLayoutParams::SOFT_INPUT_ADJUST_UNSPECIFIED) {
        mWindowAttributes->mSoftInputMode = (mWindowAttributes->mSoftInputMode
            & ~IWindowManagerLayoutParams::SOFT_INPUT_MASK_ADJUST)
            | (oldSoftInputMode
            & IWindowManagerLayoutParams::SOFT_INPUT_MASK_ADJUST);
    }

    mWindowAttributesChanged = TRUE;
    ScheduleTraversals();
}

void ViewRootImpl::HandleAppVisibility(
    /* [in] */ Boolean visible)
{
    if (mAppVisible != visible) {
        mAppVisible = visible;
        ScheduleTraversals();
    }
}

void ViewRootImpl::HandleGetNewSurface()
{
    mNewSurfaceNeeded = TRUE;
    mFullRedrawNeeded = TRUE;
    ScheduleTraversals();
}

void ViewRootImpl::HandleScreenStateChange(
    /* [in] */ Boolean on)
{
    if (on != mAttachInfo->mScreenOn) {
        mAttachInfo->mScreenOn = on;
        if (mView != NULL) {
            VIEW_PROBE(mView)->DispatchScreenStateChanged(
                on ? IView::SCREEN_STATE_ON : IView::SCREEN_STATE_OFF);
        }
        if (on) {
            mFullRedrawNeeded = TRUE;
            ScheduleTraversals();
        }
    }
}

ECode ViewRootImpl::RequestFitSystemWindowsEx()
{
    FAIL_RETURN(CheckThread());
    mFitSystemWindowsRequested = TRUE;
    ScheduleTraversals();

    return NOERROR;
}

ECode ViewRootImpl::RequestLayout()
{
    FAIL_RETURN(CheckThread());

    mLayoutRequested = TRUE;
    ScheduleTraversals();

    return NOERROR;
}

ECode ViewRootImpl::RequestLayoutEx()
{
    return RequestLayout();
}

ECode ViewRootImpl::IsLayoutRequested(
    /* [out] */ Boolean* result)
{
    assert(result);
    *result = mLayoutRequested;

    return NOERROR;
}

ECode ViewRootImpl::IsLayoutRequestedEx(
    /* [out] */ Boolean* result)
{
    return IsLayoutRequested(result);
}

void ViewRootImpl::Invalidate()
{
    mDirty->Set(0, 0, mWidth, mHeight);
    ScheduleTraversals();
}

void ViewRootImpl::InvalidateWorld(
    /* [in] */ IView* view)
{
    view->InvalidateEx2();
    ViewGroup* parent = VIEWGROUP_PROBE(view);
    if (parent) {
        Int32 count = parent->GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = parent->GetChildAt(i);
            InvalidateWorld(child);
        }
    }
}

ECode ViewRootImpl::InvalidateChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    AutoPtr<IViewParent> parent;
    return InvalidateChildInParent(NULL, dirty, (IViewParent**)&parent);
}

ECode ViewRootImpl::InvalidateChildInParent(
    /* [in] */ ArrayOf<Int32>* location,
    /* [in] */ IRect* dirty,
    /* [out] */ IViewParent** parent)
{
    FAIL_RETURN(CheckThread());

    if (DEBUG_DRAW)
        Logger::V(TAG, "Invalidate child: "/* + dirty*/);

    Boolean isEmpty;
    if (dirty == NULL) {
        Invalidate();
        *parent = NULL;
        return NOERROR;
    }
    else if ((dirty->IsEmpty(&isEmpty), isEmpty) && !mIsAnimating) {
        *parent = NULL;
        return NOERROR;
    }

    if (mCurScrollY != 0 || mTranslator != NULL) {
        mTempRect->SetEx(dirty);
        dirty = mTempRect;
        if (mCurScrollY != 0) {
            dirty->Offset(0, -mCurScrollY);
        }
        if (mTranslator != NULL) {
            mTranslator->TranslateRectInAppWindowToScreen(dirty);
        }
        if (mAttachInfo->mScalingRequired) {
            dirty->Inset(-1, -1);
        }
    }

    CRect* localDirty = mDirty;
    localDirty->IsEmpty(&isEmpty);
    Boolean contains;
    localDirty->ContainsEx2(dirty, &contains);
    if (!isEmpty && !contains) {
        mAttachInfo->mSetIgnoreDirtyState = TRUE;
        mAttachInfo->mIgnoreDirtyState = TRUE;
    }

    // Add the new dirty rect to the current one
    localDirty->UnionEx(dirty);
    // Intersect with the bounds of the window to skip
    // updates that lie outside of the visible region
    const Float appScale = mAttachInfo->mApplicationScale;
    Boolean intersected;
    localDirty->Intersect(
        0, 0, (Int32)(mWidth * appScale + 0.5f),
        (Int32)(mHeight * appScale + 0.5f), &intersected);
    if (!intersected) {
        localDirty->SetEmpty();
    }
    if (!mWillDrawSoon && (intersected || mIsAnimating)) {
        ScheduleTraversals();
    }

    *parent = NULL;
    return NOERROR;
}

void ViewRootImpl::SetStopped(
    /* [in] */ Boolean stopped)
{
    if (mStopped != stopped) {
        mStopped = stopped;
        if (!stopped) {
            ScheduleTraversals();
        }
    }
}

ECode ViewRootImpl::GetParent(
    /* [out] */ IViewParent** parent)
{
    assert(parent);
    *parent = NULL;

    return NOERROR;
}

ECode ViewRootImpl::GetParentEx(
    /* [out] */ IViewParent** parent)
{
    return GetParent(parent);
}

ECode ViewRootImpl::GetChildVisibleRect(
    /* [in] */ IView* child,
    /* [in] */ IRect* r,
    /* [in] */ IPoint* offset,
    /* [out] */ Boolean* result)
{
    if (child != mView) {
        Logger::W(TAG, "child is not mine, honest!");

        return E_RUNTIME_EXCEPTION;
    }

    // Note: don't apply scroll offset, because we want to know its
    // visibility in the virtual canvas being given to the view hierarchy.
    //
    return r->Intersect(0, 0, mWidth, mHeight, result);
}

ECode ViewRootImpl::BringChildToFront(
    /* [in] */ IView* child)
{
    return NOERROR;
}

Int32 ViewRootImpl::GetHostVisibility()
{
    assert(mView.Get());

    Int32 visibility = IView::GONE;
    if (mAppVisible) {
        mView->GetVisibility(&visibility);
    }

    return visibility;
}

void ViewRootImpl::DisposeResizeBuffer()
{
    if (mResizeBuffer != NULL) {
        //mResizeBuffer->Destroy();
        mResizeBuffer = NULL;
    }
}

void ViewRootImpl::RequestTransitionStart(
    /* [in] */ ILayoutTransition* transition)
{
    AutoPtr<ILayoutTransition> temp = transition;
    List<AutoPtr<ILayoutTransition> >::Iterator iter =
        Find(mPendingTransitions.Begin(), mPendingTransitions.End(), temp);
    if (iter == mPendingTransitions.End()) {
        mPendingTransitions.PushBack(transition);
    }
}

void ViewRootImpl::ScheduleTraversals()
{
    if (!mTraversalScheduled) {
        mTraversalScheduled = TRUE;
        mHandler->mLooper->PostSyncBarrier(&mTraversalBarrier);
        mChoreographer->PostCallback(
            IChoreographer::CALLBACK_TRAVERSAL, mTraversalRunnable, NULL);
        ScheduleConsumeBatchedInput();
    }
}

void ViewRootImpl::UnscheduleTraversals()
{
    if (mTraversalScheduled) {
        mTraversalScheduled = FALSE;
        mHandler->mLooper->RemoveSyncBarrier(mTraversalBarrier);
        mChoreographer->RemoveCallbacks(
            IChoreographer::CALLBACK_TRAVERSAL, mTraversalRunnable, NULL);
    }
}

void ViewRootImpl::DoTraversal()
{
    if (mTraversalScheduled) {
        mTraversalScheduled = FALSE;
        mHandler->mLooper->RemoveSyncBarrier(mTraversalBarrier);

        if (mProfile) {
            //Debug.startMethodTracing("ViewAncestor");
        }

        //Trace::TraceBegin(Trace::TRACE_TAG_VIEW, "performTraversals");
        PerformTraversals();
        //Trace::TraceEnd(Trace::TRACE_TAG_VIEW);

        if (mProfile) {
            //Debug.stopMethodTracing();
            mProfile = FALSE;
        }
    }
}


void ViewRootImpl::ApplyKeepScreenOnFlag(
    /* [in] */ CWindowManagerLayoutParams* params)
{
    // Update window's global keep screen on flag: if a view has requested
    // that the screen be kept on, then it is always set; otherwise, it is
    // set to whatever the client last requested for the global state.
    if (mAttachInfo->mKeepScreenOn) {
        params->mFlags |= IWindowManagerLayoutParams::FLAG_KEEP_SCREEN_ON;
    }
    else {
        params->mFlags =
            (params->mFlags & ~IWindowManagerLayoutParams::FLAG_KEEP_SCREEN_ON)
            | (mClientWindowLayoutFlags&IWindowManagerLayoutParams::FLAG_KEEP_SCREEN_ON);
    }
}

Boolean ViewRootImpl::CollectViewAttributes()
{
    View::AttachInfo* attachInfo = mAttachInfo;
    if (attachInfo->mRecomputeGlobalAttributes) {
        //ger::oG.i(TAG, "Computing view hierarchy attributes!");
        attachInfo->mRecomputeGlobalAttributes = FALSE;
        Boolean oldScreenOn = attachInfo->mKeepScreenOn;
        attachInfo->mKeepScreenOn = FALSE;
        attachInfo->mSystemUiVisibility = 0;
        attachInfo->mHasSystemUiListeners = FALSE;
        VIEW_PROBE(mView)->DispatchCollectViewAttributes(attachInfo, 0);
        attachInfo->mSystemUiVisibility &= ~attachInfo->mDisabledSystemUiVisibility;
        CWindowManagerLayoutParams* params = mWindowAttributes;
        if (attachInfo->mKeepScreenOn != oldScreenOn
            || attachInfo->mSystemUiVisibility != params->mSubtreeSystemUiVisibility
            || attachInfo->mHasSystemUiListeners != params->mHasSystemUiListeners) {
            ApplyKeepScreenOnFlag(params);
            params->mSubtreeSystemUiVisibility = attachInfo->mSystemUiVisibility;
            params->mHasSystemUiListeners = attachInfo->mHasSystemUiListeners;
            mView->DispatchWindowSystemUiVisiblityChanged(attachInfo->mSystemUiVisibility);
            return TRUE;
        }
    }
    return FALSE;
}

Boolean ViewRootImpl::MeasureHierarchy(
    /* [in] */ View* host,
    /* [in] */ CWindowManagerLayoutParams* lp,
    /* [in] */ IResources* res,
    /* [in] */ Int32 desiredWindowWidth,
    /* [in] */ Int32 desiredWindowHeight)
{
    Int32 childWidthMeasureSpec;
    Int32 childHeightMeasureSpec;
    Boolean windowSizeMayChange = FALSE;

    if (DEBUG_ORIENTATION || DEBUG_LAYOUT)
        Logger::V(TAG, "Measuring 0x%08x in display %d x %d ...",
            host, desiredWindowWidth, desiredWindowHeight);

    Boolean goodMeasure = FALSE;
    if (lp->mWidth == IViewGroupLayoutParams::WRAP_CONTENT) {
        // On large screens, we don't want to allow dialogs to just
        // stretch to fill the entire width of the screen to display
        // one line of text.  First try doing the layout at a smaller
        // size to see if it will fit.
        AutoPtr<IDisplayMetrics> packageMetrics;
        res->GetDisplayMetrics((IDisplayMetrics**)&packageMetrics);
        res->GetValue(R::dimen::config_prefDialogWidth, mTmpValue, TRUE);
        Int32 baseSize = 0;
        Int32 type;
        mTmpValue->GetType(&type);
        if (type == ITypedValue::TYPE_DIMENSION) {
            Float dimension;
            mTmpValue->GetDimension(packageMetrics, &dimension);
            baseSize = (Int32)dimension;
        }

        if (DEBUG_DIALOG)
            Logger::V(TAG, "Window 0x%08x : baseSize=%d", mView.Get(), baseSize);

        if (baseSize != 0 && desiredWindowWidth > baseSize) {
            childWidthMeasureSpec = GetRootMeasureSpec(baseSize, lp->mWidth);
            childHeightMeasureSpec = GetRootMeasureSpec(desiredWindowHeight, lp->mHeight);
            PerformMeasure(childWidthMeasureSpec, childHeightMeasureSpec);

            if (DEBUG_DIALOG)
                Logger::V(TAG, "Window 0x%08x : measured (%d, %d)",
                    mView.Get(), host->GetMeasuredWidth(), host->GetMeasuredHeight());

            if ((host->GetMeasuredWidthAndState() & IView::MEASURED_STATE_TOO_SMALL) == 0) {
                goodMeasure = TRUE;
            }
            else {
                // Didn't fit in that size... try expanding a bit.
                baseSize = (baseSize + desiredWindowWidth) / 2;
                if (DEBUG_DIALOG)
                    Logger::V(TAG, "Window 0x%08x : next baseSize=%d",
                    mView.Get(), baseSize);

                childWidthMeasureSpec = GetRootMeasureSpec(baseSize, lp->mWidth);
                PerformMeasure(childWidthMeasureSpec, childHeightMeasureSpec);

                if (DEBUG_DIALOG)
                    Logger::V(TAG, "Window 0x%08x : measured (%d, %d)",
                        mView.Get(), host->GetMeasuredWidth(), host->GetMeasuredHeight());

                if ((host->GetMeasuredWidthAndState() & IView::MEASURED_STATE_TOO_SMALL) == 0) {
                    if (DEBUG_DIALOG)
                        Logger::V(TAG, "Good!");
                    goodMeasure = TRUE;
                }
            }
        }
    }

    if (!goodMeasure) {
        childWidthMeasureSpec = GetRootMeasureSpec(desiredWindowWidth, lp->mWidth);
        childHeightMeasureSpec = GetRootMeasureSpec(desiredWindowHeight, lp->mHeight);
        PerformMeasure(childWidthMeasureSpec, childHeightMeasureSpec);
        if (mWidth != host->GetMeasuredWidth() || mHeight != host->GetMeasuredHeight()) {
            windowSizeMayChange = TRUE;
        }
    }

    if (DBG) {
        Logger::D(TAG, "======================================");
        Logger::D(TAG, "performTraversals -- after measure");
        //host->Debug();
    }

    return windowSizeMayChange;
}

void ViewRootImpl::PerformTraversals()
{
    LocalTrace(mView, "::ENTER >> ViewRootImpl::PerformTraversals()");
    //LogView(String("PerformTraversals"));

    // cache mView since it is used so much below...
    //
    View* host = mView != NULL ? VIEW_PROBE(mView) : NULL;

    if (host == NULL || !mAdded) {
        LocalTrace(mView, "::LEAVE >> ViewRootImpl::PerformTraversals() : host == NULL || !mAdded.");
        return;
    }

    mIsInTraversal = TRUE;
    mWillDrawSoon = TRUE;

    Boolean windowSizeMayChange = FALSE;
    Boolean newSurface = FALSE;
    Boolean surfaceChanged = FALSE;
    CWindowManagerLayoutParams* lp = mWindowAttributes;

    Int32 desiredWindowWidth = 0;
    Int32 desiredWindowHeight = 0;

    View::AttachInfo* attachInfo = mAttachInfo;

    const Int32 viewVisibility = GetHostVisibility();
    Boolean viewVisibilityChanged = mViewVisibility != viewVisibility
        || mNewSurfaceNeeded;

    CWindowManagerLayoutParams* params = NULL;
    if (mWindowAttributesChanged) {
        mWindowAttributesChanged = FALSE;
        surfaceChanged = TRUE;
        params = lp;
    }
    AutoPtr<ICompatibilityInfo> compatibilityInfo;
    mCompatibilityInfo->Get((ICompatibilityInfo**)&compatibilityInfo);
    Boolean supportsScreen;
    compatibilityInfo->SupportsScreen(&supportsScreen);
    if (supportsScreen == mLastInCompatMode) {
        params = lp;
        mFullRedrawNeeded = TRUE;
        mLayoutRequested = TRUE;
        if (mLastInCompatMode) {
            params->mFlags &= ~IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW;
            mLastInCompatMode = FALSE;
        }
        else {
            params->mFlags |= IWindowManagerLayoutParams::FLAG_COMPATIBLE_WINDOW;
            mLastInCompatMode = TRUE;
        }
    }

    mWindowAttributesChangesFlag = 0;

    CRect* frame = mWinFrame;
    if (mFirst) {
        mFullRedrawNeeded = TRUE;
        mLayoutRequested = TRUE;

        if (lp->mType == IWindowManagerLayoutParams::TYPE_STATUS_BAR_PANEL) {
            // NOTE -- system code, won't try to do compat mode.
            AutoPtr<IPoint> size;
            CPoint::New((IPoint**)&size);
            mDisplay->GetRealSize(size);
            size->GetX(&desiredWindowWidth);
            size->GetY(&desiredWindowHeight);
        }
        else {
            AutoPtr<IContext> ctx;
            mView->GetContext((IContext**)&ctx);
            assert(ctx);

            AutoPtr<IResources> resources;
            ctx->GetResources((IResources**)&resources);
            assert(resources);

            AutoPtr<IDisplayMetrics> displayMetrics;
            resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
            displayMetrics->GetWidthPixels(&desiredWindowWidth);
            displayMetrics->GetHeightPixels(&desiredWindowHeight);
        }

        // For the very first time, tell the view hierarchy that it
        // is attached to the window.  Note that at this point the surface
        // object is not initialized to its backing store, but soon it
        // will be (assuming the window is visible).
        //
        attachInfo->mSurface = mSurface;
        // We used to use the following condition to choose 32 bits drawing caches:
        // PixelFormat.hasAlpha(lp->mFormat) || lp.format == PixelFormat.RGBX_8888
        // However, windows are now always 32 bits by default, so choose 32 bits
        attachInfo->mUse32BitDrawingCache = TRUE;
        attachInfo->mHasWindowFocus = FALSE;
        attachInfo->mWindowVisibility = viewVisibility;
        attachInfo->mRecomputeGlobalAttributes = FALSE;
        attachInfo->mKeepScreenOn = FALSE;
        viewVisibilityChanged = FALSE;

        AutoPtr<IResources> resources = host->GetResources();
        assert(resources);
        AutoPtr<IConfiguration> config;
        resources->GetConfiguration((IConfiguration**)&config);
        mLastConfiguration->SetTo(config);
        mLastSystemUiVisibility = mAttachInfo->mSystemUiVisibility;
        // Set the layout direction if it has not been set before (inherit is the default)
        if (mViewLayoutDirectionInitial == IView::LAYOUT_DIRECTION_INHERIT) {
            Int32 direction;
            mLastConfiguration->GetLayoutDirection(&direction);
            host->SetLayoutDirection(direction);
        }
        host->DispatchAttachedToWindow(attachInfo, 0);
        mFitSystemWindowsInsets->SetEx(mAttachInfo->mContentInsets);
        host->FitSystemWindows(mFitSystemWindowsInsets);
        Logger::I(TAG, "Screen on initialized: %d", attachInfo->mKeepScreenOn);
    }
    else {
        frame->GetWidth(&desiredWindowWidth);
        frame->GetHeight(&desiredWindowHeight);
        if (desiredWindowWidth != mWidth || desiredWindowHeight != mHeight) {
            if (DEBUG_ORIENTATION) {
                Logger::V(TAG, "View 0x%08x resized to l = %d, r = %d, t = %d, b = %d"
                    , (Int32)host, frame->mLeft, frame->mRight, frame->mTop, frame->mBottom);
            }

            mFullRedrawNeeded = TRUE;
            mLayoutRequested = TRUE;
            windowSizeMayChange = TRUE;
        }
    }
    if (viewVisibilityChanged) {
        attachInfo->mWindowVisibility = viewVisibility;
        host->DispatchWindowVisibilityChanged(viewVisibility);

        if (viewVisibility != IView::VISIBLE || mNewSurfaceNeeded) {
            DestroyHardwareResources();
        }

        if (viewVisibility == IView::GONE) {
            // After making a window gone, we will count it as being
            // shown for the first time the next time it gets focus.
            //
            mHasHadWindowFocus = FALSE;
        }
    }

    // Execute enqueued actions on every traversal in case a detached view enqueued an action
    GetRunQueue()->ExecuteActions(attachInfo->mHandler);

    Boolean insetsChanged = FALSE;

    Boolean layoutRequested = mLayoutRequested && !mStopped;
    if (layoutRequested) {
        AutoPtr<IContext> ctx;
        mView->GetContext((IContext**)&ctx);
        assert(ctx);

        AutoPtr<IResources> res;
        ctx->GetResources((IResources**)&res);
        assert(res);
        if (mFirst) {
            // make sure touch mode code executes by setting cached value
            // to opposite of the added touch mode.
            //
            mAttachInfo->mInTouchMode = !mAddedTouchMode;
            EnsureTouchModeLocally(mAddedTouchMode);
        }
        else {
            Boolean equals;
            mPendingContentInsets->EqualsEx(mAttachInfo->mContentInsets, &equals);
            if (!equals) {
                insetsChanged = TRUE;
            }
            mPendingVisibleInsets->EqualsEx(mAttachInfo->mVisibleInsets, &equals);
            if (!equals) {
                mAttachInfo->mVisibleInsets->SetEx(mPendingVisibleInsets);
                if (DEBUG_LAYOUT)
                    Logger::V(TAG, "Visible insets changing to: "
                        /*+ mAttachInfo->mVisibleInsets*/);
            }
            if (lp->mWidth == IViewGroupLayoutParams::WRAP_CONTENT
                || lp->mHeight == IViewGroupLayoutParams::WRAP_CONTENT) {
                windowSizeMayChange = TRUE;

                if (lp->mType == IWindowManagerLayoutParams::TYPE_STATUS_BAR_PANEL) {
                    // NOTE -- system code, won't try to do compat mode.
                    AutoPtr<IPoint> size;
                    CPoint::New((IPoint**)&size);
                    mDisplay->GetRealSize(size);
                    size->GetX(&desiredWindowWidth);
                    size->GetY(&desiredWindowHeight);
                }
                else {
                    AutoPtr<IDisplayMetrics> packageMetrics;
                    res->GetDisplayMetrics((IDisplayMetrics**)&packageMetrics);
                    assert(packageMetrics);

                    packageMetrics->GetWidthPixels(&desiredWindowWidth);
                    packageMetrics->GetHeightPixels(&desiredWindowHeight);
                }
            }
        }

        // Ask host how big it wants to be
        windowSizeMayChange |= MeasureHierarchy(host, lp, res,
                desiredWindowWidth, desiredWindowHeight);
    }
    if (CollectViewAttributes()) {
        params = lp;
    }

    if (attachInfo->mForceReportNewAttributes) {
        attachInfo->mForceReportNewAttributes = FALSE;
        params = lp;
    }

    if (mFirst || attachInfo->mViewVisibilityChanged) {
        attachInfo->mViewVisibilityChanged = FALSE;
        Int32 resizeMode = mSoftInputMode &
            IWindowManagerLayoutParams::SOFT_INPUT_MASK_ADJUST;

        // If we are in auto resize mode, then we need to determine
        // what mode to use now.
        //
        if (resizeMode == IWindowManagerLayoutParams::SOFT_INPUT_ADJUST_UNSPECIFIED) {
            List<View* >::Iterator iter = attachInfo->mScrollContainers.Begin();
            for (; iter!=attachInfo->mScrollContainers.End(); ++iter) {
                if ((*iter)->IsShown()) {
                    resizeMode = IWindowManagerLayoutParams::SOFT_INPUT_ADJUST_RESIZE;
                }
            }

            if (resizeMode == 0) {
                resizeMode = IWindowManagerLayoutParams::SOFT_INPUT_ADJUST_PAN;
            }

            if ((lp->mSoftInputMode &
                IWindowManagerLayoutParams::SOFT_INPUT_MASK_ADJUST) != resizeMode) {
                    lp->mSoftInputMode = (lp->mSoftInputMode &
                        ~IWindowManagerLayoutParams::SOFT_INPUT_MASK_ADJUST) |
                        resizeMode;
                params = lp;
            }
        }
    }
    if (params != NULL && (host->mPrivateFlags & View::PFLAG_REQUEST_TRANSPARENT_REGIONS) != 0) {
        if (!PixelFormat::FormatHasAlpha(params->mFormat)) {
            params->mFormat = IPixelFormat::TRANSLUCENT;
        }
    }

    if (mFitSystemWindowsRequested) {
        mFitSystemWindowsRequested = FALSE;
        mFitSystemWindowsInsets->SetEx(mAttachInfo->mContentInsets);
        host->FitSystemWindows(mFitSystemWindowsInsets);
        if (mLayoutRequested) {
            // Short-circuit catching a new layout request here, so
            // we don't need to go through two layout passes when things
            // change due to fitting system windows, which can happen a lot.
            AutoPtr<IContext> ctx;
            mView->GetContext((IContext**)&ctx);
            assert(ctx);

            AutoPtr<IResources> res;
            ctx->GetResources((IResources**)&res);
            assert(res);
            windowSizeMayChange |= MeasureHierarchy(
                host, lp, res, desiredWindowWidth, desiredWindowHeight);
        }
    }

    if (layoutRequested) {
        // Clear this now, so that if anything requests a layout in the
        // rest of this function we will catch it and re-run a full
        // layout pass.
        mLayoutRequested = FALSE;
    }

    Int32 frameWidth, framewHeight;
    frame->GetWidth(&frameWidth);
    frame->GetHeight(&framewHeight);

    Boolean windowShouldResize = layoutRequested && windowSizeMayChange
        && ((mWidth != host->GetMeasuredWidth() || mHeight != host->GetMeasuredHeight())
        || (lp->mWidth == IViewGroupLayoutParams::WRAP_CONTENT &&
        frameWidth < desiredWindowWidth && frameWidth != mWidth)
        || (lp->mHeight == IViewGroupLayoutParams::WRAP_CONTENT &&
        framewHeight < desiredWindowHeight && framewHeight != mHeight));

    Boolean computesInternalInsets;
    attachInfo->mTreeObserver->HasComputeInternalInsetsListeners(&computesInternalInsets);

    Boolean insetsPending = FALSE;
    Int32 relayoutResult = 0;
    if (mFirst || windowShouldResize || insetsChanged
        || viewVisibilityChanged || params != NULL) {
        if (viewVisibility == IView::VISIBLE) {
            // If this window is giving internal insets to the window
            // manager, and it is being added or changing its visibility,
            // then we want to first give the window manager "fake"
            // insets to cause it to effectively ignore the content of
            // the window during layout.  This avoids it briefly causing
            // other windows to resize/move based on the raw frame of the
            // window, waiting until we can finish laying out this window
            // and get back to the window manager with the ultimately
            // computed insets.
            //
            insetsPending = computesInternalInsets
                && (mFirst || viewVisibilityChanged);
        }

        if (mSurfaceHolder != NULL) {
            mSurfaceHolder->mSurfaceLock.Lock();
            mDrawingAllowed = TRUE;
        }

        Boolean hwInitialized = FALSE;
        Boolean contentInsetsChanged = FALSE;
        Boolean visibleInsetsChanged;
        Boolean hadSurface = FALSE;
        mSurface->IsValid(&hadSurface);

        if (DEBUG_LAYOUT) {
           Logger::I(TAG, "host=w:%d, h:%d, params=%p", host->GetMeasuredWidth(),
                host->GetMeasuredHeight(), params);
        }

        Int32 surfaceGenerationId;
        mSurface->GetGenerationId(&surfaceGenerationId);
        relayoutResult = RelayoutWindow(params, viewVisibility, insetsPending);

        if (DEBUG_LAYOUT) {
            String fs, cs, vs;
            frame->ToShortString(&fs);
            mPendingContentInsets->ToShortString(&cs);
            mPendingVisibleInsets->ToShortString(&vs);
            Logger::V(TAG, "relayout: frame=%s, content=%s, visible=%s, surface=%p",
                    fs.string(), cs.string(), vs.string(), mSurface.Get());
        }

        Int32 seq;
        mPendingConfiguration->GetSeq(&seq);
        if (seq != 0) {
            if (DEBUG_CONFIGURATION) {
                Logger::D(TAG, "Visible with new config: "
                    /*+ mPendingConfiguration*/);
            }
            UpdateConfiguration(mPendingConfiguration, !mFirst);
            mPendingConfiguration->SetSeq(0);
        }

        mPendingContentInsets->EqualsEx(
            mAttachInfo->mContentInsets, &contentInsetsChanged);
        contentInsetsChanged = !contentInsetsChanged;
        mPendingVisibleInsets->EqualsEx(
            mAttachInfo->mVisibleInsets, &visibleInsetsChanged);
        visibleInsetsChanged = !visibleInsetsChanged;

        if (contentInsetsChanged) {
            Logger::V(TAG, "TODO, ViewRootImpl::PerformTraversals LINE:%d", __LINE__);
            // TODO
            // Boolean isValid;
            // if (mWidth > 0 && mHeight > 0 && lp != NULL &&
            //     ((lp->mSystemUiVisibility|lp->mSubtreeSystemUiVisibility)
            //         & IView::SYSTEM_UI_LAYOUT_FLAGS) == 0 &&
            //     mSurface != NULL && (mSurface->IsValid(&isValid), isValid) &&
            //     !mAttachInfo->mTurnOffWindowResizeAnim &&
            //     mAttachInfo->mHardwareRenderer != NULL &&
            //     mAttachInfo->mHardwareRenderer->IsEnabled() &&
            //     mAttachInfo->mHardwareRenderer->Validate() &&
            //     lp != NULL && !CPixelFormat::FormatHasAlpha(lp->mFormat)) {

            //     DisposeResizeBuffer();

            //     Boolean completed = FALSE;
            //     AutoPtr<HardwareCanvas> hwRendererCanvas =
            //         mAttachInfo->mHardwareRenderer->GetCanvas();
            //     AutoPtr<HardwareCanvas> layerCanvas;
            //     //try {
            //         if (mResizeBuffer == NULL) {
            //             mResizeBuffer = mAttachInfo->mHardwareRenderer->CreateHardwareLayer(
            //                     mWidth, mHeight, FALSE);
            //         }
            //         else if (mResizeBuffer->GetWidth() != mWidth ||
            //             mResizeBuffer->GetHeight() != mHeight) {
            //             mResizeBuffer->Resize(mWidth, mHeight);
            //         }
            //         // TODO: should handle create/resize failure
            //         layerCanvas = mResizeBuffer->Start(hwRendererCanvas);
            //         layerCanvas->SetViewport(mWidth, mHeight);
            //         layerCanvas->OnPreDraw(NULL);
            //         Int32 restoreCount;
            //         layerCanvas->Save(&restoreCount);

            //         Int32 yoff = mScrollY;
            //         // Boolean scrolling = mScroller != NULL
            //         //     && mScroller->ComputeScrollOffset();
            //         // if (scrolling) {
            //         //     yoff = mScroller->GetCurrY();
            //         //     mScroller->AbortAnimation();
            //         // }
            //         // else {
            //         //     yoff = mScrollY;
            //         // }

            //         layerCanvas->Translate(0, -yoff);
            //         if (mTranslator != NULL) {
            //             mTranslator->TranslateCanvas(layerCanvas);
            //         }

            //         AutoPtr<IDisplayList> displayList = mView->mDisplayList;
            //         if (displayList != NULL) {
            //             layerCanvas->DrawDisplayList(displayList, NULL,
            //                     IDisplayList::FLAG_CLIP_CHILDREN);
            //         }
            //         else {
            //             mView->Draw(layerCanvas);
            //         }

            //         DrawAccessibilityFocusedDrawableIfNeeded(layerCanvas);

            //         mResizeBufferStartTime = SystemClock::GetUptimeMillis();

            //         AutoPtr<IResources> res;
            //         mView->GetResources((IResources**)&res);
            //         assert(res);
            //         res->GetInteger(
            //             R::integer::config_mediumAnimTime, &mResizeBufferDuration);
            //         completed = TRUE;

            //         layerCanvas->RestoreToCount(restoreCount);
            //     // } catch (OutOfMemoryError e) {
            //     //     Logger::W(TAG, "Not enough memory for content change anim buffer", e);
            //     // } finally {
            //         if (layerCanvas != NULL) {
            //             layerCanvas->OnPostDraw();
            //         }
            //         if (mResizeBuffer != NULL) {
            //             mResizeBuffer->End(hwRendererCanvas);
            //             if (!completed) {
            //                 mResizeBuffer->Destroy();
            //                 mResizeBuffer = NULL;
            //             }
            //         }
            //     //}
            // }
            mAttachInfo->mContentInsets->SetEx(mPendingContentInsets);

            if (DEBUG_LAYOUT) {
                Logger::D(TAG, "Content insets changing to: "
                    /*+ mAttachInfo->mContentInsets*/);
            }
        }
        if (contentInsetsChanged || mLastSystemUiVisibility !=
            mAttachInfo->mSystemUiVisibility || mFitSystemWindowsRequested) {
            mLastSystemUiVisibility = mAttachInfo->mSystemUiVisibility;
            mFitSystemWindowsRequested = FALSE;
            mFitSystemWindowsInsets->SetEx(mAttachInfo->mContentInsets);
            host->FitSystemWindows(mFitSystemWindowsInsets);
        }

        if (visibleInsetsChanged) {
            mAttachInfo->mVisibleInsets->SetEx(mPendingVisibleInsets);

            if (DEBUG_LAYOUT) {
                Logger::D(TAG, "Visible insets changing to: "
                    /*+ mAttachInfo->mVisibleInsets*/);
            }
        }

        Boolean surfaceValid = FALSE;
        mSurface->IsValid(&surfaceValid);
        Int32 gId;
        if (!hadSurface) {
            if (surfaceValid) {
                // If we are creating a new surface, then we need to
                // completely redraw it.  Also, when we get to the
                // point of drawing it we will hold off and schedule
                // a new traversal instead.  This is so we can tell the
                // window manager about all of the windows being displayed
                // before actually drawing them, so it can display then
                // all at once.
                //
                newSurface = TRUE;
                mFullRedrawNeeded = TRUE;
                mPreviousTransparentRegion->SetEmpty();

                // if (mAttachInfo->mHardwareRenderer != NULL) {
                //     AutoPtr<ISurface> surface;
                //     mHolder->GetSurface((ISurface**)&surface);
                //     if (FAILED(mAttachInfo->mHardwareRenderer->Initialize(
                //         surface, &hwInitialized)) {
                //         Logger::E(TAG, "OutOfResourcesException initializing HW surface");

                //         if (!mWindowSession->OutOfMemory(mWindow) &&
                //             CProcess::MyUid() != IProcess::SYSTEM_UID) {
                //             Slogger::W(TAG, "No processes killed for memory; killing self");
                //             CProcess::KillProcess(CProcess::MyPid());
                //         }

                //         mLayoutRequested = TRUE;    // ask wm for a new surface next time.
                //         return;
                //     }
                // }
            }
        }
        else if (!surfaceValid) {
            // If the surface has been removed, then reset the scroll
            // positions.
            mLastScrolledFocus = NULL;
            mScrollY = mCurScrollY = 0;
            // if (mScroller != NULL) {
            //     mScroller->AbortAnimation();
            // }
            DisposeResizeBuffer();
            // Our surface is gone
            // if (mAttachInfo->mHardwareRenderer != NULL &&
            //     mAttachInfo->mHardwareRenderer->IsEnabled()) {
            //     mAttachInfo->mHardwareRenderer->Destroy(TRUE);
            // }
        }
        // else if (surfaceGenerationId != (mSurface->GetGenerationId(&gId), gId) &&
        //     mSurfaceHolder == NULL && mAttachInfo->mHardwareRenderer != NULL) {
        //     mFullRedrawNeeded = TRUE;
        //     AutoPtr<ISurface> surface;
        //     mHolder->GetSurface((ISurface**)&surface);
        //     if (FAILED(mAttachInfo->mHardwareRenderer->UpdateSurface(surface)) {
        //         Logger::E(TAG, "OutOfResourcesException updating HW surface");

        //         if (!mWindowSession->OutOfMemory(mWindow) &&
        //             CProcess::MyUid() != IProcess::SYSTEM_UID) {
        //             Slogger::W(TAG, "No processes killed for memory; killing self");
        //             CProcess::KillProcess(CProcess::MyPid());
        //         }

        //         mLayoutRequested = TRUE;    // ask wm for a new surface next time.
        //         return;
        //     }
        // }

        if (DEBUG_ORIENTATION) {
            String fs;
            frame->ToShortString(&fs);
            Logger::V(TAG, "Relayout returned: frame=%s", fs.string());
        }

        attachInfo->mWindowLeft = frame->mLeft;
        attachInfo->mWindowTop = frame->mTop;

        // !!FIXME!! This next section handles the case where we did not get the
        // window size we asked for. We should avoid this by getting a maximum size from
        // the window session beforehand.
        //
        frame->GetWidth(&mWidth);
        frame->GetHeight(&mHeight);
        if (mSurfaceHolder != NULL) {
            // The app owns the surface; tell it about what is going on.
            Boolean isValid = FALSE;
            mSurface->IsValid(&isValid);
            if (isValid) {
                // XXX .copyFrom() doesn't work!
                //mSurfaceHolder.mSurface.copyFrom(mSurface);
                mSurfaceHolder->mSurface = mSurface;
            }

            mSurfaceHolder->SetSurfaceFrameSize(mWidth, mHeight);
            mSurfaceHolder->mSurfaceLock.Unlock();
            mSurface->IsValid(&isValid);
            if (isValid) {
                if (!hadSurface) {
                    mSurfaceHolder->UngetCallbacks();

                    mIsCreating = TRUE;
                    mSurfaceHolderCallback->SurfaceCreated(mSurfaceHolder);
                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks =
                        mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        (*iter).Get()->SurfaceCreated(mSurfaceHolder);
                    }
                    surfaceChanged = TRUE;
                }

                if (surfaceChanged) {
                    mSurfaceHolderCallback->SurfaceChanged(mSurfaceHolder,
                        lp->mFormat, mWidth, mHeight);

                    Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks =
                        mSurfaceHolder->GetCallbacks();
                    Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                    for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                        (*iter).Get()->SurfaceChanged(
                            mSurfaceHolder, lp->mFormat, mWidth, mHeight);
                    }
                }
                mIsCreating = FALSE;
            }
            else if (hadSurface) {
                mSurfaceHolder->UngetCallbacks();

                mSurfaceHolderCallback->SurfaceDestroyed(mSurfaceHolder);

                Vector<AutoPtr<ISurfaceHolderCallback> >& callbacks =
                    mSurfaceHolder->GetCallbacks();
                Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
                for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                    (*iter).Get()->SurfaceDestroyed(mSurfaceHolder);
                }

                mSurfaceHolder->mSurfaceLock.Lock();
                mSurfaceHolder->mSurface = NULL;
                CSurface::New((ISurface**)&(mSurfaceHolder->mSurface));
                mSurfaceHolder->mSurfaceLock.Unlock();
            }
        }
        // if (mAttachInfo->mHardwareRenderer != NULL &&
        //     mAttachInfo->mHardwareRenderer->IsEnabled()) {
        //     if (hwInitialized || windowShouldResize ||
        //         mWidth != mAttachInfo->mHardwareRenderer->GetWidth() ||
        //         mHeight != mAttachInfo->mHardwareRenderer->GetHeight()) {
        //         mAttachInfo->mHardwareRenderer->Setup(mWidth, mHeight);
        //         if (!hwInitialized) {
        //             AutoPtr<ISurface> surface;
        //             mHolder->GetSurface((ISurface**)&surface);
        //             mAttachInfo->mHardwareRenderer->Invalidate(surface);
        //             mFullRedrawNeeded = TRUE;
        //         }
        //     }
        // }

        if (!mStopped) {
            Boolean focusChangedDueToTouchMode = EnsureTouchModeLocally(
                (relayoutResult & IWindowManagerGlobal::RELAYOUT_RES_IN_TOUCH_MODE) != 0);
            if (focusChangedDueToTouchMode || mWidth != host->GetMeasuredWidth()
                || mHeight != host->GetMeasuredHeight() || contentInsetsChanged) {
                Int32 childWidthMeasureSpec = GetRootMeasureSpec(mWidth, lp->mWidth);
                Int32 childHeightMeasureSpec = GetRootMeasureSpec(mHeight, lp->mHeight);

                if (DEBUG_LAYOUT) {
                    Logger::D(TAG, "Ooops, something changed!  mWidth=%d measuredWidth=%d"
                        " measuredWidth%d mHeight=%d measuredHeight=%d coveredInsetsChanged=%d"
                        , mWidth, host->GetMeasuredWidth(), mHeight, host->GetMeasuredHeight()
                        , contentInsetsChanged);
                }

                // Ask host how big it wants to be
                PerformMeasure(childWidthMeasureSpec, childHeightMeasureSpec);

                // Implementation of weights from IWindowManagerLayoutParams*
                // We just grow the dimensions as needed and re-measure if
                // needs be
                //
                Int32 width = host->GetMeasuredWidth();
                Int32 height = host->GetMeasuredHeight();
                Boolean measureAgain = FALSE;

                if (lp->mHorizontalWeight > 0.0f) {
                    width += (Int32) ((mWidth - width) * lp->mHorizontalWeight);
                    childWidthMeasureSpec = View::MeasureSpec::MakeMeasureSpec(
                        width, View::MeasureSpec::EXACTLY);
                    measureAgain = TRUE;
                }
                if (lp->mVerticalWeight > 0.0f) {
                    height += (Int32) ((mHeight - height) * lp->mVerticalWeight);
                    childHeightMeasureSpec = View::MeasureSpec::MakeMeasureSpec(
                        height, View::MeasureSpec::EXACTLY);
                    measureAgain = TRUE;
                }

                if (measureAgain) {
                    if (DEBUG_LAYOUT) {
                        Logger::D(TAG, "And hey let's measure once more: width=%d"
                            " height=%d", width, height);
                    }
                    PerformMeasure(childWidthMeasureSpec, childHeightMeasureSpec);
                }

                mLayoutRequested = TRUE;
            }
        }
    }
    else {
        // Not the first pass and no window/insets/visibility change but the window
        // may have moved and we need check that and if so to update the left and right
        // in the attach info. We translate only the window frame since on window move
        // the window manager tells us only for the new frame but the insets are the
        // same and we do not want to translate them more than once.

        // TODO: Well, we are checking whether the frame has changed similarly
        // to how this is done for the insets. This is however incorrect since
        // the insets and the frame are translated. For example, the old frame
        // was (1, 1 - 1, 1) and was translated to say (2, 2 - 2, 2), now the new
        // reported frame is (2, 2 - 2, 2) which implies no change but this is not
        // TRUE since we are comparing a not translated value to a translated one.
        // This scenario is rare but we may want to fix that.

        Boolean windowMoved = (attachInfo->mWindowLeft != frame->mLeft
                || attachInfo->mWindowTop != frame->mTop);
        if (windowMoved) {
            if (mTranslator != NULL) {
                mTranslator->TranslateRectInScreenToAppWinFrame(frame);
            }
            attachInfo->mWindowLeft = frame->mLeft;
            attachInfo->mWindowTop = frame->mTop;
        }
    }
    const Boolean didLayout = layoutRequested && !mStopped;
    Boolean triggerGlobalLayoutListener = didLayout
        || attachInfo->mRecomputeGlobalAttributes;
    if (didLayout) {
        PerformLayout();

        if ((host->mPrivateFlags & View::PFLAG_REQUEST_TRANSPARENT_REGIONS) != 0) {
            // start out transparent
            // TODO: AVOID THAT CALL BY CACHING THE RESULT?
            //
            host->GetLocationInWindow(mTmpLocation, mTmpLocation + 1);
            Boolean result;
            mTransparentRegion->SetEx2(mTmpLocation[0], mTmpLocation[1],
                mTmpLocation[0] + host->mRight - host->mLeft,
                mTmpLocation[1] + host->mBottom - host->mTop, &result);

            host->GatherTransparentRegion(mTransparentRegion);
            if (mTranslator != NULL) {
                mTranslator->TranslateRegionInWindowToScreen(mTransparentRegion);
            }

            Boolean isEqual;
            mTransparentRegion->EqualsEx(mPreviousTransparentRegion, &isEqual);
            if (!isEqual) {
                mPreviousTransparentRegion->Set(mTransparentRegion, &result);
                // reconfigure window manager
                mWindowSession->SetTransparentRegion(mWindow, mTransparentRegion);
            }
        }

        if (DBG) {
            Logger::D(TAG, "======================================");
            Logger::D(TAG, "performTraversals -- after setFrame");
            host->Debug();
        }
    }
    if (triggerGlobalLayoutListener) {
        attachInfo->mRecomputeGlobalAttributes = FALSE;
        attachInfo->mTreeObserver->DispatchOnGlobalLayout();

        Boolean isEnabled = FALSE;
        AutoPtr<IAccessibilityManager> manager;
        CAccessibilityManager::GetInstance(host->mContext, (IAccessibilityManager**)&manager);
        manager->IsEnabled(&isEnabled);
        if (isEnabled) {
            PostSendWindowContentChangedCallback(mView);
        }
    }

    if (computesInternalInsets) {
        // Clear the original insets.
        ViewTreeObserver::InternalInsetsInfo* insets =
            (ViewTreeObserver::InternalInsetsInfo*)attachInfo->mGivenInternalInsets.Get();
        insets->Reset();

        // Compute new insets in place.
        attachInfo->mTreeObserver->DispatchOnComputeInternalInsets(insets);

        // Tell the window manager.
        Boolean equal = FALSE;
        mLastGivenInsets->Equals(insets, &equal);
        if (insetsPending || !equal) {
            ((ViewTreeObserver::InternalInsetsInfo*)mLastGivenInsets.Get())->Set(insets);

            // Translate insets to screen coordinates if needed.
            AutoPtr<IRect> contentInsets;
            AutoPtr<IRect> visibleInsets;
            AutoPtr<IRegion> touchableRegion;
            if (mTranslator != NULL) {
                mTranslator->GetTranslatedContentInsets(
                    insets->mContentInsets, (IRect**)&contentInsets);
                mTranslator->GetTranslatedVisibleInsets(
                    insets->mVisibleInsets, (IRect**)&visibleInsets);
                mTranslator->GetTranslatedTouchableArea(
                    insets->mTouchableRegion, (IRegion**)&touchableRegion);
            }
            else {
                contentInsets = insets->mContentInsets;
                visibleInsets = insets->mVisibleInsets;
                touchableRegion = insets->mTouchableRegion;
            }

            mWindowSession->SetInsets(
                mWindow, insets->mTouchableInsets,
                contentInsets, visibleInsets, touchableRegion);
        }
    }
    Boolean skipDraw = FALSE;

    if (mFirst) {
        // handle first focus request
        //if (DEBUG_INPUT_RESIZE) {
        //    Logger::D(TAG, StringBuffer("First: mView.hasFocus()=")
        //    + mView->HasFocus());
        //}

        if (mView != NULL) {
            Boolean hasFocus;
            mView->HasFocus(&hasFocus);
            if (!hasFocus) {
                Boolean result;
                mView->RequestFocusEx(IView::FOCUS_FORWARD, &result);
                AutoPtr<IView> temp;
                mView->FindFocus((IView**)&temp);
                mRealFocusedView = temp;
                mFocusedView = mRealFocusedView;
                if (DEBUG_INPUT_RESIZE) {
                    Logger::D(TAG, "First: requested focused view=0x%08x",
                        (Int32)mFocusedView.Get());
                }
            }
            else {
                AutoPtr<IView> temp;
                mView->FindFocus((IView**)&temp);
                mRealFocusedView = temp;
                if (DEBUG_INPUT_RESIZE) {
                    Logger::D(TAG, "First: existing focused view=0x%08x",
                        (Int32)mRealFocusedView.Get());
                }
            }
        }
        if ((relayoutResult & IWindowManagerGlobal::RELAYOUT_RES_ANIMATING) != 0) {
            // The first time we relayout the window, if the system is
            // doing window animations, we want to hold of on any future
            // draws until the animation is done.
            mWindowsAnimating = TRUE;
        }
    }
    else if (mWindowsAnimating) {
        skipDraw = TRUE;
    }
    mFirst = FALSE;
    mWillDrawSoon = FALSE;
    mNewSurfaceNeeded = FALSE;
    mViewVisibility = viewVisibility;

    if (mAttachInfo->mHasWindowFocus) {
        const Boolean imTarget =
            CWindowManagerLayoutParams::MayUseInputMethod(mWindowAttributes->mFlags);
        if (imTarget != mLastWasImTarget) {
            mLastWasImTarget = imTarget;
            AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL && imTarget) {
               imm->StartGettingWindowFocus(mView);
               AutoPtr<IView> focView;
               mView->FindFocus((IView**)&focView);
               imm->OnWindowFocus(mView, focView,
                   mWindowAttributes->mSoftInputMode,
                   !mHasHadWindowFocus, mWindowAttributes->mFlags);
            }
        }
    }

    // Remember if we must report the next draw.
    if ((relayoutResult & IWindowManagerGlobal::RELAYOUT_RES_FIRST_TIME) != 0) {
        mReportNextDraw = TRUE;
    }
    Boolean cancelDraw = FALSE;
    attachInfo->mTreeObserver->DispatchOnPreDraw(&cancelDraw);

    cancelDraw = cancelDraw || viewVisibility != IView::VISIBLE;
    if (!cancelDraw && !newSurface) {
        if (!skipDraw || mReportNextDraw) {
            if (mPendingTransitions.IsEmpty() == FALSE) {
                List<AutoPtr<ILayoutTransition> >::Iterator iter;
                for (iter = mPendingTransitions.Begin();
                    iter != mPendingTransitions.End(); ++iter) {
                    (*iter)->StartChangingAnimations();
                }
                mPendingTransitions.Clear();
            }

            PerformDraw();
        }
    }
    else {
        if (viewVisibility == IView::VISIBLE) {
            // Try again
            ScheduleTraversals();
        }
        else if (mPendingTransitions.IsEmpty() == FALSE) {
            List<AutoPtr<ILayoutTransition> >::Iterator iter;
            for (iter = mPendingTransitions.Begin();
                iter != mPendingTransitions.End(); ++iter) {
                (*iter)->EndChangingAnimations();
            }
            mPendingTransitions.Clear();
        }
    }

    mIsInTraversal = FALSE;

    LocalTrace(mView, "::LEAVE << ViewRootImpl::PerformTraversals()");
}

void ViewRootImpl::PerformMeasure(
    /* [in] */ Int32 childWidthMeasureSpec,
    /* [in] */ Int32 childHeightMeasureSpec)
{
    LocalTrace(mView, "::ENTER >> ViewRootImpl::PerformMeasure()");

    //Trace::TraceBegin(Trace::TRACE_TAG_VIEW, "measure");
    mView->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
    //Trace::TraceEnd(Trace::TRACE_TAG_VIEW);

    LocalTrace(mView, "::LEAVE << ViewRootImpl::PerformMeasure()");
}

void ViewRootImpl::PerformLayout()
{
    LocalTrace(mView, "::ENTER >> ViewRootImpl::PerformLayout()");

    mLayoutRequested = FALSE;
    mScrollMayChange = TRUE;

    View* host = VIEW_PROBE(mView);

    if (DEBUG_ORIENTATION || DEBUG_LAYOUT) {
        Logger::V(TAG, "Laying out 0x%08x to (%d, %d)",
            host, host->GetMeasuredWidth(), host->GetMeasuredHeight());
    }

    //Trace::TraceBegin(Trace::TRACE_TAG_VIEW, "layout");
    host->Layout(0, 0, host->GetMeasuredWidth(), host->GetMeasuredHeight());
    //Trace::TraceEnd(Trace::TRACE_TAG_VIEW);

    LocalTrace(mView, "::LEAVE << ViewRootImpl::PerformLayout()");
}

ECode ViewRootImpl::RequestTransparentRegion(
    /* [in] */ IView* child)
{
    // the test below should not fail unless someone is messing with us
    //
    FAIL_RETURN(CheckThread());

    if (mView.Get() == child) {
        VIEW_PROBE(mView)->mPrivateFlags
            |= View::PFLAG_REQUEST_TRANSPARENT_REGIONS;

        // Need to make sure we re-evaluate the window attributes next
        // time around, to ensure the window has the correct format.
        //
        mWindowAttributesChanged = TRUE;
        mWindowAttributesChangesFlag = 0;
        RequestLayout();
    }

    return NOERROR;
}

Int32 ViewRootImpl::GetRootMeasureSpec(
    /* [in] */ Int32 windowSize,
    /* [in] */ Int32 rootDimension)
{
    Int32 measureSpec;
    switch (rootDimension) {
        case IViewGroupLayoutParams::MATCH_PARENT:
            // Window can't resize. Force root view to be windowSize.
            measureSpec = View::MeasureSpec::MakeMeasureSpec(
                windowSize, View::MeasureSpec::EXACTLY);
            break;
        case IViewGroupLayoutParams::WRAP_CONTENT:
            // Window can resize. Set max size for root view.
            measureSpec = View::MeasureSpec::MakeMeasureSpec(
                windowSize, View::MeasureSpec::AT_MOST);
            break;
        default:
            // Window wants to be an exact size. Force root view to be that size.
            measureSpec = View::MeasureSpec::MakeMeasureSpec(
                rootDimension, View::MeasureSpec::EXACTLY);
            break;
    }

    return measureSpec;
}

void ViewRootImpl::OnHardwarePreDraw(
    /* [in] */ IHardwareCanvas* canvas)
{
    //canvas->Translate(0, -mHardwareYOffset);
}

void ViewRootImpl::OnHardwarePostDraw(
    /* [in] */ IHardwareCanvas* canvas)
{
    // if (mResizeBuffer != NULL) {
    //     mResizePaint->SetAlpha(mResizeAlpha);
    //     canvas->DrawHardwareLayer(mResizeBuffer, 0.0f, mHardwareYOffset, mResizePaint);
    // }
    // DrawAccessibilityFocusedDrawableIfNeeded(canvas);
}

/**
 * @hide
 */
void ViewRootImpl::OutputDisplayList(
    /* [in] */ IView* view)
{
    // if (mAttachInfo != NULL && mAttachInfo->mHardwareCanvas != NULL) {
    //     AutoPtr<IDisplayList> displayList;
    //     view->GetDisplayList(&displayList);
    //     if (displayList != NULL) {
    //         mAttachInfo->mHardwareCanvas->OutputDisplayList(displayList);
    //     }
    // }
}

/**
 * @see #PROPERTY_PROFILE_RENDERING
 */
void ViewRootImpl::ProfileRendering(
    /* [in] */ Boolean enabled)
{
    if (mProfileRendering) {
        mRenderProfilingEnabled = enabled;
        if (mRenderProfiler == NULL) {
            AutoPtr<IWeakReferenceSource> wrs = THIS_PROBE(IWeakReferenceSource);
            AutoPtr<IWeakReference> wr;
            wrs->GetWeakReference((IWeakReference**)&wr);
            AutoPtr<IRunnable> runnable = new RenderProfileRunnable(wr);
            CThread::New(runnable, String("Rendering Profiler"),
                (IThread**)&mRenderProfiler);
            mRenderProfiler->Start();
        }
        else {
            mRenderProfiler->Interrupt();
            mRenderProfiler = NULL;
        }
    }
}

/**
 * Called from draw() when DEBUG_FPS is enabled
 */
void ViewRootImpl::TrackFPS()
{
    // Tracks frames per second drawn. First value in a series of draws may be bogus
    // because it down not account for the intervening idle time
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 nowTime;
    system->GetCurrentTimeMillis(&nowTime);
    if (mFpsStartTime < 0) {
        mFpsStartTime = mFpsPrevTime = nowTime;
        mFpsNumFrames = 0;
    }
    else {
        ++mFpsNumFrames;
        Int64 frameTime = nowTime - mFpsPrevTime;
        Int64 totalTime = nowTime - mFpsStartTime;
        Logger::V(TAG, "0x%08x\tFrame time:\t%lld", this, frameTime);
        mFpsPrevTime = nowTime;
        if (totalTime > 1000) {
            Float fps = (Float)mFpsNumFrames * 1000 / totalTime;
            Logger::V(TAG, "0x%08x\tFPS time:\t%f", this, fps);
            mFpsStartTime = nowTime;
            mFpsNumFrames = 0;
        }
    }
}

void ViewRootImpl::PerformDraw()
{
    LocalTrace(mView, "::ENTER >> ViewRootImpl::PerformDraw()");

    if (!mAttachInfo->mScreenOn && !mReportNextDraw) {
        return;
    }

    Boolean fullRedrawNeeded = mFullRedrawNeeded;
    mFullRedrawNeeded = FALSE;

    mIsDrawing = TRUE;
    //Trace::TraceBegin(Trace::TRACE_TAG_VIEW, "draw");

    Draw(fullRedrawNeeded);

    mIsDrawing = FALSE;
    //Trace::TraceEnd(Trace::TRACE_TAG_VIEW);

    if (mReportNextDraw) {
        mReportNextDraw = FALSE;

        if (LOCAL_LOGV) {
            AutoPtr<ICharSequence> title;
            mWindowAttributes->GetTitle((ICharSequence**)&title);
            String str;
            title->ToString(&str);
            Logger::V(TAG, "FINISHED DRAWING: %s", str.string());
        }

        Boolean isValid;
        if (mSurfaceHolder != NULL && (mSurface->IsValid(&isValid), isValid)) {
            mSurfaceHolderCallback->SurfaceRedrawNeeded(mSurfaceHolder);
            Vector<AutoPtr<ISurfaceHolderCallback> > callbacks =
                mSurfaceHolder->GetCallbacks();
            Vector<AutoPtr<ISurfaceHolderCallback> >::Iterator iter;
            for (iter = callbacks.Begin(); iter != callbacks.End(); ++iter) {
                ISurfaceHolderCallback2* callback2 =
                    (ISurfaceHolderCallback2*)(*iter)->Probe(EIID_ISurfaceHolderCallback2);
                if (callback2) {
                    callback2->SurfaceRedrawNeeded(mSurfaceHolder);
                }
            }
        }

        mWindowSession->FinishDrawing(mWindow);
    }

    LocalTrace(mView, "::LEAVE << ViewRootImpl::PerformDraw()");
}

void ViewRootImpl::Draw(
    /* [in] */ Boolean fullRedrawNeeded)
{
    CSurface* surface = mSurface;
    Boolean surfaceValid = FALSE;
    if (surface == NULL ||
        !(surface->IsValid(&surfaceValid), surfaceValid)) {
        return;
    }

    if (DEBUG_FPS) {
        TrackFPS();
    }

    if (!sFirstDrawComplete) {
        Mutex::Autolock lock(sFirstDrawHandlersLock);

        sFirstDrawComplete = TRUE;
        Boolean bval;
        List<AutoPtr<IRunnable> >::Iterator iter = sFirstDrawHandlers.Begin();
        for (; iter != sFirstDrawHandlers.End(); ++iter) {
            mHandler->Post(iter->Get(), &bval);
        }
    }

    ScrollToRectOrFocus(NULL, FALSE);

    if (mAttachInfo->mViewScrollChanged) {
        mAttachInfo->mViewScrollChanged = FALSE;
        mAttachInfo->mTreeObserver->DispatchOnScrollChanged();
    }

    Int32 yoff = mScrollY;
    // const Boolean animating = mScroller != NULL && mScroller->ComputeScrollOffset();

    // if (animating) {
    //     yoff = mScroller->GetCurrY();
    // }
    // else {
    //     yoff = mScrollY;
    // }

    if (mCurScrollY != yoff) {
        mCurScrollY = yoff;
        fullRedrawNeeded = TRUE;
    }

    Float appScale = mAttachInfo->mApplicationScale;
    Boolean scalingRequired = mAttachInfo->mScalingRequired;

    Int32 resizeAlpha = 0;
    if (mResizeBuffer != NULL) {
        // Int64 deltaTime = SystemClock::GetUptimeMillis() - mResizeBufferStartTime;
        // if (deltaTime < mResizeBufferDuration) {
        //     Float amt = deltaTime/(Float)mResizeBufferDuration;
        //     Float newAmt;
        //     mResizeInterpolator->GetInterpolation(amt, &newAmt);
        //     animating = TRUE;
        //     resizeAlpha = 255 - (Int32)(newAmt*255);
        // }
        // else {
        //     DisposeResizeBuffer();
        // }
    }

    CRect* dirty = mDirty;
    if (mSurfaceHolder != NULL) {
        // The app owns the surface, we won't draw.
        //
        dirty->SetEmpty();
        // if (animating) {
        //     if (mScroller != NULL) {
        //         mScroller->AbortAnimation();
        //     }
        //     DisposeResizeBuffer();
        // }
        return;
    }

    if (fullRedrawNeeded) {
        mAttachInfo->mIgnoreDirtyState = TRUE;
        dirty->Set(0, 0, (Int32)(mWidth * appScale + 0.5f), (Int32)(mHeight * appScale + 0.5f));
    }

    //if (DEBUG_ORIENTATION || DEBUG_DRAW) {
    //    Logger::V(TAG, "Draw " + mView + "/"
    //        + mWindowAttributes.getTitle()
    //        + ": dirty={" + dirty->mLeft + "," + dirty->top
    //        + "," + dirty->right + "," + dirty->bottom + "} surface="
    //        + surface + " surface.isValid()=" + surface.isValid() + ", appScale:" +
    //        appScale + ", width=" + mWidth + ", height=" + mHeight);
    //}

    mAttachInfo->mTreeObserver->DispatchOnDraw();

    Boolean isEmpty;
    if (!(dirty->IsEmpty(&isEmpty), isEmpty) || mIsAnimating) {
        /*if (mAttachInfo->mHardwareRenderer != NULL
            && mAttachInfo->mHardwareRenderer->IsEnabled()) {
            // Draw with hardware renderer.
            mIsAnimating = FALSE;
            mHardwareYOffset = yoff;
            mResizeAlpha = resizeAlpha;

            mCurrentDirty->SetEx(dirty);
            mCurrentDirty->Union(mPreviousDirty);
            mPreviousDirty->SetEx(dirty);
            dirty->SetEmpty();

            if (mAttachInfo->mHardwareRenderer->Draw(mView, mAttachInfo, this,
                    animating ? NULL : mCurrentDirty)) {
                mPreviousDirty->Set(0, 0, mWidth, mHeight);
            }
        }
        else */if (!DrawSoftware(surface, mAttachInfo, yoff, scalingRequired, dirty)) {
            return;
        }
    }

    // if (animating) {
    //     mFullRedrawNeeded = TRUE;
    //     ScheduleTraversals();
    // }
}

/**
 * @return TRUE if drawing was succesfull, FALSE if an error occurred
 */
Boolean ViewRootImpl::DrawSoftware(
    /* [in] */ ISurface* surface,
    /* [in] */ View::AttachInfo* attachInfo,
    /* [in] */ Int32 yoff,
    /* [in] */ Boolean scalingRequired,
    /* [in] */ CRect* dirty)
{
    // If we get here with a disabled & requested hardware renderer, something went
    // wrong (an invalidate posted right before we destroyed the hardware surface
    // for instance) so we should just bail out. Locking the surface with software
    // rendering at this point would lock it forever and prevent hardware renderer
    // from doing its job when it comes back.
    // if (attachInfo->mHardwareRenderer != NULL && !attachInfo->mHardwareRenderer->IsEnabled() &&
    //     attachInfo->mHardwareRenderer->IsRequested())
    // {
    //     mFullRedrawNeeded = TRUE;
    //     ScheduleTraversals();
    //     return FALSE;
    // }
    AutoPtr<ICanvas> canvas;

    Int32 left = dirty->mLeft;
    Int32 top = dirty->mTop;
    Int32 right = dirty->mRight;
    Int32 bottom = dirty->mBottom;

    ECode ec;
    do {
        ec = surface->LockCanvas(dirty, (ICanvas**)&canvas);
        if (FAILED(ec)) {
            break;
        }

        if (left != dirty->mLeft || top != dirty->mTop || right != dirty->mRight ||
            bottom != dirty->mBottom) {
            mAttachInfo->mIgnoreDirtyState = TRUE;
        }

        // TODO: Do this in native
        ec = canvas->SetDensity(mDensity);
    } while(0);

    if (ec == E_OUT_OF_RESOURCES_EXCEPTION) {
        Logger::E(TAG, "OutOfResourcesException locking surface");

        Boolean outOfMemory;
        mWindowSession->OutOfMemory(mWindow, &outOfMemory);
        if (!outOfMemory) {
            Slogger::W(TAG, "No processes killed for memory; killing self");
            Process::KillProcess(Process::MyPid());
        }

        mLayoutRequested = TRUE;    // ask wm for a new surface next time.
        return FALSE;
    }
    else if (ec == E_ILLEGAL_ARGUMENT_EXCEPTION) {
        Logger::E(TAG, "Could not lock surface");
        // Don't assume this is due to out of memory, it could be
        // something else, and if it is something else then we could
        // kill stuff (or ourself) for no reason.
        mLayoutRequested = TRUE;    // ask wm for a new surface next time.
        return FALSE;
    }

    if (DEBUG_ORIENTATION || DEBUG_DRAW) {
        Int32 w, h;
        canvas->GetWidth(&w);
        canvas->GetHeight(&h);
        Logger::V(TAG, "Surface 0x%08x drawing to bitmap w=%d, h=%d",
            surface, w, h);
        //canvas.drawARGB(255, 255, 0, 0);
    }

    // If this bitmap's format includes an alpha channel, we
    // need to clear it before drawing so that the child will
    // properly re-composite its drawing on a transparent
    // background. This automatically respects the clip/dirty region
    // or
    // If we are applying an offset, we need to clear the area
    // where the offset doesn't appear to avoid having garbage
    // left in the blank areas.
    Boolean isOpaque = FALSE;
    canvas->IsOpaque(&isOpaque);
    if (!isOpaque || yoff != 0) {
        if (FAILED(canvas->DrawColorEx(0, PorterDuffMode_CLEAR))) {
            goto _DrawSoftware_;
        }
    }

    dirty->SetEmpty();
    mIsAnimating = FALSE;
    mAttachInfo->mDrawingTime = SystemClock::GetUptimeMillis();
    VIEW_PROBE(mView)->mPrivateFlags |= View::PFLAG_DRAWN;

    //if (DEBUG_DRAW) {
    //    AutoPtr<IContext> cxt;
    //    mView->GetContext((IContext**)&cxt);

    //    Logger::I(TAG, "Drawing: package:" + cxt.getPackageName() +
    //        ", metrics=" + cxt.getResources().getDisplayMetrics() +
    //        ", compatibilityInfo=" + cxt.getResources().getCompatibilityInfo());
    //}

    canvas->Translate(0, (Float)-yoff);
    if (mTranslator != NULL) {
        mTranslator->TranslateCanvas(canvas);
    }

    canvas->SetScreenDensity(scalingRequired ? mNoncompatDensity : 0);
    mAttachInfo->mIgnoreDirtyState = FALSE;

    mView->Draw(canvas);

    DrawAccessibilityFocusedDrawableIfNeeded(canvas);

    if (!attachInfo->mSetIgnoreDirtyState) {
        // Only clear the flag if it was not set during the mView.draw() call
        attachInfo->mIgnoreDirtyState = FALSE;
    }

_DrawSoftware_:
    if (FAILED(surface->UnlockCanvasAndPost(canvas))) {
        Logger::E(TAG, "Could not unlock surface");
        mLayoutRequested = TRUE;    // ask wm for a new surface next time.
        //noinspection ReturnInsideFinallyBlock
        return FALSE;
    }

    if (LOCAL_LOGV) {
        Logger::V(TAG, "Surface 0x%08x unlockCanvasAndPost", surface);
    }

    return TRUE;
}

void ViewRootImpl::DrawAccessibilityFocusedDrawableIfNeeded(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IAccessibilityManager> manager;
    CAccessibilityManager::GetInstance(VIEW_PROBE(mView)->mContext, (IAccessibilityManager**)&manager);
    Boolean isEnabled, isTouchExplorationEnabled;
    if (!(manager->IsEnabled(&isEnabled), isEnabled)
        || !(manager->IsTouchExplorationEnabled(&isTouchExplorationEnabled),
            isTouchExplorationEnabled)) {
        return;
    }

    if (mAccessibilityFocusedHost == NULL ||
        VIEW_PROBE(mAccessibilityFocusedHost)->mAttachInfo == NULL) {
        return;
    }

    AutoPtr<IDrawable> drawable = GetAccessibilityFocusedDrawable();
    if (drawable == NULL) {
        return;
    }

    AutoPtr<IAccessibilityNodeProvider> provider;
    mAccessibilityFocusedHost->GetAccessibilityNodeProvider(
        (IAccessibilityNodeProvider**)&provider);
    AutoPtr<IRect> bounds =
        VIEW_PROBE(mView)->mAttachInfo->mTmpInvalRect;
    if (provider == NULL) {
        VIEWGROUP_PROBE(mAccessibilityFocusedHost)->GetBoundsOnScreen(bounds);
    }
    else {
        if (mAccessibilityFocusedVirtualView == NULL) {
            return;
        }
        mAccessibilityFocusedVirtualView->GetBoundsInScreen(bounds);
    }
    bounds->Offset(-mAttachInfo->mWindowLeft, -mAttachInfo->mWindowTop);
    Boolean res;
    bounds->Intersect(0, 0, mAttachInfo->mViewRootImpl->mWidth,
        mAttachInfo->mViewRootImpl->mHeight, &res);
    drawable->SetBoundsEx(bounds);
    drawable->Draw(canvas);
}

AutoPtr<IDrawable> ViewRootImpl::GetAccessibilityFocusedDrawable()
{
    if (mAttachInfo != NULL) {
        // Lazily load the accessibility focus drawable.
        if (mAttachInfo->mAccessibilityFocusDrawable == NULL) {
            AutoPtr<ITypedValue> value;
            CTypedValue::New((ITypedValue**)&value);
            AutoPtr<IContext> context;
            mView->GetContext((IContext**)&context);
            AutoPtr<IResourcesTheme> theme;
            context->GetTheme((IResourcesTheme**)&theme);
            Boolean resolved;
            theme->ResolveAttribute(
                R::attr::accessibilityFocusedDrawable, value, TRUE, &resolved);
            if (resolved) {
                AutoPtr<IResources> res;
                context->GetResources((IResources**)&res);
                Int32 resourceId;
                value->GetResourceId(&resourceId);
                res->GetDrawable(resourceId,
                    (IDrawable**)&mAttachInfo->mAccessibilityFocusDrawable);
            }
        }
        return mAttachInfo->mAccessibilityFocusDrawable;
    }
    return NULL;
}

void ViewRootImpl::InvalidateDisplayLists()
{
    List<AutoPtr<IDisplayList> >::Iterator iter = mDisplayLists.Begin();
    for (; iter != mDisplayLists.Begin(); ++iter) {
        (*iter)->Invalidate();
        (*iter)->Clear();
    }

    mDisplayLists.Clear();
}

Boolean ViewRootImpl::ScrollToRectOrFocus(
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    View::AttachInfo* attachInfo = mAttachInfo;
    CRect* ci = attachInfo->mContentInsets;
    CRect* vi = attachInfo->mVisibleInsets;
    Int32 scrollY = 0;
    Boolean handled = FALSE;

    if (vi->mLeft > ci->mLeft || vi->mTop > ci->mTop
        || vi->mRight > ci->mRight || vi->mBottom > ci->mBottom) {
        // We'll assume that we aren't going to change the scroll
        // offset, since we want to avoid that unless it is actually
        // going to make the focus visible...  otherwise we scroll
        // all over the place.
        //
        scrollY = mScrollY;

        // We can be called for two different situations: during a draw,
        // to update the scroll position if the focus has changed (in which
        // case 'rectangle' is NULL), or in response to a
        // requestChildRectangleOnScreen() call (in which case 'rectangle'
        // is non-NULL and we just want to scroll to whatever that
        // rectangle is).
        //
        View* focus = mRealFocusedView != NULL ? VIEW_PROBE(mRealFocusedView) : NULL;

        // When in touch mode, focus points to the previously focused view,
        // which may have been removed from the view hierarchy. The following
        // line checks whether the view is still in our hierarchy.
        //
        if (focus == NULL || focus->mAttachInfo != mAttachInfo) {
            mRealFocusedView = NULL;
            return FALSE;
        }

        if ((IView*)focus->Probe(EIID_IView) != mLastScrolledFocus.Get()) {
            // If the focus has changed, then ignore any requests to scroll
            // to a rectangle; first we want to make sure the entire focus
            // view is visible.
            //
            rectangle = NULL;
        }

        //if (DEBUG_INPUT_RESIZE) Logger::V(TAG, "Eval scroll: focus=" + focus
        //    + " rectangle=" + rectangle + " ci=" + ci
        //    + " vi=" + vi);

        if ((IView*)focus->Probe(EIID_IView) == mLastScrolledFocus.Get()
            && !mScrollMayChange && rectangle == NULL) {
                //// Optimization: if the focus hasn't changed since last
                //// time, and no layout has happened, then just leave things
                //// as they are.
                //if (DEBUG_INPUT_RESIZE) Logger::V(TAG, "Keeping scroll y="
                //    + mScrollY + " vi=" + vi->toShortString());
        }
        else if (focus != NULL) {
            // We need to determine if the currently focused view is
            // within the visible part of the window and, if not, apply
            // a pan so it can be seen.
            //
            mLastScrolledFocus = (IView*)focus->Probe(EIID_IView);
            mScrollMayChange = FALSE;

            if (DEBUG_INPUT_RESIZE) {
                Logger::D(TAG, "Need to scroll?");
            }

            // Try to find the rectangle from the focus view.
            //
            if (focus->GetGlobalVisibleRect(mVisRect, NULL)) {
                //if (DEBUG_INPUT_RESIZE) Logger::V(TAG, "Root w="
                //    + mView.getWidth() + " h=" + mView.getHeight()
                //    + " ci=" + ci->toShortString()
                //    + " vi=" + vi->toShortString());

                if (rectangle == NULL) {
                    focus->GetFocusedRect(mTempRect);

                    //if (DEBUG_INPUT_RESIZE) Logger::V(TAG, "Focus " + focus
                    //    + ": focusRect=" + mTempRect.toShortString());

                    IViewGroup* viewGroup =
                        (IViewGroup*)mView->Probe(EIID_IViewGroup);
                    if (viewGroup) {
                        viewGroup->OffsetDescendantRectToMyCoords(
                            (IView*)focus->Probe(EIID_IView), (IRect*)mTempRect);
                    }

                    //if (DEBUG_INPUT_RESIZE) Logger::V(TAG,
                    //    "Focus in window: focusRect="
                    //    + mTempRect.toShortString()
                    //    + " visRect=" + mVisRect.toShortString());
                }
                else {
                    mTempRect->SetEx(rectangle);
                    //if (DEBUG_INPUT_RESIZE) Logger::V(TAG,
                    //    "Request scroll to rect: "
                    //    + mTempRect.toShortString()
                    //    + " visRect=" + mVisRect.toShortString());
                }

                Boolean isIntersect;
                mTempRect->IntersectEx(mVisRect, &isIntersect);
                if (isIntersect) {
                    //if (DEBUG_INPUT_RESIZE) Logger::V(TAG,
                    //    "Focus window visible rect: "
                    //    + mTempRect.toShortString());

                    Int32 height, tempHeight;
                    mView->GetHeight(&height);
                    mTempRect->GetHeight(&tempHeight);
                    if (tempHeight > (height - vi->mTop - vi->mBottom)) {
                        // If the focus simply is not going to fit, then
                        // best is probably just to leave things as-is.
                        //
                        if (DEBUG_INPUT_RESIZE) {
                            Logger::D(TAG, "Too tall; leaving scrollY=%d", scrollY);
                        }
                    }
                    else if ((mTempRect->mTop-scrollY) < vi->mTop) {
                        scrollY -= vi->mTop - (mTempRect->mTop-scrollY);
                        if (DEBUG_INPUT_RESIZE) {
                            Logger::D(TAG, "Top covered; scrollY=%d", scrollY);
                        }
                    }
                    else if ((mTempRect->mBottom - scrollY) >
                        (height - vi->mBottom)) {
                        scrollY += (mTempRect->mBottom - scrollY)
                            - (height - vi->mBottom);

                        if (DEBUG_INPUT_RESIZE) {
                            Logger::D(TAG, "Bottom covered; scrollY=%d", scrollY);
                        }
                    }
                    handled = TRUE;
                }
            }
        }
    }

    if (scrollY != mScrollY) {
        if (DEBUG_INPUT_RESIZE) {
            Logger::D(TAG, "Pan scroll changed: old=%d, new=%d", mScrollY, scrollY);
        }
        if (!immediate && mResizeBuffer == NULL) {
            if (mScroller == NULL) {
                AutoPtr<IContext> ctx;
                mView->GetContext((IContext**)&ctx);
                CScroller::NewByFriend(ctx, (CScroller**)&mScroller);
            }
            mScroller->StartScroll(0, mScrollY, 0, scrollY - mScrollY);
        }
        else if (mScroller != NULL) {
            mScroller->AbortAnimation();
        }
        mScrollY = scrollY;
    }

    return handled;
}

AutoPtr<IView> ViewRootImpl::GetAccessibilityFocusedHost()
{
    return mAccessibilityFocusedHost;
}

AutoPtr<IAccessibilityNodeInfo> ViewRootImpl::GetAccessibilityFocusedVirtualView()
{
    return mAccessibilityFocusedVirtualView;
}

void ViewRootImpl::SetAccessibilityFocus(
    /* [in] */ IView* view,
    /* [in] */ IAccessibilityNodeInfo* node)
{
    // If we have a virtual view with accessibility focus we need
    // to clear the focus and Invalidate the virtual view bounds.
    if (mAccessibilityFocusedVirtualView != NULL) {

        IAccessibilityNodeInfo* focusNode = mAccessibilityFocusedVirtualView;
        View* focusHost = VIEW_PROBE(mAccessibilityFocusedHost);
        focusHost->ClearAccessibilityFocusNoCallbacks();

        // Wipe the state of the current accessibility focus since
        // the call into the provider to clear accessibility focus
        // will fire an accessibility event which will end up calling
        // this method and we want to have clean state when this
        // invocation happens.
        mAccessibilityFocusedHost = NULL;
        mAccessibilityFocusedVirtualView = NULL;

        AutoPtr<IAccessibilityNodeProvider> provider = focusHost->GetAccessibilityNodeProvider();
        if (provider != NULL) {
            // Invalidate the area of the cleared accessibility focus.
            focusNode->GetBoundsInParent(mTempRect);
            focusHost->Invalidate(mTempRect);
            // Clear accessibility focus in the virtual node.
            Int64 sourceNodeId;
            focusNode->GetSourceNodeId(&sourceNodeId);
            Int32 virtualNodeId = CAccessibilityNodeInfo::GetVirtualDescendantId(sourceNodeId);
            Boolean result;
            provider->PerformAction(virtualNodeId,
                IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS, NULL, &result);
        }
        focusNode->Recycle();
    }
    if (mAccessibilityFocusedHost != NULL) {
        // Clear accessibility focus in the view.
        VIEW_PROBE(mAccessibilityFocusedHost)->ClearAccessibilityFocusNoCallbacks();
    }

    // Set the new focus host and node.
    mAccessibilityFocusedHost = view;
    mAccessibilityFocusedVirtualView = node;
}

ECode ViewRootImpl::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    FAIL_RETURN(CheckThread());

    if (DEBUG_INPUT_RESIZE) {
        Logger::V(TAG, "Request child focus: focus now 0x%08x", focused);
    }

    mAttachInfo->mTreeObserver->DispatchOnGlobalFocusChange(mOldFocusedView, focused);
    ScheduleTraversals();

    mFocusedView = mRealFocusedView = focused;

    return NOERROR;
}

ECode ViewRootImpl::ClearChildFocus(
    /* [in] */ IView* child)
{
    FAIL_RETURN(CheckThread());

    if (DEBUG_INPUT_RESIZE) {
        Logger::D(TAG, "Clearing child focus");
    }

    mOldFocusedView = mFocusedView;

    // Invoke the listener only if there is no view to take focus
    AutoPtr<IView> focus;
    FocusSearch(NULL, IView::FOCUS_FORWARD, (IView**)&focus);
    if (focus == NULL) {
        mAttachInfo->mTreeObserver->DispatchOnGlobalFocusChange(mOldFocusedView, NULL);
    }

    mFocusedView = mRealFocusedView = NULL;

    return NOERROR;
}

//@Override
ECode ViewRootImpl::GetParentForAccessibilityEx(
    /* [out] */ IViewParent** parent)
{
    VALIDATE_NOT_NULL(parent);
    *parent = NULL;

    return NOERROR;
}

ECode ViewRootImpl::FocusableViewAvailable(
    /* [in] */ IView* v)
{
    assert(v);
    FAIL_RETURN(CheckThread());

    if (mView != NULL) {
        Boolean hasFocus;
        mView->HasFocus(&hasFocus);
        if (!hasFocus) {
            Boolean result;
            v->RequestFocus(&result);
        }
        else {
            // the one case where will transfer focus away from the current one
            // is if the current view is a view group that prefers to give focus
            // to its children first AND the view is a descendant of it.
            //
            mFocusedView = NULL;
            mView->FindFocus((IView**)&mFocusedView);

            Boolean descendantsHaveDibsOnFocus = FALSE;

            IViewGroup* viewGroup = mFocusedView != NULL ?
                    IViewGroup::Probe(mFocusedView) : NULL;
            if (viewGroup) {
                Int32 focusability;
                viewGroup->GetDescendantFocusability(&focusability);
                descendantsHaveDibsOnFocus =
                    focusability == ViewGroup::FOCUS_AFTER_DESCENDANTS;
            }

            if (descendantsHaveDibsOnFocus && IsViewDescendantOf(v, mFocusedView)) {
                // If a view gets the focus, the listener will be invoked from requestChildFocus()
                Boolean result;
                v->RequestFocus(&result);
            }
        }
    }

    return NOERROR;
}

ECode ViewRootImpl::RecomputeViewAttributes(
    /* [in] */ IView* child)
{
    FAIL_RETURN(CheckThread());

    if (mView.Get() == child) {
        mAttachInfo->mRecomputeGlobalAttributes = TRUE;
        if (!mWillDrawSoon) {
            ScheduleTraversals();
        }
    }

    return NOERROR;
}

void ViewRootImpl::DispatchDetachedFromWindow()
{
    if (mView != NULL && VIEW_PROBE(mView)->mAttachInfo != NULL) {
        // if (mAttachInfo->mHardwareRenderer != NULL &&
        //     mAttachInfo->mHardwareRenderer->IsEnabled()) {
        //     mAttachInfo->mHardwareRenderer->Validate();
        // }
        VIEW_PROBE(mView)->DispatchDetachedFromWindow();
    }

    mAccessibilityInteractionConnectionManager->EnsureNoConnection();
    Boolean result;
    mAccessibilityManager->RemoveAccessibilityStateChangeListener(
        mAccessibilityInteractionConnectionManager, &result);
    RemoveSendWindowContentChangedCallback();

    DestroyHardwareRenderer();

    SetAccessibilityFocus(NULL, NULL);

    mView = NULL;
    mAttachInfo->mRootView = NULL;
    mAttachInfo->mSurface = NULL;

    mSurface->ReleaseSurface();

    if (mInputQueueCallback != NULL && mInputQueue != NULL) {
        mInputQueueCallback->OnInputQueueDestroyed(mInputQueue);
        mInputQueueCallback = NULL;
        mInputQueue = NULL;
    }
    else if (mInputEventReceiver != NULL) {
        mInputEventReceiver->Dispose();
        mInputEventReceiver = NULL;
    }

    mWindowSession->Remove(mWindow);

    // Dispose the input channel after removing the window so the Window Manager
    // doesn't interpret the input channel being closed as an abnormal termination.
    if (mInputChannel != NULL) {
        mInputChannel->Dispose();
        mInputChannel = NULL;
    }

    UnscheduleTraversals();
}

void ViewRootImpl::UpdateConfiguration(
    /* [in] */ IConfiguration* inConfig,
    /* [in] */ Boolean force)
{
    //if (DEBUG_CONFIGURATION) Logger::V(TAG,
    //    "Applying new config to window "
    //    + mWindowAttributes.getTitle()
    //    + ": " + config);

    AutoPtr<IConfiguration> config;
    AutoPtr<ICompatibilityInfo> ci;
    mCompatibilityInfo->GetIfNeeded((ICompatibilityInfo**)&ci);
    if (ci != NULL) {
        CConfiguration::New(inConfig, (IConfiguration**)&config);
        ci->ApplyToConfiguration(mNoncompatDensity, config);
    }
    else {
        config = inConfig;
    }

    {
        Mutex::Autolock lock(sConfigCallbacksLock);

        List<AutoPtr<IComponentCallbacks> >::Iterator iter;
        for (iter = sConfigCallbacks.Begin(); iter != sConfigCallbacks.End(); ++iter) {
            iter->Get()->OnConfigurationChanged(config);
        }
    }

    if (mView != NULL) {
        // At this point the resources have been updated to
        // have the most recent config, whatever that is.  Use
        // the on in them which may be newer.
        //
        AutoPtr<IResources> resources;
        mView->GetResources((IResources**)&resources);
        assert(resources);

        config = NULL;
        resources->GetConfiguration((IConfiguration**)&config);
        assert(config.Get() != NULL);

        Int32 result = 0;
        mLastConfiguration->Diff((CConfiguration*)config.Get(), &result);

        if (force || (result != 0)) {
            Int32 lastLayoutDirection;
            mLastConfiguration->GetLayoutDirection(&lastLayoutDirection);
            Int32 currentLayoutDirection;
            config->GetLayoutDirection(&currentLayoutDirection);
            mLastConfiguration->SetTo(config);
            if (lastLayoutDirection != currentLayoutDirection &&
                mViewLayoutDirectionInitial == IView::LAYOUT_DIRECTION_INHERIT) {
                mView->SetLayoutDirection(currentLayoutDirection);
            }
            mView->DispatchConfigurationChanged(config);
        }
    }
}

/**
 * Return TRUE if child is an ancestor of parent, (or equal to the parent).
 */
Boolean ViewRootImpl::IsViewDescendantOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    assert(child != NULL && parent != NULL);
    if (child == parent) {
        return TRUE;
    }

    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);

    return IViewGroup::Probe(theParent) && IsViewDescendantOf(IView::Probe(theParent), parent);
}

void ViewRootImpl::ForceLayout(
    /* [in] */ IView* view)
{
    assert(view != NULL);
    view->ForceLayout();
    IViewGroup* group = IViewGroup::Probe(view);
    if (group) {
        Int32 count = 0;
        group->GetChildCount(&count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child;
            group->GetChildAt(i, (IView**)&child);
            ForceLayout(child);
        }
    }
}

/**
* Something in the current window tells us we need to change the touch mode.  For
* example, we are not in touch mode, and the user touches the screen.
*
* If the touch mode has changed, tell the window manager, and handle it locally.
*
* @param inTouchMode Whether we want to be in touch mode.
* @return True if the touch mode changed and focus changed was changed as a result
*/
Boolean ViewRootImpl::EnsureTouchMode(
    /* [in] */ Boolean inTouchMode)
{
    if (DBG) {
        Logger::D("touchmode", "ensureTouchMode(%d), current touch mode is%d",
            inTouchMode, mAttachInfo->mInTouchMode);
    }

    if (mAttachInfo->mInTouchMode == inTouchMode) {
        return FALSE;
    }

    // tell the window manager
    //
    mWindowSession->SetInTouchMode(inTouchMode);
    //try {
    //    mWindowSession.setInTouchMode(inTouchMode);
    //} catch (RemoteException e) {
    //    throw new RuntimeException(e);
    //}

    // handle the change
    return EnsureTouchModeLocally(inTouchMode);
}

/**
* Ensure that the touch mode for this window is set, and if it is changing,
* take the appropriate action.
* @param inTouchMode Whether we want to be in touch mode.
* @return True if the touch mode changed and focus changed was changed as a result
*/
Boolean ViewRootImpl::EnsureTouchModeLocally(
    /* [in] */ Boolean inTouchMode)
{
    if (DBG) {
        Logger::D("touchmode", "ensureTouchModeLocally(%d), current touch mode is %d",
            inTouchMode, mAttachInfo->mInTouchMode);
    }

    if (mAttachInfo->mInTouchMode == inTouchMode) {
        return FALSE;
    }

    mAttachInfo->mInTouchMode = inTouchMode;
    mAttachInfo->mTreeObserver->DispatchOnTouchModeChanged(inTouchMode);

    return (inTouchMode) ? EnterTouchMode() : LeaveTouchMode();
}

Boolean ViewRootImpl::EnterTouchMode()
{
    if (mView != NULL) {
        Boolean hasFocus = FALSE;
        mView->HasFocus(&hasFocus);
        if (hasFocus) {
            // note: not relying on mFocusedView here because this could
            // be when the window is first being added, and mFocused isn't
            // set yet.
            //
            AutoPtr<IView> focused;
            mView->FindFocus((IView**)&focused);
            if (focused != NULL) {
                Boolean isFocusable;
                focused->IsFocusableInTouchMode(&isFocusable);
                if (!isFocusable) {
                    AutoPtr<IViewGroup> ancestorToTakeFocus =
                        FindAncestorToTakeFocusInTouchMode(focused.Get());

                    if (ancestorToTakeFocus != NULL) {
                        // there is an ancestor that wants focus after its descendants that
                        // is focusable in touch mode.. give it focus
                        //
                        Boolean result;
                        ancestorToTakeFocus->RequestFocus(&result);

                        return result;
                    }
                    else {
                        // nothing appropriate to have focus in touch mode, clear it out
                        //
                        VIEW_PROBE(mView)->UnFocus();
                        mAttachInfo->mTreeObserver->DispatchOnGlobalFocusChange(focused, NULL);
                        mFocusedView = NULL;
                        mOldFocusedView = NULL;

                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

/**
* Find an ancestor of focused that wants focus after its descendants and is
* focusable in touch mode.
* @param focused The currently focused view.
* @return An appropriate view, or NULL if no such view exists.
*/
AutoPtr<IViewGroup> ViewRootImpl::FindAncestorToTakeFocusInTouchMode(
    /* [in] */ IView* focused)
{
    AutoPtr<IViewParent> parent;
    focused->GetParent((IViewParent**)&parent);

    AutoPtr<IViewGroup> vgParent = IViewGroup::Probe(parent);
    while (vgParent) {
        Int32 focusability;
        vgParent->GetDescendantFocusability(&focusability);
        Boolean isFocusable;
        vgParent->IsFocusableInTouchMode(&isFocusable);
        if (focusability == ViewGroup::FOCUS_AFTER_DESCENDANTS
            && isFocusable) {
            return vgParent;
        }

        Boolean isRootNamespace;
        vgParent->IsRootNamespace(&isRootNamespace);
        if (isRootNamespace) {
            return NULL;
        }
        else {
            parent = NULL;
            vgParent->GetParent((IViewParent**)&parent);
            vgParent = IViewGroup::Probe(parent);
        }
    }

    return NULL;
}

Boolean ViewRootImpl::LeaveTouchMode()
{
    if (mView != NULL) {
        Boolean hasFocus = FALSE;
        mView->HasFocus(&hasFocus);
        if (hasFocus) {
            // i learned the hard way to not trust mFocusedView :)
            //
            AutoPtr<IView> temp;
            mView->FindFocus((IView**)&temp);
            mFocusedView = temp;
            IViewGroup* vg = IViewGroup::Probe(mFocusedView);
            if (vg == NULL) {
                // some view has focus, let it keep it
                return FALSE;
            }
            else {
                Int32 focusability;
                vg->GetDescendantFocusability(&focusability);
                if (focusability != ViewGroup::FOCUS_AFTER_DESCENDANTS) {
                    // some view group has focus, and doesn't prefer its children
                    // over itself for focus, so let them keep it.
                    return FALSE;
                }
            }
        }

        // find the best view to give focus to in this brave new non-touch-mode
        // world
        //
        AutoPtr<IView> focused;
        FocusSearch(NULL, IView::FOCUS_DOWN, (IView**)&focused);
        if (focused != NULL) {
            Boolean result;
            focused->RequestFocusEx(IView::FOCUS_DOWN, &result);
            return result;
        }
    }

    return FALSE;
}

void ViewRootImpl::DeliverInputEvent(
    /* [in] */ QueuedInputEvent* q)
{
    //Trace::TraceBegin(Trace::TRACE_TAG_VIEW, "deliverInputEvent");

    if (IKeyEvent::Probe(q->mEvent)) {
        DeliverKeyEvent(q);
    }
    else {
        Int32 source;
        q->mEvent->GetSource(&source);
        if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
            DeliverPointerEvent(q);
        }
        else if ((source & IInputDevice::SOURCE_CLASS_TRACKBALL) != 0) {
            DeliverTrackballEvent(q);
        }
        else {
            DeliverGenericMotionEvent(q);
        }
    }

    //Trace::TraceEnd(Trace::TRACE_TAG_VIEW);
}

void ViewRootImpl::DeliverPointerEvent(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IMotionEvent> event = IMotionEvent::Probe(q->mEvent);
    SurfaceView::AdjustSurfaceViewMotion(event);

    if (DBG) {
        Logger::D("DeliverPointerEvent", "====================A MotionEvent===================");
        Int64 downTime, eventTime;
        Float x, y, pressure, size, orientation;
        event->GetDownTime(&downTime);
        event->GetEventTime(&eventTime);
        event->GetX(&x);
        event->GetY(&y);
        event->GetPressure(&pressure);
        event->GetSize(&size);
        event->GetOrientation(&orientation);
        Logger::D("DeliverPointerEvent", "    event = %p, downtime = %lld, eventtime = %lld\n",
           event.Get(), downTime, eventTime);
        Logger::D("DeliverPointerEvent", "    x = %.2f, y = %.2f, pressure = %.2f, size = %.2f, orientation = %.2f\n",
           x, y, pressure, size, orientation);
        // LogView(String("MotionEvent on"));
        Logger::D("DeliverPointerEvent", "====================A MotionEvent===================");
    }

    Boolean isTouchEvent;
    event->IsTouchEvent(&isTouchEvent);

    if (mInputEventConsistencyVerifier != NULL) {
        if (isTouchEvent) {
            mInputEventConsistencyVerifier->OnTouchEvent(event, 0);
        }
        else {
            mInputEventConsistencyVerifier->OnGenericMotionEvent(event, 0);
        }
    }

    // If there is no view, then the event will not be handled.
    if (mView == NULL || !mAdded) {
        FinishInputEvent(q, FALSE);
        return;
    }

    // Translate the pointer event for compatibility, if needed.
    if (mTranslator != NULL) {
        mTranslator->TranslateEventInScreenToAppWindow(event);
    }

    // Enter touch mode on down or scroll.
    Int32 action;
    event->GetAction(&action);
    if (action == IMotionEvent::ACTION_DOWN || action == IMotionEvent::ACTION_SCROLL) {
        EnsureTouchMode(TRUE);
    }

    // Offset the scroll position.
    if (mCurScrollY != 0) {
        event->OffsetLocation(0, mCurScrollY);
    }

    if (MEASURE_LATENCY) {
        //mLt->Sample("A Dispatching PointerEvents", System.nanoTime() - event->getEventTimeNano());
    }

    // Remember the touch position for possible drag-initiation.
    if (isTouchEvent) {
        Float rawX, rawY;
        event->GetRawX(&rawX);
        event->GetRawY(&rawY);
        mLastTouchPoint->Set(rawX, rawY);
    }

    // Dispatch touch to view hierarchy.
    Boolean handled;
    mView->DispatchPointerEvent(event, &handled);
    if (MEASURE_LATENCY) {
        //mLt->Sample("B Dispatched PointerEvents ", System.nanoTime() - event->getEventTimeNano());
    }
    if (handled) {
        FinishInputEvent(q, TRUE);
        return;
    }

    // Pointer event was unhandled.
    FinishInputEvent(q, FALSE);
}

void ViewRootImpl::DeliverTrackballEvent(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IMotionEvent> event = IMotionEvent::Probe(q->mEvent);
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTrackballEvent(event, 0);
    }

    if (mView != NULL && mAdded &&
        (q->mFlags & QueuedInputEvent::FLAG_DELIVER_POST_IME) == 0) {
        if (LOCAL_LOGV)
            Logger::V(TAG, "Dispatching trackball 0x%08x to 0x%08x ", event.Get(), mView.Get());

        // Dispatch to the IME before propagating down the view hierarchy.
        // The IME will eventually call back into handleImeFinishedEvent.
        if (mLastWasImTarget) {
            AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL) {
                Int32 seq;
                event->GetSequenceNumber(&seq);
                if (DEBUG_IMF)
                    Logger::V(TAG, "Sending trackball event to IME: seq=%d, event=0x%08x",
                        seq, event.Get());
                AutoPtr<IContext> context;
                mView->GetContext((IContext**)&context);
                imm->DispatchTrackballEvent(
                    context, seq, event, mInputMethodCallback);
                return;
            }
        }
    }

    // Not dispatching to IME, continue with post IME actions.
    DeliverTrackballEventPostIme(q);
}

void ViewRootImpl::DeliverTrackballEventPostIme(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IMotionEvent> event = IMotionEvent::Probe(q->mEvent);

    // If there is no view, then the event will not be handled.
    if (mView == NULL || !mAdded) {
        FinishInputEvent(q, FALSE);
        return;
    }

    // Deliver the trackball event to the view.
    Boolean handled;
    mView->DispatchTrackballEvent(event, &handled);
    if (handled) {
        // If we reach this, we delivered a trackball event to mView and
        // mView consumed it. Because we will not translate the trackball
        // event into a key event, touch mode will not exit, so we exit
        // touch mode here.
        EnsureTouchMode(FALSE);

        FinishInputEvent(q, TRUE);
        mLastTrackballTime = Elastos::Core::Math::INT32_MIN_VALUE;
        return;
    }

    Int64 curTime = SystemClock::GetUptimeMillis();
    if ((mLastTrackballTime + MAX_TRACKBALL_DELAY) < curTime) {
        // It has been too Int64 since the last movement,
        // so restart at the beginning.
        mTrackballAxisX->Reset(0);
        mTrackballAxisY->Reset(0);
        mLastTrackballTime = curTime;
    }

    Int32 action;
    event->GetAction(&action);

    Int32 metastate;
    event->GetMetaState(&metastate);

    switch (action) {
    case IMotionEvent::ACTION_DOWN:
        {
            mTrackballAxisX->Reset(2);
            mTrackballAxisY->Reset(2);
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                IKeyEvent::ACTION_DOWN, IKeyEvent::KEYCODE_DPAD_CENTER,
                0, metastate, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
                IKeyEvent::FLAG_FALLBACK, IInputDevice::SOURCE_KEYBOARD,
                (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
        }
        break;
    case IMotionEvent::ACTION_UP:
        {
            mTrackballAxisX->Reset(2);
            mTrackballAxisY->Reset(2);
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                IKeyEvent::ACTION_UP, IKeyEvent::KEYCODE_DPAD_CENTER,
                0, metastate, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
                IKeyEvent::FLAG_FALLBACK, IInputDevice::SOURCE_KEYBOARD,
                (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
        }
        break;
    }

    if (DEBUG_TRACKBALL) {
        //Logger::D(
        //    TAG, StringBuffer("TB X=") + mTrackballAxisX->mPosition + " step="
        //    + mTrackballAxisX->mStep + " dir=" + mTrackballAxisX->mDir
        //    + " acc=" + mTrackballAxisX->mAcceleration
        //    + " move=" + x + " / Y=" + mTrackballAxisY->mPosition + " step="
        //    + mTrackballAxisY->mStep + " dir=" + mTrackballAxisY->mDir
        //    + " acc=" + mTrackballAxisY->mAcceleration
        //    + " move=" + y);
    }

    Float x, y;
    event->GetX(&x);
    event->GetY(&y);

    Int64 eventTime;
    event->GetEventTime(&eventTime);

    Float xOff = mTrackballAxisX->Collect(x, eventTime, "X");
    Float yOff = mTrackballAxisY->Collect(y, eventTime, "Y");

    // Generate DPAD events based on the trackball movement.
    // We pick the axis that has moved the most as the direction of
    // the DPAD.  When we generate DPAD events for one axis, then the
    // other axis is reset -- we don't want to perform DPAD jumps due
    // to slight movements in the trackball when making major movements
    // along the other axis.
    Int32 keycode = 0;
    Int32 movement = 0;
    Float accel = 1;
    if (xOff > yOff) {
        Float xPrecision;
        event->GetXPrecision(&xPrecision);
        movement = mTrackballAxisX->Generate(2/xPrecision);
        if (movement != 0) {
            keycode = movement > 0 ? IKeyEvent::KEYCODE_DPAD_RIGHT
                : IKeyEvent::KEYCODE_DPAD_LEFT;
            accel = mTrackballAxisX->mAcceleration;
            mTrackballAxisY->Reset(2);
        }
    }
    else if (yOff > 0) {
        Float yPrecision;
        event->GetYPrecision(&yPrecision);
        movement = mTrackballAxisY->Generate(2/yPrecision);
        if (movement != 0) {
            keycode = movement > 0 ? IKeyEvent::KEYCODE_DPAD_DOWN
                : IKeyEvent::KEYCODE_DPAD_UP;
            accel = mTrackballAxisY->mAcceleration;
            mTrackballAxisX->Reset(2);
        }
    }

    if (keycode != 0) {
        if (movement < 0)
            movement = -movement;
        Int32 accelMovement = (Int32)(movement * accel);

        if (DEBUG_TRACKBALL) {
            //Logger::D(
                //TAG, StringBuffer("Move: movement=") + movement
                //+ " accelMovement=" + accelMovement
                //+ " accel=" + accel);
        }

        if (accelMovement > movement) {
            if (DEBUG_TRACKBALL)
                Logger::D("foo", "Delivering fake DPAD: %d", keycode);
            movement--;

            AutoPtr<IKeyEvent> keyEvent;
            Int32 repeatCount = accelMovement - movement;
            CKeyEvent::New(
                curTime, curTime,
                IKeyEvent::ACTION_MULTIPLE, keycode, repeatCount,
                metastate, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
                IKeyEvent::FLAG_FALLBACK, IInputDevice::SOURCE_KEYBOARD,
                (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
        }

        while (movement > 0) {
            if (DEBUG_TRACKBALL)
                Logger::D("foo", "Delivering fake DPAD: %d", keycode);

            movement--;
            curTime = SystemClock::GetUptimeMillis();

            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                curTime, curTime,
                IKeyEvent::ACTION_DOWN, keycode, 0,
                metastate, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
                IKeyEvent::FLAG_FALLBACK, IInputDevice::SOURCE_KEYBOARD,
                (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
            keyEvent = NULL;
            CKeyEvent::New(
                curTime, curTime,
                IKeyEvent::ACTION_UP, keycode, 0,
                metastate, IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
                IKeyEvent::FLAG_FALLBACK, IInputDevice::SOURCE_KEYBOARD,
                (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
        }

        mLastTrackballTime = curTime;
    }

    // Unfortunately we can't tell whether the application consumed the keys, so
    // we always consider the trackball event handled.
    FinishInputEvent(q, TRUE);
}

void ViewRootImpl::DeliverGenericMotionEvent(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IMotionEvent> event = IMotionEvent::Probe(q->mEvent);
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnGenericMotionEvent(event, 0);
    }

    if (mView != NULL && mAdded && (q->mFlags
        & QueuedInputEvent::FLAG_DELIVER_POST_IME) == 0) {
        if (LOCAL_LOGV)
            Logger::V(TAG, "Dispatching generic motion 0x%08x to 0x%08x",
                event.Get(), mView.Get());

        // Dispatch to the IME before propagating down the view hierarchy.
        // The IME will eventually call back into handleImeFinishedEvent.
        if (mLastWasImTarget) {
            AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL) {
                Int32 seq;
                event->GetSequenceNumber(&seq);
                if (DEBUG_IMF)
                    Logger::V(TAG, "Sending generic motion event to IME: seq=%d"
                        " event=0x%08x", seq, event.Get());
                AutoPtr<IContext> context;
                mView->GetContext((IContext**)&context);
                imm->DispatchGenericMotionEvent(
                    context, seq, event, mInputMethodCallback);
                return;
            }
        }
    }

    // Not dispatching to IME, continue with post IME actions.
    DeliverGenericMotionEventPostIme(q);
}

void ViewRootImpl::DeliverGenericMotionEventPostIme(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IMotionEvent> event = IMotionEvent::Probe(q->mEvent);
    Int32 source;
    event->GetSource(&source);
    Boolean isJoystick = (source & IInputDevice::SOURCE_CLASS_JOYSTICK) != 0;

    // If there is no view, then the event will not be handled.
    if (mView == NULL || !mAdded) {
        if (isJoystick) {
            UpdateJoystickDirection(event, FALSE);
        }
        FinishInputEvent(q, FALSE);
        return;
    }

    // Deliver the event to the view.
    Boolean handled;
    mView->DispatchGenericMotionEvent(event, &handled);
    if (handled) {
        if (isJoystick) {
            UpdateJoystickDirection(event, FALSE);
        }
        FinishInputEvent(q, TRUE);
        return;
    }

    if (isJoystick) {
        // Translate the joystick event into DPAD keys and try to deliver
        // those.
        UpdateJoystickDirection(event, TRUE);
        FinishInputEvent(q, TRUE);
    }
    else {
        FinishInputEvent(q, FALSE);
    }
}

void ViewRootImpl::UpdateJoystickDirection(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Boolean synthesizeNewKeys)
{
    Int64 time;
    event->GetEventTime(&time);
    Int32 metaState;
    event->GetMetaState(&metaState);
    Int32 deviceId;
    event->GetDeviceId(&deviceId);
    Int32 source;
    event->GetSource(&source);

    Float value;
    event->GetAxisValue(IMotionEvent::AXIS_HAT_X, &value);
    Int32 xDirection = JoystickAxisValueToDirection(value);
    if (xDirection == 0) {
        event->GetX(&value);
        xDirection = JoystickAxisValueToDirection(value);
    }

    event->GetAxisValue(IMotionEvent::AXIS_HAT_Y, &value);
    Int32 yDirection = JoystickAxisValueToDirection(value);
    if (yDirection == 0) {
        event->GetY(&value);
        yDirection = JoystickAxisValueToDirection(value);
    }

    if (xDirection != mLastJoystickXDirection) {
        if (mLastJoystickXKeyCode != 0) {
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                time, time, IKeyEvent::ACTION_UP, mLastJoystickXKeyCode,
                0, metaState, deviceId, 0, IKeyEvent::FLAG_FALLBACK,
                source, (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
            mLastJoystickXKeyCode = 0;
        }

        mLastJoystickXDirection = xDirection;

        if (xDirection != 0 && synthesizeNewKeys) {
            mLastJoystickXKeyCode = xDirection > 0
                    ? IKeyEvent::KEYCODE_DPAD_RIGHT : IKeyEvent::KEYCODE_DPAD_LEFT;
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                time, time, IKeyEvent::ACTION_DOWN, mLastJoystickXKeyCode,
                0, metaState, deviceId, 0, IKeyEvent::FLAG_FALLBACK,
                source, (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
        }
    }

    if (yDirection != mLastJoystickYDirection) {
        if (mLastJoystickYKeyCode != 0) {
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                time, time, IKeyEvent::ACTION_UP, mLastJoystickYKeyCode,
                0, metaState, deviceId, 0, IKeyEvent::FLAG_FALLBACK,
                source, (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);

            mLastJoystickYKeyCode = 0;
        }

        mLastJoystickYDirection = yDirection;

        if (yDirection != 0 && synthesizeNewKeys) {
            mLastJoystickYKeyCode = yDirection > 0
                    ? IKeyEvent::KEYCODE_DPAD_DOWN : IKeyEvent::KEYCODE_DPAD_UP;
            AutoPtr<IKeyEvent> keyEvent;
            CKeyEvent::New(
                time, time, IKeyEvent::ACTION_DOWN, mLastJoystickYKeyCode,
                0, metaState, deviceId, 0, IKeyEvent::FLAG_FALLBACK,
                source, (IKeyEvent**)&keyEvent);
            EnqueueInputEvent(keyEvent);
        }
    }
}

Int32 ViewRootImpl::JoystickAxisValueToDirection(
    /* [in] */ Float value)
{
    if (value >= 0.5f) {
        return 1;
    }
    else if (value <= -0.5f) {
        return -1;
    }
    else {
        return 0;
    }
}

Boolean ViewRootImpl::IsNavigationKey(
    /* [in] */ IKeyEvent* keyEvent)
{
    Int32 keyCode;
    keyEvent->GetKeyCode(&keyCode);
    switch (keyCode) {
    case IKeyEvent::KEYCODE_DPAD_LEFT:
    case IKeyEvent::KEYCODE_DPAD_RIGHT:
    case IKeyEvent::KEYCODE_DPAD_UP:
    case IKeyEvent::KEYCODE_DPAD_DOWN:
    case IKeyEvent::KEYCODE_DPAD_CENTER:
    case IKeyEvent::KEYCODE_PAGE_UP:
    case IKeyEvent::KEYCODE_PAGE_DOWN:
    case IKeyEvent::KEYCODE_MOVE_HOME:
    case IKeyEvent::KEYCODE_MOVE_END:
    case IKeyEvent::KEYCODE_TAB:
    case IKeyEvent::KEYCODE_SPACE:
    case IKeyEvent::KEYCODE_ENTER:
        return TRUE;
    }
    return FALSE;
}

Boolean ViewRootImpl::IsTypingKey(
    /* [in] */ IKeyEvent* keyEvent)
{
    Int32 unicodeChar;
    keyEvent->GetUnicodeChar(&unicodeChar);
    return unicodeChar > 0;
}

Boolean ViewRootImpl::CheckForLeavingTouchModeAndConsume(
    /* [in] */ IKeyEvent* event)
{
    // Only relevant in touch mode.
    if (!mAttachInfo->mInTouchMode) {
        return FALSE;
    }

    // Only consider leaving touch mode on DOWN or MULTIPLE actions, never on UP.
    Int32 action;
    event->GetAction(&action);
    if (action != IKeyEvent::ACTION_DOWN
        && action != IKeyEvent::ACTION_MULTIPLE) {
        return FALSE;
    }

    Int32 flags;
    event->GetFlags(&flags);
    if ((flags&IKeyEvent::FLAG_KEEP_TOUCH_MODE) != 0) {
        return FALSE;
    }

    // If the key can be used for keyboard navigation then leave touch mode
    // and select a focused view if needed (in ensureTouchMode).
    // When a new focused view is selected, we consume the navigation key because
    // navigation doesn't make much sense unless a view already has focus so
    // the key's purpose is to set focus.
    if (IsNavigationKey(event)) {
        return EnsureTouchMode(FALSE);
    }

    // If the key can be used for typing then leave touch mode
    // and select a focused view if needed (in ensureTouchMode).
    // Always allow the view to process the typing key.
    if (IsTypingKey(event)) {
        EnsureTouchMode(FALSE);
        return FALSE;
    }

    return FALSE;
}

void ViewRootImpl::DeliverKeyEvent(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IKeyEvent> event = IKeyEvent::Probe(q->mEvent);
    {
        Slogger::D(TAG, "====================A KeyEvent===================\n");
        Boolean isDown, isSystem, alt, shift, sym, longPress;
        event->IsDown(&isDown);
        event->IsSystem(&isSystem);
        event->IsAltPressed(&alt);
        event->IsShiftPressed(&shift);
        event->IsSymPressed(&sym);
        event->IsLongPress(&longPress);

        if (LOCAL_LOGV)
            Slogger::D(TAG, "   isDown = %d, isSystem = %d, alt = %d, shift = %d, sym = %d, longPress = %d\n",
                       isDown, isSystem, alt, shift, sym, longPress);

        Int32 keyCode, scanCode, deviceID, uChar;
        String chars;
        Int64 downTime, eventTime;
        Char32 label, number;

        event->GetKeyCode(&keyCode);
        event->GetScanCode(&scanCode);
        event->GetKeyboardDevice(&deviceID);
        event->GetCharacters(&chars);
        event->GetDownTime(&downTime);
        event->GetEventTime(&eventTime);
        event->GetDisplayLabel(&label);
        event->GetUnicodeChar(&uChar);
        event->GetNumber(&number);

        if (LOCAL_LOGV)
        {
            Slogger::D(TAG, "   keyCode = %d, scanCode = %d, deviceID = %d, unicodeChar = %d\n",
                        keyCode, scanCode, deviceID, uChar);
            Slogger::D(TAG, "   characters = %s\n", (const char*)chars);
            Slogger::D(TAG, "   downTime = %d, eventTime = %d\n", downTime, eventTime);
            Slogger::D(TAG, "   label = %d, '%c', number = %d\n", label, (char)label, number);
        }
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnKeyEvent(event, 0);
    }

    if (mView != NULL && mAdded && (q->mFlags
        & QueuedInputEvent::FLAG_DELIVER_POST_IME) == 0) {
        if (LOCAL_LOGV)
            Logger::V(TAG, "Dispatching key 0x%08x to 0x%08x", event.Get(), mView.Get());

        // Perform predispatching before the IME.
        Boolean handled = TRUE;
        mView->DispatchKeyEventPreIme(event, &handled);
        if (handled) {
            FinishInputEvent(q, TRUE);
            return;
        }

        // Dispatch to the IME before propagating down the view hierarchy.
        // The IME will eventually call back into handleImeFinishedEvent.
        if (mLastWasImTarget) {
             AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL) {
                Int32 seq;
                event->GetSequenceNumber(&seq);
                if (DEBUG_IMF)
                    Logger::V(TAG, "Sending key event to IME: seq=%d, event=0x%08x",
                        seq, event.Get());
                AutoPtr<IContext> ctx;
                mView->GetContext((IContext**)&ctx);
                imm->DispatchKeyEvent(ctx, seq, event, mInputMethodCallback);
                return;
            }
        }
    }

    // Not dispatching to IME, continue with post IME actions.
    DeliverKeyEventPostIme(q);
}

void ViewRootImpl::HandleImeFinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    QueuedInputEvent* q = mCurrentInputEvent;
    Int32 eventSeq;
    if (q != NULL && (q->mEvent->GetSequenceNumber(&eventSeq), eventSeq) == seq) {
        if (DEBUG_IMF) {
            Logger::V(TAG, "IME finished event: seq=%d handled=%d event=0x%08x",
                seq, handled, q);
        }
        if (handled) {
            FinishInputEvent(q, TRUE);
        }
        else {
            if (IKeyEvent::Probe(q->mEvent)) {
                AutoPtr<IKeyEvent> event = IKeyEvent::Probe(q->mEvent);
                Int32 action;
                event->GetAction(&action);
                if (action != IKeyEvent::ACTION_UP) {
                    // If the window doesn't currently have input focus, then drop
                    // this event->  This could be an event that came back from the
                    // IME dispatch but the window has lost focus in the meantime.
                    if (!mAttachInfo->mHasWindowFocus) {
                        Slogger::W(TAG, "Dropping event due to no window focus: 0x%08x", event.Get());
                        FinishInputEvent(q, TRUE);
                        return;
                    }
                }
                DeliverKeyEventPostIme(q);
            }
            else {
                IMotionEvent* event = IMotionEvent::Probe(q->mEvent);
                Int32 action;
                event->GetAction(&action);
                if (action != IMotionEvent::ACTION_CANCEL
                    && action != IMotionEvent::ACTION_UP) {
                    // If the window doesn't currently have input focus, then drop
                    // this event->  This could be an event that came back from the
                    // IME dispatch but the window has lost focus in the meantime.
                    if (!mAttachInfo->mHasWindowFocus) {
                        Slogger::W(TAG, "Dropping event due to no window focus: 0x%08x", event);
                        FinishInputEvent(q, TRUE);
                        return;
                    }
                }
                Int32 source;
                q->mEvent->GetSource(&source);
                if ((source & IInputDevice::SOURCE_CLASS_TRACKBALL) != 0) {
                    DeliverTrackballEventPostIme(q);
                }
                else {
                    DeliverGenericMotionEventPostIme(q);
                }
            }
        }
    }
    else {
        if (DEBUG_IMF) {
            Logger::V(TAG, "IME finished event: seq=%d, handled=%d,"
                " event not found!", seq, handled);
        }
    }
}

void ViewRootImpl::DeliverKeyEventPostIme(
    /* [in] */ QueuedInputEvent* q)
{
    AutoPtr<IKeyEvent> event = IKeyEvent::Probe(q->mEvent);

    // If the view went away, then the event will not be handled.
    if (mView == NULL || !mAdded) {
        FinishInputEvent(q, FALSE);
        return;
    }

    // If the key's purpose is to exit touch mode then we consume it and consider it handled.
    if (CheckForLeavingTouchModeAndConsume(event)) {
        FinishInputEvent(q, TRUE);
        return;
    }

    // Make sure the fallback event policy sees all keys that will be delivered to the
    // view hierarchy.
    mFallbackEventHandler->PreDispatchKeyEvent(event);

    // Deliver the key to the view hierarchy.
    Boolean handled;
    mView->DispatchKeyEvent(event, &handled);
    if (handled) {
        FinishInputEvent(q, TRUE);
        return;
    }

    // If the Control modifier is held, try to interpret the key as a shortcut.
    Int32 action;
    event->GetAction(&action);
    Boolean isCtrlPressed;
    Int32 repeatCount, keyCode;
    event->GetKeyCode(&keyCode);
    if (action == IKeyEvent::ACTION_DOWN
        && (event->IsCtrlPressed(&isCtrlPressed), isCtrlPressed)
        && (event->GetRepeatCount(&repeatCount), repeatCount) == 0
        && !CKeyEvent::IsModifierKey(keyCode)) {
        if (mView->DispatchKeyShortcutEvent(event, &handled), handled) {
            FinishInputEvent(q, TRUE);
            return;
        }
    }

    // Apply the fallback event policy.
    if (mFallbackEventHandler->DispatchKeyEvent(event, &handled), handled) {
        FinishInputEvent(q, TRUE);
        return;
    }

    // Handle automatic focus changes.
    if (action == IKeyEvent::ACTION_DOWN) {
        Int32 direction = 0;
        Boolean hasNoModifiers, hasModifiers;
        event->HasNoModifiers(&hasNoModifiers);
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_LEFT:
                if (hasNoModifiers) {
                    direction = IView::FOCUS_LEFT;
                }
                break;
            case IKeyEvent::KEYCODE_DPAD_RIGHT:
                if (hasNoModifiers) {
                    direction = IView::FOCUS_RIGHT;
                }
                break;
            case IKeyEvent::KEYCODE_DPAD_UP:
                if (hasNoModifiers) {
                    direction = IView::FOCUS_UP;
                }
                break;
            case IKeyEvent::KEYCODE_DPAD_DOWN:
                if (hasNoModifiers) {
                    direction = IView::FOCUS_DOWN;
                }
                break;
            case IKeyEvent::KEYCODE_TAB:
                if (hasNoModifiers) {
                    direction = IView::FOCUS_FORWARD;
                }
                else if (event->HasModifiers(IKeyEvent::META_SHIFT_ON, &hasModifiers), hasModifiers) {
                    direction = IView::FOCUS_BACKWARD;
                }
                break;
        }

        if (direction != 0) {
            AutoPtr<IView> focused;
            mView->FindFocus((IView**)&focused);
            if (focused != NULL) {
                AutoPtr<IView> v;
                focused->FocusSearch(direction, (IView**)&v);
                if (v != NULL && v != focused) {
                    // do the math the get the interesting rect
                    // of previous focused into the coord system of
                    // newly focused view
                    focused->GetFocusedRect(mTempRect);
                    IViewGroup* viewGroup = IViewGroup::Probe(mView.Get());
                    if (viewGroup) {
                        viewGroup->OffsetDescendantRectToMyCoords(
                            focused, mTempRect);
                        viewGroup->OffsetRectIntoDescendantCoords(
                            v, mTempRect);
                    }
                    Boolean res;
                    v->RequestFocusEx2(direction, mTempRect, &res);
                    if (res) {
                        PlaySoundEffect(SoundEffectConstants::GetContantForFocusDirection(direction));
                        FinishInputEvent(q, TRUE);
                        return;
                    }
                }

                // Give the focused view a last chance to handle the dpad key.
                if (mView->DispatchUnhandledMove(focused, direction, &handled), handled) {
                    FinishInputEvent(q, TRUE);
                    return;
                }
            }
        }
    }

    // Key was unhandled.
    FinishInputEvent(q, FALSE);
}

/* drag/drop */
void ViewRootImpl::SetLocalDragState(
    /* [in] */ IInterface* obj)
{
    mLocalDragState = obj;
}

void ViewRootImpl::HandleDragEvent(
    /* [in] */ IDragEvent* event)
{
    // From the root, only drag start/end/location are dispatched.  entered/exited
    // are determined and dispatched by the viewgroup hierarchy, who then report
    // that back here for ultimate reporting back to the framework.
    if (mView != NULL && mAdded) {
        Int32 what;
        event->GetAction(&what);

        if (what == IDragEvent::ACTION_DRAG_EXITED) {
            // A direct EXITED event means that the window manager knows we've just crossed
            // a window boundary, so the current drag target within this one must have
            // just been exited.  Send it the usual notifications and then we're done
            // for now.
            Boolean res;
            mView->DispatchDragEvent(event, &res);
        }
        else {
            // Cache the drag description when the operation starts, then fill it in
            // on subsequent calls as a convenience
            if (what == IDragEvent::ACTION_DRAG_STARTED) {
                mCurrentDragView = NULL;    // Start the current-recipient tracking
                // mDragDescription = event->mClipDescription;
            }
            else {
                // event->mClipDescription = mDragDescription;
            }

            // For events with a [screen] location, translate into window coordinates
            if ((what == IDragEvent::ACTION_DRAG_LOCATION) || (what == IDragEvent::ACTION_DROP)) {
                Float x, y;
                event->GetX(&x);
                event->GetY(&y);
                mDragPoint->Set(x, y);
                if (mTranslator != NULL) {
                    mTranslator->TranslatePointInScreenToAppWindow(mDragPoint);
                }

                if (mCurScrollY != 0) {
                    mDragPoint->Offset(0, mCurScrollY);
                }

                mDragPoint->GetX(&x);
                mDragPoint->GetY(&y);
                event->SetX(x);
                event->SetY(y);
            }

            // Remember who the current drag target is pre-dispatch
            IView* prevDragView = mCurrentDragView;

            // Now dispatch the drag/drop event
            Boolean result;
            mView->DispatchDragEvent(event, &result);

            // If we changed apparent drag target, tell the OS about it
            if (prevDragView != mCurrentDragView.Get()) {
                //try {
                    if (prevDragView != NULL) {
                        mWindowSession->DragRecipientExited(mWindow);
                    }
                    if (mCurrentDragView != NULL) {
                        mWindowSession->DragRecipientEntered(mWindow);
                    }
                //} catch (RemoteException e) {
                //    Slog.e(TAG, "Unable to note drag target change");
                //}
            }

            // Report the drop result when we're done
            if (what == IDragEvent::ACTION_DROP) {
                mDragDescription = NULL;
                Logger::I(TAG, "Reporting drop result: %d" , result);
                if (FAILED(mWindowSession->ReportDropResult(mWindow, result)))
                    Logger::E(TAG, "Unable to report drop result");
            }

            // When the drag operation ends, release any local state object
            // that may have been in use
            if (what == IDragEvent::ACTION_DRAG_ENDED) {
                SetLocalDragState(NULL);
            }
        }
    }
    event->Recycle();
}

void ViewRootImpl::HandleDispatchSystemUiVisibilityChanged(
    /* [in] */ SystemUiVisibilityInfo* args)
{
    if (mSeq != args->mSeq) {
        // The sequence has changed, so we need to update our value and make
        // sure to do a traversal afterward so the window manager is given our
        // most recent data.
        mSeq = args->mSeq;
        mAttachInfo->mForceReportNewAttributes = TRUE;
        ScheduleTraversals();
    }

    if (mView == NULL) {
        delete args;
        return;
    }

    if (args->mLocalChanges != 0) {
        VIEW_PROBE(mView)->UpdateLocalSystemUiVisibility(args->mLocalValue, args->mLocalChanges);
    }

    if (mAttachInfo != NULL) {
        Int32 visibility = args->mGlobalVisibility & IView::SYSTEM_UI_CLEARABLE_FLAGS;
        if (visibility != mAttachInfo->mGlobalSystemUiVisibility) {
            mAttachInfo->mGlobalSystemUiVisibility = visibility;
            mView->DispatchSystemUiVisibilityChanged(visibility);
        }
    }

    delete args;
}

void ViewRootImpl::HandleDispatchDoneAnimating()
{
    if (mWindowsAnimating) {
        mWindowsAnimating = FALSE;
        Boolean isEmpty;
        mDirty->IsEmpty(&isEmpty);
        if (!isEmpty || mIsAnimating)  {
            ScheduleTraversals();
        }
    }
}

void ViewRootImpl::GetLastTouchPoint(
    /* [in] */ IPoint* outLocation)
{
    Float x, y;
    mLastTouchPoint->GetX(&x);
    mLastTouchPoint->GetY(&y);
    outLocation->Set((Int32)x, (Int32)y);
}

void ViewRootImpl::SetDragFocus(
    /* [in] */ IView* newDragTarget)
{
    if (mCurrentDragView.Get() != newDragTarget) {
        mCurrentDragView = newDragTarget;
    }
}

AutoPtr<IAudioManager> ViewRootImpl::GetAudioManager()
{
    if (mView == NULL) {
        Logger::E(TAG, "getAudioManager called when there is no mView");
        return NULL;
    }

    if (mAudioManager == NULL) {
        AutoPtr<IContext> context;
        mView->GetContext((IContext**)&context);

        AutoPtr<IInterface> audioService;
        context->GetSystemService(IContext::AUDIO_SERVICE, (IInterface**)&audioService);
        mAudioManager = IAudioManager::Probe(audioService);
    }

    return mAudioManager;
}

AutoPtr<IAccessibilityInteractionController> ViewRootImpl::GetAccessibilityInteractionController()
{
    if (mView == NULL) {
        Logger::E(TAG, "getAccessibilityInteractionController"
            " called when there is no mView");
        assert(0);
/*        throw new IllegalStateException("getAccessibilityInteractionController"
                + " called when there is no mView");*/
    }
    if (mAccessibilityInteractionController == NULL) {
        CAccessibilityInteractionController::New(
            (Handle32)this, (IAccessibilityInteractionController**)&mAccessibilityInteractionController);
    }
    return mAccessibilityInteractionController;
}

Int32 ViewRootImpl::RelayoutWindow(
    /* [in] */ CWindowManagerLayoutParams* params,
    /* [in] */ Int32 viewVisibility,
    /* [in] */ Boolean insetsPending)
{
    Float appScale = mAttachInfo->mApplicationScale;
    Boolean restore = FALSE;

    if (params != NULL && mTranslator != NULL) {
        restore = TRUE;
        params->Backup();
        mTranslator->TranslateWindowLayout(params);
    }

    if (params != NULL) {
        if (DBG) {
            Logger::D(TAG, "WindowLayout in layoutWindow:" /*+ params*/);
        }
    }

    mPendingConfiguration->SetSeq(0);

    //ger::oG.d(TAG, ">>>>>> CALLING relayout");
    if (params != NULL && mOrigWindowType != params->mType) {
        // For compatibility with old apps, don't crash here.
        if (mTargetSdkVersion < Build::VERSION_CODES::ICE_CREAM_SANDWICH) {
            Slogger::W(TAG, "Window type can not be changed after "
                "the window is added; ignoring change of 0x%08x", mView.Get());
            params->mType = mOrigWindowType;
        }
    }

    Int32 measuredWidth, measuredHeight;
    AutoPtr<IRect> winFrame, pendingContentInsets, pendingVisibleInsets;
    AutoPtr<IConfiguration> pendingConfiguration;
    AutoPtr<ISurface> surface;
    Int32 relayoutResult = 0;

    mView->GetMeasuredWidth(&measuredWidth);
    mView->GetMeasuredHeight(&measuredHeight);

    mWindowSession->Relayout(
        (IIWindow*)mWindow, mSeq, params,
        (Int32)(measuredWidth * appScale + 0.5f),
        (Int32)(measuredHeight * appScale + 0.5f),
        viewVisibility, insetsPending ? IWindowManagerGlobal::RELAYOUT_INSETS_PENDING : 0,
        mWinFrame.Get(), mPendingContentInsets.Get(), mPendingVisibleInsets.Get(),
        mPendingConfiguration.Get(), mSurface.Get(),
        (IRect**)&winFrame, (IRect**)&pendingContentInsets,
        (IRect**)&pendingVisibleInsets, (IConfiguration**)&pendingConfiguration,
        &relayoutResult, (ISurface**)&surface);
    if (winFrame != NULL)
        mWinFrame->SetEx(winFrame);
    if (pendingContentInsets != NULL)
        mPendingContentInsets->SetEx(pendingContentInsets);
    if (pendingVisibleInsets != NULL)
        mPendingVisibleInsets->SetEx(pendingVisibleInsets);
    if (pendingConfiguration != NULL)
        mPendingConfiguration->SetTo(pendingConfiguration);
    if (surface != NULL) {
        Handle32 nativeSurface;
        surface->GetSurface(&nativeSurface);
        mSurface->SetSurface(nativeSurface);
    }

    if (restore) {
        params->Restore();
    }

    if (mTranslator != NULL) {
        mTranslator->TranslateRectInScreenToAppWinFrame(mWinFrame);
        mTranslator->TranslateRectInScreenToAppWindow(mPendingContentInsets);
        mTranslator->TranslateRectInScreenToAppWindow(mPendingVisibleInsets);
    }

    return relayoutResult;
}

ECode ViewRootImpl::PlaySoundEffect(
    /* [in] */ Int32 effectId)
{
    FAIL_RETURN(CheckThread());

    AutoPtr<IAudioManager> audioManager = GetAudioManager();
    if (audioManager == NULL) {
        Logger::E(TAG, "Audio service is not ready when play sound effect.");
        return E_RUNTIME_EXCEPTION;
    }

    ECode ec = E_ILLEGAL_ARGUMENT_EXCEPTION;

    switch (effectId) {
        case SoundEffectConstants::CLICK:
            ec = audioManager->PlaySoundEffect(IAudioManager::FX_KEY_CLICK);
            if (FAILED(ec))
                goto _Exit_;
            return NOERROR;
        case SoundEffectConstants::NAVIGATION_DOWN:
            ec = audioManager->PlaySoundEffect(IAudioManager::FX_FOCUS_NAVIGATION_DOWN);
            if (FAILED(ec))
                goto _Exit_;
            return NOERROR;
        case SoundEffectConstants::NAVIGATION_LEFT:
            ec = audioManager->PlaySoundEffect(IAudioManager::FX_FOCUS_NAVIGATION_LEFT);
            if (FAILED(ec))
                goto _Exit_;
            return NOERROR;
        case SoundEffectConstants::NAVIGATION_RIGHT:
            ec = audioManager->PlaySoundEffect(IAudioManager::FX_FOCUS_NAVIGATION_RIGHT);
            if (FAILED(ec))
                goto _Exit_;
            return NOERROR;
        case SoundEffectConstants::NAVIGATION_UP:
            ec = audioManager->PlaySoundEffect(IAudioManager::FX_FOCUS_NAVIGATION_UP);
            if (FAILED(ec))
                goto _Exit_;
            return NOERROR;
        default:
            Logger::E(TAG, "unknown effect id %d  not defined in SoundEffectConstants.class.getCanonicalName().", effectId);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

_Exit_:
    // Exception thrown by getAudioManager() when mView is NULL
    Logger::E(TAG, "FATAL EXCEPTION when attempting to play sound effect: ");
    return ec;
}

ECode ViewRootImpl::PerformHapticFeedback(
    /* [in] */ Int32 effectId,
    /* [in] */ Boolean always,
    /* [out] */ Boolean* result)
{
    return mWindowSession->PerformHapticFeedback(mWindow, effectId, always, result);
}

ECode ViewRootImpl::FocusSearch(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out */ IView** result)
{
    VALIDATE_NOT_NULL(result);

    FAIL_RETURN(CheckThread());

    IViewGroup* viewGroup = (IViewGroup*)mView->Probe(EIID_IViewGroup);
    if (viewGroup == NULL) {
        *result  = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FocusFinder::GetInstance()->FindNextFocus(
        IVIEWGROUP_PROBE(mView), focused, direction, result);

    return NOERROR;
}

ECode ViewRootImpl::FocusSearchEx(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out */ IView** result)
{
    return FocusSearch(focused, direction, result);
}

void ViewRootImpl::Debug()
{
    //mView->Debug();
}

void ViewRootImpl::DumpGfxInfo(
    /* [in] */ ArrayOf<Int32>* info)
{
    (*info)[0] = (*info)[1] = 0;
    if (mView != NULL) {
        GetGfxInfo(mView, info);
    }
}

void ViewRootImpl::GetGfxInfo(
    /* [in] */ IView* view,
    /* [in] */ ArrayOf<Int32>* info)
{
    AutoPtr<IDisplayList> displayList;
    view->GetDisplayList((IDisplayList**)&displayList);
    (*info)[0]++;
    if (displayList != NULL) {
        Int32 size;
        displayList->GetSize(&size);
        (*info)[1] += size;
    }

    IViewGroup* group = IViewGroup::Probe(view);
    if (group) {
        Int32 count;
        group->GetChildCount(&count);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child;
            group->GetChildAt(i, (IView**)&child);
            GetGfxInfo(child, info);
        }
    }
}

ECode ViewRootImpl::Die(
    /* [in] */ Boolean immediate)
{
    // Make sure we do execute immediately if we are in the middle of a traversal or the damage
    // done by dispatchDetachedFromWindow will cause havoc on return.

    // For DoDie to release
    AddRef();

    if (immediate && !mIsInTraversal) {
        return DoDie();
    }
    else {
        if (!mIsDrawing) {
            DestroyHardwareRenderer();
        }
        else {
            Logger::E(TAG, "Attempting to destroy the window while drawing!\n" /*+
                    "  window=" + this + ", title=" + mWindowAttributes.getTitle()*/);
        }

        Boolean result;
        mHandler->SendEmptyMessage(MSG_DIE, &result);
    }

    return NOERROR;
}

ECode ViewRootImpl::DoDie()
{
    FAIL_RETURN(CheckThread());

    if (LOCAL_LOGV) {
        Logger::D(TAG, "DIE in 0x%08x of 0x%08x", this, mSurface.Get());
    }

    Mutex::Autolock lock(mSyncLock);

    if (mAdded) {
        DispatchDetachedFromWindow();
    }

    if (mAdded && !mFirst) {
        DestroyHardwareRenderer();

        if (mView != NULL) {
            Int32 viewVisibility;
            mView->GetVisibility(&viewVisibility);

            Boolean viewVisibilityChanged = mViewVisibility != viewVisibility;
            if (mWindowAttributesChanged || viewVisibilityChanged) {
                // If layout params have been changed, first give them
                // to the window manager to make sure it has the correct
                // animation info.
                //
                if ((RelayoutWindow(mWindowAttributes, viewVisibility, FALSE)
                    & IWindowManagerGlobal::RELAYOUT_RES_FIRST_TIME) != 0) {
                    mWindowSession->FinishDrawing(mWindow);
                }
            }
        }

        mSurface->ReleaseSurface();
        mAdded = FALSE;
    }

    Release();
    // TODO: stalling tactics for wrong reference count

    return NOERROR;
}

void ViewRootImpl::RequestUpdateConfiguration(
    /* [in] */ IConfiguration* config)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_UPDATE_CONFIGURATION, config, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::LoadSystemProperties()
{
    Boolean layout = SystemProperties::GetBoolean(IView::DEBUG_LAYOUT_PROPERTY, FALSE);
    if (layout != mAttachInfo->mDebugLayout) {
        mAttachInfo->mDebugLayout = layout;
        Boolean res;
        if (mHandler->HasMessages(MSG_INVALIDATE_WORLD, &res), !res) {
            mHandler->SendEmptyMessageDelayed(MSG_INVALIDATE_WORLD, 200, &res);
        }
    }
}

void ViewRootImpl::DestroyHardwareRenderer()
{
    // View::AttachInfo* attachInfo = mAttachInfo;
    // HardwareRenderer* hardwareRenderer = attachInfo->mHardwareRenderer;

    // if (hardwareRenderer != NULL) {
    //     if (mView != NULL) {
    //         hardwareRenderer->DestroyHardwareResources(mView);
    //     }
    //     hardwareRenderer->Destroy(TRUE);
    //     hardwareRenderer->SetRequested(FALSE);

    //     attachInfo->mHardwareRenderer = NULL;
    //     attachInfo->mHardwareAccelerated = FALSE;
    // }
}

void ViewRootImpl::DispatchImeFinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx2(MSG_IME_FINISHED_EVENT,
        seq, handled ? 1 : 0, (IMessage**)&msg);
    msg->SetAsynchronous(TRUE);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchFinishInputConnection(
    /* [in] */ IInputConnection* connection)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_FINISH_INPUT_CONNECTION, connection, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchResized(
    /* [in] */ IRect* frame,
    /* [in] */ IRect* contentInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    // if (DEBUG_LAYOUT) Logger::V(TAG, "Resizing " + this + ": frame=" + frame.toShortString()
    //         + " contentInsets=" + contentInsets.toShortString()
    //         + " visibleInsets=" + visibleInsets.toShortString()
    //         + " reportDraw=" + reportDraw);

    if (mTranslator != NULL) {
        mTranslator->TranslateRectInScreenToAppWindow(frame);
        mTranslator->TranslateRectInScreenToAppWindow(contentInsets);
        mTranslator->TranslateRectInScreenToAppWindow(visibleInsets);
    }

    Boolean sameProcessCall = (Binder::GetCallingPid() == Process::MyPid());
    AutoPtr<IRect> argFrame, argContentInsets, argVisibleInsets;
    AutoPtr<IConfiguration> argConfig;
    if (sameProcessCall) {
        CRect::New(frame, (IRect**)&argFrame);
        CRect::New(contentInsets, (IRect**)&argContentInsets);
        CRect::New(visibleInsets, (IRect**)&argVisibleInsets);
        if (newConfig != NULL) {
            CConfiguration::New(newConfig, (IConfiguration**)&argConfig);
        }
    }
    else {
        argFrame = frame;
        argContentInsets = contentInsets;
        argVisibleInsets = visibleInsets;
        argConfig = newConfig;
    }

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = argFrame;
    args->mArg2 = argContentInsets;
    args->mArg3 = argVisibleInsets;
    args->mArg4 = argConfig;

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(reportDraw ? MSG_RESIZED_REPORT : MSG_RESIZED,
        args, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchMoved(
    /* [in] */ Int32 newX,
    /* [in] */ Int32 newY)
{
    if (DEBUG_LAYOUT)
        Logger::V(TAG, "Window moved 0x%08x: newX=%d, newY=%d", this, newX, newY);

    if (mTranslator != NULL) {
        AutoPtr<IPointF> point;
        CPointF::New(newX, newY, (IPointF**)&point);
        mTranslator->TranslatePointInScreenToAppWindow(point);
        Float x, y;
        point->GetX(&x);
        point->GetY(&y);
        newX = (Int32)(x + 0.5);
        newY = (Int32)(y + 0.5);
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx2(MSG_WINDOW_MOVED, newX, newY, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

AutoPtr<ViewRootImpl::QueuedInputEvent> ViewRootImpl::ObtainQueuedInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ InputEventReceiver* receiver,
    /* [in] */ Int32 flags)
{
    AutoPtr<QueuedInputEvent> q = mQueuedInputEventPool;
    if (q != NULL) {
        mQueuedInputEventPoolSize -= 1;
        mQueuedInputEventPool = q->mNext;
        q->mNext = NULL;
    }
    else {
        q = new QueuedInputEvent();
    }

    q->mEvent = event;
    q->mReceiver = receiver;
    q->mFlags = flags;
    return q;
}

void ViewRootImpl::RecycleQueuedInputEvent(
    /* [in] */ QueuedInputEvent* q)
{
    q->mEvent = NULL;
    q->mReceiver = NULL;

    if (mQueuedInputEventPoolSize < MAX_QUEUED_INPUT_EVENT_POOL_SIZE) {
        mQueuedInputEventPoolSize += 1;
        q->mNext = mQueuedInputEventPool;
        mQueuedInputEventPool = q;
    }
}

void ViewRootImpl::EnqueueInputEvent(
    /* [in] */ IInputEvent* event)
{
    EnqueueInputEvent(event, NULL, 0, FALSE);
}

void ViewRootImpl::EnqueueInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ InputEventReceiver* receiver,
    /* [in] */ Int32 flags,
    /* [in] */ Boolean processImmediately)
{
    AutoPtr<QueuedInputEvent> q = ObtainQueuedInputEvent(event, receiver, flags);

    // Always enqueue the input event in order, regardless of its time stamp.
    // We do this because the application or the IME may inject key events
    // in response to touch events and we want to ensure that the injected keys
    // are processed in the order they were received and we cannot trust that
    // the time stamp of injected events are monotonic.
    QueuedInputEvent* last = mFirstPendingInputEvent;
    if (last == NULL) {
        mFirstPendingInputEvent = q;
    }
    else {
        while (last->mNext != NULL) {
            last = last->mNext;
        }
        last->mNext = q;
    }

    if (processImmediately) {
        DoProcessInputEvents();
    }
    else {
        ScheduleProcessInputEvents();
    }
}

void ViewRootImpl::ScheduleProcessInputEvents()
{
    if (!mProcessInputEventsScheduled) {
        mProcessInputEventsScheduled = TRUE;

        AutoPtr<IMessage> msg;
        mHandler->ObtainMessage(MSG_PROCESS_INPUT_EVENTS, (IMessage**)&msg);
        msg->SetAsynchronous(TRUE);
        Boolean result;
        mHandler->SendMessage(msg, &result);
    }
}

void ViewRootImpl::DoProcessInputEvents()
{
    while (mCurrentInputEvent == NULL && mFirstPendingInputEvent != NULL) {
        AutoPtr<QueuedInputEvent> q = mFirstPendingInputEvent;
        mFirstPendingInputEvent = q->mNext;
        q->mNext = NULL;
        mCurrentInputEvent = q;
        DeliverInputEvent(q);
    }

    // We are done processing all input events that we can process right now
    // so we can clear the pending flag immediately.
    if (mProcessInputEventsScheduled) {
        mProcessInputEventsScheduled = FALSE;
        mHandler->RemoveMessages(MSG_PROCESS_INPUT_EVENTS);
    }
}

ECode ViewRootImpl::FinishInputEvent(
    /* [in] */ QueuedInputEvent* q,
    /* [in] */ Boolean handled)
{
    if (q != mCurrentInputEvent) {
        Logger::E(TAG, "finished input event out of order");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (q->mReceiver != NULL) {
        q->mReceiver->FinishInputEvent(q->mEvent, handled);
    }
    else {
        q->mEvent->RecycleIfNeededAfterDispatch();
    }

    RecycleQueuedInputEvent(q);

    mCurrentInputEvent = NULL;
    if (mFirstPendingInputEvent != NULL) {
        ScheduleProcessInputEvents();
    }

    return NOERROR;
}

void ViewRootImpl::ScheduleConsumeBatchedInput()
{
    if (!mConsumeBatchedInputScheduled) {
        mConsumeBatchedInputScheduled = TRUE;
        mChoreographer->PostCallback(IChoreographer::CALLBACK_INPUT,
                mConsumedBatchedInputRunnable, NULL);
    }
}

void ViewRootImpl::UnscheduleConsumeBatchedInput()
{
    if (mConsumeBatchedInputScheduled) {
        mConsumeBatchedInputScheduled = FALSE;
        mChoreographer->RemoveCallbacks(IChoreographer::CALLBACK_INPUT,
                mConsumedBatchedInputRunnable, NULL);
    }
}

void ViewRootImpl::DoConsumeBatchedInput(
    /* [in] */ Int64 frameTimeNanos)
{
    if (mConsumeBatchedInputScheduled) {
        mConsumeBatchedInputScheduled = FALSE;
        if (mInputEventReceiver != NULL) {
            mInputEventReceiver->ConsumeBatchedInputEvents(frameTimeNanos);
        }
        DoProcessInputEvents();
    }
}

void ViewRootImpl::DispatchInvalidateDelayed(
    /* [in] */ IView* view,
    /* [in] */ Int64 delayMilliseconds)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_INVALIDATE, view, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessageDelayed(msg, delayMilliseconds, &result);
}

void ViewRootImpl::DispatchInvalidateRectDelayed(
    /* [in] */ View::AttachInfo::InvalidateInfo* info,
    /* [in] */ Int64 delayMilliseconds)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_INVALIDATE_RECT, info, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessageDelayed(msg, delayMilliseconds, &result);
}

void ViewRootImpl::DispatchInvalidateOnAnimation(
    /* [in] */ IView* view)
{
    mInvalidateOnAnimationRunnable->AddView(view);
}

void ViewRootImpl::DispatchInvalidateRectOnAnimation(
    /* [in] */ View::AttachInfo::InvalidateInfo* info)
{
    mInvalidateOnAnimationRunnable->AddViewRect(info);
}

void ViewRootImpl::EnqueueDisplayList(
    /* [in] */ IDisplayList* displayList)
{
    mDisplayLists.PushBack(displayList);

    mHandler->RemoveMessages(MSG_INVALIDATE_DISPLAY_LIST);
    Boolean result;
    mHandler->SendEmptyMessage(MSG_INVALIDATE_DISPLAY_LIST, &result);
}

void ViewRootImpl::DequeueDisplayList(
    /* [in] */ IDisplayList* displayList)
{
    AutoPtr<IDisplayList> temp = displayList;
    List<AutoPtr<IDisplayList> >::Iterator iter =
        Find(mDisplayLists.Begin(), mDisplayLists.End(), temp);
    if (iter != mDisplayLists.End()) {
        mDisplayLists.Erase(iter);
        displayList->Invalidate();
        if (mDisplayLists.IsEmpty()) {
            mHandler->RemoveMessages(MSG_INVALIDATE_DISPLAY_LIST);
        }
    }
}

void ViewRootImpl::CancelInvalidate(
    /* [in] */ IView* view)
{
    mHandler->RemoveMessagesEx(MSG_INVALIDATE, view);
    // fixme: might leak the AttachInfo.InvalidateInfo objects instead of returning
    // them to the pool
    mHandler->RemoveMessagesEx(MSG_INVALIDATE_RECT, view);
    mInvalidateOnAnimationRunnable->RemoveView(view);
}

void ViewRootImpl::DispatchKey(
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_DISPATCH_KEY, event, (IMessage**)&msg);
    msg->SetAsynchronous(TRUE);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchKeyFromIme(
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_DISPATCH_KEY_FROM_IME, event, (IMessage**)&msg);
    msg->SetAsynchronous(TRUE);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchUnhandledKey(
    /* [in] */ IKeyEvent* event)
{
    Int32 flags;
    event->GetFlags(&flags);
    if ((flags & IKeyEvent::FLAG_FALLBACK) == 0) {
        AutoPtr<IKeyCharacterMap> kcm;
        event->GetKeyCharacterMap((IKeyCharacterMap**)&kcm);
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        Int32 metaState;
        event->GetMetaState(&metaState);

        // Check for fallback actions specified by the key character map.
        AutoPtr<IFallbackAction> fallbackAction;
        kcm->GetFallbackAction(keyCode, metaState, (IFallbackAction**)&fallbackAction);
        if (fallbackAction != NULL) {
            flags |= IKeyEvent::FLAG_FALLBACK;
            Int64 downTime, eventTime;
            Int32 action, repeatCount, deviceId, scanCode, source;
            event->GetDownTime(&downTime);
            event->GetEventTime(&eventTime);
            event->GetAction(&action);
            event->GetRepeatCount(&repeatCount);
            event->GetDeviceId(&deviceId);
            event->GetScanCode(&scanCode);
            event->GetSource(&source);
            AutoPtr<IKeyEvent> fallbackEvent = CKeyEvent::Obtain(
                    downTime, eventTime, action,
                    ((CKeyCharacterMap::FallbackAction*)fallbackAction.Get())->mKeyCode,
                    repeatCount,
                    ((CKeyCharacterMap::FallbackAction*)fallbackAction.Get())->mMetaState,
                    deviceId, scanCode, flags, source, String(NULL));
            fallbackAction->Recycle();

            DispatchKey(fallbackEvent);
        }
    }
}

void ViewRootImpl::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx2(MSG_DISPATCH_APP_VISIBILITY,
        visible ? 1 : 0, 0, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchScreenStateChange(
    /* [ijn] */ Boolean on)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx2(MSG_DISPATCH_SCREEN_STATE,
        on ? 1 : 0, 0, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchGetNewSurface()
{
    Boolean result;
    mHandler->SendEmptyMessage(MSG_DISPATCH_GET_NEW_SURFACE, &result);
}

void ViewRootImpl::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx2(MSG_WINDOW_FOCUS_CHANGED,
        hasFocus ? 1 : 0, inTouchMode ? 1 : 0, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(reason, (ICharSequence**)&seq);

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_CLOSE_SYSTEM_DIALOGS,
        seq, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchDragEvent(
    /* [in] */ IDragEvent* event)
{
    Int32 what = MSG_DISPATCH_DRAG_EVENT;
    Int32 action;
    event->GetAction(&action);
    if (action == IDragEvent::ACTION_DRAG_LOCATION) {
        what = MSG_DISPATCH_DRAG_LOCATION_EVENT;
        mHandler->RemoveMessages(what);
    }

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(what, event, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchSystemUiVisibilityChanged(
    /* [in] */ Int32 seq,
    /* [in] */ Int32 globalVisibility,
    /* [in] */ Int32 localValue,
    /* [in] */ Int32 localChanges)
{
    AutoPtr<SystemUiVisibilityInfo> args = new SystemUiVisibilityInfo();
    args->mSeq = seq;
    args->mGlobalVisibility = globalVisibility;
    args->mLocalValue = localValue;
    args->mLocalChanges = localChanges;

    AutoPtr<IMessage> msg;
    mHandler->ObtainMessageEx(MSG_DISPATCH_SYSTEM_UI_VISIBILITY, args, (IMessage**)&msg);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void ViewRootImpl::DispatchDoneAnimating()
{
    Boolean result;
    mHandler->SendEmptyMessage(MSG_DISPATCH_DONE_ANIMATING, &result);
}

void ViewRootImpl::DispatchCheckFocus()
{
    Boolean result;
    if (mHandler->HasMessages(MSG_CHECK_FOCUS, &result), !result) {
        // This will result in a call to checkFocus() below.
        mHandler->SendEmptyMessage(MSG_CHECK_FOCUS, &result);
    }
}

/**
 * Post a callback to send a
 * {@link AccessibilityEvent#TYPE_WINDOW_CONTENT_CHANGED} event->
 * This event is send at most once every
 * {@link ViewConfiguration#getSendRecurringAccessibilityEventsInterval()}.
 */
void ViewRootImpl::PostSendWindowContentChangedCallback(
    /* [in] */ IView* source)
{
    if (mSendWindowContentChangedAccessibilityEvent == NULL) {
        mSendWindowContentChangedAccessibilityEvent =
            new SendWindowContentChangedAccessibilityEvent();
    }
    AutoPtr<IView> oldSource = mSendWindowContentChangedAccessibilityEvent->mSource;
    if (oldSource == NULL) {
        mSendWindowContentChangedAccessibilityEvent->mSource = source;
        Boolean result;
        mHandler->PostDelayed(
            mSendWindowContentChangedAccessibilityEvent,
            CViewConfiguration::GetSendRecurringAccessibilityEventsInterval(),
            &result);
    }
    else {
        mSendWindowContentChangedAccessibilityEvent->mSource =
                GetCommonPredecessor(oldSource, source);
    }
}

/**
 * Remove a posted callback to send a
 * {@link AccessibilityEvent#TYPE_WINDOW_CONTENT_CHANGED} event->
 */
void ViewRootImpl::RemoveSendWindowContentChangedCallback()
{
    if (mSendWindowContentChangedAccessibilityEvent != NULL) {
        mHandler->RemoveCallbacks(mSendWindowContentChangedAccessibilityEvent);
    }
}

ECode ViewRootImpl::ShowContextMenuForChild(
    /* [in] */ IView* originalView,
    /* [out] */ Boolean* result)
{
    assert(result);
    *result = FALSE;

    return NOERROR;
}

ECode ViewRootImpl::StartActionModeForChildEx(
    /* [in] */ IView* originalView,
    /* [in] */ IActionModeCallback* callback,
    /* [out] */ IActionMode** actionMode)
{
    VALIDATE_NOT_NULL(actionMode);
    *actionMode = NULL;
    return NOERROR;
}

ECode ViewRootImpl::CreateContextMenuEx(
    /* [in] */ IContextMenu* menu)
{
    return NOERROR;
}

void ViewRootImpl::CreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
}

ECode ViewRootImpl::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    return NOERROR;
}

ECode ViewRootImpl::RequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;

    if (mView == NULL) {
        return NOERROR;
    }

    // Intercept accessibility focus events fired by virtual nodes to keep
    // track of accessibility focus position in such nodes.
    Int32 eventType;
    event->GetEventType(&eventType);
    switch (eventType) {
        case IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED: {
            Int64 sourceNodeId;
            event->GetSourceNodeId(&sourceNodeId);
            Int32 accessibilityViewId = CAccessibilityNodeInfo::GetAccessibilityViewId(
                    sourceNodeId);
            AutoPtr<IView> source = VIEW_PROBE(mView.Get())->FindViewByAccessibilityId(accessibilityViewId);
            if (source != NULL) {
                AutoPtr<IAccessibilityNodeProvider> provider;
                source->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
                if (provider != NULL) {
                    AutoPtr<IAccessibilityNodeInfo> node;
                    provider->CreateAccessibilityNodeInfo(
                        CAccessibilityNodeInfo::GetVirtualDescendantId(sourceNodeId),
                        (IAccessibilityNodeInfo**)&node);
                    SetAccessibilityFocus(source, node);
                }
            }
        }
        break;
        case IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED: {
            Int64 sourceNodeId;
            event->GetSourceNodeId(&sourceNodeId);
            Int32 accessibilityViewId = CAccessibilityNodeInfo::GetAccessibilityViewId(
                    sourceNodeId);
            AutoPtr<IView> source = VIEW_PROBE(mView.Get())->FindViewByAccessibilityId(accessibilityViewId);
            if (source != NULL) {
                AutoPtr<IAccessibilityNodeProvider> provider;
                source->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
                if (provider != NULL) {
                    SetAccessibilityFocus(NULL, NULL);
                }
            }
        }
        break;
    }
    mAccessibilityManager->SendAccessibilityEvent(event);
    *res = TRUE;
    return NOERROR;
}

//@Override
ECode ViewRootImpl::ChildAccessibilityStateChanged(
    /* [in] */ IView* child)
{
    PostSendWindowContentChangedCallback(child);
    return NOERROR;
}

AutoPtr<IView> ViewRootImpl::GetCommonPredecessor(
    /* [in] */ IView* first,
    /* [in] */ IView* second)
{
    if (mAttachInfo != NULL) {
        HashSet<AutoPtr<IView> >& seen = mTempHashSet;
        seen.Clear();
        AutoPtr<IView>firstCurrent = first;
        while (firstCurrent != NULL) {
            seen.Insert(firstCurrent);
            AutoPtr<IViewParent> firstCurrentParent;
            firstCurrent->GetParent((IViewParent**)&firstCurrentParent);
            firstCurrent = IView::Probe(firstCurrentParent);
        }

        AutoPtr<IView> secondCurrent = second;
        while (secondCurrent != NULL) {
            if (seen.Find(secondCurrent) != seen.End()) {
                seen.Clear();
                return secondCurrent;
            }

            AutoPtr<IViewParent> secondCurrentParent;
            secondCurrent->GetParent((IViewParent**)&secondCurrentParent);
            secondCurrent = IView::Probe(secondCurrentParent);
        }
        seen.Clear();
    }
    return NULL;
}

ECode ViewRootImpl::CheckThread()
{
    if (mThreadID != pthread_self()) {
//         throw new CalledFromWrongThreadException(
//                    "Only the original thread that created a view hierarchy can touch its views.");
        Logger::E(TAG,
            "Only the original thread that created a view hierarchy can touch its views.");

        return E_CALLED_FROM_WRONG_THREAD_EXCEPTION;
    }

    return NOERROR;
}

ECode ViewRootImpl::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    // ViewRootImpl never intercepts touch event, so this can be a no-op

    return NOERROR;
}

ECode ViewRootImpl::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean* result)
{
    assert(result);
    *result = ScrollToRectOrFocus(rectangle, immediate);
    if (rectangle != NULL) {
        mTempRect->SetEx(rectangle);
        mTempRect->Offset(0, -mCurScrollY);
        mTempRect->Offset(mAttachInfo->mWindowLeft, mAttachInfo->mWindowTop);
        mWindowSession->OnRectangleOnScreenRequested(
            IBinder::Probe(mWindow.Get()), mTempRect, immediate);
    }

    return NOERROR;
}

ECode ViewRootImpl::ChildHasTransientStateChangedEx(
    /* [in] */ IView* child,
    /* [in] */ Boolean hasTransientState)
{
    // Do nothing.
    return NOERROR;
}

// void ViewRootImpl::HandleInvalidateRect(
//     /* [in] */ View::AttachInfo::InvalidateInfo* info)
// {
//     info->mTarget->InvalidateEx(info->mLeft, info->mTop, info->mRight, info->mBottom);
//     info->ReleaseInfo();
// }

// void ViewRootImpl::HandleProcessInputEvents()
// {
//     mProcessInputEventsScheduled = FALSE;
//     DoProcessInputEvents();
// }

// void ViewRootImpl::HandleResized(
//     /* [in] */ IRect* frame,
//     /* [in] */ IRect* contentInsets,
//     /* [in] */ IRect* visibleInsets,
//     /* [in] */ Boolean reportDraw,
//     /* [in] */ IConfiguration* newConfig)
// {
//     if (!reportDraw) {
//         Boolean isEqual1, isEqual2, isEqual3;
//         mWinFrame->Equals(frame, &isEqual1);
//         if (isEqual1
//             && (mPendingContentInsets->EqualsEx(contentInsets, &isEqual2), isEqual2)
//             && (mPendingVisibleInsets->EqualsEx(visibleInsets, &isEqual3), isEqual3)
//             && newConfig == NULL) {
//             return;
//         }
//     }

//     // MSG: MSG_RESIZED_REPORT
//     if (mAdded) {
//         if (newConfig != NULL) {
//             UpdateConfiguration(newConfig, FALSE);
//         }

//         mWinFrame->SetEx(frame);
//         mPendingContentInsets->SetEx(contentInsets);
//         mPendingVisibleInsets->SetEx(visibleInsets);

//         if (reportDraw) {
//             mReportNextDraw = TRUE;
//         }

//         if (mView != NULL) {
//             ForceLayout(mView);
//         }

//         RequestLayout();
//     }
// }

// void ViewRootImpl::HandleWindowMoved(
//     /* [in] */ Int32 l,
//     /* [in] */ Int32 t)
// {
//     if (mAdded) {
//         Int32 w, h;
//         mWinFrame->GetWidth(&w);
//         mWinFrame->GetHeight(&h);

//         mWinFrame->mLeft = l;
//         mWinFrame->mRight = l + w;
//         mWinFrame->mTop = t;
//         mWinFrame->mBottom = t + h;

//         if (mView != NULL) {
//             ForceLayout(mView);
//         }
//         RequestLayout();
//     }
// }

void ViewRootImpl::HandleWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus,
    /* [in] */ Boolean inTouchMode)
{
    if (mAdded) {
        mAttachInfo->mHasWindowFocus = hasWindowFocus;

        ProfileRendering(hasWindowFocus);

        if (hasWindowFocus) {
            EnsureTouchModeLocally(inTouchMode);
            Logger::D("ViewRootImpl", "TODO no impl, HandleWindowFocusChanged,line:%d ,hwRender:%p", __LINE__,mAttachInfo->mHardwareRenderer.Get());
            //now because the HardwareRender is not implemented.
            //so if need the OPENGL initialize function,try the below code
            //here is bug, you should open the app twice, the first time is black screen
            //AutoPtr<ISurface> surface;
            //mHolder->GetSurface((ISurface**)&surface);
            //Logger::D("ViewRootImpl", "HandleWindowFocusChanged,line:%d ,surface:%p", __LINE__,surface.Get());
            //CGLES20::InitGL(surface);
            // Boolean isValid;
            // if (mAttachInfo->mHardwareRenderer != NULL &&
            //     mSurface != NULL && (mSurface->IsValid(&isValid), isValid)) {
            //     mFullRedrawNeeded = TRUE;

            //     AutoPtr<ISurface> surface;
            //     mHolder->GetSurface((ISurface**)&surface);
            //     Boolean res;
            //     ECode ec = mAttachInfo->mHardwareRenderer->InitializeIfNeeded(
            //         mWidth, mHeight, surface, &res)
            //     if (ec == E_OUT_OF_RESOURCES_EXCEPTION) {
            //         Logger::E(TAG, "OutOfResourcesException locking surface");

            //         Boolean outOfMemory;
            //         mWindowSession->OutOfMemory(mWindow, &outOfMemory);
            //         if (!outOfMemory) {
            //             Slogger:W(TAG, "No processes killed for memory; killing self");
            //             CProcess::KillProcess(Process::MyPid());
            //         }

            //         // Retry in a bit.
            //         sendMessageDelayed(obtainMessage(msg.what, msg.arg1, msg.arg2), 500);
            //         return;
            //     }
            //     else if (FAILED(ec))
            //     {
            //         return;
            //     }

            //     if (res) {
            //         mFullRedrawNeeded = TRUE;
            //     }
            // }
        }

        mLastWasImTarget = CWindowManagerLayoutParams::MayUseInputMethod(
                mWindowAttributes->mFlags);

        AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
        if (mView != NULL) {
            if (hasWindowFocus && imm != NULL && mLastWasImTarget) {
                imm->StartGettingWindowFocus(mView);
            }

            mAttachInfo->mKeyDispatchState->Reset();
            mView->DispatchWindowFocusChanged(hasWindowFocus);
        }

        // Note: must be done after the focus change callbacks,
        // so all of the view state is set up correctly.
        if (hasWindowFocus) {
            if (imm != NULL && mLastWasImTarget) {
                AutoPtr<IView> focus;
                mView->FindFocus((IView**)&focus);
                imm->OnWindowFocus(
                    mView, focus, mWindowAttributes->mSoftInputMode,
                    !mHasHadWindowFocus, mWindowAttributes->mFlags);
            }

            // Clear the forward bit.  We can just do this directly, since
            // the window manager doesn't care about it.
            //
            mWindowAttributes->mSoftInputMode &=
                ~IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION;

            AutoPtr<IWindowManagerLayoutParams> params;
            mView->GetLayoutParams((IViewGroupLayoutParams**)&params);

            ((CWindowManagerLayoutParams*)params.Get())->mSoftInputMode &=
                ~IWindowManagerLayoutParams::SOFT_INPUT_IS_FORWARD_NAVIGATION;
            mHasHadWindowFocus = TRUE;
        }

        SetAccessibilityFocus(NULL, NULL);

        Boolean isEnabled;
        if (mView != NULL && (mAccessibilityManager->IsEnabled(&isEnabled), isEnabled)) {
            if (hasWindowFocus) {
                IAccessibilityEventSource::Probe(mView)->SendAccessibilityEvent(
                    IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED);
            }
        }
    }
}

void ViewRootImpl::LogView(
    /* [in] */ const String& info)
{
#ifdef _DEBUG
    if (mView != NULL) {
        Int32 id;
        mView->GetId(&id);
        if (id == 0x7f0700a2 /* SystemUIR::content_parent */) return;

        if (id != IView::NO_ID) {
            AutoPtr<IResources> resources;
            mView->GetResources((IResources**)&resources);
            String name;
            resources->GetResourceEntryName(id, &name);
            printf("    %s: viewId = 0x%08x, IView* = %p, type = %s\n", info.string(), id, mView.Get(), name.string());
            Logger::D("ViewRootImpl", "    %s: viewId = 0x%08x, IView* = %p, type = %s\n", info.string(), id, mView.Get(), name.string());
        }
        else {
            printf("    %s: viewId = 0x%08x, IView* = %p\n", info.string(), id, mView.Get());
            Logger::D("ViewRootImpl", "    %s: viewId = 0x%08x, IView* = %p\n", info.string(), id, mView.Get());
        }
    }
#endif
}

ECode ViewRootImpl::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    INTERFACE_ADDREF(*weakReference)
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
