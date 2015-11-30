#ifndef __ELASTOS_DROID_TEXT_METHOD_CHARACTERPICKERDIALOG_H__
#define __ELASTOS_DROID_TEXT_METHOD_CHARACTERPICKERDIALOG_H__

#include "elastos/droid/app/Dialog.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Text::Method::ICharacterPickerDialog;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IButton;


namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

class CharacterPickerDialog
    : public Dialog
    , public IAdapterViewOnItemClickListener
    , public IViewOnClickListener
    , public ICharacterPickerDialog
{
private:
    class OptionsAdapter
        : public BaseAdapter
    {
    public:
        OptionsAdapter();

        CAR_INTERFACE_DECL()

        CARAPI constructor(
            /* [in] */ IContext* context,
            /* [in] */ ICharacterPickerDialog* host);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent
            /* [out] */ IView** ret);

        CARAPI GetCount(
            /* [out] */ Int32** ret);

        CARAPI GetItem(
            /* [in] */ Int position,
            /* [out] */ IInterface** ret);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64** ret);

    private:
        ICharacterPickerDialog* mHost;
    };

public:
    CharacterPickerDialog();

    virtual ~CharacterPickerDialog();

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IView* view,
        /* [in] */ IEditable* text,
        /* [in] */ const String& options,
        /* [in] */ Boolean insert);

    CARAPI OnItemClick(
        /* [in] */ IAdapterView* parent,
        /* [in] */ IView* view,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    CARAPI OnClick(
        /* [in] */ IView* v);

protected:
    //@Override
    CARAPI_(void) OnCreate(
        /* [in] */ IBundle* savedInstanceState);

private:
    CARAPI_(void) ReplaceCharacterAndClose(
        /* [in] */ ICharSequence* replace);

private:
    AutoPtr<IView> mView;
    AutoPtr<IEditable> mText;
    String mOptions;
    Boolean mInsert;
    AutoPtr<ILayoutInflater> mInflater;
    AutoPtr<IButton> mCancelButton;
};


} // namespace Method
} // namespace Text
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_METHOD_CHARACTERPICKERDIALOG_H__