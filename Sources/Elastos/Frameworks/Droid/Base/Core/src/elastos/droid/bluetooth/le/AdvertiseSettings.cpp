
#include "elastos/droid/bluetooth/le/AdvertiseSettings.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                      AdvertiseSettings::Builder
//=====================================================================
ECode AdvertiseSettings::Builder::SetAdvertiseMode(
    /* [in] */ Int32 advertiseMode)
{
    // ==================before translated======================
    // if (advertiseMode < ADVERTISE_MODE_LOW_POWER
    //         || advertiseMode > ADVERTISE_MODE_LOW_LATENCY) {
    //     throw new IllegalArgumentException("unknown mode " + advertiseMode);
    // }
    // mMode = advertiseMode;
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::Builder::SetTxPowerLevel(
    /* [in] */ Int32 txPowerLevel)
{
    // ==================before translated======================
    // if (txPowerLevel < ADVERTISE_TX_POWER_ULTRA_LOW
    //         || txPowerLevel > ADVERTISE_TX_POWER_HIGH) {
    //     throw new IllegalArgumentException("unknown tx power level " + txPowerLevel);
    // }
    // mTxPowerLevel = txPowerLevel;
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::Builder::SetConnectable(
    /* [in] */ Boolean connectable)
{
    // ==================before translated======================
    // mConnectable = connectable;
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::Builder::SetTimeout(
    /* [in] */ Int32 timeoutMillis)
{
    // ==================before translated======================
    // if (timeoutMillis < 0 || timeoutMillis > LIMITED_ADVERTISING_MAX_MILLIS) {
    //     throw new IllegalArgumentException("timeoutMillis invalid (must be 0-"
    //                         + LIMITED_ADVERTISING_MAX_MILLIS + " milliseconds)");
    // }
    // mTimeoutMillis = timeoutMillis;
    // return this;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::Builder::Build(
    /* [out] */ IAdvertiseSettings** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new AdvertiseSettings(mMode, mTxPowerLevel, mConnectable, mTimeoutMillis);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                          AdvertiseSettings
//=====================================================================
const Int32 AdvertiseSettings::LIMITED_ADVERTISING_MAX_MILLIS;

ECode AdvertiseSettings::GetMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAdvertiseMode;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::GetTxPowerLevel(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAdvertiseTxPowerLevel;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::IsConnectable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAdvertiseConnectable;
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::GetTimeout(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mAdvertiseTimeoutMillis;
    assert(0);
    return NOERROR;
}

CARAPI AdvertiseSettings::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    // ==================before translated======================
    // return "Settings [mAdvertiseMode=" + mAdvertiseMode
    //      + ", mAdvertiseTxPowerLevel=" + mAdvertiseTxPowerLevel
    //      + ", mAdvertiseConnectable=" + mAdvertiseConnectable
    //      + ", mAdvertiseTimeoutMillis=" + mAdvertiseTimeoutMillis + "]";
    assert(0);
    *info = String("");
    return NOERROR;
}

ECode AdvertiseSettings::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // dest.writeInt(mAdvertiseMode);
    // dest.writeInt(mAdvertiseTxPowerLevel);
    // dest.writeInt(mAdvertiseConnectable ? 1 : 0);
    // dest.writeInt(mAdvertiseTimeoutMillis);
    assert(0);
    return NOERROR;
}

ECode AdvertiseSettings::ReadFromParcel(
            /* [in] */ IParcel* source)
{
    return NOERROR;
}

AdvertiseSettings::AdvertiseSettings(
    /* [in] */ Int32 advertiseMode,
    /* [in] */ Int32 advertiseTxPowerLevel,
    /* [in] */ Boolean advertiseConnectable,
    /* [in] */ Int32 advertiseTimeout)
{
    // ==================before translated======================
    // mAdvertiseMode = advertiseMode;
    // mAdvertiseTxPowerLevel = advertiseTxPowerLevel;
    // mAdvertiseConnectable = advertiseConnectable;
    // mAdvertiseTimeoutMillis = advertiseTimeout;
}

AdvertiseSettings::AdvertiseSettings(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // mAdvertiseMode = in.readInt();
    // mAdvertiseTxPowerLevel = in.readInt();
    // mAdvertiseConnectable = in.readInt() != 0 ? true : false;
    // mAdvertiseTimeoutMillis = in.readInt();
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos


