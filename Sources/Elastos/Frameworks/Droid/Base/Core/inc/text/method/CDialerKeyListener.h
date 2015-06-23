
#ifndef __CDIALERKEYLISTENER_H__
#define __CDIALERKEYLISTENER_H__

#include "_CDialerKeyListener.h"
#include "text/method/DialerKeyListener.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CDialerKeyListener)
    , public DialerKeyListener
{
public:
    IBaseKeyListener_METHODS_DECL()
    IKeyListener_METHODS_DECL()
    IInputFilter_METHODS_DECL()

    static CARAPI_(AutoPtr<IDialerKeyListener>) GetInstance();

private:
    static AutoPtr<IDialerKeyListener> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CDIALERKEYLISTENER_H__
