
#ifndef __CLINKMOVEMENTMETHOD_H__
#define __CLINKMOVEMENTMETHOD_H__

#include "_CLinkMovementMethod.h"
#include "text/method/LinkMovementMethod.h"
#include "text/method/BaseMovementMethodMacro.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CLinkMovementMethod)
    , public LinkMovementMethod
{
public:
    IBASEMOVEMENTMETHOD_METHODS_DECL();

    static CARAPI_(AutoPtr<IMovementMethod>) GetInstance();

private:
    static AutoPtr<ILinkMovementMethod> sInstance;
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CLINKMOVEMENTMETHOD_H__
