
#ifndef __CURSORCONTROLLER_H__
#define __CURSORCONTROLLER_H__

// import android.view.ViewTreeObserver;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * A CursorController instance can be used to control a cursor in the text.
 */
class CursorController : public IOnTouchModeChangeListener
{
public:
    /**
     * Hide the cursor controller from screen.
     */
    virtual CARAPI Hide();

    /**
     * @return true if the CursorController is currently visible
     */
    virtual CARAPI IsShowing(
        /* [out] */ Boolean* result);

    /**
     * Called when the handle is about to start updating its position.
     * @param handle
     */
    virtual CARAPI BeforeStartUpdatingPosition(
        /* [in] */ HandleView* handle);

    /**
     * Update the controller's position.
     */
    virtual CARAPI UpdatePosition(
        /* [in] */ HandleView* handle,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Called when the view is detached from window. Perform house keeping task, such as
     * stopping Runnable thread that would otherwise keep a reference on the context, thus
     * preventing the activity to be recycled.
     */
    virtual CARAPI OnDetached();
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__CURSORCONTROLLER_H__
