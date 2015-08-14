
#ifndef __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHODHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHODHELPER_H__

#include "_CArrowKeyMovementMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CArrowKeyMovementMethodHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IMovementMethod** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CARROWKEYMOVEMENTMETHODHELPER_H__
