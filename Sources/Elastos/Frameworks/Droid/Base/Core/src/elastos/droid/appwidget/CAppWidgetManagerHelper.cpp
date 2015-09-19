
#include "appwidget/CAppWidgetManagerHelper.h"
#include "appwidget/CAppWidgetManager.h"

namespace Elastos {
namespace Droid {
namespace AppWidget {

ECode CAppWidgetManagerHelper::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IAppWidgetManager** appWidgetManager)
{
    return CAppWidgetManager::GetInstance(context, appWidgetManager);
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
