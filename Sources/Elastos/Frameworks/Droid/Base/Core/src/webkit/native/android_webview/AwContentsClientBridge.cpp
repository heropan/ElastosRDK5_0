
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//========================================================================================
//       AwContentsClientBridge::ClientCertificateRequestCallback::ProceedRunnable
//========================================================================================

AwContentsClientBridge::ClientCertificateRequestCallback::ProceedRunnable::ProceedRunnable(
    /* [in] */ ClientCertificateRequestCallback* owner,
    /* [in] */ const IPrivateKey* privateKey,
    /* [in] */ const ArrayOf<IX509Certificate>* chain)
    : mOwner(owner)
{
}

ECode AwContentsClientBridge::ClientCertificateRequestCallback::ProceedRunnable::Run()
{
    mOwner->ProceedOnUiThread(mPrivateKey, mChain);
    return NOERROR;
}

//========================================================================================
//       AwContentsClientBridge::ClientCertificateRequestCallback::IgnoreRunnable
//========================================================================================

AwContentsClientBridge::ClientCertificateRequestCallback::IgnoreRunnable::IgnoreRunnable(
    /* [in] */ ClientCertificateRequestCallback* owner)
    : mOwner(owner)
{
}

ECode AwContentsClientBridge::ClientCertificateRequestCallback::IgnoreRunnableRun()
{
    mOwner->IgnoreOnUiThread();
    return NOERROR;
}

//========================================================================================
//       AwContentsClientBridge::ClientCertificateRequestCallback::CancelRunnable
//========================================================================================

AwContentsClientBridge::ClientCertificateRequestCallback::CancelRunnable::CancelRunnable(
    /* [in] */ ClientCertificateRequestCallback* owner)
    : mOwner(owner)
{
}

ECode AwContentsClientBridge::ClientCertificateRequestCallback::CancelRunnable::Run()
{
    mOwner->CancelOnUiThread();
    return NOERROR;
}

//==============================================================================
//          AwContentsClientBridge::ClientCertificateRequestCallback
//==============================================================================

AwContentsClientBridge::ClientCertificateRequestCallback::ClientCertificateRequestCallback(
    /* [in] */ AwContentsClientBridge* owner,
    /* [in] */ Int32 id,
    /* [in] */ String host,
    /* [in] */ Int32 port)
    : mOwner(owner)
    , mId(id)
    , mHost(host)
    , mPort(port)
    , mIsCalled(FALSE)
{
}

void AwContentsClientBridge::ClientCertificateRequestCallback::Proceed(
    /* [in] */ const IPrivateKey* privateKey,
    /* [in] */ const ArrayOf<IX509Certificate>* chain)
{
    AutoPtr<IRunnable> runnable = new ProceedRunnable(this, privateKey, chain);
    ThreadUtils::RunOnUiThread(runnable);
}

void AwContentsClientBridge::ClientCertificateRequestCallback::Ignore()
{
    AutoPtr<IRunnable> runnable = new IgnoreRunnableRun(this);
    ThreadUtils::RunOnUiThread(runnable);
}

void AwContentsClientBridge::ClientCertificateRequestCallback::Cancel()
{
    AutoPtr<IRunnable> runnable = new CancelRunnable(this);
    ThreadUtils::RunOnUiThread(runnable);
}

void AwContentsClientBridge::ClientCertificateRequestCallback::ProceedOnUiThread(
    /* [in] */ IPrivateKey* privateKey,
    /* [in] */ ArrayOf<X509Certificate>* chain)
{
    CheckIfCalled();

    AutoPtr<AndroidPrivateKey> key = mLocalKeyStore->CreateKey(privateKey);

    if (key == NULL || chain == NULL || chain->GetLength() == 0) {
//        Log.w(TAG, "Empty client certificate chain?");
        ProvideResponse(NULL, NULL);
        return;
    }
    // Encode the certificate chain.
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > > encodedChain = ArrayOf< AutoPtr< ArrayOf<Byte> > >::Alloc(chain->GetLength());
    //try {
        for (Int32 i = 0; i < chain->GetLength(); ++i) {
            (*encodedChain)[i] = (*chain)[i]->GetEncoded();
        }
    //} catch (CertificateEncodingException e) {
    //    Log.w(TAG, "Could not retrieve encoded certificate chain: " + e);
    //    provideResponse(null, null);
    //    return;
    //}
    mLookupTable->Allow(mHost, mPort, key, encodedChain);
    ProvideResponse(key, encodedChain);
}

void AwContentsClientBridge::ClientCertificateRequestCallback::IgnoreOnUiThread()
{
    CheckIfCalled();
    ProvideResponse(NULL, NULL);
}

void AwContentsClientBridge::ClientCertificateRequestCallback::CancelOnUiThread()
{
    CheckIfCalled();
    mLookupTable->Deny(mHost, mPort);
    ProvideResponse(NULL, NULL);
}

void AwContentsClientBridge::ClientCertificateRequestCallback::CheckIfCalled()
{
    if (mIsCalled) {
        throw new IllegalStateException("The callback was already called.");
    }
    mIsCalled = TRUE;
}

void AwContentsClientBridge::ClientCertificateRequestCallback::ProvideResponse(
    /* [in] */ AndroidPrivateKey* androidKey,
    /* [in] */ ArrayOf< AutoPtr<ArrayOf<byte> > >* certChain)
{
    if (mNativeContentsClientBridge == 0) {
        return;
    }

    NativeProvideClientCertificateResponse(mNativeContentsClientBridge, mId,
            certChain, androidKey);
}

//=========================================================================
//               AwContentsClientBridge::InnerValueCallback
//=========================================================================

AwContentsClientBridge::InnerValueCallback::InnerValueCallback(
    /* [in] */ AwContentsClientBridge* owner,
    /* [in] */ Int32 id)
    : mOwner(owner)
    , mId(id)
{
}

ECode AwContentsClientBridge::InnerValueCallback::OnReceiveValue(
    /* [in] */ IInterface* value)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, value, mId);
    ThreadUtils::RunOnUiThread(runnable);
    return NOERROR;
}

//=========================================================================
//         AwContentsClientBridge::InnerValueCallback::InnerRunnable
//=========================================================================

AwContentsClientBridge::InnerValueCallback::InnerRunnable::InnerRunnable(
    /* [in] */ InnerValueCallback* owner,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 id)
    : mOwner(owner)
    , mValue(value)
    , mId(id)
{
}

ECode AwContentsClientBridge::InnerValueCallback::InnerRunnable::Run()
{
    AutoPtr<IBoolean> iValue = mValue->Probe(EIID_IBoolean);
    Boolean value;
    iValue->GetValue(&value);
    mOwner->ProceedSslError(value, mId);
    return NOERROR;
}

//===============================================================
//                    AwContentsClientBridge
//===============================================================

const String AwContentsClientBridge::TAG("AwContentsClientBridge");


// Used for mocking this class in tests.
AwContentsClientBridge::AwContentsClientBridge(
    /* [in] */ DefaultAndroidKeyStore* keyStore,
    /* [in] */ ClientCertLookupTable* table)
    : mLocalKeyStore(keyStore)
    , mLookupTable(table)
    , mNativeContentsClientBridge(0)
{
}

AwContentsClientBridge::AwContentsClientBridge(
    /* [in] */ AwContentsClient* client,
    /* [in] */ DefaultAndroidKeyStore* keyStore,
    /* [in] */ ClientCertLookupTable table)
    : mNativeContentsClientBridge(0)
{
    assert(client != NULL);
    mClient = client;
    mLocalKeyStore = keyStore;
    mLookupTable = table;
}

// Used by the native peer to set/reset a weak ref to the native peer.
//@CalledByNative
void AwContentsClientBridge::SetNativeContentsClientBridge(
    /* [in] */ Int64 nativeContentsClientBridge)
{
    mNativeContentsClientBridge = nativeContentsClientBridge;
}

// If returns false, the request is immediately canceled, and any call to proceedSslError
// has no effect. If returns true, the request should be canceled or proceeded using
// proceedSslError().
// Unlike the webview classic, we do not keep keep a database of certificates that
// are allowed by the user, because this functionality is already handled via
// ssl_policy in native layers.
//@CalledByNative
Boolean AwContentsClientBridge::AllowCertificateError(
    /* [in] */ Int32 certError,
    /* [in] */ ArrayOf<Byte>* derBytes,
    /* [in] */ const String& url,
    /* [in] */ const Int32 id)
{
    const AutoPtr<ISslCertificate> cert = SslUtil::GetCertificateFromDerBytes(derBytes);
    if (cert == NULL) {
        // if the certificate or the client is null, cancel the request
        return FALSE;
    }
    const AutoPtr<ISslError> sslError = SslUtil::SslErrorFromNetErrorCode(certError, cert, url);
    AutoPtr<IValueCallback> callback = new InnerValueCallback(this, id);
    mClient->OnReceivedSslError(callback, sslError);
    return TRUE;
}

void AwContentsClientBridge::ProceedSslError(
    /* [in] */ Boolean proceed,
    /* [in] */ Int32 id)
{
    if (mNativeContentsClientBridge == 0) return;
    NativeProceedSslError(mNativeContentsClientBridge, proceed, id);
}

// Intentionally not private for testing the native peer of this class.
//@CalledByNative
void AwContentsClientBridge::SelectClientCertificate(
    /* [in] */ Int32 id,
    /* [in] */ const ArrayOf<String>* keyTypes,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* encodedPrincipals,
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    assert(mNativeContentsClientBridge != 0);
    AutoPtr<ClientCertLookupTable::Cert> cert = mLookupTable->GetCertData(host, port);
    if (mLookupTable->IsDenied(host, port)) {
        NativeProvideClientCertificateResponse(mNativeContentsClientBridge, id,
                NULL, NULL);
        return;
    }
    if (cert != NULL) {
        NativeProvideClientCertificateResponse(mNativeContentsClientBridge, id,
                cert.certChain, cert.privateKey);
        return;
    }
    // Build the list of principals from encoded versions.
    AutoPtr< ArrayOf<IPrincipal> > principals;
    if (encodedPrincipals->GetLength() > 0) {
        principals = ArrayOf<IPrincipal>::Alloc(encodedPrincipals->GetLength());
        for (Int32 n = 0; n < encodedPrincipals.length; n++) {
            //try {
                principals[n] = new X500Principal(encodedPrincipals[n]);
            //} catch (IllegalArgumentException e) {
            //    Log.w(TAG, "Exception while decoding issuers list: " + e);
            //    nativeProvideClientCertificateResponse(mNativeContentsClientBridge, id,
            //        null, null);
            //    return;
            //}
        }

    }

    const AutoPtr<ClientCertificateRequestCallback> callback =
            new ClientCertificateRequestCallback(id, host, port);
    mClient->OnReceivedClientCertRequest(callback, keyTypes, principals, host, port);
}

//@CalledByNative
void AwContentsClientBridge::handleJsAlert(
    /* [in] */ String url,
    /* [in] */ String message,
    /* [in] */ Int32 id)
{
    AutoPtr<JsResultHandler> handler = new JsResultHandler(this, id);
    mClient->HandleJsAlert(url, message, handler);
}

//@CalledByNative
void AwContentsClientBridge::HandleJsConfirm(
    /* [in] */ String url,
    /* [in] */ String message,
    /* [in] */ Int32 id)
{
    AutoPtr<JsResultHandler> handler = new JsResultHandler(this, id);
    mClient->HandleJsConfirm(url, message, handler);
}

//@CalledByNative
void AwContentsClientBridge::HandleJsPrompt(
    /* [in] */ String url,
    /* [in] */ String message,
    /* [in] */ String defaultValue,
    /* [in] */ Int32 id)
{
    AutoPtr<JsResultHandler> handler = new JsResultHandler(this, id);
    mClient->HandleJsPrompt(url, message, defaultValue, handler);
}

//@CalledByNative
void AwContentsClientBridge::HandleJsBeforeUnload(
    /* [in] */ String url,
    /* [in] */ String message,
    /* [in] */ Int32 id)
{
    AutoPtr<JsResultHandler> handler = new JsResultHandler(this, id);
    mClient->HandleJsBeforeUnload(url, message, handler);
}

//@CalledByNative
Boolean AwContentsClientBridge::ShouldOverrideUrlLoading(
    /* [in] */ String url)
{
    return mClient->ShouldOverrideUrlLoading(url);
}

void AwContentsClientBridge::ConfirmJsResult(
    /* [in] */ Int32 id,
    /* [in] */ String prompt)
{
    if (mNativeContentsClientBridge == 0) return;
    NativeConfirmJsResult(mNativeContentsClientBridge, id, prompt);
}

void AwContentsClientBridge::CancelJsResult(
    /* [in] */ Int32 id)
{
    if (mNativeContentsClientBridge == 0) return;
    NativeCancelJsResult(mNativeContentsClientBridge, id);
}

//--------------------------------------------------------------------------------------------
//  Native methods
//--------------------------------------------------------------------------------------------
void AwContentsClientBridge::NativeProceedSslError(
    /* [in] */ Int64 nativeAwContentsClientBridge,
    /* [in] */ Boolean proceed,
    /* [in] */ Int32 id)
{
}

void AwContentsClientBridge::NativeProvideClientCertificateResponse(
    /* [in] */ Int64 nativeAwContentsClientBridge,
    /* [in] */ Int32 id,
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain,
    /* [in] */ AndroidPrivateKey* androidKey)
{
}

void AwContentsClientBridge::NativeConfirmJsResult(
    /* [in] */ Int64 nativeAwContentsClientBridge,
    /* [in] */ Int32 id,
    /* [in] */ String prompt)
{
}

void AwContentsClientBridge::NativeCancelJsResult(
    /* [in] */ Int64 nativeAwContentsClientBridge,
    /* [in] */ Int32 id)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos