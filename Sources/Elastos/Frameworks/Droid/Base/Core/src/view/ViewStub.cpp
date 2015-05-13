
#include "view/ViewStub.h"
#include "view/LayoutInflater.h"
#include "R.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace View {

ViewStub::ViewStub()
    : mLayoutResource(0)
    , mInflatedId(0)
{
}

ECode ViewStub::InitViewStub(
    /* [in] */ IContext* context)
{
    Initialize(context);
    return NOERROR;
}

ECode ViewStub::InitViewStub(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutResource)
{
    mLayoutResource = layoutResource;
    Initialize(context);
    return NOERROR;
}

ECode ViewStub::InitViewStub(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::ViewStub),
        ARRAY_SIZE(R::styleable::ViewStub));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetResourceId(R::styleable::ViewStub_inflatedId, IView::NO_ID, &mInflatedId);
    a->GetResourceId(R::styleable::ViewStub_layout, 0, &mLayoutResource);

    a->Recycle();

    a = NULL;
    attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::View),
            ARRAY_SIZE(R::styleable::View));
    context->ObtainStyledAttributesEx3(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetResourceId(R::styleable::View_id, IView::NO_ID, &mID);
    a->Recycle();

    Initialize(context);
    return NOERROR;
}

void ViewStub::Initialize(
    /* [in] */ IContext* context)
{
    mContext = context;
    SetVisibility(IView::GONE);
    SetWillNotDraw(TRUE);
}

ECode ViewStub::GetInflatedId(
    /* [out] */ Int32* id)
{
    assert(id != NULL);
    *id = mInflatedId;
    return NOERROR;
}

ECode ViewStub::SetInflatedId(
    /* [in] */ Int32 inflatedId)
{
    mInflatedId = inflatedId;
    return NOERROR;
}

ECode ViewStub::GetLayoutResource(
    /* [out] */ Int32* resource)
{
    assert(resource != NULL);
    *resource = mLayoutResource;
    return NOERROR;
}

ECode ViewStub::SetLayoutResource(
    /* [in] */ Int32 layoutResource)
{
    mLayoutResource = layoutResource;
    return NOERROR;
}

ECode ViewStub::SetLayoutInflater(
    /* [in] */ ILayoutInflater* inflater)
{
    mInflater = inflater;
    return NOERROR;
}

ECode ViewStub::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    assert(inflater != NULL);
    *inflater = mInflater;
    INTERFACE_ADDREF(*inflater);
    return NOERROR;
}

void ViewStub::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    SetMeasuredDimension(0, 0);
}

ECode ViewStub::Draw(
    /* [in] */ ICanvas* canvas)
{
    return NOERROR;
}

void ViewStub::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
}

ECode ViewStub::SetVisibility(
    /* [in] */ Int32 visibility)
{
    if (mInflatedViewRef != NULL) {
        AutoPtr<IView> view;
        mInflatedViewRef->Resolve(EIID_IView, (IInterface**)&view);
        if (view != NULL) {
            view->SetVisibility(visibility);
        }
        else {
            Slogger::E("ViewStub", "setVisibility called on un-referenced view");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    } else {
        View::SetVisibility(visibility);
        if (visibility == IView::VISIBLE || visibility == IView::INVISIBLE) {
            AutoPtr<IView> tmp;
            Inflate((IView**)&tmp);
        }
    }

    return NOERROR;
}

ECode ViewStub::Inflate(
    /* [out] */ IView** retView)
{
    assert(retView != NULL);
    AutoPtr<IViewParent> viewParent = GetParent();

    if (viewParent != NULL && IViewGroup::Probe(viewParent) != NULL) {
        if (mLayoutResource != 0) {
            AutoPtr<IViewGroup> parent = IViewGroup::Probe(viewParent);
            AutoPtr<ILayoutInflater> factory;
            if (mInflater != NULL) {
                factory = mInflater;
            } else {
                LayoutInflater::From(mContext, (ILayoutInflater**)&factory);
            }

            AutoPtr<IView> view;
            factory->InflateEx2(mLayoutResource, parent,
                    FALSE, (IView**)&view);

            if (mInflatedId != IView::NO_ID) {
                view->SetId(mInflatedId);
            }

            Int32 index = 0;
            parent->IndexOfChild((IView*)this->Probe(EIID_IView), &index);
            parent->RemoveViewInLayout((IView*)this->Probe(EIID_IView));

            AutoPtr<IViewGroupLayoutParams> layoutParams = GetLayoutParams();
            if (layoutParams != NULL) {
                parent->AddViewEx4(view, index, layoutParams);
            } else {
                parent->AddViewEx(view, index);
            }

            IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(view);
            assert(wrs != NULL);
            mInflatedViewRef = NULL;
            wrs->GetWeakReference((IWeakReference**)&mInflatedViewRef);

            if (mInflateListener != NULL) {
                mInflateListener->OnInflate(THIS_PROBE(IViewStub), view);
            }

            *retView = view;
            INTERFACE_ADDREF(*retView);
            return NOERROR;
        }
        else {
            Slogger::E("ViewStub", "ViewStub must have a valid layoutResource");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }
    else {
        Slogger::E("ViewStub", "ViewStub must have a non-null ViewGroup viewParent");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

ECode ViewStub::SetOnInflateListener(
    /* [in] */ IOnInflateListener* inflateListener)
{
    mInflateListener = inflateListener;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
