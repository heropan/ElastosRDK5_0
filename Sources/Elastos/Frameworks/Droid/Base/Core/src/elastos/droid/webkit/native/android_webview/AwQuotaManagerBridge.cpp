
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
    : mNextId(0)
    , mNativeAwQuotaManagerBridgeImpl(nativeAwQuotaManagerBridgeImpl)
{
    mPendingGetOriginCallbacks =
            new SparseArray<ValueCallback<Origins>>();
    mPendingGetQuotaForOriginCallbacks = new SparseArray<ValueCallback<Long>>();
    mPendingGetUsageForOriginCallbacks = new SparseArray<ValueCallback<Long>>();
    NativeInit(mNativeAwQuotaManagerBridgeImpl);
}

// TODO(boliu): This should be obtained from Java AwBrowserContext that owns this.
Int64 AwQuotaManagerBridge::NativeGetDefaultNativeAwQuotaManagerBridge()
{
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
    /* [in] */ String origin)
{
    NativeDeleteOrigin(mNativeAwQuotaManagerBridgeImpl, origin);
}

/**
 * Implements WebStorage.getOrigins. Get the per origin usage and quota of APIs 2-5 in
 * aggregate.
 */
void AwQuotaManagerBridge::GetOrigins(
    /* [in] */ IValueCallback* callback)
{
    Int32 callbackId = GetNextId();
    assert mPendingGetOriginCallbacks.get(callbackId) == null;
    mPendingGetOriginCallbacks.put(callbackId, callback);
    nativeGetOrigins(mNativeAwQuotaManagerBridgeImpl, callbackId);
}

/**
 * Implements WebStorage.getQuotaForOrigin. Get the quota of APIs 2-5 in aggregate for given
 * origin.
 */
void AwQuotaManagerBridge::GetQuotaForOrigin(
    /* [in] */ String origin, ValueCallback<Long> callback)
{
    Int32 callbackId = getNextId();
    assert mPendingGetQuotaForOriginCallbacks.get(callbackId) == null;
    mPendingGetQuotaForOriginCallbacks.put(callbackId, callback);
    NativeGetUsageAndQuotaForOrigin(mNativeAwQuotaManagerBridgeImpl, origin, callbackId, true);
}

/**
 * Implements WebStorage.getUsageForOrigin. Get the usage of APIs 2-5 in aggregate for given
 * origin.
 */
void AwQuotaManagerBridge::GetUsageForOrigin(
    /* [in] */ String origin,
    /* [in] */ IValueCallback* callback)
{
    Int32 callbackId = GetNextId();
    assert mPendingGetUsageForOriginCallbacks.get(callbackId) == null;
    mPendingGetUsageForOriginCallbacks.put(callbackId, callback);
    NativeGetUsageAndQuotaForOrigin(mNativeAwQuotaManagerBridgeImpl, origin, callbackId, false);
}

//@CalledByNative
void AwQuotaManagerBridge::OnGetOriginsCallback(
    /* [in] */ Int32 callbackId,
    /* [in] */ ArrayOf<String>* origin,
    /* [in] */ ArrayOf<Int64>* usages,
    /* [in] */ ArrayOf<Int64>* quotas)
{
    assert mPendingGetOriginCallbacks.get(callbackId) != null;
    mPendingGetOriginCallbacks.get(callbackId).onReceiveValue(
        new Origins(origin, usages, quotas));
    mPendingGetOriginCallbacks.remove(callbackId);
}

//@CalledByNative
void AwQuotaManagerBridge::OnGetUsageAndQuotaForOriginCallback(
    /* [in] */ Int32 callbackId,
    /* [in] */ Boolean isQuota,
    /* [in] */ Int64 usage,
    /* [in] */ Int64 quota)
{
    if (isQuota) {
        assert mPendingGetQuotaForOriginCallbacks.get(callbackId) != null;
        mPendingGetQuotaForOriginCallbacks.get(callbackId).onReceiveValue(quota);
        mPendingGetQuotaForOriginCallbacks.remove(callbackId);
    }
    else {
        assert mPendingGetUsageForOriginCallbacks.get(callbackId) != null;
        mPendingGetUsageForOriginCallbacks.get(callbackId).onReceiveValue(usage);
        mPendingGetUsageForOriginCallbacks.remove(callbackId);
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
    /* [in] */ String origin)
{
}

void AwQuotaManagerBridge::NativeGetOrigins(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
    /* [in] */ Int32 callbackId)
{
}

void AwQuotaManagerBridge::NativeGetUsageAndQuotaForOrigin(
    /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
    /* [in] */ String origin,
    /* [in] */ Int32 callbackId,
    /* [in] */ Boolean isQuota)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
