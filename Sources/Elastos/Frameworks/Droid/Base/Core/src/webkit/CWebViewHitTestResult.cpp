
#include "webkit/CWebViewHitTestResult.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CWebViewHitTestResult::CWebViewHitTestResult()
    : mType(UNKNOWN_TYPE)
{}

ECode CWebViewHitTestResult::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CWebViewHitTestResult::SetExtra(
    /* [in] */ const String& extra)
{
    mExtra = extra;
    return NOERROR;
}

ECode CWebViewHitTestResult::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CWebViewHitTestResult::GetExtra(
    /* [out] */ String* extra)
{
    VALIDATE_NOT_NULL(extra);
    *extra = mExtra;
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos