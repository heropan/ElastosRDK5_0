
#include "elastos/droid/net/http/ElastosHttpClient.h"

using Org::Apache::Http::Client::EIID_IHttpClient;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL_2(ElastosHttpClient, Object, IHttpClient, IElastosHttpClient)

Int64 ElastosHttpClient::DEFAULT_SYNC_MIN_GZIP_BYTES = 256;
AutoPtr<ArrayOf<String> > ElastosHttpClient::sTextContentTypes = InitTextContentTypes();
const Int32 ElastosHttpClient::SOCKET_OPERATION_TIMEOUT = 60 * 1000;
const String ElastosHttpClient::TAG("ElastosHttpClient");
const AutoPtr<IHttpRequestInterceptor> ElastosHttpClient::THREAD_CHECK_INTERCEPTOR = InitThreadCheckInterceptor();

ElastosHttpClient::ElastosHttpClient()
{}

ElastosHttpClient::~ElastosHttpClient()
{
#if 0 // TODO: Translate codes below
        super.finalize();
        if (mLeakedException != NULL) {
            Logger::E(TAG, "Leak found", mLeakedException);
            mLeakedException = NULL;
        }
#endif
}

AutoPtr<IHttpRequestInterceptor> ElastosHttpClient::InitThreadCheckInterceptor()
{
    AutoPtr<IHttpRequestInterceptor> rev;
#if 0 // TODO: Translate codes below
    new HttpRequestInterceptor() {
        public void process(HttpRequest request, HttpContext context) {
            // Prevent the HttpRequest from being sent on the main thread
            if (Looper.myLooper() != NULL && Looper.myLooper() == Looper.getMainLooper() ) {
                throw new RuntimeException("This thread forbids HTTP requests");
            }
        }
    };
#endif
    return rev;
}

ECode ElastosHttpClient::GetDEFAULT_SYNC_MIN_GZIP_BYTES(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = DEFAULT_SYNC_MIN_GZIP_BYTES;
    return NOERROR;
}

ECode ElastosHttpClient::SetDEFAULT_SYNC_MIN_GZIP_BYTES(
    /* [in] */ Int64 DEFAULT_SYNC_MIN_GZIP_BYTES)
{
    ElastosHttpClient::DEFAULT_SYNC_MIN_GZIP_BYTES = DEFAULT_SYNC_MIN_GZIP_BYTES;
    return NOERROR;
}

AutoPtr<ArrayOf<String> > ElastosHttpClient::InitTextContentTypes()
{
    AutoPtr<ArrayOf<String> > textContentTypes = ArrayOf<String>::Alloc(3);
    (*textContentTypes)[0] = String("text/");
    (*textContentTypes)[1] = String("application/xml");
    (*textContentTypes)[2] = String("application/json");

    return textContentTypes;
}

ECode ElastosHttpClient::NewInstance(
    /* [in] */ const String& userAgent,
    /* [in] */ IContext* context,
    /* [out] */ IElastosHttpClient** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        HttpParams params = new BasicHttpParams();
        // Turn off stale checking.  Our connections break all the time anyway,
        // and it's not worth it to pay the penalty of checking every time.
        HttpConnectionParams.setStaleCheckingEnabled(params, FALSE);
        HttpConnectionParams.setConnectionTimeout(params, SOCKET_OPERATION_TIMEOUT);
        HttpConnectionParams.setSoTimeout(params, SOCKET_OPERATION_TIMEOUT);
        HttpConnectionParams.setSocketBufferSize(params, 8192);
        // Don't handle redirects -- return them to the caller.  Our code
        // often wants to re-POST after a redirect, which we must do ourselves.
        HttpClientParams.setRedirecting(params, FALSE);
        // Use a session cache for SSL sockets
        SSLSessionCache sessionCache = context == NULL ? NULL : new SSLSessionCache(context);
        // Set the specified user agent and register standard protocols.
        HttpProtocolParams.setUserAgent(params, userAgent);
        SchemeRegistry schemeRegistry = new SchemeRegistry();
        schemeRegistry.register(new Scheme("http",
                PlainSocketFactory.getSocketFactory(), 80));
        schemeRegistry.register(new Scheme("https",
                SSLCertificateSocketFactory.getHttpSocketFactory(
                SOCKET_OPERATION_TIMEOUT, sessionCache), 443));
        ClientConnectionManager manager =
                new ThreadSafeClientConnManager(params, schemeRegistry);
        // We use a factory method to modify superclass initialization
        // parameters without the funny call-a-static-method dance.
        return new ElastosHttpClient(manager, params);
#endif
}

ECode ElastosHttpClient::NewInstance(
    /* [in] */ const String& userAgent,
    /* [out] */ IElastosHttpClient** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return newInstance(userAgent, NULL /* session cache */);
#endif
}

ElastosHttpClient::ElastosHttpClient(
    /* [in] */ IClientConnectionManager* ccm,
    /* [in] */ IHttpParams* params)
{
#if 0 // TODO: Translate codes below
        this.delegate = new DefaultHttpClient(ccm, params) {
            @Override
            protected BasicHttpProcessor createHttpProcessor() {
                // Add interceptor to prevent making requests from main thread.
                BasicHttpProcessor processor = super.createHttpProcessor();
                processor.addRequestInterceptor(sThreadCheckInterceptor);
                processor.addRequestInterceptor(new CurlLogger());
                return processor;
            }
            @Override
            protected HttpContext createHttpContext() {
                // Same as DefaultHttpClient.createHttpContext() minus the
                // cookie store.
                HttpContext context = new BasicHttpContext();
                context.setAttribute(
                        ClientContext.AUTHSCHEME_REGISTRY,
                        getAuthSchemes());
                context.setAttribute(
                        ClientContext.COOKIESPEC_REGISTRY,
                        getCookieSpecs());
                context.setAttribute(
                        ClientContext.CREDS_PROVIDER,
                        getCredentialsProvider());
                return context;
            }
        };
#endif
}

ECode ElastosHttpClient::ModifyRequestToAcceptGzipResponse(
    /* [in] */ IHttpRequest* request)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        request.addHeader("Accept-Encoding", "gzip");
#endif
}

ECode ElastosHttpClient::GetUngzippedContent(
    /* [in] */ IHttpEntity* entity,
    /* [out] */ IInputStream** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        InputStream responseStream = entity.getContent();
        if (responseStream == NULL) return responseStream;
        Header header = entity.getContentEncoding();
        if (header == NULL) return responseStream;
        String contentEncoding = header.getValue();
        if (contentEncoding == NULL) return responseStream;
        if (contentEncoding.contains("gzip")) responseStream
                = new GZIPInputStream(responseStream);
        return responseStream;
#endif
}

ECode ElastosHttpClient::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mLeakedException != NULL) {
            getConnectionManager().shutdown();
            mLeakedException = NULL;
        }
#endif
}

ECode ElastosHttpClient::GetParams(
    /* [out] */ IHttpParams** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getParams();
#endif
}

ECode ElastosHttpClient::GetConnectionManager(
    /* [out] */ IClientConnectionManager** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getConnectionManager();
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpUriRequest* request,
    /* [out] */ IHttpResponse** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(request);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpUriRequest* request,
    /* [in] */ IHttpContext* context,
    /* [out] */ IHttpResponse** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(request, context);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [out] */ IHttpResponse** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(target, request);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context,
    /* [out] */ IHttpResponse** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(target, request, context);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpUriRequest* request,
    /* [in] */ IResponseHandler* responseHandler,
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(request, responseHandler);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpUriRequest* request,
    /* [in] */ IResponseHandler* responseHandler,
    /* [in] */ IHttpContext* context,
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(request, responseHandler, context);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [in] */ IResponseHandler* responseHandler,
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(target, request, responseHandler);
#endif
}

ECode ElastosHttpClient::Execute(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [in] */ IResponseHandler* responseHandler,
    /* [in] */ IHttpContext* context,
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.execute(target, request, responseHandler, context);
#endif
}

ECode ElastosHttpClient::GetCompressedEntity(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ IContentResolver* resolver,
    /* [out] */ IAbstractHttpEntity** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AbstractHttpEntity entity;
        if (data.length < getMinGzipSize(resolver)) {
            entity = new ByteArrayEntity(data);
        } else {
            ByteArrayOutputStream arr = new ByteArrayOutputStream();
            OutputStream zipper = new GZIPOutputStream(arr);
            zipper.write(data);
            zipper.close();
            entity = new ByteArrayEntity(arr.toByteArray());
            entity.setContentEncoding("gzip");
        }
        return entity;
#endif
}

ECode ElastosHttpClient::GetMinGzipSize(
    /* [in] */ IContentResolver* resolver,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return DEFAULT_SYNC_MIN_GZIP_BYTES;  // For now, this is just a constant.
#endif
}

ECode ElastosHttpClient::EnableCurlLogging(
    /* [in] */ const String& name,
    /* [in] */ Int32 level)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
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
#endif
}

ECode ElastosHttpClient::DisableCurlLogging()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mCurlConfiguration != NULL)
    {
        delete mCurlConfiguration;
        mCurlConfiguration = NULL;
    }

    return NOERROR;
#endif
}

ECode ElastosHttpClient::ToCurl(
    /* [in] */ IHttpUriRequest* request,
    /* [in] */ Boolean logAuthToken,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        StringBuilder builder = new StringBuilder();
        builder.append("curl ");
        // add in the method
        builder.append("-X ");
        builder.append(request.getMethod());
        builder.append(" ");
        for (Header header: request.getAllHeaders()) {
            if (!logAuthToken
                    && (header.getName().equals("Authorization") ||
                        header.getName().equals("Cookie"))) {
                continue;
            }
            builder.append("--header \"");
            builder.append(header.toString().trim());
            builder.append("\" ");
        }
        URI uri = request.getURI();
        // If this is a wrapped request, use the URI from the original
        // request instead. getURI() on the wrapper seems to return a
        // relative URI. We want an absolute URI.
        if (IRequestWrapper::Probe(request) != NULL) {
            HttpRequest original = ((RequestWrapper) request).getOriginal();
            if (IHttpUriRequest::Probe(original) != NULL) {
                uri = ((HttpUriRequest) original).getURI();
            }
        }
        builder.append("\"");
        builder.append(uri);
        builder.append("\"");
        if (IHttpEntityEnclosingRequest::Probe(request) != NULL) {
            HttpEntityEnclosingRequest entityRequest =
                    (HttpEntityEnclosingRequest) request;
            HttpEntity entity = entityRequest.getEntity();
            if (entity != NULL && entity.isRepeatable()) {
                if (entity.getContentLength() < 1024) {
                    ByteArrayOutputStream stream = new ByteArrayOutputStream();
                    entity.writeTo(stream);
                    if (isBinaryContent(request)) {
                        String base64 = Base64.encodeToString(stream.toByteArray(), Base64.NO_WRAP);
                        builder.insert(0, "echo '" + base64 + "' | base64 -d > /tmp/$$.bin; ");
                        builder.append(" --data-binary @/tmp/$$.bin");
                    } else {
                        String entityString = stream.toString();
                        builder.append(" --data-ascii \"")
                                .append(entityString)
                                .append("\"");
                    }
                } else {
                    builder.append(" [TOO MUCH DATA TO INCLUDE]");
                }
            }
        }
        return builder.toString();
#endif
}

ECode ElastosHttpClient::IsBinaryContent(
    /* [in] */ IHttpUriRequest* request,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Header[] headers;
        headers = request.getHeaders(Headers.CONTENT_ENCODING);
        if (headers != NULL) {
            for (Header header : headers) {
                if ("gzip".equalsIgnoreCase(header.getValue())) {
                    return TRUE;
                }
            }
        }
        headers = request.getHeaders(Headers.CONTENT_TYPE);
        if (headers != NULL) {
            for (Header header : headers) {
                for (String contentType : textContentTypes) {
                    if (header.getValue().StartWith(contentType)) {
                        return FALSE;
                    }
                }
            }
        }
        return TRUE;
#endif
}

ECode ElastosHttpClient::ParseDate(
    /* [in] */ const String& dateString,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return HttpDateTime.parse(dateString);
#endif
}

//================================================================
// ElastosHttpClient::LoggingConfiguration
//================================================================
ElastosHttpClient::LoggingConfiguration::LoggingConfiguration(
    /* [in] */ const String& tag,
    /* [in] */ Int32 level)
    : TAG(tag)
    , LEVEL(level)
{}

ECode ElastosHttpClient::LoggingConfiguration::IsLoggable(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return Logger::IsLoggable(mTag, mLevel);
#endif
}

ECode ElastosHttpClient::LoggingConfiguration::Println(
    /* [in] */ const String& message)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    return Logger::Println(mLevel, mTag, message);
#endif
}

//================================================================
// ElastosHttpClient::CurlLogger
//================================================================
ECode ElastosHttpClient::CurlLogger::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CElastosHttpClient.cpp

ECode CElastosHttpClient::HttpRequestInterceptor::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    // Prevent the HttpRequest from being sent on the main thread
    AutoPtr<ILooper> l;
    CLooperHelper::MyLooper((ILooper**)&l);
    AutoPtr<ILooper> ml;
    helper->GetMainLooper((ILooper**)&ml);
    if (l != NULL && l == ml ) {
        // throw new RuntimeException("This thread forbids HTTP requests");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

CElastosHttpClient::CurlLogger::CurlLogger(
    /* [in] */ LoggingConfiguration* logCongfig)
    : mLogConfig(logCongfig)
{}

CElastosHttpClient::CElastosHttpClient()
    : mCurlConfiguration(NULL)
{}

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
#endif