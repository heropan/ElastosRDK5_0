
#include "text/method/CTimeKeyListenerHelper.h"
#include "text/method/CTimeKeyListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CTimeKeyListenerHelper, CTimeKeyListener, CTimeKeyListener)

ECode CTimeKeyListenerHelper::GetInstance(
    /* [out] */ ITimeKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ITimeKeyListener> tkl = CTimeKeyListener::GetInstance();
    *ret = tkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTimeKeyListenerHelper::GetCHARACTERS(
    /* [out, callee] */ ArrayOf<Char32>** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr< ArrayOf<Char32> > o = CTimeKeyListener::GetCHARACTERS();
    *ret = o;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

