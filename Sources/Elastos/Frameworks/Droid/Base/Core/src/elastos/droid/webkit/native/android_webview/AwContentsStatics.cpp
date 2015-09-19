
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

static AutoPtr<ClientCertLookupTable> AwContentsStatics::sClientCertLookupTable;
static String AwContentsStatics::sUnreachableWebDataUrl;

/**
 * Return the client certificate lookup table.
 */
AutoPtr<ClientCertLookupTable> AwContentsStatics::GetClientCertLookupTable()
{
    ThreadUtils::AssertOnUiThread();
    if (sClientCertLookupTable == NULL) {
        sClientCertLookupTable = new ClientCertLookupTable();
    }
    return sClientCertLookupTable;
}

/**
 * Clear client cert lookup table. Should only be called from UI thread.
 */
void AwContentsStatics::ClearClientCertPreferences(
    /* [in] */ IRunnable* callback)
{
    ThreadUtils::AssertOnUiThread();
    GetClientCertLookupTable()->Clear();
    NativeClearClientCertPreferences(callback);
}

//@CalledByNative
void AwContentsStatics::clientCertificatesCleared(
    /* [in] */ IRunnable* callback)
{
    if (callback == NULL) return;
    callback->Run();
}

/**
 * Set Data Reduction Proxy key for authentication.
 */
void AwContentsStatics::SetDataReductionProxyKey(
    /* [in] */ String key)
{
    ThreadUtils::AssertOnUiThread();
    NativeSetDataReductionProxyKey(key);
}

/*
 * Enable or disable data reduction proxy.
 */
void AwContentsStatics::SetDataReductionProxyEnabled(
    /* [in] */ Boolean enabled)
{
    ThreadUtils::AssertOnUiThread();
    NativeSetDataReductionProxyEnabled(enabled);
}

String AwContentsStatics::GetUnreachableWebDataUrl()
{
    if (sUnreachableWebDataUrl == NULL) {
        sUnreachableWebDataUrl = NativeGetUnreachableWebDataUrl();
    }
    return sUnreachableWebDataUrl;
}

void AwContentsStatics::setRecordFullDocument(
    /* [in] */ Boolean recordFullDocument)
{
    NativeSetRecordFullDocument(recordFullDocument);
}

//--------------------------------------------------------------------------------------------
//  Native methods
//--------------------------------------------------------------------------------------------
void AwContentsStatics::NativeClearClientCertPreferences(
    /* [in] */ IRunnable* callback)
{
}

void AwContentsStatics::NativeSetDataReductionProxyKey(
    /* [in] */ String key)
{
}

void AwContentsStatics::NativeSetDataReductionProxyEnabled(
    /* [in] */ Boolean enabled)
{
}

String AwContentsStatics::NativeGetUnreachableWebDataUrl()
{
}

void AwContentsStatics::NativeSetRecordFullDocument(
    /* [in] */ Boolean recordFullDocument)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos