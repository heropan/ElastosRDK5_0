
#include "ext/frameworkdef.h"
#include "appwidget/CAppWidgetHostViewHelper.h"
#include "appwidget/CAppWidgetHostView.h"

namespace Elastos {
namespace Droid {
namespace AppWidget {

ECode CAppWidgetHostViewHelper::GetDefaultPaddingForWidget(
    /* [in] */ IContext* context,
    /* [in] */ IComponentName* component,
    /* [in] */ IRect* padding,
    /* [out] */ IRect** rect)
{
    VALIDATE_NOT_NULL(rect);
    AutoPtr<IRect> r = CAppWidgetHostView::GetDefaultPaddingForWidget(context, component, padding);
    *rect = r.Get();
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos
