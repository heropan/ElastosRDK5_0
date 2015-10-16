
#include "elastos/droid/view/ViewTreeObserver.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/graphics/CRegion.h"

using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CRegion;

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(ViewTreeObserver::InternalInsetsInfo, IInternalInsetsInfo);

ViewTreeObserver::InternalInsetsInfo::InternalInsetsInfo()
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mContentInsets));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mVisibleInsets));
    ASSERT_SUCCEEDED(CRegion::New((IRegion**)&mTouchableRegion));
}

ECode ViewTreeObserver::InternalInsetsInfo::SetTouchableInsets(
    /* [in] */ Int32 val)
{
    mTouchableInsets = val;
    return NOERROR;
}

ECode ViewTreeObserver::InternalInsetsInfo::GetTouchableInsets(
    /* [out] */ Int32* val)
{
    assert(val != NULL);
    *val = mTouchableInsets;
    return NOERROR;
}

void ViewTreeObserver::InternalInsetsInfo::Reset()
{
    mContentInsets->SetEmpty();
    mVisibleInsets->SetEmpty();
    mTouchableRegion->SetEmpty();
    mTouchableInsets = TOUCHABLE_INSETS_FRAME;
}

ECode ViewTreeObserver::InternalInsetsInfo::Equals(
    /* [in] */ IInternalInsetsInfo* otherObj,
    /* [out] */ Boolean* equal)
{
    VALIDATE_NOT_NULL(equal);
    *equal = FALSE;

    if (otherObj == NULL) {
        return NOERROR;
    }

    InternalInsetsInfo* other = (InternalInsetsInfo*)otherObj;

    Boolean isEqual;
    if (mContentInsets->Equals(other->mContentInsets, &isEqual), !isEqual) {
        return NOERROR;
    }

    if (mVisibleInsets->Equals(other->mVisibleInsets, &isEqual), !isEqual) {
        return NOERROR;
    }

    if (mTouchableRegion->Equals(other->mTouchableRegion, &isEqual), !isEqual) {
        return NOERROR;
    }

    *equal = mTouchableInsets == other->mTouchableInsets;
    return NOERROR;
}

ECode ViewTreeObserver::InternalInsetsInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);

    return Equals(IInternalInsetsInfo::Probe(other), result);
}

ECode ViewTreeObserver::InternalInsetsInfo::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);

    Int32 result = 0;
    if (mContentInsets != NULL) {
        mContentInsets->GetHashCode(&result);
    }

    Int32 temp = 0;
    if (mVisibleInsets != NULL) {
        mVisibleInsets->GetHashCode(&temp);
    }
    result = 31 * result + temp;

    temp = 0;
    if (mTouchableRegion != NULL) {
        mTouchableRegion->GetHashCode(&temp);
    }
    result = 31 * result + temp;
    result = 31 * result + mTouchableInsets;
    *hash = result;
    return NOERROR;
}

ECode ViewTreeObserver::InternalInsetsInfo::GetContentInsets(
    /* [out] */ IRect** contentInsets)
{
    VALIDATE_NOT_NULL(contentInsets);

    *contentInsets = mContentInsets;
    REFCOUNT_ADD(*contentInsets);
    return NOERROR;
}

ECode ViewTreeObserver::InternalInsetsInfo::GetVisibleInsets(
    /* [out] */ IRect** visibleInsets)
{
    VALIDATE_NOT_NULL(visibleInsets);

    *visibleInsets = mVisibleInsets;
    REFCOUNT_ADD(*visibleInsets);
    return NOERROR;
}

ECode ViewTreeObserver::InternalInsetsInfo::GetTouchableRegion(
    /* [out] */ IRegion** touchableRegion)
{
    VALIDATE_NOT_NULL(touchableRegion);
    *touchableRegion = mTouchableRegion;
    REFCOUNT_ADD(*touchableRegion);
    return NOERROR;
}

void ViewTreeObserver::InternalInsetsInfo::Set(
    /* [in] */ InternalInsetsInfo* other)
{
    mContentInsets->Set(other->mContentInsets);
    mVisibleInsets->Set(other->mVisibleInsets);
    Boolean res;
    mTouchableRegion->Set(other->mTouchableRegion, &res);
    mTouchableInsets = other->mTouchableInsets;
}

ViewTreeObserver::ViewTreeObserver()
    : mAlive(TRUE)
{}

PInterface ViewTreeObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IViewTreeObserver*)this;
    }
    else if (riid == EIID_IViewTreeObserver) {
        return (IViewTreeObserver*)this;
    }

    return NULL;
}

UInt32 ViewTreeObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewTreeObserver::Release()
{
    return ElRefBase::Release();
}

ECode ViewTreeObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnScrollChangedListener(
    /* [in] */ IOnScrollChangedListener* listener)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnScrollChangedListener(
    /* [in] */ IOnScrollChangedListener* victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnTouchModeChangeListener(
    /* [in] */ IOnTouchModeChangeListener* listener)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnTouchModeChangeListener(
    /* [in] */ IOnTouchModeChangeListener* victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnComputeInternalInsetsListener(
    /* [in] */ IOnComputeInternalInsetsListener* listener)
{
    CheckIsAlive();

    // if (mOnComputeInternalInsetsListeners == NULL) {
    //     mOnComputeInternalInsetsListeners =
    //             new CopyOnWriteArrayList<OnComputeInternalInsetsListener>();
    // }

    mOnComputeInternalInsetsListeners.PushBack(listener);
    return NOERROR;
}

ECode ViewTreeObserver::RemoveOnComputeInternalInsetsListener(
    /* [in] */ IOnComputeInternalInsetsListener* victim)
{
    CheckIsAlive();

    mOnComputeInternalInsetsListeners.Remove(victim);
    return NOERROR;
}

ECode ViewTreeObserver::Merge(
    /* [in] */ IViewTreeObserver* observerObj)
{
    ViewTreeObserver* observer = (ViewTreeObserver*)observerObj;

    // if (observer.mOnGlobalFocusListeners != NULL) {
    //     if (mOnGlobalFocusListeners != NULL) {
    //         mOnGlobalFocusListeners.addAll(observer.mOnGlobalFocusListeners);
    //     } else {
    //         mOnGlobalFocusListeners = observer.mOnGlobalFocusListeners;
    //     }
    // }

    if (!observer->mOnGlobalLayoutListeners.IsEmpty()) {
        // if (mOnGlobalLayoutListeners != NULL) {
        //     mOnGlobalLayoutListeners.addAll(observer.mOnGlobalLayoutListeners);
        // } else {
        //     mOnGlobalLayoutListeners = observer.mOnGlobalLayoutListeners;
        // }
        List<AutoPtr<IOnGlobalLayoutListener> >::Iterator iter = observer->mOnGlobalLayoutListeners.Begin();
        for (; iter != observer->mOnGlobalLayoutListeners.End(); ++iter) {
            AutoPtr<IOnGlobalLayoutListener> item = (AutoPtr<IOnGlobalLayoutListener>)(*iter);

            mOnGlobalLayoutListeners.PushBack(item);
        }
    }

    if (!observer->mOnPreDrawListeners.IsEmpty()) {
        // if (mOnPreDrawListeners != NULL) {
        //     //mOnPreDrawListeners.addall(observer->mOnPreDrawListeners);
        // } else {
        //     mOnPreDrawListeners = observer->mOnPreDrawListeners;
        // }
        List<AutoPtr<IOnPreDrawListener> >::Iterator iter = observer->mOnPreDrawListeners.Begin();
        for (; iter != observer->mOnPreDrawListeners.End(); ++iter) {
            AutoPtr<IOnPreDrawListener> item = (AutoPtr<IOnPreDrawListener>)(*iter);

            mOnPreDrawListeners.PushBack(item);
        }
    }

    // if (observer.mOnTouchModeChangeListeners != NULL) {
    //     if (mOnTouchModeChangeListeners != NULL) {
    //         mOnTouchModeChangeListeners.addAll(observer.mOnTouchModeChangeListeners);
    //     } else {
    //         mOnTouchModeChangeListeners = observer.mOnTouchModeChangeListeners;
    //     }
    // }

    // if (observer.mOnComputeInternalInsetsListeners != NULL) {
    //     if (mOnComputeInternalInsetsListeners != NULL) {
    //         mOnComputeInternalInsetsListeners.addAll(observer.mOnComputeInternalInsetsListeners);
    //     } else {
    //         mOnComputeInternalInsetsListeners = observer.mOnComputeInternalInsetsListeners;
    //     }
    // }

    observer->Kill();

    return NOERROR;
}

ECode ViewTreeObserver::AddOnGlobalFocusChangeListener(
    /* [in] */ IOnGlobalFocusChangeListener* listener)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::RemoveOnGlobalFocusChangeListener(
    /* [in] */ IOnGlobalFocusChangeListener* victim)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::AddOnGlobalLayoutListener(
    /* [in] */ IOnGlobalLayoutListener* listener)
{
    assert(listener != NULL);
    CheckIsAlive();

    mOnGlobalLayoutListeners.PushBack(listener);
    return NOERROR;
}

ECode ViewTreeObserver::RemoveGlobalOnLayoutListener(
    /* [in] */ IOnGlobalLayoutListener*  victim)
{
    CheckIsAlive();

    mOnGlobalLayoutListeners.Remove(victim);
    return NOERROR;
}

ECode ViewTreeObserver:: CheckIsAlive()
{
    if (!mAlive) {
        // throw new IllegalStateException("This ViewTreeObserver is not alive, call "
        //         + "getViewTreeObserver() again");
        // return E_ILLEGAL_STATE_EXCEPTION;
        assert(0);
    }
    return NOERROR;
}

ECode ViewTreeObserver::IsAlive(
    /* [out] */ Boolean* alive)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

void ViewTreeObserver::Kill()
{
    mAlive = FALSE;
}

ECode ViewTreeObserver::DispatchOnGlobalFocusChange(
    /* [in] */ IView* oldFocus,
    /* [in] */ IView* newFocus)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnGlobalLayout()
{
    // NOTE: because of the use of CopyOnWriteArrayList, we *must* use an iterator to
    // perform the dispatching. The iterator is a safe guard against listeners that
    // could mutate the list by calling the various add/remove methods. This prevents
    // the array from being modified while we iterate it.

    // // final CopyOnWriteArray<OnGlobalLayoutListener> listeners = mOnGlobalLayoutListeners;
    // // if (listeners != null && listeners.size() > 0) {
    // //     CopyOnWriteArray.Access<OnGlobalLayoutListener> access = listeners.start();
    // //     try {
    // //         int count = access.size();
    // //         for (int i = 0; i < count; i++) {
    // //             access.get(i).onGlobalLayout();
    // //         }
    // //     } finally {
    // //         listeners.end();
    // //     }
    // // }
    if (!mOnGlobalLayoutListeners.IsEmpty()) {
        List<AutoPtr<IOnGlobalLayoutListener> > access(mOnGlobalLayoutListeners);
        List<AutoPtr<IOnGlobalLayoutListener> >::Iterator iter = access.Begin();
        for (; iter != access.End(); ++iter) {
            (*iter)->OnGlobalLayout();
        }
    }
    return NOERROR;
}

ECode ViewTreeObserver::DispatchOnPreDraw(
    /* [out] */ Boolean* cancelDraw)
{
    VALIDATE_NOT_NULL(cancelDraw);
    *cancelDraw = FALSE;

    CheckIsAlive();

    ///final CopyOnWriteArrayList<OnPreDrawListener> listeners = mOnPreDrawListeners;
    if (!mOnPreDrawListeners.IsEmpty()) {
        // for (OnPreDrawListener listener : listeners) {
        //     *cancelDraw |= !listener.onPreDraw();
        // }

        List<AutoPtr<IOnPreDrawListener> > access(mOnPreDrawListeners);
        Boolean isPreDraw;
        List<AutoPtr<IOnPreDrawListener> >::Iterator iter = access.Begin();
        for (; iter != access.End(); ++iter) {
            isPreDraw = FALSE;
            assert((*iter) != NULL);
            (*iter)->OnPreDraw(&isPreDraw);
            *cancelDraw |= !isPreDraw;
        }
    }

    return NOERROR;
}

ECode ViewTreeObserver::DispatchOnDraw()
{
    List<AutoPtr<IOnDrawListener> >::Iterator iter = mOnDrawListeners.Begin();
    for (; iter != mOnDrawListeners.End(); ++iter) {
        assert((*iter) != NULL);
        (*iter)->OnDraw();
    }

    return NOERROR;
}

ECode ViewTreeObserver::DispatchOnTouchModeChanged(
    /* [in] */ Boolean inTouchMode)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnScrollChanged()
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::HasComputeInternalInsetsListeners(
    /* [out] */ Boolean* has)
{
    CheckIsAlive();
    return E_NOT_IMPLEMENTED;
}

ECode ViewTreeObserver::DispatchOnComputeInternalInsets(
    /* [in] */ IInternalInsetsInfo* inoutInfo)
{
    // NOTE: because of the use of CopyOnWriteArrayList, we *must* use an iterator to
    // perform the dispatching. The iterator is a safe guard against listeners that
    // could mutate the list by calling the various add/remove methods. This prevents
    // the array from being modified while we iterate it.

    List<AutoPtr<IOnComputeInternalInsetsListener> >::Iterator iter = mOnComputeInternalInsetsListeners.Begin();
    for (; iter != mOnComputeInternalInsetsListeners.End(); ++iter) {
        AutoPtr<IOnComputeInternalInsetsListener> listener = (AutoPtr<IOnComputeInternalInsetsListener>)(*iter);

        listener->OnComputeInternalInsets(inoutInfo);
    }

    return NOERROR;
}

ECode ViewTreeObserver::AddOnPreDrawListener(
    /* [in] */ IOnPreDrawListener* listener)
{
    assert(listener != NULL);
    CheckIsAlive();
    mOnPreDrawListeners.PushBack(listener);

    return NOERROR;
}

ECode ViewTreeObserver::RemoveOnPreDrawListener(
    /* [in] */ IOnPreDrawListener* victim)
{
    CheckIsAlive();
    mOnPreDrawListeners.Remove(victim);

    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
