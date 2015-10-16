#include <ext/frameworkdef.h>
#include "elastos/droid/app/CApplicationErrorReportHelper.h"
#include "elastos/droid/app/CApplicationErrorReport.h"

namespace Elastos {
namespace Droid {
namespace App {


ECode CApplicationErrorReportHelper::constructor()
{
    return NOERROR;
}

ECode CApplicationErrorReportHelper::GetErrorReportReceiver(
    /* [in] */ IContext* context,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 appFlags,
    /* [out] */ IComponentName** receiver)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(receiver);

    *receiver = CApplicationErrorReport::GetErrorReportReceiver(
            context, packageName, appFlags);
    return NOERROR;
}

ECode CApplicationErrorReportHelper::GetErrorReportReceiver(
    /* [in] */ IPackageManager* pm,
    /* [in] */ const String& errorPackage,
    /* [in] */ const String& receiverPackage,
    /* [out] */ IComponentName** receiver)
{
    VALIDATE_NOT_NULL(pm);
    VALIDATE_NOT_NULL(receiver);

    *receiver = CApplicationErrorReport::GetErrorReportReceiver(
            pm, errorPackage, receiverPackage);
    return NOERROR;
}


} // namespace App
} // namespace Droid
} // namespace Elastos
