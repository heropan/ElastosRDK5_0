
#include "elastos/droid/net/LinkQualityInfo.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL_2(LinkQualityInfo, Object, ILinkQualityInfo, IParcelable)

const Int32 LinkQualityInfo::sOBJECT_TYPE_LINK_QUALITY_INFO = 1;
const Int32 LinkQualityInfo::sOBJECT_TYPE_WIFI_LINK_QUALITY_INFO = 2;
const Int32 LinkQualityInfo::sOBJECT_TYPE_MOBILE_LINK_QUALITY_INFO = 3;

LinkQualityInfo::LinkQualityInfo()
    : UNKNOWN_INT(Elastos::Core::Math::INT32_MAX_VALUE)
    , UNKNOWN_LONG(Elastos::Core::Math::INT64_MAX_VALUE)
    , NORMALIZED_MIN_SIGNAL_STRENGTH(0)
    , NORMALIZED_MAX_SIGNAL_STRENGTH(99)
    , NORMALIZED_SIGNAL_STRENGTH_RANGE(NORMALIZED_MAX_SIGNAL_STRENGTH - NORMALIZED_MIN_SIGNAL_STRENGTH + 1)
    , mNetworkType(IConnectivityManager::TYPE_NONE)
    , mNormalizedSignalStrength(UNKNOWN_INT)
    , mPacketCount(UNKNOWN_LONG)
    , mPacketErrorCount(UNKNOWN_LONG)
    , mTheoreticalTxBandwidth(UNKNOWN_INT)
    , mTheoreticalRxBandwidth(UNKNOWN_INT)
    , mTheoreticalLatency(UNKNOWN_INT)
    , mLastDataSampleTime(UNKNOWN_LONG)
    , mDataSampleDuration(UNKNOWN_INT)
{}

ECode LinkQualityInfo::constructor()
{
    return NOERROR;
}

ECode LinkQualityInfo::InitializeFromParcel(
    /* [in] */ IParcel* in)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mNetworkType = in.readInt();
        mNormalizedSignalStrength = in.readInt();
        mPacketCount = in.readLong();
        mPacketErrorCount = in.readLong();
        mTheoreticalTxBandwidth = in.readInt();
        mTheoreticalRxBandwidth = in.readInt();
        mTheoreticalLatency = in.readInt();
        mLastDataSampleTime = in.readLong();
        mDataSampleDuration = in.readInt();

#endif
}

ECode LinkQualityInfo::GetNetworkType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mNetworkType;
    return NOERROR;
}

ECode LinkQualityInfo::SetNetworkType(
    /* [in] */ Int32 networkType)
{
    mNetworkType = networkType;
    return NOERROR;
}

ECode LinkQualityInfo::GetNormalizedSignalStrength(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mNormalizedSignalStrength;
    return NOERROR;
}

ECode LinkQualityInfo::SetNormalizedSignalStrength(
    /* [in] */ Int32 normalizedSignalStrength)
{
    mNormalizedSignalStrength = normalizedSignalStrength;
    return NOERROR;
}

ECode LinkQualityInfo::GetPacketCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mPacketCount;
    return NOERROR;
}

ECode LinkQualityInfo::SetPacketCount(
    /* [in] */ Int64 packetCount)
{
    mPacketCount = packetCount;
    return NOERROR;
}

ECode LinkQualityInfo::GetPacketErrorCount(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mPacketErrorCount;
    return NOERROR;
}

ECode LinkQualityInfo::SetPacketErrorCount(
    /* [in] */ Int64 packetErrorCount)
{
    mPacketErrorCount = packetErrorCount;
    return NOERROR;
}

ECode LinkQualityInfo::GetTheoreticalTxBandwidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTheoreticalTxBandwidth;
    return NOERROR;
}

ECode LinkQualityInfo::SetTheoreticalTxBandwidth(
    /* [in] */ Int32 theoreticalTxBandwidth)
{
    mTheoreticalTxBandwidth = theoreticalTxBandwidth;
    return NOERROR;
}

ECode LinkQualityInfo::GetTheoreticalRxBandwidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTheoreticalRxBandwidth;
    return NOERROR;
}

ECode LinkQualityInfo::SetTheoreticalRxBandwidth(
    /* [in] */ Int32 theoreticalRxBandwidth)
{
    mTheoreticalRxBandwidth = theoreticalRxBandwidth;
    return NOERROR;
}

ECode LinkQualityInfo::GetTheoreticalLatency(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mTheoreticalLatency;
    return NOERROR;
}

ECode LinkQualityInfo::SetTheoreticalLatency(
    /* [in] */ Int32 theoreticalLatency)
{
    mTheoreticalLatency = theoreticalLatency;
    return NOERROR;
}

ECode LinkQualityInfo::GetLastDataSampleTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mLastDataSampleTime;
    return NOERROR;
}

ECode LinkQualityInfo::SetLastDataSampleTime(
    /* [in] */ Int64 lastDataSampleTime)
{
    mLastDataSampleTime = lastDataSampleTime;
    return NOERROR;
}

ECode LinkQualityInfo::GetDataSampleDuration(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mDataSampleDuration;
    return NOERROR;
}

ECode LinkQualityInfo::SetDataSampleDuration(
    /* [in] */ Int32 dataSampleDuration)
{
    mDataSampleDuration = dataSampleDuration;
    return NOERROR;
}

ECode LinkQualityInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                public LinkQualityInfo createFromParcel(Parcel in) {
                    int objectType = in.readInt();
                    if (objectType == OBJECT_TYPE_LINK_QUALITY_INFO) {
                        LinkQualityInfo li = new LinkQualityInfo();
                        li.initializeFromParcel(in);
                        return li;
                    } else if (objectType == OBJECT_TYPE_WIFI_LINK_QUALITY_INFO) {
                        return WifiLinkQualityInfo.createFromParcelBody(in);
                    } else if (objectType == OBJECT_TYPE_MOBILE_LINK_QUALITY_INFO) {
                        return MobileLinkQualityInfo.createFromParcelBody(in);
                    } else {
                        return null;
                    }
                }
                public LinkQualityInfo[] newArray(int size) {
                    return new LinkQualityInfo[size];
                }

#endif
}

ECode LinkQualityInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                public LinkQualityInfo createFromParcel(Parcel in) {
                    int objectType = in.readInt();
                    if (objectType == OBJECT_TYPE_LINK_QUALITY_INFO) {
                        LinkQualityInfo li = new LinkQualityInfo();
                        li.initializeFromParcel(in);
                        return li;
                    } else if (objectType == OBJECT_TYPE_WIFI_LINK_QUALITY_INFO) {
                        return WifiLinkQualityInfo.createFromParcelBody(in);
                    } else if (objectType == OBJECT_TYPE_MOBILE_LINK_QUALITY_INFO) {
                        return MobileLinkQualityInfo.createFromParcelBody(in);
                    } else {
                        return null;
                    }
                }
                public LinkQualityInfo[] newArray(int size) {
                    return new LinkQualityInfo[size];
                }

#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
