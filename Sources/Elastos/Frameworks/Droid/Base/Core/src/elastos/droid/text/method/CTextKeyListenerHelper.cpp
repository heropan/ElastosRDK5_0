
#include "elastos/droid/text/method/CTextKeyListenerHelper.h"
#include "elastos/droid/text/method/CTextKeyListener.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_INTERFACE_IMPL(CTextKeyListenerHelper, Singleton, ITextKeyListenerHelper)

CAR_SINGLETON_IMPL(CTextKeyListenerHelper)

ECode CTextKeyListenerHelper::Clear(
    /* [in] */ IEditable* e)
{
    return TextKeyListener::Clear(e);
}

ECode CTextKeyListenerHelper::GetInstance(
    /* [out] */ ITextKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    return TextKeyListener::GetInstance(ret);
}

ECode CTextKeyListenerHelper::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap,
    /* [out] */ ITextKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    return TextKeyListener::GetInstance(autotext, cap, ret);
}

ECode CTextKeyListenerHelper::ShouldCap(
    /* [in] */ Capitalize cap,
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 off,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    return TextKeyListener::ShouldCap(cap, cs, off, ret);
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

