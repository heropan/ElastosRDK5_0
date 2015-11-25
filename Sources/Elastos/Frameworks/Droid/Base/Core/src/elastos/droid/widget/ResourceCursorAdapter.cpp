
#include "elastos/droid/widget/ResourceCursorAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(ResourceCursorAdapter, CursorAdapter, IResourceCursorAdapter);
ResourceCursorAdapter::ResourceCursorAdapter()
    : mLayout(0)
    , mDropDownLayout(0)
{}

ECode ResourceCursorAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c)
{
    CursorAdapter::constructor(context, c);
    return InitImpl(context, layout);
}

ECode ResourceCursorAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery)
{
    CursorAdapter::constructor(context, c, autoRequery);
    return InitImpl(context, layout);
}

ECode ResourceCursorAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ Int32 flags)
{
    CursorAdapter::constructor(context, c, flags);
    return InitImpl(context, layout);
}

ECode ResourceCursorAdapter::InitImpl(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout)
{
    VALIDATE_NOT_NULL(context);

    mLayout = mDropDownLayout = layout;
    return context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
}

ECode ResourceCursorAdapter::NewView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return mInflater->Inflate(mLayout, parent, FALSE, view);
}

ECode ResourceCursorAdapter::NewDropDownView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return mInflater->Inflate(mDropDownLayout, parent, FALSE, view);
}

ECode ResourceCursorAdapter::SetViewResource(
    /* [in] */ Int32 layout)
{
    mLayout = layout;
    return NOERROR;
}

ECode ResourceCursorAdapter::SetDropDownViewResource(
    /* [in] */ Int32 dropDownLayout)
{
    mDropDownLayout = dropDownLayout;
    return NOERROR;
}

} // namespace Elastos
} // namespace Droid
} // namespace Widget
