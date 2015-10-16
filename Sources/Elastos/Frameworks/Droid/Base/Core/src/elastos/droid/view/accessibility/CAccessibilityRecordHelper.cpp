
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/accessibility/CAccessibilityRecordHelper.h"
#include "elastos/droid/view/accessibility/CAccessibilityRecord.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

ECode CAccessibilityRecordHelper::Obtain(
    /* [in] */ IAccessibilityRecord* record,
    /* [out] */ IAccessibilityRecord** newInstance)
{
    VALIDATE_NOT_NULL(newInstance);
    return CAccessibilityRecord::Obtain(record, newInstance);
}

ECode CAccessibilityRecordHelper::Obtain(
    /* [out] */ IAccessibilityRecord** newInstance)
{
    VALIDATE_NOT_NULL(newInstance);
    return CAccessibilityRecord::Obtain(newInstance);
}

} // Accessibility
} // View
} // Droid
} // Elastos
