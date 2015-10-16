
#ifndef __ELASTOS_DROID_SERVER_AM_CORESETTINGSOBSERVER_H__
#define __ELASTOS_DROID_SERVER_AM_CORESETTINGSOBSERVER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/database/ContentObserver.h"
#include "am/CActivityManagerService.h"

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Database::ContentObserver;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

/**
 * Helper class for watching a set of core settings which the framework
 * propagates to application processes to avoid multiple lookups and potentially
 * disk I/O operations. Note: This class assumes that all core settings reside
 * in {@link Settings.Secure}.
 */
class CoreSettingsObserver
    : public ContentObserver
{
public:
    CoreSettingsObserver(
        /* [in] */ CActivityManagerService* activityManagerService);

    static CARAPI_(void) Init();

    CARAPI_(AutoPtr<IBundle>) GetCoreSettingsLocked();

    CARAPI OnChange(
        /* [in] */ Boolean selfChange);

private:
    CARAPI_(void) SendCoreSettings();

    CARAPI_(void) BeginObserveCoreSettings();

    CARAPI_(void) PopulateCoreSettings(
        /* [in] */ IBundle* snapshot);

private:
   static const String TAG;

   // mapping form property name to its type
   static HashMap<String, String> sCoreSettingToTypeMap; // = new HashMap<String, Class<?>>();

    AutoPtr<IBundle> mCoreSettings;

    CActivityManagerService* mActivityManagerService;   // weak-ref
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_AM_CORESETTINGSOBSERVER_H__
