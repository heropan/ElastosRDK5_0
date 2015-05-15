
#ifndef __CCHOICEFORMATHELPER_H__
#define __CCHOICEFORMATHELPER_H__

#include "_CChoiceFormatHelper.h"

namespace Elastos {
namespace Text {

CarClass(CChoiceFormatHelper)
{
public:
    CARAPI NextDouble(
        /* [in] */ Double value,
        /* [out] */ Double* nextValue);

    CARAPI NextDoubleEx(
        /* [in] */ Double value,
        /* [in] */ Boolean increment,
        /* [out] */ Double* nextValue);

    CARAPI PreviousDouble(
        /* [in] */ Double value,
        /* [out] */ Double* previousValue);

};

} // namespace Text
} // namespace Elastos

#endif // __CCHOICEFORMATHELPER_H__
