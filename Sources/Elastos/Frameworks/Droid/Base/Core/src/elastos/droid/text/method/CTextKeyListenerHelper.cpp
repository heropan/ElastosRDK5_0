
#include "elastos/droid/text/method/CTextKeyListenerHelper.h"
#include "elastos/droid/text/method/CTextKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CTextKeyListenerHelper, CTextKeyListener, CTextKeyListener)

ECode CTextKeyListenerHelper::Clear(
    /* [in] */ IEditable* e)
{
    CTextKeyListener::Clear(e);
    return NOERROR;
}

ECode CTextKeyListenerHelper::GetInstance(
    /* [out] */ ITextKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ITextKeyListener> tkl = CTextKeyListener::GetInstance();
    *ret = tkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextKeyListenerHelper::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap,
    /* [out] */ ITextKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ITextKeyListener> tkl = CTextKeyListener::GetInstance(autotext, cap);
    *ret = tkl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextKeyListenerHelper::ShouldCap(
    /* [in] */ Capitalize cap,
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 off,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=CTextKeyListener::ShouldCap(cap, cs, off);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

