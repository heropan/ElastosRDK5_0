
#ifndef __ACTIVITYMANAGERNATIVE_H__
#define __ACTIVITYMANAGERNATIVE_H__

#include "ext/frameworkext.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentSender;

namespace Elastos {
namespace Droid {
namespace App {

class ActivityManagerNative
{
public:
    static CARAPI_(AutoPtr<IIActivityManager>) GetDefault();

    /**
     * Convenience for checking whether the system is ready.  For internal use only.
     */
    static CARAPI_(Boolean) IsSystemReady();

    /**
     * Convenience for sending a sticky broadcast.  For internal use only.
     * If you don't care about permission, use null.
     */
    static CARAPI_(void) BroadcastStickyIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& permission,
        /* [in] */ Int32 userId);

    static CARAPI_(void) NoteWakeupAlarm(
        /* [in] */ IPendingIntent* ps);

protected:
    static Boolean sSystemReady;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ACTIVITYMANAGERNATIVE_H__
