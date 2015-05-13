#include "CAccessibilityServiceClientWrapper.h"
#include "os/HandlerCaller.h"
#include "view/accessibility/CAccessibilityInteractionClientHelper.h"

using Elastos::Droid::Os::HandlerCaller;
using Elastos::Droid::Os::EIID_IHandlerCallerCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClientHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityInteractionClientHelper;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const Int32 CAccessibilityServiceClientWrapper::DO_SET_SET_CONNECTION = 10;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_INTERRUPT = 20;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_ACCESSIBILITY_EVENT = 30;
const Int32 CAccessibilityServiceClientWrapper::DO_ON_GESTURE = 40;

ECode CAccessibilityServiceClientWrapper::constructor(
    /* [in] */ IContext * context,
    /* [in] */ ILooper * looper,
    /* [in] */ IAccessibilityServiceCallbacks* callback)
{
    mCallback = callback;
    mCaller = new HandlerCaller(context, looper, THIS_PROBE(IHandlerCallerCallback), FALSE);
    return NOERROR;
}

ECode CAccessibilityServiceClientWrapper::SetConnection(
    /* [in] */ IAccessibilityServiceConnection* connection,
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

ECode CAccessibilityServiceClientWrapper::ExecuteMessage(
    /* [in] */ IMessage* message)
{
    Int32 what, arg1;
    message->GetWhat(&what);
    message->GetArg1(&arg1);

    switch (what) {
        case DO_ON_ACCESSIBILITY_EVENT : {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            IAccessibilityEvent* event = IAccessibilityEvent::Probe(obj);
            if (event != NULL) {
                AutoPtr<IAccessibilityInteractionClientHelper> helper;
                CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
                AutoPtr<IAccessibilityInteractionClient> client;
                helper->GetInstance((IAccessibilityInteractionClient**)&client);

                client->OnAccessibilityEvent(event);
                mCallback->OnAccessibilityEvent(event);
                event->Recycle();
            }
            return NOERROR;
        }

        case DO_ON_INTERRUPT :
            mCallback->OnInterrupt();
            return NOERROR;

        case DO_SET_SET_CONNECTION : {
            AutoPtr<IAccessibilityInteractionClientHelper> helper;
            CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
            AutoPtr<IAccessibilityInteractionClient> client;
            helper->GetInstance((IAccessibilityInteractionClient**)&client);

            const Int32 connectionId = arg1;
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            IAccessibilityServiceConnection* connection = IAccessibilityServiceConnection::Probe(obj);

            if (connection != NULL) {
                client->AddConnection(connectionId, connection);
                mCallback->OnSetConnectionId(connectionId);
                mCallback->OnServiceConnected();
            }
            else {
                client->RemoveConnection(connectionId);
                mCallback->OnSetConnectionId(IAccessibilityInteractionClient::NO_ID);
            }
            return NOERROR;
        }

        case DO_ON_GESTURE : {
            Boolean result;
            mCallback->OnGesture(arg1, &result);
            return NOERROR;
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
