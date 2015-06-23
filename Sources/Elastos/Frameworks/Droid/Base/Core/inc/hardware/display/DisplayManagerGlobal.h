
#ifndef __DISPLAYMANAGERGLOBAL_H__
#define __DISPLAYMANAGERGLOBAL_H__

#include "ext/frameworkdef.h"
#include "Elastos.Droid.Core_server.h"
#include "os/HandlerBase.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Core::Mutex;
using Elastos::Droid::View::ICompatibilityInfoHolder;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IDisplayInfo;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IBinder;

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
    : public ElRefBase
    , public IDisplayManagerGlobal
{
    friend class CDisplayManagerCallback;

private:
    class DisplayListenerDelegate
        : public HandlerBase
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
    static CARAPI_(AutoPtr<DisplayManagerGlobal>) GetInstance();

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
     * for legacy applications.
     *
     * @param displayId The logical display id.
     * @param cih The compatibility info, or NULL if none is required.
     * @return The display object, or NULL if there is no display with the given id.
     */
    CARAPI GetCompatibleDisplay(
        /* [in] */ Int32 displayId,
        /* [in] */ ICompatibilityInfoHolder* cih,
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

    CARAPI RegisterDisplayListener(
        /* [in] */ IDisplayListener* listener,
        /* [in] */ IHandler* handler);

    CARAPI UnregisterDisplayListener(
        /* [in] */ IDisplayListener* listener);

    CARAPI ScanWifiDisplays();

    CARAPI ConnectWifiDisplay(
        /* [in] */ const String& deviceAddress);

    CARAPI DisconnectWifiDisplay();

    CARAPI RenameWifiDisplay(
        /* [in] */ const String& deviceAddress,
        /* [in] */ const String& alias);

    CARAPI ForgetWifiDisplay(
        /* [in] */ const String& deviceAddress);

    CARAPI GetWifiDisplayStatus(
        /* [out] */ IWifiDisplayStatus** status);

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

    static AutoPtr<DisplayManagerGlobal> sInstance;
    static Mutex sInstanceLock;

    Mutex mLock;

    AutoPtr<IIDisplayManager> mDm;

    AutoPtr<IDisplayManagerCallback> mCallback;
    List<AutoPtr<DisplayListenerDelegate> > mDisplayListeners;

    HashMap<Int32, AutoPtr<IDisplayInfo> > mDisplayInfoCache;
    AutoPtr<ArrayOf<Int32> > mDisplayIdCache;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __DISPLAYMANAGERGLOBAL_H__
