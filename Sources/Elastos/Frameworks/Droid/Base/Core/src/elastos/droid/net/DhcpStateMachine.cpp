
#include "elastos/droid/net/DhcpStateMachine.h"
#ifdef DROID_CORE

#if 0 // TODO: Waiting for CPendingIntentHelper, CIntent, CIntentFilter
#include "app/CPendingIntentHelper.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#endif

#endif

#if 0 // TODO: Waiting for NetworkUtils
#include "net/NetworkUtils.h"
#endif
#include "elastos/droid/os/SystemClock.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::App::IPendingIntentHelper;
#if 0 // TODO: Waiting for CPendingIntentHelper, CIntent, CIntentFilter
using Elastos::Droid::App::CPendingIntentHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Net::NetworkUtils;
#endif
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Net {

const String DhcpStateMachine::TAG("DhcpStateMachine");
const Boolean DhcpStateMachine::DBG = FALSE;
const String DhcpStateMachine::WAKELOCK_TAG("DHCP");
const Int32 DhcpStateMachine::DHCP_RENEW;
const String DhcpStateMachine::ACTION_DHCP_RENEW("android.net.wifi.DHCP_RENEW");
const Int32 DhcpStateMachine::MIN_RENEWAL_TIME_SECS;
const Int32 DhcpStateMachine::BASE;
const Int32 DhcpStateMachine::CMD_START_DHCP;
const Int32 DhcpStateMachine::CMD_STOP_DHCP;
const Int32 DhcpStateMachine::CMD_RENEW_DHCP;
const Int32 DhcpStateMachine::CMD_PRE_DHCP_ACTION;
const Int32 DhcpStateMachine::CMD_POST_DHCP_ACTION;
const Int32 DhcpStateMachine::CMD_ON_QUIT;
const Int32 DhcpStateMachine::CMD_PRE_DHCP_ACTION_COMPLETE;
const Int32 DhcpStateMachine::DHCP_SUCCESS;
const Int32 DhcpStateMachine::DHCP_FAILURE;

#if 0 // TODO: Waiting for StateMachine
CAR_INTERFACE_IMPL(DhcpStateMachine, StateMachine, IDhcpStateMachine)
#else
CAR_INTERFACE_IMPL(DhcpStateMachine, Object, IDhcpStateMachine)
#endif

//==============================================================
// DhcpStateMachine::DefaultState
//==============================================================
ECode DhcpStateMachine::DefaultState::Exit()
{
    if (DBG) Logger::D(TAG, "DefaultState::Exit");
    // mOwner->mContext->UnregisterReceiver(mOwner->mBroadcastReceiver);
    return NOERROR;
}

ECode DhcpStateMachine::DefaultState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0
    // if (DBG) Log.d(TAG, getName() + message.toString() + "\n");
    if (DBG) Logger::D(TAG, "DefaultState::ProcessMessage");
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case CMD_RENEW_DHCP:
            Logger::E(TAG, "Error! Failed to handle a DHCP renewal on %s", mOwner->mInterfaceName.string());
            mOwner->mDhcpRenewWakeLock->ReleaseLock();
            break;
        default:
            // Log.e(TAG, "Error! unhandled message  " + message);
            break;
    }
    return HANDLED;
#endif
}


//==============================================================
// DhcpStateMachine::StoppedState
//==============================================================
ECode DhcpStateMachine::StoppedState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    if (DBG) Logger::D(TAG, "StoppedState::Enter");
    return NOERROR;
}

ECode DhcpStateMachine::StoppedState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0
    Boolean retValue = HANDLED;
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case CMD_START_DHCP:
            if (DBG) Logger::D("DhcpStateMachine::StoppedState", "ProcessMessage CMD_START_DHCP");
            if (mOwner->mRegisteredForPreDhcpNotification) {
                /* Notify controller before starting DHCP */
                mOwner->mController->SendMessage(CMD_PRE_DHCP_ACTION);
                mOwner->TransitionTo(mOwner->mWaitBeforeStartState);
            }
            else {
                if (mOwner->RunDhcp(DhcpAction_START)) {
                    mOwner->TransitionTo(mOwner->mRunningState);
                }
            }
            break;
        case CMD_STOP_DHCP:
            //ignore
            break;
        default:
            retValue = NOT_HANDLED;
            break;
    }
    *result = retValue;
    return NOERROR;
#endif
}


//==============================================================
// DhcpStateMachine::WaitBeforeStartState
//==============================================================
ECode DhcpStateMachine::WaitBeforeStartState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    if (DBG) Logger::D(TAG, "WaitBeforeStartState::Enter");
    return NOERROR;
}

ECode DhcpStateMachine::WaitBeforeStartState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0
    Boolean retValue = HANDLED;
    // if (DBG) Log.d(TAG, getName() + message.toString() + "\n");
    if (DBG) Logger::D(TAG, "WaitBeforeStartState::ProcessMessage");
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case CMD_PRE_DHCP_ACTION_COMPLETE:
            if (mOwner->RunDhcp(DhcpAction_START)) {
                mOwner->TransitionTo(mOwner->mRunningState);
            }
            else {
                mOwner->TransitionTo(mOwner->mStoppedState);
            }
            break;
        case CMD_STOP_DHCP:
            mOwner->TransitionTo(mOwner->mStoppedState);
            break;
        case CMD_START_DHCP:
            //ignore
            break;
        default:
            retValue = NOT_HANDLED;
            break;
    }
    *result = retValue;
    return NOERROR;
#endif
}


//==============================================================
// DhcpStateMachine::RunningState
//==============================================================
ECode DhcpStateMachine::RunningState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    if (DBG) Logger::D(TAG, "RunningState::Enter");
    return NOERROR;
}

ECode DhcpStateMachine::RunningState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0
    Boolean retValue = HANDLED;
    // if (DBG) Log.d(TAG, getName() + message.toString() + "\n");
    if (DBG) Logger::D(TAG, "RunningState::ProcessMessage");
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case CMD_STOP_DHCP:
            mOwner->mAlarmManager->Cancel(mOwner->mDhcpRenewalIntent);
            if (!NetworkUtils::StopDhcp(mOwner->mInterfaceName)) {
                Logger::E(TAG, "Failed to stop Dhcp on %s", mOwner->mInterfaceName.string());
            }
            mOwner->TransitionTo(mOwner->mStoppedState);
            break;
        case CMD_RENEW_DHCP:
            if (mOwner->mRegisteredForPreDhcpNotification) {
                /* Notify controller before starting DHCP */
                mOwner->mController->SendMessage(CMD_PRE_DHCP_ACTION);
                mOwner->TransitionTo(mOwner->mWaitBeforeRenewalState);
                //mDhcpRenewWakeLock is released in WaitBeforeRenewalState
            }
            else {
                if (!mOwner->RunDhcp(DhcpAction_RENEW)) {
                    mOwner->TransitionTo(mOwner->mStoppedState);
                }
                mOwner->mDhcpRenewWakeLock->ReleaseLock();
            }
            break;
        case CMD_START_DHCP:
            //ignore
            break;
        default:
            retValue = NOT_HANDLED;
    }
    *result = retValue;
    return NOERROR;
#endif
}


//==============================================================
// DhcpStateMachine::WaitBeforeRenewalState
//==============================================================
ECode DhcpStateMachine::WaitBeforeRenewalState::Enter()
{
    // if (DBG) Log.d(TAG, getName() + "\n");
    if (DBG) Logger::D(TAG, "WaitBeforeRenewalState::Enter");
    return NOERROR;
}

ECode DhcpStateMachine::WaitBeforeRenewalState::ProcessMessage(
    /* [in] */ IMessage* msg,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0
    Boolean retValue = HANDLED;
    // if (DBG) Log.d(TAG, getName() + message.toString() + "\n");
    if (DBG) Logger::D(TAG, "WaitBeforeRenewalState::ProcessMessage");
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case CMD_STOP_DHCP:
            mOwner->mAlarmManager->Cancel(mOwner->mDhcpRenewalIntent);
            if (!NetworkUtils::StopDhcp(mOwner->mInterfaceName)) {
                Logger::E(TAG, "Failed to stop Dhcp on %s", mOwner->mInterfaceName.string());
            }
            mOwner->TransitionTo(mOwner->mStoppedState);
            break;
        case CMD_PRE_DHCP_ACTION_COMPLETE:
            if (mOwner->RunDhcp(mOwner->DhcpAction_RENEW)) {
               mOwner->TransitionTo(mOwner->mRunningState);
            }
            else {
               mOwner->TransitionTo(mOwner->mStoppedState);
            }
            break;
        case CMD_START_DHCP:
            //ignore
            break;
        default:
            retValue = NOT_HANDLED;
            break;
    }
    *result = retValue;
    return NOERROR;
#endif
}

ECode DhcpStateMachine::WaitBeforeRenewalState::Exit()
{
    if (DBG) Logger::D(TAG, "WaitBeforeRenewalState::Exit");
    mOwner->mDhcpRenewWakeLock->ReleaseLock();
    return NOERROR;
}

//==============================================================
// DhcpStateMachine::MyBroadcastReceiver
//==============================================================
DhcpStateMachine::MyBroadcastReceiver::MyBroadcastReceiver(
   /* [in] */ DhcpStateMachine* owner)
   : mOwner(owner)
{}

//@Override
ECode DhcpStateMachine::MyBroadcastReceiver::OnReceive(
   /* [in] */ IContext* ctx,
   /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
#if 0

   //DHCP renew
//    if (DBG) Log.d(TAG, "Sending a DHCP renewal " + this);
   //Lock released after 40s in worst case scenario
   mOwner->mDhcpRenewWakeLock->AcquireLock(40000);
   mOwner->SendMessage(CMD_RENEW_DHCP);
   return NOERROR;
#endif
}

//==============================================================
// DhcpStateMachine::WaitBeforeRenewalState
//==============================================================
DhcpStateMachine::DhcpStateMachine(
    /* [in] */ IContext* context,
    /* [in] */ IStateMachine* controller,
    /* [in] */ const String& intf)
#if 0 // TODO: Waiting for StateMachine
    : StateMachine(TAG)
    , mController(controller)
#else
    : mController(controller)
#endif
    , mContext(context)
    , mInterfaceName(intf)
    , mRegisteredForPreDhcpNotification(FALSE)
{
#if 0
    mDefaultState = new DefaultState(this);
    mStoppedState = new StoppedState(this);
    mWaitBeforeStartState = new WaitBeforeStartState(this);
    mRunningState = new RunningState(this);
    mWaitBeforeRenewalState = new WaitBeforeRenewalState(this);

    mContext->GetSystemService(IContext::ALARM_SERVICE, (IInterface**)&mAlarmManager);
    AutoPtr<IIntent> dhcpRenewalIntent;
    CIntent::New(ACTION_DHCP_RENEW, NULL, (IIntent**)&dhcpRenewalIntent);
    AutoPtr<IPendingIntentHelper> intentHelper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&intentHelper);
    intentHelper->GetBroadcast(mContext, DHCP_RENEW, dhcpRenewalIntent, 0, (IPendingIntent**)&mDhcpRenewalIntent);

    AutoPtr<IPowerManager> powerManager;
    mContext->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&powerManager);
    powerManager->NewWakeLock(IPowerManager::PARTIAL_WAKE_LOCK, WAKELOCK_TAG, (IPowerManagerWakeLock**)&mDhcpRenewWakeLock);
    mDhcpRenewWakeLock->SetReferenceCounted(FALSE);

    mBroadcastReceiver = new MyBroadcastReceiver(this);
    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New(ACTION_DHCP_RENEW, (IIntentFilter**)&intentFilter);
    AutoPtr<IIntent> intent;
    mContext->RegisterReceiver(mBroadcastReceiver, intentFilter, (IIntent**)&intent);

    AddState(mDefaultState);
    AddState(mStoppedState, mDefaultState);
    AddState(mWaitBeforeStartState, mDefaultState);
    AddState(mRunningState, mDefaultState);
    AddState(mWaitBeforeRenewalState, mDefaultState);

    SetInitialState(mStoppedState);
#endif
}

AutoPtr<IDhcpStateMachine> DhcpStateMachine::MakeDhcpStateMachine(
    /* [in] */ IContext* context,
    /* [in] */ IStateMachine* controller,
    /* [in] */ const String& intf)
{
    return NULL;
#if 0
    AutoPtr<DhcpStateMachine> dsm = new DhcpStateMachine(context, controller, intf);
    dsm->Start();
    return dsm->Probe(EIID_IDhcpStateMachine);
#endif
}

/**
* This sends a notification right before DHCP request/renewal so that the
* controller can do certain actions before DHCP packets are sent out.
* When the controller is ready, it sends a CMD_PRE_DHCP_ACTION_COMPLETE message
* to indicate DHCP can continue
*
* This is used by Wifi at this time for the purpose of doing BT-Wifi coex
* handling during Dhcp
*/
ECode DhcpStateMachine::RegisterForPreDhcpNotification()
{
    mRegisteredForPreDhcpNotification = TRUE;
    return NOERROR;
}

/**
* Quit the DhcpStateMachine.
*
* @hide
*/
ECode DhcpStateMachine::DoQuit()
{
    return E_NOT_IMPLEMENTED;
#if 0
    Quit();
#endif
}

void DhcpStateMachine::OnQuitting()
{
#if 0
    mController->SendMessage(CMD_ON_QUIT);
#endif
}

Boolean DhcpStateMachine::RunDhcp(
    /* [in] */ DhcpAction dhcpAction)
{
    return FALSE;
#if 0
    Boolean success = FALSE;
    AutoPtr<DhcpInfoInternal> dhcpInfoInternal = new DhcpInfoInternal();

    if (dhcpAction == DhcpAction_START) {
            /* Stop any existing DHCP daemon before starting new */
        NetworkUtils::StopDhcp(mInterfaceName);
        if (DBG) Logger::D(TAG, "DHCP request on %s", mInterfaceName.string());
        success = NetworkUtils::RunDhcp(mInterfaceName, dhcpInfoInternal);
        mDhcpInfo = dhcpInfoInternal;
    }
    else if (dhcpAction == DhcpAction_RENEW) {
        if (DBG) Logger::D(TAG, "DHCP renewal on %s", mInterfaceName.string());
        success = NetworkUtils::RunDhcpRenew(mInterfaceName, dhcpInfoInternal);
        dhcpInfoInternal->UpdateFromDhcpRequest(mDhcpInfo);
    }

    if (success) {
        if (DBG) Logger::D(TAG, "DHCP succeeded on %s", mInterfaceName.string());
        Int64 leaseDuration = dhcpInfoInternal->mLeaseDuration; //int to long conversion

        //Sanity check for renewal
        if (leaseDuration >= 0) {
            //TODO: would be good to notify the user that his network configuration is
            //bad and that the device cannot renew below MIN_RENEWAL_TIME_SECS
            if (leaseDuration < MIN_RENEWAL_TIME_SECS) {
                leaseDuration = MIN_RENEWAL_TIME_SECS;
            }
            //Do it a bit earlier than half the lease duration time
            //to beat the native DHCP client and avoid extra packets
            //48% for one hour lease time = 29 minutes
            mAlarmManager->Set(IAlarmManager::ELAPSED_REALTIME_WAKEUP,
                    SystemClock::GetElapsedRealtime() +
                    leaseDuration * 480, //in milliseconds
                    mDhcpRenewalIntent);
        }
        else {
            //infinite lease time, no renewal needed
        }

        AutoPtr<IMessage> m = mController->ObtainMessage(CMD_POST_DHCP_ACTION, DHCP_SUCCESS, 0, dhcpInfoInternal);
        mController->SendMessage(m);
    }
    else {
 //        Log.e(TAG, "DHCP failed on " + mInterfaceName + ": " + NetworkUtils.getDhcpError());
        NetworkUtils::StopDhcp(mInterfaceName);
        AutoPtr<IMessage> m = mController->ObtainMessage(CMD_POST_DHCP_ACTION, DHCP_FAILURE, 0);
        mController->SendMessage(m);
    }
    return success;
#endif
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
