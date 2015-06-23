
#include "content/CIntentShortcutIconResourceHelper.h"
#include "content/CIntentShortcutIconResource.h"
#include <ext/frameworkdef.h>

using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Content {

ECode CIntentShortcutIconResourceHelper::FromContext(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId,
    /* [out] */ IIntentShortcutIconResource** shortcutIconRes)
{
    VALIDATE_NOT_NULL(shortcutIconRes)
    *shortcutIconRes = NULL;
    VALIDATE_NOT_NULL(context)

    AutoPtr<IIntentShortcutIconResource> icon;
    FAIL_RETURN(CIntentShortcutIconResource::New((IIntentShortcutIconResource**)&icon))
    String packageName;
    FAIL_RETURN(context->GetPackageName(&packageName))
    FAIL_RETURN(icon->SetPackageName(packageName))
    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res))
    String resourceName;
    FAIL_RETURN(res->GetResourceName(resourceId, &resourceName))
    FAIL_RETURN(icon->SetResourceName(resourceName))
    *shortcutIconRes = icon;
    REFCOUNT_ADD(*shortcutIconRes)
    return NOERROR;
}

}
}
}

