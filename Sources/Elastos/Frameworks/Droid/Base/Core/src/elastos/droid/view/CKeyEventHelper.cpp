
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/CKeyEventHelper.h"
#include "elastos/droid/view/CKeyEvent.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CKeyEventHelper::GetMaxKeyCode(
    /* [out] */ Int32* keyCode)
{
    VALIDATE_NOT_NULL(keyCode);
    *keyCode = CKeyEvent::GetMaxKeyCode();

    return NOERROR;
}

ECode CKeyEventHelper::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c,
    /* [out] */ Int32* deadChar)
{
    VALIDATE_NOT_NULL(deadChar);
    *deadChar = CKeyEvent::GetDeadChar(accent, c);

    return NOERROR;
}

ECode CKeyEventHelper::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 source,
    /* [in] */ const String& characters,
    /* [out] */ IKeyEvent** keyEvent)
{
    VALIDATE_NOT_NULL(keyEvent);
    AutoPtr<CKeyEvent> temp = CKeyEvent::Obtain(
        downTime, eventTime, action, code, repeat, metaState,
        deviceId, scancode, flags, source, characters);
    *keyEvent = temp.Get();
    REFCOUNT_ADD(*keyEvent)

    return NOERROR;
}

ECode CKeyEventHelper::Obtain(
    /* [in] */ IKeyEvent* otherEvent,
    /* [out] */ IKeyEvent** keyEvent)
{
    VALIDATE_NOT_NULL(keyEvent);
    AutoPtr<CKeyEvent> temp = CKeyEvent::Obtain(otherEvent);
    *keyEvent = temp.Get();
    REFCOUNT_ADD(*keyEvent)

    return NOERROR;
}

ECode CKeyEventHelper::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    return CKeyEvent::ChangeTimeRepeat(event, eventTime, newRepeat, newEvent);
}

ECode CKeyEventHelper::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [in] */ Int32 newFlags,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    return CKeyEvent::ChangeTimeRepeat(event, eventTime, newRepeat, newFlags, newEvent);
}

ECode CKeyEventHelper::ChangeAction(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 action,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    return CKeyEvent::ChangeAction(event, action, newEvent);
}

ECode CKeyEventHelper::ChangeFlags(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(event);
    return CKeyEvent::ChangeFlags(event, flags, newEvent);
}

ECode CKeyEventHelper::IsGamepadButton(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = CKeyEvent::IsGamepadButton(keyCode);

    return NOERROR;
}

ECode CKeyEventHelper::GetModifierMetaStateMask(
    /* [out] */ Int32* mask)
{
    VALIDATE_NOT_NULL(mask);
    *mask = CKeyEvent::GetModifierMetaStateMask();

    return NOERROR;
}

ECode CKeyEventHelper::IsModifierKey(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = CKeyEvent::IsModifierKey(keyCode);

    return NOERROR;
}

ECode CKeyEventHelper::NormalizeMetaState(
    /* [in] */ Int32 metaState,
    /* [out] */ Int32* normalizeMetaState)
{
    VALIDATE_NOT_NULL(normalizeMetaState);
    *normalizeMetaState = CKeyEvent::NormalizeMetaState(metaState);

    return NOERROR;
}

ECode CKeyEventHelper::MetaStateHasNoModifiers(
    /* [in] */ Int32 metaState,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = CKeyEvent::MetaStateHasNoModifiers(metaState);

    return NOERROR;
}


ECode CKeyEventHelper::MetaStateHasModifiers(
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 modifiers,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return CKeyEvent::MetaStateHasModifiers(metaState, modifiers, res);
}

ECode CKeyEventHelper::ActionToString(
    /* [in] */ Int32 action,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CKeyEvent::ActionToString(action);

    return NOERROR;
}

ECode CKeyEventHelper::KeyCodeToString(
    /* [in] */ Int32 keyCode,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CKeyEvent::KeyCodeToString(keyCode);

    return NOERROR;
}

ECode CKeyEventHelper::KeyCodeFromString(
    /* [in] */ const String& symbolicName,
    /* [out] */ Int32* keyCode)
{
    VALIDATE_NOT_NULL(keyCode);
    return CKeyEvent::KeyCodeFromString(symbolicName, keyCode);
}

ECode CKeyEventHelper::MetaStateToString(
    /* [in] */ Int32 metaState,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CKeyEvent::MetaStateToString(metaState);

    return NOERROR;
}

ECode CKeyEventHelper::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ IKeyEvent** newEvent)
{
    VALIDATE_NOT_NULL(newEvent);
    return CKeyEvent::CreateFromParcelBody(in, newEvent);
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
