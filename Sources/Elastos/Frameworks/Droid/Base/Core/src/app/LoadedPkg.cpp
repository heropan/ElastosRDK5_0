
#include "app/LoadedPkg.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuffer.h>
#include "os/CUserHandle.h"
#include "os/Process.h"
#include "os/Handler.h"
#include "app/CInnerReceiver.h"
#include "app/CInstrumentationHelper.h"
#include "content/pm/PackageManager.h"
#include "content/pm/CApplicationInfo.h"
#include "app/CActivityThread.h"
#include "app/ActivityManagerNative.h"
#include "app/CContextImpl.h"
#include "app/CInnerConnection.h"
#include "view/CCompatibilityInfoHolder.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::CPathClassLoader;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Etl::Pair;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::CUserHandle;
using Elastos::Droid::Content::EIID_IBroadcastReceiverPendingResult;
using Elastos::Droid::Content::Pm::PackageManager;
using Elastos::Droid::Content::Pm::CApplicationInfo;
using Elastos::Droid::App::CInnerReceiver;
using Elastos::Droid::App::IContextImpl;
using Elastos::Droid::App::CActivityThread;
using Elastos::Droid::App::CInstrumentationHelper;
using Elastos::Droid::View::CCompatibilityInfoHolder;


namespace Elastos{
namespace Droid{
namespace App{

// b74a59a5-260f-4c6a-a7c6-9251d220727e
extern "C" const InterfaceID EIID_LoadedPkg =
    { 0xb74a59a5, 0x260f, 0x4c6a, { 0xa7, 0xc6, 0x92, 0x51, 0xd2, 0x20, 0x72, 0x7e } };

const String LoadedPkg::TAG("LoadedPkg");


//==============================================================================
// LoadedPkg::ReceiverDispatcher::Args
//==============================================================================
LoadedPkg::ReceiverDispatcher::Args::Args(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser,
    /* [in] */ ReceiverDispatcher* rd)
    : PendingResult(resultCode, resultData, resultExtras,
            rd->mRegistered ? TYPE_REGISTERED : TYPE_UNREGISTERED,
            ordered, sticky, IBinder::Probe(rd->mIIntentReceiver), sendingUser)
    , mCurIntent(intent)
    , mOrdered(ordered)
    , mHost(rd)
{}

ECode LoadedPkg::ReceiverDispatcher::Args::Run()
{
    AutoPtr<IBroadcastReceiver> receiver = mHost->mReceiver;
    Boolean ordered = mOrdered;
    if (CActivityThread::DEBUG_BROADCAST) {
        Int32 seq = -1;
        mCurIntent->GetInt32Extra(String("seq"), -1, &seq);
        String action;
        mCurIntent->GetAction(&action);
        Slogger::I(CActivityThread::TAG, "Dispatching broadcast %s seq=%d to %p"
                , action.string(), seq, mHost->mReceiver.Get());
        Slogger::I(CActivityThread::TAG, "  mRegistered=%d mOrderedHint=%d", mHost->mRegistered, ordered);
    }

    AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
    AutoPtr<IIntent> intent = mCurIntent;
    mCurIntent = NULL;

    if (receiver == NULL || mHost->mForgotten) {
        if (mHost->mRegistered && ordered) {
            if (CActivityThread::DEBUG_BROADCAST) {
                Slogger::I(CActivityThread::TAG, "Finishing NULL broadcast to %d", mHost->mReceiver.Get());
            }
            SendFinished(mgr);
        }
        mHost = NULL;
        return NOERROR;
    }
    // Trace.traceBegin(Trace.TRACE_TAG_ACTIVITY_MANAGER, "broadcastReceiveReg");
    // try {
//TODO: Need ClassLoader
    // ClassLoader cl =  mReceiver.getClass().getClassLoader();
    // intent.setExtrasClassLoader(cl);
    // setExtrasClassLoader(cl);
    receiver->SetPendingResult((IBroadcastReceiverPendingResult*)this);
    ECode ec = receiver->OnReceive(mHost->mContext, intent);
    if (FAILED(ec)) {
        if (mHost->mRegistered && ordered) {
            if (CActivityThread::DEBUG_BROADCAST) {
                Slogger::I(CActivityThread::TAG, "Finishing failed broadcast to %p", mHost->mReceiver.Get());
            }

            SendFinished(mgr);
        }
        Boolean result;
        if (mHost->mInstrumentation == NULL ||
                (mHost->mInstrumentation->OnException(mHost->mReceiver, ec, &result), !result)) {
            // Trace.traceEnd(Trace.TRACE_TAG_ACTIVITY_MANAGER);
            Slogger::E(CActivityThread::TAG, "Error receiving broadcast %p in %p ec: 0x%08x", intent.Get(), mHost->mReceiver.Get(), ec);
            mHost = NULL;
            return E_RUNTIME_EXCEPTION;
            // throw new RuntimeException(
            //     "Error receiving broadcast " + intent
            //     + " in " + mReceiver, e);
        }
    }
    // } catch (Exception e) {
    //     if (mRegistered && ordered) {
    //         // if (ActivityThread.DEBUG_BROADCAST) Slog.i(ActivityThread.TAG,
    //         //         "Finishing failed broadcast to " + mReceiver);
    //         SendFinished(mgr);
    //     }
    //     if (mInstrumentation == NULL ||
    //             !mInstrumentation->OnException(mReceiver, e)) {
    //         // Trace.traceEnd(Trace.TRACE_TAG_ACTIVITY_MANAGER);
    //         // throw new RuntimeException(
    //         //     "Error receiving broadcast " + intent
    //         //     + " in " + mReceiver, e);
    //         return E_RUNTIME_EXCEPTION;
    //     }
    // }

    AutoPtr<IBroadcastReceiverPendingResult> result;
    receiver->GetPendingResult((IBroadcastReceiverPendingResult**)&result);
    if (result != NULL) {
        Finish();
    }

    // Trace.traceEnd(Trace.TRACE_TAG_ACTIVITY_MANAGER);
    mHost = NULL;
    return NOERROR;
}

PInterface LoadedPkg::ReceiverDispatcher::Args::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IBroadcastReceiverPendingResult*)this;
    }
    else if (riid == EIID_IBroadcastReceiverPendingResult) {
        return (IBroadcastReceiverPendingResult*)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 LoadedPkg::ReceiverDispatcher::Args::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LoadedPkg::ReceiverDispatcher::Args::Release()
{
    return ElRefBase::Release();
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBroadcastReceiverPendingResult*)this) {
        *pIID = EIID_IBroadcastReceiverPendingResult;
    }
    else if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode LoadedPkg::ReceiverDispatcher::Args::SetResultCode(
    /* [in] */ Int32 code)
{
    return BroadcastReceiver::PendingResult::SetResultCode(code);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    return BroadcastReceiver::PendingResult::GetResultCode(resultCode);
}

ECode LoadedPkg::ReceiverDispatcher::Args::SetResultData(
    /* [in] */ const String& data)
{
    return BroadcastReceiver::PendingResult::SetResultData(data);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetResultData(
    /* [out] */ String* resultData)
{
    return BroadcastReceiver::PendingResult::GetResultData(resultData);
}

ECode LoadedPkg::ReceiverDispatcher::Args::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::PendingResult::SetResultExtras(extras);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** resultExtras)
{
    return BroadcastReceiver::PendingResult::GetResultExtras(makeMap, resultExtras);
}

ECode LoadedPkg::ReceiverDispatcher::Args::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::PendingResult::SetResult(code, data, extras);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    return BroadcastReceiver::PendingResult::GetAbortBroadcast(isAborted);
}

ECode LoadedPkg::ReceiverDispatcher::Args::AbortBroadcast()
{
    return BroadcastReceiver::PendingResult::AbortBroadcast();
}

ECode LoadedPkg::ReceiverDispatcher::Args::ClearAbortBroadcast()
{
    return BroadcastReceiver::PendingResult::ClearAbortBroadcast();
}

ECode LoadedPkg::ReceiverDispatcher::Args::Finish()
{
    return BroadcastReceiver::PendingResult::Finish();
}

ECode LoadedPkg::ReceiverDispatcher::Args::SetExtrasClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return BroadcastReceiver::PendingResult::SetExtrasClassLoader(cl);
}

ECode LoadedPkg::ReceiverDispatcher::Args::SendFinished(
    /* [in] */ IIActivityManager* am)
{
    return BroadcastReceiver::PendingResult::SendFinished(am);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return BroadcastReceiver::PendingResult::GetSendingUserId(userId);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetOrderedHint(
    /* [out] */ Boolean* orderedHint)
{
    return BroadcastReceiver::PendingResult::GetOrderedHint(orderedHint);
}

ECode LoadedPkg::ReceiverDispatcher::Args::GetInitialStickyHint(
    /* [out] */ Boolean* initialStickyHint)
{
    return BroadcastReceiver::PendingResult::GetInitialStickyHint(initialStickyHint);
}


//==============================================================================
// LoadedPkg::ReceiverDispatcher
//==============================================================================

LoadedPkg::ReceiverDispatcher::ReceiverDispatcher(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* activityThread,
    /* [in] */ IInstrumentation* instrumentation,
    /* [in] */ Boolean registered)
    : mIIntentReceiver(NULL)
    , mReceiver(receiver)
    , mContext(context)
    , mActivityThread(activityThread)
    , mInstrumentation(instrumentation)
    , mRegistered(registered)
    , mForgotten(FALSE)
    , mHasUsed(FALSE)
{

    // if (activityThread == null) {
    //     throw new NullPointerException("Handler must not be null");
    // }
    assert(activityThread != NULL);
    CInnerReceiver::New((Handle32)this, !registered, &mIIntentReceiver);
    // mLocation = new IntentReceiverLeaked(null);
    // mLocation.fillInStackTrace();
}

LoadedPkg::ReceiverDispatcher::~ReceiverDispatcher()
{
    if (mRegistered && mIIntentReceiver != NULL) {
        mIIntentReceiver->Release();
    }
}

CAR_INTERFACE_IMPL(LoadedPkg::ReceiverDispatcher, IWeakReferenceSource)

ECode LoadedPkg::ReceiverDispatcher::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode LoadedPkg::ReceiverDispatcher::Validate(
   /* [in] */ IContext* context,
   /* [in] */ IHandler* activityThread)
{
    if (mContext.Get() != context) {
        Slogger::E(LoadedPkg::TAG, "Receiver %p registered with differing Context (was %p now %p)"
                , mReceiver.Get(), mContext.Get(), context);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (mActivityThread.Get() != activityThread) {
        Slogger::E(LoadedPkg::TAG, "Receiver %p registered with differing handler (was %p now %p)"
                , mReceiver.Get(), mActivityThread.Get(), activityThread);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<IBroadcastReceiver> LoadedPkg::ReceiverDispatcher::GetIntentReceiver()
{
    return mReceiver;
}

AutoPtr<IIntentReceiver> LoadedPkg::ReceiverDispatcher::GetIIntentReceiver()
{
    AutoPtr<IIntentReceiver> r = mIIntentReceiver;
    if (!mHasUsed && !mRegistered) {
        // mIIntentReceiver hold ReceiverDispatcher, so release but not set to null!
        mIIntentReceiver->Release();
    }
    mHasUsed = TRUE;

    return r;
}

void LoadedPkg::ReceiverDispatcher::PerformReceive(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ Int32 sendingUser)
{
    if (CActivityThread::DEBUG_BROADCAST) {
        Int32 seq = -1;
        intent->GetInt32Extra(String("seq"), -1, &seq);
        String action;
        intent->GetAction(&action);
        Slogger::I(CActivityThread::TAG, "Enqueueing broadcast %s seq=%d to %p"
                , action.string(),  seq, mReceiver.Get());
    }

    AutoPtr<Args> args = new Args(intent, resultCode, data, extras, ordered,
            sticky, sendingUser, this);
    Boolean result;
    mActivityThread->Post(args, &result);
    if (!result) {
        if (mRegistered && ordered) {
            AutoPtr<IIActivityManager> mgr = ActivityManagerNative::GetDefault();
            if (CActivityThread::DEBUG_BROADCAST) {
                Slogger::I(CActivityThread::TAG, "Finishing sync broadcast to %p", mReceiver.Get());
            }
            args->SendFinished(mgr);
        }
    }
}

//==============================================================================
// LoadedPkg::ServiceDispatcher::RunConnection
//==============================================================================

LoadedPkg::ServiceDispatcher::RunConnection::RunConnection(
   /* [in] */ ServiceDispatcher* serviceDispatcher,
   /* [in] */ IComponentName* name,
   /* [in] */ IBinder* service,
   /* [in] */ Int32 command)
    : mServiceDispatcher(serviceDispatcher)
    , mName(name)
    , mService(service)
    , mCommand(command)
{}

ECode LoadedPkg::ServiceDispatcher::RunConnection::Run()
{
    if (mCommand == 0) {
        return mServiceDispatcher->DoConnected(mName, mService);
    }
    else if (mCommand == 1) {
        return mServiceDispatcher->DoDeath(mName, mService);
    }
    return NOERROR;
}

//==============================================================================
// LoadedPkg::ServiceDispatcher::DeathMonitor
//==============================================================================

CAR_INTERFACE_IMPL(LoadedPkg::ServiceDispatcher::DeathMonitor, IProxyDeathRecipient);

LoadedPkg::ServiceDispatcher::DeathMonitor::DeathMonitor(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service,
    /* [in] */ ServiceDispatcher* owner)
    : mName(name)
    , mService(service)
{
    AutoPtr<IWeakReferenceSource> wrs = (IWeakReferenceSource*)owner;
    wrs->GetWeakReference((IWeakReference**)&mOwner);
}

//@Override
ECode LoadedPkg::ServiceDispatcher::DeathMonitor::ProxyDied()
{
    Slogger::W("LoadedPkg::ServiceDispatcher::DeathMonitor", " >> ProxyDied()");
    AutoPtr<IInterface> obj;
    mOwner->Resolve(EIID_IInterface, (IInterface**)&obj);
    if (obj) {
        ServiceDispatcher* sd = (ServiceDispatcher*)obj.Get();
        return sd->Death(mName, mService);
    }

    return NOERROR;
}

//==============================================================================
// LoadedPkg::ServiceDispatcher
//==============================================================================
CAR_INTERFACE_IMPL(LoadedPkg::ServiceDispatcher, IWeakReferenceSource)

ECode LoadedPkg::ServiceDispatcher::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

LoadedPkg::ServiceDispatcher::ServiceDispatcher(
    /* [in] */ IServiceConnection* conn,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* activityThread,
    /* [in] */ Int32 flags)
    : mConnection(conn)
    , mContext(context)
    , mActivityThread(activityThread)
    , mFlags(flags)
    , mDied(FALSE)
    , mForgotten(FALSE)
{
    CInnerConnection::NewByFriend((CInnerConnection**)&mIServiceConnection);
    mIServiceConnection->Init(this);
//                mLocation = new ServiceConnectionLeaked(null);
//                mLocation.fillInStackTrace();
}

ECode LoadedPkg::ServiceDispatcher::Validate(
   /* [in] */ IContext* context,
   /* [in] */ IHandler* handler)
{
    if (mContext.Get() != context) {
        Slogger::E("LoadedPkg::ServiceDispatcher",
            "ServiceConnection %p registered with differing Context (was %p new %p)",
            mConnection.Get(), mContext.Get(), context);
        return E_RUNTIME_EXCEPTION;
    }

    if (mActivityThread.Get() != handler) {
        Slogger::E("LoadedPkg::ServiceDispatcher",
            "ServiceConnection %p registered with differing handler (was %p new %p)",
            mConnection.Get(), mActivityThread.Get(), handler);
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

void LoadedPkg::ServiceDispatcher::DoForget()
{
    AutoLock lock(mLock);

    HashMap<AutoPtr<IComponentName>, AutoPtr<ConnectionInfo> >::Iterator it;
    for (it = mActiveConnections.Begin(); it != mActiveConnections.End(); ++it) {
        AutoPtr<ConnectionInfo> ci = it->mSecond;
        AutoPtr<IProxy> proxy = (IProxy*)ci->mBinder->Probe(EIID_IProxy);
        if (proxy != NULL) {
            Boolean res = FALSE;
            proxy->UnlinkToDeath(ci->mDeathMonitor, 0, &res);
        }
    }

    mActiveConnections.Clear();
    mForgotten = TRUE;
}

AutoPtr<IServiceConnection> LoadedPkg::ServiceDispatcher::GetServiceConnection()
{
    return mConnection;
}

AutoPtr<IIServiceConnection> LoadedPkg::ServiceDispatcher::GetIServiceConnection()
{
    return mIServiceConnection;
}

Int32 LoadedPkg::ServiceDispatcher::GetFlags()
{
    return mFlags;
}

void LoadedPkg::ServiceDispatcher::Connected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    if (mActivityThread != NULL) {
        AutoPtr<RunConnection> con = new RunConnection(this, name, service, 0);
        Boolean result;
        mActivityThread->Post(con, &result);
    }
    else {
        DoConnected(name, service);
    }
}

ECode LoadedPkg::ServiceDispatcher::DoConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    AutoPtr<ConnectionInfo> old;
    AutoPtr<ConnectionInfo> info;

    {
        AutoLock lock(mLock);

        if (mForgotten) {
            // We unbound before receiving the connection; ignore
            // any connection received.
            return NOERROR;
        }

        HashMap<AutoPtr<IComponentName>, AutoPtr<ConnectionInfo> >::Iterator it =
            mActiveConnections.Find(name);
        if (it != mActiveConnections.End()) {
            old = it->mSecond;
        }
        if (old != NULL && old->mBinder.Get() == service) {
            // Huh, already have this one.  Oh well!
            return NOERROR;
        }

        if (service != NULL) {
            // A new service is being connected... set it all up.
            mDied = FALSE;
            info = new ConnectionInfo();
            info->mBinder = service;
            info->mDeathMonitor = new DeathMonitor(name, service, this);
            AutoPtr<IProxy> proxy = (IProxy*)service->Probe(EIID_IProxy);
            if (proxy != NULL) proxy->LinkToDeath(info->mDeathMonitor, 0);
            mActiveConnections[name] = info;
        }
        else {
            // The named service is being disconnected... clean up.
            mActiveConnections.Erase(name);
        }

        if (old != NULL) {
            AutoPtr<IProxy> proxy = (IProxy*)old->mBinder->Probe(EIID_IProxy);
            if (proxy != NULL) {
                Boolean res;
                proxy->UnlinkToDeath(old->mDeathMonitor, 0, &res);
            }
        }
    }

    // If there was an old service, it is not disconnected.
    if (old != NULL) {
        FAIL_RETURN(mConnection->OnServiceDisconnected(name));
    }
    // If there is a new service, it is now connected.
    if (service != NULL) {
        FAIL_RETURN(mConnection->OnServiceConnected(name, service));
    }
    return NOERROR;
}

ECode LoadedPkg::ServiceDispatcher::Death(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    AutoPtr<ConnectionInfo> old;

    {
        AutoLock lock(mLock);
        mDied = true;

        HashMap<AutoPtr<IComponentName>, AutoPtr<ConnectionInfo> >::Iterator it;
        it = mActiveConnections.Find(name);
        if (it != mActiveConnections.End()) {
            old = it->mSecond;
            mActiveConnections.Erase(it);
        }

        if (old == NULL || old->mBinder.Get() != service) {
            // Death for someone different than who we last
            // reported...  just ignore it.
            return NOERROR;
        }

        AutoPtr<IProxy> proxy = (IProxy*)old->mBinder->Probe(EIID_IProxy);
        if (proxy != NULL) {
            Boolean res;
            proxy->UnlinkToDeath(old->mDeathMonitor, 0, &res);
        }
    }

    if (mActivityThread != NULL) {
        AutoPtr<RunConnection> con = new RunConnection(this, name, service, 1);
        Boolean result;
        return mActivityThread->Post(con, &result);
    }
    else {
        return DoDeath(name, service);
    }
}

ECode LoadedPkg::ServiceDispatcher::DoDeath(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return mConnection->OnServiceDisconnected(name);
}

//==============================================================================
// LoadedPkg
//==============================================================================
AutoPtr<IApplication> LoadedPkg::GetApplication()
{
    return mApplication;
}

LoadedPkg::LoadedPkg(
    /* [in] */ CActivityThread* activityThread,
    /* [in] */ IApplicationInfo* aInfo,
    /* [in] */ ICompatibilityInfo* compatInfo,
    /* [in] */ IActivityThread* mainThread,
    /* [in] */ IClassLoader* baseLoader,
    /* [in] */ Boolean securityViolation,
    /* [in] */ Boolean includeCode)
{
    mActivityThread = activityThread;
    mApplicationInfo = aInfo;
    CApplicationInfo* info = (CApplicationInfo*)aInfo;
    mPackageName = info->mPackageName;
    mAppDir = info->mSourceDir;
    Int32 myUid = Process::MyUid();
    mResDir = info->mUid == myUid ? info->mSourceDir : info->mPublicSourceDir;
    if (!CUserHandle::IsSameUser(info->mUid, myUid) && !Process::IsIsolated()) {
        info->mDataDir = PackageManager::GetDataDirForUser(CUserHandle::GetUserId(myUid),
             mPackageName);
    }

    mSharedLibraries = info->mSharedLibraryFiles;
    mDataDir = info->mDataDir;
    if (!mDataDir.IsNull()) {
        CFile::New(mDataDir, (IFile**)&mDataDirFile);
    }
    else {
        mDataDirFile = NULL;
    }

    mLibDir = info->mNativeLibraryDir;
    mBaseClassLoader = baseLoader;
    mSecurityViolation = securityViolation;
    mIncludeCode = includeCode;
    CCompatibilityInfoHolder::New((ICompatibilityInfoHolder**)&mCompatibilityInfo);
    mCompatibilityInfo->Set(compatInfo);

    mResDir = info->mSourceDir;
    mSecurityViolation = securityViolation;
    mIncludeCode = includeCode;

    if (mAppDir.IsNull()) {
        if (CActivityThread::sSystemContext == NULL) {
            CActivityThread::sSystemContext = CContextImpl::CreateSystemContext(mainThread);
            AutoPtr<IResources> resources;
            CActivityThread::sSystemContext->GetResources((IResources**)&resources);
            AutoPtr<IConfiguration> config;
            mainThread->GetConfiguration((IConfiguration**)&config);
            AutoPtr<IDisplayMetrics> metrics;
            ((CActivityThread*)mainThread)->GetDisplayMetricsLocked(IDisplay::DEFAULT_DISPLAY,
                    compatInfo, (IDisplayMetrics**)&metrics);
            resources->UpdateConfiguration(config, metrics);
//            Slogger::I(TAG, "Created system resources %s: %s", "resources", "configuration");
//                    + sSystemContext.getResources() + ": "
//                    + sSystemContext.getResources().getConfiguration());
        }
        CActivityThread::sSystemContext->GetClassLoader((IClassLoader**)&mClassLoader);
        CActivityThread::sSystemContext->GetResources((IResources**)&mResources);
    }

    mClientCount = 0;
}

LoadedPkg::LoadedPkg(
    /* [in] */ CActivityThread* activityThread,
    /* [in] */ const String& name,
    /* [in] */ IContext* systemContext,
    /* [in] */ IApplicationInfo* aInfo,
    /* [in] */ ICompatibilityInfo* compatInfo)
{
    mActivityThread = activityThread;
    mApplicationInfo = aInfo;
    if (aInfo == NULL) {
        CApplicationInfo::New((IApplicationInfo**)&mApplicationInfo);
    }
    mApplicationInfo->SetPackageName(name);
    mApplicationInfo->SetProcessName(name);
    mPackageName = name;
    mAppDir = NULL;
    mResDir = NULL;
    mSharedLibraries = NULL;
    mDataDir = NULL;
    mDataDirFile = NULL;
    mLibDir = NULL;
    mBaseClassLoader = NULL;
    mSecurityViolation = FALSE;
    mIncludeCode = TRUE;
    systemContext->GetClassLoader((IClassLoader**)&mClassLoader);
    systemContext->GetResources((IResources**)&mResources);

    CCompatibilityInfoHolder::New((ICompatibilityInfoHolder**)&mCompatibilityInfo);
    mCompatibilityInfo->Set(compatInfo);

    mClientCount = 0;
}

LoadedPkg::~LoadedPkg()
{
    mReceivers.Clear();
    mUnregisteredReceivers.Clear();

    mServices.Clear();
    mUnboundServices.Clear();
}

UInt32 LoadedPkg::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LoadedPkg::Release()
{
    return ElRefBase::Release();
}

PInterface LoadedPkg::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IWeakReferenceSource*)this;
    }
    else if (riid == EIID_IWeakReference) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_LoadedPkg) {
        return reinterpret_cast<PInterface>(this);
    }
    return NULL;
}

ECode LoadedPkg::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IWeakReferenceSource*)this) {
        *iid = EIID_IWeakReference;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode LoadedPkg::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

ECode LoadedPkg::GetPackageName(
    /* [out] */ String* packageName)
{
    VALIDATE_NOT_NULL(packageName);
    *packageName = mPackageName;
    return NOERROR;
}

ECode LoadedPkg::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mApplicationInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

Boolean LoadedPkg::IsSecurityViolation()
{
    return mSecurityViolation;
}

//private static String[] getLibrariesFor(String packageName) {
//    ApplicationInfo ai = null;
//    try {
//        ai = ActivityThread.getPackageManager().getApplicationInfo(packageName,
//                PackageManager.GET_SHARED_LIBRARY_FILES, UserHandle.myUserId());
//    } catch (RemoteException e) {
//        throw new AssertionError(e);
//    }
//
//    if (ai == null) {
//        return null;
//    }
//
//    return ai.sharedLibraryFiles;
//}
//private static String combineLibs(String[] list1, String[] list2) {
//    StringBuilder result = new StringBuilder(300);
//    boolean first = true;
//
//    if (list1 != null) {
//        for (String s : list1) {
//            if (first) {
//                first = false;
//            } else {
//                result.append(':');
//            }
//            result.append(s);
//        }
//    }
//
//    // Only need to check for duplicates if list1 was non-empty.
//    boolean dupCheck = !first;
//
//    if (list2 != null) {
//        for (String s : list2) {
//            if (dupCheck && ArrayUtils.contains(list1, s)) {
//                continue;
//            }
//
//            if (first) {
//                first = false;
//            } else {
//                result.append(':');
//            }
//            result.append(s);
//        }
//    }
//
//    return result.toString();
//}

ECode LoadedPkg::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    VALIDATE_NOT_NULL(loader);

    AutoLock lock(mLock);

    if (mClassLoader != NULL) {
        *loader = mClassLoader;
        REFCOUNT_ADD(*loader);
        return NOERROR;
    }

    // if (mIncludeCode && !mPackageName.equals("android")) {
    //     String zip = mAppDir;
    //     String libraryPath = mLibDir;

    //     /*
    //      * The following is a bit of a hack to inject
    //      * instrumentation into the system: If the app
    //      * being started matches one of the instrumentation names,
    //      * then we combine both the "instrumentation" and
    //      * "instrumented" app into the path, along with the
    //      * concatenation of both apps' shared library lists.
    //      */

    //     String instrumentationAppDir =
    //             mActivityThread.mInstrumentationAppDir;
    //     String instrumentationAppLibraryDir =
    //             mActivityThread.mInstrumentationAppLibraryDir;
    //     String instrumentationAppPackage =
    //             mActivityThread.mInstrumentationAppPackage;
    //     String instrumentedAppDir =
    //             mActivityThread.mInstrumentedAppDir;
    //     String instrumentedAppLibraryDir =
    //             mActivityThread.mInstrumentedAppLibraryDir;
    //     String[] instrumentationLibs = null;

    //     if (mAppDir.equals(instrumentationAppDir)
    //             || mAppDir.equals(instrumentedAppDir)) {
    //         zip = instrumentationAppDir + ":" + instrumentedAppDir;
    //         libraryPath = instrumentationAppLibraryDir + ":" + instrumentedAppLibraryDir;
    //         if (! instrumentedAppDir.equals(instrumentationAppDir)) {
    //             instrumentationLibs =
    //                 getLibrariesFor(instrumentationAppPackage);
    //         }
    //     }

    //     if ((mSharedLibraries != null) ||
    //             (instrumentationLibs != null)) {
    //         zip =
    //             combineLibs(mSharedLibraries, instrumentationLibs)
    //             + ':' + zip;
    //     }

    //     /*
    //      * With all the combination done (if necessary, actually
    //      * create the class loader.
    //      */

    //     if (ActivityThread.localLOGV)
    //         Slog.v(ActivityThread.TAG, "Class path: " + zip + ", JNI path: " + libraryPath);

    //     // Temporarily disable logging of disk reads on the Looper thread
    //     // as this is early and necessary.
    //     StrictMode.ThreadPolicy oldPolicy = StrictMode.allowThreadDiskReads();

    //     mClassLoader =
    //         ApplicationLoaders.getDefault().getClassLoader(
    //             zip, libraryPath, mBaseClassLoader);
    //     initializeJavaContextClassLoader();

    //     StrictMode.setThreadPolicy(oldPolicy);
    // } else {
    //     if (mBaseClassLoader == null) {
    //         mClassLoader = ClassLoader.getSystemClassLoader();
    //     } else {
    //         mClassLoader = mBaseClassLoader;
    //     }
    // }
//TODO:
    ASSERT_SUCCEEDED(CPathClassLoader::New(String("Elastos.Droid.Core.eco"), (IClassLoader**)&mClassLoader));
    *loader = mClassLoader;
    REFCOUNT_ADD(*loader);
    return NOERROR;
}

ECode LoadedPkg::GetAppDir(
    /* [out] */ String* appDir)
{
    VALIDATE_NOT_NULL(appDir);
    *appDir = mAppDir;
    return NOERROR;
}

ECode LoadedPkg::GetLibDir(
    /* [out] */ String* libDir)
{
     VALIDATE_NOT_NULL(libDir);
     *libDir = mLibDir;
     return NOERROR;
}

ECode LoadedPkg::GetResDir(
    /* [out] */ String* resDir)
{
    VALIDATE_NOT_NULL(resDir);
    *resDir = mResDir;
    return NOERROR;
}

ECode LoadedPkg::GetDataDir(
    /* [out] */ String* dataDir)
{
    VALIDATE_NOT_NULL(dataDir);
    *dataDir = mDataDir;
    return NOERROR;
}

ECode LoadedPkg::GetDataDirFile(
    /* [out] */ IFile** dirFile)
{
    VALIDATE_NOT_NULL(dirFile);
    *dirFile = mDataDirFile;
    REFCOUNT_ADD(*dirFile);
    return NOERROR;
}

ECode LoadedPkg::GetAssets(
    /* [in] */ IActivityThread* mainThread,
    /* [out] */ IAssetManager** manager)
{
    AutoPtr<IResources> res;
    FAIL_RETURN(GetResources(mainThread, (IResources**)&res));
    return res->GetAssets(manager);
}

ECode LoadedPkg::GetResources(
    /* [in] */ IActivityThread* mainThread,
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(mainThread);
    VALIDATE_NOT_NULL(res);

    if (mResources == NULL) {
        ((CActivityThread*)mainThread)->GetTopLevelResources(mResDir,
                IDisplay::DEFAULT_DISPLAY, NULL, this, (IResources**)&mResources);
    }
    *res = mResources.Get();
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode LoadedPkg::MakeApplication(
    /* [in] */ Boolean forceDefaultAppClass,
    /* [in] */ IInstrumentation* instrumentation,
    /* [out] */ IApplication** result)
{
    VALIDATE_NOT_NULL(result);

    if (mApplication != NULL) {
        *result = mApplication;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }

    AutoPtr<IApplication> app;
    String appClass;
    FAIL_RETURN(mApplicationInfo->GetClassName(&appClass));
    if (forceDefaultAppClass || (appClass.IsNull())) {
        appClass = String("Application");/* "android.app.Application" */;

        //    try {
        AutoPtr<IClassLoader> cl;
        GetClassLoader((IClassLoader**)&cl);
        AutoPtr<CContextImpl> appContext;
        CContextImpl::NewByFriend((CContextImpl**)&appContext);
        appContext->Init(this, NULL, (CActivityThread*)mActivityThread);
        ECode ec = mActivityThread->mInstrumentation->NewApplication(
                cl, appClass, appContext.Get(), (IApplication**)&app);
        if (FAILED(ec)) {
            Slogger::E(TAG, "Unable to instantiate application %s, ec: 0x%08x", appClass.string(), ec);
            *result = NULL;
            return E_RUNTIME_EXCEPTION;
        }
        // } catch (Exception e) {
        // if (!mActivityThread.mInstrumentation.onException(app, e)) {
        //     throw new RuntimeException(
        //         "Unable to instantiate application " + appClass
        //         + ": " + e.toString(), e);
        // }
        appContext->SetOuterContext(app);
    }
    else {
        //    try {
        String appSourceDir, packageName;
        mApplicationInfo->GetSourceDir(&appSourceDir);
        mApplicationInfo->GetPackageName(&packageName);
        StringBuilder sb;
        if (appSourceDir.EndWith(".epk")) {
            sb.AppendCStr("/data/elastos/");
            sb.AppendString(packageName);
            sb.AppendCStr(".eco");
        }
        else {
            sb.AppendCStr("/data/data/com.elastos.runtime/elastos/");
            sb.AppendString(packageName);
            sb.AppendCStr("/");
            sb.AppendString(packageName);
            sb.AppendCStr(".eco");
        }
        String path = sb.ToString();
        AutoPtr<IModuleInfo> moduleInfo;
        if (FAILED(CReflector::AcquireModuleInfo(path.string(), (IModuleInfo**)&moduleInfo))) {
            Slogger::E(TAG, "HandleBindApplication: Cann't Find the Instrumentation path is %s", path.string());
            return E_RUNTIME_EXCEPTION;
        }
        Int32 index = appClass.LastIndexOf('.');
        String shortClassName = index > 0 ? appClass.Substring(index + 1) : appClass;
        AutoPtr<IClassInfo> classInfo;
        if (FAILED(moduleInfo->GetClassInfo(shortClassName, (IClassInfo**)&classInfo))) {
            Slogger::E(TAG, "HandleBindApplication: Get class info of %s failed.", shortClassName.string());
            return E_RUNTIME_EXCEPTION;
        }

        AutoPtr<CContextImpl> appContext;
        CContextImpl::NewByFriend((CContextImpl**)&appContext);
        appContext->Init(this, NULL, (CActivityThread*)mActivityThread);
        AutoPtr<IInstrumentationHelper> helper;
        CInstrumentationHelper::AcquireSingleton((IInstrumentationHelper**)&helper);
        ECode ec = helper->NewApplication(classInfo, appContext, (IApplication**)&app);
        if (FAILED(ec)) {
            Slogger::E(TAG, "Unable to instantiate application %s, ec: 0x%08x", appClass.string(), ec);
            *result = NULL;
            return E_RUNTIME_EXCEPTION;
        }
    //    } catch (Exception e) {
    //        if (!mActivityThread.mInstrumentation.onException(app, e)) {
    //            throw new RuntimeException(
    //                "Unable to instantiate application " + appClass
    //                + ": " + e.toString(), e);
    //        }
    //    }
        appContext->SetOuterContext(app);
    }

    ((CActivityThread*)mActivityThread)->mAllApplications.PushBack(app);
    mApplication = app;

    if (instrumentation != NULL) {
//        try {
        ECode ec = instrumentation->CallApplicationOnCreate(app);
        if (FAILED(ec)) {
            Boolean val;
            instrumentation->OnException(app, ec, &val);
            if (!val) {
                return E_RUNTIME_EXCEPTION;
            }
        }
//        } catch (Exception e) {
//            if (!instrumentation.onException(app, e)) {
//                throw new RuntimeException(
//                    "Unable to create application " + app.getClass().getName()
//                    + ": " + e.toString(), e);
//            }
//        }
    }

    *result = app;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode LoadedPkg::RemoveContextRegistrations(
    /* [in] */ IContext* context,
    /* [in] */ const String& who,
    /* [in] */ const String& what)
{
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();

    // final boolean reportRegistrationLeaks = StrictMode.vmRegistrationLeaksEnabled();
    HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> >::Iterator it = mReceivers.Find(context);
    AutoPtr<ReceiverMap> rmap;
    if (it != mReceivers.End()) {
        rmap = it->mSecond;
        mReceivers.Erase(it);
    }

    if (rmap != NULL) {
        ReceiverMapIterator it = rmap->Begin();
        while (it != rmap->End()) {
            AutoPtr<ReceiverDispatcher> rd = it->mSecond;

            // IntentReceiverLeaked leak = new IntentReceiverLeaked(
            //         what + " " + who + " has leaked IntentReceiver "
            //         + rd.getIntentReceiver() + " that was " +
            //         "originally registered here. Are you missing a " +
            //         "call to unregisterReceiver()?");
            // leak.setStackTrace(rd.getLocation().getStackTrace());
            Slogger::E(TAG, "%s has leaked IntentReceiver that was originally registered here."
                " Are you missing a call to unregisterReceiver()?", who.string());
            // if (reportRegistrationLeaks) {
            //     StrictMode.onIntentReceiverLeaked(leak);
            // }

            // try {
            AutoPtr<IIntentReceiver> ir = rd->GetIIntentReceiver();
            am->UnregisterReceiver(ir);
            // } catch (RemoteException e) {
            //     // system crashed, nothing we can do
            // }
        }
    }

    it = mUnregisteredReceivers.Find(context);
    if (it != mUnregisteredReceivers.End()) {
        mUnregisteredReceivers.Erase(it);
    }

    // Slogger::I(TAG, "RemoveContextRegistrations: %s, %s", who.string(), what.string());

    AutoPtr<ServiceMap> smap;
    HashMap<AutoPtr<IContext>, AutoPtr<ServiceMap> >::Iterator sit;

    sit = mServices.Find(context);
    if (sit != mServices.End()) {
        smap = sit->mSecond;
        mServices.Erase(sit);
    }

    if (smap != NULL) {
        Boolean bv;
        ServiceMapIterator it = smap->Begin();
        while (it != smap->End()) {
            AutoPtr<ServiceDispatcher> sd = it->mSecond;

            // ServiceConnectionLeaked leak = new ServiceConnectionLeaked(
            //         what + " " + who + " has leaked ServiceConnection "
            //         + sd.getServiceConnection() + " that was originally bound here");
            // leak.setStackTrace(sd.getLocation().getStackTrace());
            Slogger::E(TAG, "%s has leaked ServiceConnection that was originally bound here.", who.string());
            // if (reportRegistrationLeaks) {
            //     StrictMode.onServiceConnectionLeaked(leak);
            // }

            // try {
            AutoPtr<IIServiceConnection> sc = sd->GetIServiceConnection();
            am->UnbindService(sc, &bv);
            // } catch (RemoteException e) {
            //     // system crashed, nothing we can do
            // }
            sd->DoForget();
        }
    }

    sit = mUnboundServices.Find(context);
    if (sit != mUnboundServices.End()) {
        mUnboundServices.Erase(sit);
    }

    return NOERROR;
}

ECode LoadedPkg::GetReceiverDispatcher(
    /* [in] */ IBroadcastReceiver* r,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IInstrumentation* instrumentation,
    /* [in] */ Boolean registered,
    /* [out] */ IIntentReceiver** ir)
{
    VALIDATE_NOT_NULL(ir);
    *ir = NULL;

    AutoLock lock(mReceiversLock);

    AutoPtr<ReceiverDispatcher> rd;
    AutoPtr<ReceiverMap> map;
    if (registered) {
        HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> >::Iterator it = mReceivers.Find(context);
        if (it != mReceivers.End()) {
            map = it->mSecond;
            if (map.Get() != NULL){
                ReceiverMapIterator ite = map->Find(r);
                if (ite != map->End()) {
                    rd = ite->mSecond;
                }
            }
        }
    }

    if (rd == NULL){
        rd = new ReceiverDispatcher(r, context, handler, instrumentation, registered);
        if (registered) {
            if (map == NULL) {
                map = new ReceiverMap();
                mReceivers[context] = map;
            }
            (*map)[r] = rd;
        }
    }
    else {
        FAIL_RETURN(rd->Validate(context, handler));
    }

    rd->mForgotten = FALSE;
    AutoPtr<IIntentReceiver> _ir = rd->GetIIntentReceiver();
    *ir = _ir;
    REFCOUNT_ADD(*ir);
    return NOERROR;
}

ECode LoadedPkg::ForgetReceiverDispatcher(
    /* [in] */ IContext* context,
    /* [in] */ IBroadcastReceiver* r,
    /* [out] */ IIntentReceiver** ir)
{
    VALIDATE_NOT_NULL(ir);
    *ir = NULL;

    AutoLock lock(mReceiversLock);

    AutoPtr<ReceiverDispatcher> rd;
    AutoPtr<ReceiverMap> map, holder;
    HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> >::Iterator it = mReceivers.Find(context);
    if (it != mReceivers.End()) map = it->mSecond;
    if (map != NULL) {
        ReceiverMapIterator ite = map->Find(r);
        if (ite != map->End()) rd = ite->mSecond;
        if (rd != NULL) {
            map->Erase(ite);
            if (map->Begin() == map->End()) {
                mReceivers.Erase(it);
                map = NULL;
            }
            Boolean b;
            r->GetDebugUnregister(&b);
            if (b) {
                HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> >::Iterator iter = mUnregisteredReceivers.Find(context);
                if (iter != mUnregisteredReceivers.End()) holder = iter->mSecond;
                if (holder == NULL) {
                    holder = new ReceiverMap();
                    mUnregisteredReceivers[context] = holder;
                }
            // RuntimeException ex = new IllegalArgumentException(
            //         "Originally unregistered here:");
            // ex.fillInStackTrace();
            // rd.setUnregisterLocation(ex);
                (*holder)[r] = rd;
            }
            rd->mForgotten = TRUE;
            AutoPtr<IIntentReceiver> _ir = rd->GetIIntentReceiver();
            *ir = _ir;
            REFCOUNT_ADD(*ir);
            return NOERROR;
        }
    }

    HashMap<AutoPtr<IContext>, AutoPtr<ReceiverMap> >::Iterator iter =  mUnregisteredReceivers.Find(context);
    if (iter != mUnregisteredReceivers.End()) holder = iter->mSecond;
    if (holder != NULL) {
        ReceiverMapIterator ite = holder->Find(r);
        if (ite != holder->End()) rd = ite->mSecond;
        if (rd != NULL) {
        //         RuntimeException ex = rd.getUnregisterLocation();
        //         throw new IllegalArgumentException(
        //                 "Unregistering Receiver " + r
        //                 + " that was already unregistered", ex);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    if (context == NULL) {
        //     throw new IllegalStateException("Unbinding Receiver " + r
        //             + " from Context that is no longer in use: " + context);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
        //     throw new IllegalArgumentException("Receiver not registered: " + r);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode LoadedPkg::GetServiceDispatcher(
    /* [in] */ IServiceConnection* c,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ Int32 flags,
    /* [out] */ IIServiceConnection** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoLock lock(mServicesLock);

    AutoPtr<ServiceDispatcher> sd;
    AutoPtr< ServiceMap > map = mServices[context];
    if (map != NULL) {
        sd = (*map)[c];
    }
    if (sd == NULL) {
        sd = new ServiceDispatcher(c, context, handler, flags);
        if (map == NULL) {
            map = new ServiceMap();
            mServices[context] = map;
        }
        (*map)[c] = sd;
    } else {
        sd->Validate(context, handler);
    }
    AutoPtr<IIServiceConnection> con = sd->GetIServiceConnection();
    *result = con.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode LoadedPkg::ForgetServiceDispatcher(
    /* [in] */ IContext* context,
    /* [in] */ IServiceConnection* c,
    /* [out] */ IIServiceConnection** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoLock lock(mServicesLock);

    AutoPtr<ServiceDispatcher> sd;
    AutoPtr<ServiceMap> map;
    HashMap<AutoPtr<IContext>, AutoPtr<ServiceMap> >::Iterator it = mServices.Find(context);
    if (it != mServices.End()) {
        map = it->mSecond;
    }
    if (map != NULL) {
        ServiceMapIterator sdit = map->Find(c);
        if (sdit != map->End()) {
            sd = sdit->mSecond;
        }
        if (sd != NULL) {
            map->Erase(sdit);
            sd->DoForget();
            if (map->Begin() == map->End()) {
                mServices.Erase(it);
                map = NULL;
            }
            if ((sd->GetFlags() & IContext::BIND_DEBUG_UNBIND) != 0) {
                AutoPtr<ServiceMap> holder = mUnboundServices[context];
                if (holder == NULL) {
                    holder = new ServiceMap();
                    mUnboundServices[context] = holder;
                }
//                RuntimeException ex = new IllegalArgumentException(
//                        "Originally unbound here:");
//                ex.fillInStackTrace();
//                sd.setUnbindLocation(ex);
                (*holder)[c] = sd;
            }
            AutoPtr<IIServiceConnection> con = sd->GetIServiceConnection();
            *result = con.Get();
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }

    AutoPtr<ServiceMap> holder;
    it = mUnboundServices.Find(context);
    if (it != mUnboundServices.End()) {
        holder = it->mSecond;
    }
    if (holder != NULL) {
        ServiceMapIterator sdit = holder->Find(c);
        if (sdit != holder->End()) {
            sd = sdit->mSecond;
        }
        if (sd != NULL) {
//            RuntimeException ex = sd.getUnbindLocation();
//            throw new IllegalArgumentException(
//                    "Unbinding Service " + c
//                    + " that was already unbound", ex);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    if (context == NULL) {
//        throw new IllegalStateException("Unbinding Service " + c
//                + " from Context that is no longer in use: " + context);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else {
//        throw new IllegalArgumentException("Service not registered: " + c);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

}
}
}
