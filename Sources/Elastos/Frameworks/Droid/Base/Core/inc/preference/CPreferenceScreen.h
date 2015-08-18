
#ifndef __ELASTOS_DROID_PREFERENCE_CPREFERENCESCREEN_H__
#define __ELASTOS_DROID_PREFERENCE_CPREFERENCESCREEN_H__

#include "_Elastos_Droid_Preference_CPreferenceScreen.h"
#include "PreferenceGroup.h"

using Elastos::Droid::App::IDialog;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreferenceScreen)
    , public PreferenceGroup
    , public IPreferenceScreen
    , public IAdapterViewOnItemClickListener
    , public IDialogInterfaceOnDismissListener
{
public:
    CAR_INTERFACE_DECL()

    CARAPI GetRootAdapter(
        /* [out] */ IListAdapter** adapter);

    CARAPI OnCreateRootAdapter(
        /* [out] */ IListAdapter** adapter);

    CARAPI Bind(
        /* [in] */ IListView* listView);

    CARAPI OnClick();

    CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);

    CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI SetOrderingAsAdded(
        /* [in] */ Boolean orderingAsAdded);

    CARAPI IsOrderingAsAdded(
        /* [out] */ Boolean* added);

    CARAPI GetPreferenceCount(
        /* [out] */ Int32* count);

    CARAPI GetPreference(
        /* [in] */ Int32 index,
        /* [out] */ IPreference** preference);

    CARAPI AddPreference(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    CARAPI RemovePreference(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    CARAPI RemoveAll();

    CARAPI FindPreference(
        /* [in] */ ICharSequence* key,
        /* [out] */ IPreference** preference);

    CARAPI SortPreferences();

    CARAPI OnPrepareAddPreference(
        /* [in] */ IPreference* preference,
        /* [out] */ Boolean* result);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

protected:
    CARAPI IsOnSameScreenAsChildren(
        /* [out] */ Boolean* isOnSameScreenAsChildren);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI ShowDialog(
        /* [in] */ IBundle* state);

private:
    AutoPtr<IListAdapter> mRootAdapter;
    AutoPtr<IDialog>      mDialog;
    AutoPtr<IListView>    mListView;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CPREFERENCESCREEN_H__
