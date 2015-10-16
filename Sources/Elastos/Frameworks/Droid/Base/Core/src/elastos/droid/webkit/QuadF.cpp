
#include "elastos/droid/graphics/CPointF.h"
#include "elastos/droid/webkit/QuadF.h"

#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Graphics::CPointF;

namespace Elastos {
namespace Droid {
namespace Webkit {

QuadF::QuadF()
{
    CPointF::New((IPointF**)&p1);
    CPointF::New((IPointF**)&p2);
    CPointF::New((IPointF**)&p3);
    CPointF::New((IPointF**)&p4);
}

CAR_INTERFACE_IMPL(QuadF, IInterface)

void QuadF::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    p1->Offset(dx, dy);
    p2->Offset(dx, dy);
    p3->Offset(dx, dy);
    p4->Offset(dx, dy);
}

/**
 * Determines if the quadrilateral contains the given point. This does
 * not work if the quadrilateral is self-intersecting or if any inner
 * angle is reflex (greater than 180 degrees).
 */
Boolean QuadF::ContainsPoint(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return IsPointInTriangle(x, y, p1, p2, p3) ||
                IsPointInTriangle(x, y, p1, p3, p4);
}

String QuadF::GoString()
{
    StringBuilder s("QuadF(");
    Float x, y;

    p1->GetX(&x);
    p1->GetY(&y);
    s.AppendInt32(x);
    s.AppendCStr(",");
    s.AppendInt32(y);

    s.AppendCStr(" - ");

    p2->GetX(&x);
    p2->GetY(&y);
    s.AppendInt32(x);
    s.AppendCStr(",");
    s.AppendInt32(y);

    s.AppendCStr(" - ");

    p3->GetX(&x);
    p3->GetY(&y);
    s.AppendInt32(x);
    s.AppendCStr(",");
    s.AppendInt32(y);

    s.AppendCStr(" - ");

    p4->GetX(&x);
    p4->GetY(&y);
    s.AppendInt32(x);
    s.AppendCStr(",");
    s.AppendInt32(y);

    s.AppendCStr(")");

    return s.ToString();
}

Boolean QuadF::IsPointInTriangle(
    /* [in] */ Float x0,
    /* [in] */ Float y0,
    /* [in] */ IPointF* r1,
    /* [in] */ IPointF* r2,
    /* [in] */ IPointF* r3)
{
    // Use the barycentric technique
    Float x1, x2, x3, y1, y2, y3;

    r1->GetX(&x1);
    r2->GetX(&x2);
    r3->GetX(&x3);
    r1->GetY(&y1);
    r2->GetY(&y2);
    r3->GetY(&y3);
    Float x13 = x1 - x3;
    Float y13 = y1 - y3;
    Float x23 = x2 - x3;
    Float y23 = y2 - y3;
    Float x03 = x0 - x3;
    Float y03 = y0 - y3;

    Float determinant = (y23 * x13) - (x23 * y13);
    Float lambda1 = ((y23 * x03) - (x23 * y03))/determinant;
    Float lambda2 = ((x13 * y03) - (y13 * x03))/determinant;
    Float lambda3 = 1 - lambda1 - lambda2;

    return lambda1 >= 0.0f && lambda2 >= 0.0f && lambda3 >= 0.0f;
}

void QuadF::SetQuadFP1(
    /* [in] */ IInterface* obj,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    ((QuadF*)obj)->p1->Set(x, y);
}

void QuadF::SetQuadFP2(
    /* [in] */ IInterface* obj,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    ((QuadF*)obj)->p2->Set(x, y);
}

void QuadF::SetQuadFP3(
    /* [in] */ IInterface* obj,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    ((QuadF*)obj)->p3->Set(x, y);
}

void QuadF::SetQuadFP4(
    /* [in] */ IInterface* obj,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    ((QuadF*)obj)->p4->Set(x, y);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
