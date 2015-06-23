
#ifndef __ACCESSIBILITYINJECTORFALLBACK_H__
#define __ACCESSIBILITYINJECTORFALLBACK_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include <elastos/Stack.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::Stack;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

/**
 * This class injects accessibility into WebViews with disabled JavaScript or
 * WebViews with enabled JavaScript but for which we have no accessibility
 * script to inject.
 * </p>
 * Note: To avoid changes in the framework upon changing the available
 *       navigation axis, or reordering the navigation axis, or changing
 *       the key bindings, or defining sequence of actions to be bound to
 *       a given key this class is navigation axis agnostic. It is only
 *       aware of one navigation axis which is in fact the default behavior
 *       of webViews while using the DPAD/TrackBall.
 * </p>
 * In general a key binding is a mapping from modifiers + key code to
 * a sequence of actions. For more detail how to specify key bindings refer to
 * {@link android.provider.Settings.Secure#ACCESSIBILITY_WEB_CONTENT_KEY_BINDINGS}.
 * </p>
 * The possible actions are invocations to
 * {@link #setCurrentAxis(int, boolean, String)}, or
 * {@link #traverseCurrentAxis(int, boolean, String)}
 * {@link #traverseGivenAxis(int, int, boolean, String)}
 * {@link #performAxisTransition(int, int, boolean, String)}
 * referred via the values of:
 * {@link #ACTION_SET_CURRENT_AXIS},
 * {@link #ACTION_TRAVERSE_CURRENT_AXIS},
 * {@link #ACTION_TRAVERSE_GIVEN_AXIS},
 * {@link #ACTION_PERFORM_AXIS_TRANSITION},
 * respectively.
 * The arguments for the action invocation are specified as offset
 * hexademical pairs. Note the last argument of the invocation
 * should NOT be specified in the binding as it is provided by
 * this class. For details about the key binding implementation
 * refer to {@link AccessibilityWebContentKeyBinding}.
 */
class AccessibilityInjectorFallback : public ElRefBase
{
private:
    /**
     * Represents a web content key-binding.
     */
    class AccessibilityWebContentKeyBinding : public ElRefBase
    {
    public:
        /**
         * Creates a new instance.
         * @param keyCodeAndModifiers The key for the binding (key and modifiers).
         * @param actionSequence The sequence of action for the binding.
         */
        AccessibilityWebContentKeyBinding(
            /* [in] */ Int64 keyCodeAndModifiers,
            /* [in] */ ArrayOf<Int32>* actionSequence);

        /**
         * @return The key code of the binding key.
         */
        CARAPI_(Int32) GetKeyCode();

        /**
         * @return The meta state of the binding key.
         */
        CARAPI_(Int32) GetModifiers();

        /**
         * @return The number of actions in the key binding.
         */
        CARAPI_(Int32) GetActionCount();

        /**
         * @param index The action for a given action <code>index</code>.
         */
        CARAPI_(Int32) GetAction(
            /* [in] */ Int32 index);

        /**
         * @param index The action code for a given action <code>index</code>.
         */
        CARAPI_(Int32) GetActionCode(
            /* [in] */ Int32 index);

        /**
         * @param index The first argument for a given action <code>index</code>.
         */
        CARAPI_(Int32) GetFirstArgument(
            /* [in] */ Int32 index);

        /**
         * @param index The second argument for a given action <code>index</code>.
         */
        CARAPI_(Int32) GetSecondArgument(
            /* [in] */ Int32 index);

        /**
         * @param index The third argument for a given action <code>index</code>.
         */
        CARAPI_(Int32) GetThirdArgument(
            /* [in] */ Int32 index);

        CARAPI_(String) ToString();

    private:
        static const Int32 MODIFIERS_OFFSET = 32;
        static const Int64 MODIFIERS_MASK = 0xFFFFFFF00000000ll;

        static const Int32 KEY_CODE_OFFSET = 0;
        static const Int64 KEY_CODE_MASK = 0x00000000FFFFFFFFll;

        static const Int32 ACTION_OFFSET = 24;
        static const Int32 ACTION_MASK = 0xFF000000;

        static const Int32 FIRST_ARGUMENT_OFFSET = 16;
        static const Int32 FIRST_ARGUMENT_MASK = 0x00FF0000;

        static const Int32 SECOND_ARGUMENT_OFFSET = 8;
        static const Int32 SECOND_ARGUMENT_MASK = 0x0000FF00;

        static const Int32 THIRD_ARGUMENT_OFFSET = 0;
        static const Int32 THIRD_ARGUMENT_MASK = 0x000000FF;

        Int64 mKeyCodeAndModifiers;

        AutoPtr< ArrayOf<Int32> > mActionSequence;
    };

public:
    /**
     * Creates a new injector associated with a given {@link WebViewClassic}.
     *
     * @param webView The associated WebViewClassic.
     */
    AccessibilityInjectorFallback(
        /* [in] */ CWebViewClassic* webView);

    /**
     * Processes a key down <code>event</code>.
     *
     * @return True if the event was processed.
     */
    virtual CARAPI_(Boolean) OnKeyEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    /**
     * Called when the <code>selectionString</code> has changed.
     */
    virtual CARAPI_(void) OnSelectionStringChange(
        /* [in] */ const String& selectionString);

private:
    /**
     * Set the current navigation axis which will be used while
     * calling {@link #traverseCurrentAxis(int, boolean, String)}.
     *
     * @param axis The axis to set.
     * @param sendEvent Whether to send an accessibility event to
     *        announce the change.
     */
    CARAPI_(void) SetCurrentAxis(
        /* [in] */ Int32 axis,
        /* [in] */ Boolean sendEvent,
        /* [in] */ const String& contentDescription);

    /**
     * Performs conditional transition one axis to another.
     *
     * @param fromAxis The axis which must be the current for the transition to occur.
     * @param toAxis The axis to which to transition.
     * @param sendEvent Flag if to send an event to announce successful transition.
     * @param contentDescription A description of the performed action.
     */
    CARAPI_(void) PerformAxisTransition(
        /* [in] */ Int32 fromAxis,
        /* [in] */ Int32 toAxis,
        /* [in] */ Boolean sendEvent,
        /* [in] */ const String& contentDescription);

    /**
     * Traverse the document along the current navigation axis.
     *
     * @param direction The direction of traversal.
     * @param sendEvent Whether to send an accessibility event to
     *        announce the change.
     * @param contentDescription A description of the performed action.
     * @see #setCurrentAxis(int, boolean, String)
     */
    CARAPI_(Boolean) TraverseCurrentAxis(
        /* [in] */ Int32 direction,
        /* [in] */ Boolean sendEvent,
        /* [in] */ const String& contentDescription);

    /**
     * Returns the {@link WebView}-defined direction for the given
     * {@link AccessibilityNodeInfo}-defined action.
     *
     * @param action An accessibility action identifier.
     * @return A web view navigation direction.
     */
    static CARAPI_(Int32) GetDirectionForAction(
        /* [in] */ Int32 action);

    /**
     * Returns the {@link WebView}-defined axis for the given
     * {@link AccessibilityNodeInfo}-defined granularity.
     *
     * @param granularity An accessibility granularity identifier.
     * @return A web view navigation axis.
     */
    static CARAPI_(Int32) GetAxisForGranularity(
        /* [in] */ Int32 granularity);

    /**
     * Traverse the document along the given navigation axis.
     *
     * @param direction The direction of traversal.
     * @param axis The axis along which to traverse.
     * @param sendEvent Whether to send an accessibility event to
     *        announce the change.
     * @param contentDescription A description of the performed action.
     */
    CARAPI_(Boolean) TraverseGivenAxis(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 axis,
        /* [in] */ Boolean sendEvent,
        /* [in] */ const String& contentDescription);

    /**
     * Sends an {@link AccessibilityEvent}.
     *
     * @param event The event to send.
     */
    CARAPI_(void) SendAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * @return An accessibility event whose members are populated except its
     *         text and content description.
     */
    CARAPI_(AutoPtr<IAccessibilityEvent>) GetPartialyPopulatedAccessibilityEvent(
        /* [in] */ Int32 eventType);

    /**
     * Ensures that the Web content key bindings are loaded.
     */
    CARAPI_(void) EnsureWebContentKeyBindings();

    CARAPI_(Boolean) IsEnterActionKey(
        /* [in] */ Int32 keyCode);

private:
    static const String _LOG_TAG;

    static const Boolean DEBUG = TRUE;

    static const Int32 ACTION_SET_CURRENT_AXIS = 0;
    static const Int32 ACTION_TRAVERSE_CURRENT_AXIS = 1;
    static const Int32 ACTION_TRAVERSE_GIVEN_AXIS = 2;
    static const Int32 ACTION_PERFORM_AXIS_TRANSITION = 3;
    static const Int32 ACTION_TRAVERSE_DEFAULT_WEB_VIEW_BEHAVIOR_AXIS = 4;

    // WebView navigation axes from WebViewCore.h, plus an additional axis for
    // the default behavior.
    static const Int32 NAVIGATION_AXIS_CHARACTER = 0;
    static const Int32 NAVIGATION_AXIS_WORD = 1;
    static const Int32 NAVIGATION_AXIS_SENTENCE = 2;
    //@SuppressWarnings("unused")
    static const Int32 NAVIGATION_AXIS_HEADING = 3;
    static const Int32 NAVIGATION_AXIS_SIBLING = 5;
    //@SuppressWarnings("unused")
    static const Int32 NAVIGATION_AXIS_PARENT_FIRST_CHILD = 5;
    static const Int32 NAVIGATION_AXIS_DOCUMENT = 6;
    static const Int32 NAVIGATION_AXIS_DEFAULT_WEB_VIEW_BEHAVIOR = 7;

    // WebView navigation directions from WebViewCore.h.
    static const Int32 NAVIGATION_DIRECTION_BACKWARD = 0;
    static const Int32 NAVIGATION_DIRECTION_FORWARD = 1;

    // these are the same for all instances so make them process wide
    static List< AutoPtr<AccessibilityWebContentKeyBinding> > sBindings;

    // handle to the WebViewClassic this injector is associated with.
    AutoPtr<CWebViewClassic> mWebView;
    AutoPtr<IWebView> mWebViewInternal;

    // events scheduled for sending as soon as we receive the selected text
    Stack< AutoPtr<IAccessibilityEvent> > mScheduledEventStack;

    // the current traversal axis
    Int32 mCurrentAxis; // sentence

    // we need to consume the up if we have handled the last down
    Boolean mLastDownEventHandled;

    // getting two empty selection strings in a row we let the WebView handle the event
    Boolean mIsLastSelectionStringNull;

    // keep track of last direction
    Int32 mLastDirection;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ACCESSIBILITYINJECTORFALLBACK_H__
