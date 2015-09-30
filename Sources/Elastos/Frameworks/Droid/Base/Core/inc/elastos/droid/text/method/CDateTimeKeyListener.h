
#ifndef __ELASTOS_DROID_TEXT_METHOD_CDATETIMEKEYLISTENER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CDATETIMEKEYLISTENER_H__

#include "_Elastos_Droid_Text_Method_CDateTimeKeyListener.h"
#include "elastos/droid/text/method/DateTimeKeyListener.h"




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

#endif // __ELASTOS_DROID_TEXT_METHOD_CDATETIMEKEYLISTENER_H__
