
#ifndef __ELASTOS_DROID_TEXT_METHOD_CMULTITAPKEYLISTENERHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CMULTITAPKEYLISTENERHELPER_H__

#include "_Elastos_Droid_Text_Method_CMultiTapKeyListenerHelper.h"
#include "elastos/droid/text/method/MultiTapKeyListener.h"
#include <elastos/core/Singleton.h>

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
    , public IMultiTapKeyListenerHelper
    , public Singleton
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI GetInstance(
        /* [in] */ Boolean autotext,
        /* [in] */ Capitalize cap,
        /* [out] */ IMultiTapKeyListener** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CMULTITAPKEYLISTENERHELPER_H__
