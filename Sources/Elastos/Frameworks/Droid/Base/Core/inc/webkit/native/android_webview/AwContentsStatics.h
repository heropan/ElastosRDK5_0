
#ifndef __AWCONTENTSSTATICS_H__
#define __AWCONTENTSSTATICS_H__

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;

// import java.lang.Runnable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Implementations of various static methods, and also a home for static
 * data structures that are meant to be shared between all webviews.
 */
//@JNINamespace("android_webview")
class AwContentsStatics
{
public:
    /**
     * Return the client certificate lookup table.
     */
    static CARAPI_(AutoPtr<ClientCertLookupTable>) GetClientCertLookupTable();

    /**
     * Clear client cert lookup table. Should only be called from UI thread.
     */
    static CARAPI_(void) ClearClientCertPreferences(
        /* [in] */ IRunnable* callback);

    /**
     * Set Data Reduction Proxy key for authentication.
     */
    static CARAPI_(void) SetDataReductionProxyKey(
        /* [in] */ String key);

    /*
     * Enable or disable data reduction proxy.
     */
    static CARAPI_(void) SetDataReductionProxyEnabled(
        /* [in] */ Boolean enabled);

    static CARAPI_(String) GetUnreachableWebDataUrl();

    static CARAPI_(void) SetRecordFullDocument(
        /* [in] */ Boolean recordFullDocument);

private:
    //@CalledByNative
    static CARAPI_(void) ClientCertificatesCleared(
        /* [in] */ IRunnable* callback);

    //--------------------------------------------------------------------------------------------
    //  Native methods
    //--------------------------------------------------------------------------------------------
    static CARAPI_(void) NativeClearClientCertPreferences(
        /* [in] */ IRunnable* callback);

    static CARAPI_(void) NativeSetDataReductionProxyKey(
        /* [in] */ String key);

    static CARAPI_(void) NativeSetDataReductionProxyEnabled(
        /* [in] */ Boolean enabled);

    static CARAPI_(String) NativeGetUnreachableWebDataUrl();

    static CARAPI_(void) NativeSetRecordFullDocument(
        /* [in] */ Boolean recordFullDocument);

private:
    static AutoPtr<ClientCertLookupTable> sClientCertLookupTable;

    static String sUnreachableWebDataUrl;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWCONTENTSSTATICS_H__
