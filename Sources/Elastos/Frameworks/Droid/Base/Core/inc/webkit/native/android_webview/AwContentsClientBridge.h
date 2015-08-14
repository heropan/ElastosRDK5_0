
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENTBRIDGE_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENTBRIDGE_H__

// import android.net.http.SslCertificate;
// import android.net.http.SslError;
// import android.util.Log;
// import android.webkit.ValueCallback;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;
// import org.chromium.net.AndroidPrivateKey;
// import org.chromium.net.DefaultAndroidKeyStore;

// import java.security.Principal;
// import java.security.PrivateKey;
// import java.security.cert.CertificateEncodingException;
// import java.security.cert.X509Certificate;

// import javax.security.auth.x500.X500Principal;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * This class handles the JNI communication logic for the the AwContentsClient class.
 * Both the Java and the native peers of AwContentsClientBridge are owned by the
 * corresponding AwContents instances. This class and its native peer are connected
 * via weak references. The native AwContentsClientBridge sets up and clear these weak
 * references.
 */
//@JNINamespace("android_webview")
class AwContentsClientBridge
{
public:
    /**
     * Callback to communicate clientcertificaterequest back to the AwContentsClientBridge.
     * The public methods should be called on UI thread.
     * A request can not be proceeded, ignored  or canceled more than once. Doing this
     * is a programming error and causes an exception.
     */
    class ClientCertificateRequestCallback
    {
    private:
        class ProceedRunnable
            : public Object
            , public IRunnable
        {
        public:
            ProceedRunnable(
                /* [in] */ ClientCertificateRequestCallback* owner,
                /* [in] */ const IPrivateKey* privateKey,
                /* [in] */ const ArrayOf<IX509Certificate>* chain);

            CARAPI Run();

        private:
            ClientCertificateRequestCallback* mOwner;
            const IPrivateKey* mPrivateKey,
            const ArrayOf<IX509Certificate>* mChain;
        };

        class IgnoreRunnable
            : public Object
            , public IRunnable
        {
        public:
            IgnoreRunnable(
                /* [in] */ ClientCertificateRequestCallback* owner);

            CARAPI Run();

        private:
            ClientCertificateRequestCallback* mOwner;
        };

        class CancelRunnable
            : public Object
            , public IRunnable
        {
        public:
            CancelRunnable(
                /* [in] */ ClientCertificateRequestCallback* owner);

            CARAPI Run();

        private:
            ClientCertificateRequestCallback* mOwner;
        };

    public:
        ClientCertificateRequestCallback(
            /* [in] */ AwContentsClientBridge* owner,
            /* [in] */ Int32 id,
            /* [in] */ String host,
            /* [in] */ Int32 port);

        virtual CARAPI_(void) Proceed(
            /* [in] */ const IPrivateKey* privateKey,
            /* [in] */ const ArrayOf<IX509Certificate>* chain);

        virtual CARAPI_(void) Ignore();

        virtual CARAPI_(void) Cancel();

    private:
        CARAPI_(void) ProceedOnUiThread(
            /* [in] */ IPrivateKey* privateKey,
            /* [in] */ ArrayOf<IX509Certificate>* chain);

        CARAPI_(void) IgnoreOnUiThread();

        CARAPI_(void) CancelOnUiThread();

        CARAPI_(void) CheckIfCalled();

        CARAPI_(void) ProvideResponse(
            /* [in] */ AndroidPrivateKey* androidKey,
            /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain);

    private:
        AwContentsClientBridge* mOwner;

        Int32 mId;
        String mHost;
        Int32 mPort;
        Boolean mIsCalled;
    };

private:
    class InnerValueCallback
        : public Object
        , public IValueCallback
    {
    private:
        class InnerRunnable
            : public Object
            , public IRunnable
        {
        public:
            InnerRunnable(
                /* [in] */ InnerValueCallback* owner,
                /* [in] */ IInterface* value,
                /* [in] */ Int32 id);

            CARAPI Run();

        private:
            InnerValueCallback* mOwner;
            IInterface* mValue;
            Int32 mId;
        };

    public:
        InnerValueCallback(
            /* [in] */ AwContentsClientBridge* owner,
            /* [in] */ Int32 id);

        CARAPI OnReceiveValue(
            /* [in] */ IInterface* value);

    private:
        AwContentsClientBridge* mOwner;
        Int32 mId;
    };

public:
    static const String TAG;

    AwContentsClientBridge(
        /* [in] */ AwContentsClient* client,
        /* [in] */ DefaultAndroidKeyStore* keyStore,
        /* [in] */ ClientCertLookupTable* table);

    virtual CARAPI_(void) ConfirmJsResult(
        /* [in] */ Int32 id,
        /* [in] */ String prompt);

    virtual CARAPI_(void) CancelJsResult(
        /* [in] */ Int32 id);

protected:
    // Used for mocking this class in tests.
    AwContentsClientBridge(
        /* [in] */ DefaultAndroidKeyStore* keyStore,
        /* [in] */ ClientCertLookupTable* table);

    // Intentionally not private for testing the native peer of this class.
    //@CalledByNative
    virtual CARAPI_(void) SelectClientCertificate(
        /* [in] */ const Int32 id,
        /* [in] */ const ArrayOf<String>* keyTypes,
        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* encodedPrincipals,
        /* [in] */ const String& host,
        /* [in] */ const Int32 port);

private:
    // Used by the native peer to set/reset a weak ref to the native peer.
    //@CalledByNative
    CARAPI_(void) SetNativeContentsClientBridge(
        /* [in] */ Int64 nativeContentsClientBridge);

    // If returns false, the request is immediately canceled, and any call to proceedSslError
    // has no effect. If returns true, the request should be canceled or proceeded using
    // proceedSslError().
    // Unlike the webview classic, we do not keep keep a database of certificates that
    // are allowed by the user, because this functionality is already handled via
    // ssl_policy in native layers.
    //@CalledByNative
    CARAPI_(Boolean) AllowCertificateError(
        /* [in] */ Int32 certError,
        /* [in] */ ArrayOf<Byte>* derBytes,
        /* [in] */ const String& url,
        /* [in] */ const Int32 id);

    CARAPI_(void) ProceedSslError(
        /* [in] */ Boolean proceed,
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleJsAlert(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleJsConfirm(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleJsPrompt(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ String defaultValue,
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(void) HandleJsBeforeUnload(
        /* [in] */ String url,
        /* [in] */ String message,
        /* [in] */ Int32 id);

    //@CalledByNative
    CARAPI_(Boolean) ShouldOverrideUrlLoading(
        /* [in] */ String url);

    //--------------------------------------------------------------------------------------------
    //  Native methods
    //--------------------------------------------------------------------------------------------
    CARAPI_(void) NativeProceedSslError(
        /* [in] */ Int64 nativeAwContentsClientBridge,
        /* [in] */ Boolean proceed,
        /* [in] */ Int32 id);

    CARAPI_(void) NativeProvideClientCertificateResponse(
        /* [in] */ Int64 nativeAwContentsClientBridge,
        /* [in] */ Int32 id,
        /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain,
        /* [in] */ AndroidPrivateKey* androidKey);

    CARAPI_(void) NativeConfirmJsResult(
        /* [in] */ Int64 nativeAwContentsClientBridge,
        /* [in] */ Int32 id,
        /* [in] */ String prompt);

    CARAPI_(void) NativeCancelJsResult(
        /* [in] */ Int64 nativeAwContentsClientBridge,
        /* [in] */ Int32 id);

private:
    AutoPtr<AwContentsClient> mClient;
    // The native peer of this object.
    Int64 mNativeContentsClientBridge;

    AutoPtr<DefaultAndroidKeyStore> mLocalKeyStore;

    AutoPtr<ClientCertLookupTable> mLookupTable;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTSCLIENTBRIDGE_H__
