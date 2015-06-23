
#include "CRequestQueue.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const Int32 CRequestQueue::CONNECTION_COUNT = 4;

ECode CRequestQueue::constructor(
    /* [in] */ Elastos::Droid::Content::IContext* context)
{
    return RequestQueue::Init(context, CONNECTION_COUNT);
}

ECode CRequestQueue::constructor(
    /* [in] */ Elastos::Droid::Content::IContext* context,
    /* [in] */ Int32 connectionCount)
{
    return RequestQueue::Init(context, connectionCount);
}

ECode CRequestQueue::EnablePlatformNotifications()
{
    return RequestQueue::EnablePlatformNotifications();
}

ECode CRequestQueue::DisablePlatformNotifications()
{
    return RequestQueue::DisablePlatformNotifications();
}

ECode CRequestQueue::GetProxyHost(
    /* [out] */ Org::Apache::Http::IHttpHost** host)
{
    return RequestQueue::GetProxyHost(host);
}

ECode CRequestQueue::QueueRequest(
    /* [in] */ const String& url,
    /* [in] */ const String& method,
    /* [in] */ IObjectStringMap* headers,
    /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle)
{
    HashMap<String, String> headersMap;
    if (headers != NULL) {
        AutoPtr<ArrayOf<String> > keyArray;
        AutoPtr<IObjectContainer> objContainer;
        headers->GetAllItems((ArrayOf<String>**)&keyArray, (IObjectContainer**)&objContainer);

        Int32 len = keyArray->GetLength();
        for (Int32 i = 0; i < len; i++) {
            String key = (*keyArray)[i];
            AutoPtr<ICharSequence> value;
            headers->Get(key, (IInterface**)&value);
            String svalue;
            value->ToString(&svalue);

            headersMap[key] = svalue;
        }
    }
    AutoPtr<IRequestQueue> reqQueue = (IRequestQueue*)this->Probe(EIID_IRequestQueue);
    return RequestQueue::QueueRequest(reqQueue, url, method, &headersMap, eventHandler,
        bodyProvider, bodyLength, handle);
}

ECode CRequestQueue::QueueRequest(
    /* [in] */ const String& url,
    /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ IObjectStringMap* headers,
    /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle)
{
    HashMap<String, String> headersMap;
    if (headers != NULL) {
        AutoPtr<ArrayOf<String> > keyArray;
        AutoPtr<IObjectContainer> objContainer;
        headers->GetAllItems((ArrayOf<String>**)&keyArray, (IObjectContainer**)&objContainer);

        Int32 len = keyArray->GetLength();
        for (Int32 i = 0; i < len; i++) {
            String key = (*keyArray)[i];
            AutoPtr<ICharSequence> value;
            headers->Get(key, (IInterface**)&value);
            String svalue;
            value->ToString(&svalue);

            headersMap[key] = svalue;
        }
    }
    AutoPtr<IRequestQueue> reqQueue = (IRequestQueue*)this->Probe(EIID_IRequestQueue);
    return RequestQueue::QueueRequest(reqQueue, url, uri, method, &headersMap, eventHandler,
        bodyProvider, bodyLength, handle);
}

ECode CRequestQueue::QueueSynchronousRequest(
    /* [in] */ const String& url,
    /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ IObjectStringMap* headers,
    /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle)
{
    HashMap<String, String> headersMap;
    if (headers != NULL) {
        AutoPtr<ArrayOf<String> > keyArray;
        AutoPtr<IObjectContainer> objContainer;
        headers->GetAllItems((ArrayOf<String>**)&keyArray, (IObjectContainer**)&objContainer);

        Int32 len = keyArray->GetLength();
        for (Int32 i = 0; i < len; i++) {
            String key = (*keyArray)[i];
            AutoPtr<ICharSequence> value;
            headers->Get(key, (IInterface**)&value);
            String svalue;
            value->ToString(&svalue);

            headersMap[key] = svalue;
        }
    }
    AutoPtr<IRequestQueue> reqQueue = (IRequestQueue*)this->Probe(EIID_IRequestQueue);
    return RequestQueue::QueueSynchronousRequest(reqQueue, url, uri, method, &headersMap, eventHandler,
        bodyProvider, bodyLength, handle);
}

ECode CRequestQueue::GetRequest(
    /* [out] */ IRequest** request)
{
    return RequestQueue::GetRequest((Request**)request);
}

ECode CRequestQueue::GetRequest(
    /* [in] */ Org::Apache::Http::IHttpHost* host,
    /* [out] */ IRequest** request)
{
    return RequestQueue::GetRequest(host, (Request**)request);
}

ECode CRequestQueue::HaveRequest(
    /* [in] */ Org::Apache::Http::IHttpHost* host,
    /* [out] */ Boolean* haveRequest)
{
    return RequestQueue::HaveRequest(host, haveRequest);
}

ECode CRequestQueue::RequeueRequest(
    /* [in] */ IRequest* request)
{
    return RequestQueue::RequeueRequest((Request*)request);
}

ECode CRequestQueue::Shutdown()
{
    return RequestQueue::Shutdown();
}

ECode CRequestQueue::StartTiming()
{
    return RequestQueue::StartTiming();
}

ECode CRequestQueue::StopTiming()
{
    return RequestQueue::StopTiming();
}

}
}
}
}

