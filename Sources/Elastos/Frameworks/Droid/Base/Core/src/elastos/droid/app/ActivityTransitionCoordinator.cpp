
#include "elastos/droid/app/ActivityTransitionCoordinator.h"
#include "elastos/droid/graphics/CMatrix.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/graphics/CRectF.h"
#include "elastos/droid/View/View.h"
//#include "elastos/droid/View/GhostView.h"

#include <elastos/core/Math.h>

using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CRectF;
//using Elastos::Droid::View::GhostView;
using Elastos::Droid::View::View;
using Elastos::Droid::View::IGhostView;
using Elastos::Droid::Transition::ITransitionSet;
using Elastos::Droid::Transition::CTransitionSet;

// using Elastos::Droid::content.Context;
// using Elastos::Droid::graphics.Matrix;
// using Elastos::Droid::graphics.Rect;
// using Elastos::Droid::graphics.RectF;
// using Elastos::Droid::os.Bundle;
// using Elastos::Droid::os.Handler;
// using Elastos::Droid::os.Parcelable;
// using Elastos::Droid::os.ResultReceiver;

// using Elastos::Droid::util.ArrayMap;
// using Elastos::Droid::View::IView;
// using Elastos::Droid::View::IViewGroup;
// using Elastos::Droid::View::IViewGroupOverlay;
// using Elastos::Droid::View::IViewParent;
// using Elastos::Droid::View::IViewTreeObserver;
// using Elastos::Droid::View::IWindow;
// using Elastos::Droid::widget.ImageView;

// import java.util.ArrayList;
// import java.util.Collection;

namespace Elastos {
namespace Droid {
namespace App {

//========================================================================
// ActivityTransitionCoordinator::ContinueTransitionListener
//========================================================================
ActivityTransitionCoordinator::ContinueTransitionListener::ContinueTransitionListener(
    /* [in] */ ActivityTransitionCoordinator* host)
    : mHost(host)
{
}

ECode ActivityTransitionCoordinator::ContinueTransitionListener::OnTransitionStart(
    /* [in] */ ITransition* transition)
{
    mHost->mIsStartingTransition = FALSE;
    AutoPtr<IRunnable> pending = mHost->mPendingTransition;
    mHost->mPendingTransition = NULL;
    if (pending != NULL) {
        return mHost->StartTransition(pending);
    }

    return NOERROR;
}

//========================================================================
// ActivityTransitionCoordinator::FixedEpicenterCallback
//========================================================================
CAR_INTERFACE_IMPL(ActivityTransitionCoordinator::FixedEpicenterCallback, Object, IEpicenterCallback)

void ActivityTransitionCoordinator::FixedEpicenterCallback::SetEpicenter(
    /* [in] */ IRect* epicenter)
{
    mEpicenter = epicenter;
}

ECode ActivityTransitionCoordinator::FixedEpicenterCallback::OnGetEpicenter(
    /* [in] */ ITransition* transition,
    /* [out] */ IRect** rect)
{
    VALIDATE_NOT_NULL(rect)
    *rect = mEpicenter;
    REFCOUNT_ADD(*rect)
    return NOERROR;
}


//========================================================================
// ActivityTransitionCoordinator::GhostViewListeners
//========================================================================
CAR_INTERFACE_IMPL(ActivityTransitionCoordinator::GhostViewListeners, Object, IOnPreDrawListener)

ActivityTransitionCoordinator::GhostViewListenersGhostViewListeners(
    /* [in] */ IView* view,
    /* [in] */ IView* parent,
    /* [in] */ IViewGroup* decor)
    : mView(view)
    , mParent(parent)
    , mDecor(decor)
{}

AutoPtr<IView> ActivityTransitionCoordinator::GhostViewListenersGetView()
{
    return mView;
}

ECode ActivityTransitionCoordinator::GhostViewListenersOnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    assert(0 && "TODO");
    // AutoPtr<IGhostView> ghostView = GhostView::GetGhost(mView);
    // if (ghostView == NULL) {
    //     AutoPtr<IViewTreeObserver> vto;
    //     mParent->GetViewTreeObserver((IViewTreeObserver**)&vto);
    //     vto->RemoveOnPreDrawListener(THIS_PROBE(IOnPreDrawListener));
    // }
    // else {
    //     GhostView::CalculateMatrix(mView, mDecor, mMatrix);
    //     ghostView->SetMatrix(mMatrix);
    // }
    *result = TRUE;
    return NOERROR;
}

//========================================================================
// ActivityTransitionCoordinator::SharedElementOriginalState
//========================================================================
ActivityTransitionCoordinator::SharedElementOriginalState::SharedElementOriginalState()
    : mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
    , mMeasuredWidth(0)
    , mMeasuredHeight(0)
    , mTranslationZ(0)
    , mElevation(0)
{}

ECode ActivityTransitionCoordinator::SharedElementOriginalState::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = String("ActivityTransitionCoordinator::SharedElementOriginalState");
    return NOERROR;
}

//========================================================================
// ActivityTransitionCoordinator::DecorViewOnPreDrawListener
//========================================================================
ActivityTransitionCoordinator::DecorViewOnPreDrawListener::DecorViewOnPreDrawListener(
    /* [in] */ ActivityTransitionCoordinator* host,
    /* [in] */ IView* decorView)
    : mHost(host)
    , mDecorView(decorView)
{
}

ECode ActivityTransitionCoordinator::DecorViewOnPreDrawListener::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IViewTreeObserver> vto;
    mDecorView->GetViewTreeObserver((IViewTreeObserver**)&vto);
    vto->RemoveOnPreDrawListener(THIS_PROBE(IOnPreDrawListener));
    mHost->NotifySharedElementEnd(snapshots);

    *result= TRUE;
    return NOERROR;
}

//========================================================================
// ActivityTransitionCoordinator::GhostVisibilityOnPreDrawListener
//========================================================================
ActivityTransitionCoordinator::GhostVisibilityOnPreDrawListener::GhostVisibilityOnPreDrawListener(
    /* [in] */ ActivityTransitionCoordinator* host,
    /* [in] */ IView* decorView,
    /* [in] */ Int32 visibility)
    : mHost(host)
    , mDecorView(decorView)
    , mVisibility(visibility)
{
}

ECode ActivityTransitionCoordinator::GhostVisibilityOnPreDrawListener::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IViewTreeObserver> vto;
    mDecorView->GetViewTreeObserver((IViewTreeObserver**)&vto);
    vto->RemoveOnPreDrawListener(THIS_PROBE(IOnPreDrawListener));
    mHost->SetGhostVisibility(mVisibility);

    *result= TRUE;
    return NOERROR;
}

//========================================================================
// ActivityTransitionCoordinator
//========================================================================
static const String ActivityTransitionCoordinator::KEY_SCREEN_LEFT("shared_element:screenLeft");
static const String ActivityTransitionCoordinator::KEY_SCREEN_TOP("shared_element:screenTop");
static const String ActivityTransitionCoordinator::KEY_SCREEN_RIGHT("shared_element:screenRight");
static const String ActivityTransitionCoordinator::KEY_SCREEN_BOTTOM("shared_element:screenBottom");
static const String ActivityTransitionCoordinator::KEY_TRANSLATION_Z("shared_element:translationZ");
static const String ActivityTransitionCoordinator::KEY_SNAPSHOT("shared_element:bitmap");
static const String ActivityTransitionCoordinator::KEY_SCALE_TYPE("shared_element:scaleType");
static const String ActivityTransitionCoordinator::KEY_IMAGE_MATRIX("shared_element:imageMatrix");
static const String ActivityTransitionCoordinator::KEY_ELEVATION("shared_element:elevation");

static const String ActivityTransitionCoordinator::TAG("ActivityTransitionCoordinator");

/**
 * For Activity transitions, the called Activity's listener to receive calls
 * when transitions complete.
 */
static const String ActivityTransitionCoordinator::KEY_REMOTE_RECEIVER("android:remoteReceiver");

static AutoPtr<ArrayOf<ImageViewScaleType> > InitSCALE_TYPE_VALUES()
{
    AutoPtr<ArrayOf<ImageViewScaleType> > values = ArrayOf<ImageViewScaleType>::Alloc(8);
    values->Set(0, ImageViewScaleType_MATRIX);
    values->Set(1, ImageViewScaleType_FIT_XY);
    values->Set(2, ImageViewScaleType_FIT_START);
    values->Set(3, ImageViewScaleType_FIT_CENTER);
    values->Set(4, ImageViewScaleType_FIT_END);
    values->Set(5, ImageViewScaleType_CENTER);
    values->Set(6, ImageViewScaleType_CENTER_CROP);
    values->Set(7, ImageViewScaleType_CENTER_INSIDE);
    return values;
}

static const AutoPtr<ArrayOf<ImageViewScaleType> > ActivityTransitionCoordinator::SCALE_TYPE_VALUES;// = ImageView.ScaleType.values();

CAR_INTERFACE_IMPL(ActivityTransitionCoordinator, ResultReceiver, IActivityTransitionCoordinator::)

ActivityTransitionCoordinator::ActivityTransitionCoordinator()
    : mIsReturning(FALSE)
    , mIsStartingTransition(FALSE)
{
    CArrayList::New((IArrayList**)&mAllSharedElementNames);
    CArrayList::New((IArrayList**)&mSharedElements);
    CArrayList::New((IArrayList**)&mSharedElementNames);
    CArrayList::New((IArrayList**)&mTransitioningViews);

    mEpicenterCallback = new FixedEpicenterCallback();
}

ActivityTransitionCoordinator::~ActivityTransitionCoordinator()
{}

ECode ActivityTransitionCoordinator::constructor(
    /* [in] */ IWindow* window,
    /* [in] */ IArrayList* allSharedElementNames, //ArrayList<String>
    /* [in] */ ISharedElementCallback* listener,
    /* [in] */ Boolean isReturning)
{
    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    FAIL_RETURN(ResultReceiver::constructor(handler))
    mWindow = window;
    mListener = listener;
    mAllSharedElementNames = allSharedElementNames;
    mIsReturning = isReturning;
    return NOERROR;
}

ECode ActivityTransitionCoordinator::ViewsReady(
    /* [in] */ IArrayMap* sharedElements) //ArrayMap<String, View>
{
    sharedElements->RetainAll(ICollection::Probe(mAllSharedElementNames));
    if (mListener != NULL) {
        mListener->OnMapSharedElements(mAllSharedElementNames, sharedElements);
    }
    AutoPtr<ISet> keyset;
    sharedElements->GetKeySet((ISet**)&keyset);
    mSharedElementNames->AddAll(ICollection::Probe(keyset));
    AutoPtr<ICollection> values;
    sharedElements->GetValues((ICollection**)&values);
    mSharedElements->AddAll(values;

    if (GetViewsTransition() != NULL && mTransitioningViews != NULL) {
        AutoPtr<IViewGroup> decorView;
        GetDecor((IViewGroup**)&decorView);
        if (decorView != NULL) {
            decorView->CaptureTransitioningViews(mTransitioningViews);
        }
        mTransitioningViews->RemoveAll(ICollection::Probe(mSharedElements));
    }

    return SetEpicenter();
}

ECode ActivityTransitionCoordinator::StripOffscreenViews()
{
    if (mTransitioningViews == NULL) {
        return NOERROR;
    }

    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    Int32 size;
    Boolean bval;
    mTransitioningViews->GetSize(&size);
    for (Int32 i = size - 1; i >= 0; i--) {
        AutoPtr<IInterface> obj;
        mTransitioningViews->Get(i, (IView**)&obj);
        IView* view = IView::Probe(obj);
        view->GetGlobalVisibleRect(r, &bval);
        if (!bval) {
            mTransitioningViews->Remove(i);
            ShowView(view, TRUE);
        }
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::GetWindow(
    /* [out] */ IWindow** window)
{
    VALIDATE_NOT_NULL(window)
    *window = mWindow;
    REFCOUNT_ADD(*window)
    return NOERROR;
}

ECode ActivityTransitionCoordinator::GetDecor(
    /* [out] */ IViewGroup** vg)
{
    VALIDATE_NOT_NULL(vg)
    if (mWindow != NULL) {
        AutoPtr<IView> view;
        mWindow->GetDecorView((IView**)&view)
        *vg = IViewGroup::Probe(view)
        REFCOUNT_ADD(*vg)
    }
    else {
        *vg = NULL;
    }

    return NOERROR;
}

ECode ActivityTransitionCoordinator::SetEpicenter()
{
    AutoPtr<IView> epicenter;
    Boolean b1, b2;
    mAllSharedElementNames->IsEmpty(&b1);
    mSharedElementNames->IsEmpty(&b2);
    if (!b1 && !b2) {
        AutoPtr<IInterface> obj;
        mAllSharedElementNames->Get(0, (IInterface**)&obj);
        Int32 index;
        mSharedElementNames->IndexOf(obj, &index);
        if (index >= 0) {
            obj = NULL;
            mSharedElements->Get(index, (IInterface**)&obj);
            epicenter = IView::Probe(obj);
        }
    }
    return SetEpicenter(epicenter);
}

ECode ActivityTransitionCoordinator::SetEpicenter(
    /* [in] */ IView* view)
{
    if (view == NULL) {
        mEpicenterCallback->SetEpicenter(NULL);
    }
    else {
        AutoPtr<IRect> epicenter;
        CRect::New((IRect**)&epicenter);
        view->GetBoundsOnScreen(epicenter);
        mEpicenterCallback->SetEpicenter(epicenter);
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::GetAcceptedNames(
    /* [out] */ IArrayList** list)
{
    VALIDATE_NOT_NULL(list)
    *list = mSharedElementNames;
    REFCOUNT_ADD(*list)
    return NOERROR;
}

ECode ActivityTransitionCoordinator::GetMappedNames(
    /* [out] */ IArrayList** list)
{
    VALIDATE_NOT_NULL(list)
    Int32 size;
    mSharedElements->GetSize(&size);
    AutoPtr<IArrayList> names;
    CArrayList::New(size, (IArrayList**)&names);
    String name;
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj;
        mSharedElements->Get(i, (IInterface**)&obj);
        IView::Probe(obj)->GetTransitionName(&name);
        AutoPtr<ICharsequence> seq = CoreUtils::Convert(name);
        names->Add(IInterface::Probe(seq));
    }

    *list = names;
    REFCOUNT_ADD(*list)
    return NOERROR;
}

ECode ActivityTransitionCoordinator::CopyMappedViews(
    /* [out] */ IArrayList** list)
{
    VALIDATE_NOT_NULL(list)
    return CArrayList::New(ICollection::Probe(mSharedElements), (IArrayList**)&list);
}

ECode ActivityTransitionCoordinator::GetAllSharedElementNames(
    /* [out] */ IArrayList** list)
{
    VALIDATE_NOT_NULL(list)
    *list = mAllSharedElementNames;
    REFCOUNT_ADD(*list)
    return NOERROR;
}

AutoPtr<ITransition> ActivityTransitionCoordinator::SetTargets(
    /* [in] */ ITransition* transition,
    /* [in] */ Boolean add)
{
    Boolean bval;
    if (transition == NULL ||
        (add &&
            (mTransitioningViews == NULL || (mTransitioningViews->IsEmpty(&bval), bval)))) {
        return NULL;
    }

    // Add the targets to a set containing transition so that transition
    // remains unaffected. We don't want to modify the targets of transition itself.
    AutoPtr<ITransitionSet> set;
    CTransitionSet::New((ITransitionSet**)&set);
    if (mTransitioningViews != NULL) {
        Int32 size;
        mTransitioningViews->GetSize(&size);
        for (Int32 i = size - 1; i >= 0; i--) {
            AutoPtr<IInterface> obj;
            mTransitioningViews->Get(i, (IInterface**)&obj);
            IView* view = IView::Probe(obj);
            if (add) {
                set->AddTarget(view);
            }
            else {
                set->ExcludeTarget(view, TRUE);
            }
        }
    }
    // By adding the transition after addTarget, we prevent addTarget from
    // affecting transition.
    set->AddTransition(transition);

    if (!add && mTransitioningViews != NULL && (mTransitioningViews->IsEmpty(&bval), bval)) {
        // Allow children of excluded transitioning views, but not the views themselves
        AutoPtr<ITransitionSet> ts;
        CTransitionSet::New((ITransitionSet**)&ts);
        ts->AddTransition(set);

        set = ts;
    }

    return ITransition::Probe(set);
}

AutoPtr<ITransition> ActivityTransitionCoordinator::ConfigureTransition(
    /* [in] */ ITransition* transition,
    /* [in] */ Boolean includeTransitioningViews)
{
    AutoPtr<ITransition> result = transition;
    if (result != NULL) {
        AutoPtr<IInterface> obj;
        ICloneable::Probe(transition)->Clone((IInterface**)&obj);
        result = ITransition::Probe(obj);
        result->SetEpicenterCallback(mEpicenterCallback);
        AutoPtr<ITransition> tmp = result;
        result = SetTargets(tmp, includeTransitioningViews);
    }
    return result;
}

AutoPtr<ITransition> ActivityTransitionCoordinator::MergeTransitions(
    /* [in] */ ITransition* transition1,
    /* [in] */ ITransition* transition2)
{
    if (transition1 == NULL) {
        return transition2;
    }
    else if (transition2 == NULL) {
        return transition1;
    }
    else {
        AutoPtr<ITransitionSet> transitionSet;
        CTransitionSet::New((ITransitionSet**)&transitionSet);
        transitionSet->AddTransition(transition1);
        transitionSet->AddTransition(transition2);
        return ITransition::Probe(transitionSet);
    }
}

AutoPtr<IArrayMap> ActivityTransitionCoordinator::MapSharedElements(
    /* [in] */ IArrayList* accepted, //ArrayList<String>
    /* [in] */ IArrayList* localViews) //ArrayList<View>
{
    AutoPtr<IArrayMap> sharedElements;
    CArrayMap::New((IArrayMap**)&sharedElements);

    Int32 size;
    if (accepted != NULL) {
        accepted->GetSize(&size);
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> key, value;
            accepted->Get(i, (IInterface**)&key);
            localViews->Get(i, (IInterface**)&value);
            sharedElements->Put(key, value);
        }
    }
    else {
        AutoPtr<IViewGroup> decorView;
        GetDecor((IViewGroup**)&decorView);
        if (decorView != NULL) {
            decorView->FindNamedViews(sharedElements);
        }
    }
    return sharedElements;
}

ECode ActivityTransitionCoordinator::SetResultReceiver(
    /* [in] */ IResultReceiver* resultReceiver)
{
    mResultReceiver = resultReceiver;
    return NOERROR;
}

ECode ActivityTransitionCoordinator::SetSharedElementState(
    /* [in] */ IView* view,
    /* [in] */ const String& name,
    /* [in] */ IBundle* transitionArgs,
    /* [in] */ IMatrix* tempMatrix,
    /* [in] */ IRectF* tempRect,
    /* [in] */ ArrayOf<Int32>* decorLoc)
{
    AutoPtr<IBundle> sharedElementBundle;
    transitionArgs->GetBundle(name, (IBundle**)&sharedElementBundle);
    if (sharedElementBundle == NULL) {
        return NOERROR;
    }

    if (IImageView::Probe(view)) {
        Int32 scaleTypeInt;
        sharedElementBundle->GetInt32(KEY_SCALE_TYPE, -1, &scaleTypeInt);
        if (scaleTypeInt >= 0) {
            IImageView* imageView = IImageView::Probe(view);
            ImageViewScaleType scaleType = (*SCALE_TYPE_VALUES)[scaleTypeInt];
            imageView->SetScaleType(scaleType);
            if (scaleType == ImageViewScaleType_MATRIX) {
                AutoPtr<ArrayOf<Float> > matrixValues;
                sharedElementBundle->GetFloatArray(KEY_IMAGE_MATRIX, (ArrayOf<Float>**)&matrixValues);
                tempMatrix->SetValues(matrixValues);
                imageView->SetImageMatrix(tempMatrix);
            }
        }
    }

    Float z;
    sharedElementBundle->GetFloat(KEY_TRANSLATION_Z, &z);
    view->SetTranslationZ(z);
    Float elevation;
    sharedElementBundle->GetFloat(KEY_ELEVATION, &elevation);
    view->SetElevation(elevation);

    Float left, top, right, bottom;
    sharedElementBundle->GetFloat(KEY_SCREEN_LEFT, &left);
    sharedElementBundle->GetFloat(KEY_SCREEN_TOP, &top);
    sharedElementBundle->GetFloat(KEY_SCREEN_RIGHT, &right);
    sharedElementBundle->GetFloat(KEY_SCREEN_BOTTOM, &bottom);

    if (decorLoc != NULL) {
        left -= (*decorLoc)[0];
        top -= (*decorLoc)[1];
        right -= (*decorLoc)[0];
        bottom -= (*decorLoc)[1];
    }
    else {
        // Find the location in the view's parent
        GetSharedElementParentMatrix(view, tempMatrix);
        tempRect->Set(left, top, right, bottom);
        tempMatrix->MapRect(tempRect);

        Float leftInParent, topInParent;
        tempRect->GetLeft(&leftInParent);
        tempRect->GetTop(&topInParent);

        // Find the size of the view
        AutoPtr<IMatrix> im;
        view->GetInverseMatrix((IMatrix**)&im);
        im->MapRect(tempRect);
        Float width, height;
        tempRect->GetWidth(&width);
        tempRect->GetHeight(&height);

        // Now determine the offset due to view transform:
        view->SetLeft(0);
        view->SetTop(0);
        view->SetRight(Elastos::Core::Math::Round(width));
        view->SetBottom(Elastos::Core::Math::round(height));
        tempRect->Set(0, 0, width, height);

        im = NULL;
        view->GetMatrix((IMatrix**)&im);
        im->MapRect(tempRect);

        AutoPtr<IViewParent> pv;
        view->GetParent((IViewParent**)&pv);
        AutoPtr<IView> parent = IView::Probe(pv);

        Float l, t;
        tempRect->GetLeft(&l);
        tempRect->GetTop(&t);
        Int32 x, y;
        parent->GetScrollX(&x);
        parent->GetScrollY(&y);
        left = leftInParent - l + x;
        top = topInParent - t + y;
        right = left + width;
        bottom = top + height;
    }

    Int32 x = Elastos::Core::Math::Round(left);
    Int32 y = Elastos::Core::Math::Round(top);
    Int32 width = Elastos::Core::Math::Round(right) - x;
    Int32 height = Elastos::Core::Math::Round(bottom) - y;
    Int32 widthSpec = View::MeasureSpec::MakeMeasureSpec(width, View::MeasureSpec::EXACTLY);
    Int32 heightSpec = View::MeasureSpec::MakeMeasureSpec(height, View::MeasureSpec::EXACTLY);
    view->Measure(widthSpec, heightSpec);

    return view->Layout(x, y, x + width, y + height);
}

ECode ActivityTransitionCoordinator::GetSharedElementParentMatrix(
    /* [in] */ IView* view,
    /* [in] */ IMatrix* matrix)
{
    // Find the location in the view's parent
    AutoPtr<IViewParent> pv;
    view->GetParent((IViewParent**)&pv);
    IView* parent = IView::Probe(pv);
    matrix->Reset();
    parent->TransformMatrixToLocal(matrix);
    return NOERROR;
}

AutoPtr<List<AutoPtr<SharedElementOriginalState> > > ActivityTransitionCoordinator::SetSharedElementState(
    /* [in] */ IBundle* sharedElementState,
    /* [in] */ IArrayList* snapshots) //ArrayList<View>
{
    AutoPtr<List<AutoPtr<SharedElementOriginalState> > > originalImageState = new List<AutoPtr<SharedElementOriginalState> >();
    if (sharedElementState != NULL) {
        AutoPtr<IMatrix> tempMatrix;
        CMatrix::New((IMatrix**)&tempMatrix);
        AutoPtr<IRectF> tempRect;
        CRectF::New((IRectF**)&tempRect);
        Int32 numSharedElements;
        mSharedElements->GetSize(&numSharedElements);
        for (Int32 i = 0; i < numSharedElements; i++) {
            AutoPtr<IInterface> obj, nameObj;
            mSharedElements->Get(i, (IInterface**)&obj);
            mSharedElementNames->Get(i, (IInterface**)&nameObj);
            IView* sharedElement = IView::Probe(obj);
            String name = Object::ToString(nameObj);
            AutoPtr<SharedElementOriginalState> originalState = GetOldSharedElementState(sharedElement,
                    name, sharedElementState);
            originalImageState->PushBack(originalState);
            SetSharedElementState(sharedElement, name, sharedElementState,
                    tempMatrix, tempRect, NULL);
        }
    }
    if (mListener != NULL) {
        mListener->OnSharedElementStart(mSharedElementNames, mSharedElements, snapshots);
    }
    return originalImageState;
}

ECode ActivityTransitionCoordinator::NotifySharedElementEnd(
    /* [in] */ IArrayList* snapshots)
{
    if (mListener != NULL) {
        mListener->OnSharedElementEnd(mSharedElementNames, mSharedElements, snapshots);
    }
}

ECode ActivityTransitionCoordinator::ScheduleSetSharedElementEnd(
    /* [in] */ IArrayList* snapshots) //ArrayList<View>
{
    AutoPtr<IView> decorView;
    GetDecor((IView**)&decorView);
    if (decorView != NULL) {
        AutoPtr<IViewTreeObserver> vto;
        decorView->GetViewTreeObserver((IViewTreeObserver**)&vto);
        AutoPtr<IOnPreDrawListener> listener = new DecorViewOnPreDrawListener(decorView, this);
        vto->AddOnPreDrawListener(listener);
    }

    return NOERROR;
}

AutoPtr<SharedElementOriginalState> ActivityTransitionCoordinator::GetOldSharedElementState(
    /* [in] */ IView* view,
    /* [in] */ const String& name,
    /* [in] */ IBundle* transitionArgs)
{
    AutoPtr<SharedElementOriginalState> state = new SharedElementOriginalState();
     = view->getLeft(&state->mLeft);
     = view->getTop(&state->mTop);
     = view->getRight(&state->mRight);
     = view->getBottom(&state->mBottom);
     = view->getMeasuredWidth(&state->mMeasuredWidth);
     = view->getMeasuredHeight(&state->mMeasuredHeight);
     = view->getTranslationZ(&state->mTranslationZ);
     = view->getElevation(&state->mElevation);

    IImageView* imageView = IImageView::Probe(view);
    if (imageView == NULL) {
        return state;
    }

    AutoPtr<IBundle> bundle;
    transitionArgs->GetBundle(name, (IBundle**)&bundle);
    if (bundle == NULL) {
        return state;
    }
    Int32 scaleTypeInt;
    bundle->GetInt32(KEY_SCALE_TYPE, -1, &scaleTypeInt);
    if (scaleTypeInt < 0) {
        return state;
    }

    imageView->GetScaleType(＆state->mScaleType);
    if (state->mScaleType == ImageViewScaleType_MATRIX) {
        AutoPtr<IMatrix> m;
        imageView->GetImageMatrix((IMatrix**)&m);
        state->mMatrix = NULL;
        CMatrix::New(m, (IMatrix**)&state->mMatrix);
    }
    return state;
}

AutoPtr<IArrayList> ActivityTransitionCoordinator::CreateSnapshots( //ArrayList<View>
    /* [in] */ IBundle* state,
    /* [in] */ ICollection* names) //Collection<String>
{
    Int32 numSharedElements;
    names->GetSize(&numSharedElements);
    if (numSharedElements == 0) {
        return NULL;
    }

    AutoPtr<IArrayList> snapshots;
    CArrayList::New(numSharedElements, (IArrayList**)&snapshots);
    AutoPtr<IWindow> window;
    GetWindow((IWindow**)&window);
    AutoPtr<IContext> context;
    window->GetContext((IContext**)&context);
    AutoPtr<ArrayOf<Int32> > decorLoc = ArrayOf<Int32>::Alloc(2);

    AutoPtr<IViewGroup> decorView;
    GetDecor((IViewGroup**)&decorView);
    if (decorView != NULL) {
        decorView->GetLocationOnScreen(decorLoc);
    }

    AutoPtr<IIterator> it;
    names->GetIterator((IIterator)&&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        it->GetNext((IInterface**)&obj);
        String name = Object::ToString(obj);
        AutoPtr<IBundle> sharedElementBundle;
        state->GetBundle(name, (IBundle**)&sharedElementBundle);
        if (sharedElementBundle != NULL) {
            AutoPtr<IParcelable> parcelable;
            sharedElementBundle->GetParcelable(KEY_SNAPSHOT, (IParcelable**)&parcelable);
            AutoPtr<IView> snapshot;
            if (parcelable != NULL && mListener != NULL) {
                mListener->OnCreateSnapshotView(context, parcelable, (IView**)&snapshot);
            }
            if (snapshot != NULL) {
                SetSharedElementState(snapshot, name, state, NULL, NULL, decorLoc);
            }
            snapshots->Add(snapshot);
        }
    }
    return snapshots;
}

ECode ActivityTransitionCoordinator::SetOriginalSharedElementState(
    /* [in] */ IArrayList* sharedElements, //ArrayList<View>
    /* [in] */ List<AutoPtr<SharedElementOriginalState> >* originalState)
{
    List<AutoPtr<SharedElementOriginalState> >::Iterator it;
    Int32 i = 0;
    for (it = originalState->Begin(); it != originalState->End(); ++it, ++i) {
        AutoPtr<IInterface> viewObj;
        sharedElements->Get(i, (IInterface**)&viewObj);
        IView* view = IView::Probe(viewObj);
        AutoPtr<SharedElementOriginalState> state = *it;

        ImageView* imageView = IImageView::Probe(view);
        if (imageView != NULL && state->mScaleType != ImageViewScaleType_NULL) {
            imageView->SetScaleType(state->mScaleType);
            if (state->mScaleType == ImageViewScaleType_MATRIX) {
              imageView->SetImageMatrix(state->mMatrix);
            }
        }
        view->SetElevation(state->mElevation);
        view->SetTranslationZ(state->mTranslationZ);
        Int32 widthSpec = View::MeasureSpec::MakeMeasureSpec(state->mMeasuredWidth,
                View::MeasureSpec::EXACTLY);
        Int32 heightSpec = View::MeasureSpec::MakeMeasureSpec(state->mMeasuredHeight,
                View::MeasureSpec::EXACTLY);
        view->Measure(widthSpec, heightSpec);
        view->Layout(state->mLeft, state->mTop, state->mRight, state->mBottom);
    }
    return NOERROR;
}

AutoPtr<IBundle> ActivityTransitionCoordinator::CaptureSharedElementState()
{
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    AutoPtr<IRectF> tempBounds;
    CRectF::New((IRectF**)&tempBounds);
    AutoPtr<IMatrix> tempMatrix;
    CMatrix::New((IMatrix**)&tempMatrix);

    Int32 size;
    mSharedElements->GetSize(&size);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> obj, nameObj;
        mSharedElements->Get(i, (IInterface**)&obj);
        sharedElement = IView::Probe(obj);
        mSharedElementNames->Get(i, (IInterface**)&nameObj);
        String name = Object::ToString(nameObj);
        CaptureSharedElementState(sharedElement, name, bundle, tempMatrix, tempBounds);
    }
    return bundle;
}

ECode ActivityTransitionCoordinator::ClearState()
{
    // Clear the state so that we can't hold any references accidentally and leak memory.
    mWindow = NULL;
    mSharedElements->Clear();
    mTransitioningViews = NULL;
    mOriginalAlphas.Clear();
    mResultReceiver = NULL;
    mPendingTransition = NULL;
    mListener = NULL;
    return NOERROR;
}

Int64 ActivityTransitionCoordinator::GetFadeDuration()
{
    AutoPtr<IWindow> window;
    GetWindow((IWindow**)&window);
    Int64 duration;
    window->GetTransitionBackgroundFadeDuration(&duration);
    return duration;
}

ECode ActivityTransitionCoordinator::HideViews(
    /* [in] */ IArrayList* views) //ArrayList<View>
{
    Int32 count;
    views->GetSize(&count);
    Float alpha;
    HashMap<AutoPtr<IView>, Float> > it;
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IInterface> obj;
        views->Get(i, (IInterface**)&obj);
        AutoPtr<IView> view = IView::Probe(obj);
        it = mOriginalAlphas.Find(view);
        if (it == mOriginalAlphas.End()) {
            view->GetAlpha(&alpha);
            mOriginalAlphas[view] = alpha;
        }
        view->SetAlpha(0.0f);
    }

    return NOERROR;
}

ECode ActivityTransitionCoordinator::ShowViews(
    /* [in] */ IArrayList* views, //ArrayList<View>
    /* [in] */ Boolean setTransitionAlpha)
{
    Int32 count;
    views->GetSize(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IInterface> obj;
        views->Get(i, (IInterface**)&obj);
        AutoPtr<IView> view = IView::Probe(obj);
        ShowView(view, setTransitionAlpha);
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::ShowView(
    /* [in] */ IView* view,
    /* [in] */ Boolean setTransitionAlpha)
{
    HashMap<AutoPtr<IView>, Float> > it = mOriginalAlphas.Find(view);
    if (it != mOriginalAlphas.End()) {
        view->SetAlpha(it->mSecond);
        mOriginalAlphas.Erase(it);
    }

    if (setTransitionAlpha) {
        view->SetTransitionAlpha(1.0f);
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::CaptureSharedElementState(
    /* [in] */ IView* view,
    /* [in] */ const String& name,
    /* [in] */ IBundle* transitionArgs,
    /* [in] */ IMatrix* tempMatrix,
    /* [in] */ IRectF* tempBounds)
{
    AutoPtr<IBundle> sharedElementBundle;
    CBundle::New((IBundle**)&sharedElementBundle);
    tempMatrix->Reset();
    view->TransformMatrixToGlobal(tempMatrix);
    Int32 w, h;
    view->GetWidth(&w);
    view->GetHeight(&h);
    tempBounds->Set(0, 0, w, h);
    tempMatrix->MapRect(tempBounds);

    Float l, r, t, b;
    tempBounds->Get(&l, &t, &r, &b);
    sharedElementBundle->PutFloat(KEY_SCREEN_LEFT, l);
    sharedElementBundle->PutFloat(KEY_SCREEN_RIGHT, &r);
    sharedElementBundle->PutFloat(KEY_SCREEN_TOP, &t);
    sharedElementBundle->PutFloat(KEY_SCREEN_BOTTOM, &b);
    Float z, e;
    view->GetTranslationZ(&z);
    view->GetElevation(&e);
    sharedElementBundle->PutFloat(KEY_TRANSLATION_Z, z);
    sharedElementBundle->PutFloat(KEY_ELEVATION, e);

    AutoPtr<IParcelable> bitmap;
    if (mListener != NULL) {
        mListener->OnCaptureSharedElementSnapshot(view, tempMatrix, tempBounds, (IParcelable**)&bitmap);
    }

    if (bitmap != NULL) {
        sharedElementBundle->PutParcelable(KEY_SNAPSHOT, bitmap);
    }

    ImageView* imageView = IImageView::Probe(view);
    if (imageView) {
        ImageViewScaleType type;
        imageView->GetScaleType(&type);
        Int32 scaleTypeInt = ScaleTypeToInt(type);
        sharedElementBundle->PutInt32(KEY_SCALE_TYPE, scaleTypeInt);
        if (type == ImageViewScaleType_MATRIX) {
            AutoPtr<ArrayOf<Float> > matrix = ArrayOf<Float>::Alloc(9);
            AutoPtr<IMatrix> m;
            imageView->GetImageMatrix((IMatrix**)&m);
            m->GetValues((ArrayOf<Float>**)&matrix);
            sharedElementBundle->PutFloatArray(KEY_IMAGE_MATRIX, matrix);
        }
    }

    transitionArgs-PutBundle(name, sharedElementBundle);
    return NOERROR;
}

ECode ActivityTransitionCoordinator::StartTransition(
    /* [in] */ IRunnable* runnable)
{
    if (mIsStartingTransition) {
        mPendingTransition = runnable;
    } else {
        mIsStartingTransition = TRUE;
        runnable->Run();
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::TransitionStarted()
{
    mIsStartingTransition = FALSE;
    return NOERROR;
}

ECode ActivityTransitionCoordinator::MoveSharedElementsToOverlay()
{
    Boolean bval;
    mWindow->GetSharedElementsUseOverlay(&bval);
    if (!bval) {
        return NOERROR;
    }

    Int32 numSharedElements;
    mSharedElements->GetSize(&numSharedElements);
    AutoPtr<IViewGroup> decor;
    GetDecor((IViewGroup**)&decor);
    if (decor != NULL) {
        Boolean moveWithParent = MoveSharedElementWithParent();
        for (Int32 i = 0; i < numSharedElements; i++) {
            AutoPtr<IInterface> obj;
            mSharedElements->Get(i, (IInterface**)&obj);
            IView* view = IView::Probe(obj);
            assert(0 && "TODO");
            // GhostView::AddGhost(view, decor);
            AutoPtr<IViewParent> vp;
            view->GetParent((IViewParent**)&vp);
            IViewGroup* parent = IViewGroup::Probe(vp);
            if (moveWithParent && !IsInTransitionGroup(parent, decor)) {
                AutoPtr<GhostViewListeners> listener = new GhostViewListeners(view, parent, decor);
                AutoPtr<IViewTreeObserver> vto;
                parent->GetViewTreeObserver((IViewTreeObserver**)&vto);
                vto->AddOnPreDrawListener((IOnPreDrawListener*)listener.Get());
                mGhostViewListeners.PushBack(listener);
            }
        }
    }
    return NOERROR;
}

Boolean ActivityTransitionCoordinator::MoveSharedElementWithParent()
{
    return TRUE;
}

Boolean ActivityTransitionCoordinator::IsInTransitionGroup(
    /* [in] */ IViewParent* viewParent,
    /* [in] */ IViewGroup* decor)
{
    IViewGroup parent = IViewGroup::Probe(viewParent);
    if (viewParent == decor || parent == NULL) {
        return FALSE;
    }
    Boolean bval;
    parent->IsInTransitionGroup(&bval);
    if (bval) {
        return TRUE;
    }
    else {
        AutoPtr<IViewParent> vp;
        parent->GetParent((IViewParent**)&vp)
        return IsInTransitionGroup(vp, decor);
    }
}

ECode ActivityTransitionCoordinator::MoveSharedElementsFromOverlay()
{
    List<AutoPtr<GhostViewListeners> >::Iterator it;
    for (it = mGhostViewListeners.Begin(); it != mGhostViewListeners.End(); ++it) {
        AutoPtr<GhostViewListeners> listener = *it;
        AutoPtr<IView> view;
        listener-GetView((IView**)&View);
        AutoPtr<IViewParent> vp;
        View->GetParent((IViewParent**)&vp);
        IViewGroup* parent = IViewGroup::Probe(vp);
        AutoPtr<IViewTreeObserver> vto;
        parent->GetViewTreeObserver((IViewTreeObserver**)&vto);
        vto->RemoveOnPreDrawListener((IOnPreDrawListener*)listener.Get());
    }
    mGhostViewListeners.Clear();

    Boolean bval;
    if (mWindow == NULL || (mWindow->GetSharedElementsUseOverlay(&bval), !bval)) {
        return NOERROR;
    }

    AutoPtr<IViewGroup> decor;
    GetDecor((IViewGroup**)&decor);
    if (decor != NULL) {
        AutoPtr<IViewGroupOverlay> overlay;
        decor->GetOverlay((IViewGroupOverlay**)&overlay);
        Int32 count;
        mSharedElements->GetSize(&count);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IInterface> obj;
            mSharedElements->Get(i, (IInterface**)&obj);
            IView* sharedElement = IView::Probe(obj);
            assert(0 && "TODO");
            // GhostView::RemoveGhost(sharedElement);
        }
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::SetGhostVisibility(
    /* [in] */ Int32 visibility)
{
    Int32 numSharedElements;
    mSharedElements->GetSize(&numSharedElements);
    for (Int32 i = 0; i < numSharedElements; i++) {
        AutoPtr<IInterface> obj;
        mSharedElements->Get(i, (IInterface**)&obj);
        IView* view = IView::Probe(obj);
        assert(0 && "TODO");
        // AutoPtr<GhostView> ghostView = GhostView::GetGhost(view);
        // if (ghostView != NULL) {
        //     ghostView->SetVisibility(visibility);
        // }
    }
    return NOERROR;
}

ECode ActivityTransitionCoordinator::ScheduleGhostVisibilityChange(
    /* [in] */ Int32 visibility)
{
    AutoPtr<IViewGroup> decor;
    GetDecor((IViewGroup**)&decor);
    if (decor != NULL) {
        IView* decorView = IView::Probe(decor);
        AutoPtr<IViewTreeObserver> vto;
        decorView->GetViewTreeObserver((IViewTreeObserver**)&vto);

        AutoPtr<IOnPreDrawListener> listener = new GhostVisibilityOnPreDrawListener(this, decorView, visibility);
        vto->AddOnPreDrawListener(listener);
    }
    return NOERROR;
}

Int32 ActivityTransitionCoordinator::ScaleTypeToInt(
    /* [in] */ ImageViewScaleType scaleType)
{
    for (Int32 i = 0; i < SCALE_TYPE_VALUES->GetLength(); i++) {
        if (scaleType == (*SCALE_TYPE_VALUES)[i]) {
            return i;
        }
    }
    return -1;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
