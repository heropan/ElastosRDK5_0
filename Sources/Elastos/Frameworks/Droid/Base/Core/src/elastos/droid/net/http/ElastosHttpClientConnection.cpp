
#include "elastos/droid/net/http/ElastosHttpClientConnection.h"

using Org::Apache::Http::EIID_IHttpConnection;
using Org::Apache::Http::EIID_IHttpInetConnection;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL_3(ElastosHttpClientConnection, Object, IHttpInetConnection, IHttpConnection, IElastosHttpClientConnection)

ElastosHttpClientConnection::ElastosHttpClientConnection()
    : mMaxHeaderCount(0)
    , mMaxLineLength(0)
    , mOpen(FALSE)
{}

ECode ElastosHttpClientConnection::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.entityserializer =  new EntitySerializer(
                new StrictContentLengthStrategy());

#endif
}

ECode ElastosHttpClientConnection::Bind(
    /* [in] */ ISocket* socket,
    /* [in] */ IHttpParams* params)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (socket == null) {
            throw new IllegalArgumentException("Socket may not be null");
        }
        if (params == null) {
            throw new IllegalArgumentException("HTTP parameters may not be null");
        }
        assertNotOpen();
        socket.setTcpNoDelay(HttpConnectionParams.getTcpNoDelay(params));
        socket.setSoTimeout(HttpConnectionParams.getSoTimeout(params));
        int linger = HttpConnectionParams.getLinger(params);
        if (linger >= 0) {
            socket.setSoLinger(linger > 0, linger);
        }
        this.socket = socket;
        int buffersize = HttpConnectionParams.getSocketBufferSize(params);
        this.inbuffer = new SocketInputBuffer(socket, buffersize, params);
        this.outbuffer = new SocketOutputBuffer(socket, buffersize, params);
        maxHeaderCount = params.getIntParameter(
                CoreConnectionPNames.MAX_HEADER_COUNT, -1);
        maxLineLength = params.getIntParameter(
                CoreConnectionPNames.MAX_LINE_LENGTH, -1);
        this.requestWriter = new HttpRequestWriter(outbuffer, null, params);
        this.metrics = new HttpConnectionMetricsImpl(
                inbuffer.getMetrics(),
                outbuffer.getMetrics());
        this.open = true;

#endif
}

ECode ElastosHttpClientConnection::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    AutoPtr<StringBuilder> buffer = new StringBuilder();
    buffer->AppendCStr("CElastosHttpClientConnection[");
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
#endif
}

ECode ElastosHttpClientConnection::AssertNotOpen()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (mOpen) return E_ILLEGALSTATE_EXCEPTION;
    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::AssertOpen()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    if (!mOpen) return E_ILLEGALSTATE_EXCEPTION;
    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::IsOpen(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(isOpen);

    // to make this method useful, we want to check if the socket is connected
    *isOpen = FALSE;
    if (mOpen && mSocket != NULL) {
        Boolean isConnected;
        mSocket->IsConnected(&isConnected);
        *isOpen = isConnected;
    }

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::GetLocalAddress(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mSocket != NULL) {
        return mSocket->GetLocalAddress(address);
    } else {
        *address = NULL;
    }

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::GetLocalPort(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mSocket != NULL) {
        return mSocket->GetLocalPort(port);
    } else {
        *port = -1;
    }

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::GetRemoteAddress(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mSocket != NULL) {
        return mSocket->GetInetAddress(address);
    } else {
        *address = NULL;
    }

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::GetRemotePort(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mSocket != NULL) {
        return mSocket->GetPort(port);
    } else {
        *port = -1;
    }

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::SetSocketTimeout(
    /* [in] */ Int32 timeout)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    FAIL_RETURN(AssertOpen());
    if (mSocket != NULL) {
        // It is not quite clear from the original documentation if there are any
        // other legitimate cases for a socket exception to be thrown when setting
        // SO_TIMEOUT besides the socket being already closed
        return mSocket->SetSoTimeout(timeout);
    }

    return E_ILLEGALSTATE_EXCEPTION;
#endif
}

ECode ElastosHttpClientConnection::GetSocketTimeout(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mSocket != NULL) {
        return mSocket->GetSoTimeout(timeout);
    }

    *timeout = -1;
    return E_ILLEGALSTATE_EXCEPTION;
#endif
}

ECode ElastosHttpClientConnection::Shutdown()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mOpen = FALSE;
    if (mSocket != NULL) {
        return mSocket->Close();
    }

    return E_ILLEGALSTATE_EXCEPTION;
#endif
}

ECode ElastosHttpClientConnection::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode ElastosHttpClientConnection::SendRequestHeader(
    /* [in] */ IHttpRequest* request)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (request == NULL)
    {
        // return E_ILLEGALSTATE_EXCEPTION;;
    }

    FAIL_RETURN(AssertOpen());
    // tmRequestWriter->Write(request);
    // mMetrics->IncrementRequestCount();

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::SendRequestEntity(
    /* [in] */ IHttpEntityEnclosingRequest* request)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode ElastosHttpClientConnection::DoFlush()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.outbuffer.flush();

#endif
}

ECode ElastosHttpClientConnection::Flush()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    FAIL_RETURN(AssertOpen());
    return DoFlush();
#endif
}

ECode ElastosHttpClientConnection::ParseResponseHeader(
    /* [in] */ IHeaders* headers,
    /* [out] */ IStatusLine** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        assertOpen();
        CharArrayBuffer current = new CharArrayBuffer(64);
        if (inbuffer.readLine(current) == -1) {
            throw new NoHttpResponseException("The target server failed to respond");
        }
        // Create the status line from the status string
        StatusLine statusline = BasicLineParser.DEFAULT.parseStatusLine(
                current, new ParserCursor(0, current.length()));

        if (HttpLog.LOGV) HttpLog.v("read: " + statusline);
        int statusCode = statusline.getStatusCode();
        // Parse header body
        CharArrayBuffer previous = null;
        int headerNumber = 0;
        while(true) {
            if (current == null) {
                current = new CharArrayBuffer(64);
            } else {
                // This must be he buffer used to parse the status
                current.clear();
            }
            int l = inbuffer.readLine(current);
            if (l == -1 || current.length() < 1) {
                break;
            }
            // Parse the header name and value
            // Check for folded headers first
            // Detect LWS-char see HTTP/1.0 or HTTP/1.1 Section 2.2
            // discussion on folded headers
            char first = current.charAt(0);
            if ((first == ' ' || first == '\t') && previous != null) {
                // we have continuation folded header
                // so append value
                int start = 0;
                int length = current.length();
                while (start < length) {
                    char ch = current.charAt(start);
                    if (ch != ' ' && ch != '\t') {
                        break;
                    }
                    start++;
                }
                if (maxLineLength > 0 &&
                        previous.length() + 1 + current.length() - start >
                            maxLineLength) {
                    throw new IOException("Maximum line length limit exceeded");
                }
                previous.append(' ');
                previous.append(current, start, current.length() - start);
            } else {
                if (previous != null) {
                    headers.parseHeader(previous);
                }
                headerNumber++;
                previous = current;
                current = null;
            }
            if (maxHeaderCount > 0 && headerNumber >= maxHeaderCount) {
                throw new IOException("Maximum header count exceeded");
            }
        }
        if (previous != null) {
            headers.parseHeader(previous);
        }
        if (statusCode >= 200) {
            this.metrics.incrementResponseCount();
        }
        return statusline;

#endif
}

ECode ElastosHttpClientConnection::ReceiveResponseEntity(
    /* [in] */ IHeaders* headers,
    /* [out] */ IHttpEntity** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        assertOpen();
        BasicHttpEntity entity = new BasicHttpEntity();
        long len = determineLength(headers);
        if (len == ContentLengthStrategy.CHUNKED) {
            entity.setChunked(true);
            entity.setContentLength(-1);
            entity.setContent(new ChunkedInputStream(inbuffer));
        } else if (len == ContentLengthStrategy.IDENTITY) {
            entity.setChunked(false);
            entity.setContentLength(-1);
            entity.setContent(new IdentityInputStream(inbuffer));
        } else {
            entity.setChunked(false);
            entity.setContentLength(len);
            entity.setContent(new ContentLengthInputStream(inbuffer, len));
        }
        String contentTypeHeader = headers.getContentType();
        if (contentTypeHeader != null) {
            entity.setContentType(contentTypeHeader);
        }
        String contentEncodingHeader = headers.getContentEncoding();
        if (contentEncodingHeader != null) {
            entity.setContentEncoding(contentEncodingHeader);
        }
       return entity;

#endif
}

ECode ElastosHttpClientConnection::DetermineLength(
    /* [in] */ const IHeaders* headers,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
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
#endif
}

ECode ElastosHttpClientConnection::IsStale(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    FAIL_RETURN(AssertOpen());

    Boolean available;
    // if(NOERROR == mInbuffer->IsDataAvailable(1, &available)) {
    //     *isStale = FALSE;
    // }

    *isStale = TRUE;

    return NOERROR;
#endif
}

ECode ElastosHttpClientConnection::GetMetrics(
    /* [out] */ IHttpConnectionMetrics** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return this.metrics;

#endif
}


} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
