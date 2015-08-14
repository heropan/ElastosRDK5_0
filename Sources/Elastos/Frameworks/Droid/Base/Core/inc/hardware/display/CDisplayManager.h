
#ifndef __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYMANAGER_H__
#define __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYMANAGER_H__

#include "_CDisplayManager.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::IDisplay;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

/**
 * Manages the properties of attached displays.
 * <p>
 * Get an instance of this class by calling
 * {@link android.content.Context#getSystemService(java.lang.String)
 * Context.getSystemService()} with the argument
 * {@link android.content.Context#DISPLAY_SERVICE}.
 * </p>
 */
CarClass(CDisplayManager)
{
public:
    CDisplayManager();

    ~CDisplayManager();

    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * Gets information about a logical display.
     *
     * The display metrics may be adjusted to provide compatibility
     * for legacy applications.
     *
     * @param displayId The logical display id.
     * @return The display object, or NULL if there is no valid display with the given id.
     */
    CARAPI GetDisplay(
        /* [in] */ Int32 displayId,
        /* [out] */ IDisplay** display);

    /**
     * Gets all currently valid logical displays.
     *
     * @return An array containing all displays.
     */
    CARAPI GetDisplays(
        /* [out] */ ArrayOf<IDisplay*>** displays);

    /**
     * Gets all currently valid logical displays of the specified category.
     * <p>
     * When there are multiple displays in a category the returned displays are sorted
     * of preference.  For example, if the requested category is
     * {@link #DISPLAY_CATEGORY_PRESENTATION} and there are multiple presentation displays
     * then the displays are sorted so that the first display in the returned array
     * is the most preferred presentation display.  The application may simply
     * use the first display or allow the user to choose.
     * </p>
     *
     * @param category The requested display category or NULL to return all displays.
     * @return An array containing all displays sorted by order of preference.
     *
     * @see #DISPLAY_CATEGORY_PRESENTATION
     */
    CARAPI GetDisplays(
        /* [in] */ const String& category,
        /* [out] */ ArrayOf<IDisplay*>** displays);

    /**
     * Registers an display listener to receive notifications about when
     * displays are added, removed or changed.
     *
     * @param listener The listener to register.
     * @param handler The handler on which the listener should be invoked, or NULL
     * if the listener should be invoked on the calling thread's looper.
     *
     * @see #unregisterDisplayListener
     */
    CARAPI RegisterDisplayListener(
        /* [in] */ IDisplayListener* listener,
        /* [in] */ IHandler* handler);

    /**
     * Unregisters an input device listener.
     *
     * @param listener The listener to unregister.
     *
     * @see #registerDisplayListener
     */
    CARAPI UnregisterDisplayListener(
        /* [in] */ IDisplayListener* listener);

    /**
     * Initiates a fresh scan of availble Wifi displays.
     * The results are sent as a {@link #ACTION_WIFI_DISPLAY_STATUS_CHANGED} broadcast.
     * @hide
     */
    CARAPI ScanWifiDisplays();

    /**
     * Connects to a Wifi display.
     * The results are sent as a {@link #ACTION_WIFI_DISPLAY_STATUS_CHANGED} broadcast.
     * <p>
     * Automatically remembers the display after a successful connection, if not
     * already remembered.
     * </p><p>
     * Requires {@link android.Manifest.permission#CONFIGURE_WIFI_DISPLAY} to connect
     * to unknown displays.  No permissions are required to connect to already known displays.
     * </p>
     *
     * @param deviceAddress The MAC address of the device to which we should connect.
     * @hide
     */
    CARAPI ConnectWifiDisplay(
        /* [in] */ const String& deviceAddress);

    /**
     * Disconnects from the current Wifi display.
     * The results are sent as a {@link #ACTION_WIFI_DISPLAY_STATUS_CHANGED} broadcast.
     * @hide
     */
    CARAPI DisconnectWifiDisplay();

    /**
     * Renames a Wifi display.
     * <p>
     * The display must already be remembered for this call to succeed.  In other words,
     * we must already have successfully connected to the display at least once and then
     * not forgotten it.
     * </p><p>
     * Requires {@link android.Manifest.permission#CONFIGURE_WIFI_DISPLAY}.
     * </p>
     *
     * @param deviceAddress The MAC address of the device to rename.
     * @param alias The alias name by which to remember the device, or NULL
     * or empty if no alias should be used.
     * @hide
     */
    CARAPI RenameWifiDisplay(
        /* [in] */ const String& deviceAddress,
        /* [in] */ const String& alias);

    /**
     * Forgets a previously remembered Wifi display.
     * <p>
     * Automatically disconnects from the display if currently connected to it.
     * </p><p>
     * Requires {@link android.Manifest.permission#CONFIGURE_WIFI_DISPLAY}.
     * </p>
     *
     * @param deviceAddress The MAC address of the device to forget.
     * @hide
     */
    CARAPI ForgetWifiDisplay(
        /* [in] */ const String& deviceAddress);

    /**
     * Gets the current Wifi display status.
     * Watch for changes in the status by registering a broadcast receiver for
     * {@link #ACTION_WIFI_DISPLAY_STATUS_CHANGED}.
     *
     * @return The current Wifi display status.
     * @hide
     */
    CARAPI GetWifiDisplayStatus(
        /* [out] */ IWifiDisplayStatus** status);

private:
    CARAPI_(void) AddMatchingDisplaysLocked(
        /* [in] */ List<AutoPtr<IDisplay> >& displays,
        /* [in] */ ArrayOf<Int32>* displayIds,
        /* [in] */ Int32 matchType);

    CARAPI_(AutoPtr<IDisplay>) GetOrCreateDisplayLocked(
        /* [in] */ Int32 displayId,
        /* [in] */ Boolean assumeValid);

private:
    static const char* TAG;
    static const Boolean DEBUG = FALSE;

    AutoPtr<IContext> mContext;
    AutoPtr<IDisplayManagerGlobal> mGlobal;

    Object mLock;
    HashMap<Int32, AutoPtr<IDisplay> > mDisplays;

    List<AutoPtr<IDisplay> > mTempDisplays;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYMANAGER_H__
