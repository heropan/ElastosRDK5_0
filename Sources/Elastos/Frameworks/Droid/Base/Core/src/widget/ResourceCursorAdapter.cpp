#include "widget/ResourceCursorAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

ResourceCursorAdapter::ResourceCursorAdapter()
    : mLayout(0)
    , mDropDownLayout(0)
{}

ResourceCursorAdapter::ResourceCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c)
    : CursorAdapter(context, c)
    , mLayout(0)
    , mDropDownLayout(0)
{
    InitImpl(context, layout);
}

ResourceCursorAdapter::ResourceCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery)
    : CursorAdapter(context, c, autoRequery)
    , mLayout(0)
    , mDropDownLayout(0)
{
    InitImpl(context, layout);
}

ResourceCursorAdapter::ResourceCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ Int32 flags)
    : CursorAdapter(context, c, flags)
    , mLayout(0)
    , mDropDownLayout(0)
{
    InitImpl(context, layout);
}

ECode ResourceCursorAdapter::InitImpl(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout)
{
    VALIDATE_NOT_NULL(context);

    mLayout = mDropDownLayout = layout;
    return context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE,
        (IInterface**)&mInflater);
}

ECode ResourceCursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c)
{
    CursorAdapter::Init(context, c);
    return InitImpl(context, layout);
}

ECode ResourceCursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery)
{
    CursorAdapter::Init(context, c, autoRequery);
    return InitImpl(context, layout);
}

ECode ResourceCursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layout,
    /* [in] */ ICursor* c,
    /* [in] */ Int32 flags)
{
    CursorAdapter::Init(context, c, flags);
    return InitImpl(context, layout);
}

AutoPtr<IView> ResourceCursorAdapter::NewView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view;
    mInflater->Inflate(mLayout, parent, FALSE, (IView**)&view);
    return view;
}

AutoPtr<IView> ResourceCursorAdapter::NewDropDownView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view;
    mInflater->Inflate(mDropDownLayout, parent, FALSE, (IView**)&view);
    return view;
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

}// namespace Elastos
}// namespace Droid
}// namespace Widget
