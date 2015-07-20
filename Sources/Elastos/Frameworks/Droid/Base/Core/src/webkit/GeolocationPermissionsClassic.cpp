
#include "os/CMessageHelper.h"
#include "webkit/CWebViewCore.h"
#include "webkit/GeolocationPermissionsClassic.h"
#include <elastos/core/Thread.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::CBoolean;
using Elastos::Core::IBoolean;
using Elastos::Core::ICharSequence;
using Elastos::Core::Thread;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Utility::CHashMap;
using Elastos::Utility::CHashSet;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//         GeolocationPermissionsClassic::InnerUIHandler
//===============================================================

GeolocationPermissionsClassic::InnerUIHandler::InnerUIHandler(
    /* [in] */ GeolocationPermissionsClassic* owner)
    : mOwner(owner)
{
}

ECode GeolocationPermissionsClassic::InnerUIHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    // Runs on the UI thread.
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case RETURN_ORIGINS: {
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<ISet> origins;
            AutoPtr<ICharSequence> ORIGINSWrapper;
            CStringWrapper::New(ORIGINS, (ICharSequence**)&ORIGINSWrapper);
            values->Get(ORIGINSWrapper, (IInterface**)&origins);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKWrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKWrapper);
            values->Get(CALLBACKWrapper, (IInterface**)&callback);
            callback->OnReceiveValue(origins);
        } break;
        case RETURN_ALLOWED: {
            AutoPtr<IMap> values;
            msg->GetObj((IInterface**)&values);
            AutoPtr<IBoolean> allowed;
            AutoPtr<ICharSequence> ALLOWEDWrapper;
            CStringWrapper::New(ALLOWED, (ICharSequence**)&ALLOWEDWrapper);
            values->Get(ALLOWEDWrapper, (IInterface**)&allowed);
            AutoPtr<IValueCallback> callback;
            AutoPtr<ICharSequence> CALLBACKWrapper;
            CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKWrapper);
            values->Get(CALLBACKWrapper, (IInterface**)&callback);
            callback->OnReceiveValue(allowed);
        } break;
    }

    return NOERROR;
}

//===============================================================
//         GeolocationPermissionsClassic::InnerUIHandler
//===============================================================

GeolocationPermissionsClassic::InnerHandler::InnerHandler(
    /* [in] */ GeolocationPermissionsClassic* owner)
    : mOwner(owner)
{
}

ECode GeolocationPermissionsClassic::InnerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    // Runs on the WebKit thread.
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case GET_ORIGINS: {
                AutoPtr<ISet> origins = NativeGetOrigins();
                AutoPtr<IValueCallback> callback;
                msg->GetObj((IInterface**)&callback);
                AutoPtr<IMap> values;
                CHashMap::New((IMap**)&values);
                AutoPtr<ICharSequence> CALLBACKWrapper;
                CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKWrapper);
                AutoPtr<IInterface> oldValue1;
                values->Put(CALLBACKWrapper, callback, (IInterface**)&oldValue1);
                AutoPtr<ICharSequence> ORIGINSWrapper;
                CStringWrapper::New(ORIGINS, (ICharSequence**)&ORIGINSWrapper);
                AutoPtr<IInterface> oldValue2;
                values->Put(ORIGINSWrapper, origins, (IInterface**)&oldValue2);
                AutoPtr<IMessage> msg;
                AutoPtr<IMessageHelper> mh;
                CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
                mh->Obtain(NULL, RETURN_ORIGINS, values, (IMessage**)&msg);
                mOwner->PostUIMessage(msg);
            } break;

        case GET_ALLOWED: {
                AutoPtr<IMap> values;
                msg->GetObj((IInterface**)&values);
                AutoPtr<ICharSequence> originCS, ORIGINWrapper;
                CStringWrapper::New(ORIGIN, (ICharSequence**)&ORIGINWrapper);
                values->Get(ORIGINWrapper, (IInterface**)&originCS);
                String origin;
                originCS->ToString(&origin);
                AutoPtr<ICharSequence> CALLBACKWrapper;
                CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKWrapper);
                AutoPtr<IValueCallback> callback;
                values->Get(CALLBACKWrapper, (IInterface**)&callback);
                Boolean allowed = NativeGetAllowed(origin);
                AutoPtr<IMap> retValues;
                CHashMap::New((IMap**)&retValues);
                AutoPtr<IInterface> oldValue1;
                retValues->Put(CALLBACKWrapper, callback, (IInterface**)&oldValue1);
                AutoPtr<IBoolean> iAllowed;
                CBoolean::New(allowed, (IBoolean**)&iAllowed);
                AutoPtr<ICharSequence> ALLOWEDWrapper;
                CStringWrapper::New(ALLOWED, (ICharSequence**)&ALLOWEDWrapper);
                AutoPtr<IInterface> oldValue2;
                retValues->Put(ALLOWEDWrapper, iAllowed, (IInterface**)&oldValue2);
                AutoPtr<IMessage> msg;
                AutoPtr<IMessageHelper> mh;
                CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
                mh->Obtain(NULL, RETURN_ALLOWED, retValues, (IMessage**)&msg);
                mOwner->PostUIMessage(msg);
            } break;
        case CLEAR: {
                AutoPtr<ICharSequence> obj;
                msg->GetObj((IInterface**)&obj);
                String str;
                obj->ToString(&str);
                NativeClear(str);
            } break;
        case ALLOW: {
                AutoPtr<ICharSequence> obj;
                msg->GetObj((IInterface**)&obj);
                String str;
                obj->ToString(&str);
                NativeAllow(str);
            } break;
        case CLEAR_ALL:
            NativeClearAll();
            break;
    }

    return NOERROR;
}

//===============================================================
//               GeolocationPermissionsClassic
//===============================================================

// Message ids
const Int32 GeolocationPermissionsClassic::GET_ORIGINS;
const Int32 GeolocationPermissionsClassic::GET_ALLOWED;
const Int32 GeolocationPermissionsClassic::CLEAR;
const Int32 GeolocationPermissionsClassic::ALLOW;
const Int32 GeolocationPermissionsClassic::CLEAR_ALL;

// Message ids on the UI thread
const Int32 GeolocationPermissionsClassic::RETURN_ORIGINS;
const Int32 GeolocationPermissionsClassic::RETURN_ALLOWED;

const String GeolocationPermissionsClassic::ORIGINS("origins");
const String GeolocationPermissionsClassic::ORIGIN("origin");
const String GeolocationPermissionsClassic::CALLBACK("callback");
const String GeolocationPermissionsClassic::ALLOWED("allowed");

// Global instance
AutoPtr<GeolocationPermissionsClassic> GeolocationPermissionsClassic::sInstance;

GeolocationPermissionsClassic::GeolocationPermissionsClassic()
{
}

CAR_INTERFACE_IMPL(GeolocationPermissionsClassic, IGeolocationPermissions);

AutoPtr<GeolocationPermissionsClassic> GeolocationPermissionsClassic::GetInstance()
{
    if (sInstance == NULL) {
        sInstance = new GeolocationPermissionsClassic();
    }

    return sInstance;
}

/**
 * Creates the UI message handler. Must be called on the UI thread.
 * @hide
 */
void GeolocationPermissionsClassic::CreateUIHandler()
{
    if (mUIHandler == NULL) {
        mUIHandler = new InnerUIHandler(this);
    }
}

/**
 * Creates the message handler. Must be called on the WebKit thread.
 * @hide
 */
//synchronized
void GeolocationPermissionsClassic::CreateHandler()
{
    AutoLock lock(mLock);

    if (mHandler == NULL) {
        mHandler = new InnerHandler(this);

        Vector< AutoPtr<IMessage> >::Iterator iter = mQueuedMessages.Begin();
        for(; iter != mQueuedMessages.End(); ++iter) {
            Boolean result = FALSE;
            mHandler->SendMessage(*iter, &result);
        }
        mQueuedMessages.Clear();
    }
}


// Note that we represent the origins as strings. These are created using
// WebCore::SecurityOrigin::toString(). As long as all 'HTML 5 modules'
// (Database, Geolocation etc) do so, it's safe to match up origins based
// on this string.
ECode GeolocationPermissionsClassic::GetOrigins(
    /* [in] */ IValueCallback* callback)
{
    if (callback != NULL) {
        AutoPtr<IThread> thread = Thread::GetCurrentThread();
        String name;
        thread->GetName(&name);
        if (CWebViewCore::THREAD_NAME.Equals(name)) {
            AutoPtr<ISet> origins = NativeGetOrigins();
            callback->OnReceiveValue(origins);
        }
        else {
            AutoPtr<IMessage> msg;
            AutoPtr<IMessageHelper> mh;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
            mh->Obtain(NULL, GET_ORIGINS, callback, (IMessage**)&msg);
            PostMessage(msg);
        }
    }

    return NOERROR;
}

ECode GeolocationPermissionsClassic::GetAllowed(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callback)
{
    if (callback == NULL) {
        return NOERROR;
    }

    if (origin == NULL) {
        callback->OnReceiveValue(NULL);
        return NOERROR;
    }

    AutoPtr<IThread> thread = Thread::GetCurrentThread();
    String name;
    thread->GetName(&name);
    if (CWebViewCore::THREAD_NAME.Equals(name)) {
        Boolean allowed = NativeGetAllowed(origin);
        AutoPtr<IBoolean> value;
        CBoolean::New(allowed, (IBoolean**)&value);
        callback->OnReceiveValue(value);
    }
    else {
        AutoPtr<IMap> values;
        CHashMap::New((IMap**)&values);
        AutoPtr<ICharSequence> ORIGINWrapper;
        CStringWrapper::New(ORIGIN, (ICharSequence**)&ORIGINWrapper);
        AutoPtr<ICharSequence> originWrapper;
        CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
        AutoPtr<IInterface> oldValue1;
        values->Put(ORIGINWrapper, originWrapper, (IInterface**)&oldValue1);
        AutoPtr<ICharSequence> CALLBACKWrapper;
        CStringWrapper::New(CALLBACK, (ICharSequence**)&CALLBACKWrapper);
        AutoPtr<IInterface> oldValue2;
        values->Put(CALLBACKWrapper, callback, (IInterface**)&oldValue2);
        AutoPtr<IMessage> msg;
        AutoPtr<IMessageHelper> mh;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
        mh->Obtain(NULL, GET_ALLOWED, values, (IMessage**)&msg);
        PostMessage(msg);
    }

    return NOERROR;
}

// This method may be called before the WebKit
// thread has intialized the message handler. Messages will be queued until
// this time.
ECode GeolocationPermissionsClassic::Clear(
    /* [in] */ const String& origin)
{
    // Called on the UI thread.
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<ICharSequence> originWrapper;
    CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
    mh->Obtain(NULL, CLEAR, originWrapper, (IMessage**)&msg);
    PostMessage(msg);
    return NOERROR;
}

// This method may be called before the WebKit
// thread has intialized the message handler. Messages will be queued until
// this time.
ECode GeolocationPermissionsClassic::Allow(
    /* [in] */ const String& origin)
{
    // Called on the UI thread.
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    AutoPtr<ICharSequence> originWrapper;
    CStringWrapper::New(origin, (ICharSequence**)&originWrapper);
    mh->Obtain(NULL, ALLOW, originWrapper, (IMessage**)&msg);
    PostMessage(msg);
    return NOERROR;
}

ECode GeolocationPermissionsClassic::ClearAll()
{
    // Called on the UI thread.
    AutoPtr<IMessage> msg;
    AutoPtr<IMessageHelper> mh;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&mh);
    mh->Obtain(NULL, CLEAR_ALL, (IMessage**)&msg);
    PostMessage(msg);
    return  NOERROR;
}

/**
 * Utility function to send a message to our handler.
 */
//synchronized
void GeolocationPermissionsClassic::PostMessage(
    /* [in] */ IMessage* msg)
{
    AutoLock lock(mLock);

    if (mHandler == NULL) {
        mQueuedMessages.PushBack(msg);
    }
    else {
        Boolean result = FALSE;
        mHandler->SendMessage(msg, &result);
    }
}

/**
 * Utility function to send a message to the handler on the UI thread
 */
void GeolocationPermissionsClassic::PostUIMessage(
    /* [in] */ IMessage* msg)
{
    if (mUIHandler != NULL) {
        Boolean result = FALSE;
        mUIHandler->SendMessage(msg, &result);
    }
}

// Native functions, run on the WebKit thread.
AutoPtr<ISet> GeolocationPermissionsClassic::NativeGetOrigins()
{
    //android::GeolocationPermissions::OriginSet origins = android::GeolocationPermissions::getOrigins();
    AutoPtr<ISet> set;
    assert(0);//TODO
    /*
    CHashSet::New((ISet**)&set);
    android::GeolocationPermissions::OriginSet::const_iterator end = origins.end();
    for (android::GeolocationPermissions::OriginSet::const_iterator iter = origins.begin(); iter != end; ++iter) {
        AutoPtr<ICharSequence> value;
        CStringWrapper::New(String((*iter).ascii().data()), (ICharSequence**)&value);
        Boolean modified = FALSE;
        set->Add(value, &modified);
    }
    */
    return set;
}

Boolean GeolocationPermissionsClassic::NativeGetAllowed(
    /* [in] */ const String& origin)
{
    assert(0);//TODO
    //WTF::String originString((const char*)origin);
    //return android::GeolocationPermissions::getAllowed(originString);
    return false;
}

void GeolocationPermissionsClassic::NativeClear(
    /* [in] */ const String& origin)
{
    assert(0);//TODO
    //WTF::String originString((const char*)origin);
    //android::GeolocationPermissions::clear(originString);
}

void GeolocationPermissionsClassic::NativeAllow(
    /* [in] */ const String& origin)
{
    assert(0);//TODO
    //WTF::String originString((const char*)origin);
    //android::GeolocationPermissions::allow(originString);
}

void GeolocationPermissionsClassic::NativeClearAll()
{
    assert(0);//TODO
    //android::GeolocationPermissions::clearAll();
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
