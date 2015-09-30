
#ifndef __ELASTOS_DROID_TEXT_METHOD_CDIALERKEYLISTENER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CDIALERKEYLISTENER_H__

#include "_Elastos_Droid_Text_Method_CDialerKeyListener.h"
#include "elastos/droid/text/method/DialerKeyListener.h"




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

#endif // __ELASTOS_DROID_TEXT_METHOD_CDIALERKEYLISTENER_H__
