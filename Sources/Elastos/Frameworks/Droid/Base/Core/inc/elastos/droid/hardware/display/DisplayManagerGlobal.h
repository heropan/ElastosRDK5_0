
#ifndef __ELASTOS_DROID_HARDWARE_DISPLAY_DISPLAYMANAGERGLOBAL_H__
#define __ELASTOS_DROID_HARDWARE_DISPLAY_DISPLAYMANAGERGLOBAL_H__

#include "Elastos.Droid.Core_server.h"
#include "elastos/droid/os/Handler.h"
#include <elastos/core/Object.h>
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IDisplayInfo;
using Elastos::Droid::View::IDisplayAdjustments;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::Media::Projection::IMediaProjection;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

/**
 * Manager communication with the display manager service on behalf of
 * an application process.  You're probably looking for {@link DisplayManager}.
 *
 * @hide
 */
class DisplayManagerGlobal
    : public Object
    , public IDisplayManagerGlobal
{
    friend class CDisplayManagerCallback;

private:
    class DisplayListenerDelegate
        : public Handler
    {
    public:
        DisplayListenerDelegate(
            /* [in] */ IDisplayListener* listener,
            /* [in] */ ILooper* looper);

        CARAPI_(void) SendDisplayEvent(
            /* [in] */ Int32 displayId,
            /* [in] */ Int32 event);

        CARAPI_(void) ClearEvents();

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    public:
        AutoPtr<IDisplayListener> mListener;
    };

    class VirtualDisplayCallbackDelegate : public Handler
    {
    public:
        VirtualDisplayCallbackDelegate(
            /* [in] */ IVirtualDisplayCallback* callback,
            /* [in] */ IHandler* handler);

        //@Override
        HandleMessage(
            /* [in] */ IMessage* msg);

    public:
        static const Int32 MSG_DISPLAY_PAUSED;
        static const Int32 MSG_DISPLAY_RESUMED;
        static const Int32 MSG_DISPLAY_STOPPED;

    private:
        const AutoPtr<IVirtualDisplayCallback> mCallback;
    };

    class VirtualDisplayCallback
        : public Object
        , public IIVirtualDisplayCallback
    {
    public:
        CAR_INTERFACE_DECL();

        VirtualDisplayCallback(
            /* [in] */ IVirtualDisplayCallback* callback,
            /* [in] */ IHandler* handler);

        //@Override // Binder call
        OnPaused();

        //@Override // Binder call
        OnResumed();

        //@Override // Binder call
        OnStopped();

    private:
        AutoPtr<VirtualDisplayCallbackDelegate> mDelegate;
    };

private:
    DisplayManagerGlobal(
        /* [in] */ IIDisplayManager* dm);

    ~DisplayManagerGlobal();

public:
    CAR_INTERFACE_DECL();

    /**
     * Gets an instance of the display manager global singleton.
     *
     * @return The display manager instance, may be NULL early in system startup
     * before the display manager has been fully initialized.
     */
    static CARAPI_(AutoPtr<IDisplayManagerGlobal>) GetInstance();

    /**
     * Get information about a particular logical display.
     *
     * @param displayId The logical display id.
     * @return Information about the specified display, or NULL if it does not exist.
     * This object belongs to an internal cache and should be treated as if it were immutable.
     */
    CARAPI GetDisplayInfo(
        /* [in] */ Int32 displayId,
        /* [out] */ IDisplayInfo** displayInfo);

    /**
     * Gets all currently valid logical display ids.
     *
     * @return An array containing all display ids.
     */
    CARAPI GetDisplayIds(
        /* [out, callee] */ ArrayOf<Int32>** displayIds);

    /**
     * Gets information about a logical display.
     *
     * The display metrics may be adjusted to provide compatibility
     * for legacy applications or limited screen areas.
     *
     * @param displayId The logical display id.
     * @param daj The compatibility info and activityToken.
     * @return The display object, or null if there is no display with the given id.
     */
    CARAPI GetCompatibleDisplay(
        /* [in] */ Int32 displayId,
        /* [in] */ IDisplayAdjustments* cih,
        /* [out] */ IDisplay** display);

    /**
     * Gets information about a logical display without applying any compatibility metrics.
     *
     * @param displayId The logical display id.
     * @return The display object, or NULL if there is no display with the given id.
     */
    CARAPI GetRealDisplay(
        /* [in] */ Int32 displayId,
        /* [out] */ IDisplay** display);

    /**
     * Gets information about a logical display without applying any compatibility metrics.
     *
     * @param displayId The logical display id.
     * @param IBinder the activity token for this display.
     * @return The display object, or null if there is no display with the given id.
     */
    CARAPI GetRealDisplay(
        /* [in] */ Int32 displayId,
        /* [in] */ IBinder* token,
        /* [out] */ IDisplay** display);

    CARAPI RegisterDisplayListener(
        /* [in] */ IDisplayListener* listener,
        /* [in] */ IHandler* handler);

    CARAPI UnregisterDisplayListener(
        /* [in] */ IDisplayListener* listener);

    CARAPI StartWifiDisplayScan();

    CARAPI StopWifiDisplayScan();

    CARAPI ConnectWifiDisplay(
        /* [in] */ const String& deviceAddress);

    CARAPI PauseWifiDisplay();

    CARAPI ResumeWifiDisplay();

    CARAPI DisconnectWifiDisplay();

    CARAPI RenameWifiDisplay(
        /* [in] */ const String& deviceAddress,
        /* [in] */ const String& alias);

    CARAPI ForgetWifiDisplay(
        /* [in] */ const String& deviceAddress);

    CARAPI GetWifiDisplayStatus(
        /* [out] */ IWifiDisplayStatus** status);

    CARAPI CreateVirtualDisplay(
        /* [in] */ IContext* context,
        /* [in] */ IMediaProjection* projection,
        /* [in] */ const String& name,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 densityDpi,
        /* [in] */ ISurface* surface,
        /* [in] */ Int32 flags,
        /* [in] */ IVirtualDisplayCallback* cb,
        /* [in] */ IHandler* handler,
        /* [out] */ IVirtualDisplay** outvd);

    CARAPI SetVirtualDisplaySurface(
        /* [in] */ IIVirtualDisplayCallback* token,
        /* [in] */ ISurface* surface);

    CARAPI ResizeVirtualDisplay(
        /* [in] */ IIVirtualDisplayCallback* token,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 densityDpi);

    CARAPI ReleaseVirtualDisplay(
        /* [in] */ IIVirtualDisplayCallback* token);

private:
    CARAPI_(List<AutoPtr<DisplayListenerDelegate> >::Iterator) FindDisplayListenerLocked(
        /* [in] */ IDisplayListener* listener);

    CARAPI_(void) RegisterCallbackIfNeededLocked();

    CARAPI_(void) HandleDisplayEvent(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 event);

public:
    static const Int32 EVENT_DISPLAY_ADDED = 1;
    static const Int32 EVENT_DISPLAY_CHANGED = 2;
    static const Int32 EVENT_DISPLAY_REMOVED = 3;

private:
    static const String TAG;
    static const Boolean DEBUG = FALSE;

    // True if display info and display ids should be cached.
    //
    // FIXME: The cache is currently disabled because it's unclear whether we have the
    // necessary guarantees that the caches will always be flushed before clients
    // attempt to observe their new state.  For example, depending on the order
    // in which the binder transactions take place, we might have a problem where
    // an application could start processing a configuration change due to a display
    // orientation change before the display info cache has actually been invalidated.
    static const Boolean USE_CACHE = FALSE;

    static AutoPtr<IDisplayManagerGlobal> sInstance;
    static Object sInstanceLock;

    Object mLock;

    AutoPtr<IIDisplayManager> mDm;

    AutoPtr<IIDisplayManagerCallback> mCallback;
    List<AutoPtr<DisplayListenerDelegate> > mDisplayListeners;

    HashMap<Int32, AutoPtr<IDisplayInfo> > mDisplayInfoCache;
    AutoPtr<ArrayOf<Int32> > mDisplayIdCache;

    Int32 mWifiDisplayScanNestCount;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_DISPLAY_DISPLAYMANAGERGLOBAL_H__
