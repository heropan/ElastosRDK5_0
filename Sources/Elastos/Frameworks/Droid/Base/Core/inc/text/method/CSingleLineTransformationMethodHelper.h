
#ifndef __CSINGLELINETRANSFORMATIONMETHODHELPER_H__
#define __CSINGLELINETRANSFORMATIONMETHODHELPER_H__

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

#endif // __CSINGLELINETRANSFORMATIONMETHODHELPER_H__
