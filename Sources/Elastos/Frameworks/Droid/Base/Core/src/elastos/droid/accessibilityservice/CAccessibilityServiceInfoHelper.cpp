#include "CAccessibilityServiceInfoHelper.h"
#include "CAccessibilityServiceInfo.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

/**
 * Returns the string representation of a feedback type. For example,
 * {@link #FEEDBACK_SPOKEN} is represented by the string FEEDBACK_SPOKEN.
 *
 * @param feedbackType The feedback type.
 * @return The string representation.
 */
ECode CAccessibilityServiceInfoHelper::FeedbackTypeToString(
    /* [in] */ Int32 feedbackType,
    /* [out] */ String* str)
{
    return CAccessibilityServiceInfo::FeedbackTypeToString(feedbackType, str);
}

/**
 * Returns the string representation of a flag. For example,
 * {@link #DEFAULT} is represented by the string DEFAULT.
 *
 * @param flag The flag.
 * @return The string representation.
 */
ECode CAccessibilityServiceInfoHelper::FlagToString(
    /* [in] */ Int32 flag,
    /* [out] */ String* str)
{
    return CAccessibilityServiceInfo::FlagToString(flag, str);
}

}
}
}
