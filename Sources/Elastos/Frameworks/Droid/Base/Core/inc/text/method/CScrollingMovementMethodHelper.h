
#ifndef __CSCROLLINGMOVEMENTMETHODHELPER_H__
#define __CSCROLLINGMOVEMENTMETHODHELPER_H__

#include "_CScrollingMovementMethodHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CScrollingMovementMethodHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IMovementMethod** ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CSCROLLINGMOVEMENTMETHODHELPER_H__
