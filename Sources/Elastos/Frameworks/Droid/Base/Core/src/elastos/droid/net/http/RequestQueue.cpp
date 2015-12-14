
#include "Elastos.CoreLibrary.Apache.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "elastos/droid/net/http/RequestQueue.h"
// #include "elastos/droid/net/GetValue.h"
#include "elastos/droid/net/http/HttpLog.h"

using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::INetworkInfo;
// using Elastos::Droid::Net::IProxy;
using Elastos::Droid::Net::IWebAddress;
using Elastos::Droid::Utility::ILog;

using Elastos::Utility::IIterator;
using Elastos::Utility::ILinkedList;
using Elastos::Utility::IListIterator;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

//============================================================
// RequestQueue::ActivePool
//============================================================
CAR_INTERFACE_IMPL(RequestQueue::ActivePool, Object, IRequestQueueConnectionManager)

RequestQueue::ActivePool::ActivePool(
    /* [in] */ Int32 connectionCount,
    /* [in] */ RequestQueue* host)
    : mTotalRequest(0)
    , mTotalConnection(0)
    , mConnectionCount(connectionCount)
    , mHost(host)
{
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
    synchronized(mHost) {
        mHost->Notify();
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
    /* [out] */ ConnectionThread** thread)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(thread);

    synchronized(mHost) {
        for (Int32 i = 0; i < mThreads->GetLength(); i++) {
            ConnectionThread* ct = (*mThreads)[i];
            Connection* connection = ct->mConnection;
            if (connection != NULL && connection->mHost->Equals(host)) {
                *thread = ct;
                REFCOUNT_ADD(*thread);
                return NOERROR;
            }
        }
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

    host = mParent->DetermineHost(host);
    Connection* con;
    con = mIdleCache->GetConnection(host);
    if (con == NULL) {
        mTotalConnection++;
        con = Connection::GetConnection(mParent->mContext, host, mParent->mProxyHost, mParent);
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
    VALIDATE_NOT_NULL(result)

    return mIdleCache->CacheConnection(connection.getHost(), connection, result);
#endif
}

//===============================================
// RequestQueue::SyncFeeder
//===============================================
ECode RequestQueue::SyncFeeder::GetRequest(
    /* [out] */ IRequest** req)
{
    VALIDATE_NOT_NULL(req);

    AutoPtr<IRequest> r = mRequest;
    mRequest = NULL;
    *req = r;
    REFCOUNT_ADD(*req);
    return NOERROR;
}

ECode RequestQueue::SyncFeeder::GetRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ IRequest** req)
{
    VALIDATE_NOT_NULL(req)

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
    /* [in] */ IRequest* request)
{
    VALIDATE_NOT_NULL(request)

    mRequest = request;
    return NOERROR;
}

//===============================================
// RequestQueue::InnerSub_BroadcastReceiver
//===============================================
ECode RequestQueue::InnerSub_BroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return mHost->SetProxyConfig();
}

RequestQueue::InnerSub_BroadcastReceiver::InnerSub_BroadcastReceiver(
    /* [in] */ RequestQueue* host)
    : mHost(host)
{}

//===============================================
// RequestQueue
//===============================================
const Int32 RequestQueue::CONNECTION_COUNT = 4;

CAR_INTERFACE_IMPL_2(RequestQueue, Object, IRequestQueue, IRequestFeeder)

ECode RequestQueue::constructor(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    return constructor(context, CONNECTION_COUNT);
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
        mProxyChangeReceiver = new LocalBroadcastReceiver(this);
        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New(IProxy::PROXY_CHANGE_ACTION, (IIntentFilter**)&filter);
        AutoPtr<IIntent> intent;
        mContext->RegisterReceiver(mProxyChangeReceiver, filter, (IIntent**)&intent);
    }
    // we need to resample the current proxy setup
    SetProxyConfig();
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
    mConnectivityManager->GetActiveNetworkInfo((INetworkInfo**)&info);
    Int32 type;
    if (info != NULL /*&& (info.getType(&type), type) == IConnectivityManager::TYPE_WIFI*/) {
        mProxyHost = NULL;
    } else {
        String host;
        Proxy::GetHost(mContext, &host);
        if (HttpLog::LOGV) {
            HttpLog::V(String("RequestQueue.setProxyConfig ") + host);
        }
        if (host.IsNullOrEmpty()) {
            mProxyHost = NULL;
        } else {
            mActivePool->DisablePersistence();
            Int32 port;
            Proxy::GetPort(mContext, &port);
            CHttpHost::New(host, port, String("http"), &mProxyHost);
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
    /* [in] */ const String& url,
    /* [in] */ const String& method,
    /* [in] */ IMap* headers,
    /* [in] */ IEventHandler* eventHandler,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ IRequestHandle** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    AutoPtr<IWebAddress> uri;
    CWebAddress::New(url, (IWebAddress**)&uri);
    return QueueRequest(reqQueue, url, uri, method, headers, eventHandler,
        bodyProvider, bodyLength, handle);
#endif
}

ECode RequestQueue::QueueRequest(
    /* [in] */ const String& url,
    /* [in] */ IWebAddress* uri,
    /* [in] */ const String& method,
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
    uri->GetHost(&host);
    Int32 port;
    uri->GetPort(&port);
    String scheme;
    uri->GetScheme(&scheme);
    AutoPtr<IHttpHost> httpHost;
    // CHttpHost::New(host, port, scheme, (IHttpHost**)&httpHost);

    // set up request
    String uriPath;
    uri->GetPath(&uriPath);
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
    /* [in] */ const String& url,
    /* [in] */ IWebAddress* uri,
    /* [in] */ const String& method,
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
        uri->ToString(&suri);
        HttpLog::V(String("RequestQueue.dispatchSynchronousRequest ") + suri);
    }

    AutoPtr<IHttpHost> host;
    String shost;
    uri->GetHost(&shost);
    Int32 port;
    uri->GetPort(&port);
    String scheme;
    uri->GetScheme(&scheme);
    CHttpHost::New(shost, port, scheme, (IHttpHost**)&host);

    String uriPath;
    uri->GetPath(&uriPath);
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

AutoPtr<IHttpHost> RequestQueue::DetermineHost(
    /* [in] */ IHttpHost* host)
{
    return host;
#if 0 // TODO: Translate codes below
    // There used to be a comment in ConnectionThread about t-mob's proxy
    // being really bad about https. But, HttpsConnection actually looks
    // for a proxy and connects through it anyway. I think that this check
    // is still valid because if a site is https, we will use
    // HttpsConnection rather than HttpConnection if the proxy address is
    // not secure.
    String schemeName;
    host->GetSchemeName(&schemeName);
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
    synchronized(this) {
        *result = !mPending.IsEmpty();
    }
    return NOERROR;
#endif
}

ECode RequestQueue::Dump()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    synchronized(this) {
        HttpLog::V(String("dump()"));
        StringBuilder dump;
        Int32 count = 0;
        AutoPtr<IIterator> iter;

        // mActivePool.log(dump);
        Boolean isEmpty;
        if (IMap::Probe(mPending)->IsEmpty(&isEmpty), !isEmpty) {
            AutoPtr<ISet> entrySet;
            mPending->GetEntrySet((ISet**)&entrySet);
            entrySet->GetIterator((IIterator**)&iter);
            Boolean hasNext;
            while (iter->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                iter->GetNext((IInterface**)&obj);
                AutoPtr<IMapEntry> entry = IMapEntry::Probe(obj);
                obj = NULL;
                entry->GetKey((IInterface**)&obj);
                String hostName;
                IHttpHost::Probe(obj)->GetHostName(&hostName);
                StringBuilder line("p");
                line.Append(count++);
                line.Append(" ");
                line.Append(hostName);
                line.Append(" ");
                AutoPtr<ILinkedList> reqList;
                obj = NULL;
                entry->GetValue((IInterface**)&obj);
                AutoPtr<IListIterator> reqIter;
                ILinkedList::Probe(obj)->GetListIterator(0, (IListIterator**)&regIter);
                    // while (iter.hasNext()) {
                while (regIter->HasNext(&hasNext), hasNext) {
                        // Request request = (Request)iter.next();
                    obj = NULL;
                    regIter->GetNext((IInterface**)&obj);
                    String s;
                    IObject::Probe(obj)->ToString(&s);
                    line.Append(s + " ");
                }
                dump += line;
                dump.Append("\n");
            }
        }
        String s;
        dump->ToString(&s);
        HttpLog::V(s);
    }

    return NOERROR;
#endif
}

ECode RequestQueue::GetRequest(
    /* [out] */ IRequest** req)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(req);

    synchronized(this) {

        AutoPtr<IRequest> ret;

        if (!GetValue(mPending, mPending->IsEmpty)) {
            ret = RemoveFirst(mPending);
        }
        if (HttpLog::LOGV) {
            HttpLog::V("RequestQueue.getRequest() => %s", GetValue(IObject::Probe(ret), IObject::ToString).string());
        }
        *req = ret;
    }
    return NOERROR;
#endif
}

ECode RequestQueue::GetRequest(
    /* [in] */ IHttpHost* host,
    /* [out] */ IRequest** req)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(req);

    synchronized(this) {
        AutoPtr<IRequest> ret;

        if (GetValue(mPending, mPending->ContainsKey, IInterface::Probe(host))) {
            AutoPtr<IInterface> obj;
            mPending->Get(IInterface::Probe(host), (IInterface**)&obj);
            AutoPtr<ILinkedList> reqList = ILinkedList::Probe(obj);
            obj = NULL;
            reqList->RemoveFirst((IInterface**)&obj);
            ret = IRequest::Probe(obj);
            if (GetValue(reqList, reqList->IsEmpty) {
                mPending->Remove(IInterface::Probe(host));
            }
        }
        if (HttpLog::LOGV) {
            String shost;
            // host->ToString(&shost);
            String sret;
            // ret->ToString(&sret);
            HttpLog::V("RequestQueue.getRequest(%s) => %s",
                    GetValue(IObject::Probe(host), IObject::ToString).string(),
                    GetValue(IObject::Probe(ret), IObject::ToString).string());
        }
        *req = ret;
        REFCOUNT_ADD(*req)
    }
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

    synchronized(this) {
        mPending->ContainsKey(IInterface::Probe(host), result);
    }
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
    synchronized(this) {
        AutoPtr<IHttpHost> host = request->mProxyHost == NULL
            ? request->mHost : request->mProxyHost;
        AutoPtr<IRequestList> reqList;
        if (GetValue(mPending, mPending->ContainsKey, IInterface::Probe(host))) {
            reqList = IRequestList::Probe(GetValue(mPending, mPending->Get, IInterface::Probe(host)));
        } else {
            CLinkedList::New((IRequestList**)&reqList);
            mPending->Put(IInterface::Probe(host), IInterface::Probe(reqList));
        }
        if (head) {
            reqList->AddFirst(IInterface::Probe(request));
        } else {
            reqList->Add(IInterface::Probe(request));
        }
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

AutoPtr<IRequest> RequestQueue::RemoveFirst(
    /* [in] */ IHashMap* requestQueue)
{
    AutoPtr<IRequest> ret;
#if 0 // TODO: Translate codes below
    AutoPtr<IIterator> iter;
    GetValue(requestQueue, requestQueue->GetEntrySet)->GetIterator((IIterator**)&iter);
    if (GetValue(iter, iter->HasNext)) {
        AutoPtr<IMapEntry> entry;
        iter->GetNext((IMapEntry**)&entry);
        AutoPtr<IRequestList> reqList = IRequestList::Probe(GetValue(entry, entry->GetValue));
        reqList->RemoveFirst((IRequest)&ret);
        if(GetValue(reqList, reqList->IsEmpty)) {
            requestQueue->Remove(GetValue(entry, entry->GetKey));
        }
    }
#endif
    return ret;
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
