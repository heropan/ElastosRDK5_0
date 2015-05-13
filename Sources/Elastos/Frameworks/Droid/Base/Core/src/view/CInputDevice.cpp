
#include "view/CInputDevice.h"
#include "view/CKeyCharacterMap.h"
#include "view/CMotionEvent.h"
#include "hardware/input/CInputManager.h"

using Elastos::Core::CObjectContainer;
using Elastos::Droid::View::CMotionEvent;
using Elastos::Droid::Hardware::Input::CInputManager;

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(CInputDevice::MotionRange, IMotionRange);

CInputDevice::MotionRange::MotionRange(
    /* [in] */ Int32 axis,
    /* [in] */ Int32 source,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float flat,
    /* [in] */ Float fuzz) :
    mAxis(axis),
    mSource(source),
    mMin(min),
    mMax(max),
    mFlat(flat),
    mFuzz(fuzz)
{
}

ECode CInputDevice::MotionRange::GetAxis(
    /* [out] */ Int32* axis)
{
    VALIDATE_NOT_NULL(axis);
    *axis = mAxis;

    return NOERROR;
}

ECode CInputDevice::MotionRange::GetSource(
    /* [out] */ Int32* source)
{
    VALIDATE_NOT_NULL(source);
    *source = mSource;

    return NOERROR;
}

ECode CInputDevice::MotionRange::GetMin(
    /* [out] */ Float* minimum)
{
    VALIDATE_NOT_NULL(minimum);
    *minimum = mMin;

    return NOERROR;
}

ECode CInputDevice::MotionRange::GetMax(
    /* [out] */ Float* maximum)
{
    VALIDATE_NOT_NULL(maximum);
    *maximum = mMax;

    return NOERROR;
}

ECode CInputDevice::MotionRange::GetRange(
    /* [out] */ Float* range)
{
    VALIDATE_NOT_NULL(range);
    *range = mMax - mMin;

    return NOERROR;
}

ECode CInputDevice::MotionRange::GetFlat(
    /* [out] */ Float* flat)
{
    VALIDATE_NOT_NULL(flat);
    *flat = mFlat;

    return NOERROR;
}

ECode CInputDevice::MotionRange::GetFuzz(
    /* [out] */ Float* fuzz)
{
    VALIDATE_NOT_NULL(fuzz);
    *fuzz = mFuzz;

    return NOERROR;
}

CInputDevice::CInputDevice()
{
}

CInputDevice::~CInputDevice()
{
}

ECode CInputDevice::constructor()
{
    return NOERROR;
}

ECode CInputDevice::constructor(
    /* [in] */ Int32 id,
    /* [in] */ Int32 generation,
    /* [in] */ const String& name,
    /* [in] */ const String& descriptor,
    /* [in] */ Boolean isExternal,
    /* [in] */ Int32 sources,
    /* [in] */ Int32 keyboardType,
    /* [in] */ IKeyCharacterMap* keyCharacterMap,
    /* [in] */ Boolean hasVibrator)
{
    mId = id;
    mGeneration = generation;
    mName = name;
    mDescriptor = descriptor;
    mIsExternal = isExternal;
    mSources = sources;
    mKeyboardType = keyboardType;
    mKeyCharacterMap = keyCharacterMap;
    mHasVibrator = hasVibrator;

    return NOERROR;
}

// ECode CInputDevice::constructor(
//     /* [in] */ IParcel* in)
// {
//     in->ReadInt32(&mId);
//     in->ReadInt32(&mGeneration);
//     in->ReadString(&mName);
//     in->ReadString(&mDescriptor);
//     in->ReadBoolean(&mIsExternal);
//     in->ReadInt32(&mSources);
//     in->ReadInt32(&mKeyboardType);
//     CKeyCharacterMap::New(in, (IKeyCharacterMap**)&mKeyCharacterMap);
//     in->ReadBoolean(&mHasVibrator);

//     for (;;) {
//         Int32 axis;
//         in->ReadInt32(&axis);
//         if (axis < 0) {
//             break;
//         }
//         Int32 source;
//         Float min, max, flat, fuzz;
//         in->ReadInt32(&source);
//         in->ReadFloat(&min);
//         in->ReadFloat(&max);
//         in->ReadFloat(&flat);
//         in->ReadFloat(&fuzz);
//         AddMotionRange(axis, source, min, max, flat, fuzz);
//     }

//     return NOERROR;
// }

ECode CInputDevice::GetDevice(
    /* [in] */ Int32 id,
    /* [out] */ IInputDevice** device)
{
    VALIDATE_NOT_NULL(device);
    return CInputManager::GetInstance()->GetInputDevice(id, device);
}

ECode CInputDevice::GetDeviceIds(
    /* [out, callee] */ ArrayOf<Int32>** deviceIds)
{
    VALIDATE_NOT_NULL(deviceIds);
    return CInputManager::GetInstance()->GetInputDeviceIds(deviceIds);
}

ECode CInputDevice::GetId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;

    return NOERROR;
}

ECode CInputDevice::GetGeneration(
    /* [out] */ Int32* generation)
{
    VALIDATE_NOT_NULL(generation);
    *generation = mGeneration;

    return NOERROR;
}

ECode CInputDevice::GetDescriptor(
    /* [out] */ String* descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    *descriptor = mDescriptor;

    return NOERROR;
}

ECode CInputDevice::IsVirtual(
    /* [out] */ Boolean* isVirtual)
{
     VALIDATE_NOT_NULL(isVirtual);
     *isVirtual = mId < 0;

     return NOERROR;
}

ECode CInputDevice::IsExternal(
    /* [out] */ Boolean* isExternal)
{
    VALIDATE_NOT_NULL(isExternal);
    *isExternal = mIsExternal;

    return NOERROR;
}

ECode CInputDevice::IsFullKeyboard(
    /* [out] */ Boolean* isFullKeyboard)
{
    VALIDATE_NOT_NULL(isFullKeyboard);
    *isFullKeyboard = (mSources & SOURCE_KEYBOARD) == SOURCE_KEYBOARD
        && mKeyboardType == KEYBOARD_TYPE_ALPHABETIC;

    return NOERROR;
}

ECode CInputDevice::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;

    return NOERROR;
}

ECode CInputDevice::GetSources(
    /* [out] */ Int32* sources)
{
    VALIDATE_NOT_NULL(sources);
    *sources = mSources;

    return NOERROR;
}

ECode CInputDevice::GetKeyboardType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mKeyboardType;

    return NOERROR;
}

ECode CInputDevice::GetKeyCharacterMap(
    /* [out] */ IKeyCharacterMap** keyCharacterMap)
{
    VALIDATE_NOT_NULL(keyCharacterMap);
    *keyCharacterMap = mKeyCharacterMap;
    INTERFACE_ADDREF(*keyCharacterMap);

    return NOERROR;
}

ECode CInputDevice::GetMotionRange(
    /* [in] */ Int32 axis,
    /* [out] */ IMotionRange** montionRange)
{
    VALIDATE_NOT_NULL(montionRange);

    List<AutoPtr<MotionRange> >::Iterator iter = mMotionRanges.Begin();
    for (; iter != mMotionRanges.End(); ++iter) {
        if ((*iter)->mAxis == axis) {
            *montionRange = iter->Get();
            INTERFACE_ADDREF(*montionRange);
            return NOERROR;
        }
    }
    *montionRange = NULL;

    return NOERROR;
}

ECode CInputDevice::GetMotionRangeEx(
    /* [in] */ Int32 axis,
    /* [in] */ Int32 source,
    /* [out] */ IMotionRange** montionRange)
{
    VALIDATE_NOT_NULL(montionRange);

    List<AutoPtr<MotionRange> >::Iterator iter = mMotionRanges.Begin();
    for (; iter != mMotionRanges.End(); ++iter) {
        if ((*iter)->mAxis == axis && (*iter)->mSource == source) {
            *montionRange = iter->Get();
            INTERFACE_ADDREF(*montionRange);
            return NOERROR;
        }
    }
    *montionRange = NULL;

    return NOERROR;
}

ECode CInputDevice::GetMotionRanges(
    /* [out] */ IObjectContainer** motionRanges)
{
    VALIDATE_NOT_NULL(motionRanges);

    CObjectContainer::New(motionRanges);
    List<AutoPtr<MotionRange> >::Iterator iter = mMotionRanges.Begin();
    for (; iter != mMotionRanges.End(); ++iter) {
        (*motionRanges)->Add(iter->Get());
    }

    return NOERROR;
}

ECode CInputDevice::AddMotionRange(
    /* [in] */ Int32 axis,
    /* [in] */ Int32 source,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float flat,
    /* [in] */ Float fuzz)
{
    AutoPtr<MotionRange> range = new MotionRange(axis, source, min, max, flat, fuzz);
    mMotionRanges.PushBack(range);
    return NOERROR;
}

ECode CInputDevice::GetVibrator(
    /* [out] */ IVibrator** vibrator)
{
    VALIDATE_NOT_NULL(vibrator);

    Mutex::Autolock lock(mMotionRangesLock);
    if (mVibrator == NULL) {
        if (mHasVibrator) {
            CInputManager::GetInstance()->GetInputDeviceVibrator(
                mId, (IVibrator**)&mVibrator);
        }
        else {
            //mVibrator = CNullVibrator::GetInstance();
            assert(0);
        }
    }
    *vibrator = mVibrator;
    INTERFACE_ADDREF(*vibrator);

    return NOERROR;
}

ECode CInputDevice::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    in->ReadInt32(&mId);
    in->ReadInt32(&mGeneration);
    in->ReadString(&mName);
    in->ReadString(&mDescriptor);
    in->ReadBoolean(&mIsExternal);
    in->ReadInt32(&mSources);
    in->ReadInt32(&mKeyboardType);
    if (mKeyCharacterMap != NULL) {
        IParcelable::Probe(mKeyCharacterMap)->ReadFromParcel(in);
    }
    else {
        CKeyCharacterMap::New(in, (IKeyCharacterMap**)&mKeyCharacterMap);
    }
    in->ReadBoolean(&mHasVibrator);

    for (;;) {
        Int32 axis;
        in->ReadInt32(&axis);
        if (axis < 0) {
            break;
        }
        Int32 source;
        Float min, max, flat, fuzz;
        in->ReadInt32(&source);
        in->ReadFloat(&min);
        in->ReadFloat(&max);
        in->ReadFloat(&flat);
        in->ReadFloat(&fuzz);
        AddMotionRange(axis, source, min, max, flat, fuzz);
    }

    return NOERROR;
}

ECode CInputDevice::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt32(mId);
    out->WriteInt32(mGeneration);
    out->WriteString(mName);
    out->WriteString(mDescriptor);
    out->WriteBoolean(mIsExternal);
    out->WriteInt32(mSources);
    out->WriteInt32(mKeyboardType);
    IParcelable::Probe(mKeyCharacterMap)->WriteToParcel(out);
    out->WriteBoolean(mHasVibrator);

    List<AutoPtr<MotionRange> >::Iterator iter = mMotionRanges.Begin();
    for (; iter != mMotionRanges.End(); ++iter) {
        MotionRange* range = iter->Get();
        if (range != NULL) {
            out->WriteInt32(range->mAxis);
            out->WriteInt32(range->mSource);
            out->WriteFloat(range->mMin);
            out->WriteFloat(range->mMax);
            out->WriteFloat(range->mFlat);
            out->WriteFloat(range->mFuzz);
        }
    }
    out->WriteInt32(-1);

    return NOERROR;
}


ECode CInputDevice::ToString(
    /* [out] */ String* str)
{
    StringBuilder description;
    ((((description += "Input Device ") += mId) += ": ") += mName) += "\n";
    ((description += "  Descriptor: ") += mDescriptor) += "\n";
    ((description += "  Generation: ") += "  Generation: ") += "\n";
    ((description += "  Generation: ") += mGeneration) += "\n";
    ((description += "  Location: ") += mIsExternal ? "external" : "built-in") += "\n";
    description += "  Keyboard Type: ";
    switch (mKeyboardType) {
        case IInputDevice::KEYBOARD_TYPE_NONE:
            description += "none";
            break;
        case IInputDevice::KEYBOARD_TYPE_NON_ALPHABETIC:
            description += "non-alphabetic";
            break;
        case IInputDevice::KEYBOARD_TYPE_ALPHABETIC:
            description += "alphabetic";
            break;
    }
    description += "\n";
    ((description += "  Has Vibrator: ") += "  Has Vibrator: ") += "\n";
    ((description += "  Sources: 0x") += StringUtils::Int32ToHexString(mSources)) += " (";
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_KEYBOARD, String("keyboard"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_DPAD, String("dpad"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_TOUCHSCREEN, String("touchscreen"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_MOUSE, String("mouse"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_STYLUS, String("stylus"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_TRACKBALL, String("trackball"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_TOUCHPAD, String("touchpad"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_JOYSTICK, String("joystick"));
    AppendSourceDescriptionIfApplicable(description, IInputDevice::SOURCE_GAMEPAD, String("gamepad"));
    description += " )\n";
    List<AutoPtr<MotionRange> >::Iterator itAxes = mMotionRanges.Begin();
    for(; itAxes != mMotionRanges.End(); itAxes++)
    {
        AutoPtr<MotionRange> range = *itAxes;
        (description += "    ") += CMotionEvent::AxisToString(range->mAxis);
        (description += ": source=0x") += StringUtils::Int32ToHexString(range->mSource);
        (description += " min=") += range->mMin;
        (description += " max=") += range->mMax;
        (description += " flat=") += range->mFlat;
        (description += " fuzz=") += range->mFuzz;
        description += "\n";
    }
    *str = description.ToString();
    return NOERROR;
}

ECode CInputDevice::AppendSourceDescriptionIfApplicable(
    /* [in] */ StringBuilder& description,
    /* [in] */ Int32 source,
    /* [in] */ const String& sourceName)
{
    if ((mSources & source) == source) {
        description += " ";
        description += sourceName;
    }
    return NOERROR;
}


}   //namespace View
}   //namespace Droid
}   //namespace Elastos
