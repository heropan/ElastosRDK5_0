
#ifndef __ELASTOS_DROID_INPUTMETHODSERVICE_CSOFTINPUTWINDOW_H__
#define  __ELASTOS_DROID_INPUTMETHODSERVICE_CSOFTINPUTWINDOW_H__

#include "_Elastos_Droid_InputMethodService_CSoftInputWindow.h"
// #include "app/Dialog.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;
// using Elastos::Droid::App::Dialog;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

/**
 * A SoftInputWindow is a Dialog that is intended to be used for a top-level input
 * method window.  It will be displayed along the edge of the screen, moving
 * the application user interface away from it so that the focused item is
 * always visible.
 */
CarClass(CSoftInputWindow)
    , public /*Dialog*/Object
    , public ISoftInputWindow
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CSoftInputWindow();

    CARAPI SetToken(
        /* [in] */ IBinder* token);

    // @Override
    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    /**
     * Get the size of the DockWindow.
     *
     * @return If the DockWindow sticks to the top or bottom of the screen, the
     *         return value is the height of the DockWindow, and its width is
     *         equal to the width of the screen; If the DockWindow sticks to the
     *         left or right of the screen, the return value is the width of the
     *         DockWindow, and its height is equal to the height of the screen.
     */
    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Set the size of the DockWindow.
     *
     * @param size If the DockWindow sticks to the top or bottom of the screen,
     *        <var>size</var> is the height of the DockWindow, and its width is
     *        equal to the width of the screen; If the DockWindow sticks to the
     *        left or right of the screen, <var>size</var> is the width of the
     *        DockWindow, and its height is equal to the height of the screen.
     */
    CARAPI SetSize(
        /* [in] */ Int32 size);

    /**
     * Set which boundary of the screen the DockWindow sticks to.
     *
     * @param gravity The boundary of the screen to stick. See {#link
     *        android.view.Gravity.LEFT}, {#link android.view.Gravity.TOP},
     *        {#link android.view.Gravity.BOTTOM}, {#link
     *        android.view.Gravity.RIGHT}.
     */
    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    /**
     * Create a DockWindow that uses a custom style.
     *
     * @param context The Context in which the DockWindow should run. In
     *        particular, it uses the window manager and theme from this context
     *        to present its UI.
     * @param theme A style resource describing the theme to use for the window.
     *        See <a href="{@docRoot}reference/available-resources.html#stylesandthemes">Style
     *        and Theme Resources</a> for more information about defining and
     *        using styles. This theme is applied on top of the current theme in
     *        <var>context</var>. If 0, the default dialog theme will be used.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ IDispatcherState* dispatcherState);

private:
    CARAPI_(void) InitDockWindow();

private:
    AutoPtr<IDispatcherState> mDispatcherState;
    AutoPtr<IRect> mBounds;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__SOFTINPUTWINDOW_H__
