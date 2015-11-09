
#include "elastos/droid/net/http/RequestQueue.h"

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Net::IProxy;
using Elastos::Droid::Net::IWebAddress;
using Elastos::Droid::Utility::ILog;
using Elastos::IO::IInputStream;
using Elastos::Utility::IIterator;
using Elastos::Utility::ILinkedHashMap;
using Elastos::Utility::ILinkedList;
using Elastos::Utility::IListIterator;
using Elastos::Utility::IMap;
using Org::Apache::Http::IHttpHost;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(RequestQueue, Object, IRequestQueue, IRequestFeeder)

const Int32 RequestQueue::CONNECTION_COUNT = 4;

ECode RequestQueue::constructor(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this(context, CONNECTION_COUNT);
#endif
}

ECode RequestQueue::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 connectionCount)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mContext = context;

    mPending.Resize(32);

    mActivePool = new ActivePool(connectionCount, this);
    mActivePool->Startup();

    context->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&mConnectivityManager);

    return NOERROR;
#endif
}

ECode RequestQueue::EnablePlatformNotifications()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    if (HttpLog::LOGV) {
        HttpLog::V(String("RequestQueue.enablePlatformNotifications() network"));
    }

    if (mProxyChangeReceiver == NULL) {
        // mProxyChangeReceiver = new LocalBroadcastReceiver(this);
        AutoPtr<IIntentFilter> filter;
        // CIntentFilter::New(IProxy::PROXY_CHANGE_ACTION, (IIntentFilter**)&filter);
        AutoPtr<IIntent> intent;
        mContext->RegisterReceiver(mProxyChangeReceiver, filter, (IIntent**)&intent);
    }
    // we need to resample the current proxy setup
    // SetProxyConfig();
    return NOERROR;
#endif
}

ECode RequestQueue::DisablePlatformNotifications()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    if (HttpLog::LOGV) {
        HttpLog::V(String("RequestQueue.disablePlatformNotifications() network"));
    }

    if (mProxyChangeReceiver != NULL) {
        mContext->UnregisterReceiver(mProxyChangeReceiver);
        mProxyChangeReceiver = NULL;
    }
    return NOERROR;
#endif
}

ECode RequestQueue::SetProxyConfig()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<INetworkInfo> info;
    // mConnectivityManager->GetActiveNetworkInfo((INetworkInfo**)&info);
    Int32 type;
    if (info != NULL /*&& (info.getType(&type), type) == IConnectivityManager::TYPE_WIFI*/) {
        mProxyHost = NULL;
    } else {
        String host;
        // Proxy::GetHost(mContext, &host);
        if (HttpLog::LOGV) {
            HttpLog::V(String("RequestQueue.setProxyConfig ") + host);
        }
        if (host.IsNullOrEmpty()) {
            mProxyHost = NULL;
        } else {
            mActivePool->DisablePersistence();
            Int32 port;
            // Proxy::GetPort(mContext, &port);
            // CHttpHost::New(host, port, String("http"), &mProxyHost);
        }
    }
    return NOERROR;
#endif
}

ECode RequestQueue::GetProxyHost(
    /* [out] */ IHttpHost** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(host);
    *host = mProxyHost;
    REFCOUNT_ADD(*host);
    return NOERROR;
#endif
}

ECode RequestQueue::QueueRequest(
    /* [in] */ String url,
    /* [in] */ String method,
    /* [in] */ IMap* headers,
    /* [in] */ IEventHandler* eventHandler,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ IRequestHandle** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<IWebAddress> uri;
    // TODO:
    // CWebAddress::New(url, (IWebAddress**)&uri);
    return QueueRequest(reqQueue, url, uri, method, headers, eventHandler,
        bodyProvider, bodyLength, handle);
#endif
}

ECode RequestQueue::QueueRequest(
    /* [in] */ String url,
    /* [in] */ IWebAddress* uri,
    /* [in] */ String method,
    /* [in] */ IMap* headers,
    /* [in] */ IEventHandler* eventHandler,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ IRequestHandle** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        String suri;
        // uri->ToString(&suri);
        HttpLog::V(String("RequestQueue.queueRequest ") + suri);
    }

    // Ensure there is an eventHandler set
    if (eventHandler == NULL) {
        CLoggingEventHandler::New((IEventHandler**)&eventHandler);
    }

    /* Create and queue request */
    Request* req;
    String host;
    // TODO:
    // uri->GetHost(&host);
    Int32 port;
    // uri->GetPort(&port);
    String scheme;
    // uri->GetScheme(&scheme);
    AutoPtr<IHttpHost> httpHost;
    // CHttpHost::New(host, port, scheme, (IHttpHost**)&httpHost);

    // set up request
    String uriPath;
    // uri->GetPath(&uriPath);
    req/* = new Request(method, httpHost, mProxyHost, uriPath, bodyProvider,
        bodyLength, eventHandler, headers)*/;

    QueueRequest(req, FALSE);

    mActivePool->mTotalRequest++;

    // dump();
    mActivePool->StartConnectionThread();

    AutoPtr<IObjectStringMap> headersMap;
    CObjectStringMap::New((IObjectStringMap**)&headersMap);
    if (headers != NULL) {
        HashMap<String, String>::Iterator it = headers->Begin();
        for (; it != headers->End(); ++it) {
            String key = it->mFirst;
            AutoPtr<ICharSequence> value;
            CString::New(it->mSecond, (ICharSequence**)&value);
            headersMap->Put(key, value);
        }
    }

    return CRequestHandle::New(reqQueue, url, uri, method, headersMap, bodyProvider,
        bodyLength, req, handle);
#endif
}

ECode RequestQueue::QueueSynchronousRequest(
    /* [in] */ String url,
    /* [in] */ IWebAddress* uri,
    /* [in] */ String method,
    /* [in] */ IMap* headers,
    /* [in] */ IEventHandler* eventHandler,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ IRequestHandle** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        String suri;
        // uri->ToString(&suri);
        HttpLog::V(String("RequestQueue.dispatchSynchronousRequest ") + suri);
    }

    AutoPtr<IHttpHost> host;
    String shost;
    // TODO:
    // uri->GetHost(&shost);
    Int32 port;
    // uri->GetPort(&port);
    String scheme;
    // uri->GetScheme(&scheme);
    // CHttpHost::New(shost, port, scheme, (IHttpHost**)&host);

    String uriPath;
    // uri->GetPath(&uriPath);
    // TODO:
    Request* req/* = new Request(method, host, mProxyHost, uriPath,
        bodyProvider, bodyLength, eventHandler, headers)*/;

    // Open a new connection that uses our special RequestFeeder
    // implementation.
    AutoPtr<IHttpHost> newHost;
    newHost = DetermineHost(host);
    AutoPtr<IConnection> conn/* = Connection::GetConnection(mContext, newHost, mProxyHost,
        new RequestQueue::SyncFeeder(this))*/;

    AutoPtr<IObjectStringMap> headersMap;
    CObjectStringMap::New((IObjectStringMap**)&headersMap);
    if (headers != NULL) {
        HashMap<String, String>::Iterator it = headers->Begin();
        for (; it != headers->End(); ++it) {
            String key = it->mFirst;
            AutoPtr<ICharSequence> value;
            CString::New(it->mSecond, (ICharSequence**)&value);
            headersMap->Put(key, value);
        }
    }

    // TODO: I would like to process the request here but LoadListener
    // needs a RequestHandle to process some messages.
    return CRequestHandle::New(reqQueue, url, uri, method, headersMap, bodyProvider,
            bodyLength, req, conn, handle);
#endif
}

ECode RequestQueue::DetermineHost(
    /* [in] */ IHttpHost* host,
    /* [out] */ IHttpHost** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // There used to be a comment in ConnectionThread about t-mob's proxy
    // being really bad about https. But, HttpsConnection actually looks
    // for a proxy and connects through it anyway. I think that this check
    // is still valid because if a site is https, we will use
    // HttpsConnection rather than HttpConnection if the proxy address is
    // not secure.
    String schemeName;
    // TODO:
    // host->GetSchemeName(&schemeName);
    if (mProxyHost == NULL || schemeName.Equals("https")) {
        return host;
    }
    return mProxyHost;
#endif
}

ECode RequestQueue::RequestsPending(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    return !mPending.IsEmpty();
#endif
}

ECode RequestQueue::Dump()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    HttpLog::V(String("dump()"));
    StringBuilder dump;
    Int32 count = 0;
    // Iterator<Map.Entry<HttpHost, LinkedList<Request>>> iter;

    // // mActivePool.log(dump);

    // if (!mPending.isEmpty()) {
    //     iter = mPending.entrySet().iterator();
    //     while (iter.hasNext()) {
    //         Map.Entry<HttpHost, LinkedList<Request>> entry = iter.next();
    //         String hostName = entry.getKey().getHostName();
    //         StringBuilder line = new StringBuilder("p" + count++ + " " + hostName + " ");

    //         LinkedList<Request> reqList = entry.getValue();
    //         ListIterator reqIter = reqList.listIterator(0);
    //         while (iter.hasNext()) {
    //             Request request = (Request)iter.next();
    //             line.append(request + " ");
    //         }
    //         dump.append(line);
    //         dump.append("\n");
    //     }
    // }
    HttpLog::V(dump->ToString());

    return NOERROR;
#endif
}

ECode RequestQueue::GetRequest(
    /* [out] */ IRequest** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(req);

    AutoLock lock(mLock);

    Request* ret = NULL;

    if (!mPending.IsEmpty()) {
        ret = RemoveFirst(mPending);
    }
    if (HttpLog::LOGV) {
        String sret;
        ret->ToString(&sret);
        HttpLog::V(String("RequestQueue.getRequest() => ") + sret);
    }
    *req = ret;
    return NOERROR;
#endif
}

ECode RequestQueue::GetRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ IRequest** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(req);

    AutoLock lock(mLock);

    Request* ret = NULL;

    HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >::Iterator it = mPending.Find(host);
    if (it != mPending.End()) {
        AutoPtr<RequestList> reqList = &(it->mSecond);
        // TODO:
        // ret = reqList->RemoveFirst();
        if (reqList->IsEmpty()) {
            // mPending.Erase(it);
        }
    }
    if (HttpLog::LOGV) {
        String shost;
        // host->ToString(&shost);
        String sret;
        // ret->ToString(&sret);
        HttpLog::V(String("RequestQueue.getRequest(") + shost + String(") => ") + sret);
    }
    *req = ret;
    return NOERROR;
#endif
}

ECode RequestQueue::HaveRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(result);

    AutoLock lock(mLock);

    HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >::Iterator it;// = mPending.Find(host);
    *result = it != mPending.End();
    return NOERROR;
#endif
}

ECode RequestQueue::RequeueRequest(
    /* [in] */ IRequest* request)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return QueueRequest(request, TRUE);
#endif
}

ECode RequestQueue::Shutdown()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return mActivePool->Shutdown();
#endif
}

ECode RequestQueue::QueueRequest(
    /* [in] */ IRequest* request,
    /* [in] */ Boolean head)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoLock lock(mLock);

    AutoPtr<IHttpHost> host = request->mProxyHost == NULL
        ? request->mHost : request->mProxyHost;
    AutoPtr<RequestList> reqList;
    HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >::Iterator it;// = mPending.Find(host);
    if (it != mPending.End()) {
        reqList = &(it->mSecond);
    } else {
        reqList = new List<Request*>();
        // TODO:
        // mPending.Put(host, reqList);
    }
    if (head) {
        reqList->Insert(0, request);
    } else {
        reqList->PushBack(request);
    }

    return NOERROR;
#endif
}

ECode RequestQueue::StartTiming()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return mActivePool->StartTiming();
#endif
}

ECode RequestQueue::StopTiming()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return mActivePool->StopTiming();
#endif
}

ECode RequestQueue::RemoveFirst(
    /* [in] */ IHashMap* requestQueue,
    /* [out] */ IRequest** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<Request> ret;
    HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >::Iterator it;// = requestQueue->Begin();

    // for (; it != requestQueue->End(); ++it) {
    //     AutoPtr<RequestList> reqList = &(it->mSecond);
    //     // TODO:
    //     // ret = reqList->RemoveFirst();
    //     if (reqList->IsEmpty()) {
    //         requestQueue->Erase(it);
    //     }
    // }

    return ret;
#endif
}

//============================================================
// RequestQueue::ActivePool
//============================================================
ECode RequestQueue::ActivePool::ActivePool(
    /* [in] */ Int32 connectionCount)
    : mTotalRequest(0)
    , mTotalConnection(0)
    , mConnectionCount(connectionCount)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mIdleCache = new IdleCache();
    mThreads = ArrayOf<ConnectionThread*>::Alloc(mConnectionCount);
    for (Int32 i = 0; i < mConnectionCount; i++) {
        (*mThreads)[i] = new ConnectionThread(
                mParent->mContext, i, this, mParent);
    }
#endif
}

ECode RequestQueue::ActivePool::Startup()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ((*mThreads)[i])->Start();
    }

    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::Shutdown()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ((*mThreads)[i])->RequestStop();
    }

    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::StartConnectionThread()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized (RequestQueue.this) {
                    RequestQueue.this.notify();
                }

#endif
}

ECode RequestQueue::ActivePool::StartTiming()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ConnectionThread* rt = (*mThreads)[i];
        rt->mCurrentThreadTime = -1;
        rt->mTotalThreadTime = 0;
    }
    mTotalRequest = 0;
    mTotalConnection = 0;

    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::StopTiming()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    Int32 totalTime = 0;
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ConnectionThread* rt = (*mThreads)[i];
        if (rt->mCurrentThreadTime != -1) {
            totalTime += rt->mTotalThreadTime;
        }
        rt->mCurrentThreadTime = 0;
    }
    Logger::D(String("Http"), String("Http thread used ") + StringUtils::Int32ToString(totalTime) + String(" ms ")
        + String(" for ") + StringUtils::Int32ToString(mTotalRequest) + String(" requests and ")
        + StringUtils::Int32ToString(mTotalConnection) + String(" new connections"));

    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::LogState()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<StringBuilder> dump = new StringBuilder();
    for (Int32 i = 0; i < mConnectionCount; i++) {
        String str;
        ((*mThreads)[i])->ToString(&str);
        dump->AppendString(str + String("\n"));
    }
    HttpLog::V(dump->ToString());

    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::GetProxyHost(
    /* [out] */ IHttpHost** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(host);
    *host = mParent->mProxyHost;
    REFCOUNT_ADD(*host);
    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::DisablePersistence()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    for (Int32 i = 0; i < mConnectionCount; i++) {
        Connection* connection = ((*mThreads)[i])->mConnection;
        if (connection != NULL) {
            // connection->SetCanPersist(false);
        }
    }
    mIdleCache->Clear();

    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::GetThread(
    /* [in] */ IHttpHost* host,
    /* [out] */ IConnectionThread** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(thread);

    AutoLock lock(mLock);

    for (Int32 i = 0; i < mThreads->GetLength(); i++) {
        ConnectionThread* ct = (*mThreads)[i];
        Connection* connection = ct->mConnection;
        // TODO:
        // if (connection != NULL && connection->mHost->Equals(host)) {
        //     *thread = ct;
        //     REFCOUNT_ADD(*thread);
        //     return NOERROR;
        // }
    }

    *thread = NULL;
    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::GetConnection(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [out] */ IConnection** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(conn);

    // host = mParent->DetermineHost(host);
    Connection* con;
    // con = mIdleCache->GetConnection(host);
    if (con == NULL) {
        mTotalConnection++;
        // con = Connection::GetConnection(mParent->mContext, host, mParent->mProxyHost, mParent);
    }
    *conn = con;
    REFCOUNT_ADD(*conn);
    return NOERROR;
#endif
}

ECode RequestQueue::ActivePool::RecycleConnection(
    /* [in] */ IConnection* connection,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return mIdleCache.cacheConnection(connection.getHost(), connection);

#endif
}

//===============================================
// RequestQueue::SyncFeeder
//===============================================
ECode RequestQueue::SyncFeeder::GetRequest(
    /* [out] */ Request** req)
{
    VALIDATE_NOT_NULL(*req);

    AutoPtr<Request> r = mRequest;
    mRequest = NULL;
    *req = r;
    REFCOUNT_ADD(*req);
    return NOERROR;
}

ECode RequestQueue::SyncFeeder::GetRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ Request** req)
{
    VALIDATE_NOT_NULL(*req)
    *req = NULL;
    VALIDATE_NOT_NULL(host)

    return GetRequest(req);
}

ECode RequestQueue::SyncFeeder::HaveRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRequest != NULL;
    VALIDATE_NOT_NULL(host)

    return NOERROR;
}

ECode RequestQueue::SyncFeeder::RequeueRequest(
    /* [in] */ Request* request)
{
    VALIDATE_NOT_NULL(request)

    mRequest = request;
    return NOERROR;
}

ECode RequestQueue::InnerSub_BroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return SetProxyConfig();
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
