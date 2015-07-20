
#include "view/CKeyCharacterMap.h"
#include "view/CKeyEvent.h"
#include "view/CInputDevice.h"
#include "os/SystemClock.h"
#include "hardware/input/CInputManager.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Character.h>
#include <utils/String8.h>
#include <binder/Parcel.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::Character;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Hardware::Input::CInputManager;
using Elastos::Droid::Hardware::Input::IInputManager;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Vector;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace View {
const char* CKeyCharacterMap::TAG = "CKeyCharacterMap";
const Int32 CKeyCharacterMap::ACCENT_ACUTE;
const Int32 CKeyCharacterMap::ACCENT_BREVE;
const Int32 CKeyCharacterMap::ACCENT_CARON;
const Int32 CKeyCharacterMap::ACCENT_CEDILLA;
const Int32 CKeyCharacterMap::ACCENT_CIRCUMFLEX;
const Int32 CKeyCharacterMap::ACCENT_COMMA_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_COMMA_ABOVE_RIGHT;
const Int32 CKeyCharacterMap::ACCENT_DOT_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_DOT_BELOW; // approximate
const Int32 CKeyCharacterMap::ACCENT_DOUBLE_ACUTE;
const Int32 CKeyCharacterMap::ACCENT_GRAVE;
const Int32 CKeyCharacterMap::ACCENT_HOOK_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_HORN; // approximate
const Int32 CKeyCharacterMap::ACCENT_MACRON;
const Int32 CKeyCharacterMap::ACCENT_MACRON_BELOW;
const Int32 CKeyCharacterMap::ACCENT_OGONEK;
const Int32 CKeyCharacterMap::ACCENT_REVERSED_COMMA_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_RING_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_STROKE; // approximate
const Int32 CKeyCharacterMap::ACCENT_TILDE;
const Int32 CKeyCharacterMap::ACCENT_TURNED_COMMA_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_UMLAUT;
const Int32 CKeyCharacterMap::ACCENT_VERTICAL_LINE_ABOVE;
const Int32 CKeyCharacterMap::ACCENT_VERTICAL_LINE_BELOW;

/* Legacy dead key display characters used in previous versions of the API.
 * We still support these characters by mapping them to their non-legacy version. */
const Int32 CKeyCharacterMap::ACCENT_GRAVE_LEGACY;
const Int32 CKeyCharacterMap::ACCENT_CIRCUMFLEX_LEGACY;
const Int32 CKeyCharacterMap::ACCENT_TILDE_LEGACY;

/**
 * Maps Unicode combining diacritical to display-form dead key.
 */
HashMap<Int32, Int32> CKeyCharacterMap::sCombiningToAccent;
HashMap<Int32, Int32> CKeyCharacterMap::sAccentToCombining;
HashMap<Int32, Int32> CKeyCharacterMap::sDeadKeyCache;
Mutex CKeyCharacterMap::sDeadKeyCacheLock;

static AutoPtr<IKeyEvent> CreateKeyEventFromNative(
    /* [in] */ const android::KeyEvent* event)
{
    AutoPtr<IKeyEvent> eventObj;
    ECode ec = CKeyEvent::New(nanoseconds_to_milliseconds(event->getDownTime()),
            nanoseconds_to_milliseconds(event->getEventTime()),
            event->getAction(),
            event->getKeyCode(),
            event->getRepeatCount(),
            event->getMetaState(),
            event->getDeviceId(),
            event->getScanCode(),
            event->getFlags(),
            event->getSource(),
            (IKeyEvent**)&eventObj);
    if (FAILED(ec)) {
//        LOGE("An exception occurred while obtaining a key event.");
        return NULL;
    }
    return eventObj;
}

static AutoPtr<IStringBuilder> CreateStringBuilder()
{
    AutoPtr<IStringBuilder> stringBuilder = new StringBuilder;

    return stringBuilder;
}

const AutoPtr<IStringBuilder> CKeyCharacterMap::sDeadKeyBuilder = CreateStringBuilder();

const Boolean CKeyCharacterMap::mIsStaticInited = CKeyCharacterMap::InitStatic();

Boolean CKeyCharacterMap::InitStatic()
{
    AddCombining(0x0300, ACCENT_GRAVE);
    AddCombining(0x0301, ACCENT_ACUTE);
    AddCombining(0x0302, ACCENT_CIRCUMFLEX);
    AddCombining(0x0303, ACCENT_TILDE);
    AddCombining(0x0304, ACCENT_MACRON);
    AddCombining(0x0306, ACCENT_BREVE);
    AddCombining(0x0307, ACCENT_DOT_ABOVE);
    AddCombining(0x0308, ACCENT_UMLAUT);
    AddCombining(0x0309, ACCENT_HOOK_ABOVE);
    AddCombining(0x030A, ACCENT_RING_ABOVE);
    AddCombining(0x030B, ACCENT_DOUBLE_ACUTE);
    AddCombining(0x030C, ACCENT_CARON);
    AddCombining(0x030D, ACCENT_VERTICAL_LINE_ABOVE);
    //AddCombining(0x030E, ACCENT_DOUBLE_VERTICAL_LINE_ABOVE);
    //AddCombining(0x030F, ACCENT_DOUBLE_GRAVE);
    //AddCombining(0x0310, ACCENT_CANDRABINDU);
    //AddCombining(0x0311, ACCENT_INVERTED_BREVE);
    AddCombining(0x0312, ACCENT_TURNED_COMMA_ABOVE);
    AddCombining(0x0313, ACCENT_COMMA_ABOVE);
    AddCombining(0x0314, ACCENT_REVERSED_COMMA_ABOVE);
    AddCombining(0x0315, ACCENT_COMMA_ABOVE_RIGHT);
    AddCombining(0x031B, ACCENT_HORN);
    AddCombining(0x0323, ACCENT_DOT_BELOW);
    //AddCombining(0x0326, ACCENT_COMMA_BELOW);
    AddCombining(0x0327, ACCENT_CEDILLA);
    AddCombining(0x0328, ACCENT_OGONEK);
    AddCombining(0x0329, ACCENT_VERTICAL_LINE_BELOW);
    AddCombining(0x0331, ACCENT_MACRON_BELOW);
    AddCombining(0x0335, ACCENT_STROKE);
    //AddCombining(0x0342, ACCENT_PERISPOMENI);
    //AddCombining(0x0344, ACCENT_DIALYTIKA_TONOS);
    //AddCombining(0x0345, ACCENT_YPOGEGRAMMENI);

    // One-way mappings to equivalent preferred accents.
    sCombiningToAccent[0x0340] = ACCENT_GRAVE;
    sCombiningToAccent[0x0341] = ACCENT_ACUTE;
    sCombiningToAccent[0x0343] = ACCENT_COMMA_ABOVE;

    // One-way legacy mappings to preserve compatibility with older applications.
    sAccentToCombining[ACCENT_GRAVE_LEGACY] = 0x0300;
    sAccentToCombining[ACCENT_CIRCUMFLEX_LEGACY] = 0x0302;
    sAccentToCombining[ACCENT_TILDE_LEGACY] = 0x0303;

    // Non-standard decompositions.
    // Stroke modifier for Finnish multilingual keyboard and others.
    AddDeadKey(ACCENT_STROKE, 'D', 0x0110);
    AddDeadKey(ACCENT_STROKE, 'G', 0x01e4);
    AddDeadKey(ACCENT_STROKE, 'H', 0x0126);
    AddDeadKey(ACCENT_STROKE, 'I', 0x0197);
    AddDeadKey(ACCENT_STROKE, 'L', 0x0141);
    AddDeadKey(ACCENT_STROKE, 'O', 0x00d8);
    AddDeadKey(ACCENT_STROKE, 'T', 0x0166);
    AddDeadKey(ACCENT_STROKE, 'd', 0x0111);
    AddDeadKey(ACCENT_STROKE, 'g', 0x01e5);
    AddDeadKey(ACCENT_STROKE, 'h', 0x0127);
    AddDeadKey(ACCENT_STROKE, 'i', 0x0268);
    AddDeadKey(ACCENT_STROKE, 'l', 0x0142);
    AddDeadKey(ACCENT_STROKE, 'o', 0x00f8);
    AddDeadKey(ACCENT_STROKE, 't', 0x0167);

    return TRUE;
}

void CKeyCharacterMap::AddCombining(
    /* [in] */ Int32 combining,
    /* [in] */ Int32 accent)
{
    sCombiningToAccent[combining] = accent;
    sAccentToCombining[accent] = combining;
}

void CKeyCharacterMap::AddDeadKey(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c,
    /* [in] */ Int32 result)
{
    HashMap<Int32, Int32>::Iterator find = sAccentToCombining.Find(accent);
    if (find == sAccentToCombining.End()) {
        ALOGE("Invalid dead key declaration.");
        assert(0);
        //throw new IllegalStateException("Invalid dead key declaration.");
    }
    Int32 combining = find->mSecond;
    Int32 combination = (combining << 16) | c;
    sDeadKeyCache[combination] = result;
}

Int32 CKeyCharacterMap::FallbackAction::MAX_RECYCLED = 10;
Mutex CKeyCharacterMap::FallbackAction::sRecycleLock;
AutoPtr<CKeyCharacterMap::FallbackAction> CKeyCharacterMap::FallbackAction::sRecycleBin;
Int32 CKeyCharacterMap::FallbackAction::sRecycledCount = 0;

CAR_INTERFACE_IMPL(CKeyCharacterMap::FallbackAction, IFallbackAction);

CKeyCharacterMap::FallbackAction::FallbackAction()
{
}

AutoPtr<CKeyCharacterMap::FallbackAction> CKeyCharacterMap::FallbackAction::Obtain()
{
    AutoPtr<FallbackAction> target;
    AutoLock lock(sRecycleLock);
    if (sRecycleBin == NULL) {
        target = new FallbackAction();
    }
    else {
        target = sRecycleBin;
        sRecycleBin = target->mNext;
        sRecycledCount--;
        target->mNext = NULL;
    }

    return target;
}

CKeyCharacterMap::FallbackAction::Recycle()
{
    AutoLock lock(sRecycleLock);
    if (sRecycledCount < MAX_RECYCLED) {
        mNext = sRecycleBin;
        sRecycleBin = this;
        sRecycledCount++;
    }
    else {
        mNext = NULL;
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(CKeyCharacterMap::KeyData, IKeyData);

CKeyCharacterMap::KeyData::KeyData()
{
    mMeta = ArrayOf<Char32>::Alloc(META_LENGTH);
}

CKeyCharacterMap::CKeyCharacterMap()
    : mMap(NULL)
{}

ECode CKeyCharacterMap::constructor()
{
    return NOERROR;
}

CKeyCharacterMap::~CKeyCharacterMap()
{
    delete mMap;
}

ECode CKeyCharacterMap::constructor(
        /* [in] */ IParcel* in)
{
    if (in == NULL) {
        Slogger::E(TAG, String("parcel must not be NULL"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeReadFromParcel(in);
    if (mMap == NULL) {
        Slogger::E(TAG, String("Could not read KeyCharacterMap from parcel."));
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

ECode CKeyCharacterMap::constructor(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 map)
{
    mMap = new NativeKeyCharacterMap(deviceId, reinterpret_cast<android::KeyCharacterMap*>(map));

    return NOERROR;
}

ECode CKeyCharacterMap::Load(
    /* [in] */ Int32 deviceId,
    /* [out] */ IKeyCharacterMap** kcm)
{
    VALIDATE_NOT_NULL(kcm);

    AutoPtr<IInputManager> im = CInputManager::GetInstance();
    AutoPtr<IInputDevice> inputDevice;
    im->GetInputDevice(deviceId, (IInputDevice**)&inputDevice);
    if (inputDevice == NULL) {
        im->GetInputDevice(VIRTUAL_KEYBOARD, (IInputDevice**)&inputDevice);
        if (inputDevice == NULL) {
            Slogger::E(TAG, String("Could not load key character map for device ")
                + StringUtils::Int32ToString(deviceId));
            return E_UNAVAILABLE_EXCEPTION;
        }
    }
    return inputDevice->GetKeyCharacterMap(kcm);
}

ECode CKeyCharacterMap::Get(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 metaState,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    metaState = CKeyEvent::NormalizeMetaState(metaState);
    Char32 ch = NativeGetCharacter(keyCode, metaState);

    HashMap<Int32, Int32>::Iterator find = sCombiningToAccent.Find(ch);
    if (find != sCombiningToAccent.End()) {
        *value = find->mSecond | COMBINING_ACCENT;
    }
    else {
        *value = ch;
    }

    return NOERROR;
}

ECode CKeyCharacterMap::GetFallbackAction(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 metaState,
    /* [out] */ IFallbackAction** fallbackAction)
{
    VALIDATE_NOT_NULL(fallbackAction);

    AutoPtr<FallbackAction> action = FallbackAction::Obtain();
    metaState = CKeyEvent::NormalizeMetaState(metaState);
    if (NativeGetFallbackAction(keyCode, metaState, action)) {
        action->mMetaState = CKeyEvent::NormalizeMetaState(action->mMetaState);
        *fallbackAction = action;
        REFCOUNT_ADD(*fallbackAction);
        return NOERROR;
    }

    action->Recycle();
    *fallbackAction = NULL;

    return NOERROR;
}

ECode CKeyCharacterMap::GetNumber(
    /* [in] */ Int32 keyCode,
    /* [out] */ Char32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = NativeGetNumber(keyCode);

    return NOERROR;
}

ECode CKeyCharacterMap::GetMatch(
    /* [in] */ Int32 keyCode,
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [out] */ Char32* match)
{
    VALIDATE_NOT_NULL (match);
    return GetMatch(keyCode, chars, 0, match);
}

ECode CKeyCharacterMap::GetMatch(
    /* [in] */ Int32 keyCode,
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 metaState,
    /* [out] */ Char32* match)
{
    VALIDATE_NOT_NULL(match);
    metaState = CKeyEvent::NormalizeMetaState(metaState);
    *match = NativeGetMatch(keyCode, chars, metaState);

    return NOERROR;
}

ECode CKeyCharacterMap::GetDisplayLabel(
    /* [in] */ Int32 keyCode,
    /* [out] */ Char32* label)
{
    VALIDATE_NOT_NULL(label);
    *label = NativeGetDisplayLabel(keyCode);

    return NOERROR;
}

Int32 CKeyCharacterMap::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c)
{
    HashMap<Int32, Int32>::Iterator find = sAccentToCombining.Find(accent);
    if (find == sAccentToCombining.End()) {
        return 0;
    }

    Int32 combining = find->mSecond;
    Int32 combination = (combining << 16) | c;
    Int32 combined;
    AutoLock lock(sDeadKeyCacheLock);
    find = sDeadKeyCache.Find(combination);
    if (find == sDeadKeyCache.End()) {
        combined = -1;
        sDeadKeyBuilder->Reset();
        sDeadKeyBuilder->AppendChar((Char32)c);
        sDeadKeyBuilder->AppendChar((Char32)combining);
        //String result = Normalizer.normalize(sDeadKeyBuilder, Normalizer.Form.NFC);
        String result;
        sDeadKeyBuilder->ToString(&result);
        combined = result.GetLength() == 1 ? result.GetChar(0) : 0;
        sDeadKeyCache[combination] = combined;
    }
    else {
        combined = find->mSecond;
    }

    return combined;
}

ECode CKeyCharacterMap::GetKeyData(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyData* results,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    KeyData* keyData = (KeyData*)results;
    if (keyData->mMeta->GetLength() < KeyData::META_LENGTH) {
        Slogger::E(TAG, String("results.meta.length must be >= ")
            + StringUtils::Int32ToString(KeyData::META_LENGTH));
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Char32 displayLabel = NativeGetDisplayLabel(keyCode);
    if (displayLabel == 0) {
        return FALSE;
    }

    keyData->mDisplayLabel = displayLabel;
    keyData->mNumber = NativeGetNumber(keyCode);
    (*keyData->mMeta)[0] = NativeGetCharacter(keyCode, 0);
    (*keyData->mMeta)[1] = NativeGetCharacter(keyCode, CKeyEvent::META_SHIFT_ON);
    (*keyData->mMeta)[2] = NativeGetCharacter(keyCode, CKeyEvent::META_ALT_ON);
    (*keyData->mMeta)[3] = NativeGetCharacter(keyCode,
            CKeyEvent::META_ALT_ON | CKeyEvent::META_SHIFT_ON);
    return TRUE;
}

ECode CKeyCharacterMap::GetEvents(
    /* [in] */ ArrayOf<Char32>* chars,
    /* [out, callee] */ ArrayOf<IKeyEvent*>** keyEvents)
{
    VALIDATE_NOT_NULL(keyEvents);
    AutoPtr<ArrayOf<IKeyEvent*> > temp = NativeGetEvents(*chars);
    *keyEvents = temp;
    REFCOUNT_ADD(*keyEvents)

    return NOERROR;
}

ECode CKeyCharacterMap::IsPrintingKey(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    Int32 type = Character::GetType(NativeGetDisplayLabel(keyCode));
    switch (type)
    {
        case Character::SPACE_SEPARATOR:
        case Character::LINE_SEPARATOR:
        case Character::PARAGRAPH_SEPARATOR:
        case Character::CONTROL:
        case Character::FORMAT:
            *res = FALSE;
        default:
            *res = TRUE;
    }

    return NOERROR;
}

ECode CKeyCharacterMap::GetKeyboardType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = NativeGetKeyboardType();

    return NOERROR;
}

ECode CKeyCharacterMap::GetModifierBehavior(
    /* [out] */ Int32* behavior)
{
    VALIDATE_NOT_NULL(behavior);
    Int32 type;
    GetKeyboardType(&type);
    switch (type) {
        case FULL:
        case SPECIAL_FUNCTION:
            *behavior = MODIFIER_BEHAVIOR_CHORDED;
        default:
            *behavior = MODIFIER_BEHAVIOR_CHORDED_OR_TOGGLED;
    }

    return NOERROR;
}

ECode CKeyCharacterMap::DeviceHasKey(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* hasKey)
{
    VALIDATE_NOT_NULL(hasKey);
    ArrayOf_<Int32, 1> keyCodes;
    keyCodes[0] = keyCode;
    AutoPtr<ArrayOf<Boolean> > hasKeys;
    FAIL_RETURN(CInputManager::GetInstance()->DeviceHasKeys(
        keyCodes, (ArrayOf<Boolean>**)&hasKeys));
    *hasKey = (*hasKeys)[0];

    return NOERROR;
}

ECode CKeyCharacterMap::DeviceHasKeys(
    /* [in] */ const ArrayOf<Int32>& keyCodes,
    /* [out, callee]] */ ArrayOf<Boolean>** hasKeys)
{
    VALIDATE_NOT_NULL(hasKeys);
    return CInputManager::GetInstance()->DeviceHasKeys(keyCodes, hasKeys);
}

ECode CKeyCharacterMap::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    if (in == NULL) {
        Slogger::E(TAG, String("parcel must not be NULL"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeReadFromParcel(in);

    return NOERROR;
}

//@Override
ECode CKeyCharacterMap::WriteToParcel(
    /* [in] */ IParcel* out)
{
    if (out == NULL) {
        Slogger::E(TAG, String("parcel must not be NULL"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeWriteToParcel(out);

    return NOERROR;
}

ECode CKeyCharacterMap::GetMap(
    /* [in] */ Int32* map)
{
    VALIDATE_NOT_NULL(map);

    *map = reinterpret_cast<Int32>(
        new NativeKeyCharacterMap(mMap->getDeviceId(), mMap->getMap()));
    return NOERROR;
}

void CKeyCharacterMap::NativeReadFromParcel(
        /* [in] */ IParcel* in)
{
    Handle32 data;
    in->GetElementPayload(&data);
    android::Parcel* parcel = (android::Parcel*)data;
    if (!parcel) {
        return;
    }

    int32_t deviceId = parcel->readInt32();
    if (parcel->errorCheck()) {
        return;
    }

    android::sp<android::KeyCharacterMap> kcm =
        android::KeyCharacterMap::readFromParcel(parcel);
    if (!kcm.get()) {
        return;
    }
    mMap = new NativeKeyCharacterMap(deviceId, kcm);
}

void CKeyCharacterMap::NativeWriteToParcel(
    /* [in] */ IParcel* out)
{
    Handle32 data;
    out->GetElementPayload(&data);
    android::Parcel* parcel = (android::Parcel*)data;
    if (parcel) {
        parcel->writeInt32(mMap->getDeviceId());
        mMap->getMap()->writeToParcel(parcel);
    }
}

Char32 CKeyCharacterMap::NativeGetCharacter(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 metaState)
{
    return mMap->getMap()->getCharacter(keyCode, metaState);
}

Boolean CKeyCharacterMap::NativeGetFallbackAction(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 metaState,
    /* [in] */ FallbackAction* outFallbackAction)
{
    android::KeyCharacterMap::FallbackAction fallbackAction;

    Boolean result =  mMap->getMap()->getFallbackAction(keyCode, metaState, &fallbackAction);
    if (result) {
        outFallbackAction->mKeyCode = fallbackAction.keyCode;
        outFallbackAction->mMetaState = fallbackAction.metaState;
    }
    return result;
}

Char32 CKeyCharacterMap::NativeGetNumber(
    /* [in] */ Int32 keyCode)
{
    return mMap->getMap()->getNumber(keyCode);
}

Char32 CKeyCharacterMap::NativeGetMatch(
    /* [in] */ Int32 keyCode,
    /* [in] */ const ArrayOf<Char32>& chars,
    /* [in] */ Int32 metaState)
{
    Int32 numChars = chars.GetLength();
    AutoPtr<ArrayOf<char16_t> > array = ArrayOf<char16_t>::Alloc(numChars);
    for (Int32 i = 0; i < numChars; i++)
        (*array)[i] = (char16_t)chars[i];

    return mMap->getMap()->getMatch(keyCode, array->GetPayload(), size_t(numChars), metaState);
}

Char32 CKeyCharacterMap::NativeGetDisplayLabel(
    /* [in] */ Int32 keyCode)
{
    return mMap->getMap()->getDisplayLabel(keyCode);
}

Int32 CKeyCharacterMap::NativeGetKeyboardType()
{
    return mMap->getMap()->getKeyboardType();
}

AutoPtr<ArrayOf<IKeyEvent*> > CKeyCharacterMap::NativeGetEvents(
    /* [in] */ const ArrayOf<Char32>& chars)
{
    Int32 numChars = chars.GetLength();
    AutoPtr<ArrayOf<char16_t> > array = ArrayOf<char16_t>::Alloc(numChars);
    for (Int32 i = 0; i < numChars; i++)
        (*array)[i] = (char16_t)chars[i];

    android::Vector<android::KeyEvent> events;
    AutoPtr<ArrayOf<IKeyEvent*> > result;
    if (mMap->getMap()->getEvents(mMap->getDeviceId(), array->GetPayload(), size_t(numChars), events)) {
        result = ArrayOf<IKeyEvent*>::Alloc(events.size());
        if (result != NULL) {
            for (size_t i = 0; i < events.size(); i++) {
                AutoPtr<IKeyEvent> keyEvent = CreateKeyEventFromNative(&events.itemAt(i));
                if (keyEvent == NULL)
                    break; // threw OOM exception
                result->Set(i, keyEvent);
            }
        }
    }

    return result;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
