
#include "elastos/droid/wifi/CWifiChannel.h"

namespace Elastos {
namespace Droid {
namespace Wifi {

CAR_INTERFACE_IMPL_2(CWifiChannel, Object, IParcelable, IWifiChannel)

CAR_OBJECT_IMPL(CWifiChannel)

ECode CWifiChannel::constructor()
{
    mFreqMHz = 0;
    mChannelNum = 0;
    mIsDFS = FALSE;
    return NOERROR;
}

ECode CWifiChannel::GetFreqMHz(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mFreqMHz;
    return NOERROR;
}

ECode CWifiChannel::SetFreqMHz(
    /* [in] */ Int32 freqMHz)
{
    mFreqMHz = freqMHz;
    return NOERROR;
}

ECode CWifiChannel::GetChannelNum(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mChannelNum;
    return NOERROR;
}

ECode CWifiChannel::SetChannelNum(
    /* [in] */ Int32 channelNum)
{
    mChannelNum = channelNum;
    return NOERROR;
}

ECode CWifiChannel::GetIsDFS(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIsDFS;
    return NOERROR;
}

ECode CWifiChannel::SetIsDFS(
    /* [in] */ Boolean isDFS)
{
    mIsDFS = isDFS;
    return NOERROR;
}

/** check for validity */
ECode CWifiChannel::IsValid(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mFreqMHz < MIN_FREQ_MHZ || mFreqMHz > MAX_FREQ_MHZ) {
        *result = FALSE;
        return NOERROR;
    }

    if (mChannelNum < MIN_CHANNEL_NUM || mChannelNum > MAX_CHANNEL_NUM) {
        *result = FALSE;
        return NOERROR;
    }

    *result = TRUE;

    return NOERROR;
}

/** implement Parcelable interface */
//@Override
ECode CWifiChannel::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;
    return NOERROR;
}

/** implement Parcelable interface */
//@Override
ECode CWifiChannel::WriteToParcel(
    /* [in] */ IParcel* out,
    /* [in] */ Int32 flags)
{
    out->WriteInt32(mFreqMHz);
    out->WriteInt32(mChannelNum);
    return out->WriteInt32(mIsDFS ? 1 : 0);
}

ECode CWifiChannel::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

ECode CWifiChannel::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(0);
    // TODO
    return E_NOT_IMPLEMENTED;
}

} // namespace Wifi
} // namespace Droid
} // namespace Elastos
