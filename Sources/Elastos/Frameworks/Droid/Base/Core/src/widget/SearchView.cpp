
#include <elastos/Math.h>
#include "widget/SearchView.h"
#include "text/TextUtils.h"
#include "text/CSpannableStringBuilder.h"
#include "text/style/CImageSpan.h"
#include "content/CIntent.h"
#include "util/CTypedValue.h"
#include "net/Uri.h"
#include "os/CBundle.h"
#include "app/CPendingIntentHelper.h"
#include "widget/SuggestionsAdapter.h"

using Elastos::Core::Math;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::Text::CSpannableStringBuilder;
using Elastos::Droid::Text::Style::IImageSpan;
using Elastos::Droid::Text::Style::CImageSpan;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::View::EIID_IOnPreDrawListener;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewOnKeyListener;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IViewOnFocusChangeListener;
using Elastos::Droid::Speech::IRecognizerIntent;
using Elastos::Droid::View::EIID_IViewOnLayoutChangeListener;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::App::ISearchManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IPendingIntentHelper;
using Elastos::Droid::App::CPendingIntentHelper;

namespace Elastos{
namespace Droid{
namespace Widget{

// {50c74954-5585-4c65-826c-665878e53611}
extern "C" const InterfaceID EIID_SearchView =
        { 0x50c74954, 0x5585, 0x4c65, { 0x82, 0x6c, 0x66, 0x58, 0x78, 0xe5, 0x36, 0x11 } };

// {7371bd5f-8785-422e-b7ba-ef1cd384e5c3}
extern "C" const InterfaceID EIID_SearchAutoComplete =
        { 0x7371bd5f, 0x8785, 0x422e, { 0xb7, 0xba, 0xef, 0x1c, 0xd3, 0x84, 0xe5, 0xc3 } };

const Boolean SearchView::DBG = FALSE;
const String SearchView::IME_OPTION_NO_MICROPHONE("nm");
const String SearchView::SEARCHVIEW_NAME = String("CSearchView");

SearchView::SearchView()
    : mIconifiedByDefault(FALSE)
    , mIconified(FALSE)
    , mSubmitButtonEnabled(FALSE)
    , mQueryRefinement(FALSE)
    , mClearingFocus(FALSE)
    , mMaxWidth(0)
    , mVoiceButtonEnabled(FALSE)
    , mExpandedInActionView(FALSE)
    , mCollapsedImeOptions(0)
{
    mShowImeRunnable = new ShowImeRunnable(this);
    mUpdateDrawableStateRunnable = new UpdateDrawableStateRunnable(this);
    mReleaseCursorRunnable = new ReleaseCursorRunnable(this);
    mOnClickListener = new SearchViewClickListener(this);
    mTextKeyListener = new SearchViewKeyListener(this);
    mOnEditorActionListener = new SearchViewEditorActionListener(this);
    mOnItemClickListener = new SearchViewItemClickListener(this);
    mOnItemSelectedListener = new SearchViewItemSelectedListener(this);
    mTextWatcher = new SearchViewTextWatcher(this);
}

SearchView::SearchView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mIconifiedByDefault(FALSE)
    , mIconified(FALSE)
    , mSubmitButtonEnabled(FALSE)
    , mQueryRefinement(FALSE)
    , mClearingFocus(FALSE)
    , mMaxWidth(0)
    , mVoiceButtonEnabled(FALSE)
    , mExpandedInActionView(FALSE)
    , mCollapsedImeOptions(0)
{
    mShowImeRunnable = new ShowImeRunnable(this);
    mUpdateDrawableStateRunnable = new UpdateDrawableStateRunnable(this);
    mReleaseCursorRunnable = new ReleaseCursorRunnable(this);
    mOnClickListener = new SearchViewClickListener(this);
    mTextKeyListener = new SearchViewKeyListener(this);
    mOnEditorActionListener = new SearchViewEditorActionListener(this);
    mOnItemClickListener = new SearchViewItemClickListener(this);
    mOnItemSelectedListener = new SearchViewItemSelectedListener(this);
    mTextWatcher = new SearchViewTextWatcher(this);

    Init(context, attrs);
}

ECode SearchView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    LinearLayout::Init(context, attrs);
    AutoPtr<IInterface> inter;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inter);
    AutoPtr<ILayoutInflater> inflater = ILayoutInflater::Probe(inter);
    AutoPtr<IView> view;
    inflater->InflateEx2(R::layout::search_view, (IViewGroup*)this->Probe(EIID_IViewGroup), TRUE, (IView**)&view);

    mSearchButton = FindViewById(R::id::search_button);
    mQueryTextView = (ISearchViewSearchAutoComplete*)(FindViewById(R::id::search_src_text).Get());
    mQueryTextView->SetSearchView((ISearchView*)this->Probe(EIID_ISearchView));

    mSearchEditFrame = FindViewById(R::id::search_edit_frame);
    mSearchPlate = FindViewById(R::id::search_plate);
    mSubmitArea = FindViewById(R::id::submit_area);
    mSubmitButton = FindViewById(R::id::search_go_btn);
    mCloseButton = IImageView::Probe(FindViewById(R::id::search_close_btn));
    mVoiceButton = FindViewById(R::id::search_voice_btn);
    mSearchHintIcon = IImageView::Probe(FindViewById(R::id::search_mag_icon));

    mSearchButton->SetOnClickListener(mOnClickListener);
    mCloseButton->SetOnClickListener(mOnClickListener);
    mSubmitButton->SetOnClickListener(mOnClickListener);
    mVoiceButton->SetOnClickListener(mOnClickListener);
    mQueryTextView->SetOnClickListener(mOnClickListener);

    mQueryTextView->AddTextChangedListener(mTextWatcher);
    mQueryTextView->SetOnEditorActionListener(mOnEditorActionListener);
    mQueryTextView->SetOnItemClickListener(mOnItemClickListener);
    mQueryTextView->SetOnItemSelectedListener(mOnItemSelectedListener);
    mQueryTextView->SetOnKeyListener(mTextKeyListener);

    AutoPtr<IViewOnFocusChangeListener> mFocusChangeListener = new SearchViewFocusChangeListener(this);
    mQueryTextView->SetOnFocusChangeListener(mFocusChangeListener);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::SearchView),
            ARRAY_SIZE(R::styleable::SearchView));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs, attrIds, 0, 0, (ITypedArray**)&a));

    Boolean byDefault = FALSE;
    a->GetBoolean(R::styleable::SearchView_iconifiedByDefault, TRUE, &byDefault);
    SetIconifiedByDefault(byDefault);
    Int32 maxWidth = 0;
    a->GetDimensionPixelSize(R::styleable::SearchView_maxWidth, -1, &maxWidth);
    if (maxWidth != -1) {
        SetMaxWidth(maxWidth);
    }

    AutoPtr<ICharSequence> queryHint;
    a->GetText(R::styleable::SearchView_queryHint, (ICharSequence**)&queryHint);
    if (!TextUtils::IsEmpty(queryHint)) {
        SetQueryHint(queryHint);
    }

    Int32 imeOptions = 0;
    a->GetInt32(R::styleable::SearchView_imeOptions, -1, &imeOptions);
    if (imeOptions != -1) {
        SetImeOptions(imeOptions);
    }

    Int32 inputType = 0;
    a->GetInt32(R::styleable::SearchView_inputType, -1, &inputType);
    if (inputType != -1) {
        SetInputType(inputType);
    }

    a->Recycle();

    Boolean focusable = TRUE;
    attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::View),
            ARRAY_SIZE(R::styleable::View));
    a = NULL;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(attrs, attrIds, 0, 0, (ITypedArray**)&a));

    a->GetBoolean(R::styleable::View_focusable, focusable, &focusable);
    a->Recycle();
    SetFocusable(focusable);

    CIntent::New(IRecognizerIntent::ACTION_WEB_SEARCH, (IIntent**)&mVoiceWebSearchIntent);
    mVoiceWebSearchIntent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    mVoiceWebSearchIntent->PutStringExtra(IRecognizerIntent::EXTRA_LANGUAGE_MODEL, IRecognizerIntent::LANGUAGE_MODEL_WEB_SEARCH);

    CIntent::New(IRecognizerIntent::ACTION_RECOGNIZE_SPEECH, (IIntent**)&mVoiceAppSearchIntent);
    mVoiceAppSearchIntent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);

    Int32 index = 0;
    mQueryTextView->GetDropDownAnchor(&index);
    mDropDownAnchor = FindViewById(index);
    if (mDropDownAnchor) {
        AutoPtr<IViewOnLayoutChangeListener> mLayoutChangeListener = new SearchViewLayoutChangeListener(this);
        mDropDownAnchor->AddOnLayoutChangeListener(mLayoutChangeListener);
    }

    UpdateViewsVisibility(mIconifiedByDefault);
    UpdateQueryHint();
    return NOERROR;
}

ECode SearchView::SetSearchableInfo(
    /* [in] */ ISearchableInfo* searchable)
{
    mSearchable = searchable;
    if(mSearchable)
    {
        UpdateSearchAutoComplete();
        UpdateQueryHint();
    }
    //Cache the voice search capability
    mVoiceButtonEnabled = HasVoiceSearch();

    if (mVoiceButtonEnabled)
    {
        // Disable the microphone on the keyboard, as a mic is displayed near the text box
        // TODO: use imeOptions to disable voice input when the new API will be available
        mQueryTextView->SetPrivateImeOptions(IME_OPTION_NO_MICROPHONE);
    }
    UpdateViewsVisibility(IsIconified());
    return NOERROR;
}

ECode SearchView::SetAppSearchData(
    /* [in] */ IBundle* appSearchData)
{
    mAppSearchData = appSearchData;
    return NOERROR;
}

ECode SearchView::SetImeOptions(
    /* [in] */ Int32 imeOptions)
{
    mQueryTextView->SetImeOptions(imeOptions);
    return NOERROR;
}

Int32 SearchView::GetImeOptions()
{
    Int32 options = 0;
    mQueryTextView->GetImeOptions(&options);
    return options;
}

ECode SearchView::SetInputType(
    /* [in] */ Int32 inputType)
{
    mQueryTextView->SetInputType(inputType);
    return NOERROR;
}

Int32 SearchView::GetInputType()
{
    Int32 type = 0;
    mQueryTextView->GetInputType(&type);
    return type;
}

Boolean SearchView::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    // Don't accept focus if in the middle of clearing focus
    if (mClearingFocus) return FALSE;
    // Check if SearchView is focusable.
    if (!IsFocusable()) return FALSE;
    // If it is not iconified, then give the focus to the text field
    if (!IsIconified()) {
        Boolean result = FALSE;
        mQueryTextView->RequestFocusEx2(direction, previouslyFocusedRect, &result);
        if (result) {
            UpdateViewsVisibility(FALSE);
        }
        return result;
    } else {
        LinearLayout::RequestFocus(direction, previouslyFocusedRect);
    }

    return FALSE;
}

ECode SearchView::ClearFocus()
{
    mClearingFocus = TRUE;
    SetImeVisibility(FALSE);
    LinearLayout::ClearFocus();
    mQueryTextView->ClearFocus();
    mClearingFocus = FALSE;
    return NOERROR;
}

ECode SearchView::SetOnQueryTextListener(
    /* [in] */ ISearchViewOnQueryTextListener* listener)
{
    mOnQueryChangeListener = listener;
    return NOERROR;
}

ECode SearchView::SetOnCloseListener(
    /* [in] */ IOnCloseListener* listener)
{
    mOnCloseListener = listener;
    return NOERROR;
}

ECode SearchView::SetOnQueryTextFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener* listener)
{
    mOnQueryTextFocusChangeListener = listener;
    return NOERROR;
}

ECode SearchView::SetOnSuggestionListener(
    /* [in] */ ISearchViewOnSuggestionListener* listener)
{
    mOnSuggestionListener = listener;
    return NOERROR;
}

ECode SearchView::SetOnSearchClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    mOnSearchClickListener = listener;
    return NOERROR;
}

AutoPtr<ICharSequence> SearchView::GetQuery()
{
    AutoPtr<ICharSequence> csq;
    mQueryTextView->GetText((ICharSequence**)&csq);
    return csq;
}

ECode SearchView::SetQuery(
    /* [in] */ ICharSequence* query,
    /* [in] */ Boolean submit)
{
    mQueryTextView->SetText(query);
    if (query) {
        Int32 length = 0;
        mQueryTextView->GetLength(&length);
        mQueryTextView->SetSelectionEx(length);
        mUserQuery = query;
    }

    // If the query is not empty and submit is requested, submit the query
    if (submit && !TextUtils::IsEmpty(query)) {
        OnSubmitQuery();
    }
    return NOERROR;
}

ECode SearchView::SetQueryHint(
    /* [in] */ ICharSequence* hint)
{
    mQueryHint = hint;
    UpdateQueryHint();
    return NOERROR;
}

AutoPtr<ICharSequence> SearchView::GetQueryHint()
{

    if (mQueryHint) {
        return mQueryHint;
    } else if (mSearchable) {
        AutoPtr<ICharSequence> hint;
        Int32 hintId = 0;
        mSearchable->GetHintId(&hintId);
        if (hintId != 0) {
            String str;
            GetContext()->GetString(hintId, &str);
            CStringWrapper::New(str, (ICharSequence**)&hint);
        }
        return hint;
    }
    return NULL;
}

ECode SearchView::SetIconifiedByDefault(
    /* [in] */ Boolean iconified)
{
    if (mIconifiedByDefault == iconified) {
        return NOERROR;
    }
    mIconifiedByDefault = iconified;
    UpdateViewsVisibility(iconified);
    UpdateQueryHint();
    return NOERROR;
}

Boolean SearchView::IsIconfiedByDefault()
{
    return mIconifiedByDefault;
}

ECode SearchView::SetIconified(
    /* [in] */ Boolean iconify)
{
    if (iconify) {
        OnCloseClicked();
    } else {
        OnSearchClicked();
    }
    return NOERROR;
}

Boolean SearchView::IsIconified()
{
    return mIconified;
}

ECode SearchView::SetSubmitButtonEnabled(
    /* [in] */ Boolean enabled)
{
    mSubmitButtonEnabled = enabled;
    UpdateViewsVisibility(IsIconified());
    return NOERROR;
}

Boolean SearchView::IsSubmitButtonEnabled()
{
    return mSubmitButtonEnabled;
}

ECode SearchView::SetQueryRefinementEnabled(
    /* [in] */ Boolean enable)
{
    mQueryRefinement = enable;
    if (ISuggestionsAdapter::Probe(mSuggestionsAdapter)) {
        AutoPtr<ISuggestionsAdapter> adapter = ISuggestionsAdapter::Probe(mSuggestionsAdapter);
        adapter->SetQueryRefinement(enable ? ISuggestionsAdapter::REFINE_ALL : ISuggestionsAdapter::REFINE_BY_ENTRY);
    }
    return NOERROR;
}

Boolean SearchView::IsQueryRefinementEnabled()
{
    return mQueryRefinement;
}

ECode SearchView::SetSuggestionsAdapter(
    /* [in] */ ICursorAdapter* adapter)
{
    mSuggestionsAdapter = adapter;
    mQueryTextView->SetAdapter(mSuggestionsAdapter);
    return NOERROR;
}

AutoPtr<ICursorAdapter> SearchView::GetSuggestionsAdapter()
{
    return mSuggestionsAdapter;
}

ECode SearchView::SetMaxWidth(
    /* [in] */ Int32 maxpixels)
{
    mMaxWidth = maxpixels;
    RequestLayout();
    return NOERROR;
}

Int32 SearchView::GetMaxWidth()
{
    return mMaxWidth;
}

void SearchView::OnQueryRefine(
    /* [in] */ ICharSequence* queryText)
{
    SetQuery(queryText);
}

Boolean SearchView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (mSearchable == NULL) {
        return FALSE;
    }

    // if it's an action specified by the searchable activity, launch the
    // entered query with the action key
    AutoPtr<IActionKeyInfo> actionKey;
    mSearchable->FindActionKey(keyCode, (IActionKeyInfo**)&actionKey);
    String actionMsg;
    actionKey->GetQueryActionMsg(&actionMsg);
    if (actionKey && (actionMsg != NULL)) {
        AutoPtr<ICharSequence> csq;
        mQueryTextView->GetText((ICharSequence**)&csq);
        String text;
        csq->ToString(&text);
        LaunchQuerySearch(keyCode, actionMsg, text);
        return TRUE;
    }
    return LinearLayout::OnKeyDown(keyCode, event);
}

Boolean SearchView::OnSuggestionsKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // guard against possible race conditions (late arrival after dismiss)
    if (mSearchable == NULL) return FALSE;
    if (mSuggestionsAdapter == NULL) return FALSE;

    Int32 action = 0;
    event->GetAction(&action);
    Boolean hasNoModifiers = FALSE;
    event->HasNoModifiers(&hasNoModifiers);
    if (action == IKeyEvent::ACTION_DOWN && hasNoModifiers) {
        // First, check for enter or search (both of which we'll treat as a
        // "click")
        if (keyCode == IKeyEvent::KEYCODE_ENTER || keyCode == IKeyEvent::KEYCODE_SEARCH || keyCode == IKeyEvent::KEYCODE_TAB) {
            Int32 position = 0;
            mQueryTextView->GetListSelection(&position);
            return OnItemClicked(position, IKeyEvent::KEYCODE_UNKNOWN, String(NULL));
        }

        if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT || keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
            // give "focus" to text editor, with cursor at the beginning if
            // left key, at end if right key
            // TODO: Reverse left/right for right-to-left languages, e.g.
            // Arabic
            Int32 length = 0;
            mQueryTextView->GetLength(&length);
            Int32 selPoint = (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT) ? 0 : length;

            mQueryTextView->SetSelectionEx(selPoint);
            mQueryTextView->SetListSelection(0);
            mQueryTextView->ClearListSelection();
            mQueryTextView->EnsureImeVisible(TRUE);

            return TRUE;
        }

        // Next, check for an "up and out" move
        Int32 selection = 0;
        mQueryTextView->GetListSelection(&selection);
        if ( keyCode == IKeyEvent::KEYCODE_DPAD_UP && 0 == selection) {
            // TODO: restoreUserQuery();
            // let ACTV complete the move
            return FALSE;
        }

        // Next, check for an "action key"
        AutoPtr<IActionKeyInfo> actionKey;
        mSearchable->FindActionKey(keyCode, (IActionKeyInfo**)&actionKey);
        String aMsg, column;
        actionKey->GetSuggestActionMsg(&aMsg);
        actionKey->GetSuggestActionMsgColumn(&column);
        if(actionKey && ((aMsg != NULL) || (column != NULL))) {
            Int32 position = 0;
            mQueryTextView->GetListSelection(&position);
            if (position != IListView::INVALID_POSITION) {
                AutoPtr<ICursor> c;
                ICursorFilterClient::Probe(mSuggestionsAdapter)->GetCursor((ICursor**)&c);
                Boolean result = FALSE;
                c->MoveToPosition(position, &result);
                if (result) {
                    String actionMsg = GetActionKeyMessage(c, actionKey);
                    if (actionMsg != NULL && actionMsg.GetLength() > 0) {
                        return OnItemClicked(position, keyCode, actionMsg);
                    }
                }
            }
        }
    }

    return FALSE;
}

ECode SearchView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    LinearLayout::OnWindowFocusChanged(hasWindowFocus);

    PostUpdateFocusedState();
    return NOERROR;
}

ECode SearchView::OnActionViewCollapsed()
{
    ClearFocus();
    UpdateViewsVisibility(TRUE);
    mQueryTextView->SetImeOptions(mCollapsedImeOptions);
    mExpandedInActionView = FALSE;
    return NOERROR;
}

ECode SearchView::OnActionViewExpanded()
{
    if (mExpandedInActionView)
        return NOERROR;

    mExpandedInActionView = TRUE;
    mQueryTextView->GetImeOptions(&mCollapsedImeOptions);
    mQueryTextView->SetImeOptions(mCollapsedImeOptions | IEditorInfo::IME_FLAG_NO_FULLSCREEN);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String(""), (ICharSequence**)&csq);
    mQueryTextView->SetText(csq);
    SetIconified(FALSE);
    return NOERROR;
}

ECode SearchView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(SEARCHVIEW_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode SearchView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    LinearLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(SEARCHVIEW_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

ECode SearchView::OnRtlPropertiesChanged(
    /* [in] */ Int32 layoutDirection)
{
    mQueryTextView->SetLayoutDirection(layoutDirection);
    return NOERROR;
}

Boolean SearchView::IsLandscapeMode(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<IConfiguration> config;
    resources->GetConfiguration((IConfiguration**)&config);
    Int32 orientation = 0;
    config->GetOrientation(&orientation);
    return orientation == IConfiguration::ORIENTATION_LANDSCAPE;
}

void SearchView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (IsIconified()) {
        LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
        return;
    }

    Int32 widthMode = View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 width = View::MeasureSpec::GetSize(widthMeasureSpec);

    switch (widthMode) {
        case View::MeasureSpec::AT_MOST:
            if (mMaxWidth > 0) {
                width = Elastos::Core::Math::Min(mMaxWidth, width);
            } else {
                width = Elastos::Core::Math::Min(GetPreferredWidth(), width);
            }
            break;

        case View::MeasureSpec::EXACTLY:
            if (mMaxWidth > 0) {
                width = Elastos::Core::Math::Min(mMaxWidth, width);
            }
            break;

        case View::MeasureSpec::UNSPECIFIED:
            width = mMaxWidth > 0 ? mMaxWidth : GetPreferredWidth();
            break;
    }
    widthMode = View::MeasureSpec::EXACTLY;
    LinearLayout::OnMeasure(View::MeasureSpec::MakeMeasureSpec(width, widthMode), heightMeasureSpec);
}

ECode SearchView::OnDetachedFromWindow()
{
    RemoveCallbacks(mUpdateDrawableStateRunnable);
    Post(mReleaseCursorRunnable);
    LinearLayout::OnDetachedFromWindow();
    return NOERROR;
}

Int32 SearchView::GetPreferredWidth()
{
    AutoPtr<IResources> res;
    GetContext()->GetResources((IResources**)&res);
    Int32 size = 0;
    res->GetDimensionPixelSize(R::dimen::search_view_preferred_width, &size);
    return size;
}

void SearchView::UpdateViewsVisibility(
    /* [in] */ Boolean collapsed)
{
    mIconified = collapsed;
    Int32 visCollapsed = collapsed ? IView::VISIBLE : IView::GONE;
    AutoPtr<ICharSequence> csq;
    mQueryTextView->GetText((ICharSequence**)&csq);
    Boolean hasText = !TextUtils::IsEmpty(csq);

    mSearchButton->SetVisibility(visCollapsed);
    UpdateSubmitButton(hasText);
    mSearchEditFrame->SetVisibility(collapsed ? IView::GONE : IView::VISIBLE);
    mSearchHintIcon->SetVisibility(mIconifiedByDefault ? IView::GONE : IView::VISIBLE);
    UpdateCloseButton();
    UpdateVoiceButton(!hasText);
    UpdateSubmitArea();
}

Boolean SearchView::HasVoiceSearch()
{
    Boolean enable = FALSE;
    mSearchable->GetVoiceSearchEnabled(&enable);
    if (mSearchable && enable) {
        AutoPtr<IIntent> testIntent;
        Boolean webSearch = FALSE, recognizer = FALSE;
        mSearchable->GetVoiceSearchLaunchWebSearch(&webSearch);
        mSearchable->GetVoiceSearchLaunchRecognizer(&recognizer);
        if (webSearch) {
            testIntent = mVoiceWebSearchIntent;
        } else if (recognizer) {
            testIntent = mVoiceAppSearchIntent;
        }
        if (testIntent) {
            AutoPtr<IPackageManager> pm;
            GetContext()->GetPackageManager((IPackageManager**)&pm);
            AutoPtr<IResolveInfo> ri;
            pm->ResolveActivity(testIntent, IPackageManager::MATCH_DEFAULT_ONLY, (IResolveInfo**)&ri);
            return ri != NULL;
        }
    }
    return FALSE;
}

Boolean SearchView::IsSubmitAreaEnabled()
{
    return (mSubmitButtonEnabled || mVoiceButtonEnabled) && !IsIconified();
}

void SearchView::UpdateSubmitButton(
    /* [in] */ Boolean hasText)
{
    Int32 visibility = IView::GONE;
    if (mSubmitButtonEnabled && IsSubmitAreaEnabled() && HasFocus()
        && (hasText || !mVoiceButtonEnabled)) {
        visibility = IView::VISIBLE;
    }
    mSubmitButton->SetVisibility(visibility);
}

void SearchView::UpdateSubmitArea()
{
    Int32 visibility = IView::GONE, sub = 0, voice = 0;
    mSubmitButton->GetVisibility(&sub);
    mVoiceButton->GetVisibility(&voice);
    if (IsSubmitAreaEnabled() && (sub == IView::VISIBLE || voice == IView::VISIBLE)){
        visibility = IView::VISIBLE;
    }
    mSubmitArea->SetVisibility(visibility);
}

void SearchView::UpdateCloseButton()
{
    AutoPtr<ICharSequence> csq;
    mQueryTextView->GetText((ICharSequence**)&csq);
    Boolean hasText = !TextUtils::IsEmpty(csq);
    Boolean showClose = hasText || (mIconifiedByDefault && !mExpandedInActionView);
    mCloseButton->SetVisibility(showClose ? IView::VISIBLE : IView::GONE);
    AutoPtr<IDrawable> drawable;
    mCloseButton->GetDrawable((IDrawable**)&drawable);
    Boolean res = FALSE;
    drawable->SetState(hasText ? View::ENABLED_STATE_SET : View::EMPTY_STATE_SET, &res);
}

void SearchView::PostUpdateFocusedState()
{
    Post(mUpdateDrawableStateRunnable);
}

void SearchView::UpdateFocusedState()
{
    Boolean focused = FALSE, res = FALSE;
    mQueryTextView->HasFocus(&focused);
    AutoPtr<IDrawable> plate;
    mSearchPlate->GetBackground((IDrawable**)&plate);
    plate->SetState(focused ? View::FOCUSED_STATE_SET : View::EMPTY_STATE_SET, &res);
    AutoPtr<IDrawable> area;
    mSubmitArea->GetBackground((IDrawable**)&area);
    area->SetState(focused ? View::FOCUSED_STATE_SET : View::EMPTY_STATE_SET, &res);
    Invalidate();
}

void SearchView::SetImeVisibility(
    /* [in] */ Boolean visible)
{
    if (visible) {
        Post(mShowImeRunnable);
    } else {
        RemoveCallbacks(mShowImeRunnable);
        AutoPtr<IInterface> inter;
        GetContext()->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInterface**)&inter);
        AutoPtr<IInputMethodManager> imm = IInputMethodManager::Probe(inter);
        if (imm) {
            Boolean res = FALSE;
            imm->HideSoftInputFromWindow(GetWindowToken(), 0, &res);
        }
    }
}

String SearchView::GetActionKeyMessage(
   /* [in] */ ICursor* c,
   /* [in] */ IActionKeyInfo* actionKey)
{
   return String(NULL);
}

Int32 SearchView::GetSearchIconId()
{
    AutoPtr<ITypedValue> outValue;
    CTypedValue::New((ITypedValue**)&outValue);
    AutoPtr<IResourcesTheme> theme;
    GetContext()->GetTheme((IResourcesTheme**)&theme);
    Boolean res = FALSE;
    theme->ResolveAttribute(R::attr::searchViewSearchIcon, outValue, TRUE, &res);
    Int32 resourcesId = 0;
    outValue->GetResourceId(&resourcesId);
    return resourcesId;
}

AutoPtr<ICharSequence> SearchView::GetDecoratedHint(
    /* [in] */ ICharSequence* hintText)
{
    if (!mIconifiedByDefault) return hintText;

    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("   "), (ICharSequence**)&csq);
    AutoPtr<ISpannableStringBuilder> ssb;
    CSpannableStringBuilder::New(csq, (ISpannableStringBuilder**)&ssb);
    ssb->Append(hintText);
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);
    AutoPtr<IDrawable> searchIcon;
    resources->GetDrawable(GetSearchIconId(), (IDrawable**)&searchIcon);
    Float size = 0;
    mQueryTextView->GetTextSize(&size);
    Int32 textSize = (Int32)(size * 1.25);
    searchIcon->SetBounds(0, 0, textSize, textSize);
    AutoPtr<IImageSpan> span;
    CImageSpan::New(searchIcon, (IImageSpan**)&span);
    ssb->SetSpan(span, 1, 2, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);

    return ssb;
}

void SearchView::UpdateQueryHint()
{
    if (mQueryHint) {
        mQueryTextView->SetHint(GetDecoratedHint(mQueryHint));
    } else if (mSearchable) {
        AutoPtr<ICharSequence> hint;
        Int32 hintId = 0;
        mSearchable->GetHintId(&hintId);
        if (hintId != 0) {
            String str;
            GetContext()->GetString(hintId, &str);
            CStringWrapper::New(str, (ICharSequence**)&hint);
        }
        if (hint) {
            mQueryTextView->SetHint(GetDecoratedHint(hint));
        }
    } else {
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(String(""), (ICharSequence**)&csq);
        mQueryTextView->SetHint(csq);
    }
}

void SearchView::UpdateSearchAutoComplete()
{
    mQueryTextView->SetDropDownAnimationStyle(0); // no animation
    Int32 threshold = 0, options = 0;
    mSearchable->GetSuggestThreshold(&threshold);
    mSearchable->GetImeOptions(&options);
    mQueryTextView->SetThreshold(threshold);
    mQueryTextView->SetImeOptions(options);

    Int32 inputType = 0;
    mSearchable->GetInputType(&inputType);
    // We only touch this if the input type is set up for text (which it almost certainly
    // should be, in the case of search!)
    if ((inputType & IInputType::TYPE_MASK_CLASS) == IInputType::TYPE_CLASS_TEXT) {
        // The existence of a suggestions authority is the proxy for "suggestions
        // are available here"
        inputType &= ~IInputType::TYPE_TEXT_FLAG_AUTO_COMPLETE;
        String str;
        mSearchable->GetSuggestAuthority(&str);
        if (str != NULL) {
            inputType |= IInputType::TYPE_TEXT_FLAG_AUTO_COMPLETE;
            // TYPE_TEXT_FLAG_AUTO_COMPLETE means that the text editor is performing
            // auto-completion based on its own semantics, which it will present to the user
            // as they type. This generally means that the input method should not show its
            // own candidates, and the spell checker should not be in action. The text editor
            // supplies its candidates by calling InputMethodManager.displayCompletions(),
            // which in turn will call InputMethodSession.displayCompletions().
            inputType |= IInputType::TYPE_TEXT_FLAG_NO_SUGGESTIONS;
        }
    }
    mQueryTextView->SetInputType(inputType);
    if (mSuggestionsAdapter) {
        (ICursorFilterClient::Probe(mSuggestionsAdapter))->ChangeCursor(NULL);
    }
    // attach the suggestions adapter, if suggestions are available
    // The existence of a suggestions authority is the proxy for "suggestions available here"
    String search;
    mSearchable->GetSuggestAuthority(&search);
    if (search != NULL)
    {
        mSuggestionsAdapter = new SuggestionsAdapter(GetContext(),
                     (ISearchView*)(this->Probe(EIID_ISearchView)), mSearchable, 0);
        mQueryTextView->SetAdapter(mSuggestionsAdapter);
        (ISuggestionsAdapter::Probe(mSuggestionsAdapter))->SetQueryRefinement(
                mQueryRefinement ? ISuggestionsAdapter::REFINE_ALL : ISuggestionsAdapter::REFINE_BY_ENTRY);
    }
}

void SearchView::UpdateVoiceButton(
    /* [in] */ Boolean empty)
{
    Int32 visibility = IView::GONE;
    if (mVoiceButtonEnabled && !IsIconified() && empty) {
        visibility = IView::VISIBLE;
        mSubmitButton->SetVisibility(IView::GONE);
    }
    mVoiceButton->SetVisibility(visibility);
}

void SearchView::OnTextChanged(
    /* [in] */ ICharSequence* newText)
{
    AutoPtr<ICharSequence> text;
    mQueryTextView->GetText((ICharSequence**)&text);
    mUserQuery = text;
    Boolean hasText = !TextUtils::IsEmpty(text);
    UpdateSubmitButton(hasText);
    UpdateVoiceButton(!hasText);
    UpdateCloseButton();
    UpdateSubmitArea();
    if (mOnQueryChangeListener && !TextUtils::Equals(newText, mOldQueryText)) {
        String str;
        newText->ToString(&str);
        Boolean res = FALSE;
        mOnQueryChangeListener->OnQueryTextChange(str, &res);
    }
    mOldQueryText = newText;
}

void SearchView::OnSubmitQuery()
{
    AutoPtr<ICharSequence> query;
    mQueryTextView->GetText((ICharSequence**)&query);
    if (query && TextUtils::GetTrimmedLength(query) > 0) {
        Boolean res = FALSE;
        String str;
        query->ToString(&str);
        mOnQueryChangeListener->OnQueryTextSubmit(str, &res);
        if (!mOnQueryChangeListener || !res) {
            if (FALSE /*mSearchable*/) {
                LaunchQuerySearch(IKeyEvent::KEYCODE_UNKNOWN, String(NULL), str);
                SetImeVisibility(FALSE);
            }
            DismissSuggestions();
        }
    }
}

void SearchView::DismissSuggestions()
{
    mQueryTextView->DismissDropDown();
}

void SearchView::OnCloseClicked()
{
    AutoPtr<ICharSequence> text;
    mQueryTextView->GetText((ICharSequence**)&text);
    if (TextUtils::IsEmpty(text)) {
        if (mIconifiedByDefault) {
            Boolean res = FALSE;
            mOnCloseListener->OnClose(&res);
            if (!mOnCloseListener || !res) {
                ClearFocus();
                UpdateViewsVisibility(TRUE);
            }
        }
    } else {
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(String(""), (ICharSequence**)&csq);
        mQueryTextView->SetText(csq);
        Boolean result = false;
        mQueryTextView->RequestFocus(&result);
        SetImeVisibility(TRUE);
    }
}

void SearchView::OnSearchClicked()
{
    UpdateViewsVisibility(FALSE);
    Boolean res = FALSE;
    mQueryTextView->RequestFocus(&res);
    SetImeVisibility(TRUE);
    if (mOnSearchClickListener) {
        mOnSearchClickListener->OnClick((IView*)this->Probe(EIID_IView));
    }
}

void SearchView::OnVoiceClicked()
{
    // guard against possible race conditions
    if (NULL == mSearchable) {
        return;
    }
    AutoPtr<ISearchableInfo> searchable = mSearchable;
    //try {
    Boolean webSearch = FALSE, recognizer = FALSE;
    searchable->GetVoiceSearchLaunchWebSearch(&webSearch);
    searchable->GetVoiceSearchLaunchRecognizer(&recognizer);
    if (webSearch) {
        AutoPtr<IIntent>     webSearchIntent = CreateVoiceWebSearchIntent(mVoiceWebSearchIntent, searchable);
        ASSERT_SUCCEEDED(GetContext()->StartActivity(webSearchIntent));
    } else if (recognizer) {
        AutoPtr<IIntent> appSearchIntent = CreateVoiceAppSearchIntent(mVoiceAppSearchIntent, searchable);
        ASSERT_SUCCEEDED(GetContext()->StartActivity(appSearchIntent));
    }
    //} catch (ActivityNotFoundException e) {
        //Log.w(LOG_TAG, "Could not find voice search activity");
    //}
}

ECode SearchView::OnTextFocusChanged()
{
    UpdateViewsVisibility(IsIconified());
    PostUpdateFocusedState();
    Boolean focus = FALSE;
    mQueryTextView->HasFocus(&focus);
    if (focus) {
        ForceSuggestionQuery();
    }
    return NOERROR;
}

void SearchView::AdjustDropDownSizeAndPosition()
{
    Int32 width = 0;
    mDropDownAnchor->GetWidth(&width);
    if (width > 1) {
        AutoPtr<IResources> res;
        GetContext()->GetResources((IResources**)&res);
        Int32 anchorPadding = 0;
        mSearchPlate->GetPaddingLeft(&anchorPadding);
        AutoPtr<IRect> dropDownPadding;
        CRect::New((IRect**)&dropDownPadding);
        Boolean isLayoutRtl = IsLayoutRtl();
        Int32 iconOffset = 0;
        if (mIconifiedByDefault) {
            Int32 w = 0, l = 0;
            res->GetDimensionPixelSize(R::dimen::dropdownitem_icon_width, &w);
            res->GetDimensionPixelSize(R::dimen::dropdownitem_icon_width, &l);
            iconOffset = w + l;
        } else {
            iconOffset = 0;
        }
        AutoPtr<IDrawable> d;
        mQueryTextView->GetDropDownBackground((IDrawable**)&d);
        Boolean result = FALSE;
        d->GetPadding(dropDownPadding, &result);
        Int32 offset = 0;
        AutoPtr<CRect> cr = (CRect*)dropDownPadding.Get();
        if (isLayoutRtl) {
            offset = -cr->mLeft;
        } else {
            offset = anchorPadding - (cr->mLeft + iconOffset);
        }
        mQueryTextView->SetDropDownHorizontalOffset(offset);
        Int32 anchor = 0;
        mDropDownAnchor->GetWidth(&anchor);
        Int32 dropDownWidth = anchor + cr->mLeft + cr->mRight + iconOffset - anchorPadding;
        mQueryTextView->SetDropDownWidth(dropDownWidth);
    }
}

Boolean SearchView::OnItemClicked(
    /* [in] */ Int32 position,
    /* [in] */ Int32 actionKey,
    /* [in] */ const String& actionMsg)
{
    Boolean res = FALSE;
    mOnSuggestionListener->OnSuggestionClick(position, &res);
    if (!mOnSuggestionListener || !res) {
        LaunchSuggestion(position, IKeyEvent::KEYCODE_UNKNOWN, String(NULL));
        SetImeVisibility(FALSE);
        DismissSuggestions();
        return TRUE;
    }
    return FALSE;
}

Boolean SearchView::OnItemSelected(
    /* [in] */ Int32 position)
{
    Boolean res = FALSE;
    mOnSuggestionListener->OnSuggestionSelect(position, &res);
    if (!mOnSuggestionListener || !res) {
        RewriteQueryFromSuggestion(position);
        return TRUE;
    }
    return FALSE;
}

void SearchView::RewriteQueryFromSuggestion(
    /* [in] */ Int32 position)
{
    AutoPtr<ICharSequence> oldQuery;
    mQueryTextView->GetText((ICharSequence**)&oldQuery);
    AutoPtr<ICursor> c;
    (ICursorFilterClient::Probe(mSuggestionsAdapter))->GetCursor((ICursor**)&c);
    if (c) {
        return;
    }
    Boolean res = FALSE;
    c->MoveToPosition(position, &res);
    if (res) {
        AutoPtr<ICharSequence> newQuery;
        (ICursorFilterClient::Probe(mSuggestionsAdapter))->ConvertToString(c, (ICharSequence**)&newQuery);
        if (newQuery) {
            SetQuery(newQuery);
        } else {
            SetQuery(oldQuery);
        }
    } else {
        SetQuery(oldQuery);
    }
}

Boolean SearchView::LaunchSuggestion(
    /* [in] */ Int32 position,
    /* [in] */ Int32 actionKey,
    /* [in] */ const String& actionMsg)
{
    AutoPtr<ICursor> c;
    (ICursorFilterClient::Probe(mSuggestionsAdapter))->GetCursor((ICursor**)&c);
    Boolean res = FALSE;
    c->MoveToPosition(position, &res);
    if (c && res) {
        AutoPtr<IIntent> intent = CreateIntentFromSuggestion(c, actionKey, actionMsg);
        LaunchIntent(intent);

        return TRUE;
    }
    return FALSE;
}

void SearchView::LaunchIntent(
    /* [in] */ IIntent* intent)
{
    if (!intent) {
        return;
    }
    //try {
        ASSERT_SUCCEEDED(GetContext()->StartActivity(intent));
    //} catch (RuntimeException ex) {
    //  Log.e(LOG_TAG, "Failed launch activity: " + intent, ex);
    //}
}

void SearchView::SetQuery(
    /* [in] */ ICharSequence* query)
{
    mQueryTextView->SetTextFilter(query, TRUE);
    // Move the cursor to the end
    Int32 length = 0;
    query->GetLength(&length);
    mQueryTextView->SetSelectionEx(TextUtils::IsEmpty(query) ? 0 : length);
}

void SearchView::LaunchQuerySearch(
    /* [in] */ Int32 actionKey,
    /* [in] */ const String& actionMsg,
    /* [in] */ const String& query)
{
    String action = IIntent::ACTION_SEARCH;
    AutoPtr<IIntent> intent = CreateIntent(action, NULL, String(NULL), query, actionKey, actionMsg);
    GetContext()->StartActivity(intent);
}

AutoPtr<IIntent> SearchView::CreateIntent(
    /* [in] */ const String& action,
    /* [in] */ IUri* data,
    /* [in] */ const String& extraData,
    /* [in] */ const String& query,
    /* [in] */ Int32 actionKey,
    /* [in] */ const String& actionMsg)
{
    AutoPtr<IIntent> intent;
    CIntent::New(action, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    if (data) {
        intent->SetData(data);
    }
    intent->PutCharSequenceExtra(ISearchManager::USER_QUERY, mUserQuery);
    if (query != NULL) {
        intent->PutStringExtra(ISearchManager::QUERY, query);
    }
    if (extraData != NULL) {
        intent->PutStringExtra(ISearchManager::EXTRA_DATA_KEY, extraData);
    }
    if (mAppSearchData != NULL) {
        intent->PutBundleExtra(ISearchManager::APP_DATA, mAppSearchData);
    }
    if (actionKey != IKeyEvent::KEYCODE_UNKNOWN) {
        intent->PutInt32Extra(ISearchManager::ACTION_KEY, actionKey);
        intent->PutStringExtra(ISearchManager::ACTION_MSG, actionMsg);
    }
    AutoPtr<IComponentName> name;
    mSearchable->GetSearchActivity((IComponentName**)&name);
    intent->SetComponent(name);
    return intent;
}

AutoPtr<IIntent> SearchView::CreateVoiceWebSearchIntent(
   /* [in] */ IIntent* baseIntent,
   /* [in] */ ISearchableInfo* searchable)
{
    AutoPtr<IIntent> voiceIntent;
    CIntent::New(baseIntent, (IIntent**)&voiceIntent);
    AutoPtr<IComponentName> searchActivity;
    searchable->GetSearchActivity((IComponentName**)&searchActivity);
    String temp(NULL);
    if(searchActivity != NULL)
    {
        searchActivity->FlattenToShortString(&temp);
    }
    voiceIntent->PutStringExtra(IRecognizerIntent::EXTRA_CALLING_PACKAGE, temp);
    return voiceIntent;
}

AutoPtr<IIntent> SearchView::CreateVoiceAppSearchIntent(
   /* [in] */ IIntent* baseIntent,
   /* [in] */ ISearchableInfo* searchable)
{
    AutoPtr<IComponentName> searchActivity;
    searchable->GetSearchActivity((IComponentName**)&searchActivity);

    // create the necessary intent to set up a search-and-forward operation
    // in the voice search system.   We have to keep the bundle separate,
    // because it becomes immutable once it enters the PendingIntent
    AutoPtr<IIntent> queryIntent;
    CIntent::New(IIntent::ACTION_SEARCH, (IIntent**)&queryIntent);
    queryIntent->SetComponent(searchActivity);
    AutoPtr<IPendingIntent> pending;
    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    helper->GetActivity(GetContext(), 0, queryIntent, IPendingIntent::FLAG_ONE_SHOT, (IPendingIntent**)&pending);

    // Now set up the bundle that will be inserted into the pending intent
    // when it's time to do the search.  We always build it here (even if empty)
    // because the voice search activity will always need to insert "QUERY" into
    // it anyway.
    AutoPtr<IBundle> queryExtras;
    CBundle::New((IBundle**)&queryExtras);
    if (mAppSearchData != NULL) {
        queryExtras->PutParcelable(ISearchManager::APP_DATA, IParcelable::Probe(mAppSearchData));
    }

    // Now build the intent to launch the voice search.  Add all necessary
    // extras to launch the voice recognizer, and then all the necessary extras
    // to forward the results to the searchable activity
    AutoPtr<IIntent>  voiceIntent;
    CIntent::New(baseIntent, (IIntent**)&voiceIntent);

    // Add all of the configuration options supplied by the searchable's metadata
    String languageModel = IRecognizerIntent::LANGUAGE_MODEL_FREE_FORM;
    String prompt;
    String language;
    Int32 maxResults = 1;

    AutoPtr<IResources> resources = GetResources();
    Int32 idTemp;
    if ((searchable->GetVoiceLanguageModeId(&idTemp), idTemp) != 0) {
        resources->GetString(idTemp, &languageModel);
    }
    if ((searchable->GetVoicePromptTextId(&idTemp), idTemp) != 0) {
        resources->GetString(idTemp, &prompt);
    }
    if ((searchable->GetVoiceLanguageId(&idTemp), idTemp) != 0) {
        resources->GetString(idTemp, &language);
    }
    if ((searchable->GetVoiceMaxResults(&idTemp), idTemp) != 0) {
        searchable->GetVoiceMaxResults(&maxResults);
    }
    voiceIntent->PutStringExtra(IRecognizerIntent::EXTRA_LANGUAGE_MODEL, languageModel);
    voiceIntent->PutStringExtra(IRecognizerIntent::EXTRA_PROMPT, prompt);
    voiceIntent->PutStringExtra(IRecognizerIntent::EXTRA_LANGUAGE, language);
    voiceIntent->PutInt32Extra(IRecognizerIntent::EXTRA_MAX_RESULTS, maxResults);
    String sTemp(NULL);
    if(searchActivity != NULL)
    {
        searchActivity->FlattenToShortString(&sTemp);
    }
    voiceIntent->PutStringExtra(IRecognizerIntent::EXTRA_CALLING_PACKAGE, sTemp);

    // Add the values that configure forwarding the results
    voiceIntent->PutParcelableExtra(IRecognizerIntent::EXTRA_RESULTS_PENDINGINTENT, IParcelable::Probe(pending));
    voiceIntent->PutParcelableExtra(IRecognizerIntent::EXTRA_RESULTS_PENDINGINTENT_BUNDLE, IParcelable::Probe(queryExtras));

    return voiceIntent;
}

AutoPtr<IIntent> SearchView::CreateIntentFromSuggestion(
    /* [in] */ ICursor* c,
    /* [in] */ Int32 actionKey,
    /* [in] */ const String& actionMsg)
{
     //try {
    String action;// = SuggestionsAdapter::GetColumnString(c, ISearchManager::SUGGEST_COLUMN_INTENT_ACTION);

    if (action == NULL) {
        mSearchable->GetSuggestIntentAction(&action);
    }
    if (action == NULL) {
        action = IIntent::ACTION_SEARCH;
    }

    String data;//= SuggestionsAdapter::GetColumnString(c, ISearchManager::SUGGEST_COLUMN_INTENT_DATA);
    if (data == NULL) {
        mSearchable->GetSuggestIntentData(&data);
    }
    if (data != NULL) {
        String id;// = SuggestionsAdapter::GetColumnString(c, ISearchManager::SUGGEST_COLUMN_INTENT_DATA_ID);
        if (id != NULL) {
            String str;
            Uri::Encode(id, &str);
            data += data;
            data += "/";
            data += str;
        }
    }
    AutoPtr<IUri> p;
    Uri::Parse(data, (IUri**)&p);
    AutoPtr<IUri> dataUri = (data == NULL) ? NULL : p;

    String query;//= SuggestionsAdapter::GetColumnString(c, ISearchManager::SUGGEST_COLUMN_QUERY);
    String extraData;// = SuggestionsAdapter::GetColumnString(c, ISearchManager::SUGGEST_COLUMN_INTENT_EXTRA_DATA);

    return CreateIntent(action, dataUri, extraData, query, actionKey, actionMsg);
    //} catch (RuntimeException e ) {
    //    int rowNum;
    //    try {
    //        rowNum = c.getPosition();
    //    } catch (RuntimeException e2 ) {
    //        rowNum = -1;
    //    }
    //    Log.w(LOG_TAG, "Search suggestions cursor at row " + rowNum +
    //                    " returned exception.", e);
    //    return null;
    //}
}

void SearchView::ForceSuggestionQuery()
{
    _SearchAutoComplete* temp = reinterpret_cast<_SearchAutoComplete*>(mQueryTextView->Probe(EIID_SearchAutoComplete));
    temp->DoBeforeTextChanged();
    temp->DoAfterTextChanged();
}

//==========================================================
//          SearchView::_SearchAutoComplete
//==========================================================
SearchView::_SearchAutoComplete::_SearchAutoComplete()
{

}

SearchView::_SearchAutoComplete::_SearchAutoComplete(
    /* [in] */ IContext* context)
{
    Init(context);
}

SearchView::_SearchAutoComplete::_SearchAutoComplete(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

SearchView::_SearchAutoComplete::_SearchAutoComplete(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init(context, attrs, defStyle);
}

ECode SearchView::_SearchAutoComplete::Init(
    /* [in] */ IContext* context)
{
    AutoCompleteTextView::Init(context);
    mThreshold = GetThreshold();
    return NOERROR;
}

ECode SearchView::_SearchAutoComplete::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoCompleteTextView::Init(context, attrs);
    mThreshold = GetThreshold();
    return NOERROR;
}

ECode SearchView::_SearchAutoComplete::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoCompleteTextView::Init(context, attrs, defStyle);
    mThreshold = GetThreshold();
    return NOERROR;
}

void SearchView::_SearchAutoComplete::SetSearchView(
    /* [in] */ ISearchView* searchView)
{
    mSearchView = searchView;
}

ECode SearchView::_SearchAutoComplete::SetThreshold(
    /* [in] */ Int32 threshold)
{
    AutoCompleteTextView::SetThreshold(threshold);
    mThreshold = threshold;
    return NOERROR;
}

ECode SearchView::_SearchAutoComplete::PerformCompletion()
{
    //todo nothing
    return NOERROR;
}

ECode SearchView::_SearchAutoComplete::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    AutoCompleteTextView::OnWindowFocusChanged(hasWindowFocus);
    Boolean hasFocus = FALSE;
    mSearchView->HasFocus(&hasFocus);
    if (hasWindowFocus && hasFocus && GetVisibility() == IView::VISIBLE) {
        AutoPtr<IInterface> inter;
        GetContext()->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInterface**)&inter);
        AutoPtr<IInputMethodManager> inputManager = IInputMethodManager::Probe(inter);
        Boolean res = FALSE;
        inputManager->ShowSoftInput((IView*)this->Probe(EIID_IView), 0, &res);
        // If in landscape mode, then make sure that
        // the ime is in front of the dropdown.
        if (IsLandscapeMode(GetContext())) {
            EnsureImeVisible(TRUE);
        }
    }
    return NOERROR;
}

Boolean SearchView::_SearchAutoComplete::EnoughToFilter()
{
    return mThreshold <= 0 || AutoCompleteTextView::EnoughToFilter();
}

Boolean SearchView::_SearchAutoComplete::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        // special case for the back key, we do not even try to send it
        // to the drop down list but instead, consume it immediately
        Int32 action = 0, count = 0;
        event->GetAction(&action);
        event->GetRepeatCount(&count);
        if (action == IKeyEvent::ACTION_DOWN && count == 0) {
            AutoPtr<IDispatcherState> state;
            GetKeyDispatcherState((IDispatcherState**)&state);
            if (state) {
                state->StartTracking(event, this->Probe(EIID_IInterface));
            }
            return TRUE;
        } else if (action == IKeyEvent::ACTION_UP) {
            AutoPtr<IDispatcherState> state;
            GetKeyDispatcherState((IDispatcherState**)&state);
            if (state) {
                state->HandleUpEvent(event);
            }
            Boolean tracking = FALSE, canceled = FALSE;
            event->IsTracking(&tracking);
            event->IsCanceled(&canceled);
            if (tracking && !canceled) {
                mSearchView->ClearFocus();
                SearchView* svTemp = reinterpret_cast<SearchView*>(mSearchView->Probe(EIID_SearchView));
                svTemp->SetImeVisibility(FALSE);
                return TRUE;
            }
        }
    }
    return AutoCompleteTextView::OnKeyPreIme(keyCode, event);
}

void SearchView::_SearchAutoComplete::ReplaceText(
    /* [in] */ ICharSequence* text)
{
    //todo nothing
}

void SearchView::_SearchAutoComplete::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AutoCompleteTextView::OnFocusChanged(focused, direction, previouslyFocusedRect);
    mSearchView->OnTextFocusChanged();
}

Boolean SearchView::_SearchAutoComplete::IsEmpty()
{
    return TextUtils::GetTrimmedLength(GetText()) == 0;
}


//==========================================================
//          SearchView::ShowImeRunnable
//==========================================================
SearchView::ShowImeRunnable::ShowImeRunnable(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::ShowImeRunnable::Run()
{
    AutoPtr<IInterface> inter;
    mHost->GetContext()->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInterface**)&inter);
    AutoPtr<IInputMethodManager> imm = IInputMethodManager::Probe(inter);
    if (imm) {
        imm->ShowSoftInputUnchecked(0, NULL);
    }
    return NOERROR;
}

//==========================================================
//          SearchView::UpdateDrawableStateRunnable
//==========================================================
SearchView::UpdateDrawableStateRunnable::UpdateDrawableStateRunnable(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::UpdateDrawableStateRunnable::Run()
{
    mHost->UpdateFocusedState();
    return NOERROR;
}

//==========================================================
//          SearchView::ReleaseCursorRunnable
//==========================================================
SearchView::ReleaseCursorRunnable::ReleaseCursorRunnable(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::ReleaseCursorRunnable::Run()
{
    if (mHost->mSuggestionsAdapter && ISuggestionsAdapter::Probe(mHost->mSuggestionsAdapter)) {
        //mHost->mSuggestionsAdapter->ChangeCursor(NULL);
    }
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewClickListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewClickListener, IViewOnClickListener)

SearchView::SearchViewClickListener::SearchViewClickListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewClickListener::OnClick(
    /* [in] */ IView* v)
{
    if (v == mHost->mSearchButton) {
        mHost->OnSearchClicked();
    } else if (v == mHost->mCloseButton) {
        mHost->OnCloseClicked();
    } else if (v == mHost->mSubmitButton) {
        mHost->OnSubmitQuery();
    } else if (v == mHost->mVoiceButton) {
        mHost->OnVoiceClicked();
    } else if (v == mHost->mQueryTextView) {
        mHost->ForceSuggestionQuery();
    }
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewKeyListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewKeyListener, IViewOnKeyListener)

SearchView::SearchViewKeyListener::SearchViewKeyListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{
}

ECode SearchView::SearchViewKeyListener::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    /*if (!mHost->mSearchable)
    {
        *result = FALSE;
        return NOERROR;
    }*/

    Boolean showing = FALSE;
    mHost->mQueryTextView->IsPopupShowing(&showing);
    Int32 selection = 0;
    mHost->mQueryTextView->GetListSelection(&selection);
    if (showing && selection != IListView::INVALID_POSITION) {
        *result = mHost->OnSuggestionsKey(v, keyCode, event);
        return NOERROR;
    }

    Boolean modifiers = FALSE;
    event->HasNoModifiers(&modifiers);
    _SearchAutoComplete* temp = reinterpret_cast<_SearchAutoComplete*>(mHost->mQueryTextView->Probe(EIID_SearchAutoComplete));
    if (!temp->IsEmpty() && modifiers) {
        Int32 action = 0;
        event->GetAction(&action);
        if (action == IKeyEvent::ACTION_UP) {
            if (keyCode == IKeyEvent::KEYCODE_ENTER) {
                v->CancelLongPress();
                AutoPtr<ICharSequence> csq;
                mHost->mQueryTextView->GetText((ICharSequence**)&csq);
                String str;
                csq->ToString(&str);
                mHost->LaunchQuerySearch(IKeyEvent::KEYCODE_UNKNOWN, String(NULL), str);
                *result = TRUE;
                return NOERROR;
            }
        }
        if (action == IKeyEvent::ACTION_DOWN) {
            /*AutoPtr<SearchableInfo::ActionKeyInfo> actionKey;
            mHost->mSearchable->FindActionKey(keyCode, (SearchableInfo::ActionKeyInfo**)&actionKey);
            if (actionKey && actionKey->GetQueryActionMsg() != NULL) {
                AutoPtr<ICharSequence> csq;
                mHost->mQueryTextView->GetText((ICharSequence**)&csq);
                String str;
                csq->ToString(&str);
                mHost->LaunchQuerySearch(keyCode, actionKey.getQueryActionMsg(), str);
                *result = TRUE;
                return NOERROR;
            }*/
        }
    }

    *result = FALSE;
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewEditorActionListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewEditorActionListener, IOnEditorActionListener)

SearchView::SearchViewEditorActionListener::SearchViewEditorActionListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewEditorActionListener::OnEditorAction(
    /* [in] */ ITextView* v,
    /* [in] */ Int32 actionId,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    mHost->OnSubmitQuery();
    *result = TRUE;
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewItemClickListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewItemClickListener, IAdapterViewOnItemClickListener)

SearchView::SearchViewItemClickListener::SearchViewItemClickListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mHost->OnItemClicked(position, IKeyEvent::KEYCODE_UNKNOWN, String(NULL));
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewItemSelectedListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewItemSelectedListener, IAdapterViewOnItemSelectedListener)

SearchView::SearchViewItemSelectedListener::SearchViewItemSelectedListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewItemSelectedListener::OnItemSelected(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mHost->OnItemSelected(position);
    return NOERROR;
}

ECode SearchView::SearchViewItemSelectedListener::OnNothingSelected(
    /* [in] */ IAdapterView* parent)
{
    //do nothing
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewTextWatcher
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewTextWatcher, ITextWatcher)

SearchView::SearchViewTextWatcher::SearchViewTextWatcher(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewTextWatcher::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    //do nothing
    return NOERROR;
}

ECode SearchView::SearchViewTextWatcher::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    mHost->OnTextChanged(s);
    return NOERROR;
}

ECode SearchView::SearchViewTextWatcher::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    //do nothing
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewFocusChangeListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewFocusChangeListener, IViewOnFocusChangeListener)

SearchView::SearchViewFocusChangeListener::SearchViewFocusChangeListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewFocusChangeListener::OnFocusChange(
    /* [in] */ IView* v,
    /* [in] */ Boolean hasFocus)
{
    if (mHost->mOnQueryTextFocusChangeListener) {
        mHost->mOnQueryTextFocusChangeListener->OnFocusChange((IView*)this->Probe(EIID_IView), hasFocus);
    }
    return NOERROR;
}

//==========================================================
//          SearchView::SearchViewLayoutChangeListener
//==========================================================
CAR_INTERFACE_IMPL(SearchView::SearchViewLayoutChangeListener, IViewOnLayoutChangeListener)

SearchView::SearchViewLayoutChangeListener::SearchViewLayoutChangeListener(
    /* [in] */ SearchView* host)
    : mHost(host)
{

}

ECode SearchView::SearchViewLayoutChangeListener::OnLayoutChange(
    /* [in] */ IView* v,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 oldLeft,
    /* [in] */ Int32 oldTop,
    /* [in] */ Int32 oldRight,
    /* [in] */ Int32 oldBottom)
{
    mHost->AdjustDropDownSizeAndPosition();
    return NOERROR;
}

} //namespace Widget
} //namespace Droid
} //namespace Elastos
