
#ifndef __CARROWKEYMOVEMENTMETHOD_H__
#define __CARROWKEYMOVEMENTMETHOD_H__

#include "_CArrowKeyMovementMethod.h"
#include "text/method/ArrowKeyMovementMethod.h"



namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CArrowKeyMovementMethod)
    , public ArrowKeyMovementMethod
{
public:
    IBASEMOVEMENTMETHOD_METHODS_DECL();

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

#endif // __CARROWKEYMOVEMENTMETHOD_H__
