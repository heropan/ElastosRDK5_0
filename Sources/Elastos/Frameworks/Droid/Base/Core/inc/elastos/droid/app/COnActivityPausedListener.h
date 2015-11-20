#ifndef __ELASTOS_DROID_APP_CONACTIVITYPAUSEDLISTENER_H__
#define __ELASTOS_DROID_APP_CONACTIVITYPAUSEDLISTENER_H__

#include "_Elastos_Droid_App_COnActivityPausedListener.h"

namespace Elastos {
namespace Droid {
namespace App {

CarClass(COnActivityPausedListener)
{
public:
    /**
     * Called when the given activity is paused.
     */
    CARAPI OnPaused(
        /* [in] */ IActivity * activity);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CONACTIVITYPAUSEDLISTENER_H__
