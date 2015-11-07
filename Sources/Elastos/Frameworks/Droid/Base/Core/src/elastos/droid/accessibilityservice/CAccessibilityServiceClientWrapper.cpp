#include "elastos/droid/accessibilityservice/CAccessibilityServiceClientWrapper.h"
#include "elastos/droid/internal/os/CHandlerCaller.h"
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClient.h"

using Elastos::Droid::Internal::Os::EIID_IHandlerCallerCallback;
using Elastos::Droid::Internal::Os::CHandlerCaller;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::View::Accessibility::CAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const Int32 CAccessibilityServiceClientWrapper::DO_SET_SET_CONNECTION = 1;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_INTERRUPT = 2;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_ACCESSIBILITY_EVENT = 3;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_GESTURE = 4;
const Int32 CAccessibilityServiceClientWrapper::DO_CLEAR_ACCESSIBILITY_CACHE = 5;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_KEY_EVENT = 6;

CAR_INTERFACE_IMPL_2(CAccessibilityServiceClientWrapper, Object, IIAccessibilityServiceClient,
        IBinder/*, IHandlerCallerCallback*/)

CAR_OBJECT_IMPL(CAccessibilityServiceClientWrapper)

CAccessibilityServiceClientWrapper::CAccessibilityServiceClientWrapper()
    : mConnectionId(0)
{}

CAccessibilityServiceClientWrapper::~CAccessibilityServiceClientWrapper()
{}

ECode CAccessibilityServiceClientWrapper::constructor()
{
    return NOERROR;
}

ECode CAccessibilityServiceClientWrapper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* looper,
    /* [in] */ IAccessibilityServiceCallbacks* callback)
{
    mCallback = callback;
    // CHandlerCaller::New(context, looper, THIS_PROBE(IHandlerCallerCallback), TRUE, FALSE, (IHandlerCaller**)mCaller);
    return NOERROR;
}

ECode CAccessibilityServiceClientWrapper::SetConnection(
    /* [in] */ IIAccessibilityServiceConnection* connection,
    /* [in] */ Int32 connectionId)
{
    AutoPtr<IMessage> message;
    mCaller->ObtainMessageIO(DO_SET_SET_CONNECTION, connectionId, connection, (IMessage**)&message);
    return mCaller->SendMessage(message);
}

ECode CAccessibilityServiceClientWrapper::OnInterrupt()
{
    AutoPtr<IMessage> message;
    mCaller->ObtainMessage(DO_ON_INTERRUPT, (IMessage**)&message);
    return mCaller->SendMessage(message);
}

ECode CAccessibilityServiceClientWrapper::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AutoPtr<IMessage> message;
    mCaller->ObtainMessageO(DO_ON_ACCESSIBILITY_EVENT, event, (IMessage**)&message);
    return mCaller->SendMessage(message);
}

ECode CAccessibilityServiceClientWrapper::OnGesture(
    /* [in] */ Int32 gestureId)
{
    AutoPtr<IMessage> message;
    mCaller->ObtainMessageI(DO_ON_GESTURE, gestureId, (IMessage**)&message);
    return mCaller->SendMessage(message);
}

ECode CAccessibilityServiceClientWrapper::ClearAccessibilityCache()
{
    AutoPtr<IMessage> message;
    mCaller->ObtainMessage(DO_CLEAR_ACCESSIBILITY_CACHE, (IMessage**)&message);
    return mCaller->SendMessage(message);
}

ECode CAccessibilityServiceClientWrapper::OnKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 sequence)
{
    AutoPtr<IMessage> message;
    mCaller->ObtainMessageIO(DO_ON_KEY_EVENT, sequence, event, (IMessage**)&message);
    return mCaller->SendMessage(message);
}

ECode CAccessibilityServiceClientWrapper::ExecuteMessage(
    /* [in] */ IMessage* message)
{
    Int32 what, arg1;
    message->GetWhat(&what);
    message->GetArg1(&arg1);
    ECode ec;

    switch (what) {
        case DO_ON_ACCESSIBILITY_EVENT : {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);

            IAccessibilityEvent* event = IAccessibilityEvent::Probe(obj);
            if (event != NULL) {
                AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
                client->OnAccessibilityEvent(event);
                mCallback->OnAccessibilityEvent(event);
                // Make sure the event is recycled.
                // try {
                ec = IAccessibilityRecord::Probe(event)->Recycle();
                if (ec != NOERROR) {
                    return E_ILLEGAL_STATE_EXCEPTION;
                }
                // } catch (IllegalStateException ise) {
                //             /* ignore - best effort */
                // }
            }
            break;
        }

        case DO_ON_INTERRUPT :
            mCallback->OnInterrupt();
            break;

        case DO_SET_SET_CONNECTION : {
            AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
            mConnectionId = arg1;
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            IIAccessibilityServiceConnection* connection = IIAccessibilityServiceConnection::Probe(obj);

            if (connection != NULL) {
                client->AddConnection(mConnectionId, connection);
                mCallback->OnSetConnectionId(mConnectionId);
                mCallback->OnServiceConnected();
            }
            else {
                client->RemoveConnection(mConnectionId);
                client->ClearCache();
                mCallback->OnSetConnectionId(IAccessibilityInteractionClient::NO_ID);
            }
            break;
        }

        case DO_ON_GESTURE : {
            Boolean result;
            mCallback->OnGesture(arg1, &result);
            break;
        }
        case DO_CLEAR_ACCESSIBILITY_CACHE : {
            AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
            client->ClearCache();
            break;
        }

        case DO_ON_KEY_EVENT : {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            AutoPtr<IKeyEvent> event = IKeyEvent::Probe(obj);
            // try {
            AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
            AutoPtr<IIAccessibilityServiceConnection> connection;
            client->GetConnection(mConnectionId, (IIAccessibilityServiceConnection**)&connection);
            if (connection != NULL) {
                Boolean result;
                mCallback->OnKeyEvent(event, &result);
                Int32 sequence = arg1;
                // try {
                ec = connection->SetOnKeyEventResult(result, sequence);
                // } catch (RemoteException re) {
                            /* ignore */
                if (ec != NOERROR) {
                    return E_REMOTE_EXCEPTION;
                }
                // }
            }
            // } finally {
                    // Make sure the event is recycled.
                    // try {
            ec = IAccessibilityRecord::Probe(event)->Recycle();
                    // } catch (IllegalStateException ise) {
                        /* ignore - best effort */
            if (ec != NOERROR) {
                return E_ILLEGAL_STATE_EXCEPTION;
            }
                    // // }
            // }
            break;
        }

        default :
            // Logger::W(LOG_TAG, "Unknown message type %d", what);
            break;
    }
    return NOERROR;
}

ECode CAccessibilityServiceClientWrapper::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = String("CAccessibilityServiceClientWrapper");
    return NOERROR;
}

}
}
}
