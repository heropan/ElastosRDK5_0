
#ifndef __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHOD_H__
#define __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHOD_H__

#include "_Elastos_Droid_Text_Method_CArrowKeyMovementMethod.h"
#include "text/method/ArrowKeyMovementMethod.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CArrowKeyMovementMethod)
    , public ArrowKeyMovementMethod
{
public:
    CAR_OBJECT_DECL()

    static CARAPI_(AutoPtr<IMovementMethod>) GetInstance();

    //Test
//    CARAPI_(void) RefShow();

private:
    static AutoPtr<IArrowKeyMovementMethod> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHOD_H__
