
#ifndef __ELASTOS_DROID_PREFERENCE_PREFERENCEGROUP_H__
#define __ELASTOS_DROID_PREFERENCE_PREFERENCEGROUP_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/preference/Preference.h"

namespace Elastos {
namespace Droid {
namespace Preference {

class PreferenceGroup
    : public Preference
    //, public IGenericInflaterParent
    , public IPreferenceGroup
{
public:
    PreferenceGroup();

    virtual ~PreferenceGroup() {}

    CAR_INTERFACE_DECL()

    virtual CARAPI SetOrderingAsAdded(
        /* [in] */  Boolean orderingAsAdded);

    virtual CARAPI IsOrderingAsAdded(
        /* [out] */ Boolean* result);

    virtual CARAPI AddItemFromInflater(
        /* [in] */  IPreference* preference);

    virtual CARAPI GetPreferenceCount(
        /* [out] */  Int32* count);

    virtual CARAPI GetPreference(
        /* [in] */  Int32 index,
        /* [out] */  IPreference** preference);

    virtual CARAPI AddPreference(
        /* [in] */  IPreference* preference,
        /* [out] */  Boolean* result);

    virtual CARAPI RemovePreference(
        /* [in] */  IPreference* preference,
        /* [out] */  Boolean* result);

    virtual CARAPI RemoveAll();

    virtual CARAPI FindPreference(
        /* [in] */  ICharSequence* key,
        /* [out] */  IPreference** preference);

    virtual CARAPI NotifyDependencyChange(
        /* [in] */ Boolean disableDependents);

protected:
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    virtual CARAPI OnPrepareAddPreference(
        /* [in] */  IPreference* preference,
        /* [out] */  Boolean* result);

    virtual CARAPI IsOnSameScreenAsChildren(
        /* [out] */  Boolean* isOnSameScreenAsChildren);

    virtual CARAPI OnAttachedToActivity();

    virtual CARAPI OnPrepareForRemoval();

    virtual CARAPI SortPreferences();

    virtual CARAPI DispatchSaveInstanceState(
        /* [in] */ IBundle* container);

    virtual CARAPI DispatchRestoreInstanceState(
        /* [in] */ IBundle* container);

private:
    CARAPI_(Boolean) RemovePreferenceInt(
        /* [in] */  IPreference* preference);

private:
    List<AutoPtr<IPreference> > mPreferenceList;

    Boolean mOrderingAsAdded;

    Int32 mCurrentPreferenceOrder;

    Boolean mAttachedToActivity;

    Object mLock;
};

} // Preference
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_PREFERENCE_PREFERENCEGROUP_H__
