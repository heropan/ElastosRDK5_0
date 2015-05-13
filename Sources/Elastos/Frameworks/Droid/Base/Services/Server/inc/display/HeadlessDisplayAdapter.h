
#ifndef __HEADLESSDISPLAYADAPTER_H__
#define __HEADLESSDISPLAYADAPTER_H__

#include "display/DisplayAdapter.h"
#include "display/DisplayDevice.h"
#include "display/DisplayDeviceInfo.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

/**
 * Provides a fake default display for headless systems.
 * <p>
 * Display adapters are guarded by the {@link DisplayManagerService.SyncRoot} lock.
 * </p>
 */
class HeadlessDisplayAdapter : public DisplayAdapter
{
private:
    class HeadlessDisplayDevice : public DisplayDevice
    {
    public:
        HeadlessDisplayDevice(
            /* [in] */ HeadlessDisplayAdapter* owner);

        //@Override
        CARAPI_(AutoPtr<DisplayDeviceInfo>) GetDisplayDeviceInfoLocked();

    private:
        AutoPtr<DisplayDeviceInfo> mInfo;
        HeadlessDisplayAdapter* mOwner;
    };

public:
    // Called with SyncRoot lock held.
    HeadlessDisplayAdapter(
        /* [in] */ Object* syncRoot,
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ IDisplayAdapterListener* listener);

    //@Override
    CARAPI_(void) RegisterLocked();

private:
    static const String TAG;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__HEADLESSDISPLAYADAPTER_H__
