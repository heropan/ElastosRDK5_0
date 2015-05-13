
#include "text/method/CDateKeyListenerHelper.h"
#include "text/method/CDateKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CDateKeyListenerHelper, CDateKeyListener, CDateKeyListener)

ECode CDateKeyListenerHelper::GetCHARACTERS(
    /* [out, callee] */ ArrayOf<Char32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Char32> > o = CDateKeyListener::GetCHARACTERS();
    *ret = o;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CDateKeyListenerHelper::GetInstance(
    /* [out] */ IDateKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDateKeyListener> dkl = CDateKeyListener::GetInstance();
    *ret = dkl;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

