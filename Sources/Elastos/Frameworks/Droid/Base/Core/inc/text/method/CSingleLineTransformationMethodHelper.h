
#ifndef __ELASTOS_DROID_TEXT_METHOD_CSINGLELINETRANSFORMATIONMETHODHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CSINGLELINETRANSFORMATIONMETHODHELPER_H__

#include "_CSingleLineTransformationMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CSingleLineTransformationMethodHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ ISingleLineTransformationMethod** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CSINGLELINETRANSFORMATIONMETHODHELPER_H__
