
#include "text/method/CDateTimeKeyListenerHelper.h"
#include "text/method/CDateTimeKeyListener.h"
#include "ext/frameworkext.h"

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
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateTimeKeyListenerHelper::GetInstance(
    /* [out] */ IDateTimeKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDateTimeKeyListener> dtkl = CDateTimeKeyListener::GetInstance();
    *ret = dtkl;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

