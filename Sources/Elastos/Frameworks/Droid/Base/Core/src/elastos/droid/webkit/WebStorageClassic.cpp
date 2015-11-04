
#include "elastos/droid/os/CMessageHelper.h"
#include "elastos/droid/webkit/CWebViewCore.h"
#include "elastos/droid/webkit/WebStorageClassic.h"

#include <elastos/core/Thread.h>

using Elastos::Core::CInteger64;
using Elastos::Core::IInteger64;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Core::Thread;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Utility::CHashMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//              WebStorageClassic::InnerUIHandler
//===============================================================

WebStorageClassic::InnerUIHandler::InnerUIHandler(
    /* [in] */ WebStorageClassic* owner)
    : mOwner(owner)
{
}

ECode WebStorageClassic::InnerUIHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case RETURN_ORIGINS: {
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<IMap> origins;
            AutoPtr<ICharSequence> ORIGINSwrapper;
            CStringWrapper::New(ORIGINS, (ICharSequence**)&ORIGINSwrapper);
            values->Get(ORIGINSwrapper, (IInterface**)&origins);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKwrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
            values->Get(CALLBACKwrapper, (IInterface**)&callback);
            callback->OnReceiveValue(origins);
            } break;

        case RETURN_USAGE_ORIGIN: {
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKwrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
            values->Get(CALLBACKwrapper, (IInterface**)&callback);
            AutoPtr<IInteger64> iValue;
            AutoPtr<ICharSequence> USAGEWrapper;
            CStringWrapper::New(USAGE, (ICharSequence**)&USAGEWrapper);
            values->Get(USAGEWrapper, (IInterface**)&iValue);
            callback->OnReceiveValue(iValue);
            } break;

        case RETURN_QUOTA_ORIGIN: {
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKwrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
            values->Get(CALLBACKwrapper, (IInterface**)&callback);
            AutoPtr<ICharSequence> QUOTAwrapper;
            CStringWrapper::New(QUOTA, (ICharSequence**)&QUOTAwrapper);
            AutoPtr<IInteger64> iValue;
            values->Get(QUOTAwrapper, (IInterface**)&iValue);
            callback->OnReceiveValue(iValue);
            } break;
    }

    return NOERROR;
}

ECode WebStorageClassic::InnerUIHandler::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebStorageClassic::InnerUIHandler";
    return NOERROR;
}

//===============================================================
//               WebStorageClassic::InnerHandler
//===============================================================

WebStorageClassic::InnerHandler::InnerHandler(
    /* [in] */ WebStorageClassic* owner)
    : mOwner(owner)
{
}

ECode WebStorageClassic::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case SET_QUOTA_ORIGIN: {
            AutoPtr<IWebStorageOrigin> website;
            msg->GetObj((IInterface**)&website);
            String origin;
            Int64 quota;
            website->GetOrigin(&origin);
            website->GetQuota(&quota);
            NativeSetQuotaForOrigin(origin,
                                    quota);
            } break;

        case DELETE_ORIGIN: {
            AutoPtr<IWebStorageOrigin> website;
            msg->GetObj((IInterface**)&website);
            String origin;
            website->GetOrigin(&origin);
            NativeDeleteOrigin(origin);
            } break;

        case DELETE_ALL:
            NativeDeleteAllData();
            break;

        case GET_ORIGINS: {
            mOwner->SyncValues();
            AutoPtr<IValueCallback> callback;
            msg->GetObj((IInterface**)&callback);
            AutoPtr<IMap> origins;
            CHashMap::New(mOwner->mOrigins, (IMap**)&origins);
            AutoPtr<IMap> values;
            CHashMap::New((IMap**)&values);
            AutoPtr<ICharSequence> CALLBACKwrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
            AutoPtr<IInterface> oldValue1;
            values->Put(CALLBACKwrapper, callback, (IInterface**)&oldValue1);
            AutoPtr<ICharSequence> ORIGINSwrapper;
            CStringWrapper::New(ORIGINS, (ICharSequence**)&ORIGINSwrapper);
            AutoPtr<IInterface> oldValue2;
            values->Put(ORIGINSwrapper, origins, (IInterface**)&oldValue2);

            AutoPtr<IMessageHelper> mh;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
            AutoPtr<IMessage> UImsg;
            mh->Obtain(NULL, RETURN_ORIGINS, values, (IMessage**)&UImsg);
            mOwner->PostUIMessage(UImsg);
            } break;

        case GET_USAGE_ORIGIN: {
            mOwner->SyncValues();
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<ICharSequence> origin;
            AutoPtr<ICharSequence> ORIGINwrapper;
            CStringWrapper::New(ORIGIN, (ICharSequence**)&ORIGINwrapper);
            values->Get(ORIGINwrapper, (IInterface**)&origin);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKwrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
            values->Get(CALLBACKwrapper, (IInterface**)&callback);
            AutoPtr<IWebStorageOrigin> website;
            mOwner->mOrigins->Get(origin, (IInterface**)&website);
            AutoPtr<IMap> retValues;
            CHashMap::New((IMap**)&retValues);
            AutoPtr<IInterface> oldValue;
            retValues->Put(CALLBACKwrapper, callback, (IInterface**)&oldValue);
            if (website != NULL) {
                Int64 usage;
                website->GetUsage(&usage);
                AutoPtr<IInteger64> iusage;
                CInteger64::New(usage, (IInteger64**)&iusage);
                AutoPtr<ICharSequence> USAGEWrapper;
                CStringWrapper::New(USAGE, (ICharSequence**)&USAGEWrapper);
                AutoPtr<IInterface> oldValue;
                retValues->Put(USAGEWrapper, iusage, (IInterface**)&oldValue);
            }
            AutoPtr<IMessageHelper> mh;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
            AutoPtr<IMessage> UImsg;
            mh->Obtain(NULL, RETURN_USAGE_ORIGIN, retValues, (IMessage**)&UImsg);
            mOwner->PostUIMessage(UImsg);
            } break;

        case GET_QUOTA_ORIGIN: {
            mOwner->SyncValues();
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<ICharSequence> origin;
            AutoPtr<ICharSequence> ORIGINwrapper;
            CStringWrapper::New(ORIGIN, (ICharSequence**)&ORIGINwrapper);
            values->Get(ORIGINwrapper, (IInterface**)&ORIGINwrapper);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKwrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
            values->Get(CALLBACKwrapper, (IInterface**)&callback);
            AutoPtr<IWebStorageOrigin> website;
            mOwner->mOrigins->Get(origin, (IInterface**)&website);
            AutoPtr<IMap> retValues;
            CHashMap::New((IMap**)&retValues);
            AutoPtr<IInterface> oldValue;
            retValues->Put(CALLBACKwrapper, callback, (IInterface**)&oldValue);
            if (website != NULL) {
                Int64 quota;
                website->GetQuota(&quota);
                AutoPtr<IInteger64> iquota;
                CInteger64::New(quota, (IInteger64**)&iquota);
                AutoPtr<IInterface> oldValue;
                AutoPtr<ICharSequence> QUOTAwrapper;
                CStringWrapper::New(QUOTA, (ICharSequence**)&QUOTAwrapper);
                retValues->Put(QUOTAwrapper, iquota, (IInterface**)&oldValue);
            }
            AutoPtr<IMessageHelper> mh;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
            AutoPtr<IMessage> UImsg;
            mh->Obtain(NULL, RETURN_QUOTA_ORIGIN, retValues, (IMessage**)&UImsg);
            mOwner->PostUIMessage(UImsg);
            } break;

        case UPDATE:
            mOwner->SyncValues();
            break;
    }

    return NOERROR;
}

ECode WebStorageClassic::InnerHandler::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebStorageClassic::InnerHandler";
    return NOERROR;
}

//===============================================================
//                      WebStorageClassic
//===============================================================

extern "C" const InterfaceID EIID_WebStorageClassic =
        { 0xafadf029, 0x03b7, 0x12a4, { 0x2a, 0xb5, 0xa5, 0xce, 0x49, 0x28, 0x98, 0xf0 } };

// Message ids
const Int32 WebStorageClassic::UPDATE;
const Int32 WebStorageClassic::SET_QUOTA_ORIGIN;
const Int32 WebStorageClassic::DELETE_ORIGIN;
const Int32 WebStorageClassic::DELETE_ALL;
const Int32 WebStorageClassic::GET_ORIGINS;
const Int32 WebStorageClassic::GET_USAGE_ORIGIN;
const Int32 WebStorageClassic::GET_QUOTA_ORIGIN;

// Message ids on the UI thread
const Int32 WebStorageClassic::RETURN_ORIGINS;
const Int32 WebStorageClassic::RETURN_USAGE_ORIGIN;
const Int32 WebStorageClassic::RETURN_QUOTA_ORIGIN;

// Global instance of a WebStorage
AutoPtr<WebStorageClassic> WebStorageClassic::sWebStorage;

const String WebStorageClassic::ORIGINS("origins");
const String WebStorageClassic::ORIGIN("origin");
const String WebStorageClassic::CALLBACK("callback");
const String WebStorageClassic::USAGE("usage");
const String WebStorageClassic::QUOTA("quota");

WebStorageClassic::WebStorageClassic()
{
}

PInterface WebStorageClassic::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IWebStorage*)this;
    }
    else if ( riid == EIID_IWebStorage) {
        return (IWebStorage*)this;
    }
    else if ( riid == EIID_WebStorageClassic) {
        return (WebStorageClassic*)this;
    }
    return NULL;
}

UInt32 WebStorageClassic::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 WebStorageClassic::Release()
{
    return ElRefBase::Release();
}

ECode WebStorageClassic::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IWebStorage*)this) {
        *iid = EIID_IWebStorage;
    }
    else if (object == (IInterface*)(WebStorageClassic*)this) {
        *iid = EIID_WebStorageClassic;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

/**
 * @hide
 * Message handler, UI side
 * @hide
 */
void WebStorageClassic::CreateUIHandler()
{
    if (mUIHandler == NULL) {
        mUIHandler = new InnerUIHandler(this);
    }
}

/**
 * Message handler, WebCore side
 * @hide
 */
//synchronized
void WebStorageClassic::CreateHandler()
{
    Elastos::Core::AutoLock lock(mLock);

    if (mHandler == NULL) {
        mHandler = new InnerHandler(this);
    }
}

/*
 * When calling getOrigins(), getUsageForOrigin() and getQuotaForOrigin(),
 * we need to get the values from WebCore, but we cannot block while doing so
 * as we used to do, as this could result in a full deadlock (other WebCore
 * messages received while we are still blocked here, see http://b/2127737).
 *
 * We have to do everything asynchronously, by providing a callback function.
 * We post a message on the WebCore thread (mHandler) that will get the result
 * from WebCore, and we post it back on the UI thread (using mUIHandler).
 * We can then use the callback function to return the value.
 */
ECode WebStorageClassic::GetOrigins(
    /* [in] */ IValueCallback* callback)
{
    if (callback != NULL) {
        AutoPtr<IThread> thread = Thread::GetCurrentThread();
        String threadName;
        thread->GetName(&threadName);
        if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
            SyncValues();
            callback->OnReceiveValue(mOrigins);
        }
        else {
            AutoPtr<IMessageHelper> mh;
            AutoPtr<IMessage> msg;
            mh->Obtain(NULL, GET_ORIGINS, callback, (IMessage**)&msg);
            PostMessage(msg);
        }
    }

    return NOERROR;
}

/**
 * Returns a list of origins having a database
 * should only be called from WebViewCore.
 */
AutoPtr<ICollection> WebStorageClassic::GetOriginsSync()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String threadName;
    thread->GetName(&threadName);
    if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
        Update();
        AutoPtr<ICollection> collection;
        mOrigins->Values((ICollection**)&collection);
        return collection;
    }
    return NULL;
}

ECode WebStorageClassic::GetUsageForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callback)
{
    if (callback == NULL) {
        return NOERROR;
    }

    if (origin.IsNullOrEmpty()) {
        callback->OnReceiveValue(NULL);
        return NOERROR;
    }

    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String threadName;
    thread->GetName(&threadName);
    if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
        SyncValues();
        AutoPtr<IWebStorageOrigin> website;
        AutoPtr<ICharSequence> originWrapper;
        CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
        mOrigins->Get(originWrapper, (IInterface**)&website);
        Int64 usage;
        website->GetUsage(&usage);
        AutoPtr<IInteger64> iusage;
        CInteger64::New(usage, (IInteger64**)&iusage);
        callback->OnReceiveValue(iusage);
    }
    else {
        AutoPtr<IMap> values;
        CHashMap::New((IMap**)&values);
        AutoPtr<ICharSequence> ORIGINwrapper, originWrapper;
        CStringWrapper::New(ORIGIN, (ICharSequence**)&ORIGINwrapper);
        CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
        AutoPtr<IInterface> oldValue1;
        values->Put(ORIGINwrapper, originWrapper, (IInterface**)&oldValue1);
        AutoPtr<ICharSequence> CALLBACKwrapper;
        CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
        AutoPtr<IInterface> oldValue2;
        values->Put(CALLBACKwrapper, callback, (IInterface**)&oldValue2);
        AutoPtr<IMessageHelper> mh;
        AutoPtr<IMessage> msg;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        mh->Obtain(NULL, GET_USAGE_ORIGIN, values, (IMessage**)&msg);
        PostMessage(msg);
    }

    return NOERROR;
}

ECode WebStorageClassic::GetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callback)
{
    if (callback == NULL) {
        return NOERROR;
    }

    if (origin.IsNullOrEmpty()) {
        callback->OnReceiveValue(NULL);
        return NOERROR;
    }

    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String threadName;
    thread->GetName(&threadName);
    if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
        SyncValues();
        AutoPtr<IWebStorageOrigin> website;
        AutoPtr<ICharSequence> originWrapper;
        CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
        mOrigins->Get(originWrapper, (IInterface**)&website);
        Int64 usage;
        website->GetUsage(&usage);
        AutoPtr<IInteger64> iusage;
        CInteger64::New(usage, (IInteger64**)&iusage);
        callback->OnReceiveValue(iusage);
    }
    else {
        AutoPtr<IMap> values;
        CHashMap::New((IMap**)&values);
        AutoPtr<ICharSequence> ORIGINwrapper, originWrapper;
        CStringWrapper::New(ORIGIN, (ICharSequence**)&ORIGINwrapper);
        CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
        AutoPtr<IInterface> oldValue1;
        values->Put(ORIGINwrapper, originWrapper, (IInterface**)&oldValue1);
        AutoPtr<ICharSequence> CALLBACKwrapper;
        CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKwrapper);
        AutoPtr<IInterface> oldValue2;
        values->Put(CALLBACKwrapper, callback, (IInterface**)&oldValue2);
        AutoPtr<IMessage> msg;
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        mh->Obtain(NULL, GET_QUOTA_ORIGIN, values, (IMessage**)&msg);
        PostMessage(msg);
    }

    return NOERROR;
}

//@Override
ECode WebStorageClassic::SetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota)
{
    if (!origin.IsNullOrEmpty()) {
        AutoPtr<IThread> thread = Thread::GetCurrentThread();
        String threadName;
        thread->GetName(&threadName);
        if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
            NativeSetQuotaForOrigin(origin, quota);
        }
        else {
            AutoPtr<IMessage> msg;
            AutoPtr<IMessageHelper> mh;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
            AutoPtr<IWebStorageOrigin> webStorageOrigin = new WebStorage::Origin(origin, quota);
            mh->Obtain(NULL, SET_QUOTA_ORIGIN, webStorageOrigin, (IMessage**)&msg);
            PostMessage(msg);
        }
    }

    return NOERROR;
}

//@Override
ECode WebStorageClassic::DeleteOrigin(
    /* [in] */ const String& origin)
{
    if (!origin.IsNullOrEmpty()) {
        AutoPtr<IThread> thread = Thread::GetCurrentThread();
        String threadName;
        thread->GetName(&threadName);
        if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
            NativeDeleteOrigin(origin);
        }
        else {
            AutoPtr<IMessage> msg;
            AutoPtr<IMessageHelper> mh;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
            AutoPtr<IWebStorageOrigin> webStorageOrigin = new WebStorage::Origin(origin);
            mh->Obtain(NULL, DELETE_ORIGIN, webStorageOrigin, (IMessage**)&msg);
            PostMessage(msg);
        }
    }

    return NOERROR;
}

ECode WebStorageClassic::DeleteAllData()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String threadName;
    thread->GetName(&threadName);
    if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
        NativeDeleteAllData();
    }
    else {
        AutoPtr<IMessage> msg;
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        mh->Obtain(NULL, DELETE_ALL, (IMessage**)&msg);
        PostMessage(msg);
    }

    return NOERROR;
}

/**
 * Sets the maximum size of the ApplicationCache.
 * This should only ever be called on the WebKit thread.
 * Not part of the base-class API: this is only used by dump render tree.
 */
void WebStorageClassic::SetAppCacheMaximumSize(
    /* [in] */ Int64 size)
{
    NativeSetAppCacheMaximumSize(size);
}

/**
 * Get the singleton instance of this class.
 * @return The singleton {@link WebStorage} instance.
 */
AutoPtr<WebStorageClassic> WebStorageClassic::GetInstance()
{
    if (sWebStorage == NULL) {
      sWebStorage = new WebStorageClassic();
    }

    return sWebStorage;
}

/**
 * @hide
 * Post a Sync request
 */
void WebStorageClassic::Update()
{
    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String threadName;
    thread->GetName(&threadName);
    if (CWebViewCore::THREAD_NAME.Equals(threadName)) {
        SyncValues();
    }
    else {
        AutoPtr<IMessage> msg;
        AutoPtr<IMessageHelper> mh;
        mh->Obtain(NULL, UPDATE, (IMessage**)&msg);
        PostMessage(msg);
    }
}

/**
 * Utility function to send a message to our handler
 */
//synchronized
void WebStorageClassic::PostMessage(
    /* [in] */ IMessage* msg)
{
    Elastos::Core::AutoLock lock(mLock);
    if (mHandler != NULL) {
        Boolean result = FALSE;
        mHandler->SendMessage(msg, &result);
    }
}

/**
 * Utility function to send a message to the handler on the UI thread
 */
void WebStorageClassic::PostUIMessage(
    /* [in] */ IMessage* msg)
{
    if (mUIHandler != NULL) {
        Boolean result = FALSE;
        mUIHandler->SendMessage(msg, &result);
    }
}

/**
 * Run on the WebCore thread
 * set the local values with the current ones
 */
void WebStorageClassic::SyncValues()
{
    Elastos::Utility::HashSet<String>* tmp = NativeGetOrigins();
    CHashMap::New((IMap**)&mOrigins);
    Elastos::Utility::HashSet<String>::Iterator iter = tmp->Begin();
    String origin;
    for (; iter != tmp->End(); ++iter) {
        origin = *iter;
        AutoPtr<IWebStorageOrigin> website = new WebStorage::Origin(origin,
                             NativeGetQuotaForOrigin(origin),
                             NativeGetUsageForOrigin(origin));
        AutoPtr<ICharSequence> originWrapper;
        CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
        AutoPtr<IInterface> oldValue;
        mOrigins->Put(originWrapper, website, (IInterface**)&oldValue);
    }
}

// Native functions
Elastos::Utility::HashSet<String>* WebStorageClassic::NativeGetOrigins()
{
    assert(0);//TODO
    /*
    WTF::Vector<RefPtr<WebCore::SecurityOrigin> > coreOrigins;
    WebCore::DatabaseTracker::tracker().origins(coreOrigins);
    WTF::Vector<WebCore::KURL> manifestUrls;
    if (WebCore::cacheStorage().manifestURLs(&manifestUrls)) {
        Int32 size = manifestUrls.size();
        for (Int32 i = 0; i < size; ++i) {
            RefPtr<WebCore::SecurityOrigin> manifestOrigin = WebCore::SecurityOrigin::create(manifestUrls[i]);
            if (manifestOrigin.get() == 0)
                continue;
            coreOrigins.append(manifestOrigin);
        }
    }

    Elastos::Utility::HashSet<String>* set = new Elastos::Utility::HashSet<String>();
    for (unsigned i = 0; i < coreOrigins.size(); ++i) {
        WebCore::SecurityOrigin* origin = coreOrigins[i].get();
        WTF::String url = origin->toString();
        Elastos::String jUrl(url.ascii().data());
        set->Insert(jUrl);
    }

    return set;
    */
    return NULL;
}

Int64 WebStorageClassic::NativeGetUsageForOrigin(
    /* [in] */ const String& origin)
{
    assert(0);//TODO
    /*
    WTF::String originStr((const char*)origin);
    RefPtr<WebCore::SecurityOrigin> securityOrigin = WebCore::SecurityOrigin::createFromString(originStr);
    Int64 usage = WebCore::DatabaseTracker::tracker().usageForOrigin(securityOrigin.get());
    WTF::Vector<WebCore::KURL> manifestUrls;
    if (!WebCore::cacheStorage().manifestURLs(&manifestUrls))
        return usage;
    Int32 size = manifestUrls.size();
    for (Int32 i = 0; i < size; ++i) {
        RefPtr<WebCore::SecurityOrigin> manifestOrigin = WebCore::SecurityOrigin::create(manifestUrls[i]);
        if (manifestOrigin.get() == 0)
            continue;
        if (manifestOrigin->isSameSchemeHostPort(securityOrigin.get())) {
            int64_t cacheSize = 0;
            WebCore::cacheStorage().cacheGroupSize(manifestUrls[i].string(), &cacheSize);
            usage += cacheSize;
        }
    }
    return usage;
    */
    return 0;
}

Int64 WebStorageClassic::NativeGetQuotaForOrigin(
    /* [in] */ const String& origin)
{
    assert(0);//TODO
    /*
    WTF::String originStr((const char*)origin);
    RefPtr<WebCore::SecurityOrigin> securityOrigin = WebCore::SecurityOrigin::createFromString(originStr);
    Int64 quota = WebCore::DatabaseTracker::tracker().quotaForOrigin(securityOrigin.get());
    return quota;
    */
    return 0;
}

void WebStorageClassic::NativeSetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota)
{
    assert(0);//TODO
    /*
    WTF::String originStr((const char*)origin);
    RefPtr<WebCore::SecurityOrigin> securityOrigin = WebCore::SecurityOrigin::createFromString(originStr);
    WebCore::DatabaseTracker::tracker().setQuota(securityOrigin.get(), quota);
    */
}

void WebStorageClassic::NativeDeleteOrigin(
    /* [in] */ const String& origin)
{
    assert(0);//TODO
    /*
    WTF::String originStr((const char*)origin);
    RefPtr<WebCore::SecurityOrigin> securityOrigin = WebCore::SecurityOrigin::createFromString(originStr);
    WebCore::DatabaseTracker::tracker().deleteOrigin(securityOrigin.get());

    WTF::Vector<WebCore::KURL> manifestUrls;
    if (!WebCore::cacheStorage().manifestURLs(&manifestUrls))
        return;
    Int32 size = manifestUrls.size();
    for (Int32 i = 0; i < size; ++i) {
        RefPtr<WebCore::SecurityOrigin> manifestOrigin = WebCore::SecurityOrigin::create(manifestUrls[i]);
        if (manifestOrigin.get() == 0)
            continue;
        if (manifestOrigin->isSameSchemeHostPort(securityOrigin.get()))
            WebCore::cacheStorage().deleteCacheGroup(manifestUrls[i]);
    }
    */
}

void WebStorageClassic::NativeDeleteAllData()
{
    assert(0);//TODO
    /*
    // delete WebSQL database
    WebCore::DatabaseTracker::tracker().deleteAllDatabases();
    // delete AppCache
    WebCore::cacheStorage().deleteAllEntries();

    // FIXME: this is a workaround for eliminating any DOM Storage data (both
    // session and local storage) as there is no functionality inside WebKit at the
    // moment to do it. That functionality is a WIP in https://bugs.webkit.org/show_bug.cgi?id=51878
    // and when that patch lands and we merge it, we should move towards that approach instead.
    WebCore::PageGroup::clearDomStorage();
    */
}

void WebStorageClassic::NativeSetAppCacheMaximumSize(
    /* [in] */ Int64 size)
{
    assert(0);//TODO
    /*
    WebCore::cacheStorage().setMaximumSize(size);
    */
}

ECode WebStorageClassic::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "WebStorageClassic";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
