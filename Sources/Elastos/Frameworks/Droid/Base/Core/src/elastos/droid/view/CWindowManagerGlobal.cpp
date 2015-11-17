
#include "elastos/droid/animation/CValueAnimator.h"
#include "elastos/droid/app/CActivityManager.h"
#include "elastos/droid/content/res/CConfiguration.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/os/CServiceManager.h"
#include "elastos/droid/os/SystemProperties.h"
#include "elastos/droid/view/CWindowManagerGlobal.h"
#include "elastos/droid/view/CWindowManagerLayoutParams.h"
#include "elastos/droid/view/ViewRootImpl.h"
#include "elastos/droid/view/CWindowManagerGlobalSessionCallback.h"
//#include "elastos/droid/view/inputmethod/CInputMethodManager.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Droid::App::CActivityManager;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
//using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Internal::View::IInputMethodClient;
using Elastos::Droid::Internal::View::IInputContext;
using Elastos::Droid::Animation::CValueAnimator;
using Elastos::Core::StringUtils;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

const char* CWindowManagerGlobal::TAG = "WindowManager";

//===========================================================================
// CWindowManagerGlobal::SystemPropertyUpdaterRunnable
//===========================================================================

CWindowManagerGlobal::SystemPropertyUpdaterRunnable::SystemPropertyUpdaterRunnable(
    /* [in] */ CWindowManagerGlobal* owner)
    : mOwner(owner)
{}

ECode CWindowManagerGlobal::SystemPropertyUpdaterRunnable::Run()
{
    AutoLock lock(mOwner->mLock);
    Int32 count = 0;
    mOwner->mRoots->GetSize(&count);
    for (Int32 i = count - 1; i >= 0; --i) {
        AutoPtr<IInterface> temp;
        mOwner->mRoots->Get(i, (IInterface**)&temp);
        AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
        impl->LoadSystemProperties();
    }

    return NOERROR;
}

//===========================================================================
// CWindowManagerGlobal
//===========================================================================
AutoPtr<IWindowManagerGlobal> CWindowManagerGlobal::sDefaultWindowManager;
Object CWindowManagerGlobal::sDefaultWindowManagerLock;
AutoPtr<IIWindowManager> CWindowManagerGlobal::sWindowManagerService;
AutoPtr<IWindowSession> CWindowManagerGlobal::sWindowSession;

CAR_INTERFACE_IMPL(CWindowManagerGlobal, Object, IWindowManagerGlobal)

CAR_OBJECT_IMPL(CWindowManagerGlobal)

CWindowManagerGlobal::CWindowManagerGlobal()
    : mNeedsEglTerminate(FALSE)
{
    CArrayList::New((IArrayList**)&mViews);
    CArrayList::New((IArrayList**)&mRoots);
    CArrayList::New((IArrayList**)&mParams);
    CArrayList::New((IArrayList**)&mDyingViews);
}

ECode CWindowManagerGlobal::constructor()
{
    return NOERROR;
}

AutoPtr<IWindowManagerGlobal> CWindowManagerGlobal::GetInstance()
{
    AutoLock lock(sDefaultWindowManagerLock);

    if (sDefaultWindowManager == NULL) {
        CWindowManagerGlobal::New((IWindowManagerGlobal**)&sDefaultWindowManager);
    }
    return sDefaultWindowManager;
}

AutoPtr<IIWindowManager> CWindowManagerGlobal::GetWindowManagerService()
{
    AutoLock lock(sDefaultWindowManagerLock);

    if (sWindowManagerService == NULL) {
        AutoPtr<IServiceManager> sm;
        CServiceManager::AcquireSingleton((IServiceManager**)&sm);
        sm->GetService(String("window"),
            (IInterface**)(IIWindowManager**)&sWindowManagerService);
    }
    return sWindowManagerService;
}

AutoPtr<IWindowSession> CWindowManagerGlobal::GetWindowSession(
    /* [in] */ ILooper* mainLooper)
{
    AutoLock lock(sDefaultWindowManagerLock);

    if (sWindowSession == NULL) {
        //try {
        assert(0);
        AutoPtr<IInputMethodManager> imm;// = CInputMethodManager::GetInstance(mainLooper);
        AutoPtr<IIWindowManager> wm = GetWindowManagerService();
        AutoPtr<IInputMethodClient> client;
        imm->GetClient((IInputMethodClient**)&client);
        AutoPtr<IInputContext> ctx;
        imm->GetInputContext((IInputContext**)&ctx);
        AutoPtr<IIWindowSessionCallback> cb;
        assert(0);
        //CWindowManagerGlobalSessionCallback::New((IIWindowSessionCallback**)&cb);
        wm->OpenSession(cb, client, ctx, (IWindowSession**)&sWindowSession);
        Float animatorScale;
        wm->GetCurrentAnimatorScale(&animatorScale);
        CValueAnimator::SetDurationScale(animatorScale);
        // } catch (RemoteException e) {
        //     Log.e(TAG, "Failed to open window session", e);
        // }
    }
    return sWindowSession;
}

AutoPtr<IWindowSession> CWindowManagerGlobal::PeekWindowSession()
{
    AutoLock lock(sDefaultWindowManagerLock);
    return sWindowSession;
}

ECode CWindowManagerGlobal::GetViewRootNames(
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names)
    AutoLock lock(mLock);

    Int32 numRoots = 0;
    mRoots->GetSize(&numRoots);

    AutoPtr< ArrayOf<String> > mViewRoots = ArrayOf<String>::Alloc(numRoots);
    for (Int32 i = 0; i < numRoots; ++i)
    {
        AutoPtr<IInterface> temp;
        mRoots->Get(i, (IInterface**)&temp);
        AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
        (*mViewRoots)[i] = GetWindowName(impl);
    }

    *names = mViewRoots;
    REFCOUNT_ADD(*names);
    return NOERROR;
}

ECode CWindowManagerGlobal::GetRootView(
    /* [in] */ const String& name,
    /* [out] */ IView** view)
{
    {
        AutoLock lock(mLock);
        Int32 numRoots;
        mRoots->GetSize(&numRoots);
        for (Int32 i = numRoots; i >= 0; --i)
        {
            AutoPtr<IInterface> temp;
            mRoots->Get(i, (IInterface**)&temp);
            AutoPtr<IViewRootImpl> root = IViewRootImpl::Probe(temp);
            String impl = GetWindowName(root);
            if (name.Equals(impl)) {
                AutoPtr<IView> result;
                root->GetView((IView**)&result);
                *view = result;
                REFCOUNT_ADD(*view);
                return NOERROR;
            }
        }
    }
    *view = NULL;
    return NOERROR;
}

ECode CWindowManagerGlobal::GetWindowManagerService(
    /* [out] */ IIWindowManager** windowManager)
{
    VALIDATE_NOT_NULL(windowManager);
    AutoPtr<IIWindowManager> temp = GetWindowManagerService();
    *windowManager = temp;
    REFCOUNT_ADD(*windowManager);

    return NOERROR;
}

ECode CWindowManagerGlobal::GetWindowSession(
    /* [in] */ ILooper* mainLooper,
    /* [out] */ IWindowSession** windowSession)
{
    VALIDATE_NOT_NULL(windowSession);
    AutoPtr<IWindowSession> temp = GetWindowSession(mainLooper);
    *windowSession = temp;
    REFCOUNT_ADD(*windowSession)

    return NOERROR;
}

ECode CWindowManagerGlobal::PeekWindowSession(
    /* [out] */ IWindowSession** windowSession)
{
    VALIDATE_NOT_NULL(windowSession);
    AutoPtr<IWindowSession> temp = PeekWindowSession();
    *windowSession = temp;
    REFCOUNT_ADD(*windowSession)

    return NOERROR;
}

ECode CWindowManagerGlobal::AddView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ IDisplay* display,
    /* [in] */ IWindow* parentWindow)
{
    if (view == NULL) {
        Logger::E(TAG, "view must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (display == NULL) {
        Logger::E(TAG, "display must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (IWindowManagerLayoutParams::Probe(params) == NULL) {
        Logger::E(TAG, "Params must be WindowManager.LayoutParams");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    CWindowManagerLayoutParams* wparams =
        (CWindowManagerLayoutParams*)IWindowManagerLayoutParams::Probe(params);
    if (parentWindow != NULL) {
        if (wparams->mType == IWindowManagerLayoutParams::TYPE_NAVIGATION_BAR) {
            // TODO: TYPE_NAVIGATION_BAR
            AutoPtr<IBinder> binder;
            view->GetWindowToken((IBinder**)&binder);
            wparams->SetToken(binder);
        }
        else {
            parentWindow->AdjustLayoutParamsForSubWindow(IWindowManagerLayoutParams::Probe(wparams));
        }
    } else {
        // If there's no parent and we're running on L or above (or in the
        // system context), assume we want hardware acceleration.
        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);

        if (context != NULL) {
            AutoPtr<IApplicationInfo> info;
            context->GetApplicationInfo((IApplicationInfo**)&info);
            Int32 version;
            info->GetTargetSdkVersion(&version);
            if (version >= Build::VERSION_CODES::LOLLIPOP) {
                wparams->mFlags |= IWindowManagerLayoutParams::FLAG_HARDWARE_ACCELERATED;
            }
        }
    }

    AutoPtr<IViewRootImpl> root;
    AutoPtr<IView> panelParentView;

    {
        AutoLock lock(mLock);

        // Start watching for system property changes.
        if (mSystemPropertyUpdater == NULL) {
            mSystemPropertyUpdater = new SystemPropertyUpdaterRunnable(this);
            //TODO:
            //
            // SystemProperties::AddChangeCallback(mSystemPropertyUpdater);
        }

        Int32 index;
        FindViewLocked(view, FALSE, &index);
        if (index >= 0) {
            Boolean contains;
            mDyingViews->Contains((IInterface*)view->Probe(EIID_IInterface), &contains);
            if (contains) {
                // Don't wait for MSG_DIE to make it's way through root's queue.
                AutoPtr<IInterface> temp;
                mRoots->Get(index, (IInterface**)&temp);
                AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
                impl->DoDie();
            } else {
                Logger::E(TAG, "View 0x%08x has already been added to the window manager.", view);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            // The previous removeView() had not completed executing. Now it has.
        }

        // If this is a panel window, then find the window it is being
        // attached to for future reference.
        if (wparams->mType >= IWindowManagerLayoutParams::FIRST_SUB_WINDOW &&
            wparams->mType <= IWindowManagerLayoutParams::LAST_SUB_WINDOW) {
            Int32 count = 0;
            mRoots->GetSize(&count);
            for (Int32 i=0; i<count; i++) {
                AutoPtr<IInterface> temp;
                mRoots->Get(i, (IInterface**)&temp);
                AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
                if (IBinder::Probe(VIEWIMPL_PROBE(impl)->mWindow.Get()) == wparams->mToken) {
                    AutoPtr<IInterface> it;
                    mViews->Get(i, (IInterface**)&it);
                    AutoPtr<IView> tempView = IView::Probe(it);
                    panelParentView = tempView;
                }
            }
        }

        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);
        assert(0);
        //root = new ViewRootImpl(context, display);

        view->SetLayoutParams(params);


        mViews->Add(view);
        mRoots->Add(root);
        mParams->Add(IWindowManagerLayoutParams::Probe(wparams));
    }

    // do this last because it fires off messages to start doing things
    ECode ec = root->SetView(view, IWindowManagerLayoutParams::Probe(wparams), panelParentView);
    if (FAILED(ec)) {
        // BadTokenException or InvalidDisplayException, clean up.
        AutoLock lock(mLock);
        Int32 index;
        FindViewLocked(view, FALSE, &index);
        if (index >= 0) {
            RemoveViewLocked(index, TRUE);
        }
    }

    return ec;
}

ECode CWindowManagerGlobal::UpdateViewLayout(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (view == NULL) {
        Logger::E(TAG, "view must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!IWindowManagerLayoutParams::Probe(params)) {
        Logger::E(TAG, "Params must be WindowManager.LayoutParams");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    IWindowManagerLayoutParams* wparams = IWindowManagerLayoutParams::Probe(params);

    view->SetLayoutParams(IViewGroupLayoutParams::Probe(wparams));

    AutoLock lock(mLock);
    Int32 index;
    FAIL_RETURN(FindViewLocked(view, TRUE, &index));

    AutoPtr<IInterface> temp;
    mRoots->Get(index, (IInterface**)&temp);
    AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
    mParams->Remove(index);
    mParams->Add(index, (IInterface*)wparams->Probe(EIID_IInterface));
    impl->SetLayoutParams(wparams, FALSE);

    return NOERROR;
}

ECode CWindowManagerGlobal::RemoveView(
    /* [in] */ IView* view,
    /* [in] */ Boolean immediate)
{
    if (view == NULL) {
        Logger::E(TAG, "view must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoLock lock(mLock);
    Int32 index;
    FAIL_RETURN(FindViewLocked(view, TRUE, &index));
    AutoPtr<IInterface> temp;
    mRoots->Get(index, (IInterface**)&temp);
    AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
    AutoPtr<IView> curView;
    impl->GetView((IView**)&curView);
    RemoveViewLocked(index, immediate);
    if (curView.Get() == view) {
        return NOERROR;
    }

    Logger::E(TAG, "Calling with view 0x%08x but the ViewAncestor"
        " is attached to 0x%08x", view, curView.Get());
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CWindowManagerGlobal::CloseAll(
    /* [in] */ IBinder* token,
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    AutoLock lock(mLock);
    if (mViews == NULL)
        return NOERROR;

    Logger::I(TAG, "Closing all windows of %p", token);

    Int32 count = 0;
    mViews->GetSize(&count);
    for (Int32 i = count - 1; i >= 0; --i) {
        //Log.i("foo", "@ " + i + " token " + (*mParams)[i].token
        //        + " view " + (*mRoots)[i].getView());
        AutoPtr<IInterface> temp;
        mParams->Get(i, (IInterface**)&temp);
        AutoPtr<IWindowManagerLayoutParams> param = IWindowManagerLayoutParams::Probe(temp);
        AutoPtr<IBinder> paramToken;
        param->GetToken((IBinder**)&paramToken);
        if (token == NULL || token == paramToken) {
            temp = NULL;
            mRoots->Get(i, (IInterface**)&temp);
            AutoPtr<IViewRootImpl> root = IViewRootImpl::Probe(temp);

            //Log.i("foo", "Force closing " + root);
            if (who != NULL) {
                // WindowLeaked leak = new WindowLeaked(
                //         what + " " + who + " has leaked window "
                //         + root.getView() + " that was originally added here");
                // leak.setStackTrace(root.getLocation().getStackTrace());
                // Log.e(TAG, leak.getMessage(), leak);
                Logger::E(TAG, "%s %s has leaked window ", what.string(), who.string());
            }

            RemoveViewLocked(i, FALSE);
        }
    }
    return NOERROR;
}

void CWindowManagerGlobal::RemoveViewLocked(
    /* [in] */ Int32 index,
    /* [in] */ Boolean immediate)
{
    AutoPtr<IInterface> temp;
    mRoots->Get(index, (IInterface**)&temp);
    AutoPtr<IViewRootImpl> root = IViewRootImpl::Probe(temp);
    AutoPtr<IView> view;
    root->GetView((IView**)&view);

    if (view != NULL) {
        AutoPtr<IContext> ctx;
        view->GetContext((IContext**)&ctx);
        assert(0);
        AutoPtr<IInputMethodManager> imm;// = CInputMethodManager::GetInstance(ctx);
        if (imm != NULL) {
            temp = NULL;
            mViews->Get(index, (IInterface**)&temp);
            AutoPtr<IView> v = IView::Probe(temp);
            AutoPtr<IBinder> binder;
            v->GetWindowToken((IBinder**)&binder);
            imm->WindowDismissed(binder);
        }
    }

    Boolean deferred;
    root->Die(immediate, &deferred);

    if (view != NULL) {
        VIEW_PROBE(view)->AssignParent(NULL);
        if (deferred) {
            mDyingViews->Add((IInterface*)view->Probe(EIID_IInterface));
        }
    }
}

ECode CWindowManagerGlobal::DoRemoveView(
    /* [in] */ IViewRootImpl* root)
{
    AutoLock lock(mLock);
    {
        Int32 index = 0;
        mRoots->IndexOf(root, &index);
        if (index >= 0) {
            mRoots->Remove(index);
            mParams->Remove(index);
            AutoPtr<IInterface> temp;
            mViews->Remove(index, (IInterface**)&temp);
            AutoPtr<IView> view = IView::Probe(temp);
            IList::Probe(mDyingViews)->Remove((IInterface*)view->Probe(EIID_IInterface));
        }
    }
    assert(0);
    /*if (HardwareRenderer::sTrimForeground && HardwareRenderer::IsAvailable()) {
        DoTrimForeground();
    }*/
    return NOERROR;
}

ECode CWindowManagerGlobal::FindViewLocked(
    /* [in] */ IView* view,
    /* [in] */ Boolean required,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 index = 0;
    mViews->IndexOf(view, &index);
    if (required && index < 0) {
        //Logger::D("view = %d not attached to window manager", view);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = index;
    return NOERROR;
}

Boolean CWindowManagerGlobal::ShouldDestroyEglContext(
    /* [in] */ Int32 trimLevel)
{
    // On low-end gfx devices we trim when memory is moderate;
    // on high-end devices we do this when low.
    if (trimLevel >= IComponentCallbacks2::TRIM_MEMORY_COMPLETE) {
        return TRUE;
    }
    if (trimLevel >= IComponentCallbacks2::TRIM_MEMORY_MODERATE
            && !CActivityManager::IsHighEndGfx()) {
        return TRUE;
    }
    return FALSE;
}

ECode CWindowManagerGlobal::TrimMemory(
    /* [in] */ Int32 level)
{
    assert(0);
    //if (HardwareRenderer::IsAvailable()) {
        if (ShouldDestroyEglContext(level)) {
            // Destroy all hardware surfaces and resources associated to
            // known windows
            AutoLock lock(mLock);
            {
                Int32 count;
                mRoots->GetSize(&count);
                for (int i = count - 1; i >= 0; --i) {
                    AutoPtr<IInterface> temp;
                    mRoots->Get(i, (IInterface**)&temp);
                    AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
                    impl->DestroyHardwareResources();
                }
            }
            // Force a full memory flush
            level = IComponentCallbacks2::TRIM_MEMORY_COMPLETE;
        }

        assert(0);
        /*HardwareRenderer::TrimMemory(level);

        if (HardwareRenderer::sTrimForeground) {
            DoTrimForeground();
        }*/
    //}
    return NOERROR;
}

void CWindowManagerGlobal::TrimForeground()
{
    assert(0);
    //if (HardwareRenderer::sTrimForeground && HardwareRenderer::IsAvailable()) {
        AutoPtr<IWindowManagerGlobal> wm = GetInstance();
        ((CWindowManagerGlobal*)wm.Get())->DoTrimForeground();
    //}
}

void CWindowManagerGlobal::DoTrimForeground()
{
    Boolean hasVisibleWindows = FALSE;
    AutoLock lock(mLock);
    {
        Int32 size;
        mRoots->GetSize(&size);
        for (Int32 i = size - 1; i >= 0; --i) {
            /*AutoPtr<IInterface> temp;
            mRoots->Get(i, (IInterface**)&temp);
            AutoPtr<IViewRootImpl> root = IViewRootImpl::Probe(temp);
            Int32 visiblity;
            root->GetHostVisibility(&visiblity);
            ViewRootImpl* impl = (ViewRootImpl*)root.Get();
            if (NULL != impl->mView && visiblity == IView::VISIBLE
                && NULL != impl->mAttachInfo->mHardwareRenderer) {
                hasVisibleWindows = TRUE;
            } else {
                root->DestroyHardwareResources();
            }*/
        }
    }

    if (!hasVisibleWindows) {
        assert(0);
        /*HardwareRenderer::TrimMemory(
                IComponentCallbacks2::TRIM_MEMORY_COMPLETE);*/
    }
}

ECode CWindowManagerGlobal::DumpGfxInfo(
    /* [in] */ IFileDescriptor* fd)
{
    // FileOutputStream fout = new FileOutputStream(fd);
    // PrintWriter pw = new FastPrintWriter(fout);
    // try {
    //     AutoLock lock(mLock);
    //         if (mViews != NULL) {
    //             final Int32 count = mViews->GetSize();

    //             pw.println("Profile data in ms:");

    //             for (Int32 i = 0; i < count; i++) {
    //                 ViewRootImpl root = (*mRoots)[i];
    //                 String name = getWindowName(root);
    //                 pw.printf("\n\t%s", name);

    //                 HardwareRenderer renderer =
    //                         root.getView().mAttachInfo.mHardwareRenderer;
    //                 if (renderer != NULL) {
    //                     renderer.dumpGfxInfo(pw);
    //                 }
    //             }

    //             pw.println("\nView hierarchy:\n");

    //             Int32 viewsCount = 0;
    //             Int32 displayListsSize = 0;
    //             Int32[] info = new Int32[2];

    //             for (Int32 i = 0; i < count; i++) {
    //                 ViewRootImpl root = (*mRoots)[i];
    //                 root.dumpGfxInfo(info);

    //                 String name = getWindowName(root);
    //                 pw.printf("  %s\n  %d views, %.2f kB of display lists",
    //                         name, info[0], info[1] / 1024.0f);
    //                 HardwareRenderer renderer =
    //                         root.getView().mAttachInfo.mHardwareRenderer;
    //                 if (renderer != NULL) {
    //                     pw.printf(", %d frames rendered", renderer.getFrameCount());
    //                 }
    //                 pw.printf("\n\n");

    //                 viewsCount += info[0];
    //                 displayListsSize += info[1];
    //             }

    //             pw.printf("\nTotal ViewRootImpl: %d\n", count);
    //             pw.printf("Total Views:        %d\n", viewsCount);
    //             pw.printf("Total DisplayList:  %.2f kB\n\n", displayListsSize / 1024.0f);
    //         }
    //     }
    // } finally {
    //     pw.flush();
    // }
    return NOERROR;
}

String CWindowManagerGlobal::GetWindowName(
    /* [in] */ IViewRootImpl* root)
{
    AutoPtr<ICharSequence> title;
    VIEWIMPL_PROBE(root)->mWindowAttributes->GetTitle((ICharSequence**)&title);
    String str;
    title->ToString(&str);
    Int32 code;
    IObject::Probe(root)->GetHashCode(&code);
    return str + String("/ViewRootImpl@") + StringUtils::ToString(code);
}

ECode CWindowManagerGlobal::SetStoppedState(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean stopped)
{
    AutoLock lock(mLock);
    if (mViews != NULL) {
        Int32 count = 0;
        mViews->GetSize(&count);
        for (Int32 i=0; i < count; i++) {
            AutoPtr<IInterface> temp;
            mParams->Get(i, (IInterface**)&temp);
            AutoPtr<IWindowManagerLayoutParams> param = IWindowManagerLayoutParams::Probe(temp);
            AutoPtr<IBinder> paramToken;
            param->GetToken((IBinder**)&paramToken);
            if (token == NULL || token == paramToken) {
                temp = NULL;
                mRoots->Get(i, (IInterface**)&temp);
                AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
                impl->SetStopped(stopped);
            }
        }
    }

    return NOERROR;
}

ECode CWindowManagerGlobal::ReportNewConfiguration(
    /* [in] */ IConfiguration* config)
{
    AutoLock lock(mLock);
    if (mViews != NULL) {
        Int32 count = 0;
        mViews->GetSize(&count);
        AutoPtr<IConfiguration> newConfig;
        CConfiguration::New(config, (IConfiguration**)&newConfig);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IInterface> temp;
            mRoots->Get(i, (IInterface**)&temp);
            AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
            impl->RequestUpdateConfiguration(newConfig);
        }
    }

    return NOERROR;
}

/** @hide */
ECode CWindowManagerGlobal::ChangeCanvasOpacity(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean opaque)
{
    if (token == NULL) {
        return NOERROR;
    }
    AutoLock lock(mLock);
    {
        Int32 count;
        mParams->GetSize(&count);
        for (Int32 i = count - 1; i >= 0; --i) {
            AutoPtr<IInterface> temp;
            mParams->Get(i, (IInterface**)&temp);
            AutoPtr<IWindowManagerLayoutParams> param = IWindowManagerLayoutParams::Probe(temp);
            CWindowManagerLayoutParams* wm = (CWindowManagerLayoutParams*)param.Get();
            if (token == wm->mToken) {
                temp = NULL;
                mRoots->Get(i, (IInterface**)&temp);
                AutoPtr<IViewRootImpl> impl = IViewRootImpl::Probe(temp);
                impl->ChangeCanvasOpacity(opaque);
                return NOERROR;
            }
        }
    }
    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
