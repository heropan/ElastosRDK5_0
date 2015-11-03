
#include "elastos/droid/net/WifiLinkQualityInfo.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(WifiLinkQualityInfo, LinkQualityInfo, IWifiLinkQualityInfo)

WifiLinkQualityInfo::WifiLinkQualityInfo()
    : mType(UNKNOWN_INT)
    , mRssi(UNKNOWN_INT)
    , mTxGood(UNKNOWN_LONG)
    , mTxBad(UNKNOWN_LONG)
{}

WifiLinkQualityInfo::constructor()
{
    return NOERROR;
}

ECode WifiLinkQualityInfo::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ IWifiLinkQualityInfo** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        WifiLinkQualityInfo li = new WifiLinkQualityInfo();
        li.initializeFromParcel(in);
        li.mType =  in.readInt();
        li.mRssi =  in.readInt();
        li.mTxGood =  in.readLong();
        li.mTxBad =  in.readLong();
        li.mBssid =  in.readString();
        return li;

#endif
}

ECode WifiLinkQualityInfo::GetType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mType;

#endif
}

ECode WifiLinkQualityInfo::SetType(
    /* [in] */ Int32 type)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mType = type;

#endif
}

ECode WifiLinkQualityInfo::GetBssid(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mBssid;

#endif
}

ECode WifiLinkQualityInfo::SetBssid(
    /* [in] */ const String& bssid)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mBssid = bssid;

#endif
}

ECode WifiLinkQualityInfo::GetRssi(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mRssi;

#endif
}

ECode WifiLinkQualityInfo::SetRssi(
    /* [in] */ Int32 rssi)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mRssi = rssi;

#endif
}

ECode WifiLinkQualityInfo::GetTxGood(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTxGood;

#endif
}

ECode WifiLinkQualityInfo::SetTxGood(
    /* [in] */ Int64 txGood)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mTxGood = txGood;

#endif
}

ECode WifiLinkQualityInfo::GetTxBad(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTxBad;

#endif
}

ECode WifiLinkQualityInfo::SetTxBad(
    /* [in] */ Int64 txBad)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mTxBad = txBad;

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
