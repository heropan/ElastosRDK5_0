
#ifndef __CDATEKEYLISTENER_H__
#define __CDATEKEYLISTENER_H__

#include "_CDateKeyListener.h"
#include "text/method/DateKeyListener.h"
#include "text/method/BaseKeyListenerMacro.h"
#include "text/method/KeyListenerMacro.h"
#include "text/InputFilterMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDateKeyListener)
    , public DateKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()
    IInputFilter_METHODS_DECL()

    static CARAPI_(AutoPtr<IDateKeyListener>) GetInstance();

private:
    static AutoPtr<IDateKeyListener> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDATEKEYLISTENER_H__
