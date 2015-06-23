
#ifndef __CDIALERKEYLISTENERHELPER_H__
#define __CDIALERKEYLISTENERHELPER_H__

#include "_CDialerKeyListenerHelper.h"

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDialerKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI GetCHARACTERS(
        /* [out, callee] */ ArrayOf<Char32>** ret);

    CARAPI GetInstance(
        /* [out] */ IDialerKeyListener** ret);

};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDIALERKEYLISTENERHELPER_H__
