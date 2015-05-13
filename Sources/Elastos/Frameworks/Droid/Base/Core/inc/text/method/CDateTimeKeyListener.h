
#ifndef __CDATETIMEKEYLISTENER_H__
#define __CDATETIMEKEYLISTENER_H__

#include "_CDateTimeKeyListener.h"
#include "text/method/DateTimeKeyListener.h"
#include "text/method/BaseKeyListenerMacro.h"
#include "text/method/KeyListenerMacro.h"
#include "text/InputFilterMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDateTimeKeyListener)
    , public DateTimeKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()
    IInputFilter_METHODS_DECL()

    static CARAPI_(AutoPtr<IDateTimeKeyListener>) GetInstance();

private:
    static AutoPtr<IDateTimeKeyListener> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDATETIMEKEYLISTENER_H__
