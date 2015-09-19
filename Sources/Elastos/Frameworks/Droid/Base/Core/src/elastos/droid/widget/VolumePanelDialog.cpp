#include "widget/VolumePanelDialog.h"
#include "widget/CVolumePanel.h"

namespace Elastos {
namespace Droid {
namespace Widget {

Boolean VolumePanelDialog::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);

    if (action == IMotionEvent::ACTION_OUTSIDE) {
        CVolumePanel* panel = (CVolumePanel*)mVolumePanel.Get();
        if (IsShowing()) {
            panel->ForceTimeout();
            return TRUE;
        }
    }
    return FALSE;
}

ECode VolumePanelDialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ IVolumePanel* panel)
{
    mVolumePanel = panel;
    return Dialog::Init(context, theme);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
