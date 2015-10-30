
#ifndef __ELASTOS_DROID_WEBKIT_QUADF_H__
#define __ELASTOS_DROID_WEBKIT_QUADF_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Graphics::IPointF;

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * A quadrilateral, determined by four points, clockwise order. Typically
 * p1 is "top-left" and p4 is "bottom-left" following webkit's rectangle-to-
 * FloatQuad conversion.
 */
class QuadF : public Object
{
public:
    QuadF();

    CAR_INTERFACE_DECL()

    virtual CARAPI_(void) Offset(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Determines if the quadrilateral contains the given point. This does
     * not work if the quadrilateral is self-intersecting or if any inner
     * angle is reflex (greater than 180 degrees).
     */
    virtual CARAPI_(Boolean) ContainsPoint(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(String) GoString();

    static void SetQuadFP1(
        /* [in] */ IInterface* obj,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static void SetQuadFP2(
        /* [in] */ IInterface* obj,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static void SetQuadFP3(
        /* [in] */ IInterface* obj,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static void SetQuadFP4(
        /* [in] */ IInterface* obj,
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI ToString(
        /* [out] */ String* info);

public:
    AutoPtr<IPointF> p1;
    AutoPtr<IPointF> p2;
    AutoPtr<IPointF> p3;
    AutoPtr<IPointF> p4;

private:
    static CARAPI_(Boolean) IsPointInTriangle(
        /* [in] */ Float x0,
        /* [in] */ Float y0,
        /* [in] */ IPointF* r1,
        /* [in] */ IPointF* r2,
        /* [in] */ IPointF* r3);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_QUADF_H__
