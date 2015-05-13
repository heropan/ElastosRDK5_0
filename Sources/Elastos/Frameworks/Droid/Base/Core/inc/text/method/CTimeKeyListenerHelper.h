
#ifndef __CTIMEKEYLISTENERHELPER_H__
#define __CTIMEKEYLISTENERHELPER_H__

#include "_CTimeKeyListenerHelper.h"
#include "text/method/BaseKeyListenerMacro.h"
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

#endif // __CTIMEKEYLISTENERHELPER_H__
