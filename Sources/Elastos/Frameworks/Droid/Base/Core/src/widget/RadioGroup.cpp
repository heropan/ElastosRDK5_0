
#include "widget/RadioGroup.h"
#include "R.h"
#include "widget/CRadioGroupLayoutParams.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::R;
using Elastos::Droid::View::EIID_IViewGroupOnHierarchyChangeListener;
using Elastos::Droid::View::EIID_IView;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//              RadioGroup::CheckedStateTracker
//==============================================================================

CAR_INTERFACE_IMPL(RadioGroup::CheckedStateTracker, ICompoundButtonOnCheckedChangeListener);

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
    buttonView->GetId(&id);
    mHost->SetCheckedId(id);

    return NOERROR;
}

//==============================================================================
//              RadioGroup::PassThroughHierarchyChangeListener
//==============================================================================

CAR_INTERFACE_IMPL(RadioGroup::PassThroughHierarchyChangeListener, IViewGroupOnHierarchyChangeListener);

RadioGroup::PassThroughHierarchyChangeListener::PassThroughHierarchyChangeListener(
    /* [in] */ RadioGroup* host)
    : mHost(host)
{}

/**
 * {@inheritDoc}
 */
ECode RadioGroup::PassThroughHierarchyChangeListener::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    IRadioButton* rb = IRadioButton::Probe(child);
    IView* hostView = (IView*)mHost->Probe(EIID_IView);
    if (parent == hostView && NULL != rb) {
        Int32 id;
        child->GetId(&id);
        // generates an id if it's missing
        if (id == IView::NO_ID) {
            id = View::GenerateViewId();
            child->SetId(id);
        }
        rb->SetOnCheckedChangeWidgetListener(mHost->mChildOnCheckedChangeListener);
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewAdded(parent, child);
    }

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
ECode RadioGroup::PassThroughHierarchyChangeListener::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    IRadioButton* rb = IRadioButton::Probe(child);
    IView* hostView = (IView*)mHost->Probe(EIID_IView);
    if (parent == hostView && NULL != rb) {
        rb->SetOnCheckedChangeWidgetListener(NULL);
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(parent, child);
    }

    return NOERROR;
}

//==============================================================================
//              RadioGroup
//==============================================================================

RadioGroup::RadioGroup()
    : mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{}

/**
 * {@inheritDoc}
 */
RadioGroup::RadioGroup(
    /* [in] */ IContext* context)
    : LinearLayout(context)
    , mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{
    SetOrientation(ILinearLayout::VERTICAL);
    InitInternal();
}

/**
 * {@inheritDoc}
 */
RadioGroup::RadioGroup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{
    Init(context, attrs);
}

ECode RadioGroup::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(LinearLayout::Init(context));
    SetOrientation(ILinearLayout::VERTICAL);
    InitInternal();

    return NOERROR;
}

ECode RadioGroup::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs));

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
    InitInternal();

    return NOERROR;
}

void RadioGroup::InitInternal()
{
    mChildOnCheckedChangeListener = new CheckedStateTracker(this);
    mPassThroughListener = new PassThroughHierarchyChangeListener(this);
    LinearLayout::SetOnHierarchyChangeListener(mPassThroughListener.Get());
}

/**
 * {@inheritDoc}
 */
//@Override
ECode RadioGroup::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    // the user listener is delegated to our pass-through listener
    mPassThroughListener->mOnHierarchyChangeListener = listener;
    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
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

//@Override
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
            IRadioButton::Probe(child)->GetId(&id);
            SetCheckedId(id);
        }
    }

    return LinearLayout::AddView(child, index, params);
}

/**
 * <p>Sets the selection to the radio button whose identifier is passed in
 * parameter. Using -1 as the selection identifier clears the selection;
 * such an operation is equivalent to invoking {@link #clearCheck()}.</p>
 *
 * @param id the unique id of the radio button to select in this group
 *
 * @see #getCheckedRadioButtonId()
 * @see #clearCheck()
 */
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
    AutoPtr<IView> checkedView = FindViewById(viewId);
    if (IRadioButton::Probe(checkedView) && ICheckable::Probe(checkedView)) {
        ICheckable::Probe(checkedView)->SetChecked(checked);
    }
}

/**
 * <p>Returns the identifier of the selected radio button in this group.
 * Upon empty selection, the returned value is -1.</p>
 *
 * @return the unique id of the selected radio button in this group
 *
 * @see #check(Int32)
 * @see #clearCheck()
 */
Int32 RadioGroup::GetCheckedRadioButtonId()
{
    return mCheckedId;
}

/**
 * <p>Clears the selection. When the selection is cleared, no radio button
 * in this group is selected and {@link #getCheckedRadioButtonId()} returns
 * null.</p>
 *
 * @see #check(Int32)
 * @see #getCheckedRadioButtonId()
 */
ECode RadioGroup::ClearCheck()
{
    return Check(-1);
}

/**
 * <p>Register a callback to be invoked when the checked radio button
 * changes in this group.</p>
 *
 * @param listener the callback to call on checked state change
 */
ECode RadioGroup::SetOnCheckedChangeListener(
    /* [in] */ IRadioGroupOnCheckedChangeListener* listener)
{
    mOnCheckedChangeListener = listener;

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
ECode RadioGroup::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IRadioGroupLayoutParams> lp;
    FAIL_RETURN(CRadioGroupLayoutParams::New(GetContext(), attrs, (IRadioGroupLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    INTERFACE_ADDREF(*params);
    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
Boolean RadioGroup::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p != NULL && IRadioGroupLayoutParams::Probe(p) != NULL;
}

//@Override
ECode RadioGroup::GenerateDefaultLayoutParams(
    /* [out] */ ILinearLayoutLayoutParams** params)
{
    AutoPtr<IRadioGroupLayoutParams> p;
    CRadioGroupLayoutParams::New(
        IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::WRAP_CONTENT,
        (IRadioGroupLayoutParams**)&p);

    *params = ILinearLayoutLayoutParams::Probe(p.Get());
    INTERFACE_ADDREF(*params)
    return NOERROR;
}

//@Override
ECode RadioGroup::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CRadioGroup"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    return NOERROR;
}

ECode RadioGroup::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    LinearLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CRadioGroup"), (ICharSequence**)&seq);
    info->SetClassName(seq);
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
