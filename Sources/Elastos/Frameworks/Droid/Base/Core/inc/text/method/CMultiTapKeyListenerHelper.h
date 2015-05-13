
#ifndef __CMULTITAPKEYLISTENERHELPER_H__
#define __CMULTITAPKEYLISTENERHELPER_H__

#include "_CMultiTapKeyListenerHelper.h"
#include "text/method/BaseKeyListenerMacro.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CMultiTapKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap,
        /* [out] */ IMultiTapKeyListener** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CMULTITAPKEYLISTENERHELPER_H__
