
#include "ext/frameworkdef.h"
#include "widget/CToastHelper.h"
#include "widget/Toast.h"

namespace Elastos {
namespace Droid {
namespace Widget {

ECode CToastHelper::MakeText(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 duration,
    /* [out] */ IToast** toast)
{
    VALIDATE_NOT_NULL(toast);
    return Toast::MakeText(context, text, duration, toast);
}

ECode CToastHelper::MakeTextEx(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Int32 duration,
    /* [out] */ IToast** toast)
{
    VALIDATE_NOT_NULL(toast);
    return Toast::MakeTextEx(context, resId, duration, toast);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
