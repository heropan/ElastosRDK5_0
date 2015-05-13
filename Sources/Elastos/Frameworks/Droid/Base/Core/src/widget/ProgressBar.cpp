
#include "widget/ProgressBar.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/CLinearInterpolator.h"
#include "view/animation/CTransformation.h"
#include "view/animation/CAlphaAnimation.h"
#include "widget/CProgressBarSavedState.h"
#include "widget/CProgressBar.h"
#include "graphics/drawable/CLayerDrawable.h"
#include "graphics/drawable/CStateListDrawable.h"
#include "graphics/drawable/CBitmapDrawable.h"
#include "graphics/drawable/CShapeDrawable.h"
#include "graphics/drawable/CClipDrawable.h"
#include "graphics/drawable/CAnimationDrawable.h"
#include "graphics/drawable/shapes/CRoundRectShape.h"
#include "CProgressBarSavedState.h"
#include "graphics/CBitmapShader.h"
#include "view/accessibility/CAccessibilityManager.h"
#include <elastos/Math.h>
#include <elastos/Thread.h>

using Elastos::Droid::R;
using Elastos::Core::Threading::Thread;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Graphics::Drawable::ILayerDrawable;
using Elastos::Droid::Graphics::Drawable::CLayerDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_ILayerDrawable;
using Elastos::Droid::Graphics::Drawable::IStateListDrawable;
using Elastos::Droid::Graphics::Drawable::CStateListDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_IStateListDrawable;
using Elastos::Droid::Graphics::Drawable::IBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::CBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_IBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::IShapeDrawable;
using Elastos::Droid::Graphics::Drawable::CShapeDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_IShapeDrawable;
using Elastos::Droid::Graphics::Drawable::IClipDrawable;
using Elastos::Droid::Graphics::Drawable::CClipDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_IClipDrawable;
using Elastos::Droid::Graphics::Drawable::IAnimationDrawable;
using Elastos::Droid::Graphics::Drawable::CAnimationDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_IAnimationDrawable;
using Elastos::Droid::Graphics::Drawable::Shapes::IRoundRectShape;
using Elastos::Droid::Graphics::Drawable::Shapes::CRoundRectShape;
using Elastos::Droid::Graphics::Drawable::Shapes::EIID_IRoundRectShape;
using Elastos::Droid::Graphics::Drawable::IClipDrawable;
using Elastos::Droid::Graphics::Drawable::IAnimatable;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::Animation::IAlphaAnimation;
using Elastos::Droid::View::Animation::CAlphaAnimation;
using Elastos::Droid::View::Animation::ILinearInterpolator;
using Elastos::Droid::View::Animation::CLinearInterpolator;
using Elastos::Droid::View::Animation::CTransformation;
using Elastos::Droid::View::Animation::ITransformation;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::Graphics::IBitmapShader;
using Elastos::Droid::Graphics::CBitmapShader;
using Elastos::Droid::Graphics::ShaderTileMode;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Utility::EIID_IPoolable;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;

namespace Elastos {
namespace Droid {
namespace Widget {

AutoPtr<IPool> ProgressBar::RefreshData::sPool = Pools::AcquireSynchronizedPool(
        Pools::AcquireFinitePool(new RefreshDataPoolableManager(), POOL_MAX));

CAR_INTERFACE_IMPL(ProgressBar::RefreshData, IPoolable)

ProgressBar::RefreshProgressRunnable::RefreshProgressRunnable(
    /* [in] */ IWeakReference* host) : mWeakHost(host)
{
}

ECode ProgressBar::RefreshProgressRunnable::Run()
{
    AutoPtr<IInterface> obj;
    mWeakHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    IProgressBar* pb = IProgressBar::Probe(obj);

    if (pb != NULL) {
        CProgressBar* host = (CProgressBar*)pb;
        Mutex::Autolock lock(host->mLock);
        Int32 count = host->mRefreshData.GetSize();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<RefreshData> rd = host->mRefreshData[i];
            host->DoRefreshProgress(rd->mId, rd->mProgress, rd->mFromUser, TRUE);
            rd->Recycle();
        }
        host->mRefreshData.Clear();
        host->mRefreshIsPosted = FALSE;
    }

    return NOERROR;
}

ProgressBar::AccessibilityEventSender::AccessibilityEventSender(
    /* [in] */ IWeakReference* host) : mWeakHost(host)
{
}

ECode ProgressBar::AccessibilityEventSender::Run()
{
    AutoPtr<IInterface> obj;
    mWeakHost->Resolve(EIID_IInterface, (IInterface**)&obj);
    IProgressBar* pb = IProgressBar::Probe(obj);

    if (pb != NULL) {
        CProgressBar* host = (CProgressBar*)pb;
        host->SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    }
    return NOERROR;
}

ProgressBar::RefreshData::RefreshData()
    : mId(0), mProgress(0)
    , mFromUser(FALSE), mIsPooled(FALSE)
{
}

ECode ProgressBar::RefreshData::Recycle()
{
    sPool->ReleaseElement(this);
    return NOERROR;
}

ECode ProgressBar::RefreshData::SetNextPoolable(
    /* [in] */ IPoolable* element)
{
    mNext = (RefreshData*)element;

    return NOERROR;
}

/**
 * @hide
 */
ECode ProgressBar::RefreshData::GetNextPoolable(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);

    *element = mNext;
    INTERFACE_ADDREF(*element);

     return NOERROR;
}

ECode ProgressBar::RefreshData::IsPooled(
    /* [out] */ Boolean* isPooled)
{
    VALIDATE_NOT_NULL(isPooled);

    *isPooled = mIsPooled;

    return NOERROR;
}

ECode ProgressBar::RefreshData::SetPooled(
    /* [in] */ Boolean isPooled)
{
    mIsPooled = isPooled;
    return NOERROR;
}
AutoPtr<ProgressBar::RefreshData> ProgressBar::RefreshData::Obtain(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    AutoPtr<RefreshData> rd;
    sPool->Acquire((IPoolable**)&rd);
    rd->mId = id;
    rd->mProgress = progress;
    rd->mFromUser = fromUser;
    return rd;
}

CAR_INTERFACE_IMPL(ProgressBar::RefreshData::RefreshDataPoolableManager, IPoolableManager)

ECode ProgressBar::RefreshData::RefreshDataPoolableManager::NewInstance(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);
    *element = new RefreshData();
    if (*element == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    INTERFACE_ADDREF(*element);

    return NOERROR;
}

ECode ProgressBar::RefreshData::RefreshDataPoolableManager::OnAcquired(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

ECode ProgressBar::RefreshData::RefreshDataPoolableManager::OnReleased(
    /* [in] */ IPoolable* element)
{
    return NOERROR;
}

ProgressBar::ProgressBar()
    : mMinWidth(0), mMaxWidth(0)
    , mMinHeight(0), mMaxHeight(0)
    , mProgress(0), mSecondaryProgress(0)
    , mMax(0), mBehavior(0)
    , mDuration(0), mIndeterminate(FALSE)
    , mOnlyIndeterminate(FALSE), mHasAnimation(FALSE)
    , mNoInvalidate(FALSE), mUiThreadId(0)
    , mShouldStartAnimationDrawable(FALSE)
    , mInDrawing(FALSE), mAttached(FALSE)
    , mRefreshIsPosted(FALSE)
{}

ProgressBar::ProgressBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 styleRes)
    : View(context, attrs, defStyle)
    , mMinWidth(0), mMaxWidth(0)
    , mMinHeight(0), mMaxHeight(0)
    , mProgress(0), mSecondaryProgress(0)
    , mMax(0), mBehavior(0)
    , mDuration(0), mIndeterminate(FALSE)
    , mOnlyIndeterminate(FALSE), mHasAnimation(FALSE)
    , mNoInvalidate(FALSE), mUiThreadId(0)
    , mShouldStartAnimationDrawable(FALSE)
    , mInDrawing(FALSE), mAttached(FALSE)
    , mRefreshIsPosted(FALSE)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle, styleRes));
}

ECode ProgressBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 styleRes)
{
    FAIL_RETURN(View::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle, styleRes);
}

ECode ProgressBar::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 styleRes)
{
    Thread::GetCurrentThread()->GetId(&mUiThreadId);
    InitProgressBar();

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ProgressBar),
            ARRAY_SIZE(R::styleable::ProgressBar));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, attrIds, defStyle, styleRes, (ITypedArray**)&a);

    mNoInvalidate = TRUE;
    AutoPtr<IDrawable> drawable;
    a->GetDrawable(R::styleable::ProgressBar_progressDrawable, (IDrawable**)&drawable);
    if (drawable != NULL) {
        drawable = Tileify(drawable, FALSE);
        // Calling this method can set mMaxHeight, make sure the corresponding
        // XML attribute for mMaxHeight is read after calling this method
        SetProgressDrawable(drawable);
    }

    a->GetInt32(R::styleable::ProgressBar_indeterminateDuration, mDuration, &mDuration);

    a->GetDimensionPixelSize(R::styleable::ProgressBar_minWidth, mMinWidth, &mMinWidth);
    a->GetDimensionPixelSize(R::styleable::ProgressBar_maxWidth, mMaxWidth, &mMaxWidth);
    a->GetDimensionPixelSize(R::styleable::ProgressBar_minHeight, mMinHeight, &mMinHeight);
    a->GetDimensionPixelSize(R::styleable::ProgressBar_maxHeight, mMaxHeight, &mMaxHeight);

    a->GetInt32(R::styleable::ProgressBar_indeterminateBehavior, mBehavior, &mBehavior);

    Int32 resID;
    a->GetResourceId(
        R::styleable::ProgressBar_interpolator,
        R::anim::linear_interpolator, &resID); // default to linear interpolator
    if (resID > 0) {
        SetInterpolator(context, resID);
    }
    Int32 result;
    a->GetInt32(R::styleable::ProgressBar_max, mMax, &result);
    SetMax(result);
    a->GetInt32(R::styleable::ProgressBar_progress, mProgress, &result);
    SetProgress(result);
    a->GetInt32(R::styleable::ProgressBar_secondaryProgress, mSecondaryProgress, &result);
    SetSecondaryProgress(result);
    drawable = NULL;
    a->GetDrawable(R::styleable::ProgressBar_indeterminateDrawable, (IDrawable**)&drawable);
    if (drawable != NULL) {
        drawable = TileifyIndeterminate(drawable);
        SetIndeterminateDrawable(drawable);
    }

    a->GetBoolean(
        R::styleable::ProgressBar_indeterminateOnly, mOnlyIndeterminate, &mOnlyIndeterminate);
    mNoInvalidate = FALSE;

    Boolean b;
    a->GetBoolean(R::styleable::ProgressBar_indeterminate, mIndeterminate, &b);
    SetIndeterminate(mOnlyIndeterminate || b);
    a->Recycle();
    return NOERROR;
}

/**
 * Converts a drawable to a tiled version of itself. It will recursively
 * traverse layer and state list drawables.
 */
AutoPtr<IDrawable> ProgressBar::Tileify(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Boolean clip)
{
    if (drawable->Probe(EIID_ILayerDrawable)) {
        AutoPtr<ILayerDrawable> background = (ILayerDrawable*) drawable->Probe(EIID_ILayerDrawable);
        Int32 N;
        background->GetNumberOfLayers(&N);
        AutoPtr<ArrayOf<IDrawable*> > outDrawables = ArrayOf<IDrawable*>::Alloc(N);

        for (Int32 i = 0; i < N; i++) {
            Int32 id;
            background->GetId(i, &id);
            AutoPtr<IDrawable> dTemp;
            background->GetDrawable(i, (IDrawable**)&dTemp);
            outDrawables->Set(i, Tileify(dTemp,
                    (id == R::id::progress || id == R::id::secondaryProgress)));
        }

        AutoPtr<ILayerDrawable> newBg;
        CLayerDrawable::New(outDrawables, (ILayerDrawable**)&newBg);

        for (Int32 i = 0; i < N; i++) {
            Int32 id;
            background->GetId(i, &id);
            newBg->SetId(i, id);
        }

        return newBg;

    } else if (drawable->Probe(EIID_IStateListDrawable)) {
        AutoPtr<IStateListDrawable> in =
                (IStateListDrawable*) drawable->Probe(EIID_IStateListDrawable);
        AutoPtr<IStateListDrawable> out;
        CStateListDrawable::New((IStateListDrawable**)&out);
        Int32 numStates;
        in->GetStateCount(&numStates);
        for (Int32 i = 0; i < numStates; i++) {
            AutoPtr<ArrayOf<Int32> > stateSet;
            AutoPtr<IDrawable> stateDrawable;
            in->GetStateSet(i, (ArrayOf<Int32>**)&stateSet);
            in->GetStateDrawable(i, (IDrawable**)&stateDrawable);
            AutoPtr<IDrawable> tileify = Tileify(stateDrawable, clip);
            out->AddState(*stateSet, tileify);
        }
        return out;

    } else if (drawable->Probe(EIID_IBitmapDrawable)) {
        AutoPtr<IBitmapDrawable> bitmapDrawable;
        bitmapDrawable = (IBitmapDrawable*)drawable->Probe(EIID_IBitmapDrawable);
        AutoPtr<IBitmap> tileBitmap;
        bitmapDrawable->GetBitmap((IBitmap**)&tileBitmap);
        if (mSampleTile == NULL) {
            mSampleTile = tileBitmap;
        }

        AutoPtr<IShapeDrawable> shapeDrawable;
        CShapeDrawable::New(GetDrawableShape(), (IShapeDrawable**)&shapeDrawable);
        AutoPtr<IBitmapShader> bitmapShader;
        CBitmapShader::New(tileBitmap,
                ShaderTileMode_REPEAT, ShaderTileMode_CLAMP, (IBitmapShader**)&bitmapShader);
        AutoPtr<IPaint> paint;
        shapeDrawable->GetPaint((IPaint**)&paint);
        paint->SetShader(bitmapShader);

        AutoPtr<IClipDrawable> clipDrawable;
        CClipDrawable::New(shapeDrawable, IGravity::LEFT,
                IClipDrawable::HORIZONTAL, (IClipDrawable**)&clipDrawable);
        if(clip)
        {
            return clipDrawable;
        }else{
            return shapeDrawable;
        }
    }

    return drawable;
}

AutoPtr<IShape> ProgressBar::GetDrawableShape()
{
    AutoPtr<ArrayOf<Float> > roundedCorners = ArrayOf<Float>::Alloc(8);
    for(Int32 i = 0; i < 8; i++)
    {
        (*roundedCorners)[i] = 5.0f;
    }
    AutoPtr<IRoundRectShape> result;
    CRoundRectShape::New(roundedCorners, NULL, NULL, (IRoundRectShape**)&result);
    return result;
}

/**
 * Convert a AnimationDrawable for use as a barberpole animation.
 * Each frame of the animation is wrapped in a ClipDrawable and
 * given a tiling BitmapShader.
 */
AutoPtr<IDrawable> ProgressBar::TileifyIndeterminate(
    /* [in] */ IDrawable* inDrawable)
{
    AutoPtr<IDrawable> drawable = inDrawable;
    if (drawable->Probe(EIID_IAnimationDrawable)) {
        AutoPtr<IAnimationDrawable> background =
                (IAnimationDrawable*)drawable->Probe(EIID_IAnimationDrawable);
        Int32 N = 0;
        background->GetNumberOfFrames(&N);
        AutoPtr<IAnimationDrawable> newBg;
        CAnimationDrawable::New((IAnimationDrawable**)&newBg);
        Boolean oneShot;
        background->IsOneShot(&oneShot);
        newBg->SetOneShot(oneShot);

        Boolean rst;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawable> temp;
            background->GetFrame(i, (IDrawable**)&temp);
            AutoPtr<IDrawable> frame = Tileify(temp, TRUE);
            frame->SetLevel(10000, &rst);
            Int32 duration;
            background->GetDuration(i, &duration);
            newBg->AddFrame(frame, duration);
        }
        newBg->SetLevel(10000, &rst);
        drawable = newBg;
    }
    return drawable;
}

/**
 * <p>
 * Initialize the progress bar's default values:
 * </p>
 * <ul>
 * <li>progress = 0</li>
 * <li>max = 100</li>
 * <li>animation duration = 4000 ms</li>
 * <li>indeterminate = FALSE</li>
 * <li>behavior = repeat</li>
 * </ul>
 */
void ProgressBar::InitProgressBar()
{
    mMax = 100;
    mProgress = 0;
    mSecondaryProgress = 0;
    mIndeterminate = FALSE;
    mOnlyIndeterminate = FALSE;
    mDuration = 4000;
    mBehavior = IAnimation::RESTART;
    mMinWidth = 24;
    mMaxWidth = 48;
    mMinHeight = 24;
    mMaxHeight = 48;
}

/**
 * <p>Indicate whether this progress bar is in indeterminate mode.</p>
 *
 * @return TRUE if the progress bar is in indeterminate mode
 */
//synchronized
//
Boolean ProgressBar::IsIndeterminate()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mIndeterminate;
}

/**
 * <p>Change the indeterminate mode for this progress bar. In indeterminate
 * mode, the progress is ignored and the progress bar shows an infinite
 * animation instead.</p>
 *
 * If this progress bar's style only supports indeterminate mode (such as the circular
 * progress bars), then this will be ignored.
 *
 * @param indeterminate TRUE to enable the indeterminate mode
 */
//synchronized
//
ECode ProgressBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    Mutex::Autolock lock(*GetSelfLock());

    if ((!mOnlyIndeterminate || !mIndeterminate) && indeterminate != mIndeterminate) {
        mIndeterminate = indeterminate;

        if (indeterminate) {
            // swap between indeterminate and regular backgrounds
            mCurrentDrawable = mIndeterminateDrawable;
            StartAnimation();
        }
        else {
            mCurrentDrawable = mProgressDrawable;
            StopAnimation();
        }
    }

    return NOERROR;
}

/**
 * <p>Get the drawable used to draw the progress bar in
 * indeterminate mode.</p>
 *
 * @return a {@link android.graphics.drawable.Drawable} instance
 *
 * @see #setIndeterminateDrawable(android.graphics.drawable.Drawable)
 * @see #setIndeterminate(Boolean)
 */
AutoPtr<IDrawable> ProgressBar::GetIndeterminateDrawable()
{
    return mIndeterminateDrawable;
}

/**
 * <p>Define the drawable used to draw the progress bar in
 * indeterminate mode.</p>
 *
 * @param d the new drawable
 *
 * @see #getIndeterminateDrawable()
 * @see #setIndeterminate(Boolean)
 */
ECode ProgressBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable* d)
{
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    mIndeterminateDrawable = d;
    if (mIndeterminateDrawable && CanResolveLayoutDirection()) {
        mIndeterminateDrawable->SetLayoutDirection(GetLayoutDirection());
    }
    if (mIndeterminate) {
        mCurrentDrawable = d;
        PostInvalidate();
    }
    return NOERROR;
}

/**
 * <p>Get the drawable used to draw the progress bar in
 * progress mode.</p>
 *
 * @return a {@link android.graphics.drawable.Drawable} instance
 *
 * @see #setProgressDrawable(android.graphics.drawable.Drawable)
 * @see #setIndeterminate(Boolean)
 */
AutoPtr<IDrawable> ProgressBar::GetProgressDrawable()
{
    return mProgressDrawable;
}

/**
 * <p>Define the drawable used to draw the progress bar in
 * progress mode.</p>
 *
 * @param d the new drawable
 *
 * @see #getProgressDrawable()
 * @see #setIndeterminate(Boolean)
 */
ECode ProgressBar::SetProgressDrawable(
    /* [in] */ IDrawable* d)
{
    Boolean needUpdate;
    if (mProgressDrawable != NULL && d != mProgressDrawable) {
        mProgressDrawable->SetCallback(NULL);
        needUpdate = TRUE;
    } else {
        needUpdate = FALSE;
    }
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));

        if (CanResolveLayoutDirection()) {
            d->SetLayoutDirection(GetLayoutDirection());
        }

        // Make sure the ProgressBar is always tall enough
        Int32 drawableHeight;
        d->GetMinimumHeight(&drawableHeight);
        if (mMaxHeight < drawableHeight) {
            mMaxHeight = drawableHeight;
            RequestLayout();
        }
    }
    mProgressDrawable = d;
    if (!mIndeterminate) {
        mCurrentDrawable = d;
        PostInvalidate();
    }

    if (needUpdate) {
        UpdateDrawableBounds(GetWidth(), GetHeight());
        UpdateDrawableState();
        DoRefreshProgress(R::id::progress, mProgress, FALSE, FALSE);
        DoRefreshProgress(R::id::secondaryProgress, mSecondaryProgress, FALSE, FALSE);
    }

    return NOERROR;
}

/**
 * @return The drawable currently used to draw the progress bar
 */
AutoPtr<IDrawable> ProgressBar::GetCurrentDrawable()
{
    return mCurrentDrawable;
}

Boolean ProgressBar::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return who == mProgressDrawable || who == mIndeterminateDrawable
        || View::VerifyDrawable(who);
}

ECode ProgressBar::JumpDrawablesToCurrentState()
{
    View::JumpDrawablesToCurrentState();
    if (mProgressDrawable != NULL) mProgressDrawable->JumpToCurrentState();
    if (mIndeterminateDrawable != NULL) mIndeterminateDrawable->JumpToCurrentState();
    return NOERROR;
}

/**
 * @hide
 */
ECode ProgressBar::OnResolveDrawables(
    /* [in] */ Int32 layoutDirection)
{
    AutoPtr<IDrawable > d = mCurrentDrawable;
    if (d != NULL) {
        d->SetLayoutDirection(layoutDirection);
    }
    if (mIndeterminateDrawable != NULL) {
        mIndeterminateDrawable->SetLayoutDirection(layoutDirection);
    }
    if (mProgressDrawable != NULL) {
        mProgressDrawable->SetLayoutDirection(layoutDirection);
    }
    return NOERROR;
}

ECode ProgressBar::PostInvalidate()
{
    if (!mNoInvalidate) {
        View::PostInvalidate();
    }

    return NOERROR;
}

//synchronized
//
void ProgressBar::DoRefreshProgress(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser,
    /* [in] */ Boolean callBackToApp)
{
    Mutex::Autolock lock(*GetSelfLock());

    Float scale = mMax > 0 ? (Float)progress / (Float)mMax : 0;
    AutoPtr<IDrawable> d = mCurrentDrawable;
    if (d != NULL) {
        AutoPtr<IDrawable> progressDrawable;
        AutoPtr<ILayerDrawable> layerDrawable = ILayerDrawable::Probe(d);
        if (layerDrawable) {
            layerDrawable->FindDrawableByLayerId(id, (IDrawable**)&progressDrawable);
            if (progressDrawable != NULL && CanResolveLayoutDirection()) {
                progressDrawable->SetLayoutDirection(GetLayoutDirection());
            }
        }
        Int32 level = (Int32)(scale * MAX_LEVEL);
        Boolean tmp;
        if (progressDrawable) {
            progressDrawable->SetLevel(level, &tmp);
        } else {
            d->SetLevel(level, &tmp);
        }
    }
    else {
        Invalidate();
    }
    if (callBackToApp && id == R::id::progress) {
        OnProgressRefresh(scale, fromUser);
    }
}

void ProgressBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
    Boolean enable = FALSE;
    AutoPtr<IAccessibilityManager> am;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&am);
    if (am) {
        am->IsEnabled(&enable);
    }

    if (enable) {
        ScheduleAccessibilityEventSender();
    }
}

void ProgressBar::RefreshProgress(
    /* [in] */ Int32 id,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    Mutex::Autolock lock(*GetSelfLock());

    Int64 cId;
    Thread::GetCurrentThread()->GetId(&cId);
    if (mUiThreadId == cId) {
        DoRefreshProgress(id, progress, fromUser, true);
    }
    else {
        if (mRefreshProgressRunnable == NULL) {
            AutoPtr<IWeakReferenceSource> wrs = THIS_PROBE(IWeakReferenceSource);
            assert(wrs);
            AutoPtr<IWeakReference> wr;
            wrs->GetWeakReference((IWeakReference**)&wr);
            mRefreshProgressRunnable = new RefreshProgressRunnable(wr);
        }
        AutoPtr<RefreshData> rd = RefreshData::Obtain(id, progress, fromUser);
        mRefreshData.PushBack(rd);
        if (mAttached && !mRefreshIsPosted) {
            Post(mRefreshProgressRunnable);
            mRefreshIsPosted = TRUE;
        }
    }
}

/**
 * <p>Set the current progress to the specified value. Does not do anything
 * if the progress bar is in indeterminate mode.</p>
 *
 * @param progress the new progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #getProgress()
 * @see #incrementProgressBy(Int32)
 */
ECode ProgressBar::SetProgress(
    /* [in] */ Int32 progress)
{
    return SetProgress(progress, FALSE);
}

//synchronized
//
ECode ProgressBar::SetProgress(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    Mutex::Autolock lock(*GetSelfLock());

    if (mIndeterminate) {
        return NOERROR;
    }

    if (progress < 0) {
        progress = 0;
    }

    if (progress > mMax) {
        progress = mMax;
    }

    if (progress != mProgress) {
        mProgress = progress;
        RefreshProgress(R::id::progress, mProgress, fromUser);
    }
    return NOERROR;
}

/**
 * <p>
 * Set the current secondary progress to the specified value. Does not do
 * anything if the progress bar is in indeterminate mode.
 * </p>
 *
 * @param secondaryProgress the new secondary progress, between 0 and {@link #getMax()}
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #getSecondaryProgress()
 * @see #incrementSecondaryProgressBy(Int32)
 */
//synchronized
ECode ProgressBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    Mutex::Autolock lock(*GetSelfLock());

    if (mIndeterminate) {
        return NOERROR;
    }

    if (secondaryProgress < 0) {
        secondaryProgress = 0;
    }

    if (secondaryProgress > mMax) {
        secondaryProgress = mMax;
    }

    if (secondaryProgress != mSecondaryProgress) {
        mSecondaryProgress = secondaryProgress;
        RefreshProgress(R::id::secondaryProgress, mSecondaryProgress, FALSE);
    }
    return NOERROR;
}


/**
 * <p>Get the progress bar's current level of progress. Return 0 when the
 * progress bar is in indeterminate mode.</p>
 *
 * @return the current progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #setProgress(Int32)
 * @see #setMax(Int32)
 * @see #getMax()
 */
//synchronized
//
Int32 ProgressBar::GetProgress()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mIndeterminate ? 0 : mProgress;
}

/**
 * <p>Get the progress bar's current level of secondary progress. Return 0 when the
 * progress bar is in indeterminate mode.</p>
 *
 * @return the current secondary progress, between 0 and {@link #getMax()}
 *
 * @see #setIndeterminate(Boolean)
 * @see #isIndeterminate()
 * @see #setSecondaryProgress(Int32)
 * @see #setMax(Int32)
 * @see #getMax()
 */
//synchronized
//
Int32 ProgressBar::GetSecondaryProgress()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mIndeterminate ? 0 : mSecondaryProgress;
}

/**
 * <p>Return the upper limit of this progress bar's range.</p>
 *
 * @return a positive integer
 *
 * @see #setMax(Int32)
 * @see #getProgress()
 * @see #getSecondaryProgress()
 */
//synchronized
//
Int32 ProgressBar::GetMax()
{
    Mutex::Autolock lock(*GetSelfLock());

    return mMax;
}

/**
 * <p>Set the range of the progress bar to 0...<tt>max</tt>.</p>
 *
 * @param max the upper range of this progress bar
 *
 * @see #getMax()
 * @see #setProgress(Int32)
 * @see #setSecondaryProgress(Int32)
 */
//synchronized
//
ECode ProgressBar::SetMax(
    /* [in] */ Int32 max)
{
    Mutex::Autolock lock(*GetSelfLock());

    if (max < 0) {
        max = 0;
    }
    if (max != mMax) {
        mMax = max;
        PostInvalidate();

        if (mProgress > max) {
            mProgress = max;

        }

        RefreshProgress(R::id::progress, mProgress, FALSE);
    }

    return NOERROR;
}

/**
 * <p>Increase the progress bar's progress by the specified amount.</p>
 *
 * @param diff the amount by which the progress must be increased
 *
 * @see #setProgress(Int32)
 */
//synchronized
//
ECode ProgressBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    return SetProgress(mProgress + diff, FALSE);
}

/**
 * <p>Increase the progress bar's secondary progress by the specified amount.</p>
 *
 * @param diff the amount by which the secondary progress must be increased
 *
 * @see #setSecondaryProgress(Int32)
 */
//synchronized
//
ECode ProgressBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    return SetSecondaryProgress(mSecondaryProgress + diff);
}

/**
 * <p>Start the indeterminate progress animation.</p>
 */
void ProgressBar::StartAnimation()
{
    if (GetVisibility() != IView::VISIBLE) {
        return;
    }

    if (IAnimatable::Probe(mIndeterminateDrawable.Get())) {
        mShouldStartAnimationDrawable = TRUE;
        mHasAnimation = FALSE;
    }
    else {
        mHasAnimation = TRUE;

        if (mInterpolator == NULL) {
            CLinearInterpolator::New((ILinearInterpolator**)&mInterpolator);
        }

        if (mTransformation == NULL) {
            CTransformation::New((ITransformation**)&mTransformation);
        } else {
            mTransformation->Clear();
        }

        if (mAnimation == NULL) {
            CAlphaAnimation::New(0.0f, 1.0f, (IAlphaAnimation**)&mAnimation);
        } else {
            mAnimation->Reset();
        }

        mAnimation->SetRepeatMode(mBehavior);
        mAnimation->SetRepeatCount(IAnimation::ANIMATION_INFINITE);
        mAnimation->SetDuration(mDuration);
        mAnimation->SetInterpolatorEx(mInterpolator);
        mAnimation->SetStartTime(IAnimation::START_ON_FIRST_FRAME);
    }

    PostInvalidate();
}

/**
 * <p>Stop the indeterminate progress animation.</p>
 */
void ProgressBar::StopAnimation()
{
    mHasAnimation = FALSE;
    if (IAnimatable::Probe(mIndeterminateDrawable.Get())) {
        IAnimatable::Probe(mIndeterminateDrawable.Get())->Stop();
        mShouldStartAnimationDrawable = FALSE;
    }

    PostInvalidate();
}

/**
 * Sets the acceleration curve for the indeterminate animation.
 * The interpolator is loaded as a resource from the specified context.
 *
 * @param context The application environment
 * @param resID The resource identifier of the interpolator to load
 */
ECode ProgressBar::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resID)
{
    AutoPtr<IInterpolator> p;
    AnimationUtils::LoadInterpolator(context, resID, (IInterpolator**)&p);
    SetInterpolator(p);

    return NOERROR;
}

/**
 * Sets the acceleration curve for the indeterminate animation.
 * Defaults to a linear interpolation.
 *
 * @param interpolator The interpolator which defines the acceleration curve
 */
ECode ProgressBar::SetInterpolator(
    /* [in] */ IInterpolator* interpolator)
{
    mInterpolator = interpolator;

    return NOERROR;
}

/**
 * Gets the acceleration curve type for the indeterminate animation.
 *
 * @return the {@link Interpolator} associated to this animation
 */
AutoPtr<IInterpolator> ProgressBar::GetInterpolator()
{
    return mInterpolator;
}

ECode ProgressBar::SetVisibility(
    /* [in] */ Int32 v)
{
    if (GetVisibility() != v) {
        View::SetVisibility(v);

        if (mIndeterminate) {
            // let's be nice with the UI thread
            if (v == IView::GONE || v == IView::INVISIBLE) {
                StopAnimation();
            }
            else {
                StartAnimation();
            }
        }
    }

    return NOERROR;
}

void ProgressBar::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    View::OnVisibilityChanged(changedView, visibility);

    if (mIndeterminate) {
        // let's be nice with the UI thread
        if (visibility == IView::GONE || visibility == IView::INVISIBLE) {
            StopAnimation();
        }
        else {
            StartAnimation();
        }
    }
}

ECode ProgressBar::InvalidateDrawable(
    /* [in] */ IDrawable* dr)
{
    if (!mInDrawing) {
        if (VerifyDrawable(dr)) {
            AutoPtr<IRect> dirty;
            dr->GetBounds((IRect**)&dirty);
            Int32 scrollX = mScrollX + mPaddingLeft;
            Int32 scrollY = mScrollY + mPaddingTop;

            CRect* _dirty = (CRect*)dirty.Get();
            Invalidate(_dirty->mLeft + scrollX, _dirty->mTop + scrollY,
                    _dirty->mRight + scrollX, _dirty->mBottom + scrollY);
        }
        else {
            View::InvalidateDrawable(dr);
        }
    }

    return NOERROR;
}

void ProgressBar::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    UpdateDrawableBounds(w, h);
}

void ProgressBar::UpdateDrawableBounds(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    // onDraw will translate the canvas so we draw starting at 0,0.
    // Subtract out padding for the purposes of the calculations below.
    w -= mPaddingRight + mPaddingLeft;
    h -= mPaddingTop + mPaddingBottom;

    Int32 right = w;
    Int32 bottom = h;
    Int32 top = 0;
    Int32 left = 0;

    if (mIndeterminateDrawable != NULL) {
        // Aspect ratio logic does not apply to AnimationDrawables
        if (mOnlyIndeterminate && !(mIndeterminateDrawable->Probe(EIID_IAnimationDrawable))) {
            // Maintain aspect ratio. Certain kinds of animated drawables
            // get very confused otherwise.
            Int32 intrinsicWidth;
            mIndeterminateDrawable->GetIntrinsicWidth(&intrinsicWidth);
            Int32 intrinsicHeight;
            mIndeterminateDrawable->GetIntrinsicHeight(&intrinsicHeight);
            Float intrinsicAspect = (Float) intrinsicWidth / intrinsicHeight;
            Float boundAspect = (Float) w / h;
            if (intrinsicAspect != boundAspect) {
                if (boundAspect > intrinsicAspect) {
                    // New width is larger. Make it smaller to match height.
                    Int32 width = (Int32) (h * intrinsicAspect);
                    left = (w - width) / 2;
                    right = left + width;
                } else {
                    // New height is larger. Make it smaller to match width.
                    Int32 height = (Int32) (w * (1 / intrinsicAspect));
                    top = (h - height) / 2;
                    bottom = top + height;
                }
            }
        }
        if (IsLayoutRtl()) {
            Int32 tempLeft = left;
            left = w - right;
            right = w - tempLeft;
        }
        mIndeterminateDrawable->SetBounds(left, top, right, bottom);
    }

    if (mProgressDrawable != NULL) {
        mProgressDrawable->SetBounds(0, 0, right, bottom);
    }
}

//synchronized
//
void ProgressBar::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    Mutex::Autolock lock(*GetSelfLock());

    View::OnDraw(canvas);

    AutoPtr<IDrawable> d = mCurrentDrawable;
    if (d.Get() != NULL) {
        // Translate canvas so a indeterminate circular progress bar with padding
        // rotates properly in its animation

        Int32 s;
        canvas->Save(&s);
        if(IsLayoutRtl()) {
            canvas->Translate(GetWidth() - mPaddingRight, mPaddingTop);
            canvas->Scale(-1.0f, 1.0f);
        } else {
            canvas->Translate(mPaddingLeft, mPaddingTop);
        }
        Int64 time = GetDrawingTime();
        if (mHasAnimation) {
            Boolean res;
            mAnimation->GetTransformation(time, mTransformation, &res);
            Float scale;
            mTransformation->GetAlpha(&scale);
            mInDrawing = TRUE;
            d->SetLevel((Int32)(scale * MAX_LEVEL), &res);
            mInDrawing = FALSE;
            PostInvalidateOnAnimation();
        }
        d->Draw(canvas);
        canvas->Restore();
        if (mShouldStartAnimationDrawable && IAnimatable::Probe(d)) {
            ((IAnimatable*)(IAnimatable::Probe(d)))->Start();
            mShouldStartAnimationDrawable = FALSE;
        }
    }
}

//synchronized
//
void ProgressBar::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Mutex::Autolock lock(*GetSelfLock());

    AutoPtr<IDrawable> d = mCurrentDrawable;

    Int32 dw = 0;
    Int32 dh = 0;
    if (d != NULL) {
        d->GetIntrinsicWidth(&dw);
        d->GetIntrinsicHeight(&dh);
        dw = Elastos::Core::Math::Max(mMinWidth, Elastos::Core::Math::Min(mMaxWidth, dw));
        dh = Elastos::Core::Math::Max(mMinHeight, Elastos::Core::Math::Min(mMaxHeight, dh));
    }

    UpdateDrawableState();
    dw += mPaddingLeft + mPaddingRight;
    dh += mPaddingTop + mPaddingBottom;

    SetMeasuredDimension(ResolveSizeAndState(dw, widthMeasureSpec, 0),
            ResolveSizeAndState(dh, heightMeasureSpec, 0));
}

ECode ProgressBar::DrawableStateChanged()
{
    View::DrawableStateChanged();
    UpdateDrawableState();
    return NOERROR;
}

void ProgressBar::UpdateDrawableState()
{
    AutoPtr<ArrayOf<Int32> > state = GetDrawableState();

    Boolean stateful;
    if (mProgressDrawable != NULL && (mProgressDrawable->IsStateful(&stateful), stateful)) {
        mProgressDrawable->SetState(state, &stateful);
    }

    if (mIndeterminateDrawable != NULL && (mIndeterminateDrawable->IsStateful(&stateful), stateful)) {
        mIndeterminateDrawable->SetState(state, &stateful);
    }
}

AutoPtr<IParcelable> ProgressBar::OnSaveInstanceState()
{
    // Force our ancestor class to save its state
    AutoPtr<IParcelable> superState = View::OnSaveInstanceState();
    AutoPtr<CProgressBarSavedState> ss;
    CProgressBarSavedState::NewByFriend(superState, (CProgressBarSavedState**)&ss);

    ss->mProgress = mProgress;
    ss->mSecondaryProgress = mSecondaryProgress;

    return (IParcelable*)ss->Probe(EIID_IParcelable);
}

void ProgressBar::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    AutoPtr<IProgressBarSavedState> ss = IProgressBarSavedState::Probe(state);

    AutoPtr<IParcelable> p;
    ss->GetSuperState((IParcelable**)&p);
    View::OnRestoreInstanceState(p);

    SetProgress(((CProgressBarSavedState*)(ss.Get()))->mProgress);
    SetSecondaryProgress(((CProgressBarSavedState*)ss.Get())->mSecondaryProgress);
}

ECode ProgressBar::OnAttachedToWindow()
{
    View::OnAttachedToWindow();
    if (mIndeterminate) {
        StartAnimation();
    }
    {
        Mutex::Autolock lock(*GetSelfLock());
        Int32 count = mRefreshData.GetSize();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<RefreshData> rd = mRefreshData[i];
            DoRefreshProgress(rd->mId, rd->mProgress, rd->mFromUser, TRUE);
            rd->Recycle();
        }
        mRefreshData.Clear();
    }
    mAttached = TRUE;
    return NOERROR;
}

ECode ProgressBar::OnDetachedFromWindow()
{
//
    if (mIndeterminate) {
        StopAnimation();
    }
    if (mRefreshProgressRunnable != NULL) {
        RemoveCallbacks(mRefreshProgressRunnable);
    }
    if (mRefreshProgressRunnable != NULL && mRefreshIsPosted) {
        RemoveCallbacks(mRefreshProgressRunnable);
    }
    if (mAccessibilityEventSender != NULL) {
        RemoveCallbacks(mAccessibilityEventSender);
    }
    // This should come after stopAnimation(), otherwise an invalidate message remains in the
    // queue, which can prevent the entire view hierarchy from being GC'ed during a rotation
    View::OnDetachedFromWindow();
    mAttached = FALSE;
    return NOERROR;
}

//@Override
ECode ProgressBar::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    View::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(String("CProgressBar"), (ICharSequence**)&className));
    event->SetClassName(className);
    event->SetItemCount(mMax);
    event->SetCurrentItemIndex(mProgress);
    return NOERROR;
}

//@Override
ECode ProgressBar::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    View::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(String("CProgressBar"), (ICharSequence**)&className));
    info->SetClassName(className);
    return NOERROR;
}

/**
 * Schedule a command for sending an accessibility event.
 * </br>
 * Note: A command is used to ensure that accessibility events
 *       are sent at most one in a given time frame to save
 *       system resources while the progress changes quickly.
 */
void ProgressBar::ScheduleAccessibilityEventSender()
{
    if (mAccessibilityEventSender == NULL) {
        AutoPtr<IWeakReferenceSource> wrs = THIS_PROBE(IWeakReferenceSource);
        assert(wrs);
        AutoPtr<IWeakReference> wr;
        wrs->GetWeakReference((IWeakReference**)&wr);
        mAccessibilityEventSender = new AccessibilityEventSender(wr);
    }
    else {
        RemoveCallbacks(mAccessibilityEventSender);
    }
    PostDelayed(mAccessibilityEventSender, TIMEOUT_SEND_ACCESSIBILITY_EVENT);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

