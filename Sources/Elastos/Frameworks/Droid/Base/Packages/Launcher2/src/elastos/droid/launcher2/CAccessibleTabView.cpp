
#include "elastos/droid/launcher2/CAccessibleTabView.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Launcher2 {

CAR_INTERFACE_IMPL(CAccessibleTabView, TextView, IAccessibleTabView);

CAR_OBJECT_IMPL(CAccessibleTabView);

ECode CAccessibleTabView::constructor(
    /* [in] */ IContext* context)
{
    return TextView::constructor(context);
}

ECode CAccessibleTabView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TextView::constructor(context, attrs);
}

ECode CAccessibleTabView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TextView::constructor(context, attrs, defStyle);
}

ECode CAccessibleTabView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    assert(0 && "need class FocusHelper");
    // if(FocusHelper::HandleTabKeyEvent(this, keyCode, event)) {
    //     *result = TRUE;
    //     return NOERROR;
    // }
    Boolean res;
    TextView::OnKeyDown(keyCode, event, &res);
    *result = res;
    return NOERROR;
}

ECode CAccessibleTabView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    assert(0 && "need class FocusHelper");
    // if (FocusHelper::HandleTabKeyEvent(this, keyCode, event)) {
    //     *result = TRUE;
    //     return NOERROR;
    // }
    Boolean res;
    TextView::OnKeyUp(keyCode, event, &res);
    *result = res;
    return NOERROR;
}

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos