#ifndef __DISPLAYCONTENT_H__
#define __DISPLAYCONTENT_H__

#include "wm/MagnificationSpec.h"
#include "wm/WindowState.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::Mutex;
using Elastos::Utility::Etl::List;
using Elastos::Droid::View::IDisplayInfo;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::Os::IRemoteCallbackList;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class WindowState;

/**
 * Utility class for keeping track of the WindowStates and other pertinent contents of a
 * particular Display.
 *
 * IMPORTANT: No method from this class should ever be used without holding
 * WindowManagerService.mWindowMap.
 */
class DisplayContent
    : public ElRefBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL()

    /**
     * @param display May not be null.
     */
    DisplayContent(
        /* [in] */ IDisplay* display);

    CARAPI_(Int32) GetDisplayId();

    CARAPI_(List< AutoPtr<WindowState> >&) GetWindowList();

    CARAPI_(AutoPtr<IDisplay>) GetDisplay();

    CARAPI_(AutoPtr<IDisplayInfo>) GetDisplayInfo();

    CARAPI_(void) UpdateDisplayInfo();

    // void dump(String prefix, PrintWriter pw);

public:
    // Specification for magnifying the display content.
    AutoPtr<MagnificationSpec> mMagnificationSpec;

    // Callback for observing content changes on a display.
    AutoPtr<IRemoteCallbackList> mDisplayContentChangeListeners;

    // This protects the following display size properties, so that
    // getDisplaySize() doesn't need to acquire the global lock.  This is
    // needed because the window manager sometimes needs to use ActivityThread
    // while it has its global state locked (for example to load animation
    // resources), but the ActivityThread also needs get the current display
    // size sometimes when it has its package lock held.
    //
    // These will only be modified with both mWindowMap and mDisplaySizeLock
    // held (in that order) so the window manager doesn't need to acquire this
    // lock when needing these values in its normal operation.
    Mutex mDisplaySizeLock;
    Int32 mInitialDisplayWidth;
    Int32 mInitialDisplayHeight;
    Int32 mInitialDisplayDensity;
    Int32 mBaseDisplayWidth;
    Int32 mBaseDisplayHeight;
    Int32 mBaseDisplayDensity;

    // Accessed directly by all users.
    Boolean mLayoutNeeded;
    Int32 mPendingLayoutChanges;
    Boolean mIsDefaultDisplay;

private:
    /** Unique identifier of this stack. */
    Int32 mDisplayId;

    /** Z-ordered (bottom-most first) list of all Window objects. Assigned to an element
     * from mDisplayWindows; */
    List< AutoPtr<WindowState> > mWindows;

    AutoPtr<IDisplayInfo> mDisplayInfo;
    AutoPtr<IDisplay> mDisplay;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__DISPLAYCONTENT_H__
