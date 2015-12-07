
#ifndef __ELASTOS_DROID_SERVER_DISPLAY_LOCALDISPLAYADAPTER_H__
#define __ELASTOS_DROID_SERVER_DISPLAY_LOCALDISPLAYADAPTER_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/server/display/DisplayAdapter.h"
#include "elastos/droid/server/display/DisplayDevice.h"
#include "elastos/droid/server/display/DisplayDeviceInfo.h"
#include "elastos/droid/view/DisplayEventReceiver.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::View::IPhysicalDisplayInfo;
using Elastos::Droid::View::DisplayEventReceiver;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

/**
 * A display adapter for the local displays managed by Surface Flinger.
 * <p>
 * Display adapters are guarded by the {@link DisplayManagerService.SyncRoot} lock.
 * </p>
 */
class LocalDisplayAdapter
    : public DisplayAdapter
{
private:
    class LocalDisplayDevice
        : public DisplayDevice
    {
    public:
        LocalDisplayDevice(
            /* [in] */ LocalDisplayAdapter* owner,
            /* [in] */ IBinder* displayToken,
            /* [in] */ Int32 builtInDisplayId,
            /* [in] */ IPhysicalDisplayInfo* phys);

        CARAPI_(Boolean) UpdatePhysicalDisplayInfoLocked(
            /* [in] */ IPhysicalDisplayInfo* phys);

        //@Override
        CARAPI_(void) ApplyPendingDisplayDeviceInfoChangesLocked();

        //@Override
        CARAPI_(AutoPtr<DisplayDeviceInfo>) GetDisplayDeviceInfoLocked();

        //@Override
        CARAPI_(void) BlankLocked();

        //@Override
        CARAPI_(void) UnblankLocked();

        //@Override
        CARAPI_(void) DumpLocked(
            /* [in] */ IPrintWriter* pw);

    private:
        Int32 mBuiltInDisplayId;
        AutoPtr<IPhysicalDisplayInfo> mPhys;

        AutoPtr<DisplayDeviceInfo> mInfo;
        Boolean mHavePendingChanges;
        Boolean mBlanked;
        LocalDisplayAdapter* mHost;
    };

    class HotplugDisplayEventReceiver
        : public DisplayEventReceiver
    {
    public:
        HotplugDisplayEventReceiver(
            /* [in] */ ILooper* looper,
            /* [in] */ LocalDisplayAdapter* owner);

        //@Override
        CARAPI_(void) OnHotplug(
            /* [in] */ Int64 timestampNanos,
            /* [in] */ Int32 builtInDisplayId,
            /* [in] */ Boolean connected);

    private:
        LocalDisplayAdapter* mHost;
    };

public:
    // Called with SyncRoot lock held.
    LocalDisplayAdapter(
        /* [in] */ Object* syncRoot,
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ IDisplayAdapterListener* listener);

    virtual ~LocalDisplayAdapter();

    //@Override
    CARAPI_(void) RegisterLocked();

private:
    CARAPI_(void) ScanDisplaysLocked();

private:
    static const String TAG;
    static const Int32 BUILT_IN_DISPLAY_IDS_TO_SCAN[];
    static const Int32 BUILT_IN_DISPLAY_IDS_COUNT = 2;

    HashMap<Int32, AutoPtr<LocalDisplayDevice> > mDevices;
    AutoPtr<HotplugDisplayEventReceiver> mHotplugReceiver;
    AutoPtr<IPhysicalDisplayInfo> mTempPhys;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_DISPLAY_LOCALDISPLAYADAPTER_H__
