
#include "view/menu/ExpandedMenuView.h"
#include "view/menu/MenuBuilderBase.h"

using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CAR_INTERFACE_IMPL(ExpandedMenuView::OnItemClickListener, IAdapterViewOnItemClickListener)

ECode ExpandedMenuView::OnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return mOwner->OnItemClick(parent, v, position, id);
}

ExpandedMenuView::ExpandedMenuView()
    : mAnimations(0)
{}

ExpandedMenuView::ExpandedMenuView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mAnimations(0)
{
    Init(context, attrs);
}


ECode ExpandedMenuView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ListView::Init(context, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::MenuView),
        ARRAY_SIZE(R::styleable::MenuView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);

    a->GetResourceId(R::styleable::MenuView_windowAnimationStyle, 0, &mAnimations);
    a->Recycle();

    return SetOnItemClickListener(new OnItemClickListener(this));
}

ECode ExpandedMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu)
{
    mMenu = menu;

    return NOERROR;
}

ECode ExpandedMenuView::OnDetachedFromWindow()
{
    ListView::OnDetachedFromWindow();

    // Clear the cached bitmaps of children
    SetChildrenDrawingCacheEnabled(FALSE);
    return NOERROR;
}

Boolean ExpandedMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item)
{
    Boolean state;
    mMenu->PerformItemAction(item, 0, &state);
    return state;
}

ECode ExpandedMenuView::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<IInterface> item;
    GetAdapter()->GetItem(position, (IInterface**)&item);
    InvokeItem(IMenuItemImpl::Probe(item));
    return NOERROR;
}

Int32 ExpandedMenuView::GetWindowAnimations()
{
    return  mAnimations;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
