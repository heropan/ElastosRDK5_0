#include "text/CAutoTextHelper.h"
#include "text/CAutoText.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CAutoTextHelper::Get(
    /* [in] */ ICharSequence* src,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IView* view,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CAutoText::Get(src, start, end, view);
    return NOERROR;
}

ECode CAutoTextHelper::GetSize(
    /* [in] */ IView* view,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CAutoText::GetSize(view);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos