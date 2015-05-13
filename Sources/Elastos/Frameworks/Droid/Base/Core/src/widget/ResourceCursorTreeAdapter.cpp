#include "ResourceCursorTreeAdapter.h"
/**
 * A fairly simple ExpandableListAdapter that creates views defined in an XML
 * file. You can specify the XML file that defines the appearance of the views.
 */
namespace Elastos {
namespace Droid {
namespace Widget {

ResourceCursorTreeAdapter::ResourceCursorTreeAdapter()
    : mCollapsedGroupLayout(0)
    , mExpandedGroupLayout(0)
    , mChildLayout(0)
    , mLastChildLayout(0)
{}

ResourceCursorTreeAdapter::ResourceCursorTreeAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 childLayout,
    /* [in] */ Int32 lastChildLayout)
    : CursorTreeAdapter(cursor, context)
    , mCollapsedGroupLayout(0)
    , mExpandedGroupLayout(0)
    , mChildLayout(0)
    , mLastChildLayout(0)
{
    ASSERT_SUCCEEDED(InitSelf(context, cursor, collapsedGroupLayout,
     expandedGroupLayout, childLayout, lastChildLayout));
}

ResourceCursorTreeAdapter::ResourceCursorTreeAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 childLayout)
    : CursorTreeAdapter(cursor, context)
    , mCollapsedGroupLayout(0)
    , mExpandedGroupLayout(0)
    , mChildLayout(0)
    , mLastChildLayout(0)
{
    ASSERT_SUCCEEDED(InitSelf(context, cursor, collapsedGroupLayout,
     expandedGroupLayout, childLayout, childLayout));
}

ResourceCursorTreeAdapter::ResourceCursorTreeAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 groupLayout,
    /* [in] */ Int32 childLayout)
    : CursorTreeAdapter(cursor, context)
    , mCollapsedGroupLayout(0)
    , mExpandedGroupLayout(0)
    , mChildLayout(0)
    , mLastChildLayout(0)
{
    ASSERT_SUCCEEDED(InitSelf(context, cursor, groupLayout, groupLayout, childLayout, childLayout));
}

ECode ResourceCursorTreeAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 childLayout,
    /* [in] */ Int32 lastChildLayout)
{
    CursorTreeAdapter::Init(cursor, context);
    InitSelf(context, cursor, collapsedGroupLayout, expandedGroupLayout, childLayout, lastChildLayout);
    return NOERROR;
}

ECode ResourceCursorTreeAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 childLayout)
{
    return Init(context, cursor, collapsedGroupLayout,
     expandedGroupLayout, childLayout, childLayout);
}

ECode ResourceCursorTreeAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 groupLayout,
    /* [in] */ Int32 childLayout)
{
    return Init(context, cursor, groupLayout, groupLayout, childLayout, childLayout);
}

ECode ResourceCursorTreeAdapter::InitSelf(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 childLayout,
    /* [in] */ Int32 lastChildLayout)
{
    mCollapsedGroupLayout = collapsedGroupLayout;
    mExpandedGroupLayout = expandedGroupLayout;
    mChildLayout = childLayout;
    mLastChildLayout = lastChildLayout;

    AutoPtr<IInterface> sTemp;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&sTemp);
    mInflater = ILayoutInflater::Probe(sTemp);
    return NOERROR;
}

AutoPtr<IView> ResourceCursorTreeAdapter::NewChildView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Boolean isLastChild,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    mInflater->InflateEx2((isLastChild) ? mLastChildLayout : mChildLayout, parent, FALSE, (IView**)&v);
    return v;
}

AutoPtr<IView> ResourceCursorTreeAdapter::NewGroupView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ Boolean isExpanded,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    mInflater->InflateEx2((isExpanded) ? mExpandedGroupLayout : mCollapsedGroupLayout, parent, FALSE, (IView**)&v);
    return v;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
