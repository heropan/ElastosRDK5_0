
#include "elastos/droid/app/CDisableCarModeActivity.h"

#include "elastos/droid/os/ServiceManager.h"

using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::App::IUiModeManager;
using Elastos::Droid::App::IIUiModeManager;
using Elastos::Droid::App::EIID_IIUiModeManager;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

const String CDisableCarModeActivity::TAG("CDisableCarModeActivity");

ECode CDisableCarModeActivity::constructor()
{
    return NOERROR;
}

ECode CDisableCarModeActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Activity::OnCreate(savedInstanceState);
//     try {
        AutoPtr<IIUiModeManager> uiModeManager = (IIUiModeManager*)ServiceManager::GetService(String("uimode"))->Probe(EIID_IIUiModeManager);
        uiModeManager->DisableCarMode(IUiModeManager::DISABLE_CAR_MODE_GO_HOME);
//     } catch (RemoteException e) {
//         Log.e(TAG, "Failed to disable car mode", e);
//     }
    Finish();
    return NOERROR;
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos