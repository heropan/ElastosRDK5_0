
#include "connectivity/CTethering.h"
#include "net/NetworkUtils.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CStringWrapper;
using Elastos::Core::CInteger32;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Net::IInet4Address;
using Elastos::Net::IInetAddress;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Hardware::Usb::IUsbManager;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Net::ILinkProperties;
using Elastos::Droid::Net::NetworkInfoDetailedState;
using Elastos::Droid::Net::NetworkInfoDetailedState_FAILED;
using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Net::IInterfaceConfiguration;
using Elastos::Droid::Net::ILinkAddress;
using Elastos::Droid::Net::CLinkAddress;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsSecure;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Internal::Utility::IState;
using Elastos::Droid::Internal::Telephony::IPhoneConstants;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

//===========================================================================
//  CTethering::StateReceiver
//===========================================================================
ECode CTethering::StateReceiver::OnReceive(
    /* [in] */ IContext* content,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IUsbManager::ACTION_USB_STATE)) {
        {
            Object::Autolock lock(mOwner->mPublicSync);
            Boolean usbConnected;
            intent->GetBooleanExtra(IUsbManager::USB_CONNECTED, FALSE, &usbConnected);
            intent->GetBooleanExtra(IUsbManager::USB_FUNCTION_RNDIS, FALSE, &mOwner->mRndisEnabled);
            // start tethering if we have a request pending
            if (usbConnected && mOwner->mRndisEnabled && mOwner->mUsbTetherRequested) {
                mOwner->TetherUsb(TRUE);
            }
            mOwner->mUsbTetherRequested = FALSE;
        }
    }
    else if (action.Equals(IConnectivityManager::CONNECTIVITY_ACTION)) {
        AutoPtr<IParcelable> obj;
        intent->GetParcelableExtra(
                IConnectivityManager::EXTRA_NETWORK_INFO, (IParcelable**)&obj);
        AutoPtr<INetworkInfo> networkInfo = INetworkInfo::Probe(obj);
        NetworkInfoDetailedState state;
        if (networkInfo != NULL &&
                (networkInfo->GetDetailedState(&state), state != NetworkInfoDetailedState_FAILED)) {
            if (VDBG) Logger::D(TAG, "Tethering got CONNECTIVITY_ACTION");
            mOwner->mTetherMasterSM->SendMessage(TetherMasterSM::CMD_UPSTREAM_CHANGED);
        }
    }
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherInterfaceSM::InitialState
//===========================================================================
ECode CTethering::TetherInterfaceSM::InitialState::Enter()
{
    mOwner->SetAvailable(TRUE);
    mOwner->SetTethered(FALSE);
    mOwner->mOwner->SendTetherStateChangedBroadcast();
    return NOERROR;
}

ECode CTethering::TetherInterfaceSM::InitialState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 what;
    message->GetWhat(&what);
    if (DBG) {
        Logger::D(CTethering::TAG, "InitialState.processMessage what=%d", what);
    }
    Boolean retValue = TRUE;
    switch (what) {
        case CMD_TETHER_REQUESTED:
            mOwner->SetLastError(IConnectivityManager::TETHER_ERROR_NO_ERROR);
            mOwner->mOwner->mTetherMasterSM->SendMessage(TetherMasterSM::CMD_TETHER_MODE_REQUESTED,
                    mOwner);
            mOwner->TransitionTo(mOwner->mStartingState);
            break;
        case CMD_INTERFACE_DOWN:
            mOwner->TransitionTo(mOwner->mUnavailableState);
            break;
        default:
            retValue = FALSE;
            break;
    }
    *value = retValue;
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherInterfaceSM::StartingState
//===========================================================================
ECode CTethering::TetherInterfaceSM::StartingState::Enter()
{
    mOwner->SetAvailable(FALSE);
    if (mOwner->mUsb) {
        if (!mOwner->mOwner->ConfigureUsbIface(TRUE)) {
            mOwner->mOwner->mTetherMasterSM->SendMessage(TetherInterfaceSM::CMD_TETHER_UNREQUESTED,
                    mOwner);
            mOwner->SetLastError(IConnectivityManager::TETHER_ERROR_IFACE_CFG_ERROR);

            mOwner->TransitionTo(mOwner->mInitialState);
            return NOERROR;
        }
    }
    mOwner->mOwner->SendTetherStateChangedBroadcast();

    // Skipping StartingState
    mOwner->TransitionTo(mOwner->mTetheredState);
    return NOERROR;
}

ECode CTethering::TetherInterfaceSM::StartingState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 what;
    message->GetWhat(&what);
    if (DBG) {
        Logger::D(CTethering::TAG, "StartingState.processMessage what=%d", what);
    }
    Boolean retValue = TRUE;
    switch (what) {
        // maybe a parent class?
        case CMD_TETHER_UNREQUESTED:
            mOwner->mOwner->mTetherMasterSM->SendMessage(TetherMasterSM::CMD_TETHER_MODE_UNREQUESTED,
                    mOwner);
            if (mOwner->mUsb) {
                if (!mOwner->mOwner->ConfigureUsbIface(FALSE)) {
                    mOwner->SetLastErrorAndTransitionToInitialState(
                            IConnectivityManager::TETHER_ERROR_IFACE_CFG_ERROR);
                    break;
                }
            }
            mOwner->TransitionTo(mOwner->mInitialState);
            break;
        case CMD_CELL_DUN_ERROR:
        case CMD_IP_FORWARDING_ENABLE_ERROR:
        case CMD_IP_FORWARDING_DISABLE_ERROR:
        case CMD_START_TETHERING_ERROR:
        case CMD_STOP_TETHERING_ERROR:
        case CMD_SET_DNS_FORWARDERS_ERROR:
            mOwner->SetLastErrorAndTransitionToInitialState(
                    IConnectivityManager::TETHER_ERROR_MASTER_ERROR);
            break;
        case CMD_INTERFACE_DOWN:
            mOwner->mOwner->mTetherMasterSM->SendMessage(TetherMasterSM::CMD_TETHER_MODE_UNREQUESTED,
                    mOwner);
            mOwner->TransitionTo(mOwner->mUnavailableState);
            break;
        default:
            retValue = FALSE;
    }
    *value = retValue;
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherInterfaceSM::TetheredState
//===========================================================================
ECode CTethering::TetherInterfaceSM::TetheredState::Enter()
{
    ECode ec = mOwner->mOwner->mNMService->TetherInterface(mOwner->mIfaceName);
    if(FAILED(ec)){
        Logger::E(CTethering::TAG, "Error Tethering: 0x%x", ec);
        mOwner->SetLastError(IConnectivityManager::TETHER_ERROR_TETHER_IFACE_ERROR);
        mOwner->TransitionTo(mOwner->mInitialState);
        return ec;
    }
    if (DBG) Logger::D(CTethering::TAG, "Tethered %s", mOwner->mIfaceName.string());
    mOwner->SetAvailable(FALSE);
    mOwner->SetTethered(TRUE);
    mOwner->mOwner->SendTetherStateChangedBroadcast();
    return NOERROR;
}

void CTethering::TetherInterfaceSM::TetheredState::CleanupUpstream()
{
    ECode ec = NOERROR;
    if (!mOwner->mMyUpstreamIfaceName.IsNull()) {
        // note that we don't care about errors here.
        // sometimes interfaces are gone before we get
        // to remove their rules, which generates errors.
        // just do the best we can.
        // about to tear down NAT; gather remaining statistics
        ec = mOwner->mOwner->mStatsService->ForceUpdate();
        if (FAILED(ec)) {
            if (VDBG) Logger::E(CTethering::TAG, "Exception in forceUpdate: %x", ec);
        }
        ec = mOwner->mOwner->mNMService->DisableNat(mOwner->mIfaceName, mOwner->mMyUpstreamIfaceName);
        if (FAILED(ec)) {
            if (VDBG) Logger::E(CTethering::TAG, "Exception in disableNat: %x", ec);
        }
        mOwner->mMyUpstreamIfaceName = NULL;
    }
}

ECode CTethering::TetherInterfaceSM::TetheredState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    Int32 what;
    message->GetWhat(&what);
    if (DBG) {
        Logger::D(CTethering::TAG, "TetheredState.processMessage what=%d", what);
    }
    Boolean retValue = TRUE;
    Boolean error = FALSE;
    ECode ec = NOERROR;
    switch (what) {
        case CMD_TETHER_UNREQUESTED:
        case CMD_INTERFACE_DOWN:
            CleanupUpstream();
            ec = mOwner->mOwner->mNMService->UntetherInterface(mOwner->mIfaceName);
            if (FAILED(ec)) {
                mOwner->SetLastErrorAndTransitionToInitialState(
                        IConnectivityManager::TETHER_ERROR_UNTETHER_IFACE_ERROR);
                break;
            }
            mOwner->mOwner->mTetherMasterSM->SendMessage(TetherMasterSM::CMD_TETHER_MODE_UNREQUESTED,
                    mOwner);
            if (what == CMD_TETHER_UNREQUESTED) {
                if (mOwner->mUsb) {
                    if (!mOwner->mOwner->ConfigureUsbIface(FALSE)) {
                        mOwner->SetLastError(
                                IConnectivityManager::TETHER_ERROR_IFACE_CFG_ERROR);
                    }
                }
                mOwner->TransitionTo(mOwner->mInitialState);
            }
            else if (what == CMD_INTERFACE_DOWN) {
                mOwner->TransitionTo(mOwner->mUnavailableState);
            }
            if (DBG) Logger::D(CTethering::TAG, "Untethered %s", mOwner->mIfaceName.string());
            break;

        case CMD_TETHER_CONNECTION_CHANGED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            String newUpstreamIfaceName;
            if (obj != NULL) ICharSequence::Probe(obj)->ToString(&newUpstreamIfaceName);
            if ((mOwner->mMyUpstreamIfaceName.IsNull() && newUpstreamIfaceName.IsNull()) ||
                    (!mOwner->mMyUpstreamIfaceName.IsNull() &&
                    mOwner->mMyUpstreamIfaceName.Equals(newUpstreamIfaceName))) {
                if (VDBG) Logger::D(CTethering::TAG, "Connection changed noop - dropping");
                break;
            }
            CleanupUpstream();
            if (!newUpstreamIfaceName.IsNull()) {
                ec = mOwner->mOwner->mNMService->EnableNat(mOwner->mIfaceName, newUpstreamIfaceName);
                if (FAILED(ec)) {
                    Logger::E(CTethering::TAG, "Exception enabling Nat: %d", ec);
                    ec = mOwner->mOwner->mNMService->UntetherInterface(mOwner->mIfaceName);
                    if (FAILED(ec)) {}

                    mOwner->SetLastError(IConnectivityManager::TETHER_ERROR_ENABLE_NAT_ERROR);
                    mOwner->TransitionTo(mOwner->mInitialState);
                    *value = TRUE;
                    return ec;
                }
            }
            mOwner->mMyUpstreamIfaceName = newUpstreamIfaceName;
            break;
        }
        case CMD_CELL_DUN_ERROR:
        case CMD_IP_FORWARDING_ENABLE_ERROR:
        case CMD_IP_FORWARDING_DISABLE_ERROR:
        case CMD_START_TETHERING_ERROR:
        case CMD_STOP_TETHERING_ERROR:
        case CMD_SET_DNS_FORWARDERS_ERROR:
            error = TRUE;
            // fall through
        case CMD_TETHER_MODE_DEAD:
            CleanupUpstream();
            ec = mOwner->mOwner->mNMService->UntetherInterface(mOwner->mIfaceName);
            if (FAILED(ec)) {
                mOwner->SetLastErrorAndTransitionToInitialState(
                        IConnectivityManager::TETHER_ERROR_UNTETHER_IFACE_ERROR);
                break;
            }
            if (error) {
                mOwner->SetLastErrorAndTransitionToInitialState(
                        IConnectivityManager::TETHER_ERROR_MASTER_ERROR);
                break;
            }
            if (DBG) {
                Logger::D(CTethering::TAG, "Tether lost upstream connection %s", mOwner->mIfaceName.string());
            }
            mOwner->mOwner->SendTetherStateChangedBroadcast();
            if (mOwner->mUsb) {
                if (!mOwner->mOwner->ConfigureUsbIface(FALSE)) {
                    mOwner->SetLastError(IConnectivityManager::TETHER_ERROR_IFACE_CFG_ERROR);
                }
            }
            mOwner->TransitionTo(mOwner->mInitialState);
            break;
        default:
            retValue = FALSE;
            break;
    }
    *value = retValue;
    return ec;
}


//===========================================================================
//  CTethering::TetherInterfaceSM::UnavailableState
//===========================================================================
ECode CTethering::TetherInterfaceSM::UnavailableState::Enter()
{
    mOwner->SetAvailable(FALSE);
    mOwner->SetLastError(IConnectivityManager::TETHER_ERROR_NO_ERROR);
    mOwner->SetTethered(FALSE);
    mOwner->mOwner->SendTetherStateChangedBroadcast();
    return NOERROR;
}

ECode CTethering::TetherInterfaceSM::UnavailableState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    Boolean retValue = TRUE;
    Int32 what;
    message->GetWhat(&what);
    switch (what) {
        case CMD_INTERFACE_UP:
            mOwner->TransitionTo(mOwner->mInitialState);
            break;
        default:
            retValue = FALSE;
            break;
    }
    *value = retValue;
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherInterfaceSM
//===========================================================================
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_MODE_DEAD;
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_REQUESTED;
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_UNREQUESTED;
const Int32 CTethering::TetherInterfaceSM::CMD_INTERFACE_DOWN;
const Int32 CTethering::TetherInterfaceSM::CMD_INTERFACE_UP;
const Int32 CTethering::TetherInterfaceSM::CMD_CELL_DUN_ERROR;
const Int32 CTethering::TetherInterfaceSM::CMD_IP_FORWARDING_ENABLE_ERROR;
const Int32 CTethering::TetherInterfaceSM::CMD_IP_FORWARDING_DISABLE_ERROR;
const Int32 CTethering::TetherInterfaceSM::CMD_START_TETHERING_ERROR;
const Int32 CTethering::TetherInterfaceSM::CMD_STOP_TETHERING_ERROR;
const Int32 CTethering::TetherInterfaceSM::CMD_SET_DNS_FORWARDERS_ERROR;
const Int32 CTethering::TetherInterfaceSM::CMD_TETHER_CONNECTION_CHANGED;

CTethering::TetherInterfaceSM::TetherInterfaceSM(
    /* [in] */ const String& name,
    /* [in] */ ILooper* looper,
    /* [in] */ Boolean usb,
    /* [in] */ CTethering* owner)
    : StateMachine(name, looper)
    , mLastError(0)
    , mIfaceName(name)
    , mUsb(usb)
    , mAvailable(FALSE)
    , mTethered(FALSE)
    , mOwner(owner)
{
    SetLastError(IConnectivityManager::TETHER_ERROR_NO_ERROR);

    mInitialState = new InitialState(this);
    AddState(mInitialState);
    mStartingState = new StartingState(this);
    AddState(mStartingState);
    mTetheredState = new TetheredState(this);
    AddState(mTetheredState);
    mUnavailableState = new UnavailableState(this);
    AddState(mUnavailableState);

    SetInitialState(mInitialState);
}

CAR_INTERFACE_IMPL(CTethering::TetherInterfaceSM, IInterface)

String CTethering::TetherInterfaceSM::ToString()
{
    StringBuilder sb;
    sb += mIfaceName;
    sb += "-";
    AutoPtr<IState> current = GetCurrentState();
    if (current == mInitialState) sb += "InitialState";
    if (current == mStartingState) sb += "StartingState";
    if (current == mTetheredState) sb += "TetheredState";
    if (current == mUnavailableState) sb += "UnavailableState";
    if (mAvailable) sb += " - Available";
    if (mTethered) sb += " - Tethered";
    sb += " - lastError =";
    sb += mLastError;
    return sb.ToString();
}

Int32 CTethering::TetherInterfaceSM::GetLastError()
{
    Object::Autolock lock(mOwner->mPublicSync);
    return mLastError;
}

void CTethering::TetherInterfaceSM::SetLastError(
    /* [in] */ Int32 error)
{
    Object::Autolock lock(mOwner->mPublicSync);

    mLastError = error;

    if (IsErrored()) {
        if (mUsb) {
            // note everything's been unwound by this point so nothing to do on
            // further error..
            mOwner->ConfigureUsbIface(FALSE);
        }
    }
}

Boolean CTethering::TetherInterfaceSM::IsAvailable()
{
    Object::Autolock lock(mOwner->mPublicSync);
    return mAvailable;
}

void CTethering::TetherInterfaceSM::SetAvailable(
    /* [in] */ Boolean available)
{
    Object::Autolock lock(mOwner->mPublicSync);
    mAvailable = available;
}

Boolean CTethering::TetherInterfaceSM::IsTethered()
{
    Object::Autolock lock(mOwner->mPublicSync);
    return mTethered;
}

void CTethering::TetherInterfaceSM::SetTethered(
    /* [in] */ Boolean tethered)
{
    Object::Autolock lock(mOwner->mPublicSync);
    mTethered = tethered;
}

Boolean CTethering::TetherInterfaceSM::IsErrored()
{
    Object::Autolock lock(mOwner->mPublicSync);
    return mLastError != IConnectivityManager::TETHER_ERROR_NO_ERROR;
}

void CTethering::TetherInterfaceSM::SetLastErrorAndTransitionToInitialState(
    /* [in] */ Int32 error)
{
    SetLastError(error);
    TransitionTo(mInitialState);
}


//===========================================================================
//  CTethering::TetherMasterSM::TetherMasterUtilState
//===========================================================================
const Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TRY_TO_SETUP_MOBILE_CONNECTION = TRUE;
const Boolean CTethering::TetherMasterSM::TetherMasterUtilState::WAIT_FOR_NETWORK_TO_SETTLE = FALSE;

ECode CTethering::TetherMasterSM::TetherMasterUtilState::ProcessMessage(
    /* [in] */ IMessage* m,
    /* [out]*/ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

String CTethering::TetherMasterSM::TetherMasterUtilState::EnableString(
    /* [in] */ Int32 apnType)
{
    switch (apnType) {
    case IConnectivityManager::TYPE_MOBILE_DUN:
        // TODO:
        // return Phone::FEATURE_ENABLE_DUN_ALWAYS;
        return String("enableDUNAlways");
    case IConnectivityManager::TYPE_MOBILE:
    case IConnectivityManager::TYPE_MOBILE_HIPRI:
        // TOOD:
        // return Phone::FEATURE_ENABLE_HIPRI;
        return String("enableHIPRI");
    }
    return String(NULL);
}

Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOnUpstreamMobileConnection(
    /* [in] */Int32 apnType)
{
    Boolean retValue = TRUE;
    if (apnType == IConnectivityManager::TYPE_NONE) return FALSE;
    if (apnType != mOwner->mMobileApnReserved) TurnOffUpstreamMobileConnection();

    String enableString = EnableString(apnType);
    if (enableString.IsNull()) return FALSE;

    AutoPtr<IBinder> binder;
    CBinder::New((IBinder**)&binder);
    Int32 result = IPhoneConstants::APN_REQUEST_FAILED;
    mOwner->mOwner->mConnService->StartUsingNetworkFeature(
        IConnectivityManager::TYPE_MOBILE, enableString, binder, &result);

    switch (result) {
        case IPhoneConstants::APN_ALREADY_ACTIVE:
        case IPhoneConstants::APN_REQUEST_STARTED: {
            mOwner->mMobileApnReserved = apnType;
            AutoPtr<IMessage> m = mOwner->ObtainMessage(CMD_CELL_CONNECTION_RENEW);
            m->SetArg1(++mOwner->mCurrentConnectionSequence);
            mOwner->SendMessageDelayed(m, CELL_CONNECTION_RENEW_MS);
            break;
        }
        case IPhoneConstants::APN_REQUEST_FAILED:
        default:
            retValue = FALSE;
            break;
    }

    return retValue;
}

Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOffUpstreamMobileConnection()
{
    // ignore pending renewal requests
    ++mOwner->mCurrentConnectionSequence;
    if (mOwner->mMobileApnReserved != IConnectivityManager::TYPE_NONE) {
        Int32 result;
        ECode ec = mOwner->mOwner->mConnService->StopUsingNetworkFeature(IConnectivityManager::TYPE_MOBILE,
                EnableString(mOwner->mMobileApnReserved), &result);
        if (FAILED(ec)) {
            return FALSE;
        }
        mOwner->mMobileApnReserved = IConnectivityManager::TYPE_NONE;
    }
    return TRUE;
}

Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOnMasterTetherSettings()
{
    ECode ec = mOwner->mOwner->mNMService->SetIpForwardingEnabled(TRUE);
    if (FAILED(ec)) {
        mOwner->TransitionTo(mOwner->mSetIpForwardingEnabledErrorState);
        return FALSE;
    }
    ec = mOwner->mOwner->mNMService->StartTethering(mOwner->mOwner->mDhcpRange);
    if (FAILED(ec)) {
        if (FAILED(mOwner->mOwner->mNMService->StopTethering()) ||
            FAILED(mOwner->mOwner->mNMService->StartTethering(mOwner->mOwner->mDhcpRange))) {
            mOwner->TransitionTo(mOwner->mStartTetheringErrorState);
            return FALSE;
        }
    }
    ec = mOwner->mOwner->mNMService->SetDnsForwarders(mOwner->mOwner->mDefaultDnsServers);
    if (FAILED(ec)) {
        mOwner->TransitionTo(mOwner->mSetDnsForwardersErrorState);
        return FALSE;
    }
    return TRUE;
}

Boolean CTethering::TetherMasterSM::TetherMasterUtilState::TurnOffMasterTetherSettings()
{
    ECode ec = mOwner->mOwner->mNMService->StopTethering();
    if (FAILED(ec)) {
        mOwner->TransitionTo(mOwner->mStopTetheringErrorState);
        return FALSE;
    }
    ec = mOwner->mOwner->mNMService->SetIpForwardingEnabled(FALSE);
    if (FAILED(ec)) {
        mOwner->TransitionTo(mOwner->mSetIpForwardingDisabledErrorState);
        return FALSE;
    }
    mOwner->TransitionTo(mOwner->mInitialState);
    return TRUE;
}

void CTethering::TetherMasterSM::TetherMasterUtilState::ChooseUpstreamType(
    /* [in]*/ Boolean tryCell)
{
    ECode ec = NOERROR;
    Int32 upType = IConnectivityManager::TYPE_NONE;
    String iface;
    AutoPtr<IContentResolver> cr;
    mOwner->mOwner->mContext->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    Int32 wifi_cheat_on;
    settingsSecure->GetInt32(cr, ISettingsSecure::WIFI_CHEAT_ON, 0, &wifi_cheat_on);

    mOwner->mOwner->UpdateConfiguration();

    {
        Object::Autolock lock(mOwner->mOwner->mPublicSync);

        if (VDBG) {
            Logger::D(CTethering::TAG, "chooseUpstreamType has upstream iface types:");
            List< AutoPtr<IInteger32> >::Iterator it = mOwner->mOwner->mUpstreamIfaceTypes.Begin();
            for (; it != mOwner->mOwner->mUpstreamIfaceTypes.End(); ++it) {
                Int32 netType;
                (*it)->GetValue(&netType);
                Logger::D(CTethering::TAG, " %d", netType);
            }
        }

        List< AutoPtr<IInteger32> >::Iterator it = mOwner->mOwner->mUpstreamIfaceTypes.Begin();
        for (; it != mOwner->mOwner->mUpstreamIfaceTypes.End(); ++it) {
            Int32 netType;
            (*it)->GetValue(&netType);
            AutoPtr<INetworkInfo> info;
            ec = mOwner->mOwner->mConnService->GetNetworkInfo(netType, (INetworkInfo **)&info);
            if (FAILED(ec)) {}
            Boolean connected;
            if ((info != NULL) && (info->IsConnected(&connected), connected)) {
                AutoPtr<ILinkProperties> linkProperties0;
                ec = mOwner->mOwner->mConnService->GetLinkProperties(netType, (ILinkProperties**)&linkProperties0);
                if (FAILED(ec)) {}
                if (linkProperties0 != NULL) {
                    linkProperties0->GetInterfaceName(&iface);
                }
                if (!iface.IsNull()){
                    upType = netType;
                    break;
                }
            }
        }

        if (upType == IConnectivityManager::TYPE_NONE) {
            List< AutoPtr<IInteger32> >::Iterator it = mOwner->mOwner->mUpstreamIfaceTypes.Begin();
            for (; it != mOwner->mOwner->mUpstreamIfaceTypes.End(); ++it) {
                Int32 netType;
                (*it)->GetValue(&netType);
                AutoPtr<INetworkInfo> info;
                AutoPtr<INetworkInfo> info_eth;
                ec = mOwner->mOwner->mConnService->GetNetworkInfo(netType, (INetworkInfo**)&info);
                if (SUCCEEDED(ec)) {
                    if (wifi_cheat_on != 0) {
                        ec = mOwner->mOwner->mConnService->GetNetworkInfo(IConnectivityManager::TYPE_ETHERNET, (INetworkInfo**)&info_eth);
                    }
                }
                Boolean connected;
                if ((info != NULL) && (info->IsConnected(&connected), connected)) {
                    upType = netType;
                    if (wifi_cheat_on != 0) {
                        //eth cheated as wifi when eth connected,funct getNetworkInfo@ConnectivityService.java
                        if (upType == IConnectivityManager::TYPE_WIFI
                                && info_eth != NULL && (info_eth->IsConnected(&connected), connected))
                            upType = IConnectivityManager::TYPE_ETHERNET;
                    }
                    break;
                }
            }
        }
    }

    if (DBG) {
        Logger::D(CTethering::TAG, "chooseUpstreamType(%d), preferredApn =%d, got type=%d", tryCell,
                mOwner->mOwner->mPreferredUpstreamMobileApn, upType);
    }

    // if we're on DUN, put our own grab on it
    if (upType == IConnectivityManager::TYPE_MOBILE_DUN ||
            upType == IConnectivityManager::TYPE_MOBILE_HIPRI) {
        TurnOnUpstreamMobileConnection(upType);
    }
    else if (upType != IConnectivityManager::TYPE_NONE) {
        /* If we've found an active upstream connection that's not DUN/HIPRI
         * we should stop any outstanding DUN/HIPRI start requests.
         *
         * If we found NONE we don't want to do this as we want any previous
         * requests to keep trying to bring up something we can use.
         */
        TurnOffUpstreamMobileConnection();
    }

    if (upType == IConnectivityManager::TYPE_NONE) {
        Boolean tryAgainLater = TRUE;
        if ((tryCell == TRY_TO_SETUP_MOBILE_CONNECTION) &&
                TurnOnUpstreamMobileConnection(mOwner->mOwner->mPreferredUpstreamMobileApn)) {
            // we think mobile should be coming up - don't set a retry
            tryAgainLater = FALSE;
        }
        if (tryAgainLater) {
            mOwner->SendMessageDelayed(CMD_RETRY_UPSTREAM, UPSTREAM_SETTLE_TIME_MS);
        }
    }
    else {
        AutoPtr<ILinkProperties> linkProperties;
        ec = mOwner->mOwner->mConnService->GetLinkProperties(upType, (ILinkProperties**)&linkProperties);
        if (FAILED(ec)) {}
        if (linkProperties != NULL) {
            linkProperties->GetInterfaceName(&iface);
            AutoPtr< ArrayOf<String> > dnsServers = mOwner->mOwner->mDefaultDnsServers;
            AutoPtr<IObjectContainer> dnses;
            linkProperties->GetDnses((IObjectContainer**)&dnses);
            if (dnses != NULL) {
                // we currently only handle IPv4
                AutoPtr<IObjectContainer> v4Dnses;
                CObjectContainer::New((IObjectContainer**)&v4Dnses);
                AutoPtr<IObjectEnumerator> it;
                dnses->GetObjectEnumerator((IObjectEnumerator**)&it);
                Boolean succeeded;
                while (it->MoveNext(&succeeded), succeeded) {
                    AutoPtr<IInterface> obj;
                    it->Current((IInterface**)&obj);
                    if (IInet4Address::Probe(obj) != NULL) {
                        v4Dnses->Add(IInetAddress::Probe(obj));
                    }
                }
                Int32 count;
                v4Dnses->GetObjectCount(&count);
                if (count > 0) {
                    dnsServers = NetworkUtils::MakeStrings(v4Dnses);
                }
            }
            ec = mOwner->mOwner->mNMService->SetDnsForwarders(dnsServers);
            if (FAILED(ec)) {
                mOwner->TransitionTo(mOwner->mSetDnsForwardersErrorState);
            }
        }
    }
    NotifyTetheredOfNewUpstreamIface(iface);
}

void CTethering::TetherMasterSM::TetherMasterUtilState::NotifyTetheredOfNewUpstreamIface(
    /* [in]*/ const String& ifaceName)
{
    if (DBG) Logger::D(CTethering::TAG, "notifying tethered with iface =%s", ifaceName.string());
    mOwner->mUpstreamIfaceName = ifaceName;
    List< AutoPtr<TetherInterfaceSM> >::Iterator it;
    for (it = mOwner->mNotifyList.Begin(); it != mOwner->mNotifyList.End(); ++it) {
        AutoPtr<TetherInterfaceSM> sm = *it;
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(ifaceName, (ICharSequence**)&cs);
        sm->SendMessage(TetherInterfaceSM::CMD_TETHER_CONNECTION_CHANGED, cs);
    }
}


//===========================================================================
//  CTethering::TetherMasterSM::InitialState
//===========================================================================
ECode CTethering::TetherMasterSM::InitialState::Enter()
{
    return NOERROR;
}

ECode CTethering::TetherMasterSM::InitialState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 what;
    message->GetWhat(&what);
    if (DBG) Logger::D(CTethering::TAG, "MasterInitialState.processMessage what=%d", what);
    Boolean retValue = TRUE;
    switch (what) {
        case CMD_TETHER_MODE_REQUESTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            TetherInterfaceSM* who = (TetherInterfaceSM*)obj.Get();
            if (VDBG) Logger::D(CTethering::TAG, "Tether Mode requested by %p", who);
            mOwner->mNotifyList.PushBack(who);
            mOwner->TransitionTo(mOwner->mTetherModeAliveState);
            break;
        }
        case CMD_TETHER_MODE_UNREQUESTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            TetherInterfaceSM* who = (TetherInterfaceSM*)obj.Get();
            if (VDBG) Logger::D(CTethering::TAG, "Tether Mode unrequested by %p", who);
            mOwner->mNotifyList.Remove(who);
            break;
        }
        default:
            retValue = FALSE;
            break;
    }
    *value = retValue;
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM::TetherModeAliveState
//===========================================================================
ECode CTethering::TetherMasterSM::TetherModeAliveState::Enter()
{
    TurnOnMasterTetherSettings(); // may transition us out

    mTryCell = !WAIT_FOR_NETWORK_TO_SETTLE; // better try something first pass
                                            // or crazy tests cases will fail
    ChooseUpstreamType(mTryCell);
    mTryCell = !mTryCell;
    return NOERROR;
}

ECode CTethering::TetherMasterSM::TetherModeAliveState::Exit()
{
    TurnOffUpstreamMobileConnection();
    NotifyTetheredOfNewUpstreamIface(String(NULL));
    return NOERROR;
}

ECode CTethering::TetherMasterSM::TetherModeAliveState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 what;
    message->GetWhat(&what);
    if (DBG) Logger::D(CTethering::TAG, "TetherModeAliveState.processMessage what=%d", what);
    Boolean retValue = TRUE;
    switch (what) {
        case CMD_TETHER_MODE_REQUESTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            TetherInterfaceSM* who = (TetherInterfaceSM*)obj.Get();
            if (VDBG) Logger::D(CTethering::TAG, "Tether Mode requested by %p", who);
            mOwner->mNotifyList.PushBack(who);
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(mOwner->mUpstreamIfaceName, (ICharSequence**)&cs);
            who->SendMessage(TetherInterfaceSM::CMD_TETHER_CONNECTION_CHANGED, cs);
            break;
        }
        case CMD_TETHER_MODE_UNREQUESTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            AutoPtr<TetherInterfaceSM> who = (TetherInterfaceSM*)obj.Get();
            if (VDBG) Logger::D(CTethering::TAG, "Tether Mode unrequested by %p", who.Get());
            List< AutoPtr<TetherInterfaceSM> >::Iterator it = Find(
                    mOwner->mNotifyList.Begin(), mOwner->mNotifyList.End(), who);
            if (it != mOwner->mNotifyList.End()) {
                if (DBG) Logger::D(CTethering::TAG, "TetherModeAlive removing notifyee %p", who.Get());
                mOwner->mNotifyList.Erase(it);
                if (mOwner->mNotifyList.IsEmpty()) {
                    TurnOffMasterTetherSettings(); // transitions appropriately
                }
                else {
                    if (DBG) {
                        Logger::D(CTethering::TAG, "TetherModeAlive still has %s  live requests:",
                                mOwner->mNotifyList.GetSize());
                        // for (Object o : mNotifyList) Log.d(TAG, "  " + o);
                    }
                }
            }
            else {
                Logger::E(CTethering::TAG, "TetherModeAliveState UNREQUESTED has unknown who: %p", who.Get());
            }
            break;
        }
        case CMD_UPSTREAM_CHANGED:
            // need to try DUN immediately if Wifi goes down
            mTryCell = !WAIT_FOR_NETWORK_TO_SETTLE;
            ChooseUpstreamType(mTryCell);
            mTryCell = !mTryCell;
            break;
        case CMD_CELL_CONNECTION_RENEW: {
            // make sure we're still using a requested connection - may have found
            // wifi or something since then.
            Int32 arg1;
            message->GetArg1(&arg1);
            if (mOwner->mCurrentConnectionSequence == arg1) {
                if (VDBG) {
                    Logger::D(CTethering::TAG, "renewing mobile connection - requeuing for another %sms",
                            CELL_CONNECTION_RENEW_MS);
                }
                TurnOnUpstreamMobileConnection(mOwner->mMobileApnReserved);
            }
            break;
        }
        case CMD_RETRY_UPSTREAM:
            ChooseUpstreamType(mTryCell);
            mTryCell = !mTryCell;
            break;
        default:
            retValue = FALSE;
            break;
    }
    *value = retValue;
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM::ErrorState
//===========================================================================
ECode CTethering::TetherMasterSM::ErrorState::ProcessMessage(
    /* [in] */ IMessage* message,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Boolean retValue = TRUE;
    Int32 what;
    message->GetWhat(&what);
    switch (what) {
        case CMD_TETHER_MODE_REQUESTED: {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            AutoPtr<TetherInterfaceSM> who = (TetherInterfaceSM*)obj.Get();
            who->SendMessage(mErrorNotification);
            break;
        }
        default:
           retValue = FALSE;
    }
    *value = retValue;
    return NOERROR;
}

void CTethering::TetherMasterSM::ErrorState::Notify(
    /* [in] */Int32 msgType)
{
    mErrorNotification = msgType;
    List< AutoPtr<TetherInterfaceSM> >::Iterator it;
    for (it = mOwner->mNotifyList.Begin(); it != mOwner->mNotifyList.End(); ++it) {
        AutoPtr<TetherInterfaceSM> sm = *it;
        sm->SendMessage(msgType);
    }
}


//===========================================================================
//  CTethering::TetherMasterSM::SetIpForwardingEnabledErrorState
//===========================================================================
ECode CTethering::TetherMasterSM::SetIpForwardingEnabledErrorState::Enter()
{
    Logger::E(CTethering::TAG, "Error in setIpForwardingEnabled");
    Notify(TetherInterfaceSM::CMD_IP_FORWARDING_ENABLE_ERROR);
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM::SetIpForwardingDisabledErrorState
//===========================================================================
ECode CTethering::TetherMasterSM::SetIpForwardingDisabledErrorState::Enter()
{
    Logger::E(CTethering::TAG, "Error in setIpForwardingDisabled");
    Notify(TetherInterfaceSM::CMD_IP_FORWARDING_DISABLE_ERROR);
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM::StartTetheringErrorState
//===========================================================================
ECode CTethering::TetherMasterSM::StartTetheringErrorState::Enter()
{
    Logger::E(CTethering::TAG, "Error in startTethering");
    Notify(TetherInterfaceSM::CMD_START_TETHERING_ERROR);
    ECode ec = mOwner->mOwner->mNMService->SetIpForwardingEnabled(FALSE);
    if (FAILED(ec)) {}
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM::StopTetheringErrorState
//===========================================================================
ECode CTethering::TetherMasterSM::StopTetheringErrorState::Enter()
{
    Logger::E(CTethering::TAG, "Error in stopTethering");
    Notify(TetherInterfaceSM::CMD_STOP_TETHERING_ERROR);
    ECode ec = mOwner->mOwner->mNMService->SetIpForwardingEnabled(FALSE);
    if (FAILED(ec)) {}
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM::SetDnsForwardersErrorState
//===========================================================================
ECode CTethering::TetherMasterSM::SetDnsForwardersErrorState::Enter()
{
    Logger::E(CTethering::TAG, "Error in setDnsForwarders");
    Notify(TetherInterfaceSM::CMD_SET_DNS_FORWARDERS_ERROR);
    ECode ec = mOwner->mOwner->mNMService->StopTethering();
    if (FAILED(ec)) {}
    ec = mOwner->mOwner->mNMService->SetIpForwardingEnabled(FALSE);
    if (FAILED(ec)) {}
    return NOERROR;
}


//===========================================================================
//  CTethering::TetherMasterSM
//===========================================================================
const Int32 CTethering::TetherMasterSM::CMD_TETHER_MODE_REQUESTED;
const Int32 CTethering::TetherMasterSM::CMD_TETHER_MODE_UNREQUESTED;
const Int32 CTethering::TetherMasterSM::CMD_UPSTREAM_CHANGED;
const Int32 CTethering::TetherMasterSM::CMD_CELL_CONNECTION_RENEW;
const Int32 CTethering::TetherMasterSM::CMD_RETRY_UPSTREAM;
const Int32 CTethering::TetherMasterSM::UPSTREAM_SETTLE_TIME_MS;
const Int32 CTethering::TetherMasterSM::CELL_CONNECTION_RENEW_MS;

CTethering::TetherMasterSM::TetherMasterSM(
    /* [in] */ const String& name,
    /* [in] */ ILooper* looper,
    /* [in] */ CTethering* owner)
    : StateMachine(name, looper)
    , mSequenceNumber(0)
    , mCurrentConnectionSequence(0)
    , mMobileApnReserved(0)
    , mOwner(owner)
{
    //Add states
    mInitialState = new InitialState(this);
    AddState(mInitialState);
    mTetherModeAliveState = new TetherModeAliveState(this);
    AddState(mTetherModeAliveState);

    mSetIpForwardingEnabledErrorState = new SetIpForwardingEnabledErrorState(this);
    AddState(mSetIpForwardingEnabledErrorState);
    mSetIpForwardingDisabledErrorState = new SetIpForwardingDisabledErrorState(this);
    AddState(mSetIpForwardingDisabledErrorState);
    mStartTetheringErrorState = new StartTetheringErrorState(this);
    AddState(mStartTetheringErrorState);
    mStopTetheringErrorState = new StopTetheringErrorState(this);
    AddState(mStopTetheringErrorState);
    mSetDnsForwardersErrorState = new SetDnsForwardersErrorState(this);
    AddState(mSetDnsForwardersErrorState);

    SetInitialState(mInitialState);
}

CTethering::TetherMasterSM::~TetherMasterSM()
{
    mNotifyList.Clear();
}


//===========================================================================
//  CTethering
//===========================================================================
static AutoPtr<IInteger32> InitType(
    /* [in] */ Int32 type)
{
    AutoPtr<IInteger32> itype;
    CInteger32::New(type, (IInteger32**)&itype);
    return itype;
}

static AutoPtr< ArrayOf<String> > InitDhcpDefaultRange()
{
    AutoPtr< ArrayOf<String> > range = ArrayOf<String>::Alloc(14);
    (*range)[0] = "192.168.42.2";
    (*range)[1] = "192.168.42.254";
    (*range)[2] = "192.168.43.2";
    (*range)[3] = "192.168.43.254";
    (*range)[4] = "192.168.44.2";
    (*range)[5] = "192.168.44.254";
    (*range)[6] = "192.168.45.2";
    (*range)[7] = "192.168.45.254";
    (*range)[8] = "192.168.46.2";
    (*range)[9] = "192.168.46.254";
    (*range)[10] = "192.168.47.2";
    (*range)[11] = "192.168.47.254";
    (*range)[12] = "192.168.48.2";
    (*range)[13] = "192.168.48.254";
    return range;
}

const String CTethering::TAG("Tethering");
const Boolean CTethering::DBG = FALSE;
const Boolean CTethering::VDBG = FALSE;

const AutoPtr<IInteger32> CTethering::MOBILE_TYPE = InitType(IConnectivityManager::TYPE_MOBILE);
const AutoPtr<IInteger32> CTethering::HIPRI_TYPE = InitType(IConnectivityManager::TYPE_MOBILE_HIPRI);
const AutoPtr<IInteger32> CTethering::DUN_TYPE = InitType(IConnectivityManager::TYPE_MOBILE_DUN);

const String CTethering::USB_NEAR_IFACE_ADDR("192.168.42.129");
const Int32 CTethering::USB_PREFIX_LENGTH = 24;

AutoPtr< ArrayOf<String> > CTethering::DHCP_DEFAULT_RANGE = InitDhcpDefaultRange();

const String CTethering::DNS_DEFAULT_SERVER1("8.8.8.8");
const String CTethering::DNS_DEFAULT_SERVER2("8.8.4.4");

CTethering::CTethering()
    : mWifiTethered(FALSE)
    , mPreferredUpstreamMobileApn(0)
    , mIfaces(20)
    , mRndisEnabled(FALSE)
    , mUsbTetherRequested(FALSE)
{}

CTethering::~CTethering()
{
    mUpstreamIfaceTypes.Clear();
    mIfaces.Clear();
}

ECode CTethering::constructor(
    /* [in] */ IContext* context,
    /* [in] */ INetworkManagementService* nmService,
    /* [in] */ INetworkStatsService* statsService,
    /* [in] */ IIConnectivityManager* connService,
    /* [in] */ ILooper* looper)
{
    mContext = context;
    mNMService = nmService;
    mStatsService = statsService;
    mConnService = connService;
    mLooper = looper;

    // make our own thread so we don't anr the system
    CHandlerThread::New(String("Tethering"), (IHandlerThread**)&mThread);
    mThread->Start();
    mThread->GetLooper((ILooper**)&mLooper);
    mTetherMasterSM = new TetherMasterSM(String("TetherMaster"), mLooper, this);
    mTetherMasterSM->Start();

    mStateReceiver = new StateReceiver(this);
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IUsbManager::ACTION_USB_STATE);
    filter->AddAction(IConnectivityManager::CONNECTIVITY_ACTION);
    AutoPtr<IIntent> result;
    mContext->RegisterReceiver(mStateReceiver, filter, (IIntent**)&result);

    filter = NULL;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_MEDIA_SHARED);
    filter->AddAction(IIntent::ACTION_MEDIA_UNSHARED);
    filter->AddDataScheme(String("file"));
    result = NULL;
    mContext->RegisterReceiver(mStateReceiver, filter, (IIntent**)&result);

    AutoPtr<IResources> resource;
    context->GetResources((IResources**)&resource);
    resource->GetStringArray(
            R::array::config_tether_dhcp_range, (ArrayOf<String>**)&mDhcpRange);
    if ((mDhcpRange->GetLength() == 0) || (mDhcpRange->GetLength() % 2 == 1)) {
        mDhcpRange = DHCP_DEFAULT_RANGE;
    }

    // load device config info
    UpdateConfiguration();

    // TODO - remove and rely on real notifications of the current iface
    mDefaultDnsServers = ArrayOf<String>::Alloc(2);
    (*mDefaultDnsServers)[0] = DNS_DEFAULT_SERVER1;
    (*mDefaultDnsServers)[1] = DNS_DEFAULT_SERVER2;
    return NOERROR;
}

void CTethering::UpdateConfiguration()
{
    AutoPtr<IResources> resource;
    mContext->GetResources((IResources**)&resource);
    AutoPtr< ArrayOf<String> > tetherableUsbRegexs;
    resource->GetStringArray(
            R::array::config_tether_usb_regexs, (ArrayOf<String>**)&tetherableUsbRegexs);
    AutoPtr< ArrayOf<String> > tetherableWifiRegexs;
    resource->GetStringArray(
            R::array::config_tether_wifi_regexs, (ArrayOf<String>**)&tetherableWifiRegexs);
    AutoPtr< ArrayOf<String> > tetherableBluetoothRegexs;
    resource->GetStringArray(
            R::array::config_tether_bluetooth_regexs, (ArrayOf<String>**)&tetherableBluetoothRegexs);

    AutoPtr< ArrayOf<Int32> > ifaceTypes;
    resource->GetIntArray(
            R::array::config_tether_upstream_types, (ArrayOf<Int32>**)&ifaceTypes);
    for (Int32 i = 0; i != ifaceTypes->GetLength(); i++) {
        AutoPtr<IInteger32> type;
        CInteger32::New((*ifaceTypes)[i], (IInteger32**)&type);
        mUpstreamIfaceTypes.PushBack(type);
    }

    {
        Object::Autolock lock(mPublicSync);
        mTetherableUsbRegexs = tetherableUsbRegexs;
        mTetherableWifiRegexs = tetherableWifiRegexs;
        mTetherableBluetoothRegexs = tetherableBluetoothRegexs;
    }

    // check if the upstream type list needs to be modified due to secure-settings
    CheckDunRequired();
}

ECode CTethering::InterfaceStatusChanged(
    /* [in] */const String& iface,
    /* [in] */ Boolean up)
{
    if (VDBG) Logger::D(TAG, "interfaceStatusChanged %s, %d", iface.string(), up);
    Boolean found = FALSE;
    Boolean usb = FALSE;
    {
        Object::Autolock lock(mPublicSync);
        if (IsWifi(iface)) {
            found = TRUE;
        }
        else if (IsUsb(iface)) {
            found = TRUE;
            usb = TRUE;
        }
        else if (IsBluetooth(iface)) {
            found = TRUE;
        }
        if (found == FALSE) return NOERROR;

        AutoPtr<TetherInterfaceSM> sm;
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it = mIfaces.Find(iface);
        if (it != mIfaces.End()) {
            sm = it->mSecond;
        }
        if (up) {
            if (sm == NULL) {
                sm = new TetherInterfaceSM(iface, mLooper, usb, this);
                mIfaces[iface] = sm;
                sm->Start();
            }
        }
        else {
            if (IsUsb(iface)) {
                // ignore usb0 down after enabling RNDIS
                // we will handle disconnect in interfaceRemoved instead
                if (VDBG) Logger::D(TAG, "ignore interface down for %s", iface.string());
            }
            else if (sm != NULL) {
                if (iface.Equals("p2p0")) {
                    // Logger::D(TAG, "Ignore p2p0 down flags, nosense !!!!!");
                    return NOERROR;
                }
                sm->SendMessage(TetherInterfaceSM::CMD_INTERFACE_DOWN);
                mIfaces.Erase(it);
            }
        }
    }
    return NOERROR;
}

ECode CTethering::InterfaceLinkStateChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    if (VDBG) Logger::D(TAG, "interfaceLinkStateChanged %s, %d", iface.string(), up);
    return InterfaceStatusChanged(iface, up);
}

Boolean CTethering::IsUsb(
    /* [in] */ const String& iface)
{
    Object::Autolock lock(mPublicSync);

    for (Int32 i = 0; i < mTetherableUsbRegexs->GetLength(); i++){
        String regex = (*mTetherableUsbRegexs)[i];
        AutoPtr<IPatternHelper> helper;
        CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
        Boolean match = FALSE;
        helper->Matches(regex, iface, &match);
        if(match) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean CTethering::IsWifi(
    /* [in] */ const String& iface)
{
    Object::Autolock lock(mPublicSync);

    for (Int32 i = 0; i < mTetherableWifiRegexs->GetLength(); i++){
        String regex = (*mTetherableWifiRegexs)[i];
        AutoPtr<IPatternHelper> helper;
        CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
        Boolean match = FALSE;
        helper->Matches(regex, iface, &match);
        if(match) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean CTethering::IsBluetooth(
    /* [in] */ const String& iface)
{
    Object::Autolock lock(mPublicSync);

    for (Int32 i = 0; i < mTetherableBluetoothRegexs->GetLength(); i++){
        String regex = (*mTetherableBluetoothRegexs)[i];
        AutoPtr<IPatternHelper> helper;
        CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
        Boolean match = FALSE;
        helper->Matches(regex, iface, &match);
        if(match) {
            return TRUE;
        }
    }
    return FALSE;
}

ECode CTethering::InterfaceAdded(
    /* [in] */ const String& iface)
{
    if (VDBG) Logger::D(TAG, "interfaceAdded %s", iface.string());
    Boolean found = FALSE;
    Boolean usb = FALSE;
    {
        Object::Autolock lock(mPublicSync);

        if (IsWifi(iface)) {
            found = TRUE;
        }
        if (IsUsb(iface)) {
            found = TRUE;
            usb = TRUE;
        }
        if (IsBluetooth(iface)) {
            found = TRUE;
        }

        if (found == FALSE) {
            if (VDBG) Logger::D(TAG, "%s is not a tetherable iface, ignoring", iface.string());
            return NOERROR;
        }

        AutoPtr<TetherInterfaceSM> sm;
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it = mIfaces.Find(iface);
        if (it != mIfaces.End()) {
            sm = it->mSecond;
        }
        if (sm != NULL) {
            if (VDBG) Logger::D(TAG, "active iface (%s) reported as added, ignoring", iface.string());
            return NOERROR;
        }
        sm = new TetherInterfaceSM(iface, mLooper, usb, this);
        mIfaces[iface] = sm;
        sm->Start();
    }
    return NOERROR;
}

ECode CTethering::InterfaceRemoved(
    /* [in] */ const String& iface)
{
    if (VDBG) Logger::D(TAG, "interfaceRemoved %s", iface.string());
    {
        Object::Autolock lock(mPublicSync);

        AutoPtr<TetherInterfaceSM> sm;
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it = mIfaces.Find(iface);
        if (it != mIfaces.End()) {
            sm = it->mSecond;
        }
        if (sm == NULL) {
            if (VDBG) {
                Logger::E(TAG, "attempting to remove unknown iface (%s), ignoring", iface.string());
            }
            return NOERROR;
        }
        sm->SendMessage(TetherInterfaceSM::CMD_INTERFACE_DOWN);
        mIfaces.Erase(it);
    }
    return NOERROR;
}

ECode CTethering::LimitReached(
    /* [in] */ const String& limitName,
    /* [in] */ const String& iface)
{
    return NOERROR;
}

ECode CTethering::InterfaceClassDataActivityChanged(
    /* [in] */ const String& lable,
    /* [in] */ Boolean active)
{
    return NOERROR;
}

ECode CTethering::Tether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (DBG) Logger::D(TAG, "Tethering %s", iface.string());
    AutoPtr<TetherInterfaceSM> sm;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it = mIfaces.Find(iface);
        if (it != mIfaces.End()) {
            sm = it->mSecond;
        }
    }
    if (sm == NULL) {
        Logger::E(TAG, "Tried to Tether an unknown iface :%s, ignoring", iface.string());
        *value = IConnectivityManager::TETHER_ERROR_UNKNOWN_IFACE;
        return NOERROR;
    }
    if (!sm->IsAvailable() && !sm->IsErrored()) {
        Logger::E(TAG, "Tried to Tether an unavailable iface :%s, ignoring", iface.string());
        *value = IConnectivityManager::TETHER_ERROR_UNAVAIL_IFACE;
        return NOERROR;
    }
    sm->SendMessage(TetherInterfaceSM::CMD_TETHER_REQUESTED);
    *value = IConnectivityManager::TETHER_ERROR_NO_ERROR;
    return NOERROR;
}

ECode CTethering::Untether(
    /* [in] */ const String& iface,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (DBG) Logger::D(TAG, "Untethering %s", iface.string());
    AutoPtr<TetherInterfaceSM> sm;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it = mIfaces.Find(iface);
        if (it != mIfaces.End()) {
            sm = it->mSecond;
        }
    }
    if (sm == NULL) {
        Logger::E(TAG, "Tried to Untether an unknown iface :%s, ignoring", iface.string());
        *value = IConnectivityManager::TETHER_ERROR_UNKNOWN_IFACE;
        return NOERROR;
    }
    if (sm->IsErrored()) {
        Logger::E(TAG, "Tried to Untethered an errored iface :%s, ignoring", iface.string());
        *value = IConnectivityManager::TETHER_ERROR_UNAVAIL_IFACE;
        return NOERROR;
    }
    sm->SendMessage(TetherInterfaceSM::CMD_TETHER_REQUESTED);
    *value = IConnectivityManager::TETHER_ERROR_NO_ERROR;
    return NOERROR;

}

ECode CTethering::GetLastTetherError(
    /* [in] */ const String& iface,
    /* [in] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<TetherInterfaceSM> sm;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it = mIfaces.Find(iface);
        if (it != mIfaces.End()) {
            sm = it->mSecond;
        }
        if(sm == NULL){
            Logger::E(TAG, "Tried to getLastTetherError on an unknown iface :%s", iface.string());
            *value = IConnectivityManager::TETHER_ERROR_UNKNOWN_IFACE;
            return NOERROR;
        }
        *value = sm->GetLastError();
        return NOERROR;
    }
}

// TODO - move all private methods used only by the state machine into the state machine
// to clarify what needs synchronized protection.
void CTethering::SendTetherStateChangedBroadcast()
{
    Boolean result = FALSE;
    ECode ec = mConnService->IsTetheringSupported(&result);
    if (FAILED(ec) || !result) return;

    AutoPtr<IObjectContainer> availableList, activeList, erroredList;
    CParcelableObjectContainer::New((IObjectContainer**)&availableList);
    CParcelableObjectContainer::New((IObjectContainer**)&activeList);
    CParcelableObjectContainer::New((IObjectContainer**)&erroredList);

    Boolean wifiTethered = FALSE;
    Boolean usbTethered = FALSE;
    Boolean bluetoothTethered = FALSE;

    {
        Object::Autolock lock(mPublicSync);

        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it;
        for (it = mIfaces.Begin(); it != mIfaces.End(); ++it) {
            String iface = it->mFirst;
            AutoPtr<TetherInterfaceSM> sm = it->mSecond;
            if (sm != NULL) {
                if (sm->IsErrored()) {
                    AutoPtr<ICharSequence> cs;
                    CStringWrapper::New(iface, (ICharSequence**)&cs);
                    erroredList->Add(cs);
                }
                else if (sm->IsAvailable()) {
                    AutoPtr<ICharSequence> cs;
                    CStringWrapper::New(iface, (ICharSequence**)&cs);
                    availableList->Add(cs);
                }
                else if (sm->IsTethered()) {
                    if (IsUsb(iface)) {
                        usbTethered = TRUE;
                    }
                    else if (IsWifi(iface)) {
                        wifiTethered = TRUE;
                    }
                    else if (IsBluetooth(iface)) {
                        bluetoothTethered = TRUE;
                    }
                    AutoPtr<ICharSequence> cs;
                    CStringWrapper::New(iface, (ICharSequence**)&cs);
                    activeList->Add(cs);
                }
            }
        }
    }
    AutoPtr<IIntent> broadcast;
    CIntent::New(IConnectivityManager::ACTION_TETHER_STATE_CHANGED, (IIntent**)&broadcast);
    broadcast->AddFlags(IIntent::FLAG_RECEIVER_REPLACE_PENDING |
            IIntent::FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
    broadcast->PutStringArrayListExtra(IConnectivityManager::EXTRA_AVAILABLE_TETHER,
            availableList);
    broadcast->PutStringArrayListExtra(IConnectivityManager::EXTRA_ACTIVE_TETHER, activeList);
    broadcast->PutStringArrayListExtra(IConnectivityManager::EXTRA_ERRORED_TETHER,
            erroredList);
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);
    mContext->SendStickyBroadcastAsUser(broadcast, ALL);
    /*if (DBG) {
        Log.d(TAG, "sendTetherStateChangedBroadcast " + availableList.size() + ", " +
                activeList.size() + ", " + erroredList.size());
    }*/

    if (usbTethered) {
        if (wifiTethered || bluetoothTethered) {
            ShowTetheredNotification(R::drawable::stat_sys_tether_general);
        }
        else {
            ShowTetheredNotification(R::drawable::stat_sys_tether_usb);
        }
    }
    else if (wifiTethered) {
        if (bluetoothTethered) {
            ShowTetheredNotification(R::drawable::stat_sys_tether_general);
        }
        else {
            ShowTetheredNotification(R::drawable::stat_sys_tether_wifi);
        }
    }
    else if (bluetoothTethered) {
        ShowTetheredNotification(R::drawable::stat_sys_tether_bluetooth);
    }
    else {
        ClearTetheredNotification();
    }
}

void CTethering::ShowTetheredNotification(
    /* [in] */ Int32 icon)
{
    AutoPtr<INotificationManager> notificationManager;
    mContext->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&notificationManager);
    if (notificationManager == NULL){
        return;
    }

    AutoPtr<IUserHandleHelper> uhhelper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&uhhelper);
    AutoPtr<IUserHandle> ALL;
    uhhelper->GetALL((IUserHandle**)&ALL);
    if (mTetheredNotification != NULL) {
        Int32 _icon;
        mTetheredNotification->GetIcon(&_icon);
        if (_icon == icon) {
            return;
        }
        notificationManager->CancelAsUser(String(NULL), _icon, ALL);
    }

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent **)&intent);
    intent->SetClassName(String("com.android.settings"), String("com.android.settings.TetherSettings"));
    intent->SetFlags(IIntent::FLAG_ACTIVITY_NO_HISTORY);

    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    AutoPtr<IPendingIntent> pi;
    AutoPtr<IUserHandle> CURRENT;
    uhhelper->GetCURRENT((IUserHandle**)&CURRENT);
    helper->GetActivityAsUser(mContext, 0, intent, 0, NULL, CURRENT, (IPendingIntent**)&pi);

    AutoPtr<IResourcesHelper> resHelper;
    CResourcesHelper::AcquireSingleton((IResourcesHelper**)&resHelper);
    AutoPtr<IResources> r;
    resHelper->GetSystem((IResources**)&r);

    AutoPtr<ICharSequence> title;
    r->GetText(R::string::tethered_notification_title, (ICharSequence**)&title);
    AutoPtr<ICharSequence> message;
    r->GetText(R::string::tethered_notification_message, (ICharSequence**)&message);

    if (mTetheredNotification == NULL) {
        CNotification::New((INotification**)&mTetheredNotification);
        mTetheredNotification->SetWhen(0);
    }
    mTetheredNotification->SetIcon(icon);
    Int32 defaultValue;
    mTetheredNotification->GetDefaults(&defaultValue);
    mTetheredNotification->SetDefaults(defaultValue & ~INotification::DEFAULT_SOUND);
    mTetheredNotification->SetFlags(INotification::FLAG_ONGOING_EVENT);
    mTetheredNotification->SetTickerText(title);
    mTetheredNotification->SetLatestEventInfo(mContext, title, message, pi);

    notificationManager->NotifyAsUser(String(NULL), icon, mTetheredNotification, ALL);
}

void CTethering::ClearTetheredNotification()
{
    AutoPtr<INotificationManager> notificationManager;
    mContext->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&notificationManager);
    if (notificationManager != NULL && mTetheredNotification != NULL) {
        Int32 icon;
        mTetheredNotification->GetIcon(&icon);
        AutoPtr<IUserHandleHelper> uhhelper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&uhhelper);
        AutoPtr<IUserHandle> ALL;
        uhhelper->GetALL((IUserHandle**)&ALL);
        notificationManager->CancelAsUser(String(NULL), icon, ALL);
        mTetheredNotification = NULL;
   }
}

void CTethering::TetherUsb(
    /* [in] */ Boolean enable)
{
    if (VDBG) Logger::D(TAG, "tetherUsb %d", enable);

    AutoPtr< ArrayOf<String> > ifaces;
    ECode ec = mNMService->ListInterfaces((ArrayOf<String> **)&ifaces);
    if (FAILED(ec)) {
        Logger::E(TAG, "Error listing Interfaces%x", ec);
        return;
    }
    for (Int32 i = 0; i < ifaces->GetLength(); i++){
        String iface = (*ifaces)[i];
        if (IsUsb(iface)) {
            Int32 result;
            if (enable) {
                Tether(iface, &result);
            }
            else {
                Untether(iface, &result);
            }
            if (result == IConnectivityManager::TETHER_ERROR_NO_ERROR) {
                return;
            }
        }
    }
    Logger::E(TAG, "unable start or stop USB tethering");
}

Boolean CTethering::ConfigureUsbIface(
    /* [in] */ Boolean enabled)
{
    if (VDBG) Logger::D(TAG, "configureUsbIface(%d)", enabled);

    // toggle the USB interfaces
    AutoPtr<ArrayOf<String> > ifaces;
    ECode ec = mNMService->ListInterfaces((ArrayOf<String> **)&ifaces);
    if (FAILED(ec)) {
        Logger::E(TAG, "Error listing Interfaces%x", ec);
        return FALSE;
    }
    for (Int32 i = 0; i < ifaces->GetLength(); i++){
        String iface = (*ifaces)[i];
        if (IsUsb(iface)) {
            AutoPtr<IInterfaceConfiguration> ifcg;
            ec = mNMService->GetInterfaceConfig(iface, (IInterfaceConfiguration**)&ifcg);
            if (FAILED(ec)) {
                Logger::E(TAG, "Error configuring Interfaces%x", ec);
                return FALSE;
            }
            if (ifcg != NULL) {
                AutoPtr<IInetAddress> addr;
                NetworkUtils::NumericToInetAddress(USB_NEAR_IFACE_ADDR, (IInetAddress**)&addr);

                AutoPtr<ILinkAddress> linkAddr;
                CLinkAddress::New(addr, USB_PREFIX_LENGTH, (ILinkAddress **)&linkAddr);
                ifcg->SetLinkAddress(linkAddr);
                if (enabled) {
                    ec = ifcg->SetInterfaceUp();
                }
                else {
                    ec = ifcg->SetInterfaceDown();
                }
                if (FAILED(ec)) {
                    Logger::E(TAG, "Error configuring Interfaces%x", ec);
                    return FALSE;
                }
                ifcg->ClearFlag(String("running"));
                ec = mNMService->SetInterfaceConfig(iface, ifcg);
                if (FAILED(ec)) {
                    Logger::E(TAG, "Error configuring Interfaces%x", ec);
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

ECode CTethering::GetTetherableUsbRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTetherableUsbRegexs;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CTethering::GetTetherableWifiRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTetherableWifiRegexs;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CTethering::GetTetherableBluetoothRegexs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTetherableBluetoothRegexs;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CTethering::SetUsbTethering(
    /* [in] */ Boolean enable,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (VDBG) Logger::D(TAG, "setUsbTethering(%d)", enable);
    AutoPtr<IUsbManager> usbManager;
    mContext->GetSystemService(IContext::USB_SERVICE, (IInterface**)&usbManager);

    {
        Object::Autolock lock(mPublicSync);

        if (enable){
            if (mRndisEnabled) {
                TetherUsb(TRUE);
            }
            else {
                mUsbTetherRequested = TRUE;
                usbManager->SetCurrentFunction(IUsbManager::USB_FUNCTION_RNDIS, FALSE);
            }
        }
        else{
            TetherUsb(FALSE);
            if (mRndisEnabled) {
                usbManager->SetCurrentFunction(String(NULL), FALSE);
            }
            mUsbTetherRequested = FALSE;
        }
    }
    *result = IConnectivityManager::TETHER_ERROR_NO_ERROR;
    return NOERROR;
}

AutoPtr< ArrayOf<Int32> > CTethering::GetUpstreamIfaceTypes()
{
    AutoPtr< ArrayOf<Int32> > values;
    {
        Object::Autolock lock(mPublicSync);
        UpdateConfiguration();
        values = ArrayOf<Int32>::Alloc(mUpstreamIfaceTypes.GetSize());
        List< AutoPtr<IInteger32> >::Iterator it = mUpstreamIfaceTypes.Begin();
        Int32 i = 0;
        for (; it != mUpstreamIfaceTypes.End(); ++it, ++i) {
            Int32 value;
            (*it)->GetValue(&value);
            (*values)[i] = value;
        }
    }
    return values;
}

void CTethering::CheckDunRequired()
{
    AutoPtr<IContentResolver> contentresolver;
    mContext->GetContentResolver((IContentResolver**)&contentresolver);
    Int32 secureSetting = 0;
    AutoPtr<ISettingsGlobal> global;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&global);
    global->GetInt32(contentresolver, ISettingsGlobal::TETHER_DUN_REQUIRED,
         2, &secureSetting);
    {
        Object::Autolock lock(mPublicSync);
        // 2 = not set, 0 = DUN not required, 1 = DUN required
        if (secureSetting != 2) {
            Int32 requiredApn = (secureSetting == 1 ?
                    IConnectivityManager::TYPE_MOBILE_DUN :
                    IConnectivityManager::TYPE_MOBILE_HIPRI);
            if (requiredApn == IConnectivityManager::TYPE_MOBILE_DUN) {
                while (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), MOBILE_TYPE)
                        != mUpstreamIfaceTypes.End()) {
                    mUpstreamIfaceTypes.Remove(MOBILE_TYPE);
                }
                while (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), HIPRI_TYPE)
                        != mUpstreamIfaceTypes.End()) {
                    mUpstreamIfaceTypes.Remove(HIPRI_TYPE);
                }
                if (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), DUN_TYPE)
                        == mUpstreamIfaceTypes.End()) {
                    mUpstreamIfaceTypes.PushBack(DUN_TYPE);
                }
            }
            else {
                while (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), DUN_TYPE)
                        != mUpstreamIfaceTypes.End()) {
                    mUpstreamIfaceTypes.Remove(DUN_TYPE);
                }
                if (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), MOBILE_TYPE)
                        == mUpstreamIfaceTypes.End()) {
                    mUpstreamIfaceTypes.PushBack(MOBILE_TYPE);
                }
                if (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), HIPRI_TYPE)
                        == mUpstreamIfaceTypes.End()) {
                    mUpstreamIfaceTypes.PushBack(HIPRI_TYPE);
                }
            }
        }
        if (Find(mUpstreamIfaceTypes.Begin(), mUpstreamIfaceTypes.End(), DUN_TYPE)
                != mUpstreamIfaceTypes.End()) {
            mPreferredUpstreamMobileApn = IConnectivityManager::TYPE_MOBILE_DUN;
        }
        else {
            mPreferredUpstreamMobileApn = IConnectivityManager::TYPE_MOBILE_HIPRI;
        }
    }
}

ECode CTethering::GetTetheredIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    List<String> list;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it;
        for (it = mIfaces.Begin(); it != mIfaces.End(); ++it) {
            AutoPtr<TetherInterfaceSM> sm = it->mSecond;
            if (sm->IsTethered()) {
                list.PushBack(it->mFirst);
            }
        }
    }
    AutoPtr< ArrayOf<String> > retVal = ArrayOf<String>::Alloc(list.GetSize());
    List<String>::Iterator it;
    Int32 i;
    for (it = list.Begin(), i = 0; it != list.End(); ++it, ++i) {
        (*retVal)[i] = *it;
    }
    *result = retVal;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CTethering::GetTetheredIfacePairs(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    List<String> list;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it;
        for (it = mIfaces.Begin(); it != mIfaces.End(); ++it) {
            AutoPtr<TetherInterfaceSM> sm = it->mSecond;
            if (sm->IsTethered()) {
                list.PushBack(sm->mMyUpstreamIfaceName);
                list.PushBack(sm->mIfaceName);
            }
        }
    }
    AutoPtr< ArrayOf<String> > retVal = ArrayOf<String>::Alloc(list.GetSize());
    List<String>::Iterator it;
    Int32 i;
    for (it = list.Begin(), i = 0; it != list.End(); ++it, ++i) {
        (*retVal)[i] = *it;
    }
    *result = retVal;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

ECode CTethering::GetTetherableIfaces(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    List<String> list;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it;
        for (it = mIfaces.Begin(); it != mIfaces.End(); ++it) {
            AutoPtr<TetherInterfaceSM> sm = it->mSecond;
            if (sm->IsAvailable()) {
                list.PushBack(it->mFirst);
            }
        }
    }
    AutoPtr< ArrayOf<String> > retVal = ArrayOf<String>::Alloc(list.GetSize());
    List<String>::Iterator it;
    Int32 i;
    for (it = list.Begin(), i = 0; it != list.End(); ++it, ++i) {
        (*retVal)[i] = *it;
    }
    *result = retVal;
    ARRAYOF_ADDREF(*result);
    return NOERROR;
}

AutoPtr< ArrayOf<String> > CTethering::GetErroredIfaces()
{
    List<String> list;
    {
        Object::Autolock lock(mPublicSync);
        HashMap<String, AutoPtr<TetherInterfaceSM> >::Iterator it;
        for (it = mIfaces.Begin(); it != mIfaces.End(); ++it) {
            AutoPtr<TetherInterfaceSM> sm = it->mSecond;
            if (sm->IsErrored()) {
                list.PushBack(it->mFirst);
            }
        }
    }
    AutoPtr< ArrayOf<String> > retVal = ArrayOf<String>::Alloc(list.GetSize());
    List<String>::Iterator it;
    Int32 i;
    for (it = list.Begin(), i = 0; it != list.End(); ++it, ++i) {
        (*retVal)[i] = *it;
    }
    return retVal;
}

//TODO: Temporary handling upstream change triggered without
//      CONNECTIVITY_ACTION. Only to accomodate interface
//      switch during HO.
//      @see bug/4455071
void CTethering::HandleTetherIfaceChange()
{
    mTetherMasterSM->SendMessage(TetherMasterSM::CMD_UPSTREAM_CHANGED);
}

ECode CTethering::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    assert(0);
//     Boolean result;
// //  mContext->CheckCallingOrSelfPermission(
// //          (android::Manifest::permission::DUMP) != IPackageManager::PERMISSION_GRANTED, &result);
// //
//     if (result) {
// //        /*pw.println("Permission Denial: can't dump ConnectivityService.Tether " +
// //                "from from pid=" + Binder.getCallingPid() + ", uid=" +
// //                Binder.getCallingUid());*/
//         return NOERROR;
//     }

//     {
//         Object::Autolock lock(mPublicSync);
// //      //pw.println("mUpstreamIfaceTypes: ");
// //      /*for (Integer netType : mUpstreamIfaceTypes) {
// //          pw.println(" " + netType);
// //      }*/
// //
// //      //pw.println();
// //      //pw.println("Tether state:");
// //      /*for (Object o : mIfaces.values()) {
// //          pw.println(" " + o);
// //      }*/
//     }
// //
// //    //pw.println();
    return NOERROR;
}

ECode CTethering::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

} // namespace Connectivity
} // namespace Server
} // namespace Droid
} // namespace Elastos
