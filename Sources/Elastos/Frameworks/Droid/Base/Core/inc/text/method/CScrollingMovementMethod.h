
#ifndef __CSCROLLINGMOVEMENTMETHOD_H__
#define __CSCROLLINGMOVEMENTMETHOD_H__

#include "_CScrollingMovementMethod.h"
#include "text/method/ScrollingMovementMethod.h"


namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CScrollingMovementMethod)
    , public ScrollingMovementMethod
{
public:
    IBASEMOVEMENTMETHOD_METHODS_DECL();

    static CARAPI_(AutoPtr<IMovementMethod>) GetInstance();

private:
    static AutoPtr<IScrollingMovementMethod> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CSCROLLINGMOVEMENTMETHOD_H__
