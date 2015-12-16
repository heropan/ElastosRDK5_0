
#include "Elastos.Droid.Database.h"
#include "Elastos.Droid.View.h"
#include "Elastos.Droid.Widget.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "elastos/droid/widget/SimpleExpandableListAdapter.h"

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(SimpleExpandableListAdapter, BaseExpandableListAdapter, ISimpleExpandableListAdapter);
SimpleExpandableListAdapter::SimpleExpandableListAdapter()
    : mExpandedGroupLayout(0)
    , mCollapsedGroupLayout(0)
    , mChildLayout(0)
    , mLastChildLayout(0)
{}

ECode SimpleExpandableListAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IList* groupData,
    /* [in] */ Int32 groupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IList* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return constructor(context, groupData, groupLayout, groupLayout, groupFrom, groupTo, childData,
                childLayout, childLayout, childFrom, childTo);
}

ECode SimpleExpandableListAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IList* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IList* childData,
    /* [in] */ Int32 childLayout,
    /* [in] */ ArrayOf<String>* childFrom,
    /* [in] */ ArrayOf<Int32>* childTo)
{
    return constructor(context, groupData, expandedGroupLayout, collapsedGroupLayout,
                groupFrom, groupTo, childData, childLayout, childLayout,
                childFrom, childTo);
}

ECode SimpleExpandableListAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IList* groupData,
    /* [in] */ Int32 expandedGroupLayout,
    /* [in] */ Int32 collapsedGroupLayout,
    /* [in] */ ArrayOf<String>* groupFrom,
    /* [in] */ ArrayOf<Int32>* groupTo,
    /* [in] */ IList* childData,
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

ECode SimpleExpandableListAdapter::GetChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ IInterface** child)
{
    VALIDATE_NOT_NULL(child);
    AutoPtr<IList> group;
    mChildData->Get(groupPosition, (IInterface**)&group);
    return group->Get(childPosition, child);
}

ECode SimpleExpandableListAdapter::GetChildId(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = childPosition;
    return NOERROR;
}

ECode SimpleExpandableListAdapter::GetChildView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean isLastChild,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v;
    if (convertView == NULL) {
        NewChildView(isLastChild, parent, (IView**)&v);
    } else {
        v = convertView;
    }
    AutoPtr<IList> group;
    mChildData->Get(groupPosition, (IInterface**)&group);
    AutoPtr<IMap> children;
    group->Get(childPosition, (IInterface**)&children);
    BindView(v, children, mChildFrom, mChildTo);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode SimpleExpandableListAdapter::NewChildView(
    /* [in] */ Boolean isLastChild,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return mInflater->Inflate((isLastChild) ? mLastChildLayout : mChildLayout, parent, FALSE, view);
}

ECode SimpleExpandableListAdapter::GetChildrenCount(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    AutoPtr<IList> group;
    mChildData->Get(groupPosition, (IInterface**)&group);
    return group->GetSize(count);
}

ECode SimpleExpandableListAdapter::GetGroup(
    /* [in] */ Int32 groupPosition,
    /* [out] */ IInterface** child)
{
    VALIDATE_NOT_NULL(child);
    return mGroupData->Get(groupPosition, child);
}

ECode SimpleExpandableListAdapter::GetGroupCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return mGroupData->GetSize(count);
}

ECode SimpleExpandableListAdapter::GetGroupId(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = groupPosition;
    return NOERROR;
}

ECode SimpleExpandableListAdapter::GetGroupView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Boolean isExpanded,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v;
    if (convertView == NULL) {
        NewGroupView(isExpanded, parent, (IView**)&v);
    } else {
        v = convertView;
    }
    AutoPtr<IMap> group;
    mGroupData->Get(groupPosition, (IInterface**)&group);
    BindView(v, group, mGroupFrom, mGroupTo);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode SimpleExpandableListAdapter::NewGroupView(
    /* [in] */ Boolean isExpanded,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return mInflater->Inflate((isExpanded) ? mExpandedGroupLayout : mCollapsedGroupLayout, parent, FALSE, view);
}

ECode SimpleExpandableListAdapter::IsChildSelectable(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode SimpleExpandableListAdapter::HasStableIds(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = TRUE;
    return NOERROR;
}

ECode SimpleExpandableListAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ IMap* data,
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
            AutoPtr<ICharSequence> key;
            CString::New((*from)[i], (ICharSequence**)&key);
            data->Get(key, (IInterface**)&temp);
            AutoPtr<ICharSequence> cs = ICharSequence::Probe(temp);
            if (cs != NULL) {
                tv->SetText(cs);
            }
        }
    }
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
