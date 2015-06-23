
#ifndef __CAPPLICATIONERRORREPORTHELPER_H__
#define __CAPPLICATIONERRORREPORTHELPER_H__

#include "_CApplicationErrorReportHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IPackageManager;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CApplicationErrorReportHelper)
{
public:
    CARAPI constructor();

    CARAPI GetErrorReportReceiver(
        /* [in] */ IContext* context,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 appFlags,
        /* [out] */ IComponentName** receiver);

    CARAPI GetErrorReportReceiver(
        /* [in] */ IPackageManager* pm,
        /* [in] */ const String& errorPackage,
        /* [in] */ const String& receiverPackage,
        /* [out] */ IComponentName** receiver);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CAPPLICATIONERRORREPORTHELPER_H__
