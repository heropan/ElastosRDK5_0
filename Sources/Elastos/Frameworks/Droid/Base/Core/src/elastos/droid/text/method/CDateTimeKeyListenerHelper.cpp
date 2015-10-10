
#include "elastos/droid/text/method/CDateTimeKeyListenerHelper.h"
#include "elastos/droid/text/method/CDateTimeKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CDateTimeKeyListenerHelper, CDateTimeKeyListener, CDateTimeKeyListener)

ECode CDateTimeKeyListenerHelper::GetCHARACTERS(
    /* [out, callee] */ ArrayOf<Char32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Char32> > o = CDateTimeKeyListener::GetCHARACTERS();
    *ret = o;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CDateTimeKeyListenerHelper::GetInstance(
    /* [out] */ IDateTimeKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDateTimeKeyListener> dtkl = CDateTimeKeyListener::GetInstance();
    *ret = dtkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

