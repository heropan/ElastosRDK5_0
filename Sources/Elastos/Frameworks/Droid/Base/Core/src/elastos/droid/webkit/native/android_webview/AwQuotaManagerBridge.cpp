#include "elastos/droid/webkit/native/android_webview/AwQuotaManagerBridge.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"

//using Elastos::Droid::Utility::CSparseArray;TODO now use hashmap replace this
using Elastos::Droid::Webkit::Base::ThreadUtils;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//                 AwQuotaManagerBridge::Origins
//===============================================================

AwQuotaManagerBridge::Origins::Origins(
    /* [in] */ ArrayOf<String>* origins,
    /* [in] */ ArrayOf<Int64>* usages,
    /* [in] */ ArrayOf<Int64>* quotas)
    : mOrigins(origins)
    , mUsages(usages)
    , mQuotas(quotas)
{
}

//===============================================================
//                   AwQuotaManagerBridge
//===============================================================

AutoPtr<AwQuotaManagerBridge> AwQuotaManagerBridge::sInstance;

AwQuotaManagerBridge::AwQuotaManagerBridge(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl)
    : mNativeAwQuotaManagerBridgeImpl(nativeAwQuotaManagerBridgeImpl)
    , mNextId(0)
{
    //mPendingGetOriginCallbacks = new SparseArray<ValueCallback<Origins>>();TODO
    //mPendingGetQuotaForOriginCallbacks = new SparseArray<ValueCallback<Long>>();TODO
    //mPendingGetUsageForOriginCallbacks = new SparseArray<ValueCallback<Long>>();TODO
    NativeInit(mNativeAwQuotaManagerBridgeImpl);
}

// TODO(boliu): This should be obtained from Java AwBrowserContext that owns this.
Int64 AwQuotaManagerBridge::NativeGetDefaultNativeAwQuotaManagerBridge()
{
    return 0;
}

AutoPtr<AwQuotaManagerBridge> AwQuotaManagerBridge::GetInstance()
{
    ThreadUtils::AssertOnUiThread();
    if (sInstance == NULL) {
        sInstance = new AwQuotaManagerBridge(NativeGetDefaultNativeAwQuotaManagerBridge());
    }
    return sInstance;
}

Int32 AwQuotaManagerBridge::GetNextId()
{
    ThreadUtils::AssertOnUiThread();
    return ++mNextId;
}

/*
 * There are five HTML5 offline storage APIs.
 * 1) Web Storage (ie the localStorage and sessionStorage variables)
 * 2) Web SQL database
 * 3) Application cache
 * 4) Indexed Database
 * 5) Filesystem API
 */

/**
 * Implements WebStorage.deleteAllData(). Clear the storage of all five offline APIs.
 *
 * TODO(boliu): Actually clear Web Storage.
 */
void AwQuotaManagerBridge::DeleteAllData()
{
    NativeDeleteAllData(mNativeAwQuotaManagerBridgeImpl);
}

/**
 * Implements WebStorage.deleteOrigin(). Clear the storage of APIs 2-5 for the given origin.
 */
void AwQuotaManagerBridge::DeleteOrigin(
    /* [in] */ const String& origin)
{
    NativeDeleteOrigin(mNativeAwQuotaManagerBridgeImpl, origin);
}

/**
 * Implements WebStorage.getOrigins. Get the per origin usage and quota of APIs 2-5 in
 * aggregate.
 */
void AwQuotaManagerBridge::GetOrigins(
    /* [in] */ /*TODO IValueCallback*/IInterface* callback)
{
    Int32 callbackId = GetNextId();
    assert(mPendingGetOriginCallbacks[callbackId] == NULL);
    mPendingGetOriginCallbacks[callbackId] =  callback;
    NativeGetOrigins(mNativeAwQuotaManagerBridgeImpl, callbackId);
}

/**
 * Implements WebStorage.getQuotaForOrigin. Get the quota of APIs 2-5 in aggregate for given
 * origin.
 */
void AwQuotaManagerBridge::GetQuotaForOrigin(
    /* [in] */ const String& origin, /*TODO IValueCallback*/IInterface* callback)
{
    Int32 callbackId = GetNextId();
    assert(mPendingGetQuotaForOriginCallbacks[callbackId] == NULL);
    mPendingGetQuotaForOriginCallbacks[callbackId] = callback;
    NativeGetUsageAndQuotaForOrigin(mNativeAwQuotaManagerBridgeImpl, origin, callbackId, true);
}

/**
 * Implements WebStorage.getUsageForOrigin. Get the usage of APIs 2-5 in aggregate for given
 * origin.
 */
void AwQuotaManagerBridge::GetUsageForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ /*TODO IValueCallback*/IInterface* callback)
{
    Int32 callbackId = GetNextId();
    assert(mPendingGetUsageForOriginCallbacks[callbackId] == NULL);
    mPendingGetUsageForOriginCallbacks[callbackId]  = callback;
    NativeGetUsageAndQuotaForOrigin(mNativeAwQuotaManagerBridgeImpl, origin, callbackId, false);
}

//@CalledByNative
void AwQuotaManagerBridge::OnGetOriginsCallback(
    /* [in] */ Int32 callbackId,
    /* [in] */ ArrayOf<String>* origin,
    /* [in] */ ArrayOf<Int64>* usages,
    /* [in] */ ArrayOf<Int64>* quotas)
{
    assert(mPendingGetOriginCallbacks[callbackId] != NULL);
    //TODO mPendingGetOriginCallbacks[callbackId]->OnReceiveValue(new Origins(origin, usages, quotas));
    mPendingGetOriginCallbacks.Erase(callbackId);
}

//@CalledByNative
void AwQuotaManagerBridge::OnGetUsageAndQuotaForOriginCallback(
    /* [in] */ Int32 callbackId,
    /* [in] */ Boolean isQuota,
    /* [in] */ Int64 usage,
    /* [in] */ Int64 quota)
{
    if (isQuota) {
        assert(mPendingGetQuotaForOriginCallbacks[callbackId] != NULL);
        AutoPtr<IInteger64> iQuota;
        CInteger64::New(quota, (IInteger64**)&iQuota);
        //TODO mPendingGetQuotaForOriginCallbacks[callbackId]->OnReceiveValue(iQuota);
        mPendingGetQuotaForOriginCallbacks.Erase(callbackId);
    }
    else {
        assert(mPendingGetUsageForOriginCallbacks[callbackId] != NULL);
        AutoPtr<IInteger64> iUsage;
        CInteger64::New(usage, (IInteger64**)&iUsage);
        //TODO mPendingGetUsageForOriginCallbacks[callbackId]->OnReceiveValue(iUsage);
        mPendingGetUsageForOriginCallbacks.Erase(callbackId);
    }
}

void AwQuotaManagerBridge::NativeInit(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl)
{
}

void AwQuotaManagerBridge::NativeDeleteAllData(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl)
{
}

void AwQuotaManagerBridge::NativeDeleteOrigin(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
    /* [in] */ const String& origin)
{
}

void AwQuotaManagerBridge::NativeGetOrigins(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
    /* [in] */ Int32 callbackId)
{
}

void AwQuotaManagerBridge::NativeGetUsageAndQuotaForOrigin(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
    /* [in] */ const String& origin,
    /* [in] */ Int32 callbackId,
    /* [in] */ Boolean isQuota)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
