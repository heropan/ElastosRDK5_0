
#include "elastos/droid/webkit/native/ui/base/DeviceFormFactor.h"
#include "elastos/droid/content/res/CResources.h"
#include "elastos/droid/content/res/CConfiguration.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::CResources;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::CConfiguration;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                           DeviceFormFactor
//=====================================================================
const Int32 DeviceFormFactor::MINIMUM_TABLET_WIDTH_DP;
Boolean DeviceFormFactor::sIsTablet = FALSE;

Boolean DeviceFormFactor::IsTablet(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // if (sIsTablet == null) {
    //     int minimumScreenWidthDp = context.getResources().getConfiguration().
    //             smallestScreenWidthDp;
    //     sIsTablet = minimumScreenWidthDp >= MINIMUM_TABLET_WIDTH_DP;
    // }
    // return sIsTablet;

    if (!sIsTablet) {
        AutoPtr<IResources> resources;
        CResources::New((IResources**)&resources);
        context->GetResources((IResources**)&resources);

        AutoPtr<IConfiguration> configuration;
        CConfiguration::New((IConfiguration**)&configuration);
        resources->GetConfiguration((IConfiguration**)&configuration);

        Int32 minimumScreenWidthDp = configuration->SMALLEST_SCREEN_WIDTH_DP_UNDEFINED;
        sIsTablet = minimumScreenWidthDp >= MINIMUM_TABLET_WIDTH_DP;
    }
    return sIsTablet;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


