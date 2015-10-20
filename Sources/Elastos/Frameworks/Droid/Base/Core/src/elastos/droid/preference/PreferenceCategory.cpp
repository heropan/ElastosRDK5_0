
#include "elastos/droid/preference/PreferenceCategory.h"
#include <elastos/droid/R.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Preference {

ECode PreferenceCategory::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    PreferenceGroup::Init(context, attrs, defStyleAttr, defStyleRes);
    return NOERROR;
}

ECode PreferenceCategory::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode PreferenceCategory::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::preferenceCategoryStyle);
}

ECode PreferenceCategory::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

CAR_INTERFACE_IMPL(PreferenceCategory, PreferenceGroup, IPreferenceCategory)

const String PreferenceCategory::TAG("PreferenceCategory");

ECode PreferenceCategory::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable)
    *isEnable = FALSE;
    return NOERROR;
}

ECode PreferenceCategory::ShouldDisableDependents(
    /* [out] */ Boolean* should)
{
    Preference::IsEnabled(should);
    *should = !(*should);
    return NOERROR;
}

ECode PreferenceCategory::OnPrepareAddPreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    if (IPreferenceCategory::Probe(preference) != NULL) {
        Slogger::E(TAG, "Cannot add a %s directly to a ", TAG.string(), TAG.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return PreferenceGroup::OnPrepareAddPreference(preference, result);
}

ECode PreferenceCategory::SetOrderingAsAdded(
    /* [in] */ Boolean orderingAsAdded)
{
    return PreferenceGroup::SetOrderingAsAdded(orderingAsAdded);
}

ECode PreferenceCategory::IsOrderingAsAdded(
    /* [out] */ Boolean* added)
{
    return PreferenceGroup::IsOrderingAsAdded(added);
}

ECode PreferenceCategory::GetPreferenceCount(
    /* [out] */ Int32* count)
{
    return PreferenceGroup::GetPreferenceCount(count);
}

ECode PreferenceCategory::GetPreference(
    /* [in] */ Int32 index,
    /* [out] */ IPreference** preference)
{
    return PreferenceGroup::GetPreference(index, preference);
}

ECode PreferenceCategory::AddPreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::AddPreference(preference, result);
}

ECode PreferenceCategory::RemovePreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::RemovePreference(preference, result);
}

ECode PreferenceCategory::RemoveAll()
{
    return PreferenceGroup::RemoveAll();
}

ECode PreferenceCategory::FindPreference(
    /* [in] */ ICharSequence* key,
    /* [out] */ IPreference** preference)
{
    return PreferenceGroup::FindPreference(key, preference);
}

ECode PreferenceCategory::IsOnSameScreenAsChildren(
    /* [out] */ Boolean* isOnSameScreenAsChildren)
{
    return PreferenceGroup::IsOnSameScreenAsChildren(isOnSameScreenAsChildren);
}

ECode PreferenceCategory::SortPreferences()
{
    return PreferenceGroup::SortPreferences();
}

} // Preference
} // Droid
} // Elastos
