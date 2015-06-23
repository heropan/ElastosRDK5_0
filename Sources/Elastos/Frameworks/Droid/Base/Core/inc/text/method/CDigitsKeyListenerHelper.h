
#ifndef __CDIGITSKEYLISTENERHELPER_H__
#define __CDIGITSKEYLISTENERHELPER_H__

#include "_CDigitsKeyListenerHelper.h"

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDigitsKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI GetInstance(
        /* [out] */ IDigitsKeyListener** ret);

    CARAPI GetInstance(
        /* [in] */ Boolean sign,
        /* [in] */ Boolean decimal,
        /* [out] */ IDigitsKeyListener** ret);

    CARAPI GetInstance(
        /* [in] */ const String& accepted,
        /* [out] */ IDigitsKeyListener** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDIGITSKEYLISTENERHELPER_H__
