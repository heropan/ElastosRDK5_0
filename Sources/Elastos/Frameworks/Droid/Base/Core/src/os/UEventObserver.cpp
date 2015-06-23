
#include "os/UEventObserver.h"
#include <elastos/Vector.h>
#include <elastos/utility/logging/Logger.h>
#include <hardware_legacy/uevent.h>

using Elastos::Utility::Vector;
using Elastos::Core::EIID_IThread;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Os {

const String UEventObserver::TAG("UEventObserver");
const Boolean UEventObserver::DEBUG = FALSE;
AutoPtr<UEventObserver::UEventThread> UEventObserver::sThread;
Mutex UEventObserver::sClsLock;

//===============================================================
//UEventObserver::UEvent
//===============================================================
UEventObserver::UEvent::UEvent(
    /* [in] */ const String& message)
{
    Int32 offset = 0;
    Int32 length = message.GetLength();
    Int32 equals, at;
    while (offset < length) {
        equals = message.IndexOf(String("="), offset);
        at = message.IndexOf(String("\0"), offset);
        if (at < 0) break;

        if (equals > offset && equals < at) {
            // key is before the equals sign, and value is after
            mMap[message.Substring(offset, equals)] = message.Substring(equals + 1, at);
        }

        offset = at + 1;
    }
}

String UEventObserver::UEvent::Get(
    /* [in] */ const String& key)
{
    HashMap<String, String>::Iterator it = mMap.Find(key);
    return it != mMap.End() ? it->mSecond : String(NULL);
}

String UEventObserver::UEvent::Get(
    /* [in] */ const String& key,
    /* [in] */ const String& defaultValue)
{
    HashMap<String, String>::Iterator it = mMap.Find(key);
    return it != mMap.End() ? it->mSecond : defaultValue;
}

String UEventObserver::UEvent::ToString()
{
   // return mMap.toString();
    assert(0);
    return String(NULL);
}


//===============================================================
//UEventObserver::UEventThread
//===============================================================
UEventObserver::UEventThread::UEventThread()
{
    Thread::Init(String("UEventObserver"));
}

ECode UEventObserver::UEventThread::Run()
{
    FAIL_RETURN(NativeSetup());

    while (TRUE) {
        String message = NativeWaitForNextEvent();
        if (!message.IsNull()) {
            if (DEBUG) {
                Logger::D(TAG, message);
            }
            SendEvent(message);
        }
    }
    return NOERROR;
}

void UEventObserver::UEventThread::SendEvent(
    /* [in] */ const String& message)
{
    {
        Mutex::Autolock lock(mKeysAndObserversLock);

        List< AutoPtr<IInterface> >::Iterator it = mKeysAndObservers.Begin();
        while (it != mKeysAndObservers.End()) {
            AutoPtr<ICharSequence> csStr = ICharSequence::Probe(*it);
            assert(csStr != NULL);
            String key;
            csStr->ToString(&key);
            if (message.Contains(key)) {
                List< AutoPtr<IInterface> >::Iterator tmpIt = it;
                AutoPtr<IInterface> obj = *(++tmpIt);
                AutoPtr<UEventObserver> observer = (UEventObserver*)obj.Get();
                mTempObserversToSignal.PushBack(observer);
            }
            ++it;
            ++it;
        }
    }

    if (!mTempObserversToSignal.IsEmpty()) {
        AutoPtr<UEvent> event = new UEvent(message);
        List< AutoPtr<UEventObserver> >::Iterator it;
        for (it = mTempObserversToSignal.Begin(); it != mTempObserversToSignal.End(); ++it) {
            AutoPtr<UEventObserver> observer = *it;
            observer->OnUEvent(event);
        }
        mTempObserversToSignal.Clear();
    }
}

void UEventObserver::UEventThread::AddObserver(
    /* [in] */ const String& match,
    /* [in] */ UEventObserver* observer)
{
    Mutex::Autolock lock(mKeysAndObserversLock);

    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(match, (ICharSequence**)&charSeq);
    mKeysAndObservers.PushBack(charSeq);
    mKeysAndObservers.PushBack(observer->Probe(EIID_IInterface));
    NativeAddMatch(match);
}

/** Removes every key/value pair where value=observer from mObservers */
void UEventObserver::UEventThread::RemoveObserver(
    /* [in] */ UEventObserver* observer)
{
    Mutex::Autolock lock(mKeysAndObserversLock);

    List< AutoPtr<IInterface> >::Iterator it = mKeysAndObservers.Begin();
    while (it != mKeysAndObservers.End()) {
        List< AutoPtr<IInterface> >::Iterator oit = it;
        AutoPtr<IInterface> obj = *(++oit);
        if(obj.Get() == observer->Probe(EIID_IInterface)) {
            AutoPtr<ICharSequence> csStr = ICharSequence::Probe(*it);
            assert(csStr != NULL);
            String match;
            csStr->ToString(&match);
            NativeRemoveMatch(match);
            mKeysAndObservers.Erase(it);
            mKeysAndObservers.Erase(oit);
        }
        else {
            ++it;
            ++it;
        }
    }
}


//===============================================================
//UEventObserver
//===============================================================
UEventObserver::~UEventObserver()
{
    StopObserving();
}

CAR_INTERFACE_IMPL(UEventObserver, IInterface);

ECode UEventObserver::NativeSetup()
{
    if (!uevent_init()) {
        //jniThrowException(env, "java/lang/RuntimeException", "Unable to open socket for UEventObserver");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

static Mutex sMatchesMutex;
static Vector<String> sMatches;

static Boolean IsMatch(
    /* [in] */ const char* buffer,
    /* [in] */ Int32 length)
{
    Mutex::Autolock lock(sMatchesMutex);

    Vector<String>::Iterator it;
    for (it = sMatches.Begin(); it != sMatches.End(); ++it) {
        String match = *it;

        // Consider all zero-delimited fields of the buffer.
        const char* field = buffer;
        const char* end = buffer + length + 1;
        do {
            if (strstr(field, match.string())) {
                Logger::V("Matched uevent message with pattern: %s", match.string());
                return TRUE;
            }
            field += strlen(field) + 1;
        } while (field != end);
    }
    return FALSE;
}

String UEventObserver::NativeWaitForNextEvent()
{
    char buffer[1024];

    for (;;) {
        int length = uevent_next_event(buffer, sizeof(buffer) - 1);
        if (length <= 0) {
            return String(NULL);
        }
        buffer[length] = '\0';

        // Logger::V("Received uevent message: %s", buffer);

        if (IsMatch(buffer, length)) {
            // Assume the message is ASCII.
            return String(buffer);
        }
    }
}

void UEventObserver::NativeAddMatch(
    /* [in] */ const String& matchStr)
{
    Mutex::Autolock lock(sMatchesMutex);
    sMatches.PushBack(matchStr);
}

void UEventObserver::NativeRemoveMatch(
    /* [in] */ const String& matchStr)
{
    Mutex::Autolock lock(sMatchesMutex);

    Vector<String>::Iterator it;
    for (it = sMatches.Begin(); it != sMatches.End(); ++it) {
        if ((*it).Equals(matchStr)) {
            sMatches.Erase(it);
            break; // only remove first occurrence
        }
    }
}

AutoPtr<UEventObserver::UEventThread> UEventObserver::GetThread()
{
    Mutex::Autolock lock(sClsLock);

    if (sThread == NULL) {
        sThread = new UEventThread();
        sThread->Start();
    }
    return sThread;
}

AutoPtr<UEventObserver::UEventThread> UEventObserver::PeekThread()
{
    Mutex::Autolock lock(sClsLock);
    return sThread;
}

ECode UEventObserver::StartObserving(
    /* [in] */ const String& match)
{
    if (match.IsNullOrEmpty()) {
        //throw new IllegalArgumentException("match substring must be non-empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<UEventThread> t = GetThread();
    t->AddObserver(match, this);
    return NOERROR;
}

void UEventObserver::StopObserving()
{
    AutoPtr<UEventThread> t = GetThread();
    if (t != NULL) {
        t->RemoveObserver(this);
    }
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
