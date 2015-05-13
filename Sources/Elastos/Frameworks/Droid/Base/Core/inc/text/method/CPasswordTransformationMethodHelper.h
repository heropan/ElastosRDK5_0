
#ifndef __CPASSWORDTRANSFORMATIONMETHODHELPER_H__
#define __CPASSWORDTRANSFORMATIONMETHODHELPER_H__

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

#endif // __CPASSWORDTRANSFORMATIONMETHODHELPER_H__
