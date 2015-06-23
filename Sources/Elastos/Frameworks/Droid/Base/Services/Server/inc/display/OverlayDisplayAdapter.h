
#ifndef __OVERLAYDISPLAYADAPTER_H__
#define __OVERLAYDISPLAYADAPTER_H__

#include "ext/frameworkext.h"
#include "display/DisplayAdapter.h"
#include "display/DisplayDevice.h"
#include "os/Runnable.h"
#include "database/ContentObserver.h"
#include <elastos/utility/etl/List.h>
#include <elastos/Core/Object.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::Object;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Graphics::ISurfaceTexture;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Utility::Regex::IPattern;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

class OverlayDisplayWindow;
class DisplayDeviceInfo;

/**
 * A display adapter that uses overlay windows to simulate secondary displays
 * for development purposes.  Use Development Settings to enable one or more
 * overlay displays.
 * <p>
 * This object has two different handlers (which may be the same) which must not
 * get confused.  The main handler is used to posting messages to the display manager
 * service as usual.  The UI handler is only used by the {@link OverlayDisplayWindow}.
 * </p><p>
 * Display adapters are guarded by the {@link DisplayManagerService.SyncRoot} lock.
 * </p>
 */
class OverlayDisplayAdapter : public DisplayAdapter
{
private:

    class OverlayDisplayContentObserver
        : public ContentObserver
    {
    public:
        OverlayDisplayContentObserver(
            /* [in] */ IHandler* handler,
            /* [in] */ OverlayDisplayAdapter* host);

        virtual CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        OverlayDisplayAdapter* mHost;
    };

    class RegisterRunnable
        : public Runnable
    {
    public:
        RegisterRunnable(
            /* [in] */ OverlayDisplayAdapter* host);

        virtual CARAPI Run();

    private:
        OverlayDisplayAdapter* mHost;
    };

    class OverlayDisplayDevice : public DisplayDevice
    {
    public:
        OverlayDisplayDevice(
            /* [in] */ OverlayDisplayAdapter* owner,
            /* [in] */ IBinder* displayToken,
            /* [in] */ const String& name,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Float refreshRate,
            /* [in] */ Int32 densityDpi,
            /* [in] */ ISurfaceTexture* surfaceTexture);

        CARAPI_(void) ClearSurfaceTextureLocked();

        //@Override
        CARAPI_(void) PerformTraversalInTransactionLocked();

        //@Override
        CARAPI_(AutoPtr<DisplayDeviceInfo>) GetDisplayDeviceInfoLocked();

    private:
        String mName;
        Int32 mWidth;
        Int32 mHeight;
        Float mRefreshRate;
        Int32 mDensityDpi;

        AutoPtr<ISurface> mSurface;
        AutoPtr<ISurfaceTexture> mSurfaceTexture;
        AutoPtr<DisplayDeviceInfo> mInfo;

        OverlayDisplayAdapter* mHost;
    };

    /**
     * Functions as a handle for overlay display devices which are created and
     * destroyed asynchronously.
     *
     * Guarded by the {@link DisplayManagerService.SyncRoot} lock.
     */
    class OverlayDisplayHandle
        : public ElRefBase
        , public IOverlayDisplayWindowListener
    {
    private:
        // Runs on the UI thread.
        class ShowRunnable
            : public Runnable
        {
        public:
            ShowRunnable(
                /* [in] */ OverlayDisplayHandle* host);

            virtual CARAPI Run();

        private:
            OverlayDisplayHandle* mHost;
        };

        // Runs on the UI thread.
        class DismissRunnable
            : public Runnable
        {
        public:
            DismissRunnable(
                /* [in] */ OverlayDisplayHandle* host);

            virtual CARAPI Run();

        private:
            OverlayDisplayHandle* mHost;
        };

    public:
        CAR_INTERFACE_DECL()

        OverlayDisplayHandle(
            /* [in] */ OverlayDisplayAdapter* owner,
            /* [in] */ const String& name,
            /* [in] */ Int32 width,
            /* [in] */ Int32 height,
            /* [in] */ Int32 densityDpi,
            /* [in] */ Int32 gravity);

        CARAPI_(void) DismissLocked();

        // Called on the UI thread.
        //@Override
        CARAPI OnWindowCreated(
            /* [in] */ ISurfaceTexture* surfaceTexture,
            /* [in] */ Float refreshRate);

        // Called on the UI thread.
        //@Override
        CARAPI OnWindowDestroyed();

        CARAPI_(void) DumpLocked(
            /* [in] */ IPrintWriter* pw);

    private:
        // Runs on the UI thread.
        CARAPI_(void) HandleShow();

        // Runs on the UI thread.
        CARAPI_(void) HandleDismiss();

    private:
        String mName;
        Int32 mWidth;
        Int32 mHeight;
        Int32 mDensityDpi;
        Int32 mGravity;

        AutoPtr<IRunnable> mShowRunnable;
        AutoPtr<IRunnable> mDismissRunnable;

        AutoPtr<OverlayDisplayWindow> mWindow;
        AutoPtr<OverlayDisplayDevice> mDevice;
        OverlayDisplayAdapter* mHost;
    };

public:
    // Called with SyncRoot lock held.
    OverlayDisplayAdapter(
        /* [in] */ Object* syncRoot,
        /* [in] */ IContext* context,
        /* [in] */ IHandler* handler,
        /* [in] */ IDisplayAdapterListener* listener,
        /* [in] */ IHandler* uiHandler);

    //@Override
    CARAPI_(void) DumpLocked(
        /* [in] */ IPrintWriter* pw);

    //@Override
    CARAPI_(void) RegisterLocked();

private:
    CARAPI_(void) UpdateOverlayDisplayDevices();

    CARAPI_(void) UpdateOverlayDisplayDevicesLocked();

    static CARAPI_(Int32) ChooseOverlayGravity(
        /* [in] */ Int32 overlayNumber);

protected:
    static const String TAG;
    static const Boolean DEBUG;

private:
    static const Int32 MIN_WIDTH = 100;
    static const Int32 MIN_HEIGHT = 100;
    static const Int32 MAX_WIDTH = 4096;
    static const Int32 MAX_HEIGHT = 4096;

    static const AutoPtr<IPattern> SETTING_PATTERN;

private:
    AutoPtr<IHandler> mUiHandler;
    List<AutoPtr<OverlayDisplayHandle> > mOverlays;
    String mCurrentOverlaySetting;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__OVERLAYDISPLAYADAPTER_H__
