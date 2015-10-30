
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/transition/CChangeBounds.h"
#include "elastos/droid/transition/CTransitionValues.h"
#include "elastos/droid/transition/CTransitionUtils.h"
#include "elastos/droid/animation/RectEvaluator.h"
#include "elastos/droid/animation/ObjectAnimator.h"
#include "elastos/droid/animation/PropertyValuesHolder.h"
#include "elastos/droid/graphics/CRect.h"
//#include "elastos/droid/graphics/CBitmap.h"
//#include "elastos/droid/view/View.h"

#include <elastos/core/Math.h>

using Elastos::Droid::Animation::RectEvaluator;
using Elastos::Droid::Animation::ObjectAnimator;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::IPropertyValuesHolder;
using Elastos::Droid::Animation::PropertyValuesHolder;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::ICanvas;
//using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::ECLSID_CPointF;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Transition::CTransitionValues;
using Elastos::Droid::Transition::CTransitionUtils;
using Elastos::Droid::Utility::EIID_IProperty;
using Elastos::Droid::View::IViewParent;
//using Elastos::Droid::View::View;
using Elastos::Droid::View::IViewOverlay;

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::CInteger32;

namespace Elastos {
namespace Droid {
namespace Transition {

//===============================================================
// CChangeBounds::
//===============================================================
String CChangeBounds::PROPNAME_BOUNDS = String("android:changeBounds:bounds");
String CChangeBounds::PROPNAME_PARENT = String("android:changeBounds:parent");
String CChangeBounds::PROPNAME_WINDOW_X = String("android:changeBounds:windowX");
String CChangeBounds::PROPNAME_WINDOW_Y = String("android:changeBounds:windowY");
AutoPtr<ArrayOf<String> > CChangeBounds::sTransitionProperties = ArrayOf<String>::Alloc(4);

AutoPtr<IProperty> CChangeBounds::DRAWABLE_ORIGIN_PROPERTY = new PointFProperty(String("boundsOrigin"));

//String CChangeBounds::LOG_TAG = "ChangeBounds";

AutoPtr<ITypeEvaluator> CChangeBounds::sRectEvaluator;// = new RectEvaluator();

CAR_OBJECT_IMPL(CChangeBounds)

CAR_INTERFACE_IMPL(CChangeBounds, Transition, IChangeBounds)

CChangeBounds::CChangeBounds()
{
    (*sTransitionProperties)[0] = PROPNAME_BOUNDS;
    (*sTransitionProperties)[1] = PROPNAME_PARENT;
    (*sTransitionProperties)[2] = PROPNAME_WINDOW_X;
    (*sTransitionProperties)[3] = PROPNAME_WINDOW_Y;
}

ECode CChangeBounds::constructor()
{
    return NOERROR;
}

ECode CChangeBounds::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Transition::constructor(context, attrs);
    return NOERROR;
}

ECode CChangeBounds::GetTransitionProperties(
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result)

    *result = sTransitionProperties;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CChangeBounds::SetResizeClip(
    /* [in] */ Boolean resizeClip)
{
    mResizeClip = resizeClip;
    return NOERROR;
}

ECode CChangeBounds::SetReparent(
    /* [in] */ Boolean reparent)
{
    mReparent = reparent;
    return NOERROR;
}

void CChangeBounds::CaptureValues(
    /* [in] */ ITransitionValues* values)
{
    AutoPtr<CTransitionValues> cVals = (CTransitionValues*) values;
    AutoPtr<IView> view = cVals->mView;

    Boolean bIsLaidOut = FALSE;
    view->IsLaidOut(&bIsLaidOut);
    Int32 w = 0, h = 0;
    view->GetWidth(&w);
    view->GetHeight(&h);
    if (bIsLaidOut || w != 0 || h != 0) {
        Int32 l = 0, t = 0, r = 0, b = 0;
        view->GetLeft(&l);
        view->GetTop(&t);
        view->GetRight(&r);
        view->GetBottom(&b);
        AutoPtr<IRect> rt;
        CRect::New(l, t, r, b, (IRect**)&rt);
        AutoPtr<ICharSequence> pro_bounds;
        CString::New(PROPNAME_BOUNDS, (ICharSequence**)&pro_bounds);
        cVals->mValues->Put(pro_bounds, rt);
        AutoPtr<IViewParent> p;
        cVals->mView->GetParent((IViewParent**)&p);
        AutoPtr<ICharSequence> pro_parent;
        CString::New(PROPNAME_PARENT, (ICharSequence**)&pro_parent);
        cVals->mValues->Put(pro_parent, p);
        if (mReparent) {
            Int32 x = 0, y = 0;
            cVals->mView->GetLocationInWindow(&x, &y);
            AutoPtr<IInteger32> pX, pY;
            CInteger32::New(x, (IInteger32**)&pX);
            CInteger32::New(y, (IInteger32**)&pY);
            AutoPtr<ICharSequence> pro_x;
            CString::New(PROPNAME_WINDOW_X, (ICharSequence**)&pro_x);
            AutoPtr<ICharSequence> pro_y;
            CString::New(PROPNAME_WINDOW_Y, (ICharSequence**)&pro_y);
            cVals->mValues->Put(pro_x, pX);
            cVals->mValues->Put(pro_y, pY);
        }
    }
}

ECode CChangeBounds::CaptureStartValues(
    /* [in] */ ITransitionValues* transitionValues)
{
    CaptureValues(transitionValues);
    return NOERROR;
}

ECode CChangeBounds::CaptureEndValues(
    /* [in] */ ITransitionValues* transitionValues)
{
    CaptureValues(transitionValues);
    return NOERROR;
}

Boolean CChangeBounds::ParentMatches(
    /* [in] */ IView* startParent,
    /* [in] */ IView* endParent)
{
    Boolean parentMatches = TRUE;
    if (mReparent) {
        AutoPtr<ITransitionValues> endValues;
        GetMatchedTransitionValues(startParent, TRUE, (ITransitionValues**)&endValues);
        if (endValues == NULL) {
            parentMatches = Object::Equals(startParent->Probe(EIID_IInterface), endParent->Probe(EIID_IInterface));
        }
        else {
            assert(0 && "TODO");
        //    AutoPtr<View> cEndValues = (View*)endValues;
        //    parentMatches = Object::Equals(endParent->Probe(EIID_IInterface), cEndValues->mView->Probe(EIID_IInterface));
        }
    }
    return parentMatches;
}

ECode CChangeBounds::CreateAnimator(
    /* [in] */ IViewGroup* sceneRoot,
    /* [in] */ ITransitionValues* startValues,
    /* [in] */ ITransitionValues* endValues,
    /* [out] */ IAnimator** result)
{
    VALIDATE_NOT_NULL(result)

    if (startValues == NULL || endValues == NULL) {
        *result = NULL;
        return NOERROR;
    }
    AutoPtr<CTransitionValues> cStartV = (CTransitionValues*)startValues;
    AutoPtr<CTransitionValues> cEndV = (CTransitionValues*)endValues;
    AutoPtr<IMap> startParentVals = cStartV->mValues;
    AutoPtr<IMap> endParentVals = cEndV->mValues;
    AutoPtr<ICharSequence> pro_parent;
    CString::New(PROPNAME_PARENT, (ICharSequence**)&pro_parent);
    AutoPtr<IInterface> sp, ep;
    startParentVals->Get(pro_parent, (IInterface**)&sp);
    endParentVals->Get(pro_parent, (IInterface**)&ep);
    AutoPtr<IViewGroup> startParent = IViewGroup::Probe(sp);
    AutoPtr<IViewGroup> endParent = IViewGroup::Probe(ep);
    if (startParent == NULL || endParent == NULL) {
        *result = NULL;
        return NOERROR;
    }
    AutoPtr<IView> view = cEndV->mView;
    if (ParentMatches(IView::Probe(startParent), IView::Probe(endParent))) {
        AutoPtr<ICharSequence> pro_bounds;
        CString::New(PROPNAME_BOUNDS, (ICharSequence**)&pro_bounds);
        AutoPtr<IInterface> sB, eB;
        cStartV->mValues->Get(pro_bounds, (IInterface**)&sB);
        cEndV->mValues->Get(pro_bounds, (IInterface**)&eB);
        AutoPtr<CRect> startBounds = (CRect*)IRect::Probe(sB);
        AutoPtr<CRect> endBounds = (CRect*)IRect::Probe(eB);
        Int32 startLeft = startBounds->mLeft;
        Int32 endLeft = endBounds->mLeft;
        Int32 startTop = startBounds->mTop;
        Int32 endTop = endBounds->mTop;
        Int32 startRight = startBounds->mRight;
        Int32 endRight = endBounds->mRight;
        Int32 startBottom = startBounds->mBottom;
        Int32 endBottom = endBounds->mBottom;
        Int32 startWidth = startRight - startLeft;
        Int32 startHeight = startBottom - startTop;
        Int32 endWidth = endRight - endLeft;
        Int32 endHeight = endBottom - endTop;
        Int32 numChanges = 0;
        if ((startWidth != 0 && startHeight != 0) || (endWidth != 0 && endHeight != 0)) {
            if (startLeft != endLeft || startTop != endTop) ++numChanges;
            if (startRight != endRight || startBottom != endBottom) ++numChanges;
        }
        if (numChanges > 0) {
            if (!mResizeClip) {
                AutoPtr<IAnimator> anim;
                if (startWidth == endWidth && startHeight == endHeight) {
                    Int32 l = 0, t = 0;
                    view->GetLeft(&l);
                    view->GetTop(&t);
                    view->OffsetLeftAndRight(startLeft - l);
                    view->OffsetTopAndBottom(startTop - t);
                    AutoPtr<IPathMotion> pM;
                    GetPathMotion((IPathMotion**)&pM);
                    AutoPtr<IPath> positionPath;
                    pM->GetPath(0, 0, endLeft - startLeft,
                            endTop - startTop, (IPath**)&positionPath);
                    AutoPtr<IObjectAnimator> p = ObjectAnimator::OfInt32(view, new HorizontalOffsetProperty(),
                            new VerticalOffsetProperty(), positionPath);
                    anim = IAnimator::Probe(p);
                }
                else {
                    if (startLeft != endLeft) view->SetLeft(startLeft);
                    if (startTop != endTop) view->SetTop(startTop);
                    if (startRight != endRight) view->SetRight(startRight);
                    if (startBottom != endBottom) view->SetBottom(startBottom);
                    AutoPtr<IObjectAnimator> topLeftAnimator;
                    if (startLeft != endLeft || startTop != endTop) {
                        AutoPtr<IPathMotion> pM;
                        GetPathMotion((IPathMotion**)&pM);
                        AutoPtr<IPath> topLeftPath;
                        pM->GetPath(startLeft, startTop,
                                endLeft, endTop, (IPath**)&topLeftPath);
                        topLeftAnimator = ObjectAnimator::OfInt32(IObject::Probe(view), String("left"), String("top"), topLeftPath);
                    }
                    AutoPtr<IObjectAnimator> bottomRightAnimator = NULL;
                    if (startRight != endRight || startBottom != endBottom) {
                        AutoPtr<IPathMotion> pM;
                        GetPathMotion((IPathMotion**)&pM);
                        AutoPtr<IPath> bottomRightPath;
                        pM->GetPath(startRight, startBottom,
                                endRight, endBottom, (IPath**)&bottomRightPath);
                        bottomRightAnimator = ObjectAnimator::OfInt32(IObject::Probe(view), String("right"), String("bottom"), bottomRightPath);
                    }
                    AutoPtr<ITransitionUtils> utils;
                    CTransitionUtils::AcquireSingleton((ITransitionUtils**)&utils);
                    utils->MergeAnimators(IAnimator::Probe(topLeftAnimator), IAnimator::Probe(bottomRightAnimator), (IAnimator**)&anim);
                }
                AutoPtr<IViewParent> vp;
                view->GetParent((IViewParent**)&vp);
                if (IViewGroup::Probe(vp) != NULL) {
                    AutoPtr<IViewParent> vp;
                    view->GetParent((IViewParent**)&vp);
                    AutoPtr<IViewGroup> parent = IViewGroup::Probe(vp);
                    parent->SuppressLayout(TRUE);
                    AutoPtr<ITransitionListener> transitionListener = new TransitionListenerAdapter_1(parent);
                    AddListener(transitionListener);
                }
                *result = anim;
                REFCOUNT_ADD(*result)
                return NOERROR;
            }
            else {
                if (startWidth != endWidth) view->SetRight(endLeft +
                        Elastos::Core::Math::Max(startWidth, endWidth));
                if (startHeight != endHeight) view->SetBottom(endTop +
                        Elastos::Core::Math::Max(startHeight, endHeight));
                // TODO: don't clobber TX/TY
                if (startLeft != endLeft) view->SetTranslationX(startLeft - endLeft);
                if (startTop != endTop) view->SetTranslationY(startTop - endTop);
                // Animate location with translationX/Y and size with clip bounds
                Float transXDelta = endLeft - startLeft;
                Float transYDelta = endTop - startTop;
                Int32 widthDelta = endWidth - startWidth;
                Int32 heightDelta = endHeight - startHeight;
                numChanges = 0;
                if (transXDelta != 0) numChanges++;
                if (transYDelta != 0) numChanges++;
                if (widthDelta != 0 || heightDelta != 0) numChanges++;
                AutoPtr<IObjectAnimator> translationAnimator;
                if (transXDelta != 0 || transYDelta != 0) {
                    AutoPtr<IPathMotion> pM;
                    GetPathMotion((IPathMotion**)&pM);
                    AutoPtr<IPath> bottomRightPath;
                    pM->GetPath(0, 0, transXDelta, transYDelta, (IPath**)&bottomRightPath);
                    assert(0 && "TODO");
                    // translationAnimator = ObjectAnimator::OfFloat(view, View::TRANSLATION_X,
                    //         View::TRANSLATION_Y, topLeftPath);
                }
                AutoPtr<IObjectAnimator> clipAnimator;
                if (widthDelta != 0 || heightDelta != 0) {
                    AutoPtr<IRect> tempStartBounds;
                    CRect::New(0, 0, startWidth, startHeight, (IRect**)&tempStartBounds);
                    AutoPtr<IRect> tempEndBounds;
                    CRect::New(0, 0, endWidth, endHeight, (IRect**)&tempStartBounds);
                    assert(0 && "TODO");
                    // clipAnimator = ObjectAnimator::OfObject(view, "clipBounds", sRectEvaluator,
                    //         tempStartBounds, tempEndBounds);
                }
                AutoPtr<ITransitionUtils> utils;
                CTransitionUtils::AcquireSingleton((ITransitionUtils**)&utils);
                AutoPtr<IAnimator> anim;
                utils->MergeAnimators(IAnimator::Probe(translationAnimator), IAnimator::Probe(clipAnimator), (IAnimator**)&anim);
                AutoPtr<IViewParent> vp;
                view->GetParent((IViewParent**)&vp);
                if (IViewGroup::Probe(vp) != NULL) {
                    AutoPtr<IViewGroup> parent = IViewGroup::Probe(vp);
                    parent->SuppressLayout(TRUE);
                    AutoPtr<ITransitionListener> transitionListener = new TransitionListenerAdapter_1(parent);
                    AddListener(transitionListener);
                }
                anim->AddListener(new AnimatorListenerAdapter_2(view));
                *result = anim;
                REFCOUNT_ADD(*result)
                return NOERROR;
            }
        }
    }
    else {
        AutoPtr<IInterface> sX, sY, eX, eY;
        AutoPtr<ICharSequence> pro_x;
        CString::New(PROPNAME_WINDOW_X, (ICharSequence**)&pro_x);
        AutoPtr<ICharSequence> pro_y;
        CString::New(PROPNAME_WINDOW_Y, (ICharSequence**)&pro_y);
        cStartV->mValues->Get(pro_x, (IInterface**)&sX);
        cStartV->mValues->Get(pro_y, (IInterface**)&sY);
        cEndV->mValues->Get(pro_x, (IInterface**)&eX);
        cEndV->mValues->Get(pro_y, (IInterface**)&eY);
        Int32 startX = 0, startY = 0, endX = 0, endY = 0;
        IInteger32::Probe(sX)->GetValue(&startX);
        IInteger32::Probe(sY)->GetValue(&startY);
        IInteger32::Probe(eX)->GetValue(&endX);
        IInteger32::Probe(eY)->GetValue(&endY);
        // TODO: also handle size changes: check bounds and animate size changes
        if (startX != endX || startY != endY) {
            Int32 n0 = 0, n1 = 0;
            IView::Probe(sceneRoot)->GetLocationInWindow(&n0, &n1);
            (*mTempLocation)[0] = n0;
            (*mTempLocation)[1] = n1;
            Int32 w = 0, h = 0;
            view->GetWidth(&w);
            view->GetHeight(&h);
            assert(0 && "TODO");
            AutoPtr<IBitmap> bitmap;// = CBitmap::CreateBitmap(w, h, Bitmap::Config::ARGB_8888);
            AutoPtr<ICanvas> canvas;// = new Canvas(bitmap);
            view->Draw(canvas);
            AutoPtr<IBitmapDrawable> drawable;// = new BitmapDrawable(bitmap);
            Float transitionAlpha;
            view->GetTransitionAlpha(&transitionAlpha);
            view->SetTransitionAlpha(0);
            AutoPtr<IViewOverlay> ol;
            IView::Probe(sceneRoot)->GetOverlay((IViewOverlay**)&ol);
            ol->Add(IDrawable::Probe(drawable));
            AutoPtr<IPathMotion> pM;
            GetPathMotion((IPathMotion**)&pM);
            AutoPtr<IPath> topLeftPath;
            pM->GetPath(startX - (*mTempLocation)[0],
                    startY - (*mTempLocation)[1], endX - (*mTempLocation)[0], endY - (*mTempLocation)[1], (IPath**)&topLeftPath);
            AutoPtr<IPropertyValuesHolder> origin = PropertyValuesHolder::OfObject(
                    DRAWABLE_ORIGIN_PROPERTY, NULL, topLeftPath);
            AutoPtr<ArrayOf<IPropertyValuesHolder*> > arr = ArrayOf<IPropertyValuesHolder*>::Alloc(1);
            (*arr)[0] = origin;
            AutoPtr<IObjectAnimator> anim = ObjectAnimator::OfPropertyValuesHolder(drawable, arr);
            AutoPtr<IAnimatorListener> p = new AnimatorListenerAdapter_1(sceneRoot, drawable, view, transitionAlpha);
            IAnimator::Probe(anim)->AddListener(p);
            *result = IAnimator::Probe(anim);
            REFCOUNT_ADD(*result)
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

//===============================================================
// CChangeBounds::OffsetProperty::
//===============================================================

CChangeBounds::OffsetProperty::OffsetProperty(
    /* [in] */ const String& name) : Int32Property(name)
{
}

ECode CChangeBounds::OffsetProperty::SetValue(
    /* [in] */ IInterface* view,
    /* [in] */ Int32 value)
{
    Int32 offset = value - mPreviousValue;
    OffsetBy(IView::Probe(view), offset);
    mPreviousValue = value;
    return NOERROR;
}

ECode CChangeBounds::OffsetProperty::Get(
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** rst)
{
    *rst = NULL;
    return NOERROR;
}

//===============================================================
// CChangeBounds::HorizontalOffsetProperty::
//===============================================================

CChangeBounds::HorizontalOffsetProperty::HorizontalOffsetProperty()
    : OffsetProperty(String("offsetLeftAndRight"))
{
}

void CChangeBounds::HorizontalOffsetProperty::OffsetBy(
    /* [in] */ IView* view,
    /* [in] */ Int32 by)
{
    view->OffsetLeftAndRight(by);
}

//===============================================================
// CChangeBounds::VerticalOffsetProperty::
//===============================================================

CChangeBounds::VerticalOffsetProperty::VerticalOffsetProperty()
    : OffsetProperty(String("offsetTopAndBottom"))
{
}

void CChangeBounds::VerticalOffsetProperty::OffsetBy(
    /* [in] */ IView* view,
    /* [in] */ Int32 by)
{
    view->OffsetTopAndBottom(by);
}

//===============================================================
// CChangeBounds::PointFProperty::
//===============================================================
CAR_INTERFACE_IMPL(CChangeBounds::PointFProperty, Object, IProperty)

CChangeBounds::PointFProperty::PointFProperty(
    /* [in] */ const String& str)
{
    CRect::New((IRect**)&mBounds);
    mName = str;
}

ECode CChangeBounds::PointFProperty::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    VALIDATE_NOT_NULL(readOnly)
    *readOnly = FALSE;
    return NOERROR;
}

ECode CChangeBounds::PointFProperty::Set(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* value)
{
    AutoPtr<IDrawable> dObj = IDrawable::Probe(obj);
    AutoPtr<IPointF> pVal = IPointF::Probe(value);
    assert(0 && "TODO");
    // AutoPtr<CPointF> cVal = (CPointF*)pVal;
    // dObj->CopyBounds(mBounds);
    // mBounds->OffsetTo(Elastos::Core::Math::Round(cVal->mX), Elastos::Core::Math::Round(cVal->mY));
    dObj->SetBounds(mBounds);
    return NOERROR;
}

ECode CChangeBounds::PointFProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)

    AutoPtr<IDrawable> dObj = IDrawable::Probe(obj);
    dObj->CopyBounds(mBounds);
    AutoPtr<CRect> cBounds = (CRect*)mBounds.Get();
    assert(0 && "TODO");
    // AutoPtr<IPointF> p;
    // CPointF::New(cBounds->mLeft, cBounds->mTop, (IPointF**)&p);
    return NOERROR;
}

ECode CChangeBounds::PointFProperty::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mName;
    return NOERROR;
}

ECode CChangeBounds::PointFProperty::GetType(
    /* [out] */ ClassID* id)
{
    VALIDATE_NOT_NULL(id)
    *id = ECLSID_CPointF;
    return NOERROR;
}

ECode CChangeBounds::PointFProperty::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    VALIDATE_NOT_NULL(writeOnly)
    *writeOnly = FALSE;
    return NOERROR;
}

ECode CChangeBounds::PointFProperty::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
    VALIDATE_NOT_NULL(forbidden)
    *forbidden = FALSE;
    return NOERROR;
}

//===============================================================
// CChangeBounds::TransitionListenerAdapter_1::
//===============================================================
CChangeBounds::TransitionListenerAdapter_1::TransitionListenerAdapter_1(
    /* [in] */ IViewGroup* parent)
{
    mCanceled = FALSE;
    mParent = parent;
}

ECode CChangeBounds::TransitionListenerAdapter_1::OnTransitionCancel(
    /* [in] */ ITransition* transition)
{
    mParent->SuppressLayout(FALSE);
    mCanceled = TRUE;
    return NOERROR;
}

ECode CChangeBounds::TransitionListenerAdapter_1::OnTransitionEnd(
    /* [in] */ ITransition* transition)
{
    if (!mCanceled) {
        mParent->SuppressLayout(FALSE);
    }
    return NOERROR;
}

ECode CChangeBounds::TransitionListenerAdapter_1::OnTransitionPause(
    /* [in] */ ITransition* transition)
{
    mParent->SuppressLayout(FALSE);
    return NOERROR;
}

ECode CChangeBounds::TransitionListenerAdapter_1::OnTransitionResume(
    /* [in] */ ITransition* transition)
{
    mParent->SuppressLayout(TRUE);
    return NOERROR;
}

//===============================================================
// CChangeBounds::AnimatorListenerAdapter_1::
//===============================================================
CChangeBounds::AnimatorListenerAdapter_1::AnimatorListenerAdapter_1(
    /* [in] */ IViewGroup* sceneRoot,
    /* [in] */ IBitmapDrawable* drawable,
    /* [in] */ IView* v,
    /* [in] */ Float alpha)
{
    mSR = sceneRoot;
    mDrawable = drawable;
    mV = v;
    mAlpha = alpha;
}

ECode CChangeBounds::AnimatorListenerAdapter_1::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    AutoPtr<IViewOverlay> ol;
    IView::Probe(mSR)->GetOverlay((IViewOverlay**)&ol);
    ol->Remove(IDrawable::Probe(mDrawable));
    mV->SetTransitionAlpha(mAlpha);
    return NOERROR;
}

//===============================================================
// CChangeBounds::AnimatorListenerAdapter_2::
//===============================================================
CChangeBounds::AnimatorListenerAdapter_2::AnimatorListenerAdapter_2(
    /* [in] */ IView* v)
{
    mV = v;
}

ECode CChangeBounds::AnimatorListenerAdapter_2::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    mV->SetClipBounds(NULL);
    return NOERROR;
}

} // namespace Transition
} // namepsace Droid
} // namespace Elastos
