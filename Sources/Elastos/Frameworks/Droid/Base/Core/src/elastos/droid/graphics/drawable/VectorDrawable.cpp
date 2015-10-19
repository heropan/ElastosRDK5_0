
#include "elastos/droid/graphics/drawable/VectorDrawable.h"
#include "elastos/droid/graphics/drawable/CVectorDrawable.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/graphics/CMatrix.h"
#include "elastos/droid/graphics/CPath.h"
#include "elastos/droid/graphics/CPathMeasure.h"
#include "elastos/droid/graphics/CColor.h"
#include "elastos/droid/content/res/CTypedArray.h"
#include "elastos/droid/utility/CArrayMap.h"
#include "elastos/droid/utility/Xml.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/utility/logging/Logger.h>
#include <math.h>

using Elastos::Droid::Content::Res::CTypedArray;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Utility::CArrayMap;
using Elastos::Droid::Utility::ILayoutDirection;
using Elastos::Droid::Utility::PathParser;
using Elastos::Droid::Utility::Xml;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuffer;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CStack;
using Elastos::Utility::IStack;
using Elastos::Utility::IList;
using Elastos::Utility::IMap;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

const String VectorDrawable::LOGTAG = String("VectorDrawable")/*.class.getSimpleName()*/;
const String VectorDrawable::SHAPE_CLIP_PATH = String("clip-path");
const String VectorDrawable::SHAPE_GROUP = String("group");
const String VectorDrawable::SHAPE_PATH = String("path");
const String VectorDrawable::SHAPE_VECTOR = String("vector");
const Int32 VectorDrawable::LINECAP_BUTT = 0;
const Int32 VectorDrawable::LINECAP_ROUND = 1;
const Int32 VectorDrawable::LINECAP_SQUARE = 2;
const Int32 VectorDrawable::LINEJOIN_MITER = 0;
const Int32 VectorDrawable::LINEJOIN_ROUND = 1;
const Int32 VectorDrawable::LINEJOIN_BEVEL = 2;
const Boolean VectorDrawable::DBG_VECTOR_DRAWABLE = FALSE;

VectorDrawable::VectorDrawableState::VectorDrawableState(
    /* [in] */ VectorDrawableState* copy)
    : mChangingConfigurations(0)
    , mTintMode(DEFAULT_TINT_MODE)
    , mAutoMirrored(FALSE)
    , mCachedTintMode(-1)
    , mCachedRootAlpha(0)
    , mCachedAutoMirrored(FALSE)
    , mCacheDirty(FALSE)
{
    if (copy != NULL) {
        mThemeAttrs = copy->mThemeAttrs;
        mChangingConfigurations = copy->mChangingConfigurations;
        mVPathRenderer = new VPathRenderer(copy->mVPathRenderer);
        if (copy->mVPathRenderer->mFillPaint != NULL) {
            CPaint::New(copy->mVPathRenderer->mFillPaint, (IPaint**)&mVPathRenderer->mFillPaint);
        }
        if (copy->mVPathRenderer->mStrokePaint != NULL) {
            CPaint::New(copy->mVPathRenderer->mStrokePaint, (IPaint**)&mVPathRenderer->mStrokePaint);
        }
        mTint = copy->mTint;
        mTintMode = copy->mTintMode;
        mAutoMirrored = copy->mAutoMirrored;
    }
}

void VectorDrawable::VectorDrawableState::DrawCachedBitmapWithRootAlpha(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IColorFilter* filter)
{
    // The bitmap's size is the same as the bounds.
    AutoPtr<IPaint> p = GetPaint(filter);
    canvas->DrawBitmap(mCachedBitmap, 0.f, 0.f, p);
}

Boolean VectorDrawable::VectorDrawableState::HasTranslucentRoot()
{
    return mVPathRenderer->GetRootAlpha() < 255;
}

AutoPtr<IPaint> VectorDrawable::VectorDrawableState::GetPaint(
    /* [in] */ IColorFilter* filter)
{
    if (!HasTranslucentRoot() && filter == NULL) {
        return NULL;
    }

    if (mTempPaint == NULL) {
        CPaint::New((IPaint**)&mTempPaint);
        mTempPaint->SetFilterBitmap(TRUE);
    }
    mTempPaint->SetAlpha(mVPathRenderer->GetRootAlpha());
    mTempPaint->SetColorFilter(filter);
    return mTempPaint;
}

void VectorDrawable::VectorDrawableState::UpdateCachedBitmap(
    /* [in] */ IRect* bounds)
{
    mCachedBitmap->EraseColor(IColor::TRANSPARENT);
    AutoPtr<ICanvas> tmpCanvas;
    CCanvas::New(mCachedBitmap, (ICanvas**)&tmpCanvas);
    Int32 width = 0, height = 0;
    bounds->GetWidth(&width);
    bounds->GetHeight(&height);
    mVPathRenderer->Draw(tmpCanvas, width, height, NULL);
}

void VectorDrawable::VectorDrawableState::CreateCachedBitmapIfNeeded(
    /* [in] */ IRect* bounds)
{
    Int32 width = 0, height = 0;
    bounds->GetWidth(&width);
    bounds->GetHeight(&height);
    if (mCachedBitmap == NULL || !CanReuseBitmap(width, height)) {
        CBitmap::CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&mCachedBitmap);
        mCacheDirty = TRUE;
    }
}

Boolean VectorDrawable::VectorDrawableState::CanReuseBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    Int32 w = 0, h = 0;
    mCachedBitmap->GetWidth(&w);
    mCachedBitmap->GetHeight(&h);

    if (width == w && height == h) {
        return TRUE;
    }
    return FALSE;
}

Boolean VectorDrawable::VectorDrawableState::CanReuseCache()
{
    if (!mCacheDirty
            && mCachedThemeAttrs == mThemeAttrs
            && mCachedTint == mTint
            && mCachedTintMode == mTintMode
            && mCachedAutoMirrored == mAutoMirrored
            && mCachedRootAlpha == mVPathRenderer->GetRootAlpha()) {
        return TRUE;
    }
    return FALSE;
}

void VectorDrawable::VectorDrawableState::UpdateCacheStates()
{
    // Use shallow copy here and shallow comparison in canReuseCache(),
    // likely hit cache miss more, but practically not much difference.
    mCachedThemeAttrs = mThemeAttrs;
    mCachedTint = mTint;
    mCachedTintMode = mTintMode;
    mCachedRootAlpha = mVPathRenderer->GetRootAlpha();
    mCachedAutoMirrored = mAutoMirrored;
    mCacheDirty = FALSE;
}

ECode VectorDrawable::VectorDrawableState::CanApplyTheme(
    /* [out] */ Boolean* can)
{
    VALIDATE_NOT_NULL(can);
    *can = (ConstantState::CanApplyTheme(can), *can) || mThemeAttrs != NULL
            || (mVPathRenderer != NULL && mVPathRenderer->CanApplyTheme());
    return NOERROR;
}

VectorDrawable::VectorDrawableState::VectorDrawableState()
    : mChangingConfigurations(0)
    , mTintMode(DEFAULT_TINT_MODE)
    , mAutoMirrored(FALSE)
    , mCachedTintMode(-1)
    , mCachedRootAlpha(0)
    , mCachedAutoMirrored(FALSE)
    , mCacheDirty(FALSE)
{
    mVPathRenderer = new VPathRenderer();
}

ECode VectorDrawable::VectorDrawableState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CVectorDrawable::New(this, NULL, NULL, drawable);
}

ECode VectorDrawable::VectorDrawableState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CVectorDrawable::New(this, res, NULL, drawable);
}

ECode VectorDrawable::VectorDrawableState::NewDrawable(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CVectorDrawable::New(this, res, theme, drawable);
}

ECode VectorDrawable::VectorDrawableState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config)
    *config = mChangingConfigurations;
    return NOERROR;
}

///////////////////////////////////// VPathRenderer ////////////////////////////
AutoPtr<IMatrix> VectorDrawable::VPathRenderer::IDENTITY_MATRIX = InitStatic();
VectorDrawable::VPathRenderer::VPathRenderer()
    : mChangingConfigurations(0)
    , mBaseWidth(0)
    , mBaseHeight(0)
    , mViewportWidth(0)
    , mViewportHeight(0)
    , mRootAlpha(0xFF)
{
    CMatrix::New((IMatrix**)&mFinalPathMatrix);
    CArrayMap::New((IArrayMap**)&mVGTargetsMap);
    mRootGroup = new VGroup();
    CPath::New((IPath**)&mPath);
    CPath::New((IPath**)&mRenderPath);
}

AutoPtr<IMatrix> VectorDrawable::VPathRenderer::InitStatic()
{
    CMatrix::New((IMatrix**)&IDENTITY_MATRIX);
    return IDENTITY_MATRIX;
}

void VectorDrawable::VPathRenderer::SetRootAlpha(
    /* [in] */ Int32 alpha)
{
    mRootAlpha = alpha;
}

Int32 VectorDrawable::VPathRenderer::GetRootAlpha()
{
    return mRootAlpha;
}

void VectorDrawable::VPathRenderer::SetAlpha(
    /* [in] */ Float alpha)
{
    SetRootAlpha((Int32) (alpha * 255));
}

Float VectorDrawable::VPathRenderer::GetAlpha()
{
    return GetRootAlpha() / 255.0f;
}

VectorDrawable::VPathRenderer::VPathRenderer(
    /* [in] */ VPathRenderer* copy)
    : mChangingConfigurations(0)
    , mBaseWidth(0)
    , mBaseHeight(0)
    , mViewportWidth(0)
    , mViewportHeight(0)
    , mRootAlpha(0xFF)
{
    CMatrix::New((IMatrix**)&mFinalPathMatrix);
    CArrayMap::New((IArrayMap**)&mVGTargetsMap);

    mRootGroup = new VGroup(copy->mRootGroup, mVGTargetsMap);
    CPath::New(copy->mPath, (IPath**)&mPath);
    CPath::New(copy->mRenderPath, (IPath**)&mRenderPath);
    mBaseWidth = copy->mBaseWidth;
    mBaseHeight = copy->mBaseHeight;
    mViewportWidth = copy->mViewportWidth;
    mViewportHeight = copy->mViewportHeight;
    mChangingConfigurations = copy->mChangingConfigurations;
    mRootAlpha = copy->mRootAlpha;
    mRootName = copy->mRootName;
    if (copy->mRootName != NULL) {
        AutoPtr<ICharSequence> cs;
        CString::New(copy->mRootName, (ICharSequence**)&cs);
        IMap::Probe(mVGTargetsMap)->Put(cs, (IObject*)this);
    }
}

Boolean VectorDrawable::VPathRenderer::CanApplyTheme()
{
    // If one of the paths can apply theme, then return TRUE;
    return RecursiveCanApplyTheme(mRootGroup);
}

Boolean VectorDrawable::VPathRenderer::RecursiveCanApplyTheme(
    /* [in] */ VGroup* currentGroup)
{
    // We can do a tree traverse here, if there is one path return TRUE,
    // then we return TRUE for the whole tree.
    AutoPtr<IArrayList> children = currentGroup->mChildren;
    Int32 size = 0;
    IList::Probe(children)->GetSize(&size);

    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> child;
        children->Get(i, (IInterface**)&child);
        if (IVGroup::Probe(child)) {
            AutoPtr<VGroup> childGroup = (VGroup*)IVGroup::Probe(child);
            if (childGroup->CanApplyTheme()
                    || RecursiveCanApplyTheme(childGroup)) {
                return TRUE;
            }
        } else if (IVPath::Probe(child)) {
            AutoPtr<VPath> childPath = (VPath*)IVPath::Probe(child);
            if (childPath->CanApplyTheme()) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void VectorDrawable::VPathRenderer::ApplyTheme(
    /* [in] */ IResourcesTheme* t)
{
    // Apply theme to every path of the tree.
    RecursiveApplyTheme(mRootGroup, t);
}

void VectorDrawable::VPathRenderer::RecursiveApplyTheme(
    /* [in] */ VGroup* currentGroup,
    /* [in] */ IResourcesTheme* t)
{
    // We can do a tree traverse here, apply theme to all paths which
    // can apply theme.
    AutoPtr<IArrayList> children = currentGroup->mChildren;
    Int32 size = 0;
    IList::Probe(children)->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> child;
        children->Get(i, (IInterface**)&child);
        if (IVGroup::Probe(child)) {
            AutoPtr<VGroup> childGroup = (VGroup*)IVGroup::Probe(child);
            if (childGroup->CanApplyTheme()) {
                childGroup->ApplyTheme(t);
            }
            RecursiveApplyTheme(childGroup, t);
        } else if (IVPath::Probe(child)) {
            AutoPtr<VPath> childPath = (VPath*)IVPath::Probe(child);
            if (childPath->CanApplyTheme()) {
                childPath->ApplyTheme(t);
            }
        }
    }
}

void VectorDrawable::VPathRenderer::DrawGroupTree(
    /* [in] */ VGroup* currentGroup,
    /* [in] */ IMatrix* currentMatrix,
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IColorFilter* filter)
{
    // Calculate current group's matrix by preConcat the parent's and
    // and the current one on the top of the stack.
    // Basically the Mfinal = Mviewport * M0 * M1 * M2;
    // Mi the local matrix at level i of the group tree.
    currentGroup->mStackedMatrix->Set(currentMatrix);

    Boolean res = FALSE;
    currentGroup->mStackedMatrix->PreConcat(currentGroup->mLocalMatrix, &res);

    // Draw the group tree in the same order as the XML file.
    Int32 size = 0;
    currentGroup->mChildren->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> child;
        currentGroup->mChildren->Get(i, (IInterface**)&child);
        if (IVGroup::Probe(child)) {
            AutoPtr<VGroup> childGroup = (VGroup*)IVGroup::Probe(child);
            DrawGroupTree(childGroup, currentGroup->mStackedMatrix,
                    canvas, w, h, filter);
        } else if (IVPath::Probe(child)) {
            AutoPtr<VPath> childPath = (VPath*)IVPath::Probe(child);
            DrawPath(currentGroup, childPath, canvas, w, h, filter);
        }
    }
}

void VectorDrawable::VPathRenderer::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IColorFilter* filter)
{
    // Travese the tree in pre-order to draw.
    DrawGroupTree(mRootGroup, IDENTITY_MATRIX, canvas, w, h, filter);
}

void VectorDrawable::VPathRenderer::DrawPath(
    /* [in] */ VGroup* vGroup,
    /* [in] */ VPath* vPath,
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IColorFilter* filter)
{
    Float scaleX = w / mViewportWidth;
    Float scaleY = h / mViewportHeight;
    Float minScale = Elastos::Core::Math::Min(scaleX, scaleY);

    mFinalPathMatrix->Set(vGroup->mStackedMatrix);
    Boolean res = FALSE;
    mFinalPathMatrix->PostScale(scaleX, scaleY, &res);

    vPath->ToPath(mPath);
    AutoPtr<IPath> path = mPath;

    mRenderPath->Reset();

    if (vPath->IsClipPath()) {
        mRenderPath->AddPath(path, mFinalPathMatrix);
        Boolean isNonEmpty = FALSE;
        canvas->ClipPath(mRenderPath, RegionOp_REPLACE, &isNonEmpty);
    } else {
        AutoPtr<VFullPath> fullPath = (VFullPath*) vPath;
        if (fullPath->mTrimPathStart != 0.0f || fullPath->mTrimPathEnd != 1.0f) {
            // Float start = (fullPath->mTrimPathStart + fullPath->mTrimPathOffset) % 1.0f;
            // Float end = (fullPath->mTrimPathEnd + fullPath->mTrimPathOffset) % 1.0f;
            Float start = fmod((fullPath->mTrimPathStart + fullPath->mTrimPathOffset), 1.0f);
            Float end = fmod((fullPath->mTrimPathEnd + fullPath->mTrimPathOffset), 1.0f);

            if (mPathMeasure == NULL) {
                CPathMeasure::New((IPathMeasure**)&mPathMeasure);
            }
            mPathMeasure->SetPath(mPath, FALSE);

            Float len = 0;
            mPathMeasure->GetLength(&len);
            start = start * len;
            end = end * len;
            path->Reset();
            Boolean bv = FALSE;
            if (start > end) {
                mPathMeasure->GetSegment(start, len, path, TRUE, &bv);
                mPathMeasure->GetSegment(0.f, end, path, TRUE, &bv);
            } else {
                mPathMeasure->GetSegment(start, end, path, TRUE, &bv);
            }
            path->RLineTo(0, 0); // fix bug in measure
        }
        mRenderPath->AddPath(path, mFinalPathMatrix);

        if (fullPath->mFillColor != IColor::TRANSPARENT) {
            if (mFillPaint == NULL) {
                CPaint::New((IPaint**)&mFillPaint);
                mFillPaint->SetStyle(PaintStyle_FILL);
                mFillPaint->SetAntiAlias(TRUE);
            }

            AutoPtr<IPaint> fillPaint = mFillPaint;
            fillPaint->SetColor(ApplyAlpha(fullPath->mFillColor, fullPath->mFillAlpha));
            fillPaint->SetColorFilter(filter);
            canvas->DrawPath(mRenderPath, fillPaint);
        }

        if (fullPath->mStrokeColor != IColor::TRANSPARENT) {
            if (mStrokePaint == NULL) {
                CPaint::New((IPaint**)&mStrokePaint);
                mStrokePaint->SetStyle(PaintStyle_STROKE);
                mStrokePaint->SetAntiAlias(TRUE);
            }

            AutoPtr<IPaint> strokePaint = mStrokePaint;
            if (fullPath->mStrokeLineJoin != -1) {
                strokePaint->SetStrokeJoin(fullPath->mStrokeLineJoin);
            }

            if (fullPath->mStrokeLineCap != -1) {
                strokePaint->SetStrokeCap(fullPath->mStrokeLineCap);
            }

            strokePaint->SetStrokeMiter(fullPath->mStrokeMiterlimit);
            strokePaint->SetColor(ApplyAlpha(fullPath->mStrokeColor, fullPath->mStrokeAlpha));
            strokePaint->SetColorFilter(filter);
            strokePaint->SetStrokeWidth(fullPath->mStrokeWidth * minScale);
            canvas->DrawPath(mRenderPath, strokePaint);
        }
    }
}

//////////////////////////////////// VGroup ////////////////////////////
CAR_INTERFACE_IMPL(VectorDrawable::VGroup, Object, IVGroup);
VectorDrawable::VGroup::VGroup(
    /* [in] */ VGroup* copy,
    /* [in] */ IArrayMap/*<String, Object>*/* targetsMap)
{
    CMatrix::New((IMatrix**)&mStackedMatrix);
    CArrayList::New((IArrayList**)&mChildren);
    CMatrix::New((IMatrix**)&mLocalMatrix);

    mRotate = copy->mRotate;
    mPivotX = copy->mPivotX;
    mPivotY = copy->mPivotY;
    mScaleX = copy->mScaleX;
    mScaleY = copy->mScaleY;
    mTranslateX = copy->mTranslateX;
    mTranslateY = copy->mTranslateY;
    mThemeAttrs = copy->mThemeAttrs;
    mGroupName = copy->mGroupName;
    mChangingConfigurations = copy->mChangingConfigurations;
    if (mGroupName != NULL) {
        AutoPtr<ICharSequence> cs;
        CString::New(mGroupName, (ICharSequence**)&cs);
        IMap::Probe(targetsMap)->Put(cs, (IVGroup*)this);
    }

    mLocalMatrix->Set(copy->mLocalMatrix);

    AutoPtr<IArrayList> children = copy->mChildren;
    Int32 size = 0;
    children->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> copyChild;
        children->Get(i, (IInterface**)&copyChild);
        if (IVGroup::Probe(copyChild)) {
            AutoPtr<VGroup> copyGroup = (VGroup*)IVGroup::Probe(copyChild);
            AutoPtr<VGroup> vg = new VGroup(copyGroup, targetsMap);
            mChildren->Add((IVGroup*)vg);
        } else {
            AutoPtr<VPath> newPath;
            if (IVFullPath::Probe(copyChild)) {
                newPath = new VFullPath((VFullPath*) IVFullPath::Probe(copyChild));
            } else if (IVClipPath::Probe(copyChild)) {
                newPath = new VClipPath((VClipPath*) IVClipPath::Probe(copyChild));
            } else {
                // throw new IllegalStateException("Unknown object in the tree!");
                // return E_ILLEGAL_STATE_EXCEPTION;
                assert(0);
            }
            mChildren->Add((IVPath*)newPath);
            if (newPath->mPathName != NULL) {
                AutoPtr<ICharSequence> cs;
                CString::New(newPath->mPathName, (ICharSequence**)&cs);
                IMap::Probe(targetsMap)->Put(cs, (IVPath*)newPath);
            }
        }
    }
}

VectorDrawable::VGroup::VGroup()
    : mRotate(0)
    , mPivotX(0)
    , mPivotY(0)
    , mScaleX(1)
    , mScaleY(1)
    , mTranslateX(0)
    , mTranslateY(0)
    , mChangingConfigurations(0)
{
    CMatrix::New((IMatrix**)&mStackedMatrix);
    CArrayList::New((IArrayList**)&mChildren);
    CMatrix::New((IMatrix**)&mLocalMatrix);
}

String VectorDrawable::VGroup::GetGroupName()
{
    return mGroupName;
}

AutoPtr<IMatrix> VectorDrawable::VGroup::GetLocalMatrix()
{
    return mLocalMatrix;
}

void VectorDrawable::VGroup::Inflate(
    /* [in] */ IResources* res,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IResourcesTheme* theme)
{
    AutoPtr<ITypedArray> a;
    Int32 size = 0;
    assert(0 && "TODO");
    // size = ARRAY_SIZE(R::styleable::VectorDrawableGroup);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    // layout->Copy(R::styleable::VectorDrawableGroup, size);
    ObtainAttributes(res, theme, attrs, layout, (ITypedArray**)&a);
    UpdateStateFromTypedArray(a);
    a->Recycle();
}

void VectorDrawable::VGroup::UpdateStateFromTypedArray(
    /* [in] */ ITypedArray* a)
{
    // Account for any configuration changes.
    Int32 config = 0;
    a->GetChangingConfigurations(&config);
    mChangingConfigurations |= config;

    // Extract the theme attributes, if any.
    ((CTypedArray*)a)->ExtractThemeAttrs((ArrayOf<Int32>**)&mThemeAttrs);
    assert(0 && "TODO");
    // a->GetFloat(R::styleable::VectorDrawableGroup_rotation, mRotate, &mRotate);
    // a->GetFloat(R::styleable::VectorDrawableGroup_pivotX, mPivotX, &mPivotX);
    // a->GetFloat(R::styleable::VectorDrawableGroup_pivotY, mPivotY, &mPivotY);
    // a->GetFloat(R::styleable::VectorDrawableGroup_scaleX, mScaleX, &mScaleX);
    // a->GetFloat(R::styleable::VectorDrawableGroup_scaleY, mScaleY, &mScaleY);
    // a->GetFloat(R::styleable::VectorDrawableGroup_translateX, mTranslateX, &mTranslateX);
    // a->GetFloat(R::styleable::VectorDrawableGroup_translateY, mTranslateY, &mTranslateY);

    String groupName;
    // a->GetString(R::styleable::VectorDrawableGroup_name, &groupName);
    if (groupName != NULL) {
        mGroupName = groupName;
    }

    UpdateLocalMatrix();
}

Boolean VectorDrawable::VGroup::CanApplyTheme()
{
    return mThemeAttrs != NULL;
}

void VectorDrawable::VGroup::ApplyTheme(
    /* [in] */ IResourcesTheme* t)
{
    if (mThemeAttrs == NULL) {
        return;
    }

    AutoPtr<ITypedArray> a;
    Int32 size = 0;
    assert(0 && "TODO");
    // size = ARRAY_SIZE(R::styleable::VectorDrawableGroup);
    // AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    // layout->Copy(R::styleable::VectorDrawableGroup, size);
    // t->ResolveAttributes(mThemeAttrs, layout, (ITypedArray**)&a);
    UpdateStateFromTypedArray(a);
    a->Recycle();
}

void VectorDrawable::VGroup::UpdateLocalMatrix()
{
    // The order we apply is the same as the
    // RenderNode.cpp::applyViewPropertyTransforms().
    mLocalMatrix->Reset();
    Boolean res = FALSE;
    mLocalMatrix->PostTranslate(-mPivotX, -mPivotY, &res);
    mLocalMatrix->PostScale(mScaleX, mScaleY, &res);
    mLocalMatrix->PostRotate(mRotate, 0, 0, &res);
    mLocalMatrix->PostTranslate(mTranslateX + mPivotX, mTranslateY + mPivotY, &res);
}

Float VectorDrawable::VGroup::GetRotation()
{
    return mRotate;
}

void VectorDrawable::VGroup::SetRotation(
    /* [in] */ Float rotation)
{
    if (rotation != mRotate) {
        mRotate = rotation;
        UpdateLocalMatrix();
    }
}

Float VectorDrawable::VGroup::GetPivotX()
{
    return mPivotX;
}

void VectorDrawable::VGroup::SetPivotX(
    /* [in] */ Float pivotX)
{
    if (pivotX != mPivotX) {
        mPivotX = pivotX;
        UpdateLocalMatrix();
    }
}

Float VectorDrawable::VGroup::GetPivotY()
{
    return mPivotY;
}

void VectorDrawable::VGroup::SetPivotY(
    /* [in] */ Float pivotY)
{
    if (pivotY != mPivotY) {
        mPivotY = pivotY;
        UpdateLocalMatrix();
    }
}

Float VectorDrawable::VGroup::GetScaleX()
{
    return mScaleX;
}

void VectorDrawable::VGroup::SetScaleX(
    /* [in] */ Float scaleX)
{
    if (scaleX != mScaleX) {
        mScaleX = scaleX;
        UpdateLocalMatrix();
    }
}

Float VectorDrawable::VGroup::GetScaleY()
{
    return mScaleY;
}

void VectorDrawable::VGroup::SetScaleY(
    /* [in] */ Float scaleY)
{
    if (scaleY != mScaleY) {
        mScaleY = scaleY;
        UpdateLocalMatrix();
    }
}

Float VectorDrawable::VGroup::GetTranslateX()
{
    return mTranslateX;
}

void VectorDrawable::VGroup::SetTranslateX(
    /* [in] */ Float translateX)
{
    if (translateX != mTranslateX) {
        mTranslateX = translateX;
        UpdateLocalMatrix();
    }
}

Float VectorDrawable::VGroup::GetTranslateY()
{
    return mTranslateY;
}

void VectorDrawable::VGroup::SetTranslateY(
    /* [in] */ Float translateY)
{
    if (translateY != mTranslateY) {
        mTranslateY = translateY;
        UpdateLocalMatrix();
    }
}

////////////////////////////// VPath /////////////////////
CAR_INTERFACE_IMPL(VectorDrawable::VPath, Object, IVPath);
VectorDrawable::VPath::VPath()
    : mChangingConfigurations(0)
{
    // Empty constructor.
}

VectorDrawable::VPath::VPath(
    /* [in] */ VPath* copy)
    : mChangingConfigurations(0)
{
    mPathName = copy->mPathName;
    mChangingConfigurations = copy->mChangingConfigurations;
    mNodes = PathParser::DeepCopyNodes(copy->mNodes);
}

void VectorDrawable::VPath::ToPath(
    /* [in] */ IPath* path)
{
    path->Reset();
    if (mNodes != NULL) {
        PathDataNode::NodesToPath(mNodes, path);
    }
}

String VectorDrawable::VPath::GetPathName()
{
    return mPathName;
}

Boolean VectorDrawable::VPath::CanApplyTheme()
{
    return FALSE;
}

void VectorDrawable::VPath::ApplyTheme(
    /* [in] */ IResourcesTheme* t)
{
}

Boolean VectorDrawable::VPath::IsClipPath()
{
    return FALSE;
}

AutoPtr<ArrayOf<PathDataNode*> > VectorDrawable::VPath::GetPathData()
{
    return mNodes;
}

void VectorDrawable::VPath::SetPathData(
    /* [in] */ ArrayOf<PathDataNode*>* nodes)
{
    if (!PathParser::CanMorph(mNodes, nodes)) {
        // This should not happen in the middle of animation.
        mNodes = PathParser::DeepCopyNodes(nodes);
    } else {
        PathParser::UpdateNodes(mNodes, nodes);
    }
}

///////////////////////////// VClipPath /////////////////////
CAR_INTERFACE_IMPL(VectorDrawable::VClipPath, VPath, IVClipPath);
VectorDrawable::VClipPath::VClipPath()
{
    // Empty constructor.
}

VectorDrawable::VClipPath::VClipPath(
    /* [in] */ VClipPath* copy)
    : VPath(copy)
{
}

void VectorDrawable::VClipPath::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IResourcesTheme* theme)
{
    AutoPtr<ITypedArray> a;
    Int32 size = 0;
    assert(0 && "TODO");
    // size = ARRAY_SIZE(R::styleable::VectorDrawableClipPath);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    // layout->Copy(R::styleable::VectorDrawableClipPath, size);
    ObtainAttributes(r, theme, attrs, layout, (ITypedArray**)&a);
    UpdateStateFromTypedArray(a);
    a->Recycle();
}

void VectorDrawable::VClipPath::UpdateStateFromTypedArray(
    /* [in] */ ITypedArray* a)
{
    // Account for any configuration changes.
    Int32 config = 0;
    a->GetChangingConfigurations(&config);
    mChangingConfigurations |= config;

    String pathName;
    assert(0 && "TODO");
    // a->GetString(R::styleable::VectorDrawableClipPath_name, &pathName);
    if (pathName != NULL) {
        mPathName = pathName;
    }

    String pathData;
    assert(0 && "TODO");
    // a->GetString(R::styleable::VectorDrawableClipPath_pathData, &pathData);
    if (pathData != NULL) {
        mNodes = PathParser::CreateNodesFromPathData(pathData);
    }
}

Boolean VectorDrawable::VClipPath::IsClipPath()
{
    return TRUE;
}

///////////////////////////// VFullPath /////////////////
CAR_INTERFACE_IMPL(VectorDrawable::VFullPath, VPath, IVFullPath);
VectorDrawable::VFullPath::VFullPath()
    : mStrokeColor(IColor::TRANSPARENT)
    , mStrokeWidth(0)
    , mFillColor(IColor::TRANSPARENT)
    , mStrokeAlpha(1.0f)
    , mFillRule(0)
    , mFillAlpha(1.0f)
    , mTrimPathStart(0)
    , mTrimPathEnd(1)
    , mTrimPathOffset(0)
    , mStrokeLineCap(PaintCap_BUTT)
    , mStrokeLineJoin(PaintJoin_MITER)
    , mStrokeMiterlimit(4)
{
    // Empty constructor.
}

VectorDrawable::VFullPath::VFullPath(
    /* [in] */ VFullPath* copy)
    : VPath(copy)
{
    mThemeAttrs = copy->mThemeAttrs;

    mStrokeColor = copy->mStrokeColor;
    mStrokeWidth = copy->mStrokeWidth;
    mStrokeAlpha = copy->mStrokeAlpha;
    mFillColor = copy->mFillColor;
    mFillRule = copy->mFillRule;
    mFillAlpha = copy->mFillAlpha;
    mTrimPathStart = copy->mTrimPathStart;
    mTrimPathEnd = copy->mTrimPathEnd;
    mTrimPathOffset = copy->mTrimPathOffset;

    mStrokeLineCap = copy->mStrokeLineCap;
    mStrokeLineJoin = copy->mStrokeLineJoin;
    mStrokeMiterlimit = copy->mStrokeMiterlimit;
}

PaintCap VectorDrawable::VFullPath::GetStrokeLineCap(
    /* [in] */ Int32 id,
    /* [in] */ PaintCap defValue)
{
    switch (id) {
        case VectorDrawable::LINECAP_BUTT:
            return PaintCap_BUTT;
        case VectorDrawable::LINECAP_ROUND:
            return PaintCap_ROUND;
        case VectorDrawable::LINECAP_SQUARE:
            return PaintCap_SQUARE;
        default:
            return defValue;
    }
}

PaintJoin VectorDrawable::VFullPath::GetStrokeLineJoin(
    /* [in] */ Int32 id,
    /* [in] */ PaintJoin defValue)
{
    switch (id) {
        case LINEJOIN_MITER:
            return PaintJoin_MITER;
        case LINEJOIN_ROUND:
            return PaintJoin_ROUND;
        case LINEJOIN_BEVEL:
            return PaintJoin_BEVEL;
        default:
            return defValue;
    }
}

Boolean VectorDrawable::VFullPath::CanApplyTheme()
{
    return mThemeAttrs != NULL;
}

void VectorDrawable::VFullPath::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IResourcesTheme* theme)
{
    AutoPtr<ITypedArray> a;
    Int32 size = 0;
    assert(0 && "TODO");
    // size = ARRAY_SIZE(R::styleable::VectorDrawablePath);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    // layout->Copy(R::styleable::VectorDrawablePath, size);
    ObtainAttributes(r, theme, attrs, layout, (ITypedArray**)&a);
    UpdateStateFromTypedArray(a);
    a->Recycle();
}

void VectorDrawable::VFullPath::UpdateStateFromTypedArray(
    /* [in] */ ITypedArray* a)
{
    // Account for any configuration changes.
    Int32 config = 0;
    a->GetChangingConfigurations(&config);
    mChangingConfigurations |= config;

    // Extract the theme attributes, if any.
    ((CTypedArray*)a)->ExtractThemeAttrs((ArrayOf<Int32>**)&mThemeAttrs);

    String pathName;
    assert(0 && "TODO");
    // a->GetString(R::styleable::VectorDrawablePath_name, &pathName);
    if (pathName != NULL) {
        mPathName = pathName;
    }

    String pathData;
    assert(0 && "TODO");
    // a->GetString(R::styleable::VectorDrawablePath_pathData, &pathData);
    if (pathData != NULL) {
        mNodes = PathParser::CreateNodesFromPathData(pathData);
    }

    assert(0 && "TODO");
    // a->GetColor(R::styleable::VectorDrawablePath_fillColor, mFillColor, &mFillColor);
    // a->GetFloat(R::styleable::VectorDrawablePath_fillAlpha, mFillAlpha, &mFillAlpha);
    // Int32 value = 0;
    // a->GetInt32(R::styleable::VectorDrawablePath_strokeLineCap, -1, &value);
    // mStrokeLineCap = GetStrokeLineCap(value, mStrokeLineCap);

    // a->GetInt32(R::styleable::VectorDrawablePath_strokeLineJoin, -1, &value);
    // mStrokeLineJoin = GetStrokeLineJoin(value, mStrokeLineJoin);
    // a->GetFloat(R::styleable::VectorDrawablePath_strokeMiterLimit, mStrokeMiterlimit, &mStrokeMiterlimit);
    // a->GetColor(R::styleable::VectorDrawablePath_strokeColor, mStrokeColor, &mStrokeColor);
    // a->GetFloat(R::styleable::VectorDrawablePath_strokeAlpha, mStrokeAlpha, &mStrokeAlpha);
    // a->GetFloat(R::styleable::VectorDrawablePath_strokeWidth, mStrokeWidth, &mStrokeWidth);
    // a->GetFloat(R::styleable::VectorDrawablePath_trimPathEnd, mTrimPathEnd, &mTrimPathEnd);
    // a->GetFloat(R::styleable::VectorDrawablePath_trimPathOffset, mTrimPathOffset, &mTrimPathOffset);
    // a->GetFloat(R::styleable::VectorDrawablePath_trimPathStart, mTrimPathStart, &mTrimPathStart);
}

void VectorDrawable::VFullPath::ApplyTheme(
    /* [in] */ IResourcesTheme* t)
{
    if (mThemeAttrs == NULL) {
        return;
    }

    AutoPtr<ITypedArray> a;
    assert(0 && "TODO");
    // Int32 size = ARRAY_SIZE(R::styleable::VectorDrawablePath);
    // AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    // layout->Copy(R::styleable::VectorDrawablePath, size);
    // t->ResolveAttributes(mThemeAttrs, layout, (ITypedArray**)a);
    UpdateStateFromTypedArray(a);
    a->Recycle();
}

Int32 VectorDrawable::VFullPath::GetStrokeColor()
{
    return mStrokeColor;
}

void VectorDrawable::VFullPath::SetStrokeColor(
    /* [in] */ Int32 strokeColor)
{
    mStrokeColor = strokeColor;
}

Float VectorDrawable::VFullPath::GetStrokeWidth()
{
    return mStrokeWidth;
}

void VectorDrawable::VFullPath::SetStrokeWidth(
    /* [in] */ Float strokeWidth)
{
    mStrokeWidth = strokeWidth;
}

Float VectorDrawable::VFullPath::GetStrokeAlpha()
{
    return mStrokeAlpha;
}

void VectorDrawable::VFullPath::SetStrokeAlpha(
    /* [in] */ Float strokeAlpha)
{
    mStrokeAlpha = strokeAlpha;
}

Int32 VectorDrawable::VFullPath::GetFillColor()
{
    return mFillColor;
}

void VectorDrawable::VFullPath::SetFillColor(
    /* [in] */ Int32 fillColor)
{
    mFillColor = fillColor;
}

Float VectorDrawable::VFullPath::GetFillAlpha()
{
    return mFillAlpha;
}

void VectorDrawable::VFullPath::SetFillAlpha(
    /* [in] */ Float fillAlpha)
{
    mFillAlpha = fillAlpha;
}

Float VectorDrawable::VFullPath::GetTrimPathStart()
{
    return mTrimPathStart;
}

void VectorDrawable::VFullPath::SetTrimPathStart(
    /* [in] */ Float trimPathStart)
{
    mTrimPathStart = trimPathStart;
}

Float VectorDrawable::VFullPath::GetTrimPathEnd()
{
    return mTrimPathEnd;
}

void VectorDrawable::VFullPath::SetTrimPathEnd(
    /* [in] */ Float trimPathEnd)
{
    mTrimPathEnd = trimPathEnd;
}

Float VectorDrawable::VFullPath::GetTrimPathOffset()
{
    return mTrimPathOffset;
}

void VectorDrawable::VFullPath::SetTrimPathOffset(
    /* [in] */ Float trimPathOffset)
{
    mTrimPathOffset = trimPathOffset;
}

////////////////////////////////// VectorDrawable ///////////////
CAR_INTERFACE_IMPL(VectorDrawable, Drawable, IVectorDrawable);
VectorDrawable::VectorDrawable()
    : mMutated(FALSE)
    , mAllowCaching(TRUE)
{
    mVectorState = new VectorDrawableState();
}

VectorDrawable::VectorDrawable(
    /* [in] */ VectorDrawableState* state,
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme)
    : mMutated(FALSE)
    , mAllowCaching(TRUE)
{
    constructor(state, res, theme);
}

ECode VectorDrawable::constructor(
    /* [in] */ VectorDrawableState* state,
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme)
{
    Boolean can = FALSE;
    if (theme != NULL && (state->CanApplyTheme(&can), can)) {
        // If we need to apply a theme, implicitly mutate.
        mVectorState = new VectorDrawableState(state);
        ApplyTheme(theme);
    } else {
        mVectorState = state;
    }

    mTintFilter = UpdateTintFilter(mTintFilter, state->mTint, state->mTintMode);
}

ECode VectorDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> dr;
    if (!mMutated && (Drawable::Mutate((IDrawable**)&dr), dr.Get()) == this) {
        mVectorState = new VectorDrawableState(mVectorState);
        mMutated = TRUE;
    }
    *drawable = THIS_PROBE(IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

AutoPtr<IInterface> VectorDrawable::GetTargetByName(
    /* [in] */ const String& name)
{
    AutoPtr<IInterface> obj;
    AutoPtr<ICharSequence> cs;
    CString::New(name, (ICharSequence**)&cs);
    IMap::Probe(mVectorState->mVPathRenderer->mVGTargetsMap)->Get(cs, (IInterface**)&obj);
    return obj;
}

ECode VectorDrawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    GetChangingConfigurations(&mVectorState->mChangingConfigurations);
    *state = mVectorState;
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode VectorDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IRect> bounds;
    GetBounds((IRect**)&bounds);
    Int32 w = 0, h = 0;
    bounds->GetWidth(&w);
    bounds->GetHeight(&h);
    if (w == 0 || h == 0) {
        // too small to draw
        return NOERROR;
    }

    Int32 saveCount = 0;
    canvas->Save(&saveCount);
    Boolean needMirroring = NeedMirroring();

    Int32 left = 0, top = 0;
    bounds->GetLeft(&left);
    bounds->GetTop(&top);
    canvas->Translate(left, top);
    if (needMirroring) {
        canvas->Translate(w, 0);
        canvas->Scale(-1.0f, 1.0f);
    }

    // Color filters always override tint filters.
    AutoPtr<IColorFilter> colorFilter = mColorFilter == NULL ? IColorFilter::Probe(mTintFilter) : mColorFilter.Get();

    if (!mAllowCaching) {
        // AnimatedVectorDrawable
        if (!mVectorState->HasTranslucentRoot()) {
            mVectorState->mVPathRenderer->Draw(canvas, w, h, colorFilter);
        } else {
            mVectorState->CreateCachedBitmapIfNeeded(bounds);
            mVectorState->UpdateCachedBitmap(bounds);
            mVectorState->DrawCachedBitmapWithRootAlpha(canvas, colorFilter);
        }
    } else {
        // Static Vector Drawable case.
        mVectorState->CreateCachedBitmapIfNeeded(bounds);
        if (!mVectorState->CanReuseCache()) {
            mVectorState->UpdateCachedBitmap(bounds);
            mVectorState->UpdateCacheStates();
        }
        mVectorState->DrawCachedBitmapWithRootAlpha(canvas, colorFilter);
    }

    return canvas->RestoreToCount(saveCount);
}

ECode VectorDrawable::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = mVectorState->mVPathRenderer->GetRootAlpha();
    return NOERROR;
}

ECode VectorDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (mVectorState->mVPathRenderer->GetRootAlpha() != alpha) {
        mVectorState->mVPathRenderer->SetRootAlpha(alpha);
        InvalidateSelf();
    }
    return NOERROR;
}

ECode VectorDrawable::SetColorFilter(
    /* [in] */ IColorFilter* colorFilter)
{
    mColorFilter = colorFilter;
    return InvalidateSelf();
}

ECode VectorDrawable::SetTintList(
    /* [in] */ IColorStateList* tint)
{
    AutoPtr<VectorDrawableState> state = mVectorState;
    if (state->mTint.Get() != tint) {
        state->mTint = tint;
        mTintFilter = UpdateTintFilter(mTintFilter, tint, state->mTintMode);
        InvalidateSelf();
    }
    return NOERROR;
}

ECode VectorDrawable::SetTintMode(
    /* [in] */ PorterDuffMode tintMode)
{
    AutoPtr<VectorDrawableState> state = mVectorState;
    if (state->mTintMode != tintMode) {
        state->mTintMode = tintMode;
        mTintFilter = UpdateTintFilter(mTintFilter, state->mTint, tintMode);
        InvalidateSelf();
    }
    return NOERROR;
}

ECode VectorDrawable::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    Boolean tmp = FALSE;
    *isStateful = (Drawable::IsStateful(isStateful), *isStateful) || (mVectorState != NULL && mVectorState->mTint != NULL
            && (mVectorState->mTint->IsStateful(&tmp), tmp));
    return NOERROR;
}

Boolean VectorDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    AutoPtr<VectorDrawableState> state = mVectorState;
    if (state->mTint.Get() != NULL && state->mTintMode != -1) {
        mTintFilter = UpdateTintFilter(mTintFilter, state->mTint, state->mTintMode);
        InvalidateSelf();
        return TRUE;
    }
    return FALSE;
}

ECode VectorDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    return IPixelFormat::TRANSLUCENT;
}

ECode VectorDrawable::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = (Int32) mVectorState->mVPathRenderer->mBaseWidth;
    return NOERROR;
}

ECode VectorDrawable::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = (Int32) mVectorState->mVPathRenderer->mBaseHeight;
    return NOERROR;
}

ECode VectorDrawable::CanApplyTheme(
    /* [out] */ Boolean* can)
{
    VALIDATE_NOT_NULL(can);
    Boolean tmp = FALSE;
    *can = (Drawable::CanApplyTheme(can), *can) || mVectorState.Get() != NULL && (mVectorState->CanApplyTheme(&tmp), tmp);
    return NOERROR;
}

ECode VectorDrawable::ApplyTheme(
    /* [out] */ IResourcesTheme* t)
{
    Drawable::ApplyTheme(t);

    AutoPtr<VectorDrawableState> state = mVectorState;
    if (state.Get() != NULL && state->mThemeAttrs.Get() != NULL) {
        AutoPtr<ITypedArray> a;
        assert(0 && "TODO");
        // Int32 size = ARRAY_SIZE(R::styleable::VectorDrawable);
        // AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
        // layout->Copy(R::styleable::VectorDrawable, size);
        // t->ResolveAttributes(state->mThemeAttrs, layout, (ITypedArray**)&a);

        // try {
        state->mCacheDirty = TRUE;
        if (FAILED(UpdateStateFromTypedArray(a))) {
            a->Recycle();
            return E_RUNTIME_EXCEPTION;
        }

        // } catch (XmlPullParserException e) {
        //     throw new RuntimeException(e);
        // } finally {
        a->Recycle();
        // }

        mTintFilter = UpdateTintFilter(mTintFilter, state->mTint, state->mTintMode);
    }

    AutoPtr<VPathRenderer> path = state->mVPathRenderer;
    if (path != NULL && path->CanApplyTheme()) {
        path->ApplyTheme(t);
    }
    return NOERROR;
}

ECode VectorDrawable::GetPixelSize(
    /* [out] */ Float* size)
{
    VALIDATE_NOT_NULL(size);
    if (mVectorState.Get() == NULL && mVectorState->mVPathRenderer.Get() == NULL ||
            mVectorState->mVPathRenderer->mBaseWidth == 0 ||
            mVectorState->mVPathRenderer->mBaseHeight == 0 ||
            mVectorState->mVPathRenderer->mViewportHeight == 0 ||
            mVectorState->mVPathRenderer->mViewportWidth == 0)
    {
        *size = 1; // fall back to 1:1 pixel mapping.
        return NOERROR;
    }
    Float intrinsicWidth = mVectorState->mVPathRenderer->mBaseWidth;
    Float intrinsicHeight = mVectorState->mVPathRenderer->mBaseHeight;
    Float viewportWidth = mVectorState->mVPathRenderer->mViewportWidth;
    Float viewportHeight = mVectorState->mVPathRenderer->mViewportHeight;
    Float scaleX = viewportWidth / intrinsicWidth;
    Float scaleY = viewportHeight / intrinsicHeight;
    *size = Elastos::Core::Math::Min(scaleX, scaleY);
    return NOERROR;
}

AutoPtr<IVectorDrawable> VectorDrawable::Create(
    /* [in] */ IResources* resources,
    /* [in] */ Int32 rid)
{
    // try {
    AutoPtr<IXmlPullParser> parser;
    if (FAILED(resources->GetXml(rid, (IXmlResourceParser**)&parser))) {
        return NULL;
    }
    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
    Int32 type = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::START_TAG &&
            type != IXmlPullParser::END_DOCUMENT) {
        // Empty loop
    }
    if (type != IXmlPullParser::START_TAG) {
        // throw new XmlPullParserException("No start tag found");
        assert(0);
    }

    AutoPtr<IVectorDrawable> drawable;
    CVectorDrawable::New((IVectorDrawable**)&drawable);
    if(FAILED(IDrawable::Probe(drawable)->Inflate(resources, parser, attrs))) {
        return NULL;
    }

    return drawable;
    // // } catch (XmlPullParserException e) {
    // //     Log.e(LOGTAG, "parser error", e);
    // // } catch (IOException e) {
    // //     Log.e(LOGTAG, "parser error", e);
    // // }
    // return NULL;
}

Int32 VectorDrawable::ApplyAlpha(
    /* [in] */ Int32 color,
    /* [in] */ Float alpha)
{
    Int32 alphaBytes = 0;
    AutoPtr<IColor> colorHelper;
    CColor::AcquireSingleton((IColor**)&colorHelper);
    colorHelper->Alpha(color, &alphaBytes);
    color &= 0x00FFFFFF;
    color |= ((Int32) (alphaBytes * alpha)) << 24;
    return color;
}

ECode VectorDrawable::Inflate(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IResourcesTheme* theme) /*throws XmlPullParserException, IOException*/
{
    AutoPtr<VectorDrawableState> state = mVectorState;
    AutoPtr<VPathRenderer> pathRenderer = new VPathRenderer();
    state->mVPathRenderer = pathRenderer;

    AutoPtr<ITypedArray> a;
    Int32 size = 0;
    assert(0 && "TODO");
    // size = ARRAY_SIZE(R::styleable::VectorDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    // layout->Copy(R::styleable::VectorDrawable, size);
    ECode ec = ObtainAttributes(res, theme, attrs, layout, (ITypedArray**)&a);
    if (FAILED(ec)) {
        return ec;
    }
    ec = UpdateStateFromTypedArray(a);
    if (FAILED(ec)) {
        a->Recycle();
        return ec;
    }
    a->Recycle();

    state->mCacheDirty = TRUE;
    FAIL_RETURN(InflateInternal(res, parser, attrs, theme));

    mTintFilter = UpdateTintFilter(mTintFilter, state->mTint, state->mTintMode);
    return NOERROR;
}

ECode VectorDrawable::UpdateStateFromTypedArray(
    /* [in] */ ITypedArray* a) /*throws XmlPullParserException*/
{
    AutoPtr<VectorDrawableState> state = mVectorState;
    AutoPtr<VPathRenderer> pathRenderer = state->mVPathRenderer;

    // Account for any configuration changes.
    Int32 config = 0;
    a->GetChangingConfigurations(&config);
    state->mChangingConfigurations |= config;

    // Extract the theme attributes, if any.
    ((CTypedArray*)a)->ExtractThemeAttrs((ArrayOf<Int32>**)&state->mThemeAttrs);

    Int32 tintMode = 0;
    assert(0 && "TODO");
    // a->GetInt32(R::styleable::VectorDrawable_tintMode, -1, &tintMode);
    if (tintMode != -1) {
        FAIL_RETURN(Drawable::ParseTintMode(tintMode, PorterDuffMode_SRC_IN, &state->mTintMode));
    }

    AutoPtr<IColorStateList> tint;
    assert(0 && "TODO");
    // a->GetColorStateList(R::styleable::VectorDrawable_tint, (IColorStateList**)&tint);
    if (tint != NULL) {
        state->mTint = tint;
    }

    assert(0 && "TODO");
    // a->GetBoolean(R::styleable::VectorDrawable_autoMirrored, state->mAutoMirrored, &state->mAutoMirrored);
    // a->GetFloat(R::styleable::VectorDrawable_viewportWidth, pathRenderer->mViewportWidth, &pathRenderer->mViewportWidth);
    // a->GetFloat(R::styleable::VectorDrawable_viewportHeight, pathRenderer->mViewportHeight, &pathRenderer->mViewportHeight);

    if (pathRenderer->mViewportWidth <= 0) {
        // throw new XmlPullParserException(a.getPositionDescription() +
        //         "<vector> tag requires viewportWidth > 0");
        return E_XML_PULL_PARSER_EXCEPTION;
    } else if (pathRenderer->mViewportHeight <= 0) {
        // throw new XmlPullParserException(a.getPositionDescription() +
        //         "<vector> tag requires viewportHeight > 0");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    assert(0 && "TODO");
    // a->GetDimension(R::styleable::VectorDrawable_width, pathRenderer->mBaseWidth, &pathRenderer->mBaseWidth);
    // a->GetDimension(R::styleable::VectorDrawable_height, pathRenderer->mBaseHeight, &pathRenderer->mBaseHeight);

    if (pathRenderer->mBaseWidth <= 0) {
        // throw new XmlPullParserException(a.getPositionDescription() +
        //         "<vector> tag requires width > 0");
        return E_XML_PULL_PARSER_EXCEPTION;
    } else if (pathRenderer->mBaseHeight <= 0) {
        // throw new XmlPullParserException(a.getPositionDescription() +
        //         "<vector> tag requires height > 0");
        return E_XML_PULL_PARSER_EXCEPTION;
    }

    Float alphaInFloat = 0;
    assert(0 && "TODO");
    // a->GetFloat(R::styleable::VectorDrawable_alpha, pathRenderer->GetAlpha(), &alphaInFloat);
    pathRenderer->SetAlpha(alphaInFloat);

    String name;
    assert(0 && "TODO");
    // a->GetString(R::styleable::VectorDrawable_name, &name);
    if (name != NULL) {
        pathRenderer->mRootName = name;
        AutoPtr<ICharSequence> cs;
        CString::New(name, (ICharSequence**)&cs);
        IMap::Probe(pathRenderer->mVGTargetsMap)->Put(cs, (IObject*)pathRenderer);
    }
    return NOERROR;
}

ECode VectorDrawable::InflateInternal(
    /* [in] */ IResources* res,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IResourcesTheme* theme) /*throws XmlPullParserException, IOException*/
{
    AutoPtr<VectorDrawableState> state = mVectorState;
    AutoPtr<VPathRenderer> pathRenderer = state->mVPathRenderer;
    Boolean noPathTag = TRUE;

    // Use a stack to help to build the group tree.
    // The top of the stack is always the current group.
    AutoPtr<IStack> groupStack;
    CStack::New((IStack**)&groupStack);
    groupStack->Push((IVGroup*)pathRenderer->mRootGroup);

    Int32 eventType = 0;
    parser->GetEventType(&eventType);
    while (eventType != IXmlPullParser::END_DOCUMENT) {
        if (eventType == IXmlPullParser::START_TAG) {
            String tagName;
            parser->GetName(&tagName);
            AutoPtr<IInterface> currentGroupObj;
            groupStack->Peek((IInterface**)&currentGroupObj);
            AutoPtr<VGroup> currentGroup = (VGroup*)IVGroup::Probe(currentGroupObj);

            if (SHAPE_PATH.Equals(tagName)) {
                AutoPtr<VFullPath> path = new VFullPath();
                path->Inflate(res, attrs, theme);
                currentGroup->mChildren->Add((IVFullPath*)path);
                if (path->GetPathName() != NULL) {
                    AutoPtr<ICharSequence> cs;
                    CString::New(path->GetPathName(), (ICharSequence**)&cs);
                    IMap::Probe(pathRenderer->mVGTargetsMap)->Put(cs, (IVFullPath*)path);
                }
                noPathTag = FALSE;
                state->mChangingConfigurations |= path->mChangingConfigurations;
            } else if (SHAPE_CLIP_PATH.Equals(tagName)) {
                AutoPtr<VClipPath> path = new VClipPath();
                path->Inflate(res, attrs, theme);
                currentGroup->mChildren->Add((IVClipPath*)path);
                if (path->GetPathName() != NULL) {
                    AutoPtr<ICharSequence> cs;
                    CString::New(path->GetPathName(), (ICharSequence**)&cs);
                    IMap::Probe(pathRenderer->mVGTargetsMap)->Put(cs, (IVClipPath*)path);
                }
                state->mChangingConfigurations |= path->mChangingConfigurations;
            } else if (SHAPE_GROUP.Equals(tagName)) {
                AutoPtr<VGroup> newChildGroup = new VGroup();
                newChildGroup->Inflate(res, attrs, theme);
                currentGroup->mChildren->Add((IVGroup*)newChildGroup);
                groupStack->Push((IVGroup*)newChildGroup);
                if (newChildGroup->GetGroupName() != NULL) {
                    AutoPtr<ICharSequence> cs;
                    CString::New(newChildGroup->GetGroupName(), (ICharSequence**)&cs);
                    IMap::Probe(pathRenderer->mVGTargetsMap)->Put(cs, (IVGroup*)newChildGroup);
                }
                state->mChangingConfigurations |= newChildGroup->mChangingConfigurations;
            }
        } else if (eventType == IXmlPullParser::END_TAG) {
            String tagName;
            parser->GetName(&tagName);
            if (SHAPE_GROUP.Equals(tagName)) {
                AutoPtr<IInterface> obj;
                groupStack->Pop((IInterface**)&obj);
            }
        }
        parser->Next(&eventType);
    }

    // Print the tree out for debug.
    if (DBG_VECTOR_DRAWABLE) {
        PrintGroupTree(pathRenderer->mRootGroup, 0);
    }

    if (noPathTag) {
        AutoPtr<StringBuffer> tag = new StringBuffer();

        if (tag->GetLength() > 0) {
            tag->Append(String(" or "));
        }
        tag->Append(SHAPE_PATH);

        // throw new XmlPullParserException("no " + tag + " defined");
        return E_XML_PULL_PARSER_EXCEPTION;
    }
    return NOERROR;
}

void VectorDrawable::PrintGroupTree(
    /* [in] */ VGroup* currentGroup,
    /* [in] */ Int32 level)
{
    String indent = String("");
    for (Int32 i = 0; i < level; i++) {
        indent += String("    ");
    }
    // Print the current node
    Logger::V(LOGTAG, indent + String("current group is :") + currentGroup->GetGroupName()
            + String(" rotation is ") + StringUtils::ToString(currentGroup->mRotate));
    String content;
    IObject::Probe(currentGroup->GetLocalMatrix())->ToString(&content);
    Logger::V(LOGTAG, indent + String("matrix is :") + content);
    // Then print all the children groups
    Int32 size = 0;
    currentGroup->mChildren->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> child;
        currentGroup->mChildren->Get(i, (IInterface**)&child);
        if (IVGroup::Probe(child)) {
            PrintGroupTree((VGroup*) IVGroup::Probe(child), level + 1);
        }
    }
}

ECode VectorDrawable::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = (Drawable::GetChangingConfigurations(config), *config) | mVectorState->mChangingConfigurations;
    return NOERROR;
}

void VectorDrawable::SetAllowCaching(
    /* [in] */ Boolean allowCaching)
{
    mAllowCaching = allowCaching;
}

Boolean VectorDrawable::NeedMirroring()
{
    Boolean bv = FALSE;
    Int32 dir = 0;
    return (IsAutoMirrored(&bv), bv) && (GetLayoutDirection(&dir), dir) == ILayoutDirection::RTL;
}

ECode VectorDrawable::SetAutoMirrored(
    /* [in] */ Boolean mirrored)
{
    if (mVectorState->mAutoMirrored != mirrored) {
        mVectorState->mAutoMirrored = mirrored;
        InvalidateSelf();
    }
    return NOERROR;
}

ECode VectorDrawable::IsAutoMirrored(
    /* [out] */ Boolean* mirrored)
{
    VALIDATE_NOT_NULL(mirrored);
    *mirrored = mVectorState->mAutoMirrored;
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
