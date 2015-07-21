
#include "am/CoreSettingsObserver.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Provider::CSettingsSecure;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const String CoreSettingsObserver::TAG("CoreSettingsObserver"); // = CoreSettingsObserver.class.getSimpleName();

// mapping form property name to its type
HashMap<String, String> CoreSettingsObserver::sCoreSettingToTypeMap;

void CoreSettingsObserver::Init()
{
    sCoreSettingToTypeMap[ISettingsSecure::LONG_PRESS_TIMEOUT] = String("Int32");
    // add other core settings here...
}

CoreSettingsObserver::CoreSettingsObserver(
    /* [in] */ CActivityManagerService* activityManagerService)
    : ContentObserver(activityManagerService->mHandler)
    , mActivityManagerService(activityManagerService)
{
    Init();
    CBundle::New((IBundle**)&mCoreSettings);
    BeginObserveCoreSettings();
    SendCoreSettings();
}

AutoPtr<IBundle> CoreSettingsObserver::GetCoreSettingsLocked()
{
    AutoPtr<IBundle> coreSettings;
    mCoreSettings->Clone((IBundle**)&coreSettings);
    return coreSettings;
}

// @Override
ECode CoreSettingsObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    AutoLock lock(mActivityManagerService->mLock);
    SendCoreSettings();
    return NOERROR;
}

void CoreSettingsObserver::SendCoreSettings()
{
    PopulateCoreSettings(mCoreSettings);
    mActivityManagerService->OnCoreSettingsChange(mCoreSettings);
}

void CoreSettingsObserver::BeginObserveCoreSettings()
{
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    HashMap<String, String>::Iterator it;
    for (it = sCoreSettingToTypeMap.Begin(); it != sCoreSettingToTypeMap.End(); ++it) {
        String setting = it->mFirst;
        AutoPtr<IUri> uri;
        settingsSecure->GetUriFor(setting, (IUri**)&uri);
        AutoPtr<IContentResolver> resolver;
        mActivityManagerService->mContext->GetContentResolver((IContentResolver**)&resolver);
        resolver->RegisterContentObserver(uri, FALSE, this);
    }
}

void CoreSettingsObserver::PopulateCoreSettings(
    /* [in] */ IBundle* snapshot)
{
    AutoPtr<IContext> context = mActivityManagerService->mContext;
    HashMap<String, String>::Iterator it;
    for (it = sCoreSettingToTypeMap.Begin(); it != sCoreSettingToTypeMap.End(); ++it) {
        String setting = it->mFirst;
        String type = it->mSecond;
        // try {
        AutoPtr<ISettingsSecure> settingsSecure;
        CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
        AutoPtr<IContentResolver> resolver;
        context->GetContentResolver((IContentResolver**)&resolver);
            if (type.Equals("String")) {
                String value;
                settingsSecure->GetString(resolver, setting, &value);
                snapshot->PutString(setting, value);
            } else if (type.Equals("Int32")) {
                Int32 value;
                settingsSecure->GetInt32(resolver, setting, &value);
                snapshot->PutInt32(setting, value);
            } else if (type.Equals("Float")) {
                Float value;
                settingsSecure->GetFloat(resolver, setting, &value);
                snapshot->PutFloat(setting, value);
            } else if (type.Equals("Int64")) {
                Int64 value;
                settingsSecure->GetInt64(resolver, setting, &value);
                snapshot->PutInt64(setting, value);
            }
            else {
                Slogger::W(TAG, "Cannot find setting \"%s\" with type \"%s\"", setting.string(), type.string());
            }
        // } catch (SettingNotFoundException snfe) {
        //     Log.w(LOG_TAG, "Cannot find setting \"" + setting + "\"", snfe);
        // }
    }
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
