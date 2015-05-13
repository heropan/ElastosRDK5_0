
#include "CAndroidHttpClientConnection.h"
#include "ext/frameworkext.h"
#include "elastos/StringBuilder.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAndroidHttpClientConnection::CAndroidHttpClientConnection()
    : mMaxHeaderCount(0)
    , mMaxLineLength(0)
    , mOpen(FALSE)
{}

CAndroidHttpClientConnection::~CAndroidHttpClientConnection()
{}

ECode CAndroidHttpClientConnection::constructor()
{
    // TODO:
    // AutoPtr<IStrictContentLengthStrategy> strategy;
    // CStrictContentLengthStrategy::New((IStrictContentLengthStrategy**)&strategy);
    // return CEntitySerializer::New(strategy, (IEntitySerializer)&mEntityserializer);
    return E_NOT_IMPLEMENTED;
}

ECode CAndroidHttpClientConnection::Bind(
    /* [in] */ Elastos::Net::ISocket* socket,
    /* [in] */ Org::Apache::Http::Params::IHttpParams* params)
{
    VALIDATE_NOT_NULL(socket);
    VALIDATE_NOT_NULL(params);

    // FAIL_RETURN(AssertNotOpen());
    // AutoPtr<IHttpConnectionParamsHelper> helper;
    // CHttpConnectionParamsHelper::AcquireSingleton((IHttpConnectionParamsHelper**)&helper);
    // Boolean on;
    // helper->GetTcpNoDelay(params, &on);
    // socket->SetTcpNoDelay(on);
    // Int32 timeout;
    // helper->GetSoTimeout(params, &timeout);
    // socket->SetSoTimeout(timeout);

    // Int32 linger;
    // helper->GetLinger(params, &linger);
    // if (linger >= 0) {
    //     socket->SetSoLinger(linger > 0, linger);
    // }
    // mSocket = socket;

    // Int32 buffersize;
    // helper->GetSocketBufferSize(params, &buffersize);
    // CSocketInputBuffer::New(socket, buffersize, params, (ISocketInputBuffer**)&mInbuffer);

    // params->GetIntParameter(
    //         ICoreConnectionPNames::MAX_HEADER_COUNT, -1, &mMaxHeaderCount);
    // params->GetIntParameter(
    //         ICoreConnectionPNames::MAX_LINE_LENGTH, -1, &mMaxLineLength);

    // CHttpRequestWriter::New(outbuffer, NULL, params, (IHttpMessageWriter**)&mRequestWriter);

    // AutoPtr<IHttpTransportMetrics> inMetrics;
    // mInbuffer->GetMetrics((IHttpTransportMetrics**)&inMetrics);
    // AutoPtr<IHttpTransportMetrics> outMetrics;
    // mOutbuffer->GetMetrics((IHttpTransportMetrics**)&outMetrics);
    // CHttpConnectionMetricsImpl(inMetrics, outMetrics, (IHttpConnectionMetricsImpl**)&mMetrics);

    // mOpen = TRUE;
    // return NOERROR;
    return E_NOT_IMPLEMENTED;
}

ECode CAndroidHttpClientConnection::ToString(
    /* [out] */ String* str)
{
    AutoPtr<StringBuilder> buffer = new StringBuilder();
    buffer->AppendCStr("CAndroidHttpClientConnection[");
    Boolean isOpen;
    IsOpen(&isOpen);
    if (isOpen) {
        Int32 port;
        GetRemotePort(&port);
        buffer->AppendInt32(port);
    } else {
        buffer->AppendCStr("closed");
    }
    buffer->AppendCStr("]");
    *str = buffer->ToString();
    return NOERROR;
}

ECode CAndroidHttpClientConnection::AssertNotOpen()
{
    if (mOpen) return E_ILLEGALSTATE_EXCEPTION;
    return NOERROR;
}

ECode CAndroidHttpClientConnection::AssertOpen()
{
    if (!mOpen) return E_ILLEGALSTATE_EXCEPTION;
    return NOERROR;
}

ECode CAndroidHttpClientConnection::IsOpen(
    /* [out] */ Boolean* isOpen)
{
    VALIDATE_NOT_NULL(isOpen);

    // to make this method useful, we want to check if the socket is connected
    *isOpen = FALSE;
    if (mOpen && mSocket != NULL) {
        Boolean isConnected;
        mSocket->IsConnected(&isConnected);
        *isOpen = isConnected;
    }

    return NOERROR;
}

ECode CAndroidHttpClientConnection::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    if (mSocket != NULL) {
        return mSocket->GetLocalAddress(address);
    } else {
        *address = NULL;
    }

    return NOERROR;
}

ECode CAndroidHttpClientConnection::GetLocalPort(
    /* [out] */ Int32* port)
{
    if (mSocket != NULL) {
        return mSocket->GetLocalPort(port);
    } else {
        *port = -1;
    }

    return NOERROR;
}

ECode CAndroidHttpClientConnection::GetRemoteAddress(
    /* [out] */ IInetAddress** address)
{
    if (mSocket != NULL) {
        return mSocket->GetInetAddress(address);
    } else {
        *address = NULL;
    }

    return NOERROR;
}

ECode CAndroidHttpClientConnection::GetRemotePort(
    /* [out] */ Int32* port)
{
    if (mSocket != NULL) {
        return mSocket->GetPort(port);
    } else {
        *port = -1;
    }

    return NOERROR;
}

ECode CAndroidHttpClientConnection::SetSocketTimeout(
    /* [in] */ Int32 timeout)
{
    FAIL_RETURN(AssertOpen());
    if (mSocket != NULL) {
        // It is not quite clear from the original documentation if there are any
        // other legitimate cases for a socket exception to be thrown when setting
        // SO_TIMEOUT besides the socket being already closed
        return mSocket->SetSoTimeout(timeout);
    }

    return E_ILLEGALSTATE_EXCEPTION;
}

ECode CAndroidHttpClientConnection::GetSocketTimeout(
    /* [out] */ Int32* timeout)
{
    if (mSocket != NULL) {
        return mSocket->GetSoTimeout(timeout);
    }

    *timeout = -1;
    return E_ILLEGALSTATE_EXCEPTION;
}

ECode CAndroidHttpClientConnection::Shutdown()
{
    mOpen = FALSE;
    if (mSocket != NULL) {
        return mSocket->Close();
    }

    return E_ILLEGALSTATE_EXCEPTION;
}

ECode CAndroidHttpClientConnection::Close()
{
    if (!mOpen)
    {
        return NOERROR;
    }

    mOpen = FALSE;
    DoFlush();

    if(!FAILED(mSocket->ShutdownOutput()) {
    }

    if(!FAILED(mSocket->ShutdownInput()) {
    }

    return mSocket->Close();
}

ECode CAndroidHttpClientConnection::SendRequestHeader(
    /* [in] */ Org::Apache::Http::IHttpRequest* request)
{
    if (request == NULL)
    {
        // return E_ILLEGALSTATE_EXCEPTION;;
    }

    FAIL_RETURN(AssertOpen());
    // tmRequestWriter->Write(request);
    // mMetrics->IncrementRequestCount();

    return NOERROR;
}

ECode CAndroidHttpClientConnection::SendRequestEntity(
    /* [in] */ Org::Apache::Http::IHttpEntityEnclosingRequest* request)
{
    if (request == NULL) {
        // return E_ILLEGALSTATE_EXCEPTION;;
    }
    FAIL_RETURN(AssertOpen());

    AutoPtr<IHttpEntity> entity;
    // request->GetEntity((IHttpEntity**)&entity);
    if (entity == NULL) {
        // return E_ILLEGALSTATE_EXCEPTION;
    }
    // return mEntityserializer->Serialize(mOutbuffer, request, entity);
    return E_NOT_IMPLEMENTED;
}

ECode CAndroidHttpClientConnection::DoFlush()
{
    // if (mOutbuffer != NULL)
    // {
    //     return mOutbuffer->Flush();
    // }

    // return E_ILLEGALSTATE_EXCEPTION;
    return E_NOT_IMPLEMENTED;
}

ECode CAndroidHttpClientConnection::Flush()
{
    FAIL_RETURN(AssertOpen());
    return DoFlush();
}

ECode CAndroidHttpClientConnection::ParseResponseHeader(
    /* [in] */ Elastos::Droid::Net::Http::IHeaders* headers,
    /* [out] */ Org::Apache::Http::IStatusLine** statusLine)
{
    VALIDATE_NOT_NULL(statusLine);

    FAIL_RETURN(AssertOpen());

    // AutoPtrI<ICharArrayBuffer> current;
    // CCharArrayBuffer::New(64, (ICharArrayBuffer**)&current);

    // Int32 count;
    // if (NOERROR != mInbuffer->ReadLine(current, &count) || count == -1) {
    //     // return E_NO_HTTP_RESPONSE_EXCEPTION;
    //     return E_NOT_IMPLEMENTED;
    // }

    // // Create the status line from the status string
    // AutoPtr<IStatusLine> slDefault;
    // AutoPtr<IBasicLineParserHelper> helper;
    // CBasicLineParserHelper::AcquireSingleton(&helper);
    // helper->GetDEFAULT((IStatusLine**)&slDefault);
    // AutoPtr<IParserCursor> cursor;
    // CParserCursor(0, current->Length(), &cursor);
    // AutoPtr<IStatusLine> locStatusline;
    // slDefault->ParseStatusLine(current, cursor, (IStatusLine**)&locStatusline);

    // // if (HttpLog.LOGV) HttpLog.v("read: " + locStatusline);
    // Int32 statusCode;
    // locStatusline->GetStatusCode(&statusCode);

    // // Parse header body
    // AutoPtr<ICharArrayBuffer> previous;
    // Int32 headerNumber = 0;
    // while(TRUE) {
    //     if (current == NULL) {
    //         CCharArrayBuffer::New(64, (ICharArrayBuffer**)&current);
    //     } else {
    //         // This must be he buffer used to parse the status
    //         current->Clear();
    //     }
    //     Int32 l;
    //     mInbuffer->ReadLine(current, &l);
    //     if (l == -1 || current->Length() < 1) {
    //         break;
    //     }
    //     // Parse the header name and value
    //     // Check for folded headers first
    //     // Detect LWS-char see HTTP/1.0 or HTTP/1.1 Section 2.2
    //     // discussion on folded headers
    //     Char8 first;
    //     current->CharAt(0, &first);
    //     if ((first == ' ' || first == '\t') && previous != NULL) {
    //         // we have continuation folded header
    //         // so append value
    //         Int32 start = 0;
    //         Int32 length = current->Length();
    //         while (start < length) {
    //             Char8 ch;
    //             current->CharAt(start, &ch);
    //             if (ch != ' ' && ch != '\t') {
    //                 break;
    //             }
    //             start++;
    //         }
    //         if (maxLineLength > 0 &&
    //                 previous->Length() + 1 + current->Length() - start >
    //                     maxLineLength) {
    //             // throw new IOException("Maximum line length limit exceeded");
    //             return E_IO_EXCEPTION;
    //         }
    //         previous->Append(' ');
    //         previous->Append(current, start, current.length() - start);
    //     } else {
    //         if (previous != NULL) {
    //             headers->ParseHeader(previous);
    //         }
    //         headerNumber++;
    //         previous = current;
    //         current = NULL;
    //     }
    //     if (maxHeaderCount > 0 && headerNumber >= maxHeaderCount) {
    //         // throw new IOException("Maximum header count exceeded");
    //         return E_IO_EXCEPTION;
    //     }
    // }

    // if (previous != NULL) {
    //     headers>ParseHeader(previous);
    // }

    // if (statusCode >= 200) {
    //     mMetrics->IncrementResponseCount();
    // }
    // *statusline = locStatusline;
    return E_NOT_IMPLEMENTED;
}

ECode CAndroidHttpClientConnection::ReceiveResponseEntity(
    /* [in] */ Elastos::Droid::Net::Http::IHeaders* headers,
    /* [out] */ Org::Apache::Http::IHttpEntity** entity)
{
    VALIDATE_NOT_NULL(entity);

    FAIL_RETURN(AssertOpen());

    AutoPtr<IBasicHttpEntity> localEntity;
    // CBasicHttpEntity::New(&localEntity);

    // Int64 len;
    // DetermineLength(headers, &len);
    // if (len == IContentLengthStrategy::CHUNKED) {
    //     localEntity->SetChunked(TRUE);
    //     localEntity->SetContentLength(-1);
    //     AutoPtr<IChunkedInputStream> stream;
    //     CChunkedInputStream::New(mInbuffer, (IChunkedInputStream**)&stream);
    //     localEntity->SetContent(stream);
    // } else if (len == ContentLengthStrategy.IDENTITY) {
    //     localEntity->SetChunked(FALSE);
    //     localEntity->SetContentLength(-1);
    //     AutoPtr<IIdentityInputStream> stream;
    //     CIdentityInputStream::New(mInbuffer, (IIdentityInputStream**)&stream);
    //     localEntity->SetContent(stream);
    // } else {
    //     localEntity->SetChunked(FALSE);
    //     localEntity->SetContentLength(len);
    //     AutoPtr<IContentLengthInputStream> stream;
    //     CContentLengthInputStream::New(mInbuffer, len, (IContentLengthInputStream**)&stream);
    //     localEntity->SetContent(stream);
    // }

    // String contentTypeHeader;
    // headers->GetContentType(&contentTypeHeader);
    // if (!contentTypeHeader.IsNullOrEmpty()) {
    //     localEntity->SetContentType(contentTypeHeader);
    // }
    // String contentEncodingHeader;
    // headers->GetContentEncoding(&contentEncodingHeader);
    // if (!contentEncodingHeader.IsNullOrEmpty()) {
    //     localEntity->SetContentEncoding(contentEncodingHeader);
    // }

    // *entity = localEntity;
    return E_NOT_IMPLEMENTED;
}

Int64 CAndroidHttpClientConnection::DetermineLength(
        /* [in] */ IHeaders* headers)
{
    Int64 transferEncoding;
    headers->GetTransferEncoding(&transferEncoding);
    // We use Transfer-Encoding if present and ignore Content-Length.
    // RFC2616, 4.4 item number 3
    if (transferEncoding < IHeaders::NO_TRANSFER_ENCODING) {
        return transferEncoding;
    } else {
        Int64 contentlen;
        headers->GetContentLength(&contentlen);
        if (contentlen > IHeaders::NO_CONTENT_LENGTH) {
            return contentlen;
        } else {
            return IContentLengthStrategy::IDENTITY;
        }
    }
}

ECode CAndroidHttpClientConnection::IsStale(
        /* [out] */ Boolean* isStale)
{
    FAIL_RETURN(AssertOpen());

    Boolean available;
    // if(NOERROR == mInbuffer->IsDataAvailable(1, &available)) {
    //     *isStale = FALSE;
    // }

    *isStale = TRUE;

    return NOERROR;
}

}
}
}
}

