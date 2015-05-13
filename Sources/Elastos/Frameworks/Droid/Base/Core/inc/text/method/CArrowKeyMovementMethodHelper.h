
#ifndef __CARROWKEYMOVEMENTMETHODHELPER_H__
#define __CARROWKEYMOVEMENTMETHODHELPER_H__

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

#endif // __CARROWKEYMOVEMENTMETHODHELPER_H__
