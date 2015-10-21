
#include "Connection.h"
#include "Request.h"
#include "elastos/droid/os/SystemClock.h"
#include <elastos/utility/etl/List.h>
#include <elastos/core/Thread.h>

using Elastos::Droid::Os::SystemClock;
using Elastos::Core::Thread;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Int32 Connection::SOCKET_TIMEOUT = 60000;
const Int32 Connection::SEND = 0;
const Int32 Connection::READ = 1;
const Int32 Connection::DRAIN = 2;
const Int32 Connection::DONE = 3;

const AutoPtr<ArrayOf<String> > Connection::STATES = InitSTATES();
AutoPtr<ArrayOf<String> > Connection::InitSTATES()
{
    AutoPtr<ArrayOf<String> > sArray = ArrayOf<String>::Alloc(4);

    (*sArray)[0] = "SEND";
    (*sArray)[0] = "READ";
    (*sArray)[0] = "DRAIN";
    (*sArray)[0] = "DONE";

    return sArray;
}

Int32 Connection::STATE_NORMAL = 0;
Int32 Connection::STATE_CANCEL_REQUESTED = 1;
const Int32 Connection::RETRY_REQUEST_LIMIT = 2;
const Int32 Connection::MIN_PIPE = 2;
const Int32 Connection::MAX_PIPE = 3;
const String Connection::HTTP_CONNECTION = String("http.connection");

Connection::Connection(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [in] */ RequestFeeder* requestFeeder)
    : mCanPersist(FALSE)
    , mActive(STATE_NORMAL)
    , mContext(context)
    , mHost(host)
    , mRequestFeeder(requestFeeder)
{
    //TODO:
    // CBasicHttpContext::New(NULL, (IHttpContext**)&mHttpContext);
}

AutoPtr<IHttpHost> Connection::GetHost()
{
    return mHost;
}

IConnection* Connection::GetConnection(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [in] */ IHttpHost* proxy,
    /* [in] */ RequestFeeder* requestFeeder)
{
    // String name;
    // host->GetSchemeName(&name);
    // if (name.Equals("http")) {
    //     return new HttpConnection(context, host, requestFeeder);
    // }

    // // Otherwise, default to https
    // return new HttpsConnection(context, host, proxy, requestFeeder);
    return NULL;
}

ECode Connection::Cancel()
{
    mActive = STATE_CANCEL_REQUESTED;
    // if (HttpLog.LOGV) HttpLog.v(
    //     "Connection.cancel(): connection closed " + mHost);
    return CloseConnection();
}

ECode Connection::ProcessRequests(
        /* [in] */ Request* firstRequest)
{
    Request* req;
    Boolean empty;
    Int32 error = IEventHandler::OK;
    Boolean exception = FALSE;

    // LinkedList<Request> pipe = new LinkedList<Request>();
    List<AutoPtr<Request> > pipe;

    Int32 minPipe = MIN_PIPE;
    Int32 maxPipe = MAX_PIPE;
    Int32 state = SEND;

    while (state != DONE) {
        // if (HttpLog.LOGV) HttpLog.v(
        //         states[state] + " pipe " + pipe.size());

        /* If a request was cancelled, give other cancel requests
           some time to go through so we don't uselessly restart
           connections */
        if (mActive == STATE_CANCEL_REQUESTED) {
            Thread::Sleep(100);
            mActive = STATE_NORMAL;
        }

        switch (state) {
            case SEND: {
                if (pipe.GetSize() == maxPipe) {
                    state = READ;
                    break;
                }
                /* get a request */
                if (firstRequest == NULL) {
                    mRequestFeeder->GetRequest(mHost, (Request**)&req);
                } else {
                    req = firstRequest;
                    firstRequest = NULL;
                }
                if (req == NULL) {
                    state = DRAIN;
                    break;
                }
                req->SetConnection(this);

                /* Don't work on cancelled requests. */
                if (req->mCancelled) {
                    // if (HttpLog.LOGV) HttpLog.v(
                    //         "processRequests(): skipping cancelled request "
                    //         + req);
                    req->Complete();
                    break;
                }

                // TODO:
                if (mHttpClientConnection == NULL/* ||
                    !mHttpClientConnection->IsOpen()*/) {
                    /* If this call fails, the address is bad or
                       the net is down.  Punt for now.

                       FIXME: blow out entire queue here on
                       connection failure if net up? */

                    if (!OpenHttpConnection(req)) {
                        state = DONE;
                        break;
                    }
                }

                /* we have a connection, let the event handler
                 * know of any associated certificate,
                 * potentially none.
                 */
                req->mEventHandler->Certificate(mCertificate);


                /* FIXME: don't increment failure count if old
                   connection?  There should not be a penalty for
                   attempting to reuse an old connection */
                // TODO:
                ECode eResult = req->SendRequest(mHttpClientConnection);
                /*if (eResult == E_HTTP_EXCEPTION) {
                    error = IEventHandler::ERROR;
                    exception = TRUE;
                } else */if (eResult == E_IO_EXCEPTION){
                    error = IEventHandler::ERROR_IO;
                    exception = TRUE;
                } else if (eResult == E_ILLEGAL_ARGUMENT_EXCEPTION) {
                    error = IEventHandler::ERROR_IO;
                    exception = TRUE;
                }

                if (exception) {
                    if (HttpFailure(req, error, exception) &&
                        !req->mCancelled) {
                        /* retry request if not permanent failure
                           or cancelled */
                        pipe.PushBack(req);
                    }
                    exception = FALSE;
                    state = ClearPipe(pipe) ? DONE : SEND;
                    minPipe = maxPipe = 1;
                    break;
                }

                pipe.PushBack(req);
                if (!mCanPersist) state = READ;
                break;

            }
            case DRAIN:
            case READ: {
                mRequestFeeder->HaveRequest(mHost, &empty);
                empty = !empty;
                Int32 pipeSize = pipe.GetSize();
                if (state != DRAIN && pipeSize < minPipe &&
                    !empty && mCanPersist) {
                    state = SEND;
                    break;
                } else if (pipeSize == 0) {
                    /* Done if no other work to do */
                    state = empty ? DONE : SEND;
                    break;
                }

                // TODO:
                // req = (Request*)pipe.RemoveFirst();
                // if (HttpLog.LOGV) HttpLog.v(
                //         "processRequests() reading " + req);

                ECode eResult = req->ReadResponse(mHttpClientConnection);
                /*if (eResult == E_PARSE_EXCEPTION) {
                    error = IEventHandler::ERROR_IO;
                    exception = TRUE;
                } else */if (eResult == E_IO_EXCEPTION){
                    error = IEventHandler::ERROR_IO;
                    exception = TRUE;
                } else if (eResult == E_ILLEGAL_ARGUMENT_EXCEPTION) {
                    error = IEventHandler::ERROR_IO;
                    exception = TRUE;
                }

                if (exception) {
                    if (HttpFailure(req, error, exception) &&
                        !req->mCancelled) {
                        /* retry request if not permanent failure
                           or cancelled */
                        req->Reset();
                        pipe.PushBack(req);
                    }
                    exception = NULL;
                    mCanPersist = FALSE;
                }
                if (!mCanPersist) {
                    // if (HttpLog.LOGV) HttpLog.v(
                    //         "processRequests(): no persist, closing " +
                    //         mHost);

                    CloseConnection();

                    // TODO:
                    // mHttpContext->RemoveAttribute(HTTP_CONNECTION);
                    ClearPipe(pipe);
                    minPipe = maxPipe = 1;
                    state = SEND;
                }
                break;
            }
        }
    }
}

Boolean Connection::ClearPipe(
        /* [in] */ List<AutoPtr<Request> >& pipe)
{
    Boolean empty = TRUE;
    // if (HttpLog.LOGV) HttpLog.v(
    //         "Connection.clearPipe(): clearing pipe " + pipe.size());
    // synchronized (mRequestFeeder) {
        Request* tReq;

        List<Request*>::Iterator itor;
        for (itor = pipe.Begin(); itor != pipe.End(); itor++) {
            tReq = *itor;
            // if (HttpLog.LOGV) HttpLog.v(
            //         "clearPipe() adding back " + mHost + " " + tReq);
            mRequestFeeder->RequeueRequest(tReq);
            empty = FALSE;
        }
        if (empty) {
            empty = mRequestFeeder->HaveRequest(mHost, &empty);
            empty = !empty;
        }
    // }
    return empty;
}

Boolean Connection::OpenHttpConnection(
        /* [in] */ Request* req)
{
    // TODO:
    Int64 now/* = SystemClock::GetUptimeMillis()*/;
    Int32 error = IEventHandler::OK;
    Boolean exception = FALSE;


    // reset the certificate to null before opening a connection
    mCertificate = NULL;
    ECode eResult = OpenConnection(req, (IElastosHttpClientConnection**)&mHttpClientConnection);
    if (mHttpClientConnection != NULL) {
        // TODO:
        // mHttpClientConnection->SetSocketTimeout(SOCKET_TIMEOUT);
        // mHttpContext->SetAttribute(HTTP_CONNECTION, mHttpClientConnection);
    } else {
        // we tried to do SSL tunneling, failed,
        // and need to drop the request;
        // we have already informed the handler
        req->mFailCount = RETRY_REQUEST_LIMIT;
        return FALSE;
    }

    /*if (eResult == E_UNKNOWN_HOST_EXCEPTION) {
        // if (HttpLog.LOGV) HttpLog.v("Failed to open connection");
        error = IEventHandler::ERROR_LOOKUP;
        exception = TRUE;
    } else*/ if (eResult == E_ILLEGAL_ARGUMENT_EXCEPTION) {
        // if (HttpLog.LOGV) HttpLog.v("Illegal argument exception");
        error = IEventHandler::ERROR_CONNECT;
        req->mFailCount = RETRY_REQUEST_LIMIT;
        exception = TRUE;
    }/* else if (eResult == E_SSL_CONNECTION_CLOSED_BY_USER_ECXEPTION) {
        // hack: if we have an SSL connection failure,
        // we don't want to reconnect
        req->mFailCount = RETRY_REQUEST_LIMIT;
        // no error message
        return FALSE;
    }else if (eResult == E_SSL_HANDSHAKE_EXCEPTION) {
        // hack: if we have an SSL connection failure,
        // we don't want to reconnect
        req->mFailCount = RETRY_REQUEST_LIMIT;
        // if (HttpLog.LOGV) HttpLog.v(
        //         "SSL exception performing handshake");
        error = IEventHandler::ERROR_FAILED_SSL_HANDSHAKE;
        exception = TRUE;
    }*/  else if (eResult == E_IO_EXCEPTION) {
        error = IEventHandler::ERROR_CONNECT;
        exception = TRUE;
    }

    // if (HttpLog.LOGV) {
    //     long now2 = SystemClock.uptimeMillis();
    //     HttpLog.v("Connection.openHttpConnection() " +
    //               (now2 - now) + " " + mHost);
    // }

    if (error == IEventHandler::OK) {
        return TRUE;
    } else {
        if (req->mFailCount < RETRY_REQUEST_LIMIT) {
            // requeue
            mRequestFeeder->RequeueRequest(req);
            req->mFailCount++;
        } else {
            HttpFailure(req, error, exception);
        }
        return error == IEventHandler::OK;
    }
}

Boolean Connection::HttpFailure(
        /* [in] */ Request* req,
        /* [in] */ Int32 errorId,
        /* [in] */ Boolean isException)
{
    Boolean ret = TRUE;

    // if (HttpLog.LOGV) HttpLog.v(
    //         "httpFailure() ******* " + e + " count " + req.mFailCount +
    //         " " + mHost + " " + req.getUri());

    if (++req->mFailCount >= RETRY_REQUEST_LIMIT) {
        ret = FALSE;
        String error;
        if (errorId < 0) {
            AutoPtr<IErrorStrings> errorStrs;
            CErrorStrings::AcquireSingleton((IErrorStrings**)&errorStrs);
            errorStrs->GetString(errorId, mContext, &error);
        } else {
            // Throwable cause = e.getCause();
            // error = cause != null ? cause.toString() : e.getMessage();
        }
        req->mEventHandler->Error(errorId, error);
        req->Complete();
    }

    CloseConnection();
    // TODO:
    // mHttpContext->RemoveAttribute(HTTP_CONNECTION);

    return ret;
}

ECode Connection::GetHttpContext(
    /* [out] */ IHttpContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mHttpContext;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

Boolean Connection::KeepAlive(
    /* [in] */ IHttpEntity* entity,
    /* [in] */ IProtocolVersion* ver,
    /* [in] */ Int32 connType,
    /* [in] */ IHttpContext* context)
{
    AutoPtr<IHttpConnection> conn;
    // TODO:
    // context->GetAttribute(IExecutionContext::HTTP_CONNECTION, (IHttpConnection**)&conn);

    if (conn != NULL/* && !conn->IsOpen()*/) return FALSE;
    // do NOT check for stale connection, that is an expensive operation

    if (entity != NULL) {
        // if (entity->GetContentLength() < 0) {
        //     if (!entity->IsChunked() || ver->LessEquals(IHttpVersion::HTTP_1_0)) {
        //         // if the content length is not known and is not chunk
        //         // encoded, the connection cannot be reused
        //         return FALSE;
        //     }
        // }
    }
    // Check for 'Connection' directive
    if (connType == IHeaders::CONN_CLOSE) {
        return FALSE;
    } else if (connType == IHeaders::CONN_KEEP_ALIVE) {
        return TRUE;
    }
    // Resorting to protocol version default close connection policy
    // return !ver->LessEquals(IHttpVersion::HTTP_1_0);
    return FALSE;
}

ECode Connection::SetCanPersist(
    /* [in] */ IHttpEntity* entity,
    /* [in] */ IProtocolVersion* ver,
    /* [in] */ Int32 connType)
{
    mCanPersist = KeepAlive(entity, ver, connType, mHttpContext);
    return NOERROR;
}

ECode Connection::SetCanPersist(
    /* [in] */ Boolean canPersist)
{
    mCanPersist = canPersist;
    return NOERROR;
}

ECode Connection::GetCanPersist(
    /* [out] */ Boolean* canPersist)
{
    VALIDATE_NOT_NULL(canPersist);
    *canPersist = mCanPersist;
    return NOERROR;
}

ECode Connection::ToString(
        /* [out] */ String* str)
{
    // return mHost->ToString(str);
    return E_NOT_IMPLEMENTED;
}

ECode Connection::GetBuf(
    /* [out, callee] */ ArrayOf<Byte>** buf)
{
    if (mBuf == NULL) {
        mBuf = ArrayOf<Byte>::Alloc(8192);
    }

    *buf = mBuf;
    REFCOUNT_ADD(*buf);
    return NOERROR;
}

}
}
}
}
