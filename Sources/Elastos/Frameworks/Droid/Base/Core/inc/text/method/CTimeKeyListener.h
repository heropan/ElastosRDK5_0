
#ifndef __CTIMEKEYLISTENER_H__
#define __CTIMEKEYLISTENER_H__

#include "_CTimeKeyListener.h"
#include "text/method/TimeKeyListener.h"
#include "text/method/BaseKeyListenerMacro.h"
#include "text/method/KeyListenerMacro.h"
#include "text/InputFilterMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CTimeKeyListener)
    , public TimeKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()
    IInputFilter_METHODS_DECL()

    static CARAPI_(AutoPtr<ITimeKeyListener>) GetInstance();

private:
    static AutoPtr<ITimeKeyListener> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CTIMEKEYLISTENER_H__
