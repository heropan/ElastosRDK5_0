
#include "RequestQueue.h"
#include "ConnectionThread.h"
#include "HttpLog.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::Net::INetworkInfo;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

RequestQueue::ActivePool::ActivePool(
        /* [in] */ Int32 connectionCount,
        /* [in] */ RequestQueue* parent)
    : mIdleCache(NULL)
    , mTotalRequest(0)
    , mTotalConnection(0)
    , mConnectionCount(connectionCount)
    , mParent(parent)
{
    mIdleCache = new IdleCache();
    mThreads = ArrayOf<ConnectionThread*>::Alloc(mConnectionCount);
    for (Int32 i = 0; i < mConnectionCount; i++) {
        (*mThreads)[i] = new ConnectionThread(
                mParent->mContext, i, this, mParent);
    }
}

RequestQueue::ActivePool::~ActivePool()
{}

ECode RequestQueue::ActivePool::Startup()
{
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ((*mThreads)[i])->Start();
    }

    return NOERROR;
}

ECode RequestQueue::ActivePool::Shutdown()
{
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ((*mThreads)[i])->RequestStop();
    }

    return NOERROR;
}

ECode RequestQueue::ActivePool::StartConnectionThread()
{
    AutoLock lock(mLock);
    // return mParent->Notify();
    return E_NOT_IMPLEMENTED;
}

ECode RequestQueue::ActivePool::StartTiming()
{
    for (Int32 i = 0; i < mConnectionCount; i++) {
        ConnectionThread* rt = (*mThreads)[i];
        rt->mCurrentThreadTime = -1;
        rt->mTotalThreadTime = 0;
    }
    mTotalRequest = 0;
    mTotalConnection = 0;

    return NOERROR;
}

ECode RequestQueue::ActivePool::StopTiming() {
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
}

ECode RequestQueue::ActivePool::LogState() {
    AutoPtr<StringBuilder> dump = new StringBuilder();
    for (Int32 i = 0; i < mConnectionCount; i++) {
        String str;
        ((*mThreads)[i])->ToString(&str);
        dump->AppendString(str + String("\n"));
    }
    HttpLog::V(dump->ToString());

    return NOERROR;
}

ECode RequestQueue::ActivePool::GetProxyHost(
    /* [out] */ IHttpHost** host)
{
    VALIDATE_NOT_NULL(host);
    *host = mParent->mProxyHost;
    REFCOUNT_ADD(*host);
    return NOERROR;
}

ECode RequestQueue::ActivePool::GetThread(
    /* [in] */ IHttpHost* host,
    /* [out] */ ConnectionThread** thread)
{
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
}

ECode RequestQueue::ActivePool::DisablePersistence()
{
    for (Int32 i = 0; i < mConnectionCount; i++) {
        Connection* connection = ((*mThreads)[i])->mConnection;
        if (connection != NULL) {
            // connection->SetCanPersist(false);
        }
    }
    mIdleCache->Clear();

    return NOERROR;
}

ECode RequestQueue::ActivePool::GetConnection(
    /* [in] */ IContext* context,
    /* [in] */ IHttpHost* host,
    /* [out] */ Connection** conn)
{
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
}

ECode RequestQueue::ActivePool::RecycleConnection(
    /* [in] */ Connection* connection,
    /* [in] */ Boolean* result)
{
    // return mIdleCache->CacheConnection(connection->GetHost(), connection, result);
    return E_NOT_IMPLEMENTED;
}

RequestQueue::LocalBroadcastReceiver::LocalBroadcastReceiver(
    /* [in] */ RequestQueue* parent)
    : mParent(parent)
{}

ECode RequestQueue::LocalBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return mParent->SetProxyConfig();
}

RequestQueue::SyncFeeder::SyncFeeder(
    /* [in] */ RequestQueue* parent)
    : mRequest(NULL)
    , mParent(parent)
{}

ECode RequestQueue::SyncFeeder::GetRequest(
    /* [out] */ Request** req)
{
    VALIDATE_NOT_NULL(req);

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
    return GetRequest(req);
}

ECode RequestQueue::SyncFeeder::HaveRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRequest != NULL;
    return NOERROR;
}

ECode RequestQueue::SyncFeeder::RequeueRequest(
    /* [in] */ Request* request)
{
    mRequest = request;
    return NOERROR;
}

RequestQueue::RequestQueue()
    : mActivePool(NULL)
{}

RequestQueue::~RequestQueue()
{}

ECode RequestQueue::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 connectionCount)
{
    mContext = context;

    mPending.Resize(32);

    mActivePool = new ActivePool(connectionCount, this);
    mActivePool->Startup();

    context->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&mConnectivityManager);

    return NOERROR;
}

ECode RequestQueue::EnablePlatformNotifications()
{
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
}

ECode RequestQueue::DisablePlatformNotifications()
{
    AutoLock lock(mLock);

    if (HttpLog::LOGV) {
        HttpLog::V(String("RequestQueue.disablePlatformNotifications() network"));
    }

    if (mProxyChangeReceiver != NULL) {
        mContext->UnregisterReceiver(mProxyChangeReceiver);
        mProxyChangeReceiver = NULL;
    }
    return NOERROR;
}

ECode RequestQueue::SetProxyConfig()
{
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
}

ECode RequestQueue::GetProxyHost(
    /* [out] */ Org::Apache::Http::IHttpHost** host)
{
    VALIDATE_NOT_NULL(host);
    *host = mProxyHost;
    REFCOUNT_ADD(*host);
    return NOERROR;
}

ECode RequestQueue::QueueRequest(
    /* [in] */ IRequestQueue* reqQueue,
    /* [in] */ const String& url,
    /* [in] */ const String& method,
    /* [in] */ HashMap<String, String>* headers,
    /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle)
{
    AutoPtr<IWebAddress> uri;
    // TODO:
    // CWebAddress::New(url, (IWebAddress**)&uri);
    return QueueRequest(reqQueue, url, uri, method, headers, eventHandler,
        bodyProvider, bodyLength, handle);
}

ECode RequestQueue::QueueRequest(
    /* [in] */ IRequestQueue* reqQueue,
    /* [in] */ const String& url,
    /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ HashMap<String, String>* headers,
    /* [in] */ IEventHandler* eventHandler,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ IRequestHandle** handle)
{
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
            CStringWrapper::New(it->mSecond, (ICharSequence**)&value);
            headersMap->Put(key, value);
        }
    }

    return CRequestHandle::New(reqQueue, url, uri, method, headersMap, bodyProvider,
        bodyLength, req, handle);
}

ECode RequestQueue::QueueSynchronousRequest(
    /* [in] */ IRequestQueue* reqQueue,
    /* [in] */ const String& url,
    /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ HashMap<String, String>* headers,
    /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle)
{
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
            CStringWrapper::New(it->mSecond, (ICharSequence**)&value);
            headersMap->Put(key, value);
        }
    }

    // TODO: I would like to process the request here but LoadListener
    // needs a RequestHandle to process some messages.
    return CRequestHandle::New(reqQueue, url, uri, method, headersMap, bodyProvider,
            bodyLength, req, conn, handle);
}

IHttpHost* RequestQueue::DetermineHost(
    /* [in] */ IHttpHost* host)
{
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
}

Boolean RequestQueue::RequestsPending()
{
    AutoLock lock(mLock);

    return !mPending.IsEmpty();
}

ECode RequestQueue::Dump()
{
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
}

ECode RequestQueue::GetRequest(
         /* [out] */ Request** req)
{
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
}

ECode RequestQueue::GetRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ Request** req)
{
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
}

ECode RequestQueue::HaveRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoLock lock(mLock);

    HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >::Iterator it;// = mPending.Find(host);
    *result = it != mPending.End();
    return NOERROR;
}

ECode RequestQueue::RequeueRequest(
    /* [in] */ Request* request)
{
    return QueueRequest(request, TRUE);
}

ECode RequestQueue::Shutdown()
{
    return mActivePool->Shutdown();
}

CARAPI RequestQueue::QueueRequest(
    /* [in] */ Request* request,
    /* [in] */ Boolean head)
{
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
}

ECode RequestQueue::StartTiming()
{
    return mActivePool->StartTiming();
}

ECode RequestQueue::StopTiming()
{
    return mActivePool->StopTiming();
}

AutoPtr<Request> RequestQueue::RemoveFirst(
    /* [in] */ HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >* requestQueue)
{
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
}

}
}
}
}
