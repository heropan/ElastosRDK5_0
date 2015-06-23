
#include "CAndroidHttpClientHelper.h"
#include "CHttpDateTime.h"
#include "ext/frameworkext.h"

using namespace Elastos::IO;
using namespace Org::Apache::Http;

using Elastos::Droid::Net::Internal::Http::IHttpDateTime;
using Elastos::Droid::Net::Internal::Http::CHttpDateTime;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

Int64 CAndroidHttpClientHelper::DEFAULT_SYNC_MIN_GZIP_BYTES = 256;

ECode CAndroidHttpClientHelper::NewInstance(
    /* [in] */ const String& userAgent,
    /* [in] */ Elastos::Droid::Content::IContext* context,
    /* [out] */ Elastos::Droid::Net::Http::IAndroidHttpClient** client)
{
    VALIDATE_NOT_NULL(client);

    // AutoPtr<IHttpParams> params;
    // CBasicHttpParams::New((IHttpParams**)&params);

    // // Turn off stale checking.  Our connections break all the time anyway,
    // // and it's not worth it to pay the penalty of checking every time.
    // AutoPtr<IHttpConnectionParams> conParamsHelper;
    // CHttpConnectionParams::AcquireSingleton((IHttpConnectionParams**)&conParamsHelper);
    // conParamsHelper->SetStaleCheckingEnabled(params, FALSE);

    // conParamsHelper->SetConnectionTimeout(params, SOCKET_OPERATION_TIMEOUT);
    // conParamsHelper->SetSoTimeout(params, SOCKET_OPERATION_TIMEOUT);
    // conParamsHelper->SetSocketBufferSize(params, 8192);

    // // Don't handle redirects -- return them to the caller.  Our code
    // // often wants to re-POST after a redirect, which we must do ourselves.
    // conParamsHelper->SetRedirecting(params, FALSE);

    // // Use a session cache for SSL sockets
    // AutoPtr<ISSLSessionCache> sessionCache;
    // if (context != NULL) {
    //     CSSLSessionCache::New(context, (ISSLSessionCache**)&sessionCache);
    // }

    // // Set the specified user agent and register standard protocols.
    // AutoPtr<IHttpProtocolParams> proParamsHelper;
    // CHttpProtocolParams::AcquireSingleton((IHttpConnectionParams**)&proParamsHelper);
    // proParamsHelper->SetUserAgent(params, userAgent);
    // AutoPtr<ISchemeRegistry> schemeRegistry;
    // CSchemeRegistry::New((ISchemeRegistry**)&schemeRegistry);

    // AutoPtr<IPlainSocketFactoryHelper> psFactoryHelper;
    // CPlainSocketFactory::AcquireSingleton((IPlainSocketFactory**)&psFactoryHelper);
    // AutoPtr<IPlainSocketFactory> psFactory;
    // psFactoryHelper->GetSocketFactory((IPlainSocketFactory**)&psFactory);
    // AutoPtr<IScheme> httpScheme;
    // CScheme::New(String("http"), psFactory, 80, (IScheme**)&httpScheme);

    // schemeRegistry->Register(httpScheme);
    // AutoPtr<ISSLCertificateSocketFactory> sslCeFactoryHelper;
    // CSSLCertificateSocketFactory::AcquireSingleton((IPlainSocketFactory**)&sslCeFactoryHelper);
    // AutoPtr<IPlainSocketFactory> sslCeFactory;
    // sslCeFactoryHelper->GetHttpSocketFactory(SOCKET_OPERATION_TIMEOUT, sessionCache, (IPlainSocketFactory**)&sslCeFactory);
    // AutoPtr<IScheme> httpsScheme;
    // CScheme::New(String("https"), sslCeFactory, 443, (IScheme**)&httpScheme);
    // schemeRegistry->Register(httpsScheme);

    // AutoPtr<IClientConnectionManager> manager;
    // CThreadSafeClientConnManager::New(params, schemeRegistry, (IClientConnectionManager**)&manager);

    // // We use a factory method to modify superclass initialization
    // // parameters without the funny call-a-static-method dance.
    // CAndroidHttpClient* cClient =  new CAndroidHttpClient(manager, params);
    // if (cClient != NULL) {
    //     *client = (IAndroidHttpClient*)cClient;
    //     return NOERROR;
    // }

    return E_RUNTIME_EXCEPTION;
}

ECode CAndroidHttpClientHelper::NewInstance(
    /* [in] */ const String& userAgent,
    /* [out] */ Elastos::Droid::Net::Http::IAndroidHttpClient** client)
{
    return NewInstance(userAgent, NULL, client);
}

ECode CAndroidHttpClientHelper::ModifyRequestToAcceptGzipResponse(
    /* [in, out] */ Org::Apache::Http::IHttpRequest* request)
{
    VALIDATE_NOT_NULL(request);
    // return request->AddHeader(String("Accept-Encoding"), String("gzip");
    return E_NOT_IMPLEMENTED;
}

ECode CAndroidHttpClientHelper::GetUngzippedContent(
    /* [in] */ Org::Apache::Http::IHttpEntity* entity,
    /* [out] */ Elastos::IO::IInputStream** stream)
{
    AutoPtr<IInputStream> responseStream;
    // TODO:
    // entity->GetContent((IInputStream**)&responseStream);
    if (responseStream == NULL) {
        *stream = NULL;
        return NOERROR;
    }
    AutoPtr<IHeader> header;
    // entity->GetContentEncoding((IHeader**)&header);
    if (header == NULL) {
        *stream = responseStream;
        REFCOUNT_ADD(*stream);
        return NOERROR;
    }
    String contentEncoding;
    // header->GetValue(&contentEncoding);
    if (contentEncoding.IsNullOrEmpty()) {
        *stream = responseStream;
        REFCOUNT_ADD(*stream);
        return NOERROR;
    }
    AutoPtr<IInputStream> gzipResponseStream;
    if (contentEncoding.Contains("gzip")) {
        CGZIPInputStream::New(responseStream, (IInputStream**)&gzipResponseStream);
    }
    *stream = gzipResponseStream;
    REFCOUNT_ADD(*stream);
    return NOERROR;
}

ECode CAndroidHttpClientHelper::GetCompressedEntity(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Elastos::Droid::Content::IContentResolver* resolver,
    /* [out] */ Org::Apache::Http::Entity::IAbstractHttpEntity** entity)
{
    AutoPtr<IAbstractHttpEntity> localEntity;
    Int64 size;
    GetMinGzipSize(resolver, &size);
    if (data->GetLength() < size) {
        // TODO:
        // CByteArrayEntity::New(data, (IAbstractHttpEntity**)&localEntity);
    } else {
        AutoPtr<IByteArrayOutputStream> arr;
        CByteArrayOutputStream::New((IByteArrayOutputStream**)&arr);
        AutoPtr<IOutputStream> zipper;
        CGZIPOutputStream::New(arr, (IOutputStream**)&zipper);
        zipper->WriteBytes(*data);
        zipper->Close();
        // CByteArrayEntity::New(arr, (IAbstractHttpEntity**)&localEntity);
        // localEntity->SetContentEncoding(String("gzip"));
    }
    *entity = localEntity;
    return NOERROR;
}

ECode CAndroidHttpClientHelper::GetMinGzipSize(
    /* [in] */ Elastos::Droid::Content::IContentResolver* resolver,
    /* [out] */ Int64* size)
{
    // For now, this is just a constant.
    *size = DEFAULT_SYNC_MIN_GZIP_BYTES;
    return NOERROR;
}

ECode CAndroidHttpClientHelper::ParseDate(
    /* [in] */ const String& dateString,
    /* [out] */ Int64* date)
{
    VALIDATE_NOT_NULL(date);

    AutoPtr<IHttpDateTime> dTime;
    CHttpDateTime::AcquireSingleton((IHttpDateTime**)&dTime);
    return dTime->Parse(dateString, date);
}

}
}
}
}

