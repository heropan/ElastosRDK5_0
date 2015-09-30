
#ifndef __ELASTOS_DROID_TEXT_METHOD_CLINKMOVEMENTMETHOD_H__
#define __ELASTOS_DROID_TEXT_METHOD_CLINKMOVEMENTMETHOD_H__

#include "_Elastos_Droid_Text_Method_CLinkMovementMethod.h"
#include "elastos/droid/text/method/LinkMovementMethod.h"


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

#endif // __ELASTOS_DROID_TEXT_METHOD_CLINKMOVEMENTMETHOD_H__
