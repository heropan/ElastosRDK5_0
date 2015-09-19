
#ifndef __ELASTOS_DROID_SERVICE_CSANDMAN_H__
#define __ELASTOS_DROID_SERVICE_CSANDMAN_H__

#include "_Elastos_Droid_service_CSandman.h"
#include <ext/frameworkext.h>


namespace Elastos {
namespace Droid {
namespace service {

CarClass(CSandman)
{
public:
    /**
     * Returns true if the specified dock app intent should be started.
     * False if we should dream instead, if appropriate.
     */
    /*static*/
    CARAPI ShouldStartDockApp(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* result);

    /**
     * Starts a dream manually.
     */
    /*static*/
    CARAPI StartDreamByUserRequest(
        /* [in] */ IContext* context);

    /**
     * Starts a dream when docked if the system has been configured to do so,
     * otherwise does nothing.
     */
    /*static*/
    CARAPI StartDreamWhenDockedIfAppropriate(
        /* [in] */ IContext* context);

private:
    // The sandman is eternal.  No one instantiates him.
    CSandman();

    /*static*/ CARAPI_(void) StartDream(
        /* [in] */ IContext* context,
        /* [in] */ Boolean docked);

    /*static*/ CARAPI_(Boolean) IsScreenSaverEnabled(
        /* [in] */ IContext* context);

    /*static*/ CARAPI_(Boolean) IsScreenSaverActivatedOnDock(
        /* [in] */ IContext* context);

private:
    static const String TAG; // = "Sandman";

    // The component name of a special dock app that merely launches a dream.
    // We don't want to launch this app when docked because it causes an unnecessary
    // activity transition.  We just want to start the dream.
    static const ComponentName SOMNAMBULATOR_COMPONENT; // = new ComponentName("com.android.systemui", "com.android.systemui.Somnambulator");
};

} // namespace service
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVICE_CSANDMAN_H__
