
#include <elastos/core/StringUtils.h>
#include "os/CMessage.h"
#include "provider/CSettingsSecure.h"
#include "text/CSimpleStringSplitter.h"
#include "text/TextUtils.h"
#include "view/accessibility/CAccessibilityEventHelper.h"
#include "view/accessibility/CAccessibilityManagerHelper.h"
#include "webkit/AccessibilityInjectorFallback.h"
#include "webkit/CWebViewCore.h"
#include "webkit/CWebViewClassic.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Os::CMessage;
using Elastos::Droid::Provider::CSettingsSecure;
using Elastos::Droid::Provider::ISettingsSecure;
using Elastos::Droid::Text::CSimpleStringSplitter;
using Elastos::Droid::Text::ISimpleStringSplitter;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::Accessibility::CAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManagerHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityManagerHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {

//==================================================================
//AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding
//==================================================================
const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::MODIFIERS_OFFSET;
const Int64 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::MODIFIERS_MASK;

const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::KEY_CODE_OFFSET;
const Int64 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::KEY_CODE_MASK;

const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::ACTION_OFFSET;
const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::ACTION_MASK;

const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::FIRST_ARGUMENT_OFFSET;
const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::FIRST_ARGUMENT_MASK;

const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::SECOND_ARGUMENT_OFFSET;
const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::SECOND_ARGUMENT_MASK;

const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::THIRD_ARGUMENT_OFFSET;
const Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::THIRD_ARGUMENT_MASK;

/**
 * Creates a new instance.
 * @param keyCodeAndModifiers The key for the binding (key and modifiers).
 * @param actionSequence The sequence of action for the binding.
 */
AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::AccessibilityWebContentKeyBinding(
    /* [in] */ Int64 keyCodeAndModifiers,
    /* [in] */ ArrayOf<Int32>* actionSequence)
    : mKeyCodeAndModifiers(keyCodeAndModifiers)
    , mActionSequence(actionSequence)
{
}

/**
 * @return The key code of the binding key.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetKeyCode()
{
    return (Int32) ((mKeyCodeAndModifiers & KEY_CODE_MASK) >> KEY_CODE_OFFSET);
}

/**
 * @return The meta state of the binding key.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetModifiers()
{
    return (Int32) ((mKeyCodeAndModifiers & MODIFIERS_MASK) >> MODIFIERS_OFFSET);
}

/**
 * @return The number of actions in the key binding.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetActionCount()
{
    return mActionSequence->GetLength();
}

/**
 * @param index The action for a given action <code>index</code>.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetAction(
    /* [in] */ Int32 index)
{
    return (*mActionSequence)[index];
}

/**
 * @param index The action code for a given action <code>index</code>.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetActionCode(
    /* [in] */ Int32 index)
{
    return ((*mActionSequence)[index] & ACTION_MASK) >> ACTION_OFFSET;
}

/**
 * @param index The first argument for a given action <code>index</code>.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetFirstArgument(
    /* [in] */ Int32 index)
{
    return ((*mActionSequence)[index] & FIRST_ARGUMENT_MASK) >> FIRST_ARGUMENT_OFFSET;
}

/**
 * @param index The second argument for a given action <code>index</code>.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetSecondArgument(
    /* [in] */ Int32 index)
{
    return ((*mActionSequence)[index] & SECOND_ARGUMENT_MASK) >> SECOND_ARGUMENT_OFFSET;
}

/**
 * @param index The third argument for a given action <code>index</code>.
 */
Int32 AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::GetThirdArgument(
    /* [in] */ Int32 index)
{
    return ((*mActionSequence)[index] & THIRD_ARGUMENT_MASK) >> THIRD_ARGUMENT_OFFSET;
}

String AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding::ToString()
{
    StringBuilder builder;
    builder += "modifiers: ";
    builder += GetModifiers();
    builder += ", keyCode: ";
    builder += GetKeyCode();
    builder += ", actions[";
    for (Int32 i = 0, count = GetActionCount(); i < count; i++) {
        builder += "{actionCode";
        builder += i;
        builder += ": ";
        builder += GetActionCode(i);
        builder += ", firstArgument: ";
        builder += GetFirstArgument(i);
        builder += ", secondArgument: ";
        builder += GetSecondArgument(i);
        builder += ", thirdArgument: ";
        builder += GetThirdArgument(i);
        builder += "}";
    }
    builder += "]";
    return builder.ToString();
}

//==================================================================
//                     AccessibilityInjectorFallback
//==================================================================
const String AccessibilityInjectorFallback::_LOG_TAG("AccessibilityInjector");

List< AutoPtr<AccessibilityInjectorFallback::AccessibilityWebContentKeyBinding> > AccessibilityInjectorFallback::sBindings;

const Boolean AccessibilityInjectorFallback::DEBUG;

const Int32 AccessibilityInjectorFallback::ACTION_SET_CURRENT_AXIS;
const Int32 AccessibilityInjectorFallback::ACTION_TRAVERSE_CURRENT_AXIS;
const Int32 AccessibilityInjectorFallback::ACTION_TRAVERSE_GIVEN_AXIS;
const Int32 AccessibilityInjectorFallback::ACTION_PERFORM_AXIS_TRANSITION;
const Int32 AccessibilityInjectorFallback::ACTION_TRAVERSE_DEFAULT_WEB_VIEW_BEHAVIOR_AXIS;

// WebView navigation axes from WebViewCore.h, plus an additional axis for
// the default behavior.
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_CHARACTER;
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_WORD;
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_SENTENCE;
//@SuppressWarnings("unused")
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_HEADING;
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_SIBLING;
//@SuppressWarnings("unused")
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_PARENT_FIRST_CHILD;
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_DOCUMENT;
const Int32 AccessibilityInjectorFallback::NAVIGATION_AXIS_DEFAULT_WEB_VIEW_BEHAVIOR;

// WebView navigation directions from WebViewCore.h.
const Int32 AccessibilityInjectorFallback::NAVIGATION_DIRECTION_BACKWARD;
const Int32 AccessibilityInjectorFallback::NAVIGATION_DIRECTION_FORWARD;

/*
// these are the same for all instances so make them process wide
ArrayList<AccessibilityWebContentKeyBinding> AccessibilityInjectorFallback::sBindings =
    new ArrayList<AccessibilityWebContentKeyBinding>();*/

/**
 * Creates a new injector associated with a given {@link WebViewClassic}.
 *
 * @param webView The associated WebViewClassic.
 */
AccessibilityInjectorFallback::AccessibilityInjectorFallback(
    /* [in] */ CWebViewClassic* webView)
    : mCurrentAxis(2)
    , mLastDownEventHandled(FALSE)
    , mIsLastSelectionStringNull(FALSE)
    , mLastDirection(0)
{
    mWebView = webView;
    mWebView->GetWebView((IWebView**)&mWebViewInternal);
    EnsureWebContentKeyBindings();
}

/**
 * Processes a key down <code>event</code>.
 *
 * @return True if the event was processed.
 */
Boolean AccessibilityInjectorFallback::OnKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // We do not handle ENTER in any circumstances.
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (IsEnterActionKey(keyCode)) {
        return FALSE;
    }

    Int32 action;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_UP) {
        return mLastDownEventHandled;
    }

    mLastDownEventHandled = FALSE;

    AutoPtr<AccessibilityWebContentKeyBinding> binding;
    List< AutoPtr<AccessibilityWebContentKeyBinding> >::Iterator iter = sBindings.Begin();
    for (; iter != sBindings.End(); ++iter) {
        AutoPtr<AccessibilityWebContentKeyBinding> candidate = *iter;
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        Boolean hasModifiers = FALSE;
        event->HasModifiers(candidate->GetModifiers(), &hasModifiers);
        if (keyCode == candidate->GetKeyCode()
                && hasModifiers) {
            binding = candidate;
            break;
        }
    }

    if (binding == NULL) {
        return FALSE;
    }

    for (Int32 i = 0, count = binding->GetActionCount(); i < count; i++) {
        Int32 actionCode = binding->GetActionCode(i);
        String contentDescription = StringUtils::Int32ToHexString(binding->GetAction(i));
        Boolean sendEvent = FALSE;

        switch (actionCode) {
            case ACTION_SET_CURRENT_AXIS: {
                Int32 axis = binding->GetFirstArgument(i);
                sendEvent = (binding->GetSecondArgument(i) == 1);
                SetCurrentAxis(axis, sendEvent, contentDescription);
                mLastDownEventHandled = TRUE;
                break;
            }
            case ACTION_TRAVERSE_CURRENT_AXIS: {
                Int32 direction = binding->GetFirstArgument(i);
                // on second null selection string in same direction - WebView handles the event
                if (direction == mLastDirection && mIsLastSelectionStringNull) {
                    mIsLastSelectionStringNull = FALSE;
                    return FALSE;
                }
                mLastDirection = direction;
                sendEvent = (binding->GetSecondArgument(i) == 1);
                mLastDownEventHandled = TraverseCurrentAxis(direction, sendEvent,
                        contentDescription);
                break;
            }
            case ACTION_TRAVERSE_GIVEN_AXIS: {
                Int32 direction = binding->GetFirstArgument(i);
                // on second null selection string in same direction => WebView handle the event
                if (direction == mLastDirection && mIsLastSelectionStringNull) {
                    mIsLastSelectionStringNull = FALSE;
                    return FALSE;
                }
                mLastDirection = direction;
                Int32 axis =  binding->GetSecondArgument(i);
                sendEvent = (binding->GetThirdArgument(i) == 1);
                TraverseGivenAxis(direction, axis, sendEvent, contentDescription);
                mLastDownEventHandled = TRUE;
                break;
            }
            case ACTION_PERFORM_AXIS_TRANSITION: {
                Int32 fromAxis = binding->GetFirstArgument(i);
                Int32 toAxis = binding->GetSecondArgument(i);
                sendEvent = (binding->GetThirdArgument(i) == 1);
                PerformAxisTransition(fromAxis, toAxis, sendEvent, contentDescription);
                mLastDownEventHandled = TRUE;
                break;
            }
            case ACTION_TRAVERSE_DEFAULT_WEB_VIEW_BEHAVIOR_AXIS: {
                // This is a special case since we treat the default WebView navigation
                // behavior as one of the possible navigation axis the user can use.
                // If we are not on the default WebView navigation axis this is NOP.
                if (mCurrentAxis == NAVIGATION_AXIS_DEFAULT_WEB_VIEW_BEHAVIOR) {
                    // While WebVew handles navigation we do not get null selection
                    // strings so do not check for that here as the cases above.
                    mLastDirection = binding->GetFirstArgument(i);
                    sendEvent = (binding->GetSecondArgument(i) == 1);
                    TraverseGivenAxis(mLastDirection, NAVIGATION_AXIS_DEFAULT_WEB_VIEW_BEHAVIOR,
                        sendEvent, contentDescription);
                    mLastDownEventHandled = FALSE;
                }
                else {
                    mLastDownEventHandled = TRUE;
                }
                break;
            }

//            default:
//                Log.w(LOG_TAG, "Unknown action code: " + actionCode);
        }
    }

    return mLastDownEventHandled;
}

/**
 * Set the current navigation axis which will be used while
 * calling {@link #traverseCurrentAxis(int, boolean, String)}.
 *
 * @param axis The axis to set.
 * @param sendEvent Whether to send an accessibility event to
 *        announce the change.
 */
void AccessibilityInjectorFallback::SetCurrentAxis(
    /* [in] */ Int32 axis,
    /* [in] */ Boolean sendEvent,
    /* [in] */ const String& contentDescription)
{
    mCurrentAxis = axis;
    if (sendEvent) {
        AutoPtr<IAccessibilityEvent> event = GetPartialyPopulatedAccessibilityEvent(
                IAccessibilityEvent::TYPE_ANNOUNCEMENT);
        AutoPtr<IObjectContainer> container;
        event->GetText((IObjectContainer**)&container);
        AutoPtr<ICharSequence> csAxis;
        CStringWrapper::New(StringUtils::Int32ToString(axis), (ICharSequence**)&csAxis);
        container->Add(csAxis);
        event->SetContentDescription(StringBuilder(contentDescription).ToCharSequence());
        SendAccessibilityEvent(event);
    }
}

/**
 * Performs conditional transition one axis to another.
 *
 * @param fromAxis The axis which must be the current for the transition to occur.
 * @param toAxis The axis to which to transition.
 * @param sendEvent Flag if to send an event to announce successful transition.
 * @param contentDescription A description of the performed action.
 */
void AccessibilityInjectorFallback::PerformAxisTransition(
    /* [in] */ Int32 fromAxis,
    /* [in] */ Int32 toAxis,
    /* [in] */ Boolean sendEvent,
    /* [in] */ const String& contentDescription)
{
    if (mCurrentAxis == fromAxis) {
        SetCurrentAxis(toAxis, sendEvent, contentDescription);
    }
}

/**
 * Traverse the document along the current navigation axis.
 *
 * @param direction The direction of traversal.
 * @param sendEvent Whether to send an accessibility event to
 *        announce the change.
 * @param contentDescription A description of the performed action.
 * @see #setCurrentAxis(int, boolean, String)
 */
Boolean AccessibilityInjectorFallback::TraverseCurrentAxis(
    /* [in] */ Int32 direction,
    /* [in] */ Boolean sendEvent,
    /* [in] */ const String& contentDescription)
{
    return TraverseGivenAxis(direction, mCurrentAxis, sendEvent, contentDescription);
}

Boolean AccessibilityInjectorFallback::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY:
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY: {
            Int32 direction = GetDirectionForAction(action);
            Int32 args;
            arguments->GetInt32(
                    IAccessibilityNodeInfo::ACTION_ARGUMENT_MOVEMENT_GRANULARITY_INT, &args);
            Int32 axis = GetAxisForGranularity(args);
            return TraverseGivenAxis(direction, axis, TRUE, String(NULL));
        }
        case IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT:
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT: {
            Int32 direction = GetDirectionForAction(action);
            // TODO: Add support for moving by object.
            Int32 axis = NAVIGATION_AXIS_SENTENCE;
            return TraverseGivenAxis(direction, axis, TRUE, String(NULL));
        }
        default:
            return FALSE;
    }
}

/**
 * Returns the {@link WebView}-defined direction for the given
 * {@link AccessibilityNodeInfo}-defined action.
 *
 * @param action An accessibility action identifier.
 * @return A web view navigation direction.
 */
Int32 AccessibilityInjectorFallback::GetDirectionForAction(
    /* [in] */ Int32 action)
{
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_NEXT_HTML_ELEMENT:
        case IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY:
            return NAVIGATION_DIRECTION_FORWARD;
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_HTML_ELEMENT:
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY:
            return NAVIGATION_DIRECTION_BACKWARD;
        default:
            return -1;
    }
}

/**
 * Returns the {@link WebView}-defined axis for the given
 * {@link AccessibilityNodeInfo}-defined granularity.
 *
 * @param granularity An accessibility granularity identifier.
 * @return A web view navigation axis.
 */
Int32 AccessibilityInjectorFallback::GetAxisForGranularity(
    /* [in] */ Int32 granularity)
{
    switch (granularity) {
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_CHARACTER:
            return NAVIGATION_AXIS_CHARACTER;
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_WORD:
            return NAVIGATION_AXIS_WORD;
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_LINE:
            return NAVIGATION_AXIS_SENTENCE;
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_PARAGRAPH:
            // TODO: This should map to object once we implement it.
            return NAVIGATION_AXIS_SENTENCE;
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_PAGE:
            return NAVIGATION_AXIS_DOCUMENT;
        default:
            return -1;
    }
}

/**
 * Traverse the document along the given navigation axis.
 *
 * @param direction The direction of traversal.
 * @param axis The axis along which to traverse.
 * @param sendEvent Whether to send an accessibility event to
 *        announce the change.
 * @param contentDescription A description of the performed action.
 */
Boolean AccessibilityInjectorFallback::TraverseGivenAxis(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 axis,
    /* [in] */ Boolean sendEvent,
    /* [in] */ const String& contentDescription)
{
    AutoPtr<IWebViewCore> webViewCore;// = mWebView->GetWebViewCore();
    if (webViewCore == NULL) {
        return FALSE;
    }

    AutoPtr<IAccessibilityEvent> event;
    if (sendEvent) {
        event = GetPartialyPopulatedAccessibilityEvent(
                IAccessibilityEvent::TYPE_VIEW_TEXT_TRAVERSED_AT_MOVEMENT_GRANULARITY);
        // the text will be set upon receiving the selection string
        event->SetContentDescription(StringBuilder(contentDescription).ToCharSequence());
    }
    mScheduledEventStack.Push(event);

    // if the axis is the default let WebView handle the event which will
    // result in cursor ring movement and selection of its content
    if (axis == NAVIGATION_AXIS_DEFAULT_WEB_VIEW_BEHAVIOR) {
        return FALSE;
    }

    AutoPtr<IMessage> msg;
    CMessage::New((IMessage**)&msg);
    msg->SetWhat(CWebViewCore::EventHub::MODIFY_SELECTION);
    msg->SetArg1(direction);
    msg->SetArg2(axis);
    webViewCore->SendMessage(msg);
    return TRUE;
}

/**
 * Called when the <code>selectionString</code> has changed.
 */
void AccessibilityInjectorFallback::OnSelectionStringChange(
    /* [in] */ const String& selectionString)
{
//    if (DEBUG) {
//        Log.d(LOG_TAG, "Selection string: " + selectionString);
//    }
    mIsLastSelectionStringNull = (selectionString.IsNull());
    if (mScheduledEventStack.IsEmpty()) {
        return;
    }

    AutoPtr<IAccessibilityEvent> event = mScheduledEventStack.GetTop();
    mScheduledEventStack.Pop();
    if ((event != NULL) && !selectionString.IsNullOrEmpty()) {
        AutoPtr<IObjectContainer> container;
        event->GetText((IObjectContainer**)&container);
        AutoPtr<ICharSequence> csSelectionString;
        CStringWrapper::New(selectionString, (ICharSequence**)&csSelectionString);
        container->Add(csSelectionString);
        event->SetFromIndex(0);
        event->SetToIndex(selectionString.GetLength());
        SendAccessibilityEvent(event);
    }
}

/**
 * Sends an {@link AccessibilityEvent}.
 *
 * @param event The event to send.
 */
void AccessibilityInjectorFallback::SendAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
//    if (DEBUG) {
//        Log.d(LOG_TAG, "Dispatching: " + event);
//    }
    // accessibility may be disabled while waiting for the selection string
    AutoPtr<IAccessibilityManagerHelper> accessibilityManagerHelper;
    AutoPtr<IAccessibilityManager> accessibilityManager;
    AutoPtr<IContext> context;

    CAccessibilityManagerHelper::AcquireSingleton((IAccessibilityManagerHelper**)&accessibilityManagerHelper);
    context = mWebView->GetContext();
    accessibilityManagerHelper->GetInstance(context, (IAccessibilityManager**)&accessibilityManager);
    Boolean enable = FALSE;
    if (accessibilityManager->IsEnabled(&enable), enable) {
        accessibilityManager->SendAccessibilityEvent(event);
    }
}

/**
 * @return An accessibility event whose members are populated except its
 *         text and content description.
 */
AutoPtr<IAccessibilityEvent> AccessibilityInjectorFallback::GetPartialyPopulatedAccessibilityEvent(
    /* [in] */ Int32 eventType)
{
    AutoPtr<IAccessibilityEventHelper> eventHelper;
    CAccessibilityEventHelper::AcquireSingleton((IAccessibilityEventHelper**)&eventHelper);
    AutoPtr<IAccessibilityEvent> event;
    eventHelper->Obtain(eventType, (IAccessibilityEvent**)&event);
    mWebViewInternal->OnInitializeAccessibilityEvent(event);
    return event;
}

/**
 * Ensures that the Web content key bindings are loaded.
 */
void AccessibilityInjectorFallback::EnsureWebContentKeyBindings()
{
    if (sBindings.IsEmpty() == FALSE) {
        return;
    }

    String webContentKeyBindingsString;
    AutoPtr<IContext> context = mWebView->GetContext();
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsSecure> settingsSecure;
    CSettingsSecure::AcquireSingleton((ISettingsSecure**)&settingsSecure);
    settingsSecure->GetString(cr,
            ISettingsSecure::ACCESSIBILITY_WEB_CONTENT_KEY_BINDINGS, &webContentKeyBindingsString);

    AutoPtr<ISimpleStringSplitter> semiColonSplitter;
    CSimpleStringSplitter::New(';', (ISimpleStringSplitter**)&semiColonSplitter);
    semiColonSplitter->SetString(webContentKeyBindingsString);

    Boolean bNext = FALSE;
    while (semiColonSplitter->HasNext(&bNext), bNext) {
        AutoPtr<ICharSequence> bindingStringCS;
        semiColonSplitter->Next((IInterface**)&bindingStringCS);
        if (TextUtils::IsEmpty(bindingStringCS)) {
            //Log.e(LOG_TAG, "Disregarding malformed Web content key binding: "
            //       + webContentKeyBindingsString);
            continue;
        }
        String bindingString;
        bindingStringCS->ToString(&bindingString);
        AutoPtr< ArrayOf<String> > keyValueArray;// = bindingString.split("=");
        if (keyValueArray->GetLength() != 2) {
            //Log.e(LOG_TAG, "Disregarding malformed Web content key binding: " + bindingString);
            continue;
        }
        //try {
        assert(0);
//            long keyCodeAndModifiers = Long.decode(keyValueArray[0].trim());
//            String[] actionStrings = keyValueArray[1].split(":");
//            int[] actions = new int[actionStrings.length];
//            for (int i = 0, count = actions.length; i < count; i++) {
//                actions[i] = Integer.decode(actionStrings[i].trim());
//            }
//            sBindings.add(new AccessibilityWebContentKeyBinding(keyCodeAndModifiers, actions));
        //} catch (NumberFormatException nfe) {
        //    Log.e(LOG_TAG, "Disregarding malformed key binding: " + bindingString);
        //}
    }
}

Boolean AccessibilityInjectorFallback::IsEnterActionKey(
    /* [in] */ Int32 keyCode)
{
    return keyCode == IKeyEvent::KEYCODE_DPAD_CENTER
            || keyCode == IKeyEvent::KEYCODE_ENTER
            || keyCode == IKeyEvent::KEYCODE_NUMPAD_ENTER;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
