
#include "elastos/droid/text/method/CMetaKeyKeyListenerHelper.h"
#include "elastos/droid/text/method/MetaKeyKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CAR_INTERFACE_DECL(CMetaKeyKeyListenerHelper, Singleton, IMetaKeyKeyListenerHelper)

CAR_SINGLETON_DECL(CMetaKeyKeyListenerHelper)

ECode CMetaKeyKeyListenerHelper::ResetMetaState(
    /* [in] */ ISpannable* text)
{
    text->RemoveSpan(MetaKeyKeyListener::CAP);
    text->RemoveSpan(MetaKeyKeyListener::ALT);
    text->RemoveSpan(MetaKeyKeyListener::SYM);
    text->RemoveSpan(MetaKeyKeyListener::SELECTING);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::GetMetaState(
    /* [in] */ ICharSequence* text,
    /* [out] */ Int32* state)
{
    assert(state != NULL);
    *state = MetaKeyKeyListener::GetMetaState(text);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::GetMetaState(
    /* [in] */ ICharSequence* text,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = MetaKeyKeyListener::GetMetaState(text, event);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::GetMetaState(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 meta,
    /* [out] */ Int32* state)
{
    assert(state != NULL);
    *state = MetaKeyKeyListener::GetMetaState(text, meta);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::GetMetaState(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 meta,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)
    *ret = MetaKeyKeyListener::GetMetaState(text, meta, event);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::GetMetaState(
    /* [in] */ Int64 state,
    /* [out] */ Int32* ret)
{
    assert(ret != NULL);
    *ret = MetaKeyKeyListener::GetMetaState(state);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::GetMetaState(
    /* [in] */ Int64 state,
    /* [in] */ Int32 meta,
    /* [out] */ Int32* ret)
{
    assert(ret != NULL);
    *ret = MetaKeyKeyListener::GetMetaState(state, meta);
    return NOERROR;

}

Int32 CMetaKeyKeyListenerHelper::GetActive(
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

    if (flag == MetaKeyKeyListener::LOCKED) {
        return lock;
    }
    else if (flag != 0) {
        return on;
    }
    else {
        return 0;
    }
}

ECode CMetaKeyKeyListenerHelper::AdjustMetaAfterKeypress(
    /* [in] */ ISpannable* content)
{
    MetaKeyKeyListener::Adjust(content, MetaKeyKeyListener::CAP);
    MetaKeyKeyListener::Adjust(content, MetaKeyKeyListener::ALT);
    MetaKeyKeyListener::Adjust(content, MetaKeyKeyListener::SYM);
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::IsMetaTracker(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* what,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = what == MetaKeyKeyListener::CAP || what == MetaKeyKeyListener::ALT || what == MetaKeyKeyListener::SYM ||
           what == MetaKeyKeyListener::SELECTING;
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::IsSelectingMetaTracker(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* what,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    *state = what == MetaKeyKeyListener::SELECTING;
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::StartSelecting(
    /* [in] */ IView* view,
    /* [in] */ ISpannable* content)
{
    assert(content != NULL);
    return content->SetSpan(MetaKeyKeyListener::SELECTING, 0, 0, MetaKeyKeyListener::PRESSED);
}

ECode CMetaKeyKeyListenerHelper::StopSelecting(
    /* [in] */ IView* view,
    /* [in] */ ISpannable* content)
{
    assert(content != NULL);
    return content->RemoveSpan(MetaKeyKeyListener::SELECTING);;
}

ECode CMetaKeyKeyListenerHelper::ClearMetaKeyState(
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    assert(content != NULL);

    if ((states & IMetaKeyKeyListener::META_SHIFT_ON) != 0)
        content->RemoveSpan(MetaKeyKeyListener::CAP);
    if ((states & IMetaKeyKeyListener::META_ALT_ON) != 0)
        content->RemoveSpan(MetaKeyKeyListener::ALT);
    if ((states & IMetaKeyKeyListener::META_SYM_ON) != 0)
        content->RemoveSpan(MetaKeyKeyListener::SYM);
    if ((states & IMetaKeyKeyListener::META_SELECTING) != 0)
        content->RemoveSpan(MetaKeyKeyListener::SELECTING);

    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::ResetLockedMeta(
    /* [in] */ Int64 state,
    /* [out] */ Int64* meta)
{
    assert(meta != NULL);
    if ((state & IMetaKeyKeyListener::META_CAP_LOCKED) != 0) {
        state &= ~MetaKeyKeyListener::META_SHIFT_MASK;
    }
    if ((state & IMetaKeyKeyListener::META_ALT_LOCKED) != 0) {
        state &= ~MetaKeyKeyListener::META_ALT_MASK;
    }
    if ((state & IMetaKeyKeyListener::META_SYM_LOCKED) != 0) {
        state &= ~MetaKeyKeyListener::META_SYM_MASK;
    }

    *meta = state;
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::AdjustMetaAfterKeypress(
    /* [in] */ Int64 state,
    /* [out] */ Int64* result)
{
    assert(result != NULL);
    if ((state & MetaKeyKeyListener::META_CAP_PRESSED) != 0) {
        state = (state & ~MetaKeyKeyListener::META_SHIFT_MASK) | IMetaKeyKeyListener::META_SHIFT_ON | MetaKeyKeyListener::META_CAP_USED;
    } else if ((state & MetaKeyKeyListener::META_CAP_RELEASED) != 0) {
        state &= ~MetaKeyKeyListener::META_SHIFT_MASK;
    }

    if ((state & MetaKeyKeyListener::META_ALT_PRESSED) != 0) {
        state = (state & ~MetaKeyKeyListener::META_ALT_MASK) | IMetaKeyKeyListener::META_ALT_ON | MetaKeyKeyListener::META_ALT_USED;
    } else if ((state & MetaKeyKeyListener::META_ALT_RELEASED) != 0) {
        state &= ~MetaKeyKeyListener::META_ALT_MASK;
    }

    if ((state & MetaKeyKeyListener::META_SYM_PRESSED) != 0) {
        state = (state & ~MetaKeyKeyListener::META_SYM_MASK) | IMetaKeyKeyListener::META_SYM_ON | MetaKeyKeyListener::META_SYM_USED;
    } else if ((state & MetaKeyKeyListener::META_SYM_RELEASED) != 0) {
        state &= ~MetaKeyKeyListener::META_SYM_MASK;
    }

    *result = state;
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::HandleKeyDown(
    /* [in] */ Int64 state,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Int64* result)
{
    assert(result != NULL);
    if (keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT/*KeyEvent.KEYCODE_SHIFT_LEFT*/ || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT/*KeyEvent.KEYCODE_SHIFT_RIGHT*/) {
        *result = MetaKeyKeyListener::Press(state, IMetaKeyKeyListener::META_SHIFT_ON, MetaKeyKeyListener::META_SHIFT_MASK,
                IMetaKeyKeyListener::META_CAP_LOCKED, MetaKeyKeyListener::META_CAP_PRESSED,
                MetaKeyKeyListener::META_CAP_RELEASED, MetaKeyKeyListener::META_CAP_USED);
    }

    if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT/*KeyEvent.KEYCODE_ALT_LEFT*/ || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT/*KeyEvent.KEYCODE_ALT_RIGHT*/
            || keyCode == IKeyEvent::KEYCODE_NUM/*KeyEvent.KEYCODE_NUM*/) {
        *result = MetaKeyKeyListener::Press(state, IMetaKeyKeyListener::META_ALT_ON, MetaKeyKeyListener::META_ALT_MASK,
                IMetaKeyKeyListener::META_ALT_LOCKED, MetaKeyKeyListener::META_ALT_PRESSED,
                MetaKeyKeyListener::META_ALT_RELEASED, MetaKeyKeyListener::META_ALT_USED);
    }

    if (keyCode == IKeyEvent::KEYCODE_SYM/*KeyEvent.KEYCODE_SYM*/) {
        *result = MetaKeyKeyListener::Press(state, IMetaKeyKeyListener::META_SYM_ON, MetaKeyKeyListener::META_SYM_MASK,
                IMetaKeyKeyListener::META_SYM_LOCKED, MetaKeyKeyListener::META_SYM_PRESSED,
                MetaKeyKeyListener::META_SYM_RELEASED, MetaKeyKeyListener::META_SYM_USED);
    }

    *result = state;
    return NOERROR;
}

ECode CMetaKeyKeyListenerHelper::HandleKeyUp(
    /* [in] */ Int64 state,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Int64* result)
{
    assert(result != NULL);
    if (keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT) {
        *result = MetaKeyKeyListener::ReleaseInternal(state, IMetaKeyKeyListener::META_SHIFT_ON, MetaKeyKeyListener::META_SHIFT_MASK,
                MetaKeyKeyListener::META_CAP_PRESSED, MetaKeyKeyListener::META_CAP_RELEASED,
                MetaKeyKeyListener::META_CAP_USED, event);
    }

    if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT
            || keyCode == IKeyEvent::KEYCODE_NUM) {
        *result = MetaKeyKeyListener::ReleaseInternal(state, IMetaKeyKeyListener::META_ALT_ON, MetaKeyKeyListener::META_ALT_MASK,
                MetaKeyKeyListener::META_ALT_PRESSED, MetaKeyKeyListener::META_ALT_RELEASED,
                MetaKeyKeyListener::META_ALT_USED, event);
    }

    if (keyCode == IKeyEvent::KEYCODE_SYM) {
        *result = MetaKeyKeyListener::ReleaseInternal(state, IMetaKeyKeyListener::META_SYM_ON, MetaKeyKeyListener::META_SYM_MASK,
                MetaKeyKeyListener::META_SYM_PRESSED, MetaKeyKeyListener::META_SYM_RELEASED,
                MetaKeyKeyListener::META_SYM_USED, event);
    }

    *result = state;
    return NOERROR;
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
