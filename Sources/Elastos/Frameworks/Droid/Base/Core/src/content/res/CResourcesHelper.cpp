
#include "ext/frameworkdef.h"
#include "content/res/CResourcesHelper.h"
#include "content/res/CResources.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

ECode CResourcesHelper::SelectDefaultTheme(
    /* [in] */ Int32 curTheme,
    /* [in] */ Int32 targetSdkVersion,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = CResources::SelectDefaultTheme(curTheme, targetSdkVersion);
    return NOERROR;
}

ECode CResourcesHelper::SelectSystemTheme(
    /* [in] */ Int32 curTheme,
    /* [in] */ Int32 targetSdkVersion,
    /* [in] */ Int32 orig,
    /* [in] */ Int32 holo,
    /* [in] */ Int32 deviceDefault,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = CResources::SelectSystemTheme(curTheme, targetSdkVersion, orig, holo, deviceDefault);
    return NOERROR;
}

ECode CResourcesHelper::GetSystem(
    /* [out] */ IResources** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IResources> r = CResources::GetSystem();
    *res = r;
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CResourcesHelper::UpdateSystemConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics,
    /* [in] */ ICompatibilityInfo* compat)
{
    CResources::UpdateSystemConfiguration(config, metrics, compat);
    return NOERROR;
}

ECode CResourcesHelper::UpdateSystemConfiguration(
    /* [in] */ IConfiguration* config,
    /* [in] */ IDisplayMetrics* metrics)
{
    CResources::UpdateSystemConfiguration(config, metrics);
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos
