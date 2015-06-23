
#include "media/CSandman.h"
#include <elastos/utility/logging/Slogger.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace service {

const String CSandman::TAG = "Sandman";
const ComponentName CSandman::SOMNAMBULATOR_COMPONENT; // = new ComponentName("com.android.systemui", "com.android.systemui.Somnambulator");

CSandman::CSandman()
{}

ECode CSandman::ShouldStartDockApp(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ComponentName name = intent->ResolveActivity(context->GetPackageManager());
    *result = name != NULL && !name->Equals(SOMNAMBULATOR_COMPONENT);
    return NOERROR;
}

ECode CSandman::StartDreamByUserRequest(
    /* [in] */ IContext* context)
{
    StartDream(context, FALSE);
    return NOERROR;
}

ECode CSandman::StartDreamWhenDockedIfAppropriate(
    /* [in] */ IContext* context)
{
    if (!IsScreenSaverEnabled(context) || !IsScreenSaverActivatedOnDock(context)) {
        Slogger::I(TAG, "Dreams currently disabled for docks.");
        return NOERROR;
    }

    StartDream(context, TRUE);
    return NOERROR;
}

void CSandman::StartDream(
    /* [in] */ IContext* context,
    /* [in] */ Boolean docked)
{
    // try {
        IDreamManager dreamManagerService = IDreamManager::Stub->AsInterface(
                ServiceManager->GetService(DreamService::DREAM_SERVICE));
        if (dreamManagerService != NULL && !dreamManagerService->IsDreaming()) {
            if (docked) {
                Slogger::I(TAG, "Activating dream while docked.");

                // Wake up.
                // The power manager will wake up the system automatically when it starts
                // receiving power from a dock but there is a race between that happening
                // and the UI mode manager starting a dream.  We want the system to already
                // be awake by the time this happens.  Otherwise the dream may not start.
                PowerManager powerManager =
                        (PowerManager)context->GetSystemService(Context.POWER_SERVICE);
                powerManager->WakeUp(SystemClock->UptimeMillis());
            }
            else {
                Slogger::I(TAG, "Activating dream by user request.");
            }

            // Dream.
            dreamManagerService->Dream();
        }
    // } catch (RemoteException ex) {
        // Slog.e(TAG, "Could not start dream when docked.", ex);
    // }
}

Boolean CSandman::IsScreenSaverEnabled(
    /* [in] */ IContext* context)
{
    Int32 def = context->GetResources()->GetBoolean( com.android.internal.R.bool.config_dreamsEnabledByDefault) ? 1 : 0;
    return Settings::Secure->GetInt32ForUser(context->GetContentResolver(),
            Settings::Secure::SCREENSAVER_ENABLED, def,
            UserHandle::USER_CURRENT) != 0;
}

Boolean CSandman::IsScreenSaverActivatedOnDock(
    /* [in] */ IContext* context)
{
    Int32 def = context->GetResources()->GetBoolean( com.android.internal.R.bool.config_dreamsActivatedOnDockByDefault) ? 1 : 0;
    return Settings::Secure->GetInt32ForUser(context->GetContentResolver(),
            Settings::Secure::SCREENSAVER_ACTIVATE_ON_DOCK, def,
            UserHandle::USER_CURRENT) != 0;
}

} // namespace service
} // namepsace Droid
} // namespace Elastos
