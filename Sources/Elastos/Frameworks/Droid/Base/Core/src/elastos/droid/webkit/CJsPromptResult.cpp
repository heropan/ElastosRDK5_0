
#include "elastos/droid/webkit/CJsPromptResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_OBJECT_IMPL(CJsPromptResult);

/* Notify the caller that the JsResult has completed */
void CJsPromptResult::WakeUp()
{
    mReceiver->OnJsResultComplete((IJsResult*)this->Probe(EIID_IJsResult));
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
