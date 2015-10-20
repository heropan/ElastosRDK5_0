
#include "elastos/droid/net/wifi/CWifiManager.h"
// #include "elastos/droid/net/wifi/CWifiConfiguration.h"
#include "elastos/droid/os/CBinder.h"
#include "elastos/droid/os/CWorkSource.h"
#include "elastos/droid/os/CHandlerThread.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Concurrent::CCountDownLatch;
using Elastos::Droid::Os::CBinder;
using Elastos::Droid::Os::CWorkSource;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Utility::IProtocol;

namespace Elastos {
namespace Droid {
namespace Wifi {

const String CWifiManager::TAG("WifiManager");
const Int32 CWifiManager::MIN_RSSI = -100;
const Int32 CWifiManager::MAX_RSSI = -55;
const Int32 CWifiManager::MAX_ACTIVE_LOCKS = 50;
const Int32 CWifiManager::INVALID_KEY = 0;
const Int32 CWifiManager::BASE = IProtocol::BASE_WIFI_MANAGER;

Mutex CWifiManager::sThreadRefLock;
Int32 CWifiManager::sThreadRefCount;
AutoPtr<IHandlerThread> CWifiManager::sHandlerThread;


//==============================================================
// CWifiManager::WifiLock
//==============================================================
CWifiManager::WifiLock::WifiLock(
    /* [in] */ Int32 lockType,
    /* [in] */ const String& tag,
    /* [in] */ CWifiManager* owner)
    : mTag(tag)
    , mRefCount(0)
    , mLockType(lockType)
    , mRefCounted(TRUE)
    , mHeld(FALSE)
    , mOwner(owner)
{
    CBinder::New((IBinder**)&mBinder);
}

CWifiManager::WifiLock::~WifiLock()
{
    AutoLock lock(mBinderLock);
    if (mHeld) {
        // try {
        Boolean result = FALSE;
        mOwner->mService->ReleaseWifiLock(mBinder, &result);
        {
            AutoLock lock(mOwner->_m_syncLock);
            mOwner->mActiveLockCount--;
        }
        // } catch (RemoteException ignore) {
        // }
    }
}

CAR_INTERFACE_IMPL(CWifiManager::WifiLock, IWifiLock)

ECode CWifiManager::WifiLock::AcquireLock()
{
    AutoLock lock(mBinderLock);
    if (mRefCounted ? (++mRefCount == 1) : (!mHeld)) {
        // try {
        Boolean result = FALSE;
        mOwner->mService->AcquireWifiLock(mBinder, mLockType, mTag, mWorkSource, &result);
        {
            AutoLock lock(mOwner->_m_syncLock);

            if (mOwner->mActiveLockCount >= MAX_ACTIVE_LOCKS) {
                mOwner->mService->ReleaseWifiLock(mBinder, &result);
                // throw new UnsupportedOperationException(
                //             "Exceeded maximum number of wifi locks");
                return E_UNSUPPORTED_OPERATION_EXCEPTION;
            }
            mOwner->mActiveLockCount++;
        }
        // } catch (RemoteException ignore) {
        // }
        mHeld = TRUE;
    }
    return NOERROR;
}

ECode CWifiManager::WifiLock::ReleaseLock()
{
    AutoLock lock(mBinderLock);
    if (mRefCounted ? (--mRefCount == 0) : (mHeld)) {
        // try {
        Boolean result = FALSE;
        mOwner->mService->ReleaseWifiLock(mBinder, &result);
        {
            AutoLock lock(mOwner->_m_syncLock);
            mOwner->mActiveLockCount--;
        }
        // } catch (RemoteException ignore) {
        // }
        mHeld = FALSE;
    }
    if (mRefCount < 0) {
        // throw new RuntimeException("WifiLock under-locked " + mTag);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CWifiManager::WifiLock::SetReferenceCounted(
    /* [in] */ Boolean refCounted)
{
    mRefCounted = refCounted;
    return NOERROR;
}

ECode CWifiManager::WifiLock::IsHeld(
    /* [out] */ Boolean* held)
{
    VALIDATE_NOT_NULL(held);
    AutoLock lock(mBinderLock);
    *held = mHeld;
    return NOERROR;
}

ECode CWifiManager::WifiLock::SetWorkSource(
    /* [in] */ IWorkSource* ws)
{
    AutoLock lock(mBinderLock);

    Int32 size = 0;
    if (ws != NULL && (ws->GetSize(&size), size == 0)) {
        ws = NULL;
    }
    Boolean changed = TRUE;
    if (ws == NULL) {
        mWorkSource = NULL;
    }
    else if (mWorkSource == NULL) {
        changed = mWorkSource != NULL;
        CWorkSource::New(ws, (IWorkSource**)&mWorkSource);
    }
    else {
        mWorkSource->Diff(ws, &changed);
        if (changed) {
            mWorkSource->Set(ws);
        }
    }
    if (changed && mHeld) {
        // try {
        mOwner->mService->UpdateWifiLockWorkSource(mBinder, mWorkSource);
        // } catch (RemoteException e) {
        // }
    }
    return NOERROR;
}


//==============================================================
// CWifiManager::MulticastLock
//==============================================================
CWifiManager::MulticastLock::MulticastLock(
    /* [in] */ const String& tag,
    /* [in] */ CWifiManager* owner)
    : mTag(tag)
    , mRefCount(0)
    , mRefCounted(TRUE)
    , mHeld(FALSE)
    , mOwner(owner)
{
    CBinder::New((IBinder**)&mBinder);
}

CWifiManager::MulticastLock::~MulticastLock()
{
    SetReferenceCounted(FALSE);
    ReleaseLock();
}

CAR_INTERFACE_IMPL(CWifiManager::MulticastLock, IMulticastLock)

ECode CWifiManager::MulticastLock::AcquireLock()
{
    {
        AutoLock lock(mBinderLock);

        if (mRefCounted ? (++mRefCount == 1) : (!mHeld)) {
            // try {
            mOwner->mService->AcquireMulticastLock(mBinder, mTag);
            {
                AutoLock lock(mOwner->_m_syncLock);

                if (mOwner->mActiveLockCount >= MAX_ACTIVE_LOCKS) {
                    mOwner->mService->ReleaseMulticastLock();
                    // throw new UnsupportedOperationException(
                    //         "Exceeded maximum number of wifi locks");
                    return E_UNSUPPORTED_OPERATION_EXCEPTION;
                }
                mOwner->mActiveLockCount++;
            }
            // } catch (RemoteException ignore) {
            // }
            mHeld = TRUE;
        }
    }
    return NOERROR;
}

ECode CWifiManager::MulticastLock::ReleaseLock()
{
    AutoLock lock(mBinderLock);

    if (mRefCounted ? (--mRefCount == 0) : (mHeld)) {
        // try {
        mOwner->mService->ReleaseMulticastLock();
        {
            AutoLock lock(mOwner->_m_syncLock);
            mOwner->mActiveLockCount--;
        }
        // } catch (RemoteException ignore) {
        // }
        mHeld = FALSE;
    }
    if (mRefCount < 0) {
        // throw new RuntimeException("MulticastLock under-locked "
        //         + mTag);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CWifiManager::MulticastLock::SetReferenceCounted(
    /* [in] */ Boolean refCounted)
{
    mRefCounted = refCounted;
    return NOERROR;
}

ECode CWifiManager::MulticastLock::IsHeld(
    /* [in] */ Boolean* held)
{
    VALIDATE_NOT_NULL(held);
    AutoLock lock(mBinderLock);
    *held = mHeld;
    return NOERROR;
}


//==============================================================
// CWifiManager::ServiceHandler
//==============================================================

ECode CWifiManager::ServiceHandler::HandleMessage(
    /* [in] */ IMessage* message)
{
    Int32 arg2;
    message->GetArg2(&arg2);
    AutoPtr<IInterface> listener = mOwner->RemoveListener(arg2);
    Int32 what;
    message->GetWhat(&what);
    switch (what) {
        case AsyncChannel::CMD_CHANNEL_HALF_CONNECTED:
        {
            // Logger::D(TAG, "ServiceHandler HandleMessage CMD_CHANNEL_HALF_CONNECTED");
            Int32 arg1;
            message->GetArg1(&arg1);
            if (arg1 == AsyncChannel::STATUS_SUCCESSFUL) {
                mOwner->mAsyncChannel->SendMessage(AsyncChannel::CMD_CHANNEL_FULL_CONNECTION);
            } else {
                Logger::E(TAG, "Failed to set up channel connection");
                // This will cause all further async API calls on the WifiManager
                // to fail and throw an exception
                if (mOwner->mAsyncChannel) {
                    mOwner->mAsyncChannel->Disconnected();
                    mOwner->mAsyncChannel = NULL;
                }
            }
            mOwner->mConnected->CountDown();
            break;
        }
        case AsyncChannel::CMD_CHANNEL_FULLY_CONNECTED:
            // Ignore
            break;
        case AsyncChannel::CMD_CHANNEL_DISCONNECTED:
            // Logger::E(TAG, "ServiceHandler HandleMessage CMD_CHANNEL_DISCONNECTED Channel connection lost");
            // This will cause all further async API calls on the WifiManager
            // to fail and throw an exception
            if (mOwner->mAsyncChannel) {
                mOwner->mAsyncChannel->Disconnected();
                mOwner->mAsyncChannel = NULL;
            }
            mLooper->Quit();
            break;
            /* ActionListeners grouped together */
        case IWifiManager::CONNECT_NETWORK_FAILED:
        case IWifiManager::FORGET_NETWORK_FAILED:
        case IWifiManager::SAVE_NETWORK_FAILED:
        case IWifiManager::CANCEL_WPS_FAILED:
        case IWifiManager::DISABLE_NETWORK_FAILED:
        {
            Int32 arg1;
            message->GetArg1(&arg1);
            if (listener != NULL) {
                IWifiManagerActionListener::Probe(listener)->OnFailure(arg1);
            }
            break;
        }
            /* ActionListeners grouped together */
        case IWifiManager::CONNECT_NETWORK_SUCCEEDED:
        case IWifiManager::FORGET_NETWORK_SUCCEEDED:
        case IWifiManager::SAVE_NETWORK_SUCCEEDED:
        case IWifiManager::CANCEL_WPS_SUCCEDED:
        case IWifiManager::DISABLE_NETWORK_SUCCEEDED:
            if (listener != NULL) {
                IWifiManagerActionListener::Probe(listener)->OnSuccess();
            }
            break;
        case IWifiManager::START_WPS_SUCCEEDED:
        {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            if (listener != NULL) {
                AutoPtr<IWpsResult> result = IWpsResult::Probe(obj);
                String pin;
                result->GetPin(&pin);
                IWpsListener::Probe(listener)->OnStartSuccess(pin);
                //Listener needs to stay until completion or failure
                AutoLock lock(mOwner->mListenerMapLock);
                mOwner->mListenerMap[arg2] = listener;
            }
            break;
        }
        case IWifiManager::WPS_COMPLETED:
            if (listener != NULL) {
                IWpsListener::Probe(listener)->OnCompletion();
            }
            break;
        case IWifiManager::WPS_FAILED:
        {
            Int32 arg1;
            message->GetArg1(&arg1);
            if (listener != NULL) {
                IWpsListener::Probe(listener)->OnFailure(arg1);
            }
            break;
        }
        case IWifiManager::RSSI_PKTCNT_FETCH_SUCCEEDED:
        {
            AutoPtr<IInterface> obj;
            message->GetObj((IInterface**)&obj);
            if (listener != NULL) {
                AutoPtr<IRssiPacketCountInfo> info = IRssiPacketCountInfo::Probe(obj);
                if (info != NULL) {
                    Int32 txgood, txbad;
                    info->GetTxgood(&txgood);
                    info->GetTxbad(&txbad);
                    ITxPacketCountListener::Probe(listener)->OnSuccess(txgood + txbad);
                }
                else {
                    ITxPacketCountListener::Probe(listener)->OnFailure(IWifiManager::ERROR);
                }
            }
            break;
        }
        case IWifiManager::RSSI_PKTCNT_FETCH_FAILED:
        {
            Int32 arg1;
            message->GetArg1(&arg1);
            if (listener != NULL) {
                ITxPacketCountListener::Probe(listener)->OnFailure(arg1);
            }
            break;
        }
        default:
            //ignore
            break;
    }
    return NOERROR;
}

//==============================================================
// CWifiManager
//==============================================================
CWifiManager::CWifiManager()
    : mActiveLockCount(0)
    , mListenerKey(1)
{
    mAsyncChannel = new AsyncChannel();
    CCountDownLatch::New(1, (ICountDownLatch**)&mConnected);
}

CWifiManager::~CWifiManager()
{
    if (mAsyncChannel) {
        mAsyncChannel->Disconnected();
        mAsyncChannel = NULL;
    }

    //try {
    {
        AutoLock lock(sThreadRefLock);
        if (--sThreadRefCount == 0 && sHandlerThread != NULL) {
            AutoPtr<ILooper> l;
            sHandlerThread->GetLooper((ILooper**)&l);
            if (l != NULL) {
                l->Quit();
            }
        }
    }
    //} finally {
    //super.finalize();
    //}
}

ECode CWifiManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIWifiManager* service)
{
    mContext = context;
    mService = service;
    Init();
    return NOERROR;
}

ECode CWifiManager::GetConfiguredNetworks(
    /* [out] */ IObjectContainer** networks)
{
    VALIDATE_NOT_NULL(networks);
    // try {
    return mService->GetConfiguredNetworks(networks);
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CWifiManager::AddNetwork(
    /* [in] */ IWifiConfiguration* config,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    if (config == NULL) {
        *id = -1;
        return NOERROR;
    }
    config->SetNetworkId(-1);
    *id = AddOrUpdateNetwork(config);
    return NOERROR;
}

ECode CWifiManager::UpdateNetwork(
    /* [in] */ IWifiConfiguration* config,
    /* [out] */ Int32* networkId)
{
    VALIDATE_NOT_NULL(networkId);
    Int32 netId;
    if (config == NULL || (config->GetNetworkId(&netId), netId < 0)) {
        *networkId = -1;
        return NOERROR;
    }
    *networkId = AddOrUpdateNetwork(config);
    return NOERROR;
}

Int32 CWifiManager::AddOrUpdateNetwork(
    /* [in] */ IWifiConfiguration* config)
{
    // try {
    Int32 id = 0;
    ECode ec = mService->AddOrUpdateNetwork(config, &id);
    if (FAILED(ec)) {
        return -1;
    }
    return id;
    // } catch (RemoteException e) {
    //     return -1;
    // }
}

ECode CWifiManager::RemoveNetwork(
    /* [in] */ Int32 netId,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // try {
    return mService->RemoveNetwork(netId, succeeded);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CWifiManager::EnableNetwork(
    /* [in] */ Int32 netId,
    /* [in] */ Boolean disableOthers,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // try {
    return mService->EnableNetwork(netId, disableOthers, succeeded);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CWifiManager::DisableNetwork(
    /* [in] */ Int32 netId,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // try {
    return mService->DisableNetwork(netId, succeeded);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CWifiManager::Disconnect(
    /*[out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = TRUE;
    ECode ec = mService->Disconnect();
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
}

ECode CWifiManager::Reconnect(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = TRUE;
    ECode ec = mService->Reconnect();
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
}

ECode CWifiManager::Reassociate(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = TRUE;
    ECode ec = mService->Reassociate();
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
}

ECode CWifiManager::PingSupplicant(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    if (mService == NULL) {
        *succeeded = FALSE;
        return NOERROR;
    }
    // try {
    return mService->PingSupplicant(succeeded);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CWifiManager::StartScan(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    *succeeded = TRUE;
    ECode ec = mService->StartScan(FALSE);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
}

ECode CWifiManager::StartScanActive(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded)
    *succeeded = TRUE;
    ECode ec = mService->StartScan(TRUE);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
}

ECode CWifiManager::GetConnectionInfo(
    /* [out] */ IWifiInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    AutoPtr<IWifiInfo> temp;
    FAIL_RETURN(mService->GetConnectionInfo((IWifiInfo**)&temp));
    *info = temp;
    REFCOUNT_ADD(*info);
    return NOERROR;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
}

ECode CWifiManager::GetScanResults(
    /* [out] */ IObjectContainer** results)
{
    VALIDATE_NOT_NULL(results);
    *results = NULL;

    AutoPtr<IObjectContainer> temp;
    FAIL_RETURN(mService->GetScanResults((IObjectContainer**)&temp));
    *results = temp;
    REFCOUNT_ADD(*results);
    return NOERROR;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
}

ECode CWifiManager::SaveConfiguration(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // try {
    ECode ec = mService->SaveConfiguration(succeeded);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode CWifiManager::SetCountryCode(
    /* [in] */ const String& country,
    /* [in] */ Boolean persist)
{
    // try {
    return mService->SetCountryCode(country, persist);
    // } catch (RemoteException e) { }
}

ECode CWifiManager::SetFrequencyBand(
    /* [in] */ Int32 band,
    /* [in] */ Boolean persist)
{
    // try {
    return mService->SetFrequencyBand(band, persist);
    // } catch (RemoteException e) { }
}

ECode CWifiManager::GetFrequencyBand(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    // try {
    ECode ec = mService->GetFrequencyBand(state);
    if (FAILED(ec)) {
        *state = -1;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     return -1;
    // }
}

ECode CWifiManager::IsDualBandSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    // try {
    ECode ec = mService->IsDualBandSupported(supported);
    if (FAILED(ec)) {
        *supported = FALSE;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode CWifiManager::GetDhcpInfo(
    /* [out] */ IDhcpInfo** info)
{
    VALIDATE_NOT_NULL(info);
    // try {
    ECode ec = mService->GetDhcpInfo(info);
    if (FAILED(ec)) {
        *info = NULL;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     return NULL;
    // }
}

ECode CWifiManager::SetWifiEnabled(
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    // try {
    ECode ec = mService->SetWifiEnabled(enabled, succeeded);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     return FALSE;
    // }
}

ECode CWifiManager::GetWifiState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    // try {
    ECode ec = mService->GetWifiEnabledState(state);
    if (FAILED(ec)) {
        *state = IWifiManager::WIFI_STATE_UNKNOWN;
    }
    return NOERROR;
    // } catch (RemoteException e) {
    //     return WIFI_STATE_UNKNOWN;
    // }
}

ECode CWifiManager::IsWifiEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    Int32 state = 0;
    *enabled = (GetWifiState(&state), state) == WIFI_STATE_ENABLED;
    return NOERROR;
}

ECode CWifiManager::GetTxPacketCount(
    /* [in] */ ITxPacketCountListener* listener)
{
    ValidateChannel();
    mAsyncChannel->SendMessage(IWifiManager::RSSI_PKTCNT_FETCH, 0, PutListener(listener));
    return NOERROR;
}

Int32 CWifiManager::CalculateSignalLevel(
    /* [in] */ Int32 rssi,
    /* [in] */ Int32 numLevels)
{
    if (rssi <= MIN_RSSI) {
        return 0;
    }
    else if (rssi >= MAX_RSSI) {
        return numLevels - 1;
    }
    else {
        Float inputRange = (MAX_RSSI - MIN_RSSI);
        Float outputRange = (numLevels - 1);
        return (Int32)((Float)(rssi - MIN_RSSI) * outputRange / inputRange);
    }
}

Int32 CWifiManager::CompareSignalLevel(
    /* [in] */ Int32 rssiA,
    /* [in] */ Int32 rssiB)
{
    return rssiA - rssiB;
}

ECode CWifiManager::SetWifiApEnabled(
    /* [in] */ IWifiConfiguration* wifiConfig,
    /* [in] */ Boolean enabled,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->SetWifiApEnabled(wifiConfig, enabled);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

ECode CWifiManager::GetWifiApState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    //try {
    ECode ec = mService->GetWifiApEnabledState(state);
    if (FAILED(ec)) {
        *state = WIFI_AP_STATE_FAILED;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //    return WIFI_AP_STATE_FAILED;
    //}
}

ECode CWifiManager::IsWifiApEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    Int32 state = 0;
    *enabled = (GetWifiApState(&state), state) == WIFI_AP_STATE_ENABLED;
    return NOERROR;
}

ECode CWifiManager::GetWifiApConfiguration(
    /* [out] */ IWifiConfiguration** config)
{
    VALIDATE_NOT_NULL(config);
    //try {
    ECode ec = mService->GetWifiApConfiguration(config);
    if (FAILED(ec)) {
        *config = NULL;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //    return NULL;
    //}
}

ECode CWifiManager::SetWifiApConfiguration(
    /* [in] */ IWifiConfiguration* wifiConfig,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->SetWifiApConfiguration(wifiConfig);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

ECode CWifiManager::StartWifi(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->StartWifi();
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

ECode CWifiManager::StopWifi(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->StopWifi();
    if (ec != NOERROR) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

ECode CWifiManager::AddToBlacklist(
    /* [in] */ const String& bssid,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->AddToBlacklist(bssid);
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

ECode CWifiManager::ClearBlacklist(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->ClearBlacklist();
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

Int32 CWifiManager::PutListener(
    /* [in] */ IInterface* listener)
{
    if (listener == NULL) return INVALID_KEY;
    Int32 key = 0;
    {
        AutoLock lock(mListenerMapLock);

        do {
            key = mListenerKey++;
        } while (key == INVALID_KEY);
        mListenerMap[key] = listener;
    }
    return key;
}

AutoPtr<IInterface> CWifiManager::RemoveListener(
    /* [in] */ Int32 key)
{
    if (key == INVALID_KEY) return NULL;
    {
        AutoLock lock(mListenerMapLock);

        AutoPtr<IInterface> listener;
        HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mListenerMap.Find(key);
        if (it != mListenerMap.End()) {
            listener = it->mSecond;
            mListenerMap.Erase(it);
        }
        return listener;
    }
}

void CWifiManager::Init()
{
    GetWifiServiceMessenger((IMessenger**)&mWifiServiceMessenger);
    if (mWifiServiceMessenger == NULL) {
        mAsyncChannel = NULL;
        return;
    }

    {
        AutoLock lock(sThreadRefLock);
        if (++sThreadRefCount == 1) {
            sHandlerThread = NULL;
            CHandlerThread::New(String("WifiManager"), (IHandlerThread**)&sHandlerThread);
            sHandlerThread->Start();
        }
    }

    AutoPtr<ILooper> l;
    sHandlerThread->GetLooper((ILooper**)&l);
    mHandler = new ServiceHandler(l, this);
    mAsyncChannel->Connect(mContext, mHandler, mWifiServiceMessenger);
    // try {
    if (FAILED(mConnected->Await())) {
    // } catch (InterruptedException e) {
        Logger::E(TAG, "interrupted wait at init");
    }
}

ECode CWifiManager::ValidateChannel()
{
    if (mAsyncChannel == NULL) {
        //throw new IllegalStateException
        Logger::E(TAG,
           "No permission to access and change wifi or a bad initialization");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CWifiManager::Connect(
    /* [in] */ IWifiConfiguration* config,
    /* [in] */ IWifiManagerActionListener* listener)
{
    if (config == NULL) {
        // throw new IllegalArgumentException("config cannot be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ValidateChannel();
    // Use INVALID_NETWORK_ID for arg1 when passing a config object
    // arg1 is used to pass network id when the network already exists
    mAsyncChannel->SendMessage(CONNECT_NETWORK, IWifiConfiguration::INVALID_NETWORK_ID,
           PutListener(listener), config);
    return NOERROR;
}

ECode CWifiManager::Connect(
    /* [in] */ Int32 networkId,
    /* [in] */ IWifiManagerActionListener* listener)
{
    if (networkId < 0) {
        // throw new IllegalArgumentException("Network id cannot be negative");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ValidateChannel();
    mAsyncChannel->SendMessage(CONNECT_NETWORK, networkId, PutListener(listener));
    return NOERROR;
}

ECode CWifiManager::Save(
    /* [in] */ IWifiConfiguration* config,
    /* [in] */ IWifiManagerActionListener* listener)
{
    if (config == NULL) {
        // throw new IllegalArgumentException("config cannot be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ValidateChannel();
    mAsyncChannel->SendMessage(SAVE_NETWORK, 0, PutListener(listener), config);
    return NOERROR;
}

ECode CWifiManager::Forget(
    /* [in] */ Int32 netId,
    /* [in] */ IWifiManagerActionListener* listener)
{
    if (netId < 0) {
        // throw new IllegalArgumentException("Network id cannot be negative");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ValidateChannel();
    mAsyncChannel->SendMessage(FORGET_NETWORK, netId, PutListener(listener));
    return NOERROR;
}

ECode CWifiManager::Disable(
    /* [in] */ Int32 netId,
    /* [in] */ IWifiManagerActionListener* listener)
{
    if (netId < 0) {
        // throw new IllegalArgumentException("Network id cannot be negative");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ValidateChannel();
    mAsyncChannel->SendMessage(DISABLE_NETWORK, netId, PutListener(listener));
    return NOERROR;
}

ECode CWifiManager::StartWps(
    /* [in] */ IWpsInfo* config,
    /* [in] */ IWpsListener* listener)
{
    if (config == NULL) {
        // throw new IllegalArgumentException("config cannot be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ValidateChannel();
    mAsyncChannel->SendMessage(START_WPS, 0, PutListener(listener), config);
    return NOERROR;
}

ECode CWifiManager::CancelWps(
    /* [in] */ IWifiManagerActionListener* listener)
{
    ValidateChannel();
    mAsyncChannel->SendMessage(CANCEL_WPS, 0, PutListener(listener));
    return NOERROR;
}

ECode CWifiManager::GetWifiServiceMessenger(
    /* [out] */ IMessenger** msgr)
{
    VALIDATE_NOT_NULL(msgr);
    //try {
    ECode ec = mService->GetWifiServiceMessenger(msgr);
    if (FAILED(ec)) {
        *msgr = NULL;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //    return NULL;
    //} catch (SecurityException e) {
    //    return NULL;
    //}
}

ECode CWifiManager::GetWifiStateMachineMessenger(
    /* [out] */ IMessenger** msgr)
{
    VALIDATE_NOT_NULL(msgr);
    //try {
    ECode ec = mService->GetWifiStateMachineMessenger(msgr);
    if (FAILED(ec)) {
        *msgr = NULL;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //    return NULL;
    //}
}

ECode CWifiManager::GetConfigFile(
    /* [out] */ String* cfgFile)
{
    VALIDATE_NOT_NULL(cfgFile);
    //try {
    return mService->GetConfigFile(cfgFile);
    //} catch (RemoteException e) {
    //    return NULL;
    //}
}

ECode CWifiManager::CreateWifiLock(
    /* [in] */ Int32 lockType,
    /* [in] */ const String& tag,
    /* [out] */ IWifiLock** lock)
{
    VALIDATE_NOT_NULL(lock);
    *lock = new WifiLock(lockType, tag, this);
    REFCOUNT_ADD(*lock);
    return NOERROR;
}

ECode CWifiManager::CreateWifiLock(
    /* [in] */ const String& tag,
    /* [out] */ IWifiLock** lock)
{
    VALIDATE_NOT_NULL(lock);
    *lock = new WifiLock(WIFI_MODE_FULL, tag, this);
    REFCOUNT_ADD(*lock);
    return NOERROR;
}

ECode CWifiManager::CreateMulticastLock(
    /* [in] */ const String& tag,
    /* [out] */ IMulticastLock** lock)
{
    VALIDATE_NOT_NULL(lock);
    *lock = new MulticastLock(tag, this);
    REFCOUNT_ADD(*lock);
    return NOERROR;
}

ECode CWifiManager::IsMulticastEnabled(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    //try {
    ECode ec = mService->IsMulticastEnabled(allowed);
    if (FAILED(ec)) {
        *allowed = FALSE;
    }
    return NOERROR;
    //} catch (RemoteException e) {
    //    return FALSE;
    //}
}

ECode CWifiManager::InitializeMulticastFiltering(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    //try {
    *succeeded = TRUE;
    ECode ec = mService->InitializeMulticastFiltering();
    if (FAILED(ec)) {
        *succeeded = FALSE;
    }
    return NOERROR;
    //    return TRUE;
    //} catch (RemoteException e) {
    //     return FALSE;
    //}
}

ECode CWifiManager::CaptivePortalCheckComplete()
{
    //try {
    return mService->CaptivePortalCheckComplete();
    //} catch (RemoteException e) {}
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos
