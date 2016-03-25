
#include "elastos/droid/settings/wifi/CWpsDialogHelper.h"

namespace Elastos {
namespace Droid {
namespace Settings {
namespace Wifi {

CAR_INTERFACE_IMPL(CWpsDialogHelper, Singleton, IWpsDialogHelper)

CAR_SINGLETON_IMPL(CWpsDialogHelper)

ECode CWpsDialogHelper::WpsDialogStateFromString(
    /* [in] */ const String& str,
    /* [out] */ WpsDialogState* state)
{
    VALIDATE_NOT_NULL(state)

    if (str.Equals("WPS_INIT")) {
        *state = WpsDialogState_WPS_INIT;
    }
    else if (str.Equals("WPS_START")) {
        *state = WpsDialogState_WPS_START;
    }
    else if (str.Equals("WPS_COMPLETE")) {
        *state = WpsDialogState_WPS_COMPLETE;
    }
    else if (str.Equals("CONNECTED")) {
        *state = WpsDialogState_CONNECTED;
    }
    else if (str.Equals("WPS_FAILED")) {
        *state = WpsDialogState_WPS_FAILED;
    }
    else if (str.Equals("NONE")) {
        *state = WpsDialogState_NONE;
    }

    return NOERROR;
}

ECode CWpsDialogHelper::WpsDialogStateToString(
    /* [in] */ WpsDialogState state,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    if (state == WpsDialogState_WPS_INIT) {
        *str = "WPS_INIT";
    }
    else if (state == WpsDialogState_WPS_START) {
        *str = "WPS_START";
    }
    else if (state == WpsDialogState_WPS_COMPLETE) {
        *str = "WPS_COMPLETE";
    }
    else if (state == WpsDialogState_CONNECTED) {
        *str = "CONNECTED";
    }
    else if (state == WpsDialogState_WPS_FAILED) {
        *str = "WPS_FAILED";
    }
    else if (state == WpsDialogState_NONE) {
        *str = "NONE";
    }
    return NOERROR;
}

ECode CWpsDialogHelper::Index(
    /* [in] */ WpsDialogState state,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)

    if (state == WpsDialogState_WPS_INIT) {
        *index = 0;
    }
    else if (state == WpsDialogState_WPS_START) {
        *index = 1;
    }
    else if (state == WpsDialogState_WPS_COMPLETE) {
        *index = 2;
    }
    else if (state == WpsDialogState_CONNECTED) {
        *index = 3;
    }
    else if (state == WpsDialogState_WPS_FAILED) {
        *index = 4;
    }
    else if (state == WpsDialogState_NONE) {
        *index = 5;
    }
    return NOERROR;
}

} // namespace Wifi
} // namespace Settings
} // namespace Droid
} // namespace Elastos
