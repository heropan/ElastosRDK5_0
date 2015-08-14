
#ifndef __ELASTOS_DROID_INPUTMETHODSERVICE_CSOFTINPUTWINDOW_H__
#define  __ELASTOS_DROID_INPUTMETHODSERVICE_CSOFTINPUTWINDOW_H__

#include "_CSoftInputWindow.h"
#include "app/Dialog.h"




using Elastos::Droid::App::Dialog;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace InputMethodService {


class SoftInputWindow : public Dialog
{
public:
    SoftInputWindow();

    CARAPI SetToken(
        /* [in] */ IBinder* token);

    // @Override
    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasFocus);

    CARAPI_(Boolean) DispatchTouchEvent(
        /* [in] */ IMotionEvent* ev);

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

protected:
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
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ IDispatcherState* dispatcherState);

private:
    CARAPI_(void) InitDockWindow();

protected:
    AutoPtr<IDispatcherState> mDispatcherState;
    AutoPtr<IRect> mBounds;
};

/**
 * A SoftInputWindow is a Dialog that is intended to be used for a top-level input
 * method window.  It will be displayed along the edge of the screen, moving
 * the application user interface away from it so that the focused item is
 * always visible.
 */
CarClass(CSoftInputWindow), public SoftInputWindow
{
public:
    IDIALOG_METHODS_DECL();
    IWINDOWCALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ IDispatcherState* dispatcherState);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetToken(
        /* [in] */ IBinder* token);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI SetSize(
        /* [in] */ Int32 size);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__SOFTINPUTWINDOW_H__
