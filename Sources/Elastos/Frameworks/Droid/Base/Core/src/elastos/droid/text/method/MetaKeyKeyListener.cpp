#include "text/method/MetaKeyKeyListener.h"
#ifdef DROID_CORE
#include "text/CNoCopySpan.h"
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Droid::View::IKeyCharacterMap;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

// These bits are privately used by the meta key key listener.
// They are deliberately assigned values outside of the representable range of an 'Int32'
// so as not to conflict with any meta key states publicly defined by KeyEvent.
const Int64 MetaKeyKeyListener::META_CAP_USED = 1L << 32;
const Int64 MetaKeyKeyListener::META_ALT_USED = 1L << 33;
const Int64 MetaKeyKeyListener::META_SYM_USED = 1L << 34;

const Int64 MetaKeyKeyListener::META_CAP_PRESSED = 1L << 40;
const Int64 MetaKeyKeyListener::META_ALT_PRESSED = 1L << 41;
const Int64 MetaKeyKeyListener::META_SYM_PRESSED = 1L << 42;

const Int64 MetaKeyKeyListener::META_CAP_RELEASED = 1L << 48;
const Int64 MetaKeyKeyListener::META_ALT_RELEASED = 1L << 49;
const Int64 MetaKeyKeyListener::META_SYM_RELEASED = 1L << 50;

const Int64 MetaKeyKeyListener::META_SHIFT_MASK = IMetaKeyKeyListener::META_SHIFT_ON | IMetaKeyKeyListener::META_CAP_LOCKED | META_CAP_USED | META_CAP_PRESSED | META_CAP_RELEASED;
const Int64 MetaKeyKeyListener::META_ALT_MASK = IMetaKeyKeyListener::META_ALT_ON | IMetaKeyKeyListener::META_ALT_LOCKED | META_ALT_USED | META_ALT_PRESSED | META_ALT_RELEASED;
const Int64 MetaKeyKeyListener::META_SYM_MASK = IMetaKeyKeyListener::META_SYM_ON | IMetaKeyKeyListener::META_SYM_LOCKED | META_SYM_USED | META_SYM_PRESSED | META_SYM_RELEASED;

const AutoPtr<IInterface> MetaKeyKeyListener::CAP = MetaKeyKeyListener::NewNoCopySpan();// = new NoCopySpan.Concrete();
const AutoPtr<IInterface> MetaKeyKeyListener::ALT = MetaKeyKeyListener::NewNoCopySpan();// = new NoCopySpan.Concrete();
const AutoPtr<IInterface> MetaKeyKeyListener::SYM = MetaKeyKeyListener::NewNoCopySpan();// = new NoCopySpan.Concrete();
const AutoPtr<IInterface> MetaKeyKeyListener::SELECTING = MetaKeyKeyListener::NewNoCopySpan();// = new NoCopySpan.Concrete();

/**
 * The meta key has been pressed but has not yet been used.
 */
const Int32 MetaKeyKeyListener::PRESSED = ISpannable::SPAN_MARK_MARK | (1 << ISpannable::SPAN_USER_SHIFT);

/**
 * The meta key has been pressed and released but has still
 * not yet been used.
 */
const Int32 MetaKeyKeyListener::RELEASED = ISpannable::SPAN_MARK_MARK | (2 << ISpannable::SPAN_USER_SHIFT);

/**
 * The meta key has been pressed and used but has not yet been released.
 */
const Int32 MetaKeyKeyListener::USED = ISpannable::SPAN_MARK_MARK | (3 << ISpannable::SPAN_USER_SHIFT);

/**
 * The meta key has been pressed and released without use, and then
 * pressed again; it may also have been released again.
 */
const Int32 MetaKeyKeyListener::LOCKED = ISpannable::SPAN_MARK_MARK | (4 << ISpannable::SPAN_USER_SHIFT);

const Int32 MetaKeyKeyListener::PRESSED_RETURN_VALUE = 1;

const Int32 MetaKeyKeyListener::LOCKED_RETURN_VALUE = 2;

CAR_INTERFACE_IMPL(MetaKeyKeyListener, Object, IMetaKeyKeyListener)

AutoPtr<IInterface> MetaKeyKeyListener::NewNoCopySpan()
{
#ifdef DROID_CORE
    AutoPtr<CNoCopySpan> noCopySpan;
    CNoCopySpan::NewByFriend((CNoCopySpan**)&noCopySpan);
#else
    AutoPtr<INoCopySpan> noCopySpan;
    CNoCopySpan::New((INoCopySpan**)&noCopySpan);
#endif
    return noCopySpan->Probe(EIID_IInterface);
}

void MetaKeyKeyListener::ResetMetaState(
    /* [in] */ ISpannable* text)
{
    text->RemoveSpan(CAP);
    text->RemoveSpan(ALT);
    text->RemoveSpan(SYM);
    text->RemoveSpan(SELECTING);
}

Int32 MetaKeyKeyListener::GetActive(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* meta,
    /* [in] */ Int32 on,
    /* [in] */ Int32 lock)
{
    if (!(ISpanned::Probe(text))) {
        return 0;
    }

    AutoPtr<ISpanned> sp = ISpanned::Probe(text);
    Int32 flag;
    sp->GetSpanFlags(meta, &flag);

    if (flag == LOCKED) {
        return lock;
    }
    else if (flag != 0) {
        return on;
    }
    else {
        return 0;
    }
}

void MetaKeyKeyListener::AdjustMetaAfterKeypress(
    /* [in] */ ISpannable* content)
{
    Adjust(content, CAP);
    Adjust(content, ALT);
    Adjust(content, SYM);
}

Boolean MetaKeyKeyListener::IsMetaTracker(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* what)
{
    return what == CAP || what == ALT || what == SYM ||
           what == SELECTING;
}

Boolean MetaKeyKeyListener::IsSelectingMetaTracker(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* what)
{
    return what == SELECTING;
}

void MetaKeyKeyListener::Adjust(
    /* [in] */ ISpannable* content,
    /* [in] */ IInterface* what)
{
    Int32 current;
    content->GetSpanFlags(what, &current);

    if (current == PRESSED) {
        content->SetSpan(what, 0, 0, USED);
    }
    else if (current == RELEASED) {
        content->RemoveSpan(what);
    }
}

void MetaKeyKeyListener::ResetLockedMeta(
    /* [in] */ ISpannable* content)
{
    ResetLock(content, CAP);
    ResetLock(content, ALT);
    ResetLock(content, SYM);
    ResetLock(content, SELECTING);
}

void MetaKeyKeyListener::ResetLock(
    /* [in] */ ISpannable* content,
    /* [in] */ IInterface* what)
{
    Int32 current;
    content->GetSpanFlags(what, &current);

    if (current == LOCKED)
        content->RemoveSpan(what);
}

Boolean MetaKeyKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT) {
        Press(content, CAP);
        return TRUE;
    }

    if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT
            || keyCode == IKeyEvent::KEYCODE_NUM) {
        Press(content, ALT);
        return TRUE;
    }

    if (keyCode == IKeyEvent::KEYCODE_SYM) {
        Press(content, SYM);
        return TRUE;
    }

    return FALSE; // no super to call through to
}

void MetaKeyKeyListener::Press(
    /* [in] */ IEditable* content,
    /* [in] */ IInterface* what)
{
    Int32 state;
    content->GetSpanFlags(what, &state);

    if (state == PRESSED) {
        ; // repeat before use
    }
    else if (state == RELEASED) {
        content->SetSpan(what, 0, 0, LOCKED);
    }
    else if (state == USED) {
        ; // repeat after use
    }
    else if (state == LOCKED) {
        content->RemoveSpan(what);
    }
    else {
        content->SetSpan(what, 0, 0, PRESSED);
    }
}

void MetaKeyKeyListener::StartSelecting(
    /* [in] */ IView* view,
    /* [in] */ ISpannable* content)
{
    content->SetSpan(SELECTING, 0, 0, PRESSED);
}

void MetaKeyKeyListener::StopSelecting(
    /* [in] */ IView* view,
    /* [in] */ ISpannable* content)
{
    content->RemoveSpan(SELECTING);
}

Boolean MetaKeyKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT) {
        ReleaseInternal(content, CAP, event);
        return TRUE;
    }

    if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT
            || keyCode == IKeyEvent::KEYCODE_NUM) {
        ReleaseInternal(content, ALT, event);
        return TRUE;
    }

    if (keyCode == IKeyEvent::KEYCODE_SYM) {
        ReleaseInternal(content, SYM, event);
        return TRUE;
    }

    return FALSE; // no super to call through to
}

void MetaKeyKeyListener::ReleaseInternal(
    /* [in] */ IEditable* content,
    /* [in] */ IInterface* what,
    /* [in] */ IKeyEvent* event)
{
    Int32 current;
    content->GetSpanFlags(what, &current);

    AutoPtr<IKeyCharacterMap> kcm;
    event->GetKeyCharacterMap((IKeyCharacterMap**)&kcm);
    Int32 modifierBehavior;
    kcm->GetModifierBehavior(&modifierBehavior);

    switch (modifierBehavior) {
    case IKeyCharacterMap::MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED: {
        if (current == USED) {
            content->RemoveSpan(what);
        }
        else if (current == PRESSED) {
            content->SetSpan(what, 0, 0, RELEASED);
        }
        break;
    }
    default:
        content->RemoveSpan(what);
        break;
    }
}

void MetaKeyKeyListener::ClearMetaKeyState(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    ClearMetaKeyState(content, states);
    return;// NOERROR;
}

void MetaKeyKeyListener::ClearMetaKeyState(
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    if ((states & IMetaKeyKeyListener::META_SHIFT_ON) != 0) {
        content->RemoveSpan(CAP);
    }
    if ((states & IMetaKeyKeyListener::META_ALT_ON) != 0) {
        content->RemoveSpan(ALT);
    }
    if ((states & IMetaKeyKeyListener::META_SYM_ON) != 0) {
        content->RemoveSpan(SYM);
    }
    if ((states & IMetaKeyKeyListener::META_SELECTING) != 0) {
        content->RemoveSpan(SELECTING);
    }
}

Int64 MetaKeyKeyListener::ResetLockedMeta(
    /* [in] */ Int64 state)
{
    if ((state & IMetaKeyKeyListener::META_CAP_LOCKED) != 0) {
        state &= ~META_SHIFT_MASK;
    }
    if ((state & IMetaKeyKeyListener::META_ALT_LOCKED) != 0) {
        state &= ~META_ALT_MASK;
    }
    if ((state & IMetaKeyKeyListener::META_SYM_LOCKED) != 0) {
        state &= ~META_SYM_MASK;
    }
    return state;
}

Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ Int64 state)
{
    Int32 result = 0;

    if ((state & IMetaKeyKeyListener::META_CAP_LOCKED) != 0) {
        result |= IMetaKeyKeyListener::META_CAP_LOCKED;
    }
    else if ((state & IMetaKeyKeyListener::META_SHIFT_ON) != 0) {
        result |= IMetaKeyKeyListener::META_SHIFT_ON;
    }

    if ((state & IMetaKeyKeyListener::META_ALT_LOCKED) != 0) {
        result |= IMetaKeyKeyListener::META_ALT_LOCKED;
    }
    else if ((state & IMetaKeyKeyListener::META_ALT_ON) != 0) {
        result |= IMetaKeyKeyListener::META_ALT_ON;
    }

    if ((state & IMetaKeyKeyListener::META_SYM_LOCKED) != 0) {
        result |= IMetaKeyKeyListener::META_SYM_LOCKED;
    }
    else if ((state & IMetaKeyKeyListener::META_SYM_ON) != 0) {
        result |= IMetaKeyKeyListener::META_SYM_ON;
    }

    return result;
}

Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ Int64 state,
    /* [in] */ Int32 meta)
{
    switch (meta) {
        case IMetaKeyKeyListener::META_SHIFT_ON:
            if ((state & IMetaKeyKeyListener::META_CAP_LOCKED) != 0) return MetaKeyKeyListener::LOCKED_RETURN_VALUE;
            if ((state & IMetaKeyKeyListener::META_SHIFT_ON) != 0) return MetaKeyKeyListener::PRESSED_RETURN_VALUE;
            return 0;

        case IMetaKeyKeyListener::META_ALT_ON:
            if ((state & IMetaKeyKeyListener::META_ALT_LOCKED) != 0) return MetaKeyKeyListener::LOCKED_RETURN_VALUE;
            if ((state & IMetaKeyKeyListener::META_ALT_ON) != 0) return MetaKeyKeyListener::PRESSED_RETURN_VALUE;
            return 0;

        case IMetaKeyKeyListener::META_SYM_ON:
            if ((state & IMetaKeyKeyListener::META_SYM_LOCKED) != 0) return MetaKeyKeyListener::LOCKED_RETURN_VALUE;
            if ((state & IMetaKeyKeyListener::META_SYM_ON) != 0) return MetaKeyKeyListener::PRESSED_RETURN_VALUE;
            return 0;

        default:
            return 0;
    }
}

const Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ ICharSequence* text,
    /* [in] */ IKeyEvent* event)
{
    VALIDATE_NOT_NULL(event)
    Int32 metaState;
    event->GetMetaState(&metaState);
    AutoPtr<IKeyCharacterMap> keyCharacterMap;
    event->GetKeyCharacterMap((IKeyCharacterMap**)&keyCharacterMap);
    Int32 behavior;
    keyCharacterMap->GetModifierBehavior(&behavior);
    if (behavior == IKeyCharacterMap.MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED) {
            metaState |= GetMetaState(text);
    }
    return metaState;
}

const Int32 MetaKeyKeyListener::GetMetaState(
       /* [in] */ ICharSequence* text,
       /* [in] */ Int32 meta,
       /* [in] */ IKeyEvent* event)
{
    Int32 metaState;
    event->GetMetaState(&metaState);
    AutoPtr<IKeyCharacterMap> kcm;
    event->GetKeyCharacterMap((IKeyCharacterMap**)&kcm);
    Int32 behavior;
    if ((kcm->GetModifierBehavior(&behavior), behavior)
            == IKeyCharacterMap.MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED) {
        metaState |= GetMetaState(text);
    }
    if (IMetaKeyKeyListener::META_SELECTING == meta) {
        // #getMetaState(long, int) does not support META_SELECTING, but we want the same
        // behavior as #getMetaState(CharSequence, int) so we need to do it here
        if ((metaState & IMetaKeyKeyListener::META_SELECTING) != 0) {
            // META_SELECTING is only ever set to PRESSED and can't be LOCKED, so return 1
            return 1;
        }
        return 0;
    }
    return GetMetaState((Int64)metaState, meta);
}

Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ ICharSequence* text)
{
    return GetActive(text, CAP, IMetaKeyKeyListener::META_SHIFT_ON, IMetaKeyKeyListener::META_CAP_LOCKED) |
           GetActive(text, ALT, IMetaKeyKeyListener::META_ALT_ON, IMetaKeyKeyListener::META_ALT_LOCKED) |
           GetActive(text, SYM, IMetaKeyKeyListener::META_SYM_ON, IMetaKeyKeyListener::META_SYM_LOCKED) |
           GetActive(text, SELECTING, IMetaKeyKeyListener::META_SELECTING, IMetaKeyKeyListener::META_SELECTING);
}

Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 meta)
{
    switch (meta) {
        case IMetaKeyKeyListener::META_SHIFT_ON:
            return GetActive(text, CAP, MetaKeyKeyListener::PRESSED_RETURN_VALUE, MetaKeyKeyListener::LOCKED_RETURN_VALUE);

        case IMetaKeyKeyListener::META_ALT_ON:
            return GetActive(text, ALT, MetaKeyKeyListener::PRESSED_RETURN_VALUE, MetaKeyKeyListener::LOCKED_RETURN_VALUE);

        case IMetaKeyKeyListener::META_SYM_ON:
            return GetActive(text, SYM, MetaKeyKeyListener::PRESSED_RETURN_VALUE, MetaKeyKeyListener::LOCKED_RETURN_VALUE);

        case IMetaKeyKeyListener::META_SELECTING:
            return GetActive(text, SELECTING, MetaKeyKeyListener::PRESSED_RETURN_VALUE, MetaKeyKeyListener::LOCKED_RETURN_VALUE);

        default:
            return 0;
    }
}

Int64 MetaKeyKeyListener::AdjustMetaAfterKeypress(
    /* [in] */ Int64 state)
{
    if ((state & META_CAP_PRESSED) != 0) {
        state = (state & ~META_SHIFT_MASK) | IMetaKeyKeyListener::META_SHIFT_ON | META_CAP_USED;
    }
    else if ((state & META_CAP_RELEASED) != 0) {
        state &= ~META_SHIFT_MASK;
    }

    if ((state & META_ALT_PRESSED) != 0) {
        state = (state & ~META_ALT_MASK) | IMetaKeyKeyListener::META_ALT_ON | META_ALT_USED;
    }
    else if ((state & META_ALT_RELEASED) != 0) {
        state &= ~META_ALT_MASK;
    }

    if ((state & META_SYM_PRESSED) != 0) {
        state = (state & ~META_SYM_MASK) | IMetaKeyKeyListener::META_SYM_ON | META_SYM_USED;
    }
    else if ((state & META_SYM_RELEASED) != 0) {
        state &= ~META_SYM_MASK;
    }
    return state;
}

Int64 MetaKeyKeyListener::HandleKeyDown(
    /* [in] */ Int64 state,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT) {
        return Press(state, IMetaKeyKeyListener::META_SHIFT_ON, META_SHIFT_MASK,
                IMetaKeyKeyListener::META_CAP_LOCKED, META_CAP_PRESSED, META_CAP_RELEASED, META_CAP_USED);
    }

    if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT
            || keyCode == IKeyEvent::KEYCODE_NUM) {
        return Press(state, IMetaKeyKeyListener::META_ALT_ON, META_ALT_MASK,
                IMetaKeyKeyListener::META_ALT_LOCKED, META_ALT_PRESSED, META_ALT_RELEASED, META_ALT_USED);
    }

    if (keyCode == IKeyEvent::KEYCODE_SYM) {
        return Press(state, IMetaKeyKeyListener::META_SYM_ON, META_SYM_MASK,
                IMetaKeyKeyListener::META_SYM_LOCKED, META_SYM_PRESSED, META_SYM_RELEASED, META_SYM_USED);
    }
    return state;
}

Int64 MetaKeyKeyListener::Press(
    /* [in] */ Int64 state,
    /* [in] */ Int32 what,
    /* [in] */ Int64 mask,
    /* [in] */ Int64 locked,
    /* [in] */ Int64 pressed,
    /* [in] */ Int64 released,
    /* [in] */ Int64 used)
{
    if ((state & pressed) != 0) {
        // repeat before use
    }
    else if ((state & released) != 0) {
        state = (state &~ mask) | what | locked;
    }
    else if ((state & used) != 0) {
        // repeat after use
    }
    else if ((state & locked) != 0) {
        state &= ~mask;
    }
    else {
        state |= what | pressed;
    }
    return state;
}

Int64 MetaKeyKeyListener::HandleKeyUp(
    /* [in] */ Int64 state,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT) {
        return ReleaseInternal(state, IMetaKeyKeyListener::META_SHIFT_ON, META_SHIFT_MASK,
                META_CAP_PRESSED, META_CAP_RELEASED, META_CAP_USED, event);
    }

    if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT
            || keyCode == IKeyEvent::KEYCODE_NUM) {
        return ReleaseInternal(state, IMetaKeyKeyListener::META_ALT_ON, META_ALT_MASK,
                META_ALT_PRESSED, META_ALT_RELEASED, META_ALT_USED, event);
    }

    if (keyCode == IKeyEvent::KEYCODE_SYM) {
        return ReleaseInternal(state, IMetaKeyKeyListener::META_SYM_ON, META_SYM_MASK,
                META_SYM_PRESSED, META_SYM_RELEASED, META_SYM_USED, event);
    }
    return state;
}

Int64 MetaKeyKeyListener::ReleaseInternal(
    /* [in] */ Int64 state,
    /* [in] */ Int32 what,
    /* [in] */ Int64 mask,
    /* [in] */ Int64 pressed,
    /* [in] */ Int64 released,
    /* [in] */ Int64 used,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyCharacterMap> kcm;
    event->GetKeyCharacterMap((IKeyCharacterMap**)&kcm);
    Int32 modifierBehavior;
    kcm->GetModifierBehavior(&modifierBehavior);
    switch (modifierBehavior) {
        case IKeyCharacterMap::MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED:
            if ((state & used) != 0) {
                state &= ~mask;
            }
            else if ((state & pressed) != 0) {
                state |= what | released;
            }
            break;

        default:
            state &= ~mask;
            break;
    }
    return state;
}

Int64 MetaKeyKeyListener::ClearMetaKeyState(
    /* [in] */ Int64 state,
    /* [in] */ Int32 which)
{
    if ((which & IMetaKeyKeyListener::META_SHIFT_ON) != 0 && (state & IMetaKeyKeyListener::META_CAP_LOCKED) != 0) {
        state &= ~META_SHIFT_MASK;
    }
    if ((which & IMetaKeyKeyListener::META_ALT_ON) != 0 && (state & IMetaKeyKeyListener::META_ALT_LOCKED) != 0) {
        state &= ~META_ALT_MASK;
    }
    if ((which & IMetaKeyKeyListener::META_SYM_ON) != 0 && (state & IMetaKeyKeyListener::META_SYM_LOCKED) != 0) {
        state &= ~META_SYM_MASK;
    }
    return state;
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
