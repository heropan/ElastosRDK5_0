
#include "content/PackageMonitor.h"
#ifdef DROID_CORE
#include "content/CIntentFilter.h"
#endif
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::EIID_IBroadcastReceiver;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Content {

static AutoPtr<IIntentFilter> InitPackageFilt()
{
    AutoPtr<IIntentFilter> filter;
#ifdef DROID_CORE
    AutoPtr<CIntentFilter> cfilter;
    ASSERT_SUCCEEDED(CIntentFilter::NewByFriend((CIntentFilter**)&cfilter))
    filter = cfilter;
#else
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter))
#endif

    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.PACKAGE_ADDED")/*IIntent::ACTION_PACKAGE_ADDED*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.PACKAGE_REMOVED")/*IIntent::ACTION_PACKAGE_REMOVED*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.PACKAGE_CHANGED")/*IIntent::ACTION_PACKAGE_CHANGED*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.QUERY_PACKAGE_RESTART")/*IIntent::ACTION_QUERY_PACKAGE_RESTART*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.PACKAGE_RESTARTED")/*IIntent::ACTION_PACKAGE_RESTARTED*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.UID_REMOVED")/*IIntent::ACTION_UID_REMOVED*/))
    ASSERT_SUCCEEDED(filter->AddDataScheme(String("package")))
    return filter;
}

static AutoPtr<IIntentFilter> InitNonDataFilt()
{
    AutoPtr<IIntentFilter> filter;
#ifdef DROID_CORE
    AutoPtr<CIntentFilter> cfilter;
    ASSERT_SUCCEEDED(CIntentFilter::NewByFriend((CIntentFilter**)&cfilter))
    filter = cfilter;
#else
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter))
#endif

    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.UID_REMOVED")/*IIntent::ACTION_UID_REMOVED*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.USER_STOPPED")/*IIntent::ACTION_USER_STOPPED*/))
    return filter;
}

static AutoPtr<IIntentFilter> InitExternalFilt()
{
    AutoPtr<IIntentFilter> filter;
#ifdef DROID_CORE
    AutoPtr<CIntentFilter> cfilter;
    ASSERT_SUCCEEDED(CIntentFilter::NewByFriend((CIntentFilter**)&cfilter))
    filter = cfilter;
#else
    ASSERT_SUCCEEDED(CIntentFilter::New((IIntentFilter**)&filter))
#endif

    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.EXTERNAL_APPLICATIONS_AVAILABLE")
        /*IIntent::ACTION_EXTERNAL_APPLICATIONS_AVAILABLE*/))
    ASSERT_SUCCEEDED(filter->AddAction(
        String("android.intent.action.EXTERNAL_APPLICATIONS_UNAVAILABLE")
        /*IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE)*/))
    return filter;
}

AutoPtr<IIntentFilter> PackageMonitor::sPackageFilt = InitPackageFilt();
AutoPtr<IIntentFilter> PackageMonitor::sNonDataFilt = InitNonDataFilt();
AutoPtr<IIntentFilter> PackageMonitor::sExternalFilt = InitExternalFilt();
Mutex PackageMonitor::sLock;
AutoPtr<IHandlerThread> PackageMonitor::sBackgroundThread;
AutoPtr<IHandler> PackageMonitor::sBackgroundHandler;

PackageMonitor::PackageMonitor()
    : mChangeType(0)
    , mChangeUserId(IUserHandle::USER_NULL)
    , mSomePackagesChanged(FALSE)
{
    mTempArray = ArrayOf<String>::Alloc(1);
}

PackageMonitor::~PackageMonitor()
{
}

PInterface PackageMonitor::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IPackageMonitor *)this;
    }
    else if ( riid == EIID_IPackageMonitor ) {
        return (IPackageMonitor *)this;
    }
    return BroadcastReceiver::Probe(riid);
}

ECode PackageMonitor::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid)
    if (object == (IInterface*)(IPackageMonitor *)this) {
        *iid = EIID_IPackageMonitor;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return BroadcastReceiver::GetInterfaceID(object, iid);
}

UInt32 PackageMonitor::AddRef()
{
    return BroadcastReceiver::AddRef();
}

UInt32 PackageMonitor::Release()
{
    return BroadcastReceiver::Release();
}

ECode PackageMonitor::GoAsync(
    /* [out] */ IBroadcastReceiverPendingResult** pendingResult)
{
    return BroadcastReceiver::GoAsync(pendingResult);
}

ECode PackageMonitor::PeekService(
    /* [in] */ IContext* myContext,
    /* [in] */ IIntent* service,
    /* [out] */ IBinder** binder)
{
    return BroadcastReceiver::PeekService(myContext, service, binder);
}

ECode PackageMonitor::SetResultCode(
    /* [in] */ Int32 code)
{
    return BroadcastReceiver::SetResultCode(code);
}

ECode PackageMonitor::GetResultCode(
    /* [out] */ Int32* code)
{
    return BroadcastReceiver::GetResultCode(code);
}

ECode PackageMonitor::SetResultData(
    /* [in] */ const String& data)
{
    return BroadcastReceiver::SetResultData(data);
}

ECode PackageMonitor::GetResultData(
    /* [out] */ String* data)
{
    return BroadcastReceiver::GetResultData(data);
}

ECode PackageMonitor::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::SetResultExtras(extras);
}

ECode PackageMonitor::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** extras)
{
    return BroadcastReceiver::GetResultExtras(makeMap, extras);
}

ECode PackageMonitor::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::SetResult(code, data, extras);
}

ECode PackageMonitor::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    return BroadcastReceiver::GetAbortBroadcast(isAborted);
}

ECode PackageMonitor::AbortBroadcast()
{
    return BroadcastReceiver::AbortBroadcast();
}

ECode PackageMonitor::ClearAbortBroadcast()
{
    return BroadcastReceiver::ClearAbortBroadcast();
}

ECode PackageMonitor::IsOrderedBroadcast(
    /* [out] */ Boolean* isOrdered)
{
    return BroadcastReceiver::IsOrderedBroadcast(isOrdered);
}

ECode PackageMonitor::IsInitialStickyBroadcast(
    /* [out] */ Boolean* isInitial)
{
    return BroadcastReceiver::IsInitialStickyBroadcast(isInitial);
}

ECode PackageMonitor::SetOrderedHint(
    /* [in] */ Boolean isOrdered)
{
    return BroadcastReceiver::SetOrderedHint(isOrdered);
}

ECode PackageMonitor::SetPendingResult(
    /* [in] */ IBroadcastReceiverPendingResult* result)
{
    return BroadcastReceiver::SetPendingResult(result);
}

ECode PackageMonitor::GetPendingResult(
    /* [out] */ IBroadcastReceiverPendingResult** pendingResult)
{
    return BroadcastReceiver::GetPendingResult(pendingResult);
}

ECode PackageMonitor::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return BroadcastReceiver::GetSendingUserId(userId);
}

ECode PackageMonitor::SetDebugUnregister(
    /* [in] */ Boolean debug)
{
    return BroadcastReceiver::SetDebugUnregister(debug);
}

ECode PackageMonitor::GetDebugUnregister(
    /* [out] */ Boolean* debugUnregister)
{
    return BroadcastReceiver::GetDebugUnregister(debugUnregister);
}

ECode PackageMonitor::Register(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* thread,
    /* [in] */ Boolean externalStorage)
{
    return Register(context, thread, NULL, externalStorage);
}

ECode PackageMonitor::Register(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* thread,
    /* [in] */ IUserHandle* user,
    /* [in] */ Boolean externalStorage)
{
    if (NULL != mRegisteredContext) {
        // throw new IllegalStateException("Already registered");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mRegisteredContext = context;
    if (NULL == thread) {
        {
            Mutex::Autolock lock(sLock);
            if (NULL == sBackgroundThread) {
//                FAIL_RETURN(CHandlerThread::New("PackageMonitor", IProcess::THREAD_PRIORITY_BACKGROUND,
//                        (IHandlerThread**)&sBackgroundThread))
//                FAIL_RETURN(sBackgroundThread->Start())
//                sBackgroundHandler = new Handler(sBackgroundThread.getLooper());
            }
            mRegisteredHandler = sBackgroundHandler;
        }
    }
    else {
//        mRegisteredHandler = new Handler(thread);
    }

    AutoPtr<IIntent> intent;
    if (NULL != user) {
        FAIL_RETURN(context->RegisterReceiverAsUser(THIS_PROBE(IBroadcastReceiver),
                user, sPackageFilt, String(NULL), mRegisteredHandler, (IIntent**)&intent))
        intent = NULL;
        FAIL_RETURN(context->RegisterReceiverAsUser(THIS_PROBE(IBroadcastReceiver),
                user, sNonDataFilt, String(NULL), mRegisteredHandler, (IIntent**)&intent))
        if (externalStorage) {
            intent = NULL;
            FAIL_RETURN(context->RegisterReceiverAsUser(THIS_PROBE(IBroadcastReceiver),
                    user, sExternalFilt, String(NULL), mRegisteredHandler, (IIntent**)&intent))
        }
    }
    else {
        FAIL_RETURN(context->RegisterReceiver(THIS_PROBE(IBroadcastReceiver),
                sPackageFilt, String(NULL), mRegisteredHandler, (IIntent**)&intent))
        intent = NULL;
        FAIL_RETURN(context->RegisterReceiver(THIS_PROBE(IBroadcastReceiver),
                sNonDataFilt, String(NULL), mRegisteredHandler, (IIntent**)&intent))
        if (externalStorage) {
            intent = NULL;
            FAIL_RETURN(context->RegisterReceiver(THIS_PROBE(IBroadcastReceiver),
                    sExternalFilt, String(NULL), mRegisteredHandler, (IIntent**)&intent))
        }
    }
    return NOERROR;
}

ECode PackageMonitor::GetRegisteredHandler(
    /* [out] */ IHandler** handler)
{
    VALIDATE_NOT_NULL(handler)
    *handler = mRegisteredHandler;
    REFCOUNT_ADD(*handler);
    return NOERROR;
}

ECode PackageMonitor::Unregister()
{
    if (NULL == mRegisteredContext) {
        // throw new IllegalStateException("Not registered");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    FAIL_RETURN(mRegisteredContext->UnregisterReceiver(THIS_PROBE(IBroadcastReceiver)))
    mRegisteredContext = NULL;
    return NOERROR;
}

ECode PackageMonitor::OnBeginPackageChanges()
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageAdded(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageRemoved(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageRemovedAllUsers(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageUpdateStarted(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageUpdateFinished(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageChanged(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 uid,
    /* [in] */ ArrayOf<String>* components)
{
    return NOERROR;
}

ECode PackageMonitor::OnHandleForceStop(
    /* [in] */ IIntent* intent,
    /* [in] */ ArrayOf<String>* packages,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean doit,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode PackageMonitor::OnHandleUserStop(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 userHandle)
{
    return NOERROR;
}

ECode PackageMonitor::OnUidRemoved(
    /* [in] */ Int32 uid)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackagesAvailable(
    /* [in] */ ArrayOf<String>* packages)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackagesUnavailable(
    /* [in] */ ArrayOf<String>* packages)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageDisappeared(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 reason)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageAppeared(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 reason)
{
    return NOERROR;
}

ECode PackageMonitor::OnPackageModified(
    /* [in] */ const String& packageName)
{
    return NOERROR;
}

ECode PackageMonitor::DidSomePackagesChange(
    /* [out] */ Boolean* isChanged)
{
    VALIDATE_NOT_NULL(isChanged)
    *isChanged = mSomePackagesChanged;
    return NOERROR;
}

ECode PackageMonitor::IsPackageAppearing(
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    if (NULL != mAppearingPackages) {
        for (Int32 i = mAppearingPackages->GetLength() - 1; i >= 0; i--) {
            if (packageName.Equals((*mAppearingPackages)[i])) {
                *result = mChangeType;
                return NOERROR;
            }
        }
    }
    *result = IPackageMonitor::PACKAGE_UNCHANGED;
    return NOERROR;
}

ECode PackageMonitor::AnyPackagesAppearing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mAppearingPackages != NULL;
    return NOERROR;
}

ECode PackageMonitor::IsPackageDisappearing(
    /* [in] */ const String& packageName,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    if (NULL != mDisappearingPackages) {
        for (Int32 i = mDisappearingPackages->GetLength() - 1; i >= 0; i--) {
            if (packageName.Equals((*mDisappearingPackages)[i])) {
                *result = mChangeType;
                return NOERROR;
            }
        }
    }
    *result = IPackageMonitor::PACKAGE_UNCHANGED;
    return NOERROR;
}

ECode PackageMonitor::AnyPackagesDisappearing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mDisappearingPackages != NULL;
    return NOERROR;
}

ECode PackageMonitor::IsPackageModified(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* isModified)
{
    VALIDATE_NOT_NULL(isModified)
    if (NULL != mModifiedPackages) {
        for (Int32 i = mModifiedPackages->GetLength() - 1; i >= 0; i--) {
            if (packageName.Equals((*mModifiedPackages)[i])) {
                *isModified = TRUE;
                return NOERROR;
            }
        }
    }
    *isModified = FALSE;
    return NOERROR;
}

ECode PackageMonitor::OnSomePackagesChanged()
{
    return NOERROR;
}

ECode PackageMonitor::OnFinishPackageChanges()
{
    return NOERROR;
}

ECode PackageMonitor::GetChangingUserId(
    /* [out] */ Int32* userId)
{
    VALIDATE_NOT_NULL(userId)
    *userId = mChangeUserId;
    return NOERROR;
}

ECode PackageMonitor::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, IUserHandle::USER_NULL, &mChangeUserId))
    if (mChangeUserId == IUserHandle::USER_NULL) {
        // throw new IllegalArgumentException("Intent broadcast does not contain user handle: " + intent);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(OnBeginPackageChanges())
    mDisappearingPackages = NULL;
    mAppearingPackages = NULL;
    mSomePackagesChanged = FALSE;
    String action;
    FAIL_RETURN(intent->GetAction(&action))
    if (IIntent::ACTION_PACKAGE_ADDED.Equals(action)) {
        String pkg;
        FAIL_RETURN(GetPackageName(intent, &pkg))
        Int32 uid = 0;
        FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_UID, 0, &uid))
        // We consider something to have changed regardless of whether
        // this is just an update, because the update is now finished
        // and the contents of the package may have changed.
        mSomePackagesChanged = TRUE;
        if (!pkg.IsNull()) {
            mAppearingPackages = mTempArray;
            (*mTempArray)[0] = pkg;
            Boolean ret = FALSE;
            FAIL_RETURN(intent->GetBooleanExtra(IIntent::EXTRA_REPLACING, FALSE, &ret))
            if (ret) {
                mModifiedPackages = mTempArray;
                mChangeType = IPackageMonitor::PACKAGE_UPDATING;
                FAIL_RETURN(OnPackageUpdateFinished(pkg, uid))
                FAIL_RETURN(OnPackageModified(pkg))
            }
            else {
                mChangeType = IPackageMonitor::PACKAGE_PERMANENT_CHANGE;
                FAIL_RETURN(OnPackageAdded(pkg, uid))
            }
            FAIL_RETURN(OnPackageAppeared(pkg, mChangeType))
            if (mChangeType == IPackageMonitor::PACKAGE_UPDATING) {
                {
                    Mutex::Autolock lock(mUpdatingPackagesLock);
                    mUpdatingPackages.Erase(pkg);
                }
            }
        }
    }
    else if (IIntent::ACTION_PACKAGE_REMOVED.Equals(action)) {
        String pkg;
        FAIL_RETURN(GetPackageName(intent, &pkg))
        Int32 uid = 0;
        FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_UID, 0, &uid))
        if (!pkg.IsNull()) {
            mDisappearingPackages = mTempArray;
            (*mTempArray)[0] = pkg;
            Boolean ret = FALSE;
            FAIL_RETURN(intent->GetBooleanExtra(IIntent::EXTRA_REPLACING, FALSE, &ret))
            if (ret) {
                mChangeType = IPackageMonitor::PACKAGE_UPDATING;
                {
                    Mutex::Autolock lock(mUpdatingPackagesLock);
                    //not used for now
                    //mUpdatingPackages.add(pkg);
                }
                FAIL_RETURN(OnPackageUpdateStarted(pkg, uid))
            }
            else {
                mChangeType = IPackageMonitor::PACKAGE_PERMANENT_CHANGE;
                // We only consider something to have changed if this is
                // not a replace; for a replace, we just need to consider
                // it when it is re-added.
                mSomePackagesChanged = TRUE;
                FAIL_RETURN(OnPackageRemoved(pkg, uid))
                FAIL_RETURN(intent->GetBooleanExtra(IIntent::EXTRA_REMOVED_FOR_ALL_USERS, FALSE, &ret))
                if (ret) {
                    FAIL_RETURN(OnPackageRemovedAllUsers(pkg, uid))
                }
            }
            FAIL_RETURN(OnPackageDisappeared(pkg, mChangeType))
        }
    }
    else if (IIntent::ACTION_PACKAGE_CHANGED.Equals(action)) {
        String pkg;
        FAIL_RETURN(GetPackageName(intent, &pkg))
        Int32 uid = 0;
        FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_UID, 0, &uid))
        AutoPtr<ArrayOf<String> > components;
        FAIL_RETURN(intent->GetStringArrayExtra(IIntent::EXTRA_CHANGED_COMPONENT_NAME_LIST,
                (ArrayOf<String>**)&components))
        if (!pkg.IsNull()) {
            mModifiedPackages = mTempArray;
            (*mTempArray)[0] = pkg;
            FAIL_RETURN(OnPackageChanged(pkg, uid, components))
            // XXX Don't want this to always cause mSomePackagesChanged,
            // since it can happen a fair amount.
            FAIL_RETURN(OnPackageModified(pkg))
        }
    }
    else if (IIntent::ACTION_QUERY_PACKAGE_RESTART.Equals(action)) {
        mDisappearingPackages = NULL;
        FAIL_RETURN(intent->GetStringArrayExtra(IIntent::EXTRA_PACKAGES, (ArrayOf<String>**)&mDisappearingPackages))
        mChangeType = IPackageMonitor::PACKAGE_TEMPORARY_CHANGE;
        Boolean canRestart = FALSE;
        Int32 uid = 0;
        FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_UID, 0, &uid))
        FAIL_RETURN(OnHandleForceStop(intent, mDisappearingPackages, uid, FALSE, &canRestart))
        if (canRestart) {
            FAIL_RETURN(BroadcastReceiver::SetResultCode(IActivity::RESULT_OK))
        }
    }
    else if (IIntent::ACTION_PACKAGE_RESTARTED.Equals(action)) {
        mDisappearingPackages = ArrayOf<String>::Alloc(1);
        String pkg;
        FAIL_RETURN(GetPackageName(intent, &pkg))
        (*mDisappearingPackages)[0] = pkg;
        mChangeType = IPackageMonitor::PACKAGE_TEMPORARY_CHANGE;
        Int32 uid = 0;
        FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_UID, 0, &uid))
        Boolean ret = FALSE;
        FAIL_RETURN(OnHandleForceStop(intent, mDisappearingPackages, uid, TRUE, &ret))
    }
    else if (IIntent::ACTION_UID_REMOVED.Equals(action)) {
        Int32 uid = 0;
        FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_UID, 0, &uid))
        FAIL_RETURN(OnUidRemoved(uid))
    }
    else if (IIntent::ACTION_USER_STOPPED.Equals(action)) {
        Boolean ret = FALSE;
        FAIL_RETURN(intent->HasExtra(IIntent::EXTRA_USER_HANDLE, &ret))
        if (ret) {
            Int32 uid = 0;
            FAIL_RETURN(intent->GetInt32Extra(IIntent::EXTRA_USER_HANDLE, 0, &uid))
            FAIL_RETURN(OnHandleUserStop(intent, uid))
        }
    } else if (IIntent::ACTION_EXTERNAL_APPLICATIONS_AVAILABLE.Equals(action)) {
        AutoPtr<ArrayOf<String> > pkgList;
        FAIL_RETURN(intent->GetStringArrayExtra(IIntent::EXTRA_CHANGED_PACKAGE_LIST, (ArrayOf<String>**)&pkgList))
        mAppearingPackages = pkgList;
        mChangeType = IPackageMonitor::PACKAGE_TEMPORARY_CHANGE;
        mSomePackagesChanged = TRUE;
        if (NULL != pkgList) {
            FAIL_RETURN(OnPackagesAvailable(pkgList))
            for (Int32 i = 0; i < pkgList->GetLength(); i++) {
                FAIL_RETURN(OnPackageAppeared((*pkgList)[i], IPackageMonitor::PACKAGE_TEMPORARY_CHANGE))
            }
        }
    }
    else if (IIntent::ACTION_EXTERNAL_APPLICATIONS_UNAVAILABLE.Equals(action)) {
        AutoPtr<ArrayOf<String> > pkgList;
        FAIL_RETURN(intent->GetStringArrayExtra(IIntent::EXTRA_CHANGED_PACKAGE_LIST, (ArrayOf<String>**)&pkgList))
        mDisappearingPackages = pkgList;
        mChangeType =IPackageMonitor:: PACKAGE_TEMPORARY_CHANGE;
        mSomePackagesChanged = TRUE;
        if (NULL != pkgList) {
            FAIL_RETURN(OnPackagesUnavailable(pkgList))
            for (Int32 i = 0; i < pkgList->GetLength(); i++) {
                FAIL_RETURN(OnPackageDisappeared((*pkgList)[i], IPackageMonitor::PACKAGE_TEMPORARY_CHANGE))
            }
        }
    }

    if (mSomePackagesChanged) {
        FAIL_RETURN(OnSomePackagesChanged())
    }

    FAIL_RETURN(OnFinishPackageChanges())
    mChangeUserId = IUserHandle::USER_NULL;
    return NOERROR;
}

ECode PackageMonitor::IsPackageUpdating(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    Mutex::Autolock lock(mUpdatingPackagesLock);
    HashSet<String>::Iterator it = mUpdatingPackages.Find(packageName);
    if (it != mUpdatingPackages.End()) {
        *result = TRUE;
    }
    return NOERROR;
}

ECode PackageMonitor::GetPackageName(
    /* [in] */ IIntent* intent,
    /* [out] */ String* pkgName)
{
    VALIDATE_NOT_NULL(pkgName)
    AutoPtr<IUri> uri;
    FAIL_RETURN(intent->GetData((IUri**)&uri))
    if (NULL != uri) {
        FAIL_RETURN(uri->GetSchemeSpecificPart(pkgName))
    }
    else {
        *pkgName = String(NULL);
    }
    return NOERROR;
}

}
}
}
}

