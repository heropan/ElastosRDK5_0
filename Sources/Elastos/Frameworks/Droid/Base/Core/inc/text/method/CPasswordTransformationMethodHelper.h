
#ifndef __ELASTOS_DROID_TEXT_METHOD_CPASSWORDTRANSFORMATIONMETHODHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CPASSWORDTRANSFORMATIONMETHODHELPER_H__

#include "_CPasswordTransformationMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CPasswordTransformationMethodHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IPasswordTransformationMethod** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CPASSWORDTRANSFORMATIONMETHODHELPER_H__
