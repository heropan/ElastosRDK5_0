
#include "elastos/droid/bluetooth/le/ScanSettings.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                        ScanSettings::Builder
//=====================================================================
CAR_INTERFACE_IMPL(ScanSettings::Builder, Object, IScanSettingsBuilder);

ScanSettings::Builder::Builder()
{
}

ECode ScanSettings::Builder::SetScanMode(
    /* [in] */ Int32 scanMode)
{
    // ==================before translated======================
    // if (scanMode < SCAN_MODE_LOW_POWER || scanMode > SCAN_MODE_LOW_LATENCY) {
    //     throw new IllegalArgumentException("invalid scan mode " + scanMode);
    // }
    // mScanMode = scanMode;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::Builder::SetCallbackType(
    /* [in] */ Int32 callbackType)
{
    // ==================before translated======================
    //
    // if (!isValidCallbackType(callbackType)) {
    //     throw new IllegalArgumentException("invalid callback type - " + callbackType);
    // }
    // mCallbackType = callbackType;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::Builder::SetScanResultType(
    /* [in] */ Int32 scanResultType)
{
    // ==================before translated======================
    // if (scanResultType < SCAN_RESULT_TYPE_FULL
    //         || scanResultType > SCAN_RESULT_TYPE_ABBREVIATED) {
    //     throw new IllegalArgumentException(
    //             "invalid scanResultType - " + scanResultType);
    // }
    // mScanResultType = scanResultType;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::Builder::SetReportDelay(
    /* [in] */ Int64 reportDelayMillis)
{
    // ==================before translated======================
    // if (reportDelayMillis < 0) {
    //     throw new IllegalArgumentException("reportDelay must be > 0");
    // }
    // mReportDelayMillis = reportDelayMillis;
    // return this;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::Builder::Build(
    /* [out] */ IScanSettings** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new ScanSettings(mScanMode, mCallbackType, mScanResultType,
    //         mReportDelayMillis);
    assert(0);
    return NOERROR;
}

Boolean ScanSettings::Builder::IsValidCallbackType(
    /* [in] */ Int32 callbackType)
{
    // ==================before translated======================
    // if (callbackType == CALLBACK_TYPE_ALL_MATCHES ||
    //         callbackType == CALLBACK_TYPE_FIRST_MATCH ||
    //         callbackType == CALLBACK_TYPE_MATCH_LOST) {
    //     return true;
    // }
    // return callbackType == (CALLBACK_TYPE_FIRST_MATCH | CALLBACK_TYPE_MATCH_LOST);
    assert(0);
    return FALSE;
}

//=====================================================================
//                             ScanSettings
//=====================================================================
CAR_INTERFACE_IMPL_2(ScanSettings, Object, IScanSettings, IParcelable);
ScanSettings::ScanSettings()
{
}

ECode ScanSettings::GetScanMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mScanMode;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::GetCallbackType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mCallbackType;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::GetScanResultType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mScanResultType;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::GetReportDelayMillis(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mReportDelayMillis;
    assert(0);
    return NOERROR;
}

ECode ScanSettings::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(dest);
    // ==================before translated======================
    // dest.writeInt(mScanMode);
    // dest.writeInt(mCallbackType);
    // dest.writeInt(mScanResultType);
    // dest.writeLong(mReportDelayMillis);
    assert(0);
    return NOERROR;
}

// @Override
CARAPI ScanSettings::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ScanSettings::ScanSettings(
    /* [in] */ Int32 scanMode,
    /* [in] */ Int32 callbackType,
    /* [in] */ Int32 scanResultType,
    /* [in] */ Int64 reportDelayMillis)
{
    // ==================before translated======================
    // mScanMode = scanMode;
    // mCallbackType = callbackType;
    // mScanResultType = scanResultType;
    // mReportDelayMillis = reportDelayMillis;
}

ScanSettings::ScanSettings(
    /* [in] */ IParcel* in)
{
    // ==================before translated======================
    // mScanMode = in.readInt();
    // mCallbackType = in.readInt();
    // mScanResultType = in.readInt();
    // mReportDelayMillis = in.readLong();
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
