
#include "CPreferenceCategory.h"
#include <R.h>
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Preference {

const String CPreferenceCategory::TAG("PreferenceCategory");

PInterface CPreferenceCategory::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IPreferenceCategory) {
        return (IPreferenceCategory*)this;
    }
    else if (riid == EIID_IPreferenceGroup) {
        return (IPreferenceGroup*)this;
    }

    return PreferenceGroup::Probe(riid);
}

UInt32 CPreferenceCategory::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPreferenceCategory::Release()
{
    return ElRefBase::Release();
}

ECode CPreferenceCategory::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IPreferenceCategory*)this) {
        *pIID = EIID_IPreferenceCategory;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IPreferenceGroup*)this) {
        *pIID = EIID_IPreferenceGroup;
        return NOERROR;
    }
    return PreferenceGroup::GetInterfaceID(pObject, pIID);
}

ECode CPreferenceCategory::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable)
    *isEnable = FALSE;
    return NOERROR;
}

ECode CPreferenceCategory::OnPrepareAddPreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    if (IPreferenceCategory::Probe(preference) != NULL) {
        Slogger::E(TAG, "Cannot add a %s directly to a ", TAG.string(), TAG.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return PreferenceGroup::OnPrepareAddPreference(preference, result);
}

ECode CPreferenceCategory::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    PreferenceGroup::Init(context, attrs, defStyle);
    return NOERROR;
}

ECode CPreferenceCategory::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::preferenceCategoryStyle);
}

ECode CPreferenceCategory::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CPreferenceCategory::SetOrderingAsAdded(
    /* [in] */ Boolean orderingAsAdded)
{
    return PreferenceGroup::SetOrderingAsAdded(orderingAsAdded);
}

ECode CPreferenceCategory::IsOrderingAsAdded(
    /* [out] */ Boolean* added)
{
    return PreferenceGroup::IsOrderingAsAdded(added);
}

ECode CPreferenceCategory::GetPreferenceCount(
    /* [out] */ Int32* count)
{
    return PreferenceGroup::GetPreferenceCount(count);
}

ECode CPreferenceCategory::GetPreference(
    /* [in] */ Int32 index,
    /* [out] */ IPreference** preference)
{
    return PreferenceGroup::GetPreference(index, preference);
}

ECode CPreferenceCategory::AddPreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::AddPreference(preference, result);
}

ECode CPreferenceCategory::RemovePreference(
    /* [in] */ IPreference* preference,
    /* [out] */ Boolean* result)
{
    return PreferenceGroup::RemovePreference(preference, result);
}

ECode CPreferenceCategory::RemoveAll()
{
    return PreferenceGroup::RemoveAll();
}

ECode CPreferenceCategory::FindPreference(
    /* [in] */ ICharSequence* key,
    /* [out] */ IPreference** preference)
{
    return PreferenceGroup::FindPreference(key, preference);
}

ECode CPreferenceCategory::IsOnSameScreenAsChildren(
    /* [out] */ Boolean* isOnSameScreenAsChildren)
{
    return PreferenceGroup::IsOnSameScreenAsChildren(isOnSameScreenAsChildren);
}

ECode CPreferenceCategory::SortPreferences()
{
    return PreferenceGroup::SortPreferences();
}

} // Preference
} // Droid
} // Elastos
