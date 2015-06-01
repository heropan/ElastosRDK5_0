#include "CChoiceFormatHelper.h"
#include "ChoiceFormat.h"

namespace Elastos {
namespace Text {

ECode CChoiceFormatHelper::NextDouble(
    /* [in] */ Double value,
    /* [out] */ Double* nextValue)
{
    VALIDATE_NOT_NULL(nextValue)
    *nextValue = ChoiceFormat::NextDouble(value);
    return NOERROR;
}

ECode CChoiceFormatHelper::NextDouble(
    /* [in] */ Double value,
    /* [in] */ Boolean increment,
    /* [out] */ Double* nextValue)
{
    VALIDATE_NOT_NULL(nextValue)
    *nextValue = ChoiceFormat::NextDouble(value,increment);
    return NOERROR;
}

ECode CChoiceFormatHelper::PreviousDouble(
    /* [in] */ Double value,
    /* [out] */ Double* previousValue)
{
    VALIDATE_NOT_NULL(previousValue)
    *previousValue = ChoiceFormat::PreviousDouble(value);
    return NOERROR;
}

} // namespace Text
} // namespace Elastos