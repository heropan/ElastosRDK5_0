
#include "app/Activity.h"
#include "app/Fragment.h"
#include "app/ActivityManagerNative.h"
#ifdef DROID_CORE
//#include "app/CSearchManager.h"
#include "os/CBundle.h"
#include "os/CHandler.h"
#include "os/CUserHandle.h"
#include "os/Looper.h"
#include "view/CWindowManagerGlobal.h"
#include "view/CMenuInflater.h"
#include "text/CSpannableStringBuilder.h"
#include "text/method/CTextKeyListenerHelper.h"
#include "content/CIntent.h"
#include "content/CIntentHelper.h"
#include "content/CComponentName.h"
#include "content/res/CResourcesHelper.h"
#include "content/res/CConfiguration.h"
#include "app/CInstrumentationHelper.h"
#include "app/CFragmentManagerImpl.h"
#include "app/CFragmentManagerImplHelper.h"
#include "app/CApplication.h"
#include "app/CActionBarImpl.h"
#include "app/CPendingIntent.h"
#include "app/CTaskStackBuilderHelper.h"
#include "app/CActivityNonConfigurationInstances.h"
#include "impl/CPolicyManager.h"
#include "net/CUriHelper.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include "text/TextUtils.h"
#include "os/Build.h"
#include "R.h"
#include <elastos/core/StringBuffer.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/Thread.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/logging/Logger.h>
#include <Elastos.CoreLibrary.h>
#include <Elastos.CoreLibrary.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::IRunnable;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Core::Thread;
using Elastos::Utility::CObjectStringMap;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Os::IServiceManager;
#ifdef DROID_CORE
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Os::Looper;
#else
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;
#endif
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::Build;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::CWindowManagerGlobal;
using Elastos::Droid::View::CMenuInflater;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::CSpannableStringBuilder;
using Elastos::Droid::Text::Method::ITextKeyListenerHelper;
using Elastos::Droid::Text::Method::CTextKeyListenerHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentHelper;
using Elastos::Droid::Content::CIntentHelper;
using Elastos::Droid::Content::IIIntentSender;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::EIID_IContextWrapper;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::CConfiguration;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Text::ISelectionHelper;
//using Elastos::Droid::Text::CSelectionHelper;
using Elastos::Droid::Text::Method::ITextKeyListener;
using Elastos::Droid::Text::Method::IKeyListener;
using Elastos::Droid::Text::Method::EIID_IKeyListener;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IWindowManagerGlobal;
using Elastos::Droid::View::EIID_IWindowCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::EIID_IContextThemeWrapper;
using Elastos::Droid::View::EIID_IViewOnCreateContextMenuListener;
using Elastos::Droid::View::EIID_ILayoutInflaterFactory2;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::ITaskStackBuilder;
using Elastos::Droid::App::CTaskStackBuilderHelper;
using Elastos::Droid::App::IInstrumentationActivityResult;
using Elastos::Droid::App::CActivityNonConfigurationInstances;
using Elastos::Droid::App::Fragment;
using Elastos::Droid::App::IFragment;
using Elastos::Droid::App::IFragmentManagerImpl;
using Elastos::Droid::App::CFragmentManagerImpl;
using Elastos::Droid::App::IFragmentManagerImplHelper;
using Elastos::Droid::App::CFragmentManagerImplHelper;
using Elastos::Droid::App::ITaskStackBuilderHelper;
using Elastos::Droid::Internal::App::CActionBarImpl;
using Elastos::Droid::Internal::Policy::IPolicyManager;
using Elastos::Droid::Internal::Policy::CPolicyManager;
using Elastos::Droid::Text::Method::IKeyListener;
using Elastos::Droid::Text::Method::EIID_IKeyListener;

namespace Elastos {
namespace Droid {
namespace App {

// e906c772-090f-4ae6-881d-2d430a060cfe
extern "C" const InterfaceID EIID_Activity =
        { 0xe906c772, 0x090f, 0x4ae6, { 0x88, 0x1d, 0x2d, 0x43, 0x0a, 0x06, 0x0c, 0xfe } };


class FragmentContainerLocal
    : public ElRefBase
    , public IFragmentContainer
{
public:
    FragmentContainerLocal(
        /* [in] */ Activity* host)
        : mHost(host)
    {}

    PInterface Probe(
        /* [in]  */ REIID riid)
    {
        if (riid == EIID_IInterface) {
            return (PInterface)this;
        }
        else if (riid == EIID_IFragmentContainer) {
            return (IFragmentContainer*)this;
        }

        return NULL;

    }

    UInt32 AddRef()
    {
        return ElRefBase::AddRef();
    }

    UInt32 Release()
    {
        return ElRefBase::Release();
    }

    ECode GetInterfaceID(
        /* [in] */ IInterface *Object,
        /* [out] */ InterfaceID *IID)
    {
        VALIDATE_NOT_NULL(IID);

        if (Object == (IInterface*)(IFragmentContainer*)this) {
            *IID = EIID_IFragmentContainer;
            return NOERROR;
        }

        return E_INVALID_ARGUMENT;
    }

    ECode FindViewById(
        /* [in] */ Int32 id,
        /* [out] */ IView** view)
    {
        return mHost->FindViewById(id, view);
    }

private:
    Activity* mHost;
};

const String Activity::TAG("Activity");
const Boolean Activity::DEBUG_LIFECYCLE = FALSE;
const String Activity::FRAGMENTS_TAG("android:fragments");

const String Activity::WINDOW_HIERARCHY_TAG("android:viewHierarchyState");
const String Activity::SAVED_DIALOG_IDS_KEY("android:savedDialogIds");
const String Activity::SAVED_DIALOGS_TAG("android:savedDialogs");
const String Activity::SAVED_DIALOG_KEY_PREFIX("android:dialog_");
const String Activity::SAVED_DIALOG_ARGS_KEY_PREFIX("android:dialog_args_");

Activity::Activity()
    : mEmbeddedID(String(NULL))
    , mIntent(NULL)
    , mActivityInfo(NULL)
    , mMainThread(NULL)
    , mParent(NULL)
    , mCalled(FALSE)
    , mCheckedForLoaderManager(FALSE)
    , mLoadersStarted(FALSE)
    , mResumed(FALSE)
    , mStopped(FALSE)
    , mFinished(FALSE)
    , mStartedActivity(FALSE)
    , mTemporaryPause(FALSE)
    , mChangingConfigurations(FALSE)
    , mConfigChangeFlags(0)
    , mWindowAdded(FALSE)
    , mVisibleFromServer(FALSE)
    , mVisibleFromClient(TRUE)
    , mAllLoaderManagers(0)
    , mResultCode(IActivity::RESULT_CANCELED)
    , mIdent(0)
    , mDestroyed(FALSE)
    , mEnableDefaultActionBarUp(FALSE)
    , mTitle(NULL)
    , mTitleColor(0)
    , mTitleReady(FALSE)
    , mDefaultKeyMode(IActivity::DEFAULT_KEYS_DISABLE)
    , mThemeResource(0)
{
    CFragmentManagerImpl::New((IFragmentManagerImpl**)&mFragments);
    mContainer = new FragmentContainerLocal(this);
    CHandler::New((IHandler**)&mHandler);
}

Activity::~Activity()
{
    mInflater = NULL;
    mManagedDialogs.Clear();
    mManagedCursors.Clear();
}

ECode Activity::Initialize()
{
    return NOERROR;
}

PInterface Activity::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IActivity*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IActivity) {
        return (IActivity*)this;
    }
    else if (riid == EIID_IContextThemeWrapper) {
        return (IContextThemeWrapper*)this;
    }
    else if (riid == EIID_IContextWrapper) {
        return (IContextWrapper*)this;
    }
    else if (riid == EIID_IContext) {
        return (IContext*)this;
    }
    else if (riid == EIID_ILayoutInflaterFactory2) {
        return (ILayoutInflaterFactory2*)this;
    }
    else if (riid == EIID_IWindowCallback) {
        return (IWindowCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }
    else if (riid == EIID_IViewOnCreateContextMenuListener) {
        return (IViewOnCreateContextMenuListener*)this;
    }
    else if (riid == EIID_IComponentCallbacks2) {
        return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_Activity) {
        return reinterpret_cast<PInterface>(this);
    }
    return NULL;
}

UInt32 Activity::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Activity::Release()
{
    return ElRefBase::Release();
}

ECode Activity::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IActivity *)this) {
        *pIID = EIID_IActivity;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IContextThemeWrapper *)this) {
        *pIID = EIID_IContextThemeWrapper;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IContextWrapper *)this) {
        *pIID = EIID_IContextWrapper;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IContext *)this) {
        *pIID = EIID_IContext;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(ILayoutInflaterFactory2 *)this) {
        *pIID = EIID_ILayoutInflaterFactory2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWindowCallback *)this) {
        *pIID = EIID_IWindowCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IKeyEventCallback *)this) {
        *pIID = EIID_IKeyEventCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IViewOnCreateContextMenuListener *)this) {
        *pIID = EIID_IViewOnCreateContextMenuListener;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IComponentCallbacks2 *)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWeakReferenceSource *)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Activity::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Activity::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Activity::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IActivity * o = IActivity::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IActivity));
    }
    return NOERROR;
}

ECode Activity::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IActivity);
    return NOERROR;
}

ECode Activity::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = ToString();
    return NOERROR;
}

String Activity::ToString()
{
    StringBuilder sb("Activity:(");
    sb += mEmbeddedID;
    sb += ")";
    return sb.ToString();
}

ECode Activity::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode Activity::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);

    *intent = mIntent;
    REFCOUNT_ADD(*intent)
    return NOERROR;
}

ECode Activity::SetIntent(
    /* [in] */ IIntent* newIntent)
{
    mIntent = newIntent;
    return NOERROR;
}

ECode Activity::GetApplication(
    /* [out] */ IApplication** application)
{
    VALIDATE_NOT_NULL(application);

    *application = mApplication;
    REFCOUNT_ADD(*application)

    return NOERROR;
}

AutoPtr<IApplication> Activity::GetApplication()
{
    return mApplication;
}

ECode Activity::IsChild(
    /* [out] */ Boolean* isChild)
{
    VALIDATE_NOT_NULL(isChild);
    *isChild = mParent != NULL;
    return NOERROR;
}

ECode Activity::GetParent(
    /* [out] */ IActivity** parent)
{
    VALIDATE_NOT_NULL(parent);
    *parent = mParent.Get();
    REFCOUNT_ADD(*parent);
    return NOERROR;
}

ECode Activity::GetWindowManager(
    /* [out] */ IWindowManager** mgr)
{
    VALIDATE_NOT_NULL(mgr);
    *mgr = mWindowManager.Get();
    REFCOUNT_ADD(*mgr);
    return NOERROR;
}

AutoPtr<IWindowManager> Activity::GetWindowManager()
{
    return mWindowManager;
}

ECode Activity::GetWindow(
    /* [out] */ IWindow** window)
{
    VALIDATE_NOT_NULL(window);
    *window = mWindow.Get();
    REFCOUNT_ADD(*window);
    return NOERROR;
}

AutoPtr<IWindow> Activity::GetWindow()
{
    return mWindow;
}

ECode Activity::GetLoaderManager(
    /* [out] */ ILoaderManager** manager)
{
    VALIDATE_NOT_NULL(manager);

    if (mLoaderManager == NULL) {
        mCheckedForLoaderManager = TRUE;
        String nullStr;
        mLoaderManager = GetLoaderManager(nullStr, mLoadersStarted, TRUE);
    }

    *manager = mLoaderManager.Get();
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

AutoPtr<ILoaderManagerImpl> Activity::GetLoaderManager(
    /* [in] */ const String& who,
    /* [in] */ Boolean started,
    /* [in] */ Boolean create)
{
    if (mAllLoaderManagers == NULL) {
        ECode ec = CObjectStringMap::New((IObjectStringMap**)&mAllLoaderManagers);
        if (FAILED(ec))
            return NULL;
    }

    AutoPtr<ILoaderManagerImpl> lm;
    mAllLoaderManagers->Get(who, (IInterface**)&lm);
    if (lm == NULL) {
        if (create) {
            // TODO
//            CLoaderManagerImpl::New(who,
//                    THIS_PROBE(IActivity), started, (ILoaderManagerImpl**)&lm);
//            mAllLoaderManagers->Put(who, (IInterface*)lm.Get());
        }
    } else {
        lm->UpdateActivity(THIS_PROBE(IActivity));
    }

    return lm;
}

ECode Activity::GetCurrentFocus(
    /* [out] */ IView** focus)
{
    VALIDATE_NOT_NULL(focus);
    *focus = NULL;
    if (mWindow != NULL) {
        return mWindow->GetCurrentFocus(focus);
    }
    return NOERROR;
}

ECode Activity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onCreate ");
        sb.Append(ToString());
        String temp;
        savedInstanceState->ToString(&temp);
        sb.Append(temp);
        Slogger::V(TAG, sb.ToString());
    }

    if (mLastNonConfigurationInstances != NULL) {
        mLastNonConfigurationInstances->GetLoaders((IObjectStringMap**)&mAllLoaderManagers);
    }

    String parentActivityName;
    FAIL_RETURN(mActivityInfo->GetParentActivityName(&parentActivityName));
    if (!parentActivityName.IsNull()) {
        if (mActionBar == NULL) {
            mEnableDefaultActionBarUp = TRUE;
        }
        else {
            FAIL_RETURN(mActionBar->SetDefaultDisplayHomeAsUpEnabled(TRUE));
        }
    }
    if (savedInstanceState != NULL) {
        AutoPtr<IParcelable> p;
        savedInstanceState->GetParcelable(FRAGMENTS_TAG, (IParcelable**)&p);

        if (mLastNonConfigurationInstances == NULL) {
            FAIL_RETURN(mFragments->RestoreAllState(p, NULL));
        }
        else {
            AutoPtr<IObjectContainer> fragments;
            mLastNonConfigurationInstances->GetFragments((IObjectContainer**)&fragments);
            FAIL_RETURN(mFragments->RestoreAllState(p, fragments));
        }
    }
    mFragments->DispatchCreate();

    AutoPtr<IApplication> app = GetApplication();
    assert(app != NULL);
    FAIL_RETURN(app->DispatchActivityCreated(THIS_PROBE(IActivity), savedInstanceState));

    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::PerformRestoreInstanceState(
    /* [in] */ IBundle* savedInstanceState)
{
    VALIDATE_NOT_NULL(savedInstanceState);
    FAIL_RETURN(OnRestoreInstanceState(savedInstanceState));
    return RestoreManagedDialogs(savedInstanceState);
}

ECode Activity::OnRestoreInstanceState(
    /* [in] */ IBundle* savedInstanceState)
{
    VALIDATE_NOT_NULL(savedInstanceState);
    if (mWindow != NULL) {
        AutoPtr<IBundle> windowState;
        savedInstanceState->GetBundle(WINDOW_HIERARCHY_TAG, (IBundle**)&windowState);
        if (windowState != NULL) {
            mWindow->RestoreHierarchyState(windowState);
        }
    }
    return NOERROR;
}

ECode Activity::RestoreManagedDialogs(
    /* [in] */ IBundle* savedInstanceState)
{
    VALIDATE_NOT_NULL(savedInstanceState);
    AutoPtr<IBundle> b;
    savedInstanceState->GetBundle(SAVED_DIALOGS_TAG, (IBundle**)&b);
    if (b == NULL) {
        return NOERROR;
    }

    AutoPtr< ArrayOf<Int32> > ids;
    b->GetInt32Array(SAVED_DIALOG_IDS_KEY, (ArrayOf<Int32>**)&ids);
    Int32 numDialogs = ids->GetLength();

    mManagedDialogs.Clear();

    for (Int32 i = 0; i < numDialogs; ++i) {
        Int32 dialogId = (*ids)[i];

        AutoPtr<IBundle> dialogState;
        b->GetBundle(SavedDialogKeyFor(dialogId), (IBundle**)&dialogState);
        if (dialogState != NULL) {
            // Calling onRestoreInstanceState() below will invoke dispatchOnCreate
            // so tell createDialog() not to do it, otherwise we get an exception
            AutoPtr<ManagedDialog> md = new ManagedDialog();
            b->GetBundle(SavedDialogArgsKeyFor(dialogId), (IBundle**)&md->mArgs);
            md->mDialog = CreateDialog(dialogId, dialogState, md->mArgs);
            if (md->mDialog != NULL) {
                mManagedDialogs.Insert(HashMap<Int32, AutoPtr<ManagedDialog> >::ValueType(dialogId, md));
                OnPrepareDialog(dialogId, md->mDialog, md->mArgs);
                md->mDialog->OnRestoreInstanceState(dialogState);
            }
        }
    }
    return NOERROR;
}

AutoPtr<IDialog> Activity::CreateDialog(
    /* [in] */ Int32 dialogId,
    /* [in] */ IBundle* state,
    /* [in] */ IBundle* args)
{
    AutoPtr<IDialog> dialog = OnCreateDialog(dialogId, args);
    if (dialog != NULL) {
        dialog->DispatchOnCreate(state);
    }
    return dialog;
}

String Activity::SavedDialogKeyFor(
    /* [in] */ Int32 key)
{
    StringBuilder sb(SAVED_DIALOG_KEY_PREFIX);
    sb.Append(key);
    return sb.ToString();
}

String Activity::SavedDialogArgsKeyFor(
        /* [in] */ Int32 key)
{
    StringBuilder sb(SAVED_DIALOG_ARGS_KEY_PREFIX);
    sb.Append(key);
    return sb.ToString();
}

ECode Activity::OnPostCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Boolean isChild;
    IsChild(&isChild);
    if (!isChild) {
        mTitleReady = TRUE;
        AutoPtr<ICharSequence> title;
        Int32 titleColor;
        GetTitle((ICharSequence**)&title);
        GetTitleColor(&titleColor);
        OnTitleChanged(title, titleColor);
    }
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnStart()
{
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onStart ");
        sb.Append(ToString());
        Slogger::V(TAG, sb.ToString());
    }
    mCalled = TRUE;

    if (!mLoadersStarted) {
        mLoadersStarted = TRUE;
        if (mLoaderManager != NULL) {
            mLoaderManager->DoStart();
        } else if (!mCheckedForLoaderManager) {
            String nullStr;
            mLoaderManager = GetLoaderManager(nullStr, mLoadersStarted, FALSE);
        }
        mCheckedForLoaderManager = TRUE;
    }

    AutoPtr<IApplication> app = GetApplication();
    return app->DispatchActivityStarted(THIS_PROBE(IActivity));
}

ECode Activity::OnRestart()
{
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnResume()
{
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onResume ");
        sb.Append(ToString());
        Slogger::V(TAG, sb.ToString());
    }
    AutoPtr<IApplication> app = GetApplication();
    FAIL_RETURN(app->DispatchActivityResumed(THIS_PROBE(IActivity)));
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnPostResume()
{
    AutoPtr<IWindow> win = GetWindow();
    if (win != NULL) {
        FAIL_RETURN(win->MakeActive());
    }
    if (mActionBar != NULL) {
        FAIL_RETURN(mActionBar->SetShowHideAnimationEnabled(TRUE));
    }
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnNewIntent(
    /* [in] */ IIntent *intent)
{
    return NOERROR;
}

ECode Activity::PerformSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    FAIL_RETURN(OnSaveInstanceState(outState));
    FAIL_RETURN(SaveManagedDialogs(outState));
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onSaveInstanceState ");
        sb.Append(ToString());
        sb.Append(": ");
        String temp;
        outState->ToString(&temp);
        sb.Append(temp);
        Slogger::V(TAG, sb.ToString());
    }
    return NOERROR;
}

ECode Activity::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    VALIDATE_NOT_NULL(outState);
    AutoPtr<IBundle> bundle;
    FAIL_RETURN(mWindow->SaveHierarchyState((IBundle**)&bundle));
    FAIL_RETURN(outState->PutBundle(WINDOW_HIERARCHY_TAG, bundle));
    AutoPtr<IParcelable> p;
    FAIL_RETURN(mFragments->SaveAllState((IParcelable**)&p));
    if (p != NULL) {
        outState->PutParcelable(FRAGMENTS_TAG, p);
    }
    AutoPtr<IApplication> app = GetApplication();
    return app->DispatchActivitySaveInstanceState(THIS_PROBE(IActivity), outState);
}

ECode Activity::SaveManagedDialogs(
    /* [in] */ IBundle* outState)
{
    Int32 numDialogs = mManagedDialogs.GetSize();
    if (numDialogs == 0) {
        return NOERROR;
    }

    AutoPtr<IBundle> dialogState;
    FAIL_RETURN(CBundle::New((IBundle**)&dialogState));
    AutoPtr<ArrayOf<Int32> > ids = ArrayOf<Int32>::Alloc(numDialogs);

    // save each dialog's bundle, gather the ids
    ManagedDialogHashMapIter it = mManagedDialogs.Begin();
    for (Int32 i = 0; it != mManagedDialogs.End(); ++i, ++it) {
        Int32 key = it->mFirst;
        (*ids)[i] = key;
        ManagedDialog* md = it->mSecond;
        AutoPtr<IBundle> di;
        md->mDialog->OnSaveInstanceState((IBundle**)&di);
        String keyStr = SavedDialogKeyFor(key);
        dialogState->PutBundle(keyStr, di);
        if (md->mArgs != NULL) {
            dialogState->PutBundle(keyStr, md->mArgs);
        }
    }

    dialogState->PutInt32Array(SAVED_DIALOG_IDS_KEY, ids);
    return outState->PutBundle(SAVED_DIALOGS_TAG, dialogState);
}

ECode Activity::OnPause()
{
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onPause ");
        sb.Append(ToString());
        Slogger::V(TAG, sb.ToString());
    }
    AutoPtr<IApplication> app = GetApplication();
    FAIL_RETURN(app->DispatchActivityPaused(THIS_PROBE(IActivity)));
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnUserLeaveHint()
{
    return NOERROR;
}

ECode Activity::OnCreateThumbnail(
    /* [in] */ IBitmap* outBitmap,
    /* [in] */ ICanvas* canvas,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    return NOERROR;
}

ECode Activity::OnCreateDescription(
    /* [out] */ ICharSequence** description)
{
    VALIDATE_NOT_NULL(description);
    *description = NULL;
    return NOERROR;
}

ECode Activity::OnStop()
{
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onStop ");
        sb.Append(ToString());
        Slogger::V(TAG, sb.ToString());
    }
    if (mActionBar != NULL) {
        FAIL_RETURN(mActionBar->SetShowHideAnimationEnabled(FALSE));
    }
    AutoPtr<IApplication> app = GetApplication();
    FAIL_RETURN(app->DispatchActivityDestroyed(THIS_PROBE(IActivity)));
    mCalled = TRUE;
    return NOERROR;
}

ECode Activity::OnDestroy()
{
    if (DEBUG_LIFECYCLE) {
        StringBuilder sb("onDestroy ");
        sb.Append(ToString());
        Slogger::V(TAG, sb.ToString());
    }
    mCalled = TRUE;

    // dismiss any dialogs we are managing.
    if (!mManagedDialogs.IsEmpty()) {
        ManagedDialogHashMapIter iter = mManagedDialogs.Begin();
        for (; iter != mManagedDialogs.End(); ++iter) {
            Boolean isShowing;
            iter->mSecond->mDialog->IsShowing(&isShowing);
            if (isShowing) {
                iter->mSecond->mDialog->Dismiss();
            }
        }
        mManagedDialogs.Clear();
    }

    // close any cursors we are managing.
    {
        AutoLock lock(mManagedCursorsLock);
        AutoPtr<ManagedCursor> cursor;
        List<AutoPtr<ManagedCursor> >::Iterator it = mManagedCursors.Begin();
        for (; it != mManagedCursors.End(); ++it) {
            cursor = *it;
            if (cursor != NULL) {
               cursor->mCursor->Close();
            }
        }
        mManagedCursors.Clear();
    }

    // Close any open search dialog
    if (mSearchManager != NULL) {
        FAIL_RETURN(mSearchManager->StopSearch());
    }

    AutoPtr<IApplication> app = GetApplication();
    return app->DispatchActivityDestroyed(THIS_PROBE(IActivity));
}

ECode Activity::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    if (DEBUG_LIFECYCLE) {
        String temp;
        newConfig->ToString(&temp);
        StringBuilder sb("onConfigurationChanged ");
        sb += ToString();
        sb += ": ";
        sb += temp;
        Slogger::V(TAG, sb.ToString());
    }
    mCalled = TRUE;

    mFragments->DispatchConfigurationChanged(newConfig);

    if (mWindow != NULL) {
        // Pass the configuration changed event to the window
        return mWindow->OnConfigurationChanged(newConfig);
    }

    if (mActionBar != NULL) {
        // Do this last; the action bar will need to access
        // view changes from above.
        return mActionBar->OnConfigurationChanged(newConfig);
    }
    return NOERROR;
}

ECode Activity::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mConfigChangeFlags;
    return NOERROR;
}

ECode Activity::GetLastNonConfigurationInstance(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = NULL;
    if (mLastNonConfigurationInstances != NULL) {
        return mLastNonConfigurationInstances->GetActivity(object);
    }
    return NOERROR;
}

ECode Activity::OnRetainNonConfigurationInstance(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    *object = NULL;
    return NOERROR;
}

ECode Activity::GetLastNonConfigurationChildInstances(
    /* [out] */ IObjectStringMap** data)
{
    VALIDATE_NOT_NULL(data);
    *data = NULL;

    if (mLastNonConfigurationInstances != NULL) {
        mLastNonConfigurationInstances->GetChildren(data);
    }
    return NOERROR;
}

ECode Activity::OnRetainNonConfigurationChildInstances(
    /* [out] */ IObjectStringMap** objectStringMap)
{
    VALIDATE_NOT_NULL(objectStringMap);
    *objectStringMap = NULL;
    return NOERROR;
}

ECode Activity::RetainNonConfigurationInstances(
    /* [out] */ IActivityNonConfigurationInstances** instance)
{
    VALIDATE_NOT_NULL(instance);
    *instance = NULL;

    AutoPtr<IInterface> activity;
    OnRetainNonConfigurationInstance((IInterface**)&activity);

    AutoPtr<IObjectStringMap> children;
    OnRetainNonConfigurationChildInstances((IObjectStringMap**)&children);

    AutoPtr<IObjectContainer> fragments;
    FAIL_RETURN(mFragments->RetainNonConfig((IObjectContainer**)&fragments));

    Boolean retainLoaders = FALSE;
    if (mAllLoaderManagers != NULL) {
        // prune out any loader managers that were already stopped and so
        // have nothing useful to retain.
        AutoPtr<IObjectContainer> values;
        mAllLoaderManagers->GetValues((IObjectContainer**)&values);

        if (values != NULL) {
            AutoPtr<IObjectEnumerator> it;
            values->GetObjectEnumerator((IObjectEnumerator**)&it);
            Boolean succeeded = FALSE, isRetaining = FALSE;;
            String who;
            ILoaderManagerImpl* lm;
            while (it->MoveNext(&succeeded), succeeded) {
                AutoPtr<IInterface> obj;
                it->Current((IInterface**)&obj);
                lm = (ILoaderManagerImpl*)obj->Probe(EIID_ILoaderManagerImpl);
                lm->IsRetaining(&isRetaining);
                if (isRetaining) {
                    retainLoaders = TRUE;
                }
                else {
                    lm->DoDestroy();
                    lm->GetName(&who);
                    mAllLoaderManagers->Remove(who);
                }
            }
        }
    }

    if (activity == NULL && children == NULL && fragments == NULL
            && !retainLoaders) {
        return NOERROR;
    }

    return CActivityNonConfigurationInstances::New(activity, children,
            fragments, mAllLoaderManagers, instance);
}

ECode Activity::OnLowMemory()
{
    if (DEBUG_LIFECYCLE) {
        Slogger::V(TAG, "onLowMemory %s", ToString().string());
    }
    mCalled = TRUE;
    return mFragments->DispatchLowMemory();
}

ECode Activity::OnTrimMemory(
    /* [in] */ Int32 level)
{
    if (DEBUG_LIFECYCLE) {
        Slogger::V(TAG, "onTrimMemory %s %d", ToString().string(), level);
    }
     mCalled = TRUE;
     return mFragments->DispatchTrimMemory(level);
}

ECode Activity::GetFragmentManager(
    /* [out] */ IFragmentManager** fmanager)
{
    VALIDATE_NOT_NULL(fmanager);
    *fmanager = mFragments;
    REFCOUNT_ADD(*fmanager)
    return NOERROR;
}

ECode Activity::InvalidateFragment(
    /* [in] */ const String& who)
{
    //Log.v(TAG, "invalidateFragmentIndex: index=" + index);
    if (mAllLoaderManagers != NULL) {
        AutoPtr<ILoaderManagerImpl> lm;
        mAllLoaderManagers->Get(who, (IInterface**)&lm);
        if (lm != NULL) {
            Boolean retaining;
            lm->IsRetaining(&retaining);
            if (!retaining) {
                lm->DoDestroy();
                mAllLoaderManagers->Remove(who);
            }
        }
    }

    return NOERROR;
}

ECode Activity::OnAttachFragment(
    /* [in] */ IFragment* fragment)
{
    return NOERROR;
}

ECode Activity::ManagedQuery(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String&selection,
    /* [in] */ const String&sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<IContentResolver> cResolver;
    GetContentResolver((IContentResolver**)&cResolver);
    AutoPtr<ICursor> c;
    cResolver->Query(uri, projection, selection, NULL, sortOrder, (ICursor**)&c);
    if (c != NULL) {
        StartManagingCursor(c);
    }
    *cursor = c;
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode Activity::ManagedQuery(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String&selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String&sortOrder,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);

    AutoPtr<IContentResolver> cResolver;
    GetContentResolver((IContentResolver**)&cResolver);
    AutoPtr<ICursor> c;
    cResolver->Query(uri, projection, selection, selectionArgs, sortOrder, (ICursor**)&c);
    if (c != NULL) {
        StartManagingCursor(c);
    }
    *cursor = c;
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode Activity::StartManagingCursor(
    /* [in] */ ICursor* c)
{
    AutoLock lock(mManagedCursorsLock);
    AutoPtr<ManagedCursor> mc = new ManagedCursor(c);
    mManagedCursors.PushBack(mc);
    return NOERROR;
}

ECode Activity::StopManagingCursor(
    /* [in] */ ICursor* c)
{
    AutoLock lock(mManagedCursorsLock);
    List<AutoPtr<ManagedCursor> >::Iterator it;
    for (it = mManagedCursors.Begin(); it != mManagedCursors.End(); ++it) {
        AutoPtr<ManagedCursor> mc = *it;
        if (mc->mCursor.Get() == c) {
            mManagedCursors.Remove(*it);
            break;
        }
    }
    return NOERROR;
}

ECode Activity::SetPersistent(
    /* [in] */ Boolean isPersistent)
{
    return NOERROR;
}

ECode Activity::FindViewById(
    /* [in] */ Int32 id,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return GetWindow()->FindViewById(id, view);
}

AutoPtr<IView> Activity::FindViewById(
    /* [in] */ Int32 id)
{
    AutoPtr<IView> view;
    GetWindow()->FindViewById(id, (IView**)&view);
    return view;
}

/**
 * Retrieve a reference to this activity's ActionBar.
 *
 * @return The Activity's ActionBar, or NULL if it does not have one.
 */
ECode Activity::GetActionBar(
    /* [out] */ IActionBar** actionbar)
{
    VALIDATE_NOT_NULL(actionbar);
    *actionbar = NULL;

    FAIL_RETURN(InitActionBar());
    *actionbar = mActionBar;
    REFCOUNT_ADD(*actionbar)
    return NOERROR;
}

/**
 * Creates a new ActionBar, locates the inflated ActionBarView,
 * initializes the ActionBar with the view, and sets mActionBar.
 */
ECode Activity::InitActionBar()
{
     AutoPtr<IWindow> window = GetWindow();

     // Initializing the window decor can change window feature flags.
     // Make sure that we have the correct set before performing the test below.
     AutoPtr<IView> d;
     FAIL_RETURN(window->GetDecorView((IView**)&d));

     Boolean isChild;
     IsChild(&isChild);
     if (isChild) {
         return NOERROR;
     }

     Boolean hasFeature;
     FAIL_RETURN(window->HasFeature(IWindow::FEATURE_ACTION_BAR, &hasFeature));
     if (!hasFeature) {
         return NOERROR;
     }

     if (mActionBar != NULL) {
         return NOERROR;
     }

    FAIL_RETURN(CActionBarImpl::New(THIS_PROBE(IActivity),
            (IActionBarImpl**)&mActionBar));
     return mActionBar->SetDefaultDisplayHomeAsUpEnabled(mEnableDefaultActionBarUp);
}

ECode Activity::SetContentView(
    /* [in] */ Int32 layoutResID)
{
    FAIL_RETURN(GetWindow()->SetContentView(layoutResID));
    return InitActionBar();
}

ECode Activity::SetContentView(
    /* [in] */ IView* view)
{
    FAIL_RETURN(GetWindow()->SetContentView(view));
    return InitActionBar();
}

ECode Activity::SetContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    FAIL_RETURN(GetWindow()->SetContentView(view, params));
    return InitActionBar();
}

ECode Activity::AddContentView(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    FAIL_RETURN(GetWindow()->AddContentView(view, params));
    return InitActionBar();
}

ECode Activity::SetFinishOnTouchOutside(
    /* [in] */ Boolean finish)
{
    return mWindow->SetCloseOnTouchOutside(finish);
}

ECode Activity::SetDefaultKeyMode(
    /* [in] */  Int32 mode)
{
    mDefaultKeyMode = mode;

    // Some modes use a SpannableStringBuilder to track & dispatch input events
    // This list must remain in sync with the switch in onKeyDown()
    switch (mode) {
    case IActivity::DEFAULT_KEYS_DISABLE:
    case IActivity::DEFAULT_KEYS_SHORTCUT:
        mDefaultKeySsb = NULL;      // not used in these modes
        break;
    case IActivity::DEFAULT_KEYS_DIALER:
    case IActivity::DEFAULT_KEYS_SEARCH_LOCAL:
    case IActivity::DEFAULT_KEYS_SEARCH_GLOBAL: {
        mDefaultKeySsb = NULL;
        CSpannableStringBuilder::New((ISpannableStringBuilder**)&mDefaultKeySsb);
        AutoPtr<ISelectionHelper> helper;
        //CSelectionHelper::AcquireSingleton((ISelectionHelper**)&helper);
        FAIL_RETURN(helper->SetSelection(mDefaultKeySsb, 0));
        break;
    }
    default:
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode Activity::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        AutoPtr<IApplicationInfo> appInfo;
        FAIL_RETURN(GetApplicationInfo((IApplicationInfo**)&appInfo));
        Int32 targetSdkVersion;
        FAIL_RETURN(appInfo->GetTargetSdkVersion(&targetSdkVersion));

        if (targetSdkVersion >= Build::VERSION_CODES::ECLAIR) {
            FAIL_RETURN(event->StartTracking());
        }
        else {
            FAIL_RETURN(OnBackPressed());
        }

        *result = TRUE;
        return NOERROR;
    }

    if (mDefaultKeyMode == IActivity::DEFAULT_KEYS_DISABLE) {
        return NOERROR;
    }
    else if (mDefaultKeyMode == IActivity::DEFAULT_KEYS_SHORTCUT) {
        AutoPtr<IWindow> win = GetWindow();

        Boolean state;
        FAIL_RETURN(win->PerformPanelShortcut(IWindow::FEATURE_OPTIONS_PANEL,
                keyCode, event, IMenu::FLAG_ALWAYS_PERFORM_CLOSE, &state));

        if (state) {
            *result = TRUE;
        }
        else {
            *result = FALSE;
        }
        return NOERROR;
    }
    else {
        // Common code for DEFAULT_KEYS_DIALER & DEFAULT_KEYS_SEARCH_*
        Boolean clearSpannable = FALSE;
        Boolean handled = FALSE, isSystem;
        Int32 repeatCount;
        event->GetRepeatCount(&repeatCount);
        event->IsSystem(&isSystem);

        if ((repeatCount != 0) || isSystem) {
            clearSpannable = TRUE;
            handled = FALSE;
        }
        else {
            AutoPtr<ITextKeyListenerHelper> listenerHelper;
            CTextKeyListenerHelper::AcquireSingleton((ITextKeyListenerHelper**)&listenerHelper);
            AutoPtr<ITextKeyListener> listener;
            listenerHelper->GetInstance((ITextKeyListener**)&listener);
            IKeyListener* listenerK = (IKeyListener*)(listener->Probe(EIID_IKeyListener));
            FAIL_RETURN(listenerK->OnKeyDown(NULL, mDefaultKeySsb, keyCode, event, &handled));
            Int32 length = 0;
            mDefaultKeySsb->GetLength(&length);
            if (handled && length > 0) {
                // something useable has been typed - dispatch it now.

                String str;
                mDefaultKeySsb->ToString(&str);
                clearSpannable = TRUE;

                switch (mDefaultKeyMode) {
                case IActivity::DEFAULT_KEYS_DIALER: {
                    StringBuilder sb("tel:");
                    sb += str;
                    AutoPtr<IUri> uri;
                    AutoPtr<IUriHelper> helper;
                    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
                    FAIL_RETURN(helper->Parse(sb.ToString(), (IUri**)&uri));

                    AutoPtr<IIntent> intent;
                    CIntent::New(IIntent::ACTION_DIAL, uri, (IIntent**)&intent);
                    FAIL_RETURN(intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK));
                    FAIL_RETURN(StartActivity(intent));
                    break;
                }

                case IActivity::DEFAULT_KEYS_SEARCH_LOCAL:
                    FAIL_RETURN(StartSearch(str, FALSE, NULL, FALSE));
                    break;
                case IActivity::DEFAULT_KEYS_SEARCH_GLOBAL:
                    FAIL_RETURN(StartSearch(str, FALSE, NULL, TRUE));
                    break;
                }
            }
        }
        if (clearSpannable) {
            FAIL_RETURN(mDefaultKeySsb->Clear());
            FAIL_RETURN(mDefaultKeySsb->ClearSpans());

            AutoPtr<ISelectionHelper> helper;
            //CSelectionHelper::AcquireSingleton((ISelectionHelper**)&helper);
            FAIL_RETURN(helper->SetSelection(mDefaultKeySsb, 0));
        }

        *result = handled;
        return NOERROR;
    }
}

ECode Activity::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Activity::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IApplicationInfo> appInfo;
    FAIL_RETURN(GetApplicationInfo((IApplicationInfo**)&appInfo));
    Int32 targetSdkVersion;
    FAIL_RETURN(appInfo->GetTargetSdkVersion(&targetSdkVersion));

    if (targetSdkVersion >= Build::VERSION_CODES::ECLAIR) {
        Boolean tracking, canceled;
        FAIL_RETURN(event->IsTracking(&tracking));
        FAIL_RETURN(event->IsCanceled(&canceled));

        if (keyCode == IKeyEvent::KEYCODE_BACK && tracking && !canceled) {
            FAIL_RETURN(OnBackPressed());

            *result = TRUE;
            return NOERROR;
        }
    }
    return NOERROR;
}

ECode Activity::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Activity::OnBackPressed()
{
    Boolean val = FALSE;
    FAIL_RETURN(mFragments->PopBackStackImmediate(&val));
    if (!val) {
        return Finish();
    }
    return NOERROR;
}

ECode Activity::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Activity::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    Boolean val;
    FAIL_RETURN(mWindow->ShouldCloseOnTouch(THIS_PROBE(IActivity), event, &val));
    if (val) {
        FAIL_RETURN(Finish());
        *result = TRUE;
    }
    return NOERROR;
}

ECode Activity::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Activity::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode Activity::OnUserInteraction()
{
    return NOERROR;
}

ECode Activity::OnWindowAttributesChanged(
    /* [in] */ IWindowManagerLayoutParams* params)
{
    // Update window manager if: we have a view, that view is
    // attached to its parent (which will be a RootView), and
    // this activity is not embedded.
    if (mParent == NULL) {
        AutoPtr<IView> decor = mDecor;
        if (decor != NULL) {
            AutoPtr<IViewParent> parent;
            FAIL_RETURN(decor->GetParent((IViewParent**)&parent));
            if (parent != NULL) {
                AutoPtr<IWindowManager> winmanager = GetWindowManager();
                return winmanager->UpdateViewLayout(decor, params);
            }
        }
    }

    return NOERROR;
}

ECode Activity::OnContentChanged()
{
    return NOERROR;
}

ECode Activity::OnWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return NOERROR;
}

ECode Activity::OnAttachedToWindow()
{
    return NOERROR;
}

ECode Activity::OnDetachedFromWindow()
{
    return NOERROR;
}

ECode Activity::HasWindowFocus(
    /* [out] */ Boolean* hasFocus)
{
    VALIDATE_NOT_NULL(hasFocus);
    *hasFocus = FALSE;

    AutoPtr<IWindow> w = GetWindow();
    if (w != NULL) {
        AutoPtr<IView> d;
        FAIL_RETURN(w->GetDecorView((IView**)&d));
        if (d != NULL) {
            return d->HasWindowFocus(hasFocus);
        }
    }
    return NOERROR;
}

ECode Activity::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isConsumed)
{
    VALIDATE_NOT_NULL(isConsumed);
    *isConsumed = FALSE;

    FAIL_RETURN(OnUserInteraction());

    AutoPtr<IWindow> win = GetWindow();
    Boolean succeeded;
    FAIL_RETURN(win->SuperDispatchKeyEvent(event, &succeeded));

    if (succeeded) {
        *isConsumed = TRUE;
        return NOERROR;
    }

    AutoPtr<IView> decor = mDecor;
    if (decor == NULL) {
        FAIL_RETURN(win->GetDecorView((IView**)&decor));
    }

    AutoPtr<IDispatcherState> dispatcher;
    if (decor != NULL) {
        FAIL_RETURN(decor->GetKeyDispatcherState((IDispatcherState**)&dispatcher));
    }

    event->Dispatch(THIS_PROBE(IKeyEventCallback), dispatcher,
            THIS_PROBE(IInterface), isConsumed);
    return NOERROR;
}

ECode Activity::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    FAIL_RETURN(OnUserInteraction());
     AutoPtr<IWindow> win = GetWindow();
     Boolean succeeded = FALSE;
     FAIL_RETURN(win->SuperDispatchKeyShortcutEvent(event, &succeeded));
     if (succeeded) {
         *result = TRUE;
         return NOERROR;
     }
     Int32 keyCode = 0;
     event->GetKeyCode(&keyCode);
     return OnKeyShortcut(keyCode, event, result);
}

ECode Activity::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* isConsumed)
{
    VALIDATE_NOT_NULL(isConsumed);
    *isConsumed = FALSE;

    Int32 action;
    event->GetAction(&action);
    if (action == IMotionEvent::ACTION_DOWN) {
        FAIL_RETURN(OnUserInteraction());
    }

    AutoPtr<IWindow> win = GetWindow();
    Boolean succeeded;
    FAIL_RETURN(win->SuperDispatchTouchEvent(event, &succeeded));

    if (succeeded) {
        *isConsumed = TRUE;
        return NOERROR;
    }

    return OnTouchEvent(event, isConsumed);
}

ECode Activity::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* isConsumed)
{
    VALIDATE_NOT_NULL(isConsumed);
    *isConsumed = FALSE;

    OnUserInteraction();

    AutoPtr<IWindow> win = GetWindow();
    Boolean succeeded;
    FAIL_RETURN(win->SuperDispatchTrackballEvent(event, &succeeded));

    if (succeeded) {
        *isConsumed = TRUE;
        return NOERROR;
    }

    return OnTrackballEvent(event, isConsumed);
}

ECode Activity::DispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* isConsumed)
{
    VALIDATE_NOT_NULL(isConsumed);
    *isConsumed = FALSE;

     OnUserInteraction();

     AutoPtr<IWindow> win = GetWindow();
     Boolean succeeded;
     FAIL_RETURN(win->SuperDispatchGenericMotionEvent(event, &succeeded));

     if (succeeded) {
         *isConsumed = TRUE;
         return NOERROR;
     }

     return OnGenericMotionEvent(event, isConsumed);
}

ECode Activity::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* isConsumed)
{
    VALIDATE_NOT_NULL(isConsumed);
    // TODO
    // event.setClassName(getClass().getName());
    String pkgName;
    GetPackageName(&pkgName);
    AutoPtr<ICharSequence> nameS;
    CStringWrapper::New(pkgName, (ICharSequence**)&nameS);
    event->SetPackageName(nameS);

    AutoPtr<IWindowManagerLayoutParams> params;
    GetWindow()->GetAttributes((IWindowManagerLayoutParams**)&params);
    Int32 width, height;
    params->GetWidth(&width);
    params->GetHeight(&height);
    Boolean isFullScreen = (width == IWindowManagerLayoutParams::MATCH_PARENT) &&
        (height == IWindowManagerLayoutParams::MATCH_PARENT);
    event->SetFullScreen(isFullScreen);

    AutoPtr<ICharSequence> title;
    GetTitle((ICharSequence**)&title);
    if (!TextUtils::IsEmpty(title)) {
        AutoPtr<IObjectContainer> txtContainer;
       event->GetText((IObjectContainer**)&txtContainer);
       txtContainer->Add(title);
    }

    *isConsumed = TRUE;
    return NOERROR;
}

ECode Activity::OnCreatePanelView(
    /* [in] */ Int32 featureId,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    return NOERROR;
}

ECode Activity::OnCreatePanelMenu(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow)
{
    VALIDATE_NOT_NULL(allowToShow);
    *allowToShow = FALSE;

    if (featureId == IWindow::FEATURE_OPTIONS_PANEL) {
        Boolean show;
        FAIL_RETURN(OnCreateOptionsMenu(menu, &show));

        AutoPtr<IMenuInflater> mi;
        FAIL_RETURN(GetMenuInflater((IMenuInflater**)&mi));
        Boolean result = FALSE;
        FAIL_RETURN(mFragments->DispatchCreateOptionsMenu(menu, mi, &result));
        show |= result;
        *allowToShow = show;
    }

    return NOERROR;
}

ECode Activity::OnPreparePanel(
    /* [in] */ Int32 featureId,
    /* [in] */ IView* view,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow)
{
    VALIDATE_NOT_NULL(allowToShow);
    *allowToShow = TRUE;

    if (featureId == IWindow::FEATURE_OPTIONS_PANEL && menu != NULL) {
        Boolean goforit = FALSE;
        OnPrepareOptionsMenu(menu, &goforit);
        Boolean result = FALSE;
        FAIL_RETURN(mFragments->DispatchPrepareOptionsMenu(menu, &result));
        goforit |= result;
        *allowToShow = goforit;
        return NOERROR;
    }

    return NOERROR;
}

ECode Activity::OnMenuOpened(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToOpen)
{
    VALIDATE_NOT_NULL(allowToOpen);
    *allowToOpen = TRUE;

    if (featureId == IWindow::FEATURE_ACTION_BAR) {
        InitActionBar();
        if (mActionBar != NULL) {
            FAIL_RETURN(mActionBar->DispatchMenuVisibilityChanged(TRUE));
        }
        else {
            Logger::E(TAG, "Tried to open action bar menu with no action bar");
        }
    }

    return NOERROR;
}

ECode Activity::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* toFinish)
{
    VALIDATE_NOT_NULL(toFinish);
    *toFinish = FALSE;

    switch(featureId) {
        case IWindow::FEATURE_OPTIONS_PANEL: {
            // Put event logging here so it gets called even if subclass
            // doesn't call through to superclass's implmeentation of each
            // of these methods below
            //EventLog.writeEvent(50000, 0, item.getTitleCondensed());
            Boolean val;
            FAIL_RETURN(OnOptionsItemSelected(item, &val));
            if (val) {
                *toFinish = TRUE;
                return NOERROR;
            }

            FAIL_RETURN(mFragments->DispatchOptionsItemSelected(item, &val));
            if (val) {
                *toFinish = TRUE;
                return NOERROR;
            }

            Int32 itemId;
            item->GetItemId(&itemId);
            Int32 options = 0;
            if (mActionBar != NULL) {
                FAIL_RETURN(mActionBar->GetDisplayOptions(&options));
            }

            if (itemId == R::id::home
                    && (options & IActionBar::DISPLAY_HOME_AS_UP) != 0) {
                if (mParent == NULL) {
                    return OnNavigateUp(toFinish);
                } else {
                    return mParent->OnNavigateUpFromChild(THIS_PROBE(IActivity), toFinish);
                }
            }
            *toFinish = FALSE;
            return NOERROR;
        }
        case IWindow::FEATURE_CONTEXT_MENU: {
//          EventLog.writeEvent(50000, 1, item.getTitleCondensed());
            Boolean isSelected;
            FAIL_RETURN(OnContextItemSelected(item, &isSelected));
            if (isSelected) {
                *toFinish = TRUE;
                return NOERROR;
            }
            return mFragments->DispatchContextItemSelected(item, toFinish);
            return NOERROR;
        }

        default:
            *toFinish = FALSE;
            break;
    }

    return NOERROR;
}

ECode Activity::OnPanelClosed(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenu* menu)
{
    switch(featureId) {
        case IWindow::FEATURE_OPTIONS_PANEL: {
            FAIL_RETURN(mFragments->DispatchOptionsMenuClosed(menu));
            FAIL_RETURN(OnOptionsMenuClosed(menu));
            break;
        }
        case IWindow::FEATURE_CONTEXT_MENU: {
            FAIL_RETURN(OnContextMenuClosed(menu));
            break;
        }
        case IWindow::FEATURE_ACTION_BAR:
            FAIL_RETURN(InitActionBar());
            FAIL_RETURN(mActionBar->DispatchMenuVisibilityChanged(FALSE));
            break;
    }

    return NOERROR;
}

ECode Activity::InvalidateOptionsMenu()
{
     return mWindow->InvalidatePanelMenu(IWindow::FEATURE_OPTIONS_PANEL);
}

ECode Activity::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow)
{
    VALIDATE_NOT_NULL(allowToShow);
    *allowToShow = TRUE;

    if (mParent != NULL) {
        return mParent->OnCreateOptionsMenu(menu, allowToShow);
    }

    return NOERROR;
}

ECode Activity::OnPrepareOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;

    if (mParent != NULL) {
        return mParent->OnPrepareOptionsMenu(menu, result);
    }

    return NOERROR;
}

ECode Activity::OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (mParent != NULL) {
        return mParent->OnOptionsItemSelected(item, result);
    }

    return NOERROR;
}

ECode Activity::OnNavigateUp(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Automatically handle hierarchical Up navigation if the proper
    // metadata is available.
    AutoPtr<IIntent> upIntent;
    GetParentActivityIntent((IIntent**)&upIntent);
    if (upIntent != NULL) {
        Boolean recreate;
        ShouldUpRecreateTask(upIntent, &recreate);
        String task;
        mActivityInfo->GetTaskAffinity(&task);
        if (task.IsNull()) {
            // Activities with a NULL affinity are special; they really shouldn't
            // specify a parent activity intent in the first place. Just finish
            // the current activity and call it a day.
            Finish();
        } else if (recreate) {
            AutoPtr<ITaskStackBuilderHelper> taskBHelper;
            CTaskStackBuilderHelper::AcquireSingleton((ITaskStackBuilderHelper**)&taskBHelper);
            AutoPtr<ITaskStackBuilder> b;
            taskBHelper->Create(this, (ITaskStackBuilder**)&b);
            OnCreateNavigateUpTaskStack(b);
            OnPrepareNavigateUpTaskStack(b);
            b->StartActivities();

            // We can't finishAffinity if we have a result.
            // Fall back and simply finish the current activity instead.
            if (mResultCode != RESULT_CANCELED || mResultData != NULL) {
                // Tell the developer what's going on to avoid hair-pulling.
                Slogger::I(TAG, "onNavigateUp only finishing topmost activity to return a result");
                Finish();
            } else {
                FinishAffinity();
            }
        } else {
            Boolean navigate;
            NavigateUpTo(upIntent, &navigate);
        }
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode Activity::OnNavigateUpFromChild(
    /* [in] */ IActivity* child,
    /* [out] */ Boolean* result)
{
    return OnNavigateUp(result);
}

ECode Activity::OnCreateNavigateUpTaskStack(
    /* [in] */ ITaskStackBuilder* builder)
{
    VALIDATE_NOT_NULL(builder);
    return builder->AddParentStack(THIS_PROBE(IActivity));
}

ECode Activity::OnPrepareNavigateUpTaskStack(
    /* [in] */ ITaskStackBuilder* builder)
{
    return NOERROR;
}

ECode Activity::OnOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    if (mParent != NULL) {
        return mParent->OnOptionsMenuClosed(menu);
    }

    return NOERROR;
}

ECode Activity::OpenOptionsMenu()
{
    return mWindow->OpenPanel(IWindow::FEATURE_OPTIONS_PANEL, NULL);
}

ECode Activity::CloseOptionsMenu()
{
    return mWindow->ClosePanel(IWindow::FEATURE_OPTIONS_PANEL);
}

ECode Activity::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return NOERROR;
}

ECode Activity::RegisterForContextMenu(
    /* [in] */ IView* view)
{
    VALIDATE_NOT_NULL(view);
    return view->SetOnCreateContextMenuListener(
            THIS_PROBE(IViewOnCreateContextMenuListener));
}

ECode Activity::UnregisterForContextMenu(
    /* [in] */ IView* view)
{
    VALIDATE_NOT_NULL(view);
    return view->SetOnCreateContextMenuListener(NULL);
}

ECode Activity::OpenContextMenu(
    /* [in] */ IView* view)
{
    VALIDATE_NOT_NULL(view);
    Boolean res;
    return view->ShowContextMenu(&res);
}

ECode Activity::CloseContextMenu()
{
    return mWindow->ClosePanel(IWindow::FEATURE_CONTEXT_MENU);
}

ECode Activity::OnContextItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (mParent != NULL) {
        return mParent->OnContextItemSelected(item, result);
    }

    return NOERROR;
}

ECode Activity::OnContextMenuClosed(
        /* [in] */ IMenu* menu)
{
    if (mParent != NULL) {
        return mParent->OnContextMenuClosed(menu);
    }

    return NOERROR;
}

AutoPtr<IDialog> Activity::OnCreateDialog(
    /* [in] */ Int32 id)
{
    return NULL;
}

AutoPtr<IDialog> Activity::OnCreateDialog(
    /* [in] */ Int32 id,
    /* [in] */ IBundle* args)
{
    return OnCreateDialog(id);
}

void Activity::OnPrepareDialog(
    /* [in] */ Int32 id,
    /* [in] */ IDialog* dialog)
{
    if (dialog);
        dialog->SetOwnerActivity(THIS_PROBE(IActivity));
}

void Activity::OnPrepareDialog(
    /* [in] */ Int32 id,
    /* [in] */ IDialog* dialog,
    /* [in] */ IBundle* args)
{
    OnPrepareDialog(id, dialog);
}

ECode Activity::ShowDialog(
    /* [in] */ Int32 id)
{
    Boolean res;
    return ShowDialog(id, NULL, &res);
}

ECode Activity::ShowDialog(
    /* [in] */ Int32 id,
    /* [in] */ IBundle* args,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ManagedDialog> md;
    ManagedDialogHashMapIter find = mManagedDialogs.Find(id);
    if (find == mManagedDialogs.End()) {
        md = new ManagedDialog();
        md->mDialog = CreateDialog(id, NULL, args);
        if (md->mDialog == NULL) {
            *result = FALSE;
            return NOERROR;
        }
        mManagedDialogs[id] = md;
    }
    else {
        md = find->mSecond;
    }

    md->mArgs = args;
    OnPrepareDialog(id, md->mDialog, args);
    md->mDialog->Show();

    *result = TRUE;
    return NOERROR;
}

ECode Activity::DismissDialog(
    /* [in] */ Int32 id)
{
    if (mManagedDialogs.IsEmpty()) {
        //throw missingDialog(id);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ManagedDialogHashMapIter find = mManagedDialogs.Find(id);
    if (find == mManagedDialogs.End()) {
        //throw missingDialog(id);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return find->mSecond->mDialog->Dismiss();
}

ECode Activity::RemoveDialog(
    /* [in] */ Int32 id)
{
    if (!mManagedDialogs.IsEmpty()) {
        ManagedDialogHashMapIter find = mManagedDialogs.Find(id);
        if (find != mManagedDialogs.End()) {
            find->mSecond->mDialog->Dismiss();
            mManagedDialogs.Erase(find);
        }
    }

    return NOERROR;
}

ECode Activity::OnSearchRequested(
    /* [out] */ Boolean* isLaunched)
{
    VALIDATE_NOT_NULL(isLaunched);
    String nullStr;
    ECode ec = StartSearch(nullStr, FALSE, NULL, FALSE);
    *isLaunched = TRUE;
    return ec;
}

ECode Activity::StartSearch(
    /* [in] */ const String& initialQuery,
    /* [in] */ Boolean selectInitialQuery,
    /* [in] */ IBundle* appSearchData,
    /* [in] */ Boolean globalSearch)
{
    FAIL_RETURN(EnsureSearchManager());

    AutoPtr<IComponentName> cn;
    FAIL_RETURN(GetComponentName((IComponentName**)&cn));
    return mSearchManager->StartSearch(
            initialQuery, selectInitialQuery, cn, appSearchData, globalSearch);
}

ECode Activity::TriggerSearch(
    /* [in] */ const String& query,
    /* [in] */ IBundle* appSearchData)
{
    FAIL_RETURN(EnsureSearchManager());

    AutoPtr<IComponentName> cn;
    FAIL_RETURN(GetComponentName((IComponentName**)&cn));
    return mSearchManager->TriggerSearch(query, cn, appSearchData);
}

ECode Activity::TakeKeyEvents(
    /* [in] */ Boolean get)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->TakeKeyEvents(get);
}

ECode Activity::RequestWindowFeature(
    /* [in] */ Int32 featureId,
    /* [out] */ Boolean* result)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->RequestFeature(featureId, result);
}

ECode Activity::SetFeatureDrawableResource(
    /* [in] */ Int32 featureId,
    /* [in] */ Int32 resId)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureDrawableResource(featureId, resId);
}

ECode Activity::SetFeatureDrawableUri(
    /* [in] */ Int32 featureId,
    /* [in] */ IUri* uri)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureDrawableUri(featureId, uri);
}
ECode Activity::SetFeatureDrawable(
        /* [in] */ Int32 featureId,
        /* [in] */ IDrawable* drawable)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureDrawable(featureId, drawable);
}

ECode Activity::SetFeatureDrawableAlpha(
        /* [in] */ Int32 featureId,
        /* [in] */ Int32 alpha)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureDrawableAlpha(featureId, alpha);
}

ECode Activity::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);
    AutoPtr<IWindow> win = GetWindow();
    return win->GetLayoutInflater(inflater);
}

ECode Activity::GetMenuInflater (
    /* [out] */ IMenuInflater** menuInflater)
{
    VALIDATE_NOT_NULL(menuInflater);

    // mMenuInflater and Activity have circular reference, modified by xihao
    InitActionBar();
    if (mActionBar != NULL) {
        AutoPtr<IContext> context;
        mActionBar->GetThemedContext((IContext**)&context);
        return CMenuInflater::New(context, THIS_PROBE(IActivity), menuInflater);
    }
    else {
        return CMenuInflater::New(THIS_PROBE(IActivity), menuInflater);
    }

    // Make sure that action views can get an appropriate theme.
    // if (mMenuInflater == NULL) {
    //     InitActionBar();
    //     if (mActionBar != NULL) {
    //         AutoPtr<IContext> context;
    //         mActionBar->GetThemedContext((IContext**)&context);
    //         FAIL_RETURN(CMenuInflater::New(context, THIS_PROBE(IActivity),
    //                (IMenuInflater**)&mMenuInflater));
    //     }
    //     else {
    //         FAIL_RETURN(CMenuInflater::New(THIS_PROBE(IActivity),
    //                (IMenuInflater**)&mMenuInflater));
    //     }
    // }
    // *menuInflater = mMenuInflater;
    // REFCOUNT_ADD(*menuInflater);
    return NOERROR;
}

ECode Activity::OnApplyThemeResource(
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    VALIDATE_NOT_NULL(theme);
    if (mParent == NULL) {
        return theme->ApplyStyle(resid, TRUE);
    }
    else {
        AutoPtr<IResourcesTheme> parentTheme;
        FAIL_RETURN(mParent->GetTheme((IResourcesTheme**)&parentTheme));
        theme->SetTo(parentTheme);
        FAIL_RETURN(theme->ApplyStyle(resid, FALSE));
    }

    return NOERROR;
}

ECode Activity::StartActivityForResult(
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 requestCode)
{
    return StartActivityForResult(intent, requestCode, NULL);
}

ECode Activity::StartActivityForResult(
    /* [in] */ IIntent *intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    if (mParent == NULL) {
        AutoPtr<IInstrumentationActivityResult> result;
        AutoPtr<IApplicationThread> at;
        FAIL_RETURN(mMainThread->GetApplicationThread((IApplicationThread**)&at));
        FAIL_RETURN(mInstrumentation->ExecStartActivity(
                mBase, IBinder::Probe(at.Get()), mToken, THIS_PROBE(IActivity),
                intent, requestCode, options, (IInstrumentationActivityResult**)&result));
        if (result != NULL) {
            Int32 resultCode;
            AutoPtr<IIntent> resultData;
            result->GetResultCode(&resultCode);
            result->GetResultData((IIntent**)&resultData);
            FAIL_RETURN(mMainThread->SendActivityResult(
                mToken, mEmbeddedID, requestCode, resultCode, resultData));
        }
        if (requestCode >= 0) {
            // If this start is requesting a result, we can avoid making
            // the activity visible until the result is received.  Setting
            // this code during onCreate(Bundle savedInstanceState) or onResume() will keep the
            // activity hidden during this time, to avoid flickering.
            // This can only be done when a result is requested because
            // that guarantees we will get information back when the
            // activity is finished, no matter what happens to it.
            mStartedActivity = TRUE;
        }
    }
    else {
        if (options != NULL) {
            return mParent->StartActivityFromChild(THIS_PROBE(IActivity), intent, requestCode, options);
        }
        else {
            // Note we want to go through this method for compatibility with
            // existing applications that may have overridden it.
            return mParent->StartActivityFromChild(THIS_PROBE(IActivity), intent, requestCode);
        }
    }

    return NOERROR;
}

ECode Activity::StartIntentSenderForResult(
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return StartIntentSenderForResult(intent, requestCode, fillInIntent, flagsMask,
            flagsValues, extraFlags, NULL);
}

ECode Activity::StartIntentSenderForResult(
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    if (mParent == NULL) {
        return StartIntentSenderForResultInner(intent, requestCode, fillInIntent,
                flagsMask, flagsValues, THIS_PROBE(IActivity), options);
    }
    else if (options != NULL) {
        return mParent->StartIntentSenderFromChild(THIS_PROBE(IActivity),
                intent, requestCode, fillInIntent, flagsMask, flagsValues, extraFlags, options);
    }
    else {
        // Note we want to go through this call for compatibility with
        // existing applications that may have overridden the method.
        return mParent->StartIntentSenderFromChild(THIS_PROBE(IActivity),
                intent, requestCode, fillInIntent, flagsMask, flagsValues, extraFlags);
    }
}

ECode Activity::StartIntentSenderForResultInner(
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* options)
{
    String resolvedType;
    if (fillInIntent != NULL) {
        AutoPtr<IContentResolver> resolver;
        GetContentResolver((IContentResolver**)&resolver);
        FAIL_RETURN(fillInIntent->SetAllowFds(FALSE));
        FAIL_RETURN(fillInIntent->ResolveTypeIfNeeded(resolver, &resolvedType));
    }

    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    Int32 result;
    AutoPtr<IApplicationThread> at;
    mMainThread->GetApplicationThread((IApplicationThread**)&at);

    String otherId;
    activity->GetID(&otherId);
    ECode ec = defaultAM->StartActivityIntentSender(at, intent,
        fillInIntent, resolvedType, mToken, otherId,
        requestCode, flagsMask, flagsValues, options, &result);
    if (result == IActivityManager::START_CANCELED) {
        //throw new IntentSender.SendIntentException();
        return E_SEND_INTENT_EXCEPTION;
    }
    FAIL_RETURN(ec);

    AutoPtr<IInstrumentationHelper> helper;
    CInstrumentationHelper::AcquireSingleton((IInstrumentationHelper**)&helper);
    FAIL_RETURN(helper->CheckStartActivityResult(result, NULL));

    if (requestCode >= 0) {
        // If this start is requesting a result, we can avoid making
        // the activity visible until the result is received.  Setting
        // this code during onCreate(Bundle savedInstanceState) or onResume() will keep the
        // activity hidden during this time, to avoid flickering.
        // This can only be done when a result is requested because
        // that guarantees we will get information back when the
        // activity is finished, no matter what happens to it.
        mStartedActivity = true;
    }
    return NOERROR;
}

ECode Activity::StartActivityIfNeeded(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [out] */ Boolean* started)
{
    return StartActivityIfNeeded(intent, requestCode, NULL, started);
}

ECode Activity::StartActivityIfNeeded(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options,
    /* [out] */ Boolean* started)
{
    VALIDATE_NOT_NULL(intent);
    VALIDATE_NOT_NULL(started);
    *started = FALSE;

    if (mParent == NULL) {
        Int32 result = IActivityManager::START_RETURN_INTENT_TO_CALLER;
        //try {
        intent->SetAllowFds(FALSE);

        AutoPtr<IContentResolver> resolver;
        FAIL_RETURN(GetContentResolver((IContentResolver**)&resolver));
        String resolvedType;
        FAIL_RETURN(intent->ResolveTypeIfNeeded(resolver, &resolvedType));

        AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
        AutoPtr<IApplicationThread> at;
        FAIL_RETURN(mMainThread->GetApplicationThread((IApplicationThread**)&at));

        String nullStr;
        ECode ec = defaultAM->StartActivity(at, intent, resolvedType, mToken, mEmbeddedID,
                requestCode, IActivityManager::START_FLAG_ONLY_IF_NEEDED,
                nullStr, NULL, options, &result);
        if(ec == (ECode)E_REMOTE_EXCEPTION) {
            ec = NOERROR;
        }
        FAIL_RETURN(ec);
//        } catch (RemoteException e) {
//        // Empty
//        }

        AutoPtr<IInstrumentationHelper> helper;
        CInstrumentationHelper::AcquireSingleton((IInstrumentationHelper**)&helper);
        FAIL_RETURN(helper->CheckStartActivityResult(result, intent));

        if (requestCode >= 0) {
            // If this start is requesting a result, we can avoid making
            // the activity visible until the result is received.  Setting
            // this code during onCreate(Bundle savedInstanceState) or onResume() will keep the
            // activity hidden during this time, to avoid flickering.
            // This can only be done when a result is requested because
            // that guarantees we will get information back when the
            // activity is finished, no matter what happens to it.
            mStartedActivity = TRUE;
        }
        *started = (result != IActivityManager::START_RETURN_INTENT_TO_CALLER);
    }

    Logger::E(TAG, "startActivityIfNeeded can only be called from a top-level activity");
//    throw new UnsupportedOperationException(
//            "startActivityIfNeeded can only be called from a top-level activity");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Activity::StartNextMatchingActivity(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* started)
{
    return StartNextMatchingActivity(intent, NULL, started);
}

ECode Activity::StartNextMatchingActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [out] */ Boolean* started)
{
    VALIDATE_NOT_NULL(started);
    *started = FALSE;

    if (mParent == NULL) {
//        try {
        intent->SetAllowFds(FALSE);
        AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

        ECode ec = defaultAM->StartNextMatchingActivity(mToken, intent, options, started);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            ec = NOERROR;
        }
//        } catch (RemoteException e) {
//        // Empty
//        }
        *started = FALSE;
        return ec;
    }

    Logger::E(TAG, "startNextMatchingActivity can only be called from a top-level activity");
//    throw new UnsupportedOperationException(
//            "startNextMatchingActivity can only be called from a top-level activity");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Activity::StartActivityFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    return StartActivityFromChild(child, intent, requestCode, NULL);
}

ECode Activity::StartActivityFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    AutoPtr<IInstrumentationActivityResult> result;
    AutoPtr<IApplicationThread> at;
    FAIL_RETURN(mMainThread->GetApplicationThread((IApplicationThread**)&at));

    FAIL_RETURN(mInstrumentation->ExecStartActivity(
            mBase, IBinder::Probe(at.Get()), mToken, child, intent, requestCode, options,
            (IInstrumentationActivityResult**)&result));
    if (result != NULL) {
        String id;
        Int32 resultCode;
        AutoPtr<IIntent> resultData;
        child->GetID(&id);
        result->GetResultCode(&resultCode);
        result->GetResultData((IIntent**)&resultData);
        return mMainThread->SendActivityResult(mToken, id,
                requestCode, resultCode, resultData);
    }
    return NOERROR;
}

ECode Activity::StartActivityFromFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode)
{
    return StartActivityFromFragment(fragment, intent, requestCode, NULL);
}

ECode Activity::StartActivityFromFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IBundle* options)
{
    AutoPtr<IInstrumentationActivityResult> result;
    AutoPtr<IApplicationThread> at;
    FAIL_RETURN(mMainThread->GetApplicationThread((IApplicationThread**)&at));

    FAIL_RETURN(mInstrumentation->ExecStartActivity(
            mBase, IBinder::Probe(at.Get()), mToken, fragment, intent, requestCode, options,
            (IInstrumentationActivityResult**)&result));
    if (result != NULL) {
        Int32 resultCode;
        AutoPtr<IIntent> resultData;
        result->GetResultCode(&resultCode);
        result->GetResultData((IIntent**)&resultData);

        String who;
        fragment->GetWho(&who);
        mMainThread->SendActivityResult(mToken, who,
                requestCode, resultCode, resultData);
    }
    return NOERROR;
}

ECode Activity::StartIntentSenderFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return StartIntentSenderFromChild(child, intent, requestCode, fillInIntent,
            flagsMask, flagsValues, extraFlags, NULL);
}

ECode Activity::StartIntentSenderFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntentSender* intent,
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return StartIntentSenderForResultInner(intent, requestCode, fillInIntent,
            flagsMask, flagsValues, child, options);
}

ECode Activity::OverridePendingTransition(
    /* [in] */ Int32 enterAnim,
    /* [in] */ Int32 exitAnim)
{
//    try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    String pkg;
    GetPackageName(&pkg);
    ECode ec = defaultAM->OverridePendingTransition(mToken, pkg, enterAnim, exitAnim);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//    }
}

ECode Activity::SetResult(
    /* [in] */ Int32 resultCode)
{
    mResultCode = resultCode;
    mResultData = NULL;

    return NOERROR;
}

ECode Activity::SetResult(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data)
{
    mResultCode = resultCode;
    mResultData = data;

    return NOERROR;
}

ECode Activity::GetCallingPackage(
    /* [out] */ String* package)
{
    VALIDATE_NOT_NULL(package);
//    try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    ECode ec = defaultAM->GetCallingPackage(mToken, package);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//    }
}

ECode Activity::GetCallingActivity(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
//    try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    ECode ec = defaultAM->GetCallingActivity(mToken, name);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//    }
}

ECode Activity::SetVisible(
    /* [in] */ Boolean visible)
{
    if (mVisibleFromClient != visible) {
        mVisibleFromClient = visible;
        if (mVisibleFromServer) {
            if (visible) return MakeVisible();
            else return mDecor->SetVisibility(IView::INVISIBLE);
        }
    }

    return NOERROR;
}

ECode Activity::MakeVisible()
{
    if (!mWindowAdded) {
        AutoPtr<IWindowManagerLayoutParams> wmlp;
        FAIL_RETURN(GetWindow()->GetAttributes((IWindowManagerLayoutParams**)&wmlp));
        AutoPtr<IViewManager> wm = GetWindowManager();
        FAIL_RETURN(wm->AddView(mDecor, wmlp));
        mWindowAdded = TRUE;
    }
    return mDecor->SetVisibility(IView::VISIBLE);
}

ECode Activity::IsFinishing(
    /* [out] */ Boolean* finished)
{
    VALIDATE_NOT_NULL(finished);
    *finished = mFinished;
    return NOERROR;
}

ECode Activity::IsDestroyed(
    /* [out] */ Boolean* destroyed)
{
    VALIDATE_NOT_NULL(destroyed);
    *destroyed = mDestroyed;
    return NOERROR;
}

ECode Activity::IsChangingConfigurations(
    /* [out] */ Boolean* changed)
{
    VALIDATE_NOT_NULL(changed);
    *changed = mChangingConfigurations;
    return NOERROR;
}

ECode Activity::Recreate()
{
    if (mParent != NULL) {
        Logger::E(TAG, "Can only be called on top-level activity");
        //throw new IllegalStateException("Can only be called on top-level activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<ILooper> myLooper;
#ifdef DROID_CORE
    myLooper = Looper::MyLooper();
#else
    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    helper->MyLooper((ILooper**)&myLooper);
#endif
    AutoPtr<ILooper> mainLooper;
    mMainThread->GetLooper((ILooper**)&mainLooper);
    if (myLooper != mainLooper) {
        Logger::E(TAG, "Must be called from main thread");
        //throw new IllegalStateException("Must be called from main thread");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return mMainThread->RequestRelaunchActivity(mToken, NULL, NULL, 0, FALSE, NULL, FALSE);
}

ECode Activity::Finish()
{
    if (mParent == NULL) {
        Int32 resultCode;
        AutoPtr<IIntent> resultData;
        {
            AutoLock lock(mThisLock);
            resultCode = mResultCode;
            resultData = mResultData;
        }

        if (FALSE) {
            String tokenStr;
            mToken->ToString(&tokenStr);
            StringBuilder sb("Finishing self: token=");
            sb += tokenStr;
            Logger::V(TAG, sb.ToString().string());
        }

        if (resultData != NULL) {
            FAIL_RETURN(resultData->SetAllowFds(FALSE));
        }

        AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
        Boolean finished;
        ECode ec = defaultAM->FinishActivity(mToken, resultCode, resultData, &finished);
        if (finished) {
            mFinished = TRUE;
        }

        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            ec = NOERROR;
        }
        return ec;
    }
    else {
        return mParent->FinishFromChild(THIS_PROBE(IActivity));
    }
}

ECode Activity::FinishAffinity()
{
    if (mParent != NULL) {
        Logger::E(TAG, "Can not be called from an embedded activity");
        //throw new IllegalStateException("Can not be called from an embedded activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mResultCode != IActivity::RESULT_CANCELED || mResultData != NULL) {
        Logger::E(TAG, "Can not be called to deliver a result");
        //throw new IllegalStateException("Can not be called to deliver a result");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    //try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
    Boolean finished;
    ECode ec = defaultAM->FinishActivityAffinity(mToken,&finished);
    if (finished) {
        mFinished = TRUE;
    }

    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//    // Empty
//    }
}

ECode Activity::FinishFromChild(
    /* [in] */ IActivity* child)
{
    return Finish();
}

ECode Activity::FinishActivity(
    /* [in] */ Int32 requestCode)
{
    if (mParent == NULL) {
//        try {
            AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
            ECode ec = defaultAM->FinishSubActivity(mToken, mEmbeddedID, requestCode);
            if (ec == (ECode)E_REMOTE_EXCEPTION) {
                ec = NOERROR;
            }
            return ec;
//        } catch (RemoteException e) {
//            // Empty
//        }
    } else {
        return mParent->FinishActivityFromChild(THIS_PROBE(IActivity), requestCode);
    }
}

ECode Activity::FinishActivityFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ Int32 requestCode)
{
    VALIDATE_NOT_NULL(child);
//try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
    String childID;
    child->GetID(&childID);
    ECode ec = defaultAM->FinishSubActivity(mToken, childID, requestCode);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//    // Empty
//    }
}

ECode Activity::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}

ECode Activity::CreatePendingResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ IIntent* data,
    /* [in] */ Int32 flags,
    /* [out] */ IPendingIntent** pIntent)
{
    VALIDATE_NOT_NULL(pIntent);
    *pIntent = NULL;
    VALIDATE_NOT_NULL(data);

    String packageName;
    GetPackageName(&packageName);

//    try {
    data->SetAllowFds(FALSE);

#ifdef DROID_CORE
    Int32 myUserId = CUserHandle::GetMyUserId();
#else
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
#endif
    AutoPtr<IBinder> token;
    if (mParent == NULL) {
        token = mToken;
    }
    else {
        mParent->GetToken((IBinder**)&token);
    }
    AutoPtr< ArrayOf<IIntent*> > intents = ArrayOf<IIntent*>::Alloc(1);
    intents->Set(0, data);

    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
    AutoPtr<IIIntentSender> target;
    ECode ec = defaultAM->GetIntentSender(
            IActivityManager::INTENT_SENDER_ACTIVITY_RESULT,
            packageName, token, mEmbeddedID, requestCode,
            intents, NULL, flags, NULL, myUserId,
            (IIIntentSender**)&target);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    FAIL_RETURN(ec);
    if (target != NULL) {
        return CPendingIntent::New(target, pIntent);
    }

//    } catch (RemoteException e) {
//    // Empty
//    }
    return NOERROR;
}

ECode Activity::SetRequestedOrientation(
    /* [int] */ Int32 requestedOrientation)
{
    if (mParent == NULL) {
        AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

        ECode ec = defaultAM->SetRequestedOrientation(mToken, requestedOrientation);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            ec = NOERROR;
        }
        return ec;
    }
    else {
        return mParent->SetRequestedOrientation(requestedOrientation);
    }
}

ECode Activity::GetRequestedOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = IActivityInfo::SCREEN_ORIENTATION_UNSPECIFIED;

    if (mParent == NULL) {
        AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

        ECode ec = defaultAM->GetRequestedOrientation(mToken, orientation);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            ec = NOERROR;
        }
        return ec;
    }
    else {
        return mParent->GetRequestedOrientation(orientation);
    }

    return NOERROR;
}

ECode Activity::GetTaskId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = -1;

    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    ECode ec = defaultAM->GetTaskForActivity(mToken, FALSE, id);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
}

ECode Activity::IsTaskRoot(
    /* [out] */ Boolean* isRoot)
{
    VALIDATE_NOT_NULL(isRoot);
    *isRoot = FALSE;

    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    Int32 task = -1;
    ECode ec = defaultAM->GetTaskForActivity(mToken, TRUE, &task);
    *isRoot = task >= 0;
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        *isRoot = FALSE;
        ec = NOERROR;
    }
    return ec;
}

ECode Activity::MoveTaskToBack(
    /* [in] */ Boolean nonRoot,
    /* [out] */ Boolean* succeed)
{
    VALIDATE_NOT_NULL(succeed);
    *succeed = FALSE;

    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
    ECode ec = defaultAM->MoveActivityTaskToBack(mToken, nonRoot, succeed);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
}

ECode Activity::GetLocalClassName(
    /* [out] */ String* clsName)
{
    VALIDATE_NOT_NULL(clsName);

    String pkg, cls;
    FAIL_RETURN(GetPackageName(&pkg));
    FAIL_RETURN(mComponent->GetClassName(&cls));
    Int32 packageLen = pkg.GetLength();
    if (!cls.StartWith(pkg) || (Int32)cls.GetLength() <= packageLen
            || cls.GetChar(packageLen) != '.') {
        *clsName = cls;
        return NOERROR;
    }
    *clsName = cls.Substring(packageLen + 1);
    return NOERROR;
}

ECode Activity::GetComponentName(
    /* [out] */ IComponentName** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mComponent;
    REFCOUNT_ADD(*name)
    return NOERROR;
}

ECode Activity::GetPreferences(
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** perferences)
{
    VALIDATE_NOT_NULL(perferences);

    String clsName;
    GetLocalClassName(&clsName);
    return GetSharedPreferences(clsName, mode, perferences);
}

ECode Activity::EnsureSearchManager()
{
    if (mSearchManager == NULL) {
        // TODO
//        return CSearchManager::New(THIS_PROBE(IActivity), NULL,
//                (ISearchManager**)&mSearchManager);
    }

    return NOERROR;
}

ECode Activity::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    FAIL_RETURN(OnTitleChanged(title, mTitleColor));

    if (mParent != NULL) {
        return mParent->OnChildTitleChanged(THIS_PROBE(IActivity), title);
    }
    return NOERROR;
}

ECode Activity::SetTitle(
    /* [in] */ Int32 titleId)
{
    AutoPtr<ICharSequence> title;
    FAIL_RETURN(GetText(titleId, (ICharSequence**)&title));
    return SetTitle(title);
}

ECode Activity::SetTitleColor(
    /* [in] */ Int32 textColor)
{
    mTitleColor = textColor;
    OnTitleChanged(mTitle, textColor);
    return NOERROR;
}

ECode Activity::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    *title = mTitle;
    REFCOUNT_ADD(*title)
    return NOERROR;
}

ECode Activity::GetTitleColor(
    /* [out] */ Int32* textColor)
{
    VALIDATE_NOT_NULL(textColor);
    *textColor = mTitleColor;
    return NOERROR;
}

ECode Activity::OnTitleChanged(
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 color)
{
    if (mTitleReady) {
        AutoPtr<IWindow> win = GetWindow();
        if (win != NULL) {
            FAIL_RETURN(win->SetTitle(title));
            if (color != 0) {
                FAIL_RETURN(win->SetTitleColor(color));
            }
        }
    }
    return NOERROR;
}

ECode Activity::OnChildTitleChanged(
    /* [in] */ IActivity* childActivity,
    /* [in] */ ICharSequence* title)
{
    return NOERROR;
}

ECode Activity::SetProgressBarVisibility(
    /* [in] */ Boolean visible)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureInt(IWindow::FEATURE_PROGRESS,
            visible ? IWindow::PROGRESS_VISIBILITY_ON : IWindow::PROGRESS_VISIBILITY_OFF);
}

ECode Activity::SetProgressBarIndeterminateVisibility(
    /* [in] */ Boolean visible)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureInt(IWindow::FEATURE_INDETERMINATE_PROGRESS,
            visible ? IWindow::PROGRESS_VISIBILITY_ON : IWindow::PROGRESS_VISIBILITY_OFF);

}

ECode Activity::SetProgressBarIndeterminate(
    /* [in] */ Boolean visible)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureInt(IWindow::FEATURE_PROGRESS,
            visible ? IWindow::PROGRESS_INDETERMINATE_ON : IWindow::PROGRESS_INDETERMINATE_OFF);
}

ECode Activity::SetProgress(
    /* [in] */ Int32 progress)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureInt(IWindow::FEATURE_PROGRESS,
            progress + IWindow::PROGRESS_START);
}

ECode Activity::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetFeatureInt(IWindow::FEATURE_PROGRESS,
            secondaryProgress + IWindow::PROGRESS_SECONDARY_START);
}

ECode Activity::SetVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->SetVolumeControlStream(streamType);
}

ECode Activity::GetVolumeControlStream(
    /* [out] */ Int32* type)
{
    AutoPtr<IWindow> win = GetWindow();
    return win->GetVolumeControlStream(type);
}

ECode Activity::RunOnUiThread(
    /* [in] */ IRunnable* action)
{
    VALIDATE_NOT_NULL(action);

    if (Thread::GetCurrentThread() != mUiThread) {
        Boolean result;
        return mHandler->Post(action, &result);
    }
    else {
        return action->Run();
    }
}

ECode Activity::OnCreateView(
    /* [in] */ const String& name,
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    return NOERROR;
}

ECode Activity::OnCreateView(
    /* [in] */ IView* parent,
    /* [in] */ const String& name,
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    VALIDATE_NOT_NULL(attrs);

    if (name != "Fragment") {
        return OnCreateView(name, context, attrs, view);
    }

    String fname;
    attrs->GetAttributeValue(String(NULL), String("class"), &fname);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Fragment),
            ARRAY_SIZE(R::styleable::Fragment));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
    if (fname.IsNull()) {
        a->GetString(R::styleable::Fragment_name, &fname);
    }
    Int32 id;
    a->GetResourceId(R::styleable::Fragment_id, IView::NO_ID, &id);
    String tag;
    a->GetString(R::styleable::Fragment_tag, &tag);
    a->Recycle();

    Int32 parentId = 0;
    if (parent != NULL) {
        parent->GetId(&parentId);
    }
    Int32 containerId = parentId;
    if (containerId == IView::NO_ID && id == IView::NO_ID && tag.IsNull()) {
//        throw new IllegalArgumentException(attrs.getPositionDescription()
//                + ": Must specify unique android:id, android:tag, or have a parent with an id for " + fname);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // If we restored from a previous state, we may already have
    // instantiated this fragment from the state and should use
    // that instance instead of making a new one.
    AutoPtr<IFragment> fragment;
    if (id != IView::NO_ID) {
        FAIL_RETURN(mFragments->FindFragmentById(id, (IFragment**)&fragment));
    }
    if (fragment == NULL && !tag.IsNull()) {
        FAIL_RETURN(mFragments->FindFragmentByTag(tag, (IFragment**)&fragment));
    }
    if (fragment == NULL && containerId != IView::NO_ID) {
        FAIL_RETURN(mFragments->FindFragmentById(containerId, (IFragment**)&fragment));
    }
    AutoPtr<IFragmentManagerImplHelper> fHelper;
    CFragmentManagerImplHelper::AcquireSingleton((IFragmentManagerImplHelper**)&fHelper);
    Boolean debug;
    fHelper->GetDEBUG(&debug);
    if (debug) {
        StringBuilder info("onCreateView: id=0x");
        info += StringUtils::Int32ToString(id, 16);
        info += " fname=";
        info += fname;
        info += " existing=";
        info += fragment;
        Logger::V(TAG, info.ToString());
    }

    Boolean inLayout;
    if (fragment != NULL) {
        fragment->IsInLayout(&inLayout);
    }
    if (fragment == NULL) {
        Fragment::Instantiate(THIS_PROBE(IContext), fname, (IFragment**)&fragment);
        fragment->SetFromLayout(TRUE);
        fragment->SetFragmentId(id != 0 ? id : containerId);
        fragment->SetContainerId(containerId);
        fragment->SetTag(tag);
        fragment->SetInLayout(TRUE);
        fragment->SetFragmentManager(mFragments);
        AutoPtr<IBundle> savedFragmentState;
        fragment->GetSavedFragmentState((IBundle**)&savedFragmentState);
        fragment->OnInflate(THIS_PROBE(IActivity), attrs, savedFragmentState);
        mFragments->AddFragment(fragment, TRUE);
    }
    else if (inLayout) {
        // A fragment already exists and it is not one we restored from
        // previous state.
//        throw new IllegalArgumentException(attrs.getPositionDescription()
//                + ": Duplicate id 0x" + Integer.toHexString(id)
//                + ", tag " + tag + ", or parent id 0x" + Integer.toHexString(containerId)
//                + " with another fragment for " + fname);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        // This fragment was retained from a previous instance; get it
        // going now.
        fragment->SetInLayout(TRUE);
        // If this fragment is newly instantiated (either right now, or
        // from last saved state), then give it the attributes to
        // initialize itself.
        Boolean retaining;
        fragment->GetRetaining(&retaining);
        AutoPtr<IBundle> savedFragmentState;
        fragment->GetSavedFragmentState((IBundle**)&savedFragmentState);
        if (!retaining) {
            fragment->OnInflate(THIS_PROBE(IActivity), attrs, savedFragmentState);
        }
        FAIL_RETURN(mFragments->MoveToState(fragment));
    }

    AutoPtr<IView> fview;
    fragment->GetView((IView**)&fview);
    if (fview == NULL) {
//        throw new IllegalStateException("Fragment " + fname
//                + " did not create a view.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (id != 0) {
       fview->SetId(id);
    }

    AutoPtr<IInterface> viewTag;
    fview->GetTag((IInterface**)&viewTag);
    if (viewTag == NULL) {
        AutoPtr<ICharSequence> tagcs;
        if (tag.IsNull()) fview->SetTag(NULL);
        else {
            CStringWrapper::New(tag, (ICharSequence**)&tagcs);
            fview->SetTag(tagcs);
        }
    }

    *view = fview;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode Activity::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return DumpInner(prefix, fd, writer, args);
}

ECode Activity::DumpInner(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    writer->PrintString(prefix); writer->PrintString(String("Local Activity "));
            //writer->PrintString(Integer.toHexString(System.identityHashCode(this)));
            writer->PrintStringln(String(" State:"));
    String innerPrefix = prefix + "  ";
    writer->PrintString(innerPrefix); writer->PrintString(String("mResumed="));
            writer->PrintBoolean(mResumed); writer->PrintString(String(" mStopped="));
            writer->PrintBoolean(mStopped); writer->PrintString(String(" mFinished="));
            writer->PrintBooleanln(mFinished);
    writer->PrintString(innerPrefix); writer->PrintString(String("mLoadersStarted="));
            writer->PrintBooleanln(mLoadersStarted);
    writer->PrintString(innerPrefix); writer->PrintString(String("mChangingConfigurations="));
            writer->PrintBooleanln(mChangingConfigurations);
    writer->PrintString(innerPrefix); writer->PrintString(String("mCurrentConfig="));
            String temp;
            mCurrentConfig->ToString(&temp);
            writer->PrintStringln(temp);
    if (mLoaderManager != NULL) {
        writer->PrintString(prefix); writer->PrintString(String("Loader Manager "));
                //writer->print(Integer.toHexString(System.identityHashCode(mLoaderManager)));
                writer->PrintStringln(String(":"));
        mLoaderManager->Dump(String(prefix + "  "), fd, writer, args);
    }
    FAIL_RETURN(mFragments->Dump(prefix, fd, writer, args));
    writer->PrintString(prefix); writer->PrintStringln(String("View Hierarchy:"));
    AutoPtr<IView> decorView;
    GetWindow()->GetDecorView((IView**)&decorView);
    DumpViewHierarchy(String(prefix + "  "), writer, decorView);
    return NOERROR;
}

ECode Activity::DumpViewHierarchy(
    /* [in] */ const String& prefix,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ IView* view)
{
    writer->PrintString(prefix);
    if (view == NULL) {
        writer->PrintStringln(String("NULL"));
        return NOERROR;
    }

//    writer->println(view.toString());
    if (!(view->Probe(EIID_IViewGroup))) {
        return NOERROR;
    }
    AutoPtr<IViewGroup> grp = (IViewGroup*)view->Probe(EIID_IViewGroup);
    Int32 N;
    grp->GetChildCount(&N);
    if (N <= 0) {
        return NOERROR;
    }
    String prefixtemp = prefix + "  ";
    for (Int32 i=0; i<N; i++) {
        AutoPtr<IView> v;
        grp->GetChildAt(i, (IView**)&v);
        DumpViewHierarchy(prefixtemp, writer, v);
    }
    return NOERROR;
}

ECode Activity::IsImmersive(
    /* [out] */ Boolean* isImmersive)
{
    VALIDATE_NOT_NULL(isImmersive);
    *isImmersive = FALSE;
//    try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
    ECode ec = defaultAM->IsImmersive(mToken, isImmersive);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//        return false;
//    }
}

ECode Activity::SetImmersive(
    /* [in] */ Boolean i)
{
//    try {
    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    ECode ec = defaultAM->SetImmersive(mToken, i);
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        ec = NOERROR;
    }
    return ec;
//    } catch (RemoteException e) {
//        // pass
//    }
}

ECode Activity::StartActionMode(
    /* [in] */ IActionModeCallback* callback,
    /* [out] */ IActionMode** mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = NULL;

    AutoPtr<IWindow> win = GetWindow();
    AutoPtr<IView> decor;
    FAIL_RETURN(win->GetDecorView((IView**)&decor));
    return decor->StartActionMode(callback, mode);
}

ECode Activity::OnWindowStartingActionMode(
    /* [in] */ IActionModeCallback* callback,
    /* [out] */ IActionMode** mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = NULL;

    FAIL_RETURN(InitActionBar());
    if (mActionBar) {
        return mActionBar->StartActionMode(callback, mode);
    }
    return NOERROR;
}

ECode Activity::OnActionModeStarted(
    /* [in] */ IActionMode* mode)
{
    return NOERROR;
}

ECode Activity::OnActionModeFinished(
    /* [in] */ IActionMode* mode)
{
    return NOERROR;
}

ECode Activity::ShouldUpRecreateTask(
    /* [in] */ IIntent* targetIntent,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(targetIntent);

//    try {
    AutoPtr<IPackageManager> pm;
    AutoPtr<IComponentName> cn;
    FAIL_RETURN(GetPackageManager((IPackageManager**)&pm));
    FAIL_RETURN(targetIntent->GetComponent((IComponentName**)&cn));
    if (cn == NULL) {
        FAIL_RETURN(targetIntent->ResolveActivity(pm, (IComponentName**)&cn));
    }

    AutoPtr<IActivityInfo> info;
    FAIL_RETURN(pm->GetActivityInfo(cn, 0, (IActivityInfo**)&info));
    String affinity;
    FAIL_RETURN(info->GetTaskAffinity(&affinity));
    if (affinity.IsNull()) {
        return NOERROR;
    }

    AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();

    Boolean val;
    ECode ec = defaultAM->TargetTaskAffinityMatchesActivity(mToken, affinity, &val);
    *result = !val;
    if (ec == (ECode)E_REMOTE_EXCEPTION || ec == (ECode)E_NAME_NOT_FOUND_EXCEPTION) {
        *result = FALSE;
        ec = NOERROR;
    }
    return NOERROR;
//    } catch (RemoteException e) {
//        return false;
//    } catch (NameNotFoundException e) {
//        return false;
//    }
}

ECode Activity::NavigateUpTo(
    /* [in] */ IIntent* inUpIntent,
    /* [out] */ Boolean* success)
{
    VALIDATE_NOT_NULL(success);
    *success = FALSE;
    VALIDATE_NOT_NULL(inUpIntent);

    if (mParent == NULL) {
        AutoPtr<IIntent> upIntent = inUpIntent;

        AutoPtr<IComponentName> destInfo;
        upIntent->GetComponent((IComponentName**)&destInfo);
        if (destInfo == NULL) {
            AutoPtr<IPackageManager> pm;
            FAIL_RETURN(GetPackageManager((IPackageManager**)&pm));
            FAIL_RETURN(upIntent->ResolveActivity(pm, (IComponentName**)&destInfo));
            if (destInfo == NULL) {
                return NOERROR;
            }

            upIntent = NULL;
            CIntent::New(inUpIntent, (IIntent**)&upIntent);
            upIntent->SetComponent(destInfo);
            inUpIntent = upIntent;
        }
        Int32 resultCode;
        AutoPtr<IIntent> resultData;
        {
            AutoLock lock(mThisLock);
            resultCode = mResultCode;
            resultData = mResultData;
        }
        if (resultData != NULL) {
            resultData->SetAllowFds(FALSE);
        }
//        try {
        AutoPtr<IIActivityManager> defaultAM = ActivityManagerNative::GetDefault();
        return defaultAM->NavigateUpTo(mToken, inUpIntent, resultCode, resultData, success);
//        } catch (RemoteException e) {
//            return false;
//        }
    }
    else {
        return mParent->NavigateUpToFromChild(THIS_PROBE(IActivity), inUpIntent, success);
    }
}

ECode Activity::NavigateUpToFromChild(
    /* [in] */ IActivity* child,
    /* [in] */ IIntent* upIntent,
    /* [out] */ Boolean* success)
{
    return NavigateUpTo(upIntent, success);
}

ECode Activity::GetParentActivityIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = NULL;

    String parentName;
    FAIL_RETURN(mActivityInfo->GetParentActivityName(&parentName));

    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(parentName, (ICharSequence**)&seq));

    if (TextUtils::IsEmpty(seq)) {
        return NOERROR;
    }

    // If the parent itself has no parent, generate a main activity intent.
    AutoPtr<IComponentName> target;
    CComponentName::New(THIS_PROBE(IActivity), parentName, (IComponentName**)&target);
//    try {
    AutoPtr<IPackageManager> pm;
    FAIL_RETURN(GetPackageManager((IPackageManager**)&pm));
    AutoPtr<IActivityInfo> parentInfo;
    FAIL_RETURN(pm->GetActivityInfo(target, 0, (IActivityInfo**)&parentInfo));
    String parentActivity;
    FAIL_RETURN(parentInfo->GetParentActivityName(&parentActivity));

    AutoPtr<IIntent> parentIntent;
    if (parentActivity.IsNull()) {
        AutoPtr<IIntentHelper> helper;
        CIntentHelper::AcquireSingleton((IIntentHelper**)&helper);
        FAIL_RETURN(helper->MakeMainActivity(target, (IIntent**)&parentIntent));
    }
    else {
        AutoPtr<IIntent> obj;
        CIntent::New((IIntent**)&obj);
        FAIL_RETURN(obj->SetComponent(target));
        parentIntent = obj;
    }

    *intent = parentIntent;
    REFCOUNT_ADD(*intent)
    return NOERROR;
//    } catch (NameNotFoundException e) {
//        Log.e(TAG, "getParentActivityIntent: bad parentActivityName '" + parentName +
//                "' in manifest");
//        return null;
//    }
}

ECode Activity::SetParent(
    /* [in] */ IActivity* parent)
{
    mParent = parent;
    return NOERROR;
}

ECode Activity::Attach(
    /* [in] */ IContext* context,
    /* [in] */ IActivityThread* aThread,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstances,
    /* [in] */ IConfiguration* config)
{
    return Attach(context, aThread, instr, token, 0, application,
            intent, info, title, parent, id, lastNonConfigurationInstances, config);
}

ECode Activity::Attach(
    /* [in] */ IContext* context,
    /* [in] */ IActivityThread* aThread,
    /* [in] */ IInstrumentation* instr,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 ident,
    /* [in] */ IApplication* application,
    /* [in] */ IIntent* intent,
    /* [in] */ IActivityInfo* info,
    /* [in] */ ICharSequence* title,
    /* [in] */ IActivity* parent,
    /* [in] */ const String& id,
    /* [in] */ IInterface* lastNonConfigurationInstances,
    /* [in] */ IConfiguration* config)
{
    FAIL_RETURN(AttachBaseContext(context));

    FAIL_RETURN(mFragments->AttachActivity(THIS_PROBE(IActivity), mContainer, NULL));

    AutoPtr<IPolicyManager> pm;
    CPolicyManager::AcquireSingleton((IPolicyManager**)&pm);
    mWindow = NULL;
    FAIL_RETURN(pm->MakeNewWindow(THIS_PROBE(IContext), (IWindow**)&mWindow));
    FAIL_RETURN(mWindow->SetCallback(THIS_PROBE(IWindowCallback)));
    AutoPtr<ILayoutInflater> inflater;
    mWindow->GetLayoutInflater((ILayoutInflater**)&inflater);
    inflater->SetPrivateFactory(THIS_PROBE(ILayoutInflaterFactory2));

    Int32 softInputMode = 0;
    FAIL_RETURN(info->GetSoftInputMode(&softInputMode));
    if (softInputMode != IWindowManagerLayoutParams::SOFT_INPUT_STATE_UNSPECIFIED) {
        FAIL_RETURN(mWindow->SetSoftInputMode(softInputMode));
    }
    Int32 uiOptions = 0;
    info->GetUiOptions(&uiOptions);
    if (uiOptions != 0) {
        FAIL_RETURN(mWindow->SetUiOptions(uiOptions));
    }

    mUiThread = Thread::GetCurrentThread();

    mMainThread = aThread;
    mInstrumentation = instr;
    mToken = token;
    mIdent = ident;
    mApplication = application;
    mIntent = intent;
    mComponent = NULL;
    intent->GetComponent((IComponentName**)&mComponent);
    mActivityInfo = info;
    mTitle = title;
    mParent = parent;
    mEmbeddedID = id;
    if (lastNonConfigurationInstances) {
        mLastNonConfigurationInstances = (IActivityNonConfigurationInstances*)
                lastNonConfigurationInstances->Probe(EIID_IActivityNonConfigurationInstances);
    }

    AutoPtr<IWindowManager> wm;
    FAIL_RETURN(context->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&wm));
    String str;
    mComponent->FlattenToString(&str);
    Int32 flags;
    info->GetFlags(&flags);
    FAIL_RETURN(mWindow->SetWindowManager(wm, mToken, str,
            (flags & IActivityInfo::FLAG_HARDWARE_ACCELERATED) != 0));

    if (mParent != NULL) {
        AutoPtr<IWindow> pWindow;
        FAIL_RETURN(mParent->GetWindow((IWindow**)&pWindow));
        FAIL_RETURN(mWindow->SetContainer(pWindow));
    }

    mWindowManager = NULL;
    FAIL_RETURN(mWindow->GetWindowManager((IWindowManager**)&mWindowManager));
    mCurrentConfig = config;

    return NOERROR;
}

ECode Activity::GetActivityToken(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);

    if (mParent != NULL) {
        return mParent->GetActivityToken(binder);
    }

    *binder = mToken.Get();
    REFCOUNT_ADD(*binder)
    return NOERROR;
}

ECode Activity::PerformCreate(
    /* [in] */ IBundle* icicle)
{
    FAIL_RETURN(OnCreate(icicle));
    AutoPtr<ITypedArray> winStyle;
    FAIL_RETURN(mWindow->GetWindowStyle((ITypedArray**)&winStyle));
    Boolean noDisplay = FALSE;
    winStyle->GetBoolean(R::styleable::Window_windowNoDisplay, FALSE, &noDisplay);
    mVisibleFromClient = !noDisplay;
    FAIL_RETURN(mFragments->DispatchActivityCreated());
    return NOERROR;
}

ECode Activity::PerformStart()
{
    FAIL_RETURN(mFragments->NoteStateNotSaved());
    mCalled = FALSE;
    Boolean exected;
    FAIL_RETURN(mFragments->ExecPendingActions(&exected));
    FAIL_RETURN(mInstrumentation->CallActivityOnStart(THIS_PROBE(IActivity)));
    if (!mCalled) {
        String temp;
        mComponent->ToShortString(&temp);
        Logger::E(TAG, "Activity %s did not call through to super.onStart()", temp.string());
//        throw new SuperNotCalledException(
//            "Activity " + mComponent.toShortString() +
//            " did not call through to super.onStart()");
        return E_SUPER_NOT_CALLED_EXCEPTION;
    }

    FAIL_RETURN(mFragments->DispatchStart());
    if (mAllLoaderManagers != NULL) {
        AutoPtr<IObjectContainer> values;
        mAllLoaderManagers->GetValues((IObjectContainer**)&values);

        if (values != NULL) {
            AutoPtr<IObjectEnumerator> it;
            values->GetObjectEnumerator((IObjectEnumerator**)&it);
            Boolean succeeded;
            while (it->MoveNext(&succeeded), succeeded) {
                AutoPtr<ILoaderManagerImpl> lm;
                it->Current((IInterface**)&lm);
                lm->FinishRetain();
                lm->DoReportStart();
            }
        }
    }

    return NOERROR;
}

ECode Activity::PerformRestart()
{
    FAIL_RETURN(mFragments->NoteStateNotSaved());

    if (mStopped) {
        mStopped = FALSE;

        if (mToken != NULL && mParent == NULL) {
            AutoPtr<IWindowManagerGlobal> wmg;
            CWindowManagerGlobal::AcquireSingleton((IWindowManagerGlobal**)&wmg);
            FAIL_RETURN(wmg->SetStoppedState(mToken, FALSE));
        }

        {
            AutoLock lock(mManagedCursorsLock);

            Int32 targetSdkVersion;
            AutoPtr<IApplicationInfo> appInfo;
            FAIL_RETURN(GetApplicationInfo((IApplicationInfo**)&appInfo));
            FAIL_RETURN(appInfo->GetTargetSdkVersion(&targetSdkVersion));

            AutoPtr<ManagedCursor> mc;
            Boolean bval;
            List<AutoPtr<ManagedCursor> >::Iterator it = mManagedCursors.Begin();
            for (; it != mManagedCursors.End(); ++it) {
                mc = *it;
                if (mc->mReleased || mc->mUpdated) {
                    mc->mCursor->Requery(&bval);
                    if (!bval) {
                        if (targetSdkVersion >= Build::VERSION_CODES::ICE_CREAM_SANDWICH) {
                            Slogger::E(TAG, "trying to requery an already closed cursor.");
                            return E_ILLEGAL_STATE_EXCEPTION;
                        }
                    }
                    mc->mReleased = FALSE;
                    mc->mUpdated = FALSE;
                }
            }
        }

        mCalled = FALSE;
        FAIL_RETURN(mInstrumentation->CallActivityOnRestart(THIS_PROBE(IActivity)));
        if (!mCalled) {
//            throw new SuperNotCalledException(
//                "Activity " + mComponent.toShortString() +
//                " did not call through to super.onRestart()");
            String temp;
            mComponent->ToShortString(&temp);
            Logger::E(TAG, "Activity %s did not call through to super.onStart()", temp.string());
            return E_SUPER_NOT_CALLED_EXCEPTION;
        }
        FAIL_RETURN(PerformStart());
    }
    return NOERROR;
}

ECode Activity::PerformResume()
{
    FAIL_RETURN(PerformRestart());

    Boolean exected;
    FAIL_RETURN(mFragments->ExecPendingActions(&exected));

    mLastNonConfigurationInstances = NULL;

    mCalled = FALSE;
    // mResumed is set by the instrumentation
    FAIL_RETURN(mInstrumentation->CallActivityOnResume(THIS_PROBE(IActivity)));
    if (!mCalled) {
//        throw new SuperNotCalledException(
//            "Activity " + mComponent.toShortString() +
//            " did not call through to super.onResume()");

        String temp;
        mComponent->ToShortString(&temp);
        Logger::E(TAG, "Activity %s did not call through to super.onResume()", temp.string());
        return E_SUPER_NOT_CALLED_EXCEPTION;
    }

    mCalled = FALSE;

    FAIL_RETURN(mFragments->DispatchResume());
    Boolean execresult;
    FAIL_RETURN(mFragments->ExecPendingActions(&execresult));
    FAIL_RETURN(OnPostResume());
    if (!mCalled) {
//        throw new SuperNotCalledException(
//            "Activity " + mComponent.toShortString() +
//            " did not call through to super.onPostResume()");
        StringBuilder sb("Activity ");
        String temp;
        mComponent->ToShortString(&temp);
        sb += temp;
        sb += String(" did not call through to super.onPostResume()");
        Logger::E(TAG, sb.ToString().string());
        return E_SUPER_NOT_CALLED_EXCEPTION;
    }
    return NOERROR;
}

ECode Activity::PerformPause()
{
    FAIL_RETURN(mFragments->DispatchPause());
    mCalled = FALSE;
    FAIL_RETURN(OnPause());
    mResumed = FALSE;
    Int32 targetSdkVersion;
    AutoPtr<IApplicationInfo> appInfo;
    FAIL_RETURN(GetApplicationInfo((IApplicationInfo**)&appInfo));
    FAIL_RETURN(appInfo->GetTargetSdkVersion(&targetSdkVersion));

    if (!mCalled && targetSdkVersion >= Build::VERSION_CODES::GINGERBREAD) {
//        throw new SuperNotCalledException(
//                "Activity " + mComponent.toShortString() +
//                " did not call through to super.onPause()");
        StringBuilder sb("Activity ");
        String temp;
        mComponent->ToShortString(&temp);
        sb += temp;
        sb += String(" did not call through to super.onPause()");
        Logger::E(TAG, sb.ToString().string());
        return E_SUPER_NOT_CALLED_EXCEPTION;
    }
    mResumed = FALSE;
    return NOERROR;
}

ECode Activity::PerformUserLeaving()
{
    FAIL_RETURN(OnUserInteraction());
    return OnUserLeaveHint();
}

ECode Activity::PerformStop()
{
    if (mLoadersStarted) {
        mLoadersStarted = FALSE;
        if (mLoaderManager != NULL) {
            if (!mChangingConfigurations) {
                mLoaderManager->DoStop();
            } else {
                mLoaderManager->DoRetain();
            }
        }
    }

    if (!mStopped) {
        if (mWindow != NULL) {
            mWindow->CloseAllPanels();
        }
        if (mToken != NULL && mParent == NULL) {
            AutoPtr<IWindowManagerGlobal> wmg;
            CWindowManagerGlobal::AcquireSingleton((IWindowManagerGlobal**)&wmg);
            FAIL_RETURN(wmg->SetStoppedState(mToken, TRUE));
        }

        FAIL_RETURN(mFragments->DispatchStop());

        mCalled = FALSE;
        FAIL_RETURN(mInstrumentation->CallActivityOnStop(THIS_PROBE(IActivity)));
        if (!mCalled) {
//            throw new SuperNotCalledException(
//                "Activity " + mComponent.toShortString() +
//                " did not call through to super.onStop()");
            StringBuilder sb("Activity ");
            String temp;
            mComponent->ToShortString(&temp);
            sb += temp;
            sb += String(" did not call through to super.onStop()");
            Logger::E(TAG, sb.ToString().string());
            return E_SUPER_NOT_CALLED_EXCEPTION;
        }

        {
            AutoLock lock(mManagedCursorsLock);
            AutoPtr<ManagedCursor> mc;
            List<AutoPtr<ManagedCursor> >::Iterator it = mManagedCursors.Begin();
            for (; it != mManagedCursors.End(); ++it) {
                mc = *it;
                if (!mc->mReleased) {
                    mc->mCursor->Deactivate();
                    mc->mReleased = TRUE;
                }
            }
        }

        mStopped = TRUE;
    }
    mResumed = FALSE;
    return NOERROR;
}

ECode Activity::PerformDestroy()
{
    mDestroyed = true;
    mWindow->Destroy();
    FAIL_RETURN(mFragments->DispatchDestroy());
    OnDestroy();
    if (mLoaderManager != NULL) {
        mLoaderManager->DoDestroy();
    }
    return NOERROR;
}

ECode Activity::IsResumed(
    /* [out] */ Boolean* isResumed)
{
    VALIDATE_NOT_NULL(isResumed);
    *isResumed = mResumed;
    return NOERROR;
}

ECode Activity::DispatchActivityResult(
    /* [in] */ const String& who,
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    if (FALSE) {
        Logger::V(TAG, "Dispatching result: who=%s, reqCode=%d, resCode=%d, data=%p", who.string(), requestCode,
                resultCode, data);
    }

    FAIL_RETURN(mFragments->NoteStateNotSaved());
    if (who.IsNull()) {
        return OnActivityResult(requestCode, resultCode, data);
    }
    else {
        AutoPtr<IFragment> frag;
        FAIL_RETURN(mFragments->FindFragmentByWho(who, (IFragment**)&frag));
        if (frag != NULL) {
            return frag->OnActivityResult(requestCode, resultCode, data);
        }
    }
    return NOERROR;
}

ECode Activity::SetStartedActivity(
    /* [in] */ Boolean started)
{
    mStartedActivity = started;
    return NOERROR;
}

ECode Activity::DispatchNewIntent(
    /* [in] */ IIntent *intent)
{
    return OnNewIntent(intent);
}

ECode Activity::SetCalled(
    /* [in] */ Boolean called)
{
    mCalled = called;
    return NOERROR;
}

ECode Activity::IsCalled(
    /* [out] */ Boolean* called)
{
    VALIDATE_NOT_NULL(called);
    *called = mCalled;
    return NOERROR;
}

ECode Activity::SetFinishing(
    /* [in] */ Boolean finished)
{
    mFinished = finished;
    return NOERROR;
}


ECode Activity::SetConfigChangeFlags(
    /* [in] */ Int32 configChangeFlags)
{
    mConfigChangeFlags = configChangeFlags;
    return NOERROR;
}

ECode Activity::GetConfigChangeFlags(
    /* [out] */ Int32* configChangeFlags)
{
    VALIDATE_NOT_NULL(configChangeFlags);
    *configChangeFlags = mConfigChangeFlags;
    return NOERROR;
}

ECode Activity::InitializeTheme()
{
    Boolean first = mTheme == NULL;
    if (first) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->NewTheme((IResourcesTheme**)&mTheme);
        AutoPtr<IResourcesTheme> theme;
        mBase->GetTheme((IResourcesTheme**)&theme);
        if (theme != NULL) {
            mTheme->SetTo(theme);
        }
    }
    return OnApplyThemeResource(mTheme, mThemeResource, first);
}

ECode Activity::SetResumed(
    /* [in] */ Boolean resumed)
{
    mResumed = resumed;
    return NOERROR;
}

ECode Activity::GetID(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mEmbeddedID;
    return NOERROR;
}

ECode Activity::GetToken(
    /* [out] */ IBinder** token)
{
    VALIDATE_NOT_NULL(token);
    *token = mToken;
    REFCOUNT_ADD(*token)
    return NOERROR;
}

// internal use.
ECode Activity::IsStartedActivity(
    /* [out] */ Boolean* isStartedActivity)
{
    VALIDATE_NOT_NULL(isStartedActivity);
    *isStartedActivity = mStartedActivity;
    return NOERROR;
}

ECode Activity::GetWindow(
    /* [out] */ IWindow** window)
{
    VALIDATE_NOT_NULL(window);
    *window = mWindow.Get();
    REFCOUNT_ADD(*window)
    return NOERROR;
}

ECode Activity::SetDecorView(
    /* [in] */ IView* decor)
{
    mDecor = decor;
    return NOERROR;
}

ECode Activity::GetDecorView(
    /* [out] */ IView** decor)
{
    VALIDATE_NOT_NULL(decor);
    *decor = mDecor.Get();
    REFCOUNT_ADD(*decor)
    return NOERROR;
}

ECode Activity::SetVisibleFromServer(
    /* [in] */ Boolean visible)
{
    mVisibleFromServer = visible;
    return NOERROR;
}

ECode Activity::IsVisibleFromServer(
    /* [out] */ Boolean * visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = mVisibleFromServer;
    return NOERROR;
}

ECode Activity::IsVisibleFromClient(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = mVisibleFromClient;
    return NOERROR;
}

ECode Activity::SetWindowAdded(
    /* [in] */ Boolean added)
{
    mWindowAdded = added;
    return NOERROR;
}

ECode Activity::IsWindowAdded(
    /* [out] */ Boolean* added)
{
    VALIDATE_NOT_NULL(added);
    *added = mWindowAdded;
    return NOERROR;
}

ECode Activity::GetWindowManager(
    /* [out] */ IWindowManager** mgr)
{
    VALIDATE_NOT_NULL(mgr);
    *mgr = mWindowManager.Get();
    REFCOUNT_ADD(*mgr)
    return NOERROR;
}

EXTERN_C ECode GrafixInit();

ECode Activity::Create(
    /* [in] */ IBundle* savedInstanceState)
{
    return OnCreate(savedInstanceState);
}

ECode Activity::PostCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return OnPostCreate(savedInstanceState);
}

ECode Activity::Start()
{
    return OnStart();
}

ECode Activity::Restart()
{
    return OnRestart();
}

ECode Activity::Resume()
{
    return OnResume();
}

ECode Activity::Pause()
{
    return OnPause();
}

ECode Activity::Stop()
{
    return OnStop();
}

ECode Activity::Destroy()
{
    return OnDestroy();
}

//
// IContext interfaces
//
ECode Activity::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    if (mBase != NULL) {
        Slogger::E(TAG, "Base context already set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mBase = newBase;
    return NOERROR;
}

ECode Activity::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);

    *ctx = mBase;
    REFCOUNT_ADD(*ctx)
    return NOERROR;
}

ECode Activity::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return mBase->GetAssets(assetManager);
}

ECode Activity::GetResources(
    /* [out] */ IResources** resources)
{
    VALIDATE_NOT_NULL(resources);
    if (mResources != NULL) {
        *resources = mResources;
        REFCOUNT_ADD(*resources)
        return NOERROR;
    }

    if (mOverrideConfiguration == NULL) {
        return mBase->GetResources(resources);
    }
    else {
        AutoPtr<IContext> resc;
        CreateConfigurationContext(mOverrideConfiguration, (IContext**)&resc);
        resc->GetResources(resources);
        mResources = *resources;
        return NOERROR;
    }
}

ECode Activity::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    return mBase->GetPackageManager(packageManager);
}

ECode Activity::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return mBase->GetContentResolver(resolver);
}

ECode Activity::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    return mBase->GetMainLooper(looper);
}

ECode Activity::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    return mBase->GetApplicationContext(ctx);
}

ECode Activity::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    AutoPtr<IContext> ctx;
    GetApplicationContext((IContext**)&ctx);
    return ctx->RegisterComponentCallbacks(componentCallback);
}

ECode Activity::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    AutoPtr<IContext> ctx;
    GetApplicationContext((IContext**)&ctx);
    return ctx->UnregisterComponentCallbacks(componentCallback);
}

ECode Activity::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    return res->GetText(resId, text);
}

ECode Activity::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    return res->GetString(resId, str);
}

ECode Activity::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    //return res->GetString(resId, formatArgs, str);
    return NOERROR;
}

ECode Activity::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return InitializeTheme();
}

ECode Activity::GetThemeResId(
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId);
    *resId = mThemeResource;
    return NOERROR;
}

ECode Activity::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    VALIDATE_NOT_NULL(theme);
    *theme = NULL;

    if (mTheme == NULL) {
        AutoPtr<IApplicationInfo> appInfo;
        GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 version;
        appInfo->GetTargetSdkVersion(&version);
        AutoPtr<IResourcesHelper> rh;
        CResourcesHelper::AcquireSingleton((IResourcesHelper**)&rh);
        rh->SelectDefaultTheme(mThemeResource, version, &mThemeResource);
        FAIL_RETURN(InitializeTheme());
    }

    *theme = mTheme;
    REFCOUNT_ADD(*theme)

    return NOERROR;
}

ECode Activity::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    return theme->ObtainStyledAttributes(attrs, styles);
}

ECode Activity::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    return theme->ObtainStyledAttributes(resid, attrs, styles);
}

ECode Activity::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    return mTheme->ObtainStyledAttributes(set, attrs, 0, 0, styles);
}

ECode Activity::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    VALIDATE_NOT_NULL(styles);
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    return mTheme->ObtainStyledAttributes(set, attrs,
            defStyleAttr, defStyleRes, styles);
}

ECode Activity::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return mBase->GetClassLoader(loader);
}

ECode Activity::GetPackageName(
    /* [out] */ String* packageName)
{
    return mBase->GetPackageName(packageName);
}

ECode Activity::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return mBase->GetApplicationInfo(info);
}

ECode Activity::GetPackageResourcePath(
    /* [out] */ String* path)
{
    return mBase->GetPackageResourcePath(path);
}

ECode Activity::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    return mBase->GetPackageCodePath(codePath);
}

ECode Activity::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return mBase->GetSharedPrefsFile(name, file);
}

ECode Activity::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    return mBase->GetSharedPreferences(name, mode, prefs);
}

ECode Activity::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    return mBase->OpenFileInput(name, fileInputStream);
}

ECode Activity::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFileOutputStream** fileOutputStream)
{
    return mBase->OpenFileOutput(name, mode, fileOutputStream);
}

ECode Activity::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return mBase->DeleteFile(name, succeeded);
}

ECode Activity::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return mBase->GetFileStreamPath(name, file);
}

ECode Activity::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    return mBase->GetFilesDir(filesDir);
}

ECode Activity::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    return mBase->GetExternalFilesDir(type, filesDir);
}

ECode Activity::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    return mBase->GetObbDir(obbDir);
}

ECode Activity::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    return mBase->GetCacheDir(cacheDir);
}

ECode Activity::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    return mBase->GetExternalCacheDir(externalDir);
}

ECode Activity::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    return mBase->GetFileList(fileList);
}

ECode Activity::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    return mBase->GetDir(name, mode, dir);
}

ECode Activity::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return mBase->OpenOrCreateDatabase(name, mode, factory, sqliteDB);
}

ECode Activity::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return mBase->OpenOrCreateDatabase(name, mode, factory, errorHandler, sqliteDB);
}

ECode Activity::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return mBase->DeleteDatabase(name, succeeded);
}

ECode Activity::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    return mBase->GetDatabasePath(name, path);
}

ECode Activity::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    return mBase->GetDatabaseList(databaseList);
}

ECode Activity::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return mBase->GetWallpaper(drawable);
}

ECode Activity::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return mBase->PeekWallpaper(drawable);
}

ECode Activity::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    return mBase->GetWallpaperDesiredMinimumWidth(minWidth);
}

ECode Activity::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    return mBase->GetWallpaperDesiredMinimumHeight(minHeight);
}

ECode Activity::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return mBase->SetWallpaper(bitmap);
}

ECode Activity::SetWallpaper(
    /* [in] */ IInputStream* data)
{
    return mBase->SetWallpaper(data);
}

ECode Activity::ClearWallpaper()
{
    return mBase->ClearWallpaper();
}

//    @Override
ECode Activity::StartActivity(
    /* [in] */ IIntent *intent)
{
    return StartActivity(intent, NULL);
}

//    @Override
ECode Activity::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
     if (options != NULL) {
         return StartActivityForResult(intent, -1, options);
     }
     else {
         // Note we want to go through this call for compatibility with
         // applications that may have overridden the method.
         return StartActivityForResult(intent, -1);
     }
}

//    @Override
ECode Activity::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return StartActivityAsUser(intent, NULL, user);
}

//    @Override
ECode Activity::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* user)
{
    if (mParent != NULL) {
        return E_RUNTIME_EXCEPTION;
    }
    AutoPtr<IInstrumentationActivityResult> result;
    AutoPtr<IApplicationThread> at;
    mMainThread->GetApplicationThread((IApplicationThread**)&at);
    FAIL_RETURN(mInstrumentation->ExecStartActivity(
            mBase, IBinder::Probe(at.Get()), mToken, THIS_PROBE(IActivity),
            intent, -1, options, user,
            (IInstrumentationActivityResult**)&result));
    if (result != NULL) {
        Int32 resultCode;
        AutoPtr<IIntent> resultData;
        result->GetResultCode(&resultCode);
        result->GetResultData((IIntent**)&resultData);
        FAIL_RETURN(mMainThread->SendActivityResult(
                mToken, mEmbeddedID, -1, resultCode, resultData));
    }
    return NOERROR;
}

//    @Override
ECode Activity::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    return StartActivities(intents, NULL);
}

//    @Override
ECode Activity::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    AutoPtr<IApplicationThread> at;
    mMainThread->GetApplicationThread((IApplicationThread**)&at);
    return mInstrumentation->ExecStartActivities(
            mBase, IBinder::Probe(at.Get()), mToken, THIS_PROBE(IActivity),
            intents, options);
}

//    @Override
ECode Activity::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    if (mParent != NULL) {
        return E_RUNTIME_EXCEPTION;
    }

#ifdef DROID_CORE
    Int32 myUserId = CUserHandle::GetMyUserId();
#else
    Int32 myUserId;
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    helper->GetMyUserId(&myUserId);
#endif

    AutoPtr<IApplicationThread> at;
    mMainThread->GetApplicationThread((IApplicationThread**)&at);
    FAIL_RETURN(mInstrumentation->ExecStartActivitiesAsUser(
            mBase, IBinder::Probe(at.Get()), mToken, THIS_PROBE(IActivity),
            intents, options, myUserId));
    return NOERROR;
}

//    @Override
ECode Activity::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return StartIntentSender(intent, fillInIntent,
            flagsMask, flagsValues, extraFlags, NULL);
}

//    @Override
ECode Activity::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    if (options != NULL) {
        return StartIntentSenderForResult(intent, -1, fillInIntent, flagsMask,
                flagsValues, extraFlags, options);
    }
    else {
        // Note we want to go through this call for compatibility with
        // applications that may have overridden the method.
        return StartIntentSenderForResult(intent, -1, fillInIntent, flagsMask,
                flagsValues, extraFlags);
    }
}

ECode Activity::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendBroadcast(intent);
}

ECode Activity::SendBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendBroadcast(intent, receiverPermission);
}

ECode Activity::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendOrderedBroadcast(intent, receiverPermission);
}

ECode Activity::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendOrderedBroadcast(intent, receiverPermission, resultReceiver, scheduler,
        initialCode, initialData, initialExtras);
}

ECode Activity::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->SendBroadcastAsUser(intent, user);
}

ECode Activity::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    return mBase->SendBroadcastAsUser(intent, user, receiverPermission);
}

ECode Activity::SendOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendOrderedBroadcastAsUser(intent, user, receiverPermission, resultReceiver, scheduler,
        initialCode, initialData, initialExtras);
}

ECode Activity::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->SendStickyBroadcast(intent);
}

ECode Activity::SendStickyOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendStickyOrderedBroadcast(intent, resultReceiver, scheduler, initialCode, initialData,
        initialExtras);
}

ECode Activity::RemoveStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return mBase->RemoveStickyBroadcast(intent);
}

ECode Activity::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->SendStickyBroadcastAsUser(intent, user);
}

ECode Activity::SendStickyOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return mBase->SendStickyOrderedBroadcastAsUser(intent, user, resultReceiver, scheduler, initialCode,
        initialData, initialExtras);
}

ECode Activity::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return mBase->RemoveStickyBroadcastAsUser(intent, user);
}

ECode Activity::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** stickyIntent)
{
    return mBase->RegisterReceiver(receiver, filter, stickyIntent);
}

ECode Activity::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return mBase->RegisterReceiver(receiver, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode Activity::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return mBase->RegisterReceiverAsUser(receiver, user, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode Activity::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    return mBase->UnregisterReceiver(receiver);
}

ECode Activity::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return mBase->StartService(service, name);
}

ECode Activity::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StopService(service, succeeded);
}

ECode Activity::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    return mBase->StartServiceAsUser(service, user, name);
}

ECode Activity::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    return mBase->StopServiceAsUser(service, user, succeeded);
}

ECode Activity::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, succeeded);
}

ECode Activity::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    return mBase->BindService(service, conn, flags, userHandle, succeeded);
}

ECode Activity::UnbindService(
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn)
{
    return mBase->UnbindService(conn);
}

ECode Activity::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    return mBase->StartInstrumentation(className, profileFile, arguments, succeeded);
}

//    @Override
ECode Activity::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    *object = NULL;

    AutoPtr<IContext> ctx;
    GetBaseContext((IContext**)&ctx);
    if (ctx == NULL) {
        Slogger::E(TAG, "System services not available to Activities before onCreate().");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (IContext::WINDOW_SERVICE.Equals(name)) {
        *object = mWindowManager;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::SEARCH_SERVICE.Equals(name)) {
        EnsureSearchManager();
        *object = mSearchManager;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }
    else if (IContext::LAYOUT_INFLATER_SERVICE.Equals(name)) {
        if (mInflater == NULL) {
            AutoPtr<ILayoutInflater> temp;
            mBase->GetSystemService(
                IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&temp);
            temp->CloneInContext(this, (ILayoutInflater**)&mInflater);
        }
        *object = mInflater;
        REFCOUNT_ADD(*object);
        return NOERROR;
    }

    return mBase->GetSystemService(name, object);
}

ECode Activity::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32 * result)
{
    return mBase->CheckPermission(permission, pid, uid, result);
}

ECode Activity::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return mBase->CheckCallingPermission(permission, value);
}

ECode Activity::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return mBase->CheckCallingOrSelfPermission(permission, perm);
}

ECode Activity::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    return mBase->EnforcePermission(permission, pid, uid, message);
}

ECode Activity::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingPermission(permission, message);
}

ECode Activity::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingOrSelfPermission(permission, message);
}

ECode Activity::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->GrantUriPermission(toCapsule, uri, modeFlags);
}

ECode Activity::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return mBase->RevokeUriPermission(uri, modeFlags);
}

ECode Activity::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return mBase->CheckUriPermission(uri, pid, uid, modeFlags, result);
}

ECode Activity::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return mBase->CheckCallingUriPermission(uri, modeFlags, permissionId);
}

ECode Activity::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* permissionId)
{
    return mBase->CheckCallingOrSelfUriPermission(uri, modeFlags, permissionId);
}

ECode Activity::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return mBase->CheckUriPermission(uri, readPermission, writePermission,
                    pid, uid, modeFlags, result);
}

ECode Activity::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceUriPermission(uri, pid, uid, modeFlags, message);
}

ECode Activity::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingUriPermission(uri, modeFlags, message);
}

ECode Activity::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceCallingOrSelfUriPermission(uri, modeFlags, message);
}

ECode Activity::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return mBase->EnforceUriPermission(uri, readPermission, writePermission,
            pid, uid, modeFlags, message);
}

ECode Activity::CreatePackageContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return mBase->CreatePackageContext(capsuleName, flags, ctx);
}

ECode Activity::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    return mBase->CreatePackageContextAsUser(packageName, flags, user, ctx);
}

ECode Activity::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateConfigurationContext(overrideConfiguration, ctx);
}

ECode Activity::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    return mBase->CreateDisplayContext(display, ctx);
}

ECode Activity::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    return mBase->GetCompatibilityInfo(displayId, infoHolder);
}

ECode Activity::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    return mBase->IsRestricted(isRestricted);
}

// interface of ContextThemeWrapper
ECode Activity::ApplyOverrideConfiguration(
    /* [in] */ IConfiguration* overrideConfiguration)
{
    if (mResources != NULL) {
        Slogger::E(TAG, "getResources() has already been called");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mOverrideConfiguration != NULL) {
        Slogger::E(TAG, "Override configuration has already been set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return CConfiguration::New(overrideConfiguration, (IConfiguration**)&mOverrideConfiguration);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
