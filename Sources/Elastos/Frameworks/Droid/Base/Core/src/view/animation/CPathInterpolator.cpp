
#include "CPathInterpolator.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CPathInterpolator);
CAR_INTERFACE_IMPL_3(CPathInterpolator, Object, IPathInterpolator,IInterpolator,ITimeInterpolator);

const Float CPathInterpolator::PRECISION = 0.002f;
ECode CPathInterpolator::constructor(
    /* [in] */ IPath* path)
{
    return InitPath(path);
}

ECode CPathInterpolator::constructor(
    /* [in] */ Float controlX,
    /* [in] */ Float controlY)
{
    return InitQuad(controlX, controlY);
}

ECode CPathInterpolator::constructor(
    /* [in] */ Float controlX1,
    /* [in] */ Float controlY1,
    /* [in] */ Float controlX2,
    /* [in] */ Float controlY2)
{
    return InitCubic(controlX1, controlY1, controlX2, controlY2);
}

ECode CPathInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<ITheme> theme;
    context->GetTheme((ITheme**)&theme);
    return constructor(res, theme, attrs);
}

ECode CPathInterpolator::constructor(
    /* [in] */ IResources* res,
    /* [in] */ ITheme* theme,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PathInterpolator),
            ARRAY_SIZE(R::styleable::PathInterpolator));

    if (theme != NULL) {
        theme->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    } else {
        res->ObtainAttributes(attrs, attrIds, (ITypedArray**)&a);
    }
    FAIL_RETURN(ParseInterpolatorFromTypeArray(a));

    return a->Recycle();
}

ECode CPathInterpolator::ParseInterpolatorFromTypeArray(
    /* [in] */ ITypedArray* a)
{
    // If there is pathData defined in the xml file, then the controls points
    // will be all coming from pathData.
    Boolean has = FALSE;
    if (a->HasValue(R::styleable::PathInterpolator_pathData, &has), has) {
        String pathData;
        a->GetString(R::styleable::PathInterpolator_pathData, &pathData);
        AutoPtr<IPath> path = PathParser::CreatePathFromPathData(pathData);
        if (path == NULL) {
            // throw new InflateException("The path is null, which is created"
            //         + " from " + pathData);
            return E_INFLATE_EXCEPTION;
        }

        FAIL_RETURN(InitPath(path));
    } else {
        if (!(a->HasValue(R::styleable::PathInterpolator_controlX1, &has), has)) {
            // throw new InflateException("pathInterpolator requires the controlX1 attribute");
            return E_INFLATE_EXCEPTION;
        } else if (!a.hasValue(R.styleable.PathInterpolator_controlY1)) {
            // throw new InflateException("pathInterpolator requires the controlY1 attribute");
            return E_INFLATE_EXCEPTION;
        }
        Float x1 = 0f;
        a->GetFloat(R::styleable::PathInterpolator_controlX1, 0, &x1);
        Float y1 = 0f;
        a->GetFloat(R::styleable::PathInterpolator_controlY1, 0, &y1);

        Boolean hasX2 = FALSE;
        a->HasValue(R::styleable::PathInterpolator_controlX2, &hasX2);
        Boolean hasY2 = FALSE;
        a->HasValue(R::styleable::PathInterpolator_controlY2, &hasY2);

        if (hasX2 != hasY2) {
            // throw new InflateException(
            //         "pathInterpolator requires both controlX2 and controlY2 for cubic Beziers.");
            return E_INFLATE_EXCEPTION;
        }

        if (!hasX2) {
            FAIL_RETURN(InitQuad(x1, y1));
        } else {
            Float x2 = 0f;
            a->GetFloat(R::styleable::PathInterpolator_controlX2, 0, &x2);
            Float y2 = 0f;
            a->GetFloat(R::styleable::PathInterpolator_controlY2, 0, &y2);
            FAIL_RETURN(InitCubic(x1, y1, x2, y2));
        }
    }

    return NOERROR;
}

ECode CPathInterpolator::InitQuad(
    /* [in] */ Float controlX,
    /* [in] */ Float controlY)
{
    AutoPtr<IPath> path;
    CPath::New((IPath**)&path);
    path->MoveTo(0, 0);
    path->QuadTo(controlX, controlY, 1f, 1f);
    return InitPath(path);
}

ECode CPathInterpolator::InitCubic(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2)
{
    AutoPtr<IPath> path;
    CPath::New((IPath**)&path);
    path->MoveTo(0, 0);
    path->CubicTo(x1, y1, x2, y2, 1f, 1f);
    return InitPath(path);
}

ECode CPathInterpolator::InitPath(
    /* [in] */ IPath* path)
{
    AutoPtr<ArrayOf<Float> > pointComponents;
    path->Approximate(PRECISION, (ArrayOf<Float>**)&pointComponents);

    Int32 numPoints = pointComponents->GetLength() / 3;
    if ((*pointComponents)[1] != 0 || (*pointComponents)[2] != 0
            || (*pointComponents)[pointComponents->GetLength() - 2] != 1
            || (*pointComponents)[pointComponents->GetLength() - 1] != 1) {
        // throw new IllegalArgumentException("The Path must start at (0,0) and end at (1,1)");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mX = ArrayOf<Float>::Alloc(numPoints);
    mY = ArrayOf<Float>::Alloc(numPoints);
    Float prevX = 0;
    Float prevFraction = 0;
    Int32 componentIndex = 0;
    for (Int32 i = 0; i < numPoints; i++) {
        Float fraction = (*pointComponents)[componentIndex++];
        Float x = (*pointComponents)[componentIndex++];
        Float y = (*pointComponents)[componentIndex++];
        if (fraction == prevFraction && x != prevX) {
            // throw new IllegalArgumentException(
            //         "The Path cannot have discontinuity in the X axis.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (x < prevX) {
            // throw new IllegalArgumentException("The Path cannot loop back on itself.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        (*mX)[i] = x;
        (*mY)[i] = y;
        prevX = x;
        prevFraction = fraction;
    }

    return NOERROR;
}

ECode CPathInterpolator::GetInterpolation(
    /* [in] */ Float t,
    /* [out] */ Float* interpolation)
{
    VALIDATE_NOT_NULL(interpolation);
    *interpolation = 0f;
    if (t <= 0) {
        return NOERROR;
    } else if (t >= 1) {
        *interpolation = 1;
        return NOERROR;
    }
    // Do a binary search for the correct x to interpolate between.
    Int32 startIndex = 0;
    Int32 endIndex = mX.length - 1;

    while (endIndex - startIndex > 1) {
        Int32 midIndex = (startIndex + endIndex) / 2;
        if (t < mX[midIndex]) {
            endIndex = midIndex;
        } else {
            startIndex = midIndex;
        }
    }

    Float xRange = (*mX)[endIndex] - (*mX)[startIndex];
    if (xRange == 0) {
        *interpolation = (*mY)[startIndex];
        return NOERROR;
    }

    Float tInRange = t - (*mX)[startIndex];
    Float fraction = tInRange / xRange;

    Float startY = (*mY)[startIndex];
    Float endY = (*mY)[endIndex];
    *interpolation = startY + (fraction * (endY - startY));
    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
