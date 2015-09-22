
#include "elastos/droid/net/MobileLinkQualityInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(MobileLinkQualityInfo, LinkQualityInfo, IMobileLinkQualityInfo)

MobileLinkQualityInfo::MobileLinkQualityInfo()
    : mMobileNetworkType(ILinkQualityInfo::UNKNOWN_INT)
    , mRssi(ILinkQualityInfo::UNKNOWN_INT)
    , mGsmErrorRate(ILinkQualityInfo::UNKNOWN_INT)
    , mCdmaDbm(ILinkQualityInfo::UNKNOWN_INT)
    , mCdmaEcio(ILinkQualityInfo::UNKNOWN_INT)
    , mEvdoDbm(ILinkQualityInfo::UNKNOWN_INT)
    , mEvdoEcio(ILinkQualityInfo::UNKNOWN_INT)
    , mEvdoSnr(ILinkQualityInfo::UNKNOWN_INT)
    , mLteSignalStrength(ILinkQualityInfo::UNKNOWN_INT)
    , mLteRsrp(ILinkQualityInfo::UNKNOWN_INT)
    , mLteRsrq(ILinkQualityInfo::UNKNOWN_INT)
    , mLteRssnr(ILinkQualityInfo::UNKNOWN_INT)
    , mLteCqi(ILinkQualityInfo::UNKNOWN_INT)
{}

ECode MobileLinkQualityInfo::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super.writeToParcel(dest, flags, OBJECT_TYPE_MOBILE_LINK_QUALITY_INFO);
        dest.writeInt(mMobileNetworkType);
        dest.writeInt(mRssi);
        dest.writeInt(mGsmErrorRate);
        dest.writeInt(mCdmaDbm);
        dest.writeInt(mCdmaEcio);
        dest.writeInt(mEvdoDbm);
        dest.writeInt(mEvdoEcio);
        dest.writeInt(mEvdoSnr);
        dest.writeInt(mLteSignalStrength);
        dest.writeInt(mLteRsrp);
        dest.writeInt(mLteRsrq);
        dest.writeInt(mLteRssnr);
        dest.writeInt(mLteCqi);

#endif
}

ECode MobileLinkQualityInfo::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ IMobileLinkQualityInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        MobileLinkQualityInfo li = new MobileLinkQualityInfo();
        li.initializeFromParcel(in);
        li.mMobileNetworkType = in.readInt();
        li.mRssi = in.readInt();
        li.mGsmErrorRate = in.readInt();
        li.mCdmaDbm = in.readInt();
        li.mCdmaEcio = in.readInt();
        li.mEvdoDbm = in.readInt();
        li.mEvdoEcio = in.readInt();
        li.mEvdoSnr = in.readInt();
        li.mLteSignalStrength = in.readInt();
        li.mLteRsrp = in.readInt();
        li.mLteRsrq = in.readInt();
        li.mLteRssnr = in.readInt();
        li.mLteCqi = in.readInt();
        return li;

#endif
}

ECode MobileLinkQualityInfo::GetMobileNetworkType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mMobileNetworkType;

#endif
}

ECode MobileLinkQualityInfo::SetMobileNetworkType(
    /* [in] */ Int32 mobileNetworkType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mMobileNetworkType = mobileNetworkType;

#endif
}

ECode MobileLinkQualityInfo::GetRssi(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mRssi;

#endif
}

ECode MobileLinkQualityInfo::SetRssi(
    /* [in] */ Int32 Rssi)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mRssi = Rssi;

#endif
}

ECode MobileLinkQualityInfo::GetGsmErrorRate(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mGsmErrorRate;

#endif
}

ECode MobileLinkQualityInfo::SetGsmErrorRate(
    /* [in] */ Int32 gsmErrorRate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mGsmErrorRate = gsmErrorRate;

#endif
}

ECode MobileLinkQualityInfo::GetCdmaDbm(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mCdmaDbm;

#endif
}

ECode MobileLinkQualityInfo::SetCdmaDbm(
    /* [in] */ Int32 cdmaDbm)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCdmaDbm = cdmaDbm;

#endif
}

ECode MobileLinkQualityInfo::GetCdmaEcio(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mCdmaEcio;

#endif
}

ECode MobileLinkQualityInfo::SetCdmaEcio(
    /* [in] */ Int32 cdmaEcio)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCdmaEcio = cdmaEcio;

#endif
}

ECode MobileLinkQualityInfo::GetEvdoDbm(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEvdoDbm;

#endif
}

ECode MobileLinkQualityInfo::SetEvdoDbm(
    /* [in] */ Int32 evdoDbm)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mEvdoDbm = evdoDbm;

#endif
}

ECode MobileLinkQualityInfo::GetEvdoEcio(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEvdoEcio;

#endif
}

ECode MobileLinkQualityInfo::SetEvdoEcio(
    /* [in] */ Int32 evdoEcio)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mEvdoEcio = evdoEcio;

#endif
}

ECode MobileLinkQualityInfo::GetEvdoSnr(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mEvdoSnr;

#endif
}

ECode MobileLinkQualityInfo::SetEvdoSnr(
    /* [in] */ Int32 evdoSnr)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mEvdoSnr = evdoSnr;

#endif
}

ECode MobileLinkQualityInfo::GetLteSignalStrength(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLteSignalStrength;

#endif
}

ECode MobileLinkQualityInfo::SetLteSignalStrength(
    /* [in] */ Int32 lteSignalStrength)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLteSignalStrength = lteSignalStrength;

#endif
}

ECode MobileLinkQualityInfo::GetLteRsrp(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLteRsrp;

#endif
}

ECode MobileLinkQualityInfo::SetLteRsrp(
    /* [in] */ Int32 lteRsrp)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLteRsrp = lteRsrp;

#endif
}

ECode MobileLinkQualityInfo::GetLteRsrq(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLteRsrq;

#endif
}

ECode MobileLinkQualityInfo::SetLteRsrq(
    /* [in] */ Int32 lteRsrq)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLteRsrq = lteRsrq;

#endif
}

ECode MobileLinkQualityInfo::GetLteRssnr(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLteRssnr;

#endif
}

ECode MobileLinkQualityInfo::SetLteRssnr(
    /* [in] */ Int32 lteRssnr)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLteRssnr = lteRssnr;

#endif
}

ECode MobileLinkQualityInfo::GetLteCqi(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLteCqi;

#endif
}

ECode MobileLinkQualityInfo::SetLteCqi(
    /* [in] */ Int32 lteCqi)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mLteCqi = lteCqi;

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
