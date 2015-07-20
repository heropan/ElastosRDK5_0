
#include "widget/Editor.h"
#include <elastos/core/Math.h>
#include <elastos/core/Character.h>
#include "R.h"
#include "util/ArrayUtils.h"
#include "os/SystemClock.h"
#include "os/CMessenger.h"
#include "content/CIntent.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include "text/CStaticLayout.h"
#include "text/DynamicLayout.h"
#include "text/style/CSuggestionRangeSpan.h"
#include "text/method/CMetaKeyKeyListenerHelper.h"
#include "text/method/CWordIterator.h"
#include "graphics/CColor.h"
#include "graphics/CPath.h"
#include "graphics/CPaint.h"
#include "view/View.h"
#include "view/LayoutInflater.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewConfigurationHelper.h"
#include "view/CDragShadowBuilder.h"
#include "view/inputmethod/CExtractedText.h"
#include "view/inputmethod/CInputMethodManager.h"
#include "widget/CLinearLayout.h"
#include "widget/TextView.h"
#include "widget/CPopupWindow.h"
#include "widget/CListView.h"
#include "widget/internal/CEditableInputConnection.h"

using Elastos::Core::Character;
using Elastos::Core::EIID_IComparator;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::CStringWrapper;

//using Elastos::Text::IBreakIterator;
using Elastos::Droid::R;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::CMessenger;
using Elastos::Droid::Os::IMessenger;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IClipDataItem;
using Elastos::Droid::Content::IClipData;
using Elastos::Droid::Content::IClipDataHelper;
//using Elastos::Droid::Content::CClipDataHelper;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Provider::ISettings;
using Elastos::Droid::Text::EIID_ISpanWatcher;
using Elastos::Droid::Text::EIID_IParcelableSpan;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::DynamicLayout;
using Elastos::Droid::Text::IDynamicLayout;
using Elastos::Droid::Text::IStaticLayout;
using Elastos::Droid::Text::CStaticLayout;
using Elastos::Droid::Text::Style::CSuggestionRangeSpan;
using Elastos::Droid::Text::Style::EIID_ISuggestionSpan;
using Elastos::Droid::Text::Style::EIID_IURLSpan;
using Elastos::Droid::Text::Method::CWordIterator;
using Elastos::Droid::Text::Method::ITransformationMethod;
using Elastos::Droid::Text::Method::IPasswordTransformationMethod;
using Elastos::Droid::Text::Method::IMetaKeyKeyListener;
using Elastos::Droid::Text::Method::IMetaKeyKeyListenerHelper;
using Elastos::Droid::Text::Method::CMetaKeyKeyListenerHelper;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::CColor;
using Elastos::Droid::Graphics::CPath;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::EIID_IOnTouchModeChangeListener;
using Elastos::Droid::View::EIID_IOnPreDrawListener;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::View;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::EIID_IActionModeCallback;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::CDragShadowBuilder;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::View::InputMethod::IInputConnection;
using Elastos::Droid::View::InputMethod::CExtractedText;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::Widget::CListView;
using Elastos::Droid::InputMethodService::EIID_IExtractEditText;
using Elastos::Droid::InputMethodService::IExtractEditText;
using Elastos::Droid::Widget::Internal::CEditableInputConnection;


#define ITEXTVIEW_PROBE(textView) ((ITextView*)((textView)->Probe(EIID_ITextView)))

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//              ActionPopupShowerRunnable
//==============================================================================

ECode ActionPopupShowerRunnable::Run()
{
    if (mHost != NULL) {
        mHost->Show();
    }
    return NOERROR;
}

//==============================================================================
//              InsertionHandleViewHiderRunnable
//==============================================================================

ECode InsertionHandleViewHiderRunnable::Run()
{
    if (mHost != NULL) {
        mHost->Hide();
    }
    return NOERROR;
}

//==============================================================================
//              HidePopupRunnable
//==============================================================================

ECode HidePopupRunnable::Run()
{
    if (mHost != NULL) {
        mHost->Hide();
    }
    return NOERROR;
}

//==============================================================================
//              ShowSuggestionRunnable
//==============================================================================

ECode ShowSuggestionRunnable::Run()
{
    if (mHost != NULL) {
        mHost->ShowSuggestions();
    }
    return NOERROR;
}

//==============================================================================
//              InputContentType
//==============================================================================

InputContentType::InputContentType()
    : mImeOptions(IEditorInfo::IME_NULL)
    , mImeActionId(0)
    , mEnterDown(FALSE)
{
}

//==============================================================================
//              InputMethodState
//==============================================================================
InputMethodState::InputMethodState()
    : mBatchEditNesting(0)
    , mCursorChanged(FALSE)
    , mSelectionModeChanged(FALSE)
    , mContentChanged(FALSE)
    , mChangedStart(0)
    , mChangedEnd(0)
    , mChangedDelta(0)
{
    CRect::New((IRect**)&mCursorRectInWindow);
    CRectF::New((IRectF**)&mTmpRectF);
    mTmpOffset = ArrayOf<Float>::Alloc(2);
    CExtractedText::New((IExtractedText**)&mExtractedText);
}

//==============================================================================
//              CustomPopupWindow
//==============================================================================
_CustomPopupWindow::_CustomPopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ Int32 defStyle,
    /* [in] */ Editor* editor,
    /* [in] */ SuggestionsPopupWindow* owner)
    : PopupWindow(context, NULL, defStyle)
    , mEditor(editor)
    , mOwner(owner)
{
}

ECode _CustomPopupWindow::Dismiss()
{
    PopupWindow::Dismiss();

    AutoPtr<PositionListener> pl = mEditor->GetPositionListener();
    pl->RemoveSubscriber(mOwner);

    // Safe cast since show() checks that mTextView->GetText() is an Editable
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    if (text != NULL && ISpannable::Probe(text)) {
        ISpannable* spannable = ISpannable::Probe(text);
        spannable->RemoveSpan(mEditor->mSuggestionRangeSpan);
    }

    mEditor->mTextView->SetCursorVisible(mOwner->mCursorWasVisibleBeforeSuggestions);
    if (mEditor->HasInsertionController()) {
        AutoPtr<InsertionPointCursorController> controller = mEditor->GetInsertionController();
        if (controller)
            controller->Show();
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(CustomPopupWindow, IPopupWindow)
IPOPUPWINDOW_METHODS_IMPL(CustomPopupWindow, _CustomPopupWindow)

CustomPopupWindow::CustomPopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ Int32 defStyle,
    /* [in] */ Editor* editor,
    /* [in] */ SuggestionsPopupWindow* owner)
    : _CustomPopupWindow(context, defStyle, editor, owner)
{
}

//==============================================================================
//              MyPopupWindow
//==============================================================================
CAR_INTERFACE_IMPL(MyPopupWindow, IPopupWindow)
IPOPUPWINDOW_METHODS_IMPL(MyPopupWindow, PopupWindow)

MyPopupWindow::MyPopupWindow(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : PopupWindow(context, attrs, defStyle)
{
}

//==============================================================================
//              PinnedPopupWindow
//==============================================================================
PinnedPopupWindow::PinnedPopupWindow(
    /* [in] */ Editor* editor)
    : mEditor(editor)
{
}

PinnedPopupWindow::~PinnedPopupWindow()
{
}

ECode PinnedPopupWindow::Init()
{
    CreatePopupWindow();

    assert(mPopupWindow != NULL);

    mPopupWindow->SetWindowLayoutType(IWindowManagerLayoutParams::TYPE_APPLICATION_SUB_PANEL);
    mPopupWindow->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
    mPopupWindow->SetHeight(IViewGroupLayoutParams::WRAP_CONTENT);

    InitContentView();

    assert(mContentView != NULL);

    AutoPtr<IViewGroupLayoutParams> wrapContent;
    CViewGroupLayoutParams::New(
            IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT,
            (IViewGroupLayoutParams**)&wrapContent);
    mContentView->SetLayoutParams(wrapContent);

    mPopupWindow->SetContentView((IView*)mContentView->Probe(EIID_IView));
    return NOERROR;
}

ECode PinnedPopupWindow::Show()
{
    AutoPtr<PositionListener> pl = mEditor->GetPositionListener();
    pl->AddSubscriber(this, FALSE /* offset is fixed */);

    ComputeLocalPosition();

    AutoPtr<PositionListener> positionListener = mEditor->GetPositionListener();
    UpdatePosition(positionListener->GetPositionX(), positionListener->GetPositionY());
    return NOERROR;
}

ECode PinnedPopupWindow::MeasureContent()
{
    AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
    AutoPtr<IDisplayMetrics> displayMetrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    Int32 width, height;
    displayMetrics->GetWidthPixels(&width);
    displayMetrics->GetHeightPixels(&height);
    mContentView->Measure(
        View::View::MeasureSpec::MakeMeasureSpec(width, View::View::MeasureSpec::AT_MOST),
        View::View::MeasureSpec::MakeMeasureSpec(height, View::View::MeasureSpec::AT_MOST));
    return NOERROR;
}

ECode PinnedPopupWindow::ComputeLocalPosition()
{
    MeasureContent();
    Int32 width;
    mContentView->GetMeasuredWidth(&width);
    Int32 offset = GetTextOffset();

    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    Float horizontal;
    layout->GetPrimaryHorizontal(offset, &horizontal);
    mPositionX = (Int32) (horizontal - width / 2.0f);
    mPositionX += mEditor->mTextView->ViewportToContentHorizontalOffset();

    Int32 line;
    layout->GetLineForOffset(offset, &line);
    mPositionY = GetVerticalLocalPosition(line);
    mPositionY += mEditor->mTextView->ViewportToContentVerticalOffset();
    return NOERROR;
}

void PinnedPopupWindow::UpdatePosition(
    /* [in] */ Int32 parentPositionX,
    /* [in] */ Int32 parentPositionY)
{
    Int32 positionX = parentPositionX + mPositionX;
    Int32 positionY = parentPositionY + mPositionY;

    positionY = ClipVertically(positionY);

    // Horizontal clipping
    AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
    AutoPtr<IDisplayMetrics> displayMetrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    Int32 widthPixels;
    displayMetrics->GetWidthPixels(&widthPixels);

    Int32 width;
    mContentView->GetMeasuredWidth(&width);
    positionX = Elastos::Core::Math::Min(widthPixels - width, positionX);
    positionX = Elastos::Core::Math::Max(0, positionX);

    if (IsShowing()) {
        mPopupWindow->Update(positionX, positionY, -1, -1);
    }
    else {
        mPopupWindow->ShowAtLocation((IView*)(mEditor->mTextView->Probe(EIID_IView)),
            IGravity::NO_GRAVITY, positionX, positionY);
    }
}

ECode PinnedPopupWindow::Hide()
{
    mPopupWindow->Dismiss();
    AutoPtr<PositionListener> positionListener = mEditor->GetPositionListener();
    positionListener->RemoveSubscriber(this);
    return NOERROR;
}

void PinnedPopupWindow::UpdatePosition(
    /* [in] */ Int32 parentPositionX,
    /* [in] */ Int32 parentPositionY,
    /* [in] */ Boolean parentPositionChanged,
    /* [in] */ Boolean parentScrolled)
{
    // Either parentPositionChanged or parentScrolled is TRUE, check if still visible
    if (IsShowing() && mEditor->IsOffsetVisible(GetTextOffset())) {
        if (parentScrolled)
            ComputeLocalPosition();
        UpdatePosition(parentPositionX, parentPositionY);
    }
    else {
        Hide();
    }
}

Boolean PinnedPopupWindow::IsShowing()
{
    Boolean isShowing;
    mPopupWindow->IsShowing(&isShowing);
    return isShowing;
}

//==============================================================================
//              EasyEditPopupWindow
//==============================================================================
CAR_INTERFACE_IMPL(EasyEditPopupWindow, IViewOnClickListener)

const Int32 EasyEditPopupWindow::POPUP_TEXT_LAYOUT = R::layout::text_edit_action_popup_text;

EasyEditPopupWindow::EasyEditPopupWindow(
    /* [in] */ Editor* editor)
    : PinnedPopupWindow(editor)
    , mDeleteTextView(NULL)
{
}

ECode EasyEditPopupWindow::CreatePopupWindow()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    mPopupWindow = new MyPopupWindow(context, NULL, R::attr::textSelectHandleWindowStyle);
    mPopupWindow->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NOT_NEEDED);
    mPopupWindow->SetClippingEnabled(TRUE);
    return NOERROR;
}

ECode EasyEditPopupWindow::InitContentView()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();

    AutoPtr<ILinearLayout> linearLayout;
    CLinearLayout::New(context, (ILinearLayout**)&linearLayout);
    linearLayout->SetOrientation(ILinearLayout::HORIZONTAL);

    mContentView = IViewGroup::Probe(linearLayout);
    mContentView->SetBackgroundResource(R::drawable::text_edit_side_paste_window);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(
        IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    AutoPtr<IViewGroupLayoutParams> wrapContent;
    CViewGroupLayoutParams::New(
        IViewGroupLayoutParams::WRAP_CONTENT,
        IViewGroupLayoutParams::WRAP_CONTENT,
        (IViewGroupLayoutParams**)&wrapContent);

    inflater->Inflate(POPUP_TEXT_LAYOUT, NULL, (IView**)&mDeleteTextView);
    mDeleteTextView->SetLayoutParams(wrapContent);
    mDeleteTextView->SetText(R::string::delete_);
    mDeleteTextView->SetOnClickListener(this);
    mContentView->AddView(mDeleteTextView);
    return NOERROR;
}

void EasyEditPopupWindow::SetEasyEditSpan(
    /* [in] */ IEasyEditSpan* easyEditSpan)
{
    mEasyEditSpan = easyEditSpan;
}

ECode EasyEditPopupWindow::OnClick(
    /* [in] */ IView* view)
{
    if (view == mDeleteTextView->Probe(EIID_IView)) {
        AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
        if (text != NULL && IEditable::Probe(text)) {
            AutoPtr<IEditable> editable = IEditable::Probe(text);
            Int32 start = 0, end = 0;
            editable->GetSpanStart(mEasyEditSpan, &start);
            editable->GetSpanEnd(mEasyEditSpan, &end);
            if (start >= 0 && end >= 0) {
                TextView* tv = (TextView*)mEditor->mTextView->Probe(EIID_TextView);
                tv->DeleteText_internal(start, end);
            }
        }
    }
    return NOERROR;
}

Int32 EasyEditPopupWindow::GetTextOffset()
{
    // Place the pop-up at the end of the span
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    if (text != NULL && IEditable::Probe(text)) {
        IEditable* editable = IEditable::Probe(text);
        Int32 end;
        editable->GetSpanEnd(mEasyEditSpan, &end);
        return end;
    }
    return 0;
}

Int32 EasyEditPopupWindow::GetVerticalLocalPosition(
    /* [in] */ Int32 line)
{
    Int32 result = 0;
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    if (layout)
        layout->GetLineBottom(line, &result);
    return result;
}

Int32 EasyEditPopupWindow::ClipVertically(
    /* [in] */ Int32 positionY)
{
    // As we display the pop-up below the span, no vertical clipping is required.
    return positionY;
}

//==============================================================================
//              SuggestionInfo
//==============================================================================
CAR_INTERFACE_IMPL(SuggestionInfo, IInterface)

SuggestionInfo::SuggestionInfo()
    : mSuggestionStart(0)
    , mSuggestionEnd(0)
    , mSuggestionIndex(-1)
{
}

//==============================================================================
//              SuggestionAdapter
//==============================================================================

_SuggestionAdapter::_SuggestionAdapter(
    /* [in] */ Editor* editor,
    /* [in] */ SuggestionsPopupWindow* popupWindow)
    : mEditor(editor)
    , mPopupWindow(popupWindow)
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
}

Int32 _SuggestionAdapter::GetCount()
{
    return mPopupWindow->mNumberOfSuggestions;
}

AutoPtr<IInterface> _SuggestionAdapter::GetItem(
    /* [in] */ Int32 position)
{
    AutoPtr<IInterface> obj = (IInterface*)((*mPopupWindow->mSuggestionInfos)[position]);
    return obj;
}

Int64 _SuggestionAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    Int64 id = position;
    return id;
}

AutoPtr<IView> _SuggestionAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<ITextView> textView = ITextView::Probe(convertView);
    if (textView == NULL) {
        TextView* tv = (TextView*)mEditor->mTextView->Probe(EIID_TextView);
        AutoPtr<IView> view;
        mInflater->Inflate(tv->mTextEditSuggestionItemLayout, parent, FALSE, (IView**)&view);
        textView = ITextView::Probe(view);
    }

    AutoPtr<SuggestionInfo> suggestionInfo = (*mPopupWindow->mSuggestionInfos)[position];
    textView->SetText(ICharSequence::Probe(suggestionInfo->mText));

    if (suggestionInfo->mSuggestionIndex == SuggestionsPopupWindow::ADD_TO_DICTIONARY
        || suggestionInfo->mSuggestionIndex == SuggestionsPopupWindow::DELETE_TEXT) {
       textView->SetBackgroundColor(IColor::TRANSPARENT);
    }
    else {
       textView->SetBackgroundColor(IColor::WHITE);
    }

    return textView;
}

IADAPTER_METHODS_IMPL(SuggestionAdapter, _SuggestionAdapter)
IBASEADAPTER_METHODS_IMPL(SuggestionAdapter, _SuggestionAdapter)
ILISTADAPTER_METHODS_IMPL(SuggestionAdapter, _SuggestionAdapter)
ISPINNERADAPTER_METHODS_IMPL(SuggestionAdapter, _SuggestionAdapter)

PInterface SuggestionAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IBaseAdapter) {
        return (IBaseAdapter*)this;
    }
    else if(riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    }
    else if(riid == EIID_IAdapter) {
        return (IAdapter*)(IBaseAdapter*)this;
    }
    else if (riid == EIID_IListAdapter) {
        return (IListAdapter*)(IBaseAdapter*)this;
    }
    else {
        return NULL;
    }
}

SuggestionAdapter::SuggestionAdapter(
    /* [in] */ Editor* editor,
    /* [in] */ SuggestionsPopupWindow* popupWindow)
    : _SuggestionAdapter(editor, popupWindow)
{
}

UInt32 SuggestionAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SuggestionAdapter::Release()
{
    return ElRefBase::Release();
}

ECode SuggestionAdapter::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return NOERROR;
}

//==============================================================================
//              SuggestionSpanComparator
//==============================================================================
CAR_INTERFACE_IMPL(SuggestionSpanComparator, IComparator)

SuggestionSpanComparator::SuggestionSpanComparator(
    /* [in] */ SuggestionsPopupWindow* host)
    : mHost(host)
{
}

ECode SuggestionSpanComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;

    AutoPtr<ISuggestionSpan> span1 = ISuggestionSpan::Probe(lhs);
    AutoPtr<ISuggestionSpan> span2 = ISuggestionSpan::Probe(rhs);
    if (!span1 || !span2) {
        return E_INVALID_ARGUMENT;
    }

    Int32 flag1, flag2;
    span1->GetFlags(&flag1);
    span2->GetFlags(&flag2);
    if (flag1 != flag2) {
        // The order here should match what is used in updateDrawState
        Boolean easy1 = (flag1 & ISuggestionSpan::FLAG_EASY_CORRECT) != 0;
        Boolean easy2 = (flag2 & ISuggestionSpan::FLAG_EASY_CORRECT) != 0;
        Boolean misspelled1 = (flag1 & ISuggestionSpan::FLAG_MISSPELLED) != 0;
        Boolean misspelled2 = (flag2 & ISuggestionSpan::FLAG_MISSPELLED) != 0;
        if (easy1 && !misspelled1) { *result = -1; return NOERROR; }
        if (easy2 && !misspelled2) { *result = 1; return NOERROR; }
        if (misspelled1) { *result = -1; return NOERROR; }
        if (misspelled2) { *result = 1; return NOERROR; }
    }

    SuggestionsPopupWindow::SuggestionSpanIterator iter1 = mHost->mSpansLengths.Find(span1);
    SuggestionsPopupWindow::SuggestionSpanIterator iter2 = mHost->mSpansLengths.Find(span2);
    if (iter1 == mHost->mSpansLengths.End() || iter2 == mHost->mSpansLengths.End())
        return NOERROR;

    AutoPtr<IInteger32> integer1 = iter1->mSecond;
    AutoPtr<IInteger32> integer2 = iter2->mSecond;
    assert(integer1 && integer2);
    Int32 value1, value2;
    integer1->GetValue(&value1);
    integer2->GetValue(&value2);
    *result = value1 - value2;
    return NOERROR;
}

//==============================================================================
//              SuggestionsPopupWindow
//==============================================================================

const Int32 SuggestionsPopupWindow::MAX_NUMBER_SUGGESTIONS = ISuggestionSpan::SUGGESTIONS_MAX_SIZE;
const Int32 SuggestionsPopupWindow::ADD_TO_DICTIONARY = -1;
const Int32 SuggestionsPopupWindow::DELETE_TEXT = -2;

CAR_INTERFACE_IMPL(SuggestionsPopupWindow, IAdapterViewOnItemClickListener)

SuggestionsPopupWindow::SuggestionsPopupWindow(
    /* [in] */ Editor* editor)
    : PinnedPopupWindow(editor)
    , mNumberOfSuggestions(0)
    , mIsShowingUp(FALSE)
{
    mCursorWasVisibleBeforeSuggestions = mEditor->mCursorVisible;
    mSuggestionSpanComparator = new SuggestionSpanComparator(this);
}

SuggestionsPopupWindow::~SuggestionsPopupWindow()
{
}

ECode SuggestionsPopupWindow::CreatePopupWindow()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    mPopupWindow = new CustomPopupWindow(
            context, R::attr::textSuggestionsWindowStyle, mEditor, this);
    mPopupWindow->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NOT_NEEDED);
    mPopupWindow->SetFocusable(TRUE);
    mPopupWindow->SetClippingEnabled(FALSE);
    return NOERROR;
}

ECode SuggestionsPopupWindow::InitContentView()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    AutoPtr<IListView> listView;
    CListView::New(context, (IListView**)&listView);

    mSuggestionsAdapter = new SuggestionAdapter(mEditor, this);
    listView->SetAdapter((IListAdapter*)mSuggestionsAdapter->Probe(EIID_IListAdapter));
    listView->SetOnItemClickListener(this);
    mContentView = IViewGroup::Probe(listView);

    // Inflate the suggestion items once and for all. + 2 for add to dictionary and delete
    mSuggestionInfos = ArrayOf<SuggestionInfo*>::Alloc(MAX_NUMBER_SUGGESTIONS + 2);
    for (Int32 i = 0; i < mSuggestionInfos->GetLength(); i++) {
        AutoPtr<SuggestionInfo> info = new SuggestionInfo();
        mSuggestionInfos->Set(i, info);
    }
    return NOERROR;
}

Boolean SuggestionsPopupWindow::IsShowingUp()
{
    return mIsShowingUp;
}

void SuggestionsPopupWindow::OnParentLostFocus()
{
    mIsShowingUp = FALSE;
}

AutoPtr<ArrayOf<ISuggestionSpan*> > SuggestionsPopupWindow::GetSuggestionSpans()
{
    AutoPtr<ArrayOf<ISuggestionSpan*> > suggestionSpans;
    Int32 pos = mEditor->mTextView->GetSelectionStart();
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    if (text && ISpannable::Probe(text)) {
        ISpannable* spannable = ISpannable::Probe(text);
        AutoPtr< ArrayOf<IInterface*> > temp;
        spannable->GetSpans(pos, pos, EIID_ISuggestionSpan, (ArrayOf<IInterface*>**)&temp);
        suggestionSpans = ArrayOf<ISuggestionSpan*>::Alloc(temp->GetLength());
        for (Int32 i = 0; i < temp->GetLength(); i++){
            suggestionSpans->Set(i, ISuggestionSpan::Probe((*temp)[i]));
        }
        mSpansLengths.Clear();
        Int32 start, end;
        for (Int32 i = 0; i < suggestionSpans->GetLength(); ++i) {
            AutoPtr<ISuggestionSpan> suggestionSpan = ISuggestionSpan::Probe((*suggestionSpans)[i]);
            assert(suggestionSpan != NULL);
            spannable->GetSpanStart(suggestionSpan, &start);
            spannable->GetSpanEnd(suggestionSpan, &end);

            AutoPtr<IInteger32> integer;
            CInteger32::New(end - start, (IInteger32**)&integer);
            mSpansLengths[suggestionSpan] = integer;
        }

        // The suggestions are sorted according to their types (easy correction first, then
        // misspelled) and to the length of the text that they cover (shorter first).
//        Arrays.sort(suggestionSpans, mSuggestionSpanComparator);
        QuickSort(suggestionSpans, 0, suggestionSpans->GetLength() - 1);
    }

    return suggestionSpans;
}

void SuggestionsPopupWindow::QuickSort(ArrayOf<ISuggestionSpan*>* array, Int32 low, Int32 high)
{
    if (array == NULL || low >= high || low < 0)
        return;

    AutoPtr<ISuggestionSpan> privot = (*array)[low];
    Int32 i = low, j = high;
    Int32 result;
    while (i < j) {
        while (i < j) {
            mSuggestionSpanComparator->Compare(
                    (*array)[j]->Probe(EIID_IInterface),
                    privot->Probe(EIID_IInterface),
                    &result);
            if (result >= 0){
                --j;
            }
        }

        if (i < j) {
            array->Set(i++, (*array)[j]);
        }

        while (i < j) {
            mSuggestionSpanComparator->Compare(
                    (*array)[i]->Probe(EIID_IInterface),
                    privot->Probe(EIID_IInterface),
                    &result);
            if (result <= 0){
                ++i;
            }
        }

        if (i < j) {
            array->Set(j--, (*array)[i]);
        }
    }

    array->Set(i, privot);

    QuickSort(array, low, i - 1);
    QuickSort(array, i + 1, high);
}

ECode SuggestionsPopupWindow::Show()
{
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    if (text && IEditable::Probe(text)) {
        return NOERROR;
    }

    if (UpdateSuggestions()) {
        mCursorWasVisibleBeforeSuggestions = mEditor->mCursorVisible;
        mEditor->mTextView->SetCursorVisible(FALSE);
        mIsShowingUp = TRUE;
        PinnedPopupWindow::Show();
    }
    return NOERROR;
}

ECode SuggestionsPopupWindow::MeasureContent()
{
    AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
    AutoPtr<IDisplayMetrics> displayMetrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    Int32 widthPixels, heightPixels;
    displayMetrics->GetWidthPixels(&widthPixels);
    displayMetrics->GetHeightPixels(&heightPixels);

    Int32 horizontalMeasure = View::View::MeasureSpec::MakeMeasureSpec(
            widthPixels, View::View::MeasureSpec::AT_MOST);
    Int32 verticalMeasure = View::View::MeasureSpec::MakeMeasureSpec(
            heightPixels, View::View::MeasureSpec::AT_MOST);

    Int32 width = 0, measuredWidth;
    AutoPtr<IView> view;
    for (Int32 i = 0; i < mNumberOfSuggestions; i++) {
        AutoPtr<IView> newView;
        mSuggestionsAdapter->GetView(i, view, mContentView, (IView**)&newView);
        AutoPtr<IViewGroupLayoutParams> params;
        newView->GetLayoutParams((IViewGroupLayoutParams**)&params);
        params->SetWidth(IViewGroupLayoutParams::WRAP_CONTENT);
        newView->Measure(horizontalMeasure, verticalMeasure);
        newView->GetMeasuredWidth(&measuredWidth);
        width = Elastos::Core::Math::Max(width, measuredWidth);
        view = newView;
    }

    // Enforce the width based on actual text widths
    mContentView->Measure(
        View::View::MeasureSpec::MakeMeasureSpec(width, View::View::MeasureSpec::EXACTLY),
        verticalMeasure);

    AutoPtr<IDrawable> popupBackground;
    mPopupWindow->GetBackground((IDrawable**)&popupBackground);
    if (popupBackground != NULL) {
        if (mEditor->mTempRect == NULL) {
            CRect::New((IRect**)&mEditor->mTempRect);
        }
        Boolean isPadding;
        popupBackground->GetPadding(mEditor->mTempRect, &isPadding);
        Int32 left, right;
        mEditor->mTempRect->GetLeft(&left);
        mEditor->mTempRect->GetRight(&right);
        width += left + right;
    }
    mPopupWindow->SetWidth(width);
    return NOERROR;
}

Int32 SuggestionsPopupWindow::GetTextOffset()
{
    return mEditor->mTextView->GetSelectionStart();
}

Int32 SuggestionsPopupWindow::GetVerticalLocalPosition(
    /* [in] */ Int32 line)
{
    Int32 result = 0;
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    if (layout)
        layout->GetLineBottom(line, &result);

    return result;
}

Int32 SuggestionsPopupWindow::ClipVertically(
    /* [in] */ Int32 positionY)
{
    AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
    AutoPtr<IDisplayMetrics> displayMetrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    Int32 heightPixels;
    displayMetrics->GetHeightPixels(&heightPixels);

    Int32 height;
    mContentView->GetMeasuredHeight(&height);
    return Elastos::Core::Math::Min(positionY, heightPixels - height);
}

ECode SuggestionsPopupWindow::Hide()
{
    PinnedPopupWindow::Hide();
    return NOERROR;
}

Boolean SuggestionsPopupWindow::UpdateSuggestions()
{
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    AutoPtr<ArrayOf<ISuggestionSpan*> > suggestionSpans = GetSuggestionSpans();

    // Suggestions are shown after a delay: the underlying spans may have been removed
    if (NULL == suggestionSpans || suggestionSpans->GetLength() == 0)
        return FALSE;
    Int32 nbSpans = suggestionSpans->GetLength();

    mNumberOfSuggestions = 0;
    Int32 spanUnionStart;
    Int32 spanUnionEnd = 0;
    text->GetLength(&spanUnionStart);
    AutoPtr<ISuggestionSpan> misspelledSpan;
    Int32 underlineColor = 0;

    Int32 spanStart, spanEnd, otherSpanStart, otherSpanEnd;
    Int32 flags, textLength;
    for (Int32 spanIndex = 0; spanIndex < nbSpans; spanIndex++) {
        AutoPtr<ISuggestionSpan> suggestionSpan = (*suggestionSpans)[spanIndex];
        spannable->GetSpanStart(suggestionSpan, &spanStart);
        spannable->GetSpanEnd(suggestionSpan, &spanEnd);

        spanUnionStart = Elastos::Core::Math::Min(spanStart, spanUnionStart);
        spanUnionEnd = Elastos::Core::Math::Max(spanEnd, spanUnionEnd);

        suggestionSpan->GetFlags(&flags);
        if ((flags & ISuggestionSpan::FLAG_MISSPELLED) != 0) {
            misspelledSpan = suggestionSpan;
        }

        // The first span dictates the background color of the highlighted text
        if (spanIndex == 0) {
            suggestionSpan->GetUnderlineColor(&underlineColor);
        }

        AutoPtr<ArrayOf<String> > suggestions;
        suggestionSpan->GetSuggestions((ArrayOf<String>**)&suggestions);
        if (suggestions != NULL) {
            Int32 nbSuggestions = suggestions->GetLength();
            String info;
            for (Int32 suggestionIndex = 0; suggestionIndex < nbSuggestions; suggestionIndex++) {
                String suggestion = (*suggestions)[suggestionIndex];

                Boolean suggestionIsDuplicate = FALSE;
                for (Int32 i = 0; i < mNumberOfSuggestions; i++) {
                    (*mSuggestionInfos)[i]->mText->ToString(&info);
                    if (info.Equals(suggestion)) {
                        AutoPtr<ISuggestionSpan> otherSuggestionSpan = (*mSuggestionInfos)[i]->mSuggestionSpan;
                        spannable->GetSpanStart(otherSuggestionSpan, &otherSpanStart);
                        spannable->GetSpanEnd(otherSuggestionSpan, &otherSpanEnd);
                        if (spanStart == otherSpanStart && spanEnd == otherSpanEnd) {
                            suggestionIsDuplicate = TRUE;
                            break;
                        }
                    }
                }

                if (!suggestionIsDuplicate) {
                    AutoPtr<SuggestionInfo> suggestionInfo = (*mSuggestionInfos)[mNumberOfSuggestions];
                    suggestionInfo->mSuggestionSpan = suggestionSpan;
                    suggestionInfo->mSuggestionIndex = suggestionIndex;
                    suggestionInfo->mText->GetLength(&textLength);
                    AutoPtr<ICharSequence> seq;
                    CStringWrapper::New(suggestion, (ICharSequence**)&seq);
                    suggestionInfo->mText->Replace(0, textLength, seq);

                    mNumberOfSuggestions++;

                    if (mNumberOfSuggestions == MAX_NUMBER_SUGGESTIONS) {
                        // Also end outer for loop
                        spanIndex = nbSpans;
                        break;
                    }
                }
            }
        }
    }

    for (Int32 i = 0; i < mNumberOfSuggestions; i++) {
        HighlightTextDifferences((*mSuggestionInfos)[i], spanUnionStart, spanUnionEnd);
    }

    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    // Add "Add to dictionary" item if there is a span with the misspelled flag
    if (misspelledSpan != NULL) {
        Int32 misspelledStart, misspelledEnd;
        spannable->GetSpanStart(misspelledSpan, &misspelledStart);
        spannable->GetSpanEnd(misspelledSpan, &misspelledEnd);
        if (misspelledStart >= 0 && misspelledEnd > misspelledStart) {
            AutoPtr<SuggestionInfo> suggestionInfo = (*mSuggestionInfos)[mNumberOfSuggestions];
            suggestionInfo->mSuggestionSpan = misspelledSpan;
            suggestionInfo->mSuggestionIndex = ADD_TO_DICTIONARY;

            String infoStr;
            context->GetString(R::string::addToDictionary, &infoStr);
            suggestionInfo->mText->GetLength(&textLength);
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(infoStr, (ICharSequence**)&seq);
            suggestionInfo->mText->Replace(0, textLength, seq);
            suggestionInfo->mText->SetSpan(suggestionInfo->mHighlightSpan, 0, 0,
                    ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);

            mNumberOfSuggestions++;
        }
    }

    // Delete item
    AutoPtr<SuggestionInfo> suggestionInfo = (*mSuggestionInfos)[mNumberOfSuggestions];
    suggestionInfo->mSuggestionSpan = NULL;
    suggestionInfo->mSuggestionIndex = DELETE_TEXT;
    String infoStr;
    context->GetString(R::string::deleteText, &infoStr);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(infoStr, (ICharSequence**)&seq);
    suggestionInfo->mText->GetLength(&textLength);
    suggestionInfo->mText->Replace(0, textLength, seq);
    suggestionInfo->mText->SetSpan(suggestionInfo->mHighlightSpan, 0, 0,
            ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    mNumberOfSuggestions++;

    if (mEditor->mSuggestionRangeSpan == NULL) {
        CSuggestionRangeSpan::New((ISuggestionRangeSpan**)&mEditor->mSuggestionRangeSpan);
    }
    if (underlineColor == 0) {
        // Fallback on the default highlight color when the first span does not provide one
        mEditor->mSuggestionRangeSpan->SetBackgroundColor(mEditor->mTextView->mHighlightColor);
    }
    else {
        Float BACKGROUND_TRANSPARENCY = 0.4f;
        AutoPtr<IColor> helper;
        CColor::AcquireSingleton((IColor**)&helper);
        Int32 alpha;
        helper->Alpha(underlineColor, &alpha);
        Int32 newAlpha = (Int32) (alpha * BACKGROUND_TRANSPARENCY);
        mEditor->mSuggestionRangeSpan->SetBackgroundColor(
                (underlineColor & 0x00FFFFFF) + (newAlpha << 24));
    }
    spannable->SetSpan(mEditor->mSuggestionRangeSpan, spanUnionStart, spanUnionEnd,
        ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);

    mSuggestionsAdapter->NotifyDataSetChanged();
    return TRUE;
}

void SuggestionsPopupWindow::HighlightTextDifferences(
    /* [in] */ SuggestionInfo* suggestionInfo,
    /* [in] */ Int32 unionStart,
    /* [in] */ Int32 unionEnd)
{
    AutoPtr<ICharSequence> seq = mEditor->mTextView->GetText();
    ISpannable* text = ISpannable::Probe(seq);
    Int32 spanStart, spanEnd;
    text->GetSpanStart(suggestionInfo->mSuggestionSpan, &spanStart);
    text->GetSpanEnd(suggestionInfo->mSuggestionSpan, &spanEnd);

    // Adjust the start/end of the suggestion span
    Int32 textLength;
    suggestionInfo->mText->GetLength(&textLength);
    suggestionInfo->mSuggestionStart = spanStart - unionStart;
    suggestionInfo->mSuggestionEnd = suggestionInfo->mSuggestionStart + textLength;
    suggestionInfo->mText->SetSpan(suggestionInfo->mHighlightSpan, 0,
            textLength, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);

    // Add the text before and after the span.
    String textAsString;
    text->ToString(&textAsString);
    String subStr = textAsString.Substring(unionStart, spanStart);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(subStr, (ICharSequence**)&cs);
    suggestionInfo->mText->Insert(0, cs);

    cs = NULL;
    subStr = textAsString.Substring(spanEnd, unionEnd);
    CStringWrapper::New(subStr, (ICharSequence**)&cs);
    suggestionInfo->mText->Append(cs);
}

ECode SuggestionsPopupWindow::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    AutoPtr<ICharSequence> seq = mEditor->mTextView->GetText();
    IEditable* editable = IEditable::Probe(seq);

    AutoPtr<SuggestionInfo> suggestionInfo = (*mSuggestionInfos)[position];

    if (suggestionInfo->mSuggestionIndex == DELETE_TEXT) {
        Int32 spanUnionStart, spanUnionEnd;
        editable->GetSpanStart(mEditor->mSuggestionRangeSpan, &spanUnionStart);
        editable->GetSpanEnd(mEditor->mSuggestionRangeSpan, &spanUnionEnd);
        if (spanUnionStart >= 0 && spanUnionEnd > spanUnionStart) {
            Int32 length;
            editable->GetLength(&length);
            if (spanUnionEnd < length) {
                Char32 endChar, startChar = '0';
                editable->GetCharAt(spanUnionEnd, &endChar);
                if (spanUnionStart > 0) {
                    editable->GetCharAt(spanUnionStart - 1, &startChar);
                }
                // Do not leave two adjacent spaces after deletion, or one at beginning of text
                if (Character::IsSpaceChar(endChar) &&
                        (spanUnionStart == 0 || Character::IsSpaceChar(startChar))) {
                    spanUnionEnd = spanUnionEnd + 1;
                }
            }
            mEditor->mTextView->DeleteText_internal(spanUnionStart, spanUnionEnd);
        }
        Hide();
        return NOERROR;
    }

    Int32 spanStart, spanEnd;
    editable->GetSpanStart(suggestionInfo->mSuggestionSpan, &spanStart);
    editable->GetSpanEnd(suggestionInfo->mSuggestionSpan, &spanEnd);
    if (spanStart < 0 || spanEnd <= spanStart) {
        // Span has been removed
        Hide();
        return NOERROR;
    }

    String originalText;
    editable->ToString(&originalText);
    originalText = originalText.Substring(spanStart, spanEnd);

    if (suggestionInfo->mSuggestionIndex == ADD_TO_DICTIONARY) {
        AutoPtr<ILocale> locale = mEditor->mTextView->GetTextServicesLocale();
        String localeStr;
        locale->ToString(&localeStr);
        AutoPtr<IIntent> intent;
        CIntent::New(ISettings::ACTION_USER_DICTIONARY_INSERT, (IIntent**)&intent);
        intent->PutStringExtra(String("word"), originalText);
        intent->PutStringExtra(String("locale"), localeStr);
        // Put a listener to replace the original text with a word which the user
        // modified in a user dictionary dialog.
        AutoPtr<ITextView> itv = (ITextView*)(mEditor->mTextView->Probe(EIID_ITextView));
        mEditor->mUserDictionaryListener->WaitForUserDictionaryAdded(
                itv, originalText, spanStart, spanEnd);
        AutoPtr<IMessenger> msger;
        CMessenger::New(mEditor->mUserDictionaryListener, (IMessenger**)&msger);
        intent->PutParcelableExtra(String("listener"), IParcelable::Probe(msger));
        Int32 flags;
        intent->GetFlags(&flags);
        intent->SetFlags(flags | IIntent::FLAG_ACTIVITY_NEW_TASK);
        context->StartActivity(intent);
        // There is no way to know if the word was indeed added. Re-check.
        // TODO The ExtractEditText should remove the span in the original text instead
        editable->RemoveSpan(suggestionInfo->mSuggestionSpan);
        Selection::SetSelection(editable, spanEnd);
        mEditor->UpdateSpellCheckSpans(spanStart, spanEnd, FALSE);
    }
    else {
        // SuggestionSpans are removed by replace: save them before
        AutoPtr<ArrayOf<IInterface*> > temp;
        editable->GetSpans(spanStart, spanEnd,
                EIID_ISuggestionSpan, (ArrayOf<IInterface*>**)&temp);
        AutoPtr<ArrayOf<ISuggestionSpan*> > suggestionSpans = ArrayOf<ISuggestionSpan*>::Alloc(temp->GetLength());
        for(Int32 i = 0; i < temp->GetLength(); i++) {
            suggestionSpans->Set(i, ISuggestionSpan::Probe((*temp)[i]));
        }
        Int32 length = suggestionSpans->GetLength();

        AutoPtr<ArrayOf<Int32> > suggestionSpansStarts = ArrayOf<Int32>::Alloc(length);
        AutoPtr<ArrayOf<Int32> > suggestionSpansEnds = ArrayOf<Int32>::Alloc(length);
        AutoPtr<ArrayOf<Int32> > suggestionSpansFlags = ArrayOf<Int32>::Alloc(length);

        Int32 start, end, flags;
        for (Int32 i = 0; i < length; i++) {
            ISuggestionSpan* suggestionSpan = (*suggestionSpans)[i];
            editable->GetSpanStart(suggestionSpan, &start);
            editable->GetSpanEnd(suggestionSpan, &end);
            editable->GetSpanFlags(suggestionSpan, &flags);
            (*suggestionSpansStarts)[i] = start;
            (*suggestionSpansEnds)[i] = end;
            (*suggestionSpansFlags)[i] = flags;

            // Remove potential misspelled flags
            Int32 suggestionSpanFlags;
            suggestionSpan->GetFlags(&suggestionSpanFlags);
            if ((suggestionSpanFlags & ISuggestionSpan::FLAG_MISSPELLED) > 0) {
                suggestionSpanFlags &= ~ISuggestionSpan::FLAG_MISSPELLED;
                suggestionSpanFlags &= ~ISuggestionSpan::FLAG_EASY_CORRECT;
                suggestionSpan->SetFlags(suggestionSpanFlags);
            }
        }

        Int32 suggestionStart = suggestionInfo->mSuggestionStart;
        Int32 suggestionEnd = suggestionInfo->mSuggestionEnd;
        AutoPtr<ICharSequence> subSeq;
        suggestionInfo->mText->SubSequence(suggestionStart, suggestionEnd, (ICharSequence**)&subSeq);
        mEditor->mTextView->ReplaceText_internal(spanStart, spanEnd, subSeq);

        // Notify source IME of the suggestion pick. Do this before swaping texts.
        String className;
        suggestionInfo->mSuggestionSpan->GetNotificationTargetClassName(&className);
        AutoPtr<ICharSequence> cnSeq;
        CStringWrapper::New(className, (ICharSequence**)&cnSeq);
        if (TextUtils::IsEmpty(cnSeq)) {
            AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
            if (imm != NULL) {
                imm->NotifySuggestionPicked(
                        suggestionInfo->mSuggestionSpan,
                        originalText,
                        suggestionInfo->mSuggestionIndex);
            }
        }

        // Swap text content between actual text and Suggestion span
        AutoPtr<ArrayOf<String> > suggestions;
        suggestionInfo->mSuggestionSpan->GetSuggestions((ArrayOf<String>**)&suggestions);
        suggestions->Set(suggestionInfo->mSuggestionIndex, originalText);

        // Restore previous SuggestionSpans
        Int32 seqLen;
        subSeq->GetLength(&seqLen);
        Int32 lengthDifference = seqLen - (spanEnd - spanStart);
        for (Int32 i = 0; i < length; i++) {
            // Only spans that include the modified region make sense after replacement
            // Spans partially included in the replaced region are removed, there is no
            // way to assign them a valid range after replacement
            if ((*suggestionSpansStarts)[i] <= spanStart &&
                    (*suggestionSpansEnds)[i] >= spanEnd) {
                mEditor->mTextView->SetSpan_internal(
                        (*suggestionSpans)[i]->Probe(EIID_IInterface),
                        (*suggestionSpansStarts)[i],
                        (*suggestionSpansEnds)[i] + lengthDifference,
                        (*suggestionSpansFlags)[i]);
            }
        }

        // Move cursor at the end of the replaced word
        Int32 newCursorPosition = spanEnd + lengthDifference;
        mEditor->mTextView->SetCursorPosition_internal(newCursorPosition, newCursorPosition);
    }

    Hide();
    return NOERROR;
}

//==============================================================================
//              SelectionActionModeCallback
//==============================================================================
SelectionActionModeCallback::SelectionActionModeCallback(
    /* [in] */ Editor* editor)
    : mEditor(editor)
{
}

PInterface SelectionActionModeCallback::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IActionModeCallback*)this;
    }
    else if (riid == EIID_IActionModeCallback) {
        return (IInterface*)(IActionModeCallback*)this;
    }

    return NULL;
}

ECode SelectionActionModeCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return NOERROR;
}

UInt32 SelectionActionModeCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SelectionActionModeCallback::Release()
{
    return ElRefBase::Release();
}

ECode SelectionActionModeCallback::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    assert(mode != NULL && menu != NULL && result != NULL);

    AutoPtr<ITypedArray> styledAttributes;
    AutoPtr<ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::SelectionModeDrawables),
        ARRAY_SIZE(R::styleable::SelectionModeDrawables));

    AutoPtr<IContext> ctx = mEditor->mTextView->GetContext();
    assert(ctx != NULL);

    ctx->ObtainStyledAttributes(
            values, (ITypedArray**)&styledAttributes);
    assert(styledAttributes != NULL);

    Boolean allowText = FALSE;
    AutoPtr<IResources> resources;
    ctx->GetResources((IResources**)&resources);
    resources->GetBoolean(R::bool_::config_allowActionMenuItemTextWithIcon, &allowText);

    String text;
    ctx->GetString(R::string::textSelectionCABTitle, &text);
    AutoPtr<ICharSequence> title;
    CStringWrapper::New(text, (ICharSequence**)&title);
    mode->SetTitle(title);
    mode->SetSubtitle(NULL);
    mode->SetTitleOptionalHint(TRUE);

    Int32 selectAllIconId = 0; // No icon by default
    if (!allowText) {
        // Provide an icon, text will not be displayed on smaller screens.
        styledAttributes->GetResourceId(
            R::styleable::SelectionModeDrawables_actionModeSelectAllDrawable, 0, &selectAllIconId);
    }

    AutoPtr<IMenuItem> menuItem;
    menu->Add(0, R::id::selectAll, 0, R::string::selectAll, (IMenuItem**)&menuItem);
    menuItem->SetIcon(selectAllIconId);
    menuItem->SetAlphabeticShortcut('a');
    menuItem->SetShowAsAction(IMenuItem::SHOW_AS_ACTION_ALWAYS | IMenuItem::SHOW_AS_ACTION_WITH_TEXT);

    if (mEditor->mTextView->CanCut()) {
        AutoPtr<IMenuItem> item, temp;
        menu->Add(0, R::id::cut, 0, R::string::cut, (IMenuItem**)&item);

        Int32 id = 0;
        styledAttributes->GetResourceId(R::styleable::SelectionModeDrawables_actionModeCutDrawable, 0, &id);
        item->SetIcon(id);
        item->SetAlphabeticShortcut('x');
        item->SetShowAsAction(IMenuItem::SHOW_AS_ACTION_ALWAYS | IMenuItem::SHOW_AS_ACTION_WITH_TEXT);
    }

    if (mEditor->mTextView->CanCopy()) {
        AutoPtr<IMenuItem> item, temp;
        menu->Add(0, R::id::copy, 0, R::string::copy, (IMenuItem**)&item);

        Int32 id = 0;
        styledAttributes->GetResourceId(R::styleable::SelectionModeDrawables_actionModeCopyDrawable, 0, &id);
        item->SetIcon(id);
        item->SetAlphabeticShortcut('c');
        item->SetShowAsAction(IMenuItem::SHOW_AS_ACTION_ALWAYS | IMenuItem::SHOW_AS_ACTION_WITH_TEXT);
    }

    if (mEditor->mTextView->CanPaste()) {
        AutoPtr<IMenuItem> item;
        menu->Add(0, R::id::paste, 0, R::string::paste, (IMenuItem**)&item);

        Int32 id = 0;
        styledAttributes->GetResourceId(R::styleable::SelectionModeDrawables_actionModePasteDrawable, 0, &id);
        item->SetIcon(id);
        item->SetAlphabeticShortcut('v');
        item->SetShowAsAction(IMenuItem::SHOW_AS_ACTION_ALWAYS | IMenuItem::SHOW_AS_ACTION_WITH_TEXT);
    }

    styledAttributes->Recycle();

    if (mEditor->mCustomSelectionActionModeCallback != NULL) {
        Boolean state = FALSE;
        mEditor->mCustomSelectionActionModeCallback->OnCreateActionMode(mode, menu, &state);
        if (!state) {
            // The custom mode can choose to cancel the action mode
            *result = FALSE;
            return NOERROR;
        }
    }

    Boolean has = FALSE;
    AutoPtr<IView> view;
    if ((menu->HasVisibleItems(&has), has) || (mode->GetCustomView((IView**)&view), view.Get()) != NULL) {
        mEditor->GetSelectionController()->Show();
        mEditor->mTextView->SetHasTransientState(TRUE);
        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

ECode SelectionActionModeCallback::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (mEditor->mCustomSelectionActionModeCallback != NULL) {
        return mEditor->mCustomSelectionActionModeCallback->OnPrepareActionMode(mode, menu, result);
    }

    *result = TRUE;
    return NOERROR;
}

ECode SelectionActionModeCallback::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (mEditor->mCustomSelectionActionModeCallback != NULL) {
        Boolean state = FALSE;
        if ((mEditor->mCustomSelectionActionModeCallback->OnActionItemClicked(mode, item, &state), state)) {
            *result = TRUE;
            return NOERROR;
        }
    }

    assert(item != NULL);
    Int32 id = 0;
    item->GetItemId(&id);
    *result = mEditor->mTextView->OnTextContextMenuItem(id);
    return NOERROR;
}

ECode SelectionActionModeCallback::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    if (mEditor->mCustomSelectionActionModeCallback != NULL) {
        mEditor->mCustomSelectionActionModeCallback->OnDestroyActionMode(mode);
    }

    /*
     * If we're ending this mode because we're detaching from a window,
     * we still have selection state to preserve. Don't clear it, we'll
     * bring back the selection mode when (if) we get reattached.
     */
    if (!mEditor->mPreserveDetachedSelection) {
        Selection::SetSelection(ISpannable::Probe(mEditor->mTextView->GetText()),
                mEditor->mTextView->GetSelectionEnd());
        mEditor->mTextView->SetHasTransientState(FALSE);
    }

    if (mEditor->mSelectionModifierCursorController != NULL) {
        mEditor->mSelectionModifierCursorController->Hide();
    }

    mEditor->mSelectionActionMode = NULL;
    return NOERROR;
}

//==============================================================================
//              ActionPopupWindow
//==============================================================================

const Int32 ActionPopupWindow::POPUP_TEXT_LAYOUT = R::layout::text_edit_action_popup_text;

CAR_INTERFACE_IMPL(ActionPopupWindow, IViewOnClickListener)

ActionPopupWindow::ActionPopupWindow(
    /* [in] */ Editor* editor)
    : PinnedPopupWindow(editor)
{
}

ActionPopupWindow::~ActionPopupWindow()
{
}

ECode ActionPopupWindow::CreatePopupWindow()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    mPopupWindow = new MyPopupWindow(context, NULL, R::attr::textSelectHandleWindowStyle);
    mPopupWindow->SetClippingEnabled(TRUE);
    return NOERROR;
}

ECode ActionPopupWindow::InitContentView()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();

    AutoPtr<ILinearLayout> linearLayout;
    CLinearLayout::New(context, (ILinearLayout**)&linearLayout);
    linearLayout->SetOrientation(ILinearLayout::HORIZONTAL);

    mContentView = IViewGroup::Probe(linearLayout);
    mContentView->SetBackgroundResource(R::drawable::text_edit_paste_window);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(
        IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    AutoPtr<IViewGroupLayoutParams> wrapContent;
    CViewGroupLayoutParams::New(
        IViewGroupLayoutParams::WRAP_CONTENT,
        IViewGroupLayoutParams::WRAP_CONTENT,
        (IViewGroupLayoutParams**)&wrapContent);

    inflater->Inflate(POPUP_TEXT_LAYOUT, NULL, (IView**)&mPasteTextView);
    mPasteTextView->SetLayoutParams(wrapContent);
    mPasteTextView->SetText(R::string::paste);
    mPasteTextView->SetOnClickListener(this);
    mContentView->AddView(mPasteTextView);

    inflater->Inflate(POPUP_TEXT_LAYOUT, NULL, (IView**)&mReplaceTextView);
    mReplaceTextView->SetLayoutParams(wrapContent);
    mReplaceTextView->SetText(R::string::replace);
    mReplaceTextView->SetOnClickListener(this);
    mContentView->AddView(mReplaceTextView);

    return NOERROR;
}

ECode ActionPopupWindow::Show()
{
    Boolean canPaste = mEditor->mTextView->CanPaste();
    Boolean canSuggest = mEditor->mTextView->IsSuggestionsEnabled()
        && mEditor->IsCursorInsideSuggestionSpan();
    mPasteTextView->SetVisibility(canPaste ? IView::VISIBLE : IView::GONE);
    mReplaceTextView->SetVisibility(canSuggest ? IView::VISIBLE : IView::GONE);

    if (!canPaste && !canSuggest) return NOERROR;

    Show();
    return NOERROR;
}

ECode ActionPopupWindow::OnClick(
    /* [in] */ IView* view)
{
    if (view == mPasteTextView->Probe(EIID_IView) && mEditor->mTextView->CanPaste()) {
        mEditor->mTextView->OnTextContextMenuItem(R::id::paste);
        Hide();
    } else if (view == mReplaceTextView->Probe(EIID_IView)) {
        Int32 middle = GetTextOffset();
        mEditor->StopSelectionActionMode();
        AutoPtr<ICharSequence> seq = mEditor->mTextView->GetText();
        ISpannable* spannable = ISpannable::Probe(seq);
        Selection::SetSelection(spannable, middle);
        mEditor->ShowSuggestions();
    }
    return NOERROR;
}

Int32 ActionPopupWindow::GetTextOffset()
{
    return (mEditor->mTextView->GetSelectionStart() + mEditor->mTextView->GetSelectionEnd()) / 2;
}

Int32 ActionPopupWindow::GetVerticalLocalPosition(
    /* [in] */ Int32 line)
{
    Int32 result = 0;
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    if (layout) {
        layout->GetLineTop(line, &result);
    }

    Int32 h;
    mContentView->GetMeasuredHeight(&h);
    result += h;
    return result;
}

Int32 ActionPopupWindow::ClipVertically(
    /* [in] */ Int32 positionY)
{
    if (positionY < 0) {
        Int32 offset = GetTextOffset();
        AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
        Int32 line, top, bottom, h;
        mContentView->GetMeasuredHeight(&h);
        layout->GetLineForOffset(offset, &line);
        layout->GetLineBottom(line, &bottom);
        layout->GetLineTop(line, &top);
        positionY += bottom - top;
        positionY += h;

        // Assumes insertion and selection handles share the same height
        AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
        AutoPtr<IDrawable> handle;
        resources->GetDrawable(mEditor->mTextView->mTextSelectHandleRes, (IDrawable**)&handle);
        Int32 height;
        handle->GetIntrinsicHeight(&height);
        positionY += height;
    }

    return positionY;
}

//==============================================================================
//              HandleView
//==============================================================================

const Int32 HandleView::HISTORY_SIZE;
const Int32 HandleView::TOUCH_UP_FILTER_DELAY_AFTER;
const Int32 HandleView::TOUCH_UP_FILTER_DELAY_BEFORE;

IVIEW_METHODS_IMPL(HandleView, Elastos::Droid::View::View)
IDRAWABLECALLBACK_METHODS_IMPL(HandleView, Elastos::Droid::View::View)
IKEYEVENTCALLBACK_METHODS_IMPL(HandleView, Elastos::Droid::View::View)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(HandleView, Elastos::Droid::View::View)

PInterface HandleView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_IInterface) {
        return (IInterface*)(IView*)this;
    }
    else if (riid == EIID_IDrawableCallback) {
        return (IInterface*)(IDrawableCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IInterface*)(IKeyEventCallback*)this;
    }
    else if (riid == EIID_IAccessibilityEventSource) {
        return (IInterface*)(IAccessibilityEventSource*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IInterface*)(IWeakReferenceSource*)this;
    }
    return NULL;
}

ECode HandleView::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* IID)
{
    assert(0);
    return NOERROR;
}

UInt32 HandleView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HandleView::Release()
{
    return ElRefBase::Release();
}

ECode HandleView::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

HandleView::HandleView(
    /* [in] */ IDrawable* drawableLtr,
    /* [in] */ IDrawable* drawableRtl,
    /* [in] */ Editor* editor)
    : View(editor->mTextView->GetContext())
    , mEditor(editor)
    , mHotspotX(0)
    , mContainer(NULL)
    , mPositionX(0)
    , mPositionY(0)
    , mIsDragging(0)
    , mTouchToWindowOffsetX(0)
    , mTouchToWindowOffsetY(0)
    , mTouchOffsetY(0)
    , mIdealVerticalOffset(0)
    , mLastParentX(0)
    , mLastParentY(0)
    , mPreviousOffset(-1)
    , mPositionHasChanged(TRUE)
    , mPreviousOffsetIndex(0)
    , mNumberPreviousOffsets(0)
{
    mPreviousOffsetsTimes = ArrayOf<Int64>::Alloc(HISTORY_SIZE);
    mPreviousOffsets = ArrayOf<Int32>::Alloc(HISTORY_SIZE);

    mDrawableLtr = drawableLtr;
    mDrawableRtl = drawableRtl;
}

void HandleView::Init()
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    CPopupWindow::New(context, NULL, R::attr::textSelectHandleWindowStyle,
            (IPopupWindow**)&mContainer);
    mContainer->SetSplitTouchEnabled(TRUE);
    mContainer->SetClippingEnabled(FALSE);
    mContainer->SetWindowLayoutType(IWindowManagerLayoutParams::TYPE_APPLICATION_SUB_PANEL);
    mContainer->SetContentView((IView*)this->Probe(EIID_IView));

    UpdateDrawable();

    Int32 handleHeight;
    mDrawable->GetIntrinsicHeight(&handleHeight);
    mTouchOffsetY = -0.3f * handleHeight;
    mIdealVerticalOffset = 0.7f * handleHeight;
}

void HandleView::UpdateDrawable()
{
    Int32 offset = GetCurrentCursorOffset();
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    Boolean isRtlCharAtOffset;
    layout->IsRtlCharAt(offset, &isRtlCharAtOffset);
    mDrawable = isRtlCharAtOffset ? mDrawableRtl : mDrawableLtr;
    mHotspotX = GetHotspotX(mDrawable, isRtlCharAtOffset);
}

void HandleView::StartTouchUpFilter(
    /* [in] */ Int32 offset)
{
    mNumberPreviousOffsets = 0;
    AddPositionToTouchUpFilter(offset);
}

void HandleView::AddPositionToTouchUpFilter(
    /* [in] */ Int32 offset)
{
    mPreviousOffsetIndex = (mPreviousOffsetIndex + 1) % HISTORY_SIZE;
    (*mPreviousOffsets)[mPreviousOffsetIndex] = offset;
    (*mPreviousOffsetsTimes)[mPreviousOffsetIndex] = SystemClock::GetUptimeMillis();
    mNumberPreviousOffsets++;
}

void HandleView::FilterOnTouchUp()
{
    Int64 now = SystemClock::GetUptimeMillis();
    Int32 i = 0;
    Int32 index = mPreviousOffsetIndex;
    Int32 iMax = Elastos::Core::Math::Min(mNumberPreviousOffsets, HISTORY_SIZE);
    while (i < iMax && (now - (*mPreviousOffsetsTimes)[index]) < TOUCH_UP_FILTER_DELAY_AFTER) {
        i++;
        index = (mPreviousOffsetIndex - i + HISTORY_SIZE) % HISTORY_SIZE;
    }

    if (i > 0 && i < iMax &&
            (now - (*mPreviousOffsetsTimes)[index]) > TOUCH_UP_FILTER_DELAY_BEFORE) {
        PositionAtCursorOffset((*mPreviousOffsets)[index], FALSE);
    }
}

Boolean HandleView::OffsetHasBeenChanged()
{
    return mNumberPreviousOffsets > 1;
}

void HandleView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 width, height;
    mDrawable->GetIntrinsicWidth(&width);
    mDrawable->GetIntrinsicHeight(&height);
    SetMeasuredDimension(width, height);
}

void HandleView::Show()
{
    if (IsShowing()) return;

    AutoPtr<PositionListener> listener = mEditor->GetPositionListener();
    listener->AddSubscriber(this, TRUE /* local position may change */);

    // Make sure the offset is always considered new, even when focusing at same position
    mPreviousOffset = -1;
    PositionAtCursorOffset(GetCurrentCursorOffset(), FALSE);

    HideActionPopupWindow();
}

void HandleView::Dismiss()
{
    mIsDragging = FALSE;
    mContainer->Dismiss();
    OnDetached();
}

void HandleView::Hide()
{
    Dismiss();

    AutoPtr<PositionListener> listener = mEditor->GetPositionListener();
    listener->RemoveSubscriber(this);
}

void HandleView::ShowActionPopupWindow(
    /* [in] */ Int32 delay)
{
    if (mActionPopupWindow == NULL) {
        mActionPopupWindow = new ActionPopupWindow(mEditor);
        mActionPopupWindow->Init();
    }

    if (mActionPopupShower == NULL) {
        mActionPopupShower = new ActionPopupShowerRunnable(mActionPopupWindow.Get());
    }
    else {
        mEditor->mTextView->RemoveCallbacks(mActionPopupShower);
    }

    mEditor->mTextView->PostDelayed(mActionPopupShower, delay);
}

void HandleView::HideActionPopupWindow()
{
    if (mActionPopupShower != NULL) {
        mEditor->mTextView->RemoveCallbacks(mActionPopupShower);
    }
    if (mActionPopupWindow != NULL) {
        mActionPopupWindow->Hide();
    }
}

Boolean HandleView::IsShowing()
{
    Boolean isShowing;
    mContainer->IsShowing(&isShowing);
    return isShowing;
}

Boolean HandleView::IsVisible()
{
    // Always show a dragging handle.
     if (mIsDragging) {
         return TRUE;
     }

     if (mEditor->mTextView->IsInBatchEditMode()) {
         return FALSE;
     }

     return mEditor->IsPositionVisible(mPositionX + mHotspotX, mPositionY);
}

void HandleView::PositionAtCursorOffset(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean parentScrolled)
{
    // A HandleView relies on the layout, which may be nulled by external methods
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    if (layout == NULL) {
        // Will update controllers' state, hiding them and stopping selection mode if needed
        mEditor->PrepareCursorControllers();
        return;
    }

    Boolean offsetChanged = offset != mPreviousOffset;
    if (offsetChanged || parentScrolled) {
        if (offsetChanged) {
            UpdateSelection(offset);
            AddPositionToTouchUpFilter(offset);
        }
        Int32 line;
        layout->GetLineForOffset(offset, &line);

        Float hor;
        layout->GetPrimaryHorizontal(offset, &hor);
        Int32 bottom;
        layout->GetLineBottom(line, &bottom);
        mPositionX = (Int32) (hor - 0.5f - mHotspotX);
        mPositionY = bottom;

        // Take TextView's padding and scroll into account.
        mPositionX += mEditor->mTextView->ViewportToContentHorizontalOffset();
        mPositionY += mEditor->mTextView->ViewportToContentVerticalOffset();

        mPreviousOffset = offset;
        mPositionHasChanged = TRUE;
    }
}

void HandleView::UpdatePosition(
    /* [in] */ Int32 parentPositionX,
    /* [in] */ Int32 parentPositionY,
    /* [in] */ Boolean parentPositionChanged,
    /* [in] */ Boolean parentScrolled)
{
    PositionAtCursorOffset(GetCurrentCursorOffset(), parentScrolled);
    if (parentPositionChanged || mPositionHasChanged) {
        if (mIsDragging) {
            // Update touchToWindow offset in case of parent scrolling while dragging
            if (parentPositionX != mLastParentX || parentPositionY != mLastParentY) {
                mTouchToWindowOffsetX += parentPositionX - mLastParentX;
                mTouchToWindowOffsetY += parentPositionY - mLastParentY;
                mLastParentX = parentPositionX;
                mLastParentY = parentPositionY;
            }

            OnHandleMoved();
        }

        if (IsVisible()) {
            Int32 positionX = parentPositionX + mPositionX;
            Int32 positionY = parentPositionY + mPositionY;
            if (IsShowing()) {
                mContainer->Update(positionX, positionY, -1, -1);
            } else {
                mContainer->ShowAtLocation(
                        (IView*)mEditor->mTextView->Probe(EIID_IView),
                        IGravity::NO_GRAVITY,
                        positionX, positionY);
            }
        } else {
            if (IsShowing()) {
                Dismiss();
            }
        }

        mPositionHasChanged = FALSE;
    }
}

void HandleView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    mDrawable->SetBounds(0, 0, mRight - mLeft, mBottom - mTop);
    mDrawable->Draw(canvas);
}

Boolean HandleView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 mask;
    event->GetActionMasked(&mask);
    Float rawX, rawY;
    event->GetRawX(&rawX);
    event->GetRawY(&rawY);
    switch (mask) {
        case IMotionEvent::ACTION_DOWN: {
            StartTouchUpFilter(GetCurrentCursorOffset());
            mTouchToWindowOffsetX = rawX - mPositionX;
            mTouchToWindowOffsetY = rawY - mPositionY;

            AutoPtr<PositionListener> positionListener = mEditor->GetPositionListener();
            mLastParentX = positionListener->GetPositionX();
            mLastParentY = positionListener->GetPositionY();
            mIsDragging = true;
            break;
        }

        case IMotionEvent::ACTION_MOVE: {
            // Vertical hysteresis: vertical down movement tends to snap to ideal offset
            Float previousVerticalOffset = mTouchToWindowOffsetY - mLastParentY;
            Float currentVerticalOffset = rawY - mPositionY - mLastParentY;
            Float newVerticalOffset;
            if (previousVerticalOffset < mIdealVerticalOffset) {
                newVerticalOffset = Elastos::Core::Math::Min(currentVerticalOffset, mIdealVerticalOffset);
                newVerticalOffset = Elastos::Core::Math::Max(newVerticalOffset, previousVerticalOffset);
            } else {
                newVerticalOffset = Elastos::Core::Math::Max(currentVerticalOffset, mIdealVerticalOffset);
                newVerticalOffset = Elastos::Core::Math::Min(newVerticalOffset, previousVerticalOffset);
            }
            mTouchToWindowOffsetY = newVerticalOffset + mLastParentY;

            Float newPosX = rawX - mTouchToWindowOffsetX + mHotspotX;
            Float newPosY = rawY - mTouchToWindowOffsetY + mTouchOffsetY;

            UpdatePosition(newPosX, newPosY);
            break;
        }

        case IMotionEvent::ACTION_UP:
            FilterOnTouchUp();
            mIsDragging = FALSE;
            break;

        case IMotionEvent::ACTION_CANCEL:
            mIsDragging = FALSE;
            break;
    }
    return TRUE;
}

Boolean HandleView::IsDragging()
{
    return mIsDragging;
}

void HandleView::OnHandleMoved()
{
    HideActionPopupWindow();
}

void HandleView::OnDetached()
{
    HideActionPopupWindow();
}

//==============================================================================
//              InsertionHandleView
//==============================================================================

const Int32 InsertionHandleView::DELAY_BEFORE_HANDLE_FADES_OUT = 4000;
const Int32 InsertionHandleView::RECENT_CUT_COPY_DURATION = 15 * 1000; // seconds

PInterface InsertionHandleView::Probe(
    /* [in]  */ REIID riid)
{
    return HandleView::Probe(riid);
}

ECode InsertionHandleView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return NOERROR;
}

UInt32 InsertionHandleView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InsertionHandleView::Release()
{
    return ElRefBase::Release();
}

InsertionHandleView::InsertionHandleView(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Editor* editor)
    : HandleView(drawable, drawable, editor)
    , mDownPositionX(0)
    , mDownPositionY(0)
{
}

void InsertionHandleView::Show()
{
    HandleView::Show();

    Int64 durationSinceCutOrCopy =
            SystemClock::GetUptimeMillis() - TextView::LAST_CUT_OR_COPY_TIME;
    if (durationSinceCutOrCopy < RECENT_CUT_COPY_DURATION) {
        ShowActionPopupWindow(0);
    }

    HideAfterDelay();
}

void InsertionHandleView::ShowWithActionPopup()
{
    Show();
    ShowActionPopupWindow(0);
}

void InsertionHandleView::HideAfterDelay()
{
    if (mHider == NULL) {
        mHider = new InsertionHandleViewHiderRunnable(this);
    } else {
        RemoveHiderCallback();
    }
    mEditor->mTextView->PostDelayed(mHider, DELAY_BEFORE_HANDLE_FADES_OUT);
}

void InsertionHandleView::RemoveHiderCallback()
{
    if (mHider != NULL) {
        mEditor->mTextView->RemoveCallbacks(mHider);
    }
}

Int32 InsertionHandleView::GetHotspotX(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Boolean isRtlRun)
{
    assert(drawable);
    Int32 width;
    drawable->GetIntrinsicWidth(&width);
    return width / 2;
}

Boolean InsertionHandleView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Boolean result = HandleView::OnTouchEvent(ev);
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();
    Int32 mask;
    ev->GetActionMasked(&mask);
    Float rawX, rawY;
    ev->GetRawX(&rawX);
    ev->GetRawY(&rawY);
    switch (mask) {
        case IMotionEvent::ACTION_DOWN:
            mDownPositionX = rawX;
            mDownPositionY = rawY;
            break;

        case IMotionEvent::ACTION_UP:
            if (!OffsetHasBeenChanged()) {
                Float deltaX = mDownPositionX - rawX;
                Float deltaY = mDownPositionY - rawY;
                Float distanceSquared = deltaX * deltaX + deltaY * deltaY;

                AutoPtr<IViewConfigurationHelper> helper;
                CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
                AutoPtr<IViewConfiguration> viewConfiguration;
                helper->Get(context, (IViewConfiguration**)&viewConfiguration);
                Int32 touchSlop;
                viewConfiguration->GetScaledTouchSlop(&touchSlop);

                if (distanceSquared < touchSlop * touchSlop) {
                    if (mActionPopupWindow != NULL && mActionPopupWindow->IsShowing()) {
                        // Tapping on the handle dismisses the displayed action popup
                        mActionPopupWindow->Hide();
                    } else {
                        ShowWithActionPopup();
                    }
                }
            }
            HideAfterDelay();
            break;

        case IMotionEvent::ACTION_CANCEL:
            HideAfterDelay();
            break;

        default:
            break;
    }

    return result;
}


Int32 InsertionHandleView::GetCurrentCursorOffset()
{
    return mEditor->mTextView->GetSelectionStart();
}

void InsertionHandleView::UpdateSelection(
    /* [in] */ Int32 offset)
{
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    Selection::SetSelection(spannable, offset);
}

void InsertionHandleView::UpdatePosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    PositionAtCursorOffset(mEditor->mTextView->GetOffsetForPosition(x, y), FALSE);
}

void InsertionHandleView::OnHandleMoved()
{
    HandleView::OnHandleMoved();
    RemoveHiderCallback();
}

void InsertionHandleView::OnDetached()
{
    HandleView::OnDetached();
    RemoveHiderCallback();
}

//==============================================================================
//              SelectionStartHandleView
//==============================================================================

PInterface SelectionStartHandleView::Probe(
    /* [in]  */ REIID riid)
{
    return HandleView::Probe(riid);
}

ECode SelectionStartHandleView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return NOERROR;
}

UInt32 SelectionStartHandleView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SelectionStartHandleView::Release()
{
    return ElRefBase::Release();
}

SelectionStartHandleView::SelectionStartHandleView(
    /* [in] */ IDrawable* drawableLtr,
    /* [in] */ IDrawable* drawableRtl,
    /* [in] */ Editor* editor)
    : HandleView(drawableLtr, drawableRtl, editor)
{
}

Int32 SelectionStartHandleView::GetHotspotX(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Boolean isRtlRun)
{
    assert(drawable);
    Int32 width;
    drawable->GetIntrinsicWidth(&width);
    if (isRtlRun) {
        return width / 4;
    } else {
        return (width * 3) / 4;
    }
}

Int32 SelectionStartHandleView::GetCurrentCursorOffset()
{
    return mEditor->mTextView->GetSelectionStart();
}

void SelectionStartHandleView::UpdateSelection(
    /* [in] */ Int32 offset)
{
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    Selection::SetSelection(spannable, offset, mEditor->mTextView->GetSelectionEnd());
    UpdateDrawable();
}

void SelectionStartHandleView::UpdatePosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Int32 offset = mEditor->mTextView->GetOffsetForPosition(x, y);

    // Handles can not cross and selection is at least one character
    Int32 selectionEnd = mEditor->mTextView->GetSelectionEnd();
    if (offset >= selectionEnd)
        offset = Elastos::Core::Math::Max(0, selectionEnd - 1);

    PositionAtCursorOffset(offset, FALSE);
}

AutoPtr<ActionPopupWindow> SelectionStartHandleView::GetActionPopupWindow()
{
    return mActionPopupWindow;
}

//==============================================================================
//              SelectionEndHandleView
//==============================================================================

PInterface SelectionEndHandleView::Probe(
    /* [in]  */ REIID riid)
{
    return HandleView::Probe(riid);
}

ECode SelectionEndHandleView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return NOERROR;
}

UInt32 SelectionEndHandleView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SelectionEndHandleView::Release()
{
    return ElRefBase::Release();
}

SelectionEndHandleView::SelectionEndHandleView(
    /* [in] */ IDrawable* drawableLtr,
    /* [in] */ IDrawable* drawableRtl,
    /* [in] */ Editor* editor)
    : HandleView(drawableLtr, drawableRtl, editor)
{

}
Int32 SelectionEndHandleView::GetHotspotX(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Boolean isRtlRun)
{
    assert(drawable);
    Int32 width;
    drawable->GetIntrinsicWidth(&width);
    if (isRtlRun) {
        return (width * 3) / 4;
    } else {
        return width / 4;
    }
}

Int32 SelectionEndHandleView::GetCurrentCursorOffset()
{
    return mEditor->mTextView->GetSelectionEnd();
}

void SelectionEndHandleView::UpdateSelection(
    /* [in] */ Int32 offset)
{
    AutoPtr<ICharSequence> text = mEditor->mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    Selection::SetSelection(spannable, mEditor->mTextView->GetSelectionStart(), offset);
    UpdateDrawable();
}

void SelectionEndHandleView::UpdatePosition(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Int32 offset = mEditor->mTextView->GetOffsetForPosition(x, y);

    // Handles can not cross and selection is at least one character
    Int32 selectionStart = mEditor->mTextView->GetSelectionStart();
    if (offset <= selectionStart) {
        offset = Elastos::Core::Math::Min(selectionStart + 1, mEditor->mTextView->GetLength());
    }

    PositionAtCursorOffset(offset, FALSE);
}

void SelectionEndHandleView::SetActionPopupWindow(
    /* [in] */ ActionPopupWindow* actionPopupWindow)
{
    mActionPopupWindow = actionPopupWindow;
}

//==============================================================================
//              CursorController
//==============================================================================
CAR_INTERFACE_IMPL(CursorController, IOnTouchModeChangeListener)

CursorController::CursorController(
    /* [in] */ Editor* editor)
    : mEditor(editor)
{
}

ECode CursorController::Show()
{
    assert(0 && "Error: Pure base methods CursorController::Show() is called.");
    return E_NOT_IMPLEMENTED;
}

ECode CursorController::Hide()
{
    assert(0 && "Error: Pure base methods CursorController::Hide() is called.");
    return E_NOT_IMPLEMENTED;
}

ECode CursorController::OnDetached()
{
    assert(0 && "Error: Pure base methods CursorController::OnDetached() is called.");
    return E_NOT_IMPLEMENTED;
}

ECode CursorController::OnTouchModeChanged(
    /* [in]*/ Boolean isInTouchMode)
{
    assert(0 && "Error: Pure base methods CursorController::OnTouchModeChanged() is called.");
    return E_NOT_IMPLEMENTED;
}

//==============================================================================
//              InsertionPointCursorController
//==============================================================================

InsertionPointCursorController::InsertionPointCursorController(
    /* [in] */ Editor* editor)
    : CursorController(editor)
{
}

ECode InsertionPointCursorController::Show()
{
    AutoPtr<InsertionHandleView> handle = GetHandle();
    handle->Show();
    return NOERROR;
}

void InsertionPointCursorController::ShowWithActionPopup()
{
    AutoPtr<InsertionHandleView> handle = GetHandle();
    handle->ShowWithActionPopup();
}

ECode InsertionPointCursorController::Hide()
{
    if (mHandle != NULL) {
        mHandle->Hide();
    }
    return NOERROR;
}

ECode InsertionPointCursorController::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        Hide();
    }
    return NOERROR;
}

AutoPtr<InsertionHandleView> InsertionPointCursorController::GetHandle()
{
    if (mEditor->mSelectHandleCenter == NULL) {
        AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
        resources->GetDrawable(
                mEditor->mTextView->mTextSelectHandleRes,
                (IDrawable**)&mEditor->mSelectHandleCenter);
    }
    if (mHandle == NULL) {
        mHandle = new InsertionHandleView(
                mEditor->mSelectHandleCenter, mEditor);
        mHandle->Init();
    }
    return mHandle;
}

ECode InsertionPointCursorController::OnDetached()
{
    AutoPtr<IViewTreeObserver> observer = mEditor->mTextView->GetViewTreeObserver();
    observer->RemoveOnTouchModeChangeListener(this);

    if (mHandle != NULL) mHandle->OnDetached();
    return NOERROR;
}

//==============================================================================
//              SelectionModifierCursorController
//==============================================================================
const Int32 SelectionModifierCursorController::DELAY_BEFORE_REPLACE_ACTION = 200; // milliseconds

SelectionModifierCursorController::SelectionModifierCursorController(
    /* [in] */ Editor* editor)
    : CursorController(editor)
    , mMinTouchOffset(0)
    , mMaxTouchOffset(0)
    , mPreviousTapUpTime(0)
    , mDownPositionX(0.0)
    , mDownPositionY(0.0)
    , mGestureStayedInTapRegion(FALSE)
{
    ResetTouchOffsets();
}

ECode SelectionModifierCursorController::Show()
{
    if (mEditor->mTextView->IsInBatchEditMode()) {
        return NOERROR;
    }
    InitDrawables();
    InitHandles();
    mEditor->HideInsertionPointCursorController();
    return NOERROR;
}

void SelectionModifierCursorController::InitDrawables()
{
    if (mEditor->mSelectHandleLeft == NULL) {
        AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
        resources->GetDrawable(
                mEditor->mTextView->mTextSelectHandleLeftRes,
                (IDrawable**)&mEditor->mSelectHandleLeft);
    }
    if (mEditor->mSelectHandleRight == NULL) {
        AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
        resources->GetDrawable(
                mEditor->mTextView->mTextSelectHandleRightRes,
                (IDrawable**)&mEditor->mSelectHandleRight);
    }
}

void SelectionModifierCursorController::InitHandles()
{
    // Lazy object creation has to be done before updatePosition() is called.
    if (mStartHandle == NULL) {
        mStartHandle = new SelectionStartHandleView(
                mEditor->mSelectHandleLeft, mEditor->mSelectHandleRight, mEditor);
        mStartHandle->Init();
    }
    if (mEndHandle == NULL) {
        mEndHandle = new SelectionEndHandleView(
                mEditor->mSelectHandleRight, mEditor->mSelectHandleLeft, mEditor);
        mEndHandle->Init();
    }

    mStartHandle->Show();
    mEndHandle->Show();

    // Make sure both left and right handles share the same ActionPopupWindow (so that
    // moving any of the handles hides the action popup).
    mStartHandle->ShowActionPopupWindow(DELAY_BEFORE_REPLACE_ACTION);
    AutoPtr<ActionPopupWindow> popuoWindow = mStartHandle->GetActionPopupWindow();
    mEndHandle->SetActionPopupWindow(popuoWindow.Get());

    mEditor->HideInsertionPointCursorController();
}

ECode SelectionModifierCursorController::Hide()
{
    if (mStartHandle != NULL) mStartHandle->Hide();
    if (mEndHandle != NULL) mEndHandle->Hide();
    return NOERROR;
}

void SelectionModifierCursorController::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<IContext> context = mEditor->mTextView->GetContext();

    // This is done even when the View does not have focus, so that Int64 presses can start
    // selection and tap can move cursor from this tap position.
    Int32 mask;
    event->GetActionMasked(&mask);
    Float x, y;
    event->GetX(&x);
    event->GetY(&y);

    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);

    switch (mask) {
        case IMotionEvent::ACTION_DOWN:
            // Remember finger down position, to be able to start selection from there
            mMinTouchOffset = mMaxTouchOffset = mEditor->mTextView->GetOffsetForPosition(x, y);

            // Double tap detection
            if (mGestureStayedInTapRegion) {
                Int64 duration = SystemClock::GetUptimeMillis() - mPreviousTapUpTime;
                Int32 timeout;
                helper->GetDoubleTapTimeout(&timeout);
                if (duration <= timeout) {
                    Float deltaX = x - mDownPositionX;
                    Float deltaY = y - mDownPositionY;
                    Float distanceSquared = deltaX * deltaX + deltaY * deltaY;

                    AutoPtr<IViewConfiguration> viewConfiguration;
                    helper->Get(context, (IViewConfiguration**)&viewConfiguration);
                    Int32 doubleTapSlop;
                    viewConfiguration->GetScaledDoubleTapSlop(&doubleTapSlop);
                    Boolean stayedInArea = distanceSquared < doubleTapSlop * doubleTapSlop;

                    if (stayedInArea && mEditor->IsPositionOnText(x, y)) {
                        mEditor->StartSelectionActionMode();
                        mEditor->mDiscardNextActionUp = TRUE;
                    }
                }
            }

            mDownPositionX = x;
            mDownPositionY = y;
            mGestureStayedInTapRegion = TRUE;
            break;

        case IMotionEvent::ACTION_POINTER_DOWN:
        case IMotionEvent::ACTION_POINTER_UP: {
                // Handle multi-point gestures. Keep min and max offset positions.
                // Only activated for devices that correctly handle multi-touch.
                AutoPtr<IPackageManager> packageManager;
                context->GetPackageManager((IPackageManager**)&packageManager);
                Boolean hasFeature;
                packageManager->HasSystemFeature(
                        IPackageManager::FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT, &hasFeature);
                if (hasFeature) {
                    UpdateMinAndMaxOffsets(event);
                }
            }
            break;

        case IMotionEvent::ACTION_MOVE:
            if (mGestureStayedInTapRegion) {
                Float deltaX = x- mDownPositionX;
                Float deltaY = y - mDownPositionY;
                Float distanceSquared = deltaX * deltaX + deltaY * deltaY;

                AutoPtr<IViewConfiguration> viewConfiguration;
                helper->Get(context, (IViewConfiguration**)&viewConfiguration);
                Int32 doubleTapTouchSlop;
                viewConfiguration->GetScaledDoubleTapTouchSlop(&doubleTapTouchSlop);

                if (distanceSquared > doubleTapTouchSlop * doubleTapTouchSlop) {
                    mGestureStayedInTapRegion = FALSE;
                }
            }
            break;

        case IMotionEvent::ACTION_UP:
            mPreviousTapUpTime = SystemClock::GetUptimeMillis();
            break;
    }
}

/**
 * @param event
 */
void SelectionModifierCursorController::UpdateMinAndMaxOffsets(
    /* [in] */ IMotionEvent* event)
{
    Int32 pointerCount;
    event->GetPointerCount(&pointerCount);
    for (Int32 index = 0; index < pointerCount; index++) {
        Float x, y;
        event->GetX(index, &x);
        event->GetY(index, &y);
        Int32 offset = mEditor->mTextView->GetOffsetForPosition(x, y);
        if (offset < mMinTouchOffset) mMinTouchOffset = offset;
        if (offset > mMaxTouchOffset) mMaxTouchOffset = offset;
    }
}

Int32 SelectionModifierCursorController::GetMinTouchOffset()
{
    return mMinTouchOffset;
}

Int32 SelectionModifierCursorController::GetMaxTouchOffset()
{
    return mMaxTouchOffset;
}

void SelectionModifierCursorController::ResetTouchOffsets()
{
    mMinTouchOffset = mMaxTouchOffset = -1;
}

/**
 * @return TRUE iff this controller is currently used to move the selection start.
 */
Boolean SelectionModifierCursorController::IsSelectionStartDragged()
{
    return mStartHandle != NULL && mStartHandle->IsDragging();
}

ECode SelectionModifierCursorController::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        Hide();
    }
    return NOERROR;
}

ECode SelectionModifierCursorController::OnDetached()
{
    AutoPtr<IViewTreeObserver> observer = mEditor->mTextView->GetViewTreeObserver();
    observer->RemoveOnTouchModeChangeListener(this);

    if (mStartHandle != NULL) mStartHandle->OnDetached();
    if (mEndHandle != NULL) mEndHandle->OnDetached();
    return NOERROR;
}

//==============================================================================
//              PositionListener
//==============================================================================

const Int32 PositionListener::MAXIMUM_NUMBER_OF_LISTENERS;

CAR_INTERFACE_IMPL(PositionListener, IOnPreDrawListener);

PositionListener::PositionListener(
    /* [in] */ Editor* editor)
    : mPositionHasChanged(TRUE)
    , mPositionX(0)
    , mPositionY(0)
    , mNumberOfListeners(0)
    , mScrollHasChanged(0)
    , mEditor(editor)
{
    mPositionListeners = ArrayOf<TextViewPositionListener*>::Alloc(MAXIMUM_NUMBER_OF_LISTENERS);
    mCanMove = ArrayOf<Boolean>::Alloc(MAXIMUM_NUMBER_OF_LISTENERS);
    mTempCoords = ArrayOf<Int32>::Alloc(2);
}

void PositionListener::AddSubscriber(
    /* [in] */ TextViewPositionListener* positionListener,
    /* [in] */ Boolean canMove)
{
    if (mNumberOfListeners == 0) {
        UpdatePosition();
        AutoPtr<IViewTreeObserver> observer = mEditor->mTextView->GetViewTreeObserver();
        observer->AddOnPreDrawListener(this);
    }

    Int32 emptySlotIndex = -1;
    for (Int32 i = 0; i < MAXIMUM_NUMBER_OF_LISTENERS; i++) {
        AutoPtr<TextViewPositionListener> listener = (*mPositionListeners)[i];
        if (listener.Get() == positionListener) {
            return;
        } else if (emptySlotIndex < 0 && listener == NULL) {
            emptySlotIndex = i;
        }
    }

    mPositionListeners->Set(emptySlotIndex, positionListener);
    (*mCanMove)[emptySlotIndex] = canMove;
    mNumberOfListeners++;
}

void PositionListener::RemoveSubscriber(
    /* [in] */ TextViewPositionListener* positionListener)
{
    for (Int32 i = 0; i < MAXIMUM_NUMBER_OF_LISTENERS; i++) {
        if ((*mPositionListeners)[i] == positionListener) {
            mPositionListeners->Set(i, NULL);
            mNumberOfListeners--;
            break;
        }
    }

    if (mNumberOfListeners == 0) {
        AutoPtr<IViewTreeObserver> observer = mEditor->mTextView->GetViewTreeObserver();
        observer->RemoveOnPreDrawListener(this);
    }
}

Int32 PositionListener::GetPositionX()
{
    return mPositionX;
}

Int32 PositionListener::GetPositionY()
{
    return mPositionY;
}

ECode PositionListener::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    UpdatePosition();

    for (Int32 i = 0; i < MAXIMUM_NUMBER_OF_LISTENERS; i++) {
        if (mPositionHasChanged || mScrollHasChanged || (*mCanMove)[i]) {
            AutoPtr<TextViewPositionListener> positionListener = (*mPositionListeners)[i];
            if (positionListener != NULL) {
                positionListener->UpdatePosition(mPositionX, mPositionY,
                        mPositionHasChanged, mScrollHasChanged);
            }
        }
    }

    mScrollHasChanged = FALSE;
    *result = TRUE;
    return NOERROR;
}

void PositionListener::UpdatePosition()
{
    Int32 x, y;
    mEditor->mTextView->GetLocationInWindow(&x, &y);
    (*mTempCoords)[0] = x;
    (*mTempCoords)[0] = y;

    mPositionHasChanged = x != mPositionX || y != mPositionY;

    mPositionX = x;
    mPositionY = y;
}

void PositionListener::OnScrollChanged()
{
    mScrollHasChanged = TRUE;
}

//==============================================================================
//              CorrectionHighlighter
//==============================================================================

const Int32 CorrectionHighlighter::FADE_OUT_DURATION;

CorrectionHighlighter::CorrectionHighlighter(
    /* [in] */ Editor* editor)
    : mStart(0)
    , mEnd(0)
    , mFadingStartTime(0)
    , mEditor(editor)
{
    CPath::New((IPath**)&mPath);
    CPaint::New((IPaint**)&mPaint);

    AutoPtr<IResources> resources = mEditor->mTextView->GetResources();
    AutoPtr<ICompatibilityInfo> info;
    resources->GetCompatibilityInfo((ICompatibilityInfo**)&info);
    Float scale;
    info->GetApplicationScale(&scale);
    mPaint->SetCompatibilityScaling(scale);
    mPaint->SetStyle(Elastos::Droid::Graphics::PaintStyle_FILL);
}

void CorrectionHighlighter::Highlight(
    /* [in] */ ICorrectionInfo* info)
{
    assert(info);
    info->GetOffset(&mStart);
    AutoPtr<ICharSequence> next;
    info->GetNewText((ICharSequence**)&next);
    Int32 length;
    next->GetLength(&length);
    mEnd = mStart + length;
    mFadingStartTime = SystemClock::GetUptimeMillis();

    if (mStart < 0 || mEnd < 0) {
        StopAnimation();
    }
}

void CorrectionHighlighter::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 cursorOffsetVertical)
{
    assert(canvas);
    if (UpdatePath() && UpdatePaint()) {
        if (cursorOffsetVertical != 0) {
            canvas->Translate(0, cursorOffsetVertical);
        }

        canvas->DrawPath(mPath, mPaint);

        if (cursorOffsetVertical != 0) {
            canvas->Translate(0, -cursorOffsetVertical);
        }
        Invalidate(TRUE); // TODO invalidate cursor region only
    } else {
        StopAnimation();
        Invalidate(FALSE); // TODO invalidate cursor region only
    }
}

Boolean CorrectionHighlighter::UpdatePaint()
{
    Int64 duration = SystemClock::GetUptimeMillis() - mFadingStartTime;
    if (duration > FADE_OUT_DURATION) return FALSE;

    AutoPtr<IColor> helper;
    CColor::AcquireSingleton((IColor**)&helper);
    Int32 highlightColorAlpha;
    helper->Alpha(mEditor->mTextView->mHighlightColor, &highlightColorAlpha);

    Float coef = 1.0f - (Float) duration / FADE_OUT_DURATION;
    Int32 color = (mEditor->mTextView->mHighlightColor & 0x00FFFFFF) +
            ((Int32) (highlightColorAlpha * coef) << 24);
    mPaint->SetColor(color);
    return TRUE;
}

Boolean CorrectionHighlighter::UpdatePath()
{
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    if (layout == NULL) return FALSE;

    // Update in case text is edited while the animation is run
    Int32 length = mEditor->mTextView->GetLength();
    Int32 start = Elastos::Core::Math::Min(length, mStart);
    Int32 end = Elastos::Core::Math::Min(length, mEnd);

    mPath->Reset();
    layout->GetSelectionPath(start, end, mPath);
    return TRUE;
}

void CorrectionHighlighter::Invalidate(
    /* [in] */ Boolean delayed)
{
    AutoPtr<ILayout> layout = mEditor->mTextView->GetLayout();
    if (layout == NULL) return;

    if (mTempRectF == NULL) {
        CRectF::New((IRectF**)&mTempRectF);
    }
    mPath->ComputeBounds(mTempRectF, FALSE);

    Int32 left = mEditor->mTextView->GetCompoundPaddingLeft();
    Int32 top = mEditor->mTextView->GetExtendedPaddingTop()
            + mEditor->mTextView->GetVerticalOffset(TRUE);

    Float l, t, r, b;
    mTempRectF->Get(&l, &t, &r, &b);
    if (delayed) {
        mEditor->mTextView->PostInvalidateOnAnimation(
                left + (Int32) l, top + (Int32) t,
                left + (Int32) r, top + (Int32) b);
    } else {
        mEditor->mTextView->PostInvalidate(
                (Int32) l, (Int32) t, (Int32) r, (Int32) b);
    }
}

void CorrectionHighlighter::StopAnimation()
{
    mEditor->mCorrectionHighlighter = NULL;
}

//==============================================================================
//              ErrorPopup
//==============================================================================
_ErrorPopup::_ErrorPopup(
    /* [in]*/ TextView* textView,
    /* [in]*/ Int32 width,
    /* [in]*/ Int32 height)
    : PopupWindow((IView*)(textView->Probe(EIID_IView)), width, height)
    , mAbove(0)
    , mView(textView)
    , mPopupInlineErrorBackgroundId(0)
    , mPopupInlineErrorAboveBackgroundId(0)
{
    // Make sure the TextView has a background set as it will be used the first time it is
    // shown and positionned. Initialized with below background, which should have
    // dimensions identical to the above version for this to work (and is more likely).
    mPopupInlineErrorBackgroundId = GetResourceId(mPopupInlineErrorBackgroundId,
            R::styleable::Theme_errorMessageBackground);
    mView->SetBackgroundResource(mPopupInlineErrorBackgroundId);
}

void _ErrorPopup::FixDirection(
    /* [in]*/ Boolean above)
{
    mAbove = above;

    if (above) {
        mPopupInlineErrorAboveBackgroundId =
            GetResourceId(mPopupInlineErrorAboveBackgroundId,
                    R::styleable::Theme_errorMessageAboveBackground);
    } else {
        mPopupInlineErrorBackgroundId = GetResourceId(mPopupInlineErrorBackgroundId,
                R::styleable::Theme_errorMessageBackground);
    }

    mView->SetBackgroundResource(above ? mPopupInlineErrorAboveBackgroundId :
        mPopupInlineErrorBackgroundId);
}

Int32 _ErrorPopup::GetResourceId(
    /* [in]*/ Int32 currentId,
    /* [in]*/ Int32 index)
{
    if (currentId == 0) {
        AutoPtr<IContext> context = mView->GetContext();
        AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
                const_cast<Int32 *>(R::styleable::Theme),
                ARRAY_SIZE(R::styleable::Theme));
        AutoPtr<ITypedArray> styledAttributes;
        context->ObtainStyledAttributes(attrIds, (ITypedArray**)&styledAttributes);

        styledAttributes->GetResourceId(index, 0, &currentId);
        styledAttributes->Recycle();
    }
    return currentId;
}

ECode _ErrorPopup::Update(
    /* [in]*/ Int32 x,
    /* [in]*/ Int32 y,
    /* [in]*/ Int32 w,
    /* [in]*/ Int32 h,
    /* [in]*/ Boolean force)
{
    PopupWindow::Update(x, y, w, h, force);

    Boolean above = IsAboveAnchor();
    if (above != mAbove) {
        FixDirection(above);
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(ErrorPopup, IPopupWindow)
IPOPUPWINDOW_METHODS_IMPL(ErrorPopup, _ErrorPopup)

ErrorPopup::ErrorPopup(
    /* [in]*/ TextView* textView,
    /* [in]*/ Int32 width,
    /* [in]*/ Int32 height)
    : _ErrorPopup(textView, width, height)
{
}

//==============================================================================
//              Blink
//==============================================================================

Blink::Blink(
    /* [in]*/ Editor* editor)
    : mCancelled(FALSE)
    , mEditor(editor)
{
}

Blink::~Blink()
{
}

ECode Blink::Run()
{
    if (mCancelled) {
       return NOERROR;
    }

    RemoveCallbacks(this);

    if (mEditor->ShouldBlink()) {
        if (mEditor->mTextView->GetLayout() != NULL) {
            mEditor->mTextView->InvalidateCursorPath();
        }

        Boolean result = FALSE;
        PostAtTime(this, SystemClock::GetUptimeMillis() + Editor::BLINK, &result);
    }

    return NOERROR;
}

void Blink::Cancel()
{
    if (!mCancelled) {
        RemoveCallbacks(this);
        mCancelled = TRUE;
    }
}

void Blink::Uncancel() {
    mCancelled = FALSE;
}

//==============================================================================
//              EasyEditSpanController
//==============================================================================

CAR_INTERFACE_IMPL(EasyEditSpanController, ISpanWatcher);

EasyEditSpanController::EasyEditSpanController(
    /* [in] */ Editor* editor)
    : mEditor(editor)
{
}

ECode EasyEditSpanController::OnSpanAdded(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* span,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    IEasyEditSpan* easyEditSpan = IEasyEditSpan::Probe(span);
    if (easyEditSpan) {
        if (mPopupWindow == NULL) {
            mPopupWindow = new EasyEditPopupWindow(mEditor);
            mPopupWindow->Init();

            mHidePopup = new HidePopupRunnable(this);
        }

        // Make sure there is only at most one EasyEditSpan in the text
        if (mPopupWindow->mEasyEditSpan != NULL) {
            text->RemoveSpan(mPopupWindow->mEasyEditSpan);
        }

        mPopupWindow->SetEasyEditSpan(easyEditSpan);

        if (mEditor->mTextView->GetWindowVisibility() != IView::VISIBLE) {
            // The window is not visible yet, ignore the text change.
            return NOERROR;
        }

        if (mEditor->mTextView->GetLayout() == NULL) {
            // The view has not been laid out yet, ignore the text change
            return NOERROR;
        }

        if (mEditor->ExtractedTextModeWillBeStarted()) {
            // The input is in extract mode. Do not handle the easy edit in
            // the original TextView, as the ExtractEditText will do
            return NOERROR;
        }

        mPopupWindow->Show();

        mEditor->mTextView->RemoveCallbacks(mHidePopup);
        mEditor->mTextView->PostDelayed(mHidePopup, DISPLAY_TIMEOUT_MS);
    }
    return NOERROR;
}

ECode EasyEditSpanController::OnSpanRemoved(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* span,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (mPopupWindow != NULL && span == mPopupWindow->mEasyEditSpan) {
        Hide();
    }
    return NOERROR;
}

ECode EasyEditSpanController::OnSpanChanged(
    /* [in] */ ISpannable* text,
    /* [in] */ IInterface* span,
    /* [in] */ Int32 ostart,
    /* [in] */ Int32 oend,
    /* [in] */ Int32 nstart,
    /* [in] */ Int32 nend)
{
    if (mPopupWindow != NULL && span == mPopupWindow->mEasyEditSpan) {
        text->RemoveSpan(mPopupWindow->mEasyEditSpan);
    }
    return NOERROR;
}

void EasyEditSpanController::Hide()
{
    if (mPopupWindow != NULL) {
        mPopupWindow->Hide();
        mEditor->mTextView->RemoveCallbacks(mHidePopup);
    }
}

//==============================================================================
//              DragLocalState
//==============================================================================
CAR_INTERFACE_IMPL(DragLocalState, IInterface);

DragLocalState::DragLocalState(
    /* [in] */ TextView* sourceTextView,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
    : mSourceTextView(sourceTextView)
    , mStart(mStart)
    , mEnd(end)
{
}

//==============================================================================
//              UserDictionaryListener
//==============================================================================

UserDictionaryListener::UserDictionaryListener()
{}

void UserDictionaryListener::WaitForUserDictionaryAdded(
    /* [in] */ ITextView* tv,
    /* [in] */ const String& originalWord,
    /* [in] */ Int32 spanStart,
    /* [in] */ Int32 spanEnd)
{
    mTextView = reinterpret_cast<TextView*>(tv->Probe(EIID_TextView));
    mOriginalWord = originalWord;
    mWordStart = spanStart;
    mWordEnd = spanEnd;
}

ECode UserDictionaryListener::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch(what)
    {
        case 0 : /* CODE_WORD_ADDED */
        case 2 : /* CODE_ALREADY_PRESENT */
        {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            AutoPtr<IBundle> bundle = IBundle::Probe(obj);
            if (!bundle)
            {
                // Log.w(TAG, "Illegal message. Abort handling onUserDictionaryAdded.");
                return NOERROR;
            }
            String originalWord, addedWord;
            bundle->GetString(String("originalWord"), &originalWord);
            bundle->GetString(String("word"), &addedWord);
            OnUserDictionaryAdded(originalWord, addedWord);
            return NOERROR;
        }
        default:
            return NOERROR;
    }
}

void UserDictionaryListener::OnUserDictionaryAdded(
    /* [in] */ const String& originalWord,
    /* [in] */ const String& addedWord)
{
    if(TextUtils::IsEmpty(mOriginalWord) || TextUtils::IsEmpty(addedWord))
    {
        return;
    }

    Int32 length;
    length = mTextView->GetLength();
    if(mWordStart < 0 || mWordEnd >= length)
    {
        return;
    }
    if(!mOriginalWord.Equals(originalWord))
    {
        return;
    }
    if(originalWord.Equals(addedWord))
    {
        return;
    }
    AutoPtr<ICharSequence> cs = mTextView->GetText();
    String str;
    cs->ToString(&str);
    String currentWord = str.Substring(mWordStart, mWordEnd);
    if(!currentWord.Equals(originalWord)) {
        return ;
    }
    AutoPtr<ICharSequence> cAddedWord;
    CStringWrapper::New(addedWord, (ICharSequence**)&cAddedWord);
    mTextView->ReplaceText_internal(mWordStart, mWordEnd, cAddedWord);
    Int32 newCursorPosition = mWordStart + addedWord.GetLength();
    mTextView->SetCursorPosition_internal(newCursorPosition, newCursorPosition);
}

//==============================================================================
//              Editor
//==============================================================================

const String Editor::TAG("Editor");
const Int32 Editor::BLINK;

AutoPtr<ArrayOf<Float> > Editor::TEMP_POSITION = ArrayOf<Float>::Alloc(2);
const Int32 Editor::DRAG_SHADOW_MAX_TEXT_LENGTH;
const Int32 Editor::EXTRACT_NOTHING;
const Int32 Editor::EXTRACT_UNKNOWN;

Editor::Editor(
    /* [in] */ TextView* textView)
    : mInsertionControllerEnabled(FALSE)
    , mSelectionControllerEnabled(FALSE)
    , mLastLayoutHeight(0)
    , mFrozenWithFocus(FALSE)
    , mSelectionMoved(FALSE)
    , mTouchFocusSelected(FALSE)
    , mInputType(IInputType::TYPE_NULL)
    , mDiscardNextActionUp(FALSE)
    , mIgnoreActionUpEvent(FALSE)
    , mShowCursor(0)
    , mCursorVisible(TRUE)
    , mSelectAllOnFocus(FALSE)
    , mTextIsSelectable(FALSE)
    , mErrorWasChanged(FALSE)
    , mErrorPopup(NULL)
    , mShowErrorAfterAttach(FALSE)
    , mInBatchEditControllers(FALSE)
    , mShowSoftInputOnFocus(TRUE)
    , mPreserveDetachedSelection(FALSE)
    , mTemporaryDetach(FALSE)
    , mCursorCount(0)
    , mLastDownPositionX(0.0)
    , mLastDownPositionY(0.0)
    , mCreatedWithASelection(FALSE)
{
    mTextView = textView;
    mCursorDrawable = ArrayOf<IDrawable *>::Alloc(2);
    mUserDictionaryListener = new UserDictionaryListener();
}

Editor::~Editor()
{
}

ECode Editor::OnAttachedToWindow()
{
    if (mShowErrorAfterAttach) {
        ShowError();
        mShowErrorAfterAttach = FALSE;
    }
    mTemporaryDetach = FALSE;

    AutoPtr<IViewTreeObserver> observer = mTextView->GetViewTreeObserver();
    // No need to create the controller.
    // The get method will add the listener on controller creation.
    if (mInsertionPointCursorController != NULL) {
        observer->AddOnTouchModeChangeListener(mInsertionPointCursorController);
    }
    if (mSelectionModifierCursorController != NULL) {
        mSelectionModifierCursorController->ResetTouchOffsets();
        observer->AddOnTouchModeChangeListener(mSelectionModifierCursorController);
    }


    UpdateSpellCheckSpans(0, mTextView->GetLength(),
            TRUE /* create the spell checker if needed */);

    if (mTextView->HasTransientState() &&
            mTextView->GetSelectionStart() != mTextView->GetSelectionEnd()) {
        // Since transient state is reference counted make sure it stays matched
        // with our own calls to it for managing selection.
        // The action mode callback will set this back again when/if the action mode starts.
        mTextView->SetHasTransientState(FALSE);

        // We had an active selection from before, start the selection mode.
        StartSelectionActionMode();
    }
    return NOERROR;
}

ECode Editor::OnDetachedFromWindow()
{
    if (mError != NULL) {
        HideError();
    }

    if (mBlink != NULL) {
        mBlink->RemoveCallbacks(mBlink);
    }

    if (mInsertionPointCursorController != NULL) {
        mInsertionPointCursorController->OnDetached();
    }

    if (mSelectionModifierCursorController != NULL) {
        mSelectionModifierCursorController->OnDetached();
    }

    if (mShowSuggestionRunnable != NULL) {
        mTextView->RemoveCallbacks(mShowSuggestionRunnable);
    }

    InvalidateTextDisplayList();

    if (mSpellChecker != NULL) {
        mSpellChecker->CloseSession();
        // Forces the creation of a new SpellChecker next time this window is created.
        // Will handle the cases where the settings has been changed in the meantime.
        mSpellChecker = NULL;
    }

    mPreserveDetachedSelection = TRUE;
    HideControllers();
    mPreserveDetachedSelection = FALSE;
    mTemporaryDetach = FALSE;
    return NOERROR;
}

void Editor::ShowError()
{
    if (mTextView->GetWindowToken() == NULL) {
        mShowErrorAfterAttach = TRUE;
        return;
    }

    if (mErrorPopup == NULL) {
        AutoPtr<IContext> context = mTextView->GetContext();
        AutoPtr<ILayoutInflater> inflater;
        LayoutInflater::From(context, (ILayoutInflater**)&inflater);

        AutoPtr<ITextView> err;
        inflater->Inflate(R::layout::textview_hint, NULL, (IView**)&err);

        AutoPtr<IResources> resources = mTextView->GetResources();
        AutoPtr<IDisplayMetrics> metrics;
        resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        Float scale;
        metrics->GetDensity(&scale);

        mErrorPopup = new ErrorPopup((TextView*)(err->Probe(EIID_TextView)),
                (Int32)(200 * scale + 0.5f), (Int32)(50 * scale + 0.5f));
        mErrorPopup->SetFocusable(FALSE);
        // The user is entering text, so the input method is needed.  We
        // don't want the popup to be displayed on top of it.
        mErrorPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NEEDED);
    }

    AutoPtr<IView> cv;
    mErrorPopup->GetContentView((IView**)&cv);
    TextView* tv = reinterpret_cast<TextView*>(cv->Probe(EIID_TextView));
    ChooseSize(mErrorPopup, mError, tv);
    tv->SetText(mError);

    mErrorPopup->ShowAsDropDown((IView*)(mTextView->Probe(EIID_IView)),
            GetErrorX(), GetErrorY());
    Boolean isAboveAnchor;
    mErrorPopup->IsAboveAnchor(&isAboveAnchor);
    mErrorPopup->FixDirection(isAboveAnchor);
}

ECode Editor::SetError(
    /* [in] */ ICharSequence* error,
    /* [in] */ IDrawable* icon)
{
    mError = TextUtils::StringOrSpannedString(error);
    mErrorWasChanged = TRUE;

    if (mError == NULL) {
        SetErrorIcon(NULL);
        if (mErrorPopup != NULL) {
            Boolean bval;
            mErrorPopup->IsShowing(&bval);
            if (bval) {
                mErrorPopup->Dismiss();
            }

            mErrorPopup = NULL;
        }

        SetErrorIcon(NULL);
    }
    else {
        SetErrorIcon(icon);
        ShowError();
    }
    return NOERROR;
}

void Editor::SetErrorIcon(
    /* [in] */ IDrawable* icon)
{
    AutoPtr<Drawables> dr = mTextView->mDrawables;
    if (dr == NULL) {
        mTextView->mDrawables = dr = new Drawables();
    }
    ITextView* tv = (ITextView*)(mTextView->Probe(EIID_ITextView));

    dr->SetErrorDrawable(icon, tv);
    mTextView->ResetResolvedDrawables();
    mTextView->Invalidate();
    mTextView->RequestLayout();
}

void Editor::HideError()
{
    if (mErrorPopup != NULL) {
        Boolean bval;
        mErrorPopup->IsShowing(&bval);
        if (bval) {
            mErrorPopup->Dismiss();
        }

        SetErrorIcon(NULL);
    }

    mShowErrorAfterAttach = FALSE;
}

Int32 Editor::GetErrorX()
{
    /*
     * The "25" is the distance between the point and the right edge
     * of the background
     */
    AutoPtr<IResources> resources = mTextView->GetResources();
    AutoPtr<IDisplayMetrics> metrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float scale;
    metrics->GetDensity(&scale);

    AutoPtr<Drawables> dr = mTextView->mDrawables;
    Int32 layoutDirection = mTextView->GetLayoutDirection();
    Int32 errorX;
    Int32 offset;
    switch (layoutDirection) {
        default:
        case IView::LAYOUT_DIRECTION_LTR: {
            Int32 w;
            mErrorPopup->GetWidth(&w);
            offset = - (dr != NULL ? dr->mDrawableSizeRight : 0) / 2 + (Int32) (25 * scale + 0.5f);
            errorX = mTextView->GetWidth() - w - mTextView->GetPaddingRight() + offset;
            break;
        }
        case IView::LAYOUT_DIRECTION_RTL:
            offset = (dr != NULL ? dr->mDrawableSizeLeft : 0) / 2 - (Int32) (25 * scale + 0.5f);
            errorX = mTextView->GetPaddingLeft() + offset;
            break;
    }

    return errorX;
}

Int32 Editor::GetErrorY()
{
    /*
     * Compound, not extended, because the icon is not clipped
     * if the text height is smaller.
     */
    Int32 compoundPaddingTop = mTextView->GetCompoundPaddingTop();
    Int32 vspace = mTextView->GetBottom() - mTextView->GetTop() -
            mTextView->GetCompoundPaddingBottom() - compoundPaddingTop;

    AutoPtr<Drawables> dr = mTextView->mDrawables;
    Int32 layoutDirection = mTextView->GetLayoutDirection();
    Int32 height;
    switch (layoutDirection) {
        default:
        case IView::LAYOUT_DIRECTION_LTR:
            height = (dr != NULL ? dr->mDrawableHeightRight : 0);
            break;
        case IView::LAYOUT_DIRECTION_RTL:
            height = (dr != NULL ? dr->mDrawableHeightLeft : 0);
            break;
    }

    Int32 icontop = compoundPaddingTop + (vspace - height) / 2;

    /*
     * The "2" is the distance between the point and the top edge
     * of the background.
     */
    AutoPtr<IResources> resources = mTextView->GetResources();
    AutoPtr<IDisplayMetrics> metrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float scale;
    metrics->GetDensity(&scale);

    return icontop + height - mTextView->GetHeight() - (Int32) (2 * scale + 0.5f);
}

void Editor::CreateInputContentTypeIfNeeded()
{
    if (mInputContentType == NULL) {
        mInputContentType = new InputContentType();
    }
}

void Editor::CreateInputMethodStateIfNeeded()
{
    if (mInputMethodState == NULL) {
        mInputMethodState = new InputMethodState();
    }
}

Boolean Editor::IsCursorVisible()
{
    // The default value is TRUE, even when there is no associated Editor
    return mCursorVisible && mTextView->IsTextEditable();
}

void Editor::PrepareCursorControllers()
{
    Boolean windowSupportsHandles = FALSE;

    AutoPtr<IView> trv = mTextView->GetRootView();
    AutoPtr<IViewGroupLayoutParams> params;
    trv->GetLayoutParams((IViewGroupLayoutParams**)&params);
    IWindowManagerLayoutParams* windowParams = IWindowManagerLayoutParams::Probe(params.Get());
    if (windowParams) {
        Int32 type;
        windowParams->GetType(&type);
        windowSupportsHandles = type < IWindowManagerLayoutParams::FIRST_SUB_WINDOW
                || type > IWindowManagerLayoutParams::LAST_SUB_WINDOW;
    }

    Boolean enabled = windowSupportsHandles && mTextView->GetLayout() != NULL;
    mInsertionControllerEnabled = enabled && IsCursorVisible();
    mSelectionControllerEnabled = enabled && mTextView->TextCanBeSelected();

    if (!mInsertionControllerEnabled) {
        HideInsertionPointCursorController();
        if (mInsertionPointCursorController != NULL) {
            mInsertionPointCursorController->OnDetached();
            mInsertionPointCursorController = NULL;
        }
    }

    if (!mSelectionControllerEnabled) {
        StopSelectionActionMode();
        if (mSelectionModifierCursorController != NULL) {
            mSelectionModifierCursorController->OnDetached();
            mSelectionModifierCursorController = NULL;
        }
    }
}

void Editor::HideInsertionPointCursorController()
{
    if (mInsertionPointCursorController != NULL) {
        mInsertionPointCursorController->Hide();
    }
}

void Editor::HideControllers()
{
    HideCursorControllers();
    HideSpanControllers();
}

void Editor::HideSpanControllers()
{
    if (mEasyEditSpanController != NULL) {
        mEasyEditSpanController->Hide();
    }
}

void Editor::HideCursorControllers()
{
    if (mSuggestionsPopupWindow != NULL && !mSuggestionsPopupWindow->IsShowingUp()) {
        // Should be done before hide insertion point controller since it triggers a show of it
        mSuggestionsPopupWindow->Hide();
    }
    HideInsertionPointCursorController();
    StopSelectionActionMode();
}

void Editor::UpdateSpellCheckSpans(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean createSpellChecker)
{
//TODO
//    if (mTextView->IsTextEditable() && mTextView->IsSuggestionsEnabled() &&
//            !(IExtractEditText::Probe(mTextView))) {
//        if (mSpellChecker == NULL && createSpellChecker) {
//            assert(0);
//            CSpellChecker::New(mTextView, (ISpellChecker**)&mSpellChecker);
//        }
//        if (mSpellChecker != NULL) {
//            mSpellChecker->SpellCheck(start, end);
//        }
//    }
}

void Editor::OnScreenStateChanged(
    /* [in] */ Int32 screenState)
{
    switch (screenState) {
        case IView::SCREEN_STATE_ON:
            ResumeBlink();
            break;
        case IView::SCREEN_STATE_OFF:
            SuspendBlink();
            break;
    }
}

void Editor::SuspendBlink()
{
    if (mBlink != NULL) {
        mBlink->Cancel();
    }
}

void Editor::ResumeBlink()
{
    if (mBlink != NULL) {
        mBlink->Uncancel();
        MakeBlink();
    }
}

void Editor::AdjustInputType(
    /* [in] */ Boolean password,
    /* [in] */ Boolean passwordInputType,
    /* [in] */ Boolean webPasswordInputType,
    /* [in] */ Boolean numberPasswordInputType)
{
    // mInputType has been set from inputType, possibly modified by mInputMethod.
    // Specialize mInputType to [web]password if we have a text class and the original input
    // type was a password.
    if ((mInputType & IInputType::TYPE_MASK_CLASS) == IInputType::TYPE_CLASS_TEXT) {
        if (password || passwordInputType) {
            mInputType = (mInputType & ~(IInputType::TYPE_MASK_VARIATION))
                    | IInputType::TYPE_TEXT_VARIATION_PASSWORD;
        }
        if (webPasswordInputType) {
            mInputType = (mInputType & ~(IInputType::TYPE_MASK_VARIATION))
                    | IInputType::TYPE_TEXT_VARIATION_WEB_PASSWORD;
        }
    } else if ((mInputType & IInputType::TYPE_MASK_CLASS) == IInputType::TYPE_CLASS_NUMBER) {
        if (numberPasswordInputType) {
            mInputType = (mInputType & ~(IInputType::TYPE_MASK_VARIATION))
                    | IInputType::TYPE_NUMBER_VARIATION_PASSWORD;
        }
    }
}

void Editor::ChooseSize(
    /* [in] */ PopupWindow* pop,
    /* [in] */ ICharSequence* text,
    /* [in] */ TextView* tv)
{
    Int32 wid = tv->GetPaddingLeft() + tv->GetPaddingRight();
    Int32 ht = tv->GetPaddingTop() + tv->GetPaddingBottom();

    AutoPtr<IResources> resources = mTextView->GetResources();
    Int32 defaultWidthInPixels;
    resources->GetDimensionPixelSize(
        R::dimen::textview_error_popup_default_width, &defaultWidthInPixels);
    AutoPtr<ILayout> layout;
    CStaticLayout::New(text, tv->GetPaint(), defaultWidthInPixels,
        Elastos::Droid::Text::ALIGN_NORMAL, 1, 0, TRUE, (IStaticLayout**)&layout);
    Float max = 0;
    Float lineWidth;
    Int32 lineCount;
    layout->GetLineCount(&lineCount);
    for (Int32 i = 0; i < lineCount; i++) {
        layout->GetLineWidth(i, &lineWidth);
        max = Elastos::Core::Math::Max(max, lineWidth);
    }

    /*
     * Now set the popup size to be big enough for the text plus the border capped
     * to DEFAULT_MAX_POPUP_WIDTH
     */
    Int32 height;
    layout->GetHeight(&height);
    pop->SetWidth(wid + (Int32) Elastos::Core::Math::Ceil(max));
    pop->SetHeight(ht + height);
}

void Editor::SetFrame()
{
    if (mErrorPopup != NULL) {
        AutoPtr<IView> cv;
        mErrorPopup->GetContentView((IView**)&cv);
        TextView* tv = reinterpret_cast<TextView*>(cv->Probe(EIID_TextView));
        assert(tv != NULL);
        ChooseSize(mErrorPopup, mError, tv);
        Int32 w, h;
        mErrorPopup->GetWidth(&w);
        mErrorPopup->GetHeight(&h);
        mErrorPopup->Update((IView*)(mTextView->Probe(EIID_IView)),
            GetErrorX(), GetErrorY(), w, h);
    }
}

Boolean Editor::CanSelectText()
{
    return HasSelectionController() && mTextView->GetLength() != 0;
}

Boolean Editor::HasPasswordTransformationMethod()
{
    AutoPtr<ITransformationMethod> method = mTextView->GetTransformationMethod();
    return IPasswordTransformationMethod::Probe(method) != NULL;
}

Boolean Editor::SelectCurrentWord()
{
    if (!CanSelectText()) {
        return FALSE;
    }

    if (HasPasswordTransformationMethod()) {
        // Always select all on a password field.
        // Cut/copy menu entries are not available for passwords, but being able to select all
        // is however useful to delete or paste to replace the entire content.
        return mTextView->SelectAllText();
    }

    Int32 inputType = mTextView->GetInputType();
    Int32 klass = inputType & IInputType::TYPE_MASK_CLASS;
    Int32 variation = inputType & IInputType::TYPE_MASK_VARIATION;

    // Specific text field types: select the entire text for these
    if (klass == IInputType::TYPE_CLASS_NUMBER ||
            klass == IInputType::TYPE_CLASS_PHONE ||
            klass == IInputType::TYPE_CLASS_DATETIME ||
            variation == IInputType::TYPE_TEXT_VARIATION_URI ||
            variation == IInputType::TYPE_TEXT_VARIATION_EMAIL_ADDRESS ||
            variation == IInputType::TYPE_TEXT_VARIATION_WEB_EMAIL_ADDRESS ||
            variation == IInputType::TYPE_TEXT_VARIATION_FILTER) {
        return mTextView->SelectAllText();
    }

    Int64 lastTouchOffsets = GetLastTouchOffsets();
    Int32 minOffset = TextUtils::UnpackRangeStartFromInt64(lastTouchOffsets);
    Int32 maxOffset = TextUtils::UnpackRangeEndFromInt64(lastTouchOffsets);

    // Safety check in case standard touch event handling has been bypassed
    if (minOffset < 0 || minOffset >= mTextView->GetLength()) return FALSE;
    if (maxOffset < 0 || maxOffset >= mTextView->GetLength()) return FALSE;

    Int32 selectionStart, selectionEnd;

    // If a URLSpan (web address, email, phone...) is found at that position, select it.
    AutoPtr<ArrayOf<IURLSpan*> > urlSpans;
    AutoPtr<ICharSequence> text = mTextView->GetText();
    ISpanned* spanned = ISpanned::Probe(text);
    if (spanned) {
        AutoPtr<ArrayOf<IInterface*> > temp;
        spanned->GetSpans(minOffset, maxOffset, EIID_IURLSpan,
                (ArrayOf<IInterface*>**)&temp);
        urlSpans = ArrayOf<IURLSpan*>::Alloc(temp->GetLength());
        for(Int32 i = 0; i < temp->GetLength(); i++) {
            urlSpans->Set(i, IURLSpan::Probe((*temp)[i]));
        }
    }

    if (urlSpans != NULL && urlSpans->GetLength() >= 1) {
        AutoPtr<IURLSpan> urlSpan = (*urlSpans)[0];
        spanned->GetSpanStart(urlSpan, &selectionStart);
        spanned->GetSpanEnd(urlSpan, &selectionEnd);
    } else {
        AutoPtr<IWordIterator> wordIterator = GetWordIterator();
        wordIterator->SetCharSequence(text, minOffset, maxOffset);

        wordIterator->GetBeginning(minOffset, &selectionStart);
        wordIterator->GetEnd(maxOffset, &selectionEnd);

        if (selectionStart == -1 /*IBreakIterator::DONE*/
               || selectionEnd == -1 /*IBreakIterator::DONE*/
               || selectionStart == selectionEnd) {
           // Possible when the word iterator does not properly handle the text's language
           Int64 range = GetCharRange(minOffset);
           selectionStart = TextUtils::UnpackRangeStartFromInt64(range);
           selectionEnd = TextUtils::UnpackRangeEndFromInt64(range);
        }
    }

    AutoPtr<ISpannable> spannable = ISpannable::Probe(text);
    Selection::SetSelection(spannable, selectionStart, selectionEnd);
    return selectionEnd > selectionStart;
}

void Editor::OnLocaleChanged()
{
    // Will be re-created on demand in getWordIterator with the proper new locale
    mWordIterator = NULL;
}

AutoPtr<IWordIterator> Editor::GetWordIterator()
{
    if (mWordIterator == NULL) {
        AutoPtr<ILocale> locale = mTextView->GetTextServicesLocale();
        CWordIterator::New(locale, (IWordIterator**)&mWordIterator);
    }
    return mWordIterator;
}

Int64 Editor::GetCharRange(
    /* [in] */ Int32 offset)
{
    AutoPtr<ICharSequence> text = mTextView->GetText();
    Int32 textLength = mTextView->GetLength();
    if (offset + 1 < textLength) {
        Char32 currentChar, nextChar;
        text->GetCharAt(offset, &currentChar);
        text->GetCharAt(offset + 1, &nextChar);
        if (Character::IsSurrogatePair(currentChar, nextChar)) {
            return TextUtils::PackRangeInInt64(offset,  offset + 2);
        }
    }
    if (offset < textLength) {
        return TextUtils::PackRangeInInt64(offset,  offset + 1);
    }
    if (offset - 2 >= 0) {
        Char32 previousChar, previousPreviousChar;
        text->GetCharAt(offset - 1, &previousChar);
        text->GetCharAt(offset - 2, &previousPreviousChar);
        if (Character::IsSurrogatePair(previousPreviousChar, previousChar)) {
            return TextUtils::PackRangeInInt64(offset - 2,  offset);
        }
    }
    if (offset - 1 >= 0) {
        return TextUtils::PackRangeInInt64(offset - 1,  offset);
    }
    return TextUtils::PackRangeInInt64(offset,  offset);
}

Boolean Editor::TouchPositionIsInSelection()
{
    Int32 selectionStart = mTextView->GetSelectionStart();
    Int32 selectionEnd = mTextView->GetSelectionEnd();

    if (selectionStart == selectionEnd) {
        return FALSE;
    }

    if (selectionStart > selectionEnd) {
        Int32 tmp = selectionStart;
        selectionStart = selectionEnd;
        selectionEnd = tmp;
        AutoPtr<ICharSequence> text = mTextView->GetText();
        Selection::SetSelection(ISpannable::Probe(text), selectionStart, selectionEnd);
    }

    AutoPtr<SelectionModifierCursorController> selectionController = GetSelectionController();
    Int32 minOffset = selectionController->GetMinTouchOffset();
    Int32 maxOffset = selectionController->GetMaxTouchOffset();

    return ((minOffset >= selectionStart) && (maxOffset < selectionEnd));
}

AutoPtr<PositionListener> Editor::GetPositionListener()
{
    if (mPositionListener == NULL) {
        mPositionListener = new PositionListener(this);
    }
    return mPositionListener;
}

Boolean Editor::IsPositionVisible(
    /* [in] */ Int32 positionX,
    /* [in] */ Int32 positionY)
{
//    synchronized (TEMP_POSITION)
    {
        AutoLock lock(mTempPositionLock);
        AutoPtr<ArrayOf<Float> > position = TEMP_POSITION;
        (*position)[0] = positionX;
        (*position)[1] = positionY;
        IView* view = (IView*)(mTextView->Probe(EIID_IView));
        IView* textView = view;

        AutoPtr<IMatrix> matrix;
        Boolean isIdentity;
        Int32 sx, sy, w, h, l, t;

        while (view != NULL) {
            if (view != textView) {
                // Local scroll is already taken into account in positionX/Y
                view->GetScrollX(&sx);
                view->GetScrollY(&sy);
                (*position)[0] -= sx;
                (*position)[1] -= sy;
            }

            view->GetWidth(&w);
            view->GetHeight(&h);
            if ((*position)[0] < 0 || (*position)[1] < 0 ||
                    (*position)[0] > w || (*position)[1] > h) {
                return FALSE;
            }

            matrix = NULL;
            view->GetMatrix((IMatrix**)&matrix);
            matrix->IsIdentity(&isIdentity);
            if (!isIdentity) {
                matrix->MapPoints(position);
            }

            view->GetLeft(&l);
            view->GetTop(&t);
            (*position)[0] += l;
            (*position)[1] += t;

            AutoPtr<IViewParent> parent;
            view->GetParent((IViewParent**)&parent);
            if (IView::Probe(parent)) {
                view = IView::Probe(parent);
            } else {
                // We've reached the ViewRoot, stop iterating
                view = NULL;
            }
        }
    }

    // We've been able to walk up the view hierarchy and the position was never clipped
    return TRUE;
}

Boolean Editor::IsOffsetVisible(
    /* [in] */ Int32 offset)
{
    AutoPtr<ILayout> layout = mTextView->GetLayout();
    if (layout == NULL) return FALSE;

    Int32 line, lineBottom;
    layout->GetLineForOffset(offset, &line);
    layout->GetLineBottom(line, &lineBottom);
    Float hor;
    layout->GetPrimaryHorizontal(offset, &hor);
    Int32 primaryHorizontal = (Int32) hor;
    return IsPositionVisible(primaryHorizontal + mTextView->ViewportToContentHorizontalOffset(),
            lineBottom + mTextView->ViewportToContentVerticalOffset());
}

Boolean Editor::IsPositionOnText(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    AutoPtr<ILayout> layout = mTextView->GetLayout();
    if (layout == NULL) return FALSE;

    Int32 line = mTextView->GetLineAtCoordinate(y);
    x = mTextView->ConvertToLocalHorizontalCoordinate(x);

    Float l, r;
    layout->GetLineLeft(line, &l);
    if (x < l) return FALSE;
    layout->GetLineRight(line, &r);
    if (x > r) return FALSE;
    return TRUE;
}

Boolean Editor::PerformLongClick(
    /* [in] */ Boolean handled)
{
    // Long press in empty space moves cursor and shows the Paste affordance if available.
    if (!handled && !IsPositionOnText(mLastDownPositionX, mLastDownPositionY) &&
            mInsertionControllerEnabled) {
        Int32 offset = mTextView->GetOffsetForPosition(mLastDownPositionX,
                mLastDownPositionY);
        StopSelectionActionMode();
        AutoPtr<ICharSequence> text = mTextView->GetText();
        Selection::SetSelection(ISpannable::Probe(text), offset);
        AutoPtr<InsertionPointCursorController> controller = GetInsertionController();
        controller->ShowWithActionPopup();
        handled = TRUE;
    }

    if (!handled && mSelectionActionMode != NULL) {
        if (TouchPositionIsInSelection()) {
            // Start a drag
            Int32 start = mTextView->GetSelectionStart();
            Int32 end = mTextView->GetSelectionEnd();
            AutoPtr<ICharSequence> selectedText = mTextView->GetTransformedText(start, end);
            assert(0 && "TODO");
            AutoPtr<IClipDataHelper> helper;
//TODO     CClipDataHelper::AcquireSingleton((IClipDataHelper**)&helper);
            AutoPtr<IClipData> data;
            helper->NewPlainText(NULL, selectedText, (IClipData**)&data);

            AutoPtr<DragLocalState> localState = new DragLocalState(mTextView, start, end);
            AutoPtr<IDragShadowBuilder> builder = GetTextThumbnailBuilder(selectedText);
            if (builder == NULL)
                return FALSE;
            mTextView->StartDrag(data, builder, localState, 0);
            StopSelectionActionMode();
        } else {
            AutoPtr<SelectionModifierCursorController> controller = GetSelectionController();
            controller->Hide();
            SelectCurrentWord();
            controller->Show();
        }
        handled = TRUE;
    }

    // Start a new selection
    if (!handled) {
        handled = StartSelectionActionMode();
    }

    return handled;
}

Int64 Editor::GetLastTouchOffsets()
{
    AutoPtr<SelectionModifierCursorController> selectionController = GetSelectionController();
    Int32 minOffset = selectionController->GetMinTouchOffset();
    Int32 maxOffset = selectionController->GetMaxTouchOffset();
    return TextUtils::PackRangeInInt64(minOffset, maxOffset);
}

void Editor::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction)
{
    mShowCursor = SystemClock::GetUptimeMillis();
    EnsureEndedBatchEdit();

    AutoPtr<ICharSequence> text = mTextView->GetText();
    AutoPtr<ISpannable> spannable = ISpannable::Probe(text);
    if (focused) {
        Int32 selStart = mTextView->GetSelectionStart();
        Int32 selEnd = mTextView->GetSelectionEnd();

        // SelectAllOnFocus fields are highlighted and not selected. Do not start text selection
        // mode for these, unless there was a specific selection already started.
        Boolean isFocusHighlighted = mSelectAllOnFocus && selStart == 0 &&
                selEnd == mTextView->GetLength();

        mCreatedWithASelection = mFrozenWithFocus && mTextView->HasSelection() &&
                !isFocusHighlighted;

        if (!mFrozenWithFocus || (selStart < 0 || selEnd < 0)) {
            // If a tap was used to give focus to that view, move cursor at tap position.
            // Has to be done before onTakeFocus, which can be overloaded.
            Int32 lastTapPosition = GetLastTapPosition();
            if (lastTapPosition >= 0) {
                Selection::SetSelection(spannable, lastTapPosition);
            }

            // Note this may have to be moved out of the Editor class
            AutoPtr<IMovementMethod> mMovement = mTextView->GetMovementMethod();
            if (mMovement != NULL) {
                mMovement->OnTakeFocus(
                        ITEXTVIEW_PROBE(mTextView),
                        spannable, direction);
            }

            // The DecorView does not have focus when the 'Done' ExtractEditText button is
            // pressed. Since it is the ViewAncestor's mView, it requests focus before
            // ExtractEditText clears focus, which gives focus to the ExtractEditText.
            // This special case ensure that we keep current selection in that case.
            // It would be better to know why the DecorView does not have focus at that time.
            if ((mTextView->Probe(EIID_IExtractEditText) || mSelectionMoved) &&
                    selStart >= 0 && selEnd >= 0) {
                /*
                 * Someone intentionally set the selection, so let them
                 * do whatever it is that they wanted to do instead of
                 * the default on-focus behavior.  We reset the selection
                 * here instead of just skipping the onTakeFocus() call
                 * because some movement methods do something other than
                 * just setting the selection in theirs and we still
                 * need to go through that path.
                 */
                Selection::SetSelection(spannable, selStart, selEnd);
            }

            if (mSelectAllOnFocus) {
                mTextView->SelectAllText();
            }

            mTouchFocusSelected = TRUE;
        }

        mFrozenWithFocus = FALSE;
        mSelectionMoved = FALSE;

        if (mError != NULL) {
            ShowError();
        }

        MakeBlink();
    }
    else {
        if (mError != NULL) {
            HideError();
        }
        // Don't leave us in the middle of a batch edit.
        mTextView->OnEndBatchEdit();

        if (mTextView->Probe(EIID_IExtractEditText)) {
            // terminateTextSelectionMode removes selection, which we want to keep when
            // ExtractEditText goes out of focus.
            Int32 selStart = mTextView->GetSelectionStart();
            Int32 selEnd = mTextView->GetSelectionEnd();
            HideControllers();
            Selection::SetSelection(spannable, selStart, selEnd);
        }
        else {
            if (mTemporaryDetach) mPreserveDetachedSelection = TRUE;
            HideControllers();
            if (mTemporaryDetach) mPreserveDetachedSelection = FALSE;
            DowngradeEasyCorrectionSpans();
        }

        // No need to create the controller
        if (mSelectionModifierCursorController != NULL) {
            mSelectionModifierCursorController->ResetTouchOffsets();
        }
    }
}

void Editor::DowngradeEasyCorrectionSpans()
{
    AutoPtr<ICharSequence> text = mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    if (spannable) {
        Int32 length;
        spannable->GetLength(&length);
        AutoPtr<ArrayOf<ISuggestionSpan*> > temp;
        spannable->GetSpans(0, length, EIID_ISuggestionSpan, (ArrayOf<IInterface*>**)&temp);
        AutoPtr<ArrayOf<ISuggestionSpan*> > suggestionSpans = ArrayOf<ISuggestionSpan*>::Alloc(temp->GetLength());
        for(Int32 i = 0; i < temp->GetLength(); i++) {
            suggestionSpans->Set(i, ISuggestionSpan::Probe((*temp)[i]));
        }
        if (suggestionSpans != NULL) {
            Int32 flags;
            for (Int32 i = 0; i < suggestionSpans->GetLength(); i++) {
                (*suggestionSpans)[i]->GetFlags(&flags);
                if ((flags & ISuggestionSpan::FLAG_EASY_CORRECT) != 0
                        && (flags & ISuggestionSpan::FLAG_MISSPELLED) == 0) {
                    flags &= ~ISuggestionSpan::FLAG_EASY_CORRECT;
                    (*suggestionSpans)[i]->SetFlags(flags);
                }
            }
        }
    }
}

void Editor::SendOnTextChanged(
    /* [in] */ Int32 start,
    /* [in] */ Int32 after)
{
    UpdateSpellCheckSpans(start, start + after, FALSE);

    // Hide the controllers as soon as text is modified (typing, procedural...)
    // We do not hide the span controllers, since they can be added when a new text is
    // inserted into the text view (voice IME).
    HideCursorControllers();
}

Int32 Editor::GetLastTapPosition()
{
    // No need to create the controller at that point, no last tap position saved
    if (mSelectionModifierCursorController != NULL) {
        Int32 lastTapPosition = mSelectionModifierCursorController->GetMinTouchOffset();
        if (lastTapPosition >= 0) {
            // Safety check, should not be possible.
            Int32 length = mTextView->GetLength();
            if (lastTapPosition > length) {
                lastTapPosition = length;
            }
            return lastTapPosition;
        }
    }

    return -1;
}

void Editor::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    if (hasWindowFocus) {
        if (mBlink != NULL) {
            mBlink->Uncancel();
            MakeBlink();
        }
    } else {
        if (mBlink != NULL) {
            mBlink->Cancel();
        }
        if (mInputContentType != NULL) {
            mInputContentType->mEnterDown = FALSE;
        }
        // Order matters! Must be done before onParentLostFocus to rely on isShowingUp
        HideControllers();
        if (mSuggestionsPopupWindow != NULL) {
            mSuggestionsPopupWindow->OnParentLostFocus();
        }

        // Don't leave us in the middle of a batch edit. Same as in onFocusChanged
        EnsureEndedBatchEdit();
    }
}

void Editor::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    assert(event);

    if (HasSelectionController()) {
        GetSelectionController()->OnTouchEvent(event);
    }

    if (mShowSuggestionRunnable != NULL) {
        mTextView->RemoveCallbacks(mShowSuggestionRunnable);
        mShowSuggestionRunnable = NULL;
    }

    Int32 masked;
    event->GetActionMasked(&masked);
    if (masked == IMotionEvent::ACTION_DOWN) {
        event->GetX(&mLastDownPositionX);
        event->GetY(&mLastDownPositionY);

        // Reset this state; it will be re-set if super.onTouchEvent
        // causes focus to move to the view.
        mTouchFocusSelected = FALSE;
        mIgnoreActionUpEvent = FALSE;
    }
}

ECode Editor::BeginBatchEdit()
{
    mInBatchEditControllers = TRUE;
    AutoPtr<InputMethodState> ims = mInputMethodState;
    if (ims != NULL) {
        Int32 nesting = ++ims->mBatchEditNesting;
        if (nesting == 1) {
            ims->mCursorChanged = FALSE;
            ims->mChangedDelta = 0;
            if (ims->mContentChanged) {
                // We already have a pending change from somewhere else,
                // so turn this into a full update.
                ims->mChangedStart = 0;
                ims->mChangedEnd = mTextView->GetLength();
            } else {
                ims->mChangedStart = EXTRACT_UNKNOWN;
                ims->mChangedEnd = EXTRACT_UNKNOWN;
                ims->mContentChanged = FALSE;
            }
            mTextView->OnBeginBatchEdit();
        }
    }
    return NOERROR;
}

ECode Editor::EndBatchEdit()
{
    mInBatchEditControllers = FALSE;
    AutoPtr<InputMethodState> ims = mInputMethodState;
    if (ims != NULL) {
        Int32 nesting = --ims->mBatchEditNesting;
        if (nesting == 0) {
            FinishBatchEdit(ims);
        }
    }
    return NOERROR;
}

void Editor::EnsureEndedBatchEdit()
{
    AutoPtr<InputMethodState> ims = mInputMethodState;
    if (ims != NULL && ims->mBatchEditNesting != 0) {
        ims->mBatchEditNesting = 0;
        FinishBatchEdit(ims);
    }
}

void Editor::FinishBatchEdit(
    /* [in] */ InputMethodState* ims)
{
    mTextView->OnEndBatchEdit();

    if (ims->mContentChanged || ims->mSelectionModeChanged) {
        mTextView->UpdateAfterEdit();
        ReportExtractedText();
    } else if (ims->mCursorChanged) {
        // Cheezy way to get us to report the current cursor location.
        mTextView->InvalidateCursor();
    }
}

Boolean Editor::ExtractText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ IExtractedText* outText)
{
    return ExtractTextInternal(request, EXTRACT_UNKNOWN, EXTRACT_UNKNOWN,
            EXTRACT_UNKNOWN, outText);
}

Boolean Editor::ExtractTextInternal(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 partialStartOffset,
    /* [in] */ Int32 partialEndOffset,
    /* [in] */ Int32 delta,
    /* [in] */ IExtractedText* outText)
{
    AutoPtr<ICharSequence> content = mTextView->GetText();
    if (content != NULL) {
        if (partialStartOffset != EXTRACT_NOTHING) {
            Int32 N;
            content->GetLength(&N);
            if (partialStartOffset < 0) {
                outText->SetPartialStartOffset(-1);
                outText->SetPartialEndOffset(-1);
                partialStartOffset = 0;
                partialEndOffset = N;
            } else {
                // Now use the delta to determine the actual amount of text
                // we need.
                partialEndOffset += delta;
                // Adjust offsets to ensure we contain full spans.
                if (ISpanned::Probe(content)) {
                    ISpanned* spanned = ISpanned::Probe(content);
                    AutoPtr<ArrayOf<IInterface*> > spans;
                    spanned->GetSpans(partialStartOffset, partialEndOffset,
                            EIID_IParcelableSpan, (ArrayOf<IInterface*>**)&spans);
                    if (spans) {
                        Int32 i = spans->GetLength();
                        Int32 j;
                        while (i > 0) {
                            i--;
                            spanned->GetSpanStart((*spans)[i], &j);
                            if (j < partialStartOffset) partialStartOffset = j;
                            spanned->GetSpanEnd((*spans)[i], &j);
                            if (j > partialEndOffset) partialEndOffset = j;
                        }
                    }
                }
                outText->SetPartialStartOffset(partialStartOffset);
                outText->SetPartialEndOffset(partialEndOffset - delta);

                if (partialStartOffset > N) {
                    partialStartOffset = N;
                } else if (partialStartOffset < 0) {
                    partialStartOffset = 0;
                }
                if (partialEndOffset > N) {
                    partialEndOffset = N;
                } else if (partialEndOffset < 0) {
                    partialEndOffset = 0;
                }
            }

            Int32 flags;
            request->GetFlags(&flags);
            if ((flags & IInputConnection::GET_TEXT_WITH_STYLES) != 0) {
                AutoPtr<ICharSequence> subSeq;
                content->SubSequence(partialStartOffset, partialEndOffset, (ICharSequence**)&subSeq);
                outText->SetText(subSeq);
            } else {
                String str = TextUtils::Substring(content, partialStartOffset, partialEndOffset);
                AutoPtr<ICharSequence> seq;
                CStringWrapper::New(str, (ICharSequence**)&seq);
                outText->SetText(seq);
            }
        } else {
            outText->SetPartialStartOffset(0);
            outText->SetPartialEndOffset(0);
            AutoPtr<ICharSequence> seq;
            CStringWrapper::New(String(""), (ICharSequence**)&seq);
            outText->SetText(seq);
        }
        outText->SetFlags(0);

        AutoPtr<IMetaKeyKeyListenerHelper> helper;
        CMetaKeyKeyListenerHelper::AcquireSingleton((IMetaKeyKeyListenerHelper**)&helper);
        Int32 state;
        helper->GetMetaState(content, IMetaKeyKeyListener::META_SELECTING, &state);
        if (state != 0) {
            Int32 flags;
            outText->GetFlags(&flags);
            outText->SetFlags(flags | IExtractedText::FLAG_SELECTING);
        }
        if (mTextView->IsSingleLine()) {
            Int32 flags;
            outText->GetFlags(&flags);
            outText->SetFlags(flags | IExtractedText::FLAG_SINGLE_LINE);
        }
        outText->SetStartOffset(0);
        outText->SetSelectionStart(mTextView->GetSelectionStart());
        outText->SetSelectionEnd(mTextView->GetSelectionEnd());
        return TRUE;
    }
    return FALSE;
}

Boolean Editor::ReportExtractedText()
{
    AutoPtr<InputMethodState> ims = mInputMethodState;
    if (ims != NULL) {
        Boolean contentChanged = ims->mContentChanged;
        if (contentChanged || ims->mSelectionModeChanged) {
            ims->mContentChanged = FALSE;
            ims->mSelectionModeChanged = FALSE;
            AutoPtr<IExtractedTextRequest> req = ims->mExtractedTextRequest;
            if (req != NULL) {
                AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
                if (imm != NULL) {
//                    if (TextView::DEBUG_EXTRACT) Log.v(TextView.LOG_TAG,
//                            "Retrieving extracted start=" + ims->mChangedStart +
//                            " end=" + ims->mChangedEnd +
//                            " delta=" + ims->mChangedDelta);
                    if (ims->mChangedStart < 0 && !contentChanged) {
                        ims->mChangedStart = EXTRACT_NOTHING;
                    }
                    if (ExtractTextInternal(req, ims->mChangedStart, ims->mChangedEnd,
                            ims->mChangedDelta, ims->mExtractedText)) {
//                        if (TextView.DEBUG_EXTRACT) Log.v(TextView.LOG_TAG,
//                                "Reporting extracted start=" +
//                                ims->mExtractedText.partialStartOffset +
//                                " end=" + ims->mExtractedText.partialEndOffset +
//                                ": " + ims->mExtractedText.text);

                        Int32 token;
                        req->GetToken(&token);
                        imm->UpdateExtractedText(
                                ITEXTVIEW_PROBE(mTextView),
                                token, ims->mExtractedText);
                        ims->mChangedStart = EXTRACT_UNKNOWN;
                        ims->mChangedEnd = EXTRACT_UNKNOWN;
                        ims->mChangedDelta = 0;
                        ims->mContentChanged = FALSE;
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

void Editor::OnDraw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ILayout* layout,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightPaint,
    /* [in] */ Int32 cursorOffsetVertical)
{
    assert(canvas != NULL);
    assert(layout != NULL);
    Int32 selectionStart = mTextView->GetSelectionStart();
    Int32 selectionEnd = mTextView->GetSelectionEnd();

    AutoPtr<InputMethodState> ims = mInputMethodState;
    if (ims != NULL && ims->mBatchEditNesting == 0) {
        AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
        if (imm != NULL) {
            Boolean active;
            imm->IsActive(ITEXTVIEW_PROBE(mTextView), &active);
            if (active) {
                Boolean reported = FALSE;
                if (ims->mContentChanged || ims->mSelectionModeChanged) {
                    // We are in extract mode and the content has changed
                    // in some way... just report complete new text to the
                    // input method.
                    reported = ReportExtractedText();
                }
                if (!reported && highlight != NULL) {
                    Int32 candStart = -1;
                    Int32 candEnd = -1;
                    AutoPtr<ICharSequence> text = mTextView->GetText();
                    ISpannable* spannable = ISpannable::Probe(text);
                    if (spannable) {
                        candStart = CEditableInputConnection::GetComposingSpanStart(spannable);
                        candEnd = CEditableInputConnection::GetComposingSpanEnd(spannable);
                    }
                    imm->UpdateSelection(ITEXTVIEW_PROBE(mTextView),
                            selectionStart, selectionEnd, candStart, candEnd);
                }
            }

            Boolean isWatch;
            imm->IsWatchingCursor(ITEXTVIEW_PROBE(mTextView), &isWatch);
            if (isWatch && highlight != NULL) {
                highlight->ComputeBounds(ims->mTmpRectF, TRUE);
                ims->mTmpOffset->Set(0, 0);
                ims->mTmpOffset->Set(1, 0);

                AutoPtr<IMatrix> matrix;
                canvas->GetMatrix((IMatrix**)&matrix);
                matrix->MapPoints(ims->mTmpOffset);
                ims->mTmpRectF->Offset((*(ims->mTmpOffset))[0], (*(ims->mTmpOffset))[1]);

                ims->mTmpRectF->Offset(0, cursorOffsetVertical);

                Float l, t, r, b;
                ims->mTmpRectF->Get(&l, &t, &r, &b);

                Int32 il = (Int32)(l + 0.5);
                Int32 it = (Int32)(t + 0.5);
                Int32 ir = (Int32)(r + 0.5);
                Int32 ib = (Int32)(b + 0.5);
                ims->mCursorRectInWindow->Set(il, it, ir, ib);
                imm->UpdateCursor(ITEXTVIEW_PROBE(mTextView), il, it, ir, ib);
            }
        }
    }

    if (mCorrectionHighlighter != NULL) {
        mCorrectionHighlighter->Draw(canvas, cursorOffsetVertical);
    }

    if (highlight != NULL && selectionStart == selectionEnd && mCursorCount > 0) {
        DrawCursor(canvas, cursorOffsetVertical);
        // Rely on the drawable entirely, do not draw the cursor line.
        // Has to be done after the IMM related code above which relies on the highlight.
        highlight = NULL;
    }

    Boolean isAccelerated = FALSE;
    canvas->IsHardwareAccelerated(&isAccelerated);
    if (mTextView->CanHaveDisplayList() && isAccelerated) {
        DrawHardwareAccelerated(canvas, layout, highlight, highlightPaint,
                cursorOffsetVertical);
    } else {
        layout->Draw(canvas, highlight, highlightPaint, cursorOffsetVertical);
    }
}

void Editor::DrawHardwareAccelerated(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ILayout* layout,
    /* [in] */ IPath* highlight,
    /* [in] */ IPaint* highlightPaint,
    /* [in] */ Int32 cursorOffsetVertical)
{
    assert(layout);

    Int64 lineRange;
    layout->GetLineRangeForDraw(canvas, &lineRange);
    Int32 firstLine = TextUtils::UnpackRangeStartFromInt64(lineRange);
    Int32 lastLine = TextUtils::UnpackRangeEndFromInt64(lineRange);
    if (lastLine < 0) return;

    layout->DrawBackground(canvas, highlight, highlightPaint, cursorOffsetVertical,
            firstLine, lastLine);

    if (IDynamicLayout::Probe(layout)) {
        if (mTextDisplayLists == NULL) {
            Int32 size = ArrayUtils::IdealObjectArraySize(0);
            mTextDisplayLists = ArrayOf<IDisplayList*>::Alloc(size);
        }

        // If the height of the layout changes (usually when inserting or deleting a line,
        // but could be changes within a span), invalidate everything. We could optimize
        // more aggressively (for example, adding offsets to blocks) but it would be more
        // complex and we would only get the benefit in some cases.
        Int32 layoutHeight;
        layout->GetHeight(&layoutHeight);
        if (mLastLayoutHeight != layoutHeight) {
            InvalidateTextDisplayList();
            mLastLayoutHeight = layoutHeight;
        }

        IDynamicLayout* dynamicLayout = IDynamicLayout::Probe(layout);
        AutoPtr<ArrayOf<Int32> > blockEndLines = ((DynamicLayout*)dynamicLayout)->GetBlockEndLines();
        AutoPtr<ArrayOf<Int32> > blockIndices = ((DynamicLayout*)dynamicLayout)->GetBlockIndices();
        Int32 numberOfBlocks = ((DynamicLayout*)dynamicLayout)->GetNumberOfBlocks();

        Int32 endOfPreviousBlock = -1;
        Int32 searchStartIndex = 0;
        for (Int32 i = 0; i < numberOfBlocks; i++) {
            Int32 blockEndLine = (*blockEndLines)[i];
            Int32 blockIndex = (*blockIndices)[i];

            Boolean blockIsInvalid = blockIndex == DynamicLayout::INVALID_BLOCK_INDEX;
            if (blockIsInvalid) {
                blockIndex = GetAvailableDisplayListIndex(blockIndices, numberOfBlocks,
                        searchStartIndex);
                // Note how dynamic layout's internal block indices get updated from Editor
                (*blockIndices)[i] = blockIndex;
                searchStartIndex = blockIndex + 1;
            }

            AutoPtr<IDisplayList> blockDisplayList = (*mTextDisplayLists)[blockIndex];
            if (blockDisplayList == NULL) {
// TODO
//                blockDisplayList = mTextView->GetHardwareRenderer().createDisplayList("Text " + blockIndex);
//                mTextDisplayLists->Set(blockIndex, blockDisplayList);
            }
            else {
                if (blockIsInvalid) blockDisplayList->Invalidate();
            }

            assert(blockDisplayList);
            Boolean valid;
            blockDisplayList->IsValid(&valid);
            if (!valid) {
                Int32 blockBeginLine = endOfPreviousBlock + 1;
                Int32 top, bottom;
                layout->GetLineTop(blockBeginLine, &top);
                layout->GetLineBottom(blockEndLine, &bottom);
                Int32 left = 0;
                Int32 right = mTextView->GetWidth();
                if (mTextView->GetHorizontallyScrolling()) {
                    Float min = Elastos::Core::Math::FLOAT_MAX_VALUE;
                    Float max = Elastos::Core::Math::FLOAT_MIN_VALUE;
                    Float l, r;
                    for (Int32 line = blockBeginLine; line <= blockEndLine; line++) {
                        layout->GetLineLeft(line, &l);
                        layout->GetLineRight(line, &r);
                        min = Elastos::Core::Math::Min(min, l);
                        max = Elastos::Core::Math::Max(max, r);
                    }
                    left = (Int32) min;
                    right = (Int32) (max + 0.5f);
                }
//TODO
//                HardwareCanvas hardwareCanvas = blockDisplayList.start();
//                try {
//                    // Tighten the bounds of the viewport to the actual text size
//                    hardwareCanvas.setViewport(right - left, bottom - top);
//                    // The dirty rect should always be NULL for a display list
//                    hardwareCanvas.onPreDraw(NULL);
//                    // drawText is always relative to TextView's origin, this translation brings
//                    // this range of text back to the top left corner of the viewport
//                    hardwareCanvas.translate(-left, -top);
//                    layout.drawText(hardwareCanvas, blockBeginLine, blockEndLine);
//                    // No need to untranslate, previous context is popped after drawDisplayList
//                } finally {
//                    hardwareCanvas.onPostDraw();
//                    blockDisplayList.end();
//                    blockDisplayList.setLeftTopRightBottom(left, top, right, bottom);
//                    // Same as drawDisplayList below, handled by our TextView's parent
//                    blockDisplayList.setClipChildren(FALSE);
//                }
            }
//
//            ((HardwareCanvas) canvas).drawDisplayList(blockDisplayList, NULL,
//                    0 /* no child clipping, our TextView parent enforces it */);

            endOfPreviousBlock = blockEndLine;
        }
    } else {
        // Boring layout is used for empty and hint text
        layout->DrawText(canvas, firstLine, lastLine);
    }
}

Int32 Editor::GetAvailableDisplayListIndex(
    /* [in] */ ArrayOf<Int32>* blockIndices,
    /* [in] */ Int32 numberOfBlocks,
    /* [in] */ Int32 searchStartIndex)
{
    Int32 length = mTextDisplayLists->GetLength();
    for (Int32 i = searchStartIndex; i < length; i++) {
        Boolean blockIndexFound = FALSE;
        for (Int32 j = 0; j < numberOfBlocks; j++) {
            if ((*blockIndices)[j] == i) {
                blockIndexFound = TRUE;
                break;
            }
        }
        if (blockIndexFound) continue;
        return i;
    }

    // No available index found, the pool has to grow
    Int32 newSize = ArrayUtils::IdealInt32ArraySize(length + 1);
    AutoPtr<ArrayOf<IDisplayList*> > displayLists = ArrayOf<IDisplayList*>::Alloc(newSize);
    displayLists->Copy(mTextDisplayLists, 0, length);
    mTextDisplayLists = displayLists;
    return length;
}

void Editor::DrawCursor(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 cursorOffsetVertical)
{
    Boolean translate = cursorOffsetVertical != 0;
    if (translate) canvas->Translate(0, cursorOffsetVertical);
    for (Int32 i = 0; i < mCursorCount; i++) {
        (*mCursorDrawable)[i]->Draw(canvas);
    }
    if (translate) canvas->Translate(0, -cursorOffsetVertical);
}

void Editor::InvalidateTextDisplayList(
    /* [in] */ ILayout* layout,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (mTextDisplayLists != NULL && IDynamicLayout::Probe(layout)) {
        Int32 firstLine, lastLine;
        layout->GetLineForOffset(start, &firstLine);
        layout->GetLineForOffset(end, &lastLine);

        IDynamicLayout* dynamicLayout = IDynamicLayout::Probe(layout);
        AutoPtr<ArrayOf<Int32> > blockEndLines = ((DynamicLayout*)dynamicLayout)->GetBlockEndLines();
        AutoPtr<ArrayOf<Int32> > blockIndices = ((DynamicLayout*)dynamicLayout)->GetBlockIndices();
        Int32 numberOfBlocks = ((DynamicLayout*)dynamicLayout)->GetNumberOfBlocks();

        Int32 i = 0;
        // Skip the blocks before firstLine
        while (i < numberOfBlocks) {
            if ((*blockEndLines)[i] >= firstLine) break;
            i++;
        }

        // Invalidate all subsequent blocks until lastLine is passed
        while (i < numberOfBlocks) {
            Int32 blockIndex = (*blockIndices)[i];
            if (blockIndex != DynamicLayout::INVALID_BLOCK_INDEX) {
                (*mTextDisplayLists)[blockIndex]->Invalidate();
            }
            if ((*blockEndLines)[i] >= lastLine) break;
            i++;
        }
    }
}

void Editor::InvalidateTextDisplayList()
{
    if (mTextDisplayLists != NULL) {
        for (Int32 i = 0; i < mTextDisplayLists->GetLength(); i++) {
            if ((*mTextDisplayLists)[i] != NULL)
                (*mTextDisplayLists)[i]->Invalidate();
        }
    }
}

void Editor::UpdateCursorsPositions()
{
    if (mTextView->mCursorDrawableRes == 0) {
        mCursorCount = 0;
        return;
    }

    AutoPtr<ILayout> layout = mTextView->GetLayout();
    AutoPtr<ILayout> hintLayout = mTextView->GetHintLayout();
    Int32 offset = mTextView->GetSelectionStart();
    Int32 line;
    layout->GetLineForOffset(offset, &line);
    Int32 top, bottom;
    layout->GetLineTop(line, &top);
    layout->GetLineTop(line + 1, &bottom);

    Boolean result;
    layout->IsLevelBoundary(offset, &result);
    mCursorCount = result ? 2 : 1;

    Int32 middle = bottom;
    if (mCursorCount == 2) {
        // Similar to what is done in {@link Layout.#getCursorPath(Int32, Path, CharSequence)}
        middle = (top + bottom) >> 1;
    }

    UpdateCursorPosition(0, top, middle, GetPrimaryHorizontal(layout, hintLayout, offset));

    if (mCursorCount == 2) {
        Float hor;
        layout->GetSecondaryHorizontal(offset, &hor);
        UpdateCursorPosition(1, middle, bottom, hor);
    }
}

Float Editor::GetPrimaryHorizontal(
    /* [in] */ ILayout* layout,
    /* [in] */ ILayout* hintLayout,
    /* [in] */ Int32 offset)
{
    Float value = 0.0;
    VALIDATE_NOT_NULL(layout);
    AutoPtr<ICharSequence> text;
    layout->GetText((ICharSequence**)&text);
    if (TextUtils::IsEmpty(text) && hintLayout != NULL) {
        text = NULL;
        hintLayout->GetText((ICharSequence**)&text);
        if (!TextUtils::IsEmpty(text)) {
            hintLayout->GetPrimaryHorizontal(offset, &value);
            return value;
        }
    }

    layout->GetPrimaryHorizontal(offset, &value);
    return value;
}

Boolean Editor::StartSelectionActionMode()
{
    if (mSelectionActionMode != NULL) {
        // Selection action mode is already started
        return FALSE;
    }

    if (!CanSelectText() || !mTextView->RequestFocus()) {
//        Log.w(TextView.LOG_TAG,
//                "TextView does not support text selection. Action mode cancelled.");
        return FALSE;
    }

    if (!mTextView->HasSelection()) {
        // There may already be a selection on device rotation
        if (!SelectCurrentWord()) {
            // No word found under cursor or text selection not permitted.
            return FALSE;
        }
    }

    Boolean willExtract = ExtractedTextModeWillBeStarted();

    // Do not start the action mode when extracted text will show up full screen, which would
    // immediately hide the newly created action bar and would be visually distracting.
    if (!willExtract) {
        AutoPtr<IActionModeCallback> actionModeCallback = new SelectionActionModeCallback(this);
        mSelectionActionMode = mTextView->StartActionMode(actionModeCallback);
    }

    Boolean selectionStarted = mSelectionActionMode != NULL || willExtract;
    if (selectionStarted && !mTextView->IsTextSelectable() && mShowSoftInputOnFocus) {
        // Show the IME to be able to replace text, except when selecting non editable text.
        AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
        if (imm != NULL) {
            imm->ShowSoftInput((IView*)(mTextView->Probe(EIID_IView)), 0, NULL);
        }
    }

    return selectionStarted;
}

Boolean Editor::ExtractedTextModeWillBeStarted()
{
    if (NULL == mTextView->Probe(EIID_IExtractEditText)) {
        AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
        if (imm != NULL) {
            Boolean fullScreen;
            imm->IsFullscreenMode(&fullScreen);
            return fullScreen;
        }
    }
    return FALSE;
}

Boolean Editor::IsCursorInsideSuggestionSpan()
{
    AutoPtr<ICharSequence> text = mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    if (!spannable) return FALSE;

    AutoPtr<ArrayOf<IInterface*> > suggestionSpans;
    spannable->GetSpans(
            mTextView->GetSelectionStart(), mTextView->GetSelectionEnd(),
            EIID_ISuggestionSpan, (ArrayOf<IInterface*>**)&suggestionSpans);
    return (suggestionSpans && suggestionSpans->GetLength() > 0);
}

Boolean Editor::IsCursorInsideEasyCorrectionSpan()
{
    AutoPtr<ICharSequence> text = mTextView->GetText();
    ISpannable* spannable = ISpannable::Probe(text);
    if (spannable) {
        Int32 length;
        spannable->GetLength(&length);
        AutoPtr<ArrayOf<IInterface*> > temp;
        spannable->GetSpans(
                mTextView->GetSelectionStart(), mTextView->GetSelectionEnd(),
                EIID_ISuggestionSpan,
                (ArrayOf<IInterface*>**)&temp);
        AutoPtr<ArrayOf<ISuggestionSpan*> > suggestionSpans = ArrayOf<ISuggestionSpan*>::Alloc(temp->GetLength());
        for(Int32 i = 0; i < temp->GetLength(); i++) {
            suggestionSpans->Set(i, ISuggestionSpan::Probe((*temp)[i]));
        }

        if (suggestionSpans != NULL) {
            Int32 flags;
            for (Int32 i = 0; i < suggestionSpans->GetLength(); i++) {
                (*suggestionSpans)[i]->GetFlags(&flags);
                if ((flags & ISuggestionSpan::FLAG_EASY_CORRECT) != 0) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

void Editor::OnTouchUpEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean selectAllGotFocus = mSelectAllOnFocus && mTextView->DidTouchFocusSelect();
    HideControllers();
    AutoPtr<ICharSequence> text = mTextView->GetText();
    Int32 length;
    text->GetLength(&length);
    if (!selectAllGotFocus && length > 0) {
        Float x, y;
        event->GetX(&x);
        event->GetX(&y);
        // Move cursor
        Int32 offset = mTextView->GetOffsetForPosition(x, y);
        Selection::SetSelection(ISpannable::Probe(text), offset);
//TODO
//        if (mSpellChecker != NULL) {
//            // When the cursor moves, the word that was typed may need spell check
//            mSpellChecker->OnSelectionChanged();
//        }
        if (!ExtractedTextModeWillBeStarted()) {

            if (IsCursorInsideEasyCorrectionSpan()) {
                mShowSuggestionRunnable = new ShowSuggestionRunnable(this);

                // removeCallbacks is performed on every touch
                AutoPtr<IViewConfigurationHelper> helper;
                CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
                Int32 doubleTapTimeout;
                helper->GetDoubleTapTimeout(&doubleTapTimeout);
                mTextView->PostDelayed(mShowSuggestionRunnable, doubleTapTimeout);
            }
            else if (HasInsertionController()) {
                GetInsertionController()->Show();
            }
        }
    }
}

void Editor::StopSelectionActionMode()
{
    if (mSelectionActionMode != NULL) {
        // This will hide the mSelectionModifierCursorController
        mSelectionActionMode->Finish();
    }
}

Boolean Editor::HasInsertionController()
{
    return mInsertionControllerEnabled;
}

Boolean Editor::HasSelectionController()
{
    return mSelectionControllerEnabled;
}

AutoPtr<InsertionPointCursorController> Editor::GetInsertionController()
{
    if (!mInsertionControllerEnabled) {
        return NULL;
    }

    if (mInsertionPointCursorController == NULL) {
        mInsertionPointCursorController = new InsertionPointCursorController(this);

        AutoPtr<IViewTreeObserver> observer = mTextView->GetViewTreeObserver();
        observer->AddOnTouchModeChangeListener(mInsertionPointCursorController);
    }

    return mInsertionPointCursorController;
}

AutoPtr<SelectionModifierCursorController> Editor::GetSelectionController()
{
    if (!mSelectionControllerEnabled) {
        return NULL;
    }

    if (mSelectionModifierCursorController == NULL) {
        mSelectionModifierCursorController = new SelectionModifierCursorController(this);

        AutoPtr<IViewTreeObserver> observer = mTextView->GetViewTreeObserver();
        observer->AddOnTouchModeChangeListener(mSelectionModifierCursorController);
    }

    return mSelectionModifierCursorController;
}

void Editor::UpdateCursorPosition(
    /* [in] */ Int32 cursorIndex,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom,
    /* [in] */ Float horizontal)
{
    if ((*mCursorDrawable)[cursorIndex] == NULL) {
        AutoPtr<IResources> resources = mTextView->GetResources();
        AutoPtr<IDrawable> cursor;
        resources->GetDrawable(mTextView->mCursorDrawableRes, (IDrawable**)&cursor);
        mCursorDrawable->Set(cursorIndex, cursor);
    }

    if (mTempRect == NULL)
        CRect::New((IRect**)&mTempRect);

    Boolean isPadding;
    (*mCursorDrawable)[cursorIndex]->GetPadding(mTempRect, &isPadding);
    Int32 width;
    (*mCursorDrawable)[cursorIndex]->GetIntrinsicWidth(&width);
    horizontal = Elastos::Core::Math::Max(0.5f, horizontal - 0.5f);

    Int32 l, t, r, b;
    mTempRect->Get(&l, &t, &r, &b);
    Int32 left = (Int32) (horizontal) - l;
    (*mCursorDrawable)[cursorIndex]->SetBounds(left, top - t, left + width,
            bottom + b);
}

ECode Editor::OnCommitCorrection(
    /* [in] */ ICorrectionInfo* info)
{
    if (mCorrectionHighlighter == NULL) {
        mCorrectionHighlighter = new CorrectionHighlighter(this);
    } else {
        mCorrectionHighlighter->Invalidate(FALSE);
    }

    mCorrectionHighlighter->Highlight(info);
    return NOERROR;
}

void Editor::ShowSuggestions()
{
    if (mSuggestionsPopupWindow == NULL) {
        mSuggestionsPopupWindow = new SuggestionsPopupWindow(this);
        mSuggestionsPopupWindow->Init();
    }
    HideControllers();
    mSuggestionsPopupWindow->Show();
}

Boolean Editor::AreSuggestionsShown()
{
    return mSuggestionsPopupWindow != NULL && mSuggestionsPopupWindow->IsShowing();
}

void Editor::OnScrollChanged()
{
    if (mPositionListener != NULL) {
        mPositionListener->OnScrollChanged();
    }
}

Boolean Editor::ShouldBlink()
{
    if (!IsCursorVisible() || !mTextView->IsFocused()) return FALSE;

    Int32 start = mTextView->GetSelectionStart();
    if (start < 0) return FALSE;

    Int32 end = mTextView->GetSelectionEnd();
    if (end < 0) return FALSE;

    return start == end;
}

void Editor::MakeBlink()
{
    if (ShouldBlink()) {
        mShowCursor = SystemClock::GetUptimeMillis();
        if (mBlink == NULL) {
            mBlink = new Blink(this);
        }

        Boolean result = FALSE;
        mBlink->RemoveCallbacks(mBlink);
        mBlink->PostAtTime(mBlink, mShowCursor + BLINK, &result);
    }
    else {
        if (mBlink != NULL) {
            mBlink->RemoveCallbacks(mBlink);
        }
    }
}

AutoPtr<IDragShadowBuilder> Editor::GetTextThumbnailBuilder(
    /* [in] */ ICharSequence* textIn)
{
    AutoPtr<IContext> context = mTextView->GetContext();
    AutoPtr<IView> view;
    View::View::Inflate(context, R::layout::text_drag_thumbnail, NULL, (IView**)&view);
    AutoPtr<ITextView> shadowView = (ITextView*)(view->Probe(EIID_ITextView));

    if (shadowView == NULL) {
        return NULL;
//        throw new IllegalArgumentException("Unable to inflate text drag thumbnail");
    }
    Int32 length;
    textIn->GetLength(&length);

    AutoPtr<ICharSequence> text = textIn;
    if (length > DRAG_SHADOW_MAX_TEXT_LENGTH) {
        text = NULL;
        textIn->SubSequence(0, DRAG_SHADOW_MAX_TEXT_LENGTH, (ICharSequence**)&text);
    }
    shadowView->SetText(text);
    shadowView->SetTextColor(mTextView->GetTextColors());

    shadowView->SetTextAppearance(mTextView->GetContext(), R::styleable::Theme_textAppearanceLarge);
    shadowView->SetGravity(IGravity::CENTER);

    AutoPtr<IViewGroupLayoutParams> layout;
    CViewGroupLayoutParams::New(
            IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT,
            (IViewGroupLayoutParams**)&layout);
    shadowView->SetLayoutParams(layout);

    Int32 size = View::View::MeasureSpec::MakeMeasureSpec(0, View::View::MeasureSpec::UNSPECIFIED);
    shadowView->Measure(size, size);

    Int32 mw, mh;
    shadowView->GetMeasuredWidth(&mw);
    shadowView->GetMeasuredHeight(&mh);
    shadowView->Layout(0, 0, mw, mh);
    shadowView->Invalidate();

    AutoPtr<IDragShadowBuilder> builder;
    CDragShadowBuilder::New(shadowView, (IDragShadowBuilder**)&builder);
    return builder;
}

void Editor::OnDrop(
    /* [in] */ IDragEvent* event)
{
    assert(event);

    AutoPtr<IContext> context = mTextView->GetContext();
    StringBuilder content("");
    AutoPtr<IClipData> clipData;
    event->GetClipData((IClipData**)&clipData);
    Int32 itemCount;
    clipData->GetItemCount(&itemCount);
    AutoPtr<IClipDataItem> item;
    AutoPtr<ICharSequence> seq;
    for (Int32 i=0; i < itemCount; i++) {
        clipData->GetItemAt(i, (IClipDataItem**)&item);
        item->CoerceToStyledText(context, (ICharSequence**)&seq);
        content.AppendCharSequence(seq);
        seq = NULL;
        item = NULL;
    }

    Float x, y;
    event->GetX(&x);
    event->GetY(&y);
    Int32 offset = mTextView->GetOffsetForPosition(x, y);

    AutoPtr<IInterface> localState;
    event->GetLocalState((IInterface**)&localState);

    AutoPtr<DragLocalState> dragLocalState = (DragLocalState*)(localState.Get());
// TODO use Probe
//    if (localState instanceof DragLocalState) {
//        dragLocalState = (DragLocalState) localState;
//    }
    Boolean dragDropIntoItself = dragLocalState != NULL &&
            dragLocalState->mSourceTextView == mTextView;

    if (dragDropIntoItself) {
        if (offset >= dragLocalState->mStart && offset < dragLocalState->mEnd) {
            // A drop inside the original selection discards the drop.
            return;
        }
    }

    AutoPtr<ICharSequence> text = mTextView->GetText();
    Int32 originalLength;
    text->GetLength(&originalLength);
    AutoPtr<ICharSequence> ctSeq = content.ToCharSequence();
    Int64 minMax = mTextView->PrepareSpacesAroundPaste(offset, offset, ctSeq);
    Int32 min = TextUtils::UnpackRangeStartFromInt64(minMax);
    Int32 max = TextUtils::UnpackRangeEndFromInt64(minMax);

    Selection::SetSelection(ISpannable::Probe(text), max);
    mTextView->ReplaceText_internal(min, max, ctSeq);

    if (dragDropIntoItself) {
        Int32 dragSourceStart = dragLocalState->mStart;
        Int32 dragSourceEnd = dragLocalState->mEnd;
        if (max <= dragSourceStart) {
            // Inserting text before selection has shifted positions
            Int32 shift = mTextView->GetLength() - originalLength;
            dragSourceStart += shift;
            dragSourceEnd += shift;
        }

        // Delete original selection
        mTextView->DeleteText_internal(dragSourceStart, dragSourceEnd);

        // Make sure we do not leave two adjacent spaces.
        Int32 prevCharIdx = Elastos::Core::Math::Max(0,  dragSourceStart - 1);
        Int32 nextCharIdx = Elastos::Core::Math::Min(mTextView->GetLength(), dragSourceStart + 1);
        if (nextCharIdx > prevCharIdx + 1) {
            AutoPtr<ICharSequence> t = mTextView->GetTransformedText(prevCharIdx, nextCharIdx);
            Char32 char0, char1;
            t->GetCharAt(0, &char0);
            t->GetCharAt(1, &char1);
            if (Character::IsSpaceChar(char0) && Character::IsSpaceChar(char1)) {
                mTextView->DeleteText_internal(prevCharIdx, prevCharIdx + 1);
            }
        }
    }
}

ECode Editor::AddSpanWatchers(
    /* [in] */ ISpannable* text)
{
    assert(text != NULL);
    Int32 textLength;
    text->GetLength(&textLength);

    if (mKeyListener != NULL) {
        text->SetSpan(mKeyListener, 0, textLength, ISpanned::SPAN_INCLUSIVE_INCLUSIVE);
    }

    if (mEasyEditSpanController == NULL) {
        mEasyEditSpanController = new EasyEditSpanController(this);
    }
    text->SetSpan(mEasyEditSpanController, 0, textLength, ISpanned::SPAN_INCLUSIVE_INCLUSIVE);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
