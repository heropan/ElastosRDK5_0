
#ifndef __ELASTOS_DROID_PACKAGES_SYSTEMUI_CRESETSESSIONDIALOG_H__
#define __ELASTOS_DROID_PACKAGES_SYSTEMUI_CRESETSESSIONDIALOG_H__

#include "_Elastos_Droid_Packages_SystemUI_CResetSessionDialog.h"
#include "elastos/droid/packages/systemui/statusbar/phone/SystemUIDialog.h"

using Elastos::Droid::Packages::SystemUI::StatusBar::Phone::SystemUIDialog;

namespace Elastos {
namespace Droid {
namespace Packages {
namespace SystemUI {

CarClass(CResetSessionDialog)
    , public SystemUIDialog
    , public IDialogInterfaceOnClickListener
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CResetSessionDialog();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 userId);

    // @Override
    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

private:
    static const Int32 BUTTON_WIPE;
    static const Int32 BUTTON_DONTWIPE;

    Int32 mUserId;
};

} // namespace SystemUI
} // namespace Packages
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_PACKAGES_SYSTEMUI_CRESETSESSIONDIALOG_H__
