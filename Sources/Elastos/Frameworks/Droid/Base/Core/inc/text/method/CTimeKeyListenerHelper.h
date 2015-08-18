
#ifndef __ELASTOS_DROID_TEXT_METHOD_CTIMEKEYLISTENERHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CTIMEKEYLISTENERHELPER_H__

#include "_Elastos_Droid_Text_Method_CTimeKeyListenerHelper.h"

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CTimeKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI GetInstance(
        /* [out] */ ITimeKeyListener** ret);

    CARAPI GetCHARACTERS(
        /* [out, callee] */ ArrayOf<Char32>** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CTIMEKEYLISTENERHELPER_H__
