
#include "elastos/droid/widget/AutoCompleteTextView.h"
#include "elastos/droid/widget/CListPopupWindow.h"
#include "elastos/droid/R.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/text/Selection.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "elastos/droid/view/inputmethod/CCompletionInfo.h"
#include "elastos/droid/view/inputmethod/CInputMethodManager.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::R;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::INoCopySpan;
using Elastos::Droid::Text::EIID_INoCopySpan;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Text::EIID_ITextWatcher;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::InputMethod::CCompletionInfo;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(AutoCompleteTextView::MyWatcher, Object, ITextWatcher);
AutoCompleteTextView::MyWatcher::MyWatcher(
    /* [in] */ AutoCompleteTextView* host)
    : mHost(host)
{}

ECode AutoCompleteTextView::MyWatcher::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    mHost->DoAfterTextChanged();
    return NOERROR;
}

ECode AutoCompleteTextView::MyWatcher::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    mHost->DoBeforeTextChanged();
    return NOERROR;
}

ECode AutoCompleteTextView::MyWatcher::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    return NOERROR;
}

CAR_INTERFACE_IMPL(AutoCompleteTextView::DropDownItemClickListener, Object, IAdapterViewOnItemClickListener);
ECode AutoCompleteTextView::DropDownItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mHost->PerformCompletion(v, position, id);

    return NOERROR;
}

CAR_INTERFACE_IMPL(AutoCompleteTextView::PassThroughClickListener, Object, IViewOnClickListener);
AutoCompleteTextView::PassThroughClickListener::PassThroughClickListener(
    /* [in] */ AutoCompleteTextView* host)
    : mHost(host)
{}

ECode AutoCompleteTextView::PassThroughClickListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->OnClickImpl();

    if (mWrapped != NULL) {
        mWrapped->OnClick(v);
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(AutoCompleteTextView::PopupDataSetObserver::PopupDataSetObserverOnchangedRunnable, Object, IRunnable);
AutoCompleteTextView::PopupDataSetObserver::PopupDataSetObserverOnchangedRunnable::PopupDataSetObserverOnchangedRunnable(
    /* [in] */ PopupDataSetObserver* host)
    : mHost(host)
{
}

ECode AutoCompleteTextView::PopupDataSetObserver::PopupDataSetObserverOnchangedRunnable::Run()
{
    AutoPtr<IInterface> view;
    mHost->mViewReference->Resolve(EIID_IAutoCompleteTextView, (IInterface**)&view);
    AutoPtr<IAutoCompleteTextView> textView = IAutoCompleteTextView::Probe(view);
    if (textView == NULL) {
        return NOERROR;
    }
    AutoCompleteTextView* autoTextView = (AutoCompleteTextView*)textView.Get();
    AutoPtr<IListAdapter> adapter = autoTextView->mAdapter;
    if (adapter == NULL) {
        return NOERROR;
    }
    Int32 count = 0;
    IAdapter::Probe(adapter)->GetCount(&count);
    autoTextView->UpdateDropDownForFilter(count);
    return NOERROR;
}

AutoCompleteTextView::PopupDataSetObserver::PopupDataSetObserver(
    /* [in] */ IWeakReference* host)
    : mViewReference(host)
{
    mUpdateRunnable = new PopupDataSetObserverOnchangedRunnable(this);
}

ECode AutoCompleteTextView::PopupDataSetObserver::OnChanged()
{
    AutoPtr<IInterface> view;
    mViewReference->Resolve(EIID_IAutoCompleteTextView, (IInterface**)&view);
    AutoPtr<IAutoCompleteTextView> textView = IAutoCompleteTextView::Probe(view);
    if (textView != NULL && (((AutoCompleteTextView*)textView.Get())->mAdapter) != NULL) {
        // If the popup is not showing already, showing it will cause
        // the list of data set observers attached to the adapter to
        // change. We can't do it from here, because we are in the middle
        // of iterating through the list of observers.
        Boolean result = FALSE;
        IView::Probe(textView)->Post(mUpdateRunnable, &result);
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(AutoCompleteTextView::PopupWindowOnDismissListener, Object, IPopupWindowOnDismissListener);
ECode AutoCompleteTextView::PopupWindowOnDismissListener::OnDismiss()
{
    return mDismissListener->OnDismiss();
}

AutoCompleteTextView::PopupWindowOnDismissListener::PopupWindowOnDismissListener(
    /* [in] */ IAutoCompleteTextViewOnDismissListener* l)
    : mDismissListener(l)
{}


const Boolean AutoCompleteTextView::DEBUG = FALSE;
const Int32 AutoCompleteTextView::EXPAND_MAX = 3;

CAR_INTERFACE_IMPL_2(AutoCompleteTextView, EditText, IAutoCompleteTextView, IFilterListener);
AutoCompleteTextView::AutoCompleteTextView()
    : mHintResource(0)
    , mThreshold(0)
    , mDropDownAnchorId(0)
    , mDropDownDismissedOnCompletion(TRUE)
    , mLastKeyCode(FALSE)
    , mOpenBefore(FALSE)
    , mBlockCompletion(FALSE)
    , mPopupCanBeUpdated(TRUE)
{
}

ECode AutoCompleteTextView::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode AutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::autoCompleteTextViewStyle);
}

ECode AutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode AutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    ASSERT_SUCCEEDED(EditText::constructor(context, attrs, defStyleAttr, defStyleRes));
    CListPopupWindow::New(context, attrs, defStyleAttr, defStyleRes, (IListPopupWindow**)&mPopup);

    mPopup->SetSoftInputMode(IWindowManagerLayoutParams::SOFT_INPUT_ADJUST_RESIZE);
    mPopup->SetPromptPosition(IListPopupWindow::POSITION_PROMPT_BELOW);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AutoCompleteTextView),
            ARRAY_SIZE(R::styleable::AutoCompleteTextView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
            attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a);

    a->GetInt32(R::styleable::AutoCompleteTextView_completionThreshold, 2, &mThreshold);

    AutoPtr<IDrawable> dropDownSelector;
    a->GetDrawable(R::styleable::AutoCompleteTextView_dropDownSelector, (IDrawable**)&dropDownSelector);
    mPopup->SetListSelector(dropDownSelector);

    // Get the anchor's id now, but the view won't be ready, so wait to actually get the
    // view and store it in mDropDownAnchorView lazily in getDropDownAnchorView later.
    // Defaults to NO_ID, in which case the getDropDownAnchorView method will simply return
    // this TextView, as a default anchoring point.
    a->GetResourceId(R::styleable::AutoCompleteTextView_dropDownAnchor,
            IView::NO_ID, &mDropDownAnchorId);

    // For dropdown width, the developer can specify a specific width, or MATCH_PARENT
    // (for full screen width) or WRAP_CONTENT (to match the width of the anchored view).
    Int32 dropDownWidth;
    a->GetLayoutDimension(
            R::styleable::AutoCompleteTextView_dropDownWidth,
            IViewGroupLayoutParams::WRAP_CONTENT, &dropDownWidth);
    mPopup->SetWidth(dropDownWidth);

    Int32 dropDownHeight;
    a->GetLayoutDimension(
            R::styleable::AutoCompleteTextView_dropDownHeight,
            IViewGroupLayoutParams::WRAP_CONTENT, &dropDownHeight);
    mPopup->SetHeight(dropDownHeight);


    a->GetResourceId(R::styleable::AutoCompleteTextView_completionHintView,
            R::layout::simple_dropdown_hint, &mHintResource);

    AutoPtr<DropDownItemClickListener> listener = new DropDownItemClickListener(this);
    mPopup->SetOnItemClickListener(listener);


    AutoPtr<ICharSequence> completionHint;
    a->GetText(R::styleable::AutoCompleteTextView_completionHint, (ICharSequence**)&completionHint);
    SetCompletionHint(completionHint);


    // Always turn on the auto complete input type flag, since it
    // makes no sense to use this widget without it.
    Int32 inputType = GetInputType();
    if ((inputType&IInputType::TYPE_MASK_CLASS)
            == IInputType::TYPE_CLASS_TEXT) {
        inputType |= IInputType::TYPE_TEXT_FLAG_AUTO_COMPLETE;
        SetRawInputType(inputType);
    }

    a->Recycle();

    SetFocusable(TRUE);

    AutoPtr<MyWatcher> myWatcher = new MyWatcher(this);
    AddTextChangedListener(myWatcher);

    mPassThroughClickListener = new PassThroughClickListener(this);
    EditText::SetOnClickListener(mPassThroughClickListener);
    return NOERROR;
}

ECode AutoCompleteTextView::SetOnClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    mPassThroughClickListener->mWrapped = listener;

    return NOERROR;
}

void AutoCompleteTextView::OnClickImpl()
{
    // If the dropdown is showing, bring the keyboard to the front
    // when the user touches the text field.
    Boolean showing = FALSE;
    if (IsPopupShowing(&showing), showing) {
        EnsureImeVisible(TRUE);
    }
}

ECode AutoCompleteTextView::SetCompletionHint(
    /* [in] */ ICharSequence* hint)
{
    mHintText = hint;

    if (hint != NULL) {
        if (mHintView == NULL) {
            AutoPtr<IContext> context;
            GetContext((IContext**)&context);
            AutoPtr<ILayoutInflater> layoutInflater;
            LayoutInflater::From(context, (ILayoutInflater**)&layoutInflater);
            AutoPtr<IView> view;
            layoutInflater->Inflate(mHintResource, NULL, (IView**)&view);
            AutoPtr<ITextView> hintView;
            view->FindViewById(R::id::text1, (IView**)&hintView);
            hintView->SetText(mHintText);
            mHintView = hintView;
            mPopup->SetPromptView(IView::Probe(hintView));
        } else {
            mHintView->SetText(hint);
        }
    } else {
        mPopup->SetPromptView(NULL);
        mHintView = NULL;
    }
    return NOERROR;
}

ECode AutoCompleteTextView::GetCompletionHint(
    /* [out] */ ICharSequence** hint)
{
    VALIDATE_NOT_NULL(hint);
    *hint = mHintText;
    REFCOUNT_ADD(*hint);
    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return mPopup->GetWidth(width);
}

ECode AutoCompleteTextView::SetDropDownWidth(
    /* [in] */ Int32 width)
{
    mPopup->SetWidth(width);

    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return mPopup->GetHeight(height);
}

ECode AutoCompleteTextView::SetDropDownHeight(
    /* [in] */ Int32 height)
{
    mPopup->SetHeight(height);

    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownAnchor(
    /* [out] */ Int32* anchor)
{
    VALIDATE_NOT_NULL(anchor);
    *anchor = mDropDownAnchorId;
    return NOERROR;
}

ECode AutoCompleteTextView::SetDropDownAnchor(
    /* [in] */ Int32 id)
{
    mDropDownAnchorId = id;
    mPopup->SetAnchorView(NULL);

    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownBackground(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return mPopup->GetBackground(drawable);
}

ECode AutoCompleteTextView::SetDropDownBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return mPopup->SetBackgroundDrawable(d);
}

ECode AutoCompleteTextView::SetDropDownBackgroundResource(
    /* [in] */ Int32 id)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IDrawable> drawable;
    context->GetDrawable(id, (IDrawable**)&drawable);

    return mPopup->SetBackgroundDrawable(drawable);
}

ECode AutoCompleteTextView::SetDropDownVerticalOffset(
    /* [in] */ Int32 offset)
{
    mPopup->SetVerticalOffset(offset);

    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownVerticalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    return mPopup->GetVerticalOffset(offset);
}

ECode AutoCompleteTextView::SetDropDownHorizontalOffset(
    /* [in] */ Int32 offset)
{
    mPopup->SetHorizontalOffset(offset);

    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownHorizontalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    return mPopup->GetHorizontalOffset(offset);
}

ECode AutoCompleteTextView::SetDropDownAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    mPopup->SetAnimationStyle(animationStyle);

    return NOERROR;
}

ECode AutoCompleteTextView::GetDropDownAnimationStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    return mPopup->GetAnimationStyle(style);
}

ECode AutoCompleteTextView::IsDropDownAlwaysVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    return mPopup->IsDropDownAlwaysVisible(visible);
}

ECode AutoCompleteTextView::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    return mPopup->SetDropDownAlwaysVisible(dropDownAlwaysVisible);
}

ECode AutoCompleteTextView::IsDropDownDismissedOnCompletion(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDropDownDismissedOnCompletion;
    return NOERROR;
}

ECode AutoCompleteTextView::SetDropDownDismissedOnCompletion(
    /* [in] */ Boolean dropDownDismissedOnCompletion)
{
    mDropDownDismissedOnCompletion = dropDownDismissedOnCompletion;

    return NOERROR;
}

ECode AutoCompleteTextView::GetThreshold(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mThreshold;
    return NOERROR;
}

ECode AutoCompleteTextView::SetThreshold(
    /* [in] */ Int32 threshold)
{
    if (threshold <= 0) {
        threshold = 1;
    }

    mThreshold = threshold;

    return NOERROR;
}

ECode AutoCompleteTextView::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* l)
{
    mItemClickListener = l;

    return NOERROR;
}

ECode AutoCompleteTextView::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* l)
{
    mItemSelectedListener = l;

    return NOERROR;
}

ECode AutoCompleteTextView::GetItemClickListener(
    /* [out] */ IAdapterViewOnItemClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mItemClickListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode AutoCompleteTextView::GetItemSelectedListener(
    /* [out] */ IAdapterViewOnItemSelectedListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mItemSelectedListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode AutoCompleteTextView::GetOnItemClickListener(
    /* [out] */ IAdapterViewOnItemClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mItemClickListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode AutoCompleteTextView::GetOnItemSelectedListener(
    /* [out] */ IAdapterViewOnItemSelectedListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mItemSelectedListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode AutoCompleteTextView::SetOnDismissListener(
    /* [in] */ IAutoCompleteTextViewOnDismissListener* dismissListener)
{
    AutoPtr<IPopupWindowOnDismissListener> wrappedListener = NULL;
    if (dismissListener != NULL) {
        wrappedListener = new PopupWindowOnDismissListener(dismissListener);
    }
    mPopup->SetOnDismissListener(wrappedListener);
    return NOERROR;
}

ECode AutoCompleteTextView::GetAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    *adapter = mAdapter;
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

ECode AutoCompleteTextView::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    if (mObserver == NULL) {
        AutoPtr<IWeakReference> wr;
        GetWeakReference((IWeakReference**)&wr);
        mObserver = new PopupDataSetObserver(wr);
    }
    else if (mAdapter != NULL) {
        IAdapter::Probe(mAdapter)->UnregisterDataSetObserver(mObserver);
    }
    mAdapter = adapter;
    if (mAdapter != NULL) {
        //noinspection unchecked
        if(mFilter) mFilter = NULL;
        AutoPtr<IFilterable> fTemp = (IFilterable*)mAdapter->Probe(EIID_IFilterable);
        fTemp->GetFilter((IFilter**)&mFilter);
        IAdapter::Probe(adapter)->RegisterDataSetObserver(mObserver);
    }
    else {
        mFilter = NULL;
    }

    mPopup->SetAdapter(mAdapter);

    return NOERROR;
}

ECode AutoCompleteTextView::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean visible = FALSE, showing = FALSE;
    if (keyCode == IKeyEvent::KEYCODE_BACK && (IsPopupShowing(&showing), showing)
            && (mPopup->IsDropDownAlwaysVisible(&visible), !visible)) {
        // special case for the back key, we do not even try to send it
        // to the drop down list but instead, consume it immediately

        Int32 action;
        event->GetAction(&action);

        Int32 count;
        event->GetRepeatCount(&count);
        if (action == IKeyEvent::ACTION_DOWN && count == 0) {
            AutoPtr<IDispatcherState> state ;
            GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                state->StartTracking(event, this->Probe(EIID_IAutoCompleteTextView));
            }
            *result = TRUE;
            return NOERROR;
        }
        else if (action == IKeyEvent::ACTION_UP) {
            AutoPtr<IDispatcherState> state;
            GetKeyDispatcherState((IDispatcherState**)&state);
            if (state != NULL) {
                state->HandleUpEvent(event);
            }

            Boolean tracking, canceled;
            event->IsTracking(&tracking);
            event->IsCanceled(&canceled);
            if (tracking && !canceled) {
                DismissDropDown();
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    return EditText::OnKeyPreIme(keyCode, event, result);
}

ECode AutoCompleteTextView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean consumed;
    mPopup->OnKeyUp(keyCode, event, &consumed);
    if (consumed) {
        switch (keyCode) {
            // if the list accepts the key events and the key event
            // was a click, the text view gets the selected item
            // from the drop down as its content
            case IKeyEvent::KEYCODE_ENTER:
            case IKeyEvent::KEYCODE_DPAD_CENTER:
            case IKeyEvent::KEYCODE_TAB:
                Boolean res;
                event->HasNoModifiers(&res);
                if (res) {
                    PerformCompletion();
                }
                *result = TRUE;
                return NOERROR;
        }
    }

    Boolean res = FALSE, showing = FALSE;
    event->HasNoModifiers(&res);
    if ((IsPopupShowing(&showing), showing) && keyCode == IKeyEvent::KEYCODE_TAB && res) {
        PerformCompletion();
        *result = TRUE;
        return NOERROR;
    }

    return EditText::OnKeyUp(keyCode, event, result);
}

ECode AutoCompleteTextView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean res;
    mPopup->OnKeyDown(keyCode, event, &res);
    if (res) {
        *result = TRUE;
        return NOERROR;
    }

    Boolean showing = FALSE;
    if (IsPopupShowing(&showing), !showing) {
        switch(keyCode) {
            case IKeyEvent::KEYCODE_DPAD_DOWN:
                event->HasNoModifiers(&res);
                if (res) {
                    PerformValidation();
                }
        }
    }

    event->HasNoModifiers(&res);
    if ((IsPopupShowing(&showing), showing) && (keyCode == IKeyEvent::KEYCODE_TAB) && res) {
        *result = TRUE;
        return NOERROR;
    }

    mLastKeyCode = keyCode;
    Boolean handled = FALSE;
    EditText::OnKeyDown(keyCode, event, &handled);
    mLastKeyCode = IKeyEvent::KEYCODE_UNKNOWN;

    if (handled && (IsPopupShowing(&showing), showing)) {
        ClearListSelection();
    }

    *result = handled;
    return NOERROR;
}

ECode AutoCompleteTextView::EnoughToFilter(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    /*if (DEBUG) Log.v(TAG, "Enough to filter: len=" + getText().length()
            + " threshold=" + mThreshold);*/

    AutoPtr<ICharSequence> text;
    GetText((ICharSequence**)&text);
    Int32 len = 0;
    text->GetLength(&len);
    *result = len >= mThreshold;
    return NOERROR;
}

void AutoCompleteTextView::DoBeforeTextChanged()
{
    if (mBlockCompletion) return;

    // when text is changed, inserted or deleted, we attempt to show
    // the drop down
    IsPopupShowing(&mOpenBefore);
    //if (DEBUG) Log.v(TAG, "before text changed: open=" + mOpenBefore);
}

void AutoCompleteTextView::DoAfterTextChanged()
{
    if (mBlockCompletion) return;

    // if the list was open before the keystroke, but closed afterwards,
    // then something in the keystroke processing (an input filter perhaps)
    // called performCompletion() and we shouldn't do any more processing.
    /*if (DEBUG) Log.v(TAG, "after text changed: openBefore=" + mOpenBefore
            + " open=" + isPopupShowing());*/
    Boolean showing = FALSE;
    if (mOpenBefore && (IsPopupShowing(&showing), !showing)) {
        return;
    }

    // the drop down is shown only when a minimum number of characters
    // was typed in the text view
    Boolean res = FALSE;
    if (EnoughToFilter(&res), res) {
        if (mFilter.Get() != NULL) {
            mPopupCanBeUpdated = TRUE;
            AutoPtr<ICharSequence> text;
            GetText((ICharSequence**)&text);
            PerformFiltering(text, mLastKeyCode);
        }
    }
    else {
        // drop down is automatically dismissed when enough characters
        // are deleted from the text view
        mPopup->IsDropDownAlwaysVisible(&res);
        if (!res) {
            DismissDropDown();
        }
        if (mFilter != NULL) {
            mFilter->DoFilter(NULL);
        }
    }
}

ECode AutoCompleteTextView::IsPopupShowing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mPopup->IsShowing(result);
}

AutoPtr<ICharSequence> AutoCompleteTextView::ConvertSelectionToString(
    /* [in] */ IInterface* selectedItem)
{
    AutoPtr<ICharSequence> cs;
    mFilter->ConvertResultToString(selectedItem, (ICharSequence**)&cs);
    return cs;
}

ECode AutoCompleteTextView::ClearListSelection()
{
    mPopup->ClearListSelection();

    return NOERROR;
}

ECode AutoCompleteTextView::SetListSelection(
    /* [in] */ Int32 position)
{
    mPopup->SetSelection(position);

    return NOERROR;
}

ECode AutoCompleteTextView::GetListSelection(
    /* [out] */ Int32* selection)
{
    VALIDATE_NOT_NULL(selection);
    return mPopup->GetSelectedItemPosition(selection);
}

void AutoCompleteTextView::PerformFiltering(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 keyCode)
{
    mFilter->DoFilter(text, (IFilterListener*)this->Probe(EIID_IFilterListener));
}

ECode AutoCompleteTextView::PerformCompletion()
{
    PerformCompletion(NULL, -1, -1);

    return NOERROR;
}

ECode AutoCompleteTextView::OnCommitCompletion(
    /* [in] */ ICompletionInfo* completion)
{
    Boolean showing = FALSE;
    if (IsPopupShowing(&showing), showing) {
        Int32 pos;
        Boolean rst;
        completion->GetPosition(&pos);
        mPopup->PerformItemClick(pos, &rst);
    }

    return NOERROR;
}

void AutoCompleteTextView::PerformCompletion(
    /* [in] */ IView* selectedView,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Boolean showing = FALSE;
    if (IsPopupShowing(&showing), showing) {
        AutoPtr<IInterface> selectedItem;
        if (position < 0) {
            mPopup->GetSelectedItem((IInterface**)&selectedItem);
        } else {
            IAdapter::Probe(mAdapter)->GetItem(position, (IInterface**)&selectedItem);
        }
        if (selectedItem == NULL) {
            //Log.w(TAG, "performCompletion: no selected item");
            return;
        }

        mBlockCompletion = TRUE;
        ReplaceText(ConvertSelectionToString(selectedItem));
        mBlockCompletion = FALSE;

        if (mItemClickListener != NULL) {

            AutoPtr<IView> selView = selectedView;
            if (selView == NULL || position < 0) {
                mPopup->GetSelectedView((IView**)&selView);
                mPopup->GetSelectedItemPosition(&position);
                mPopup->GetSelectedItemId(&id);
            }

            AutoPtr<IListView> v;
            mPopup->GetListView((IListView**)&v);

            mItemClickListener->OnItemClick(IAdapterView::Probe(v), selView, position, id);
        }
    }

    Boolean visible = FALSE;
    mPopup->IsDropDownAlwaysVisible(&visible);

    if (mDropDownDismissedOnCompletion && !visible) {
        DismissDropDown();
    }
}

ECode AutoCompleteTextView::IsPerformingCompletion(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mBlockCompletion;
    return NOERROR;
}

ECode AutoCompleteTextView::SetText(
   /* [in] */ ICharSequence* text,
   /* [in] */ Boolean filter)
{
    if (filter) {
        SetText(text);
    }
    else {
        mBlockCompletion = TRUE;
        SetText(text);
        mBlockCompletion = FALSE;
    }

    return NOERROR;
}

void AutoCompleteTextView::ReplaceText(
    /* [in] */ ICharSequence* text)
{
    ClearComposingText();

    SetText(text);
    // make sure we keep the caret at the end of the text view
    AutoPtr<ICharSequence> t;
    GetText((ICharSequence**)&t);
    AutoPtr<IEditable> spannable = IEditable::Probe(t);
    assert(spannable != NULL);
    Int32 len = 0;
    ICharSequence::Probe(spannable)->GetLength(&len);
    Selection::SetSelection(ISpannable::Probe(spannable), len);
}

ECode AutoCompleteTextView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    UpdateDropDownForFilter(count);

    return NOERROR;
}

void AutoCompleteTextView::UpdateDropDownForFilter(
    /* [in] */ Int32 count)
{
    // Not attached to window, don't update drop-down
    Int32 visibility = 0;
    if ((GetWindowVisibility(&visibility), visibility) == IView::GONE) return;

    /*
     * This checks enoughToFilter() again because filtering requests
     * are asynchronous, so the result may come back after enough text
     * has since been deleted to make it no longer appropriate
     * to filter.
     */

    Boolean dropDownAlwaysVisible;
    mPopup->IsDropDownAlwaysVisible(&dropDownAlwaysVisible);

    Boolean enoughToFilter = FALSE;
    EnoughToFilter(&enoughToFilter);
    Boolean showing = FALSE;
    if ((count > 0 || dropDownAlwaysVisible) && enoughToFilter) {
        Boolean hasFocus = FALSE;
        HasFocus(&hasFocus);
        if (hasFocus && (HasWindowFocus(&hasFocus), hasFocus) && mPopupCanBeUpdated) {
            ShowDropDown();
        }
    }
    else if (!dropDownAlwaysVisible && (IsPopupShowing(&showing), showing)) {
        DismissDropDown();
        // When the filter text is changed, the first update from the adapter may show an empty
        // count (when the query is being performed on the network). Future updates when some
        // content has been retrieved should still be able to update the list.
        mPopupCanBeUpdated = TRUE;
    }
}

ECode AutoCompleteTextView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    EditText::OnWindowFocusChanged(hasWindowFocus);

    Boolean v;
    mPopup->IsDropDownAlwaysVisible(&v);

    if (!hasWindowFocus && !v) {
        DismissDropDown();
    }

    return NOERROR;
}

void AutoCompleteTextView::OnDisplayHint(
    /* [in] */ Int32 hint)
{
    EditText::OnDisplayHint(hint);
    switch (hint) {
        case IView::INVISIBLE:

            Boolean visible;
            mPopup->IsDropDownAlwaysVisible(&visible);
            if (!visible) {
                DismissDropDown();
            }
            break;
    }
}

void AutoCompleteTextView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    EditText::OnFocusChanged(focused, direction, previouslyFocusedRect);
    // Perform validation if the view is losing focus.
    if (!focused) {
        PerformValidation();
    }

    Boolean visible;
    mPopup->IsDropDownAlwaysVisible(&visible);

    if (!focused && !visible) {
        DismissDropDown();
    }
}

ECode AutoCompleteTextView::OnAttachedToWindow()
{
    return EditText::OnAttachedToWindow();
}

ECode AutoCompleteTextView::OnDetachedFromWindow()
{
    DismissDropDown();
    return EditText::OnDetachedFromWindow();
}

ECode AutoCompleteTextView::DismissDropDown()
{
    AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
     if (imm != NULL) {
        imm->DisplayCompletions((IView*)this->Probe(EIID_IView), NULL);
     }
    mPopup->Dismiss();
    mPopupCanBeUpdated = FALSE;
    return NOERROR;
}

Boolean AutoCompleteTextView::SetFrame(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Boolean result = EditText::SetFrame(l, t, r, b);

    Boolean showing = FALSE;
    if (IsPopupShowing(&showing), showing) {
        ShowDropDown();
    }

    return result;
}

ECode AutoCompleteTextView::ShowDropDownAfterLayout()
{
    mPopup->PostShow();

    return NOERROR;
}

ECode AutoCompleteTextView::EnsureImeVisible(
    /* [in] */ Boolean visible)
{
    mPopup->SetInputMethodMode(visible
        ? IPopupWindow::INPUT_METHOD_NEEDED : IPopupWindow::INPUT_METHOD_NOT_NEEDED);

    Boolean result = FALSE;
    mPopup->IsDropDownAlwaysVisible(&result);

    if (visible || (mFilter != NULL && (EnoughToFilter(&result), result))) {
        ShowDropDown();
    }

    return NOERROR;
}

ECode AutoCompleteTextView::IsInputMethodNotNeeded(
    /* [out] */ Boolean* result)
{
    Int32 mode = 0;
    mPopup->GetInputMethodMode(&mode);
    *result = mode == IPopupWindow::INPUT_METHOD_NOT_NEEDED;
    return NOERROR;
}

ECode AutoCompleteTextView::ShowDropDown()
{
    BuildImeCompletions();

    AutoPtr<IView> view;
    mPopup->GetAnchorView((IView**)&view);
    if (view == NULL) {
        if (mDropDownAnchorId != IView::NO_ID) {
            AutoPtr<IView> anchorView;
            AutoPtr<IView> rootView;
            GetRootView((IView**)&rootView);
            rootView->FindViewById(mDropDownAnchorId, (IView**)&anchorView);
            mPopup->SetAnchorView(anchorView);
        }
        else {
            mPopup->SetAnchorView(THIS_PROBE(IView));
        }
    }

    Boolean showing = FALSE;
    if (IsPopupShowing(&showing), !showing) {
        // Make sure the list does not obscure the IME when shown for the first time.
        mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NEEDED);
        mPopup->SetListItemExpandMax(EXPAND_MAX);
    }
    mPopup->Show();
    AutoPtr<IListView> listView;

    mPopup->GetListView((IListView**)&listView);
    IView::Probe(listView)->SetOverScrollMode(IView::OVER_SCROLL_ALWAYS);

    return NOERROR;
}

ECode AutoCompleteTextView::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    mPopup->SetForceIgnoreOutsideTouch(forceIgnoreOutsideTouch);

    return NOERROR;
}

void AutoCompleteTextView::BuildImeCompletions()
{
    AutoPtr<IListAdapter> adapter = mAdapter;
    if (adapter != NULL) {
        AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
        if (imm != NULL) {
            Int32 count = 0;
            IAdapter::Probe(adapter)->GetCount(&count);
            count = Elastos::Core::Math::Min(count, 20);
            AutoPtr<ArrayOf<ICompletionInfo*> > completions = ArrayOf<ICompletionInfo*>::Alloc(count);
            Int32 realCount = 0;

            for (Int32 i = 0; i < count; i++) {
                Boolean enabled = FALSE;
                adapter->IsEnabled(i, &enabled);
                if (enabled) {
                    AutoPtr<IInterface> item;
                    IAdapter::Probe(adapter)->GetItem(i, (IInterface**)&item);
                    Int64 id = 0;
                    IAdapter::Probe(adapter)->GetItemId(i, &id);
                    AutoPtr<ICompletionInfo> tmpInfo;
                    CCompletionInfo::New(id, realCount,
                           ConvertSelectionToString(item), (ICompletionInfo**)&tmpInfo);
                    completions->Set(realCount, tmpInfo);
                }
            }

            if (realCount != count) {
                AutoPtr<ArrayOf<ICompletionInfo*> > tmp = ArrayOf<ICompletionInfo*>::Alloc(realCount);
                tmp->Copy(completions, 0, realCount);
                completions = tmp;
            }

            imm->DisplayCompletions((IView*)this->Probe(EIID_IView), completions);
        }
    }

}

ECode AutoCompleteTextView::SetValidator(
    /* [in] */ IValidator* validator)
{
    mValidator = validator;

    return NOERROR;
}

ECode AutoCompleteTextView::GetValidator(
    /* [out] */ IValidator** validator)
{
    VALIDATE_NOT_NULL(validator);
    *validator = mValidator;
    REFCOUNT_ADD(*validator);
    return NOERROR;
}

ECode AutoCompleteTextView::PerformValidation()
{
    if (mValidator == NULL) return NOERROR;

    AutoPtr<ICharSequence> text;
    GetText((ICharSequence**)&text);

    Boolean res;
    mValidator->IsValid(text, &res);

    if (!TextUtils::IsEmpty(text) && !res) {
        AutoPtr<ICharSequence> str;
        mValidator->FixText(text, (ICharSequence**)&str);
        SetText(str);
    }

    return NOERROR;
}

AutoPtr<IFilter> AutoCompleteTextView::GetFilter()
{
    return mFilter;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
