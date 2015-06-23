
#include "view/CPointerCoords.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 CPointerCoords::INITIAL_PACKED_AXIS_VALUES;

CPointerCoords::CPointerCoords()
    : mX(0.0f)
    , mY(0.0f)
    , mPressure(0.0f)
    , mSize(0.0f)
    , mTouchMajor(0.0f)
    , mTouchMinor(0.0f)
    , mToolMajor(0.0f)
    , mToolMinor(0.0f)
    , mOrientation(0.0f)
    , mPackedAxisBits(0)
{
}

ECode CPointerCoords::constructor()
{
    return NOERROR;
}

ECode CPointerCoords::constructor(
    /* [in] */ IPointerCoords* other)
{
    return CopyFrom(other);;
}

ECode CPointerCoords::SetX(
    /* [in] */ Float value)
{
    mX = value;
    return NOERROR;
}

ECode CPointerCoords::GetX(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mX;
    return NOERROR;
}

ECode CPointerCoords::SetY(
    /* [in] */ Float value)
{
    mY = value;
    return NOERROR;
}

ECode CPointerCoords::GetY(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mY;
    return NOERROR;
}

ECode CPointerCoords::SetPressure(
    /* [in] */ Float value)
{
    mPressure = value;
    return NOERROR;
}

ECode CPointerCoords::GetPressure(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mPressure;
    return NOERROR;
}

ECode CPointerCoords::SetSize(
    /* [in] */ Float value)
{
    mSize = value;
    return NOERROR;
}

ECode CPointerCoords::GetSize(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mSize;
    return NOERROR;
}

ECode CPointerCoords::SetTouchMajor(
    /* [in] */ Float value)
{
    mTouchMajor = value;
    return NOERROR;
}

ECode CPointerCoords::GetTouchMajor(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mTouchMajor;
    return NOERROR;
}

ECode CPointerCoords::SetTouchMinor(
    /* [in] */ Float value)
{
    mTouchMinor = value;
    return NOERROR;
}

ECode CPointerCoords::GetTouchMinor(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mTouchMinor;
    return NOERROR;
}

ECode CPointerCoords::SetToolMajor(
    /* [in] */ Float value)
{
    mToolMajor = value;
    return NOERROR;
}

ECode CPointerCoords::GetToolMajor(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mToolMajor;
    return NOERROR;
}

ECode CPointerCoords::SetToolMinor(
    /* [in] */ Float value)
{
    mToolMinor = value;
    return NOERROR;
}

ECode CPointerCoords::GetToolMinor(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mToolMinor;
    return NOERROR;
}

ECode CPointerCoords::SetOrientation(
    /* [in] */ Float value)
{
    mOrientation = value;
    return NOERROR;
}

ECode CPointerCoords::GetOrientation(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mOrientation;
    return NOERROR;
}

ECode CPointerCoords::Clear()
{
    mPackedAxisBits = 0;

    mX = 0;
    mY = 0;
    mPressure = 0;
    mSize = 0;
    mTouchMajor = 0;
    mTouchMinor = 0;
    mToolMajor = 0;
    mToolMinor = 0;
    mOrientation = 0;
    return NOERROR;
}

ECode CPointerCoords::CopyFrom(
    /* [in] */ IPointerCoords* otherI)
{
    VALIDATE_NOT_NULL(otherI);

    CPointerCoords* other = (CPointerCoords*)otherI;

    const Int64 bits = other->mPackedAxisBits;
    mPackedAxisBits = bits;
    if (bits != 0) {
        AutoPtr<ArrayOf<Float> > otherValues = other->mPackedAxisValues;
        const Int32 count = Elastos::Core::Math::BitCount(bits);
        AutoPtr<ArrayOf<Float> > values = mPackedAxisValues;
        if (values == NULL || count > values->GetLength()) {
            values = ArrayOf<Float>::Alloc(otherValues->GetLength());
            mPackedAxisValues = values;
        }

        values->Copy(otherValues, 0, count);
    }

    mX = other->mX;
    mY = other->mY;
    mPressure = other->mPressure;
    mSize = other->mSize;
    mTouchMajor = other->mTouchMajor;
    mTouchMinor = other->mTouchMinor;
    mToolMajor = other->mToolMajor;
    mToolMinor = other->mToolMinor;
    mOrientation = other->mOrientation;
    return NOERROR;
}

ECode CPointerCoords::GetAxisValue(
    /* [in] */ Int32 axis,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    switch (axis) {
        case IMotionEvent::AXIS_X:
            *value = mX;
            break;
        case IMotionEvent::AXIS_Y:
            *value = mY;
            break;
        case IMotionEvent::AXIS_PRESSURE:
            *value = mPressure;
            break;
        case IMotionEvent::AXIS_SIZE:
            *value = mSize;
            break;
        case IMotionEvent::AXIS_TOUCH_MAJOR:
            *value = mTouchMajor;
            break;
        case IMotionEvent::AXIS_TOUCH_MINOR:
            *value = mTouchMinor;
            break;
        case IMotionEvent::AXIS_TOOL_MAJOR:
            *value = mToolMajor;
            break;
        case IMotionEvent::AXIS_TOOL_MINOR:
            *value = mToolMinor;
            break;
        case IMotionEvent::AXIS_ORIENTATION:
            *value = mOrientation;
            break;

        default: {
            if (axis < 0 || axis > 63) {
                Logger::E("CMotionEvent", "Axis out of range.");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            const Int64 bits = mPackedAxisBits;
            const Int64 axisBit = 1ll << axis;
            if ((bits & axisBit) == 0) {
                *value = 0;
                break;
            }
            const Int32 index = Elastos::Core::Math::BitCount(bits & (axisBit - 1ll));
            *value = (*mPackedAxisValues)[index];
        }
    }

    return NOERROR;
}

ECode CPointerCoords::SetAxisValue(
    /* [in] */ Int32 axis,
    /* [in] */ Float value)
{
    switch (axis) {
        case IMotionEvent::AXIS_X:
            mX = value;
            break;
        case IMotionEvent::AXIS_Y:
            mY = value;
            break;
        case IMotionEvent::AXIS_PRESSURE:
            mPressure = value;
            break;
        case IMotionEvent::AXIS_SIZE:
            mSize = value;
            break;
        case IMotionEvent::AXIS_TOUCH_MAJOR:
            mTouchMajor = value;
            break;
        case IMotionEvent::AXIS_TOUCH_MINOR:
            mTouchMinor = value;
            break;
        case IMotionEvent::AXIS_TOOL_MAJOR:
            mToolMajor = value;
            break;
        case IMotionEvent::AXIS_TOOL_MINOR:
            mToolMinor = value;
            break;
        case IMotionEvent::AXIS_ORIENTATION:
            mOrientation = value;
            break;
        default: {

            if (axis < 0 || axis > 63) {
                Logger::E("CMotionEvent", "Axis out of range.");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            const Int64 bits = mPackedAxisBits;
            const Int64 axisBit = 1ll << axis;
            const Int32 index = Elastos::Core::Math::BitCount(bits & (axisBit - 1ll));
            AutoPtr<ArrayOf<Float> > values = mPackedAxisValues;
            if ((bits & axisBit) == 0) {
                if (values == NULL) {
                    values = ArrayOf<Float>::Alloc(INITIAL_PACKED_AXIS_VALUES);
                    mPackedAxisValues = values;
                }
                else {
                    const Int32 count = Elastos::Core::Math::BitCount(bits);
                    if (count < values->GetLength()) {
                        if (index != count) {
                            values->Copy(index + 1, values, index, count - index);
                        }
                    }
                    else {
                        AutoPtr<ArrayOf<Float> > newValues = ArrayOf<Float>::Alloc(count * 2);
                        newValues->Copy(values, 0, index);
                        newValues->Copy(index + 1, values, index, count - index);
                        values = newValues;
                        mPackedAxisValues = values;
                    }
                }
                mPackedAxisBits = bits | axisBit;
            }
            (*values)[index] = value;
        }
    }

    return NOERROR;
}

}
}
}

