
#include "ext/frameworkext.h"
#include "graphics/drawable/DrawableContainer.h"
#include "graphics/CRect.h"
#include "graphics/Insets.h"
#include "os/SystemClock.h"

using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

const Boolean DrawableContainer::DEFAULT_DITHER;
const String DrawableContainer::TAG = String("DrawableContainer");

DrawableContainer::DrawableContainer()
    : mAlpha(0xFF)
    , mCurIndex(-1)
    , mMutated(FALSE)
    , mEnterAnimationEnd(0)
    , mExitAnimationEnd(0)
{
}

ECode DrawableContainer::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mCurrDrawable != NULL) {
        mCurrDrawable->Draw(canvas);
    }
    if (mLastDrawable != NULL) {
        mLastDrawable->Draw(canvas);
    }
    return NOERROR;
}

Int32 DrawableContainer::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mDrawableContainerState->mChangingConfigurations
            | mDrawableContainerState->mChildrenChangingConfigurations;
}

Boolean DrawableContainer::GetPadding(
    /* [in] */ IRect* padding)
{
    AutoPtr<IRect> r = mDrawableContainerState->GetConstantPadding();
    if (r != NULL) {
        padding->SetEx(r);
        return TRUE;
    }
    if (mCurrDrawable != NULL) {
        Boolean result;
        mCurrDrawable->GetPadding(padding, &result);
        return result;
    } else {
        return Drawable::GetPadding(padding);
    }
}

AutoPtr<IInsets> DrawableContainer::GetLayoutInsets()
{
    if (mCurrDrawable == NULL) {
        return Insets::NONE;
    }

    AutoPtr<IInsets> set;
    mCurrDrawable->GetLayoutInsets((IInsets**)&set);
    return set;
}

ECode DrawableContainer::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (mAlpha != alpha) {
        mAlpha = alpha;
        if (mCurrDrawable != NULL) {
            if (mEnterAnimationEnd == 0) {
                AutoPtr<IDrawable> dr;
                mCurrDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha(alpha);
            }
            else {
                Animate(FALSE);
            }
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetDither(
    /* [in] */ Boolean dither)
{
    if (mDrawableContainerState->mDither != dither) {
        mDrawableContainerState->mDither = dither;
        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> dr;
            mCurrDrawable->Mutate((IDrawable**)&dr);
            dr->SetDither(mDrawableContainerState->mDither);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mColorFilter.Get() != cf) {
        mColorFilter = cf;
        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> dr;
            mCurrDrawable->Mutate((IDrawable**)&dr);
            dr->SetColorFilter(cf);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetEnterFadeDuration(
    /* [in] */ Int32 ms)
{
    mDrawableContainerState->mEnterFadeDuration = ms;
    return NOERROR;
}

ECode DrawableContainer::SetExitFadeDuration(
    /* [in] */ Int32 ms)
{
    mDrawableContainerState->mExitFadeDuration = ms;
    return NOERROR;
}

void DrawableContainer::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    if (mLastDrawable != NULL) {
        mLastDrawable->SetBoundsEx(bounds);
    }
    if (mCurrDrawable != NULL) {
        mCurrDrawable->SetBoundsEx(bounds);
    }
}

Boolean DrawableContainer::IsStateful()
{
    return mDrawableContainerState->IsStateful();
}

ECode DrawableContainer::JumpToCurrentState()
{
    Boolean changed = FALSE;
    if (mLastDrawable != NULL) {
        mLastDrawable->JumpToCurrentState();
        mLastDrawable = NULL;
        changed = TRUE;
    }
    if (mCurrDrawable != NULL) {
        mCurrDrawable->JumpToCurrentState();
        AutoPtr<IDrawable> dr;
        mCurrDrawable->Mutate((IDrawable**)&dr);
        dr->SetAlpha(mAlpha);
    }
    if (mExitAnimationEnd != 0) {
        mExitAnimationEnd = 0;
        changed = TRUE;
    }
    if (mEnterAnimationEnd != 0) {
        mEnterAnimationEnd = 0;
        changed = TRUE;
    }
    if (changed) {
        InvalidateSelf();
    }
    return NOERROR;
}

Boolean DrawableContainer::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    if (mLastDrawable != NULL) {
        Boolean isStateful;
        mLastDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &isStateful);
        return isStateful;
    }
    if (mCurrDrawable != NULL) {
        Boolean isStateful;
        mCurrDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &isStateful);
        return isStateful;
    }
    return FALSE;
}

Boolean DrawableContainer::OnLevelChange(
    /* [in] */ Int32 level)
{
    if (mLastDrawable != NULL) {
        Boolean changed;
        mLastDrawable->SetLevel(level, &changed);
        return changed;
    }
    if (mCurrDrawable != NULL) {
        Boolean changed;
        mCurrDrawable->SetLevel(level, &changed);
        return changed;
    }
    return FALSE;
}

Int32 DrawableContainer::GetIntrinsicWidth()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantWidth();
    }
    Int32 width = -1;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetIntrinsicWidth(&width);
    }
    return width;
}

Int32 DrawableContainer::GetIntrinsicHeight()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantHeight();
    }
    Int32 height = -1;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetIntrinsicHeight(&height);
    }
    return height;
}

Int32 DrawableContainer::GetMinimumWidth()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantMinimumWidth();
    }
    Int32 width = 0;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetMinimumWidth(&width);
    }
    return width;
}

Int32 DrawableContainer::GetMinimumHeight()
{
    if (mDrawableContainerState->IsConstantSize()) {
        return mDrawableContainerState->GetConstantMinimumHeight();
    }
    Int32 height = 0;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetMinimumHeight(&height);
    }
    return height;
}

ECode DrawableContainer::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (who == mCurrDrawable.Get() && callback != NULL) {
        callback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode DrawableContainer::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (who == mCurrDrawable.Get() && callback != NULL) {
        callback->ScheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }
    return NOERROR;
}

ECode DrawableContainer::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (who == mCurrDrawable && callback != NULL) {
        callback->UnscheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what);
    }
    return NOERROR;
}

Boolean DrawableContainer::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = Drawable::SetVisible(visible, restart);
    if (mLastDrawable != NULL) {
        Boolean isDifferent;
        mLastDrawable->SetVisible(visible, restart, &isDifferent);
    }
    if (mCurrDrawable != NULL) {
        Boolean isDifferent;
        mCurrDrawable->SetVisible(visible, restart, &isDifferent);
    }
    return changed;
}

Int32 DrawableContainer::GetOpacity()
{
    Boolean visible;
    return (mCurrDrawable == NULL || (mCurrDrawable->IsVisible(&visible), !visible)) ? IPixelFormat::TRANSPARENT :
            mDrawableContainerState->GetOpacity();
}

Boolean DrawableContainer::SelectDrawable(
    /* [in] */ Int32 idx)
{
    if (idx == mCurIndex) {
        return FALSE;
    }

    Int64 now = SystemClock::GetUptimeMillis();

    // if (DEBUG) android.util.Log.i(TAG, toString() + " from " + mCurIndex + " to " + idx
    //         + ": exit=" + mDrawableContainerState.mExitFadeDuration
    //         + " enter=" + mDrawableContainerState.mEnterFadeDuration);

    if (mDrawableContainerState->mExitFadeDuration > 0) {
        if (mLastDrawable != NULL) {
            Boolean isDifferent;
            mLastDrawable->SetVisible(FALSE, FALSE, &isDifferent);
        }
        if (mCurrDrawable != NULL) {
            mLastDrawable = mCurrDrawable;
            mExitAnimationEnd = now + mDrawableContainerState->mExitFadeDuration;
        }
        else {
            mLastDrawable = NULL;
            mExitAnimationEnd = 0;
        }
    }
    else if (mCurrDrawable != NULL) {
        Boolean isDifferent;
        mCurrDrawable->SetVisible(FALSE, FALSE, &isDifferent);
    }

    Boolean isDifferent;
    if (idx >= 0 && idx < mDrawableContainerState->mNumChildren) {
        AutoPtr<IDrawable> d = (*mDrawableContainerState->mDrawables)[idx];
        mCurrDrawable = d;
        mCurIndex = idx;
        if (d != NULL) {
            AutoPtr<IDrawable> dr;
            d->Mutate((IDrawable**)&dr);
            if (mDrawableContainerState->mEnterFadeDuration > 0) {
                mEnterAnimationEnd = now + mDrawableContainerState->mEnterFadeDuration;
            }
            else {
                d->SetAlpha(mAlpha);
            }
            d->SetVisible(IsVisible(), TRUE, &isDifferent);
            d->SetDither(mDrawableContainerState->mDither);
            d->SetColorFilter(mColorFilter);
            d->SetState(GetState(), &isDifferent);
            d->SetLevel(GetLevel(), &isDifferent);
            d->SetBoundsEx(GetBounds());
        }
    }
    else {
        mCurrDrawable = NULL;
        mCurIndex = -1;
    }

    if (mEnterAnimationEnd != 0 || mExitAnimationEnd != 0) {
        if (mAnimationRunnable == NULL) {
            mAnimationRunnable = new _Runnable(this);
        }
        else {
            UnscheduleSelf(mAnimationRunnable);
        }
        // Compute first frame and schedule next animation.
        Animate(true);
    }

    InvalidateSelf();
    return TRUE;
}

void DrawableContainer::Animate(
    /* [in] */ Boolean schedule)
{
    Int64 now = SystemClock::GetUptimeMillis();
    Boolean animating = FALSE;
    if (mCurrDrawable != NULL) {
        if (mEnterAnimationEnd != 0) {
            if (mEnterAnimationEnd <= now) {
                AutoPtr<IDrawable> dr;
                mCurrDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha(mAlpha);
                mEnterAnimationEnd = 0;
            }
            else {
                Int32 animAlpha = (Int32)((mEnterAnimationEnd - now) * 255)
                        / mDrawableContainerState->mEnterFadeDuration;
                //if (DEBUG) android.util.Log.i(TAG, toString() + " cur alpha " + animAlpha);
                AutoPtr<IDrawable> dr;
                mCurrDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha(((255 - animAlpha) * mAlpha) / 255);
                animating = TRUE;
            }
        }
    }
    else {
        mEnterAnimationEnd = 0;
    }

    if (mLastDrawable != NULL) {
        if (mExitAnimationEnd != 0) {
            if (mExitAnimationEnd <= now) {
                Boolean isDifferent;
                mLastDrawable->SetVisible(FALSE, FALSE, &isDifferent);
                mLastDrawable = NULL;
                mExitAnimationEnd = 0;
            }
            else {
                Int32 animAlpha = (Int32)((mExitAnimationEnd - now) * 255)
                        / mDrawableContainerState->mExitFadeDuration;
                //if (DEBUG) android.util.Log.i(TAG, toString() + " last alpha " + animAlpha);
                AutoPtr<IDrawable> dr;
                mLastDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha((animAlpha * mAlpha) / 255);
                animating = TRUE;
            }
        }
    }
    else {
        mExitAnimationEnd = 0;
    }

    if (schedule && animating) {
        ScheduleSelf(mAnimationRunnable, now + 1000 / 60);
    }
}

AutoPtr<IDrawable> DrawableContainer::GetCurrent()
{
    return mCurrDrawable;
}

AutoPtr<IDrawableConstantState> DrawableContainer::GetConstantState()
{
    if (mDrawableContainerState->CanConstantState()) {
        mDrawableContainerState->mChangingConfigurations = GetChangingConfigurations();
        AutoPtr<IDrawableConstantState> ret =
                (IDrawableConstantState*)mDrawableContainerState->Probe(EIID_IDrawableConstantState);
        return ret;
    }
    return NULL;
}

AutoPtr<IDrawable> DrawableContainer::Mutate()
{
    if (!mMutated && Drawable::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        Int32 N = mDrawableContainerState->GetChildCount();
        AutoPtr< ArrayOf<IDrawable*> > drawables = mDrawableContainerState->GetChildren();
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawable> drawable;
            if ((*drawables)[i] != NULL) (*drawables)[i]->Mutate((IDrawable**)&drawable);
        }
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

void DrawableContainer::SetConstantState(
    /* [in] */ DrawableContainerState* state)
{
    mDrawableContainerState = state;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
DrawableContainer::DrawableContainerState::DrawableContainerState(
    /* [in] */ DrawableContainerState* orig,
    /* [in] */ DrawableContainer* owner,
    /* [in] */ IResources* res)
    : mOwner(owner)
    , mChangingConfigurations(0)
    , mChildrenChangingConfigurations(0)
    , mNumChildren(0)
    , mVariablePadding(FALSE)
    , mConstantSize(FALSE)
    , mComputedConstantSize(FALSE)
    , mConstantWidth(0)
    , mConstantHeight(0)
    , mConstantMinimumWidth(0)
    , mConstantMinimumHeight(0)
    , mOpacity(0)
    , mHaveStateful(FALSE)
    , mStateful(FALSE)
    , mCheckedConstantState(FALSE)
    , mCanConstantState(FALSE)
    , mPaddingChecked(FALSE)
    , mDither(DrawableContainer::DEFAULT_DITHER)
    , mEnterFadeDuration(0)
    , mExitFadeDuration(0)
{
    if (orig != NULL) {
        mChangingConfigurations = orig->mChangingConfigurations;
        mChildrenChangingConfigurations = orig->mChildrenChangingConfigurations;

        AutoPtr< ArrayOf<IDrawable*> > origDr = orig->mDrawables;

        mDrawables = ArrayOf<IDrawable*>::Alloc(origDr->GetLength());
        mNumChildren = orig->mNumChildren;

        Int32 N = mNumChildren;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawable> dr;
            if (res != NULL) {
                AutoPtr<IDrawableConstantState> state;
                (*origDr)[i]->GetConstantState((IDrawableConstantState**)&state);
                state->NewDrawableEx(res, (IDrawable**)&dr);
                mDrawables->Set(i, dr);
            }
            else {
                AutoPtr<IDrawableConstantState> state;
                (*origDr)[i]->GetConstantState((IDrawableConstantState**)&state);
                state->NewDrawable((IDrawable**)&dr);
                mDrawables->Set(i, dr);
            }
            (*mDrawables)[i]->SetCallback(
                    owner ? (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback) : NULL);
        }

        mCheckedConstantState = mCanConstantState = TRUE;
        mVariablePadding = orig->mVariablePadding;
        if (orig->mConstantPadding != NULL) {
            CRect::New(orig->mConstantPadding, (IRect**)&mConstantPadding);
        }
        mConstantSize = orig->mConstantSize;
        mComputedConstantSize = orig->mComputedConstantSize;
        mConstantWidth = orig->mConstantWidth;
        mConstantHeight = orig->mConstantHeight;
        mConstantMinimumWidth = orig->mConstantMinimumWidth;
        mConstantMinimumHeight = orig->mConstantMinimumHeight;

        mOpacity = orig->mOpacity;
        mHaveStateful = orig->mHaveStateful;
        mStateful = orig->mStateful;

        mDither = orig->mDither;

        mEnterFadeDuration = orig->mEnterFadeDuration;
        mExitFadeDuration = orig->mExitFadeDuration;
    }
    else {
        mDrawables = ArrayOf<IDrawable*>::Alloc(10);
        mNumChildren = 0;
        mCheckedConstantState = mCanConstantState = FALSE;
    }
}

DrawableContainer::DrawableContainerState::~DrawableContainerState()
{
    mDrawables = NULL;
}

ECode DrawableContainer::DrawableContainerState::GetChangingConfigurations(
    /* [out] */ Int32* configurations)
{
    VALIDATE_NOT_NULL(configurations);

    *configurations = mChangingConfigurations;
    return NOERROR;
}

Int32 DrawableContainer::DrawableContainerState::AddChild(
    /* [in] */ IDrawable* dr)
{
    Int32 pos = mNumChildren;

    if (pos >= mDrawables->GetLength()) {
        GrowArray(pos, pos + 10);
    }

    Boolean isDifferent;
    dr->SetVisible(FALSE, TRUE, &isDifferent);
    dr->SetCallback(mOwner != NULL ? (IDrawableCallback*)mOwner->Probe(EIID_IDrawableCallback) : NULL);

    mDrawables->Set(pos, dr);
    mNumChildren++;
    Int32 config;
    dr->GetChangingConfigurations(&config);
    mChildrenChangingConfigurations |= config;
    mHaveStateful = FALSE;

    mConstantPadding = NULL;
    mPaddingChecked = FALSE;
    mComputedConstantSize = FALSE;

    return pos;
}

Int32 DrawableContainer::DrawableContainerState::GetChildCount()
{
    return mNumChildren;
}

AutoPtr< ArrayOf<IDrawable*> > DrawableContainer::DrawableContainerState::GetChildren()
{
    return mDrawables;
}

void DrawableContainer::DrawableContainerState::SetVariablePadding(
    /* [in] */ Boolean variable)
{
    mVariablePadding = variable;
}

AutoPtr<IRect> DrawableContainer::DrawableContainerState::GetConstantPadding()
{
    if (mVariablePadding) {
        return NULL;
    }
    if (mConstantPadding != NULL || mPaddingChecked) {
        return mConstantPadding;
    }

    AutoPtr<IRect> r;
    AutoPtr<IRect> t;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&t));
    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        Boolean isPadding;
        (*mDrawables)[i]->GetPadding(t, &isPadding);
        if (isPadding) {
            if (r == NULL) ASSERT_SUCCEEDED(CRect::New(0, 0, 0, 0, (IRect**)&r));
            CRect* t_ = (CRect*)t.Get();
            CRect* r_ = (CRect*)r.Get();
            if (t_->mLeft > r_->mLeft) r_->mLeft = t_->mLeft;
            if (t_->mTop > r_->mTop) r_->mTop = t_->mTop;
            if (t_->mRight > r_->mRight) r_->mRight = t_->mRight;
            if (t_->mBottom > r_->mBottom) r_->mBottom = t_->mBottom;
        }
    }
    mPaddingChecked = TRUE;
    mConstantPadding = r;
    return mConstantPadding;
}

void DrawableContainer::DrawableContainerState::SetConstantSize(
    /* [in] */ Boolean constant)
{
    mConstantSize = constant;
}

Boolean DrawableContainer::DrawableContainerState::IsConstantSize()
{
    return mConstantSize;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantWidth()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantWidth;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantHeight()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantHeight;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantMinimumWidth()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantMinimumWidth;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantMinimumHeight()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantMinimumHeight;
}

void DrawableContainer::DrawableContainerState::ComputeConstantSize()
{
    mComputedConstantSize = TRUE;

    Int32 N = GetChildCount();
    mConstantWidth = mConstantHeight = -1;
    mConstantMinimumWidth = mConstantMinimumHeight = 0;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IDrawable> dr = (*mDrawables)[i];
        Int32 s;
        dr->GetIntrinsicWidth(&s);
        if (s > mConstantWidth) mConstantWidth = s;
        dr->GetIntrinsicHeight(&s);
        if (s > mConstantHeight) mConstantHeight = s;
        dr->GetMinimumWidth(&s);
        if (s > mConstantMinimumWidth) mConstantMinimumWidth = s;
        dr->GetMinimumHeight(&s);
        if (s > mConstantMinimumHeight) mConstantMinimumHeight = s;
    }
}

void DrawableContainer::DrawableContainerState::SetEnterFadeDuration(
    /* [in] */ Int32 duration)
{
    mEnterFadeDuration = duration;
}

Int32 DrawableContainer::DrawableContainerState::GetEnterFadeDuration()
{
    return mEnterFadeDuration;
}

void DrawableContainer::DrawableContainerState::SetExitFadeDuration(
    /* [in] */ Int32 duration)
{
    mExitFadeDuration = duration;
}

Int32 DrawableContainer::DrawableContainerState::GetExitFadeDuration()
{
    return mExitFadeDuration;
}

Int32 DrawableContainer::DrawableContainerState::GetOpacity()
{
    Int32 N = GetChildCount();
    Int32 op;
    if (N > 0) {
        Int32 opacity;
        (*mDrawables)[0]->GetOpacity(&opacity);
        op = opacity;
    }
    else op = IPixelFormat::TRANSPARENT;
    for (Int32 i = 1; i < N; i++) {
        Int32 opacity;
        (*mDrawables)[i]->GetOpacity(&opacity);
        op = Drawable::ResolveOpacity(op, opacity);
    }
    mOpacity = op;
    return op;
}

Boolean DrawableContainer::DrawableContainerState::IsStateful()
{
    if (mHaveStateful) {
        return mStateful;
    }

    Boolean stateful = FALSE;
    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        Boolean state;
        (*mDrawables)[i]->IsStateful(&state);
        if (state) {
            stateful = TRUE;
            break;
        }
    }

    mStateful = stateful;
    mHaveStateful = TRUE;
    return stateful;
}

void DrawableContainer::DrawableContainerState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    AutoPtr<ArrayOf<IDrawable*> > newDrawables = ArrayOf<IDrawable*>::Alloc(newSize);
    for (Int32 i = 0; i < oldSize; i++) {
        newDrawables->Set(i, (*mDrawables)[i]);
    }
    mDrawables = newDrawables;
}

Boolean DrawableContainer::DrawableContainerState::CanConstantState()
{
    Mutex::Autolock lock(&mLock);

    if (!mCheckedConstantState) {
        mCanConstantState = TRUE;
        Int32 N = mNumChildren;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawableConstantState> state;
            (*mDrawables)[i]->GetConstantState((IDrawableConstantState**)&state);
            if (state == NULL) {
                mCanConstantState = FALSE;
                break;
            }
        }
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}

DrawableContainer::_Runnable::_Runnable(
    /* [in] */ DrawableContainer* host)
    : mHost(host)
{}

PInterface DrawableContainer::_Runnable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == Elastos::Core::EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 DrawableContainer::_Runnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DrawableContainer::_Runnable::Release()
{
    return ElRefBase::Release();
}

ECode DrawableContainer::_Runnable::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = Elastos::Core::EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode DrawableContainer::_Runnable::Run()
{
    mHost->Animate(TRUE);
    mHost->InvalidateSelf();
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
