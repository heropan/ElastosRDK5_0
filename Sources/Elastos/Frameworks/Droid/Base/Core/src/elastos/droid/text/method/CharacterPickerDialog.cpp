#include "elastos/droid/text/method/CharacterPickerDialog.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/CoreUtils.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Core::CoreUtils;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

/*****************************CharacterPickerDialog::OptionsAdapter*****************************/
CharacterPickerDialog::OptionsAdapter::OptionsAdapter()
{}

CAR_INTERFACE_IMPL_4(CharacterPickerDialog::OptionsAdapter, Object, IBaseAdapter, IListAdapter, ISpinnerAdapter, IAdapter)

ECode CharacterPickerDialog::OptionsAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ICharacterPickerDialog* host)
{
    FAIL_RETURN(constructor());
    mHost = host;
    return NOERROR;
}

ECode CharacterPickerDialog::OptionsAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent
    /* [out] */ IView** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IView> v;
    mInflater->Inflate(R.layout.character_picker_button, NULL, (IView**)&v);
    AutoPtr<IButton> b = IButton::Probe(v);
    Char32 c = mOptions->GetChar(position);
    ITextView::Probe(b)->SetText(c);
    b->SetOnClickListener(IViewOnClickListener::Probe(this));
    *ret = IView::Probe(b);
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CharacterPickerDialog::OptionsAdapter::GetCount(
    /* [out] */ Int32** ret)
{
    VALIDATE_NOT_NULL(ret);
    String str = (CharacterPickerDialog*)mHost->mOptions;
    *ret = str.GetLength();
    return NOERROR;
}

ECode CharacterPickerDialog::OptionsAdapter::GetItem(
    /* [in] */ Int position,
    /* [out] */ IInterface** ret)
{
    VALIDATE_NOT_NULL(ret);
    Char32 c = mOptions->GetChar(position);
    String str;
    str.Append(c);
    AutoPtr<ICharSequence> csq = CoreUtils::Convert(str);
    *ret = (IInterface*)csq.Get();
    REFCOUNT_ADD(*ret)
    return NOERROR;
}

ECode CharacterPickerDialog::OptionsAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64** ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = (Int64)position;
    return NOERROR;
}

/*****************************CharacterPickerDialogr*****************************/

CharacterPickerDialog::CharacterPickerDialog()
{}

CharacterPickerDialog::~CharacterPickerDialog()
{}

CAR_INTERFACE_IMPL_9(CharacterPickerDialog, Object, IAdapterViewOnItemClickListener, IViewOnClickListener, ICharacterPickerDialog,
     IDialog, IDialogInterface, IWindowCallback, IKeyEventCallback, IViewOnCreateContextMenuListener, IOnWindowDismissedCallback)

ECode CharacterPickerDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IView* view,
    /* [in] */ IEditable* text,
    /* [in] */ String options,
    /* [in] */ Boolean insert)
{
    FAIL_RETURN(constructor(context, R.style.Theme_Panel));
    mView = view;
    mText = text;
    mOptions = options;
    mInsert = insert;
    return LayoutInflater::From(context, (ILayoutInflaterHelper**)&mInflater);
}

void CharacterPickerDialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    OnCreate(savedInstanceState);

    AutoPtr<IWindow> iwindow = GetWindow();
    AutoPtr<IWindowManagerLayoutParams> params;
    iwindow->GetAttributes((IWindowManagerLayoutParams**)&params));

    AutoPtr<IBinder> ibinder;
    params->SetToken((mView->GetApplicationWindowToken((IBinder**)&ibinder), ibinder));
    params->SetType(IWindowManagerLayoutParams::TYPE_APPLICATION_ATTACHED_DIALOG);
    Int32 flags;
    params->GetFlags(&flags);
    params->SetFlags(flags | IWindow::FEATURE_NO_TITLE);

    SetContentView(R.layout.character_picker);

    AutoPtr<IGridView> grid = FindViewById(R.id.characterPicker);
    AutoPtr<IContext> context = GetContext();
    AutoPtr<OptionsAdapter> adapter = new OptionsAdapter(context.Get(), this);
    grid->SetAdapter(IAdapter::Probe(adapter));
    IAdapterView::Probe(grid)->SetOnItemClickListener(IAdapterViewOnItemClickListener::Probe(this));

    AutoPtr<IGridView> bGrid = FindViewById(R.id.cancel);
    mCancelButton = IButton::Probe(bGrid);
    IView::Probe(mCancelButton)->SetOnClickListener(IViewOnClickListener::Probe(this));
}

ECode CharacterPickerDialog::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Char32 c = mOptions.GetChar(position);
    String str;
    str.Append(c);
    AutoPtr<ICharSequence> csq = CoreUtils::Convert(str);
    ReplaceCharacterAndClose(csq.Get());
    return NOERROR;
}

void CharacterPickerDialog::ReplaceCharacterAndClose(
    /* [in] */ ICharSequence* replace)
{
    Int32 selEnd = Selection::GetSelectionEnd(ICharSequence::Probe(mText));
    if (mInsert || selEnd == 0) {
        mText->Insert(selEnd, replace);
    } else {
        mText->Replace(selEnd - 1, selEnd, replace);
    }

    Dismiss();
}

ECode CharacterPickerDialog::OnClick(
    /* [in] */ IView* v)
{
    Boolean flag(FALSE);
    if ((v->Equals(mCancelButton, &flag), flag)) {
        Dismiss();
    } else if (IButton::Probe(v)) {
        AutoPtr<ICharSequence> result;
        IButton::Probe(v)->GetText((ICharSequence**)&result);
        ReplaceCharacterAndClose(result.Get());
    }
    return NOERROR;
}

} // namespace Method
} // namespace Text
} // namespace Droid
} // namespace Elastos