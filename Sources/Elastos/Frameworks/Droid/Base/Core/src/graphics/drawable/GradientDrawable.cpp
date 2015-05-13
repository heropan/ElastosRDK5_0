
#include "ext/frameworkext.h"
#include "graphics/drawable/GradientDrawable.h"
#include "graphics/drawable/CGradientDrawable.h"
#include "graphics/CPaint.h"
#include "graphics/CPath.h"
#include "graphics/CRectF.h"
#include "graphics/CDashPathEffect.h"
#include "graphics/CLinearGradient.h"
#include "graphics/CRadialGradient.h"
#include "graphics/CSweepGradient.h"
#include "R.h"
#include <elastos/Math.h>
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

GradientDrawable::GradientState::GradientState(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
    : mChangingConfigurations(0)
    , mShape(IGradientDrawable::RECTANGLE)
    , mGradient(IGradientDrawable::LINEAR_GRADIENT)
    , mOrientation(orientation)
    , mHasSolidColor(FALSE)
    , mSolidColor(0)
    , mStrokeWidth(-1)
    , mStrokeColor(0)
    , mStrokeDashWidth(0)
    , mStrokeDashGap(0)
    , mRadius(0)
    , mWidth(-1)
    , mHeight(-1)
    , mInnerRadiusRatio(0)
    , mThicknessRatio(0)
    , mInnerRadius(0)
    , mThickness(0)
    , mCenterX(0.5f)
    , mCenterY(0.5f)
    , mGradientRadius(0.5f)
    , mUseLevel(FALSE)
    , mUseLevelForShape(FALSE)
    , mOpaque(FALSE)
{
    SetColors(colors);
}

GradientDrawable::GradientState::GradientState(
    /* [in] */ GradientState* state)
    : mChangingConfigurations(state->mChangingConfigurations)
    , mShape(state->mShape)
    , mGradient(state->mGradient)
    , mOrientation(state->mOrientation)
    , mHasSolidColor(state->mHasSolidColor)
    , mSolidColor(state->mSolidColor)
    , mStrokeWidth(state->mStrokeWidth)
    , mStrokeColor(state->mStrokeColor)
    , mStrokeDashWidth(state->mStrokeDashWidth)
    , mStrokeDashGap(state->mStrokeDashGap)
    , mRadius(state->mRadius)
    , mWidth(state->mWidth)
    , mHeight(state->mHeight)
    , mInnerRadiusRatio(state->mInnerRadiusRatio)
    , mThicknessRatio(state->mThicknessRatio)
    , mInnerRadius(state->mInnerRadius)
    , mThickness(state->mThickness)
    , mCenterX(state->mCenterX)
    , mCenterY(state->mCenterY)
    , mGradientRadius(state->mGradientRadius)
    , mUseLevel(state->mUseLevel)
    , mUseLevelForShape(state->mUseLevelForShape)
    , mOpaque(state->mOpaque)
{
    if (state->mColors != NULL) {
        mColors = state->mColors->Clone();
    }
    if (state->mPositions != NULL) {
        mPositions = state->mPositions->Clone();
    }
    if (state->mRadiusArray != NULL) {
        mRadiusArray = state->mRadiusArray->Clone();
    }
    if (state->mPadding != NULL) {
        CRect::NewByFriend((IRect*)state->mPadding.Get(), (CRect**)&mPadding);
    }
}

GradientDrawable::GradientState::~GradientState()
{
}

ECode GradientDrawable::GradientState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CGradientDrawable::New(this, (IGradientDrawable**)drawable);
}

ECode GradientDrawable::GradientState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);

    return CGradientDrawable::New(this, (IGradientDrawable**)drawable);
}

ECode GradientDrawable::GradientState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);

    *config = mChangingConfigurations;
    return NOERROR;
}

void GradientDrawable::GradientState::SetShape(
    /* [in] */ Int32 shape)
{
    mShape = shape;
    ComputeOpacity();
}

void GradientDrawable::GradientState::SetGradientType(
    /* [in] */ Int32 gradient)
{
    mGradient = gradient;
}

void GradientDrawable::GradientState::SetGradientCenter(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mCenterX = x;
    mCenterY = y;
}

void GradientDrawable::GradientState::SetColors(
    /* [in] */ ArrayOf<Int32>* colors)
{
    mHasSolidColor = FALSE;
    mColors = colors;
    ComputeOpacity();
}

void GradientDrawable::GradientState::SetSolidColor(
    /* [in] */ Int32 argb)
{
    mHasSolidColor = TRUE;
    mSolidColor = argb;
    mColors = NULL;
    ComputeOpacity();
}

void GradientDrawable::GradientState::ComputeOpacity()
{
    if (mShape != IGradientDrawable::RECTANGLE) {
        mOpaque = FALSE;
        return;
    }

    if (mRadius > 0 || mRadiusArray != NULL) {
        mOpaque = FALSE;
        return;
    }

    if (mStrokeWidth > 0 && !IsOpaque(mStrokeColor)) {
        mOpaque = FALSE;
        return;
    }

    if (mHasSolidColor) {
        mOpaque = IsOpaque(mSolidColor);
        return;
    }

    if (mColors != NULL) {
        for (Int32 i = 0; i < mColors->GetLength(); i++) {
            if (!IsOpaque((*mColors)[i])) {
                mOpaque = FALSE;
                return;
            }
        }
    }

    mOpaque = TRUE;
}

Boolean GradientDrawable::GradientState::IsOpaque(
    /* [in] */ Int32 color)
{
    return ((color >> 24) & 0xff) == 0xff;
}

void GradientDrawable::GradientState::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color)
{
    mStrokeWidth = width;
    mStrokeColor = color;
    ComputeOpacity();
}

void GradientDrawable::GradientState::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color,
    /* [in] */ Float dashWidth,
    /* [in] */ Float dashGap)
{
    mStrokeWidth = width;
    mStrokeColor = color;
    mStrokeDashWidth = dashWidth;
    mStrokeDashGap = dashGap;
    ComputeOpacity();
}

void GradientDrawable::GradientState::SetCornerRadius(
    /* [in] */ Float radius)
{
    if (radius < 0) {
        radius = 0;
    }
    mRadius = radius;
    mRadiusArray = NULL;
}

void GradientDrawable::GradientState::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    mRadiusArray = radii;
    if (radii == NULL) {
        mRadius = 0;
    }
}

void GradientDrawable::GradientState::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;
}

void GradientDrawable::GradientState::SetGradientRadius(
    /* [in] */ Float gradientRadius)
{
    mGradientRadius = gradientRadius;
}


GradientDrawable::GradientDrawable()
    : mAlpha(0xFF)
    , mDither(FALSE)
    , mRectIsDirty(FALSE)
    , mMutated(FALSE)
    , mPathIsDirty(TRUE)
{
    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mFillPaint);
    CPath::New((IPath**)&mPath);
    CRectF::New((IRectF**)&mRect);
}

/**
 * Create a new gradient drawable given an orientation and an array
 * of colors for the gradient.
 */
GradientDrawable::GradientDrawable(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
    : mAlpha(0xFF)
    , mDither(FALSE)
    , mRectIsDirty(FALSE)
    , mMutated(FALSE)
    , mPathIsDirty(TRUE)
{
    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mFillPaint);
    CPath::New((IPath**)&mPath);
    CRectF::NewByFriend((CRectF**)&mRect);
    AutoPtr<GradientState> state = new GradientState(orientation, colors);
    Init(state);
}

//@Override
Boolean GradientDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    if (mPadding != NULL) {
        padding->SetEx((IRect*)mPadding.Get());
        return TRUE;
    }
    else {
        return Drawable::GetPadding(padding);
    }
}

ECode GradientDrawable::SetCornerRadii(
    /* [in] */ ArrayOf<Float>* radii)
{
    mGradientState->SetCornerRadii(radii);
    mPathIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetCornerRadius(
    /* [in] */ Float radius)
{
    mGradientState->SetCornerRadius(radius);
    mPathIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color)
{
    return SetStroke(width, color, 0, 0);
}

ECode GradientDrawable::SetStroke(
    /* [in] */ Int32 width,
    /* [in] */ Int32 color,
    /* [in] */ Float dashWidth,
    /* [in] */ Float dashGap)
{
    mGradientState->SetStroke(width, color, dashWidth, dashGap);

    if (mStrokePaint == NULL)  {
        CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mStrokePaint);
        mStrokePaint->SetStyle(PaintStyle_STROKE);
    }
    mStrokePaint->SetStrokeWidth(width);
    mStrokePaint->SetColor(color);

    AutoPtr<IPathEffect> e;
    if (dashWidth > 0) {
        ArrayOf_<Float, 2> intervals;
        intervals[0] = dashWidth;
        intervals[1] = dashGap;
        CDashPathEffect::New(intervals, 0, (IDashPathEffect**)&e);
    }
    mStrokePaint->SetPathEffect(e);
    return InvalidateSelf();;
}

ECode GradientDrawable::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mGradientState->SetSize(width, height);
    mPathIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetShape(
    /* [in] */ Int32 shape)
{
    mRingPath = NULL;
    mPathIsDirty = TRUE;
    mGradientState->SetShape(shape);
    return InvalidateSelf();
}

ECode GradientDrawable::SetGradientType(
    /* [in] */ Int32 gradient)
{
    mGradientState->SetGradientType(gradient);
    mRectIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetGradientCenter(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mGradientState->SetGradientCenter(x, y);
    mRectIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetGradientRadius(
    /* [in] */ Float gradientRadius)
{
    mGradientState->SetGradientRadius(gradientRadius);
    mRectIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetUseLevel(
    /* [in] */ Boolean useLevel)
{
    mGradientState->mUseLevel = useLevel;
    mRectIsDirty = TRUE;
    return InvalidateSelf();
}

Int32 GradientDrawable::ModulateAlpha(
    /* [in] */ Int32 alpha)
{
    Int32 scale = mAlpha + (mAlpha >> 7);
    return alpha * scale >> 8;
}

ECode GradientDrawable::GetOrientation(
    /*[out] */ GradientDrawableOrientation* orientation)
{
    assert(orientation != NULL);
    *orientation = mGradientState->mOrientation;
    return NOERROR;
}

ECode GradientDrawable::SetOrientation(
    /* [in] */ GradientDrawableOrientation orientation)
{
    mGradientState->mOrientation = orientation;
    mRectIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::SetColors(
    /* [in] */ ArrayOf<Int32>* colors)
{
    mGradientState->SetColors(colors);
    mRectIsDirty = TRUE;
    return InvalidateSelf();
}

ECode GradientDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (!EnsureValidRect()) {
        // nothing to draw
        return NOERROR;
    }

    // remember the alpha values, in case we temporarily overwrite them
    // when we modulate them with mAlpha
    Int32 prevFillAlpha;
    mFillPaint->GetAlpha(&prevFillAlpha);
    Int32 prevStrokeAlpha = 0;
    if (mStrokePaint != NULL) mStrokePaint->GetAlpha(&prevStrokeAlpha);
    // compute the modulate alpha values
    Int32 currFillAlpha = ModulateAlpha(prevFillAlpha);
    Int32 currStrokeAlpha = ModulateAlpha(prevStrokeAlpha);

    Float width;
    Boolean haveStroke = currStrokeAlpha > 0 && mStrokePaint != NULL && (mStrokePaint->GetStrokeWidth(&width), width > 0);
    Boolean haveFill = currFillAlpha > 0;
    AutoPtr<GradientState> st = mGradientState;
    /*  we need a layer iff we're drawing both a fill and stroke, and the
        stroke is non-opaque, and our shapetype actually supports
        fill+stroke. Otherwise we can just draw the stroke (if any) on top
        of the fill (if any) without worrying about blending artifacts.
    */
    Boolean useLayer = haveStroke && haveFill && st->mShape != IGradientDrawable::LINE &&
            currStrokeAlpha < 255 && (mAlpha < 255 || mColorFilter != NULL);

    /*  Drawing with a layer is slower than direct drawing, but it
        allows us to apply paint effects like alpha and colorfilter to
        the result of multiple separate draws. In our case, if the user
        asks for a non-opaque alpha value (via setAlpha), and we're
        stroking, then we need to apply the alpha AFTER we've drawn
        both the fill and the stroke.
    */
    if (useLayer) {
        if (mLayerPaint == NULL) {
            CPaint::New((IPaint**)&mLayerPaint);
        }
        mLayerPaint->SetDither(mDither);
        mLayerPaint->SetAlpha(mAlpha);
        mLayerPaint->SetColorFilter(mColorFilter);

        Float rad;
        mStrokePaint->GetStrokeWidth(&rad);
        Int32 count;
        canvas->SaveLayerEx(mRect->mLeft - rad, mRect->mTop - rad,
                mRect->mRight + rad, mRect->mBottom + rad,
                mLayerPaint, ICanvas::HAS_ALPHA_LAYER_SAVE_FLAG, &count);

        // don't perform the filter in our individual paints
        // since the layer will do it for us
        mFillPaint->SetColorFilter(NULL);
        mStrokePaint->SetColorFilter(NULL);
    }
    else {
        /*  if we're not using a layer, apply the dither/filter to our
            individual paints
        */
        mFillPaint->SetAlpha(currFillAlpha);
        mFillPaint->SetDither(mDither);
        mFillPaint->SetColorFilter(mColorFilter);
        if (mColorFilter != NULL && !mGradientState->mHasSolidColor) {
            mFillPaint->SetColor(mAlpha << 24);
        }
        if (haveStroke) {
            mStrokePaint->SetAlpha(currStrokeAlpha);
            mStrokePaint->SetDither(mDither);
            mStrokePaint->SetColorFilter(mColorFilter);
        }
    }

    switch (st->mShape) {
        case IGradientDrawable::RECTANGLE:
            if (st->mRadiusArray != NULL) {
                if (mPathIsDirty || mRectIsDirty) {
                    mPath->Reset();
                    mPath->AddRoundRectEx((IRectF*)mRect.Get(), *st->mRadiusArray,
                            PathDirection_CW);
                    mPathIsDirty = mRectIsDirty = FALSE;
                }
                canvas->DrawPath(mPath, mFillPaint);
                if (haveStroke) {
                    canvas->DrawPath(mPath, mStrokePaint);
                }
            }
            else if (st->mRadius > 0.0f) {
                // since the caller is only giving us 1 value, we will force
                // it to be square if the rect is too small in one dimension
                // to show it. If we did nothing, Skia would clamp the rad
                // independently along each axis, giving us a thin ellipse
                // if the rect were very wide but not very tall
                Float rad = st->mRadius;
                Float width, height;
                mRect->GetWidth(&width);
                mRect->GetHeight(&height);
                Float r = Elastos::Core::Math::Min(width, height) * 0.5f;
                if (rad > r) {
                    rad = r;
                }
                canvas->DrawRoundRect(mRect, rad, rad, mFillPaint);
                if (haveStroke) {
                    canvas->DrawRoundRect(mRect, rad, rad, mStrokePaint);
                }
            }
            else {
                Int32 color = 0;
                AutoPtr<IShader> shader;
                if ((mFillPaint->GetColor(&color), color) != 0 || mColorFilter != NULL ||
                        (mFillPaint->GetShader((IShader**)&shader), shader) != NULL) {
                    canvas->DrawRect(mRect, mFillPaint);
                }
                if (haveStroke) {
                    canvas->DrawRect(mRect, mStrokePaint);
                }
            }
            break;
        case IGradientDrawable::OVAL:
            canvas->DrawOval(mRect, mFillPaint);
            if (haveStroke) {
                canvas->DrawOval(mRect, mStrokePaint);
            }
            break;
        case IGradientDrawable::LINE: {
            CRectF* r = mRect;
            Float y;
            r->GetCenterY(&y);
            canvas->DrawLine(r->mLeft, y, r->mRight, y, mStrokePaint);
            break;
        }
        case IGradientDrawable::RING:
            AutoPtr<IPath> path = BuildRing(st);
            canvas->DrawPath(path, mFillPaint);
            if (haveStroke) {
                canvas->DrawPath(path, mStrokePaint);
            }
            break;
    }

    if (useLayer) {
        canvas->Restore();
    }
    else {
        mFillPaint->SetAlpha(prevFillAlpha);
        if (haveStroke) {
            mStrokePaint->SetAlpha(prevStrokeAlpha);
        }
    }
    return NOERROR;
}

AutoPtr<IPath> GradientDrawable::BuildRing(
    /* [in] */ GradientState* st)
{
    if (mRingPath != NULL && (!st->mUseLevelForShape || !mPathIsDirty)) return mRingPath;
    mPathIsDirty = FALSE;

    Float sweep = st->mUseLevelForShape ? (360.0f * GetLevel() / 10000.0f) : 360.0f;

    AutoPtr<IRectF> bounds;
    CRectF::New((IRectF*)mRect.Get(), (IRectF**)&bounds);

    Float w, h, x, y;
    bounds->GetWidth(&w);
    bounds->GetHeight(&h);
    x = w /2.0f;
    y = h /2.0f;

    Float thickness = st->mThickness != -1 ?
            st->mThickness : w / st->mThicknessRatio;
    // inner radius
    Float radius = st->mInnerRadius != -1 ?
            st->mInnerRadius : w / st->mInnerRadiusRatio;

    AutoPtr<IRectF> innerBounds;
    CRectF::New(bounds, (IRectF**)&innerBounds);
    innerBounds->Inset(x - radius, y - radius);

    bounds = NULL;
    CRectF::New(innerBounds, (IRectF**)&bounds);
    bounds->Inset(-thickness, -thickness);

    if (mRingPath == NULL) {
        CPath::New((IPath**)&mRingPath);
    }
    else {
        mRingPath->Reset();
    }

    AutoPtr<IPath> ringPath = mRingPath;
    // arcTo treats the sweep angle mod 360, so check for that, since we
    // think 360 means draw the entire oval
    if (sweep < 360 && sweep > -360) {
        ringPath->SetFillType(PathFillType_EVEN_ODD);
        // inner top
        ringPath->MoveTo(x + radius, y);
        // outer top
        ringPath->LineTo(x + radius + thickness, y);
        // outer arc
        ringPath->ArcTo(bounds, 0.0f, sweep, FALSE);
        // inner arc
        ringPath->ArcTo(innerBounds, sweep, -sweep, FALSE);
        ringPath->Close();
    }
    else {
        // add the entire ovals
        ringPath->AddOval(bounds, PathDirection_CW);
        ringPath->AddOval(innerBounds, PathDirection_CCW);
    }

    return ringPath;
}

ECode GradientDrawable::SetColor(
    /* [in] */ Int32 argb)
{
    mGradientState->SetSolidColor(argb);
    mFillPaint->SetColor(argb);
    return InvalidateSelf();
}

Int32 GradientDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mGradientState->mChangingConfigurations;
}

ECode GradientDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (alpha != mAlpha) {
        mAlpha = alpha;
        InvalidateSelf();
    }
    return NOERROR;
}

ECode GradientDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    if (dither != mDither) {
        mDither = dither;
        InvalidateSelf();
    }
    return NOERROR;
}

ECode GradientDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (cf != mColorFilter) {
        mColorFilter = cf;
        InvalidateSelf();
    }
    return NOERROR;
}

Int32 GradientDrawable::GetOpacity()
{
    return mGradientState->mOpaque ? IPixelFormat::OPAQUE : IPixelFormat::TRANSLUCENT;
}

void GradientDrawable::OnBoundsChange(
    /* [in] */ IRect* r)
{
    Drawable::OnBoundsChange(r);
    mRingPath = NULL;
    mPathIsDirty = TRUE;
    mRectIsDirty = TRUE;
}

Boolean GradientDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    Drawable::OnLevelChange(level);
    mRectIsDirty = TRUE;
    mPathIsDirty = TRUE;
    InvalidateSelf();
    return TRUE;
}

Boolean GradientDrawable::EnsureValidRect()
{
    if (mRectIsDirty) {
        mRectIsDirty = FALSE;

        AutoPtr<CRect> bounds = (CRect*)GetBounds().Get();
        Float inset = 0;

        if (mStrokePaint != NULL) {
            mStrokePaint->GetStrokeWidth(&inset);
            inset = inset * 0.5f;
        }

        GradientState* st = mGradientState;

        mRect->Set(bounds->mLeft + inset, bounds->mTop + inset,
                   bounds->mRight - inset, bounds->mBottom - inset);

        ArrayOf<Int32>* colors = st->mColors;
        if (colors != NULL) {
            CRectF* r = mRect;
            Float x0, x1, y0, y1;

            if (st->mGradient == IGradientDrawable::LINEAR_GRADIENT) {
                Float level = st->mUseLevel ? (Float)GetLevel() / 10000.0f : 1.0f;
                switch (st->mOrientation) {
                case GradientDrawableOrientation_TOP_BOTTOM:
                    x0 = r->mLeft;          y0 = r->mTop;
                    x1 = x0;                y1 = level * r->mBottom;
                    break;
                case GradientDrawableOrientation_TR_BL:
                    x0 = r->mRight;         y0 = r->mTop;
                    x1 = level * r->mLeft;  y1 = level * r->mBottom;
                    break;
                case GradientDrawableOrientation_RIGHT_LEFT:
                    x0 = r->mRight;         y0 = r->mTop;
                    x1 = level * r->mLeft;  y1 = y0;
                    break;
                case GradientDrawableOrientation_BR_TL:
                    x0 = r->mRight;         y0 = r->mBottom;
                    x1 = level * r->mLeft;  y1 = level * r->mTop;
                    break;
                case GradientDrawableOrientation_BOTTOM_TOP:
                    x0 = r->mLeft;          y0 = r->mBottom;
                    x1 = x0;                y1 = level * r->mTop;
                    break;
                case GradientDrawableOrientation_BL_TR:
                    x0 = r->mLeft;          y0 = r->mBottom;
                    x1 = level * r->mRight; y1 = level * r->mTop;
                    break;
                case GradientDrawableOrientation_LEFT_RIGHT:
                    x0 = r->mLeft;          y0 = r->mTop;
                    x1 = level * r->mRight; y1 = y0;
                    break;
                default:/* TL_BR */
                    x0 = r->mLeft;          y0 = r->mTop;
                    x1 = level * r->mRight; y1 = level * r->mBottom;
                    break;
                }

                AutoPtr<IShader> shader;
                CLinearGradient::New(x0, y0, x1, y1, *colors,
                        st->mPositions, ShaderTileMode_CLAMP, (ILinearGradient**)&shader);
                mFillPaint->SetShader(shader);
                if (!mGradientState->mHasSolidColor) {
                    mFillPaint->SetColor(mAlpha << 24);
                }
            }
            else if (st->mGradient == IGradientDrawable::RADIAL_GRADIENT) {
                x0 = r->mLeft + (r->mRight - r->mLeft) * st->mCenterX;
                y0 = r->mTop + (r->mBottom - r->mTop) * st->mCenterY;

                Float level = st->mUseLevel ? (Float)GetLevel() / 10000.0f : 1.0f;

                AutoPtr<IShader> shader;
                CRadialGradient::New(x0, y0,
                        level * st->mGradientRadius, *colors, NULL,
                        ShaderTileMode_CLAMP, (IRadialGradient**)&shader);
                mFillPaint->SetShader(shader);
                if (!mGradientState->mHasSolidColor) {
                    mFillPaint->SetColor(mAlpha << 24);
                }
            }
            else if (st->mGradient == IGradientDrawable::SWEEP_GRADIENT) {
                x0 = r->mLeft + (r->mRight - r->mLeft) * st->mCenterX;
                y0 = r->mTop + (r->mBottom - r->mTop) * st->mCenterY;

                AutoPtr<ArrayOf<Int32> > tempColors = colors;
                AutoPtr<ArrayOf<Float> > tempPositions;

                if (st->mUseLevel) {
                    tempColors = st->mTempColors;
                    Int32 length = colors->GetLength();
                    if (tempColors == NULL || tempColors->GetLength() != length + 1) {
                        tempColors = st->mTempColors = ArrayOf<Int32>::Alloc(length + 1);
                    }
                    tempColors->Copy(colors, length);
                    (*tempColors)[length] = (*colors)[length - 1];

                    tempPositions = st->mTempPositions;
                    Float fraction = 1.0f / (Float)(length - 1);
                    if (tempPositions == NULL || tempPositions->GetLength() != length + 1) {
                        tempPositions = st->mTempPositions = ArrayOf<Float>::Alloc(length + 1);
                    }

                    Float level = (Float)GetLevel() / 10000.0f;
                    for (Int32 i = 0; i < length; i++) {
                        (*tempPositions)[i] = i * fraction * level;
                    }
                    (*tempPositions)[length] = 1.0f;

                }
                AutoPtr<IShader> shader;
                CSweepGradient::New(x0, y0, *tempColors, tempPositions, (ISweepGradient**)&shader);
                mFillPaint->SetShader(shader);
                if (!mGradientState->mHasSolidColor) {
                    mFillPaint->SetColor(mAlpha << 24);
                }
            }
        }
    }
    Boolean empty;
    mRect->IsEmpty(&empty);
    return !empty;
}

ECode GradientDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<GradientState> st = mGradientState;

    Int32 size = ARRAY_SIZE(R::styleable::GradientDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::GradientDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout,(ITypedArray**)&a);

    Drawable::InflateWithAttributes(r, parser, a, R::styleable::GradientDrawable_visible);

    Int32 shapeType;
    a->GetInt32(R::styleable::GradientDrawable_shape, IGradientDrawable::RECTANGLE, &shapeType);
    Boolean dither;
    a->GetBoolean(R::styleable::GradientDrawable_dither, FALSE, &dither);

    if (shapeType == IGradientDrawable::RING) {
        a->GetDimensionPixelSize(R::styleable::GradientDrawable_innerRadius, -1, &st->mInnerRadius);
        if (st->mInnerRadius == -1) {
            a->GetFloat(R::styleable::GradientDrawable_innerRadiusRatio, 3.0f, &st->mInnerRadiusRatio);
        }
        a->GetDimensionPixelSize(R::styleable::GradientDrawable_thickness, -1, &st->mThickness);
        if (st->mThickness == -1) {
            a->GetFloat(R::styleable::GradientDrawable_thicknessRatio, 9.0f, &st->mThicknessRatio);
        }
        a->GetBoolean(R::styleable::GradientDrawable_useLevel, TRUE, &st->mUseLevelForShape);
    }

    a->Recycle();

    SetShape(shapeType);
    SetDither(dither);

    Int32 type;

    Int32 innerDepth, depth;
    parser->GetDepth(&innerDepth);
    innerDepth += 1;
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT)
           && ((parser->GetDepth(&depth), depth >= innerDepth)
                   || type != IXmlPullParser::END_TAG)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        if (depth > innerDepth) {
            continue;
        }

        a = NULL;
        String name;
        parser->GetName(&name);

        if (name.Equals("size")) {
            size = ARRAY_SIZE(R::styleable::GradientDrawableSize);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::GradientDrawableSize, size);
            r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 width, height;
            a->GetDimensionPixelSize(R::styleable::GradientDrawableSize_width, -1, &width);
            a->GetDimensionPixelSize(R::styleable::GradientDrawableSize_height, -1, &height);
            a->Recycle();
            SetSize(width, height);
        }
        else if (name.Equals("gradient")) {
            size = ARRAY_SIZE(R::styleable::GradientDrawableGradient);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::GradientDrawableGradient, size);
            r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 startColor;
            a->GetColor(R::styleable::GradientDrawableGradient_startColor, 0, &startColor);
            Boolean hasCenterColor;
            a->HasValue(R::styleable::GradientDrawableGradient_centerColor, &hasCenterColor);
            Int32 centerColor;
            a->GetColor(R::styleable::GradientDrawableGradient_centerColor, 0, &centerColor);
            Int32 endColor;
            a->GetColor(R::styleable::GradientDrawableGradient_endColor, 0, &endColor);
            Int32 gradientType;
            a->GetInt32(R::styleable::GradientDrawableGradient_type,
                IGradientDrawable::LINEAR_GRADIENT, &gradientType);

            st->mCenterX = GetFloatOrFraction(a, R::styleable::GradientDrawableGradient_centerX, 0.5f);
            st->mCenterY = GetFloatOrFraction(a, R::styleable::GradientDrawableGradient_centerY, 0.5f);

            a->GetBoolean(R::styleable::GradientDrawableGradient_useLevel, FALSE, &st->mUseLevel);
            st->mGradient = gradientType;

            if (gradientType == IGradientDrawable::LINEAR_GRADIENT) {
                Float anglef;
                a->GetFloat(R::styleable::GradientDrawableGradient_angle, 0, &anglef);
                Int32 angle = (Int32)anglef % 360;
                if (angle % 45 != 0) {
//                    throw new XmlPullParserException(a.getPositionDescription()
//                            + "<gradient> tag requires 'angle' attribute to "
//                            + "be a multiple of 45");
                    return E_XML_PULL_PARSER_EXCEPTION;
                }

                switch (angle) {
                case 0:
                    st->mOrientation = GradientDrawableOrientation_LEFT_RIGHT;
                    break;
                case 45:
                    st->mOrientation = GradientDrawableOrientation_BL_TR;
                    break;
                case 90:
                    st->mOrientation = GradientDrawableOrientation_BOTTOM_TOP;
                    break;
                case 135:
                    st->mOrientation = GradientDrawableOrientation_BR_TL;
                    break;
                case 180:
                    st->mOrientation = GradientDrawableOrientation_RIGHT_LEFT;
                    break;
                case 225:
                    st->mOrientation = GradientDrawableOrientation_TR_BL;
                    break;
                case 270:
                    st->mOrientation = GradientDrawableOrientation_TOP_BOTTOM;
                    break;
                case 315:
                    st->mOrientation = GradientDrawableOrientation_TL_BR;
                    break;
                }
            }
            else {
                AutoPtr<ITypedValue> tv;
                a->PeekValue(R::styleable::GradientDrawableGradient_gradientRadius, (ITypedValue**)&tv);
                if (tv != NULL) {
                    Int32 type;
                    tv->GetType(&type);
                    Boolean radiusRel = type == ITypedValue::TYPE_FRACTION;
                    radiusRel ? tv->GetFraction(1.0f, 1.0f, &st->mGradientRadius)
                              : tv->GetFloat(&st->mGradientRadius);
                }
                else if (gradientType == IGradientDrawable::RADIAL_GRADIENT) {
//                    throw new XmlPullParserException(
//                            a.getPositionDescription()
//                            + "<gradient> tag requires 'gradientRadius' "
//                            + "attribute with radial type");
                    return E_XML_PULL_PARSER_EXCEPTION;
                }
            }

            a->Recycle();

            if (hasCenterColor) {
                st->mColors = ArrayOf<Int32>::Alloc(3);
                (*st->mColors)[0] = startColor;
                (*st->mColors)[1] = centerColor;
                (*st->mColors)[2] = endColor;

                st->mPositions = ArrayOf<Float>::Alloc(3);
                (*st->mPositions)[0] = 0.0f;
                // Since 0.5f is default value, try to take the one that isn't 0.5f
                (*st->mPositions)[1] = st->mCenterX != 0.5f ? st->mCenterX : st->mCenterY;
                (*st->mPositions)[2] = 1.0f;
            }
            else {
                st->mColors = ArrayOf<Int32>::Alloc(2);
                (*st->mColors)[0] = startColor;
                (*st->mColors)[1] = endColor;
            }

        }
        else if (name.Equals("solid")) {
            size = ARRAY_SIZE(R::styleable::GradientDrawableSolid);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::GradientDrawableSolid, size);

            r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 argb;
            a->GetColor(R::styleable::GradientDrawableSolid_color, 0, &argb);
            a->Recycle();
            SetColor(argb);
        }
        else if (name.Equals("stroke")) {
            size = ARRAY_SIZE(R::styleable::GradientDrawableStroke);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::GradientDrawableStroke, size);

            r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 width;
            a->GetDimensionPixelSize(R::styleable::GradientDrawableStroke_width, 0, &width);
            Int32 color;
            a->GetColor(R::styleable::GradientDrawableStroke_color, 0, &color);
            Float dashWidth;
            a->GetDimension(R::styleable::GradientDrawableStroke_dashWidth, 0, &dashWidth);
            if (dashWidth != 0.0f) {
                Float dashGap;
                a->GetDimension(R::styleable::GradientDrawableStroke_dashGap, 0, &dashGap);
                SetStroke(width, color, dashWidth, dashGap);
            }
            else {
                SetStroke(width, color);
            }
            a->Recycle();
        }
        else if (name.Equals("corners")) {
            size = ARRAY_SIZE(R::styleable::DrawableCorners);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::DrawableCorners, size);

            r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 radius;
            a->GetDimensionPixelSize(R::styleable::DrawableCorners_radius, 0, &radius);
            SetCornerRadius(radius);
            Int32 topLeftRadius;
            a->GetDimensionPixelSize(R::styleable::DrawableCorners_topLeftRadius, radius, &topLeftRadius);
            Int32 topRightRadius;
            a->GetDimensionPixelSize(R::styleable::DrawableCorners_topRightRadius, radius, &topRightRadius);
            Int32 bottomLeftRadius;
            a->GetDimensionPixelSize(R::styleable::DrawableCorners_bottomLeftRadius, radius, &bottomLeftRadius);
            Int32 bottomRightRadius;
            a->GetDimensionPixelSize(R::styleable::DrawableCorners_bottomRightRadius, radius, &bottomRightRadius);
            if (topLeftRadius != radius || topRightRadius != radius ||
                    bottomLeftRadius != radius || bottomRightRadius != radius) {
                // The corner radii are specified in clockwise order (see Path.addRoundRect())
                AutoPtr< ArrayOf<Float> > radii = ArrayOf<Float>::Alloc(8);
                (*radii)[0] = topLeftRadius;
                (*radii)[1] = topLeftRadius;
                (*radii)[2] = topRightRadius;
                (*radii)[3] = topRightRadius;
                (*radii)[4] = bottomRightRadius;
                (*radii)[5] = bottomRightRadius;
                (*radii)[6] = bottomLeftRadius;
                (*radii)[7] = bottomLeftRadius;
                SetCornerRadii(radii);
            }
            a->Recycle();
        }
        else if (name.Equals("padding")) {
            size = ARRAY_SIZE(R::styleable::GradientDrawablePadding);
            layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::GradientDrawablePadding, size);

            r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
            Int32 left, top, right, bottom;
            a->GetDimensionPixelOffset(
                    R::styleable::GradientDrawablePadding_left, 0, &left),
            a->GetDimensionPixelOffset(
                    R::styleable::GradientDrawablePadding_top, 0, &top),
            a->GetDimensionPixelOffset(
                    R::styleable::GradientDrawablePadding_right, 0, &right),
            a->GetDimensionPixelOffset(
                    R::styleable::GradientDrawablePadding_bottom, 0, &bottom);
            mPadding = NULL;
            CRect::NewByFriend(left, top, right, bottom, (CRect**)&mPadding);
            a->Recycle();
            mGradientState->mPadding = mPadding;
        }
        else {
            Logger::W("drawable", "Bad element under <shape>: %s", name.string());
        }
    }

    mGradientState->ComputeOpacity();
    return NOERROR;
}

Float GradientDrawable::GetFloatOrFraction(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [in] */ Float defaultValue)
{
    AutoPtr<ITypedValue> tv;
    a->PeekValue(index, (ITypedValue**)&tv);
    Float v = defaultValue;
    if (tv != NULL) {
        Int32 type;
        tv->GetType(&type);
        Boolean vIsFraction = type == ITypedValue::TYPE_FRACTION;
        vIsFraction ? tv->GetFraction(1.0f, 1.0f, &v) : tv->GetFloat(&v);
    }
    return v;
}

Int32 GradientDrawable::GetIntrinsicWidth()
{
    return mGradientState->mWidth;
}

Int32 GradientDrawable::GetIntrinsicHeight()
{
    return mGradientState->mHeight;
}

AutoPtr<IDrawableConstantState> GradientDrawable::GetConstantState()
{
    mGradientState->mChangingConfigurations = GetChangingConfigurations();
    return (IDrawableConstantState*)mGradientState.Get();
}

AutoPtr<IDrawable> GradientDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mGradientState = new GradientState(mGradientState);
        InitializeWithState(mGradientState);
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

GradientDrawable::GradientDrawable(
    /* [in] */ GradientState* state)
    : mAlpha(0xFF)
    , mDither(FALSE)
    , mRectIsDirty(FALSE)
    , mMutated(FALSE)
    , mPathIsDirty(TRUE)
{
    Init(state);
}

void GradientDrawable::InitializeWithState(
    /* [in] */ GradientState* state)
{
    if (state->mHasSolidColor) {
        mFillPaint->SetColor(state->mSolidColor);
    }
    else if (state->mColors == NULL) {
        // If we don't have a solid color and we don't have a gradient,
        // the app is stroking the shape, set the color to the default
        // value of state.mSolidColor
        mFillPaint->SetColor(0);
    }
    mPadding = state->mPadding;
    if (state->mStrokeWidth >= 0) {
        mStrokePaint = NULL;
        CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mStrokePaint);
        mStrokePaint->SetStyle(PaintStyle_STROKE);
        mStrokePaint->SetStrokeWidth(state->mStrokeWidth);
        mStrokePaint->SetColor(state->mStrokeColor);

        if (state->mStrokeDashWidth != 0.0f) {
            AutoPtr<IDashPathEffect> e;
            ArrayOf_<Float, 2> intervals;
            intervals[0] = state->mStrokeDashWidth;
            intervals[1] = state->mStrokeDashGap;
            CDashPathEffect::New(intervals, 0, (IDashPathEffect**)&e);
            mStrokePaint->SetPathEffect((IPathEffect*)e.Get());
        }
    }
}

ECode GradientDrawable::Init()
{
    AutoPtr<GradientState> state = new GradientState(GradientDrawableOrientation_TOP_BOTTOM, NULL);
    return Init(state);
}

ECode GradientDrawable::Init(
    /* [in] */ GradientDrawableOrientation orientation,
    /* [in] */ ArrayOf<Int32>* colors)
{
    AutoPtr<GradientState> state = new GradientState(orientation, colors);
    return Init(state);
}

ECode GradientDrawable::Init(
    /* [in] */ GradientState* state)
{
    mGradientState = state;
    InitializeWithState(state);
    mRectIsDirty = TRUE;
    mMutated = FALSE;
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
