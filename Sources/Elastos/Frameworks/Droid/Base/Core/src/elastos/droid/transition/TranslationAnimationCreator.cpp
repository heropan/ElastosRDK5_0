
#include "elastos/droid/transition/TranslationAnimationCreator.h"
#include "elastos/droid/transition/CTransitionValues.h"
#include "elastos/droid/animation/ObjectAnimator.h"
#include "elastos/droid/graphics/CPath.h"
//#include "elastos/droid/view/View.h"

#include <elastos/core/Math.h>

using Elastos::Droid::Animation::IAnimatorListener;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::ObjectAnimator;
using Elastos::Droid::Graphics::IPath;
using Elastos::Droid::Graphics::CPath;
//using Elastos::Droid::View::View;

namespace Elastos {
namespace Droid {
namespace Transition {

//===============================================================
// TranslationAnimationCreator::
//===============================================================

AutoPtr<IAnimator> TranslationAnimationCreator::CreateAnimation(
    /* [in] */ IView* view,
    /* [in] */ ITransitionValues* values,
    /* [in] */ Int32 viewPosX,
    /* [in] */ Int32 viewPosY,
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float endX,
    /* [in] */ Float endY,
    /* [in] */ ITimeInterpolator* interpolator)
{
    Float terminalX = 0;
    view->GetTranslationX(&terminalX);
    Float terminalY = 0;
    view->GetTranslationY(&terminalY);
    AutoPtr<ArrayOf<Int32> > startPosition;
    assert(0 && "TODO");
//    values->mView->GetTag(R.id.transitionPosition, (ArrayOf<Int32>**)&startPosition);
    if (startPosition != NULL) {
        startX = (*startPosition)[0] - viewPosX + terminalX;
        startY = (*startPosition)[1] - viewPosY + terminalY;
    }
    // Initial position is at translation startX, startY, so position is offset by that amount
    Int32 startPosX = viewPosX + Elastos::Core::Math::Round(startX - terminalX);
    Int32 startPosY = viewPosY + Elastos::Core::Math::Round(startY - terminalY);

    view->SetTranslationX(startX);
    view->SetTranslationY(startY);
    if (startX == endX && startY == endY) {
        return NULL;
    }
    AutoPtr<IPath> path;
    CPath::New((IPath**)&path);
    path->MoveTo(startX, startY);
    path->LineTo(endX, endY);
    assert(0 && "TODO");
    AutoPtr<IObjectAnimator> anim;// = ObjectAnimator::OfFloat(view, Elastos::Droid::View::View::TRANSLATION_X, View::TRANSLATION_Y,
            //path);

    AutoPtr<CTransitionValues> ctv = (CTransitionValues*)values;
    AutoPtr<TransitionPositionListener> listener = new TransitionPositionListener(view, ctv->mView,
            startPosX, startPosY, terminalX, terminalY);
    IAnimator::Probe(anim)->AddListener(IAnimatorListener::Probe(listener));
    IAnimator::Probe(anim)->AddPauseListener(listener);
    IAnimator::Probe(anim)->SetInterpolator(interpolator);
    return IAnimator::Probe(anim);
}

//===============================================================
// TranslationAnimationCreator::TransitionPositionListener::
//===============================================================

TranslationAnimationCreator::TransitionPositionListener::TransitionPositionListener(
    /* [in] */ IView* movingView,
    /* [in] */ IView* viewInHierarchy,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Float terminalX,
    /* [in] */ Float terminalY)
{
    mMovingView = movingView;
    mViewInHierarchy = viewInHierarchy;
    Float x, y;
    mMovingView->GetTranslationX(&x);
    mMovingView->GetTranslationY(&y);
    mStartX = startX - Elastos::Core::Math::Round(x);
    mStartY = startY - Elastos::Core::Math::Round(y);
    mTerminalX = terminalX;
    mTerminalY = terminalY;
    assert(0 && "TODO");
//    mTransitionPosition = (Int32[]) mViewInHierarchy->GetTag(R.id.transitionPosition);
    if (mTransitionPosition != NULL) {
//        mViewInHierarchy->SetTagInternal(R.id.transitionPosition, NULL);
    }
}

ECode TranslationAnimationCreator::TransitionPositionListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    if (mTransitionPosition == NULL) {
        mTransitionPosition = ArrayOf<Int32>::Alloc(2);
    }
    Float x, y;
    mMovingView->GetTranslationX(&x);
    mMovingView->GetTranslationY(&y);
    (*mTransitionPosition)[0] = Elastos::Core::Math::Round(mStartX + x);
    (*mTransitionPosition)[1] = Elastos::Core::Math::Round(mStartY + y);
    assert(0 && "TODO");
//    mViewInHierarchy->SetTagInternal(R.id.transitionPosition, mTransitionPosition);
    return NOERROR;
}

ECode TranslationAnimationCreator::TransitionPositionListener::OnAnimationEnd(
    /* [in] */ IAnimator* animator)
{
    mMovingView->SetTranslationX(mTerminalX);
    mMovingView->SetTranslationY(mTerminalY);
    return NOERROR;
}

ECode TranslationAnimationCreator::TransitionPositionListener::OnAnimationPause(
    /* [in] */ IAnimator* animator)
{
    mMovingView->GetTranslationX(&mPausedX);
    mMovingView->GetTranslationY(&mPausedY);
    mMovingView->SetTranslationX(mTerminalX);
    mMovingView->SetTranslationY(mTerminalY);
    return NOERROR;
}

ECode TranslationAnimationCreator::TransitionPositionListener::OnAnimationResume(
    /* [in] */ IAnimator* animator)
{
    mMovingView->SetTranslationX(mPausedX);
    mMovingView->SetTranslationY(mPausedY);
    return NOERROR;
}

} // namespace Transition
} // namepsace Droid
} // namespace Elastos
