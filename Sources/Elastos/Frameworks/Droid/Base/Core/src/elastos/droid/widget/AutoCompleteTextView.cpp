
#include "widget/AutoCompleteTextView.h"
#include "view/LayoutInflater.h"
#include "view/inputmethod/CCompletionInfo.h"
#include "view/inputmethod/CInputMethodManager.h"
#include "CListPopupWindow.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include <elastos/core/Math.h>

using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Text::INoCopySpan;
using Elastos::Droid::Text::EIID_INoCopySpan;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Text::EIID_ITextWatcher;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::InputMethod::CCompletionInfo;
using Elastos::Droid::View::InputMethod::CInputMethodManager;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(AutoCompleteTextView::PopupDataSetObserverOnchangedRunnable, IRunnable)
CAR_INTERFACE_IMPL(AutoCompleteTextView::PopupWindowOnDismissListener, IPopupWindowOnDismissListener)

const Boolean AutoCompleteTextView::DEBUG;
const Int32 AutoCompleteTextView::EXPAND_MAX;

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

AutoCompleteTextView::AutoCompleteTextView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mHintResource(0)
    , mThreshold(0)
    , mDropDownAnchorId(0)
    , mDropDownDismissedOnCompletion(TRUE)
    , mLastKeyCode(FALSE)
    , mOpenBefore(FALSE)
    , mBlockCompletion(FALSE)
    , mPopupCanBeUpdated(TRUE)
{
    ASSERT_SUCCEEDED(InternalInit(context, attrs, defStyle));
}

ECode AutoCompleteTextView::SetOnClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    mPassThroughClickListener->mWrapped = listener;

    return NOERROR;
}

/**
 * Private hook into the on click event, dispatched from {@link PassThroughClickListener}
 */
void AutoCompleteTextView::OnClickImpl()
{
    // If the dropdown is showing, bring the keyboard to the front
    // when the user touches the text field.
    if (IsPopupShowing()) {
        EnsureImeVisible(TRUE);
    }
}

/**
 * <p>Sets the optional hint text that is displayed at the bottom of the
 * the matching list.  This can be used as a cue to the user on how to
 * best use the list, or to provide extra information.</p>
 *
 * @param hint the text to be displayed to the user
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_completionHint
 */
ECode AutoCompleteTextView::SetCompletionHint(
    /* [in] */ ICharSequence* hint)
{
    mHintText = hint;

    if (hint != NULL) {
        if (mHintView == NULL) {
            AutoPtr<ILayoutInflater> layoutInflater;
            LayoutInflater::From(GetContext(), (ILayoutInflater**)&layoutInflater);
            AutoPtr<IView> view;
            layoutInflater->Inflate(mHintResource, NULL, (IView**)&view);
            AutoPtr<ITextView> hintView;
            view->FindViewById(R::id::text1, (IView**)&hintView);
            hintView->SetText(mHintText);
            mHintView = hintView;
            mPopup->SetPromptView(hintView);
        } else {
            mHintView->SetText(hint);
        }
    } else {
        mPopup->SetPromptView(NULL);
        mHintView = NULL;
    }
    return NOERROR;
}

/**
 * Gets the optional hint text displayed at the bottom of the the matching list.
 *
 * @return The hint text, if any
 *
 * @see #setCompletionHint(CharSequence)
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_completionHint
 */
AutoPtr<ICharSequence> AutoCompleteTextView::GetCompletionHint()
{
    return mHintText;
}

/**
 * <p>Returns the current width for the auto-complete drop down list. This can
 * be a fixed width, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill the screen, or
 * {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the width of its anchor view.</p>
 *
 * @return the width for the drop down list
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownWidth
 */
Int32 AutoCompleteTextView::GetDropDownWidth()
{
    Int32 width;
    mPopup->GetWidth(&width);
    return width;
}

/**
 * <p>Sets the current width for the auto-complete drop down list. This can
 * be a fixed width, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill the screen, or
 * {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the width of its anchor view.</p>
 *
 * @param width the width to use
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownWidth
 */
ECode AutoCompleteTextView::SetDropDownWidth(
    /* [in] */ Int32 width)
{
    mPopup->SetWidth(width);

    return NOERROR;
}

/**
 * <p>Returns the current height for the auto-complete drop down list. This can
 * be a fixed height, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill
 * the screen, or {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the height
 * of the drop down's content.</p>
 *
 * @return the height for the drop down list
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownHeight
 */
Int32 AutoCompleteTextView::GetDropDownHeight()
{
    Int32 height;
    mPopup->GetHeight(&height);
    return height;
}

/**
 * <p>Sets the current height for the auto-complete drop down list. This can
 * be a fixed height, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill
 * the screen, or {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the height
 * of the drop down's content.</p>
 *
 * @param height the height to use
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownHeight
 */
ECode AutoCompleteTextView::SetDropDownHeight(
    /* [in] */ Int32 height)
{
    mPopup->SetHeight(height);

    return NOERROR;
}

/**
 * <p>Returns the id for the view that the auto-complete drop down list is anchored to.</p>
 *
 * @return the view's id, or {@link View#NO_ID} if none specified
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownAnchor
 */
Int32 AutoCompleteTextView::GetDropDownAnchor()
{
    return mDropDownAnchorId;
}

/**
 * <p>Sets the view to which the auto-complete drop down list should anchor. The view
 * corresponding to this id will not be loaded until the next time it is needed to avoid
 * loading a view which is not yet instantiated.</p>
 *
 * @param id the id to anchor the drop down list view to
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_dropDownAnchor
 */
ECode AutoCompleteTextView::SetDropDownAnchor(
    /* [in] */ Int32 id)
{
    mDropDownAnchorId = id;
    mPopup->SetAnchorView(NULL);

    return NOERROR;
}

/**
 * <p>Gets the background of the auto-complete drop-down list.</p>
 *
 * @return the background drawable
 *
 * @attr ref android.R.styleable#PopupWindow_popupBackground
 */
AutoPtr<IDrawable> AutoCompleteTextView::GetDropDownBackground()
{
    AutoPtr<IDrawable> drawable;
    mPopup->GetBackground((IDrawable**)&drawable);

    return drawable;
}

/**
 * <p>Sets the background of the auto-complete drop-down list.</p>
 *
 * @param d the drawable to set as the background
 *
 * @attr ref android.R.styleable#PopupWindow_popupBackground
 */
ECode AutoCompleteTextView::SetDropDownBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return mPopup->SetBackgroundDrawable(d);
}

/**
 * <p>Sets the background of the auto-complete drop-down list.</p>
 *
 * @param id the id of the drawable to set as the background
 *
 * @attr ref android.R.styleable#PopupWindow_popupBackground
 */
ECode AutoCompleteTextView::SetDropDownBackgroundResource(
    /* [in] */ Int32 id)
{
    AutoPtr<IDrawable> drawable;
    GetResources()->GetDrawable(id, (IDrawable**)&drawable);

    return mPopup->SetBackgroundDrawable(drawable);
}

/**
 * <p>Sets the vertical offset used for the auto-complete drop-down list.</p>
 *
 * @param offset the vertical offset
 */
ECode AutoCompleteTextView::SetDropDownVerticalOffset(
    /* [in] */ Int32 offset)
{
    mPopup->SetVerticalOffset(offset);

    return NOERROR;
}

/**
 * <p>Gets the vertical offset used for the auto-complete drop-down list.</p>
 *
 * @return the vertical offset
 */
Int32 AutoCompleteTextView::GetDropDownVerticalOffset()
{
    Int32 offset;
    mPopup->GetVerticalOffset(&offset);

    return offset;
}

/**
 * <p>Sets the horizontal offset used for the auto-complete drop-down list.</p>
 *
 * @param offset the horizontal offset
 */
ECode AutoCompleteTextView::SetDropDownHorizontalOffset(
    /* [in] */ Int32 offset)
{
    mPopup->SetHorizontalOffset(offset);

    return NOERROR;
}

/**
 * <p>Gets the horizontal offset used for the auto-complete drop-down list.</p>
 *
 * @return the horizontal offset
 */
Int32 AutoCompleteTextView::GetDropDownHorizontalOffset()
{
    Int32 offset;
    mPopup->GetHorizontalOffset(&offset);

    return offset;
}

/**
 * <p>Sets the animation style of the auto-complete drop-down list.</p>
 *
 * <p>If the drop-down is showing, calling this method will take effect only
 * the next time the drop-down is shown.</p>
 *
 * @param animationStyle animation style to use when the drop-down appears
 *      and disappears.  Set to -1 for the default animation, 0 for no
 *      animation, or a resource identifier for an explicit animation.
 *
 * @hide Pending API council approval
 */
ECode AutoCompleteTextView::SetDropDownAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    mPopup->SetAnimationStyle(animationStyle);

    return NOERROR;
}

/**
 * <p>Returns the animation style that is used when the drop-down list appears and disappears
 * </p>
 *
 * @return the animation style that is used when the drop-down list appears and disappears
 *
 * @hide Pending API council approval
 */
Int32 AutoCompleteTextView::GetDropDownAnimationStyle()
{
    Int32 animationStyle;
    mPopup->GetAnimationStyle(&animationStyle);

    return animationStyle;
}

/**
 * @return Whether the drop-down is visible as long as there is {@link #enoughToFilter()}
 *
 * @hide Pending API council approval
 */
Boolean AutoCompleteTextView::IsDropDownAlwaysVisible()
{
    Boolean visible;
    mPopup->IsDropDownAlwaysVisible(&visible);

    return visible;
}

/**
 * Sets whether the drop-down should remain visible as long as there is there is
 * {@link #enoughToFilter()}.  This is useful if an unknown number of results are expected
 * to show up in the adapter sometime in the future.
 *
 * The drop-down will occupy the entire screen below {@link #getDropDownAnchor} regardless
 * of the size or content of the list.  {@link #getDropDownBackground()} will fill any space
 * that is not used by the list.
 *
 * @param dropDownAlwaysVisible Whether to keep the drop-down visible.
 *
 * @hide Pending API council approval
 */
ECode AutoCompleteTextView::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    mPopup->SetDropDownAlwaysVisible(dropDownAlwaysVisible);

    return NOERROR;
}

/**
 * Checks whether the drop-down is dismissed when a suggestion is clicked.
 *
 * @hide Pending API council approval
 */
Boolean AutoCompleteTextView::IsDropDownDismissedOnCompletion()
{
    return mDropDownDismissedOnCompletion;
}

/**
 * Sets whether the drop-down is dismissed when a suggestion is clicked. This is
 * TRUE by default.
 *
 * @param dropDownDismissedOnCompletion Whether to dismiss the drop-down.
 *
 * @hide Pending API council approval
 */
ECode AutoCompleteTextView::SetDropDownDismissedOnCompletion(
    /* [in] */ Boolean dropDownDismissedOnCompletion)
{
    mDropDownDismissedOnCompletion = dropDownDismissedOnCompletion;

    return NOERROR;
}

/**
 * <p>Returns the number of characters the user must type before the drop
 * down list is shown.</p>
 *
 * @return the minimum number of characters to type to show the drop down
 *
 * @see #setThreshold(Int32)
 */
Int32 AutoCompleteTextView::GetThreshold()
{
    return mThreshold;
}

/**
 * <p>Specifies the minimum number of characters the user has to type in the
 * edit box before the drop down list is shown.</p>
 *
 * <p>When <code>threshold</code> is less than or equals 0, a threshold of
 * 1 is applied.</p>
 *
 * @param threshold the number of characters to type before the drop down
 *                  is shown
 *
 * @see #getThreshold()
 *
 * @attr ref android.R.styleable#AutoCompleteTextView_completionThreshold
 */
ECode AutoCompleteTextView::SetThreshold(
    /* [in] */ Int32 threshold)
{
    if (threshold <= 0) {
        threshold = 1;
    }

    mThreshold = threshold;

    return NOERROR;
}

/**
 * <p>Sets the listener that will be notified when the user clicks an item
 * in the drop down list.</p>
 *
 * @param l the item click listener
 */
ECode AutoCompleteTextView::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* l)
{
    mItemClickListener = l;

    return NOERROR;
}

/**
 * <p>Sets the listener that will be notified when the user selects an item
 * in the drop down list.</p>
 *
 * @param l the item selected listener
 */
ECode AutoCompleteTextView::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* l)
{
    mItemSelectedListener = l;

    return NOERROR;
}

/**
 * <p>Returns the listener that is notified whenever the user clicks an item
 * in the drop down list.</p>
 *
 * @return the item click listener
 */
AutoPtr<IAdapterViewOnItemClickListener> AutoCompleteTextView::GetOnItemClickListener()
{
    return mItemClickListener;
}

/**
 * <p>Returns the listener that is notified whenever the user selects an
 * item in the drop down list.</p>
 *
 * @return the item selected listener
 */
AutoPtr<IAdapterViewOnItemSelectedListener> AutoCompleteTextView::GetOnItemSelectedListener()
{
    return mItemSelectedListener;
}

/**
 * Set a listener that will be invoked whenever the AutoCompleteTextView's
 * list of completions is dismissed.
 * @param dismissListener Listener to invoke when completions are dismissed
 */
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

/**
 * <p>Returns a filterable list adapter used for auto completion.</p>
 *
 * @return a data adapter used for auto completion
 */
AutoPtr<IListAdapter> AutoCompleteTextView::GetAdapter()
{
    return mAdapter;
}

/**
 * <p>Changes the list of data used for auto completion. The provided list
 * must be a filterable list adapter.</p>
 *
 * <p>The caller is still responsible for managing any resources used by the adapter.
 * Notably, when the AutoCompleteTextView is closed or released, the adapter is not notified.
 * A common case is the use of {@link android.widget.CursorAdapter}, which
 * contains a {@link android.database.Cursor} that must be closed.  This can be done
 * automatically (see
 * {@link android.app.Activity#startManagingCursor(android.database.Cursor)
 * startManagingCursor()}),
 * or by manually closing the cursor when the AutoCompleteTextView is dismissed.</p>
 *
 * @param adapter the adapter holding the auto completion data
 *
 * @see #getAdapter()
 * @see android.widget.Filterable
 * @see android.widget.ListAdapter
 */
ECode AutoCompleteTextView::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    if (mObserver == NULL) {
        mObserver = new PopupDataSetObserver(this);
    }
    else if (mAdapter != NULL) {
        mAdapter->UnregisterDataSetObserver(mObserver);
    }
    mAdapter = adapter;
    if (mAdapter != NULL) {
        //noinspection unchecked
        if(mFilter) mFilter = NULL;
        AutoPtr<IFilterable> fTemp = (IFilterable*)mAdapter->Probe(EIID_IFilterable);
        fTemp->GetFilter((IFilter**)&mFilter);
        adapter->RegisterDataSetObserver(mObserver);
    }
    else {
        mFilter = NULL;
    }

    mPopup->SetAdapter(mAdapter);

    return NOERROR;
}

Boolean AutoCompleteTextView::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean visible;
    mPopup->IsDropDownAlwaysVisible(&visible);
    if (keyCode == IKeyEvent::KEYCODE_BACK && IsPopupShowing()
            && !visible) {
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
            return TRUE;
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
                return TRUE;
            }
        }
    }
    return EditText::OnKeyPreIme(keyCode, event);
}

Boolean AutoCompleteTextView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
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
                return TRUE;
        }
    }

    Boolean res;
    event->HasNoModifiers(&res);
    if (IsPopupShowing() && keyCode == IKeyEvent::KEYCODE_TAB && res) {
        PerformCompletion();
        return TRUE;
    }

    return EditText::OnKeyUp(keyCode, event);
}

Boolean AutoCompleteTextView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean res;
    mPopup->OnKeyDown(keyCode, event, &res);
    if (res) {
        return TRUE;
    }

    if (!IsPopupShowing()) {
        switch(keyCode) {
            case IKeyEvent::KEYCODE_DPAD_DOWN:
                event->HasNoModifiers(&res);
                if (res) {
                    PerformValidation();
                }
        }
    }

    event->HasNoModifiers(&res);
    if (IsPopupShowing() && (keyCode == IKeyEvent::KEYCODE_TAB) && res) {
        return TRUE;
    }

    mLastKeyCode = keyCode;
    Boolean handled = EditText::OnKeyDown(keyCode, event);
    mLastKeyCode = IKeyEvent::KEYCODE_UNKNOWN;

    if (handled && IsPopupShowing()) {
        ClearListSelection();
    }

    return handled;
}

/**
 * Returns <code>TRUE</code> if the amount of text in the field meets
 * or exceeds the {@link #getThreshold} requirement.  You can override
 * this to impose a different standard for when filtering will be
 * triggered.
 */
Boolean AutoCompleteTextView::EnoughToFilter()
{
    /*if (DEBUG) Log.v(TAG, "Enough to filter: len=" + getText().length()
            + " threshold=" + mThreshold);*/

    Int32 len;
    GetText()->GetLength(&len);
    return len >= mThreshold;
}

/**
 * This is used to watch for edits to the text view.  Note that we call
 * to methods on the auto complete text view class so that we can access
 * private vars without going through thunks.
 */

AutoCompleteTextView::MyWatcher::MyWatcher(
    /* [in] */ AutoCompleteTextView* host)
    : mHost(host)
{}

PInterface AutoCompleteTextView::MyWatcher::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::MyWatcher::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::MyWatcher::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::MyWatcher::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

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

void AutoCompleteTextView::DoBeforeTextChanged()
{
    if (mBlockCompletion) return;

    // when text is changed, inserted or deleted, we attempt to show
    // the drop down
    mOpenBefore = IsPopupShowing();
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
    if (mOpenBefore && !IsPopupShowing()) {
        return;
    }

    // the drop down is shown only when a minimum number of characters
    // was typed in the text view
    if (EnoughToFilter()) {
        if (mFilter.Get() != NULL) {
            mPopupCanBeUpdated = TRUE;
            PerformFiltering(GetText(), mLastKeyCode);
        }
    }
    else {
        // drop down is automatically dismissed when enough characters
        // are deleted from the text view

        Boolean res;
        mPopup->IsDropDownAlwaysVisible(&res);
        if (!res) {
            DismissDropDown();
        }
        if (mFilter != NULL) {
            mFilter->DoFilter(NULL);
        }
    }
}

/**
 * <p>Indicates whether the popup menu is showing.</p>
 *
 * @return TRUE if the popup menu is showing, FALSE otherwise
 */
Boolean AutoCompleteTextView::IsPopupShowing()
{
    Boolean showing;
    mPopup->IsShowing(&showing);

    return showing;
}

/**
 * <p>Converts the selected item from the drop down list into a sequence
 * of character that can be used in the edit box.</p>
 *
 * @param selectedItem the item selected by the user for completion
 *
 * @return a sequence of characters representing the selected suggestion
 */
AutoPtr<ICharSequence> AutoCompleteTextView::ConvertSelectionToString(
    /* [in] */ IInterface* selectedItem)
{
    AutoPtr<ICharSequence> cs;
    mFilter->ConvertResultToString(selectedItem, (ICharSequence**)&cs);
    return cs;
}

/**
 * <p>Clear the list selection.  This may only be temporary, as user input will often bring
 * it back.
 */
ECode AutoCompleteTextView::ClearListSelection()
{
    mPopup->ClearListSelection();

    return NOERROR;
}

/**
 * Set the position of the dropdown view selection.
 *
 * @param position The position to move the selector to.
 */
ECode AutoCompleteTextView::SetListSelection(
    /* [in] */ Int32 position)
{
    mPopup->SetSelection(position);

    return NOERROR;
}

/**
 * Get the position of the dropdown view selection, if there is one.  Returns
 * {@link ListView#INVALID_POSITION ListView.INVALID_POSITION} if there is no dropdown or if
 * there is no selection.
 *
 * @return the position of the current selection, if there is one, or
 * {@link ListView#INVALID_POSITION ListView.INVALID_POSITION} if not.
 *
 * @see ListView#getSelectedItemPosition()
 */
Int32 AutoCompleteTextView::GetListSelection()
{
    Int32 pos;
    mPopup->GetSelectedItemPosition(&pos);
    return pos;
}

/**
 * <p>Starts filtering the content of the drop down list. The filtering
 * pattern is the content of the edit box. Subclasses should override this
 * method to filter with a different pattern, for instance a substring of
 * <code>text</code>.</p>
 *
 * @param text the filtering pattern
 * @param keyCode the last character inserted in the edit box; beware that
 * this will be NULL when text is being added through a soft input method.
 */
void AutoCompleteTextView::PerformFiltering(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 keyCode)
{
    mFilter->DoFilter(text, (IFilterListener*)this->Probe(EIID_IFilterListener));
}

/**
 * <p>Performs the text completion by converting the selected item from
 * the drop down list into a string, replacing the text box's content with
 * this string and finally dismissing the drop down menu.</p>
 */
ECode AutoCompleteTextView::PerformCompletion()
{
    PerformCompletion(NULL, -1, -1);

    return NOERROR;
}

ECode AutoCompleteTextView::OnCommitCompletion(
    /* [in] */ ICompletionInfo* completion)
{
    if (IsPopupShowing()) {
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
    if (IsPopupShowing()) {
        AutoPtr<IInterface> selectedItem;
        if (position < 0) {
            mPopup->GetSelectedItem((IInterface**)&selectedItem);
        } else {
            mAdapter->GetItem(position, (IInterface**)&selectedItem);
        }
        if (selectedItem == NULL) {
            //Log.w(TAG, "performCompletion: no selected item");
            return;
        }

        mBlockCompletion = TRUE;
        ReplaceText(ConvertSelectionToString(selectedItem));
        mBlockCompletion = FALSE;

        if (mItemClickListener != NULL) {

            if (selectedView == NULL || position < 0) {
                mPopup->GetSelectedView(&selectedView);
                mPopup->GetSelectedItemPosition(&position);
                mPopup->GetSelectedItemId(&id);
            }

            AutoPtr<IListView> v;
            mPopup->GetListView((IListView**)&v);

            mItemClickListener->OnItemClick(v, selectedView, position, id);
        }
    }

    Boolean visible;
    mPopup->IsDropDownAlwaysVisible(&visible);

    if (mDropDownDismissedOnCompletion && !visible) {
        DismissDropDown();
    }
}

/**
 * Identifies whether the view is currently performing a text completion, so subclasses
 * can decide whether to respond to text changed events.
 */
Boolean AutoCompleteTextView::IsPerformingCompletion()
{
    return mBlockCompletion;
}

/**
 * Like {@link #setText(CharSequence)}, except that it can disable filtering.
 *
 * @param filter If <code>FALSE</code>, no filtering will be performed
 *        as a result of this call.
 *
 * @hide Pending API council approval.
 */
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

/**
 * <p>Performs the text completion by replacing the current text by the
 * selected item. Subclasses should override this method to avoid replacing
 * the whole content of the edit box.</p>
 *
 * @param text the selected suggestion in the drop down list
 */
void AutoCompleteTextView::ReplaceText(
    /* [in] */ ICharSequence* text)
{
    ClearComposingText();

    SetText(text);
    // make sure we keep the caret at the end of the text view
    AutoPtr<IEditable> spannable = IEditable::Probe(GetText());
    assert(spannable != NULL);
    Int32 len = 0;
    spannable->GetLength(&len);
    Selection::SetSelection(spannable, len);
}

/** {@inheritDoc} */
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
    if (GetWindowVisibility() == IView::GONE) return;

    /*
     * This checks enoughToFilter() again because filtering requests
     * are asynchronous, so the result may come back after enough text
     * has since been deleted to make it no longer appropriate
     * to filter.
     */

    Boolean dropDownAlwaysVisible;
    mPopup->IsDropDownAlwaysVisible(&dropDownAlwaysVisible);

    Boolean enoughToFilter = EnoughToFilter();
    if ((count > 0 || dropDownAlwaysVisible) && enoughToFilter) {
        if (HasFocus() && HasWindowFocus() && mPopupCanBeUpdated) {
            ShowDropDown();
        }
    } else if (!dropDownAlwaysVisible && IsPopupShowing()) {
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

/**
 * <p>Closes the drop down if present on screen.</p>
 */
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


    if (IsPopupShowing()) {
        ShowDropDown();
    }

    return result;
}



/**
 * Issues a runnable to show the dropdown as soon as possible.
 *
 * @hide internal used only by SearchDialog
 */
ECode AutoCompleteTextView::ShowDropDownAfterLayout()
{
    mPopup->PostShow();

    return NOERROR;
}

/**
 * Ensures that the drop down is not obscuring the IME.
 * @param visible whether the ime should be in front. If FALSE, the ime is pushed to
 * the background.
 * @hide internal used only here and SearchDialog
 */
ECode AutoCompleteTextView::EnsureImeVisible(
    /* [in] */ Boolean visible)
{
    mPopup->SetInputMethodMode(visible
        ? IPopupWindow::INPUT_METHOD_NEEDED : IPopupWindow::INPUT_METHOD_NOT_NEEDED);

    Boolean isVisible;
    mPopup->IsDropDownAlwaysVisible(&isVisible);

    if (visible || (mFilter != NULL && EnoughToFilter())) {
        ShowDropDown();
    }

    return NOERROR;
}

/**
 * @hide internal used only here and SearchDialog
 */
Boolean AutoCompleteTextView::IsInputMethodNotNeeded()
{
    Int32 mode;
    mPopup->GetInputMethodMode(&mode);
    return mode == IPopupWindow::INPUT_METHOD_NOT_NEEDED;
}

/**
 * <p>Displays the drop down on screen.</p>
 */
ECode AutoCompleteTextView::ShowDropDown()
{
    BuildImeCompletions();

    AutoPtr<IView> view;
    mPopup->GetAnchorView((IView**)&view);
    if (view == NULL) {
        if (mDropDownAnchorId != IView::NO_ID) {
            AutoPtr<IView> anchorView;
            GetRootView()->FindViewById(mDropDownAnchorId, (IView**)&anchorView);
            mPopup->SetAnchorView(anchorView);
        } else {
            mPopup->SetAnchorView((IView*)this->Probe(EIID_IView));
        }
    }
    if (!IsPopupShowing()) {
        // Make sure the list does not obscure the IME when shown for the first time.
        mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NEEDED);
        mPopup->SetListItemExpandMax(EXPAND_MAX);
    }
    mPopup->Show();
    AutoPtr<IListView> listView;

    mPopup->GetListView((IListView**)&listView);
    listView->SetOverScrollMode(IView::OVER_SCROLL_ALWAYS);

    return NOERROR;
}

/**
 * Forces outside touches to be ignored. Normally if {@link #isDropDownAlwaysVisible()} is
 * FALSE, we allow outside touch to dismiss the dropdown. If this is set to TRUE, then we
 * ignore outside touch even when the drop down is not set to always visible.
 *
 * @hide used only by SearchDialog
 */
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
            adapter->GetCount(&count);
            count = Elastos::Core::Math::Min(count, 20);
            AutoPtr<ArrayOf<ICompletionInfo*> > completions = ArrayOf<ICompletionInfo*>::Alloc(count);
            Int32 realCount = 0;

            for (Int32 i = 0; i < count; i++) {
                Boolean enabled = FALSE;
                adapter->IsEnabled(i, &enabled);
                if (enabled) {
                    AutoPtr<IInterface> item;
                    adapter->GetItem(i, (IInterface**)&item);
                    Int64 id = 0;
                    adapter->GetItemId(i, &id);
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

/**
 * Sets the validator used to perform text validation.
 *
 * @param validator The validator used to validate the text entered in this widget.
 *
 * @see #getValidator()
 * @see #performValidation()
 */
ECode AutoCompleteTextView::SetValidator(
    /* [in] */ IValidator* validator)
{
    mValidator = validator;

    return NOERROR;
}

/**
 * Returns the Validator set with {@link #setValidator},
 * or <code>NULL</code> if it was not set.
 *
 * @see #setValidator(android.widget.AutoCompleteTextView.Validator)
 * @see #performValidation()
 */
AutoPtr<IValidator> AutoCompleteTextView::GetValidator()
{
    return mValidator;
}

/**
 * If a validator was set on this view and the current string is not valid,
 * ask the validator to fix it.
 *
 * @see #getValidator()
 * @see #setValidator(android.widget.AutoCompleteTextView.Validator)
 */
ECode AutoCompleteTextView::PerformValidation()
{
    if (mValidator == NULL) return NOERROR;

    AutoPtr<ICharSequence> text = GetText();

    Boolean res;
    mValidator->IsValid(text, &res);

    if (!TextUtils::IsEmpty(text) && !res) {
        AutoPtr<ICharSequence> str;
        mValidator->FixText(text, (ICharSequence**)&str);
        SetText(str);
    }

    return NOERROR;
}

/**
 * Returns the Filter obtained from {@link Filterable#getFilter},
 * or <code>NULL</code> if {@link #setAdapter} was not called with
 * a Filterable.
 */
AutoPtr<IFilter> AutoCompleteTextView::GetFilter()
{
    return mFilter;
}


PInterface AutoCompleteTextView::DropDownItemClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IAdapterViewOnItemClickListener) {
        return (IAdapterViewOnItemClickListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::DropDownItemClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::DropDownItemClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::DropDownItemClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::DropDownItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mHost->PerformCompletion(v, position, id);

    return NOERROR;
}



PInterface AutoCompleteTextView::PassThroughClickListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IViewOnClickListener) {
        return (IViewOnClickListener*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::PassThroughClickListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::PassThroughClickListener::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::PassThroughClickListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

/** {@inheritDoc} */
ECode AutoCompleteTextView::PassThroughClickListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->OnClickImpl();

    if (mWrapped != NULL)
        mWrapped->OnClick(v);

    return NOERROR;
}

AutoCompleteTextView::PopupDataSetObserverOnchangedRunnable::PopupDataSetObserverOnchangedRunnable(
            /* [in] */ AutoCompleteTextView* host)
{
    mHost = host;
}

ECode AutoCompleteTextView::PopupDataSetObserverOnchangedRunnable::Run()
{
   if (mHost->mAdapter != NULL) {
       Int32 count;
       mHost->mAdapter->GetCount(&count);
       mHost->UpdateDropDownForFilter(count);
   }
   return NOERROR;
}

PInterface AutoCompleteTextView::PopupDataSetObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IDataSetObserver) {
        return (IDataSetObserver*)this;
    }

    return NULL;
}

UInt32 AutoCompleteTextView::PopupDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AutoCompleteTextView::PopupDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode AutoCompleteTextView::PopupDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AutoCompleteTextView::PopupDataSetObserver::OnChanged()
{
    if (mHost->mAdapter != NULL) {
       // If the popup is not showing already, showing it will cause
       // the list of data set observers attached to the adapter to
       // change. We can't do it from here, because we are in the middle
       // of iterating throught he list of observers.
       mHost->mPopRunnable = new PopupDataSetObserverOnchangedRunnable(mHost);
       mHost->Post(mHost->mPopRunnable);
    }

    return NOERROR;
}

ECode AutoCompleteTextView::PopupDataSetObserver::OnInvalidated()
{
    return NOERROR;
}

ECode AutoCompleteTextView::InternalInit(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    CListPopupWindow::New(
        context, attrs, R::attr::autoCompleteTextViewStyle,
        (IListPopupWindow**)&mPopup);

    mPopup->SetSoftInputMode(IWindowManagerLayoutParams::SOFT_INPUT_ADJUST_RESIZE);
    mPopup->SetPromptPosition(IListPopupWindow::POSITION_PROMPT_BELOW);
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AutoCompleteTextView),
            ARRAY_SIZE(R::styleable::AutoCompleteTextView));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    a->GetInt32(R::styleable::AutoCompleteTextView_completionThreshold, 2, &mThreshold);

    AutoPtr<IDrawable> dropDownSelector;
    a->GetDrawable(R::styleable::AutoCompleteTextView_dropDownSelector,
        (IDrawable**)&dropDownSelector);
    mPopup->SetListSelector(dropDownSelector);

    Float fvalue;
    a->GetDimension(R::styleable::AutoCompleteTextView_dropDownVerticalOffset, 0.0f, &fvalue);
    Int32 dropDownVerticalOffset = (Int32)fvalue;
    mPopup->SetVerticalOffset(dropDownVerticalOffset);

    a->GetDimension(R::styleable::AutoCompleteTextView_dropDownHorizontalOffset, 0.0f, &fvalue);
    Int32 dropDownHorizontalOffset = (Int32)fvalue;
    mPopup->SetHorizontalOffset(dropDownHorizontalOffset);

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

ECode AutoCompleteTextView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(EditText::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InternalInit(context, attrs, defStyle));

    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
