
#include "elastos/droid/net/wifi/SupplicantStateTracker.h"
#include "elastos/droid/net/wifi/SupplicantStateHelper.h"
#include "elastos/droid/net/wifi/WifiMonitor.h"
#include "elastos/droid/net/wifi/WifiStateMachine.h"
#ifdef DROID_CORE
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/os/CUserHandleHelper.h"
#include "elastos/droid/net/wifi/CStateChangeResult.h"
#include "elastos/droid/net/wifi/CSupplicantState.h"
#endif
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::IUserHandle;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

const String SupplicantStateTracker::TAG("SupplicantStateTracker");
const Boolean SupplicantStateTracker::DBG = FALSE;
const Int32 SupplicantStateTracker::MAX_RETRIES_ON_AUTHENTICATION_FAILURE;


//==============================================================
// SupplicantStateTracker::DefaultState
//==============================================================
ECode SupplicantStateTracker::DefaultState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    return NOERROR;
}

ECode SupplicantStateTracker::DefaultState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    Int32 what;
    msg->GetWhat(&what);

    if (SupplicantStateTracker::DBG)
        Logger::D(SupplicantStateTracker::TAG, "DefaultState msg: %08x", what);

    switch (what) {
        case WifiMonitor::AUTHENTICATION_FAILURE_EVENT:
            mOwner->mAuthenticationFailuresCount++;
            mOwner->mAuthFailureInSupplicantBroadcast = TRUE;
            break;
        case WifiMonitor::SUPPLICANT_STATE_CHANGE_EVENT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IStateChangeResult> stateChangeResult = IStateChangeResult::Probe(obj.Get());
            SupplicantState state;
            stateChangeResult->GetSupplicantState(&state);
            mOwner->SendSupplicantStateChangedBroadcast(state, mOwner->mAuthFailureInSupplicantBroadcast);
            mOwner->mAuthFailureInSupplicantBroadcast = FALSE;
            mOwner->TransitionOnSupplicantStateChange(stateChangeResult);
            break;
        }
        case WifiStateMachine::CMD_RESET_SUPPLICANT_STATE:
            mOwner->TransitionTo(mOwner->mUninitializedState);
            break;
        case IWifiManager::CONNECT_NETWORK:
            if (DBG) Logger::D("SupplicantStateTracker::DefaultState", "ProcessMessage CONNECT_NETWORK");
            mOwner->mNetworksDisabledDuringConnect = TRUE;
            break;
        default:
            Logger::E(TAG, "Ignoring %p", msg);
            break;
    }
    *result = HANDLED;
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::UninitializedState
//==============================================================
ECode SupplicantStateTracker::UninitializedState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::InactiveState
//==============================================================
ECode SupplicantStateTracker::InactiveState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::DisconnectedState
//==============================================================
ECode SupplicantStateTracker::DisconnectedState::Enter()
{
    if (DBG) Logger::D("SupplicantStateTracker::DisconnectedState", "Enter");
    /* If a disconnect event happens after authentication failure
     * exceeds maximum retries, disable the network
     */
    AutoPtr<IMessage> message = mOwner->GetCurrentMessage();
    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    AutoPtr<IStateChangeResult> stateChangeResult = IStateChangeResult::Probe(obj.Get());
    if (mOwner->mAuthenticationFailuresCount >= MAX_RETRIES_ON_AUTHENTICATION_FAILURE) {
        Int32 networkId;
        stateChangeResult->GetNetworkId(&networkId);
        Logger::D(TAG, "Failed to authenticate, disabling network %d", networkId);
        mOwner->HandleNetworkConnectionFailure(networkId);
        mOwner->mAuthenticationFailuresCount = 0;
    }
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::ScanState
//==============================================================
ECode SupplicantStateTracker::ScanState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::HandshakeState
//==============================================================
const Int32 SupplicantStateTracker::HandshakeState::MAX_SUPPLICANT_LOOP_ITERATIONS;
ECode SupplicantStateTracker::HandshakeState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    mLoopDetectIndex = 0;
    mLoopDetectCount = 0;
    return NOERROR;
}

ECode SupplicantStateTracker::HandshakeState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    Int32 what;
    msg->GetWhat(&what);
    if (SupplicantStateTracker::DBG)
        Logger::D(SupplicantStateTracker::TAG, "HandshakeState msg: %08x", what);
    switch (what) {
        case WifiMonitor::SUPPLICANT_STATE_CHANGE_EVENT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IStateChangeResult> stateChangeResult = IStateChangeResult::Probe(obj.Get());
            SupplicantState state;
            stateChangeResult->GetSupplicantState(&state);
            if (SupplicantStateHelper::IsHandshakeState(state)) {
                if (mLoopDetectIndex > state - SupplicantState_DISCONNECTED/*state.ordinal()*/) {
                    mLoopDetectCount++;
                }
                if (mLoopDetectCount > MAX_SUPPLICANT_LOOP_ITERATIONS) {
                    Int32 networkId;
                    stateChangeResult->GetNetworkId(&networkId);
                    Logger::D(TAG, "Supplicant loop detected, disabling network %d", networkId);
                    mOwner->HandleNetworkConnectionFailure(networkId);
                }
                mLoopDetectIndex = state - SupplicantState_DISCONNECTED;//state.ordinal();
                mOwner->SendSupplicantStateChangedBroadcast(state,
                        mOwner->mAuthFailureInSupplicantBroadcast);
            }
            else {
                //Have the DefaultState handle the transition
                *result = NOT_HANDLED;
                return NOERROR;
            }
            break;
        }
        default:
            *result = NOT_HANDLED;
            return NOERROR;
    }
    *result = HANDLED;
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::CompletedState
//==============================================================
ECode SupplicantStateTracker::CompletedState::Enter()
{
    if (DBG) Logger::D("SupplicantStateTracker::CompletedState", "Enter");
    /* Reset authentication failure count */
    mOwner->mAuthenticationFailuresCount = 0;
    if (mOwner->mNetworksDisabledDuringConnect) {
        mOwner->mWifiConfigStore->EnableAllNetworks();
        mOwner->mNetworksDisabledDuringConnect = FALSE;
    }
    return NOERROR;
}

ECode SupplicantStateTracker::CompletedState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    Int32 what;
    msg->GetWhat(&what);
    if (SupplicantStateTracker::DBG)
        Logger::D(SupplicantStateTracker::TAG, "CompletedState msg: %08x", what);
    switch (what) {
        case WifiMonitor::SUPPLICANT_STATE_CHANGE_EVENT: {
            if (DBG) Logger::D("SupplicantStateTracker::CompletedState",
                "ProcessMessage SUPPLICANT_STATE_CHANGE_EVENT");
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IStateChangeResult> stateChangeResult = IStateChangeResult::Probe(obj.Get());
            SupplicantState state;
            stateChangeResult->GetSupplicantState(&state);
            mOwner->SendSupplicantStateChangedBroadcast(state, mOwner->mAuthFailureInSupplicantBroadcast);
            /* Ignore any connecting state in completed state. Group re-keying
             * events and other auth events that do not affect connectivity are
             * ignored
             */
            if (SupplicantStateHelper::IsConnecting(state)) {
                break;
            }
            mOwner->TransitionOnSupplicantStateChange(stateChangeResult);
            break;
        }
        case WifiStateMachine::CMD_RESET_SUPPLICANT_STATE:
            if (DBG) Logger::D("SupplicantStateTracker::CompletedState",
                "ProcessMessage CMD_RESET_SUPPLICANT_STATE");
            mOwner->SendSupplicantStateChangedBroadcast(SupplicantState_DISCONNECTED, FALSE);
            mOwner->TransitionTo(mOwner->mUninitializedState);
            break;
        default:
            *result = NOT_HANDLED;
            return NOERROR;
    }
    *result = HANDLED;
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker::DormantState
//==============================================================
ECode SupplicantStateTracker::DormantState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    return NOERROR;
}


//==============================================================
// SupplicantStateTracker
//==============================================================
SupplicantStateTracker::SupplicantStateTracker(
    /* [in] */ IContext* c,
    /* [in] */ WifiStateMachine* wsm,
    /* [in] */ WifiConfigStore* wcs,
    /* [in] */ ILooper* l)
    : StateMachine(TAG, l)
    , mWifiStateMachine(wsm)
    , mWifiConfigStore(wcs)
    , mAuthenticationFailuresCount(0)
    , mAuthFailureInSupplicantBroadcast(FALSE)
    , mNetworksDisabledDuringConnect(FALSE)
    , mContext(c)
    , mUninitializedState(new UninitializedState(this))
    , mDefaultState(new DefaultState(this))
    , mInactiveState(new InactiveState(this))
    , mDisconnectState(new DisconnectedState(this))
    , mScanState(new ScanState(this))
    , mHandshakeState(new HandshakeState(this))
    , mCompletedState(new CompletedState(this))
    , mDormantState(new DormantState(this))
{
    AddState(mDefaultState);
    AddState(mUninitializedState, mDefaultState);
    AddState(mInactiveState, mDefaultState);
    AddState(mDisconnectState, mDefaultState);
    AddState(mScanState, mDefaultState);
    AddState(mHandshakeState, mDefaultState);
    AddState(mCompletedState, mDefaultState);
    AddState(mDormantState, mDefaultState);

    SetInitialState(mUninitializedState);

    //start the state machine
    Start();
}

void SupplicantStateTracker::HandleNetworkConnectionFailure(
    /* [in] */ Int32 netId)
{
    /* If other networks disabled during connection, enable them */
    if (mNetworksDisabledDuringConnect) {
        mWifiConfigStore->EnableAllNetworks();
        mNetworksDisabledDuringConnect = FALSE;
    }
    /* Disable failed network */
    mWifiConfigStore->DisableNetwork(netId, IWifiConfiguration::DISABLED_AUTH_FAILURE);
}

void SupplicantStateTracker::TransitionOnSupplicantStateChange(
    /* [in] */ IStateChangeResult* stateChangeResult)
{
    SupplicantState supState;
    stateChangeResult->GetSupplicantState(&supState);

    // if (DBG) Logger::D(TAG, "Supplicant state: " + supState.toString() + "\n");

    switch (supState) {
        case SupplicantState_DISCONNECTED:
            TransitionTo(mDisconnectState);
            break;
        case SupplicantState_INTERFACE_DISABLED:
            //we should have received a disconnection already, do nothing
            break;
        case SupplicantState_SCANNING:
            TransitionTo(mScanState);
            break;
        case SupplicantState_AUTHENTICATING:
        case SupplicantState_ASSOCIATING:
        case SupplicantState_ASSOCIATED:
        case SupplicantState_FOUR_WAY_HANDSHAKE:
        case SupplicantState_GROUP_HANDSHAKE:
            TransitionTo(mHandshakeState);
            break;
        case SupplicantState_COMPLETED:
            TransitionTo(mCompletedState);
            break;
        case SupplicantState_DORMANT:
            TransitionTo(mDormantState);
            break;
        case SupplicantState_INACTIVE:
            TransitionTo(mInactiveState);
            break;
        case SupplicantState_UNINITIALIZED:
        case SupplicantState_INVALID:
            TransitionTo(mUninitializedState);
            break;
        default:
            Logger::E(TAG, "Unknown supplicant state %d", supState);
            break;
    }
}

void SupplicantStateTracker::SendSupplicantStateChangedBroadcast(
    /* [in] */ SupplicantState state,
    /* [in] */ Boolean failedAuth)
{
    AutoPtr<IIntent> intent;
    CIntent::New(IWifiManager::SUPPLICANT_STATE_CHANGED_ACTION, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT
            | IIntent::FLAG_RECEIVER_REPLACE_PENDING);

    AutoPtr<ISupplicantState> iss;
    CSupplicantState::New(state, (ISupplicantState**)&iss);
    intent->PutExtra(IWifiManager::EXTRA_NEW_STATE, IParcelable::Probe(iss.Get()));

    if (failedAuth) {
        intent->PutExtra(
            IWifiManager::EXTRA_SUPPLICANT_ERROR,
            IWifiManager::ERROR_AUTHENTICATING);
    }

    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    mContext->SendStickyBroadcastAsUser(intent, ALL);
}

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos
