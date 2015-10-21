
#include "CElastosHttpClient.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Core::StringBuffer;


namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

AutoPtr<ArrayOf<String> > CElastosHttpClient::InitTextContentTypes()
{
    AutoPtr<ArrayOf<String> > textContentTypes = ArrayOf<String>::Alloc(3);
    (*textContentTypes)[0] = String("text/");
    (*textContentTypes)[1] = String("application/xml");
    (*textContentTypes)[2] = String("application/json");

    return textContentTypes;
}

const Int32 CElastosHttpClient::SOCKET_OPERATION_TIMEOUT = 60 * 1000;
const String CElastosHttpClient::TAG("ElastosHttpClient");

AutoPtr<ArrayOf<String> > CElastosHttpClient::mTextContentTypes = InitTextContentTypes();

const CElastosHttpClient::HttpRequestInterceptor* CElastosHttpClient::mThreadCheckInterceptor = new CElastosHttpClient::HttpRequestInterceptor();

CAR_INTERFACE_IMPL(CElastosHttpClient::HttpRequestInterceptor, IHttpRequestInterceptor);

ECode CElastosHttpClient::HttpRequestInterceptor::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::HttpRequestInterceptor::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::HttpRequestInterceptor::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::HttpRequestInterceptor::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IHttpRequestInterceptor * o = IHttpRequestInterceptor::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IHttpRequestInterceptor));
    }
    return NOERROR;
}

ECode CElastosHttpClient::HttpRequestInterceptor::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IHttpRequestInterceptor);
    return NOERROR;
}

ECode CElastosHttpClient::HttpRequestInterceptor::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("HttpRequestInterceptor:(");
    sb += (Int32)THIS_PROBE(IHttpRequestInterceptor);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

ECode CElastosHttpClient::HttpRequestInterceptor::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    // Prevent the HttpRequest from being sent on the main thread
    AutoPtr<ILooperHelper> helper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&helper);
    AutoPtr<ILooper> l;
    helper->MyLooper((ILooper**)&l);
    AutoPtr<ILooper> ml;
    helper->GetMainLooper((ILooper**)&ml);
    if (l != NULL && l == ml ) {
        // throw new RuntimeException("This thread forbids HTTP requests");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CElastosHttpClient::LocalDefaultHttpClient::CreateHttpProcessor(
    /* [out] */ IBasicHttpProcessor** processor)
{
    // TODO:
//     // Add interceptor to prevent making requests from main thread.
 //    AutoPtr<IBasicHttpProcessor> localProcessor;
 //    DefaultHttpClient::CreateHttpProcessor((IBasicHttpProcessor)&localProcessor);
 //    localProcessor->AddRequestInterceptor(mThreadCheckInterceptor);
 //    AutoPtr<ICurlLogger> logger;
 //    CCurlLogger::New((ICurlLogger**)&logger);
 //    localProcessor->AddRequestInterceptor(logger);

 //    *processor = localProcessor;
 //    return NOERROR;
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::LocalDefaultHttpClient::CreateHttpContext(
    /* [out] */ IHttpContext** httpContext)
{
    // TODO:
//     // Same as DefaultHttpClient.createHttpContext() minus the
 //    // cookie store.
 //    AutoPtr<IHttpContext> context;
 //    CBasicHttpContext::New((IHttpContext)&context);
 //    context->SetAttribute(
 //            IClientContext::AUTHSCHEME_REGISTRY,
 //            getAuthSchemes());
 //    context->SetAttribute(
 //            IClientContext::COOKIESPEC_REGISTRY,
 //            getCookieSpecs());
 //    context->SetAttribute(
 //            IClientContext::CREDS_PROVIDER,
 //            getCredentialsProvider());

 //    *httpContext = context;
 //    return NOERROR;
    return E_NOT_IMPLEMENTED;
}

CElastosHttpClient::LoggingConfiguration::LoggingConfiguration(
    /* [in] */ const String& tag,
    /* [in] */ Int32 level)
    : mTag(tag)
    , mLevel(level)
{}

/**
 * Returns true if logging is turned on for this configuration.
 */
Boolean CElastosHttpClient::LoggingConfiguration::IsLoggable()
{
    return Logger::IsLoggable(mTag, mLevel);
}

ECode CElastosHttpClient::LoggingConfiguration::Println(
    /* [in] */ const String& message) {
    return Logger::Println(mLevel, mTag, message);
}

CAR_INTERFACE_IMPL(CElastosHttpClient::CurlLogger, IHttpRequestInterceptor);

CElastosHttpClient::CurlLogger::CurlLogger(
    /* [in] */ LoggingConfiguration* logCongfig)
    : mLogConfig(logCongfig)
{}

ECode CElastosHttpClient::CurlLogger::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::CurlLogger::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::CurlLogger::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::CurlLogger::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    if (mLogConfig != NULL && mLogConfig->IsLoggable()) {
        AutoPtr<IHttpUriRequest> req = (IHttpUriRequest*)request->Probe(Org::Apache::Http::Client::Methods::EIID_IHttpUriRequest);
        if (req != NULL) {
            // Never print auth token -- we used to check ro.secure=0 to
            // enable that, but can't do that in unbundled code.
            String curl;
            ToCurl(req.Get(), FALSE, &curl);
            mLogConfig->Println(curl);
        }

    }

    return NOERROR;
}

CElastosHttpClient::CElastosHttpClient()
    : mCurlConfiguration(NULL)
{}

CElastosHttpClient::constructor(
    /* [in] */  IClientConnectionManager* ccm,
    /* [in] */  IHttpParams* params)
{
    // TODO:
    // mDelegate = new LocalDefaultHttpClient(ccm, params);
}

CElastosHttpClient::~CElastosHttpClient()
{}

ECode CElastosHttpClient::Close()
{
    AutoPtr<IClientConnectionManager> manager;
    GetConnectionManager((IClientConnectionManager**)&manager);
    // TODO:
    // return manager->Shutdown();
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::EnableCurlLogging(
    /* [in] */ const String& name,
    /* [in] */ Int32 level)
{
    if (name.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (level < Logger::VERBOSE || level > Logger::ASSERT) {
        // Logger::W(TAG, StringBuffer("Level is out of range [")
        //     + Logger::VERBOSE + ".." + Logger::ASSERT + "]");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mCurlConfiguration != NULL)
    {
        delete mCurlConfiguration;
        mCurlConfiguration = NULL;
    }

    mCurlConfiguration = new LoggingConfiguration(name, level);
    return NOERROR;
}

ECode CElastosHttpClient::DisableCurlLogging()
{
    if (mCurlConfiguration != NULL)
    {
        delete mCurlConfiguration;
        mCurlConfiguration = NULL;
    }

    return NOERROR;
}

 ECode CElastosHttpClient::GetParams(
    /* [out] */ IHttpParams** params)
{
    VALIDATE_NOT_NULL(params);
    // TODO:
    // return mDelegate->GetParams(params);
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::GetConnectionManager(
        /* [out] */ IClientConnectionManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    // TODO:
    // return mDelegate->GetConnectionManager(manager);
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::Execute(
    /* [in] */ IHttpUriRequest* request,
    /* [out] */ IHttpResponse** response) {
    // TODO:
    // return mDelegate->Execute(request, response);
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::Execute(
    /* [in] */ IHttpUriRequest* request,
    /* [in] */ IHttpContext* context,
    /* [out] */ IHttpResponse** response) {
    // TODO:
    // return mDelegate->Execute(request, context, response);
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::Execute(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [out] */ IHttpResponse** response) {
    // TODO:
    // return mDelegate->Execute(target, request, response);
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClient::Execute(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context,
    /* [out] */ IHttpResponse** response) {
    // TODO:
    // return mDelegate->Execute(target, request, context, response);
    return E_NOT_IMPLEMENTED;
}

// ECode CAndroidHttpClient::Execute(
//     /* [in] */ IHttpUriRequest* request,
//     /* [in] */ IResponseHandler<? extends T>* responseHandler
//     /* [out] */ <T> T** response) {
//     // TODO:
//     // return mDelegate->Execute(request, responseHandler, response);
//     return E_NOT_IMPLEMENTED;
// }

// ECode CAndroidHttpClient::Execute(
//     /* [in] */ IHttpUriRequest* request,
//     /* [in] */ IResponseHandler<? extends T>* responseHandler,
//     /* [in] */ IHttpContext* context,
//     /* [out] */ <T> T** response) {
//     // TODO:
//     // return mDelegate->Execute(request, responseHandler, context, response);
//     return E_NOT_IMPLEMENTED;
// }

// ECode CAndroidHttpClient::Execute(
//     /* [in] */ IHttpHost* target,
//     /* [in] */ IHttpRequest* request,
//     /* [in] */ IResponseHandler<? extends T>* responseHandler,
//     /* [out] */ <T> T** response) {
//     // TODO:
//     // return mDelegate->Execute(target, request, responseHandler, response);
//     return E_NOT_IMPLEMENTED;
// }

// ECode CAndroidHttpClient::Execute(
//     /* [in] */ IHttpHost* target,
//     /* [in] */ IHttpRequest* request,
//     /* [in] */ IResponseHandler<? extends T>* responseHandler,
//     /* [in] */ IHttpContext* context,
//     /* [out] */ <T> T** response) {
//     // TODO:
//     // return mDelegate->Execute(target, request, responseHandler, context, response);
//     return E_NOT_IMPLEMENTED;
// }

ECode CElastosHttpClient::ToCurl(
        /* [in] */ IHttpUriRequest* request,
        /* [in] */ Boolean logAuthToken,
        /* [out] */ String* curl)
{
    AutoPtr<StringBuilder> builder = new StringBuilder();

    // builder->Append("curl ");

    // AutoPtr<ArrayOf<IHeader*> > headers;
    // request->GetAllHeaders(&headers);
    // for (Int32 i = 0; i < headers->GetLength(); i++) {
    //     AutoPtr<IHeader> header = (*headers)[i];
    //     String name;
    //     header->getName(&name);
    //     if (!logAuthToken
    //             && (name.Equals("Authorization") ||
    //                 name.Equals("Cookie"))) {
    //         continue;
    //     }
    //     builder->Append("--header \"");
    //     String str;
    //     header->ToString(&str);
    //     builder->Append(str);
    //     builder->Append("\" ");
    // }

    // AutoPtr<IURI> uri;
    // request->GetURI(&uri);

    // // If this is a wrapped request, use the URI from the original
    // // request instead. getURI() on the wrapper seems to return a
    // // relative URI. We want an absolute URI.
    // AutoPtr<IRequestWrapper> reqWrapper = request->Probe(EIID_IRequestWrapper);
    // if (reqWrapper != NULL) {
    //     AutoPtr<IHttpRequest> original;
    //     reqWrapper->GetOriginal((IHttpRequest**)&original);
    //     AutoPtr<IHttpRequest> hRequest = original->Probe(EIID_IHttpUriRequest);
    //     if (hRequest != NULL) {
    //         hRequest->GetURI((IURI**)&uri);
    //     }
    // }

    // builder->Append("\"");
    // builder->Append(uri);
    // builder->Append("\"");

    // AutoPtr<IHttpEntityEnclosingRequest> entityRequest = request->Probe(EIID_IHttpEntityEnclosingRequest);
    // if (entityRequest != NULL) {
    //     AutoPtr<IHttpEntity> entity;
    //     entityRequest->GetEntity((IHttpEntity**)&entityRequest);
    //     if (entity != NULL && entity->IsRepeatable()) {
    //         if (entity->GetContentLength() < 1024) {
    //             AutoPtr<IByteArrayOutputStream> stream;
    //             CByteArrayOutputStream((IByteArrayOutputStream**)&stream);
    //             entity->WriteTo(stream);

    //             if (IsBinaryContent(request)) {
    //                 String base64 = Base64.encodeToString(stream.toByteArray(), Base64.NO_WRAP);
    //                 builder.insert(0, "echo '" + base64 + "' | base64 -d > /tmp/$$.bin; ");
    //                 builder.append(" --data-binary @/tmp/$$.bin");
    //             } else {
    //                 String entityString = stream.toString();
    //                 builder.append(" --data-ascii \"")
    //                         .append(entityString)
    //                         .append("\"");
    //             }
    //         } else {
    //             builder.append(" [TOO MUCH DATA TO INCLUDE]");
    //         }
    //     }
    // }

    *curl = builder->ToString();
    return NOERROR;
}

Boolean CElastosHttpClient::IsBinaryContent(
    /* [in] */ IHttpUriRequest* request)
{
    AutoPtr<ArrayOf<IHeader*> > headers;
    // TODO:
    // request->GetHeaders(IHeaders::CONTENT_ENCODING, &headers);
    if (headers != NULL) {
        for (Int32 i = 0; i < headers->GetLength(); i++) {
            AutoPtr<IHeader> header = (*headers)[i];
            String sHeader;
            // header->GetValue(&sHeader);
            if (sHeader.EqualsIgnoreCase("gzip")) {
                return TRUE;
            }
        }
    }

    headers = NULL;

    // request->GetHeaders(IHeaders::CONTENT_TYPE, &headers);
    if (headers != NULL) {
        for (Int32 i = 0; i < headers->GetLength(); i++) {
            AutoPtr<IHeader> header = (*headers)[i];
            for (Int32 j = 0; j < mTextContentTypes->GetLength(); j++) {
                String sHeader;
                // header->GetValue(&sHeader);
                String contentType = (*mTextContentTypes)[j];
                if (sHeader.StartWith(contentType)) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

}
}
}
}

