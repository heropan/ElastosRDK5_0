
#include "text/method/CQwertyKeyListenerHelper.h"
#include "text/method/CQwertyKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

IBaseKeyListenerHelper_METHODS_IMPL(CQwertyKeyListenerHelper, CQwertyKeyListener, CQwertyKeyListener)

ECode CQwertyKeyListenerHelper::GetInstance(
    /* [in] */ Boolean autoText,
    /* [in] */ Capitalize cap,
    /* [out] */ IQwertyKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IQwertyKeyListener> qkl = CQwertyKeyListener::GetInstance(autoText, cap);
    *ret = qkl;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CQwertyKeyListenerHelper::GetInstanceForFullKeyboard(
    /* [out] */ IQwertyKeyListener** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IQwertyKeyListener> qkl = CQwertyKeyListener::GetInstanceForFullKeyboard();
    *ret = qkl;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CQwertyKeyListenerHelper::MarkAsReplaced(
    /* [in] */ ISpannable* content,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& original)
{
    CQwertyKeyListener::MarkAsReplaced(content, start, end, original);
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
