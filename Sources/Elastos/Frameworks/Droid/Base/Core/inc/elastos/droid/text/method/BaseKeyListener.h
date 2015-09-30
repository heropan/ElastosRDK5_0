#ifndef __ELASTOS_DROID_TEXT_METHOD_BaseKeyListener_H__
#define __ELASTOS_DROID_TEXT_METHOD_BaseKeyListener_H__

#include "elastos/droid/text/method/MetaKeyKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/**
 * Abstract base class for key listeners.
 *
 * Provides a basic foundation for entering and editing text.
 * Subclasses should override {@link #onKeyDown} and {@link #onKeyUp} to insert
 * characters as keys are pressed.
 * <p></p>
 * As for all implementations of {@link KeyListener}, this class is only concerned
 * with hardware keyboards.  Software input methods have no obligation to trigger
 * the methods in this class.
 */
class BaseKeyListener
    : public MetaKeyKeyListener
{
public:
    /**
     * Performs the action that happens when you press the {@link KeyEvent#KEYCODE_DEL} key in
     * a {@link TextView}.  If there is a selection, deletes the selection; otherwise,
     * deletes the character before the cursor, if any; ALT+DEL deletes everything on
     * the line the cursor is on.
     *
     * @return true if anything was deleted; false otherwise.
     */
    CARAPI_(Boolean) Backspace(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * Performs the action that happens when you press the {@link KeyEvent#KEYCODE_FORWARD_DEL}
     * key in a {@link TextView}.  If there is a selection, deletes the selection; otherwise,
     * deletes the character before the cursor, if any; ALT+FORWARD_DEL deletes everything on
     * the line the cursor is on.
     *
     * @return true if anything was deleted; false otherwise.
     */
    CARAPI_(Boolean) ForwardDelete(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(Int32) MakeTextContentType(
        /* [in] */ Capitalize caps,
        /* [in] */ Boolean autoText);

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * Base implementation handles ACTION_MULTIPLE KEYCODE_UNKNOWN by inserting
     * the event's text into the content.
     */
    CARAPI_(Boolean) OnKeyOther(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ IKeyEvent* event);

private:
    CARAPI_(Boolean) BackspaceOrForwardDelete(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Boolean isForwardDelete);

    CARAPI_(Boolean) DeleteSelection(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content);

    CARAPI_(Boolean) DeleteLine(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content);

public:
    /* package */
    static const AutoPtr<IInterface> OLD_SEL_START;// = new NoCopySpan.Concrete();

};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_BaseKeyListener_H__
