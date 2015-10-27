
#include "elastos/droid/net/NetworkInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(NetworkInfo, Object, IParcelable, INetworkInfo)

const AutoPtr<IEnumMap> NetworkInfo::STATE_MAP = CreateStateMap();

AutoPtr<IEnumMap> NetworkInfo::CreateStateMap()
{
    AutoPtr<IEnumMap> rev;
#if 0 // TODO: Translate codes below
    private static final EnumMap<DetailedState, State> stateMap =
        new EnumMap<DetailedState, State>(DetailedState.class);

    static {
        stateMap.put(DetailedState.IDLE, State.DISCONNECTED);
        stateMap.put(DetailedState.SCANNING, State.DISCONNECTED);
        stateMap.put(DetailedState.CONNECTING, State.CONNECTING);
        stateMap.put(DetailedState.AUTHENTICATING, State.CONNECTING);
        stateMap.put(DetailedState.OBTAINING_IPADDR, State.CONNECTING);
        stateMap.put(DetailedState.VERIFYING_POOR_LINK, State.CONNECTING);
        stateMap.put(DetailedState.CAPTIVE_PORTAL_CHECK, State.CONNECTING);
        stateMap.put(DetailedState.CONNECTED, State.CONNECTED);
        stateMap.put(DetailedState.SUSPENDED, State.SUSPENDED);
        stateMap.put(DetailedState.DISCONNECTING, State.DISCONNECTING);
        stateMap.put(DetailedState.DISCONNECTED, State.DISCONNECTED);
        stateMap.put(DetailedState.FAILED, State.DISCONNECTED);
        stateMap.put(DetailedState.BLOCKED, State.DISCONNECTED);
    }
#else
    assert(0);
#endif
    return rev;
}

ECode NetworkInfo::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 subtype,
    /* [in] */ const String& typeName,
    /* [in] */ const String& subtypeName)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (!ConnectivityManager.isNetworkTypeValid(type)) {
            throw new IllegalArgumentException("Invalid network type: " + type);
        }
        mNetworkType = type;
        mSubtype = subtype;
        mTypeName = typeName;
        mSubtypeName = subtypeName;
        setDetailedState(DetailedState.IDLE, null, null);
        mState = State.UNKNOWN;
        mIsAvailable = false; // until we're told otherwise, assume unavailable
        mIsRoaming = false;
        mIsConnectedToProvisioningNetwork = false;

#endif
}

ECode NetworkInfo::constructor(
    /* [in] */ INetworkInfo* source)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (source != null) {
            synchronized (source) {
                mNetworkType = source.mNetworkType;
                mSubtype = source.mSubtype;
                mTypeName = source.mTypeName;
                mSubtypeName = source.mSubtypeName;
                mState = source.mState;
                mDetailedState = source.mDetailedState;
                mReason = source.mReason;
                mExtraInfo = source.mExtraInfo;
                mIsFailover = source.mIsFailover;
                mIsRoaming = source.mIsRoaming;
                mIsAvailable = source.mIsAvailable;
                mIsConnectedToProvisioningNetwork = source.mIsConnectedToProvisioningNetwork;
            }
        }

#endif
}

ECode NetworkInfo::GetType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mNetworkType;
        }

#endif
}

ECode NetworkInfo::SetType(
    /* [in] */ Int32 type)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            mNetworkType = type;
        }

#endif
}

ECode NetworkInfo::GetSubtype(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mSubtype;
        }

#endif
}

ECode NetworkInfo::SetSubtype(
    /* [in] */ Int32 subtype,
    /* [in] */ const String& subtypeName)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            mSubtype = subtype;
            mSubtypeName = subtypeName;
        }

#endif
}

ECode NetworkInfo::GetTypeName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mTypeName;
        }

#endif
}

ECode NetworkInfo::GetSubtypeName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mSubtypeName;
        }

#endif
}

ECode NetworkInfo::IsConnectedOrConnecting(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mState == State.CONNECTED || mState == State.CONNECTING;
        }

#endif
}

ECode NetworkInfo::IsConnected(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mState == State.CONNECTED;
        }

#endif
}

ECode NetworkInfo::IsAvailable(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mIsAvailable;
        }

#endif
}

ECode NetworkInfo::SetIsAvailable(
    /* [in] */ Boolean isAvailable)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            mIsAvailable = isAvailable;
        }

#endif
}

ECode NetworkInfo::IsFailover(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mIsFailover;
        }

#endif
}

ECode NetworkInfo::SetFailover(
    /* [in] */ Boolean isFailover)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            mIsFailover = isFailover;
        }

#endif
}

ECode NetworkInfo::IsRoaming(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mIsRoaming;
        }

#endif
}

ECode NetworkInfo::SetRoaming(
    /* [in] */ Boolean isRoaming)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            mIsRoaming = isRoaming;
        }

#endif
}

ECode NetworkInfo::IsConnectedToProvisioningNetwork(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mIsConnectedToProvisioningNetwork;
        }

#endif
}

ECode NetworkInfo::SetIsConnectedToProvisioningNetwork(
    /* [in] */ Boolean val)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            mIsConnectedToProvisioningNetwork = val;
        }

#endif
}

ECode NetworkInfo::GetState(
    /* [out] */ NetworkInfoState* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mState;
        }

#endif
}

ECode NetworkInfo::GetDetailedState(
    /* [out] */ NetworkInfoDetailedState* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mDetailedState;
        }

#endif
}

ECode NetworkInfo::SetDetailedState(
    /* [in] */ NetworkInfoDetailedState detailedState,
    /* [in] */ const String& reason,
    /* [in] */ const String& extraInfo)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            this.mDetailedState = detailedState;
            this.mState = stateMap.get(detailedState);
            this.mReason = reason;
            this.mExtraInfo = extraInfo;
        }

#endif
}

ECode NetworkInfo::SetExtraInfo(
    /* [in] */ const String& extraInfo)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            this.mExtraInfo = extraInfo;
        }

#endif
}

ECode NetworkInfo::GetReason(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mReason;
        }

#endif
}

ECode NetworkInfo::GetExtraInfo(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            return mExtraInfo;
        }

#endif
}

ECode NetworkInfo::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized (this) {
            StringBuilder builder = new StringBuilder("[");
            builder.append("type: ").append(getTypeName()).append("[").append(getSubtypeName()).
            append("], state: ").append(mState).append("/").append(mDetailedState).
            append(", reason: ").append(mReason == null ? "(unspecified)" : mReason).
            append(", extra: ").append(mExtraInfo == null ? "(none)" : mExtraInfo).
            append(", roaming: ").append(mIsRoaming).
            append(", failover: ").append(mIsFailover).
            append(", isAvailable: ").append(mIsAvailable).
            append(", isConnectedToProvisioningNetwork: ").
            append(mIsConnectedToProvisioningNetwork).
            append("]");
            return builder.toString();
        }

#endif
}

ECode NetworkInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public NetworkInfo createFromParcel(Parcel in) {
                int netType = in.readInt();
                int subtype = in.readInt();
                String typeName = in.readString();
                String subtypeName = in.readString();
                NetworkInfo netInfo = new NetworkInfo(netType, subtype, typeName, subtypeName);
                netInfo.mState = State.valueOf(in.readString());
                netInfo.mDetailedState = DetailedState.valueOf(in.readString());
                netInfo.mIsFailover = in.readInt() != 0;
                netInfo.mIsAvailable = in.readInt() != 0;
                netInfo.mIsRoaming = in.readInt() != 0;
                netInfo.mIsConnectedToProvisioningNetwork = in.readInt() != 0;
                netInfo.mReason = in.readString();
                netInfo.mExtraInfo = in.readString();
                return netInfo;
            }
            public NetworkInfo[] newArray(int size) {
                return new NetworkInfo[size];
            }

#endif
}

ECode NetworkInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
            public NetworkInfo createFromParcel(Parcel in) {
                int netType = in.readInt();
                int subtype = in.readInt();
                String typeName = in.readString();
                String subtypeName = in.readString();
                NetworkInfo netInfo = new NetworkInfo(netType, subtype, typeName, subtypeName);
                netInfo.mState = State.valueOf(in.readString());
                netInfo.mDetailedState = DetailedState.valueOf(in.readString());
                netInfo.mIsFailover = in.readInt() != 0;
                netInfo.mIsAvailable = in.readInt() != 0;
                netInfo.mIsRoaming = in.readInt() != 0;
                netInfo.mIsConnectedToProvisioningNetwork = in.readInt() != 0;
                netInfo.mReason = in.readString();
                netInfo.mExtraInfo = in.readString();
                return netInfo;
            }
            public NetworkInfo[] newArray(int size) {
                return new NetworkInfo[size];
            }

#endif
}



} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CNetworkInfo.cpp
#include "elastos/droid/net/CNetworkInfo.h"
#include "elastos/droid/net/CConnectivityManager.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Net {

HashMap<NetworkInfoDetailedState, NetworkInfoState> CNetworkInfo::mStateMap;
Int32 CNetworkInfo::mInitFlag = InternalInit();

Int32 CNetworkInfo::InternalInit()
{
    mStateMap[NetworkInfoDetailedState_IDLE] = NetworkInfoState_DISCONNECTED;
    mStateMap[NetworkInfoDetailedState_SCANNING] = NetworkInfoState_DISCONNECTED;
    mStateMap[NetworkInfoDetailedState_CONNECTING] = NetworkInfoState_CONNECTING;
    mStateMap[NetworkInfoDetailedState_AUTHENTICATING] = NetworkInfoState_CONNECTING;
    mStateMap[NetworkInfoDetailedState_OBTAINING_IPADDR] = NetworkInfoState_CONNECTING;
    mStateMap[NetworkInfoDetailedState_CONNECTED] = NetworkInfoState_CONNECTED;
    mStateMap[NetworkInfoDetailedState_SUSPENDED] = NetworkInfoState_SUSPENDED;
    mStateMap[NetworkInfoDetailedState_DISCONNECTING] = NetworkInfoState_DISCONNECTING;
    mStateMap[NetworkInfoDetailedState_DISCONNECTED] = NetworkInfoState_DISCONNECTED;
    mStateMap[NetworkInfoDetailedState_FAILED] = NetworkInfoState_DISCONNECTED;
    mStateMap[NetworkInfoDetailedState_BLOCKED] = NetworkInfoState_DISCONNECTED;
    mStateMap[NetworkInfoDetailedState_VERIFYING_POOR_LINK] = NetworkInfoState_CONNECTING;
    mStateMap[NetworkInfoDetailedState_CAPTIVE_PORTAL_CHECK] = NetworkInfoState_CONNECTING;

    return 0;
}

CNetworkInfo::CNetworkInfo()
    : mNetworkType(0)
    , mSubtype(0)
    , mState(NetworkInfoState_UNKNOWN)
    , mDetailedState(NetworkInfoDetailedState_IDLE)
    , mIsFailover(FALSE)
    , mIsRoaming(FALSE)
    , mIsAvailable(FALSE)
{}

ECode CNetworkInfo::constructor()
{
    return NOERROR;
}

/**
 * @param type network type
 * @deprecated
 * @hide because this constructor was only meant for internal use (and
 * has now been superseded by the package-private constructor below).
 */
ECode CNetworkInfo::constructor(
    /* [in] */ Int32 type)
{
    return NOERROR;
}

/**
 * @hide
 */
ECode CNetworkInfo::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Int32 subtype,
    /* [in] */ const String& typeName,
    /* [in] */ const String& subtypeName)
{
    Boolean bol;
    CConnectivityManager::IsNetworkTypeValid(type, &bol);
    if (!bol) {
//        throw new IllegalArgumentException("Invalid network type: " + type);
        Slogger::E(String("NetworkInfo"), "Invalid network type: %d", type);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNetworkType = type;
    mSubtype = subtype;
    mTypeName = typeName;
    mSubtypeName = subtypeName;
    SetDetailedState(NetworkInfoDetailedState_IDLE, String(), String());
    mState = NetworkInfoState_UNKNOWN;
    mIsAvailable = FALSE; // until we're told otherwise, assume unavailable
    mIsRoaming = FALSE;
    return NOERROR;
}

/** {@hide} */
ECode CNetworkInfo::constructor(
    /* [in] */ INetworkInfo* source)
{
    if (source != NULL) {
        source->GetType(&mNetworkType);
        source->GetSubtype(&mSubtype);
        source->GetTypeName(&mTypeName);
        source->GetSubtypeName(&mSubtypeName);
        source->GetState(&mState);
        source->GetDetailedState(&mDetailedState);
        source->GetReason(&mReason);
        source->GetExtraInfo(&mExtraInfo);
        source->IsFailover(&mIsFailover);
        source->IsRoaming(&mIsRoaming);
        source->IsAvailable(&mIsAvailable);
    }
    return NOERROR;
}

ECode CNetworkInfo::SetState(
    /* [in] */ NetworkInfoState state)
{
    AutoLock lock(this);
    mState = state;
    return NOERROR;
}

/**
 * Reports the type of network to which the
 * info in this {@code NetworkInfo} pertains.
 * @return one of {@link ConnectivityManager#TYPE_MOBILE}, {@link
 * ConnectivityManager#TYPE_WIFI}, {@link ConnectivityManager#TYPE_WIMAX}, {@link
 * ConnectivityManager#TYPE_ETHERNET},  {@link ConnectivityManager#TYPE_BLUETOOTH}, or other
 * types defined by {@link ConnectivityManager}
 */
ECode CNetworkInfo::GetType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mNetworkType;
    return NOERROR;
}

/**
 * Return a network-type-specific integer describing the subtype
 * of the network.
 * @return the network subtype
 */
ECode CNetworkInfo::GetSubtype(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mSubtype;
    return NOERROR;
}

ECode CNetworkInfo::SetSubtype(
    /* [in] */ Int32 subtype,
    /* [in] */ const String& subtypeName)
{
    AutoLock lock(this);
    mSubtype = subtype;
    mSubtypeName = subtypeName;
    return NOERROR;
}

/**
 * Return a human-readable name describe the type of the network,
 * for example "WIFI" or "MOBILE".
 * @return the name of the network type
 */
ECode CNetworkInfo::GetTypeName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    AutoLock lock(this);
    *name = mTypeName;
    return NOERROR;
}

/**
 * Return a human-readable name describing the subtype of the network.
 * @return the name of the network subtype
 */
ECode CNetworkInfo::GetSubtypeName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mSubtypeName;
    return NOERROR;
}

/**
 * Indicates whether network connectivity exists or is in the process
 * of being established. This is good for applications that need to
 * do anything related to the network other than read or write data.
 * For the latter, call {@link #isConnected()} instead, which guarantees
 * that the network is fully usable.
 * @return {@code true} if network connectivity exists or is in the process
 * of being established, {@code FALSE} otherwise.
 */
ECode CNetworkInfo::IsConnectedOrConnecting(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = ((mState == NetworkInfoState_CONNECTED) || mState == (NetworkInfoState_CONNECTING));
    return NOERROR;
}

/**
 * Indicates whether network connectivity exists and it is possible to establish
 * connections and pass data.
 * <p>Always call this before attempting to perform data transactions.
 * @return {@code true} if network connectivity exists, {@code FALSE} otherwise.
 */
ECode CNetworkInfo::IsConnected(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = (mState == NetworkInfoState_CONNECTED);
    return NOERROR;
}

/**
 * Indicates whether network connectivity is possible. A network is unavailable
 * when a persistent or semi-persistent condition prevents the possibility
 * of connecting to that network. Examples include
 * <ul>
 * <li>The device is out of the coverage area for any network of this type.</li>
 * <li>The device is on a network other than the home network (i.e., roaming), and
 * data roaming has been disabled.</li>
 * <li>The device's radio is turned off, e.g., because airplane mode is enabled.</li>
 * </ul>
 * @return {@code true} if the network is available, {@code FALSE} otherwise
 */
ECode CNetworkInfo::IsAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mIsAvailable;
    return NOERROR;
}

/**
 * Sets if the network is available, ie, if the connectivity is possible.
 * @param isAvailable the new availability value.
 *
 * @hide
 */
ECode CNetworkInfo::SetIsAvailable(
    /* [in] */ Boolean isAvailable)
{
    AutoLock lock(this);
    mIsAvailable = isAvailable;
    return NOERROR;
}

/**
 * Indicates whether the current attempt to connect to the network
 * resulted from the ConnectivityManager trying to fail over to this
 * network following a disconnect from another network.
 * @return {@code true} if this is a failover attempt, {@code FALSE}
 * otherwise.
 */
ECode CNetworkInfo::IsFailover(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mIsFailover;
    return NOERROR;
}

/**
 * Set the failover Boolean.
 * @param isFailover {@code true} to mark the current connection attempt
 * as a failover.
 * @hide
 */
ECode CNetworkInfo::SetFailover(
    /* [in] */ Boolean isFailover)
{
    AutoLock lock(this);
    mIsFailover = isFailover;
    return NOERROR;
}

/**
 * Indicates whether the device is currently roaming on this network.
 * When {@code true}, it suggests that use of data on this network
 * may incur extra costs.
 * @return {@code true} if roaming is in effect, {@code FALSE} otherwise.
 */
ECode CNetworkInfo::IsRoaming(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mIsRoaming;
    return NOERROR;
}

ECode CNetworkInfo::SetRoaming(
    /* [in] */ Boolean isRoaming)
{
    AutoLock lock(this);
    mIsRoaming = isRoaming;
    return NOERROR;
}

/**
 * Reports the current coarse-grained state of the network.
 * @return the coarse-grained state
 */
ECode CNetworkInfo::GetState(
    /* [out] */ NetworkInfoState* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mState;
    return NOERROR;
}

/**
 * Reports the current fine-grained state of the network.
 * @return the fine-grained state
 */
ECode CNetworkInfo::GetDetailedState(
    /* [out] */ NetworkInfoDetailedState* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mDetailedState;
    return NOERROR;
}

/**
 * Sets the fine-grained state of the network.
 * @param detailedState the {@link DetailedState}.
 * @param reason a {@code String} indicating the reason for the state change,
 * if one was supplied. May be {@code NULL}.
 * @param extraInfo an optional {@code String} providing addditional network state
 * information passed up from the lower networking layers.
 * @hide
 */
ECode CNetworkInfo::SetDetailedState(
    /* [in] */ NetworkInfoDetailedState detailedState,
    /* [in] */ const String& reason,
    /* [in] */ const String& extraInfo)
{
    AutoLock lock(this);
    mDetailedState = detailedState;
    Int32 num = detailedState;
    mState = mStateMap[num];
    mReason = reason;
    mExtraInfo = extraInfo;
    return NOERROR;
}

/**
 * Set the extraInfo field.
 * @param extraInfo an optional {@code String} providing addditional network state
 * information passed up from the lower networking layers.
 * @hide
 */
ECode CNetworkInfo::SetExtraInfo(
    /* [in] */ const String& extraInfo)
{
    AutoLock lock(this);
    mExtraInfo = extraInfo;
    return NOERROR;
}

/**
 * Report the reason an attempt to establish connectivity failed,
 * if one is available.
 * @return the reason for failure, or NULL if not available
 */
ECode CNetworkInfo::GetReason(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mReason;
    return NOERROR;
}

/**
 * Report the extra information about the network state, if any was
 * provided by the lower networking layers.,
 * if one is available.
 * @return the extra information, or NULL if not available
 */
ECode CNetworkInfo::GetExtraInfo(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);
    *result = mExtraInfo;
    return NOERROR;
}

//@Override
ECode CNetworkInfo::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(this);

    StringBuilder builder("NetworkInfo: ");
    builder.AppendCStr("type: ");
    String str;
    GetTypeName(&str);
    builder.AppendString(str);
    builder.AppendCStr("[");
    GetSubtypeName(&str);
    builder.AppendString(str);
    builder.AppendCStr("], state: ");
    builder.AppendInt32(mState);
    builder.AppendCStr("/");
    builder.AppendInt32(mDetailedState);
    builder.AppendCStr(", reason: ");
    builder.AppendString(mReason == NULL ? String("(unspecified)") : mReason);
    builder.AppendCStr(", extra: ");
    builder.AppendString(mExtraInfo == NULL ? String("(none)") : mExtraInfo);
    builder.AppendCStr(", roaming: ");
    builder.AppendBoolean(mIsRoaming);
    builder.AppendCStr(", failover: ");
    builder.AppendBoolean(mIsFailover);
    builder.AppendCStr(", isAvailable: ");
    builder.AppendBoolean(mIsAvailable);
    *result = builder.ToString();
    return NOERROR;
}

/**
 * Implement the Parcelable interface.
 * @hide
 */
ECode CNetworkInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    AutoLock lock(this);
    dest->WriteInt32(mNetworkType);
    dest->WriteInt32(mSubtype);
    dest->WriteString(mTypeName);
    dest->WriteString(mSubtypeName);
    dest->WriteInt32(mState);//mState.name()
    dest->WriteInt32(mDetailedState);//mDetailedState.name()
    dest->WriteInt32(mIsFailover ? 1 : 0);
    dest->WriteInt32(mIsAvailable ? 1 : 0);
    dest->WriteInt32(mIsRoaming ? 1 : 0);
    dest->WriteString(mReason);
    dest->WriteString(mExtraInfo);
    return NOERROR;
}

ECode CNetworkInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);
    source->ReadInt32(&mNetworkType);
    source->ReadInt32(&mSubtype);
    source->ReadString(&mTypeName);
    source->ReadString(&mSubtypeName);
    source->ReadInt32(&mState);
    source->ReadInt32(&mDetailedState);
    Int32 num;
    source->ReadInt32(&num);
    mIsFailover = (num != 0);
    source->ReadInt32(&num);
    mIsAvailable = (num != 0);
    source->ReadInt32(&num);
    mIsRoaming = (num != 0);
    source->ReadString(&mReason);
    source->ReadString(&mExtraInfo);
    return NOERROR;
}


} // namespace Net
} // namepsace Droid
} // namespace Elastos
#endif