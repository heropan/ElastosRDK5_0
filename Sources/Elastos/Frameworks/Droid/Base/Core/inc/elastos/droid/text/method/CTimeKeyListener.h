
#ifndef __ELASTOS_DROID_TEXT_METHOD_CTIMEKEYLISTENER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CTIMEKEYLISTENER_H__

#include "_Elastos_Droid_Text_Method_CTimeKeyListener.h"
#include "text/method/TimeKeyListener.h"




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

#endif // __ELASTOS_DROID_TEXT_METHOD_CTIMEKEYLISTENER_H__
