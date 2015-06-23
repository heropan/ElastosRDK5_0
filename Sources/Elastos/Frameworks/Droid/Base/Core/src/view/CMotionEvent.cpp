
#include "view/CMotionEvent.h"
#include "view/CInputDevice.h"
#include "view/CKeyEvent.h"
#include "view/CPointerCoords.h"
#include "view/CPointerProperties.h"
#include "graphics/CMatrix.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

static Boolean ValidatePointerCount(
    /* [in] */ Int32 pointerCount)
{
    if (pointerCount < 1) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerCount must be at least 1");
        return FALSE;
    }
    return TRUE;
}

static Boolean ValidatePointerPropertiesArray(
    /* [in] */ ArrayOf<IPointerProperties*>* pointerPropertiesObjArray,
    /* [in] */ size_t pointerCount)
{
    if (!pointerPropertiesObjArray) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerProperties array must not be null");
        return FALSE;
    }
    if (size_t(pointerPropertiesObjArray->GetLength()) < pointerCount) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerProperties array must be large enough to hold all pointers");
        return FALSE;
    }
    return TRUE;
}

static Boolean ValidatePointerCoordsObjArray(
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoordsObjArray,
    /* [in] */ size_t pointerCount)
{
    if (!pointerCoordsObjArray) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerCoords array must not be null");
        return FALSE;
    }
    if (size_t(pointerCoordsObjArray->GetLength()) < pointerCount) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerCoords array must be large enough to hold all pointers");
        return FALSE;
    }
    return TRUE;
}

static Boolean ValidatePointerIndex(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ size_t pointerCount)
{
    if (pointerIndex < 0 || size_t(pointerIndex) >= pointerCount) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerIndex out of range");
        return FALSE;
    }
    return TRUE;
}

static Boolean ValidateHistoryPos(
    /* [in] */ Int32 historyPos,
    /* [in] */ size_t historySize)
{
    if (historyPos < 0 || size_t(historyPos) >= historySize) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "historyPos out of range");
        return FALSE;
    }
    return TRUE;
}

static Boolean ValidatePointerCoords(
    /* [in] */ IPointerCoords* pointerCoordsObj)
{
    if (!pointerCoordsObj) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerCoords must not be null");
        return FALSE;
    }
    return TRUE;
}

static Boolean ValidatePointerProperties(
    /* [in] */ IPointerProperties* pointerPropertiesObj)
{
    if (!pointerPropertiesObj) {
        assert(0);
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //         "pointerProperties must not be null");
        return FALSE;
    }
    return TRUE;
}

static void PointerCoordsToNative(
    /* [in] */ IPointerCoords* coords,
    /* [in] */ Float xOffset,
    /* [in] */ Float yOffset,
    /* [in] */ android::PointerCoords* outRawPointerCoords)
{
    CPointerCoords* pointerCoordsObj = (CPointerCoords*)coords;
    outRawPointerCoords->clear();
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_X, pointerCoordsObj->mX - xOffset);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_Y, pointerCoordsObj->mY - yOffset);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_PRESSURE, pointerCoordsObj->mPressure);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_SIZE, pointerCoordsObj->mSize);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_TOUCH_MAJOR, pointerCoordsObj->mTouchMajor);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_TOUCH_MINOR, pointerCoordsObj->mTouchMinor);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_TOOL_MAJOR, pointerCoordsObj->mToolMajor);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_TOOL_MINOR, pointerCoordsObj->mToolMinor);
    outRawPointerCoords->setAxisValue(AMOTION_EVENT_AXIS_ORIENTATION, pointerCoordsObj->mOrientation);

    uint64_t bits = pointerCoordsObj->mPackedAxisBits;
    if (bits) {
        ArrayOf<Float>* valuesArray = pointerCoordsObj->mPackedAxisValues;
        if (valuesArray) {
            Float* values = valuesArray->GetPayload();

            uint32_t index = 0;
            do {
                uint32_t axis = __builtin_ctzll(bits);
                uint64_t axisBit = 1LL << axis;
                bits &= ~axisBit;
                outRawPointerCoords->setAxisValue(axis, values[index++]);
            } while (bits);
        }
    }
}

static AutoPtr<ArrayOf<Float> > ObtainPackedAxisValuesArray(
    /* [in] */ uint32_t minSize,
    /* [in, out] */ IPointerCoords* coords)
{
    CPointerCoords* outPointerCoordsObj = (CPointerCoords*)coords;
    AutoPtr<ArrayOf<Float> > outValuesArray = outPointerCoordsObj->mPackedAxisValues;
    if (outValuesArray) {
        uint32_t size = outValuesArray->GetLength();
        if (minSize <= size) {
            return outValuesArray;
        }
    }
    uint32_t size = 8;
    while (size < minSize) {
        size *= 2;
    }
    outValuesArray = ArrayOf<Float>::Alloc(size);
    outPointerCoordsObj->mPackedAxisValues = outValuesArray;
    return outValuesArray;
}

static void PointerCoordsFromNative(
    /* [in] */ const android::PointerCoords* rawPointerCoords,
    /* [in] */ Float xOffset,
    /* [in] */ Float yOffset,
    /* [in] */ IPointerCoords* coords)
{
    coords->SetX(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_X) + xOffset);
    coords->SetY(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_Y) + yOffset);
    coords->SetPressure(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_PRESSURE));
    coords->SetSize(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_SIZE));
    coords->SetTouchMajor(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_TOUCH_MAJOR));
    coords->SetTouchMinor(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_TOUCH_MINOR));
    coords->SetToolMajor(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_TOOL_MAJOR));
    coords->SetToolMinor(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_TOOL_MINOR));
    coords->SetOrientation(rawPointerCoords->getAxisValue(AMOTION_EVENT_AXIS_ORIENTATION));

    const uint64_t unpackedAxisBits = 0
            | (1LL << AMOTION_EVENT_AXIS_X)
            | (1LL << AMOTION_EVENT_AXIS_Y)
            | (1LL << AMOTION_EVENT_AXIS_PRESSURE)
            | (1LL << AMOTION_EVENT_AXIS_SIZE)
            | (1LL << AMOTION_EVENT_AXIS_TOUCH_MAJOR)
            | (1LL << AMOTION_EVENT_AXIS_TOUCH_MINOR)
            | (1LL << AMOTION_EVENT_AXIS_TOOL_MAJOR)
            | (1LL << AMOTION_EVENT_AXIS_TOOL_MINOR)
            | (1LL << AMOTION_EVENT_AXIS_ORIENTATION);

    uint64_t outBits = 0;
    uint64_t remainingBits = rawPointerCoords->bits & ~unpackedAxisBits;
    if (remainingBits) {
        uint32_t packedAxesCount = __builtin_popcountll(remainingBits);
        AutoPtr<ArrayOf<Float> > outValuesArray = ObtainPackedAxisValuesArray(packedAxesCount, coords);
        if (!outValuesArray) {
            return; // OOM
        }

        Float* outValues = outValuesArray->GetPayload();

        //const float* values = rawPointerCoords->values;
        uint32_t index = 0;
        do {
            uint32_t axis = __builtin_ctzll(remainingBits);
            uint64_t axisBit = 1LL << axis;
            remainingBits &= ~axisBit;
            outBits |= axisBit;
            outValues[index++] = rawPointerCoords->getAxisValue(axis);
        } while (remainingBits);
    }

    CPointerCoords* outPointerCoordsObj = (CPointerCoords*)coords;
    outPointerCoordsObj->mPackedAxisBits = outBits;
}

static void PointerPropertiesToNative(
    /* [in] */ IPointerProperties* pointerPropertiesObj,
    /* [in] */ android::PointerProperties* outPointerProperties)
{
    outPointerProperties->clear();
    pointerPropertiesObj->GetId(&outPointerProperties->id);
    pointerPropertiesObj->GetToolType(&outPointerProperties->toolType);
}

static void PointerPropertiesFromNative(
    /* [in] */ const android::PointerProperties* pointerProperties,
    /* [in] */ IPointerProperties* outPointerPropertiesObj)
{
    outPointerPropertiesObj->SetId(pointerProperties->id);
    outPointerPropertiesObj->SetToolType(pointerProperties->toolType);
}

const Int64 CMotionEvent::NS_PER_MS;
HashMap<Int32, String> CMotionEvent::AXIS_SYMBOLIC_NAMES;

const String CMotionEvent::BUTTON_SYMBOLIC_NAMES[] = {
    String("BUTTON_PRIMARY"),
    String("BUTTON_SECONDARY"),
    String("BUTTON_TERTIARY"),
    String("BUTTON_BACK"),
    String("BUTTON_FORWARD"),
    String("0x00000020"),
    String("0x00000040"),
    String("0x00000080"),
    String("0x00000100"),
    String("0x00000200"),
    String("0x00000400"),
    String("0x00000800"),
    String("0x00001000"),
    String("0x00002000"),
    String("0x00004000"),
    String("0x00008000"),
    String("0x00010000"),
    String("0x00020000"),
    String("0x00040000"),
    String("0x00080000"),
    String("0x00100000"),
    String("0x00200000"),
    String("0x00400000"),
    String("0x00800000"),
    String("0x01000000"),
    String("0x02000000"),
    String("0x04000000"),
    String("0x08000000"),
    String("0x10000000"),
    String("0x20000000"),
    String("0x40000000"),
    String("0x80000000"),
};
HashMap<Int32, String> CMotionEvent::TOOL_TYPE_SYMBOLIC_NAMES;

const Int32 CMotionEvent::HISTORY_CURRENT;
const Int32 CMotionEvent::MAX_RECYCLED;

Mutex CMotionEvent::gRecyclerLock;
Int32 CMotionEvent::gRecyclerUsed = 0;
AutoPtr<CMotionEvent> CMotionEvent::gRecyclerTop;

Mutex CMotionEvent::gSharedTempLock;
AutoPtr<ArrayOf<IPointerCoords*> > CMotionEvent::gSharedTempPointerCoords;
AutoPtr<ArrayOf<IPointerProperties*> > CMotionEvent::gSharedTempPointerProperties;
AutoPtr<ArrayOf<Int32> > CMotionEvent::gSharedTempPointerIndexMap;

const Boolean CMotionEvent::mIsStaticInited = CMotionEvent::InitStatic();

Boolean CMotionEvent::InitStatic()
{
    HashMap<Int32, String>& names = AXIS_SYMBOLIC_NAMES;
    names[AXIS_X] = String("AXIS_X");
    names[AXIS_Y] = String("AXIS_Y");
    names[AXIS_PRESSURE] = String("AXIS_PRESSURE");
    names[AXIS_SIZE] = String("AXIS_SIZE");
    names[AXIS_TOUCH_MAJOR] = String("AXIS_TOUCH_MAJOR");
    names[AXIS_TOUCH_MINOR] = String("AXIS_TOUCH_MINOR");
    names[AXIS_TOOL_MAJOR] = String("AXIS_TOOL_MAJOR");
    names[AXIS_TOOL_MINOR] = String("AXIS_TOOL_MINOR");
    names[AXIS_ORIENTATION] = String("AXIS_ORIENTATION");
    names[AXIS_VSCROLL] = String("AXIS_VSCROLL");
    names[AXIS_HSCROLL] = String("AXIS_HSCROLL");
    names[AXIS_Z] = String("AXIS_Z");
    names[AXIS_RX] = String("AXIS_RX");
    names[AXIS_RY] = String("AXIS_RY");
    names[AXIS_RZ] = String("AXIS_RZ");
    names[AXIS_HAT_X] = String("AXIS_HAT_X");
    names[AXIS_HAT_Y] = String("AXIS_HAT_Y");
    names[AXIS_LTRIGGER] = String("AXIS_LTRIGGER");
    names[AXIS_RTRIGGER] = String("AXIS_RTRIGGER");
    names[AXIS_THROTTLE] = String("AXIS_THROTTLE");
    names[AXIS_RUDDER] = String("AXIS_RUDDER");
    names[AXIS_WHEEL] = String("AXIS_WHEEL");
    names[AXIS_GAS] = String("AXIS_GAS");
    names[AXIS_BRAKE] = String("AXIS_BRAKE");
    names[AXIS_DISTANCE] = String("AXIS_DISTANCE");
    names[AXIS_TILT] = String("AXIS_TILT");
    names[AXIS_GENERIC_1] = String("AXIS_GENERIC_1");
    names[AXIS_GENERIC_2] = String("AXIS_GENERIC_2");
    names[AXIS_GENERIC_3] = String("AXIS_GENERIC_3");
    names[AXIS_GENERIC_4] = String("AXIS_GENERIC_4");
    names[AXIS_GENERIC_5] = String("AXIS_GENERIC_5");
    names[AXIS_GENERIC_6] = String("AXIS_GENERIC_6");
    names[AXIS_GENERIC_7] = String("AXIS_GENERIC_7");
    names[AXIS_GENERIC_8] = String("AXIS_GENERIC_8");
    names[AXIS_GENERIC_9] = String("AXIS_GENERIC_9");
    names[AXIS_GENERIC_10] = String("AXIS_GENERIC_10");
    names[AXIS_GENERIC_11] = String("AXIS_GENERIC_11");
    names[AXIS_GENERIC_12] = String("AXIS_GENERIC_12");
    names[AXIS_GENERIC_13] = String("AXIS_GENERIC_13");
    names[AXIS_GENERIC_14] = String("AXIS_GENERIC_14");
    names[AXIS_GENERIC_15] = String("AXIS_GENERIC_15");
    names[AXIS_GENERIC_16] = String("AXIS_GENERIC_16");

    HashMap<Int32, String>& names2 = TOOL_TYPE_SYMBOLIC_NAMES;
    names2[TOOL_TYPE_UNKNOWN] = String("TOOL_TYPE_UNKNOWN");
    names2[TOOL_TYPE_FINGER] = String("TOOL_TYPE_FINGER");
    names2[TOOL_TYPE_STYLUS] = String("TOOL_TYPE_STYLUS");
    names2[TOOL_TYPE_MOUSE] = String("TOOL_TYPE_MOUSE");
    names2[TOOL_TYPE_ERASER] = String("TOOL_TYPE_ERASER");

    return TRUE;
}

void CMotionEvent::EnsureSharedTempPointerCapacity(
    /* [in] */ Int32 desiredCapacity)
{
    if (gSharedTempPointerCoords == NULL
            || gSharedTempPointerCoords->GetLength() < desiredCapacity) {
        Int32 capacity = gSharedTempPointerCoords != NULL ? gSharedTempPointerCoords->GetLength() : 8;
        while (capacity < desiredCapacity) {
            capacity *= 2;
        }

        gSharedTempPointerCoords = ArrayOf<IPointerCoords*>::Alloc(capacity);
        gSharedTempPointerProperties = ArrayOf<IPointerProperties*>::Alloc(capacity);
        gSharedTempPointerIndexMap = ArrayOf<Int32>::Alloc(capacity);

        for (Int32 i = 0; i < capacity; ++i) {
            AutoPtr<IPointerCoords> coords;
            CPointerCoords::New((IPointerCoords**)&coords);
            gSharedTempPointerCoords->Set(i, coords);

            AutoPtr<IPointerProperties> properties;
            CPointerProperties::New((IPointerProperties**)&properties);
            gSharedTempPointerProperties->Set(i, properties);
        }
    }
}

ECode CMotionEvent::Obtain(
    /* [out] */ CMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    {
        Mutex::Autolock lock(gRecyclerLock);

        if (gRecyclerTop == NULL) {
            return CMotionEvent::NewByFriend(event);
        }
        *event = gRecyclerTop;
        REFCOUNT_ADD(*event)
        gRecyclerTop = (*event)->mNext;
        gRecyclerUsed -= 1;
    }
    (*event)->mNext = NULL;
    (*event)->PrepareForReuse();

    return NOERROR;
}

ECode CMotionEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ ArrayOf<IPointerProperties*>* pointerProperties,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 buttonState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [in] */ Int32 source,
    /* [in] */ Int32 flags,
    /* [out] */ CMotionEvent** event)
{
    FAIL_RETURN(Obtain(event));

    (*event)->mNative = NativeInitialize((*event)->mNative,
            deviceId, source, action, flags, edgeFlags, metaState, buttonState,
            0, 0, xPrecision, yPrecision,
            downTime * NS_PER_MS, eventTime * NS_PER_MS,
            pointerCount, pointerProperties, pointerCoords);
    return NOERROR;
}

ECode CMotionEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ ArrayOf<Int32>* pointerIds,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
    /* [in] */ Int32 metaState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [in] */ Int32 source,
    /* [in] */ Int32 flags,
    /* [out] */ CMotionEvent** event)
{
    Mutex::Autolock lock(gSharedTempLock);

    EnsureSharedTempPointerCapacity(pointerCount);
    ArrayOf<IPointerProperties*>* pp = gSharedTempPointerProperties;
    for (Int32 i = 0; i < pointerCount; i++) {
        AutoPtr<IPointerProperties> obj = (*pp)[i];
        obj->Clear();
        obj->SetId((*pointerIds)[i]);
    }
    return Obtain(downTime, eventTime, action, pointerCount, pp,
            pointerCoords, metaState, 0, xPrecision, yPrecision, deviceId,
            edgeFlags, source, flags, event);
}

ECode CMotionEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size,
    /* [in] */ Int32 metaState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [out] */ CMotionEvent** event)
{
    FAIL_RETURN(Obtain(event));

    Mutex::Autolock lock(gSharedTempLock);

    EnsureSharedTempPointerCapacity(1);

    AutoPtr<IPointerProperties> properties = (*gSharedTempPointerProperties)[0];
    properties->Clear();
    properties->SetId(0);

    AutoPtr<IPointerCoords> coords = (*gSharedTempPointerCoords)[0];
    coords->Clear();
    coords->SetX(x);
    coords->SetY(y);
    coords->SetPressure(pressure);
    coords->SetSize(size);

    ArrayOf<IPointerProperties*>* pp = gSharedTempPointerProperties;
    ArrayOf<IPointerCoords*>* pc = gSharedTempPointerCoords;
    (*event)->mNative = NativeInitialize((*event)->mNative,
        deviceId, IInputDevice::SOURCE_UNKNOWN, action, 0, edgeFlags, metaState, 0,
        0, 0, xPrecision, yPrecision,
        downTime * NS_PER_MS, eventTime * NS_PER_MS,
        1, pp, pc);
    return NOERROR;
}

ECode CMotionEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointers,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size,
    /* [in] */ Int32 metaState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [out] */ CMotionEvent** event)
{
    return Obtain(downTime, eventTime, action, x, y, pressure, size,
            metaState, xPrecision, yPrecision, deviceId, edgeFlags, event);
}

ECode CMotionEvent::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 metaState,
    /* [out] */ CMotionEvent** event)
{
    return Obtain(downTime, eventTime, action, x, y, 1.0f, 1.0f,
            metaState, 1.0f, 1.0f, 0, 0, event);
}

/**
 * Create a new MotionEvent, copying from an existing one.
 */
ECode CMotionEvent::Obtain(
    /* [in] */ CMotionEvent* other,
    /* [out] */ CMotionEvent** event)
{
    if (other == NULL) {
        Logger::E("CMotionEvent", "other motion event must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(Obtain(event));
    (*event)->mNative = NativeCopy((*event)->mNative, other->mNative, TRUE /*keepHistory*/);
    return NOERROR;
}

/**
 * Create a new MotionEvent, copying from an existing one, but not including
 * any historical point information.
 */
ECode CMotionEvent::ObtainNoHistory(
    /* [in] */ CMotionEvent* other,
    /* [out] */ CMotionEvent** event)
{
    if (other == NULL) {
        Logger::E("CMotionEvent", "other motion event must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    FAIL_RETURN(Obtain(event));
    (*event)->mNative = NativeCopy((*event)->mNative, other->mNative, FALSE /*keepHistory*/);
    return NOERROR;
}

CMotionEvent::CMotionEvent()
    : mNative(NULL)
{}

CMotionEvent::~CMotionEvent()
{
    if (mNative != NULL) {
        delete mNative;
    }
}

ECode CMotionEvent::constructor()
{
    return NOERROR;
}

ECode CMotionEvent::Copy(
    /* [out] */ IInputEvent** event)
{
    VALIDATE_NOT_NULL(event);
    AutoPtr<CMotionEvent> motionEvent;
    FAIL_RETURN(Obtain(this, (CMotionEvent**)&motionEvent));
    *event = motionEvent;
    REFCOUNT_ADD(*event)

    return NOERROR;
}

ECode CMotionEvent::Recycle()
{
    InputEvent::Recycle();

    Mutex::Autolock lock(gRecyclerLock);

    if (gRecyclerUsed < MAX_RECYCLED) {
        gRecyclerUsed++;
        mNext = gRecyclerTop;
        gRecyclerTop = this;
    }

    return NOERROR;
}

ECode CMotionEvent::RecycleIfNeededAfterDispatch()
{
    return InputEvent::RecycleIfNeededAfterDispatch();
}

ECode CMotionEvent::Scale(
    /* [in] */ Float scale)
{
    if (scale != 1.0f) {
        NativeScale(mNative, scale);
    }

    return NOERROR;
}

ECode CMotionEvent::GetDeviceId(
    /* [out] */ Int32* deviceId)
{
    VALIDATE_NOT_NULL(deviceId);

    *deviceId = NativeGetDeviceId(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetDevice(
    /* [out] */ IInputDevice** device)
{
    VALIDATE_NOT_NULL(device);

    return InputEvent::GetDevice(device);
}

ECode CMotionEvent::GetSource(
    /* [out] */ Int32* source)
{
    VALIDATE_NOT_NULL(source);

    *source = NativeGetSource(mNative);

    return NOERROR;
}

ECode CMotionEvent::SetSource(
    /* [in] */ Int32 source)
{
    NativeSetSource(mNative, source);

    return NOERROR;
}

ECode CMotionEvent::GetSequenceNumber(
        /* [out] */ Int32* seq)
{
    VALIDATE_NOT_NULL(seq);

    return InputEvent::GetSequenceNumber(seq);
}

ECode CMotionEvent::GetAction(
    /* [out] */ Int32* action)
{
    VALIDATE_NOT_NULL(action);

    *action = NativeGetAction(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetActionMasked(
    /* [out] */ Int32* actionMasked)
{
    VALIDATE_NOT_NULL(actionMasked);

    *actionMasked = NativeGetAction(mNative) & ACTION_MASK;

    return NOERROR;
}

ECode CMotionEvent::GetActionIndex(
    /* [out] */ Int32* actionIndex)
{
    VALIDATE_NOT_NULL(actionIndex);

    *actionIndex = (NativeGetAction(mNative) & ACTION_POINTER_INDEX_MASK)
        >> ACTION_POINTER_INDEX_SHIFT;

    return NOERROR;
}

ECode CMotionEvent::IsTouchEvent(
    /* [out] */ Boolean* isTouchEvent)
{
    VALIDATE_NOT_NULL(isTouchEvent);

    *isTouchEvent = NativeIsTouchEvent(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);

    *flags = NativeGetFlags(mNative);

    return NOERROR;
}

ECode CMotionEvent::IsTainted(
    /* [out] */ Boolean* isTainted)
{
    VALIDATE_NOT_NULL(isTainted);
    Int32 flags;
    GetFlags(&flags);
    *isTainted = (flags & FLAG_TAINTED) != 0;

    return NOERROR;
}

ECode CMotionEvent::SetTainted(
    /* [in] */ Boolean tainted)
{
    Int32 flags;
    GetFlags(&flags);
    NativeSetFlags(mNative, tainted ? flags | FLAG_TAINTED : flags & ~FLAG_TAINTED);

    return NOERROR;
}

ECode CMotionEvent::GetDownTime(
    /* [out] */ Int64* downTime)
{
    VALIDATE_NOT_NULL(downTime);

    *downTime = NativeGetDownTimeNanos(mNative) / NS_PER_MS;

    return NOERROR;
}

/**
 * Sets the time (in ms) when the user originally pressed down to start
 * a stream of position events.
 *
 * @hide
 */
ECode CMotionEvent::SetDownTime(
    /* [in] */ Int64 downTime)
{
    NativeSetDownTimeNanos(mNative, downTime * NS_PER_MS);
    return NOERROR;
}

ECode CMotionEvent::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    VALIDATE_NOT_NULL(eventTime);

    *eventTime = NativeGetEventTimeNanos(mNative, HISTORY_CURRENT) / NS_PER_MS;

    return NOERROR;
}

ECode CMotionEvent::GetEventTimeNano(
    /* [out] */ Int64* eventTimeNano)
{
    VALIDATE_NOT_NULL(eventTimeNano);

    *eventTimeNano = NativeGetEventTimeNanos(mNative, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetX(
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);

    *x = NativeGetAxisValue(mNative, AXIS_X, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetY(
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);

    *y = NativeGetAxisValue(mNative, AXIS_Y, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetPressure(
    /* [out] */ Float* pressure)
{
    VALIDATE_NOT_NULL(pressure);

    *pressure = NativeGetAxisValue(mNative, AXIS_PRESSURE, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetSize(
    /* [out] */ Float* size)
{
    VALIDATE_NOT_NULL(size);

    *size = NativeGetAxisValue(mNative, AXIS_SIZE, 0, HISTORY_CURRENT);

    return NOERROR;
}


ECode CMotionEvent::GetTouchMajor(
    /* [out] */ Float* touchMajor)
{
    VALIDATE_NOT_NULL(touchMajor);

    *touchMajor = NativeGetAxisValue(mNative, AXIS_TOUCH_MAJOR, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetTouchMinor(
    /* [out] */ Float* touchMinor)
{
    VALIDATE_NOT_NULL(touchMinor);

    *touchMinor = NativeGetAxisValue(mNative, AXIS_TOUCH_MINOR, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetToolMajor(
    /* [out] */ Float* toolMajor)
{
    VALIDATE_NOT_NULL(toolMajor);

    *toolMajor = NativeGetAxisValue(mNative, AXIS_TOOL_MAJOR, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetToolMinor(
    /* [out] */ Float* toolMinor)
{
    VALIDATE_NOT_NULL(toolMinor);

    *toolMinor = NativeGetAxisValue(mNative, AXIS_TOOL_MINOR, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetOrientation(
    /* [out] */ Float* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = NativeGetAxisValue(mNative, AXIS_ORIENTATION, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetAxisValue(
    /* [in] */ Int32 axis,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    *value = NativeGetAxisValue(mNative, axis, 0, HISTORY_CURRENT);

    return NOERROR;
}


ECode CMotionEvent::GetPointerCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = NativeGetPointerCount(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetPointerId(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Int32* pointerId)
{
    VALIDATE_NOT_NULL(pointerId);

    *pointerId = NativeGetPointerId(mNative, pointerIndex);

    return NOERROR;
}

ECode CMotionEvent::GetToolType(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Int32* toolType)
{
    VALIDATE_NOT_NULL(toolType);
    *toolType = NativeGetToolType(mNative, pointerIndex);

    return NOERROR;
}

ECode CMotionEvent::FindPointerIndex(
    /* [in] */ Int32 pointerId,
    /* [out] */ Int32* pointerIndex)
{
    VALIDATE_NOT_NULL(pointerIndex);

    *pointerIndex = NativeFindPointerIndex(mNative, pointerId);

    return NOERROR;
}

ECode CMotionEvent::GetX(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);

    *x = NativeGetAxisValue(mNative, AXIS_X, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetY(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);

    *y = NativeGetAxisValue(mNative, AXIS_Y, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetPressure(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* pressure)
{
    VALIDATE_NOT_NULL(pressure);

    *pressure = NativeGetAxisValue(mNative, AXIS_PRESSURE, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetSize(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* size)
{
    VALIDATE_NOT_NULL(size);

    *size = NativeGetAxisValue(mNative, AXIS_SIZE, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetTouchMajor(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* touchMajor)
{
    VALIDATE_NOT_NULL(touchMajor);

    *touchMajor = NativeGetAxisValue(mNative, AXIS_TOUCH_MAJOR, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetTouchMinor(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* touchMinor)
{
    VALIDATE_NOT_NULL(touchMinor);

    *touchMinor = NativeGetAxisValue(mNative, AXIS_TOUCH_MINOR, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetToolMajor(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* toolMajor)
{
    VALIDATE_NOT_NULL(toolMajor);

    *toolMajor = NativeGetAxisValue(mNative, AXIS_TOOL_MAJOR, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetToolMinor(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* toolMinor)
{
    VALIDATE_NOT_NULL(toolMinor);

    *toolMinor = NativeGetAxisValue(mNative, AXIS_TOOL_MINOR, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetOrientation(
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = NativeGetAxisValue(mNative, AXIS_ORIENTATION, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetAxisValue(
    /* [in] */ Int32 axis,
    /* [in] */ Int32 pointerIndex,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = NativeGetAxisValue(mNative, axis, pointerIndex, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetPointerCoords(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ IPointerCoords* outPointerCoords)
{
    assert(outPointerCoords);

    NativeGetPointerCoords(mNative, pointerIndex, HISTORY_CURRENT, outPointerCoords);

    return NOERROR;
}

ECode CMotionEvent::GetPointerProperties(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ IPointerProperties* outPointerProperties)
{
    assert(outPointerProperties);
    NativeGetPointerProperties(mNative, pointerIndex, outPointerProperties);

    return NOERROR;
}

ECode CMotionEvent::GetMetaState(
    /* [out] */ Int32* metaState)
{
    VALIDATE_NOT_NULL(metaState);

    *metaState = NativeGetMetaState(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetButtonState(
    /* [out] */ Int32* buttonState)
{
    VALIDATE_NOT_NULL(buttonState);

    *buttonState = NativeGetButtonState(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetRawX(
    /* [out] */ Float* rawX)
{
    VALIDATE_NOT_NULL(rawX);

    *rawX = NativeGetRawAxisValue(mNative, AXIS_X, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetRawY(
    /* [out] */ Float* rawY)
{
    VALIDATE_NOT_NULL(rawY);

    *rawY = NativeGetRawAxisValue(mNative, AXIS_Y, 0, HISTORY_CURRENT);

    return NOERROR;
}

ECode CMotionEvent::GetXPrecision(
    /* [out] */ Float* xPrecision)
{
    VALIDATE_NOT_NULL(xPrecision);

    *xPrecision = NativeGetXPrecision(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetYPrecision(
    /* [out] */ Float* yPrecision)
{
    VALIDATE_NOT_NULL(yPrecision);

    *yPrecision = NativeGetYPrecision(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetHistorySize(
    /* [out] */ Int32* historySize)
{
    VALIDATE_NOT_NULL(historySize);

    *historySize = NativeGetHistorySize(mNative);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalEventTime(
    /* [in] */ Int32 pos,
    /* [out] */ Int64* hEventTime)
{
    VALIDATE_NOT_NULL(hEventTime);

    *hEventTime = NativeGetEventTimeNanos(mNative, pos) / NS_PER_MS;

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalEventTimeNano(
    /* [in] */ Int32 pos,
    /* [out] */ Int64* eventTimeNano)
{
    VALIDATE_NOT_NULL(eventTimeNano);

    *eventTimeNano = NativeGetEventTimeNanos(mNative, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalX(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hX)
{
    VALIDATE_NOT_NULL(hX);

    *hX = NativeGetAxisValue(mNative, AXIS_X, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalY(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hY)
{
    VALIDATE_NOT_NULL(hY);

    *hY = NativeGetAxisValue(mNative, AXIS_Y, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalPressure(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hPressure)
{
    VALIDATE_NOT_NULL(hPressure);

    *hPressure = NativeGetAxisValue(mNative, AXIS_PRESSURE, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalSize(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hSize)
{
    VALIDATE_NOT_NULL(hSize);

    *hSize = NativeGetAxisValue(mNative, AXIS_SIZE, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalTouchMajor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMajor)
{
    VALIDATE_NOT_NULL(hTouchMajor);

    *hTouchMajor = NativeGetAxisValue(mNative, AXIS_TOUCH_MAJOR, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalTouchMinor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMinor)
{
    VALIDATE_NOT_NULL(hTouchMinor);

    *hTouchMinor = NativeGetAxisValue(mNative, AXIS_TOUCH_MINOR, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalToolMajor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMajor)
{
    VALIDATE_NOT_NULL(hToolMajor);

    *hToolMajor = NativeGetAxisValue(mNative, AXIS_TOOL_MAJOR, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalToolMinor(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMinor)
{
    VALIDATE_NOT_NULL(hToolMinor);

    *hToolMinor = NativeGetAxisValue(mNative, AXIS_TOOL_MINOR, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalOrientation(
    /* [in] */ Int32 pos,
    /* [out] */ Float* hOrientation)
{
    VALIDATE_NOT_NULL(hOrientation);

    *hOrientation = NativeGetAxisValue(mNative, AXIS_ORIENTATION, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalAxisValue(
    /* [in] */ Int32 axis,
    /* [in] */ Int32 pos,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    *value = NativeGetAxisValue(mNative, axis, 0, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalX(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hX)
{
    VALIDATE_NOT_NULL(hX);

    *hX = NativeGetAxisValue(mNative, AXIS_X, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalY(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hY)
{
    VALIDATE_NOT_NULL(hY);

    *hY = NativeGetAxisValue(mNative, AXIS_Y, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalPressure(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hPressure)
{
    VALIDATE_NOT_NULL(hPressure);

    *hPressure = NativeGetAxisValue(mNative, AXIS_PRESSURE, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalSize(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hSize)
{
    VALIDATE_NOT_NULL(hSize);

    *hSize = NativeGetAxisValue(mNative, AXIS_SIZE, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalTouchMajor(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMajor)
{
    VALIDATE_NOT_NULL(hTouchMajor);

    *hTouchMajor = NativeGetAxisValue(mNative, AXIS_TOUCH_MAJOR, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalTouchMinor(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hTouchMinor)
{
    VALIDATE_NOT_NULL(hTouchMinor);

    *hTouchMinor = NativeGetAxisValue(mNative, AXIS_TOUCH_MINOR, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalToolMajor(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMajor)
{
    VALIDATE_NOT_NULL(hToolMajor);

    *hToolMajor = NativeGetAxisValue(mNative, AXIS_TOOL_MAJOR, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalToolMinor(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hToolMinor)
{
    VALIDATE_NOT_NULL(hToolMinor);

    *hToolMinor = NativeGetAxisValue(mNative, AXIS_TOOL_MINOR, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalOrientation(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* hOrientation)
{
    VALIDATE_NOT_NULL(hOrientation);

    *hOrientation = NativeGetAxisValue(mNative, AXIS_ORIENTATION, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalAxisValue(
    /* [in] */ Int32 axis,
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    *value = NativeGetAxisValue(mNative, axis, pointerIndex, pos);

    return NOERROR;
}

ECode CMotionEvent::GetHistoricalPointerCoords(
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 pos,
    /* [in] */ IPointerCoords* outPointerCoords)
{
    VALIDATE_NOT_NULL(outPointerCoords);

    NativeGetPointerCoords(mNative, pointerIndex, pos, outPointerCoords);

    return NOERROR;
}

ECode CMotionEvent::GetEdgeFlags(
    /* [out] */ Int32* edgeFlags)
{
    VALIDATE_NOT_NULL(edgeFlags);

    *edgeFlags = NativeGetEdgeFlags(mNative);

    return NOERROR;
}

ECode CMotionEvent::SetEdgeFlags(
    /* [in] */ Int32 flags)
{
    NativeSetEdgeFlags(mNative, flags);

    return NOERROR;
}

ECode CMotionEvent::SetAction(
    /* [in] */ Int32 action)
{
    NativeSetAction(mNative, action);

    return NOERROR;
}

ECode CMotionEvent::OffsetLocation(
    /* [in] */ Float deltaX,
    /* [in] */ Float deltaY)
{
    NativeOffsetLocation(mNative, deltaX, deltaY);

    return NOERROR;
}

ECode CMotionEvent::SetLocation(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Float oldX, oldY;
    GetX(&oldX);
    GetY(&oldY);
    NativeOffsetLocation(mNative, x - oldX, y - oldY);

    return NOERROR;
}

ECode CMotionEvent::Transform(
    /* [in] */ IMatrix* matrix)
{
    if (matrix == NULL) {
        Logger::E("CMotionEvent", "matrix must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    NativeTransform(mNative, matrix);

    return NOERROR;
}

ECode CMotionEvent::AddBatch(
    /* [in] */ Int64 eventTime,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size,
    /* [in] */ Int32 metaState)
{
    Mutex::Autolock lock(gSharedTempLock);

    EnsureSharedTempPointerCapacity(1);

    AutoPtr<IPointerCoords> coords = (*gSharedTempPointerCoords)[0];
    coords->Clear();
    coords->SetX(x);
    coords->SetY(y);
    coords->SetPressure(pressure);
    coords->SetSize(size);

    NativeAddBatch(mNative, eventTime * NS_PER_MS, gSharedTempPointerCoords, metaState);

    return NOERROR;
}

ECode CMotionEvent::AddBatch(
    /* [in] */ Int64 eventTime,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
    /* [in] */ Int32 metaState)
{
    NativeAddBatch(mNative, eventTime * NS_PER_MS, pointerCoords, metaState);

    return NOERROR;
}

/**
 * Adds all of the movement samples of the specified event to this one if
 * it is compatible.  To be compatible, the event must have the same device id,
 * source, action, flags, pointer count, pointer properties.
 *
 * Only applies to {@link #ACTION_MOVE} or {@link #ACTION_HOVER_MOVE} events.
 *
 * @param event The event whose movements samples should be added to this one
 * if possible.
 * @return True if batching was performed or FALSE if batching was not possible.
 * @hide
 */
ECode CMotionEvent::AddBatch(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    android::MotionEvent* eventNative = ((CMotionEvent*)event)->mNative;
    const Int32 action = NativeGetAction(mNative);
    if (action != ACTION_MOVE && action != ACTION_HOVER_MOVE) {
        *res = FALSE;
        return NOERROR;
    }
    if (action != NativeGetAction(eventNative)) {
        *res = FALSE;
        return NOERROR;
    }

    if (NativeGetDeviceId(mNative) != NativeGetDeviceId(eventNative)
        || NativeGetSource(mNative) != NativeGetSource(eventNative)
        || NativeGetFlags(mNative) != NativeGetFlags(eventNative)) {
        *res = FALSE;
        return NOERROR;
    }

    const Int32 pointerCount = NativeGetPointerCount(mNative);
    if (pointerCount != NativeGetPointerCount(eventNative)) {
        *res = FALSE;
        return NOERROR;
    }

    {
        Mutex::Autolock lock(gSharedTempLock);
        EnsureSharedTempPointerCapacity(Elastos::Core::Math::Max(pointerCount, 2));

        ArrayOf<IPointerProperties*>* pp = gSharedTempPointerProperties;
        ArrayOf<IPointerCoords*>* pc = gSharedTempPointerCoords;

        for (Int32 i = 0; i < pointerCount; i++) {
            NativeGetPointerProperties(mNative, i, (*pp)[0]);
            NativeGetPointerProperties(eventNative, i, (*pp)[1]);
            Boolean isEquals;
            (*pp)[0]->Equals((*pp)[1], &isEquals);
            if (!isEquals) {
                *res = FALSE;
                return NOERROR;
            }
        }

        const Int32 metaState = NativeGetMetaState(eventNative);
        const Int32 historySize = NativeGetHistorySize(eventNative);
        for (Int32 h = 0; h <= historySize; h++) {
            const Int32 historyPos = (h == historySize ? HISTORY_CURRENT : h);

            for (Int32 i = 0; i < pointerCount; i++) {
                NativeGetPointerCoords(eventNative, i, historyPos, (*pc)[i]);
            }

            const Int64 eventTimeNanos = NativeGetEventTimeNanos(eventNative, historyPos);
            NativeAddBatch(mNative, eventTimeNanos, pc, metaState);
        }
    }
    *res = TRUE;
    return NOERROR;
}

ECode CMotionEvent::IsWithinBoundsNoHistory(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    const Int32 pointerCount = NativeGetPointerCount(mNative);
    for (Int32 i = 0; i < pointerCount; i++) {
        const Float x = NativeGetAxisValue(mNative, AXIS_X, i, HISTORY_CURRENT);
        const Float y = NativeGetAxisValue(mNative, AXIS_Y, i, HISTORY_CURRENT);
        if (x < left || x > right || y < top || y > bottom) {
            *res = FALSE;
            return NOERROR;
        }
    }
    *res = TRUE;
    return NOERROR;
}

Float CMotionEvent::Clamp(
    /* [in] */ Float value,
    /* [in] */ Float low,
    /* [in] */ Float high)
{
    if (value < low) {
        return low;
    }
    else if (value > high) {
        return high;
    }
    return value;
}

ECode CMotionEvent::ClampNoHistory(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);

    AutoPtr<CMotionEvent> ev;
    FAIL_RETURN(Obtain((CMotionEvent**)&ev));

    Mutex::Autolock lock(gSharedTempLock);
    const Int32 pointerCount = NativeGetPointerCount(mNative);

    EnsureSharedTempPointerCapacity(pointerCount);
    ArrayOf<IPointerProperties*>* pp = gSharedTempPointerProperties;
    ArrayOf<IPointerCoords*>* pc = gSharedTempPointerCoords;
    Float x, y;
    for (Int32 i = 0; i < pointerCount; i++) {
        NativeGetPointerProperties(mNative, i, (*pp)[i]);
        AutoPtr<IPointerCoords> coords = (*pc)[i];
        NativeGetPointerCoords(mNative, i, HISTORY_CURRENT, coords);

        coords->GetX(&x);
        coords->GetY(&y);
        coords->SetX(Clamp(x, left, right));
        coords->SetY(Clamp(y, top, bottom));
    }

    ev->mNative = NativeInitialize(ev->mNative,
        NativeGetDeviceId(mNative), NativeGetSource(mNative),
        NativeGetAction(mNative), NativeGetFlags(mNative),
        NativeGetEdgeFlags(mNative), NativeGetMetaState(mNative),
        NativeGetButtonState(mNative),
        NativeGetXOffset(mNative), NativeGetYOffset(mNative),
        NativeGetXPrecision(mNative), NativeGetYPrecision(mNative),
        NativeGetDownTimeNanos(mNative),
        NativeGetEventTimeNanos(mNative, HISTORY_CURRENT),
        pointerCount, pp, pc);
    *event = ev;
    REFCOUNT_ADD(*event)

    return NOERROR;
}

/**
 * Gets an integer where each pointer id present in the event is marked as a bit.
 * @hide
 */
ECode CMotionEvent::GetPointerIdBits(
    /* [out] */ Int32* idBits)
{
    VALIDATE_NOT_NULL(idBits);

    *idBits = 0;
    const Int32 pointerCount = NativeGetPointerCount(mNative);
    for (Int32 i = 0; i < pointerCount; i++) {
        *idBits |= 1 << NativeGetPointerId(mNative, i);
    }
    return NOERROR;
}

/**
 * Splits a motion event such that it includes only a subset of pointer ids.
 * @hide
 */
ECode CMotionEvent::Split(
    /* [in] */ Int32 idBits,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);

    AutoPtr<CMotionEvent> ev;
    FAIL_RETURN(Obtain((CMotionEvent**)&ev));

    Mutex::Autolock lock(gSharedTempLock);

    const Int32 oldPointerCount = NativeGetPointerCount(mNative);
    EnsureSharedTempPointerCapacity(oldPointerCount);
    ArrayOf<IPointerProperties*>* pp = gSharedTempPointerProperties;
    ArrayOf<IPointerCoords*>* pc = gSharedTempPointerCoords;
    ArrayOf<Int32>* map = gSharedTempPointerIndexMap;

    const Int32 oldAction = NativeGetAction(mNative);
    const Int32 oldActionMasked = oldAction & ACTION_MASK;
    const Int32 oldActionPointerIndex = (oldAction & ACTION_POINTER_INDEX_MASK)
            >> ACTION_POINTER_INDEX_SHIFT;
    Int32 newActionPointerIndex = -1;
    Int32 newPointerCount = 0;
    Int32 newIdBits = 0;
    Int32 id;
    for (Int32 i = 0; i < oldPointerCount; i++) {
        AutoPtr<IPointerProperties> ppObj = (*pp)[newPointerCount];
        NativeGetPointerProperties(mNative, i, ppObj);
        ppObj->GetId(&id);
        const Int32 idBit = 1 << id;
        if ((idBit & idBits) != 0) {
            if (i == oldActionPointerIndex) {
                newActionPointerIndex = newPointerCount;
            }
            (*map)[newPointerCount] = i;
            newPointerCount += 1;
            newIdBits |= idBit;
        }
    }

    if (newPointerCount == 0) {
        Logger::E("CMotionEvent", "idBits did not match any ids in the event");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 newAction;
    if (oldActionMasked == ACTION_POINTER_DOWN || oldActionMasked == ACTION_POINTER_UP) {
        if (newActionPointerIndex < 0) {
            // An unrelated pointer changed.
            newAction = ACTION_MOVE;
        }
        else if (newPointerCount == 1) {
            // The first/last pointer went down/up.
            newAction = oldActionMasked == ACTION_POINTER_DOWN
                    ? ACTION_DOWN : ACTION_UP;
        }
        else {
            // A secondary pointer went down/up.
            newAction = oldActionMasked
                    | (newActionPointerIndex << ACTION_POINTER_INDEX_SHIFT);
        }
    }
    else {
        // Simple up/down/cancel/move or other motion action.
        newAction = oldAction;
    }

    const Int32 historySize = NativeGetHistorySize(mNative);
    for (Int32 h = 0; h <= historySize; h++) {
        const Int32 historyPos = h == historySize ? HISTORY_CURRENT : h;

        for (Int32 i = 0; i < newPointerCount; i++) {
            NativeGetPointerCoords(mNative, (*map)[i], historyPos, (*pc)[i]);
        }

        const Int64 eventTimeNanos = NativeGetEventTimeNanos(mNative, historyPos);
        if (h == 0) {
            ev->mNative = NativeInitialize(ev->mNative,
                NativeGetDeviceId(mNative), NativeGetSource(mNative),
                newAction, NativeGetFlags(mNative),
                NativeGetEdgeFlags(mNative), NativeGetMetaState(mNative),
                NativeGetButtonState(mNative),
                NativeGetXOffset(mNative), NativeGetYOffset(mNative),
                NativeGetXPrecision(mNative), NativeGetYPrecision(mNative),
                NativeGetDownTimeNanos(mNative), eventTimeNanos,
                newPointerCount, pp, pc);
        }
        else {
            NativeAddBatch(ev->mNative, eventTimeNanos, pc, 0);
        }
    }
    *event = ev;
    REFCOUNT_ADD(*event)

    return NOERROR;
}

String CMotionEvent::ToString()
{
    AutoPtr<StringBuilder> msg = new StringBuilder();
    msg->AppendCStr("MotionEvent { action=");
    Int32 action;
    GetAction(&action);
    msg->AppendString(ActionToString(action));

    Int32 pointerCount;
    GetPointerCount(&pointerCount);
    for (Int32 i = 0; i < pointerCount; i++) {
        msg->AppendCStr(", id[");
        msg->AppendInt32(i);
        msg->AppendCStr("]=");
        Int32 id;
        GetPointerId(i, &id);
        msg->AppendInt32(id);
        msg->AppendCStr(", x[");
        msg->AppendInt32(i);
        msg->AppendCStr("]=");
        Float x, y;
        GetX(i, &x);
        GetY(i, &y);
        msg->AppendFloat(x);
        msg->AppendCStr(", y[");
        msg->AppendInt32(i);
        msg->AppendCStr("]=");
        msg->AppendFloat(y);
        msg->AppendCStr(", toolType[");
        msg->AppendInt32(i);
        msg->AppendCStr("]=");
        Int32 toolType;
        GetToolType(i, &toolType);
        msg->AppendString(ToolTypeToString(toolType));
    }

    msg->AppendCStr(", buttonState=");
    Int32 buttonState;
    GetButtonState(&buttonState);
    msg->AppendString(ButtonStateToString(buttonState));
    msg->AppendCStr(", metaState=");
    Int32 metaState;
    GetMetaState(&metaState);
    msg->AppendString(CKeyEvent::MetaStateToString(metaState));
    msg->AppendCStr(", flags=0x");
    Int32 flags;
    GetFlags(&flags);
    msg->AppendString(StringUtils::Int32ToString(flags, 16));
    msg->AppendCStr(", edgeFlags=0x");
    GetEdgeFlags(&flags);
    msg->AppendString(StringUtils::Int32ToString(flags, 16));
    msg->AppendCStr(", pointerCount=");
    msg->AppendInt32(pointerCount);
    msg->AppendCStr(", historySize=");
    Int32 size;
    GetHistorySize(&size);
    msg->AppendInt32(size);
    msg->AppendCStr(", eventTime=");
    Int64 eventTime;
    GetEventTime(&eventTime);
    msg->AppendInt64(eventTime);
    msg->AppendCStr(", downTime=");
    GetDownTime(&eventTime);
    msg->AppendInt64(eventTime);
    msg->AppendCStr(", deviceId=");
    Int32 id;
    GetDeviceId(&id);
    msg->AppendInt32(id);
    msg->AppendCStr(", source=0x");
    Int32 source;
    GetSource(&source);
    msg->AppendString(StringUtils::Int32ToString(source));
    msg->AppendCStr(" }");
    return msg->ToString();
}

String CMotionEvent::ActionToString(
    /* [in] */ Int32 action)
{
    switch (action) {
        case ACTION_DOWN:
            return String("ACTION_DOWN");
        case ACTION_UP:
            return String("ACTION_UP");
        case ACTION_CANCEL:
            return String("ACTION_CANCEL");
        case ACTION_OUTSIDE:
            return String("ACTION_OUTSIDE");
        case ACTION_MOVE:
            return String("ACTION_MOVE");
        case ACTION_HOVER_MOVE:
            return String("ACTION_HOVER_MOVE");
        case ACTION_SCROLL:
            return String("ACTION_SCROLL");
        case ACTION_HOVER_ENTER:
            return String("ACTION_HOVER_ENTER");
        case ACTION_HOVER_EXIT:
            return String("ACTION_HOVER_EXIT");
    }
    Int32 index = (action & ACTION_POINTER_INDEX_MASK) >> ACTION_POINTER_INDEX_SHIFT;
    switch (action & ACTION_MASK) {
        case ACTION_POINTER_DOWN:
            return String("ACTION_POINTER_DOWN()") + StringUtils::Int32ToString(index) + ")";
        case ACTION_POINTER_UP:
            return String("ACTION_POINTER_UP()") + StringUtils::Int32ToString(index) + ")";
        default:
            return StringUtils::Int32ToString(action);
    }
}

String CMotionEvent::AxisToString(
    /* [in] */ Int32 axis)
{
    String symbolicName;
    HashMap<Int32, String>::Iterator find = AXIS_SYMBOLIC_NAMES.Find(axis);
    if (find != AXIS_SYMBOLIC_NAMES.End()) {
        symbolicName = find->mSecond;
    }

    return symbolicName.IsNull() ? StringUtils::Int32ToString(axis) : symbolicName;
}

ECode CMotionEvent::AxisFromString(
    /* [in] */ const String& symbolicName,
    /* [out] */ Int32* axis)
{
    VALIDATE_NOT_NULL(axis);
    *axis = -1;
    if (symbolicName.IsNull()) {
        Logger::E("CMotionEvent", "symbolicName must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    HashMap<Int32, String>::Iterator iter = AXIS_SYMBOLIC_NAMES.Begin();
    for (Int32 i = 0; iter != AXIS_SYMBOLIC_NAMES.End(); ++iter, i++) {
        if (symbolicName.Equals(iter->mSecond)) {
            *axis = i;
            return NOERROR;
        }
    }

    *axis = StringUtils::ParseInt32(symbolicName);
    return NOERROR;
}

String CMotionEvent::ButtonStateToString(
    /* [in] */ Int32 buttonState)
{
    if (buttonState == 0) {
        return String("0");
    }
    AutoPtr<StringBuilder> result;
    Int32 i = 0;
    while (buttonState != 0) {
        Boolean isSet = (buttonState & 1) != 0;
        buttonState = (buttonState >> 1) & 0x7FFFFFFF;; // unsigned shift!
        if (isSet) {
            String name = BUTTON_SYMBOLIC_NAMES[i];
            if (result == NULL) {
                if (buttonState == 0) {
                    return name;
                }
                result = new StringBuilder(name);
            }
            else {
                result->AppendCStr("|");
                result->AppendString(name);
            }
        }
        i += 1;
    }
    return result->ToString();
}

String CMotionEvent::ToolTypeToString(
    /* [in] */ Int32 toolType)
{
    String symbolicName;
    HashMap<Int32, String>::Iterator find = TOOL_TYPE_SYMBOLIC_NAMES.Find(toolType);
    if (find != TOOL_TYPE_SYMBOLIC_NAMES.End()) {
        symbolicName = find->mSecond;
    }

    return symbolicName.IsNull() ? StringUtils::Int32ToString(toolType) : symbolicName;
}

/** @hide */
ECode CMotionEvent::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ CMotionEvent** event)
{
    FAIL_RETURN(Obtain(event));
    (*event)->mNative = NativeReadFromParcel((*event)->mNative, in);
    return NOERROR;
}

ECode CMotionEvent::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt32(PARCEL_TOKEN_MOTION_EVENT);
    NativeWriteToParcel(mNative, out);

    return NOERROR;
}

ECode CMotionEvent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Int32 parcelToken;
    source->ReadInt32(&parcelToken); // skip token, we already know this is a MotionEvent
    mNative = NativeReadFromParcel(mNative, source);

    return NOERROR;
}

ECode CMotionEvent::GetNative(
    /* [out] */ Handle32* native)
{
    VALIDATE_NOT_NULL(native);
    *native = (Handle32)mNative;

    return NOERROR;
}

ECode CMotionEvent::SetNative(
    /* [in] */ Handle32 native)
{
    if (mNative) {
        delete mNative;
    }
    mNative = (android::MotionEvent*)native;

    return NOERROR;
}

android::MotionEvent* CMotionEvent::NativeInitialize(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 source,
    /* [in] */ Int32 action,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 edgeFlags,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 buttonState,
    /* [in] */ Float xOffset,
    /* [in] */ Float yOffset,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int64 downTimeNanos,
    /* [in] */ Int64 eventTimeNanos,
    /* [in] */ Int32 pointerCount,
    /* [in] */ ArrayOf<IPointerProperties*>* pointerPropertiesObjArray,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoordsObjArray)
{
    if (!ValidatePointerCount(pointerCount)
            || !ValidatePointerPropertiesArray(pointerPropertiesObjArray, pointerCount)
            || !ValidatePointerCoordsObjArray(pointerCoordsObjArray, pointerCount)) {
        return 0;
    }

    android::MotionEvent* event = nativePtr;
    if (!event) {
        event = new android::MotionEvent();
    }

    android::PointerProperties pointerProperties[pointerCount];
    android::PointerCoords rawPointerCoords[pointerCount];

    for (Int32 i = 0; i < pointerCount; i++) {
        AutoPtr<IPointerProperties> pointerPropertiesObj = (*pointerPropertiesObjArray)[i];
        if (!pointerPropertiesObj) {
            goto Error;
        }
        PointerPropertiesToNative(pointerPropertiesObj, &pointerProperties[i]);

        AutoPtr<IPointerCoords> pointerCoordsObj = (*pointerCoordsObjArray)[i];
        if (!pointerCoordsObj) {
            // jniThrowNullPointerException(env, "pointerCoords");
            goto Error;
        }
        PointerCoordsToNative(pointerCoordsObj, xOffset, yOffset, &rawPointerCoords[i]);
    }

    event->initialize(deviceId, source, action, flags, edgeFlags, metaState, buttonState,
            xOffset, yOffset, xPrecision, yPrecision,
            downTimeNanos, eventTimeNanos, pointerCount, pointerProperties, rawPointerCoords);

    return event;

Error:
    if (!nativePtr) {
        delete event;
    }
    return NULL;
}

android::MotionEvent* CMotionEvent::NativeCopy(
    /* [in] */ android::MotionEvent* destNativePtr,
    /* [in] */ android::MotionEvent* sourceNativePtr,
    /* [in] */ Boolean keepHistory)
{
    android::MotionEvent* destEvent = destNativePtr;
    if (!destEvent) {
        destEvent = new android::MotionEvent();
    }
    android::MotionEvent* sourceEvent = sourceNativePtr;
    destEvent->copyFrom(sourceEvent, keepHistory);
    return destEvent;
}

void CMotionEvent::NativeAddBatch(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int64 eventTimeNanos,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoordsObjArray,
    /* [in] */ Int32 metaState)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerCoordsObjArray(pointerCoordsObjArray, pointerCount)) {
        return;
    }

    android::PointerCoords rawPointerCoords[pointerCount];

    for (size_t i = 0; i < pointerCount; i++) {
        AutoPtr<IPointerCoords> pointerCoordsObj = (*pointerCoordsObjArray)[i];
        if (!pointerCoordsObj) {
            assert(0);
            // jniThrowNullPointerException(env, "pointerCoords");
            return;
        }
        PointerCoordsToNative(pointerCoordsObj,
                event->getXOffset(), event->getYOffset(), &rawPointerCoords[i]);
    }

    event->addSample(eventTimeNanos, rawPointerCoords);
    event->setMetaState(event->getMetaState() | metaState);
}

Int32 CMotionEvent::NativeGetDeviceId(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getDeviceId();
}

Int32 CMotionEvent::NativeGetSource(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getSource();
}

void CMotionEvent::NativeSetSource(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 source)
{
    android::MotionEvent* event = nativePtr;
    event->setSource(source);
}

Int32 CMotionEvent::NativeGetAction(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getAction();
}

void CMotionEvent::NativeSetAction(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 action)
{
    android::MotionEvent* event = nativePtr;
    event->setAction(action);
}

Boolean CMotionEvent::NativeIsTouchEvent(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->isTouchEvent();
}

Int32 CMotionEvent::NativeGetFlags(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getFlags();
}

void CMotionEvent::NativeSetFlags(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 flags)
{
    android::MotionEvent* event = nativePtr;
    event->setFlags(flags);
}

Int32 CMotionEvent::NativeGetEdgeFlags(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getEdgeFlags();
}

void CMotionEvent::NativeSetEdgeFlags(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 edgeFlags)
{
    android::MotionEvent* event = nativePtr;
    event->setEdgeFlags(edgeFlags);
}

Int32 CMotionEvent::NativeGetMetaState(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getMetaState();
}

Int32 CMotionEvent::NativeGetButtonState(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getButtonState();
}

void CMotionEvent::NativeOffsetLocation(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Float deltaX,
    /* [in] */ Float deltaY)
{
    android::MotionEvent* event = nativePtr;
    return event->offsetLocation(deltaX, deltaY);
}

Float CMotionEvent::NativeGetXOffset(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getXOffset();
}

Float CMotionEvent::NativeGetYOffset(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getYOffset();
}

Float CMotionEvent::NativeGetXPrecision(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getXPrecision();
}

Float CMotionEvent::NativeGetYPrecision(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getYPrecision();
}

Int64 CMotionEvent::NativeGetDownTimeNanos(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return event->getDownTime();
}

void CMotionEvent::NativeSetDownTimeNanos(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int64 downTimeNanos)
{
    android::MotionEvent* event = nativePtr;
    event->setDownTime(downTimeNanos);
}

Int32 CMotionEvent::NativeGetPointerCount(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return Int32(event->getPointerCount());
}

Int32 CMotionEvent::NativeGetPointerId(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 pointerIndex)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerIndex(pointerIndex, pointerCount)) {
        return -1;
    }
    return event->getPointerId(pointerIndex);
}

Int32 CMotionEvent::NativeGetToolType(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 pointerIndex)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerIndex(pointerIndex, pointerCount)) {
        return -1;
    }
    return event->getToolType(pointerIndex);
}

Int32 CMotionEvent::NativeFindPointerIndex(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 pointerId)
{
    android::MotionEvent* event = nativePtr;
    return Int32(event->findPointerIndex(pointerId));
}

Int32 CMotionEvent::NativeGetHistorySize(
    /* [in] */ android::MotionEvent* nativePtr)
{
    android::MotionEvent* event = nativePtr;
    return Int32(event->getHistorySize());
}

Int64 CMotionEvent::NativeGetEventTimeNanos(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 historyPos)
{
    android::MotionEvent* event = nativePtr;
    if (historyPos == HISTORY_CURRENT) {
        return event->getEventTime();
    }
    else {
        size_t historySize = event->getHistorySize();
        if (!ValidateHistoryPos(historyPos, historySize)) {
            return 0;
        }
        return event->getHistoricalEventTime(historyPos);
    }
}

Float CMotionEvent::NativeGetRawAxisValue(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 axis,
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 historyPos)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerIndex(pointerIndex, pointerCount)) {
        return 0;
    }

    if (historyPos == HISTORY_CURRENT) {
        return event->getRawAxisValue(axis, pointerIndex);
    }
    else {
        size_t historySize = event->getHistorySize();
        if (!ValidateHistoryPos(historyPos, historySize)) {
            return 0;
        }
        return event->getHistoricalRawAxisValue(axis, pointerIndex, historyPos);
    }
}

Float CMotionEvent::NativeGetAxisValue(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 axis,
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 historyPos)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerIndex(pointerIndex, pointerCount)) {
        return 0;
    }

    if (historyPos == HISTORY_CURRENT) {
        return event->getAxisValue(axis, pointerIndex);
    }
    else {
        size_t historySize = event->getHistorySize();
        if (!ValidateHistoryPos(historyPos, historySize)) {
            return 0;
        }
        return event->getHistoricalAxisValue(axis, pointerIndex, historyPos);
    }
}

void CMotionEvent::NativeGetPointerCoords(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 pointerIndex,
    /* [in] */ Int32 historyPos,
    /* [in, out] */ IPointerCoords* outPointerCoordsObj)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerIndex(pointerIndex, pointerCount)
            || !ValidatePointerCoords(outPointerCoordsObj)) {
        return;
    }

    const android::PointerCoords* rawPointerCoords;
    if (historyPos == HISTORY_CURRENT) {
        rawPointerCoords = event->getRawPointerCoords(pointerIndex);
    }
    else {
        size_t historySize = event->getHistorySize();
        if (!ValidateHistoryPos(historyPos, historySize)) {
            return;
        }
        rawPointerCoords = event->getHistoricalRawPointerCoords(pointerIndex, historyPos);
    }
    PointerCoordsFromNative(rawPointerCoords, event->getXOffset(), event->getYOffset(),
        outPointerCoordsObj);
}

void CMotionEvent::NativeGetPointerProperties(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Int32 pointerIndex,
    /* [in, out] */ IPointerProperties* outPointerPropertiesObj)
{
    android::MotionEvent* event = nativePtr;
    size_t pointerCount = event->getPointerCount();
    if (!ValidatePointerIndex(pointerIndex, pointerCount)
            || !ValidatePointerProperties(outPointerPropertiesObj)) {
        return;
    }

    const android::PointerProperties* pointerProperties = event->getPointerProperties(pointerIndex);
    PointerPropertiesFromNative(pointerProperties, outPointerPropertiesObj);
}

void CMotionEvent::NativeScale(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ Float scale)
{
    android::MotionEvent* event = nativePtr;
    event->scale(scale);
}

void CMotionEvent::NativeTransform(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ IMatrix* matrixObj)
{
    SkMatrix* matrix = (SkMatrix*)((CMatrix*)matrixObj)->Ni();
    android::MotionEvent* event = nativePtr;
    event->transform(matrix);
}

android::MotionEvent* CMotionEvent::NativeReadFromParcel(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ IParcel* parcelObj)
{
    android::MotionEvent* event = nativePtr;
    if (!event) {
        event = new android::MotionEvent();
    }

    android::Parcel* parcel;
    parcelObj->GetElementPayload((Handle32*)&parcel);

    android::status_t status = event->readFromParcel(parcel);
    if (status) {
        if (!nativePtr) {
            delete event;
        }
        assert(0);
//        jniThrowRuntimeException(env, "Failed to read MotionEvent parcel.");
        return 0;
    }
    return event;
}

void CMotionEvent::NativeWriteToParcel(
    /* [in] */ android::MotionEvent* nativePtr,
    /* [in] */ IParcel* parcelObj)
{
    android::MotionEvent* event = nativePtr;
    android::Parcel* parcel;
    parcelObj->GetElementPayload((Handle32*)&parcel);

    android::status_t status = event->writeToParcel(parcel);
    if (status) {
        assert(0);
//        jniThrowRuntimeException(env, "Failed to write MotionEvent parcel.");
    }
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
