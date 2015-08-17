
#include "TwoStatePreference.h"
#include "CTwoStatePreferenceSavedState.h"
#include "view/accessibility/CAccessibilityManagerHelper.h"
#include "view/accessibility/CAccessibilityEventHelper.h"
#include "R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

using Elastos::Core::CString;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Droid::View::Accessibility::IAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityManagerHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityManagerHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;

namespace Elastos {
namespace Droid {
namespace Preference {

TwoStatePreference::TwoStatePreference()
    : mChecked(FALSE)
    , mCheckedSet(FALSE)
    , mSendClickAccessibilityEvent(FALSE)
    , mDisableDependentsState(FALSE)
{}

void TwoStatePreference::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Preference::Init(context, attrs, defStyle);
}

void TwoStatePreference::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

void TwoStatePreference::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

ECode TwoStatePreference::OnClick()
{
    Slogger::E("TwoStatePreference", "~~~~~~~~~~~~~~~~~~~~~~on click");
    Preference::OnClick();

    Boolean isChecked;
    IsChecked(&isChecked);
    Boolean newValue = !isChecked;

    mSendClickAccessibilityEvent = TRUE;

    AutoPtr<IBoolean> interfaceV;
    CBoolean::New(newValue, (IBoolean**)&interfaceV);
    Boolean callResult;
    if (CallChangeListener(interfaceV, &callResult), !callResult) {
        return NOERROR;
    }

    SetChecked(newValue);
    return NOERROR;
}

ECode TwoStatePreference::SetChecked(
    /* [in] */ Boolean checked)
{
    // Always persist/notify the first time; don't assume the field's default of false.
    Boolean changed = mChecked != checked;
    if (changed || !mCheckedSet) {
        mChecked = checked;
        mCheckedSet = TRUE;
        Boolean isPersist;
        PersistBoolean(checked, &isPersist);
        if (changed) {
            Boolean shouldDisableDependents;
            ShouldDisableDependents(&shouldDisableDependents);
            NotifyDependencyChange(shouldDisableDependents);
            NotifyChanged();
        }
    }

    return NOERROR;
}

ECode TwoStatePreference::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    VALIDATE_NOT_NULL(isChecked)
    *isChecked = mChecked;
    return NOERROR;
}

ECode TwoStatePreference::ShouldDisableDependents(
    /* [out] */ Boolean* shouldDisableDependents)
{
    VALIDATE_NOT_NULL(shouldDisableDependents)

    Boolean shouldDisable = mDisableDependentsState ? mChecked : !mChecked;
    Boolean superShouldDisableDependents;
    Preference::ShouldDisableDependents(&superShouldDisableDependents);
    *shouldDisableDependents = shouldDisable || superShouldDisableDependents;

    return NOERROR;
}

ECode TwoStatePreference::SetSummaryOn(
    /* [in] */ ICharSequence* summary)
{
    mSummaryOn = summary;
    Boolean isChecked;
    if (IsChecked(&isChecked), isChecked) {
        NotifyChanged();
    }
    return NOERROR;
}

ECode TwoStatePreference::SetSummaryOn(
    /* [in] */ Int32 summaryResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    String summaryStr;
    context->GetString(summaryResId, &summaryStr);
    AutoPtr<ICharSequence> summary;
    CString::New(summaryStr, (ICharSequence**)&summary);
    return SetSummaryOn(summary);
}

ECode TwoStatePreference::GetSummaryOn(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    *summary = mSummaryOn;
    REFCOUNT_ADD(*summary)
    return NOERROR;
}


ECode TwoStatePreference::SetSummaryOff(
    /* [in] */ ICharSequence* summary)
{
    mSummaryOff = summary;
    Boolean isChecked;
    if (IsChecked(&isChecked), !isChecked) {
        NotifyChanged();
    }
    return NOERROR;
}

ECode TwoStatePreference::SetSummaryOff(
    /* [in] */ Int32 summaryResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    String summaryStr;
    context->GetString(summaryResId, &summaryStr);
    AutoPtr<ICharSequence> summary;
    CString::New(summaryStr, (ICharSequence**)&summary);
    return SetSummaryOff(summary);
}

ECode TwoStatePreference::GetSummaryOff(
    /* [in] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    *summary = mSummaryOff;
    REFCOUNT_ADD(*summary)
    return NOERROR;
}

ECode TwoStatePreference::GetDisableDependentsState(
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state)
    *state = mDisableDependentsState;
    return NOERROR;
}

ECode TwoStatePreference::SetDisableDependentsState(
    /* [in] */ Boolean disableDependentsState)
{
    mDisableDependentsState = disableDependentsState;
    return NOERROR;
}


ECode TwoStatePreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    Boolean retVal;
    a->GetBoolean(index, FALSE, &retVal);
    AutoPtr<IBoolean> retObj;
    CBoolean::New(retVal, (IBoolean**)&retObj);
    *value = (IInterface*)retObj;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode TwoStatePreference::OnSetInitialValue(
    /* [in] */ Boolean restorePersistedValue,
    /* [in] */ IInterface* defaultValue)
{
    Boolean value = FALSE;
    if (restorePersistedValue) {
        GetPersistedBoolean(mChecked, &value);
    }
    else {
        AutoPtr<IBoolean> defaultVal = IBoolean::Probe(defaultVal);
        if(defaultVal != NULL){
            defaultVal->GetValue(&value);
        }
    }
    SetChecked(value);
    return NOERROR;
}

void TwoStatePreference::SendAccessibilityEvent(
    /* [in] */ IView* view)
{
    // Since the view is still not attached we create, populate,
    // and send the event directly since we do not know when it
    // will be attached and posting commands is not as clean.
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    AutoPtr<IAccessibilityManagerHelper> managerHelper;
    CAccessibilityManagerHelper::AcquireSingleton((IAccessibilityManagerHelper**)&managerHelper);
    AutoPtr<IAccessibilityManager> accessibilityManager;
    managerHelper->GetInstance(ctx, (IAccessibilityManager**)&accessibilityManager);
    Boolean enabled;
    if (mSendClickAccessibilityEvent && (accessibilityManager->IsEnabled(&enabled), enabled)) {
        AutoPtr<IAccessibilityEventHelper> helper;
        CAccessibilityEventHelper::AcquireSingleton((IAccessibilityEventHelper**)&helper);
        AutoPtr<IAccessibilityEvent> event;
        helper->Obtain((IAccessibilityEvent**)&event);
        event->SetEventType(IAccessibilityEvent::TYPE_VIEW_CLICKED);
        view->OnInitializeAccessibilityEvent(event);
        Boolean isConsumed;
        view->DispatchPopulateAccessibilityEvent(event, &isConsumed);
        accessibilityManager->SendAccessibilityEvent(event);
    }

    mSendClickAccessibilityEvent = FALSE;
}

void TwoStatePreference::SyncSummaryView(
    /* [in] */ IView* view)
{
    // Sync the summary view
    AutoPtr<ITextView> summaryView;
    view->FindViewById(R::id::summary, (IView**)(ITextView**)&summaryView);
    if (summaryView != NULL) {
        Boolean useDefaultSummary = TRUE;
        if (mChecked && mSummaryOn != NULL) {
            summaryView->SetText(mSummaryOn);
            useDefaultSummary = FALSE;
        }
        else if (!mChecked && mSummaryOff != NULL) {
            summaryView->SetText(mSummaryOff);
            useDefaultSummary = FALSE;
        }

        if (useDefaultSummary) {
            AutoPtr<ICharSequence> summary;
            GetSummary((ICharSequence**)&summary);
            if (summary != NULL) {
                summaryView->SetText(summary);
                useDefaultSummary = FALSE;
            }
        }

        Int32 newVisibility = IView::GONE;
        if (!useDefaultSummary) {
            // Someone has written to it
            newVisibility = IView::VISIBLE;
        }

        Int32 visibility;
        if (summaryView->GetVisibility(&visibility), newVisibility != visibility) {
            summaryView->SetVisibility(newVisibility);
        }
    }
}

ECode TwoStatePreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)
    AutoPtr<IParcelable> superState;
    Preference::OnSaveInstanceState((IParcelable**)&superState);
    Boolean isPersistent;
    if (IsPersistent(&isPersistent), isPersistent) {
        // No need to save instance state since it's persistent
        *state = superState;
        REFCOUNT_ADD(*state)
        return NOERROR;
    }

    AutoPtr<ITwoStatePreferenceSavedState> myState;
    CTwoStatePreferenceSavedState::New(superState, (ITwoStatePreferenceSavedState**)&myState);
    Boolean isChecked;
    IsChecked(&isChecked);
    myState->SetChecked(isChecked);
    *state = IParcelable::Probe(myState);
    REFCOUNT_ADD(*state)
    return NOERROR;
}


ECode TwoStatePreference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (state == NULL || ITwoStatePreferenceSavedState::Probe(state) == NULL) {
        // Didn't save state for us in onSaveInstanceState
        Preference::OnRestoreInstanceState(state);
        return NOERROR;
    }
    AutoPtr<ITwoStatePreferenceSavedState> myState = ITwoStatePreferenceSavedState::Probe(state);
    AutoPtr<IParcelable> superState;
    myState->GetSuperState((IParcelable**)&superState);
    Preference::OnRestoreInstanceState(superState);
    Boolean isChecked;
    myState->IsChecked(&isChecked);
    SetChecked(isChecked);
    return NOERROR;
}

}
}
}
