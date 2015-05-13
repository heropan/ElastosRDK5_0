
#ifndef __SEARCHVIEW_H__
#define __SEARCHVIEW_H__

#include "ext/frameworkext.h"
#include "widget/LinearLayout.h"
#include "widget/AutoCompleteTextView.h"
#include "view/ViewMacro.h"
#include "widget/TextViewMacro.h"
#include "widget/AutoCompleteTextViewMacro.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::App::ISearchableInfo;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IViewOnFocusChangeListener;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::Widget::ISearchViewOnQueryTextListener;
using Elastos::Droid::Widget::ISearchViewOnSuggestionListener;
using Elastos::Droid::Widget::ICursorAdapter;
using Elastos::Droid::Widget::IOnCloseListener;
using Elastos::Droid::Widget::IAutoCompleteTextView;
using Elastos::Droid::Widget::ISearchView;
using Elastos::Droid::App::IActionKeyInfo;

namespace Elastos{
namespace Droid{
namespace Widget{

extern "C" const InterfaceID EIID_SearchView;
extern "C" const InterfaceID EIID_SearchAutoComplete;

class SearchView : public LinearLayout
{
private:
    class ShowImeRunnable
        : public Runnable
    {
    public:
        ShowImeRunnable(
            /* [in] */ SearchView* host);

        CARAPI Run();

    private:
        SearchView* mHost;
    };

    class UpdateDrawableStateRunnable
        : public Runnable
    {
    public:
        UpdateDrawableStateRunnable(
            /* [in] */ SearchView* host);

        CARAPI Run();

    private:
        SearchView* mHost;
    };

    class ReleaseCursorRunnable
        : public Runnable
    {
    public:
        ReleaseCursorRunnable(
            /* [in] */ SearchView* host);

        CARAPI Run();

    private:
        SearchView* mHost;
    };

    class SearchViewClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        SearchViewClickListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        SearchView* mHost;
    };

    class SearchViewKeyListener
        : public IViewOnKeyListener
        , public ElRefBase
    {
    public:
        SearchViewKeyListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnKey(
            /* [in] */ IView* v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

    private:
        SearchView* mHost;
    };

    class SearchViewEditorActionListener
        : public IOnEditorActionListener
        , public ElRefBase
    {
    public:
        SearchViewEditorActionListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnEditorAction(
            /* [in] */ ITextView* v,
            /* [in] */ Int32 actionId,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

    private:
        SearchView* mHost;
    };

    class SearchViewItemClickListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        SearchViewItemClickListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        SearchView* mHost;
    };

    class SearchViewItemSelectedListener
        : public IAdapterViewOnItemSelectedListener
        , ElRefBase
    {
    public:
        SearchViewItemSelectedListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnItemSelected(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI OnNothingSelected(
            /* [in] */ IAdapterView* parent);

    private:
        SearchView* mHost;
    };

    class SearchViewTextWatcher
        : public ITextWatcher
        , public ElRefBase
    {
    public:
        SearchViewTextWatcher(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI BeforeTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count,
            /* [in] */ Int32 after);

        CARAPI OnTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 start,
            /* [in] */ Int32 before,
            /* [in] */ Int32 count);

        CARAPI AfterTextChanged(
            /* [in] */ IEditable* s);

    private:
        SearchView* mHost;
    };

    class SearchViewFocusChangeListener
        : public IViewOnFocusChangeListener
        , ElRefBase
    {
    public:
        SearchViewFocusChangeListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        CARAPI OnFocusChange(
            /* [in] */ IView* v,
            /* [in] */ Boolean hasFocus);

    private:
        SearchView* mHost;
    };

    class SearchViewLayoutChangeListener
        : public IViewOnLayoutChangeListener
        , public ElRefBase
    {
    public:
        SearchViewLayoutChangeListener(
            /* [in] */ SearchView* host);

        CAR_INTERFACE_DECL()

        OnLayoutChange(
            /* [in] */ IView* v,
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [in] */ Int32 oldLeft,
            /* [in] */ Int32 oldTop,
            /* [in] */ Int32 oldRight,
            /* [in] */ Int32 oldBottom);

    private:
        SearchView* mHost;
    };

public:
    class _SearchAutoComplete
        : public AutoCompleteTextView
    {
    public:
        _SearchAutoComplete();

        _SearchAutoComplete(
            /* [in] */ IContext* context);

        _SearchAutoComplete(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs);

        _SearchAutoComplete(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs,
            /* [in] */ Int32 defStyle);


        CARAPI_(void) SetSearchView(
            /* [in] */ ISearchView* searchView);

        CARAPI SetThreshold(
            /* [in] */ Int32 threshold);

        CARAPI PerformCompletion();

        CARAPI OnWindowFocusChanged(
            /* [in] */ Boolean hasWindowFocus);

        CARAPI_(Boolean) EnoughToFilter();

        CARAPI_(Boolean) OnKeyPreIme(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event);

        CARAPI_(Boolean) IsEmpty();

    protected:
        CARAPI Init(
            /* [in] */ IContext* context);

        CARAPI Init(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs);

        CARAPI Init(
            /* [in] */ IContext* context,
            /* [in] */ IAttributeSet* attrs,
            /* [in] */ Int32 defStyle);


        CARAPI_(void) ReplaceText(
            /* [in] */ ICharSequence* text);

        CARAPI_(void) OnFocusChanged(
            /* [in] */ Boolean focused,
            /* [in] */ Int32 direction,
            /* [in] */ IRect* previouslyFocusedRect);

    private:
        Int32 mThreshold;
        AutoPtr<ISearchView> mSearchView;
        friend class SearchView;
    };

public:
    SearchView(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    CARAPI SetSearchableInfo(
        /* [in] */ ISearchableInfo* searchable);

    CARAPI SetAppSearchData(
        /* [in] */ IBundle* appSearchData);

    CARAPI SetImeOptions(
        /* [in] */ Int32 imeOptions);

    CARAPI_(Int32) GetImeOptions();

    CARAPI SetInputType(
        /* [in] */ Int32 inputType);

    CARAPI_(Int32) GetInputType();

    CARAPI_(Boolean) RequestFocus(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI ClearFocus();

    CARAPI SetOnQueryTextListener(
        /* [in] */ ISearchViewOnQueryTextListener* listener);

    CARAPI SetOnCloseListener(
        /* [in] */ IOnCloseListener* listener);

    CARAPI SetOnQueryTextFocusChangeListener(
        /* [in] */ IViewOnFocusChangeListener* listener);

    CARAPI SetOnSuggestionListener(
        /* [in] */ ISearchViewOnSuggestionListener* listener);

    CARAPI SetOnSearchClickListener(
        /* [in] */ IViewOnClickListener* listener);

    CARAPI_(AutoPtr<ICharSequence>) GetQuery();

    CARAPI SetQuery(
        /* [in] */ ICharSequence* query,
        /* [in] */ Boolean submit);

    CARAPI SetQueryHint(
        /* [in] */ ICharSequence* hint);

    CARAPI_(AutoPtr<ICharSequence>) GetQueryHint();

    CARAPI SetIconifiedByDefault(
        /* [in] */ Boolean iconified);

    CARAPI_(Boolean) IsIconfiedByDefault();

    CARAPI SetIconified(
        /* [in] */ Boolean iconify);

    CARAPI_(Boolean) IsIconified();

    CARAPI SetSubmitButtonEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(Boolean) IsSubmitButtonEnabled();

    CARAPI SetQueryRefinementEnabled(
        /* [in] */ Boolean enable);

    CARAPI_(Boolean) IsQueryRefinementEnabled();

    CARAPI SetSuggestionsAdapter(
        /* [in] */ ICursorAdapter* adapter);

    CARAPI_(AutoPtr<ICursorAdapter>) GetSuggestionsAdapter();

    CARAPI SetMaxWidth(
        /* [in] */ Int32 maxpixels);

    CARAPI_(Int32) GetMaxWidth();

    CARAPI_(void) OnQueryRefine(
        /* [in] */ ICharSequence* queryText);

    CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Boolean) OnSuggestionsKey(
        /* [in] */ IView* v,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    CARAPI OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    CARAPI OnActionViewCollapsed();

    CARAPI OnActionViewExpanded();

    CARAPI OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    CARAPI OnRtlPropertiesChanged(
        /* [in] */ Int32 layoutDirection);

    CARAPI OnTextFocusChanged();

    static CARAPI_(Boolean) IsLandscapeMode(
        /* [in] */ IContext* context);

protected:
    SearchView();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL);

    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI OnDetachedFromWindow();


private:
    CARAPI_(Int32) GetPreferredWidth();

    CARAPI_(void) UpdateViewsVisibility(
        /* [in] */ Boolean collapsed);

    CARAPI_(Boolean) HasVoiceSearch();

    CARAPI_(Boolean) IsSubmitAreaEnabled();

    CARAPI_(void) UpdateSubmitButton(
        /* [in] */ Boolean hasText);

    CARAPI_(void) UpdateSubmitArea();

    CARAPI_(void) UpdateCloseButton();

    CARAPI_(void) PostUpdateFocusedState();

    CARAPI_(void) UpdateFocusedState();

    CARAPI_(void) SetImeVisibility(
        /* [in] */ Boolean visible);

    CARAPI_(String) GetActionKeyMessage(
        /* [in] */ ICursor* c,
        /* [in] */ IActionKeyInfo* actionKey);

    CARAPI_(Int32) GetSearchIconId();

    CARAPI_(AutoPtr<ICharSequence>) GetDecoratedHint(
        /* [in] */ ICharSequence* hintText);

    CARAPI_(void) UpdateQueryHint();

    CARAPI_(void) UpdateSearchAutoComplete();

    CARAPI_(void) UpdateVoiceButton(
        /* [in] */ Boolean empty);

    CARAPI_(void) OnTextChanged(
        /* [in] */ ICharSequence* newText);

    CARAPI_(void) OnSubmitQuery();

    CARAPI_(void) DismissSuggestions();

    CARAPI_(void) OnCloseClicked();

    CARAPI_(void) OnSearchClicked();

    CARAPI_(void) OnVoiceClicked();

    CARAPI_(void) AdjustDropDownSizeAndPosition();

    CARAPI_(Boolean) OnItemClicked(
        /* [in] */ Int32 position,
        /* [in] */ Int32 actionKey,
        /* [in] */ const String& actionMsg);

    CARAPI_(Boolean) OnItemSelected(
        /* [in] */ Int32 position);

    CARAPI_(void) RewriteQueryFromSuggestion(
        /* [in] */ Int32 position);

    CARAPI_(Boolean) LaunchSuggestion(
        /* [in] */ Int32 position,
        /* [in] */ Int32 actionKey,
        /* [in] */ const String& actionMsg);

    CARAPI_(void) LaunchIntent(
        /* [in] */ IIntent* intent);

    CARAPI_(void) SetQuery(
        /* [in] */ ICharSequence* query);

    CARAPI_(void) LaunchQuerySearch(
        /* [in] */ Int32 actionKey,
        /* [in] */ const String& actionMsg,
        /* [in] */ const String& query);

    CARAPI_(AutoPtr<IIntent>) CreateIntent(
        /* [in] */ const String& action,
        /* [in] */ IUri* data,
        /* [in] */ const String& extraData,
        /* [in] */ const String& query,
        /* [in] */ Int32 actionKey,
        /* [in] */ const String& actionMsg);

   CARAPI_(AutoPtr<IIntent>) CreateVoiceWebSearchIntent(
       /* [in] */ IIntent* baseIntent,
       /* [in] */ ISearchableInfo* searchable);

   CARAPI_(AutoPtr<IIntent>) CreateVoiceAppSearchIntent(
       /* [in] */ IIntent* baseIntent,
       /* [in] */ ISearchableInfo* searchable);

    CARAPI_(AutoPtr<IIntent>) CreateIntentFromSuggestion(
        /* [in] */ ICursor* c,
        /* [in] */ Int32 actionKey,
        /* [in] */ const String& actionMsg);

    CARAPI_(void) ForceSuggestionQuery();

private:
    static const Boolean DBG;
    static const String IME_OPTION_NO_MICROPHONE;
    static const String SEARCHVIEW_NAME;

    AutoPtr<ISearchViewOnQueryTextListener> mOnQueryChangeListener;
    AutoPtr<IOnCloseListener> mOnCloseListener;
    AutoPtr<IViewOnFocusChangeListener> mOnQueryTextFocusChangeListener;
    AutoPtr<ISearchViewOnSuggestionListener> mOnSuggestionListener;
    AutoPtr<IViewOnClickListener> mOnSearchClickListener;

    Boolean mIconifiedByDefault;
    Boolean mIconified;
    AutoPtr<ICursorAdapter> mSuggestionsAdapter;
    AutoPtr<IView> mSearchButton;
    AutoPtr<IView> mSubmitButton;
    AutoPtr<IView> mSearchPlate;
    AutoPtr<IView> mSubmitArea;
    AutoPtr<IImageView> mCloseButton;
    AutoPtr<IView> mSearchEditFrame;
    AutoPtr<IView> mVoiceButton;
    AutoPtr<ISearchViewSearchAutoComplete> mQueryTextView;
    AutoPtr<IView> mDropDownAnchor;
    AutoPtr<IImageView> mSearchHintIcon;
    Boolean mSubmitButtonEnabled;
    AutoPtr<ICharSequence> mQueryHint;
    Boolean mQueryRefinement;
    Boolean mClearingFocus;
    Int32 mMaxWidth;
    Boolean mVoiceButtonEnabled;
    AutoPtr<ICharSequence> mOldQueryText;
    AutoPtr<ICharSequence> mUserQuery;
    Boolean mExpandedInActionView;
    Int32 mCollapsedImeOptions;
    AutoPtr<ISearchableInfo> mSearchable;
    AutoPtr<IBundle> mAppSearchData;

    AutoPtr<IRunnable> mShowImeRunnable;
    AutoPtr<IRunnable> mUpdateDrawableStateRunnable;
    AutoPtr<IRunnable> mReleaseCursorRunnable;
    AutoPtr<IIntent> mVoiceWebSearchIntent;
    AutoPtr<IIntent> mVoiceAppSearchIntent;

    AutoPtr<IViewOnClickListener> mOnClickListener;
    AutoPtr<IViewOnKeyListener> mTextKeyListener;
    AutoPtr<IOnEditorActionListener> mOnEditorActionListener;
    AutoPtr<IAdapterViewOnItemClickListener> mOnItemClickListener;
    AutoPtr<IAdapterViewOnItemSelectedListener> mOnItemSelectedListener;
    AutoPtr<ITextWatcher> mTextWatcher;
    // HashMap<String, IDrawableConstantState*> mOutsideDrawablesCache;
};

} //namespace Widget
} //namespace Droid
} //namespace Elastos

#endif
