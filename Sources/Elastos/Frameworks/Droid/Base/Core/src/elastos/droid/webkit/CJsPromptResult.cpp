
#include "elastos/droid/webkit/CJsPromptResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CJsPromptResult::constructor(
    /* [in] */ IJsResultReceiver* receiver)
{
    return JsPromptResult::Init(receiver);
}

ECode CJsPromptResult::Cancel()
{
    return JsPromptResult::Cancel();
}

ECode CJsPromptResult::Confirm()
{
    return JsResult::Confirm();
}

ECode CJsPromptResult::GetResult(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = JsPromptResult::GetResult();
    return NOERROR;
}

ECode CJsPromptResult::Confirm(
    /* [in] */ const String& result)
{
    return JsPromptResult::Confirm(result);
}

ECode CJsPromptResult::GetStringResult(
    /* [out] */ String* stringResult)
{
    VALIDATE_NOT_NULL(stringResult);
    *stringResult = JsPromptResult::GetStringResult();
    return  NOERROR;
}

/* Notify the caller that the JsResult has completed */
void CJsPromptResult::WakeUp()
{
    mReceiver->OnJsResultComplete((IJsResult*)this->Probe(EIID_IJsResult));
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
