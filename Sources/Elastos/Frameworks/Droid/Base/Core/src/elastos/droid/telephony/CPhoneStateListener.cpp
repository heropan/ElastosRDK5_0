
#include "elastos/droid/telephony/CCellLocationHelper.h"
#include "elastos/droid/telephony/CPhoneStateListener.h"
#include "elastos/droid/os/Looper.h"

using Elastos::Droid::Internal::Telephony::EIID_IIPhoneStateListener;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Droid::Privacy::IPrivacySettings;
using Elastos::Droid::Privacy::IPrivacySettingsManager;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;

namespace Elastos {
namespace Droid {
namespace Telephony {

const String CPhoneStateListener::TAG("CPhoneStateListener");
const Boolean CPhoneStateListener::DBG = FALSE;

CAR_INTERFACE_IMPL(CPhoneStateListener, Object, IPhoneStateListener)

CAR_OBJECT_IMPL(CPhoneStateListener)

//==============================================================================
// CPhoneStateListener::CPhoneStateListenerCallback
//==============================================================================

CAR_INTERFACE_IMPL(CPhoneStateListener::CPhoneStateListenerCallback,
        Object, IIPhoneStateListener);

CPhoneStateListener::CPhoneStateListenerCallback::CPhoneStateListenerCallback(
            /* [in] */ CPhoneStateListener* host)
    : mHost(host)
{}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnServiceStateChanged(
    /* [in] */ IServiceState* serviceState)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_SERVICE_STATE,
            0, 0, (IInterface*)serviceState, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnSignalStrengthChanged(
    /* [in] */ Int32 asu)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_SIGNAL_STRENGTH,
            asu, 0, (IInterface*)NULL, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnMessageWaitingIndicatorChanged(
    /* [in] */ Boolean mwi)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_MESSAGE_WAITING_INDICATOR,
            mwi ? 1 : 0, 0, (IInterface*)NULL, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnCallForwardingIndicatorChanged(
    /* [in] */ Boolean cfi)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_CALL_FORWARDING_INDICATOR,
            cfi ? 1 : 0, 0, (IInterface*)NULL, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

// we use bundle here instead of CellLocation so it can get the right subclass
ECode CPhoneStateListener::CPhoneStateListenerCallback::OnCellLocationChanged(
    /* [in] */ IBundle* location)
{
    AutoPtr<ICellLocation> cellLocation;
    AutoPtr<ICellLocationHelper> helper;
    CCellLocationHelper::AcquireSingleton((ICellLocationHelper**)&helper);
    helper->NewFromBundle(location, (ICellLocation**)&cellLocation);

    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_CELL_LOCATION,
            0, 0, location, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnCallStateChanged(
    /* [in] */ Int32 state,
    /* [in] */ const String& incomingNumber)
{
    AutoPtr<ICharSequence> cs;
    CString::New(incomingNumber, (ICharSequence**)&cs);
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_CALL_STATE,
            state, 0, cs, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnDataConnectionStateChanged(
    /* [in] */ Int32 state,
    /* [in] */ Int32 networkType)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_DATA_CONNECTION_STATE,
            state, networkType, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnDataActivity(
    /* [in] */ Int32 direction)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_DATA_ACTIVITY,
            direction, 0, NULL, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnSignalStrengthsChanged(
    /* [in] */ ISignalStrength* signalStrength)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_SIGNAL_STRENGTHS,
            0, 0, signalStrength, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnOtaspChanged(
    /* [in] */ Int32 otaspMode)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_OTASP_CHANGED,
            otaspMode, 0, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnCellInfoChanged(
    /* [in] */ IList* cellInfo)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_CELL_INFO,
            0, 0, cellInfo, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnPreciseCallStateChanged(
    /* [in] */ IPreciseCallState* callState)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_PRECISE_CALL_STATE,
            0, 0, callState, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnPreciseDataConnectionStateChanged(
    /* [in] */ IPreciseDataConnectionState* dataConnectionState)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_PRECISE_DATA_CONNECTION_STATE,
            0, 0, dataConnectionState, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnDataConnectionRealTimeInfoChanged(
    /* [in] */ IDataConnectionRealTimeInfo* dcRtInfo)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_DATA_CONNECTION_REAL_TIME_INFO,
            0, 0, dcRtInfo, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnVoLteServiceStateChanged(
    /* [in] */ IVoLteServiceState* lteState)
{
    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_VOLTE_STATE,
            0, 0, lteState, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

ECode CPhoneStateListener::CPhoneStateListenerCallback::OnOemHookRawEvent(
    /* [in] */ ArrayOf<Byte>* rawData)
{
    AutoPtr<IByteBuffer> buffer;
    AutoPtr<IByteBufferHelper> bufferHelper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&bufferHelper);
    bufferHelper->Wrap(rawData, (IByteBuffer**)&buffer);

    AutoPtr<IMessage> msg;
    mHost->mHandler->ObtainMessage(IPhoneStateListener::LISTEN_OEM_HOOK_RAW_EVENT,
            0, 0, buffer, (IMessage**)&msg);
    msg->SendToTarget();
    return NOERROR;
}

//==============================================================================
// CPhoneStateListener::MyHandler
//==============================================================================

ECode CPhoneStateListener::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    VALIDATE_NOT_NULL(msg);

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case IPhoneStateListener::LISTEN_SERVICE_STATE:
            mHost->OnServiceStateChanged(IServiceState::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_SIGNAL_STRENGTH:
            mHost->OnSignalStrengthChanged(arg1);
            break;
        case IPhoneStateListener::LISTEN_MESSAGE_WAITING_INDICATOR:
            mHost->OnMessageWaitingIndicatorChanged(arg1 != 0);
            break;
        case IPhoneStateListener::LISTEN_CALL_FORWARDING_INDICATOR:
            mHost->OnCallForwardingIndicatorChanged(arg1 != 0);
            break;
        case IPhoneStateListener::LISTEN_CELL_LOCATION:
            mHost->OnCellLocationChanged(ICellLocation::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_CALL_STATE:
            {
                AutoPtr<ICharSequence> cs = ICharSequence::Probe(obj.Get());
                String str;
                cs->ToString(&str);
                mHost->OnCallStateChanged(arg1, str);
            }
            break;
        case IPhoneStateListener::LISTEN_DATA_CONNECTION_STATE:
            mHost->OnDataConnectionStateChanged(arg1, arg2);
            mHost->OnDataConnectionStateChanged(arg1);
            break;
        case IPhoneStateListener::LISTEN_DATA_ACTIVITY:
            mHost->OnDataActivity(arg1);
            break;
        case IPhoneStateListener::LISTEN_SIGNAL_STRENGTHS:
            mHost->OnSignalStrengthsChanged(ISignalStrength::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_OTASP_CHANGED:
            mHost->OnOtaspChanged(arg1);
            break;
        case IPhoneStateListener::LISTEN_CELL_INFO:
            mHost->OnCellInfoChanged(IList::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_PRECISE_CALL_STATE:
            mHost->OnPreciseCallStateChanged(IPreciseCallState::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_PRECISE_DATA_CONNECTION_STATE:
            mHost->OnPreciseDataConnectionStateChanged(
                    IPreciseDataConnectionState::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_DATA_CONNECTION_REAL_TIME_INFO:
            mHost->OnDataConnectionRealTimeInfoChanged(
                    IDataConnectionRealTimeInfo::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_VOLTE_STATE:
            mHost->OnVoLteServiceStateChanged(IVoLteServiceState::Probe(obj.Get()));
            break;
        case IPhoneStateListener::LISTEN_OEM_HOOK_RAW_EVENT:
            {
                AutoPtr<IByteBuffer> buffer = IByteBuffer::Probe(obj.Get());
                AutoPtr<ArrayOf<Byte> > array;
                buffer->GetArray((ArrayOf<Byte>**)&array);

                mHost->OnOemHookRawEvent(array);
            }
            break;
    }

    return NOERROR;
}

//==============================================================================
// CPhoneStateListener
//==============================================================================

CPhoneStateListener::CPhoneStateListener()
    : mSubId(0)
{
}

ECode CPhoneStateListener::constructor()
{
    return constructor(ISubscriptionManager::DEFAULT_SUB_ID, Looper::GetMyLooper());
}

ECode CPhoneStateListener::constructor(
    /* [in] */ ILooper* looper)
{
    return constructor(ISubscriptionManager::DEFAULT_SUB_ID, looper);
}

ECode CPhoneStateListener::constructor(
    /* [in] */ Int64 subId)
{
    return constructor(subId, Looper::GetMyLooper());
}

ECode CPhoneStateListener::constructor(
    /* [in] */ Int64 subId,
    /* [in] */ ILooper* looper)
{
    mSubId = subId;
    mHandler = new MyHandler(this, looper);
    return NOERROR;
}

ECode CPhoneStateListener::OnServiceStateChanged(
    /* [in] */ IServiceState* serviceState)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnSignalStrengthChanged(
    /* [in] */ Int32 asu)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnMessageWaitingIndicatorChanged(
    /* [in] */ Boolean mwi)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnCallForwardingIndicatorChanged(
    /* [in] */ Boolean cfi)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnCellLocationChanged(
    /* [in] */ ICellLocation* location)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnCallStateChanged(
    /* [in] */ Int32 state,
    /* [in] */ const String& incomingNumber)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnDataConnectionStateChanged(
    /* [in] */ Int32 state)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnDataConnectionStateChanged(
    /* [in] */ Int32 state,
    /* [in] */ Int32 networkType)
{
   // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnDataActivity(
    /* [in] */ Int32 direction)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnSignalStrengthsChanged(
    /* [in] */ ISignalStrength* signalStrength)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnOtaspChanged(
    /* [in] */ Int32 otaspMode)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnCellInfoChanged(
    /* [in] */ IList* cellInfo)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnPreciseCallStateChanged(
    /* [in] */ IPreciseCallState* callState)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnPreciseDataConnectionStateChanged(
    /* [in] */ IPreciseDataConnectionState* dataConnectionState)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnDataConnectionRealTimeInfoChanged(
    /* [in] */ IDataConnectionRealTimeInfo* dcRtInfo)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnVoLteServiceStateChanged(
    /* [in] */ IVoLteServiceState* stateInfo)
{
    // default implementation empty
    return NOERROR;
}

ECode CPhoneStateListener::OnOemHookRawEvent(
    /* [in] */ ArrayOf<Byte>* rawData)
{
    // default implementation empty
    return NOERROR;
}

} // namespace Telephony
} // namespace Droid
} // namespace Elastos

