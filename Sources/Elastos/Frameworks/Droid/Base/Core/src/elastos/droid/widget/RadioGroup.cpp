
#include "elastos/droid/widget/RadioGroup.h"
#include "elastos/droid/widget/CRadioGroupLayoutParams.h"
#include "elastos/droid/R.h"
#include <elastos/core/CoreUtils.h>

using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Droid::View::EIID_IViewGroupOnHierarchyChangeListener;
using Elastos::Droid::R;
using Elastos::Core::CoreUtils;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//              RadioGroup::CheckedStateTracker
//==============================================================================

CAR_INTERFACE_IMPL(RadioGroup::LayoutParams, LinearLayout::LayoutParams, IRadioGroupLayoutParams);

RadioGroup::LayoutParams::LayoutParams()
{}

RadioGroup::LayoutParams::~LayoutParams()
{}

ECode RadioGroup::LayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayout::LayoutParams::constructor(c, attrs);
}

ECode RadioGroup::LayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return LinearLayout::LayoutParams::constructor(width, height);
}

ECode RadioGroup::LayoutParams::constructor(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float weight)
{
    return LinearLayout::LayoutParams::constructor(width, height, weight);
}

ECode RadioGroup::LayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return LinearLayout::LayoutParams::constructor(p);
}

ECode RadioGroup::LayoutParams::constructor(
    /* [in] */ IViewGroupMarginLayoutParams* source)
{
    return LinearLayout::LayoutParams::constructor(source);
}

ECode RadioGroup::LayoutParams::SetBaseAttributes(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 widthAttr,
    /* [in] */ Int32 heightAttr)
{
    Boolean res;
    a->HasValue(widthAttr, &res);
    if (res) {
        FAIL_RETURN(a->GetLayoutDimension(widthAttr, String("layout_width"), &mWidth));
    }
    else {
        mWidth = IViewGroupLayoutParams::WRAP_CONTENT;
    }

    a->HasValue(heightAttr, &res);
    if (res) {
        FAIL_RETURN(a->GetLayoutDimension(heightAttr, String("layout_height"), &mHeight));
    }
    else {
        mHeight = IViewGroupLayoutParams::WRAP_CONTENT;
    }

    return NOERROR;
}

//==============================================================================
//              RadioGroup::CheckedStateTracker
//==============================================================================

CAR_INTERFACE_IMPL(RadioGroup::CheckedStateTracker, Object, ICompoundButtonOnCheckedChangeListener);

RadioGroup::CheckedStateTracker::CheckedStateTracker(
    /* [in] */ RadioGroup* host)
    : mHost(host)
{}

ECode RadioGroup::CheckedStateTracker::OnCheckedChanged(
    /* [in] */ ICompoundButton* buttonView,
    /* [in] */ Boolean isChecked)
{
    // prevents from infinite recursion
    if (mHost->mProtectFromCheckedChange) {
        return NOERROR;
    }

    mHost->mProtectFromCheckedChange = TRUE;
    if (mHost->mCheckedId != -1) {
        mHost->SetCheckedStateForView(mHost->mCheckedId, FALSE);
    }
    mHost->mProtectFromCheckedChange = FALSE;

    Int32 id;
    IVIEW_PROBE(buttonView)->GetId(&id);
    mHost->SetCheckedId(id);

    return NOERROR;
}

//==============================================================================
//              RadioGroup::PassThroughHierarchyChangeListener
//==============================================================================

CAR_INTERFACE_IMPL(RadioGroup::PassThroughHierarchyChangeListener, Object, IViewGroupOnHierarchyChangeListener);

RadioGroup::PassThroughHierarchyChangeListener::PassThroughHierarchyChangeListener(
    /* [in] */ RadioGroup* host)
    : mHost(host)
{}

ECode RadioGroup::PassThroughHierarchyChangeListener::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    IRadioButton* rb = IRadioButton::Probe(child);
    IView* hostView = IVIEW_PROBE(mHost);
    if (parent == hostView && NULL != rb) {
        Int32 id;
        child->GetId(&id);
        // generates an id if it's missing
        if (id == IView::NO_ID) {
            id = View::GenerateViewId();
            child->SetId(id);
        }
        ICompoundButton::Probe(rb)->SetOnCheckedChangeWidgetListener(mHost->mChildOnCheckedChangeListener);
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewAdded(parent, child);
    }

    return NOERROR;
}

ECode RadioGroup::PassThroughHierarchyChangeListener::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    IRadioButton* rb = IRadioButton::Probe(child);
    IView* hostView = IVIEW_PROBE(mHost);
    if (parent == hostView && NULL != rb) {
        ICompoundButton::Probe(rb)->SetOnCheckedChangeWidgetListener(NULL);
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(parent, child);
    }

    return NOERROR;
}

//==============================================================================
//              RadioGroup
//==============================================================================

CAR_INTERFACE_IMPL(RadioGroup, LinearLayout, IRadioGroup);

RadioGroup::RadioGroup()
    : mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{}

RadioGroup::~RadioGroup()
{}

ECode RadioGroup::constructor(
    /* [in] */ IContext* context)
{
    LinearLayout::constructor(context);
    SetOrientation(ILinearLayout::VERTICAL);
    Init();
    return NOERROR;
}

RadioGroup::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::constructor(context, attrs));

    // retrieve selected radio button as requested by the user in the
    // XML layout file
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::RadioGroup),
            ARRAY_SIZE(R::styleable::RadioGroup));
    AutoPtr<ITypedArray> attributes;
    context->ObtainStyledAttributes(
            attrs, attrIds, R::attr::radioButtonStyle, 0, (ITypedArray**)&attributes);

    Int32 value;
    attributes->GetResourceId(
        R::styleable::RadioGroup_checkedButton, IView::NO_ID, &value);
    if (value != IView::NO_ID) {
        mCheckedId = value;
    }

    Int32 index;
    attributes->GetInt32(R::styleable::RadioGroup_orientation, ILinearLayout::VERTICAL, &index);
    SetOrientation(index);

    attributes->Recycle();
    Init();

    return NOERROR;
}

void RadioGroup::Init()
{
    mChildOnCheckedChangeListener = new CheckedStateTracker(this);
    mPassThroughListener = new PassThroughHierarchyChangeListener(this);
    LinearLayout::SetOnHierarchyChangeListener(mPassThroughListener.Get());
}

ECode RadioGroup::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    // the user listener is delegated to our pass-through listener
    mPassThroughListener->mOnHierarchyChangeListener = listener;
    return NOERROR;
}

ECode RadioGroup::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();

    // checks the appropriate radio button as requested in the XML file
    if (mCheckedId != -1) {
        mProtectFromCheckedChange = TRUE;
        SetCheckedStateForView(mCheckedId, TRUE);
        mProtectFromCheckedChange = FALSE;
        SetCheckedId(mCheckedId);
    }

    return NOERROR;
}

ECode RadioGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (IRadioButton::Probe(child)) {
        Boolean isChecked;
        ICheckable::Probe(child)->IsChecked(&isChecked);
        if (isChecked) {
            mProtectFromCheckedChange = TRUE;
            if (mCheckedId != -1) {
                SetCheckedStateForView(mCheckedId, FALSE);
            }
            mProtectFromCheckedChange = FALSE;
            Int32 id;
            IVIEW_PROBE(child)->GetId(&id);
            SetCheckedId(id);
        }
    }

    return LinearLayout::AddView(child, index, params);
}

ECode RadioGroup::Check(
    /* [in] */ Int32 id)
{
    // don't even bother
    if (id != -1 && (id == mCheckedId)) {
        return NOERROR;
    }

    if (mCheckedId != -1) {
        SetCheckedStateForView(mCheckedId, FALSE);
    }

    if (id != -1) {
        SetCheckedStateForView(id, TRUE);
    }

    SetCheckedId(id);

    return NOERROR;
}

void RadioGroup::SetCheckedId(
    /* [in] */ Int32 id)
{
    mCheckedId = id;
    if (mOnCheckedChangeListener != NULL) {
        mOnCheckedChangeListener->OnCheckedChanged(THIS_PROBE(IRadioGroup), mCheckedId);
    }
}

void RadioGroup::SetCheckedStateForView(
    /* [in] */ Int32 viewId,
    /* [in] */ Boolean checked)
{
    AutoPtr<IView> checkedView;
    FindViewById(viewId, (IView**)&checkedView);
    if (IRadioButton::Probe(checkedView) && ICheckable::Probe(checkedView)) {
        ICheckable::Probe(checkedView)->SetChecked(checked);
    }
}

ECode RadioGroup::GetCheckedRadioButtonId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mCheckedId;
    return NOERROR;
}

ECode RadioGroup::ClearCheck()
{
    return Check(-1);
}

ECode RadioGroup::SetOnCheckedChangeListener(
    /* [in] */ IRadioGroupOnCheckedChangeListener* listener)
{
    mOnCheckedChangeListener = listener;

    return NOERROR;
}

ECode RadioGroup::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IRadioGroupLayoutParams> lp;
    FAIL_RETURN(CRadioGroupLayoutParams::New(context, attrs, (IRadioGroupLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

Boolean RadioGroup::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p != NULL && IRadioGroupLayoutParams::Probe(p) != NULL;
}

ECode RadioGroup::GenerateDefaultLayoutParams(
    /* [out] */ ILinearLayoutLayoutParams** params)
{
    AutoPtr<IRadioGroupLayoutParams> p;
    CRadioGroupLayoutParams::New(
            IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::WRAP_CONTENT,
            (IRadioGroupLayoutParams**)&p);

    *params = ILinearLayoutLayoutParams::Probe(p);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

ECode RadioGroup::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    IAccessibilityRecord::Probe(event)->SetClassName(CoreUtils::Convert("CRadioGroup"));
    return NOERROR;
}

ECode RadioGroup::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    LinearLayout::OnInitializeAccessibilityNodeInfo(info);
    info->SetClassName(CoreUtils::Convert("CRadioGroup"));
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
