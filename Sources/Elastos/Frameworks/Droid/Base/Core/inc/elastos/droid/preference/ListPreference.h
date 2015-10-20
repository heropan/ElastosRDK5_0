
#ifndef __ELASTOS_DROID_PREFERENCE_LISTPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_LISTPREFERENCE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/preference/DialogPreference.h"
#include "elastos/droid/R.h"
#include "elastos/droid/text/TextUtils.h"

using Elastos::Droid::R;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * A {@link Preference} that displays a list of entries as
* a dialog.
* <p>
* This preference will store a string into the SharedPreferences. This string will be the value
* from the {@link #setEntryValues(CharSequence[])} array.
 *
* @attr ref android.R.styleable#ListPreference_entries
* @attr ref android.R.styleable#ListPreference_entryValues
 */
class ListPreference
    : public DialogPreference
    , public IListPreference
{
private:
    class DialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        DialogInterfaceOnClickListener(
            /* [in] */ ListPreference* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        AutoPtr<ListPreference> mHost;
    };

public:
    ListPreference();

    CAR_INTERFACE_DECL()

    CARAPI SetEntries(
        /* [in] */ ArrayOf<ICharSequence*>* entries);

    CARAPI SetEntries(
        /* [in] */ Int32 entriesResId);

    CARAPI GetEntries(
        /* [out, callee] */ ArrayOf<ICharSequence*>** entries);

    CARAPI SetEntryValues(
        /* [in] */ ArrayOf<ICharSequence*>* entryValues);

    CARAPI SetEntryValues(
        /* [in] */ Int32 entryValuesResId);

    CARAPI GetEntryValues(
        /* [out, callee] */ ArrayOf<ICharSequence*>** entryValues);

    CARAPI SetValue(
        /* [in] */ const String& value);

    CARAPI GetSummary(
        /* [out] */ ICharSequence** summary);

    CARAPI SetSummary(
        /* [in] */ ICharSequence* summary);

    CARAPI SetValueIndex(
        /* [in] */ Int32 index);

    CARAPI GetValue(
        /* [out] */ String* value);

    CARAPI GetEntry(
        /* [out] */ ICharSequence** entry);

    CARAPI FindIndexOfValue(
        /* [in] */ const String& value,
        /* [out] */ Int32* index);

    CARAPI SetDialogTitle(
        /* [in] */ ICharSequence* dialogTitle);

    CARAPI SetDialogTitle(
        /* [in] */ Int32 dialogTitleResId);

    CARAPI GetDialogTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetDialogMessage(
        /* [in] */ ICharSequence* dialogMessage);

    CARAPI SetDialogMessage(
        /* [in] */ Int32 dialogMessageResId);

    CARAPI GetDialogMessage(
        /* [out] */ ICharSequence** message);

    CARAPI SetDialogIcon(
        /* [in] */ IDrawable* dialogIcon);

    CARAPI SetDialogIcon(
        /* [in] */ Int32 dialogIconRes);

    CARAPI GetDialogIcon(
        /* [out] */ IDrawable** icon);

    CARAPI SetPositiveButtonText(
        /* [in] */ ICharSequence* positiveButtonText);

    CARAPI SetPositiveButtonText(
        /* [in] */ Int32 positiveButtonTextResId);

    CARAPI GetPositiveButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetNegativeButtonText(
        /* [in] */ ICharSequence* negativeButtonText);

    CARAPI SetNegativeButtonText(
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
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyleAttr);

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

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(Int32) GetValueIndex();

private:
    AutoPtr< ArrayOf<ICharSequence*> > mEntries;
    AutoPtr< ArrayOf<ICharSequence*> >  mEntryValues;
    String mValue;
    String mSummary;
    Int32 mClickedDialogEntryIndex;
    Boolean mValueSet;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_LISTPREFERENCE_H__
