#include "elastos/droid/webkit/native/android_webview/AwContentsStatics.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"

using Elastos::Droid::Webkit::Base::ThreadUtils;
using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

AutoPtr<ClientCertLookupTable> AwContentsStatics::sClientCertLookupTable;
String AwContentsStatics::sUnreachableWebDataUrl;

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
//param callback is IRunnable
void AwContentsStatics::ClientCertificatesCleared(
    /* [in] */ IInterface* callback)
{
    if (callback == NULL) return;
    AutoPtr<IRunnable> runCallback = (IRunnable*)(callback->Probe(EIID_IRunnable));
    runCallback->Run();
}

/**
 * Set Data Reduction Proxy key for authentication.
 */
void AwContentsStatics::SetDataReductionProxyKey(
    /* [in] */ const String& key)
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
    if (sUnreachableWebDataUrl.IsNullOrEmpty()) {
        sUnreachableWebDataUrl = NativeGetUnreachableWebDataUrl();
    }
    return sUnreachableWebDataUrl;
}

void AwContentsStatics::SetRecordFullDocument(
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
    /* [in] */ const String& key)
{
}

void AwContentsStatics::NativeSetDataReductionProxyEnabled(
    /* [in] */ Boolean enabled)
{
}

String AwContentsStatics::NativeGetUnreachableWebDataUrl()
{
    return String();
}

void AwContentsStatics::NativeSetRecordFullDocument(
    /* [in] */ Boolean recordFullDocument)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
