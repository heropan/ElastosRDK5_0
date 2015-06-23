
#include "widget/SimpleExpandableListAdapter.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Widget {

SimpleExpandableListAdapter::SimpleExpandableListAdapter()
    : mExpandedGroupLayout(0)
    , mCollapsedGroupLayout(0)
    , mChildLayout(0)
    , mLastChildLayout(0)
{}

SimpleExpandableListAdapter::SimpleExpandableListAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 groupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    ASSERT_SUCCEEDED(Init(context, groupData, groupLayout, groupFrom, groupTo,
        childData, childLayout, childFrom, childTo));
}

SimpleExpandableListAdapter::SimpleExpandableListAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    ASSERT_SUCCEEDED(Init(context, groupData, expandedGroupLayout, collapsedGroupLayout, groupFrom, groupTo,
        childData, childLayout, childFrom, childTo));
}

SimpleExpandableListAdapter::SimpleExpandableListAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ Int32 lastChildLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    ASSERT_SUCCEEDED(Init(context, groupData, expandedGroupLayout, collapsedGroupLayout, groupFrom, groupTo,
        childData, childLayout, lastChildLayout, childFrom, childTo));
}

ECode SimpleExpandableListAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 groupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return Init(context, groupData, groupLayout, groupLayout, groupFrom, groupTo, childData,
                childLayout, childLayout, childFrom, childTo);
}

ECode SimpleExpandableListAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return Init(context, groupData, expandedGroupLayout, collapsedGroupLayout,
                groupFrom, groupTo, childData, childLayout, childLayout,
                childFrom, childTo);
}


ECode SimpleExpandableListAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IObjectContainer* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ Int32 lastChildLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    mGroupData = groupData;
    mExpandedGroupLayout = expandedGroupLayout;
    mCollapsedGroupLayout = collapsedGroupLayout;
    mGroupFrom = groupFrom;
    mGroupTo = groupTo;

    mChildData = childData;
    mChildLayout = childLayout;
    mLastChildLayout = lastChildLayout;
    mChildFrom = childFrom;
    mChildTo = childTo;

    AutoPtr<IInterface> sTemp;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&sTemp);
    mInflater = ILayoutInflater::Probe(sTemp);
    return NOERROR;
}

AutoPtr<IInterface> SimpleExpandableListAdapter::GetChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    AutoPtr<IInterface> temp = GetItemFromContainer(mChildData, groupPosition);
    AutoPtr<IObjectContainer> container = IObjectContainer::Probe(temp);
    if(container == NULL)
        return NULL;
    temp = GetItemFromContainer(container, childPosition);
    return temp;
}

Int64 SimpleExpandableListAdapter::GetChildId(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return childPosition;
}

AutoPtr<IView> SimpleExpandableListAdapter::GetChildView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean isLastChild,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    if (convertView == NULL) {
        v = NewChildView(isLastChild, parent);
    }
    else {
        v = convertView;
    }
    AutoPtr<IInterface> temp = GetItemFromContainer(mChildData, groupPosition);
    AutoPtr<IObjectContainer> container = IObjectContainer::Probe(temp);
    if(container == NULL)
        return v;
    temp = GetItemFromContainer(container, childPosition);
    AutoPtr<IObjectStringMap> map = IObjectStringMap::Probe(temp);
    if (map != NULL)
        BindView(v, map, mChildFrom, mChildTo);
    return v;
}

AutoPtr<IView> SimpleExpandableListAdapter::NewChildView(
    /* [in] */ Boolean isLastChild,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    mInflater->Inflate((isLastChild) ? mLastChildLayout : mChildLayout, parent, FALSE, (IView**)&v);
    return v;
}

Int32 SimpleExpandableListAdapter::GetChildrenCount(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<IInterface> temp = GetItemFromContainer(mChildData, groupPosition);
    AutoPtr<IObjectContainer> container = IObjectContainer::Probe(temp);
    if(container == NULL) return 0;
    Int32 count;
    container->GetObjectCount(&count);
    return count;
}

AutoPtr<IInterface> SimpleExpandableListAdapter::GetGroup(
    /* [in] */ Int32 groupPosition)
{
    return GetItemFromContainer(mGroupData, groupPosition);
}

Int32 SimpleExpandableListAdapter::GetGroupCount()
{
    Int32 count;
    mGroupData->GetObjectCount(&count);
    return count;
}

Int64 SimpleExpandableListAdapter::GetGroupId(
    /* [in] */ Int32 groupPosition)
{
    return groupPosition;
}

AutoPtr<IView> SimpleExpandableListAdapter::GetGroupView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Boolean isExpanded,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    if (convertView == NULL) {
        v = NewGroupView(isExpanded, parent);
    } else {
        v = convertView;
    }
    AutoPtr<IInterface> temp = GetItemFromContainer(mGroupData, groupPosition);
    AutoPtr<IObjectStringMap> map = IObjectStringMap::Probe(temp);
    if (map != NULL)
        BindView(v, map, mGroupFrom, mGroupTo);
    return v;
}

AutoPtr<IView> SimpleExpandableListAdapter::NewGroupView(
    /* [in] */ Boolean isExpanded,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    mInflater->Inflate((isExpanded) ? mExpandedGroupLayout : mCollapsedGroupLayout,
        parent, FALSE, (IView**)&v);
    return v;
}

Boolean SimpleExpandableListAdapter::IsChildSelectable(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return TRUE;
}

Boolean SimpleExpandableListAdapter::HasStableIds()
{
    return TRUE;
}

ECode SimpleExpandableListAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ IObjectStringMap* data,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    Int32 len = to->GetLength();

    for (Int32 i = 0; i < len; i++) {
        AutoPtr<IView> v;
        view->FindViewById((*to)[i], (IView**)&v);
        AutoPtr<ITextView> tv = ITextView::Probe(v);
        if (tv != NULL) {
            AutoPtr<IInterface> temp;
            data->Get((*from)[i], (IInterface**)&temp);
            AutoPtr<ICharSequence> cs = ICharSequence::Probe(temp);
            if (cs != NULL)
                tv->SetText(cs);
        }
    }
    return NOERROR;
}

AutoPtr<IInterface> SimpleExpandableListAdapter::GetItemFromContainer(
        /* [in] */ IObjectContainer* container,
        /* [in] */ Int32 position)
{
    Int32 count;
    container->GetObjectCount(&count);
    if (count < position)
        return NULL;
    AutoPtr<IObjectEnumerator> enumerator;
    container->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    Int32 i = 0;
    while(enumerator->MoveNext(&hasNext), hasNext && (i++ < position - 1)) {
    }
    if (!hasNext) return NULL;
    AutoPtr<IInterface> temp;
    enumerator->Current((IInterface**)&temp);
    return temp;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

