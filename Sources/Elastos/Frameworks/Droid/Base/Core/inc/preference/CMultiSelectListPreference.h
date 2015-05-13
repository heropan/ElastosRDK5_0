
#ifndef __CMULTISELECTLISTPREFERENCE_H__
#define __CMULTISELECTLISTPREFERENCE_H__

#include "_CMultiSelectListPreference.h"
#include "DialogPreference.h"
#include <elastos/Set.h>

using Elastos::Utility::ISet;
using Elastos::Utility::Set;
using Elastos::Droid::Content::IDialogInterfaceOnMultiChoiceClickListener;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * A {@link Preference} that displays a list of entries as
* a dialog.
* <p>
* This preference will store a set of strings into the SharedPreferences.
* This set will contain one or more values from the
* {@link #setEntryValues(CharSequence[])} array.
 *
* @attr ref android.R.styleable#MultiSelectListPreference_entries
* @attr ref android.R.styleable#MultiSelectListPreference_entryValues
 */
CarClass(CMultiSelectListPreference)
    , public DialogPreference
    , public IMultiSelectListPreference
{
private:
    class MultiChoiceClickListener
        : public ElRefBase
        , public IDialogInterfaceOnMultiChoiceClickListener
    {
    public:
        MultiChoiceClickListener(
            /* [in] */ CMultiSelectListPreference* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which,
            /* [in] */ Boolean isChecked);

    private:
        CMultiSelectListPreference* mHost;
    };

public:
    CMultiSelectListPreference();

    CAR_INTERFACE_DECL()

    CARAPI SetEntries(
        /* [in] */ ArrayOf<ICharSequence*>* entries);

    CARAPI SetEntriesEx(
        /* [in] */ Int32 entriesResId);

    CARAPI GetEntries(
        /* [out, callee] */ ArrayOf<ICharSequence*>** entries);

    CARAPI SetEntryValues(
        /* [in] */ ArrayOf<ICharSequence*>* entryValues);

    CARAPI SetEntryValuesEx(
        /* [in] */ Int32 entryValuesResId);

    CARAPI GetEntryValues(
        /* [out, callee] */ ArrayOf<ICharSequence*>** entryValues);

    CARAPI SetValues(
        /* [in] */ ISet* values);

    CARAPI GetValues(
        /* [out] */ ISet** values);

    CARAPI FindIndexOfValue(
        /* [in] */ const String& value,
        /* [out] */ Int32* index);

    CARAPI SetDialogTitle(
        /* [in] */ ICharSequence* dialogTitle);

    CARAPI SetDialogTitleEx(
        /* [in] */ Int32 dialogTitleResId);

    CARAPI GetDialogTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetDialogMessage(
        /* [in] */ ICharSequence* dialogMessage);

    CARAPI SetDialogMessageEx(
        /* [in] */ Int32 dialogMessageResId);

    CARAPI GetDialogMessage(
        /* [out] */ ICharSequence** message);

    CARAPI SetDialogIcon(
        /* [in] */ IDrawable* dialogIcon);

    CARAPI SetDialogIconEx(
        /* [in] */ Int32 dialogIconRes);

    CARAPI GetDialogIcon(
        /* [out] */ IDrawable** icon);

    CARAPI SetPositiveButtonText(
        /* [in] */ ICharSequence* positiveButtonText);

    CARAPI SetPositiveButtonTextEx(
        /* [in] */ Int32 positiveButtonTextResId);

    CARAPI GetPositiveButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetNegativeButtonText(
        /* [in] */ ICharSequence* negativeButtonText);

    CARAPI SetNegativeButtonTextEx(
        /* [in] */ Int32 negativeButtonTextResId);

    CARAPI GetNegativeButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetDialogLayoutResource(
        /* [in] */ Int32 dialogLayoutResId);

    CARAPI GetDialogLayoutResource(
        /* [out] */ Int32* layoutResId);

    CARAPI ShowDialog(
        /* [in] */ IBundle* state);

    CARAPI NeedInputMethod(
        /* [out] */ Boolean* isNeed);

    CARAPI OnCreateDialogView(
        /* [out] */ IView** view);

    CARAPI OnBindDialogView(
        /* [in] */ IView* view);

    CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context);

protected:
    CARAPI OnPrepareDialogBuilder(
        /* [in] */ IAlertDialogBuilder* builder);

    CARAPI OnDialogClosed(
        /* [in] */ Boolean positiveResult);

    CARAPI OnGetDefaultValue(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [out] */ IInterface** value);

    CARAPI OnSetInitialValue(
        /* [in] */ Boolean restoreValue,
        /* [in] */ IInterface* defaultValue);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

private:
    CARAPI_(AutoPtr< ArrayOf<Boolean> >) GetSelectedItems();

private:
    AutoPtr<ArrayOf<ICharSequence*> > mEntries;
    AutoPtr<ArrayOf<ICharSequence*> > mEntryValues;
    Set<String> mValues;
    Set<String> mNewValues;
    Boolean mPreferenceChanged;
};

}
}
}

#endif // __CMULTISELECTLISTPREFERENCE_H__
