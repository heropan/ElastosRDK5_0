
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetHostHelper.h"
#include "appwidget/CAppWidgetHost.h"

namespace Elastos {
namespace Droid {
namespace AppWidget {

ECode CAppWidgetHostHelper::AllocateAppWidgetIdForSystem(
    /* [in] */ Int32 hostId,
    /* [out] */ Int32* appWidgetId)
{
    VALIDATE_NOT_NULL(appWidgetId);
    return CAppWidgetHost::AllocateAppWidgetIdForSystem(hostId, appWidgetId);
}

ECode CAppWidgetHostHelper::DeleteAppWidgetIdForSystem(
    /* [in] */ Int32 appWidgetId)
{
    return CAppWidgetHost::DeleteAppWidgetIdForSystem(appWidgetId);
}

ECode CAppWidgetHostHelper::DeleteAllHosts()
{
    return CAppWidgetHost::DeleteAllHosts();
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
