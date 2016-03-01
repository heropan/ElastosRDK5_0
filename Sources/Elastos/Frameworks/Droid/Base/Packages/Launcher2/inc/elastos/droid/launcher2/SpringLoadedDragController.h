#ifndef  __ELASTOS_DROID_LAUNCHER2_SPRINGLOADEDDRAGCONTROLLER_H__
#define  __ELASTOS_DROID_LAUNCHER2_SPRINGLOADEDDRAGCONTROLLER_H__

#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Launcher2 {

class SpringLoadedDragController
    : public OnAlarmListener
{
public:
    SpringLoadedDragController(
        /* [in] */ ILauncher* launcher);

    CARAPI Cancel();

    // Set a new alarm to expire for the screen that we are hovering over now
    CARAPI SetAlarm(
        /* [in] */ ICellLayout* cl);

    // this is called when our timer runs out
    CARAPI OnAlarm(
        /* [in] */ IAlarm* alarm);

public:
    // how long the user must hover over a mini-screen before it unshrinks
    const Int64 ENTER_SPRING_LOAD_HOVER_TIME;
    const Int64 ENTER_SPRING_LOAD_CANCEL_HOVER_TIME;
    const Int64 EXIT_SPRING_LOAD_HOVER_TIME;

    AutoPtr<IAlarm> mAlarm;

private:
    // the screen the user is currently hovering over, if any
    AutoPtr<ICellLayout> mScreen;
    AutoPtr<ILauncher> mLauncher;
};

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_LAUNCHER2_SPRINGLOADEDDRAGCONTROLLER_H__