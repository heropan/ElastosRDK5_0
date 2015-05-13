
#ifndef __TOAST_H__
#define __TOAST_H__

#include "ext/frameworkext.h"
#include "os/Runnable.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::App::ITransientNotification;
using Elastos::Droid::App::IINotificationManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IWindowManagerLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

extern "C" const InterfaceID EIID_Toast;

/**
 * A toast is a view containing a quick little message for the user.  The toast class
 * helps you create and show those.
 * {@more}
 *
 * <p>
 * When the view is shown to the user, appears as a floating view over the
 * application.  It will never receive focus.  The user will probably be in the
 * middle of typing something else.  The idea is to be as unobtrusive as
 * possible, while still showing the user the information you want them to see.
 * Two examples are the volume control, and the brief message saying that your
 * settings have been saved.
 * <p>
 * The easiest way to use this class is to call one of the static methods that constructs
 * everything you need and returns a new Toast object.
 */
class Toast
{
public:
    Toast(
        /* [in] */ IContext* context);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI Show();

    CARAPI Cancel();

    CARAPI SetView(
        /* [in] */ IView* view);

    virtual CARAPI GetView(
        /* [out] */ IView** view);

    virtual CARAPI SetDuration(
        /* [in] */ Int32 duration);

    virtual CARAPI GetDuration(
        /* [out] */ Int32* duration);

    virtual CARAPI SetMargin(
        /* [in] */ Float horizontalMargin,
        /* [in] */ Float verticalMargin);

    virtual CARAPI GetHorizontalMargin(
        /* [out] */ Float* horizontalMargin);

    virtual CARAPI GetVerticalMargin(
        /* [out] */ Float* verticalMargin);

    virtual CARAPI SetGravity(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 xOffset,
        /* [in] */ Int32 yOffset);

    virtual CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    virtual CARAPI GetXOffset(
        /* [out] */ Int32* xOffset);

    virtual CARAPI GetYOffset(
        /* [out] */ Int32* yOffset);

    /**
     * Make a standard toast that just contains a text view.
     *
     * @param context  The context to use.  Usually your {@link android.app.Application}
     *                 or {@link android.app.Activity} object.
     * @param text     The text to show.  Can be formatted text.
     * @param duration How long to display the message.  Either {@link #LENGTH_SHORT} or
     *                 {@link #LENGTH_LONG}
     *
     */
    static CARAPI MakeText(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 duration,
        /* [out] */ IToast** toast);

    /**
     * Make a standard toast that just contains a text view with the text from a resource.
     *
     * @param context  The context to use.  Usually your {@link android.app.Application}
     *                 or {@link android.app.Activity} object.
     * @param resId    The resource id of the string resource to use.  Can be formatted text.
     * @param duration How long to display the message.  Either {@link #LENGTH_SHORT} or
     *                 {@link #LENGTH_LONG}
     *
     * @throws Resources.NotFoundException if the resource can't be found.
     */
    static CARAPI MakeTextEx(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resId,
        /* [in] */ Int32 duration,
        /* [out] */ IToast** toast);

    virtual CARAPI SetText(
        /* [in] */ Int32 resId);

    virtual CARAPI SetTextEx(
        /* [in] */ ICharSequence* s);

    virtual CARAPI OnHide();

    virtual CARAPI IsShowing(
        /* [out] */ Boolean* showing);

    static AutoPtr<IINotificationManager> GetService();

protected:
    Toast();

    CARAPI Init(
        /* [in] */ IContext* context);

protected:
    friend class CToastHelper;

    AutoPtr<IContext> mContext;
    AutoPtr<ITransientNotification> mTN;

    Int32 mDuration;
    AutoPtr<IView> mNextView;

private:
    static AutoPtr<IINotificationManager> sService;
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif
