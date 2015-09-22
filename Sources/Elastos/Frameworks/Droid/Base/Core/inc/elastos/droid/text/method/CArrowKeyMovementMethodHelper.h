
#ifndef __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHODHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHODHELPER_H__

#include "_Elastos_Droid_Text_Method_CArrowKeyMovementMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CArrowKeyMovementMethodHelper), public IArrowKeyMovementMethodHelper

{
public:
    CAR_SINGLETON_DECL()

    CARAPI GetInstance(
        /* [out] */ IMovementMethod** ret);

private:
    static AutoPtr<IArrowKeyMovementMethod> sInstance;

};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHODHELPER_H__
