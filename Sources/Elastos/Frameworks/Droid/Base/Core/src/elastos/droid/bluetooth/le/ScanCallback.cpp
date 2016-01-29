#include "elastos/droid/bluetooth/le/ScanCallback.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                             ScanCallback
//=====================================================================
CAR_INTERFACE_IMPL(ScanCallback, Object, IScanCallback);

ScanCallback::ScanCallback()
{
}

ECode ScanCallback::OnScanResult(
    /* [in] */ Int32 callbackType,
    /* [in] */ IScanResult* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    return NOERROR;
}

ECode ScanCallback::OnBatchScanResults(
    /* [in] */ IList* results)
{
    VALIDATE_NOT_NULL(results);
    assert(0);
    return NOERROR;
}

ECode ScanCallback::OnScanFailed(
    /* [in] */ Int32 errorCode)
{
    assert(0);
    return NOERROR;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
