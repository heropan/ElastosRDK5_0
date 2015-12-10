#include "elastos/droid/content/CComponentName.h"
// #include "elastos/droid/provider/Settings.h"
#include "elastos/droid/service/dreams/Sandman.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IResources;
// using Elastos::Droid::Provider::Settings;
using Elastos::Droid::R;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Dreams {

static AutoPtr<IComponentName> InitStatic()
{
    AutoPtr<CComponentName> cc;
    CComponentName::NewByFriend(String("com.android.systemui"), String("com.android.systemui.Somnambulator"), (CComponentName**)&cc);
    return (IComponentName*)(cc.Get());
}

const String Sandman::TAG("Sandman");
AutoPtr<IComponentName> Sandman::SOMNAMBULATOR_COMPONENT = InitStatic();

Sandman::Sandman()
{
}

ECode Sandman::ShouldStartDockApp(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IComponentName> name;
    AutoPtr<IPackageManager> pm;
    context->GetPackageManager((IPackageManager**)&pm);
    intent->ResolveActivity(pm, (IComponentName**)&name);
    Boolean isEqual = FALSE;
    IObject::Probe(name)->Equals(SOMNAMBULATOR_COMPONENT, &isEqual);
    *result = name != NULL && !isEqual;
    return NOERROR;
}

ECode Sandman::StartDreamByUserRequest(
    /* [in] */ IContext* context)
{
    StartDream(context, FALSE);
    return NOERROR;
}

ECode Sandman::StartDreamWhenDockedIfAppropriate(
    /* [in] */ IContext* context)
{
    if (!IsScreenSaverEnabled(context) || !IsScreenSaverActivatedOnDock(context)) {
        Slogger::I(TAG, "Dreams currently disabled for docks.");
        return E_NULL_POINTER_EXCEPTION;
    }

    StartDream(context, TRUE);
    return NOERROR;
}

void Sandman::StartDream(
    /* [in] */ IContext* context,
    /* [in] */ Boolean docked)
{
#if 0
    try {
        IDreamManager dreamManagerService = IDreamManager.Stub.asInterface(
                ServiceManager.getService(DreamService.DREAM_SERVICE));
        if (dreamManagerService != null && !dreamManagerService.isDreaming()) {
            if (docked) {
                Slog.i(TAG, "Activating dream while docked.");

                // Wake up.
                // The power manager will wake up the system automatically when it starts
                // receiving power from a dock but there is a race between that happening
                // and the UI mode manager starting a dream.  We want the system to already
                // be awake by the time this happens.  Otherwise the dream may not start.
                PowerManager powerManager =
                        (PowerManager)context.getSystemService(Context.POWER_SERVICE);
                powerManager.wakeUp(SystemClock.uptimeMillis());
            } else {
                Slog.i(TAG, "Activating dream by user request.");
            }

            // Dream.
            dreamManagerService.dream();
        }
    } catch (RemoteException ex) {
        Slog.e(TAG, "Could not start dream when docked.", ex);
    }
#endif
}

Boolean Sandman::IsScreenSaverEnabled(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    Boolean def;
    res->GetBoolean(R::bool_::config_dreamsEnabledByDefault ? 1 : 0, &def);
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    //TODO
    // Int32 i;
    // Settings::Secure::GetInt32ForUser(resolver, ISettingsSecure::SCREENSAVER_ENABLED, def, IUserHandle::USER_CURRENT, &i);
    // return i != 0;
    assert(0);
    return FALSE;
}

Boolean Sandman::IsScreenSaverActivatedOnDock(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    Boolean def;
    res->GetBoolean(R::bool_::config_dreamsActivatedOnDockByDefault ? 1 : 0, &def);
    AutoPtr<IContentResolver> resolver;
    context->GetContentResolver((IContentResolver**)&resolver);
    //TODO
    // Int32 i;
    // Settings::Secure::GetInt32ForUser(resolver, ISettingsSecure::SCREENSAVER_ACTIVATE_ON_DOCK, def, IUserHandle::USER_CURRENT, &i);
    // return i != 0;
    assert(0);
    return FALSE;
}

} // namespace Dreams
} // namespace Service
} // namepsace Droid
} // namespace Elastos
