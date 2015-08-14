
#ifndef __ELASTOS_DROID_TEXT_METHOD_CLINKMOVEMENTMETHODHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CLINKMOVEMENTMETHODHELPER_H__

#include "_CLinkMovementMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CLinkMovementMethodHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IMovementMethod** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CLINKMOVEMENTMETHODHELPER_H__
