#ifndef __CONACTIVITYPAUSEDLISTENER_H__
#define __CONACTIVITYPAUSEDLISTENER_H__

#include "_COnActivityPausedListener.h"

namespace Elastos{
namespace Droid{
namespace App{

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

#endif // __CONACTIVITYPAUSEDLISTENER_H__
