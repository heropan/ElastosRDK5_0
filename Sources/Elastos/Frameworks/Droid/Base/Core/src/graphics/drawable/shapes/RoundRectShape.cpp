
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/RoundRectShape.h"
#include "graphics/CPath.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

RoundRectShape::RoundRectShape()
{}

/**
 * RoundRectShape constructor.
 * Specifies an outer (round)rect and an optional inner (round)rect.
 *
 * @param outerRadii An array of 8 radius values, for the outer roundrect.
 *                   The first two floats are for the
 *                   top-left corner (remaining pairs correspond clockwise).
 *                   For no rounded corners on the outer rectangle,
 *                   pass null.
 * @param inset      A RectF that specifies the distance from the inner
 *                   rect to each side of the outer rect.
 *                   For no inner, pass null.
 * @param innerRadii An array of 8 radius values, for the inner roundrect.
 *                   The first two floats are for the
 *                   top-left corner (remaining pairs correspond clockwise).
 *                   For no rounded corners on the inner rectangle,
 *                   pass null.
 *                   If inset parameter is null, this parameter is ignored.
 */
RoundRectShape::RoundRectShape(
    /* [in] */ ArrayOf<Float>* outerRadii,
    /* [in] */ IRectF* inset,
    /* [in] */ ArrayOf<Float>* innerRadii)
{
    if (outerRadii != NULL && outerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "outer radii must have >= 8 values");
        assert(0);
    }
    if (innerRadii != NULL && innerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "inner radii must have >= 8 values");
        assert(0);
    }
    mOuterRadii = outerRadii;
    mInset = (CRectF*)inset;
    mInnerRadii = innerRadii;

    if (inset != NULL) {
        CRectF::NewByFriend((CRectF**)&mInnerRect);
    }
    CPath::New((IPath**)&mPath);
}

ECode RoundRectShape::Init(
    /* [in] */ ArrayOf<Float>* outerRadii,
    /* [in] */ IRectF* inset,
    /* [in] */ ArrayOf<Float>* innerRadii)
{
    if (outerRadii != NULL && outerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "outer radii must have >= 8 values");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (innerRadii != NULL && innerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "inner radii must have >= 8 values");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mOuterRadii = outerRadii;
    mInset = (CRectF*)inset;
    mInnerRadii = innerRadii;

    if (inset != NULL) {
        CRectF::NewByFriend((CRectF**)&mInnerRect);
    }
    CPath::New((IPath**)&mPath);
    return NOERROR;
}

ECode RoundRectShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawPath(mPath, paint);
}

void RoundRectShape::OnResize(
    /* [in] */ Float w,
    /* [in] */ Float h)
{
    RectShape::OnResize(w, h);

    AutoPtr<CRectF> r = Rect();
    mPath->Reset();

    if (mOuterRadii != NULL) {
        mPath->AddRoundRectEx((IRectF*)r, *mOuterRadii, PathDirection_CW);
    }
    else {
        mPath->AddRect((IRectF*)r, PathDirection_CW);
    }
    if (mInnerRect != NULL) {
        mInnerRect->Set(r->mLeft + mInset->mLeft, r->mTop + mInset->mTop,
                        r->mRight - mInset->mRight, r->mBottom - mInset->mBottom);
        Float ww, hh;
        mInnerRect->GetWidth(&ww);
        mInnerRect->GetHeight(&hh);
        if (ww < w && hh < h) {
            if (mInnerRadii != NULL) {
                mPath->AddRoundRectEx((IRectF*)mInnerRect, *mInnerRadii,
                                    PathDirection_CCW);
            }
            else {
                mPath->AddRect((IRectF*)mInnerRect, PathDirection_CCW);
            }
        }
    }
}

void RoundRectShape::Clone(
        /* [in] */ RoundRectShape* other)
{
    RectShape::Clone((RectShape*)other);
    other->mOuterRadii = mOuterRadii != NULL ? mOuterRadii->Clone() : NULL;
    other->mInnerRadii = mInnerRadii != NULL ? mInnerRadii->Clone() : NULL;
    CRectF::NewByFriend((IRectF*)mInset.Get(), (CRectF**)&other->mInset);
    CRectF::NewByFriend((IRectF*)mInnerRect.Get(), (CRectF**)&other->mInnerRect);
    CPath::New(mPath, (IPath**)&other->mPath);
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
