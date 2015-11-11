
#include "elastos/droid/hardware/camera2/params/CMeteringRectangle.h"
#include "elastos/droid/internal/utility/Preconditions.h"
#include "elastos/droid/graphics/CPoint.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/utility/CSize.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/Arrays.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Utility::CSize;
using Elastos::Droid::Internal::Utility::Preconditions;
using Elastos::Utility::Arrays;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Params {

CAR_INTERFACE_IMPL(CMeteringRectangle, Object, IMeteringRectangle)

CAR_OBJECT_IMPL(CMeteringRectangle)

CMeteringRectangle::CMeteringRectangle()
    : mX(0)
    , mY(0)
    , mWidth(0)
    , mHeight(0)
    , mWeight(0)
{
}

CMeteringRectangle::~CMeteringRectangle()
{
}

ECode CMeteringRectangle::constructor()
{
    return NOERROR;
}

ECode CMeteringRectangle::constructor(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 meteringWeight)
{
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(x, String("x must be nonnegative")))
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(y, String("y must be nonnegative")))
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(width, String("width must be nonnegative")))
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(height, String("height must be nonnegative")))
    FAIL_RETURN(Preconditions::CheckArgumentInRange(meteringWeight, IMeteringRectangle::METERING_WEIGHT_MIN,
            IMeteringRectangle::METERING_WEIGHT_MAX, String("meteringWeight")))
    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    mWeight = meteringWeight;
    return NOERROR;
}

ECode CMeteringRectangle::constructor(
    /* [in] */ IPoint* xy,
    /* [in] */ ISize* dimensions,
    /* [in] */ Int32 meteringWeight)
{
    //FAIL_RETURN(Preconditions::CheckNotNull(xy, String("xy must not be null")))
    if (xy == NULL) {
        Slogger::E("CMeteringRectangle", "xy must not be null");
        return E_NULL_POINTER_EXCEPTION;
    }
    //FAIL_RETURN(Preconditions::CheckNotNull(dimensions, String("dimensions must not be null")))
    if (dimensions == NULL) {
        Slogger::E("CMeteringRectangle", "dimensions must not be null");
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 x;
    xy->GetX(&x);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(x, String("x must be nonnegative")))

    Int32 y;
    xy->GetY(&y);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(y, String("y must be nonnegative")))

    Int32 width;
    dimensions->GetWidth(&width);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(width, String("width must be nonnegative")))

    Int32 height;
    dimensions->GetHeight(&height);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(height, String("height must be nonnegative")))

    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(meteringWeight,
            String("meteringWeight must be nonnegative")))

    mX = x;
    mY = y;
    mWidth = width;
    mHeight = height;
    mWeight = meteringWeight;
    return NOERROR;
}

ECode CMeteringRectangle::constructor(
    /* [in] */ IRect* rect,
    /* [in] */ Int32 meteringWeight)
{
    //FAIL_RETURN(Preconditions::CheckNotNull(rect, String("rect must not be null")))
    if (rect == NULL) {
        Slogger::E("CMeteringRectangle", "rect must not be null");
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 left;
    rect->GetLeft(&left);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(left, String("rect.left must be nonnegative")))

    Int32 top;
    rect->GetTop(&top);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(top, String("rect.top must be nonnegative")))

    Int32 width;
    rect->GetWidth(&width);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(width, String("rect.width must be nonnegative")))

    Int32 height;
    rect->GetHeight(&height);
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(height, String("rect.height must be nonnegative")))

    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(meteringWeight, String("meteringWeight must be nonnegative")))

    mX = left;
    mY = top;
    mWidth = width;
    mHeight = height;
    mWeight = meteringWeight;
    return NOERROR;
}

ECode CMeteringRectangle::GetX(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mX;
    return NOERROR;
}

ECode CMeteringRectangle::GetY(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mY;
    return NOERROR;
}

ECode CMeteringRectangle::GetWidth(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mWidth;
    return NOERROR;
}

ECode CMeteringRectangle::GetHeight(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mHeight;
    return NOERROR;
}

ECode CMeteringRectangle::GetMeteringWeight(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mWeight;
    return NOERROR;
}

ECode CMeteringRectangle::GetUpperLeftPoint(
    /* [out] */ IPoint** outpoint)
{
    VALIDATE_NOT_NULL(outpoint);

    return CPoint::New(mX, mY, outpoint);
}

ECode CMeteringRectangle::GetSize(
    /* [out] */ ISize** outsize)
{
    VALIDATE_NOT_NULL(outsize);

    return CSize::New(mWidth, mHeight, outsize);
}

ECode CMeteringRectangle::GetRect(
    /* [out] */ IRect** outrect)
{
    VALIDATE_NOT_NULL(outrect);

    return CRect::New(mX, mY, mX + mWidth, mY + mHeight, outrect);
}

ECode CMeteringRectangle::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    if (IMeteringRectangle::Probe(obj) != NULL) {
        Boolean ret;
        Equals(IMeteringRectangle::Probe(obj), &ret);
        if (ret) {
            *value = TRUE;
        }
    }
    else {
        *value = FALSE;
    }
    return NOERROR;
}

ECode CMeteringRectangle::Equals(
    /* [in] */ IMeteringRectangle* other,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    if (other == NULL) {
        *value = FALSE;
        return NOERROR;
    }

    *value = (mX == ((CMeteringRectangle*)other)->mX
            && mY == ((CMeteringRectangle*)other)->mY
            && mWidth == ((CMeteringRectangle*)other)->mWidth
            && mHeight == ((CMeteringRectangle*)other)->mHeight
            && mWeight == ((CMeteringRectangle*)other)->mWeight);
    return NOERROR;
}

ECode CMeteringRectangle::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    assert(0 && "TODO: weit Hardware/Camera2/Utils/HashCodeHelpers");
    //*value = HashCodeHelpers::GetHashCode(mX, mY, mWidth, mHeight, mWeight);
    return NOERROR;
}

ECode CMeteringRectangle::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder sb;
    sb += "(x:";
    sb += mX;
    sb += ", y:";
    sb += mY;
    sb += ", w:";
    sb += mWidth;
    sb += ", h:";
    sb += mHeight;
    sb += ", wt:";
    sb += mWeight;
    sb += ")";

    return sb.ToString(str);
}

} // namespace Params
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
