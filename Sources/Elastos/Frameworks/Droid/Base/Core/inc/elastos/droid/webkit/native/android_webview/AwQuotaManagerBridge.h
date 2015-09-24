
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWQUOTAMANAGERBRIDGE_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWQUOTAMANAGERBRIDGE_H__

// import android.util.SparseArray;
// import android.webkit.ValueCallback;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Bridge between android.webview.WebStorage and native QuotaManager. This object is owned by Java
 * AwBrowserContext and the native side is owned by the native AwBrowserContext.
 *
 * TODO(boliu): Actually make this true after Java AwBrowserContext is added.
 */
//@JNINamespace("android_webview")
class AwQuotaManagerBridge
{
public:
    /**
     * This class represent the callback value of android.webview.WebStorage.getOrigins. The values
     * are optimized for JNI convenience and need to be converted.
     */
    class Origins
    {
    public:
        Origins(
            /* [in] */ ArrayOf<String>* origins,
            /* [in] */ ArrayOf<Int64>* usages,
            /* [in] */ ArrayOf<Int64>* quotas);

    private:
        // Origin, usage, and quota data in parallel arrays of same length.
        const AutoPtr< ArrayOf<String> > mOrigins;
        const AutoPtr< ArrayOf<Int64> > mUsages;
        const AutoPtr< ArrayOf<Int64> > mQuotas;
    };

public:
    static AutoPtr<AwQuotaManagerBridge> GetInstance();

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
    CARAPI_(void) DeleteAllData();

    /**
     * Implements WebStorage.deleteOrigin(). Clear the storage of APIs 2-5 for the given origin.
     */
    CARAPI_(void) DeleteOrigin(
        /* [in] */ const String& origin);

    /**
     * Implements WebStorage.getOrigins. Get the per origin usage and quota of APIs 2-5 in
     * aggregate.
     */
    CARAPI_(void) GetOrigins(
        /* [in] */ IValueCallback* callback);

    /**
     * Implements WebStorage.getQuotaForOrigin. Get the quota of APIs 2-5 in aggregate for given
     * origin.
     */
    CARAPI_(void) GetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callback);

    /**
     * Implements WebStorage.getUsageForOrigin. Get the usage of APIs 2-5 in aggregate for given
     * origin.
     */
    CARAPI_(void) GetUsageForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callback);

private:
    AwQuotaManagerBridge(
        /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl);

    // TODO(boliu): This should be obtained from Java AwBrowserContext that owns this.
    static CARAPI_(Int64) NativeGetDefaultNativeAwQuotaManagerBridge();

    CARAPI_(Int32) GetNextId();

    //@CalledByNative
    CARAPI_(void) OnGetOriginsCallback(
        /* [in] */ Int32 callbackId,
        /* [in] */ ArrayOf<String>* origin,
        /* [in] */ ArrayOf<Int64>* usages,
        /* [in] */ ArrayOf<Int64>* quotas);

    //@CalledByNative
    CARAPI_(void) OnGetUsageAndQuotaForOriginCallback(
        /* [in] */ Int32 callbackId,
        /* [in] */ Boolean isQuota,
        /* [in] */ Int64 usage,
        /* [in] */ Int64 quota);

    CARAPI_(void) NativeInit(
        /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl);

    CARAPI_(void) NativeDeleteAllData(
        /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl);

    CARAPI_(void) NativeDeleteOrigin(
        /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
        /* [in] */ const String& origin);

    CARAPI_(void) NativeGetOrigins(
        /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
        /* [in] */ Int32 callbackId);

    CARAPI_(void) NativeGetUsageAndQuotaForOrigin(
        /* [in] */ Int64 nativeAwQuotaManagerBridgeImpl,
        /* [in] */ const String& origin,
        /* [in] */ Int32 callbackId,
        /* [in] */ Boolean isQuota);

private:
    // TODO(boliu): This should be owned by Java AwBrowserContext, not a singleton.
    static AutoPtr<AwQuotaManagerBridge> sInstance;

    // This is not owning. The native object is owned by the native AwBrowserContext.
    Int64 mNativeAwQuotaManagerBridgeImpl;

    // The Java callbacks are saved here. An incrementing callback id is generated for each saved
    // callback and is passed to the native side to identify callback.
    Int32 mNextId;
    SparseArray<ValueCallback<Origins>> mPendingGetOriginCallbacks;
    SparseArray<ValueCallback<Long>> mPendingGetQuotaForOriginCallbacks;
    SparseArray<ValueCallback<Long>> mPendingGetUsageForOriginCallbacks;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWQUOTAMANAGERBRIDGE_H__
