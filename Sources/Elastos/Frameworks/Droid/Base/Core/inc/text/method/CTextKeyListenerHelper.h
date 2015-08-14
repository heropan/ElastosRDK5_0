
#ifndef __ELASTOS_DROID_TEXT_METHOD_CTEXTKEYLISTENERHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CTEXTKEYLISTENERHELPER_H__

#include "_CTextKeyListenerHelper.h"

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CTextKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI Clear(
        /* [in] */ IEditable* e);

    CARAPI GetInstance(
        /* [out] */ ITextKeyListener** ret);

    CARAPI GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap,
        /* [out] */ ITextKeyListener** ret);

    CARAPI ShouldCap(
        /* [in] */ Capitalize cap,
        /* [in] */ ICharSequence* cs,
        /* [in] */ Int32 off,
        /* [out] */ Boolean* ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CTEXTKEYLISTENERHELPER_H__
