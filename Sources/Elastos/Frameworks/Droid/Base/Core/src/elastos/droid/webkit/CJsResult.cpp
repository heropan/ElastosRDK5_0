
#include "elastos/droid/webkit/CJsResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CJsResult::constructor(
    /* [in] */ IJsResultReceiver* receiver)
{
    return JsResult::Init(receiver);
}

ECode CJsResult::Cancel()
{
    return JsResult::Cancel();
}

ECode CJsResult::Confirm()
{
    return JsResult::Confirm();
}

ECode CJsResult::GetResult(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = JsResult::GetResult();
    return NOERROR;
}
/* Notify the caller that the JsResult has completed */
void CJsResult::WakeUp()
{
    mReceiver->OnJsResultComplete((IJsResult*)this->Probe(EIID_IJsResult));
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
