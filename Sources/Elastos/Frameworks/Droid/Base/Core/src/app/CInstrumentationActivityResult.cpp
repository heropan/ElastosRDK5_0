#include "ext/frameworkext.h"
#include "app/CInstrumentationActivityResult.h"

namespace Elastos {
namespace Droid {
namespace App {

CInstrumentationActivityResult::CInstrumentationActivityResult()
    : mResultCode(0)
{
}

ECode CInstrumentationActivityResult::constructor(
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* resultData)
{
    mResultCode = resultCode;
    mResultData = resultData;
    return NOERROR;
}

ECode CInstrumentationActivityResult::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    VALIDATE_NOT_NULL(resultCode);
    *resultCode = mResultCode;
    return NOERROR;
}

ECode CInstrumentationActivityResult::GetResultData(
    /* [out] */ IIntent** resultData)
{
    VALIDATE_NOT_NULL(resultData);
    *resultData = mResultData;
    INTERFACE_ADDREF(*resultData);
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
