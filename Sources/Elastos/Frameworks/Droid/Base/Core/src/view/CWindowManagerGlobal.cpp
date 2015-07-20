
#include "view/CWindowManagerGlobal.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/ViewRootImpl.h"
#include "view/inputmethod/CInputMethodManager.h"
#include "content/res/CConfiguration.h"
#include "os/CServiceManager.h"
#include "os/SystemProperties.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>
#include "animation/CValueAnimator.h"

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Internal::View::IInputMethodClient;
using Elastos::Droid::Internal::View::IInputContext;
using Elastos::Droid::Animation::CValueAnimator;

namespace Elastos {
namespace Droid {
namespace View {

const char* CWindowManagerGlobal::TAG = "WindowManager";

AutoPtr<IWindowManagerGlobal> CWindowManagerGlobal::sDefaultWindowManager;
Mutex CWindowManagerGlobal::sDefaultWindowManagerLock;
AutoPtr<IIWindowManager> CWindowManagerGlobal::sWindowManagerService;
AutoPtr<IWindowSession> CWindowManagerGlobal::sWindowSession;

CWindowManagerGlobal::SystemPropertyUpdaterRunnable::SystemPropertyUpdaterRunnable(
    /* [in] */ CWindowManagerGlobal* owner)
    : mOwner(owner)
{}

ECode CWindowManagerGlobal::SystemPropertyUpdaterRunnable::Run()
{
    AutoLock lock(mOwner->mLock);
    Int32 count = mOwner->mRoots->GetLength();
    for (Int32 i = 0; i < count; i++) {
        (*mOwner->mRoots)[i]->LoadSystemProperties();
    }

    return NOERROR;
}

CWindowManagerGlobal::CWindowManagerGlobal()
    : mNeedsEglTerminate(FALSE)
{
}

AutoPtr<IWindowManagerGlobal> CWindowManagerGlobal::GetInstance()
{
    AutoLock lock(sDefaultWindowManagerLock);

    if (sDefaultWindowManager == NULL) {
        CWindowManagerGlobal::AcquireSingleton((IWindowManagerGlobal**)&sDefaultWindowManager);
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
        AutoPtr<IInputMethodManager> imm = CInputMethodManager::GetInstance(mainLooper);
        AutoPtr<IIWindowManager> wm = GetWindowManagerService();
        AutoPtr<IInputMethodClient> client;
        imm->GetClient((IInputMethodClient**)&client);
        AutoPtr<IInputContext> ctx;
        imm->GetInputContext((IInputContext**)&ctx);
        wm->OpenSession(client, ctx, (IWindowSession**)&sWindowSession);
        Float animatorScale;
        wm->GetAnimationScale(2, &animatorScale);
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
            parentWindow->AdjustLayoutParamsForSubWindow(wparams);
        }
    }

    AutoPtr<ViewRootImpl> root;
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
            Logger::E(TAG, "View 0x%08x has already been added to the window manager.", view);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        // If this is a panel window, then find the window it is being
        // attached to for future reference.
        if (wparams->mType >= IWindowManagerLayoutParams::FIRST_SUB_WINDOW &&
            wparams->mType <= IWindowManagerLayoutParams::LAST_SUB_WINDOW) {
            Int32 count = mViews != NULL ? mViews->GetLength() : 0;
            for (Int32 i=0; i<count; i++) {
                if (IBinder::Probe((*mRoots)[i]->mWindow.Get()) == wparams->mToken) {
                    panelParentView = (*mViews)[i];
                }
            }
        }

        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);
        root = new ViewRootImpl(context, display);

        view->SetLayoutParams(params);

        if (mViews == NULL) {
            index = 1;
            mViews = ArrayOf<IView*>::Alloc(1);
            mRoots = ArrayOf<ViewRootImpl*>::Alloc(1);
            mParams = ArrayOf<IWindowManagerLayoutParams*>::Alloc(1);
            index--;
        }
        else {
            index = mViews->GetLength() + 1;
            AutoPtr<ArrayOf<IView*> > oldViews = mViews;
            mViews = ArrayOf<IView*>::Alloc(index);
            AutoPtr<ArrayOf<ViewRootImpl*> > oldRoots = mRoots;
            mRoots = ArrayOf<ViewRootImpl*>::Alloc(index);
            AutoPtr<ArrayOf<IWindowManagerLayoutParams*> > oldParams = mParams;
            mParams = ArrayOf<IWindowManagerLayoutParams*>::Alloc(index);

            index--;
            mViews->Copy(oldViews);
            mRoots->Copy(oldRoots);
            mParams->Copy(oldParams);
        }

        mViews->Set(index, view);
        mRoots->Set(index, root);
        mParams->Set(index, wparams);
    }

    // do this last because it fires off messages to start doing things
    ECode ec = root->SetView(view, wparams, panelParentView);
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

    view->SetLayoutParams(wparams);

    AutoLock lock(mLock);
    Int32 index;
    FAIL_RETURN(FindViewLocked(view, TRUE, &index));
    mParams->Set(index, wparams);
    (*mRoots)[index]->SetLayoutParams(wparams, FALSE);

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
    AutoPtr<IView> curView = RemoveViewLocked(index, immediate);
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

    Int32 count = mViews->GetLength();
    for (Int32 i = count - 1; i >= 0; --i) {
        //Log.i("foo", "@ " + i + " token " + (*mParams)[i].token
        //        + " view " + (*mRoots)[i].getView());
        AutoPtr<IBinder> paramToken;
        if (token == NULL || ((*mParams)[i]->GetToken((IBinder**)&paramToken),
            paramToken.Get()) == token) {
            AutoPtr<ViewRootImpl> root = (*mRoots)[i];

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

AutoPtr<IView> CWindowManagerGlobal::RemoveViewLocked(
    /* [in] */ Int32 index,
    /* [in] */ Boolean immediate)
{
    AutoPtr<ViewRootImpl> root = (*mRoots)[index];
    AutoPtr<IView> view = root->GetView();

    if (view != NULL) {
        AutoPtr<IContext> ctx;
        view->GetContext((IContext**)&ctx);

        AutoPtr<IInputMethodManager> imm = CInputMethodManager::GetInstance(ctx);
        if (imm != NULL) {
            AutoPtr<IBinder> binder;
            (*mViews)[index]->GetWindowToken((IBinder**)&binder);
            imm->WindowDismissed(binder);
        }
    }

    root->Die(immediate);

    Int32 count = mViews->GetLength() - 1;

    // remove it from the list
    AutoPtr<ArrayOf<IView*> > tmpViews = ArrayOf<IView*>::Alloc(count);
    AutoPtr<ArrayOf<ViewRootImpl*> > tmpRoots = ArrayOf<ViewRootImpl*>::Alloc(count);
    AutoPtr<ArrayOf<IWindowManagerLayoutParams*> > tmpParams
        = ArrayOf<IWindowManagerLayoutParams*>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        if (i < index) {
            tmpViews->Set(i, (*mViews)[i]);
            tmpRoots->Set(i, (*mRoots)[i]);
            tmpParams->Set(i, (*mParams)[i]);
        }
        else if (i >= index) {
            tmpViews->Set(i, (*mViews)[i + 1]);
            tmpRoots->Set(i, (*mRoots)[i + 1]);
            tmpParams->Set(i, (*mParams)[i + 1]);
        }
    }
    mViews = tmpViews;
    mRoots = tmpRoots;
    mParams = tmpParams;
    if (view != NULL) {
        view->AssignParent(NULL);
        // func doesn't allow NULL...  does it matter if we clear them?
        //view.setLayoutParams(NULL);
    }
    return view;
}

ECode CWindowManagerGlobal::FindViewLocked(
    /* [in] */ IView* view,
    /* [in] */ Boolean required,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;

    if (mViews != NULL) {
        Int32 count = mViews->GetLength();
        for (Int32 i = 0; i < count; i++) {
            if ((*mViews)[i] == view) {
                *result = i;
                return NOERROR;
            }
        }
    }

    if (required) {
        Logger::E(TAG, "View not attached to window manager");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CWindowManagerGlobal::StartTrimMemory(
    /* [in] */ Int32 level)
{
    //TODO:
    //
    // if (HardwareRenderer::IsAvailable()) {
    //     // On low-end gfx devices we trim when memory is moderate;
    //     // on high-end devices we do this when low.
    //     AutoPtr<IActivityManagerHelper> amHelper;
    //     CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
    //     Boolean isHighEndGfx;
    //     amHelper->IsHighEndGfx(&isHighEndGfx);
    //     if (level >= IComponentCallbacks2::TRIM_MEMORY_COMPLETE
    //         || (level >= IComponentCallbacks2::TRIM_MEMORY_MODERATE
    //         && !isHighEndGfx)) {
    //         // Destroy all hardware surfaces and resources associated to
    //         // known windows
    //         {
    //             AutoLock lock(mLock);
    //             if (mViews == NULL)
    //                 return NOERROR;
    //             Int32 count = mViews->GetLength();
    //             for (Int32 i = 0; i < count; i++) {
    //                 (*mRoots)[i].TerminateHardwareResources();
    //             }
    //         }
    //         // Force a full memory flush
    //         mNeedsEglTerminate = TRUE;
    //         HardwareRenderer::StartTrimMemory(IComponentCallbacks2::TRIM_MEMORY_COMPLETE);
    //         return NOERROR;
    //     }

    //     HardwareRenderer::StartTrimMemory(level);
    // }
    return NOERROR;
}

ECode CWindowManagerGlobal::EndTrimMemory()
{
    // HardwareRenderer::EndTrimMemory();

    // if (mNeedsEglTerminate) {
    //     ManagedEGLContext::DoTerminate();
    //     mNeedsEglTerminate = FALSE;
    // }

    return NOERROR;
}

ECode CWindowManagerGlobal::TrimLocalMemory()
{
    AutoLock lock(mLock);
    if (mViews == NULL)
        return NOERROR;
    Int32 count = mViews->GetLength();
    for (Int32 i = 0; i < count; i++) {
        (*mRoots)[i]->DestroyHardwareLayers();
    }
    return NOERROR;
}

ECode CWindowManagerGlobal::DumpGfxInfo(
    /* [in] */ IFileDescriptor* fd)
{
    // FileOutputStream fout = new FileOutputStream(fd);
    // PrintWriter pw = new PrintWriter(fout);
    // try {
    //     AutoLock lock(mLock);
    //         if (mViews != NULL) {
    //             final Int32 count = mViews->GetLength();

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
    /* [in] */ ViewRootImpl* root)
{
    AutoPtr<ICharSequence> title;
    root->mWindowAttributes->GetTitle((ICharSequence**)&title);
    String str;
    title->ToString(&str);
    return str + "/ViewRootImpl@" + StringUtils::Int32ToString((Int32)root);
}

ECode CWindowManagerGlobal::SetStoppedState(
    /* [in] */ IBinder* token,
    /* [in] */ Boolean stopped)
{
    AutoLock lock(mLock);
    if (mViews != NULL) {
        Int32 count = mViews->GetLength();
        for (Int32 i=0; i < count; i++) {
            AutoPtr<IBinder> paramToken;
            if (token == NULL || ((*mParams)[i]->GetToken((IBinder**)&paramToken),
                paramToken.Get()) == token) {
                (*mRoots)[i]->SetStopped(stopped);
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
        Int32 count = mViews->GetLength();
        AutoPtr<IConfiguration> newConfig;
        CConfiguration::New(config, (IConfiguration**)&newConfig);
        for (Int32 i = 0; i < count; i++) {
            (*mRoots)[i]->RequestUpdateConfiguration(newConfig);
        }
    }

    return NOERROR;
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
