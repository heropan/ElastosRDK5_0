
#include "elastos/droid/net/http/ElastosHttpClientConnection.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(ElastosHttpClientConnection, $SUPER_CLASS$, IElastosHttpClientConnection)

// This var below should be set to class constructor.
mInbuffer(null)
// This var below should be set to class constructor.
mOutbuffer(null)
// This var below should be set to class constructor.
mRequestWriter(null)
// This var below should be set to class constructor.
mMetrics(null)
// This var below should be set to class constructor.
mSocket(null)

ECode ElastosHttpClientConnection::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.entityserializer =  new EntitySerializer(
                new StrictContentLengthStrategy());

#endif
}

ECode ElastosHttpClientConnection::Bind(
    /* [in] */  ,
    /* [in] */  )
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
#if 0 // TODO: Translate codes below
        StringBuilder buffer = new StringBuilder();
        buffer.append(getClass().getSimpleName()).append("[");
        if (isOpen()) {
            buffer.append(getRemotePort());
        } else {
            buffer.append("closed");
        }
        buffer.append("]");
        return buffer.toString();

#endif
}

ECode ElastosHttpClientConnection::AssertNotOpen()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this.open) {
            throw new IllegalStateException("Connection is already open");
        }

#endif
}

ECode ElastosHttpClientConnection::AssertOpen()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!this.open) {
            throw new IllegalStateException("Connection is not open");
        }

#endif
}

ECode ElastosHttpClientConnection::IsOpen(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // to make this method useful, we want to check if the socket is connected
        return (this.open && this.socket != null && this.socket.isConnected());

#endif
}

ECode ElastosHttpClientConnection::GetLocalAddress(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this.socket != null) {
            return this.socket.getLocalAddress();
        } else {
            return null;
        }

#endif
}

ECode ElastosHttpClientConnection::GetLocalPort(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this.socket != null) {
            return this.socket.getLocalPort();
        } else {
            return -1;
        }

#endif
}

ECode ElastosHttpClientConnection::GetRemoteAddress(
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this.socket != null) {
            return this.socket.getInetAddress();
        } else {
            return null;
        }

#endif
}

ECode ElastosHttpClientConnection::GetRemotePort(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this.socket != null) {
            return this.socket.getPort();
        } else {
            return -1;
        }

#endif
}

ECode ElastosHttpClientConnection::SetSocketTimeout(
    /* [in] */ Int32 timeout)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        assertOpen();
        if (this.socket != null) {
            try {
                this.socket.setSoTimeout(timeout);
            } catch (SocketException ignore) {
                // It is not quite clear from the original documentation if there are any
                // other legitimate cases for a socket exception to be thrown when setting
                // SO_TIMEOUT besides the socket being already closed
            }
        }

#endif
}

ECode ElastosHttpClientConnection::GetSocketTimeout(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (this.socket != null) {
            try {
                return this.socket.getSoTimeout();
            } catch (SocketException ignore) {
                return -1;
            }
        } else {
            return -1;
        }

#endif
}

ECode ElastosHttpClientConnection::Shutdown()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.open = false;
        Socket tmpsocket = this.socket;
        if (tmpsocket != null) {
            tmpsocket.close();
        }

#endif
}

ECode ElastosHttpClientConnection::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!this.open) {
            return;
        }
        this.open = false;
        doFlush();
        try {
            try {
                this.socket.shutdownOutput();
            } catch (IOException ignore) {
            }
            try {
                this.socket.shutdownInput();
            } catch (IOException ignore) {
            }
        } catch (UnsupportedOperationException ignore) {
            // if one isn't supported, the other one isn't either
        }
        this.socket.close();

#endif
}

ECode ElastosHttpClientConnection::SendRequestHeader(
    /* [in] */  )
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (request == null) {
            throw new IllegalArgumentException("HTTP request may not be null");
        }
        assertOpen();
        this.requestWriter.write(request);
        this.metrics.incrementRequestCount();

#endif
}

ECode ElastosHttpClientConnection::SendRequestEntity(
    /* [in] */  )
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (request == null) {
            throw new IllegalArgumentException("HTTP request may not be null");
        }
        assertOpen();
        if (request.getEntity() == null) {
            return;
        }
        this.entityserializer.serialize(
                this.outbuffer,
                request,
                request.getEntity());

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
        assertOpen();
        doFlush();

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
    /* [in] */  ,
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
    /* [in] */  ,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        long transferEncoding = headers.getTransferEncoding();
        // We use Transfer-Encoding if present and ignore Content-Length.
        // RFC2616, 4.4 item number 3
        if (transferEncoding < Headers.NO_TRANSFER_ENCODING) {
            return transferEncoding;
        } else {
            long contentlen = headers.getContentLength();
            if (contentlen > Headers.NO_CONTENT_LENGTH) {
                return contentlen;
            } else {
                return ContentLengthStrategy.IDENTITY;
            }
        }

#endif
}

ECode ElastosHttpClientConnection::IsStale(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        assertOpen();
        try {
            this.inbuffer.isDataAvailable(1);
            return false;
        } catch (IOException ex) {
            return true;
        }

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

#if 0 // old CElastosHttpClientConnection.cpp
#include "CElastosHttpClientConnection.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringBuilder.h>

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CElastosHttpClientConnection::CElastosHttpClientConnection()
    : mMaxHeaderCount(0)
    , mMaxLineLength(0)
    , mOpen(FALSE)
{}

CElastosHttpClientConnection::~CElastosHttpClientConnection()
{}

ECode CElastosHttpClientConnection::constructor()
{
    // TODO:
    // AutoPtr<IStrictContentLengthStrategy> strategy;
    // CStrictContentLengthStrategy::New((IStrictContentLengthStrategy**)&strategy);
    // return CEntitySerializer::New(strategy, (IEntitySerializer)&mEntityserializer);
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClientConnection::Bind(
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

ECode CElastosHttpClientConnection::ToString(
    /* [out] */ String* str)
{
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
}

ECode CElastosHttpClientConnection::AssertNotOpen()
{
    if (mOpen) return E_ILLEGALSTATE_EXCEPTION;
    return NOERROR;
}

ECode CElastosHttpClientConnection::AssertOpen()
{
    if (!mOpen) return E_ILLEGALSTATE_EXCEPTION;
    return NOERROR;
}

ECode CElastosHttpClientConnection::IsOpen(
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

ECode CElastosHttpClientConnection::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    if (mSocket != NULL) {
        return mSocket->GetLocalAddress(address);
    } else {
        *address = NULL;
    }

    return NOERROR;
}

ECode CElastosHttpClientConnection::GetLocalPort(
    /* [out] */ Int32* port)
{
    if (mSocket != NULL) {
        return mSocket->GetLocalPort(port);
    } else {
        *port = -1;
    }

    return NOERROR;
}

ECode CElastosHttpClientConnection::GetRemoteAddress(
    /* [out] */ IInetAddress** address)
{
    if (mSocket != NULL) {
        return mSocket->GetInetAddress(address);
    } else {
        *address = NULL;
    }

    return NOERROR;
}

ECode CElastosHttpClientConnection::GetRemotePort(
    /* [out] */ Int32* port)
{
    if (mSocket != NULL) {
        return mSocket->GetPort(port);
    } else {
        *port = -1;
    }

    return NOERROR;
}

ECode CElastosHttpClientConnection::SetSocketTimeout(
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

ECode CElastosHttpClientConnection::GetSocketTimeout(
    /* [out] */ Int32* timeout)
{
    if (mSocket != NULL) {
        return mSocket->GetSoTimeout(timeout);
    }

    *timeout = -1;
    return E_ILLEGALSTATE_EXCEPTION;
}

ECode CElastosHttpClientConnection::Shutdown()
{
    mOpen = FALSE;
    if (mSocket != NULL) {
        return mSocket->Close();
    }

    return E_ILLEGALSTATE_EXCEPTION;
}

ECode CElastosHttpClientConnection::Close()
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

ECode CElastosHttpClientConnection::SendRequestHeader(
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

ECode CElastosHttpClientConnection::SendRequestEntity(
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

ECode CElastosHttpClientConnection::DoFlush()
{
    // if (mOutbuffer != NULL)
    // {
    //     return mOutbuffer->Flush();
    // }

    // return E_ILLEGALSTATE_EXCEPTION;
    return E_NOT_IMPLEMENTED;
}

ECode CElastosHttpClientConnection::Flush()
{
    FAIL_RETURN(AssertOpen());
    return DoFlush();
}

ECode CElastosHttpClientConnection::ParseResponseHeader(
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

ECode CElastosHttpClientConnection::ReceiveResponseEntity(
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

Int64 CElastosHttpClientConnection::DetermineLength(
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

ECode CElastosHttpClientConnection::IsStale(
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
#endif