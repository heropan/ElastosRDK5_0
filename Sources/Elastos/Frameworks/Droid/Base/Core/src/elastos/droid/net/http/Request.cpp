
#include "Request.h"
// #include "CHttpsConnection.h"
#include "HttpLog.h"
#include <elastos/utility/etl/HashMap.h>

using namespace Elastos::Core;

using Org::Apache::Http::Entity::IInputStreamEntity;
using Org::Apache::Http::Message::IBasicHttpEntityEnclosingRequest;
using Org::Apache::Http::Message::EIID_IBasicHttpEntityEnclosingRequest;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String Request::HOST_HEADER = String("Host");
const String Request::ACCEPT_ENCODING_HEADER = String("Accept-Encoding");
const String Request::CONTENT_LENGTH_HEADER = String("content-length");

AutoPtr<IRequestContent> Request::mRequestContentProcessor = InitRequestContentProcessor();
AutoPtr<IRequestContent> Request::InitRequestContentProcessor()
{
    AutoPtr<IRequestContent> reqCon;
    // CRequestContent::New((IRequestContent**)&reqCon);
    return reqCon;
}

CAR_INTERFACE_IMPL(Request, IRequest);

ECode Request::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Request::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Request::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    return E_NOT_IMPLEMENTED;
}

ECode Request::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    IRequest * o = IRequest::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IRequest));
    }
    return NOERROR;
}

ECode Request::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IRequest);
    return NOERROR;
}

ECode Request::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    StringBuilder sb("Request:(");
    sb += (Int32)THIS_PROBE(IRequest);
    sb += ")";
    sb.ToString(info);
    return NOERROR;
}

Request::Request(
    /* [in] */ const String& method,
    /* [in] */ IHttpHost* host,
    /* [in] */ IHttpHost* proxyHost,
    /* [in] */ const String& path,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [in] */ IEventHandler* eventHandler,
    /* [in] */ HashMap<String, String>* headers)
    : mCancelled(FALSE)
    , mFailCount(0)
    , mReceivedBytes(0)
    , mBodyLength(bodyLength)
    , mLoadingPaused(FALSE)
    , mEventHandler(eventHandler)
    , mHost(host)
    , mProxyHost(proxyHost)
    , mPath(path)
    , mBodyProvider(bodyProvider)
{
    if (bodyProvider == NULL && !method.EqualsIgnoreCase("POST")) {
        // TODO:
        // CBasicHttpRequest::New(method, GetUri(), (IBasicHttpRequest**)&mHttpRequest);
    } else {
        // CBasicHttpEntityEnclosingRequest(method, GetUri(), (IBasicHttpRequest**)&mHttpRequest);

        // it is ok to have null entity for BasicHttpEntityEnclosingRequest.
        // By using BasicHttpEntityEnclosingRequest, it will set up the
        // correct content-length, content-type and content-encoding.
        if (bodyProvider != NULL) {
            SetBodyProvider(bodyProvider, bodyLength);
        }
    }
    AddHeader(HOST_HEADER, GetHostPort());

    /* FIXME: if webcore will make the root document a
       high-priority request, we can ask for gzip encoding only on
       high priority reqs (saving the trouble for images, etc) */
    AddHeader(ACCEPT_ENCODING_HEADER, String("gzip"));
    AddHeaders(headers);
}

Request::~Request()
{}

ECode Request::SetLoadingPaused(
     /* [in] */ Boolean pause)
{
    AutoLock lock(mClientResource);

    mLoadingPaused = pause;

    // Wake up the paused thread if we're unpausing the load.
    if (!mLoadingPaused) {
        // TODO:
        // Notify();
    }

    return NOERROR;
}

ECode Request::SetConnection(
    /* [in] */ Connection* connection)
{
    mConnection = connection;
    return NOERROR;
}

IEventHandler* Request::GetEventHandler()
{
    return mEventHandler;
}

ECode Request::AddHeader(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    if (name.IsNullOrEmpty()) {
        String damage = String("Null http header name");
        HttpLog::E(damage);
        return E_NULL_POINTER_EXCEPTION;
    }
    if (value.IsNullOrEmpty() || value.GetLength() == 0) {
        String damage = String("Null or empty value for header \"") + name + String("\"");
        HttpLog::E(damage);
        return E_RUNTIME_EXCEPTION;
    }

    // TODO:
    // return mHttpRequest->AddHeader(name, value);
    return E_NOT_IMPLEMENTED;
}

ECode Request::AddHeaders(
    /* [in] */ HashMap<String, String>* headers)
{
    if (headers == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    HashMap<String, String>::Iterator it = headers->Begin();
    for (; it != headers->End(); ++it) {
        String name = it->mFirst;
        String value = it->mSecond;
        AddHeader(name, value);
    }

    return NOERROR;
}

ECode Request::SendRequest(
    /* [in] */ IAndroidHttpClientConnection* httpClientConnection)
{
    if (mCancelled){
        return NOERROR; // don't send cancelled requests
    }

    if (HttpLog::LOGV) {
        String name;
        // TODO:
        // mHost->GetSchemeName(&name);
        HttpLog::V(String("Request.sendRequest() ") + name + String("://") + GetHostPort());
        // if (FALSE) {
        //     Iterator i = mHttpRequest.headerIterator();
        //     while (i.hasNext()) {
        //         Header header = (Header)i.next();
        //         HttpLog.v(header.getName() + ": " + header.getValue());
        //     }
        // }
    }

    AutoPtr<IHttpContext> context;
    mConnection->GetHttpContext((IHttpContext**)&context);
    // mRequestContentProcessor->Process(mHttpRequest, context);
    AutoPtr<IHttpRequest> req = (IHttpRequest*)mHttpRequest->Probe(EIID_IHttpRequest);
    httpClientConnection->SendRequestHeader(req);
    AutoPtr<IHttpEntityEnclosingRequest> httpReq = (IHttpEntityEnclosingRequest*)mHttpRequest->Probe(EIID_IHttpEntityEnclosingRequest);
    if (httpReq != NULL) {
        httpClientConnection->SendRequestEntity(httpReq);
    }

    if (HttpLog::LOGV) {
        String name;
        // mHost->GetSchemeName(&name);
        HttpLog::V(String("Request.requestSent() ")  + name + String("://") + GetHostPort() + mPath);
    }
}

ECode Request::ReadResponse(
    /* [in] */ IAndroidHttpClientConnection* httpClientConnection)
{
    if (mCancelled){
        return NOERROR; // don't send cancelled requests
    }

    AutoPtr<IStatusLine> statusLine;
    Boolean hasBody = FALSE;
    httpClientConnection->Flush();
    Int32 statusCode = 0;

    AutoPtr<IHeaders> header;
    CHeaders::New((IHeaders**)&header);
    do {
        statusLine = NULL;
        FAIL_RETURN(httpClientConnection->ParseResponseHeader(header, (IStatusLine**)&statusLine));
        // TODO:
        // statusCode = statusLine->GetStatusCode();
    } while (statusCode < IHttpStatus::SC_OK);
    if (HttpLog::LOGV) {
        String str;
        // statusLine->ToString(&str);
        HttpLog::V(String("Request.readResponseStatus() ") +
                StringUtils::Int32ToString(str.GetLength()) + String(" ") + str);
    }

    AutoPtr<IProtocolVersion> v;
    // TODO:
    // statusLine->GetProtocolVersion((IProtocolVersion**)&v);
    Int32 major;
    Int32 minor;
    // v->GetMajor(&major);
    // v->GetMinor(&minor);
    String phrase;
    // statusLine->GetReasonPhrase(&phrase);
    mEventHandler->Status(major, minor, statusCode, phrase);
    mEventHandler->Headers(header);
    AutoPtr<IHttpEntity> entity;
    AutoPtr<IHttpRequest> req = (IHttpRequest*)mHttpRequest->Probe(EIID_IHttpRequest);
    hasBody = CanResponseHaveBody(req, statusCode);

    if (hasBody) {
        httpClientConnection->ReceiveResponseEntity(header, (IHttpEntity**)&entity);
    }

    // restrict the range request to the servers claiming that they are
    // accepting ranges in bytes
    String ranges;
    header->GetAcceptRanges(&ranges);
    Boolean supportPartialContent = ranges.EqualsIgnoreCase("bytes");

    if (entity != NULL) {
        AutoPtr<IInputStream> is;
        // entity->GetContent((IInputStream**)&is);

        // process gzip content encoding
        AutoPtr<IHeader> contentEncoding;
        // entity->GetContentEncoding(&contentEncoding);
        AutoPtr<IInputStream> nis;
        AutoPtr<ArrayOf<Byte> > buf;
        Int32 count = 0;

        String value;
        if (contentEncoding != NULL &&
            (/*contentEncoding->GetValue(&value),*/ value).Equals("gzip")) {
            ECode ec;
            // ec = CGZIPInputStream::New(is, (IInputStream**)&nis);
            if(FAILED(ec)) {
                // don't throw if we have a non-OK status code
                if (statusCode == IHttpStatus::SC_OK
                        || statusCode == IHttpStatus::SC_PARTIAL_CONTENT) {
                    if (supportPartialContent && count > 0) {
                        // if there is uncommited content, we should commit them
                        // as we will continue the request
                        // TODO:
                        // mEventHandler->Data(buf, count);
                    }

                    if (nis != NULL) {
                        nis->Close();
                    }

                    return ec;
                }
            }
        } else {
            nis = is;
        }

        /* accumulate enough data to make it worth pushing it
         * up the stack */
        mConnection->GetBuf((ArrayOf<Byte>**)&buf);
        Int32 len = 0;
        Int32 lowWater = buf->GetLength() / 2;
        while (len != -1) {
            {
                AutoLock lock(mClientResource);

                while (mLoadingPaused) {
                    // Put this (network loading) thread to sleep if WebCore
                    // has asked us to. This can happen with plugins for
                    // example, if we are streaming data but the plugin has
                    // filled its internal buffers.
                    // if(FAILED(Wait()) {
                    //     HttpLog::E(String("Interrupted exception whilst network thread paused at WebCore's request."));
                    // }
                }
            }

            ECode ec;
            ec = nis->ReadBytes(buf, count, buf->GetLength() - count, &len);
            if(FAILED(ec)) {
                /* InflaterInputStream throws an EOFException when the
                   server truncates gzipped content.  Handle this case
                   as we do truncated non-gzipped content: no error */
                if (count > 0) {
                    // if there is uncommited content, we should commit them
                    // TODO:
                    // mEventHandler->Data(buf, count);
                }
                if (HttpLog::LOGV) HttpLog::V(String("readResponse() handling "));
            }

            if (len != -1) {
                count += len;
                if (supportPartialContent) mReceivedBytes += len;
            }
            if (len == -1 || count >= lowWater) {
                if (HttpLog::LOGV) {
                    HttpLog::V(String("Request.readResponse() ") + StringUtils::Int32ToString(count));
                }
                // mEventHandler->Data(buf, count);
                count = 0;
            }
        }

        if (nis != NULL) {
            nis->Close();
        }
    }

    AutoPtr<IProtocolVersion>  ver;
    // statusLine->GetProtocolVersion((IProtocolVersion**)&ver);
    Int32 type;
    header->GetConnectionType(&type);
    mConnection->SetCanPersist(entity, ver, type);
    mEventHandler->EndData();
    Complete();

    if (HttpLog::LOGV) {
        String name;
        // mHost->GetSchemeName(&name);
        HttpLog::V(String("Request.readResponse(): done ") +
            name + "://" + GetHostPort() + mPath);
    }

    return NOERROR;
}

ECode Request::Cancel()
{
    AutoLock lock(mClientResource);

    if (HttpLog::LOGV) {
        HttpLog::V(String("Request.cancel(): ") + GetUri());
    }

    // Ensure that the network thread is not blocked by a hanging request from WebCore to
    // pause the load.
    mLoadingPaused = FALSE;
    // TODO:
    // Notify();

    mCancelled = TRUE;
    if (mConnection != NULL) {
        mConnection->Cancel();
    }

    return NOERROR;
}

String Request::GetHostPort()
{
    String myScheme;
    // TODO:
    // mHost->GetSchemeName(&myScheme);
    Int32 myPort;
    // mHost->GetPort(&myPort);

    String port;
    // Only send port when we must... many servers can't deal with it
    if (myPort != 80 && myScheme.Equals("http") ||
        myPort != 443 && myScheme.Equals("https")) {
        // mHost->ToHostString(&port);
    } else {
        // mHost->GetHostName(&port);
    }

    return port;
}

String Request::GetUri()
{
    String name;
    // TODO:
    if (mProxyHost == NULL ||
            (/*mHost->GetSchemeName(&name),*/ name).Equals("https")) {
        return mPath;
    }
    // mHost->GetSchemeName(&name);
    return name + String("://") + GetHostPort() + mPath;
}

ECode Request::ToString(
    /* [out] */ String* str)
{
    *str = mPath;
    return NOERROR;
}

ECode Request::Reset()
{
    /* clear content-length header */
    // TODO:
    // mHttpRequest->RemoveHeaders(CONTENT_LENGTH_HEADER);

    if (mBodyProvider != NULL) {
        if(FAILED(mBodyProvider->Reset())) {
            if (HttpLog::LOGV) {
                HttpLog::V(String("failed to reset body provider ") + GetUri());
            }
        }

        SetBodyProvider(mBodyProvider, mBodyLength);
    }

    if (mReceivedBytes > 0) {
        // reset the fail count as we continue the request
        mFailCount = 0;
        // set the "Range" header to indicate that the retry will continue
        // instead of restarting the request
        HttpLog::V(String("*** Request.reset() to range:") + StringUtils::Int32ToString(mReceivedBytes));
        // TODO:
        // mHttpRequest->SetHeader(String("Range"), String("bytes=") + StringUtils::Int32ToString(mReceivedBytes) + String("-"));
    }

    return NOERROR;
}

ECode Request::WaitUntilComplete()
{
    AutoLock lock(mClientResource);

    if (HttpLog::LOGV) HttpLog::V(String("Request.waitUntilComplete()"));
    // TODO:
    // mClientResource.Wait();
    if (HttpLog::LOGV) HttpLog::V(String("Request.waitUntilComplete() done waiting"));

    return NOERROR;
}

ECode Request::Complete()
{
    AutoLock lock(mClientResource);

    // TODO:
    // return mClientResource.NotifyAll();
    return E_NOT_IMPLEMENTED;
}

Boolean Request::CanResponseHaveBody(
    /* [in] */ IHttpRequest* request,
    /* [in] */ Int32 status)
{
    AutoPtr<IRequestLine> line;
    // TODO:
    // request->GetRequestLine((IRequestLine**)&line);
    String str;
    line->GetMethod(&str);
    if (str.EqualsIgnoreCase("HEAD")) {
        return FALSE;
    }
    return status >= IHttpStatus::SC_OK
        && status != IHttpStatus::SC_NO_CONTENT
        && status != IHttpStatus::SC_NOT_MODIFIED;
}

ECode Request::SetBodyProvider(
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength)
{
    // TODO:
    // if (!bodyProvider->MarkSupported()) {
    //     Logger::E(String("bodyProvider must support mark()"));
    //     return E_ILLEGAL_ARGUMENT_EXCEPTION;
    // }
    // Mark beginning of stream
    bodyProvider->Mark(0x7FFFFFFF/*IInteger::MAX_VALUE*/);

    AutoPtr<IBasicHttpEntityEnclosingRequest> request
        = (IBasicHttpEntityEnclosingRequest*)mHttpRequest->Probe(EIID_IBasicHttpEntityEnclosingRequest);
    AutoPtr<IInputStreamEntity> entity;
    // CInputStreamEntity::New(bodyProvider, bodyLength, (IInputStreamEntity**)&entity);
    return request->SetEntity(entity);
}

ECode Request::HandleSslErrorResponse(
    /* [in] */ Boolean proceed)
{
    AutoPtr<IHttpsConnection> connection;
    // TODO:
    // connection = ((CHttpsConnection)(mConnection))->Probe(EIID_IHttpsConnection);
    if (connection != NULL) {
        // connection->RestartConnection(proceed);
    }

    return NOERROR;
}

ECode Request::Error(
    /* [in] */ Int32 errorId,
    /* [in] */ Int32 resourceId)
{
    AutoPtr<ICharSequence> cs;
    mConnection->mContext->GetText(resourceId, (ICharSequence**)&cs);
    String str;
    cs->ToString(&str);
    return mEventHandler->Error(errorId, str);
}

}
}
}
}
