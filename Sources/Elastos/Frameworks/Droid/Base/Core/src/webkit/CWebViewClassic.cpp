
#include "animation/CObjectAnimatorHelper.h"
#include "app/CActivityManager.h"
#include "app/CAlertDialogBuilder.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include "content/CClipDataHelper.h"
#include "graphics/CPicture.h"
#include "graphics/CPaint.h"
#include "graphics/CPoint.h"
#include "graphics/CRegionIterator.h"
#include "graphics/CBitmapShader.h"
#include "graphics/CPaintFlagsDrawFilter.h"
#include "graphics/CBitmapFactory.h"
//#include "graphics/GraphicsNative.h"
// #include "net/http/CSslCertificateHelper.h"
#include "net/Uri.h"
#include "os/SystemClock.h"
#include "os/CMessageHelper.h"
#include "os/CBundle.h"
#include "text/Selection.h"
#include "text/TextUtils.h"
#include "webkit/CWebViewClassic.h"
#include "webkit/AccessibilityInjector.h"
#include "webkit/AutoCompletePopup.h"
#include "webkit/DebugFlags.h"
#include "webkit/CWebViewCore.h"
#include "webkit/WebViewDatabaseClassic.h"
#include "webkit/ViewManager.h"
#include "webkit/NativeUtil.h"
#include "webkit/L10nUtils.h"
#include "webkit/URLUtil.h"
#include "webkit/WebCoreThreadWatchdog.h"
#include "webkit/WebTextView.h"
#include "webkit/ViewStateSerializer.h"
#include "webkit/GeolocationPermissionsClassic.h"
#include "webkit/CookieManagerClassic.h"
#include "webkit/WebIconDatabaseClassic.h"
#include "webkit/WebStorageClassic.h"
#include "webkit/WebSettingsClassic.h"
#include "webkit/CWebViewDatabaseHelper.h"
#include "webkit/HTML5VideoInline.h"
#include "webkit/HTML5VideoView.h"
#include "webkit/CPluginManager.h"
#include "webkit/CPluginManagerHelper.h"
#include "webkit/CPluginList.h"
#include "webkit/CWebViewHitTestResult.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewConfiguration.h"
#include "view/CKeyEvent.h"
#include "view/CKeyCharacterMapHelper.h"
#include "view/View.h"
#include "view/ViewRootImpl.h"
#include "view/LayoutInflater.h"
#include "view/SoundEffectConstants.h"
#include "view/accessibility/CAccessibilityManager.h"
#include "view/inputmethod/CInputMethodManager.h"
#include "widget/CArrayAdapter.h"
#include "widget/CScroller.h"
#include "widget/CLinearLayout.h"
#include "widget/CView.h"
#include "widget/CToastHelper.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::CStringWrapper;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::IByte;
using Elastos::Core::CByte;
using Elastos::Core::EIID_IByte;
using Elastos::Core::IArrayOf;
using Elastos::Core::CArrayOf;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::EIID_IInteger32;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::ISerializable;
using Elastos::IO::CFile;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::CFileInputStream;
using Elastos::Utility::CHashSet;
using Elastos::Utility::CHashMap;
using Elastos::Utility::IList;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Logging::Logger;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::CLocaleHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::AccessibilityService::IAccessibilityServiceInfo;
using Elastos::Droid::Animation::IObjectAnimatorHelper;
using Elastos::Droid::Animation::CObjectAnimatorHelper;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::App::CActivityManager;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::Content::IClipData;
using Elastos::Droid::Content::CClipDataHelper;
using Elastos::Droid::Content::IClipDataHelper;
using Elastos::Droid::Content::IClipDataItem;
using Elastos::Droid::Content::IClipboardManager;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Database::EIID_IDataSetObserver;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawable;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::CPicture;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::IDrawFilter;
using Elastos::Droid::Graphics::CRegionIterator;
using Elastos::Droid::Graphics::IRegionIterator;
using Elastos::Droid::Graphics::IShader;
using Elastos::Droid::Graphics::CBitmapShader;
using Elastos::Droid::Graphics::IBitmapShader;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPaintFlagsDrawFilter;
using Elastos::Droid::Graphics::CPaintFlagsDrawFilter;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
//using Elastos::Droid::Graphics::GraphicsNative;
using Elastos::Droid::Net::Uri;
// using Elastos::Droid::Net::Http::CSslCertificateHelper;
using Elastos::Droid::Net::Http::ISslCertificateHelper;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IPowerManager;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Security::IKeyChain;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IActionMode;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IHapticFeedbackConstants;
using Elastos::Droid::View::IInputDevice;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::View::IKeyCharacterMap;
using Elastos::Droid::View::CKeyEvent;
using Elastos::Droid::View::CKeyCharacterMapHelper;
using Elastos::Droid::View::IKeyCharacterMapHelper;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::View;
using Elastos::Droid::View::ViewRootImpl;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::SoundEffectConstants;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::View::InputMethod::EIID_IInputConnection;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::EIID_BaseInputConnection;
using Elastos::Droid::View::InputMethod::EIID_IBaseInputConnection;
using Elastos::Droid::Widget::CArrayAdapter;
using Elastos::Droid::Widget::EIID_IArrayAdapter;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Widget::CLinearLayout;
using Elastos::Droid::Widget::CScroller;
using Elastos::Droid::Widget::CView;
using Elastos::Droid::Widget::ICheckedTextView;
using Elastos::Droid::Widget::EIID_IPopupWindow;
using Elastos::Droid::Widget::IAbsoluteLayoutLayoutParams;
using Elastos::Droid::Widget::IToast;
using Elastos::Droid::Widget::IToastHelper;
using Elastos::Droid::Widget::CToastHelper;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//             _CWebViewClassicWebViewInputConnection
//===============================================================
_CWebViewClassicWebViewInputConnection::_CWebViewClassicWebViewInputConnection(
    /* [in] */ CWebViewClassic* owner)
    : BaseInputConnection(owner->mWebView, TRUE)
    , mIsKeySentByMe(FALSE)
    , mInputType(0)
    , mImeOptions(0)
    , mMaxLength(0)
    , mIsAutoFillable(FALSE)
    , mIsAutoCompleteEnabled(FALSE)
    , mBatchLevel(0)
    , mOwner(owner)
{}

PInterface _CWebViewClassicWebViewInputConnection::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_BaseInputConnection) {
        return reinterpret_cast<PInterface>((BaseInputConnection*)this);
    }

    return NULL;
}


void _CWebViewClassicWebViewInputConnection::SetAutoFillable(
    /* [in] */ Int32 queryId)
{
    AutoPtr<IWebSettings> settings;
    mOwner->GetSettings((IWebSettings**)&settings);
    IWebSettingsClassic* classic =IWebSettingsClassic::Probe(settings);
    assert(classic != NULL);
    Boolean enabled;
    classic->GetAutoFillEnabled(&enabled);
    mIsAutoFillable = enabled
            && (queryId != WebTextView::FORM_NOT_AUTOFILLABLE);
    Int32 variation = mInputType & IInputType::TYPE_MASK_VARIATION;
    if (variation != IInputType::TYPE_TEXT_VARIATION_WEB_PASSWORD
            && (mIsAutoFillable || mIsAutoCompleteEnabled)) {
        if (!mName.IsNullOrEmpty()) {
            mOwner->RequestFormData(mName, mOwner->mFieldPointer, mIsAutoFillable,
                    mIsAutoCompleteEnabled);
        }
    }
}

Boolean _CWebViewClassicWebViewInputConnection::BeginBatchEdit()
{
    if (mBatchLevel == 0) {
        mOwner->BeginTextBatch();
    }
    mBatchLevel++;
    return FALSE;
}

Boolean _CWebViewClassicWebViewInputConnection::EndBatchEdit()
{
    mBatchLevel--;
    if (mBatchLevel == 0) {
        mOwner->CommitTextBatch();
    }
    return FALSE;
}

Boolean _CWebViewClassicWebViewInputConnection::GetIsAutoFillable()
{
    return mIsAutoFillable;
}

Boolean _CWebViewClassicWebViewInputConnection::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Some IMEs send key events directly using sendKeyEvents.
    // WebViewInputConnection should treat these as text changes.
    if (!mIsKeySentByMe) {
        Int32 action, keyCode, unicodeChar;
        event->GetAction(&action);
        event->GetKeyCode(&keyCode);
        if (action == IKeyEvent::ACTION_UP) {
            if (keyCode == IKeyEvent::KEYCODE_DEL) {
                return DeleteSurroundingText(1, 0);
            }
            else if (keyCode == IKeyEvent::KEYCODE_FORWARD_DEL) {
                return DeleteSurroundingText(0, 1);
            }
            else if ((event->GetUnicodeChar(&unicodeChar), unicodeChar != 0)){
                String newComposingText("");
                newComposingText.Append((Char32)unicodeChar);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(newComposingText, (ICharSequence**)&cs);
                return CommitText(cs, 1);
            }
        }
        else if (action == IKeyEvent::ACTION_DOWN &&
                 (  keyCode == IKeyEvent::KEYCODE_DEL
                    || keyCode == IKeyEvent::KEYCODE_FORWARD_DEL
                    || (event->GetUnicodeChar(&unicodeChar), unicodeChar != 0)
                 )
                ) {
            return TRUE; // only act on action_down
        }
    }
    return BaseInputConnection::SendKeyEvent(event);
}

void _CWebViewClassicWebViewInputConnection::SetTextAndKeepSelection(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IEditable> editable = GetEditable();
    Int32 selectionStart = Selection::GetSelectionStart(editable);
    Int32 selectionEnd = Selection::GetSelectionEnd(editable);
    Int32 length;
    editable->GetLength(&length);
    text = LimitReplaceTextByMaxLength(text, length);
    editable->Replace(0, length, text);
    RestartInput();
    // Keep the previous selection.
    editable->GetLength(&length);
    selectionStart = Elastos::Core::Math::Min(selectionStart, length);
    selectionEnd = Elastos::Core::Math::Min(selectionEnd, length);
    SetSelection(selectionStart, selectionEnd);
    FinishComposingText();
}

void _CWebViewClassicWebViewInputConnection::ReplaceSelection(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IEditable> editable = GetEditable();
    Int32 selectionStart = Selection::GetSelectionStart(editable);
    Int32 selectionEnd = Selection::GetSelectionEnd(editable);
    text = LimitReplaceTextByMaxLength(text, selectionEnd - selectionStart);
    SetNewText(selectionStart, selectionEnd, text);
    editable->Replace(selectionStart, selectionEnd, text);
    RestartInput();
    // Move caret to the end of the new text
    Int32 length;
    text->GetLength(&length);
    Int32 newCaret = selectionStart + length;
    SetSelection(newCaret, newCaret);
}

Boolean _CWebViewClassicWebViewInputConnection::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    AutoPtr<IEditable> editable = GetEditable();
    Int32 start = GetComposingSpanStart(editable);
    Int32 end = GetComposingSpanEnd(editable);
    if (start < 0 || end < 0) {
        start = Selection::GetSelectionStart(editable);
        end = Selection::GetSelectionEnd(editable);
    }
    if (end < start) {
        Int32 temp = end;
        end = start;
        start = temp;
    }
    AutoPtr<ICharSequence> limitedText = LimitReplaceTextByMaxLength(text, end - start);
    SetNewText(start, end, limitedText);
    if (limitedText.Get() != text) {
        Int32 l1, l2;
        text->GetLength(&l1);
        limitedText->GetLength(&l2);
        newCursorPosition -= l1 - l2;
    }
    BaseInputConnection::SetComposingText(limitedText, newCursorPosition);
    UpdateSelection();
    if (limitedText.Get() != text) {
        Int32 length;
        limitedText->GetLength(&length);
        Int32 lastCaret = start + length;
        FinishComposingText();
        SetSelection(lastCaret, lastCaret);
    }
    return TRUE;
}

Boolean _CWebViewClassicWebViewInputConnection::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    SetComposingText(text, newCursorPosition);
    FinishComposingText();
    return TRUE;
}

Boolean _CWebViewClassicWebViewInputConnection::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength)
{
    // This code is from BaseInputConnection#deleteSurroundText.
    // We have to delete the same text in webkit.
    AutoPtr<IEditable> content = GetEditable();
    Int32 a = Selection::GetSelectionStart(content);
    Int32 b = Selection::GetSelectionEnd(content);

    if (a > b) {
        Int32 tmp = a;
        a = b;
        b = tmp;
    }

    Int32 ca = GetComposingSpanStart(content);
    Int32 cb = GetComposingSpanEnd(content);
    if (cb < ca) {
        Int32 tmp = ca;
        ca = cb;
        cb = tmp;
    }
    if (ca != -1 && cb != -1) {
        if (ca < a) a = ca;
        if (cb > b) b = cb;
    }

    Int32 length;
    content->GetLength(&length);
    Int32 endDelete = Elastos::Core::Math::Min(length, b + rightLength);
    if (endDelete > b) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(""), (ICharSequence**)&cs);
        SetNewText(b, endDelete, cs);
    }
    Int32 startDelete = Elastos::Core::Math::Max(0, a - leftLength);
    if (startDelete < a) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(""), (ICharSequence**)&cs);
        SetNewText(startDelete, a, cs);
    }
    return BaseInputConnection::DeleteSurroundingText(leftLength, rightLength);
}

Boolean _CWebViewClassicWebViewInputConnection::PerformEditorAction(
    /* [in] */ Int32 editorAction)
{
    Boolean handled = TRUE;
//    if (DebugFlags::WEB_VIEW) {
//        Logger::D("CWebViewClassic", "_CWebViewClassicWebViewInputConnection::PerformEditorAction: %d", editorAction);
//    }
    switch (editorAction) {
    case IEditorInfo::IME_ACTION_NEXT: {
        Boolean result;
        mOwner->mWebView->RequestFocus(IView::FOCUS_FORWARD, &result);
        break;
    }
    case IEditorInfo::IME_ACTION_PREVIOUS: {
        Boolean result;
        mOwner->mWebView->RequestFocus(IView::FOCUS_BACKWARD, &result);
        break;
    }
    case IEditorInfo::IME_ACTION_DONE:
        mOwner->HideSoftKeyboard();
        break;
    case IEditorInfo::IME_ACTION_GO:
    case IEditorInfo::IME_ACTION_SEARCH: {
        mOwner->HideSoftKeyboard();
        String text;
        GetEditable()->ToString(&text);
        AutoPtr<IKeyEvent> key;
        CKeyEvent::New(IKeyEvent::ACTION_DOWN,
                IKeyEvent::KEYCODE_ENTER,
                (IKeyEvent**)&key);
        mOwner->PassToJavaScript(text, key);
        key = NULL;
        CKeyEvent::New(IKeyEvent::ACTION_UP,
                IKeyEvent::KEYCODE_ENTER,
                (IKeyEvent**)&key);
        mOwner->PassToJavaScript(text, key);
        break;
    }
    default:
        handled = BaseInputConnection::PerformEditorAction(editorAction);
        break;
    }

    return handled;
}

void _CWebViewClassicWebViewInputConnection::InitEditorInfo(
    /* [in] */ CWebViewCoreTextFieldInitData* initData)
{
    Int32 type = initData->mType;
    Int32 inputType = IInputType::TYPE_CLASS_TEXT
            | IInputType::TYPE_TEXT_VARIATION_WEB_EDIT_TEXT;
    Int32 imeOptions = IEditorInfo::IME_FLAG_NO_EXTRACT_UI
            | IEditorInfo::IME_FLAG_NO_FULLSCREEN;
    if (!initData->mIsSpellCheckEnabled) {
        inputType |= IInputType::TYPE_TEXT_FLAG_NO_SUGGESTIONS;
    }
    if (WebTextView::TEXT_AREA != type) {
        if (initData->mIsTextFieldNext) {
            imeOptions |= IEditorInfo::IME_FLAG_NAVIGATE_NEXT;
        }

        if (initData->mIsTextFieldPrev) {
            imeOptions |= IEditorInfo::IME_FLAG_NAVIGATE_PREVIOUS;
        }
    }
    Int32 action = IEditorInfo::IME_ACTION_GO;
    switch (type) {
        case WebTextView::NORMAL_TEXT_FIELD:
            break;
        case WebTextView::TEXT_AREA:
            inputType |= IInputType::TYPE_TEXT_FLAG_MULTI_LINE
                    | IInputType::TYPE_TEXT_FLAG_CAP_SENTENCES
                    | IInputType::TYPE_TEXT_FLAG_AUTO_CORRECT;
            action = IEditorInfo::IME_ACTION_NONE;
            break;
        case WebTextView::PASSWORD:
            inputType |= IInputType::TYPE_TEXT_VARIATION_WEB_PASSWORD;
            break;
        case WebTextView::SEARCH:
            action = IEditorInfo::IME_ACTION_SEARCH;
            break;
        case WebTextView::EMAIL:
            // inputType needs to be overwritten because of the different text variation.
            inputType = IInputType::TYPE_CLASS_TEXT
                    | IInputType::TYPE_TEXT_VARIATION_WEB_EMAIL_ADDRESS;
            break;
        case WebTextView::NUMBER:
            // inputType needs to be overwritten because of the different class.
            inputType = IInputType::TYPE_CLASS_NUMBER | IInputType::TYPE_NUMBER_VARIATION_NORMAL
                    | IInputType::TYPE_NUMBER_FLAG_SIGNED | IInputType::TYPE_NUMBER_FLAG_DECIMAL;
            // Number and telephone do not have both a Tab key and an
            // action, so set the action to NEXT
            break;
        case WebTextView::TELEPHONE:
            // inputType needs to be overwritten because of the different class.
            inputType = IInputType::TYPE_CLASS_PHONE;
            break;
        case WebTextView::URL:
            // TYPE_TEXT_VARIATION_URI prevents Tab key from showing, so
            // exclude it for now.
            inputType |= IInputType::TYPE_TEXT_VARIATION_URI;
            break;
        default:
            break;
    }
    imeOptions |= action;
    mHint = initData->mLabel;
    mInputType = inputType;
    mImeOptions = imeOptions;
    mMaxLength = initData->mMaxLength;
    mIsAutoCompleteEnabled = initData->mIsAutoCompleteEnabled;
    mName = initData->mName;
    mOwner->mAutoCompletePopup->ClearAdapter();
}

void _CWebViewClassicWebViewInputConnection::SetupEditorInfo(
    /* [in] */ IEditorInfo* outAttrs)
{
    outAttrs->SetInputType(mInputType);
    outAttrs->SetImeOptions(mImeOptions);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(mHint, (ICharSequence**)&cs);
    outAttrs->SetHintText(cs);
    outAttrs->SetInitialCapsMode(GetCursorCapsMode(IInputType::TYPE_CLASS_TEXT));

    AutoPtr<IEditable> editable = GetEditable();
    Int32 selectionStart = Selection::GetSelectionStart(editable);
    Int32 selectionEnd = Selection::GetSelectionEnd(editable);
    if (selectionStart < 0 || selectionEnd < 0) {
        editable->GetLength(&selectionStart);
        selectionEnd = selectionStart;
    }
    outAttrs->SetInitialSelStart(selectionStart);
    outAttrs->SetInitialSelEnd(selectionEnd);
}

Boolean _CWebViewClassicWebViewInputConnection::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Boolean result = BaseInputConnection::SetSelection(start, end);
    UpdateSelection();
    return result;
}

Boolean _CWebViewClassicWebViewInputConnection::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Boolean result = BaseInputConnection::SetComposingRegion(start, end);
    UpdateSelection();
    return result;
}

/**
 * Send the selection and composing spans to the IME.
 */
void _CWebViewClassicWebViewInputConnection::UpdateSelection()
{
    AutoPtr<IEditable> editable = GetEditable();
    Int32 selectionStart = Selection::GetSelectionStart(editable);
    Int32 selectionEnd = Selection::GetSelectionEnd(editable);
    Int32 composingStart = GetComposingSpanStart(editable);
    Int32 composingEnd = GetComposingSpanEnd(editable);
    AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
    if (imm != NULL) {
        imm->UpdateSelection(mOwner->mWebView, selectionStart, selectionEnd,
                composingStart, composingEnd);
    }
}

/**
 * Sends a text change to webkit indirectly. If it is a single-
 * character add or delete, it sends it as a key stroke. If it cannot
 * be represented as a key stroke, it sends it as a field change.
 * @param start The start offset (inclusive) of the text being changed.
 * @param end The end offset (exclusive) of the text being changed.
 * @param text The new text to replace the changed text.
 */
void _CWebViewClassicWebViewInputConnection::SetNewText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* text)
{
    mIsKeySentByMe = TRUE;
    AutoPtr<IEditable> editable = GetEditable();
    AutoPtr<ICharSequence> original;
    editable->SubSequence(start, end, (ICharSequence**)&original);
    Boolean isCharacterAdd = FALSE;
    Boolean isCharacterDelete = FALSE;
    Int32 textLength;
    text->GetLength(&textLength);
    Int32 originalLength;
    original->GetLength(&originalLength);
    Int32 selectionStart = Selection::GetSelectionStart(editable);
    Int32 selectionEnd = Selection::GetSelectionEnd(editable);
    if (selectionStart == selectionEnd) {
        if (textLength > originalLength) {
            isCharacterAdd = (textLength == originalLength + 1)
                    && TextUtils::RegionMatches(text, 0, original, 0,
                            originalLength);
        }
        else if (originalLength > textLength) {
            isCharacterDelete = (textLength == originalLength - 1)
                    && TextUtils::RegionMatches(text, 0, original, 0,
                            textLength);
        }
    }
    if (isCharacterAdd) {
        Char32 ch;
        text->GetCharAt(textLength - 1, &ch);
        SendCharacter(ch);
    }
    else if (isCharacterDelete) {
        SendKey(IKeyEvent::KEYCODE_DEL);
    }
    else if ((textLength != originalLength) ||
            !TextUtils::RegionMatches(text, 0, original, 0,
                    textLength)) {
        // Send a message so that key strokes and text replacement
        // do not come out of order.
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mOwner->mPrivateHandler, CWebViewClassic::REPLACE_TEXT, start, end, text, (IMessage**)&msg);
        msg->SendToTarget();
    }
    if (mOwner->mAutoCompletePopup != NULL) {
        StringBuilder newText;
        AutoPtr<ICharSequence> cs;
        editable->SubSequence(0, start, (ICharSequence**)&cs);
        newText.AppendCharSequence(cs);
        newText.AppendCharSequence(text);
        Int32 length;
        editable->GetLength(&length);
        cs = NULL;
        editable->SubSequence(end, length, (ICharSequence**)&cs);
        newText.AppendCharSequence(cs);
        cs = newText.ToCharSequence();
        mOwner->mAutoCompletePopup->SetText(cs);
    }
    mIsKeySentByMe = FALSE;
}

/**
 * Send a single character to the WebView as a key down and up event.
 * @param c The character to be sent.
 */
void _CWebViewClassicWebViewInputConnection::SendCharacter(
    /* [in] */ Char32 c)
{
    if (mKeyCharacterMap == NULL) {
        AutoPtr<IKeyCharacterMapHelper> help;
        CKeyCharacterMapHelper::AcquireSingleton((IKeyCharacterMapHelper**)&help);
        help->Load(IKeyCharacterMap::VIRTUAL_KEYBOARD,
            (IKeyCharacterMap**)&mKeyCharacterMap);
    }
    AutoPtr< ArrayOf<Char32> > chars = ArrayOf<Char32>::Alloc(1);
    (*chars)[0] = c;
    AutoPtr< ArrayOf<IKeyEvent*> > events;
    mKeyCharacterMap->GetEvents(chars, (ArrayOf<IKeyEvent*>**)&events);
    if (events != NULL) {
        for (Int32 i = 0; i < events->GetLength(); i++) {
            SendKeyEvent((*events)[i]);
        }
    }
    else {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> msg;
        helper->Obtain(mOwner->mPrivateHandler, CWebViewClassic::KEY_PRESS, (Int32)c, 0, (IMessage**)&msg);
        msg->SendToTarget();
    }
}

/**
 * Send a key event for a specific key code, not a standard
 * unicode character.
 * @param keyCode The key code to send.
 */
void _CWebViewClassicWebViewInputConnection::SendKey(
    /* [in] */ Int32 keyCode)
{
    Int64 eventTime = SystemClock::GetUptimeMillis();
    AutoPtr<IKeyEvent> key;
    CKeyEvent::New(eventTime, eventTime,
            IKeyEvent::ACTION_DOWN, keyCode, 0, 0,
            IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
            IKeyEvent::FLAG_SOFT_KEYBOARD,
            (IKeyEvent**)&key);
    SendKeyEvent(key);
    key = NULL;
    CKeyEvent::New(SystemClock::GetUptimeMillis(), eventTime,
            IKeyEvent::ACTION_UP, keyCode, 0, 0,
            IKeyCharacterMap::VIRTUAL_KEYBOARD, 0,
            IKeyEvent::FLAG_SOFT_KEYBOARD,
            (IKeyEvent**)&key);
    SendKeyEvent(key);
}

AutoPtr<ICharSequence> _CWebViewClassicWebViewInputConnection::LimitReplaceTextByMaxLength(
    /* [in] */ ICharSequence* _text,
    /* [in] */ Int32 numReplaced)
{
    AutoPtr<ICharSequence> text = _text;
    if (mMaxLength > 0) {
        AutoPtr<IEditable> editable = GetEditable();
        Int32 length;
        editable->GetLength(&length);
        Int32 maxReplace = mMaxLength - length + numReplaced;
        Int32 l;
        _text->GetLength(&l);
        if (maxReplace < l) {
            maxReplace = Elastos::Core::Math::Max(maxReplace, 0);
            // New length is greater than the maximum. trim it down.
            text = NULL;
            _text->SubSequence(0, maxReplace, (ICharSequence**)&text);
        }
    }
    return text;
}

void _CWebViewClassicWebViewInputConnection::RestartInput()
{
    AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
    if (imm != NULL) {
        // Since the text has changed, do not allow the IME to replace the
        // existing text as though it were a completion.
        imm->RestartInput(mOwner->mWebView);
    }
}


//===============================================================
//                   CWebViewClassic::CWebViewClassicWebViewInputConnection
//===============================================================
CWebViewClassicWebViewInputConnection::CWebViewClassicWebViewInputConnection(
    /* [in] */ CWebViewClassic* owner)
    : _CWebViewClassicWebViewInputConnection(owner)
{}

PInterface CWebViewClassicWebViewInputConnection::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IBaseInputConnection *)this;
    }
    else if (riid == EIID_IBaseInputConnection) {
        return (IBaseInputConnection *)this;
    }
    else if (riid == EIID_IInputConnection) {
        return (IInputConnection*)(IBaseInputConnection *)this;
    }
    else if (riid == EIID_BaseInputConnection) {
        return (PInterface)(BaseInputConnection*)(_CWebViewClassicWebViewInputConnection*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource *)this;
    }
    return NULL;
}

UInt32 CWebViewClassicWebViewInputConnection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CWebViewClassicWebViewInputConnection::Release()
{
    return ElRefBase::Release();
}

ECode CWebViewClassicWebViewInputConnection::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IBaseInputConnection *)this) {
        *iid = EIID_IBaseInputConnection;
    }
    else if (object == (IInterface*)(IWeakReferenceSource *)this) {
        *iid = EIID_IWeakReferenceSource;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

BASEINPUTCONNECTION_METHODS_IMPL(CWebViewClassicWebViewInputConnection, _CWebViewClassicWebViewInputConnection, _CWebViewClassicWebViewInputConnection);

ECode CWebViewClassicWebViewInputConnection::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::PastePopupWindow
//===============================================================
CWebViewClassic::PastePopupWindow::PastePopupWindow(
    /* [in] */ CWebViewClassic* owner)
    : PopupWindow(owner->mContext, NULL, R::attr::textSelectHandleWindowStyle)
    , mOwner(owner)
{
    SetClippingEnabled(TRUE);
    AutoPtr<ILinearLayout> linearLayout;
    CLinearLayout::New(owner->mContext, (ILinearLayout**)&linearLayout);
    linearLayout->SetOrientation(ILinearLayout::HORIZONTAL);
    mContentView = linearLayout;
    mContentView->SetBackgroundResource(
            R::drawable::text_edit_paste_window);

    AutoPtr<IInterface> service;
    owner->mContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&service);
    AutoPtr<ILayoutInflater> inflater = ILayoutInflater::Probe(service);

    AutoPtr<IViewGroupLayoutParams> wrapContent;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT, (IViewGroupLayoutParams**)&wrapContent);

    AutoPtr<IView> v;
    inflater->Inflate(R::layout::text_edit_action_popup_text, NULL, (IView**)&v);
    mPasteTextView = ITextView::Probe(v);
    mPasteTextView->SetLayoutParams(wrapContent);
    mContentView->AddView(mPasteTextView);
    mPasteTextView->SetText(R::string::paste);
    mPasteTextView->SetOnClickListener(this);
    SetContentView(mContentView);
}

CAR_INTERFACE_IMPL_LIGHT_2(CWebViewClassic::PastePopupWindow, IPopupWindow, IViewOnClickListener);

void CWebViewClassic::PastePopupWindow::Show(
    /* [in] */ IPoint* cursorBottom,
    /* [in] */ IPoint* cursorTop,
    /* [in] */ Int32 windowLeft,
    /* [in] */ Int32 windowTop)
{
    MeasureContent();

    Int32 width, height;
    mContentView->GetMeasuredWidth(&width);
    mContentView->GetMeasuredHeight(&height);
    Int32 cursorTopX, cursorTopY;
    cursorTop->GetX(&cursorTopX);
    cursorTop->GetY(&cursorTopY);
    Int32 y = cursorTopY - height;
    Int32 x = cursorTopX - (width / 2);
    if (y < windowTop) {
        // There's not enough room vertically, move it below the
        // handle.
        mOwner->EnsureSelectionHandles();
        Int32 cursorBottomX, cursorBottomY;
        cursorBottom->GetX(&cursorBottomX);
        cursorBottom->GetY(&cursorBottomY);
        Int32 h;
        mOwner->mSelectHandleCenter->GetIntrinsicHeight(&h);
        y = cursorBottomY + h;
        x = cursorBottomX - (width / 2);
    }
    if (x < windowLeft) {
        x = windowLeft;
    }
    Boolean isShowing;
    if (IsShowing(&isShowing), !isShowing) {
        ShowAtLocation(mOwner->mWebView, IGravity::NO_GRAVITY, x, y);
    }
    Update(x, y, width, height);
}

void CWebViewClassic::PastePopupWindow::Hide()
{
    Dismiss();
}

ECode CWebViewClassic::PastePopupWindow::OnClick(
    /* [in] */ IView* view)
{
    mOwner->PasteFromClipboard();
    mOwner->SelectionDone();
    return NOERROR;
}

void CWebViewClassic::PastePopupWindow::MeasureContent()
{
    AutoPtr<IResources> res;
    mOwner->mContext->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> displayMetrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    Int32 widthPixels, heightPixels;
    displayMetrics->GetWidthPixels(&widthPixels);
    displayMetrics->GetHeightPixels(&heightPixels);
    mContentView->Measure(
            Elastos::Droid::View::View::MeasureSpec::MakeMeasureSpec(widthPixels,
                    Elastos::Droid::View::View::MeasureSpec::AT_MOST),
            Elastos::Droid::View::View::MeasureSpec::MakeMeasureSpec(heightPixels,
                    Elastos::Droid::View::View::MeasureSpec::AT_MOST));
}

ECode CWebViewClassic::PastePopupWindow::GetBackground(
    /* [out] */ IDrawable** background)
{
    VALIDATE_NOT_NULL(background);
    AutoPtr<IDrawable> bk = PopupWindow::GetBackground();
    *background = bk.Get();
    REFCOUNT_ADD(*background);
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    return PopupWindow::SetBackgroundDrawable(background);
}

ECode CWebViewClassic::PastePopupWindow::GetAnimationStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    *style = PopupWindow::GetAnimationStyle();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetIgnoreCheekPress()
{
    return PopupWindow::SetIgnoreCheekPress();
}

ECode CWebViewClassic::PastePopupWindow::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return PopupWindow::SetAnimationStyle(animationStyle);
}

ECode CWebViewClassic::PastePopupWindow::GetContentView(
    /* [out] */ IView** contentView)
{
    VALIDATE_NOT_NULL(contentView);
    AutoPtr<IView> cv = PopupWindow::GetContentView();
    *contentView = cv.Get();
    REFCOUNT_ADD(*contentView);
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetContentView(
    /* [in] */ IView* contentView)
{
    return PopupWindow::SetContentView(contentView);
}

ECode CWebViewClassic::PastePopupWindow::SetTouchInterceptor(
    /* [in] */ IViewOnTouchListener* l)
{
    return PopupWindow::SetTouchInterceptor(l);
}

ECode CWebViewClassic::PastePopupWindow::IsFocusable(
    /* [out] */ Boolean* isFocusable)
{
    VALIDATE_NOT_NULL(isFocusable);
    *isFocusable = PopupWindow::IsFocusable();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetFocusable(
     /* [in] */ Boolean focusable)
{
    return PopupWindow::SetFocusable(focusable);
}

ECode CWebViewClassic::PastePopupWindow::GetInputMethodMode(
    /* [out] */ Int32* inputMethodMode)
{
    VALIDATE_NOT_NULL(inputMethodMode);
    *inputMethodMode = PopupWindow::GetInputMethodMode();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    return PopupWindow::SetInputMethodMode(mode);
}

ECode CWebViewClassic::PastePopupWindow::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    return PopupWindow::SetSoftInputMode(mode);
}

ECode CWebViewClassic::PastePopupWindow::GetSoftInputMode(
    /* [out] */ Int32* softInputMode)
{
    VALIDATE_NOT_NULL(softInputMode);
    *softInputMode = PopupWindow::GetSoftInputMode();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::IsTouchable(
    /* [out] */ Boolean* touchable)
{
    VALIDATE_NOT_NULL(touchable);
    *touchable = PopupWindow::IsTouchable();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetTouchable(
    /* [in] */ Boolean touchable)
{
    return PopupWindow::SetTouchable(touchable);
}

ECode CWebViewClassic::PastePopupWindow::IsOutsideTouchable(
    /* [out] */ Boolean* touchable)
{
    VALIDATE_NOT_NULL(touchable);
    *touchable = PopupWindow::IsOutsideTouchable();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetOutsideTouchable(
    /* [in] */ Boolean touchable)
{
    return PopupWindow::SetOutsideTouchable(touchable);
}

ECode CWebViewClassic::PastePopupWindow::IsClippingEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = PopupWindow::IsClippingEnabled();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetClippingEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetClippingEnabled(enabled);
}

ECode CWebViewClassic::PastePopupWindow::SetClipToScreenEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetClipToScreenEnabled(enabled);
}

ECode CWebViewClassic::PastePopupWindow::IsSplitTouchEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = PopupWindow::IsSplitTouchEnabled();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetSplitTouchEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetSplitTouchEnabled(enabled);
}

ECode CWebViewClassic::PastePopupWindow::IsLayoutInScreenEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = PopupWindow::IsLayoutInScreenEnabled();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetLayoutInScreenEnabled(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetLayoutInScreenEnabled(enabled);
}

ECode CWebViewClassic::PastePopupWindow::SetLayoutInsetDecor(
    /* [in] */ Boolean enabled)
{
    return PopupWindow::SetLayoutInsetDecor(enabled);
}

ECode CWebViewClassic::PastePopupWindow::SetWindowLayoutType(
    /* [in] */ Int32 layoutType)
{
    return PopupWindow::SetWindowLayoutType(layoutType);
}

ECode CWebViewClassic::PastePopupWindow::GetWindowLayoutType(
    /* [out] */ Int32* layoutType)
{
    VALIDATE_NOT_NULL(layoutType);
    *layoutType = PopupWindow::GetWindowLayoutType();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetTouchModal(
    /* [in] */ Boolean touchModal)
{
    return PopupWindow::SetTouchModal(touchModal);
}

ECode CWebViewClassic::PastePopupWindow::SetWindowLayoutMode(
    /* [in] */ Int32 widthSpec,
    /* [in] */ Int32 heightSpec)
{
    return PopupWindow::SetWindowLayoutMode(widthSpec, heightSpec);
}

ECode CWebViewClassic::PastePopupWindow::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = PopupWindow::GetHeight();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetHeight(
    /* [in] */ Int32 height)
{
    return PopupWindow::SetHeight(height);
}

ECode CWebViewClassic::PastePopupWindow::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = PopupWindow::GetWidth();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::SetWidth(
    /* [in] */ Int32 width)
{
    return PopupWindow::SetWidth(width);
}

ECode CWebViewClassic::PastePopupWindow::IsShowing(
    /* [out] */ Boolean* isShowing)
{
    VALIDATE_NOT_NULL(isShowing);
    *isShowing = PopupWindow::IsShowing();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::ShowAtLocation(
    /* [in] */ IView* parent,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return PopupWindow::ShowAtLocation(parent, gravity, x, y);
}

ECode CWebViewClassic::PastePopupWindow::ShowAtLocation(
    /* [in] */ IBinder* token,
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return PopupWindow::ShowAtLocation(token, gravity, x, y);
}

ECode CWebViewClassic::PastePopupWindow::ShowAsDropDown(
    /* [in] */ IView* anchor)
{
    return PopupWindow::ShowAsDropDown(anchor);
}

ECode CWebViewClassic::PastePopupWindow::ShowAsDropDown(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff)
{
    return PopupWindow::ShowAsDropDown(anchor, xoff, yoff);
}

ECode CWebViewClassic::PastePopupWindow::IsAboveAnchor(
    /* [out] */ Boolean* isAboveAnchor)
{
    VALIDATE_NOT_NULL(isAboveAnchor);
    *isAboveAnchor = PopupWindow::IsAboveAnchor();
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [out] */ Int32* maxAvailableHeight)
{
    VALIDATE_NOT_NULL(maxAvailableHeight);
    *maxAvailableHeight = PopupWindow::GetMaxAvailableHeight(anchor);
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [out] */ Int32* maxAvailableHeight)
{
    VALIDATE_NOT_NULL(maxAvailableHeight);
    *maxAvailableHeight = PopupWindow::GetMaxAvailableHeight(anchor, yOffset);
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::GetMaxAvailableHeight(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 yOffset,
    /* [in] */ Boolean ignoreBottomDecorations,
    /* [out] */ Int32* maxAvailableHeight)
{
    VALIDATE_NOT_NULL(maxAvailableHeight);
    *maxAvailableHeight = PopupWindow::GetMaxAvailableHeight(anchor, yOffset, ignoreBottomDecorations);
    return NOERROR;
}

ECode CWebViewClassic::PastePopupWindow::Dismiss()
{
    return PopupWindow::Dismiss();
}

ECode CWebViewClassic::PastePopupWindow::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* l)
{
    return PopupWindow::SetOnDismissListener(l);
}

ECode CWebViewClassic::PastePopupWindow::Update()
{
    return PopupWindow::Update();
}

ECode CWebViewClassic::PastePopupWindow::Update(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(width, height);
}

ECode CWebViewClassic::PastePopupWindow::Update(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(x, y, width, height);
}

ECode CWebViewClassic::PastePopupWindow::Update(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean force)
{
    return PopupWindow::Update(x, y, width, height, force);
}

ECode CWebViewClassic::PastePopupWindow::Update(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(anchor, width, height);
}

ECode CWebViewClassic::PastePopupWindow::Update(
    /* [in] */ IView* anchor,
    /* [in] */ Int32 xoff,
    /* [in] */ Int32 yoff,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return PopupWindow::Update(anchor, xoff, yoff, width, height);
}

ECode CWebViewClassic::PastePopupWindow::SetAllowScrollingAnchorParent(
        /* [in] */ Boolean enabled)
{
    return PopupWindow::SetAllowScrollingAnchorParent(enabled);
}


//===============================================================
//                   CWebViewClassic::OnTrimMemoryListener
//===============================================================
AutoPtr<CWebViewClassic::OnTrimMemoryListener> CWebViewClassic::OnTrimMemoryListener::sInstance;

CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::OnTrimMemoryListener, IComponentCallbacks2);

void CWebViewClassic::OnTrimMemoryListener::Init(
    /* [in] */ IContext* c)
{
    if (sInstance == NULL) {
        AutoPtr<IContext> con;
        c->GetApplicationContext((IContext**)&con);
        sInstance = new OnTrimMemoryListener(con);
    }
}

CWebViewClassic::OnTrimMemoryListener::OnTrimMemoryListener(
    /* [in] */ IContext* c)
{
    c->RegisterComponentCallbacks(this);
}

ECode CWebViewClassic::OnTrimMemoryListener::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    // Ignore
    return NOERROR;
}

ECode CWebViewClassic::OnTrimMemoryListener::OnLowMemory()
{
    // Ignore
    return NOERROR;
}

ECode CWebViewClassic::OnTrimMemoryListener::OnTrimMemory(
    /* [in] */ Int32 level)
{
    if (DebugFlags::WEB_VIEW) {
        Logger::D("WebView", "onTrimMemory: %d", level);
    }

    // When framework reset EGL context during high memory pressure, all
    // the existing GL resources for the html5 video will be destroyed
    // at native side.
    // Here we just need to clean up the Surface Texture which is static.
    if (level > TRIM_MEMORY_UI_HIDDEN) {
        HTML5VideoInline::CleanupSurfaceTexture();
        HTML5VideoView::ReleaseResources();
    }
    CWebViewClassic::NativeOnTrimMemory(level);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::FocusNodeHref
//===============================================================
const String CWebViewClassic::FocusNodeHref::TITLE("title");
const String CWebViewClassic::FocusNodeHref::URL("url");
const String CWebViewClassic::FocusNodeHref::SRC("src");


//===============================================================
//                   CWebViewClassic::Factory
//===============================================================
PInterface CWebViewClassic::Factory::Probe(
    /* [in] */ REIID riid)
{
    assert(0);
    return NULL;
}

UInt32 CWebViewClassic::Factory::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CWebViewClassic::Factory::Release()
{
    return ElLightRefBase::Release();
}

ECode CWebViewClassic::Factory::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return NOERROR;
}

ECode CWebViewClassic::Factory::FindAddress(
    /* [in] */ const String& addr,
    /* [out] */ String* addrOut)
{
    VALIDATE_NOT_NULL(addrOut);
    *addrOut = CWebViewClassic::FindAddress(addr);
    return NOERROR;
}

ECode CWebViewClassic::Factory::SetPlatformNotificationsEnabled(
    /* [in] */ Boolean enable)
{
    if (enable) {
        CWebViewClassic::EnablePlatformNotifications();
    }
    else {
        CWebViewClassic::DisablePlatformNotifications();
    }
    return NOERROR;
}

ECode CWebViewClassic::Factory::GetStatics(
    /* [out] */ IWebViewFactoryProvider::IStatics** statics)
{
    VALIDATE_NOT_NULL(statics);
    *statics = this;
    REFCOUNT_ADD(*statics);
    return NOERROR;
}

ECode CWebViewClassic::Factory::CreateWebView(
    /* [in] */ IWebView* webView,
    /* [in] */ IWebViewPrivateAccess* privateAccess,
    /* [out] */ IWebViewProvider** provider)
{
    VALIDATE_NOT_NULL(provider);
    return CWebViewClassic::New(webView, privateAccess, provider);
}

ECode CWebViewClassic::Factory::GetGeolocationPermissions(
    /* [out] */ IGeolocationPermissions** gp)
{
    VALIDATE_NOT_NULL(gp);
    AutoPtr<IGeolocationPermissions> instance = GeolocationPermissionsClassic::GetInstance().Get();
    *gp = instance;
    REFCOUNT_ADD(*gp);
    return NOERROR;
}

ECode CWebViewClassic::Factory::GetCookieManager(
    /* [out] */ ICookieManager** cm)
{
    VALIDATE_NOT_NULL(cm);
    AutoPtr<ICookieManager> instance = CookieManagerClassic::GetInstance().Get();
    *cm = instance;
    REFCOUNT_ADD(*cm);
    return NOERROR;
}

ECode CWebViewClassic::Factory::GetWebIconDatabase(
    /* [out] */ IWebIconDatabase** database)
{
    VALIDATE_NOT_NULL(database);
    AutoPtr<IWebIconDatabase> instance = WebIconDatabaseClassic::GetInstance().Get();
    *database = instance;
    REFCOUNT_ADD(*database);
    return NOERROR;
}

ECode CWebViewClassic::Factory::GetWebStorage(
    /* [out] */ IWebStorage** webStorage)
{
    VALIDATE_NOT_NULL(webStorage);
    AutoPtr<IWebStorage> ws = WebStorageClassic::GetInstance().Get();
    *webStorage = ws;
    REFCOUNT_ADD(*webStorage);
    return NOERROR;
}

ECode CWebViewClassic::Factory::GetWebViewDatabase(
    /* [in] */ IContext* context,
    /* [out] */ IWebViewDatabase** database)
{
    VALIDATE_NOT_NULL(database);
    AutoPtr<IWebViewDatabase> db = WebViewDatabaseClassic::GetInstance(context).Get();
    *database = db;
    REFCOUNT_ADD(*database);
    return NOERROR;
}

ECode CWebViewClassic::Factory::GetDefaultUserAgent(
    /* [in] */ IContext* context,
    /* [out] */ String* userAgent)
{
    VALIDATE_NOT_NULL(userAgent);
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    AutoPtr<ILocale> locale;
    helper->GetDefault((ILocale**)&locale);
    *userAgent = WebSettingsClassic::GetDefaultUserAgentForLocale(context, locale);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::TrustStorageListener
//===============================================================
ECode CWebViewClassic::TrustStorageListener::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IKeyChain::ACTION_STORAGE_CHANGED)) {
        HandleCertTrustChanged();
    }
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::ProxyReceiver
//===============================================================
ECode CWebViewClassic::ProxyReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(Elastos::Droid::Net::IProxy::PROXY_CHANGE_ACTION)) {
        HandleProxyBroadcast(intent);
    }
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::PackageListener
//===============================================================
ECode CWebViewClassic::PackageListener::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    AutoPtr<IUri> data;
    intent->GetData((IUri**)&data);
    String packageName;
    data->GetSchemeSpecificPart(&packageName);
    Boolean replacing;
    intent->GetBooleanExtra(IIntent::EXTRA_REPLACING, FALSE, &replacing);
    if (IIntent::ACTION_PACKAGE_REMOVED.Equals(action) && replacing) {
        // if it is replacing, refreshPlugins() when adding
        return NOERROR;
    }

    // TODO
    assert(0);
//    if (sGoogleApps.contains(packageName)) {
//        if (Intent.ACTION_PACKAGE_ADDED.equals(action)) {
//            WebViewCore.sendStaticMessage(EventHub.ADD_PACKAGE_NAME, packageName);
//        } else {
//            WebViewCore.sendStaticMessage(EventHub.REMOVE_PACKAGE_NAME, packageName);
//        }
//    }

    AutoPtr<IPluginManagerHelper> pmh;
    CPluginManagerHelper::AcquireSingleton((IPluginManagerHelper**)&pmh);
    AutoPtr<CPluginManager> pm;
    pmh->GetInstance(context, (IPluginManager**)&pm);
    if (pm->ContainsPluginPermissionAndSignatures(packageName)) {
        pm->RefreshPlugins(IIntent::ACTION_PACKAGE_ADDED.Equals(action));
    }
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::DestroyNativeRunnable
//===============================================================
ECode CWebViewClassic::DestroyNativeRunnable::Run()
{
    // nativeDestroy also does a stopGL()
    CWebViewClassic::NativeDestroy(mNativePtr);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::SaveWebArchiveMessage
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::SaveWebArchiveMessage, IInterface)


//===============================================================
//                   CWebViewClassic::ViewSizeData
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::ViewSizeData, IInterface)


//===============================================================
//                   CWebViewClassic::SelectionHandleAlpha
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::SelectionHandleAlpha, IInterface)

void CWebViewClassic::SelectionHandleAlpha::SetAlpha(
    /* [in] */ Int32 alpha)
{
    mAlpha = alpha;
    // TODO: Use partial invalidate
    mHost->Invalidate();
}

Int32 CWebViewClassic::SelectionHandleAlpha::GetAlpha()
{
    return mAlpha;
}

void CWebViewClassic::SelectionHandleAlpha::SetTargetAlpha(
    /* [in] */ Int32 alpha)
{
    mTargetAlpha = alpha;
}

Int32 CWebViewClassic::SelectionHandleAlpha::GetTargetAlpha()
{
    return mTargetAlpha;
}


//===============================================================
//                   CWebViewClassic::_RequestFormData
//===============================================================
CWebViewClassic::_RequestFormData::_RequestFormData(
    /* [in] */ const String& name,
    /* [in] */ const String& url,
    /* [in] */ IMessage* msg,
    /* [in] */ Boolean autoFillable,
    /* [in] */ Boolean autoComplete,
    /* [in] */ CWebViewClassic* host)
{
    mName = name;
    mUrl = WebTextView::UrlForAutoCompleteData(url);
    mUpdateMessage = msg;
    mAutoFillable = autoFillable;
    mAutoComplete = autoComplete;
    AutoPtr<IWebSettings> ws;
    host->GetSettings((IWebSettings**)&ws);
    mWebSettings = (WebSettingsClassic*)ws.Get();
}

ECode CWebViewClassic::_RequestFormData::Run()
{
    AutoPtr<IObjectContainer> pastEntries;
    CObjectContainer::New((IObjectContainer**)&pastEntries);

    if (mAutoFillable) {
        // Note that code inside the adapter click handler in AutoCompletePopup depends
        // on the AutoFill item being at the top of the drop down list. If you change
        // the order, make sure to do it there too!
        AutoPtr<IAutoFillProfile> profile;
        if (mWebSettings != NULL && (mWebSettings->GetAutoFillProfile((IAutoFillProfile**)&profile), profile != NULL)) {
            AutoPtr<IResources> res;
            mHost->mWebView->GetResources((IResources**)&res);
            AutoPtr<ICharSequence> cs;
            res->GetText(R::string::autofill_this_form, (ICharSequence**)&cs);
            String str;
            cs->ToString(&str);
            str += String(" ") + mHost->mAutoFillData->GetPreviewString();
            cs = NULL;
            CStringWrapper::New(str, (ICharSequence**)&cs);
            pastEntries->Add(cs);
            mHost->mAutoCompletePopup->SetIsAutoFillProfileSet(TRUE);
        }
        else {
            // There is no autofill profile set up yet, so add an option that
            // will invite the user to set their profile up.
            AutoPtr<IResources> res;
            mHost->mWebView->GetResources((IResources**)&res);
            AutoPtr<ICharSequence> cs;
            res->GetText(R::string::setup_autofill, (ICharSequence**)&cs);
            pastEntries->Add(cs);
            mHost->mAutoCompletePopup->SetIsAutoFillProfileSet(FALSE);
        }
    }

    if (mAutoComplete) {
        AutoPtr<IArrayList> values = mHost->mDatabase->GetFormData(mUrl, mName);
        Int32 N;
        values->GetSize(&N);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<ICharSequence> cs;
            values->Get(i, (IInterface**)&cs);
            pastEntries->Add(cs);
        }
    }

    Int32 size;
    pastEntries->GetObjectCount(&size);
    if (size > 0) {
        AutoPtr<IArrayAdapter> adapter;
        CArrayAdapter::New(mHost->mContext, R::layout::web_text_view_dropdown,
                pastEntries, (IArrayAdapter**)&adapter);
        mUpdateMessage->SetObj(adapter);
        mUpdateMessage->SendToTarget();
    }
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::PrivateHandler
//===============================================================
PInterface CWebViewClassic::PrivateHandler::Probe(
    /* [in] */ REIID riid)
{
    return HandlerBase::Probe(riid);
}

UInt32 CWebViewClassic::PrivateHandler::AddRef()
{
    return HandlerBase::AddRef();
}

UInt32 CWebViewClassic::PrivateHandler::Release()
{
    return HandlerBase::Release();
}

ECode CWebViewClassic::PrivateHandler::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return HandlerBase::GetInterfaceID(object, iid);
}

ECode CWebViewClassic::PrivateHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    // exclude INVAL_RECT_MSG_ID since it is frequently output
    if (DebugFlags::WEB_VIEW && what != INVAL_RECT_MSG_ID && what != NEW_PICTURE_MSG_ID) {
        if (what >= FIRST_PRIVATE_MSG_ID
                && what <= LAST_PRIVATE_MSG_ID) {
            Logger::V(LOGTAG, "CWebViewClassic::PrivateHandler::HandleMessage:%s",HandlerPrivateDebugString[what
                    - FIRST_PRIVATE_MSG_ID].string());
        }
        else if (what >= FIRST_PACKAGE_MSG_ID
                && what <= LAST_PACKAGE_MSG_ID) {
            Logger::V(LOGTAG, "CWebViewClassic::PrivateHandler::HandleMessage:%s", HandlerPackageDebugString[what
                    - FIRST_PACKAGE_MSG_ID].string());
        }
        else {
            Logger::V(LOGTAG, "CWebViewClassic::PrivateHandler::HandleMessage:%s", StringUtils::Int32ToString(what).string());
        }
    }
    if (mHost->mWebViewCore == NULL) {
        // after WebView's destroy() is called, skip handling messages.
        return NOERROR;
    }
    if (mHost->mBlockWebkitViewMessages
            && what != WEBCORE_INITIALIZED_MSG_ID) {
        // Blocking messages from webkit
        return NOERROR;
    }
    switch (what) {
        case REMEMBER_PASSWORD: {
            AutoPtr<IBundle> data;
            msg->GetData((IBundle**)&data);
            String host, username, password;
            data->GetString(String("host"), &host);
            data->GetString(String("username"), &username);
            data->GetString(String("password"), &password);
            mHost->mDatabase->SetUsernamePassword(
                    host, username, password);
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMessage::Probe(obj)->SendToTarget();
            break;
        }
        case NEVER_REMEMBER_PASSWORD: {
            AutoPtr<IBundle> data;
            msg->GetData((IBundle**)&data);
            String host;
            data->GetString(String("host"), &host);
            mHost->mDatabase->SetUsernamePassword(host, String(NULL), String(NULL));
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IMessage::Probe(obj)->SendToTarget();
            break;
        }
        case SCROLL_SELECT_TEXT: {
            if (mHost->mAutoScrollX == 0 && mHost->mAutoScrollY == 0) {
                mHost->mSentAutoScrollMessage = FALSE;
                break;
            }
            if (mHost->mCurrentScrollingLayerId == 0) {
                mHost->PinScrollBy(mHost->mAutoScrollX, mHost->mAutoScrollY, TRUE, 0);
            }
            else {
                Int32 left, top;
                mHost->mScrollingLayerRect->GetLeft(&left);
                mHost->mScrollingLayerRect->GetTop(&top);
                mHost->ScrollLayerTo(left + mHost->mAutoScrollX,
                        top + mHost->mAutoScrollY);
            }
            Boolean result;
            SendEmptyMessageDelayed(
                    SCROLL_SELECT_TEXT, SELECT_SCROLL_INTERVAL, &result);
            break;
        }
        case SCROLL_TO_MSG_ID: {
            // arg1 = animate, arg2 = onlyIfImeIsShowing
            // obj = Point(x, y)
            Int32 arg2;
            msg->GetArg2(&arg2);
            if (arg2 == 1) {
                // This scroll is intended to bring the textfield into
                // view, but is only necessary if the IME is showing
                AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
                Boolean result;
                if (imm == NULL || (imm->IsAcceptingText(&result), !result)
                        || (imm->IsActive(mHost->mWebView, &result), !result)) {
                    break;
                }
            }
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            Int32 x, y, arg1;
            IPoint::Probe(obj)->GetX(&x);
            IPoint::Probe(obj)->GetY(&y);
            msg->GetArg1(&arg1);
            mHost->ContentScrollTo(x, y, arg1 == 1);
            break;
        }
        case UPDATE_ZOOM_RANGE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreViewState* viewState = (CWebViewCoreViewState*)obj.Get();
            // mScrollX contains the new minPrefWidth
            mHost->mZoomManager->UpdateZoomRange(viewState, mHost->GetViewWidth(), viewState->mScrollX);
            break;
        }
        case UPDATE_ZOOM_DENSITY: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            Float density;
            IFloat::Probe(obj)->GetValue(&density);
            mHost->mZoomManager->UpdateDefaultZoomDensity(density);
            break;
        }
        case NEW_PICTURE_MSG_ID: {
            // called for new content
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreDrawData* draw = (CWebViewCoreDrawData*)obj.Get();
            mHost->SetNewPicture(draw, TRUE);
            break;
        }
        case WEBCORE_INITIALIZED_MSG_ID: {
            // nativeCreate sets mNativeClass to a non-zero value
            String drawableDir = BrowserFrame::GetRawResFilename(
                    BrowserFrame::DRAWABLEDIR, mHost->mContext);
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->NativeCreate(arg1, drawableDir, CActivityManager::IsHighEndGfx());
            if (mHost->mDelaySetPicture != NULL) {
                mHost->SetNewPicture(mHost->mDelaySetPicture, TRUE);
                mHost->mDelaySetPicture = NULL;
            }
            if (mHost->mIsPaused) {
                mHost->NativeSetPauseDrawing(mHost->mNativeClass, TRUE);
            }
            AutoPtr<WebViewInputDispatcher::IWebKitCallbacks> cb = mHost->mWebViewCore->GetInputDispatcherCallbacks();
            mHost->mInputDispatcher = new WebViewInputDispatcher(this, cb);
            break;
        }
        case UPDATE_TEXTFIELD_TEXT_MSG_ID: {
            // Make sure that the textfield is currently focused
            // and representing the same node as the pointer.
            Int32 arg2;
            msg->GetArg2(&arg2);
            if (arg2 == mHost->mTextGeneration) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                String text;
                ICharSequence::Probe(obj)->ToString(&text);
                if (text.IsNull()) {
                    text = "";
                }
                Int32 arg1;
                msg->GetArg1(&arg1);
                if (mHost->mInputConnection != NULL &&
                        mHost->mFieldPointer == arg1) {
                    AutoPtr<ICharSequence> cs;
                    CStringWrapper::New(text, (ICharSequence**)&cs);
                    mHost->mInputConnection->SetTextAndKeepSelection(cs);
                }
            }
            break;
        }
        case UPDATE_TEXT_SELECTION_MSG_ID: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreTextSelectionData* data = (CWebViewCoreTextSelectionData*)obj.Get();
            mHost->UpdateTextSelectionFromMessage(arg1, arg2, data);
            break;
        }
        case TAKE_FOCUS: {
            Int32 direction;
            msg->GetArg1(&direction);
            AutoPtr<IView> focusSearch;
            mHost->mWebView->FocusSearch(direction, (IView**)&focusSearch);
            if (focusSearch != NULL && focusSearch != mHost->mWebView) {
                Boolean result;
                focusSearch->RequestFocus(&result);
            }
            break;
        }
        case CLEAR_TEXT_ENTRY:
            mHost->HideSoftKeyboard();
            break;
        case INVAL_RECT_MSG_ID: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IRect* r = IRect::Probe(obj);
            if (r == NULL) {
                mHost->Invalidate();
            }
            else {
                // we need to scale r from content into view coords,
                // which viewInvalidate() does for us
                Int32 left, top, right, bottom;
                r->Get(&left, &top, &right, &bottom);
                mHost->ViewInvalidate(left, top, right, bottom);
            }
            break;
        }
        case REQUEST_FORM_DATA: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            if (mHost->mFieldPointer == arg1) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                IArrayAdapter* adapter = IArrayAdapter::Probe(obj);
                mHost->mAutoCompletePopup->SetAdapter(adapter);
            }
            break;
        }
        case LONG_PRESS_CENTER: {
            // as this is shared by keydown and trackballdown, reset all
            // the states
            mHost->mGotCenterDown = FALSE;
            mHost->mTrackballDown = FALSE;
            Boolean result;
            mHost->mWebView->PerformLongClick(&result);
            break;
        }
        case WEBCORE_NEED_TOUCH_EVENTS: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->mInputDispatcher->SetWebKitWantsTouchEvents(arg1 != 0);
            break;
        }
        case REQUEST_KEYBOARD: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            if (arg1 == 0) {
                mHost->HideSoftKeyboard();
            }
            else {
                mHost->DisplaySoftKeyboard(FALSE);
            }
            break;
        }
        case DRAG_HELD_MOTIONLESS:
            mHost->mHeldMotionless = MOTIONLESS_TRUE;
            mHost->Invalidate();
            break;

        case SCREEN_ON: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->mWebView->SetKeepScreenOn(arg1 == 1);
            break;
        }
        case EXIT_FULLSCREEN_VIDEO: {
            if (mHost->mHTML5VideoViewProxy != NULL) {
                mHost->mHTML5VideoViewProxy->ExitFullScreenVideo();
            }
            break;
        }
        case SHOW_FULLSCREEN: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IView* view = IView::Probe(obj);
            Int32 orientation, npp;
            msg->GetArg1(&orientation);
            msg->GetArg2(&npp);

            if (mHost->InFullScreenMode()) {
                Logger::W(LOGTAG, "Should not have another full screen.");
                mHost->DismissFullScreenMode();
            }
            mHost->mFullScreenHolder = new PluginFullScreenHolder(mHost, orientation, npp);
            mHost->mFullScreenHolder->SetContentView(view);
            mHost->mFullScreenHolder->Show();
            mHost->Invalidate();
            mHost->AcquireWakeLock();

            break;
        }
        case HIDE_FULLSCREEN:
            mHost->ReleaseWakeLock();
            mHost->DismissFullScreenMode();
            break;

        case SHOW_RECT_MSG_ID: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCore::ShowRectData* data = (CWebViewCore::ShowRectData*)obj.Get();
            Int32 left = mHost->ContentToViewX(data->mLeft);
            Int32 width = mHost->ContentToViewDimension(data->mWidth);
            Int32 maxWidth = mHost->ContentToViewDimension(data->mContentWidth);
            Int32 viewWidth = mHost->GetViewWidth();
            Int32 x = (Int32)(left + data->mXPercentInDoc * width -
                    data->mXPercentInView * viewWidth);
            if (DebugFlags::WEB_VIEW) {
                Logger::V(LOGTAG, "showRectMsg=(left=%d,width=%d,maxWidth=%d,viewWidth=%d,x=%d,xPercentInDoc=%f,xPercentInView=%f)",
                        left, width, maxWidth, viewWidth, x, data->mXPercentInDoc, data->mXPercentInView);
            }
            // use the passing content width to cap x as the current
            // mContentWidth may not be updated yet
            x = Elastos::Core::Math::Max(0,
                    (Elastos::Core::Math::Min(maxWidth, x + viewWidth)) - viewWidth);
            Int32 top = mHost->ContentToViewY(data->mTop);
            Int32 height = mHost->ContentToViewDimension(data->mHeight);
            Int32 maxHeight = mHost->ContentToViewDimension(data->mContentHeight);
            Int32 viewHeight = mHost->GetViewHeight();
            Int32 y = (Int32)(top + data->mYPercentInDoc * height -
                           data->mYPercentInView * viewHeight);
            if (DebugFlags::WEB_VIEW) {
                Logger::V(LOGTAG, "showRectMsg=(top=%d,height=%d,maxHeight=%d,viewHeight=%d,y=%d,yPercentInDoc=%f,yPercentInView=%f)",
                        top, height, maxHeight, viewHeight, y, data->mYPercentInDoc, data->mYPercentInView);
            }
            // use the passing content height to cap y as the current
            // mContentHeight may not be updated yet
            y = Elastos::Core::Math::Max(0,
                    (Elastos::Core::Math::Min(maxHeight, y + viewHeight) - viewHeight));
            // We need to take into account the visible title height
            // when scrolling since y is an absolute view position.
            y = Elastos::Core::Math::Max(0, y - mHost->GetVisibleTitleHeightImpl());
            mHost->mWebView->ScrollTo(x, y);
            break;
        }

        case CENTER_FIT_RECT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            mHost->CenterFitRect(IRect::Probe(obj));
            break;
        }
        case SET_SCROLLBAR_MODES: {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mHost->mHorizontalScrollBarMode = arg1;
            mHost->mVerticalScrollBarMode = arg2;
            break;
        }
        case SELECTION_STRING_CHANGED:
            if (mHost->IsAccessibilityInjectionEnabled()) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                String str;
                ICharSequence::Probe(obj)->ToString(&str);
                mHost->GetAccessibilityInjector()->HandleSelectionChangedIfNecessary(str);
            }
            break;

        case FOCUS_NODE_CHANGED: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->mIsEditingText = (arg1 == mHost->mFieldPointer);
            if (mHost->mAutoCompletePopup != NULL && !mHost->mIsEditingText) {
                mHost->mAutoCompletePopup->ClearAdapter();
            }
            // fall through to HIT_TEST_RESULT
        }
        case HIT_TEST_RESULT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreWebKitHitTest* hit = (CWebViewCoreWebKitHitTest*)obj.Get();
            mHost->mFocusedNode = hit;
            mHost->SetTouchHighlightRects(hit);
            mHost->SetHitTestResult(hit);
            break;
        }
        case SAVE_WEBARCHIVE_FINISHED: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            SaveWebArchiveMessage* saveMessage = (SaveWebArchiveMessage*)obj.Get();
            if (saveMessage->mCallback != NULL) {
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(saveMessage->mResultFile, (ICharSequence**)&cs);
                saveMessage->mCallback->OnReceiveValue(cs);
            }
            break;
        }
        case SET_AUTOFILLABLE: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            mHost->mAutoFillData = (CWebViewCoreAutoFillData*)obj.Get();
            if (mHost->mInputConnection != NULL) {
                mHost->mInputConnection->SetAutoFillable(mHost->mAutoFillData->GetQueryId());
                mHost->mAutoCompletePopup->SetAutoFillQueryId(mHost->mAutoFillData->GetQueryId());
            }
            break;
        }
        case AUTOFILL_COMPLETE: {
            if (mHost->mAutoCompletePopup != NULL) {
                AutoPtr<IObjectContainer> pastEntries;
                CObjectContainer::New((IObjectContainer**)&pastEntries);
                AutoPtr<IArrayAdapter> adapter;
                CArrayAdapter::New(mHost->mContext, R::layout::web_text_view_dropdown,
                        pastEntries, (IArrayAdapter**)&adapter);
                mHost->mAutoCompletePopup->SetAdapter(adapter);
            }
            break;
        }
        case COPY_TO_CLIPBOARD: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            String text;
            ICharSequence::Probe(obj)->ToString(&text);
            mHost->CopyToClipboard(text);
            break;
        }
        case INIT_EDIT_FIELD:
            if (mHost->mInputConnection != NULL) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                CWebViewCoreTextFieldInitData* initData = (CWebViewCoreTextFieldInitData*)obj.Get();
                mHost->mTextGeneration = 0;
                mHost->mFieldPointer = initData->mFieldPointer;
                mHost->mInputConnection->InitEditorInfo(initData);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(initData->mText, (ICharSequence**)&cs);
                mHost->mInputConnection->SetTextAndKeepSelection(cs);
                mHost->mEditTextContentBounds->Set(initData->mContentBounds);
                mHost->mEditTextLayerId = initData->mNodeLayerId;
                mHost->NativeMapLayerRect(mHost->mNativeClass, mHost->mEditTextLayerId,
                        mHost->mEditTextContentBounds);
                mHost->mEditTextContent->Set(initData->mClientRect);
                mHost->RelocateAutoCompletePopup();
            }
            break;

        case REPLACE_TEXT:{
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            String text;
            ICharSequence::Probe(obj)->ToString(&text);
            Int32 start, end;
            msg->GetArg1(&start);
            msg->GetArg2(&end);
            Int32 cursorPosition = start + text.GetLength();
            mHost->ReplaceTextfieldText(start, end, text,
                    cursorPosition, cursorPosition);
            mHost->SelectionDone();
            break;
        }

        case UPDATE_MATCH_COUNT: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            CWebViewCoreFindAllRequest* request = (CWebViewCoreFindAllRequest*)obj.Get();
            if (request == NULL) {
                if (mHost->mFindCallback != NULL) {
                    mHost->mFindCallback->UpdateMatchCount(0, 0, TRUE);
                }
            }
            else if (request == mHost->mFindRequest) {
                Int32 matchCount, matchIndex;
                {
                    Object::Autolock lock(mHost->mFindRequest);

                    matchCount = request->mMatchCount;
                    matchIndex = request->mMatchIndex;
                }
                if (mHost->mFindCallback != NULL) {
                    mHost->mFindCallback->UpdateMatchCount(matchIndex, matchCount, FALSE);
                }
                if (mHost->mFindListener != NULL) {
                    mHost->mFindListener->OnFindResultReceived(matchIndex, matchCount, TRUE);
                }
            }
            break;
        }

        case CLEAR_CARET_HANDLE:
            if (mHost->mIsCaretSelection) {
                mHost->SelectionDone();
            }
            break;

        case KEY_PRESS: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->SendBatchableInputMessage(CWebViewCore::EventHub::KEY_PRESS, arg1, 0, NULL);
            break;
        }
        case RELOCATE_AUTO_COMPLETE_POPUP:
            mHost->RelocateAutoCompletePopup();
            break;

        case AUTOFILL_FORM: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            mHost->mWebViewCore->SendMessage(CWebViewCore::EventHub::AUTOFILL_FORM,
                    arg1, /* unused */0);
            break;
        }
        case EDIT_TEXT_SIZE_CHANGED: {
            Int32 arg1;
            msg->GetArg1(&arg1);
            if (arg1 == mHost->mFieldPointer) {
                AutoPtr<IInterface> obj;
                msg->GetObj((IInterface**)&obj);
                mHost->mEditTextContent->Set(IRect::Probe(obj));
            }
            break;
        }
        case SHOW_CARET_HANDLE:
            if (!mHost->mSelectingText && mHost->mIsEditingText && mHost->mIsCaretSelection) {
                mHost->SetupWebkitSelect();
                mHost->ResetCaretTimer();
                mHost->ShowPasteWindow();
            }
            break;

        case UPDATE_CONTENT_BOUNDS: {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            mHost->mEditTextContentBounds->Set(IRect::Probe(obj));
            mHost->NativeMapLayerRect(mHost->mNativeClass, mHost->mEditTextLayerId,
                    mHost->mEditTextContentBounds);
            break;
        }
        case SCROLL_EDIT_TEXT:
            mHost->ScrollEditWithCursor();
            break;

        case SCROLL_HANDLE_INTO_VIEW:
            mHost->ScrollDraggedSelectionHandleIntoView();
            break;

        default:
            HandlerBase::HandleMessage(msg);
            break;
    }
    return NOERROR;
}

ECode CWebViewClassic::PrivateHandler::GetUiLooper(
    /* [out] */ ILooper** looper)
{
    return HandlerBase::GetLooper(looper);
}

ECode CWebViewClassic::PrivateHandler::DispatchUiEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 flags)
{
    mHost->OnHandleUiEvent(event, eventType, flags);
    return NOERROR;
}

ECode CWebViewClassic::PrivateHandler::GetContext(
    /* [out] */ IContext** context)
{
    AutoPtr<IContext> c = mHost->GetContext();
    *context = c;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode CWebViewClassic::PrivateHandler::ShouldInterceptTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    if (!mHost->mSelectingText) {
        *result = FALSE;
        return NOERROR;
    }
    mHost->EnsureSelectionHandles();
    Float eventX, eventY;
    event->GetY(&eventY);
    event->GetX(&eventX);
    Int32 y = Elastos::Core::Math::Round(eventY - mHost->GetTitleHeight() + mHost->GetScrollY());
    Int32 x = Elastos::Core::Math::Round(eventX + mHost->GetScrollX());
    Boolean isPressingHandle;
    if (mHost->mIsCaretSelection) {
        AutoPtr<IRect> bound;
        mHost->mSelectHandleCenter->GetBounds((IRect**)&bound);
        bound->Contains(x, y, &isPressingHandle);
    }
    else {
        Boolean b1 = FALSE, b2 = FALSE;
        isPressingHandle =
                (mHost->mSelectHandleBaseBounds->Contains(x, y, &b1), b1)
                || (mHost->mSelectHandleExtentBounds->Contains(x, y, &b2), b2);
    }
    *result = isPressingHandle;
    return NOERROR;
}

ECode CWebViewClassic::PrivateHandler::ShowTapHighlight(
    /* [in] */ Boolean show)
{
    if (mHost->mShowTapHighlight != show) {
        mHost->mShowTapHighlight = show;
        mHost->Invalidate();
    }
    return NOERROR;
}

ECode CWebViewClassic::PrivateHandler::ClearPreviousHitTest()
{
    mHost->SetHitTestResult(NULL);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::_FocusTransitionDrawable
//===============================================================
CWebViewClassic::_FocusTransitionDrawable::_FocusTransitionDrawable(
    /* [in] */ CWebViewClassic* view)
{
    mWebView = view;
    CPaint::New(mWebView->mTouchHightlightPaint, (IPaint**)&mPaint);
    mPaint->GetAlpha(&mMaxAlpha);
}

ECode CWebViewClassic::_FocusTransitionDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    return NOERROR;
}

ECode CWebViewClassic::_FocusTransitionDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return NOERROR;
}

Int32 CWebViewClassic::_FocusTransitionDrawable::GetOpacity()
{
    return 0;
}

void CWebViewClassic::_FocusTransitionDrawable::SetProgress(
    /* [in] */ Float p)
{
    mProgress = p;
    if (mWebView->mFocusTransition.Get() == this) {
        if (mProgress == 1.0f) {
            mWebView->mFocusTransition = NULL;
        }
        mWebView->Invalidate();
    }
}

Float CWebViewClassic::_FocusTransitionDrawable::GetProgress()
{
    return mProgress;
}

ECode CWebViewClassic::_FocusTransitionDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mTranslate == NULL) {
        AutoPtr<IRect> bounds;
        mPreviousRegion->GetBounds((IRect**)&bounds);
        Int32 centerX, centerY;
        bounds->GetCenterX(&centerX);
        bounds->GetCenterY(&centerY);
        AutoPtr<IPoint> from;
        CPoint::New(centerX, centerY, (IPoint**)&from);
        Boolean result;
        mNewRegion->GetBounds(bounds, &result);
        bounds->GetCenterX(&centerX);
        bounds->GetCenterY(&centerY);
        AutoPtr<IPoint> to;
        CPoint::New(centerX, centerY, (IPoint**)&to);
        Int32 fromX, fromY, toX, toY;
        from->GetX(&fromX);
        from->GetY(&fromY);
        to->GetX(&toX);
        to->GetY(&toY);
        CPoint::New(fromX - toX, fromY - toY, (IPoint**)&mTranslate);
    }
    Int32 alpha = (Int32) (mProgress * mMaxAlpha);
    AutoPtr<IRegionIterator> iter;
    CRegionIterator::New(mPreviousRegion, (IRegionIterator**)&iter);
    AutoPtr<IRect> r;
    CRect::New((IRect**)&r);
    mPaint->SetAlpha(mMaxAlpha - alpha);
    Int32 translateX, translateY;
    mTranslate->GetX(&translateX);
    mTranslate->GetY(&translateY);
    Float tx = translateX * mProgress;
    Float ty = translateY * mProgress;
    Int32 save;
    canvas->Save(ICanvas::MATRIX_SAVE_FLAG, &save);
    canvas->Translate(-tx, -ty);
    Boolean bNext = FALSE;
    while ((iter->Next(r, &bNext), bNext)) {
        canvas->DrawRect(r, mPaint);
    }
    canvas->RestoreToCount(save);
    iter = NULL;
    CRegionIterator::New(mNewRegion, (IRegionIterator**)&iter);
    r = NULL;
    CRect::New((IRect**)&r);
    mPaint->SetAlpha(alpha);
    canvas->Save(ICanvas::MATRIX_SAVE_FLAG, &save);
    mTranslate->GetX(&translateX);
    mTranslate->GetY(&translateY);
    tx = translateX - tx;
    ty = translateY - ty;
    canvas->Translate(tx, ty);
    while ((iter->Next(r, &bNext), bNext)) {
        canvas->DrawRect(r, mPaint);
    }
    canvas->RestoreToCount(save);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::FocusTransitionDrawable
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::FocusTransitionDrawable, IDrawable);

IDRAWABLE_METHODS_IMPL(CWebViewClassic::FocusTransitionDrawable, _FocusTransitionDrawable)


//===============================================================
//               CWebViewClassic::InvokeListBox::Container
//===============================================================
const Int32 CWebViewClassic::InvokeListBox::Container::OPTGROUP;
const Int32 CWebViewClassic::InvokeListBox::Container::OPTION_DISABLED;
const Int32 CWebViewClassic::InvokeListBox::Container::OPTION_ENABLED;

CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::InvokeListBox::Container, IInterface)


//===============================================================
//          CWebViewClassic::InvokeListBox::MyArrayListAdapter
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::InvokeListBox::MyArrayListAdapter, IArrayAdapter)

CWebViewClassic::InvokeListBox::MyArrayListAdapter::MyArrayListAdapter(
    /* [in] */ InvokeListBox* host)

{
    AutoPtr<IObjectContainer> containers;
    if (host->mContainers != NULL) {
        CObjectContainer::New((IObjectContainer**)&containers);
        for (Int32 i = 0; i < host->mContainers->GetLength(); ++i) {
            containers->Add((*host->mContainers)[i]);
        }
    }
    ArrayAdapter::Init(host->mHost->mContext,
            host->mMultiple ? R::layout::select_dialog_multichoice :
            R::layout::webview_select_singlechoice,
            0, containers);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* _convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    // Always pass in null so that we will get a new CheckedTextView
    // Otherwise, an item which was previously used as an <optgroup>
    // element (i.e. has no check), could get used as an <option>
    // element, which needs a checkbox/radio, but it would not have
    // one.
    AutoPtr<IView> convertView = ArrayAdapter::GetView(position, NULL, parent);
    AutoPtr<Container> c = Item(position);
    if (c != NULL && Container::OPTION_ENABLED != c->mEnabled) {
        // ListView does not draw dividers between disabled and
        // enabled elements.  Use a LinearLayout to provide dividers
        AutoPtr<ILinearLayout> layout;
        CLinearLayout::New(mHost->mHost->mContext, (ILinearLayout**)&layout);
        layout->SetOrientation(ILinearLayout::VERTICAL);
        if (position > 0) {
            AutoPtr<IView> dividerTop;
            CView::New(mHost->mHost->mContext, (IView**)&dividerTop);
            dividerTop->SetBackgroundResource(
                    R::drawable::divider_horizontal_bright);
            layout->AddView(dividerTop);
        }

        if (Container::OPTGROUP == c->mEnabled) {
            // Currently select_dialog_multichoice uses CheckedTextViews.
            // If that changes, the class cast will no longer be valid.
            if (mHost->mMultiple) {
                assert(ICheckedTextView::Probe(convertView) != NULL);
                ICheckedTextView::Probe(convertView)->SetCheckMarkDrawable(NULL);
            }
        }
        else {
            // c.mEnabled == Container.OPTION_DISABLED
            // Draw the disabled element in a disabled state.
            convertView->SetEnabled(FALSE);
        }

        layout->AddView(convertView);
        if (position < ArrayAdapter::GetCount() - 1) {
            AutoPtr<IView> dividerBottom;
            CView::New(mHost->mHost->mContext, (IView**)&dividerBottom);
            dividerBottom->SetBackgroundResource(
                    R::drawable::divider_horizontal_bright);
            layout->AddView(dividerBottom);
        }
        *view = layout;
        REFCOUNT_ADD(*view);
        return NOERROR;
    }
    *view = convertView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::HasStableIds(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // AdapterView's onChanged method uses this to determine whether
    // to restore the old state.  Return false so that the old (out
    // of date) state does not replace the new, valid state.
    *result = FALSE;
    return NOERROR;
}

AutoPtr<CWebViewClassic::InvokeListBox::Container>
CWebViewClassic::InvokeListBox::MyArrayListAdapter::Item(
    /* [in] */ Int32 position)
{
    if (position < 0 || position >= ArrayAdapter::GetCount()) {
        return NULL;
    }
    return (Container*)ArrayAdapter::GetItem(position).Get();
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    AutoPtr<Container> item = Item(position);
    if (item == NULL) {
        *id = -1;
        return NOERROR;
    }
    *id = item->mId;
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<Container> item = Item(position);
    if (item == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    *result = Container::OPTION_ENABLED == item->mEnabled;
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::RegisterDataSetObserver(observer);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return UnregisterDataSetObserver(observer);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = ArrayAdapter::GetCount();
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> obj = ArrayAdapter::GetItem(position);
    *item = obj;
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = ArrayAdapter::GetItemViewType(position);
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = ArrayAdapter::GetViewTypeCount();
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = ArrayAdapter::IsEmpty();
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::NotifyDataSetChanged()
{
    return ArrayAdapter::NotifyDataSetChanged();
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::NotifyDataSetInvalidated()
{
    return ArrayAdapter::NotifyDataSetInvalidated();
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::Add(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Add(object);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::AddAll(
    /* [in] */ IObjectContainer* collection)
{
    return ArrayAdapter::AddAll(collection);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::AddAll(
    /* [in] */ ArrayOf<IInterface* >* items)
{
    return ArrayAdapter::AddAll(items);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    return ArrayAdapter::Insert(object, index);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::Remove(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Remove(object);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::Clear()
{
    return ArrayAdapter::Clear();
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    return ArrayAdapter::Sort(comparator);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    return ArrayAdapter::SetNotifyOnChange(notifyOnChange);
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    AutoPtr<IContext> c = ArrayAdapter::GetContext();
    *context = c;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::GetPosition(
    /* [in] */ IInterface* item,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ArrayAdapter::GetPosition(item);
    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::MyArrayListAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    return ArrayAdapter::SetDropDownViewResource(resource);
}


//===============================================================
//      CWebViewClassic::InvokeListBox::SingleDataSetObserver
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::InvokeListBox::SingleDataSetObserver, IDataSetObserver);

ECode CWebViewClassic::InvokeListBox::SingleDataSetObserver::OnChanged()
{
    // The filter may have changed which item is checked.  Find the
    // item that the ListView thinks is checked.
    Int32 position;
    mListView->GetCheckedItemPosition(&position);
    Int64 id;
    mAdapter->GetItemId(position, &id);
    if (mCheckedId != id) {
        // Clear the ListView's idea of the checked item, since
        // it is incorrect
        mListView->ClearChoices();
        // Search for mCheckedId.  If it is in the filtered list,
        // mark it as checked
        Int32 count;
        mAdapter->GetCount(&count);
        for (Int32 i = 0; i < count; i++) {
            Int64 itemId;
            if ((mAdapter->GetItemId(i, &itemId), itemId == mCheckedId)) {
                mListView->SetItemChecked(i, TRUE);
                break;
            }
        }
    }

    return NOERROR;
}

ECode CWebViewClassic::InvokeListBox::SingleDataSetObserver::OnInvalidated()
{
    return DataSetObserver::OnInvalidated();
}


//===============================================================
//                   CWebViewClassic::InvokeListBox
//===============================================================
CWebViewClassic::InvokeListBox::InvokeListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabled,
    /* [in] */ ArrayOf<Int32>* selected,
    /* [in] */ CWebViewClassic* host)
    : mMultiple(TRUE)
    , mSelectedArray(selected)
    , mSelection(0)
    , mHost(host)
{
    Int32 length = array->GetLength();
    mContainers = ArrayOf<Container*>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<Container> c = new Container();
        c->mString = (*array)[i];
        c->mEnabled = (*enabled)[i];
        c->mId = i;
        mContainers->Set(i, c);
    }
}

CWebViewClassic::InvokeListBox::InvokeListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabled,
    /* [in] */ Int32 selection,
    /* [in] */ CWebViewClassic* host)
    : mMultiple(FALSE)
    , mSelection(selection)
    , mHost(host)
{
    Int32 length = array->GetLength();
    mContainers = ArrayOf<Container*>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<Container> c = new Container();
        c->mString = (*array)[i];
        c->mEnabled = (*enabled)[i];
        c->mId = i;
        mContainers->Set(i, c);
    }
}

ECode CWebViewClassic::InvokeListBox::Run()
{
    if (mHost->mWebViewCore == NULL) {
        // We've been detached and/or destroyed since this was posted
        return NOERROR;
    }
    AutoPtr<IWebView> webView;
    mHost->GetWebView((IWebView**)&webView);
    Elastos::Droid::View::View* view = reinterpret_cast<Elastos::Droid::View::View*>(webView->Probe(EIID_View));
    if (view->GetWindowToken() == NULL
            || view->GetViewRootImpl() == NULL) {
        // We've been detached and/or destroyed since this was posted
        return NOERROR;
    }
    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(mHost->mContext, (ILayoutInflater**)&inflater);
    AutoPtr<IView> v;
    inflater->Inflate(R::layout::select_dialog, NULL, (IView**)&v);
    AutoPtr<IListView> listView = IListView::Probe(v);
    AutoPtr<MyArrayListAdapter> adapter = new MyArrayListAdapter(this);
    AutoPtr<IAlertDialogBuilder> b;
    CAlertDialogBuilder::New(mHost->mContext, (IAlertDialogBuilder**)&b);
    b->SetView(listView);
    b->SetCancelable(TRUE);
    b->SetInverseBackgroundForced(TRUE);

    if (mMultiple) {
        AutoPtr<IDialogInterfaceOnClickListener> positiveListener =
                new PositiveButtonClickListener(mHost, adapter, listView);
        b->SetPositiveButton(R::string::ok, positiveListener);
        AutoPtr<IDialogInterfaceOnClickListener> negativeListener =
                new NegativeButtonClickListener(mHost);
        b->SetNegativeButton(R::string::cancel, negativeListener);
    }
    AutoPtr<IAlertDialog> ad;
    b->Create((IAlertDialog**)&ad);
    mHost->mListBoxDialog = ad;
    listView->SetAdapter(adapter);
    listView->SetFocusableInTouchMode(TRUE);
    // There is a bug (1250103) where the checks in a ListView with
    // multiple items selected are associated with the positions, not
    // the ids, so the items do not properly retain their checks when
    // filtered.  Do not allow filtering on multiple lists until
    // that bug is fixed.

    listView->SetTextFilterEnabled(!mMultiple);
    if (mMultiple) {
        listView->SetChoiceMode(IListView::CHOICE_MODE_MULTIPLE);
        Int32 length = mSelectedArray->GetLength();
        for (Int32 i = 0; i < length; i++) {
            listView->SetItemChecked((*mSelectedArray)[i], TRUE);
        }
    }
    else {
        AutoPtr<IAdapterViewOnItemClickListener> listener = new ItemClickListener(mHost);
        listView->SetOnItemClickListener(listener);
        if (mSelection != -1) {
            listView->SetSelection(mSelection);
            listView->SetChoiceMode(IListView::CHOICE_MODE_SINGLE);
            listView->SetItemChecked(mSelection, TRUE);
            Int64 id;
            adapter->GetItemId(mSelection, &id);
            AutoPtr<IDataSetObserver> observer = new SingleDataSetObserver(
                    id, listView, adapter, this);
            adapter->RegisterDataSetObserver(observer);
        }
    }
    AutoPtr<IDialogInterfaceOnCancelListener> cancelListener = new CancelListener(mHost);
    mHost->mListBoxDialog->SetOnCancelListener(cancelListener);
    mHost->mListBoxDialog->Show();
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::PositiveButtonClickListener
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::PositiveButtonClickListener, IDialogInterfaceOnClickListener)

ECode CWebViewClassic::PositiveButtonClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    AutoPtr<IObjectInt32Map> arr;
    mListView->GetCheckedItemPositions((IObjectInt32Map**)&arr);
    Int32 count;
    mAdapter->GetCount(&count);
    mHost->mWebViewCore->SendMessage(
            CWebViewCore::EventHub::LISTBOX_CHOICES,
            count, 0, arr);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::NegativeButtonClickListener
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::NegativeButtonClickListener, IDialogInterfaceOnClickListener)

ECode CWebViewClassic::NegativeButtonClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->mWebViewCore->SendMessage(
            CWebViewCore::EventHub::SINGLE_LISTBOX_CHOICE, -2, 0);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::ItemClickListener
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::ItemClickListener, IAdapterViewOnItemClickListener)

ECode CWebViewClassic::ItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    // Rather than sending the message right away, send it
    // after the page regains focus.
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(NULL, CWebViewCore::EventHub::SINGLE_LISTBOX_CHOICE,
            (Int32)id, 0, (IMessage**)&msg);
    mHost->mListBoxMessage = msg;
    if (mHost->mListBoxDialog != NULL) {
        mHost->mListBoxDialog->Dismiss();
        mHost->mListBoxDialog = NULL;
    }
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::CancelListener
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::CancelListener, IDialogInterfaceOnCancelListener)

ECode CWebViewClassic::CancelListener::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    mHost->mWebViewCore->SendMessage(
            CWebViewCore::EventHub::SINGLE_LISTBOX_CHOICE, -2, 0);
    mHost->mListBoxDialog = NULL;
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::_PictureWrapperView
//===============================================================
CAR_INTERFACE_IMPL(CWebViewClassic::_PictureWrapperView, IView);

IVIEW_METHODS_IMPL(CWebViewClassic::_PictureWrapperView, Elastos::Droid::View::View)
IDRAWABLECALLBACK_METHODS_IMPL(CWebViewClassic::_PictureWrapperView, Elastos::Droid::View::View)
IKEYEVENTCALLBACK_METHODS_IMPL(CWebViewClassic::_PictureWrapperView, Elastos::Droid::View::View)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CWebViewClassic::_PictureWrapperView, Elastos::Droid::View::View)


//===============================================================
//                   CWebViewClassic::PictureWrapperView
//===============================================================
CWebViewClassic::PictureWrapperView::PictureWrapperView(
    /* [in] */ IContext* context,
    /* [in] */ IPicture* picture,
    /* [in] */ IWebView* parent)
    : _PictureWrapperView(context)
    , mPicture(picture)
    , mWebView(parent)
{
    Int32 width, height;
    SetWillNotDraw(FALSE);
    mPicture->GetWidth(&width);
    mPicture->GetHeight(&height);
    SetRight(width);
    SetBottom(height);
}

void CWebViewClassic::PictureWrapperView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    canvas->DrawPicture(mPicture);
}

ECode CWebViewClassic::PictureWrapperView::Post(
    /* [in] */ IRunnable* action,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mWebView->Post(action, result);
}


//===============================================================
//                   CWebViewClassic::CheckAsyncTask
//===============================================================
AutoPtr<IInterface> CWebViewClassic::CheckAsyncTask::DoInBackground(
    /* [in] */ ArrayOf<IInterface*>* params)
{
    AutoPtr<ISet> installedPackages;
    CHashSet::New((ISet**)&installedPackages);
    AutoPtr<IPackageManager> pm;
    mHost->mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr< ArrayOf<IInterface*> > apps;
    sGoogleApps->ToArray((ArrayOf<IInterface*>**)&apps);
    for (Int32 i = 0; i < apps->GetLength(); i++) {
        String name;
        ICharSequence::Probe((*apps)[i])->ToString(&name);
        // try {
        AutoPtr<IPackageInfo> pinfo;
        ECode ec = pm->GetPackageInfo(name,
                IPackageManager::GET_ACTIVITIES | IPackageManager::GET_SERVICES,
                (IPackageInfo**)&pinfo);
        if (FAILED(ec)) continue;
        Boolean result;
        installedPackages->Add((*apps)[i], &result);
        // } catch (PackageManager.NameNotFoundException e) {
        //     // package not found
        // }
    }
    return installedPackages;
}

void CWebViewClassic::CheckAsyncTask::OnPostExecute(
    /* [in] */ IInterface* installedPackages)
{
    if (mHost->mWebViewCore != NULL) {
        mHost->mWebViewCore->SendMessage(CWebViewCore::EventHub::ADD_PACKAGE_NAMES, installedPackages);
    }
}


//===============================================================
//                   CWebViewClassic::SavePasswordButtonClickListener
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::SavePasswordButtonClickListener, IDialogInterfaceOnClickListener);

ECode CWebViewClassic::SavePasswordButtonClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    if (mHost->mResumeMsg != NULL) {
        mMsg->SendToTarget();
        mHost->mResumeMsg = NULL;
    }
    mHost->mSavePasswordDialog = NULL;
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::SavePasswordDismissListener
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CWebViewClassic::SavePasswordDismissListener, IDialogInterfaceOnDismissListener);

ECode CWebViewClassic::SavePasswordDismissListener::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    if (mHost->mResumeMsg != NULL) {
        mMsg->SendToTarget();
        mHost->mResumeMsg = NULL;
    }
    mHost->mSavePasswordDialog = NULL;
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::RestorePictureRunnable
//===============================================================
ECode CWebViewClassic::RestorePictureRunnable::Run()
{
    // try {
    AutoPtr<IPicture> p = CPicture::CreateFromStream(mIn);
    if (p != NULL) {
        // Post a runnable on the main thread to update the
        // history picture fields.
        AutoPtr<IRunnable> r = new RestorePictureRunnableInner(mHost, p, mCopy);
        Boolean result;
        mHost->mPrivateHandler->Post(r, &result);
    }
    mIn->Close();
    // } finally {
    //     try {
    //         in.close();
    //     } catch (Exception e) {
    //         // Nothing we can do now.
    //     }
    // }
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::RestorePictureRunnableInner
//===============================================================
ECode CWebViewClassic::RestorePictureRunnableInner::Run()
{
    mHost->RestoreHistoryPictureFields(mPicture, mCopy);
    return NOERROR;
}


//===============================================================
//                   CWebViewClassic::LoadViewStateAsyncTask
//===============================================================
AutoPtr<IInterface> CWebViewClassic::LoadViewStateAsyncTask::DoInBackground(
    /* [in] */ ArrayOf<IInterface*>* params)
{
    // try {
    AutoPtr<IInputStream> stream = IInputStream::Probe((*params)[0]);
    AutoPtr<CWebViewCoreDrawData> data = ViewStateSerializer::DeserializeViewState(stream);
    return (IInterface*)data.Get();
    // } catch (IOException e) {
    //     return null;
    // }
}

void CWebViewClassic::LoadViewStateAsyncTask::OnPostExecute(
    /* [in] */ IInterface* _draw)
{
    CWebViewCoreDrawData* draw = (CWebViewCoreDrawData*)_draw;
    if (draw == NULL) {
        Logger::E(LOGTAG, "Failed to load view state!");
        return;
    }
    Int32 viewWidth = mHost->GetViewWidth();
    Int32 viewHeight = mHost->GetViewHeightWithTitle() - mHost->GetTitleHeight();
    AutoPtr<IPoint> p;
    CPoint::New(viewWidth, viewHeight, (IPoint**)&p);
    draw->mViewSize = p;
    draw->mViewState->mDefaultScale = mHost->GetDefaultZoomScale();
    mHost->mLoadedPicture = draw;
    mHost->SetNewPicture(mHost->mLoadedPicture, TRUE);
    mHost->mLoadedPicture->mViewState = NULL;
}


//===============================================================
//                         CWebViewClassic
//===============================================================
const Boolean CWebViewClassic::AUTO_REDRAW_HACK;
const Float CWebViewClassic::TEXT_SCROLL_RATE;
const Int64 CWebViewClassic::TEXT_SCROLL_FIRST_SCROLL_MS;
const Int32 CWebViewClassic::EDIT_RECT_BUFFER;
const Int64 CWebViewClassic::SELECTION_HANDLE_ANIMATION_MS;
const String CWebViewClassic::LOGTAG("CWebViewClassic");

const Int32 CWebViewClassic::TOUCH_SENT_INTERVAL;
const Float CWebViewClassic::MINIMUM_VELOCITY_RATIO_FOR_ACCELERATION;

const Int32 CWebViewClassic::TOUCH_INIT_MODE;
const Int32 CWebViewClassic::TOUCH_DRAG_START_MODE;
const Int32 CWebViewClassic::TOUCH_DRAG_MODE;
const Int32 CWebViewClassic::TOUCH_SHORTPRESS_START_MODE;
const Int32 CWebViewClassic::TOUCH_SHORTPRESS_MODE;
const Int32 CWebViewClassic::TOUCH_DOUBLE_TAP_MODE;
const Int32 CWebViewClassic::TOUCH_DONE_MODE;
const Int32 CWebViewClassic::TOUCH_PINCH_DRAG;
const Int32 CWebViewClassic::TOUCH_DRAG_LAYER_MODE;
const Int32 CWebViewClassic::TOUCH_DRAG_TEXT_MODE;

const Int32 CWebViewClassic::TAP_TIMEOUT;
const Int32 CWebViewClassic::LONG_PRESS_TIMEOUT;
const Int32 CWebViewClassic::MIN_FLING_TIME;
const Int32 CWebViewClassic::MOTIONLESS_TIME;
const Int32 CWebViewClassic::PAGE_SCROLL_OVERLAP;

const Int32 CWebViewClassic::STD_SPEED;
const Int32 CWebViewClassic::MAX_DURATION;

AutoPtr<IPaint> CWebViewClassic::mOverScrollBackground;
AutoPtr<IPaint> CWebViewClassic::mOverScrollBorder;

const Int32 CWebViewClassic::MOTIONLESS_FALSE;
const Int32 CWebViewClassic::MOTIONLESS_PENDING;
const Int32 CWebViewClassic::MOTIONLESS_TRUE;
const Int32 CWebViewClassic::MOTIONLESS_IGNORE;

const Int64 CWebViewClassic::CARET_HANDLE_STAMINA_MS;

const Int32 CWebViewClassic::HANDLE_ID_BASE;
const Int32 CWebViewClassic::HANDLE_ID_EXTENT;

const Int32 CWebViewClassic::HIGHLIGHT_COLOR;

const Boolean CWebViewClassic::DEBUG_TOUCH_HIGHLIGHT;
const Int32 CWebViewClassic::TOUCH_HIGHLIGHT_ELAPSE_TIME;

const Int32 CWebViewClassic::REMEMBER_PASSWORD;
const Int32 CWebViewClassic::NEVER_REMEMBER_PASSWORD;
const Int32 CWebViewClassic::SWITCH_TO_SHORTPRESS;
const Int32 CWebViewClassic::SWITCH_TO_LONGPRESS;
const Int32 CWebViewClassic::RELEASE_SINGLE_TAP;
const Int32 CWebViewClassic::REQUEST_FORM_DATA;
const Int32 CWebViewClassic::DRAG_HELD_MOTIONLESS;
const Int32 CWebViewClassic::PREVENT_DEFAULT_TIMEOUT;
const Int32 CWebViewClassic::SCROLL_SELECT_TEXT;

const Int32 CWebViewClassic::FIRST_PRIVATE_MSG_ID;
const Int32 CWebViewClassic::LAST_PRIVATE_MSG_ID;

const Int32 CWebViewClassic::SCROLL_TO_MSG_ID;
const Int32 CWebViewClassic::NEW_PICTURE_MSG_ID;
const Int32 CWebViewClassic::WEBCORE_INITIALIZED_MSG_ID;
const Int32 CWebViewClassic::UPDATE_TEXTFIELD_TEXT_MSG_ID;
const Int32 CWebViewClassic::UPDATE_ZOOM_RANGE;
const Int32 CWebViewClassic::TAKE_FOCUS;
const Int32 CWebViewClassic::CLEAR_TEXT_ENTRY;
const Int32 CWebViewClassic::UPDATE_TEXT_SELECTION_MSG_ID;
const Int32 CWebViewClassic::SHOW_RECT_MSG_ID;
const Int32 CWebViewClassic::LONG_PRESS_CENTER;
const Int32 CWebViewClassic::PREVENT_TOUCH_ID;
const Int32 CWebViewClassic::WEBCORE_NEED_TOUCH_EVENTS;
// obj=Rect in doc coordinates
const Int32 CWebViewClassic::INVAL_RECT_MSG_ID;
const Int32 CWebViewClassic::REQUEST_KEYBOARD;
const Int32 CWebViewClassic::SHOW_FULLSCREEN;
const Int32 CWebViewClassic::HIDE_FULLSCREEN;
const Int32 CWebViewClassic::UPDATE_MATCH_COUNT;
const Int32 CWebViewClassic::CENTER_FIT_RECT;
const Int32 CWebViewClassic::SET_SCROLLBAR_MODES;
const Int32 CWebViewClassic::SELECTION_STRING_CHANGED;
const Int32 CWebViewClassic::HIT_TEST_RESULT;
const Int32 CWebViewClassic::SAVE_WEBARCHIVE_FINISHED;

const Int32 CWebViewClassic::SET_AUTOFILLABLE;
const Int32 CWebViewClassic::AUTOFILL_COMPLETE;

const Int32 CWebViewClassic::SCREEN_ON;
const Int32 CWebViewClassic::UPDATE_ZOOM_DENSITY;
const Int32 CWebViewClassic::EXIT_FULLSCREEN_VIDEO;

const Int32 CWebViewClassic::COPY_TO_CLIPBOARD;
const Int32 CWebViewClassic::INIT_EDIT_FIELD;
const Int32 CWebViewClassic::REPLACE_TEXT;
const Int32 CWebViewClassic::CLEAR_CARET_HANDLE;
const Int32 CWebViewClassic::KEY_PRESS;
const Int32 CWebViewClassic::RELOCATE_AUTO_COMPLETE_POPUP;
const Int32 CWebViewClassic::FOCUS_NODE_CHANGED;
const Int32 CWebViewClassic::AUTOFILL_FORM;
const Int32 CWebViewClassic::SCROLL_EDIT_TEXT;
const Int32 CWebViewClassic::EDIT_TEXT_SIZE_CHANGED;
const Int32 CWebViewClassic::SHOW_CARET_HANDLE;
const Int32 CWebViewClassic::UPDATE_CONTENT_BOUNDS;
const Int32 CWebViewClassic::SCROLL_HANDLE_INTO_VIEW;

const Int32 CWebViewClassic::FIRST_PACKAGE_MSG_ID;
const Int32 CWebViewClassic::LAST_PACKAGE_MSG_ID;

const String CWebViewClassic::HandlerPrivateDebugString[] = {
    String("REMEMBER_PASSWORD"), //              = 1;
    String("NEVER_REMEMBER_PASSWORD"), //        = 2;
    String("SWITCH_TO_SHORTPRESS"), //           = 3;
    String("SWITCH_TO_LONGPRESS"), //            = 4;
    String("RELEASE_SINGLE_TAP"), //             = 5;
    String("REQUEST_FORM_DATA"), //              = 6;
    String("RESUME_WEBCORE_PRIORITY"), //        = 7;
    String("DRAG_HELD_MOTIONLESS"), //           = 8;
    String(""), //             = 9;
    String("PREVENT_DEFAULT_TIMEOUT"), //        = 10;
    String("SCROLL_SELECT_TEXT") //              = 11;
};

const String CWebViewClassic::HandlerPackageDebugString[] = {
    String("SCROLL_TO_MSG_ID"), //               = 101;
    String("102"), //                            = 102;
    String("103"), //                            = 103;
    String("104"), //                            = 104;
    String("NEW_PICTURE_MSG_ID"), //             = 105;
    String("UPDATE_TEXT_ENTRY_MSG_ID"), //       = 106;
    String("WEBCORE_INITIALIZED_MSG_ID"), //     = 107;
    String("UPDATE_TEXTFIELD_TEXT_MSG_ID"), //   = 108;
    String("UPDATE_ZOOM_RANGE"), //              = 109;
    String("UNHANDLED_NAV_KEY"), //              = 110;
    String("CLEAR_TEXT_ENTRY"), //               = 111;
    String("UPDATE_TEXT_SELECTION_MSG_ID"), //   = 112;
    String("SHOW_RECT_MSG_ID"), //               = 113;
    String("LONG_PRESS_CENTER"), //              = 114;
    String("PREVENT_TOUCH_ID"), //               = 115;
    String("WEBCORE_NEED_TOUCH_EVENTS"), //      = 116;
    String("INVAL_RECT_MSG_ID"), //              = 117;
    String("REQUEST_KEYBOARD"), //               = 118;
    String("DO_MOTION_UP"), //                   = 119;
    String("SHOW_FULLSCREEN"), //                = 120;
    String("HIDE_FULLSCREEN"), //                = 121;
    String("DOM_FOCUS_CHANGED"), //              = 122;
    String("REPLACE_BASE_CONTENT"), //           = 123;
    String("RETURN_LABEL"), //                   = 125;
    String("UPDATE_MATCH_COUNT"), //             = 126;
    String("CENTER_FIT_RECT"), //                = 127;
    String("REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID"), // = 128;
    String("SET_SCROLLBAR_MODES"), //            = 129;
    String("SELECTION_STRING_CHANGED"), //       = 130;
    String("SET_TOUCH_HIGHLIGHT_RECTS"), //      = 131;
    String("SAVE_WEBARCHIVE_FINISHED"), //       = 132;
    String("SET_AUTOFILLABLE"), //               = 133;
    String("AUTOFILL_COMPLETE"), //              = 134;
    String("SELECT_AT"), //                      = 135;
    String("SCREEN_ON"), //                      = 136;
    String("ENTER_FULLSCREEN_VIDEO"), //         = 137;
    String("UPDATE_SELECTION"), //               = 138;
    String("UPDATE_ZOOM_DENSITY") //             = 139;
};

const Int32 CWebViewClassic::DEFAULT_VIEWPORT_WIDTH;

Int32 CWebViewClassic::sMaxViewportWidth = CWebViewClassic::DEFAULT_VIEWPORT_WIDTH;

const Int32 CWebViewClassic::SNAP_NONE;
const Int32 CWebViewClassic::SNAP_LOCK;
const Int32 CWebViewClassic::SNAP_X;
const Int32 CWebViewClassic::SNAP_Y;

const Int32 CWebViewClassic::DRAW_EXTRAS_NONE;
const Int32 CWebViewClassic::DRAW_EXTRAS_SELECTION;
const Int32 CWebViewClassic::DRAW_EXTRAS_CURSOR_RING;

const Int32 CWebViewClassic::SCROLLBAR_AUTO;
const Int32 CWebViewClassic::SCROLLBAR_ALWAYSOFF;
const Int32 CWebViewClassic::SCROLLBAR_ALWAYSON;

Boolean CWebViewClassic::mLogEvent = TRUE;

Boolean CWebViewClassic::sNotificationsEnabled = TRUE;

const String CWebViewClassic::SCHEME_TEL("tel:");
const String CWebViewClassic::SCHEME_MAILTO("mailto:");
const String CWebViewClassic::SCHEME_GEO("geo:0,0?q=");

const Int64 CWebViewClassic::SELECT_SCROLL_INTERVAL;

AutoPtr<CWebViewClassic::TrustStorageListener> CWebViewClassic::sTrustStorageListener;

AutoPtr<CWebViewClassic::ProxyReceiver> CWebViewClassic::sProxyReceiver;

Boolean CWebViewClassic::sPackageInstallationReceiverAdded = FALSE;

struct ElaWebViewCallback CWebViewClassic::sElaWebViewCallback =
{
    &CWebViewClassic::SetNativeClass,
    &CWebViewClassic::GetNativeClass,
    &CWebViewClassic::ScrollBy,
    &CWebViewClassic::GetScaledMaxXScroll,
    &CWebViewClassic::GetScaledMaxYScroll,
    &CWebViewClassic::UpdateRectsForGL,
    &CWebViewClassic::ViewInvalidate,
    &CWebViewClassic::ViewInvalidateRect,
    &CWebViewClassic::PostInvalidateDelayed,
    &CWebViewClassic::PageSwapCallback,
    &QuadF::SetQuadFP1,
    &QuadF::SetQuadFP2,
    &QuadF::SetQuadFP3,
    &QuadF::SetQuadFP4
};

static AutoPtr<ISet> InitsGoogleApps()
{
    AutoPtr<ISet> apps;
    CHashSet::New((ISet**)&apps);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("com.google.android.youtube"), (ICharSequence**)&cs);
    Boolean result;
    apps->Add(cs, &result);
    return apps;
}

AutoPtr<ISet> CWebViewClassic::sGoogleApps = InitsGoogleApps();

const Int32 CWebViewClassic::NO_LEFTEDGE;

const Int32 CWebViewClassic::ZOOM_BITS;
const Int32 CWebViewClassic::SCROLL_BITS;

const Float CWebViewClassic::HSLOPE_TO_START_SNAP;
const Float CWebViewClassic::HSLOPE_TO_BREAK_SNAP;
const Float CWebViewClassic::VSLOPE_TO_START_SNAP;
const Float CWebViewClassic::VSLOPE_TO_BREAK_SNAP;

const Float CWebViewClassic::ANGLE_VERT;
const Float CWebViewClassic::ANGLE_HORIZ;
const Float CWebViewClassic::MMA_WEIGHT_N;

const Int32 CWebViewClassic::DRAG_LAYER_FINGER_DISTANCE;

const Int32 CWebViewClassic::TRACKBALL_KEY_TIMEOUT;
const Int32 CWebViewClassic::TRACKBALL_TIMEOUT;
const Int32 CWebViewClassic::TRACKBALL_WAIT;
const Int32 CWebViewClassic::TRACKBALL_SCALE;
const Int32 CWebViewClassic::TRACKBALL_SCROLL_COUNT;
const Int32 CWebViewClassic::TRACKBALL_MOVE_COUNT;
const Int32 CWebViewClassic::TRACKBALL_MULTIPLIER;
const Int32 CWebViewClassic::SELECT_CURSOR_OFFSET;
const Int32 CWebViewClassic::SELECT_SCROLL;

Mutex CWebViewClassic::sLock;

CWebViewClassic::CWebViewClassic()
    : mEditTextLayerId(0)
    , mIsEditingText(FALSE)
    , mIsBatchingTextChanges(FALSE)
    , mWidthCanMeasure(FALSE)
    , mHeightCanMeasure(FALSE)
    , mLastWidthSent(0)
    , mLastHeightSent(0)
    , mLastActualHeightSent(0)
    , mLastSwapTime(0)
    , mAverageSwapFps(0)
    , mAutoRedraw(FALSE)
    , mIsWebViewVisible(TRUE)
    , mFieldPointer(0)
    , mLastEditScroll(0)
    , mNativeClass(0)
    , mWebViewCore(NULL)
    , mTextGeneration(0)
    , mLastTouchX(0)
    , mLastTouchY(0)
    , mStartTouchX(0)
    , mStartTouchY(0)
    , mAverageAngle(0)
    , mLastTouchTime(0)
    , mLastSentTouchTime(0)
    , mCurrentTouchInterval(TOUCH_SENT_INTERVAL)
    , mMaximumFling(0)
    , mLastVelocity(0)
    , mLastVelX(0)
    , mLastVelY(0)
    , mCurrentScrollingLayerId(0)
    , mTouchMode(TOUCH_DONE_MODE)
    , mConfirmMove(FALSE)
    , mTouchInEditText(FALSE)
    , mDrawCursorRing(TRUE)
    , mIsPaused(FALSE)
    , mFocusedNode(NULL)
    , mTouchSlopSquare(0)
    , mDoubleTapSlopSquare(0)
    , mNavSlop(0)
    , mContentWidth(0)
    , mContentHeight(0)
    , mOverlayHorizontalScrollbar(TRUE)
    , mOverlayVerticalScrollbar(FALSE)
    , mInOverScrollMode(FALSE)
    , mWrapContent(FALSE)
    , mHeldMotionless(0)
    , mSelectCursorExtentLayerId(0)
    , mIsCaretSelection(FALSE)
    , mTouchHighlightX(0)
    , mTouchHighlightY(0)
    , mShowTapHighlight(FALSE)
    , mBlockWebkitViewMessages(FALSE)
    , mHardwareAccelSkia(FALSE)
    , mInitialScaleInPercent(0)
    , mSendScrollEvent(TRUE)
    , mSnapScrollMode(SNAP_NONE)
    , mSnapPositive(FALSE)
    , mHorizontalScrollBarMode(SCROLLBAR_AUTO)
    , mVerticalScrollBarMode(SCROLLBAR_AUTO)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mLastTouchUpTime(0)
    , mAutoFillData(NULL)
    , mBackgroundColor(IColor::WHITE)
    , mAutoScrollX(0)
    , mAutoScrollY(0)
    , mMinAutoScrollX(0)
    , mMaxAutoScrollX(0)
    , mMinAutoScrollY(0)
    , mMaxAutoScrollY(0)
    , mSentAutoScrollMessage(FALSE)
    , mPictureUpdatePausedForFocusChange(FALSE)
    , mCachedOverlappingActionModeHeight(-1)
    , mFindIsUp(FALSE)
    , mFindRequest(NULL)
    , mOrientation(IConfiguration::ORIENTATION_UNDEFINED)
    , mDrawHistory(FALSE)
    , mHistoryWidth(0)
    , mHistoryHeight(0)
    , mGotCenterDown(FALSE)
    , mTrackballFirstTime(0)
    , mTrackballLastTime(0)
    , mTrackballRemainsX(0)
    , mTrackballRemainsY(0)
    , mTrackballXMove(0)
    , mTrackballYMove(0)
    , mSelectingText(FALSE)
    , mShowTextSelectionExtra(FALSE)
    , mSelectionStarted(FALSE)
    , mSelectX(0)
    , mSelectY(0)
    , mTrackballDown(FALSE)
    , mTrackballUpTime(0)
    , mLastCursorTime(0)
    , mMapTrackballToArrowKeys(TRUE)
{
    CRect::New((IRect**)&mInvScreenRect);
    CRect::New((IRect**)&mScreenRect);
    CRectF::New((IRectF**)&mVisibleContentRect);
    CRect::New((IRect**)&mEditTextContentBounds);
    CRect::New((IRect**)&mEditTextContent);
    mPrivateHandler = new PrivateHandler(this);
    CRect::New((IRect**)&mScrollingLayerRect);
    CPoint::New((IPoint**)&mSelectCursorBase);
    CRect::New((IRect**)&mSelectHandleBaseBounds);
    mSelectCursorBaseTextQuad = new QuadF();
    CPoint::New((IPoint**)&mSelectCursorExtent);
    CRect::New((IRect**)&mSelectHandleExtentBounds);
    mSelectCursorExtentTextQuad = new QuadF();
    CRegion::New((IRegion**)&mTouchHighlightRegion);
    CPaint::New((IPaint**)&mTouchHightlightPaint);
    CRect::New((IRect**)&mScrollingLayerBounds);
    CRect::New((IRect**)&mLastVisibleRectSent);
    CRect::New((IRect**)&mLastGlobalRect);
    CRect::New((IRect**)&mVisibleRect);
    CRect::New((IRect**)&mGlobalVisibleRect);
    CPoint::New((IPoint**)&mScrollOffset);
    CPoint::New((IPoint**)&mGlobalVisibleOffset);
    CRect::New((IRect**)&mTempContentVisibleRect);
    AutoPtr<IPaintFlagsDrawFilter> filter;
    CPaintFlagsDrawFilter::New(ZOOM_BITS, IPaint::LINEAR_TEXT_FLAG, (IPaintFlagsDrawFilter**)&filter);
    mZoomFilter = IDrawFilter::Probe(filter);
    filter = NULL;
    CPaintFlagsDrawFilter::New(SCROLL_BITS, 0, (IPaintFlagsDrawFilter**)&filter);
    mScrollFilter = IDrawFilter::Probe(filter);
    CPoint::New((IPoint**)&mTempVisibleRectOffset);
    CRect::New((IRect**)&mTempVisibleRect);
    mBaseAlpha = new SelectionHandleAlpha(this);
    mExtentAlpha = new SelectionHandleAlpha(this);
    AutoPtr<IObjectAnimatorHelper> helper;
    CObjectAnimatorHelper::AcquireSingleton((IObjectAnimatorHelper**)&helper);
    AutoPtr< ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(1);
    (*values)[0] = 0;
    helper->OfInt32(mBaseAlpha.Get(), String("alpha"), values, (IObjectAnimator**)&mBaseHandleAlphaAnimator);
    values = ArrayOf<Int32>::Alloc(1);
    (*values)[0] = 0;
    helper->OfInt32(mExtentAlpha.Get(), String("alpha"), values, (IObjectAnimator**)&mExtentHandleAlphaAnimator);
}

CWebViewClassic::~CWebViewClassic()
{
    // try {
    Destroy();
    // } finally {
    //     super.finalize();
    // }
}

ECode CWebViewClassic::constructor(
    /* [in] */ IWebView* webView,
    /* [in] */ IWebViewPrivateAccess* privateAccess)
{
    mWebView = webView;
    mWebViewPrivate = privateAccess;
    webView->GetContext((IContext**)&mContext);
    return NOERROR;
}

/* package */
void CWebViewClassic::IncrementTextGeneration()
{
    mTextGeneration++;
}

/**
 * See {@link WebViewProvider#init(Map, boolean)}
 */
ECode CWebViewClassic::Init(
    /* [in] */ IMap* javaScriptInterfaces,
    /* [in] */ Boolean privateBrowsing)
{
    AutoPtr<IContext> context = mContext;

    // Used by the chrome stack to find application paths
    NativeUtil::SetContext(context);

    mCallbackProxy = new CallbackProxy(context, this);
    mViewManager = new ViewManager(this);
    AutoPtr<IContext> con;
    context->GetApplicationContext((IContext**)&con);
    L10nUtils::SetApplicationContext(con);
    CWebViewCore::NewByFriend(context, this, mCallbackProxy, javaScriptInterfaces, &mWebViewCore);
    mDatabase = WebViewDatabaseClassic::GetInstance(context);
    mScroller = new OverScroller(context, NULL, 0, 0, FALSE); //TODO Use OverScroller's flywheel
    mZoomManager = new ZoomManager(this, mCallbackProxy);

    /* The init method must follow the creation of certain member variables,
     * such as the mZoomManager.
     */
    Init();
    SetupPackageListener(context);
    SetupProxyListener(context);
    SetupTrustStorageListener(context);
    UpdateMultiTouchSupport(context);

    if (privateBrowsing) {
        StartPrivateBrowsing();
    }

    mAutoFillData = new CWebViewCoreAutoFillData();
    CScroller::New(context, (IScroller**)&mEditTextScroller);

    return NOERROR;
}

void CWebViewClassic::OnHandleUiEvent(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 eventType,
    /* [in] */ Int32 flags)
{
    switch (eventType) {
    case WebViewInputDispatcher::EVENT_TYPE_LONG_PRESS: {
        AutoPtr<IWebViewHitTestResult> hitTest;
        GetHitTestResult((IWebViewHitTestResult**)&hitTest);
        if (hitTest != NULL) {
            Boolean result;
            mWebView->PerformLongClick(&result);
        }
        break;
    }
    case WebViewInputDispatcher::EVENT_TYPE_DOUBLE_TAP: {
        Float x, y;
        event->GetX(&x);
        event->GetY(&y);
        mZoomManager->HandleDoubleTap(x, y);
        break;
    }
    case WebViewInputDispatcher::EVENT_TYPE_TOUCH:
        OnHandleUiTouchEvent(event);
        break;
    case WebViewInputDispatcher::EVENT_TYPE_CLICK:
        if (mFocusedNode != NULL && !mFocusedNode->mIntentUrl.IsNull()) {
            mWebView->PlaySoundEffect(SoundEffectConstants::CLICK);
            OverrideLoading(mFocusedNode->mIntentUrl);
        }
        break;
    }
}

void CWebViewClassic::OnHandleUiTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    AutoPtr<IScaleGestureDetector> detector =
            mZoomManager->GetScaleGestureDetector();

    Int32 action;
    ev->GetActionMasked(&action);
    Boolean pointerUp = action == IMotionEvent::ACTION_POINTER_UP;
    Boolean configChanged =
        action == IMotionEvent::ACTION_POINTER_UP ||
        action == IMotionEvent::ACTION_POINTER_DOWN;
    Int32 actionIndex;
    ev->GetActionIndex(&actionIndex);
    Int32 skipIndex = pointerUp ? actionIndex : -1;

    // Determine focal point
    Float sumX = 0, sumY = 0;
    Int32 count;
    ev->GetPointerCount(&count);
    for (Int32 i = 0; i < count; i++) {
        if (skipIndex == i) continue;
        Float x, y;
        ev->GetX(i, &x);
        sumX += x;
        ev->GetY(i, &y);
        sumY += y;
    }
    Int32 div = pointerUp ? count - 1 : count;
    Float x = sumX / div;
    Float y = sumY / div;

    if (configChanged) {
        mLastTouchX = Elastos::Core::Math::Round(x);
        mLastTouchY = Elastos::Core::Math::Round(y);
        ev->GetEventTime(&mLastTouchTime);
        mWebView->CancelLongPress();
        mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
    }

    if (detector != NULL) {
        Boolean result;
        detector->OnTouchEvent(ev, &result);
        if (detector->IsInProgress(&result), result) {
            ev->GetEventTime(&mLastTouchTime);

            if (!mZoomManager->SupportsPanDuringZoom()) {
                return;
            }
            mTouchMode = TOUCH_DRAG_MODE;
            if (mVelocityTracker == NULL) {
                mVelocityTracker = VelocityTracker::Obtain();
            }
        }
    }

    if (action == IMotionEvent::ACTION_POINTER_DOWN) {
        CancelTouch();
        action = IMotionEvent::ACTION_DOWN;
    }
    else if (action == IMotionEvent::ACTION_MOVE) {
        // negative x or y indicate it is on the edge, skip it.
        if (x < 0 || y < 0) {
            return;
        }
    }

    HandleTouchEventCommon(ev, action, Elastos::Core::Math::Round(x), Elastos::Core::Math::Round(y));
}

/**
 * @return The webview proxy that this classic webview is bound to.
 */
ECode CWebViewClassic::GetWebView(
    /* [out] */ IWebView** webView)
{
    VALIDATE_NOT_NULL(webView);
    *webView = mWebView;
    REFCOUNT_ADD(*webView);
    return NOERROR;
}

ECode CWebViewClassic::GetViewDelegate(
    /* [out] */ IWebViewProviderViewDelegate** delegate)
{
    VALIDATE_NOT_NULL(delegate);
    *delegate = this;
    REFCOUNT_ADD(*delegate);
    return NOERROR;
}

ECode CWebViewClassic::GetScrollDelegate(
    /* [out] */ IWebViewProviderScrollDelegate** delegate)
{
    VALIDATE_NOT_NULL(delegate);
    *delegate = this;
    REFCOUNT_ADD(*delegate);
    return NOERROR;
}

AutoPtr<CWebViewClassic> CWebViewClassic::FromWebView(
    /* [in] */ IWebView* webView)
{
    if (webView == NULL) return NULL;

    AutoPtr<IWebViewProvider> provider;
    webView->GetWebViewProvider((IWebViewProvider**)&provider);
    return (CWebViewClassic*)provider.Get();
}

// Accessors, purely for convenience (and to reduce code churn during webview proxy migration).
Int32 CWebViewClassic::GetScrollX()
{
    Int32 x;
    mWebView->GetScrollX(&x);
    return x;
}

Int32 CWebViewClassic::GetScrollY()
{
    Int32 y;
    mWebView->GetScrollY(&y);
    return y;
}

Int32 CWebViewClassic::GetWidth()
{
    Int32 width;
    mWebView->GetWidth(&width);
    return width;
}

Int32 CWebViewClassic::GetHeight()
{
    Int32 height;
    mWebView->GetHeight(&height);
    return height;
}

AutoPtr<IContext> CWebViewClassic::GetContext()
{
    return mContext;
}

void CWebViewClassic::Invalidate()
{
    mWebView->Invalidate();
}

// Setters for the Scroll X & Y, without invoking the onScrollChanged etc code paths.
void CWebViewClassic::SetScrollXRaw(
    /* [in] */ Int32 mScrollX)
{
    mWebViewPrivate->SetScrollXRaw(mScrollX);
}

void CWebViewClassic::SetScrollYRaw(
    /* [in] */ Int32 mScrollY)
{
    mWebViewPrivate->SetScrollYRaw(mScrollY);
}

/**
 * Handles update to the trust storage.
 */
void CWebViewClassic::HandleCertTrustChanged()
{
    // send a message for indicating trust storage change
    CWebViewCore::SendStaticMessage(CWebViewCore::EventHub::TRUST_STORAGE_UPDATED, NULL);
}

/*
 * @param context This method expects this to be a valid context.
 */
void CWebViewClassic::SetupTrustStorageListener(
    /* [in] */ IContext* context)
{
    if (sTrustStorageListener != NULL ) {
        return;
    }
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IKeyChain::ACTION_STORAGE_CHANGED);
    sTrustStorageListener = new TrustStorageListener();
    AutoPtr<IContext> con;
    context->GetApplicationContext((IContext**)&con);
    AutoPtr<IIntent> current;
    con->RegisterReceiver(sTrustStorageListener, filter,
        (IIntent**)&current);
    if (current != NULL) {
        HandleCertTrustChanged();
    }
}

/*
 * @param context This method expects this to be a valid context
 */
void CWebViewClassic::SetupProxyListener(
    /* [in] */ IContext* context)
{
    Mutex::Autolock lock(sLock);

    if (sProxyReceiver != NULL || sNotificationsEnabled == FALSE) {
        return;
    }
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(Elastos::Droid::Net::IProxy::PROXY_CHANGE_ACTION);
    sProxyReceiver = new ProxyReceiver();
    AutoPtr<IContext> con;
    context->GetApplicationContext((IContext**)&con);
    AutoPtr<IIntent> currentProxy;
    con->RegisterReceiver(
            sProxyReceiver, filter, (IIntent**)&currentProxy);
    if (currentProxy != NULL) {
        HandleProxyBroadcast(currentProxy);
    }
}

/*
 * @param context This method expects this to be a valid context
 */
void CWebViewClassic::DisableProxyListener(
    /* [in] */ IContext* context)
{
    Mutex::Autolock lock(sLock);

    if (sProxyReceiver == NULL) {
        return;
    }

    AutoPtr<IContext> con;
    context->GetApplicationContext((IContext**)&con);
    con->UnregisterReceiver(sProxyReceiver);
    sProxyReceiver = NULL;
}

void CWebViewClassic::HandleProxyBroadcast(
    /* [in] */ IIntent* intent)
{
    AutoPtr<IInterface> obj;
    intent->GetExtra(Elastos::Droid::Net::IProxy::EXTRA_PROXY_INFO, (IInterface**)&obj);
    IProxyProperties* proxyProperties = IProxyProperties::Probe(obj);
    String host;
    if (proxyProperties == NULL || (proxyProperties->GetHost(&host), host.IsNull())) {
        CWebViewCore::SendStaticMessage(CWebViewCore::EventHub::PROXY_CHANGED, NULL);
        return;
    }
    CWebViewCore::SendStaticMessage(CWebViewCore::EventHub::PROXY_CHANGED, proxyProperties);
}

void CWebViewClassic::SetupPackageListener(
    /* [in] */ IContext* context)
{
    /*
     * we must synchronize the instance check and the creation of the
     * receiver to ensure that only ONE receiver exists for all WebView
     * instances.
     */
    {
        Mutex::Autolock lock(sLock);

        // if the receiver already exists then we do not need to register it
        // again
        if (sPackageInstallationReceiverAdded) {
            return;
        }

        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New(IIntent::ACTION_PACKAGE_ADDED, (IIntentFilter**)&filter);
        filter->AddAction(IIntent::ACTION_PACKAGE_REMOVED);
        filter->AddDataScheme(String("package"));
        AutoPtr<IBroadcastReceiver> packageListener = new PackageListener();
        AutoPtr<IContext> con;
        context->GetApplicationContext((IContext**)&con);
        AutoPtr<IIntent> intent;
        con->RegisterReceiver(
                packageListener, filter, (IIntent**)&intent);
        sPackageInstallationReceiverAdded = TRUE;
    }
    // check if any of the monitored apps are already installed
    AutoPtr<CheckAsyncTask> task = new CheckAsyncTask(this);
    task->Execute((ArrayOf<IInterface*>*)NULL);
}

void CWebViewClassic::UpdateMultiTouchSupport(
    /* [in] */ IContext* context)
{
    mZoomManager->UpdateMultiTouchSupport(context);
}

void CWebViewClassic::UpdateJavaScriptEnabled(
    /* [in] */ Boolean enabled)
{
    if (IsAccessibilityInjectionEnabled()) {
        GetAccessibilityInjector()->UpdateJavaScriptEnabled(enabled);
    }
}

void CWebViewClassic::Init()
{
    OnTrimMemoryListener::Init(mContext);
    mWebView->SetWillNotDraw(FALSE);
    mWebView->SetClickable(TRUE);
    mWebView->SetLongClickable(TRUE);

    AutoPtr<CViewConfiguration> configuration = CViewConfiguration::Get(mContext);
    Int32 slop;
    configuration->GetScaledTouchSlop(&slop);
    mTouchSlopSquare = slop * slop;
    configuration->GetScaledDoubleTapSlop(&slop);
    mDoubleTapSlopSquare = slop * slop;
    Float density = CWebViewCore::GetFixedDisplayDensity(mContext);
    // use one line height, 16 based on our current default font, for how
    // far we allow a touch be away from the edge of a link
    mNavSlop = (Int32)(16 * density);
    mZoomManager->Init(density);
    configuration->GetScaledMaximumFlingVelocity(&mMaximumFling);

    // Compute the inverse of the density squared.
    DRAG_LAYER_INVERSE_DENSITY_SQUARED = 1 / (density * density);

    configuration->GetScaledOverscrollDistance(&mOverscrollDistance);
    configuration->GetScaledOverflingDistance(&mOverflingDistance);

    Int32 scrollBarStyle;
    mWebViewPrivate->Super_getScrollBarStyle(&scrollBarStyle);
    SetScrollBarStyle(scrollBarStyle);
    // Initially use a size of two, since the user is likely to only hold
    // down two keys at a time (shift + another key)
    mKeysPressed = new Vector< AutoPtr<IInteger32> >(2);
    mHTML5VideoViewProxy = NULL;
}

ECode CWebViewClassic::ShouldDelayChildPressedState(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode CWebViewClassic::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean enabled = FALSE;
    if (mWebView->IsEnabled(&enabled), !enabled) {
        // Only default actions are supported while disabled.
        return mWebViewPrivate->Super_performAccessibilityAction(action, arguments, result);
    }

    if (GetAccessibilityInjector()->SupportsAccessibilityAction(action)) {
        *result = GetAccessibilityInjector()->PerformAccessibilityAction(action, arguments);
        return NOERROR;
    }

    switch (action) {
        case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD:
        case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD: {
            Int32 contextHeight;
            GetContentHeight(&contextHeight);
            Int32 convertedContentHeight = ContentToViewY(contextHeight);
            Int32 top, bottom;
            mWebView->GetPaddingTop(&top);
            mWebView->GetPaddingBottom(&bottom);
            Int32 adjustedViewHeight = GetHeight() - top - bottom;
            Int32 maxScrollY = Elastos::Core::Math::Max(convertedContentHeight - adjustedViewHeight, 0);
            Boolean canScrollBackward = (GetScrollY() > 0);
            Boolean canScrollForward = ((GetScrollY() - maxScrollY) > 0);
            if ((action == IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD) && canScrollBackward) {
                mWebView->ScrollBy(0, adjustedViewHeight);
                *result = TRUE;
                return NOERROR;
            }
            if ((action == IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD) && canScrollForward) {
                mWebView->ScrollBy(0, -adjustedViewHeight);
                *result = TRUE;
                return NOERROR;
            }
            *result = FALSE;
            return NOERROR;
        }
    }

    return mWebViewPrivate->Super_performAccessibilityAction(action, arguments, result);
}

ECode CWebViewClassic::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    Boolean enabled = FALSE;
    if (mWebView->IsEnabled(&enabled), !enabled) {
        // Only default actions are supported while disabled.
        return NOERROR;
    }

    info->SetScrollable(IsScrollableForAccessibility());

    Int32 contentHeight;
    GetContentHeight(&contentHeight);
    Int32 convertedContentHeight = ContentToViewY(contentHeight);
    Int32 top, bottom;
    mWebView->GetPaddingTop(&top);
    mWebView->GetPaddingBottom(&bottom);
    Int32 adjustedViewHeight = GetHeight() - top - bottom;
    Int32 maxScrollY = Elastos::Core::Math::Max(convertedContentHeight - adjustedViewHeight, 0);
    //Boolean canScrollBackward = (GetScrollY() > 0);
    Boolean canScrollForward = ((GetScrollY() - maxScrollY) > 0);

    if (canScrollForward) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
    }

    if (canScrollForward) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
    }

    GetAccessibilityInjector()->OnInitializeAccessibilityNodeInfo(info);
    return NOERROR;
}

ECode CWebViewClassic::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    event->SetScrollable(IsScrollableForAccessibility());
    event->SetScrollX(GetScrollX());
    event->SetScrollY(GetScrollY());
    Int32 contentWidth;
    GetContentWidth(&contentWidth);
    Int32 convertedContentWidth = ContentToViewX(contentWidth);
    Int32 left;
    mWebView->GetPaddingLeft(&left);
    Int32 adjustedViewWidth = GetWidth() - left - left;
    event->SetMaxScrollX(Elastos::Core::Math::Max(convertedContentWidth - adjustedViewWidth, 0));
    Int32 contentHeight;
    GetContentHeight(&contentHeight);
    Int32 convertedContentHeight = ContentToViewY(contentHeight);
    Int32 top, bottom;
    mWebView->GetPaddingTop(&top);
    mWebView->GetPaddingBottom(&bottom);
    Int32 adjustedViewHeight = GetHeight() - top - bottom;
    event->SetMaxScrollY(Elastos::Core::Math::Max(convertedContentHeight - adjustedViewHeight, 0));
    return NOERROR;
}

Boolean CWebViewClassic::IsAccessibilityInjectionEnabled()
{
    // TODO: CAccessibilityManager not ready
    return FALSE;

    AutoPtr<IAccessibilityManager> manager;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    Boolean enabled;
    if (manager->IsEnabled(&enabled), !enabled) {
        return FALSE;
    }

    // Accessibility scripts should be injected only when a speaking service
    // is enabled. This may need to change later to accommodate Braille.
    AutoPtr<IObjectContainer> services;
    manager->GetEnabledAccessibilityServiceList(
            IAccessibilityServiceInfo::FEEDBACK_SPOKEN, (IObjectContainer**)&services);
    Int32 count;
    services->GetObjectCount(&count);
    if (count == 0) {
        return FALSE;
    }

    return TRUE;
}

AutoPtr<AccessibilityInjector> CWebViewClassic::GetAccessibilityInjector()
{
    if (mAccessibilityInjector == NULL) {
        mAccessibilityInjector = new AccessibilityInjector(this);
    }
    return mAccessibilityInjector;
}

Boolean CWebViewClassic::IsScrollableForAccessibility()
{
    Int32 left, right, bottom, top;
    mWebView->GetPaddingLeft(&left);
    mWebView->GetPaddingRight(&right);
    mWebView->GetPaddingTop(&top);
    mWebView->GetPaddingBottom(&bottom);
    Int32 contentHeight, contentWidth;
    return (GetContentWidth(&contentWidth), ContentToViewX(contentWidth) > GetWidth() - left - right
            || (GetContentHeight(&contentHeight), ContentToViewY(contentHeight) > GetHeight() - top - bottom));
}

ECode CWebViewClassic::SetOverScrollMode(
    /* [in] */ Int32 mode)
{
    if (mode != IView::OVER_SCROLL_NEVER) {
        if (mOverScrollGlow == NULL) {
            mOverScrollGlow = new OverScrollGlow(this);
        }
    }
    else {
        mOverScrollGlow = NULL;
    }
    return NOERROR;
}

void CWebViewClassic::AdjustDefaultZoomDensity(
    /* [in] */ Int32 zoomDensity)
{
    Float density = CWebViewCore::GetFixedDisplayDensity(mContext)
            * 100 / zoomDensity;
    UpdateDefaultZoomDensity(density);
}

void CWebViewClassic::UpdateDefaultZoomDensity(
    /* [in] */ Float density)
{
    mNavSlop = (Int32)(16 * density);
    mZoomManager->UpdateDefaultZoomDensity(density);
}

Int32 CWebViewClassic::GetScaledNavSlop()
{
    return ViewToContentDimension(mNavSlop);
}

Boolean CWebViewClassic::OnSavePassword(
    /* [in] */ const String& schemePlusHost,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ IMessage* resumeMsg)
{
    Boolean rVal = FALSE;
    if (resumeMsg == NULL) {
        // null resumeMsg implies saving password silently
        mDatabase->SetUsernamePassword(schemePlusHost, username, password);
    }
    else {
        if (mResumeMsg != NULL) {
            Logger::W(LOGTAG, "onSavePassword should not be called while dialog is up");
            resumeMsg->SendToTarget();
            return TRUE;
        }
        mResumeMsg = resumeMsg;
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> remember;
        helper->Obtain(mPrivateHandler, REMEMBER_PASSWORD, (IMessage**)&remember);
        AutoPtr<IBundle> data;
        remember->GetData((IBundle**)&data);
        data->PutString(String("host"), schemePlusHost);
        data->PutString(String("username"), username);
        data->PutString(String("password"), password);
        remember->SetObj(resumeMsg);

        AutoPtr<IMessage> neverRemember;
        helper->Obtain(mPrivateHandler, NEVER_REMEMBER_PASSWORD, (IMessage**)&neverRemember);
        data = NULL;
        neverRemember->GetData((IBundle**)&data);
        data->PutString(String("host"), schemePlusHost);
        data->PutString(String("username"), username);
        data->PutString(String("password"), password);
        neverRemember->SetObj(resumeMsg);

        AutoPtr<IAlertDialogBuilder> builder;
        CAlertDialogBuilder::New(mContext, (IAlertDialogBuilder**)&builder);
        builder->SetTitle(R::string::save_password_label);
        builder->SetMessage(R::string::save_password_message);
        AutoPtr<IDialogInterfaceOnClickListener> listener = new SavePasswordButtonClickListener(this, resumeMsg);
        builder->SetPositiveButton(R::string::save_password_notnow, listener);
        listener = new SavePasswordButtonClickListener(this, remember);
        builder->SetNeutralButton(R::string::save_password_remember, listener);
        listener = new SavePasswordButtonClickListener(this, neverRemember);
        builder->SetNegativeButton(R::string::save_password_never, listener);
        AutoPtr<IDialogInterfaceOnDismissListener> dismissListener = new SavePasswordDismissListener(this, resumeMsg);
        builder->SetOnDismissListener(dismissListener);
        builder->Show((IAlertDialog**)&mSavePasswordDialog);
        // Return true so that WebViewCore will pause while the dialog is
        // up.
        rVal = TRUE;
    }
    return rVal;
}

ECode CWebViewClassic::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    if (style == IView::SCROLLBARS_INSIDE_INSET
            || style == IView::SCROLLBARS_OUTSIDE_INSET) {
        mOverlayHorizontalScrollbar = mOverlayVerticalScrollbar = FALSE;
    }
    else {
        mOverlayHorizontalScrollbar = mOverlayVerticalScrollbar = TRUE;
    }
    return NOERROR;
}

/**
 * See {@link WebView#setHorizontalScrollbarOverlay(boolean)}
 */
ECode CWebViewClassic::SetHorizontalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    mOverlayHorizontalScrollbar = overlay;
    return NOERROR;
}

/**
 * See {@link WebView#setVerticalScrollbarOverlay(boolean)
 */
ECode CWebViewClassic::SetVerticalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    mOverlayVerticalScrollbar = overlay;
    return NOERROR;
}

/**
 * See {@link WebView#overlayHorizontalScrollbar()}
 */
ECode CWebViewClassic::OverlayHorizontalScrollbar(
    /* [out] */ Boolean* scrollbar)
{
    VALIDATE_NOT_NULL(scrollbar);
    *scrollbar = mOverlayHorizontalScrollbar;
    return NOERROR;
}

/**
 * See {@link WebView#overlayVerticalScrollbar()}
 */
ECode CWebViewClassic::OverlayVerticalScrollbar(
    /* [out] */ Boolean* scrollbar)
{
    VALIDATE_NOT_NULL(scrollbar);
    *scrollbar = mOverlayVerticalScrollbar;
    return NOERROR;
}

/*
 * Return the width of the view where the content of WebView should render
 * to.
 * Note: this can be called from WebCoreThread.
 */
Int32 CWebViewClassic::GetViewWidth()
{
    Boolean enabled = FALSE;
    if ((mWebView->IsVerticalScrollBarEnabled(&enabled), !enabled) || mOverlayVerticalScrollbar) {
        return GetWidth();
    }
    else {
        Int32 width = 0;
        mWebView->GetVerticalScrollbarWidth(&width);
        return Elastos::Core::Math::Max(0, GetWidth() - width);
    }
}

/**
 * Returns the height (in pixels) of the embedded title bar (if any). Does not care about
 * scrolling
 */
Int32 CWebViewClassic::GetTitleHeight()
{
    if (IWebViewClassicTitleBarDelegate::Probe(mWebView) != NULL) {
        Int32 height;
        IWebViewClassicTitleBarDelegate::Probe(mWebView)->GetTitleHeight(&height);
        return height;
    }
    return 0;
}

/**
 * See {@link WebView#getVisibleTitleHeight()}
 */
ECode CWebViewClassic::GetVisibleTitleHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    // Actually, this method returns the height of the embedded title bar if one is set via the
    // hidden setEmbeddedTitleBar method.
    *height = GetVisibleTitleHeightImpl();
    return NOERROR;
}

Int32 CWebViewClassic::GetVisibleTitleHeightImpl()
{
    // need to restrict mScrollY due to over scroll
    return Elastos::Core::Math::Max(GetTitleHeight() - Elastos::Core::Math::Max(0, GetScrollY()),
            GetOverlappingActionModeHeight());
}

Int32 CWebViewClassic::GetOverlappingActionModeHeight()
{
    if (mFindCallback == NULL) {
        return 0;
    }
    if (mCachedOverlappingActionModeHeight < 0) {
        Boolean result;
        mWebView->GetGlobalVisibleRect(mGlobalVisibleRect, mGlobalVisibleOffset, &result);
        Int32 top;
        mGlobalVisibleRect->GetTop(&top);
        mCachedOverlappingActionModeHeight = Elastos::Core::Math::Max(0,
                mFindCallback->GetActionModeGlobalBottom() - top);
    }
    return mCachedOverlappingActionModeHeight;
}

/*
 * Return the height of the view where the content of WebView should render
 * to.  Note that this excludes mTitleBar, if there is one.
 * Note: this can be called from WebCoreThread.
 */
/* package */
Int32 CWebViewClassic::GetViewHeight()
{
    return GetViewHeightWithTitle() - GetVisibleTitleHeightImpl();
}

Int32 CWebViewClassic::GetViewHeightWithTitle()
{
    Int32 height = GetHeight();
    Boolean enabled = FALSE;
    if ((mWebView->IsHorizontalScrollBarEnabled(&enabled), enabled) && !mOverlayHorizontalScrollbar) {
        Int32 barheight;
        mWebViewPrivate->GetHorizontalScrollbarHeight(&barheight);
        height -= barheight;
    }
    return height;
}

/**
 * See {@link WebView#getCertificate()}
 */
ECode CWebViewClassic::GetCertificate(
    /* [out] */ ISslCertificate** cer)
{
    VALIDATE_NOT_NULL(cer);
    *cer = mCertificate;
    REFCOUNT_ADD(*cer);
    return NOERROR;
}

/**
 * See {@link WebView#setCertificate(SslCertificate)}
 */
ECode CWebViewClassic::SetCertificate(
    /* [in] */ ISslCertificate* certificate)
{
    if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "setCertificate=%p", certificate);
    }
    // here, the certificate can be null (if the site is not secure)
    mCertificate = certificate;
    return NOERROR;
}

//-------------------------------------------------------------------------
// Methods called by activity
//-------------------------------------------------------------------------

/**
 * See {@link WebView#savePassword(String, String, String)}
 */
ECode CWebViewClassic::SavePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    mDatabase->SetUsernamePassword(host, username, password);
    return NOERROR;
}

/**
 * See {@link WebView#setHttpAuthUsernamePassword(String, String, String, String)}
 */
ECode CWebViewClassic::SetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    mDatabase->SetHttpAuthUsernamePassword(host, realm, username, password);
    return NOERROR;
}

/**
 * See {@link WebView#getHttpAuthUsernamePassword(String, String)}
 */
ECode CWebViewClassic::GetHttpAuthUsernamePassword(
    /* [in] */ const String& host,
    /* [in] */ const String& realm,
    /* [out, callee] */ ArrayOf<String>** up)
{
    VALIDATE_NOT_NULL(up);
    AutoPtr< ArrayOf<String> > password =
            mDatabase->GetHttpAuthUsernamePassword(host, realm);
    *up = password;
    ARRAYOF_ADDREF(*up);
    return NOERROR;
}

/**
 * Remove Find or Select ActionModes, if active.
 */
void CWebViewClassic::ClearActionModes()
{
    if (mSelectCallback != NULL) {
        mSelectCallback->Finish();
    }
    if (mFindCallback != NULL) {
        mFindCallback->Finish();
    }
}

/**
 * Called to clear state when moving from one page to another, or changing
 * in some other way that makes elements associated with the current page
 * (such as ActionModes) no longer relevant.
 */
void CWebViewClassic::ClearHelpers()
{
    HideSoftKeyboard();
    ClearActionModes();
    DismissFullScreenMode();
    CancelDialogs();
}

void CWebViewClassic::CancelDialogs()
{
    if (mListBoxDialog != NULL) {
        mListBoxDialog->Cancel();
        mListBoxDialog = NULL;
    }
    if (mSavePasswordDialog != NULL) {
        mSavePasswordDialog->Dismiss();
        mSavePasswordDialog = NULL;
    }
}

/**
 * See {@link WebView#destroy()}
 */
ECode CWebViewClassic::Destroy()
{
    Elastos::Droid::View::View* view = reinterpret_cast<Elastos::Droid::View::View*>(mWebView->Probe(EIID_View));
    if (view->GetViewRootImpl() != NULL) {
        Logger::E(LOGTAG, "Error: WebView.destroy() called while still attached!");
    }
    EnsureFunctorDetached();
    DestroyCar();
    DestroyNative();
    return NOERROR;
}

void CWebViewClassic::EnsureFunctorDetached()
{
    Boolean accelerated = FALSE;
    mWebView->IsHardwareAccelerated(&accelerated);
    if (accelerated) {
        Int32 drawGLFunction = NativeGetDrawGLFunction(mNativeClass);
        Elastos::Droid::View::View* view = reinterpret_cast<Elastos::Droid::View::View*>(mWebView->Probe(EIID_View));
        AutoPtr<ViewRootImpl> viewRoot = view->GetViewRootImpl();
        if (drawGLFunction != 0 && viewRoot != NULL) {
            viewRoot->DetachFunctor(drawGLFunction);
        }
    }
}

void CWebViewClassic::DestroyCar()
{
    mCallbackProxy->BlockMessages();
    if (mAccessibilityInjector != NULL) {
        mAccessibilityInjector->Destroy();
        mAccessibilityInjector = NULL;
    }
    if (mWebViewCore != NULL) {
        // Tell WebViewCore to destroy itself
        {
            Mutex::Autolock lock(mLock);

            AutoPtr<CWebViewCore> webViewCore = mWebViewCore;
            mWebViewCore = NULL; // prevent using partial webViewCore
            webViewCore->Destroy();
        }
        // Remove any pending messages that might not be serviced yet.
        mPrivateHandler->RemoveCallbacksAndMessages(NULL);
    }
}

void CWebViewClassic::DestroyNative()
{
    if (mNativeClass == 0) return;
    Int32 nptr = mNativeClass;
    mNativeClass = 0;
    AutoPtr<IThread> current = Thread::GetCurrentThread();
    AutoPtr<ILooper> l;
    mPrivateHandler->GetLooper((ILooper**)&l);
    AutoPtr<IThread> t;
    l->GetThread((IThread**)&t);
    if (current == t) {
        // We are on the main thread and can safely delete
        NativeDestroy(nptr);
    }
    else {
        AutoPtr<IRunnable> r = new DestroyNativeRunnable(nptr);
        Boolean result;
        mPrivateHandler->Post(r, &result);
    }
}

/**
 * See {@link WebView#enablePlatformNotifications()}
 */
void CWebViewClassic::EnablePlatformNotifications()
{
    Mutex::Autolock lock(sLock);

    sNotificationsEnabled = TRUE;
    AutoPtr<IContext> context = NativeUtil::GetContext();
    if (context != NULL) {
        SetupProxyListener(context);
    }
}

/**
 * See {@link WebView#disablePlatformNotifications()}
 */
void CWebViewClassic::DisablePlatformNotifications()
{
    Mutex::Autolock lock(lock);

    sNotificationsEnabled = FALSE;
    AutoPtr<IContext> context = NativeUtil::GetContext();
    if (context != NULL) {
        DisableProxyListener(context);
    }
}

/**
 * Sets JavaScript engine flags.
 *
 * @param flags JS engine flags in a String
 *
 * This is an implementation detail.
 */
ECode CWebViewClassic::SetJsFlags(
    /* [in] */ const String& flags)
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(flags, (ICharSequence**)&cs);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_JS_FLAGS, cs);
    return NOERROR;
}

/**
 * See {@link WebView#setNetworkAvailable(boolean)}
 */
ECode CWebViewClassic::SetNetworkAvailable(
    /* [in] */ Boolean networkUp)
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_NETWORK_STATE,
            networkUp ? 1 : 0, 0);
    return NOERROR;
}

/**
 * Inform WebView about the current network type.
 */
ECode CWebViewClassic::SetNetworkType(
    /* [in] */ const String& type,
    /* [in] */ const String& subtype)
{
    AutoPtr<IMap> map;
    CHashMap::New((IMap**)&map);
    AutoPtr<ICharSequence> key, value;
    CStringWrapper::New(String("type"), (ICharSequence**)&key);
    CStringWrapper::New(type, (ICharSequence**)&value);
    AutoPtr<IInterface> oldValue;
    map->Put(key, value, (IInterface**)&oldValue);
    key = NULL;
    CStringWrapper::New(String("subtype"), (ICharSequence**)&key);
    value = NULL;
    CStringWrapper::New(subtype, (ICharSequence**)&value);
    oldValue = NULL;
    map->Put(key, value, (IInterface**)&oldValue);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_NETWORK_TYPE, map);
    return NOERROR;
}

/**
 * See {@link WebView#saveState(Bundle)}
 */
ECode CWebViewClassic::SaveState(
    /* [in] */ IBundle* outState,
    /* [out] */ IWebBackForwardList** wbfl)
{
    VALIDATE_NOT_NULL(wbfl);

    if (outState == NULL) {
        *wbfl = NULL;
        return NOERROR;
    }

    // We grab a copy of the back/forward list because a client of WebView
    // may have invalidated the history list by calling clearHistory.
    AutoPtr<IWebBackForwardList> l;
    CopyBackForwardList((IWebBackForwardList**)&l);
    WebBackForwardListClassic* list = (WebBackForwardListClassic*)l.Get();
    Int32 currentIndex, size;
    list->GetCurrentIndex(&currentIndex);
    list->GetSize(&size);
    // We should fail saving the state if the list is empty or the index is
    // not in a valid range.
    if (currentIndex < 0 || currentIndex >= size || size == 0) {
        *wbfl = NULL;
        return NOERROR;
    }

    outState->PutInt32(String("index"), currentIndex);
    // FIXME: This should just be a byte[][] instead of ArrayList but
    // Parcel.java does not have the code to handle multi-dimensional
    // arrays.
    AutoPtr<IArrayList> history;
    CArrayList::New(size, (IArrayList**)&history);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<WebHistoryItemClassic> item = list->GetItemAtIndex(i);
        if (NULL == item) {
            // FIXME: this shouldn't happen
            // need to determine how item got set to null
            Logger::W(LOGTAG, "saveState: Unexpected null history item.");
            *wbfl = NULL;
            return NOERROR;
        }
        AutoPtr< ArrayOf<Byte> > data = item->GetFlattenedData();
        if (data == NULL) {
            // It would be very odd to not have any data for a given history
            // item. And we will fail to rebuild the history list without
            // flattened data.
            *wbfl = NULL;
            return NOERROR;
        }
        AutoPtr<IArrayOf> dataWrapper;
        CArrayOf::New(EIID_IByte, data->GetLength(), (IArrayOf**)&dataWrapper);
        for (Int32 i = 0; i < data->GetLength(); i++) {
            AutoPtr<IByte> bo;
            CByte::New((*data)[i], (IByte**)&bo);
            dataWrapper->Put(i, bo);
        }
        Boolean result;
        history->Add(dataWrapper, &result);
    }
    outState->PutSerializable(String("history"), ISerializable::Probe(history));
    if (mCertificate != NULL) {
        AutoPtr<ISslCertificateHelper> helper;
        assert(0);
        // CSslCertificateHelper::AcquireSingleton((ISslCertificateHelper**)&helper);
        AutoPtr<IBundle> bundle;
        helper->SaveState(mCertificate, (IBundle**)&bundle);
        outState->PutBundle(String("certificate"), bundle);
    }
    Boolean enabled = FALSE;
    IsPrivateBrowsingEnabled(&enabled);
    outState->PutBoolean(String("privateBrowsingEnabled"), enabled);
    mZoomManager->SaveZoomState(outState);
    *wbfl = list;
    REFCOUNT_ADD(*wbfl);
    return NOERROR;
}

class SavePictureRunnable : public Runnable
{
public:
    SavePictureRunnable(
        /* [in] */ IFile* temp,
        /* [in] */ IPicture* p,
        /* [in] */ IFile* dest)
        : mTemp(temp)
        , mPicture(p)
        , mDest(dest)
    {}

    CARAPI Run()
    {
        AutoPtr<IFileOutputStream> out;
        // try {
        CFileOutputStream::New(mTemp, (IFileOutputStream**)&out);
        ECode ec = mPicture->WriteToStream(out);
        // Writing the picture succeeded, rename the temporary file
        // to the destination.
        Boolean result;
        if (SUCCEEDED(ec)) {
            mTemp->RenameTo(mDest, &result);
        }
        if (out != NULL) {
            // try {
            out->Close();
            // } catch (Exception e) {
            //     // Can't do anything about that
            // }
        }
        mTemp->Delete(&result);
        // } catch (Exception e) {
        //     // too late to do anything about it.
        // } finally {
        //     if (out != null) {
        //         try {
        //             out.close();
        //         } catch (Exception e) {
        //             // Can't do anything about that
        //         }
        //     }
        //     temp.delete();
        // }
        return NOERROR;
    }

public:
    AutoPtr<IFile> mTemp;
    AutoPtr<IPicture> mPicture;
    AutoPtr<IFile> mDest;
};

/**
 * See {@link WebView#savePicture(Bundle, File)}
 */
ECode CWebViewClassic::SavePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* dest,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (dest == NULL || b == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IPicture> p;
    CapturePicture((IPicture**)&p);
    // Use a temporary file while writing to ensure the destination file
    // contains valid data.
    String path;
    dest->GetPath(&path);
    path += ".writing";
    AutoPtr<IFile> temp;
    CFile::New(path, (IFile**)&temp);
    AutoPtr<IRunnable> r = new SavePictureRunnable(temp, p, dest);
    AutoPtr<IThread> t;
    CThread::New(r, (IThread**)&t);
    t->Start();
    // now update the bundle
    b->PutInt32(String("scrollX"), GetScrollX());
    b->PutInt32(String("scrollY"), GetScrollY());
    mZoomManager->SaveZoomState(b);
    *result = TRUE;
    return NOERROR;
}

void CWebViewClassic::RestoreHistoryPictureFields(
    /* [in] */ IPicture* p,
    /* [in] */ IBundle* b)
{
    Int32 sx, sy;
    b->GetInt32(String("scrollX"), 0, &sx);
    b->GetInt32(String("scrollY"), 0, &sy);

    mDrawHistory = TRUE;
    mHistoryPicture = p;

    SetScrollXRaw(sx);
    SetScrollYRaw(sy);
    mZoomManager->RestoreZoomState(b);
    Float scale = mZoomManager->GetScale();
    Int32 with, height;
    p->GetWidth(&with);
    p->GetHeight(&height);
    mHistoryWidth = Elastos::Core::Math::Round(with * scale);
    mHistoryHeight = Elastos::Core::Math::Round(height * scale);

    Invalidate();
}

/**
 * See {@link WebView#restorePicture(Bundle, File)};
 */
ECode CWebViewClassic::RestorePicture(
    /* [in] */ IBundle* b,
    /* [in] */ IFile* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (src == NULL || b == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    Boolean exist = FALSE;
    if (src->Exists(&exist), !exist) {
        *result = FALSE;
        return NOERROR;
    }
    // try {
    AutoPtr<IFileInputStream> in;
    CFileInputStream::New(src, (IFileInputStream**)&in);
    AutoPtr<IBundle> copy;
    CBundle::New(b, (IBundle**)&copy);
    AutoPtr<IRunnable> r = new RestorePictureRunnable(this, in, copy);
    AutoPtr<IThread> t;
    CThread::New(r, (IThread**)&t);
    t->Start();
    // } catch (FileNotFoundException e){
    //     e.printStackTrace();
    // }
    *result = TRUE;
    return NOERROR;
}

/**
 * Saves the view data to the output stream. The output is highly
 * version specific, and may not be able to be loaded by newer versions
 * of WebView.
 * @param stream The {@link OutputStream} to save to
 * @param callback The {@link ValueCallback} to call with the result
 */
ECode CWebViewClassic::SaveViewState(
    /* [in] */ IOutputStream* stream,
    /* [in] */ IValueCallback* callback)
{
    if (mWebViewCore == NULL) {
        callback->OnReceiveValue(FALSE);
        return NOERROR;
    }
    AutoPtr<IInterface> obj = new CWebViewCore::SaveViewStateRequest(stream, callback);
    mWebViewCore->SendMessageAtFrontOfQueue(CWebViewCore::EventHub::SAVE_VIEW_STATE, obj);
    return NOERROR;
}

/**
 * Loads the view data from the input stream. See
 * {@link #saveViewState(java.io.OutputStream, ValueCallback)} for more information.
 * @param stream The {@link InputStream} to load from
 */
ECode CWebViewClassic::LoadViewState(
    /* [in] */ IInputStream* stream)
{
    mBlockWebkitViewMessages = TRUE;
    AutoPtr<LoadViewStateAsyncTask> task = new LoadViewStateAsyncTask(this);
    AutoPtr< ArrayOf<IInterface*> > params = ArrayOf<IInterface*>::Alloc(1);
    params->Set(0, stream);
    task->Execute(params);
    return NOERROR;
}

/**
 * Clears the view state set with {@link #loadViewState(InputStream)}.
 * This WebView will then switch to showing the content from webkit
 */
ECode CWebViewClassic::ClearViewState()
{
    mBlockWebkitViewMessages = FALSE;
    mLoadedPicture = NULL;
    Invalidate();
    return NOERROR;
}

/**
 * See {@link WebView#restoreState(Bundle)}
 */
ECode CWebViewClassic::RestoreState(
    /* [in] */ IBundle* inState,
    /* [out] */ IWebBackForwardList** wbfl)
{
    VALIDATE_NOT_NULL(wbfl);
    AutoPtr<WebBackForwardListClassic> returnList;
    if (inState == NULL) {
        *wbfl = NULL;
        return NOERROR;
    }
    Boolean result;
    if ((inState->ContainsKey(String("index"), &result), result) &&
        (inState->ContainsKey(String("history"), &result), result)) {
        assert(0);
        // mCertificate = SslCertificate.restoreState(
        //     inState.getBundle("certificate"));

        AutoPtr<WebBackForwardListClassic> list = mCallbackProxy->GetBackForwardList();
        Int32 index;
        inState->GetInt32(String("index"), &index);
        // We can't use a clone of the list because we need to modify the
        // shared copy, so synchronize instead to prevent concurrent
        // modifications.
        {
            Mutex::Autolock lock(list->mLock);

            AutoPtr<ISerializable> obj;
            inState->GetSerializable(String("history"), (ISerializable**)&obj);
            AutoPtr<IList> history = IList::Probe(obj);
            Int32 size;
            history->GetSize(&size);
            // Check the index bounds so we don't crash in native code while
            // restoring the history index.
            if (index < 0 || index >= size) {
                *wbfl = NULL;
                return NOERROR;
            }
            for (Int32 i = 0; i < size; i++) {
                AutoPtr<IInterface> e;
                history->Remove(0, (IInterface**)&e);
                AutoPtr<IArrayOf> data = IArrayOf::Probe(e);
                if (data == NULL) {
                    // If we somehow have null data, we cannot reconstruct
                    // the item and thus our history list cannot be rebuilt.
                    *wbfl = NULL;
                    return NOERROR;
                }
                Int32 N;
                data->GetLength(&N);
                AutoPtr< ArrayOf<Byte> > d = ArrayOf<Byte>::Alloc(N);
                for (Int32 j = 0; j < N; j++) {
                    AutoPtr<IInterface> o;
                    data->Get(j, (IInterface**)&o);
                    Byte v;
                    IByte::Probe(o)->GetValue(&v);
                    (*d)[j] = v;
                }
                AutoPtr<IWebHistoryItem> item = new WebHistoryItemClassic(d);
                list->AddHistoryItem(item);
            }
            // Grab the most recent copy to return to the caller.
            AutoPtr<IWebBackForwardList> l;
            CopyBackForwardList((IWebBackForwardList**)&l);
            returnList = (WebBackForwardListClassic*)l.Get();
            // Update the copy to have the correct index.
            returnList->SetCurrentIndex(index);
        }
        // Restore private browsing setting.
        Boolean enabled;
        if (inState->GetBoolean(String("privateBrowsingEnabled"), &enabled), enabled) {
            AutoPtr<IWebSettings> settings;
            GetSettings((IWebSettings**)&settings);
            ((WebSettingsClassic*)settings.Get())->SetPrivateBrowsingEnabled(TRUE);
        }
        mZoomManager->RestoreZoomState(inState);
        // Remove all pending messages because we are restoring previous
        // state.
        mWebViewCore->RemoveMessages();
        if (IsAccessibilityInjectionEnabled()) {
            GetAccessibilityInjector()->AddAccessibilityApisIfNecessary();
        }
        // Send a restore state message.
        mWebViewCore->SendMessage(CWebViewCore::EventHub::RESTORE_STATE, index);
    }
    *wbfl = (IWebBackForwardList*)returnList.Get();
    REFCOUNT_ADD(*wbfl);
    return NOERROR;
}

/**
 * See {@link WebView#loadUrl(String, Map)}
 */
ECode CWebViewClassic::LoadUrl(
    /* [in] */ const String& url,
    /* [in] */ IMap* additionalHttpHeaders)
{
    LoadUrlImpl(url, additionalHttpHeaders);
    return NOERROR;
}

void CWebViewClassic::LoadUrlImpl(
    /* [in] */ const String& url,
    /* [in] */ IMap* extraHeaders)
{
    SwitchOutDrawHistory();
    AutoPtr<CWebViewCore::GetUrlData> arg = new CWebViewCore::GetUrlData();
    arg->mUrl = url;
    arg->mExtraHeaders = extraHeaders;
    mWebViewCore->SendMessage(CWebViewCore::EventHub::LOAD_URL, (IInterface*)arg.Get());
    ClearHelpers();
}

/**
 * See {@link WebView#loadUrl(String)}
 */
ECode CWebViewClassic::LoadUrl(
    /* [in] */ const String& url)
{
    LoadUrlImpl(url);
    return NOERROR;
}

void CWebViewClassic::LoadUrlImpl(
    /* [in] */ const String& url)
{
    if (url.IsNull()) {
        return;
    }
    LoadUrlImpl(url, NULL);
}

/**
 * See {@link WebView#postUrl(String, byte[])}
 */
ECode CWebViewClassic::PostUrl(
    /* [in] */ const String& url,
    /* [in] */ ArrayOf<Byte>* postData)
{
    if (URLUtil::IsNetworkUrl(url)) {
        SwitchOutDrawHistory();
        AutoPtr<CWebViewCore::PostUrlData> arg = new CWebViewCore::PostUrlData();
        arg->mUrl = url;
        arg->mPostData = postData;
        mWebViewCore->SendMessage(CWebViewCore::EventHub::POST_URL, (IInterface*)arg.Get());
        ClearHelpers();
    }
    else {
        LoadUrlImpl(url);
    }
    return NOERROR;
}

/**
 * See {@link WebView#loadData(String, String, String)}
 */
ECode CWebViewClassic::LoadData(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding)
{
    LoadDataImpl(data, mimeType, encoding);
    return NOERROR;
}

void CWebViewClassic::LoadDataImpl(
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding)
{
    StringBuilder dataUrl("data:");
    dataUrl.AppendString(mimeType);
    if (encoding.Equals("base64")) {
        dataUrl.AppendCStr(";base64");
    }
    dataUrl.AppendCStr(",");
    dataUrl.AppendString(data);
    LoadUrlImpl(dataUrl.ToString());
}

/**
 * See {@link WebView#loadDataWithBaseURL(String, String, String, String, String)}
 */
ECode CWebViewClassic::LoadDataWithBaseURL(
    /* [in] */ const String& baseUrl,
    /* [in] */ const String& data,
    /* [in] */ const String& mimeType,
    /* [in] */ const String& encoding,
    /* [in] */ const String& historyUrl)
{
    if (!baseUrl.IsNull() && baseUrl.ToLowerCase().StartWith("data:")) {
        LoadDataImpl(data, mimeType, encoding);
        return NOERROR;
    }
    SwitchOutDrawHistory();
    AutoPtr<CWebViewCore::BaseUrlData> arg = new CWebViewCore::BaseUrlData();
    arg->mBaseUrl = baseUrl;
    arg->mData = data;
    arg->mMimeType = mimeType;
    arg->mEncoding = encoding;
    arg->mHistoryUrl = historyUrl;
    mWebViewCore->SendMessage(CWebViewCore::EventHub::LOAD_DATA, (IInterface*)arg.Get());
    ClearHelpers();
    return NOERROR;
}

/**
 * See {@link WebView#saveWebArchive(String)}
 */
ECode CWebViewClassic::SaveWebArchive(
    /* [in] */ const String& filename)
{
    SaveWebArchiveImpl(filename, FALSE, NULL);
    return NOERROR;
}

/**
 * See {@link WebView#saveWebArchive(String, boolean, ValueCallback)}
 */
ECode CWebViewClassic::SaveWebArchive(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname,
    /* [in] */ IValueCallback* callback)
{
    SaveWebArchiveImpl(basename, autoname, callback);
    return NOERROR;
}

void CWebViewClassic::SaveWebArchiveImpl(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname,
    /* [in] */ IValueCallback* callback)
{
    AutoPtr<SaveWebArchiveMessage> m = new SaveWebArchiveMessage(basename, autoname, callback);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SAVE_WEBARCHIVE, (IInterface*)m.Get());
}

/**
 * See {@link WebView#stopLoading()}
 */
ECode CWebViewClassic::StopLoading()
{
    // TODO: should we clear all the messages in the queue before sending
    // STOP_LOADING?
    SwitchOutDrawHistory();
    mWebViewCore->SendMessage(CWebViewCore::EventHub::STOP_LOADING);
    return NOERROR;
}

/**
 * See {@link WebView#reload()}
 */
ECode CWebViewClassic::Reload()
{
    ClearHelpers();
    SwitchOutDrawHistory();
    mWebViewCore->SendMessage(CWebViewCore::EventHub::RELOAD);
    return NOERROR;
}

/**
 * See {@link WebView#canGoBack()}
 */
ECode CWebViewClassic::CanGoBack(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<WebBackForwardListClassic> l = mCallbackProxy->GetBackForwardList();
    {
        Mutex::Autolock lock(l->mLock);

        if (l->GetClearPending()) {
            *result = FALSE;
            return NOERROR;
        }
        else {
            Int32 index;
            l->GetCurrentIndex(&index);
            *result = index > 0;
            return NOERROR;
        }
    }
}

/**
 * See {@link WebView#goBack()}
 */
ECode CWebViewClassic::GoBack()
{
    GoBackOrForwardImpl(-1);
    return NOERROR;
}

/**
 * See {@link WebView#canGoForward()}
 */
ECode CWebViewClassic::CanGoForward(
    /* [out] */ Boolean* result)
{
    AutoPtr<WebBackForwardListClassic> l = mCallbackProxy->GetBackForwardList();
    {
        Mutex::Autolock lock(l->mLock);

        if (l->GetClearPending()) {
            *result = FALSE;
            return NOERROR;
        }
        else {
            Int32 index, size;
            l->GetCurrentIndex(&index);
            l->GetSize(&size);
            *result = index < size - 1;
            return NOERROR;
        }
    }
}

/**
 * See {@link WebView#goForward()}
 */
ECode CWebViewClassic::GoForward()
{
    GoBackOrForwardImpl(1);
    return NOERROR;
}

/**
 * See {@link WebView#canGoBackOrForward(int)}
 */
ECode CWebViewClassic::CanGoBackOrForward(
    /* [in] */ Int32 steps,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<WebBackForwardListClassic> l = mCallbackProxy->GetBackForwardList();
    {
        Mutex::Autolock lock(l->mLock);

        if (l->GetClearPending()) {
            *result = FALSE;
            return NOERROR;
        }
        else {
            Int32 index, size;
            l->GetCurrentIndex(&index);
            Int32 newIndex = index + steps;
            l->GetSize(&size);
            *result = newIndex >= 0 && newIndex < size;
            return NOERROR;
        }
    }
}

/**
 * See {@link WebView#goBackOrForward(int)}
 */
ECode CWebViewClassic::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    GoBackOrForwardImpl(steps);
    return NOERROR;
}

void CWebViewClassic::GoBackOrForwardImpl(
    /* [in] */ Int32 steps)
{
    GoBackOrForward(steps, FALSE);
}

void CWebViewClassic::GoBackOrForward(
    /* [in] */ Int32 steps,
    /* [in] */ Boolean ignoreSnapshot)
{
    if (steps != 0) {
        ClearHelpers();
        mWebViewCore->SendMessage(CWebViewCore::EventHub::GO_BACK_FORWARD, steps,
                ignoreSnapshot ? 1 : 0);
    }
}

/**
 * See {@link WebView#isPrivateBrowsingEnabled()}
 */
ECode CWebViewClassic::IsPrivateBrowsingEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IWebSettings> s;
    GetSettings((IWebSettings**)&s);
    AutoPtr<WebSettingsClassic> settings = (WebSettingsClassic*)s.Get();
    *result = (settings != NULL) ? settings->IsPrivateBrowsingEnabled() : FALSE;
    return NOERROR;
}

void CWebViewClassic::StartPrivateBrowsing()
{
    AutoPtr<IWebSettings> s;
    GetSettings((IWebSettings**)&s);
    AutoPtr<WebSettingsClassic> settings = (WebSettingsClassic*)s.Get();
    settings->SetPrivateBrowsingEnabled(TRUE);
}

Boolean CWebViewClassic::ExtendScroll(
    /* [in] */ Int32 y)
{
    Int32 finalY = mScroller->GetFinalY();
    Int32 newY = PinLocY(finalY + y);
    if (newY == finalY) return FALSE;
    mScroller->SetFinalY(newY);
    mScroller->ExtendDuration(ComputeDuration(0, y));
    return TRUE;
}

/**
 * See {@link WebView#pageUp(boolean)}
 */
ECode CWebViewClassic::PageUp(
    /* [in] */ Boolean top,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }
    if (top) {
        // go to the top of the document
        *result = PinScrollTo(GetScrollX(), 0, TRUE, 0);
        return NOERROR;
    }
    // Page up
    Int32 h = GetHeight();
    Int32 y;
    if (h > 2 * PAGE_SCROLL_OVERLAP) {
        y = -h + PAGE_SCROLL_OVERLAP;
    }
    else {
        y = -h / 2;
    }
    *result = mScroller->IsFinished() ? PinScrollBy(0, y, TRUE, 0)
            : ExtendScroll(y);
    return NOERROR;
}

/**
 * See {@link WebView#pageDown(boolean)}
 */
ECode CWebViewClassic::PageDown(
    /* [in] */ Boolean bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }
    if (bottom) {
        *result = PinScrollTo(GetScrollX(), ComputeRealVerticalScrollRange(), TRUE, 0);
        return NOERROR;
    }
    // Page down.
    Int32 h = GetHeight();
    Int32 y;
    if (h > 2 * PAGE_SCROLL_OVERLAP) {
        y = h - PAGE_SCROLL_OVERLAP;
    }
    else {
        y = h / 2;
    }
    *result = mScroller->IsFinished() ? PinScrollBy(0, y, TRUE, 0)
            : ExtendScroll(y);
    return NOERROR;
}

/**
 * See {@link WebView#clearView()}
 */
ECode CWebViewClassic::ClearView()
{
    mContentWidth = 0;
    mContentHeight = 0;
    SetBaseLayer(0, FALSE, FALSE);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::CLEAR_CONTENT);
    return NOERROR;
}

/**
 * See {@link WebView#capturePicture()}
 */
ECode CWebViewClassic::CapturePicture(
    /* [out] */ IPicture** pic)
{
    VALIDATE_NOT_NULL(pic);
    if (mNativeClass == 0) {
        *pic = NULL;
        return NOERROR;
    }
    CPicture::New(pic);
    NativeCopyBaseContentToPicture(*pic);
    return NOERROR;
}

/**
 * See {@link WebView#getScale()}
 */
ECode CWebViewClassic::GetScale(
    /* [out] */ Float* scale)
{
    VALIDATE_NOT_NULL(scale);
    *scale = mZoomManager->GetScale();
    return NOERROR;
}

/**
 * Compute the reading level scale of the WebView
 * @param scale The current scale.
 * @return The reading level scale.
 */
Float CWebViewClassic::ComputeReadingLevelScale(
    /* [in] */ Float scale)
{
    return mZoomManager->ComputeReadingLevelScale(scale);
}

/**
 * See {@link WebView#setInitialScale(int)}
 */
ECode CWebViewClassic::SetInitialScale(
    /* [in] */ Int32 scaleInPercent)
{
    mZoomManager->SetInitialScaleInPercent(scaleInPercent);
    return NOERROR;
}

/**
 * See {@link WebView#invokeZoomPicker()}
 */
ECode CWebViewClassic::InvokeZoomPicker()
{
    AutoPtr<IWebSettings> settings;
    GetSettings((IWebSettings**)&settings);
    Boolean result = FALSE;
    settings->SupportZoom(&result);
    if (!result) {
        Logger::W(LOGTAG, "This WebView doesn't support zoom.");
        return NOERROR;
    }
    ClearHelpers();
    mZoomManager->InvokeZoomPicker();
    return NOERROR;
}

/**
 * See {@link WebView#getHitTestResult()}
 */
ECode CWebViewClassic::GetHitTestResult(
    /* [out] */ IWebViewHitTestResult** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mInitialHitTestResult;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

Int32 CWebViewClassic::GetBlockLeftEdge(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Float readingScale)
{
    Float invReadingScale = 1.0f / readingScale;
    Int32 readingWidth = (Int32)(GetViewWidth() * invReadingScale);
    Int32 left = NO_LEFTEDGE;
    if (mFocusedNode != NULL) {
        Int32 length = mFocusedNode->mEnclosingParentRects->GetLength();
        for (Int32 i = 0; i < length; i++) {
            AutoPtr<IRect> rect = (*(mFocusedNode->mEnclosingParentRects))[i];
            Int32 width;
            rect->GetWidth(&width);
            if (width < mFocusedNode->mHitTestSlop) {
                // ignore bounding boxes that are too small
                continue;
            }
            else if (width > readingWidth) {
                // stop when bounding box doesn't fit the screen width
                // at reading scale
                break;
            }

            rect->GetLeft(&left);
        }
    }

    return left;
}

/**
 * See {@link WebView#requestFocusNodeHref(Message)}
 */
ECode CWebViewClassic::RequestFocusNodeHref(
    /* [in] */ IMessage* hrefMsg)
{
    if (hrefMsg == NULL) {
        return NOERROR;
    }
    Int32 contentX = ViewToContentX(mLastTouchX + GetScrollX());
    Int32 contentY = ViewToContentY(mLastTouchY + GetScrollY());
    if (mFocusedNode != NULL && mFocusedNode->mHitTestX == contentX
            && mFocusedNode->mHitTestY == contentY) {
        AutoPtr<IBundle> data;
        hrefMsg->GetData((IBundle**)&data);
        data->PutString(FocusNodeHref::URL, mFocusedNode->mLinkUrl);
        data->PutString(FocusNodeHref::TITLE, mFocusedNode->mAnchorText);
        data->PutString(FocusNodeHref::SRC, mFocusedNode->mImageUrl);
        hrefMsg->SendToTarget();
        return NOERROR;
    }
    mWebViewCore->SendMessage(CWebViewCore::EventHub::REQUEST_CURSOR_HREF,
            contentX, contentY, hrefMsg);
    return NOERROR;
}

/**
 * See {@link WebView#requestImageRef(Message)}
 */
ECode CWebViewClassic::RequestImageRef(
    /* [in] */ IMessage* msg)
{
    if (0 == mNativeClass) return NOERROR; // client isn't initialized
    String url = mFocusedNode != NULL ? mFocusedNode->mImageUrl : String(NULL);
    AutoPtr<IBundle> data;
    msg->GetData((IBundle**)&data);
    data->PutString(String("url"), url);
    msg->SetData(data);
    msg->SendToTarget();
    return NOERROR;
}

Int32 CWebViewClassic::PinLoc(
    /* [in] */ Int32 x,
    /* [in] */ Int32 viewMax,
    /* [in] */ Int32 docMax)
{
    if (docMax < viewMax) {   // the doc has room on the sides for "blank"
        // pin the short document to the top/left of the screen
        x = 0;
    }
    else if (x < 0) {
        x = 0;
    }
    else if (x + viewMax > docMax) {
        x = docMax - viewMax;
    }
    return x;
}

// Expects x in view coordinates
Int32 CWebViewClassic::PinLocX(
    /* [in] */ Int32 x)
{
    if (mInOverScrollMode) return x;
    return PinLoc(x, GetViewWidth(), ComputeRealHorizontalScrollRange());
}

// Expects y in view coordinates
Int32 CWebViewClassic::PinLocY(
    /* [in] */ Int32 y)
{
    if (mInOverScrollMode) return y;
    return PinLoc(y, GetViewHeightWithTitle(),
            ComputeRealVerticalScrollRange() + GetTitleHeight());
}

/**
 * Given a distance in view space, convert it to content space. Note: this
 * does not reflect translation, just scaling, so this should not be called
 * with coordinates, but should be called for dimensions like width or
 * height.
 */
Int32 CWebViewClassic::ViewToContentDimension(
    /* [in] */ Int32 d)
{
    return Elastos::Core::Math::Round(d * mZoomManager->GetInvScale());
}

/**
 * Given an x coordinate in view space, convert it to content space.  Also
 * may be used for absolute heights.
 */
Int32 CWebViewClassic::ViewToContentX(
    /* [in] */ Int32 x)
{
    return ViewToContentDimension(x);
}

/**
 * Given a y coordinate in view space, convert it to content space.
 * Takes into account the height of the title bar if there is one
 * embedded into the WebView.
 */
Int32 CWebViewClassic::ViewToContentY(
    /* [in] */ Int32 y)
{
    return ViewToContentDimension(y - GetTitleHeight());
}

/**
 * Given a x coordinate in view space, convert it to content space.
 * Returns the result as a float.
 */
Float CWebViewClassic::ViewToContentXf(
    /* [in] */ Int32 x)
{
    return x * mZoomManager->GetInvScale();
}

/**
 * Given a y coordinate in view space, convert it to content space.
 * Takes into account the height of the title bar if there is one
 * embedded into the WebView. Returns the result as a float.
 */
Float CWebViewClassic::ViewToContentYf(
    /* [in] */ Int32 y)
{
    return (y - GetTitleHeight()) * mZoomManager->GetInvScale();
}

/**
 * Given a distance in content space, convert it to view space. Note: this
 * does not reflect translation, just scaling, so this should not be called
 * with coordinates, but should be called for dimensions like width or
 * height.
 */
Int32 CWebViewClassic::ContentToViewDimension(
    /* [in] */ Int32 d)
{
    return Elastos::Core::Math::Round(d * mZoomManager->GetScale());
}

/**
 * Given an x coordinate in content space, convert it to view
 * space.
 */
Int32 CWebViewClassic::ContentToViewX(
    /* [in] */ Int32 x)
{
    return ContentToViewDimension(x);
}

/**
 * Given a y coordinate in content space, convert it to view
 * space.  Takes into account the height of the title bar.
 */
Int32 CWebViewClassic::ContentToViewY(
    /* [in] */ Int32 y)
{
    return ContentToViewDimension(y) + GetTitleHeight();
}

AutoPtr<IRect> CWebViewClassic::ContentToViewRect(
    /* [in] */ IRect* x)
{
    Int32 left, right, bottom, top;
    x->Get(&left, &top, &right, &bottom);
    AutoPtr<IRect> rect;
    CRect::New(ContentToViewX(left), ContentToViewY(top),
            ContentToViewX(right), ContentToViewY(bottom), (IRect**)&rect);
    return rect;
}

/*  To invalidate a rectangle in content coordinates, we need to transform
    the rect into view coordinates, so we can then call invalidate(...).

    Normally, we would just call contentToView[XY](...), which eventually
    calls Math.round(coordinate * mActualScale). However, for invalidates,
    we need to account for the slop that occurs with antialiasing. To
    address that, we are a little more liberal in the size of the rect that
    we invalidate.

    This liberal calculation calls floor() for the top/left, and ceil() for
    the bottom/right coordinates. This catches the possible extra pixels of
    antialiasing that we might have missed with just round().
 */

// Called by JNI to invalidate the View, given rectangle coordinates in
// content space
void CWebViewClassic::ViewInvalidate(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Float scale = mZoomManager->GetScale();
    Int32 dy = GetTitleHeight();
    mWebView->Invalidate((Int32)Elastos::Core::Math::Floor(l * scale),
            (Int32)Elastos::Core::Math::Floor(t * scale) + dy,
            (Int32)Elastos::Core::Math::Ceil(r * scale),
            (Int32)Elastos::Core::Math::Ceil(b * scale) + dy);
}

// Called by JNI to invalidate the View after a delay, given rectangle
// coordinates in content space
void CWebViewClassic::ViewInvalidateDelayed(
    /* [in] */ Int64 delay,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Float scale = mZoomManager->GetScale();
    Int32 dy = GetTitleHeight();
    mWebView->PostInvalidateDelayed(delay,
            (Int32)Elastos::Core::Math::Floor(l * scale),
            (Int32)Elastos::Core::Math::Floor(t * scale) + dy,
            (Int32)Elastos::Core::Math::Ceil(r * scale),
            (Int32)Elastos::Core::Math::Ceil(b * scale) + dy);
}

void CWebViewClassic::InvalidateContentRect(
    /* [in] */ IRect* r)
{
    Int32 left, top, right, bottom;
    r->Get(&left, &top, &right, &bottom);
    ViewInvalidate(left, top, right, bottom);
}

// stop the scroll animation, and don't let a subsequent fling add
// to the existing velocity
void CWebViewClassic::AbortAnimation()
{
    mScroller->AbortAnimation();
    mLastVelocity = 0;
}

/* call from webcoreview.draw(), so we're still executing in the UI thread
*/
void CWebViewClassic::RecordNewContentSize(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Boolean updateLayout)
{
    // premature data from webkit, ignore
    if ((w | h) == 0) {
        Invalidate();
        return;
    }

    // don't abort a scroll animation if we didn't change anything
    if (mContentWidth != w || mContentHeight != h) {
        // record new dimensions
        mContentWidth = w;
        mContentHeight = h;
        // If history Picture is drawn, don't update scroll. They will be
        // updated when we get out of that mode.
        if (!mDrawHistory) {
            // repin our scroll, taking into account the new content size
            UpdateScrollCoordinates(PinLocX(GetScrollX()), PinLocY(GetScrollY()));
            if (!mScroller->IsFinished()) {
                // We are in the middle of a scroll.  Repin the final scroll
                // position.
                mScroller->SetFinalX(PinLocX(mScroller->GetFinalX()));
                mScroller->SetFinalY(PinLocY(mScroller->GetFinalY()));
            }
        }
        Invalidate();
    }
    ContentSizeChanged(updateLayout);
}

AutoPtr<IRect> CWebViewClassic::SendOurVisibleRect()
{
    if (mZoomManager->IsPreventingWebkitUpdates()) return mLastVisibleRectSent;
    CalcOurContentVisibleRect(mVisibleRect);
    // Rect.equals() checks for null input.
    Boolean result = FALSE;
    if (mVisibleRect->Equals(mLastVisibleRectSent, &result), !result) {
        if (!mBlockWebkitViewMessages) {
            Int32 left, top;
            mVisibleRect->GetLeft(&left);
            mVisibleRect->GetTop(&top);
            mScrollOffset->Set(left, top);
            mWebViewCore->RemoveMessages(CWebViewCore::EventHub::SET_SCROLL_OFFSET);
            mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_SCROLL_OFFSET,
                    mSendScrollEvent ? 1 : 0, mScrollOffset);
        }
        mLastVisibleRectSent->Set(mVisibleRect);
        mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
    }
    if ((mWebView->GetGlobalVisibleRect(mGlobalVisibleRect, &result), result) &&
        (mGlobalVisibleRect->Equals(mLastGlobalRect, &result), !result)) {
        if (DebugFlags::WEB_VIEW) {
            Int32 left, top, right, bottom;
            mGlobalVisibleRect->Get(&left, &top, &right, &bottom);
            Logger::V(LOGTAG, "sendOurVisibleRect=(l=%d,t=%d,r=%d,b=%d)", left, top, right, bottom);
        }
        // TODO: the global offset is only used by windowRect()
        // in ChromeClientAndroid ; other clients such as touch
        // and mouse events could return view + screen relative points.
        if (!mBlockWebkitViewMessages) {
            mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_GLOBAL_BOUNDS, mGlobalVisibleRect);
        }
        mLastGlobalRect->Set(mGlobalVisibleRect);
    }
    return mVisibleRect;
}

// Sets r to be the visible rectangle of our webview in view coordinates
void CWebViewClassic::CalcOurVisibleRect(
    /* [in] */ IRect* r)
{
    Boolean result;
    mWebView->GetGlobalVisibleRect(r, mGlobalVisibleOffset, &result);
    Int32 x, y;
    mGlobalVisibleOffset->GetX(&x);
    mGlobalVisibleOffset->GetY(&y);
    r->Offset(-x, -y);
}

// Sets r to be our visible rectangle in content coordinates
void CWebViewClassic::CalcOurContentVisibleRect(
    /* [in] */ IRect* r)
{
    CalcOurVisibleRect(r);
    Int32 left, top, right, bottom;
    r->Get(&left, &top, &right, &bottom);
    r->SetLeft(ViewToContentX(left));
    // viewToContentY will remove the total height of the title bar.  Add
    // the visible height back in to account for the fact that if the title
    // bar is partially visible, the part of the visible rect which is
    // displaying our content is displaced by that amount.
    r->SetTop(ViewToContentY(top + GetVisibleTitleHeightImpl()));
    r->SetRight(ViewToContentX(right));
    r->SetBottom(ViewToContentY(bottom));
}

// Sets r to be our visible rectangle in content coordinates. We use this
// method on the native side to compute the position of the fixed layers.
// Uses floating coordinates (necessary to correctly place elements when
// the scale factor is not 1)
void CWebViewClassic::CalcOurContentVisibleRectF(
    /* [in] */ IRectF* r)
{
    CalcOurVisibleRect(mTempContentVisibleRect);
    ViewToContentVisibleRect(r, mTempContentVisibleRect);
}

/**
 * Compute unzoomed width and height, and if they differ from the last
 * values we sent, send them to webkit (to be used as new viewport)
 *
 * @param force ensures that the message is sent to webkit even if the width
 * or height has not changed since the last message
 *
 * @return true if new values were sent
 */
Boolean CWebViewClassic::SendViewSizeZoom(
    /* [in] */ Boolean force)
{
    if (mBlockWebkitViewMessages) return FALSE;
    if (mZoomManager->IsPreventingWebkitUpdates()) return FALSE;

    Int32 viewWidth = GetViewWidth();
    Int32 newWidth = Elastos::Core::Math::Round(viewWidth * mZoomManager->GetInvScale());
    // This height could be fixed and be different from actual visible height.
    Int32 viewHeight = GetViewHeightWithTitle() - GetTitleHeight();
    Int32 newHeight = Elastos::Core::Math::Round(viewHeight * mZoomManager->GetInvScale());
    // Make the ratio more accurate than (newHeight / newWidth), since the
    // latter both are calculated and rounded.
    Float heightWidthRatio = (Float) viewHeight / viewWidth;
    /*
     * Because the native side may have already done a layout before the
     * View system was able to measure us, we have to send a height of 0 to
     * remove excess whitespace when we grow our width. This will trigger a
     * layout and a change in content size. This content size change will
     * mean that contentSizeChanged will either call this method directly or
     * indirectly from onSizeChanged.
     */
    if (newWidth > mLastWidthSent && mWrapContent) {
        newHeight = 0;
        heightWidthRatio = 0;
    }
    // Actual visible content height.
    Int32 actualViewHeight = Elastos::Core::Math::Round(GetViewHeight() * mZoomManager->GetInvScale());
    // Avoid sending another message if the dimensions have not changed.
    if (newWidth != mLastWidthSent || newHeight != mLastHeightSent || force ||
            actualViewHeight != mLastActualHeightSent) {
        AutoPtr<ViewSizeData> data = new ViewSizeData();
        data->mWidth = newWidth;
        data->mHeight = newHeight;
        data->mHeightWidthRatio = heightWidthRatio;
        data->mActualViewHeight = actualViewHeight;
        data->mTextWrapWidth = Elastos::Core::Math::Round(viewWidth / mZoomManager->GetTextWrapScale());
        data->mScale = mZoomManager->GetScale();
        data->mIgnoreHeight = mZoomManager->IsFixedLengthAnimationInProgress()
                && !mHeightCanMeasure;
        data->mAnchorX = mZoomManager->GetDocumentAnchorX();
        data->mAnchorY = mZoomManager->GetDocumentAnchorY();
        mWebViewCore->SendMessage(CWebViewCore::EventHub::VIEW_SIZE_CHANGED, data.Get());
        mLastWidthSent = newWidth;
        mLastHeightSent = newHeight;
        mLastActualHeightSent = actualViewHeight;
        mZoomManager->ClearDocumentAnchor();
        return TRUE;
    }
    return FALSE;
}

/**
 * Update the double-tap zoom.
 */
void CWebViewClassic::UpdateDoubleTapZoom(
    /* [in] */ Int32 doubleTapZoom)
{
    mZoomManager->UpdateDoubleTapZoom(doubleTapZoom);
}

Int32 CWebViewClassic::ComputeRealHorizontalScrollRange()
{
    if (mDrawHistory) {
        return mHistoryWidth;
    }
    else {
        // to avoid rounding error caused unnecessary scrollbar, use floor
        return (Int32) Elastos::Core::Math::Floor(mContentWidth * mZoomManager->GetScale());
    }
}

ECode CWebViewClassic::ComputeHorizontalScrollRange(
    /* [out] */ Int32* range)
{
    VALIDATE_NOT_NULL(range);
    *range = ComputeRealHorizontalScrollRange();

    // Adjust reported range if overscrolled to compress the scroll bars
    Int32 scrollX = GetScrollX();
    Int32 overscrollRight = ComputeMaxScrollX();
    if (scrollX < 0) {
        *range -= scrollX;
    }
    else if (scrollX > overscrollRight) {
        *range += scrollX - overscrollRight;
    }

    return NOERROR;
}

ECode CWebViewClassic::ComputeHorizontalScrollOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = Elastos::Core::Math::Max(GetScrollX(), 0);
    return NOERROR;
}

Int32 CWebViewClassic::ComputeRealVerticalScrollRange()
{
    if (mDrawHistory) {
        return mHistoryHeight;
    }
    else {
        // to avoid rounding error caused unnecessary scrollbar, use floor
        return (Int32) Elastos::Core::Math::Floor(mContentHeight * mZoomManager->GetScale());
    }
}

ECode CWebViewClassic::ComputeVerticalScrollRange(
    /* [out] */ Int32* range)
{
    VALIDATE_NOT_NULL(range);
    *range = ComputeRealVerticalScrollRange();

    // Adjust reported range if overscrolled to compress the scroll bars
    Int32 scrollY = GetScrollY();
    Int32 overscrollBottom = ComputeMaxScrollY();
    if (scrollY < 0) {
        *range -= scrollY;
    }
    else if (scrollY > overscrollBottom) {
        *range += scrollY - overscrollBottom;
    }

    return NOERROR;
}

ECode CWebViewClassic::ComputeVerticalScrollOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = Elastos::Core::Math::Max(GetScrollY() - GetTitleHeight(), 0);
    return NOERROR;
}

ECode CWebViewClassic::ComputeVerticalScrollExtent(
    /* [out] */ Int32* extent)
{
    VALIDATE_NOT_NULL(extent);
    *extent = GetViewHeight();
    return NOERROR;
}

ECode CWebViewClassic::OnDrawVerticalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    if (GetScrollY() < 0) {
        t -= GetScrollY();
    }
    scrollBar->SetBounds(l, t + GetVisibleTitleHeightImpl(), r, b);
    scrollBar->Draw(canvas);
    return NOERROR;
}

ECode CWebViewClassic::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    // Special-case layer scrolling so that we do not trigger normal scroll
    // updating.
    if (mTouchMode == TOUCH_DRAG_TEXT_MODE) {
        ScrollEditText(scrollX, scrollY);
        return NOERROR;
    }
    if (mTouchMode == TOUCH_DRAG_LAYER_MODE) {
        ScrollLayerTo(scrollX, scrollY);
        AnimateHandles();
        return NOERROR;
    }
    mInOverScrollMode = FALSE;
    Int32 maxX = ComputeMaxScrollX();
    Int32 maxY = ComputeMaxScrollY();
    if (maxX == 0) {
        // do not over scroll x if the page just fits the screen
        scrollX = PinLocX(scrollX);
    }
    else if (scrollX < 0 || scrollX > maxX) {
        mInOverScrollMode = TRUE;
    }
    if (scrollY < 0 || scrollY > maxY) {
        mInOverScrollMode = TRUE;
    }

    Int32 oldX = GetScrollX();
    Int32 oldY = GetScrollY();

    mWebViewPrivate->Super_scrollTo(scrollX, scrollY);

    AnimateHandles();

    if (mOverScrollGlow != NULL) {
        mOverScrollGlow->PullGlow(GetScrollX(), GetScrollY(), oldX, oldY, maxX, maxY);
    }
    return NOERROR;
}

/**
 * See {@link WebView#getUrl()}
 */
ECode CWebViewClassic::GetUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    AutoPtr<IWebHistoryItem> h;
    mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
    if (h != NULL) {
        return h->GetUrl(url);
    }
    else {
        *url = NULL;
        return NOERROR;
    }
}

/**
 * See {@link WebView#getOriginalUrl()}
 */
ECode CWebViewClassic::GetOriginalUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    AutoPtr<IWebHistoryItem> h;
    mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
    if (h != NULL) {
        return h->GetOriginalUrl(url);
    }
    else {
        *url = NULL;
        return NOERROR;
    }
}

/**
 * See {@link WebView#getTitle()}
 */
ECode CWebViewClassic::GetTitle(
    /* [out] */ String* title)
{
    VALIDATE_NOT_NULL(title);
    AutoPtr<IWebHistoryItem> h;
    mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
    if (h != NULL) {
        return h->GetTitle(title);
    }
    else {
        *title = NULL;
        return NOERROR;
    }
}

/**
 * See {@link WebView#getFavicon()}
 */
ECode CWebViewClassic::GetFavicon(
    /* [out] */ IBitmap** favicon)
{
    VALIDATE_NOT_NULL(favicon);
    AutoPtr<IWebHistoryItem> h;
    mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&h);
    if (h != NULL) {
        return h->GetFavicon(favicon);
    }
    else {
        *favicon = NULL;
        return NOERROR;
    }
}

/**
 * See {@link WebView#getTouchIconUrl()}
 */
ECode CWebViewClassic::GetTouchIconUrl(
    /* [out] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    AutoPtr<IWebHistoryItem> item;
    mCallbackProxy->GetBackForwardList()->GetCurrentItem((IWebHistoryItem**)&item);
    WebHistoryItemClassic* h = (WebHistoryItemClassic*)item.Get();
    if (h != NULL) {
        *url = h->GetTouchIconUrl();
        return NOERROR;
    }
    else {
        *url = NULL;
        return NOERROR;
    }
}

/**
 * See {@link WebView#getProgress()}
 */
ECode CWebViewClassic::GetProgress(
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    *progress = mCallbackProxy->GetProgress();
    return NOERROR;
}

/**
 * See {@link WebView#getContentHeight()}
 */
ECode CWebViewClassic::GetContentHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mContentHeight;
    return NOERROR;
}

/**
 * See {@link WebView#getContentWidth()}
 */
ECode CWebViewClassic::GetContentWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mContentWidth;
    return NOERROR;
}

ECode CWebViewClassic::GetPageBackgroundColor(
    /* [out] */ Int32* color)
{
    VALIDATE_NOT_NULL(color);
    if (mNativeClass == 0) {
        *color = IColor::WHITE;
        return NOERROR;
    }
    *color = NativeGetBackgroundColor(mNativeClass);
    return NOERROR;
}

/**
 * See {@link WebView#pauseTimers()}
 */
ECode CWebViewClassic::PauseTimers()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::PAUSE_TIMERS);
    return NOERROR;
}

/**
 * See {@link WebView#resumeTimers()}
 */
ECode CWebViewClassic::ResumeTimers()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::RESUME_TIMERS);
    return NOERROR;
}

/**
 * See {@link WebView#onPause()}
 */
ECode CWebViewClassic::OnPause()
{
    if (!mIsPaused) {
        mIsPaused = TRUE;
        mWebViewCore->SendMessage(CWebViewCore::EventHub::ON_PAUSE);
        // We want to pause the current playing video when switching out
        // from the current WebView/tab.
        if (mHTML5VideoViewProxy != NULL) {
            mHTML5VideoViewProxy->PauseAndDispatch();
        }
        if (mNativeClass != 0) {
            NativeSetPauseDrawing(mNativeClass, TRUE);
        }

        CancelDialogs();
        WebCoreThreadWatchdog::Pause();
    }
    return NOERROR;
}

ECode CWebViewClassic::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    UpdateDrawingState();
    return NOERROR;
}

void CWebViewClassic::UpdateDrawingState()
{
    if (mNativeClass == 0 || mIsPaused) return;

    Int32 visibility;
    if (mWebView->GetWindowVisibility(&visibility), visibility != IView::VISIBLE) {
        NativeSetPauseDrawing(mNativeClass, TRUE);
    }
    else if (mWebView->GetVisibility(&visibility), visibility != IView::VISIBLE) {
        NativeSetPauseDrawing(mNativeClass, TRUE);
    }
    else {
        NativeSetPauseDrawing(mNativeClass, FALSE);
    }
}

/**
 * See {@link WebView#onResume()}
 */
ECode CWebViewClassic::OnResume()
{
    if (mIsPaused) {
        mIsPaused = FALSE;
        mWebViewCore->SendMessage(CWebViewCore::EventHub::ON_RESUME);
        if (mNativeClass != 0) {
            NativeSetPauseDrawing(mNativeClass, FALSE);
        }
    }
    // We get a call to onResume for new WebViews (i.e. mIsPaused will be false). We need
    // to ensure that the Watchdog thread is running for the new WebView, so call
    // it outside the if block above.
    WebCoreThreadWatchdog::Resume();
    return NOERROR;
}

/**
 * See {@link WebView#isPaused()}
 */
ECode CWebViewClassic::IsPaused(
    /* [out] */ Boolean* result)
{
    *result = mIsPaused;
    return NOERROR;
}

/**
 * See {@link WebView#freeMemory()}
 */
ECode CWebViewClassic::FreeMemory()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::FREE_MEMORY);
    return NOERROR;
}

/**
 * See {@link WebView#clearCache(boolean)}
 */
ECode CWebViewClassic::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    // Note: this really needs to be a static method as it clears cache for all
    // WebView. But we need mWebViewCore to send message to WebCore thread, so
    // we can't make this static.
    mWebViewCore->SendMessage(CWebViewCore::EventHub::CLEAR_CACHE,
            includeDiskFiles ? 1 : 0, 0);
    return NOERROR;
}

/**
 * See {@link WebView#clearFormData()}
 */
ECode CWebViewClassic::ClearFormData()
{
    if (mAutoCompletePopup != NULL) {
        mAutoCompletePopup->ClearAdapter();
    }
    return NOERROR;
}

/**
 * See {@link WebView#clearHistory()}
 */
ECode CWebViewClassic::ClearHistory()
{
    mCallbackProxy->GetBackForwardList()->SetClearPending();
    mWebViewCore->SendMessage(CWebViewCore::EventHub::CLEAR_HISTORY);
    return NOERROR;
}

/**
 * See {@link WebView#clearSslPreferences()}
 */
ECode CWebViewClassic::ClearSslPreferences()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::CLEAR_SSL_PREF_TABLE);
    return NOERROR;
}

/**
 * See {@link WebView#copyBackForwardList()}
 */
ECode CWebViewClassic::CopyBackForwardList(
    /* [out] */ IWebBackForwardList** wbfl)

{
    VALIDATE_NOT_NULL(wbfl);
    AutoPtr<WebBackForwardListClassic> list = mCallbackProxy->GetBackForwardList()->Clone();
    *wbfl = (IWebBackForwardList*)list.Get();
    REFCOUNT_ADD(*wbfl);
    return NOERROR;
}

/**
 * See {@link WebView#setFindListener(WebView.FindListener)}.
 * @hide
 */
ECode CWebViewClassic::SetFindListener(
    /* [in] */ IWebViewFindListener* listener)
{
    mFindListener = listener;
    return NOERROR;
}

/**
 * See {@link WebView#findNext(boolean)}
 */
ECode CWebViewClassic::FindNext(
    /* [in] */ Boolean forward)
{
    if (0 == mNativeClass) return NOERROR; // client isn't initialized
    if (mFindRequest != NULL) {
        mWebViewCore->SendMessage(CWebViewCore::EventHub::FIND_NEXT, forward ? 1 : 0, mFindRequest);
    }
    return NOERROR;
}

/**
 * See {@link WebView#findAll(String)}
 */
ECode CWebViewClassic::FindAll(
    /* [in] */ const String& find,
    /* [out] */ Int32* all)
{
    VALIDATE_NOT_NULL(all);
    *all = FindAllBody(find, FALSE);
    return NOERROR;
}

ECode CWebViewClassic::FindAllAsync(
    /* [in] */ const String& find)
{
    FindAllBody(find, TRUE);
    return NOERROR;
}

Int32 CWebViewClassic::FindAllBody(
    /* [in] */ const String& find,
    /* [in] */ Boolean isAsync)
{
    if (0 == mNativeClass) return 0; // client isn't initialized
    mFindRequest = NULL;
    if (find.IsNull()) return 0;
    mWebViewCore->RemoveMessages(CWebViewCore::EventHub::FIND_ALL);
    mFindRequest = new CWebViewCoreFindAllRequest(find);
    if (isAsync) {
        mWebViewCore->SendMessage(CWebViewCore::EventHub::FIND_ALL, mFindRequest);
        return 0; // no need to wait for response
    }
    {
        Object::Autolock lock(mFindRequest);

        // try {
        mWebViewCore->SendMessageAtFrontOfQueue(CWebViewCore::EventHub::FIND_ALL, mFindRequest);
        while (mFindRequest->mMatchCount == -1) {
            ECode ec = mFindRequest->Wait();
            if (FAILED(ec)) return 0;
        }
        // }
        // catch (InterruptedException e) {
        //     return 0;
        // }
        return mFindRequest->mMatchCount;
    }
}

/**
 * Start an ActionMode for finding text in this WebView.  Only works if this
 *              WebView is attached to the view system.
 * @param text If non-null, will be the initial text to search for.
 *             Otherwise, the last String searched for in this WebView will
 *             be used to start.
 * @param showIme If true, show the IME, assuming the user will begin typing.
 *             If false and text is non-null, perform a find all.
 * @return boolean True if the find dialog is shown, false otherwise.
 */
ECode CWebViewClassic::ShowFindDialog(
    /* [in] */ const String& _text,
    /* [in] */ Boolean showIme,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    String text = _text;
    AutoPtr<FindActionModeCallback> callback = new FindActionModeCallback(mContext);
    AutoPtr<IViewParent> viewParent;
    AutoPtr<IActionMode> actionMode;
    if ((mWebView->GetParent((IViewParent**)&viewParent), viewParent == NULL) ||
        (mWebView->StartActionMode(callback, (IActionMode**)&actionMode), actionMode == NULL)) {
        // Could not start the action mode, so end Find on page
        *result = FALSE;
        return NOERROR;
    }
    mCachedOverlappingActionModeHeight = -1;
    mFindCallback = callback;
    SetFindIsUp(TRUE);
    mFindCallback->SetWebView(this);
    if (showIme) {
        mFindCallback->ShowSoftInput();
    }
    else if (!text.IsNull()) {
        mFindCallback->SetText(text);
        mFindCallback->FindAll();
        *result = TRUE;
        return NOERROR;
    }
    if (text.IsNull()) {
        text = mFindRequest == NULL ? String(NULL) : mFindRequest->mSearchText;
    }
    if (!text.IsNull()) {
        mFindCallback->SetText(text);
        mFindCallback->FindAll();
    }
    *result = TRUE;
    return NOERROR;
}

/**
 * Toggle whether the find dialog is showing, for both native and Java.
 */
void CWebViewClassic::SetFindIsUp(
    /* [in] */ Boolean isUp)
{
    mFindIsUp = isUp;
}

/**
 * Return the first substring consisting of the address of a physical
 * location. Currently, only addresses in the United States are detected,
 * and consist of:
 * - a house number
 * - a street name
 * - a street type (Road, Circle, etc), either spelled out or abbreviated
 * - a city name
 * - a state or territory, either spelled out or two-letter abbr.
 * - an optional 5 digit or 9 digit zip code.
 *
 * All names must be correctly capitalized, and the zip code, if present,
 * must be valid for the state. The street type must be a standard USPS
 * spelling or abbreviation. The state or territory must also be spelled
 * or abbreviated using USPS standards. The house number may not exceed
 * five digits.
 * @param addr The string to search for addresses.
 *
 * @return the address, or if no address is found, return null.
 */
String CWebViewClassic::FindAddress(
    /* [in] */ const String& addr)
{
    return FindAddress(addr, FALSE);
}

/**
 * Return the first substring consisting of the address of a physical
 * location. Currently, only addresses in the United States are detected,
 * and consist of:
 * - a house number
 * - a street name
 * - a street type (Road, Circle, etc), either spelled out or abbreviated
 * - a city name
 * - a state or territory, either spelled out or two-letter abbr.
 * - an optional 5 digit or 9 digit zip code.
 *
 * Names are optionally capitalized, and the zip code, if present,
 * must be valid for the state. The street type must be a standard USPS
 * spelling or abbreviation. The state or territory must also be spelled
 * or abbreviated using USPS standards. The house number may not exceed
 * five digits.
 * @param addr The string to search for addresses.
 * @param caseInsensitive addr Set to true to make search ignore case.
 *
 * @return the address, or if no address is found, return null.
 */
String CWebViewClassic::FindAddress(
    /* [in] */ const String& addr,
    /* [in] */ Boolean caseInsensitive)
{
    return CWebViewCore::NativeFindAddress(addr, caseInsensitive);
}

/**
 * See {@link WebView#clearMatches()}
 */
ECode CWebViewClassic::ClearMatches()
{
    if (mNativeClass == 0) return NOERROR;
    mWebViewCore->RemoveMessages(CWebViewCore::EventHub::FIND_ALL);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::FIND_ALL, 0);
    return NOERROR;
}

/**
 * Called when the find ActionMode ends.
 */
void CWebViewClassic::NotifyFindDialogDismissed()
{
    mFindCallback = NULL;
    mCachedOverlappingActionModeHeight = -1;
    if (mWebViewCore == NULL) {
        return;
    }
    ClearMatches();
    SetFindIsUp(FALSE);
    // Now that the dialog has been removed, ensure that we scroll to a
    // location that is not beyond the end of the page.
    PinScrollTo(GetScrollX(), GetScrollY(), FALSE, 0);
    Invalidate();
}

/**
 * See {@link WebView#documentHasImages(Message)}
 */
ECode CWebViewClassic::DocumentHasImages(
    /* [in] */ IMessage* response)
{
    if (response == NULL) {
        return NOERROR;
    }
    mWebViewCore->SendMessage(CWebViewCore::EventHub::DOC_HAS_IMAGES, response);
    return NOERROR;
}

/**
 * Request the scroller to abort any ongoing animation
 */
ECode CWebViewClassic::StopScroll()
{
    mScroller->ForceFinished(true);
    mLastVelocity = 0;
    return NOERROR;
}

ECode CWebViewClassic::ComputeScroll()
{
    if (mScroller->ComputeScrollOffset()) {
        Int32 oldX = GetScrollX();
        Int32 oldY = GetScrollY();
        Int32 x = mScroller->GetCurrX();
        Int32 y = mScroller->GetCurrY();
        Invalidate();  // So we draw again

        if (!mScroller->IsFinished()) {
            Int32 rangeX = ComputeMaxScrollX();
            Int32 rangeY = ComputeMaxScrollY();
            Int32 overflingDistance = mOverflingDistance;

            // Use the layer's scroll data if needed.
            if (mTouchMode == TOUCH_DRAG_LAYER_MODE) {
                mScrollingLayerRect->GetLeft(&oldX);
                mScrollingLayerRect->GetTop(&oldY);
                mScrollingLayerRect->GetRight(&rangeX);
                mScrollingLayerRect->GetBottom(&rangeY);
                // No overscrolling for layers.
                overflingDistance = 0;
            }
            else if (mTouchMode == TOUCH_DRAG_TEXT_MODE) {
                oldX = GetTextScrollX();
                oldY = GetTextScrollY();
                rangeX = GetMaxTextScrollX();
                rangeY = GetMaxTextScrollY();
                overflingDistance = 0;
            }

            mWebViewPrivate->OverScrollBy(x - oldX, y - oldY, oldX, oldY,
                    rangeX, rangeY,
                    overflingDistance, overflingDistance, FALSE);

            if (mOverScrollGlow != NULL) {
                mOverScrollGlow->AbsorbGlow(x, y, oldX, oldY, rangeX, rangeY);
            }
        }
        else {
            if (mTouchMode == TOUCH_DRAG_LAYER_MODE) {
                // Update the layer position instead of WebView.
                ScrollLayerTo(x, y);
            }
            else if (mTouchMode == TOUCH_DRAG_TEXT_MODE) {
                ScrollEditText(x, y);
            }
            else {
                SetScrollXRaw(x);
                SetScrollYRaw(y);
            }

            AbortAnimation();
            NativeSetIsScrolling(FALSE);
            if (!mBlockWebkitViewMessages) {
                CWebViewCore::ResumePriority();
                if (!mSelectingText) {
                    CWebViewCore::ResumeUpdatePicture(mWebViewCore);
                }
            }
            if (oldX != GetScrollX() || oldY != GetScrollY()) {
                SendOurVisibleRect();
            }
        }
    }
    else {
        mWebViewPrivate->Super_computeScroll();
    }
    return NOERROR;
}

void CWebViewClassic::ScrollLayerTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 left, top;
    mScrollingLayerRect->GetLeft(&left);
    mScrollingLayerRect->GetTop(&top);
    Int32 dx = left - x;
    Int32 dy = top - y;
    if ((dx == 0 && dy == 0) || mNativeClass == 0) {
        return;
    }
    if (mSelectingText) {
        if (mSelectCursorBaseLayerId == mCurrentScrollingLayerId) {
            mSelectCursorBase->Offset(dx, dy);
            mSelectCursorBaseTextQuad->Offset(dx, dy);
        }
        if (mSelectCursorExtentLayerId == mCurrentScrollingLayerId) {
            mSelectCursorExtent->Offset(dx, dy);
            mSelectCursorExtentTextQuad->Offset(dx, dy);
        }
    }
    if (mAutoCompletePopup != NULL &&
            mCurrentScrollingLayerId == mEditTextLayerId) {
        mEditTextContentBounds->Offset(dx, dy);
        mAutoCompletePopup->ResetRect();
    }
    NativeScrollLayer(mNativeClass, mCurrentScrollingLayerId, x, y);
    mScrollingLayerRect->SetLeft(x);
    mScrollingLayerRect->SetTop(y);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SCROLL_LAYER, mCurrentScrollingLayerId,
            mScrollingLayerRect);
    mWebViewPrivate->OnScrollChanged(GetScrollX(), GetScrollY(), GetScrollX(), GetScrollY());
    Invalidate();
}

Int32 CWebViewClassic::ComputeDuration(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    Int32 distance = Elastos::Core::Math::Max(Elastos::Core::Math::Abs(dx), Elastos::Core::Math::Abs(dy));
    Int32 duration = distance * 1000 / STD_SPEED;
    return Elastos::Core::Math::Min(duration, MAX_DURATION);
}

// helper to pin the scrollBy parameters (already in view coordinates)
// returns true if the scroll was changed
Boolean CWebViewClassic::PinScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ Boolean animate,
    /* [in] */ Int32 animationDuration)
{
    return PinScrollTo(GetScrollX() + dx, GetScrollY() + dy, animate, animationDuration);
}

// helper to pin the scrollTo parameters (already in view coordinates)
// returns true if the scroll was changed
Boolean CWebViewClassic::PinScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Boolean animate,
    /* [in] */ Int32 animationDuration)
{
    AbortAnimation();
    x = PinLocX(x);
    y = PinLocY(y);
    Int32 dx = x - GetScrollX();
    Int32 dy = y - GetScrollY();

    if ((dx | dy) == 0) {
        return FALSE;
    }
    if (animate) {
        //        Log.d(LOGTAG, "startScroll: " + dx + " " + dy);
        mScroller->StartScroll(GetScrollX(), GetScrollY(), dx, dy,
                animationDuration > 0 ? animationDuration : ComputeDuration(dx, dy));
        Invalidate();
    }
    else {
        mWebView->ScrollTo(x, y);
    }
    return TRUE;
}

// Scale from content to view coordinates, and pin.
// Also called by jni webview.cpp
Boolean CWebViewClassic::SetContentScrollBy(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy,
    /* [in] */ Boolean animate)
{
    if (mDrawHistory) {
        // disallow WebView to change the scroll position as History Picture
        // is used in the view system.
        // TODO: as we switchOutDrawHistory when trackball or navigation
        // keys are hit, this should be safe. Right?
        return FALSE;
    }
    cx = ContentToViewDimension(cx);
    cy = ContentToViewDimension(cy);
    if (mHeightCanMeasure) {
        // move our visible rect according to scroll request
        if (cy != 0) {
            AutoPtr<IRect> tempRect;
            CRect::New((IRect**)&tempRect);
            CalcOurVisibleRect(tempRect);
            tempRect->Offset(cx, cy);
            Boolean result;
            mWebView->RequestRectangleOnScreen(tempRect, &result);
        }
        // FIXME: We scroll horizontally no matter what because currently
        // ScrollView and ListView will not scroll horizontally.
        // FIXME: Why do we only scroll horizontally if there is no
        return cy == 0 && cx != 0 && PinScrollBy(cx, 0, animate, 0);
    }
    else {
        return PinScrollBy(cx, cy, animate, 0);
    }
}

/**
 * Called by CallbackProxy when the page starts loading.
 * @param url The URL of the page which has started loading.
 */
void CWebViewClassic::OnPageStarted(
    /* [in] */ const String& url)
{
    // every time we start a new page, we want to reset the
    // WebView certificate:  if the new site is secure, we
    // will reload it and get a new certificate set;
    // if the new site is not secure, the certificate must be
    // null, and that will be the case
    mWebView->SetCertificate(NULL);

    if (IsAccessibilityInjectionEnabled()) {
        GetAccessibilityInjector()->OnPageStarted(url);
    }

    // Don't start out editing.
    mIsEditingText = FALSE;
}

/**
 * Called by CallbackProxy when the page finishes loading.
 * @param url The URL of the page which has finished loading.
 */
void CWebViewClassic::OnPageFinished(
    /* [in] */ const String& url)
{
    mZoomManager->OnPageFinished(url);

    if (IsAccessibilityInjectionEnabled()) {
        GetAccessibilityInjector()->OnPageFinished(url);
    }
}

// scale from content to view coordinates, and pin
void CWebViewClassic::ContentScrollTo(
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy,
    /* [in] */ Boolean animate)
{
    if (mDrawHistory) {
        // disallow WebView to change the scroll position as History Picture
        // is used in the view system.
        return;
    }
    Int32 vx = ContentToViewX(cx);
    Int32 vy = ContentToViewY(cy);
    PinScrollTo(vx, vy, animate, 0);
}

/**
 * These are from webkit, and are in content coordinate system (unzoomed)
 */
void CWebViewClassic::ContentSizeChanged(
    /* [in] */ Boolean updateLayout)
{
    // suppress 0,0 since we usually see real dimensions soon after
    // this avoids drawing the prev content in a funny place. If we find a
    // way to consolidate these notifications, this check may become
    // obsolete
    if ((mContentWidth | mContentHeight) == 0) {
        return;
    }

    if (mHeightCanMeasure) {
        Int32 height;
        mWebView->GetMeasuredHeight(&height);
        if (height != ContentToViewDimension(mContentHeight)
                || updateLayout) {
            mWebView->RequestLayout();
        }
    }
    else if (mWidthCanMeasure) {
        Int32 width;
        mWebView->GetMeasuredWidth(&width);
        if (width != ContentToViewDimension(mContentWidth)
                || updateLayout) {
            mWebView->RequestLayout();
        }
    }
    else {
        // If we don't request a layout, try to send our view size to the
        // native side to ensure that WebCore has the correct dimensions.
        SendViewSizeZoom(FALSE);
    }
}

/**
 * See {@link WebView#setWebViewClient(WebViewClient)}
 */
ECode CWebViewClassic::SetWebViewClient(
    /* [in] */ IWebViewClient* client)
{
    mCallbackProxy->SetWebViewClient(client);
    return NOERROR;
}

/**
 * Gets the WebViewClient
 * @return the current WebViewClient instance.
 *
 * This is an implementation detail.
 */
ECode CWebViewClassic::GetWebViewClient(
    /* [out] */ IWebViewClient** wvc)
{
    VALIDATE_NOT_NULL(wvc);
    AutoPtr<IWebViewClient> client = mCallbackProxy->GetWebViewClient();
    *wvc = client;
    REFCOUNT_ADD(*wvc);
    return NOERROR;
}

/**
 * See {@link WebView#setDownloadListener(DownloadListener)}
 */
ECode CWebViewClassic::SetDownloadListener(
    /* [in] */ IDownloadListener* listener)
{
    mCallbackProxy->SetDownloadListener(listener);
    return NOERROR;
}

/**
 * See {@link WebView#setWebChromeClient(WebChromeClient)}
 */
ECode CWebViewClassic::SetWebChromeClient(
    /* [in] */ IWebChromeClient* client)
{
    mCallbackProxy->SetWebChromeClient(client);
    return NOERROR;
}

/**
 * Gets the chrome handler.
 * @return the current WebChromeClient instance.
 *
 * This is an implementation detail.
 */
ECode CWebViewClassic::GetWebChromeClient(
    /* [out] */ IWebChromeClient** wcc)
{
    VALIDATE_NOT_NULL(wcc);
    AutoPtr<IWebChromeClient> client = mCallbackProxy->GetWebChromeClient();
    *wcc = client;
    REFCOUNT_ADD(*wcc);
    return NOERROR;
}

/**
 * Set the back/forward list client. This is an implementation of
 * WebBackForwardListClient for handling new items and changes in the
 * history index.
 * @param client An implementation of WebBackForwardListClient.
 */
ECode CWebViewClassic::SetWebBackForwardListClient(
    /* [in] */ IWebBackForwardListClient* client)
{
    mCallbackProxy->SetWebBackForwardListClient(client);
    return NOERROR;
}

/**
 * Gets the WebBackForwardListClient.
 */
ECode CWebViewClassic::GetWebBackForwardListClient(
    /* [out] */ IWebBackForwardListClient** wflc)
{
    VALIDATE_NOT_NULL(wflc);
    AutoPtr<IWebBackForwardListClient> client = mCallbackProxy->GetWebBackForwardListClient();
    *wflc = client;
    REFCOUNT_ADD(*wflc);
    return NOERROR;
}

/**
 * See {@link WebView#setPictureListener(PictureListener)}
 */
ECode CWebViewClassic::SetPictureListener(
    /* [in] */ IWebViewPictureListener* listener)
{
    mPictureListener = listener;
    return NOERROR;
}

/* FIXME: Debug only! Remove for SDK! */
ECode CWebViewClassic::ExternalRepresentation(
    /* [in] */ IMessage* callback)
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::REQUEST_EXT_REPRESENTATION, callback);
    return NOERROR;
}

/* FIXME: Debug only! Remove for SDK! */
ECode CWebViewClassic::DocumentAsText(
    /* [in] */ IMessage* callback)
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::REQUEST_DOC_AS_TEXT, callback);
    return NOERROR;
}

/**
 * See {@link WebView#addJavascriptInterface(Object, String)}
 */
ECode CWebViewClassic::AddJavascriptInterface(
    /* [in] */ IInterface* object,
    /* [in] */ const String& name)
{
    if (object == NULL) {
        return NOERROR;
    }
    AutoPtr<CWebViewCore::JSInterfaceData> arg = new CWebViewCore::JSInterfaceData();

    arg->mObject = object;
    arg->mInterfaceName = name;

    // starting with JELLY_BEAN_MR1, annotations are mandatory for enabling access to
    // methods that are accessible from JS.
    if (/*mContext->getApplicationInfo().targetSdkVersion >= Build.VERSION_CODES.JELLY_BEAN_MR1*/FALSE) {
        arg->mRequireAnnotation = TRUE;
    }
    else {
        arg->mRequireAnnotation = FALSE;
    }

    mWebViewCore->SendMessage(CWebViewCore::EventHub::ADD_JS_INTERFACE, arg.Get());
    return NOERROR;
}

/**
 * See {@link WebView#removeJavascriptInterface(String)}
 */
ECode CWebViewClassic::RemoveJavascriptInterface(
    /* [in] */ const String& interfaceName)
{
    if (mWebViewCore != NULL) {
        AutoPtr<CWebViewCore::JSInterfaceData> arg = new CWebViewCore::JSInterfaceData();
        arg->mInterfaceName = interfaceName;
        mWebViewCore->SendMessage(CWebViewCore::EventHub::REMOVE_JS_INTERFACE, arg.Get());
    }

    return NOERROR;
}

/**
 * See {@link WebView#getSettings()}
 * Note this returns WebSettingsClassic, a sub-#include "webkit/of WebSettings, which can be used
 * to access extension APIs.
 */
ECode CWebViewClassic::GetSettings(
    /* [out] */ IWebSettings** settings)
{
    VALIDATE_NOT_NULL(settings);
    if (mWebViewCore != NULL) {
        AutoPtr<IWebSettingsClassic> classic;
        mWebViewCore->GetSettings((IWebSettingsClassic**)&classic);
        *settings = (IWebSettings*)classic.Get();
        REFCOUNT_ADD(*settings);
        return NOERROR;
    }
    else {
        *settings = NULL;
        return NOERROR;
    }
}

/**
 * See {@link WebView#getPluginList()}
 */
AutoPtr<IPluginList> CWebViewClassic::GetPluginList()
{
    Mutex::Autolock lock(sLock);
    AutoPtr<IPluginList> pl;
    CPluginList::New((IPluginList**)&pl);
    return pl;
}

/**
 * See {@link WebView#refreshPlugins(boolean)}
 */
ECode CWebViewClassic::RefreshPlugins(
    /* [in] */ Boolean reloadOpenPages)
{
    return NOERROR;
}

//-------------------------------------------------------------------------
// Override View methods
//-------------------------------------------------------------------------

void CWebViewClassic::DrawContent(
    /* [in] */ ICanvas* canvas)
{
    if (mDrawHistory) {
        canvas->Scale(mZoomManager->GetScale(), mZoomManager->GetScale());
        canvas->DrawPicture(mHistoryPicture);
        return;
    }
    if (mNativeClass == 0) return;

    Boolean animateZoom = mZoomManager->IsFixedLengthAnimationInProgress();
    Boolean animateScroll = ((!mScroller->IsFinished()
            || mVelocityTracker != NULL)
            && (mTouchMode != TOUCH_DRAG_MODE ||
            mHeldMotionless != MOTIONLESS_TRUE));
    if (mTouchMode == TOUCH_DRAG_MODE) {
        if (mHeldMotionless == MOTIONLESS_PENDING) {
            assert(0);
//            mPrivateHandler.removeMessages(DRAG_HELD_MOTIONLESS);
            mHeldMotionless = MOTIONLESS_FALSE;
        }
        if (mHeldMotionless == MOTIONLESS_FALSE) {
            assert(0);
//            mPrivateHandler.sendMessageDelayed(mPrivateHandler
//                    .obtainMessage(DRAG_HELD_MOTIONLESS), MOTIONLESS_TIME);
            mHeldMotionless = MOTIONLESS_PENDING;
        }
    }
    Int32 saveCount;
    canvas->Save(&saveCount);
    Boolean accelerated = FALSE;
    if (animateZoom) {
        mZoomManager->AnimateZoom(canvas);
    }
    else if (canvas->IsHardwareAccelerated(&accelerated), !accelerated) {
        canvas->Scale(mZoomManager->GetScale(), mZoomManager->GetScale());
    }

    Boolean UIAnimationsRunning = FALSE;
    // Currently for each draw we compute the animation values;
    // We may in the future decide to do that independently.
    if (mNativeClass != 0 && (canvas->IsHardwareAccelerated(&accelerated), !accelerated)
            && NativeEvaluateLayersAnimations(mNativeClass)) {
        UIAnimationsRunning = true;
        // If we have unfinished (or unstarted) animations,
        // we ask for a repaint. We only need to do this in software
        // rendering (with hardware rendering we already have a different
        // method of requesting a repaint)
        mWebViewCore->SendMessage(CWebViewCore::EventHub::NOTIFY_ANIMATION_STARTED);
        Invalidate();
    }

    // decide which adornments to draw
    Int32 extras = DRAW_EXTRAS_NONE;
    if (!mFindIsUp && mShowTextSelectionExtra) {
        extras = DRAW_EXTRAS_SELECTION;
    }

    CalcOurContentVisibleRectF(mVisibleContentRect);
    if (canvas->IsHardwareAccelerated(&accelerated), accelerated) {
        AutoPtr<IRect> invScreenRect = mIsWebViewVisible ? mInvScreenRect : NULL;
        AutoPtr<IRect> screenRect = mIsWebViewVisible ? mScreenRect : NULL;

        Float scale;
        GetScale(&scale);
        Int32 functor = NativeCreateDrawGLFunction(mNativeClass, invScreenRect,
                screenRect, mVisibleContentRect, scale, extras);
        assert(0);
//        ((HardwareCanvas) canvas)->CallDrawGLFunction(functor);
//        if (mHardwareAccelSkia != GetSettings()->GetHardwareAccelSkiaEnabled()) {
//            mHardwareAccelSkia = GetSettings()->GetHardwareAccelSkiaEnabled();
//            NativeUseHardwareAccelSkia(mHardwareAccelSkia);
//        }

    }
    else {
        AutoPtr<IDrawFilter> df;
        if (mZoomManager->IsZoomAnimating() || UIAnimationsRunning) {
            df = mZoomFilter;
        }
        else if (animateScroll) {
            df = mScrollFilter;
        }
        canvas->SetDrawFilter(df);
        NativeDraw(canvas, mVisibleContentRect, mBackgroundColor, extras);
        canvas->SetDrawFilter(NULL);
    }

    canvas->RestoreToCount(saveCount);
    DrawTextSelectionHandles(canvas);

    if (extras == DRAW_EXTRAS_CURSOR_RING) {
        if (mTouchMode == TOUCH_SHORTPRESS_START_MODE) {
            mTouchMode = TOUCH_SHORTPRESS_MODE;
        }
    }
}

/**
 * Draw the background when beyond bounds
 * @param canvas Canvas to draw into
 */
void CWebViewClassic::DrawOverScrollBackground(
    /* [in] */ ICanvas* canvas)
{
    if (mOverScrollBackground == NULL) {
        CPaint::New((IPaint**)&mOverScrollBackground);
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        AutoPtr<IBitmapFactory> factory;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
        AutoPtr<IBitmap> bm;
        factory->DecodeResource(res, R::drawable::status_bar_background, (IBitmap**)&bm);
        AutoPtr<IBitmapShader> bs;
        CBitmapShader::New(bm,
                ShaderTileMode_REPEAT, ShaderTileMode_REPEAT, (IBitmapShader**)&bs);
        mOverScrollBackground->SetShader(bs);
        mOverScrollBorder = NULL;
        CPaint::New((IPaint**)&mOverScrollBorder);
        mOverScrollBorder->SetStyle(PaintStyle_STROKE);
        mOverScrollBorder->SetStrokeWidth(0);
        mOverScrollBorder->SetColor(0xffbbbbbb);
    }

    Int32 top = 0;
    Int32 right = ComputeRealHorizontalScrollRange();
    Int32 range = ComputeRealVerticalScrollRange();
    Int32 bottom = top + range;
    // first draw the background and anchor to the top of the view
    canvas->Save(NULL);
    canvas->Translate(GetScrollX(), GetScrollY());
    Boolean result;
    canvas->ClipRect(-GetScrollX(), top - GetScrollY(), right - GetScrollX(), bottom
            - GetScrollY(), RegionOp_DIFFERENCE, &result);
    canvas->DrawPaint(mOverScrollBackground);
    canvas->Restore();
    // then draw the border
    canvas->DrawRect(-1, top - 1, right, bottom, mOverScrollBorder);
    // next clip the region for the content
    canvas->ClipRect(0, top, right, bottom, &result);
}

ECode CWebViewClassic::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    if (InFullScreenMode()) {
        return NOERROR; // no need to draw anything if we aren't visible.
    }
    // if mNativeClass is 0, the WebView is either destroyed or not
    // initialized. In either case, just draw the background color and return
    if (mNativeClass == 0) {
        canvas->DrawColor(mBackgroundColor);
        return NOERROR;
    }

    // if both mContentWidth and mContentHeight are 0, it means there is no
    // valid Picture passed to WebView yet. This can happen when WebView
    // just starts. Draw the background and return.
    if ((mContentWidth | mContentHeight) == 0 && mHistoryPicture == NULL) {
        canvas->DrawColor(mBackgroundColor);
        return NOERROR;
    }

    Boolean accelerated = FALSE;
    if (canvas->IsHardwareAccelerated(&accelerated), accelerated) {
        mZoomManager->SetHardwareAccelerated();
    }
    else {
        mWebViewCore->ResumeWebKitDraw();
    }

    Int32 saveCount;
    canvas->Save(&saveCount);
    if (mInOverScrollMode) {
        AutoPtr<IWebSettings> settings;
        GetSettings((IWebSettings**)&settings);
        Boolean result;
        settings->GetUseWebViewBackgroundForOverscrollBackground(&result);
        if (!result) {
            DrawOverScrollBackground(canvas);
        }
    }

    canvas->Translate(0, GetTitleHeight());
    DrawContent(canvas);
    canvas->RestoreToCount(saveCount);

    if (AUTO_REDRAW_HACK && mAutoRedraw) {
        Invalidate();
    }
    mWebViewCore->SignalRepaintDone();

    if (mOverScrollGlow != NULL && mOverScrollGlow->DrawEdgeGlows(canvas)) {
        Invalidate();
    }

    if (mFocusTransition != NULL) {
        mFocusTransition->Draw(canvas);
    }
    else if (ShouldDrawHighlightRect()) {
        AutoPtr<IRegionIterator> iter;
        CRegionIterator::New(mTouchHighlightRegion, (IRegionIterator**)&iter);
        AutoPtr<IRect> r;
        CRect::New((IRect**)&r);
        Boolean hasNext = FALSE;
        while (iter->Next(r, &hasNext), hasNext) {
            canvas->DrawRect(r, mTouchHightlightPaint);
        }
    }
    if (DEBUG_TOUCH_HIGHLIGHT) {
        AutoPtr<IWebSettings> settings;
        GetSettings((IWebSettings**)&settings);
        Boolean enabled = FALSE;
        if (settings->GetNavDump(&enabled), enabled) {
            if ((mTouchHighlightX | mTouchHighlightY) != 0) {
                if (mTouchCrossHairColor == NULL) {
                    CPaint::New((IPaint**)&mTouchCrossHairColor);
                    mTouchCrossHairColor->SetColor(IColor::RED);
                }
                canvas->DrawLine(mTouchHighlightX - mNavSlop,
                        mTouchHighlightY - mNavSlop, mTouchHighlightX
                                + mNavSlop + 1, mTouchHighlightY + mNavSlop
                                + 1, mTouchCrossHairColor);
                canvas->DrawLine(mTouchHighlightX + mNavSlop + 1,
                        mTouchHighlightY - mNavSlop, mTouchHighlightX
                                - mNavSlop,
                        mTouchHighlightY + mNavSlop + 1,
                        mTouchCrossHairColor);
            }
        }
    }
    return NOERROR;
}

void CWebViewClassic::RemoveTouchHighlight()
{
    SetTouchHighlightRects(NULL);
}

ECode CWebViewClassic::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams* params)
{
    Int32 height;
    params->GetHeight(&height);
    if (height == IAbsoluteLayoutLayoutParams::WRAP_CONTENT) {
        mWrapContent = TRUE;
    }
    mWebViewPrivate->Super_setLayoutParams(params);
    return NOERROR;
}

ECode CWebViewClassic::PerformLongClick(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // performLongClick() is the result of a delayed message. If we switch
    // to windows overview, the WebView will be temporarily removed from the
    // view system. In that case, do nothing.
    AutoPtr<IViewParent> viewParent;
    mWebView->GetParent((IViewParent**)&viewParent);
    if (viewParent == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    // A multi-finger gesture can look like a long press; make sure we don't take
    // long press actions if we're scaling.
    AutoPtr<IScaleGestureDetector> detector = mZoomManager->GetScaleGestureDetector();
    Boolean isInProgress = FALSE;
    if (detector != NULL && (detector->IsInProgress(&isInProgress), isInProgress)) {
        *result = FALSE;
        return NOERROR;
    }

    if (mSelectingText) {
        *result = FALSE; // long click does nothing on selection
        return NOERROR;
    }
    /* if long click brings up a context menu, the super function
     * returns true and we're done. Otherwise, nothing happened when
     * the user clicked. */
    Boolean r;
    if (mWebViewPrivate->Super_performLongClick(&r), r) {
        *result = TRUE;
        return NOERROR;
    }
    /* In the case where the application hasn't already handled the long
     * click action, look for a word under the  click. If one is found,
     * animate the text selection into view.
     * FIXME: no animation code yet */
    Boolean isSelecting;
    SelectText(&isSelecting);
    if (isSelecting) {
        mWebView->PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS, result);
    }
    else if (FocusCandidateIsEditableText()) {
        mSelectCallback = new SelectActionModeCallback();
        mSelectCallback->SetWebView(this);
        mSelectCallback->SetTextSelected(FALSE);
        AutoPtr<IActionMode> mode;
        mWebView->StartActionMode(mSelectCallback, (IActionMode**)&mode);
    }
    *result = isSelecting;
    return NOERROR;
}

/**
 * Select the word at the last click point.
 *
 * This is an implementation detail.
 */
ECode CWebViewClassic::SelectText(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 x = ViewToContentX(mLastTouchX + GetScrollX());
    Int32 y = ViewToContentY(mLastTouchY + GetScrollY());
    *result = SelectText(x, y);
    return NOERROR;
}

/**
 * Select the word at the indicated content coordinates.
 */
Boolean CWebViewClassic::SelectText(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mWebViewCore == NULL) {
        return FALSE;
    }
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SELECT_WORD_AT, x, y);
    return TRUE;
}

ECode CWebViewClassic::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    mCachedOverlappingActionModeHeight = -1;
    Int32 orientation;
    newConfig->GetOrientation(&orientation);
    if (mSelectingText && mOrientation != orientation) {
        SelectionDone();
    }
    newConfig->GetOrientation(&mOrientation);
    if (mWebViewCore != NULL && !mBlockWebkitViewMessages) {
        mWebViewCore->SendMessage(CWebViewCore::EventHub::CLEAR_CONTENT);
    }
    return NOERROR;
}

void CWebViewClassic::SetBaseLayer(
    /* [in] */ Int32 layer,
    /* [in] */ Boolean showVisualIndicator,
    /* [in] */ Boolean isPictureAfterFirstLayout)
{
    if (mNativeClass == 0) {
        return;
    }
    Boolean queueFull;
    Int32 scrollingLayer = (mTouchMode == TOUCH_DRAG_LAYER_MODE)
            ? mCurrentScrollingLayerId : 0;
    queueFull = NativeSetBaseLayer(mNativeClass, layer,
           showVisualIndicator, isPictureAfterFirstLayout,
           scrollingLayer);

    if (queueFull) {
        mWebViewCore->PauseWebKitDraw();
    }
    else {
        mWebViewCore->ResumeWebKitDraw();
    }

    if (mHTML5VideoViewProxy != NULL) {
        mHTML5VideoViewProxy->SetBaseLayer(layer);
    }
}

Int32 CWebViewClassic::GetBaseLayer()
{
    if (mNativeClass == 0) {
        return 0;
    }
    return NativeGetBaseLayer(mNativeClass);
}

void CWebViewClassic::OnZoomAnimationStart()
{

}

void CWebViewClassic::OnZoomAnimationEnd()
{
    Boolean result;
    mPrivateHandler->SendEmptyMessage(RELOCATE_AUTO_COMPLETE_POPUP, &result);
}

void CWebViewClassic::OnFixedLengthZoomAnimationStart()
{
    AutoPtr<IWebViewCore> wvc;
    GetWebViewCore((IWebViewCore**)&wvc);
    CWebViewCore::PauseUpdatePicture((CWebViewCore*)wvc.Get());
    OnZoomAnimationStart();
}

void CWebViewClassic::OnFixedLengthZoomAnimationEnd()
{
    if (!mBlockWebkitViewMessages && !mSelectingText) {
        CWebViewCore::ResumeUpdatePicture(mWebViewCore);
    }
    OnZoomAnimationEnd();
}

void CWebViewClassic::StartSelectingText()
{
    mSelectingText = TRUE;
    mShowTextSelectionExtra = TRUE;
    AnimateHandles();
}

void CWebViewClassic::AnimateHandle(
    /* [in] */ Boolean canShow,
    /* [in] */ IObjectAnimator* animator,
    /* [in] */ IPoint* selectionPoint,
    /* [in] */ Int32 selectionLayerId,
    /* [in] */ SelectionHandleAlpha* alpha)
{
    Boolean isVisible = canShow && mSelectingText
            && ((mSelectionStarted && mSelectDraggingCursor.Get() == selectionPoint)
            || IsHandleVisible(selectionPoint, selectionLayerId));
    Int32 targetValue = isVisible ? 255 : 0;
    if (targetValue != alpha->GetTargetAlpha()) {
        alpha->SetTargetAlpha(targetValue);
        AutoPtr< ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(1);
        (*values)[0] = targetValue;
        animator->SetInt32Values(values);
        animator->SetDuration(SELECTION_HANDLE_ANIMATION_MS);
        animator->Start();
    }
}

void CWebViewClassic::AnimateHandles()
{
    Boolean canShowBase = mSelectingText;
    Boolean canShowExtent = mSelectingText && !mIsCaretSelection;
    AnimateHandle(canShowBase, mBaseHandleAlphaAnimator, mSelectCursorBase,
            mSelectCursorBaseLayerId, mBaseAlpha);
    AnimateHandle(canShowExtent, mExtentHandleAlphaAnimator,
            mSelectCursorExtent, mSelectCursorExtentLayerId,
            mExtentAlpha);
}

void CWebViewClassic::EndSelectingText()
{
    mSelectingText = FALSE;
    mShowTextSelectionExtra = FALSE;
    AnimateHandles();
}

void CWebViewClassic::EnsureSelectionHandles()
{
    if (mSelectHandleCenter == NULL) {
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        res->GetDrawable(R::drawable::text_select_handle_middle, (IDrawable**)&mSelectHandleCenter);
        AutoPtr<IDrawable> dr;
        mSelectHandleCenter->Mutate((IDrawable**)&dr);
        mSelectHandleLeft = NULL;
        res->GetDrawable(R::drawable::text_select_handle_left, (IDrawable**)&mSelectHandleLeft);
        dr = NULL;
        mSelectHandleLeft->Mutate((IDrawable**)&dr);
        mSelectHandleRight = NULL;
        res->GetDrawable(R::drawable::text_select_handle_right, (IDrawable**)&mSelectHandleRight);
        dr = NULL;
        mSelectHandleRight->Mutate((IDrawable**)&dr);
        // All handles have the same height, so we can save effort with
        // this assumption.
        Int32 height;
        mSelectHandleLeft->GetIntrinsicHeight(&height);
        mSelectOffset = NULL;
        CPoint::New(0, -height, (IPoint**)&mSelectOffset);
    }
}

void CWebViewClassic::DrawHandle(
    /* [in] */ IPoint* point,
    /* [in] */ Int32 handleId,
    /* [in] */ IRect* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ ICanvas* canvas)
{
    Int32 offset;
    Int32 width;
    Int32 height;
    AutoPtr<IDrawable> drawable;
    Boolean isLeft = NativeIsHandleLeft(mNativeClass, handleId);
    if (isLeft) {
        drawable = mSelectHandleLeft;
        mSelectHandleLeft->GetIntrinsicWidth(&width);
        mSelectHandleLeft->GetIntrinsicHeight(&height);
        // Magic formula copied from TextView
        offset = (width * 3) / 4;
    }
    else {
        drawable = mSelectHandleRight;
        mSelectHandleRight->GetIntrinsicWidth(&width);
        mSelectHandleRight->GetIntrinsicHeight(&height);
        // Magic formula copied from TextView
        offset = width / 4;
    }
    Int32 pointX, pointY;
    point->GetX(&pointX);
    point->GetY(&pointY);
    Int32 x = ContentToViewDimension(pointX);
    Int32 y = ContentToViewDimension(pointY);
    bounds->Set(x - offset, y, x - offset + width, y + height);
    drawable->SetBounds(bounds);
    drawable->SetAlpha(alpha);
    drawable->Draw(canvas);
}

void CWebViewClassic::DrawTextSelectionHandles(
    /* [in] */ ICanvas* canvas)
{
    if (mBaseAlpha->GetAlpha() == 0 && mExtentAlpha->GetAlpha() == 0) {
        return;
    }
    EnsureSelectionHandles();
    if (mIsCaretSelection) {
        // Caret handle is centered
        Int32 pointX, pointY;
        mSelectCursorBase->GetX(&pointX);
        mSelectCursorBase->GetY(&pointY);
        Int32 width, height;
        mSelectHandleCenter->GetIntrinsicWidth(&width);
        Int32 x = ContentToViewDimension(pointX) - (width / 2);
        Int32 y = ContentToViewDimension(pointY);
        mSelectHandleCenter->GetIntrinsicWidth(&width);
        mSelectHandleCenter->GetIntrinsicHeight(&height);
        mSelectHandleBaseBounds->Set(x, y, x + width, y + height);
        mSelectHandleCenter->SetBounds(mSelectHandleBaseBounds);
        mSelectHandleCenter->SetAlpha(mBaseAlpha->GetAlpha());
        mSelectHandleCenter->Draw(canvas);
    }
    else {
        DrawHandle(mSelectCursorBase, HANDLE_ID_BASE,
                mSelectHandleBaseBounds, mBaseAlpha->GetAlpha(), canvas);
        DrawHandle(mSelectCursorExtent, HANDLE_ID_EXTENT,
                mSelectHandleExtentBounds, mExtentAlpha->GetAlpha(), canvas);
    }
}

Boolean CWebViewClassic::IsHandleVisible(
    /* [in] */ IPoint* selectionPoint,
    /* [in] */ Int32 layerId)
{
    Boolean isVisible = TRUE;
    Int32 pointX, pointY;
    if (mIsEditingText) {
        selectionPoint->GetY(&pointY);
        selectionPoint->GetX(&pointX);
        mEditTextContentBounds->Contains(pointX,
                pointY, &isVisible);
    }
    if (isVisible) {
        selectionPoint->GetY(&pointY);
        selectionPoint->GetX(&pointX);
        isVisible = NativeIsPointVisible(mNativeClass, layerId,
                pointX, pointY);
    }
    return isVisible;
}

/**
 * Takes an int[4] array as an output param with the values being
 * startX, startY, endX, endY
 */
void CWebViewClassic::GetSelectionHandles(
    /* [in] */ ArrayOf<Int32>* handles)
{
    mSelectCursorBase->GetX(&((*handles)[0]));
    mSelectCursorBase->GetY(&((*handles)[1]));
    mSelectCursorExtent->GetX(&((*handles)[2]));
    mSelectCursorExtent->GetY(&((*handles)[3]));
}

// Only check the flag, can be called from WebCore thread
Boolean CWebViewClassic::DrawHistory()
{
    return mDrawHistory;
}

Int32 CWebViewClassic::GetHistoryPictureWidth()
{
    Int32 width = 0;
    if (mHistoryPicture != NULL) {
        mHistoryPicture->GetWidth(&width);
    }
    return width;
}

// Should only be called in UI thread
void CWebViewClassic::SwitchOutDrawHistory()
{
    if (NULL == mWebViewCore) return; // CallbackProxy may trigger this
    Int32 progress;
    if (mDrawHistory && ((GetProgress(&progress), progress == 100) || NativeHasContent())) {
        mDrawHistory = FALSE;
        mHistoryPicture = NULL;
        Invalidate();
        Int32 oldScrollX = GetScrollX();
        Int32 oldScrollY = GetScrollY();
        SetScrollXRaw(PinLocX(GetScrollX()));
        SetScrollYRaw(PinLocY(GetScrollY()));
        if (oldScrollX != GetScrollX() || oldScrollY != GetScrollY()) {
            mWebViewPrivate->OnScrollChanged(GetScrollX(), GetScrollY(), oldScrollX, oldScrollY);
        }
        else {
            SendOurVisibleRect();
        }
    }
}

/**
 *  Delete text from start to end in the focused textfield. If there is no
 *  focus, or if start == end, silently fail.  If start and end are out of
 *  order, swap them.
 *  @param  start   Beginning of selection to delete.
 *  @param  end     End of selection to delete.
 */
void CWebViewClassic::DeleteSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    mTextGeneration++;
    AutoPtr<CWebViewCoreTextSelectionData> data
            = new CWebViewCoreTextSelectionData(start, end, 0);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::DELETE_SELECTION, mTextGeneration, 0,
            data.Get());
}

/**
 *  Set the selection to (start, end) in the focused textfield. If start and
 *  end are out of order, swap them.
 *  @param  start   Beginning of selection.
 *  @param  end     End of selection.
 */
void CWebViewClassic::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (mWebViewCore != NULL) {
        mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_SELECTION, start, end);
    }
}

ECode CWebViewClassic::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs,
    /* [out] */ IInputConnection** ic)
{
    VALIDATE_NOT_NULL(ic);
    if (mInputConnection == NULL) {
        mInputConnection = new CWebViewClassicWebViewInputConnection(this);
        mAutoCompletePopup = new AutoCompletePopup(this, mInputConnection);
    }
    mInputConnection->SetupEditorInfo(outAttrs);
    *ic = (IInputConnection*)mInputConnection.Get();
    REFCOUNT_ADD(*ic);
    return NOERROR;
}

void CWebViewClassic::RelocateAutoCompletePopup()
{
    if (mAutoCompletePopup != NULL) {
        mAutoCompletePopup->ResetRect();
        AutoPtr<IEditable> editable;
        mInputConnection->GetEditable((IEditable**)&editable);
        mAutoCompletePopup->SetText(editable);
    }
}

/**
 * Called in response to a message from webkit telling us that the soft
 * keyboard should be launched.
 */
void CWebViewClassic::DisplaySoftKeyboard(
    /* [in] */ Boolean isTextView)
{
    AutoPtr<IInputMethodManager> imm;
    mContext->GetSystemService(IContext::INPUT_METHOD_SERVICE, (IInterface**)&imm);

    // bring it back to the default level scale so that user can enter text
    Boolean zoom = mZoomManager->GetScale() < mZoomManager->GetDefaultScale();
    if (zoom) {
        mZoomManager->SetZoomCenter(mLastTouchX, mLastTouchY);
        mZoomManager->SetZoomScale(mZoomManager->GetDefaultScale(), FALSE);
    }
    // Used by plugins and contentEditable.
    // Also used if the navigation cache is out of date, and
    // does not recognize that a textfield is in focus.  In that
    // case, use WebView as the targeted view.
    // see http://b/issue?id=2457459
    Boolean result;
    imm->ShowSoftInput(mWebView, 0, &result);
}

// Called by WebKit to instruct the UI to hide the keyboard
void CWebViewClassic::HideSoftKeyboard()
{
    AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
    Boolean isActive = FALSE;
    if (imm != NULL && ((imm->IsActive(mWebView, &isActive), isActive))) {
        AutoPtr<IBinder> token;
        mWebView->GetWindowToken((IBinder**)&token);
        Boolean result;
        imm->HideSoftInputFromWindow(token, 0, &result);
    }
}

/**
 * Called by AutoCompletePopup to find saved form data associated with the
 * textfield
 * @param name Name of the textfield.
 * @param nodePointer Pointer to the node of the textfield, so it can be
 *          compared to the currently focused textfield when the data is
 *          retrieved.
 * @param autoFillable true if WebKit has determined this field is part of
 *          a form that can be auto filled.
 * @param autoComplete true if the attribute "autocomplete" is set to true
 *          on the textfield.
 */
void CWebViewClassic::RequestFormData(
    /* [in] */ const String& name,
    /* [in] */ Int32 nodePointer,
    /* [in] */ Boolean autoFillable,
    /* [in] */ Boolean autoComplete)
{
    AutoPtr<IWebSettingsClassic> settings;
    mWebViewCore->GetSettings((IWebSettingsClassic**)&settings);
    Boolean result;
    if (settings->GetSaveFormData(&result), result) {
        AutoPtr<IMessageHelper> helper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
        AutoPtr<IMessage> update;
        helper->Obtain(mPrivateHandler, REQUEST_FORM_DATA, (IMessage**)&update);
        update->SetArg1(nodePointer);
        String url;
        GetUrl(&url);
        AutoPtr<_RequestFormData> updater = new _RequestFormData(name, url,
                update, autoFillable, autoComplete, this);
        AutoPtr<IThread> t;
        CThread::New(updater, (IThread**)&t);
        t->Start();
    }
}

/**
 * Dump the display tree to "/sdcard/displayTree.txt"
 *
 * debug only
 */
ECode CWebViewClassic::DumpDisplayTree()
{
    String url;
    GetUrl(&url);
    NativeDumpDisplayTree(url);
    return NOERROR;
}

/**
 * Dump the dom tree to adb shell if "toFile" is False, otherwise dump it to
 * "/sdcard/domTree.txt"
 *
 * debug only
 */
ECode CWebViewClassic::DumpDomTree(
    /* [in] */ Boolean toFile)
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::DUMP_DOMTREE, toFile ? 1 : 0, 0);
    return NOERROR;
}

/**
 * Dump the render tree to adb shell if "toFile" is False, otherwise dump it
 * to "/sdcard/renderTree.txt"
 *
 * debug only
 */
ECode CWebViewClassic::DumpRenderTree(
    /* [in] */ Boolean toFile)
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::DUMP_RENDERTREE, toFile ? 1 : 0, 0);
    return NOERROR;
}

/**
 * Called by DRT on UI thread, need to proxy to WebCore thread.
 *
 * debug only
 */
ECode CWebViewClassic::SetUseMockDeviceOrientation()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_USE_MOCK_DEVICE_ORIENTATION);
    return NOERROR;
}

/**
 * Sets use of the Geolocation mock client. Also resets that client. Called
 * by DRT on UI thread, need to proxy to WebCore thread.
 *
 * debug only
 */
ECode CWebViewClassic::SetUseMockGeolocation()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_USE_MOCK_GEOLOCATION);
    return NOERROR;
}

/**
 * Called by DRT on WebCore thread.
 *
 * debug only
 */
ECode CWebViewClassic::SetMockGeolocationPosition(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double accuracy)
{
    mWebViewCore->SetMockGeolocationPosition(latitude, longitude, accuracy);
    return NOERROR;
}

/**
 * Called by DRT on WebCore thread.
 *
 * debug only
 */
ECode CWebViewClassic::SetMockGeolocationError(
    /* [in] */ Int32 code,
    /* [in] */ const String& message)
{
    mWebViewCore->SetMockGeolocationError(code, message);
    return NOERROR;
}

/**
 * Called by DRT on WebCore thread.
 *
 * debug only
 */
ECode CWebViewClassic::SetMockGeolocationPermission(
    /* [in] */ Boolean allow)
{
    mWebViewCore->SetMockGeolocationPermission(allow);
    return NOERROR;
}

/**
 * Called by DRT on WebCore thread.
 *
 * debug only
 */
ECode CWebViewClassic::SetMockDeviceOrientation(
    /* [in] */ Boolean canProvideAlpha,
    /* [in] */ Double alpha,
    /* [in] */ Boolean canProvideBeta,
    /* [in] */ Double beta,
    /* [in] */ Boolean canProvideGamma,
    /* [in] */ Double gamma)
{
    mWebViewCore->SetMockDeviceOrientation(canProvideAlpha, alpha, canProvideBeta, beta,
            canProvideGamma, gamma);
    return NOERROR;
}

ECode CWebViewClassic::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mBlockWebkitViewMessages) {
        *result = FALSE;
        return NOERROR;
    }
    // send complex characters to webkit for use by JS and plugins
    String str;
    if (keyCode == IKeyEvent::KEYCODE_UNKNOWN && (event->GetCharacters(&str), !str.IsNull())) {
        // pass the key to DOM
        SendBatchableInputMessage(CWebViewCore::EventHub::KEY_DOWN, 0, 0, event);
        SendBatchableInputMessage(CWebViewCore::EventHub::KEY_UP, 0, 0, event);
        // return true as DOM handles the key
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

Boolean CWebViewClassic::IsEnterActionKey(
    /* [in] */ Int32 keyCode)
{
    return keyCode == IKeyEvent::KEYCODE_DPAD_CENTER
            || keyCode == IKeyEvent::KEYCODE_ENTER
            || keyCode == IKeyEvent::KEYCODE_NUMPAD_ENTER;
}

ECode CWebViewClassic::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mAutoCompletePopup != NULL) {
        *result = mAutoCompletePopup->OnKeyPreIme(keyCode, event);
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CWebViewClassic::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (DebugFlags::WEB_VIEW) {
        Int32 unicodeChar;
        event->GetUnicodeChar(&unicodeChar);
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 now;
        system->GetCurrentTimeMillis(&now);
        Logger::V(LOGTAG, "keyDown at %lld, keyCode=%d, event:%p, unicode=0x%x", now,
                keyCode, event, unicodeChar);
    }
    if (mIsCaretSelection) {
        SelectionDone();
    }
    if (mBlockWebkitViewMessages) {
        *result = FALSE;
        return NOERROR;
    }

    // don't implement accelerator keys here; defer to host application
    Boolean pressed = FALSE;
    if (event->IsCtrlPressed(&pressed), pressed) {
        *result = FALSE;
        return NOERROR;
    }

    if (mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }

    // do this hack up front, so it always works, regardless of touch-mode
    if (AUTO_REDRAW_HACK && (keyCode == IKeyEvent::KEYCODE_CALL)) {
        mAutoRedraw = !mAutoRedraw;
        if (mAutoRedraw) {
            Invalidate();
        }
        *result = TRUE;
        return NOERROR;
    }

    // Bubble up the key event if
    // 1. it is a system key; or
    // 2. the host application wants to handle it;
    Boolean isSystem = FALSE;
    if ((event->IsSystem(&isSystem), isSystem)
            || mCallbackProxy->UiOverrideKeyEvent(event)) {
        *result = FALSE;
        return NOERROR;
    }

    // See if the accessibility injector needs to handle this event.
    if (IsAccessibilityInjectionEnabled()
            && GetAccessibilityInjector()->HandleKeyEventIfNecessary(event)) {
        *result = TRUE;
        return NOERROR;
    }

    if (keyCode == IKeyEvent::KEYCODE_PAGE_UP) {
        Boolean modifiers = FALSE;
        if (event->HasNoModifiers(&modifiers), modifiers) {
            PageUp(FALSE, result);
            *result = TRUE;
            return NOERROR;
        }
        else if (event->HasModifiers(IKeyEvent::META_ALT_ON, &modifiers), modifiers) {
            PageUp(TRUE, result);
            *result = TRUE;
            return NOERROR;
        }
    }

    if (keyCode == IKeyEvent::KEYCODE_PAGE_DOWN) {
        Boolean modifiers = FALSE;
        if (event->HasNoModifiers(&modifiers), modifiers) {
            PageDown(FALSE, result);
            *result = TRUE;
            return NOERROR;
        }
        else if (event->HasModifiers(IKeyEvent::META_ALT_ON, &modifiers), modifiers) {
            PageDown(TRUE, result);
            *result = TRUE;
            return NOERROR;
        }
    }

    Boolean modifiers = FALSE;
    if (keyCode == IKeyEvent::KEYCODE_MOVE_HOME && (event->HasNoModifiers(&modifiers), modifiers)) {
        PageUp(TRUE, result);
        *result = TRUE;
        return NOERROR;
    }

    if (keyCode == IKeyEvent::KEYCODE_MOVE_END && (event->HasNoModifiers(&modifiers), modifiers)) {
        PageDown(TRUE, result);
        *result = TRUE;
        return NOERROR;
    }

    if (keyCode >= IKeyEvent::KEYCODE_DPAD_UP
            && keyCode <= IKeyEvent::KEYCODE_DPAD_RIGHT) {
        SwitchOutDrawHistory();
    }

    if (IsEnterActionKey(keyCode)) {
        SwitchOutDrawHistory();
        Int32 count = 0;
        if (event->GetRepeatCount(&count), count == 0) {
            if (mSelectingText) {
                *result = TRUE;
                return NOERROR; // discard press if copy in progress
            }
            mGotCenterDown = TRUE;
            AutoPtr<IMessageHelper> helper;
            CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
            AutoPtr<IMessage> msg;
            helper->Obtain(mPrivateHandler, LONG_PRESS_CENTER, (IMessage**)&msg);
            mPrivateHandler->SendMessageDelayed(msg, LONG_PRESS_TIMEOUT, result);
        }
    }

    AutoPtr<IWebSettings> settings;
    GetSettings((IWebSettings**)&settings);
    Boolean enabled = FALSE;
    if (settings->GetNavDump(&enabled), enabled) {
        switch (keyCode) {
            case IKeyEvent::KEYCODE_4:
                DumpDisplayTree();
                break;
            case IKeyEvent::KEYCODE_5:
            case IKeyEvent::KEYCODE_6:
                DumpDomTree(keyCode == IKeyEvent::KEYCODE_5);
                break;
            case IKeyEvent::KEYCODE_7:
            case IKeyEvent::KEYCODE_8:
                DumpRenderTree(keyCode == IKeyEvent::KEYCODE_7);
                break;
        }
    }

    // pass the key to DOM
    SendKeyEvent(event);
    // return true as DOM handles the key
    *result = TRUE;
    return NOERROR;
}

ECode CWebViewClassic::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (DebugFlags::WEB_VIEW) {
        Int32 unicodeChar;
        event->GetUnicodeChar(&unicodeChar);
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 now;
        system->GetCurrentTimeMillis(&now);
        Logger::V(LOGTAG, "keyUp at %lld, event:%p, unicode=%d", now,
                event, unicodeChar);
    }
    if (mBlockWebkitViewMessages) {
        *result = FALSE;
        return NOERROR;
    }

    if (mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }

    // special CALL handling when cursor node's href is "tel:XXX"
    Int32 type;
    if (keyCode == IKeyEvent::KEYCODE_CALL
            && mInitialHitTestResult != NULL
            && (mInitialHitTestResult->GetType(&type), type == IWebViewHitTestResult::PHONE_TYPE)) {
        String text;
        mInitialHitTestResult->GetExtra(&text);
        AutoPtr<IUri> uri;
        Uri::Parse(text, (IUri**)&uri);
        AutoPtr<IIntent> intent;
        CIntent::New(IIntent::ACTION_DIAL, uri, (IIntent**)&intent);
        mContext->StartActivity(intent);
        *result = TRUE;
        return NOERROR;
    }

    // Bubble up the key event if
    // 1. it is a system key; or
    // 2. the host application wants to handle it;
    Boolean isSystem = FALSE;
    if ((event->IsSystem(&isSystem), isSystem)
            || mCallbackProxy->UiOverrideKeyEvent(event)) {
        *result = FALSE;
        return NOERROR;
    }

    // See if the accessibility injector needs to handle this event.
    if (IsAccessibilityInjectionEnabled()
            && GetAccessibilityInjector()->HandleKeyEventIfNecessary(event)) {
        *result = TRUE;
        return NOERROR;
    }

    if (IsEnterActionKey(keyCode)) {
        // remove the long press message first
        mPrivateHandler->RemoveMessages(LONG_PRESS_CENTER);
        mGotCenterDown = FALSE;

        if (mSelectingText) {
            CopySelection(result);
            SelectionDone();
            *result = TRUE;
            return NOERROR; // discard press if copy in progress
        }
    }

    // pass the key to DOM
    SendKeyEvent(event);
    // return true as DOM handles the key
    *result = TRUE;
    return NOERROR;
}

Boolean CWebViewClassic::StartSelectActionMode()
{
    mSelectCallback = new SelectActionModeCallback();
    mSelectCallback->SetTextSelected(!mIsCaretSelection);
    mSelectCallback->SetWebView(this);
    AutoPtr<IActionMode> actionMode;
    mWebView->StartActionMode(mSelectCallback, (IActionMode**)&actionMode);
    if (actionMode == NULL) {
        // There is no ActionMode, so do not allow the user to modify a
        // selection.
        SelectionDone();
        return FALSE;
    }
    Boolean result;
    mWebView->PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS, &result);
    return TRUE;
}

void CWebViewClassic::ShowPasteWindow()
{
    AutoPtr<IClipboardManager> cm;
    mContext->GetSystemService(IContext::CLIPBOARD_SERVICE, (IInterface**)&cm);

    Boolean hasPrimaryClip = FALSE;
    if (cm->HasPrimaryClip(&hasPrimaryClip), hasPrimaryClip) {
        Int32 x, y;
        mSelectCursorBase->GetX(&x);
        mSelectCursorBase->GetY(&y);
        AutoPtr<IPoint> cursorPoint;
        CPoint::New(ContentToViewX(x),
                ContentToViewY(y), (IPoint**)&cursorPoint);
        AutoPtr<IPoint> cursorTop = CalculateBaseCaretTop();
        Int32 pointX, pointY;
        cursorTop->GetX(&pointX);
        cursorTop->GetY(&pointY);
        cursorTop->Set(ContentToViewX(pointX),
                ContentToViewY(pointY));

        Int32 l0, l1;
        mWebView->GetLocationInWindow(&l0, &l1);
        Int32 offsetX = l0 - GetScrollX();
        Int32 offsetY = l1 - GetScrollY();
        cursorPoint->Offset(offsetX, offsetY);
        cursorTop->Offset(offsetX, offsetY);
        if (mPasteWindow == NULL) {
            mPasteWindow = new PastePopupWindow(this);
        }
        mPasteWindow->Show(cursorPoint, cursorTop, l0, l1);
    }
}

/**
 * Given segment AB, this finds the point C along AB that is closest to
 * point and then returns it scale along AB. The scale factor is AC/AB.
 *
 * @param x The x coordinate of the point near segment AB that determines
 * the scale factor.
 * @param y The y coordinate of the point near segment AB that determines
 * the scale factor.
 * @param a The first point of the line segment.
 * @param b The second point of the line segment.
 * @return The scale factor AC/AB, where C is the point on AB closest to
 *         point.
 */
Float CWebViewClassic::ScaleAlongSegment(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IPointF* a,
    /* [in] */ IPointF* b)
{
    // The bottom line of the text box is line AB
    Float aX, aY, bX, bY;
    a->GetX(&aX);
    a->GetY(&aY);
    b->GetX(&bX);
    b->GetY(&bY);
    Float abX = bX - aX;
    Float abY = bY - aY;
    Float ab2 = (abX * abX) + (abY * abY);

    // The line from first point in text bounds to bottom is AP
    Float apX = x - aX;
    Float apY = y - aY;
    Float abDotAP = (apX * abX) + (apY * abY);
    Float scale = abDotAP / ab2;
    return scale;
}

AutoPtr<IPoint> CWebViewClassic::CalculateBaseCaretTop()
{
    return CalculateCaretTop(mSelectCursorBase, mSelectCursorBaseTextQuad);
}

AutoPtr<IPoint> CWebViewClassic::CalculateDraggingCaretTop()
{
    return CalculateCaretTop(mSelectDraggingCursor, mSelectDraggingTextQuad);
}

/**
 * Assuming arbitrary shape of a quadralateral forming text bounds, this
 * calculates the top of a caret.
 */
AutoPtr<IPoint> CWebViewClassic::CalculateCaretTop(
    /* [in] */ IPoint* base,
    /* [in] */ QuadF* quad)
{
    Int32 baseX, baseY;
    base->GetX(&baseX);
    base->GetY(&baseY);
    Float scale = ScaleAlongSegment(baseX, baseY, quad->p4, quad->p3);
    Float p1X, p1Y, p2X, p2Y;
    quad->p1->GetX(&p1X);
    quad->p1->GetY(&p1Y);
    quad->p2->GetX(&p2X);
    quad->p2->GetY(&p2Y);
    Int32 x = Elastos::Core::Math::Round(ScaleCoordinate(scale, p1X, p2X));
    Int32 y = Elastos::Core::Math::Round(ScaleCoordinate(scale, p1Y, p2Y));
    AutoPtr<IPoint> point;
    CPoint::New(x, y, (IPoint**)&point);
    return point;
}

void CWebViewClassic::HidePasteButton()
{
    if (mPasteWindow != NULL) {
        mPasteWindow->Hide();
    }
}

void CWebViewClassic::SyncSelectionCursors()
{
    mSelectCursorBaseLayerId =
            NativeGetHandleLayerId(mNativeClass, HANDLE_ID_BASE,
                    mSelectCursorBase, mSelectCursorBaseTextQuad);
    mSelectCursorExtentLayerId =
            NativeGetHandleLayerId(mNativeClass, HANDLE_ID_EXTENT,
                    mSelectCursorExtent, mSelectCursorExtentTextQuad);
}

Boolean CWebViewClassic::SetupWebkitSelect()
{
    SyncSelectionCursors();
    if (!mIsCaretSelection && !StartSelectActionMode()) {
        SelectionDone();
        return FALSE;
    }
    StartSelectingText();
    mTouchMode = TOUCH_DRAG_MODE;
    return TRUE;
}

void CWebViewClassic::UpdateWebkitSelection(
    /* [in] */ Boolean isSnapped)
{
    Int32 handleId = (mSelectDraggingCursor == mSelectCursorBase)
            ? HANDLE_ID_BASE : HANDLE_ID_EXTENT;
    Int32 x, y;
    mSelectDraggingCursor->GetX(&x);
    mSelectDraggingCursor->GetY(&y);
    if (isSnapped) {
        // "center" the cursor in the snapping quad
        AutoPtr<IPoint> top = CalculateDraggingCaretTop();
        Int32 topX, topY;
        top->GetX(&topX);
        top->GetY(&topY);
        x = Elastos::Core::Math::Round((Float)(topX + x) / 2);
        y = Elastos::Core::Math::Round((Float)(topY + y) / 2);
    }
    mWebViewCore->RemoveMessages(CWebViewCore::EventHub::SELECT_TEXT);
    AutoPtr<IInteger32> handleIdObj;
    CInteger32::New(handleId, (IInteger32**)&handleIdObj);
    mWebViewCore->SendMessageAtFrontOfQueue(CWebViewCore::EventHub::SELECT_TEXT,
            x, y, handleIdObj.Get());
}

void CWebViewClassic::ResetCaretTimer()
{
    mPrivateHandler->RemoveMessages(CLEAR_CARET_HANDLE);
    if (!mSelectionStarted) {
        Boolean result;
        mPrivateHandler->SendEmptyMessageDelayed(CLEAR_CARET_HANDLE,
                CARET_HANDLE_STAMINA_MS, &result);
    }
}

/**
 * Select all of the text in this WebView.
 *
 * This is an implementation detail.
 */
ECode CWebViewClassic::SelectAll()
{
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SELECT_ALL);
    return NOERROR;
}

/**
 * Called when the selection has been removed.
 */
void CWebViewClassic::SelectionDone()
{
    if (mSelectingText) {
        HidePasteButton();
        EndSelectingText();
        // finish is idempotent, so this is fine even if selectionDone was
        // called by mSelectCallback.onDestroyActionMode
        if (mSelectCallback != NULL) {
            mSelectCallback->Finish();
            mSelectCallback = NULL;
        }
        Invalidate(); // redraw without selection
        mAutoScrollX = 0;
        mAutoScrollY = 0;
        mSentAutoScrollMessage = FALSE;
    }
}

/**
 * Copy the selection to the clipboard
 *
 * This is an implementation detail.
 */
ECode CWebViewClassic::CopySelection(
    /* [out] */ Boolean* copiedSomething)
{
    VALIDATE_NOT_NULL(copiedSomething);
    *copiedSomething = FALSE;
    String selection = GetSelection();
    if (!selection.IsNullOrEmpty()) {
        if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "copySelection \"%s\"", selection.string());
        }
        AutoPtr<IToastHelper> helper;
        CToastHelper::AcquireSingleton((IToastHelper**)&helper);
        AutoPtr<IToast> toast;
        helper->MakeText(mContext, R::string::text_copied, IToast::LENGTH_SHORT, (IToast**)&toast);
        toast->Show();
        *copiedSomething = TRUE;
        AutoPtr<IClipboardManager> cm;
        mContext->GetSystemService(IContext::CLIPBOARD_SERVICE,
                (IInterface**)&cm);
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(selection, (ICharSequence**)&cs);
        cm->SetText(cs);
        AutoPtr< ArrayOf<Int32> > handles = ArrayOf<Int32>::Alloc(4);
        GetSelectionHandles(handles);
        AutoPtr<IArrayOf> handlesObj;
        CArrayOf::New(EIID_IInteger32, 4, (IArrayOf**)&handlesObj);
        for (Int32 i = 0; i < 4; i++) {
            AutoPtr<IInteger32> value;
            CInteger32::New((*handles)[i], (IInteger32**)&value);
            handlesObj->Put(i, value.Get());
        }
        mWebViewCore->SendMessage(CWebViewCore::EventHub::COPY_TEXT, handlesObj);
    }
    Invalidate(); // remove selection region and pointer
    return NOERROR;
}

/**
 * Cut the selected text into the clipboard
 *
 * This is an implementation detail
 */
ECode CWebViewClassic::CutSelection()
{
    Boolean bResult;
    CopySelection(&bResult);
    AutoPtr< ArrayOf<Int32> > handles = ArrayOf<Int32>::Alloc(4);
    GetSelectionHandles(handles);
    AutoPtr<IArrayOf> handlesObj;
    CArrayOf::New(EIID_IInteger32, 4, (IArrayOf**)&handlesObj);
    for (Int32 i = 0; i < 4; i++) {
        AutoPtr<IInteger32> value;
        CInteger32::New((*handles)[i], (IInteger32**)&value);
        handlesObj->Put(i, value.Get());
    }
    mWebViewCore->SendMessage(CWebViewCore::EventHub::DELETE_TEXT, handlesObj);
    return NOERROR;
}

/**
 * Paste text from the clipboard to the cursor position.
 *
 * This is an implementation detail
 */
ECode CWebViewClassic::PasteFromClipboard()
{
    AutoPtr<IClipboardManager> cm;
    mContext->GetSystemService(IContext::CLIPBOARD_SERVICE,
            (IInterface**)&cm);
    AutoPtr<IClipData> clipData;
    cm->GetPrimaryClip((IClipData**)&clipData);
    if (clipData != NULL) {
        AutoPtr<IClipDataItem> clipItem;
        clipData->GetItemAt(0, (IClipDataItem**)&clipItem);
        AutoPtr<ICharSequence> pasteText;
        clipItem->GetText((ICharSequence**)&pasteText);
        if (mInputConnection != NULL) {
            mInputConnection->ReplaceSelection(pasteText);
        }
    }
    return NOERROR;
}

/**
 * Returns the currently highlighted text as a string.
 */
String CWebViewClassic::GetSelection()
{
    if (mNativeClass == 0) return String("");
    return NativeGetSelection();
}

ECode CWebViewClassic::OnAttachedToWindow()
{
    Boolean hasFocus = FALSE;
    if (mWebView->HasWindowFocus(&hasFocus), hasFocus) SetActive(TRUE);

    if (IsAccessibilityInjectionEnabled()) {
        GetAccessibilityInjector()->ToggleAccessibilityFeedback(TRUE);
    }

    UpdateHwAccelerated();
    return NOERROR;
}

ECode CWebViewClassic::OnDetachedFromWindow()
{
    ClearHelpers();
    mZoomManager->DismissZoomPicker();

    Boolean hasFocus = FALSE;
    if (mWebView->HasWindowFocus(&hasFocus), hasFocus) SetActive(FALSE);

    if (IsAccessibilityInjectionEnabled()) {
        GetAccessibilityInjector()->ToggleAccessibilityFeedback(FALSE);
    }

    UpdateHwAccelerated();

    EnsureFunctorDetached();
    return NOERROR;
}

ECode CWebViewClassic::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    // The zoomManager may be null if the webview is created from XML that
    // specifies the view's visibility param as not visible (see http://b/2794841)
    if (visibility != IView::VISIBLE && mZoomManager != NULL) {
        mZoomManager->DismissZoomPicker();
    }
    UpdateDrawingState();
    return NOERROR;
}

void CWebViewClassic::SetActive(
    /* [in] */ Boolean active)
{
    if (active) {
        Boolean hasFocus = FALSE;
        if (mWebView->HasFocus(&hasFocus), hasFocus) {
            // If our window regained focus, and we have focus, then begin
            // drawing the cursor ring
            mDrawCursorRing = TRUE;
            SetFocusControllerActive(TRUE);
        }
        else {
            mDrawCursorRing = FALSE;
            SetFocusControllerActive(FALSE);
        }
    }
    else {
        if (!mZoomManager->IsZoomPickerVisible()) {
            /*
             * The external zoom controls come in their own window, so our
             * window loses focus. Our policy is to not draw the cursor ring
             * if our window is not focused, but this is an exception since
             * the user can still navigate the web page with the zoom
             * controls showing.
             */
            mDrawCursorRing = FALSE;
        }
        mKeysPressed->Clear();
        mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
        mTouchMode = TOUCH_DONE_MODE;
        SetFocusControllerActive(FALSE);
    }
    Invalidate();
}

// To avoid drawing the cursor ring, and remove the TextView when our window
// loses focus.
ECode CWebViewClassic::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    SetActive(hasWindowFocus);
    if (hasWindowFocus) {
        WebCoreElastosBridge::SetActiveWebView(this);
        if (mPictureUpdatePausedForFocusChange) {
            CWebViewCore::ResumeUpdatePicture(mWebViewCore);
            mPictureUpdatePausedForFocusChange = FALSE;
        }
    }
    else {
        WebCoreElastosBridge::RemoveActiveWebView(this);
        AutoPtr<IWebSettings> settings;
        GetSettings((IWebSettings**)&settings);
        Boolean enabled = FALSE;
        if (settings != NULL && (settings->EnableSmoothTransition(&enabled), enabled) &&
                mWebViewCore != NULL && !CWebViewCore::IsUpdatePicturePaused(mWebViewCore)) {
            CWebViewCore::PauseUpdatePicture(mWebViewCore);
            mPictureUpdatePausedForFocusChange = TRUE;
        }
    }
    return NOERROR;
}

/*
 * Pass a message to WebCore Thread, telling the WebCore::Page's
 * FocusController to be  "inactive" so that it will
 * not draw the blinking cursor.  It gets set to "active" to draw the cursor
 * in WebViewCore.cpp, when the WebCore thread receives key events/clicks.
 */
void CWebViewClassic::SetFocusControllerActive(
    /* [in] */ Boolean active)
{
    if (mWebViewCore == NULL) return;
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_ACTIVE, active ? 1 : 0, 0);
    // Need to send this message after the document regains focus.
    if (active && mListBoxMessage != NULL) {
        mWebViewCore->SendMessage(mListBoxMessage);
        mListBoxMessage = NULL;
    }
}

ECode CWebViewClassic::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "MT focusChanged %d, %d", focused, direction);
    }
    if (focused) {
        mDrawCursorRing = TRUE;
        SetFocusControllerActive(TRUE);
    }
    else {
        mDrawCursorRing = FALSE;
        SetFocusControllerActive(FALSE);
        mKeysPressed->Clear();
    }
    Boolean isEmpty = FALSE;
    if (mTouchHighlightRegion->IsEmpty(&isEmpty), !isEmpty) {
        AutoPtr<IRect> bounds;
        mTouchHighlightRegion->GetBounds((IRect**)&bounds);
        mWebView->Invalidate(bounds);
    }
    return NOERROR;
}

void CWebViewClassic::UpdateRectsForGL()
{
    // Use the getGlobalVisibleRect() to get the intersection among the parents
    // visible == false means we're clipped - send a null rect down to indicate that
    // we should not draw
    Boolean visible = FALSE;
    mWebView->GetGlobalVisibleRect(mTempVisibleRect, mTempVisibleRectOffset, &visible);
    mInvScreenRect->Set(mTempVisibleRect);
    if (visible) {
        // Then need to invert the Y axis, just for GL
        AutoPtr<IView> rootView;
        mWebView->GetRootView((IView**)&rootView);
        Int32 rootViewHeight;
        rootView->GetHeight(&rootViewHeight);
        mScreenRect->Set(mInvScreenRect);
        Int32 savedWebViewBottom, top;
        mInvScreenRect->GetBottom(&savedWebViewBottom);
        mInvScreenRect->GetTop(&top);
        mInvScreenRect->SetBottom(rootViewHeight - top - GetVisibleTitleHeightImpl());
        mInvScreenRect->SetTop(rootViewHeight - savedWebViewBottom);
        mIsWebViewVisible = TRUE;
    }
    else {
        mIsWebViewVisible = FALSE;
    }

    Int32 x, y;
    mTempVisibleRectOffset->GetX(&x);
    mTempVisibleRectOffset->GetY(&y);
    mTempVisibleRect->Offset(-x, -y);
    ViewToContentVisibleRect(mVisibleContentRect, mTempVisibleRect);

    Float scale;
    GetScale(&scale);
    NativeUpdateDrawGLFunction(mNativeClass, mIsWebViewVisible ? mInvScreenRect : NULL,
            mIsWebViewVisible ? mScreenRect : NULL,
            mVisibleContentRect, scale);
}

// Input : viewRect, rect in view/screen coordinate.
// Output: contentRect, rect in content/document coordinate.
void CWebViewClassic::ViewToContentVisibleRect(
    /* [in] */ IRectF* contentRect,
    /* [in] */ IRect* viewRect)
{
    Int32 left, right, top, bottom;
    Float scaleX, scaleY;

    viewRect->GetLeft(&left);
    mWebView->GetScaleX(&scaleX);
    contentRect->SetLeft(ViewToContentXf(left) / scaleX);
    // viewToContentY will remove the total height of the title bar.  Add
    // the visible height back in to account for the fact that if the title
    // bar is partially visible, the part of the visible rect which is
    // displaying our content is displaced by that amount.
    viewRect->GetTop(&top);
    mWebView->GetScaleY(&scaleY);
    contentRect->SetTop(ViewToContentYf(top + GetVisibleTitleHeightImpl())
            / scaleY);
    viewRect->GetRight(&right);
    mWebView->GetScaleX(&scaleX);
    contentRect->SetRight(ViewToContentXf(right) / scaleX);
    viewRect->GetBottom(&bottom);
    mWebView->GetScaleY(&scaleY);
    contentRect->SetBottom(ViewToContentYf(bottom) / scaleY);
}

ECode CWebViewClassic::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean changed = FALSE;
    mWebViewPrivate->Super_setFrame(left, top, right, bottom, &changed);
    if (!changed && mHeightCanMeasure) {
        // When mHeightCanMeasure is true, we will set mLastHeightSent to 0
        // in WebViewCore after we get the first layout. We do call
        // requestLayout() when we get contentSizeChanged(). But the View
        // system won't call onSizeChanged if the dimension is not changed.
        // In this case, we need to call sendViewSizeZoom() explicitly to
        // notify the WebKit about the new dimensions.
        SendViewSizeZoom(FALSE);
    }
    UpdateRectsForGL();
    *result = changed;
    return NOERROR;
}

ECode CWebViewClassic::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    // adjust the max viewport width depending on the view dimensions. This
    // is to ensure the scaling is not going insane. So do not shrink it if
    // the view size is temporarily smaller, e.g. when soft keyboard is up.
    Int32 newMaxViewportWidth = (Int32) (Elastos::Core::Math::Max(w, h) / mZoomManager->GetDefaultMinZoomScale());
    if (newMaxViewportWidth > sMaxViewportWidth) {
        sMaxViewportWidth = newMaxViewportWidth;
    }

    mZoomManager->OnSizeChanged(w, h, ow, oh);

    if (mLoadedPicture != NULL && mDelaySetPicture == NULL) {
        // Size changes normally result in a new picture
        // Re-set the loaded picture to simulate that
        // However, do not update the base layer as that hasn't changed
        SetNewPicture(mLoadedPicture, FALSE);
    }
    if (mIsEditingText) {
        ScrollEditIntoView();
    }
    RelocateAutoCompletePopup();
    return NOERROR;
}

/**
 * Scrolls the edit field into view using the minimum scrolling necessary.
 * If the edit field is too large to fit in the visible window, the caret
 * dimensions are used so that at least the caret is visible.
 * A buffer of EDIT_RECT_BUFFER in view pixels is used to offset the
 * edit rectangle to ensure a margin with the edge of the screen.
 */
void CWebViewClassic::ScrollEditIntoView()
{
    AutoPtr<IRect> visibleRect;
    CRect::New(ViewToContentX(GetScrollX()),
            ViewToContentY(GetScrollY()),
            ViewToContentX(GetScrollX() + GetWidth()),
            ViewToContentY(GetScrollY() + GetViewHeightWithTitle()),
            (IRect**)&visibleRect);
    Boolean contains = FALSE;
    if (visibleRect->Contains(mEditTextContentBounds, &contains), contains) {
        return; // no need to scroll
    }
    SyncSelectionCursors();
    NativeFindMaxVisibleRect(mNativeClass, mEditTextLayerId, visibleRect);
    Int32 buffer = Elastos::Core::Math::Max(1, ViewToContentDimension(EDIT_RECT_BUFFER));
    Int32 etbLeft, etbRight, etbTop, etbBottom;
    mEditTextContentBounds->Get(&etbLeft, &etbTop, &etbRight, &etbBottom);
    AutoPtr<IRect> showRect;
    CRect::New(Elastos::Core::Math::Max(0, etbLeft - buffer),
            Elastos::Core::Math::Max(0, etbTop - buffer),
            etbRight + buffer, etbBottom + buffer, (IRect**)&showRect);
    AutoPtr<IPoint> caretTop = CalculateBaseCaretTop();
    Int32 vRectWidth, etbWidth;
    visibleRect->GetWidth(&vRectWidth);
    mEditTextContentBounds->GetWidth(&etbWidth);
    Int32 scbX, scbY, caretTopX, caretTopY;
    Int32 showRectLeft, showRectRight, showRectTop, showRectBottom;
    if (vRectWidth < etbWidth) {
        // The whole edit won't fit in the width, so use the caret rect
        mSelectCursorBase->GetX(&scbX);
        caretTop->GetX(&caretTopX);
        if (scbX < caretTopX) {
            showRect->SetLeft(Elastos::Core::Math::Max(0, scbX - buffer));
            showRect->SetRight(caretTopX + buffer);
        }
        else {
            showRect->SetLeft(Elastos::Core::Math::Max(0, caretTopX - buffer));
            showRect->SetRight(scbX + buffer);
        }
    }
    Int32 visibleRectHeight, etbHeight;
    visibleRect->GetHeight(&visibleRectHeight);
    mEditTextContentBounds->GetHeight(&etbHeight);
    if (visibleRectHeight < etbHeight) {
        // The whole edit won't fit in the height, so use the caret rect
        mSelectCursorBase->GetY(&scbY);
        caretTop->GetY(&caretTopY);
        if (scbY > caretTopY) {
            showRect->SetTop(Elastos::Core::Math::Max(0, caretTopY - buffer));
            showRect->SetBottom(scbY + buffer);
        }
        else {
            showRect->SetTop(Elastos::Core::Math::Max(0, scbY - buffer));
            showRect->SetBottom(caretTopY + buffer);
        }
    }

    if (visibleRect->Contains(showRect, &contains), contains) {
        return; // no need to scroll
    }

    Int32 scrollX = ViewToContentX(GetScrollX());
    Int32 visibleRectLeft, visibleRectRight;
    visibleRect->GetLeft(&visibleRectLeft);
    visibleRect->GetRight(&visibleRectRight);
    showRect->GetLeft(&showRectLeft);
    showRect->GetRight(&showRectRight);
    if (visibleRectLeft > showRectLeft) {
        // We are scrolled too far
        scrollX += showRectLeft - visibleRectLeft;
    }
    else if (visibleRectRight < showRectRight) {
        // We aren't scrolled enough to include the right
        scrollX += showRectRight - visibleRectRight;
    }
    Int32 scrollY = ViewToContentY(GetScrollY());
    Int32 visibleRectTop, visibleRectBottom;
    visibleRect->GetTop(&visibleRectTop);
    visibleRect->GetBottom(&visibleRectBottom);
    showRect->GetTop(&showRectTop);
    showRect->GetBottom(&showRectBottom);
    if (visibleRectTop > showRectTop) {
        scrollY += showRectTop - visibleRectTop;
    }
    else if (visibleRectBottom < showRectBottom) {
        scrollY += showRectBottom - visibleRectBottom;
    }

    ContentScrollTo(scrollX, scrollY, FALSE);
}

ECode CWebViewClassic::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    if (!mInOverScrollMode) {
        SendOurVisibleRect();
        // update WebKit if visible title bar height changed. The logic is same
        // as getVisibleTitleHeightImpl.
        Int32 titleHeight = GetTitleHeight();
        if (Elastos::Core::Math::Max(titleHeight - t, 0) != Elastos::Core::Math::Max(titleHeight - oldt, 0)) {
            SendViewSizeZoom(FALSE);
        }
    }
    return NOERROR;
}

ECode CWebViewClassic::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 action;
    event->GetAction(&action);
    if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "CWebViewClassic::DispatchKeyEvent, action: 0x%x", action);
    }
    switch (action) {
        case IKeyEvent::ACTION_DOWN: {
            Int32 keyCode;
            event->GetKeyCode(&keyCode);
            AutoPtr<IInteger32> keyCodeObj;
            CInteger32::New(keyCode, (IInteger32**)&keyCodeObj);
            mKeysPressed->PushBack(keyCodeObj);
            break;
        }
        case IKeyEvent::ACTION_MULTIPLE:
            // Always accept the action.
            break;

        case IKeyEvent::ACTION_UP: {
            Int32 keyCode;
            event->GetKeyCode(&keyCode);
            AutoPtr<IInteger32> keyCodeObj;
            CInteger32::New(keyCode, (IInteger32**)&keyCodeObj);
            Vector< AutoPtr<IInteger32> >::Iterator it =
                    Find(mKeysPressed->Begin(), mKeysPressed->End(), keyCodeObj);
            if (it == mKeysPressed->End()) {
                // We did not receive the key down for this key, so do not
                // handle the key up.
                *result = FALSE;
                return NOERROR;
            }
            else {
                // We did receive the key down.  Handle the key up, and
                // remove it from our pressed keys.
                mKeysPressed->Erase(it);
            }
            break;
        }
        default:
            // Accept the action.  This should not happen, unless a new
            // action is added to KeyEvent.
            break;
    }
    return mWebViewPrivate->Super_dispatchKeyEvent(event, result);
}

Boolean CWebViewClassic::InFullScreenMode()
{
    return mFullScreenHolder != NULL;
}

void CWebViewClassic::DismissFullScreenMode()
{
    if (InFullScreenMode()) {
        mFullScreenHolder->Hide();
        mFullScreenHolder = NULL;
        Invalidate();
    }
}

void CWebViewClassic::OnPinchToZoomAnimationStart()
{
    // cancel the single touch handling
    CancelTouch();
    OnZoomAnimationStart();
}

void CWebViewClassic::OnPinchToZoomAnimationEnd(
    /* [in] */ IScaleGestureDetector* detector)
{
    OnZoomAnimationEnd();
    // start a drag, TOUCH_PINCH_DRAG, can't use TOUCH_INIT_MODE as
    // it may trigger the unwanted click, can't use TOUCH_DRAG_MODE
    // as it may trigger the unwanted fling.
    mTouchMode = TOUCH_PINCH_DRAG;
    mConfirmMove = TRUE;
    Float x, y;
    detector->GetFocusX(&x);
    detector->GetFocusY(&y);
    StartTouch(x, y, mLastTouchTime);
}

// See if there is a layer at x, y and switch to TOUCH_DRAG_LAYER_MODE if a
// layer is found.
void CWebViewClassic::StartScrollingLayer(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (mNativeClass == 0) {
        return;
    }

    Int32 contentX = ViewToContentX((Int32) x + GetScrollX());
    Int32 contentY = ViewToContentY((Int32) y + GetScrollY());
    mCurrentScrollingLayerId = NativeScrollableLayer(mNativeClass,
            contentX, contentY, mScrollingLayerRect, mScrollingLayerBounds);
    if (mCurrentScrollingLayerId != 0) {
        mTouchMode = TOUCH_DRAG_LAYER_MODE;
    }
}

ECode CWebViewClassic::OnHoverEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }
    Float X, Y;
    event->GetX(&X);
    event->GetY(&Y);
    Int32 x = ViewToContentX((Int32) X + GetScrollX());
    Int32 y = ViewToContentY((Int32) Y + GetScrollY());
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_MOVE_MOUSE, x, y);
    mWebViewPrivate->Super_onHoverEvent(event, result);
    *result = TRUE;
    return NOERROR;
}

ECode CWebViewClassic::OnTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "CWebViewClassic::OnTouchEvent, mNativeClass:%d", mNativeClass);
    }
    VALIDATE_NOT_NULL(result);
    Boolean isClickable, isLongClickable;
    if (mNativeClass == 0 ||
        ((mWebView->IsClickable(&isClickable), !isClickable) &&
            (mWebView->IsLongClickable(&isLongClickable), !isLongClickable))) {
        *result = FALSE;
        return NOERROR;
    }

    if (mInputDispatcher == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    Boolean isFocusable, isFocusableInTouchMode, isFocused;
    if ((mWebView->IsFocusable(&isFocusable), isFocusable) &&
        (mWebView->IsFocusableInTouchMode(&isFocusableInTouchMode), isFocusableInTouchMode) &&
        (mWebView->IsFocused(&isFocused), !isFocused)) {
        Boolean result;
        mWebView->RequestFocus(&result);
    }

    if (mInputDispatcher->PostPointerEvent(ev, GetScrollX(),
            GetScrollY() - GetTitleHeight(), mZoomManager->GetInvScale())) {
        mInputDispatcher->DispatchUiEvents();
        *result = TRUE;
        return NOERROR;
    }
    else {
        Logger::W(LOGTAG, "mInputDispatcher rejected the event!");
        *result = FALSE;
        return NOERROR;
    }
}

Float CWebViewClassic::CalculateDragAngle(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    dx = Elastos::Core::Math::Abs(dx);
    dy = Elastos::Core::Math::Abs(dy);
    return (Float) Elastos::Core::Math::Atan2(dy, dx);
}

/*
* Common code for single touch and multi-touch.
* (x, y) denotes current focus point, which is the touch point for single touch
* and the middle point for multi-touch.
*/
void CWebViewClassic::HandleTouchEventCommon(
    /* [in] */ IMotionEvent* event,
    /* [in] */ Int32 action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    AutoPtr<IScaleGestureDetector> detector = mZoomManager->GetScaleGestureDetector();

    Int64 eventTime;
    event->GetEventTime(&eventTime);

    // Due to the touch screen edge effect, a touch closer to the edge
    // always snapped to the edge. As getViewWidth() can be different from
    // getWidth() due to the scrollbar, adjusting the point to match
    // getViewWidth(). Same applied to the height.
    x = Elastos::Core::Math::Min(x, GetViewWidth() - 1);
    y = Elastos::Core::Math::Min(y, GetViewHeightWithTitle() - 1);

    Int32 deltaX = mLastTouchX - x;
    Int32 deltaY = mLastTouchY - y;
    Int32 contentX = ViewToContentX(x + GetScrollX());
    Int32 contentY = ViewToContentY(y + GetScrollY());

    switch (action) {
        case IMotionEvent::ACTION_DOWN: {
            mConfirmMove = FALSE;
            Boolean isFinished = FALSE, result;
            if (mEditTextScroller->IsFinished(&isFinished), !isFinished) {
                mEditTextScroller->AbortAnimation();
            }
            if (!mScroller->IsFinished()) {
                // stop the current scroll animation, but if this is
                // the start of a fling, allow it to add to the current
                // fling's velocity
                mScroller->AbortAnimation();
                mTouchMode = TOUCH_DRAG_START_MODE;
                mConfirmMove = TRUE;
                NativeSetIsScrolling(FALSE);
            }
            else if (mPrivateHandler->HasMessages(RELEASE_SINGLE_TAP, &result), result) {
                mPrivateHandler->RemoveMessages(RELEASE_SINGLE_TAP);
                RemoveTouchHighlight();
                if (deltaX * deltaX + deltaY * deltaY < mDoubleTapSlopSquare) {
                    mTouchMode = TOUCH_DOUBLE_TAP_MODE;
                }
                else {
                    mTouchMode = TOUCH_INIT_MODE;
                }
            }
            else { // the normal case
                mTouchMode = TOUCH_INIT_MODE;
                if (mLogEvent && eventTime - mLastTouchUpTime < 1000) {
//                    EventLog.writeEvent(EventLogTags.BROWSER_DOUBLE_TAP_DURATION,
//                            (eventTime - mLastTouchUpTime), eventTime);
                }
                mSelectionStarted = FALSE;
                if (mSelectingText) {
                    EnsureSelectionHandles();
                    Int32 shiftedY = y - GetTitleHeight() + GetScrollY();
                    Int32 shiftedX = x + GetScrollX();
                    Boolean contains = FALSE;
                    if (mSelectHandleBaseBounds->Contains(shiftedX, shiftedY, &contains), contains) {
                        mSelectionStarted = TRUE;
                        mSelectDraggingCursor = mSelectCursorBase;
                        mSelectDraggingTextQuad = mSelectCursorBaseTextQuad;
                        if (mIsCaretSelection) {
                            mPrivateHandler->RemoveMessages(CLEAR_CARET_HANDLE);
                            HidePasteButton();
                        }
                    }
                    else if (mSelectHandleExtentBounds->Contains(shiftedX, shiftedY, &contains), contains) {
                        mSelectionStarted = TRUE;
                        mSelectDraggingCursor = mSelectCursorExtent;
                        mSelectDraggingTextQuad = mSelectCursorExtentTextQuad;
                    }
                    else if (mIsCaretSelection) {
                        SelectionDone();
                    }
                    if (DebugFlags::WEB_VIEW) {
                        Logger::V(LOGTAG, "select=%d,%d", contentX, contentY);
                    }
                }
            }
            // Trigger the link
            if (!mSelectingText && (mTouchMode == TOUCH_INIT_MODE
                    || mTouchMode == TOUCH_DOUBLE_TAP_MODE)) {
                Boolean result;
                mPrivateHandler->SendEmptyMessageDelayed(
                        SWITCH_TO_SHORTPRESS, TAP_TIMEOUT, &result);
                mPrivateHandler->SendEmptyMessageDelayed(
                        SWITCH_TO_LONGPRESS, LONG_PRESS_TIMEOUT, &result);
            }
            StartTouch(x, y, eventTime);
            if (mIsEditingText) {
                mEditTextContentBounds->Contains(contentX, contentY, &mTouchInEditText);
            }
            break;
        }
        case IMotionEvent::ACTION_MOVE: {
            if (!mConfirmMove && (deltaX * deltaX + deltaY * deltaY)
                    >= mTouchSlopSquare) {
                mPrivateHandler->RemoveMessages(SWITCH_TO_SHORTPRESS);
                mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
                mConfirmMove = TRUE;
                if (mTouchMode == TOUCH_DOUBLE_TAP_MODE) {
                    mTouchMode = TOUCH_INIT_MODE;
                }
                RemoveTouchHighlight();
            }
            if (mSelectingText && mSelectionStarted) {
                if (DebugFlags::WEB_VIEW) {
                    Logger::V(LOGTAG, "extend=%d,%d", contentX, contentY);
                }
                AutoPtr<IViewParent> parent;
                mWebView->GetParent((IViewParent**)&parent);
                if (parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
                if (deltaX != 0 || deltaY != 0) {
                    Int32 selectOffsetX, selectOffsetY;
                    mSelectOffset->GetX(&selectOffsetX);
                    mSelectOffset->GetY(&selectOffsetY);
                    Int32 handleX = contentX +
                            ViewToContentDimension(selectOffsetX);
                    Int32 handleY = contentY +
                            ViewToContentDimension(selectOffsetY);
                    mSelectDraggingCursor->Set(handleX, handleY);
                    Boolean inCursorText = mSelectDraggingTextQuad->ContainsPoint(handleX, handleY);
                    Boolean inEditBounds = FALSE;
                    mEditTextContentBounds->Contains(handleX, handleY, &inEditBounds);
                    if (mIsEditingText && !inEditBounds) {
                        BeginScrollEdit();
                    }
                    else {
                        EndScrollEdit();
                    }
                    Boolean snapped = FALSE;
                    if (inCursorText || (mIsEditingText && !inEditBounds)) {
                        SnapDraggingCursor();
                        snapped = TRUE;
                    }
                    UpdateWebkitSelection(snapped);
                    if (!inCursorText && mIsEditingText && inEditBounds) {
                        // Visually snap even if we have moved the handle.
                        SnapDraggingCursor();
                    }
                    mLastTouchX = x;
                    mLastTouchY = y;
                    Invalidate();
                }
                break;
            }

            if (mTouchMode == TOUCH_DONE_MODE) {
                // no dragging during scroll zoom animation, or when prevent
                // default is yes
                break;
            }

            if (mVelocityTracker == NULL) {
                Logger::E(LOGTAG, "Got null mVelocityTracker when mTouchMode = %d", mTouchMode);
            }
            else {
                mVelocityTracker->AddMovement(event);
            }

            if (mTouchMode != TOUCH_DRAG_MODE &&
                    mTouchMode != TOUCH_DRAG_LAYER_MODE &&
                    mTouchMode != TOUCH_DRAG_TEXT_MODE) {

                if (!mConfirmMove) {
                    break;
                }

                // Only lock dragging to one axis if we don't have a scale in progress.
                // Scaling implies free-roaming movement. Note this is only ever a question
                // if mZoomManager.supportsPanDuringZoom() is true.
                mAverageAngle = CalculateDragAngle(deltaX, deltaY);
                Boolean inProgress = FALSE;
                if (detector == NULL || !(detector->IsInProgress(&inProgress), inProgress)) {
                    // if it starts nearly horizontal or vertical, enforce it
                    if (mAverageAngle < HSLOPE_TO_START_SNAP) {
                        mSnapScrollMode = SNAP_X;
                        mSnapPositive = deltaX > 0;
                        mAverageAngle = ANGLE_HORIZ;
                    }
                    else if (mAverageAngle > VSLOPE_TO_START_SNAP) {
                        mSnapScrollMode = SNAP_Y;
                        mSnapPositive = deltaY > 0;
                        mAverageAngle = ANGLE_VERT;
                    }
                }

                mTouchMode = TOUCH_DRAG_MODE;
                mLastTouchX = x;
                mLastTouchY = y;
                deltaX = 0;
                deltaY = 0;

                StartScrollingLayer(x, y);
                StartDrag();
            }

            // do pan
            Boolean keepScrollBarsVisible = FALSE;
            if (deltaX == 0 && deltaY == 0) {
                keepScrollBarsVisible = TRUE;
            }
            else {
                mAverageAngle +=
                    (CalculateDragAngle(deltaX, deltaY) - mAverageAngle)
                    / MMA_WEIGHT_N;
                if (mSnapScrollMode != SNAP_NONE) {
                    if (mSnapScrollMode == SNAP_Y) {
                        // radical change means getting out of snap mode
                        if (mAverageAngle < VSLOPE_TO_BREAK_SNAP) {
                            mSnapScrollMode = SNAP_NONE;
                        }
                    }

                    if (mSnapScrollMode == SNAP_X) {
                        // radical change means getting out of snap mode
                        if (mAverageAngle > HSLOPE_TO_BREAK_SNAP) {
                            mSnapScrollMode = SNAP_NONE;
                        }
                    }
                }
                else {
                    if (mAverageAngle < HSLOPE_TO_START_SNAP) {
                        mSnapScrollMode = SNAP_X;
                        mSnapPositive = deltaX > 0;
                        mAverageAngle = (mAverageAngle + ANGLE_HORIZ) / 2;
                    }
                    else if (mAverageAngle > VSLOPE_TO_START_SNAP) {
                        mSnapScrollMode = SNAP_Y;
                        mSnapPositive = deltaY > 0;
                        mAverageAngle = (mAverageAngle + ANGLE_VERT) / 2;
                    }
                }
                if (mSnapScrollMode != SNAP_NONE) {
                    if ((mSnapScrollMode & SNAP_X) == SNAP_X) {
                        deltaY = 0;
                    }
                    else {
                        deltaX = 0;
                    }
                }
                if (deltaX * deltaX + deltaY * deltaY > mTouchSlopSquare) {
                    mHeldMotionless = MOTIONLESS_FALSE;
                }
                else {
                    mHeldMotionless = MOTIONLESS_TRUE;
                    keepScrollBarsVisible = TRUE;
                }

                mLastTouchTime = eventTime;
                Boolean allDrag = DoDrag(deltaX, deltaY);
                if (allDrag) {
                    mLastTouchX = x;
                    mLastTouchY = y;
                }
                else {
                    Int32 contentDeltaX = (Int32)Elastos::Core::Math::Floor(deltaX * mZoomManager->GetInvScale());
                    Int32 roundedDeltaX = ContentToViewDimension(contentDeltaX);
                    Int32 contentDeltaY = (Int32)Elastos::Core::Math::Floor(deltaY * mZoomManager->GetInvScale());
                    Int32 roundedDeltaY = ContentToViewDimension(contentDeltaY);
                    mLastTouchX -= roundedDeltaX;
                    mLastTouchY -= roundedDeltaY;
                }
            }

            break;
        }
        case IMotionEvent::ACTION_UP: {
            if (mIsEditingText && mSelectionStarted) {
                EndScrollEdit();
                Boolean result;
                mPrivateHandler->SendEmptyMessageDelayed(SCROLL_HANDLE_INTO_VIEW,
                        TEXT_SCROLL_FIRST_SCROLL_MS, &result);
                if (!mConfirmMove && mIsCaretSelection) {
                    ShowPasteWindow();
                    StopTouch();
                    break;
                }
            }
            mLastTouchUpTime = eventTime;
            if (mSentAutoScrollMessage) {
                mAutoScrollX = mAutoScrollY = 0;
            }
            switch (mTouchMode) {
                case TOUCH_DOUBLE_TAP_MODE: // double tap
                    mPrivateHandler->RemoveMessages(SWITCH_TO_SHORTPRESS);
                    mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
                    mTouchMode = TOUCH_DONE_MODE;
                    break;
                case TOUCH_INIT_MODE: // tap
                case TOUCH_SHORTPRESS_START_MODE:
                case TOUCH_SHORTPRESS_MODE:
                    mPrivateHandler->RemoveMessages(SWITCH_TO_SHORTPRESS);
                    mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
                    if (!mConfirmMove) {
                        if (mSelectingText) {
                            // tapping on selection or controls does nothing
                            if (!mSelectionStarted) {
                                SelectionDone();
                            }
                            break;
                        }
                        // only trigger double tap if the WebView is
                        // scalable
                        Boolean canZoomIn, canZoomOut;
                        if (mTouchMode == TOUCH_INIT_MODE
                                &&
                                ((CanZoomIn(&canZoomIn), canZoomIn)
                                ||
                                (CanZoomOut(&canZoomOut), canZoomOut)
                                )) {
                            Boolean result;
                            mPrivateHandler->SendEmptyMessageDelayed(
                                    RELEASE_SINGLE_TAP, CViewConfiguration::GetDoubleTapTimeout(), &result);
                        }
                        break;
                    }
                case TOUCH_DRAG_MODE:
                case TOUCH_DRAG_LAYER_MODE:
                case TOUCH_DRAG_TEXT_MODE:
                    mPrivateHandler->RemoveMessages(DRAG_HELD_MOTIONLESS);
                    // if the user waits a while w/o moving before the
                    // up, we don't want to do a fling
                    if (eventTime - mLastTouchTime <= MIN_FLING_TIME) {
                        if (mVelocityTracker == NULL) {
                            Logger::E(LOGTAG, "Got null mVelocityTracker");
                        }
                        else {
                            mVelocityTracker->AddMovement(event);
                        }
                        // set to MOTIONLESS_IGNORE so that it won't keep
                        // removing and sending message in
                        // drawCoreAndCursorRing()
                        mHeldMotionless = MOTIONLESS_IGNORE;
                        DoFling();
                        break;
                    }
                    else {
                        if (mScroller->SpringBack(GetScrollX(), GetScrollY(), 0,
                                ComputeMaxScrollX(), 0,
                                ComputeMaxScrollY())) {
                            Invalidate();
                        }
                    }
                    // redraw in high-quality, as we're done dragging
                    mHeldMotionless = MOTIONLESS_TRUE;
                    Invalidate();
                    // fall through
                case TOUCH_DRAG_START_MODE:
                    // TOUCH_DRAG_START_MODE should not happen for the real
                    // device as we almost certain will get a MOVE. But this
                    // is possible on emulator.
                    mLastVelocity = 0;
                    CWebViewCore::ResumePriority();
                    if (!mSelectingText) {
                        CWebViewCore::ResumeUpdatePicture(mWebViewCore);
                    }
                    break;
            }
            StopTouch();
            break;
        }
        case IMotionEvent::ACTION_CANCEL: {
            if (mTouchMode == TOUCH_DRAG_MODE) {
                mScroller->SpringBack(GetScrollX(), GetScrollY(), 0,
                        ComputeMaxScrollX(), 0, ComputeMaxScrollY());
                Invalidate();
            }
            CancelTouch();
            break;
        }
    }
}

/**
 * Returns the text scroll speed in content pixels per millisecond based on
 * the touch location.
 * @param coordinate The x or y touch coordinate in content space
 * @param min The minimum coordinate (x or y) of the edit content bounds
 * @param max The maximum coordinate (x or y) of the edit content bounds
 */
Float CWebViewClassic::GetTextScrollSpeed(
    /* [in] */ Int32 coordinate,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    if (coordinate < min) {
        return (coordinate - min) * TEXT_SCROLL_RATE;
    }
    else if (coordinate >= max) {
        return (coordinate - max + 1) * TEXT_SCROLL_RATE;
    }
    else {
        return 0.0f;
    }
}

Int32 CWebViewClassic::GetSelectionCoordinate(
    /* [in] */ Int32 coordinate,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    return Elastos::Core::Math::Max(Elastos::Core::Math::Min(coordinate, max), min);
}

void CWebViewClassic::BeginScrollEdit()
{
    if (mLastEditScroll == 0) {
        mLastEditScroll = SystemClock::GetUptimeMillis() -
                TEXT_SCROLL_FIRST_SCROLL_MS;
        ScrollEditWithCursor();
    }
}

void CWebViewClassic::ScrollDraggedSelectionHandleIntoView()
{
    if (mSelectDraggingCursor == NULL) {
        return;
    }
    Int32 x, y;
    mSelectDraggingCursor->GetX(&x);
    mSelectDraggingCursor->GetY(&y);
    Boolean contains = FALSE;
    if (mEditTextContentBounds->Contains(x,y, &contains), !contains) {
        Int32 etbRight, etbLeft, etbTop, etbBottom;
        mEditTextContentBounds->Get(&etbLeft, &etbTop, &etbRight, &etbBottom);
        Int32 left = Elastos::Core::Math::Min(0, x - etbLeft - EDIT_RECT_BUFFER);
        Int32 right = Elastos::Core::Math::Max(0, x - etbRight + EDIT_RECT_BUFFER);
        Int32 deltaX = left + right;
        Int32 above = Elastos::Core::Math::Min(0, y - etbTop - EDIT_RECT_BUFFER);
        Int32 below = Elastos::Core::Math::Max(0, y - etbBottom + EDIT_RECT_BUFFER);
        Int32 deltaY = above + below;
        if (deltaX != 0 || deltaY != 0) {
            Int32 scrollX = GetTextScrollX() + deltaX;
            Int32 scrollY = GetTextScrollY() + deltaY;
            scrollX = ClampBetween(scrollX, 0, GetMaxTextScrollX());
            scrollY = ClampBetween(scrollY, 0, GetMaxTextScrollY());
            ScrollEditText(scrollX, scrollY);
        }
    }
}

void CWebViewClassic::EndScrollEdit()
{
    mLastEditScroll = 0;
}

Int32 CWebViewClassic::ClampBetween(
    /* [in] */ Int32 value,
    /* [in] */ Int32 min,
    /* [in] */ Int32 max)
{
    return Elastos::Core::Math::Max(min, Elastos::Core::Math::Min(value, max));
}

Int32 CWebViewClassic::GetTextScrollDelta(
    /* [in] */ Float speed,
    /* [in] */ Int64 deltaT)
{
    Float distance = speed * deltaT;
    Int32 intDistance = (Int32)Elastos::Core::Math::Floor(distance);
    Float probability = distance - intDistance;
    if (Elastos::Core::Math::Random() < probability) {
        intDistance++;
    }
    return intDistance;
}

/**
 * Scrolls edit text a distance based on the last touch point,
 * the last scroll time, and the edit text content bounds.
 */
void CWebViewClassic::ScrollEditWithCursor()
{
    if (mLastEditScroll != 0) {
        Int32 selectOffsetX, selectOffsetY;
        mSelectOffset->GetX(&selectOffsetX);
        Int32 x = ViewToContentX(mLastTouchX + GetScrollX() + selectOffsetX);
        Int32 etbRight, etbLeft, etbTop, etbBottom;
        mEditTextContentBounds->Get(&etbLeft, &etbTop, &etbRight, &etbBottom);
        Float scrollSpeedX = GetTextScrollSpeed(x, etbLeft, etbRight);
        mSelectOffset->GetY(&selectOffsetY);
        Int32 y = ViewToContentY(mLastTouchY + GetScrollY() + selectOffsetY);
        Float scrollSpeedY = GetTextScrollSpeed(y, etbTop, etbBottom);
        if (scrollSpeedX == 0.0f && scrollSpeedY == 0.0f) {
            EndScrollEdit();
        }
        else {
            Int64 currentTime = SystemClock::GetUptimeMillis();
            Int64 timeSinceLastUpdate = currentTime - mLastEditScroll;
            Int32 deltaX = GetTextScrollDelta(scrollSpeedX, timeSinceLastUpdate);
            Int32 deltaY = GetTextScrollDelta(scrollSpeedY, timeSinceLastUpdate);
            Int32 scrollX = GetTextScrollX() + deltaX;
            scrollX = ClampBetween(scrollX, 0, GetMaxTextScrollX());
            Int32 scrollY = GetTextScrollY() + deltaY;
            scrollY = ClampBetween(scrollY, 0, GetMaxTextScrollY());

            mLastEditScroll = currentTime;
            if (scrollX == GetTextScrollX() && scrollY == GetTextScrollY()) {
                // By probability no text scroll this time. Try again later.
                Boolean result;
                mPrivateHandler->SendEmptyMessageDelayed(SCROLL_EDIT_TEXT,
                        TEXT_SCROLL_FIRST_SCROLL_MS, &result);
            }
            else {
                mEditTextContentBounds->Get(&etbLeft, &etbTop, &etbRight, &etbBottom);
                Int32 selectionX = GetSelectionCoordinate(x, etbLeft, etbRight);
                Int32 selectionY = GetSelectionCoordinate(y, etbTop, etbBottom);
                Int32 oldX, oldY;
                mSelectDraggingCursor->GetX(&oldX);
                mSelectDraggingCursor->GetY(&oldY);
                mSelectDraggingCursor->Set(selectionX, selectionY);
                UpdateWebkitSelection(FALSE);
                ScrollEditText(scrollX, scrollY);
                mSelectDraggingCursor->Set(oldX, oldY);
            }
        }
    }
}

void CWebViewClassic::StartTouch(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 eventTime)
{
    // Remember where the motion event started
    mStartTouchX = mLastTouchX = Elastos::Core::Math::Round(x);
    mStartTouchY = mLastTouchY = Elastos::Core::Math::Round(y);
    mLastTouchTime = eventTime;
    mVelocityTracker = VelocityTracker::Obtain();
    mSnapScrollMode = SNAP_NONE;
}

void CWebViewClassic::StartDrag()
{
    CWebViewCore::ReducePriority();
    // to get better performance, pause updating the picture
    CWebViewCore::PauseUpdatePicture(mWebViewCore);
    NativeSetIsScrolling(TRUE);

    if (mHorizontalScrollBarMode != SCROLLBAR_ALWAYSOFF
            || mVerticalScrollBarMode != SCROLLBAR_ALWAYSOFF) {
        mZoomManager->InvokeZoomPicker();
    }
}

Boolean CWebViewClassic::DoDrag(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY)
{
    Boolean allDrag = TRUE;
    if ((deltaX | deltaY) != 0) {
        Int32 oldX = GetScrollX();
        Int32 oldY = GetScrollY();
        Int32 rangeX = ComputeMaxScrollX();
        Int32 rangeY = ComputeMaxScrollY();
        Int32 contentX = (Int32)Elastos::Core::Math::Floor(deltaX * mZoomManager->GetInvScale());
        Int32 contentY = (Int32)Elastos::Core::Math::Floor(deltaY * mZoomManager->GetInvScale());

        // Assume page scrolling and change below if we're wrong
        mTouchMode = TOUCH_DRAG_MODE;

        // Check special scrolling before going to main page scrolling.
        if (mIsEditingText && mTouchInEditText && CanTextScroll(deltaX, deltaY)) {
            // Edit text scrolling
            oldX = GetTextScrollX();
            rangeX = GetMaxTextScrollX();
            deltaX = contentX;
            oldY = GetTextScrollY();
            rangeY = GetMaxTextScrollY();
            deltaY = contentY;
            mTouchMode = TOUCH_DRAG_TEXT_MODE;
            allDrag = FALSE;
        }
        else if (mCurrentScrollingLayerId != 0) {
            // Check the scrolling bounds to see if we will actually do any
            // scrolling.  The rectangle is in document coordinates.
            Int32 maxX, maxY;
            mScrollingLayerRect->GetRight(&maxX);
            mScrollingLayerRect->GetBottom(&maxY);
            Int32 left, top;
            mScrollingLayerRect->GetLeft(&left);
            Int32 resultX = ClampBetween(maxX, 0,
                    left + contentX);
            mScrollingLayerRect->GetTop(&top);
            Int32 resultY = ClampBetween(maxY, 0,
                    top + contentY);

            if (resultX != left
                    || resultY != top
                    || (contentX | contentY) == 0) {
                // In case we switched to dragging the page.
                mTouchMode = TOUCH_DRAG_LAYER_MODE;
                deltaX = contentX;
                deltaY = contentY;
                oldX = left;
                oldY = top;
                rangeX = maxX;
                rangeY = maxY;
                allDrag = FALSE;
            }
        }

        if (mOverScrollGlow != NULL) {
            mOverScrollGlow->SetOverScrollDeltas(deltaX, deltaY);
        }

        mWebViewPrivate->OverScrollBy(deltaX, deltaY, oldX, oldY,
                rangeX, rangeY,
                mOverscrollDistance, mOverscrollDistance, TRUE);
        if (mOverScrollGlow != NULL && mOverScrollGlow->IsAnimating()) {
            Invalidate();
        }
    }
    mZoomManager->KeepZoomPickerVisible();
    return allDrag;
}

void CWebViewClassic::StopTouch()
{
    if (mScroller->IsFinished() && !mSelectingText
            && (mTouchMode == TOUCH_DRAG_MODE
            || mTouchMode == TOUCH_DRAG_LAYER_MODE)) {
        CWebViewCore::ResumePriority();
        CWebViewCore::ResumeUpdatePicture(mWebViewCore);
        NativeSetIsScrolling(FALSE);
    }

    // we also use mVelocityTracker == null to tell us that we are
    // not "moving around", so we can take the slower/prettier
    // mode in the drawing code
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }

    // Release any pulled glows
    if (mOverScrollGlow != NULL) {
        mOverScrollGlow->ReleaseAll();
    }

    if (mSelectingText) {
        mSelectionStarted = FALSE;
        SyncSelectionCursors();
        if (mIsCaretSelection) {
            ResetCaretTimer();
        }
        Invalidate();
    }
}

void CWebViewClassic::CancelTouch()
{
    // we also use mVelocityTracker == null to tell us that we are
    // not "moving around", so we can take the slower/prettier
    // mode in the drawing code
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }

    if ((mTouchMode == TOUCH_DRAG_MODE
            || mTouchMode == TOUCH_DRAG_LAYER_MODE) && !mSelectingText) {
        CWebViewCore::ResumePriority();
        CWebViewCore::ResumeUpdatePicture(mWebViewCore);
        NativeSetIsScrolling(FALSE);
    }
    mPrivateHandler->RemoveMessages(SWITCH_TO_SHORTPRESS);
    mPrivateHandler->RemoveMessages(SWITCH_TO_LONGPRESS);
    mPrivateHandler->RemoveMessages(DRAG_HELD_MOTIONLESS);
    RemoveTouchHighlight();
    mHeldMotionless = MOTIONLESS_TRUE;
    mTouchMode = TOUCH_DONE_MODE;
}

void CWebViewClassic::SnapDraggingCursor()
{
    Int32 sdcX, sdcY;
    mSelectDraggingCursor->GetX(&sdcX);
    mSelectDraggingCursor->GetY(&sdcY);
    Float scale = ScaleAlongSegment(
            sdcX, sdcY,
            mSelectDraggingTextQuad->p4, mSelectDraggingTextQuad->p3);
    // clamp scale to ensure point is on the bottom segment
    scale = Elastos::Core::Math::Max(0.0f, scale);
    scale = Elastos::Core::Math::Min(scale, 1.0f);
    Float p3X, p3Y, p4X, p4Y;
    mSelectDraggingTextQuad->p4->GetX(&p4X);
    mSelectDraggingTextQuad->p3->GetX(&p3X);
    Float newX = ScaleCoordinate(scale,
            p4X, p3X);
    mSelectDraggingTextQuad->p4->GetY(&p4Y);
    mSelectDraggingTextQuad->p3->GetY(&p3Y);
    Float newY = ScaleCoordinate(scale,
            p4Y, p3Y);
    Int32 x = Elastos::Core::Math::Round(newX);
    Int32 y = Elastos::Core::Math::Round(newY);
    if (mIsEditingText) {
        Int32 left, right, top, bottom;
        mEditTextContentBounds->Get(&left, &top, &right, &bottom);
        x = ClampBetween(x, left, right);
        y = ClampBetween(y, top, bottom);
    }
    mSelectDraggingCursor->Set(x, y);
}

Float CWebViewClassic::ScaleCoordinate(
    /* [in] */ Float scale,
    /* [in] */ Float coord1,
    /* [in] */ Float coord2)
{
    Float diff = coord2 - coord1;
    return coord1 + (scale * diff);
}

ECode CWebViewClassic::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 source = 0;
    event->GetSource(&source);
    if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
        Int32 action;
        event->GetAction(&action);
        switch (action) {
            case IMotionEvent::ACTION_SCROLL: {
                Float vscroll;
                Float hscroll;
                Int32 state;
                event->GetMetaState(&state);
                if ((state & IKeyEvent::META_SHIFT_ON) != 0) {
                    vscroll = 0;
                    event->GetAxisValue(IMotionEvent::AXIS_VSCROLL, &hscroll);
                }
                else {
                    Float value;
                    event->GetAxisValue(IMotionEvent::AXIS_VSCROLL, &value);
                    vscroll = -value;
                    event->GetAxisValue(IMotionEvent::AXIS_HSCROLL, &hscroll);
                }
                if (hscroll != 0 || vscroll != 0) {
                    Float scrollFactor;
                    mWebViewPrivate->GetVerticalScrollFactor(&scrollFactor);
                    Int32 vdelta = (Int32) (vscroll * scrollFactor);
                    mWebViewPrivate->GetHorizontalScrollFactor(&scrollFactor);
                    Int32 hdelta = (Int32) (hscroll * scrollFactor);

                    AbortAnimation();
                    Int32 oldTouchMode = mTouchMode;
                    Float x, y;
                    event->GetX(&x);
                    event->GetY(&y);
                    StartScrollingLayer(x, y);
                    DoDrag(hdelta, vdelta);
                    mTouchMode = oldTouchMode;
                    *result = TRUE;
                    return NOERROR;
                }
            }
        }
    }
    return mWebViewPrivate->Super_onGenericMotionEvent(event, result);
}

ECode CWebViewClassic::SetMapTrackballToArrowKeys(
    /* [in] */ Boolean setMap)
{
    mMapTrackballToArrowKeys = setMap;
    return NOERROR;
}

void CWebViewClassic::ResetTrackballTime()
{
    mTrackballLastTime = 0;
}

ECode CWebViewClassic::OnTrackballEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 time;
    ev->GetEventTime(&time);
    Int32 state;
    ev->GetMetaState(&state);
    if ((state & IKeyEvent::META_ALT_ON) != 0) {
        Float y;
        if (ev->GetY(&y), y > 0) PageDown(TRUE, result);
        if (ev->GetY(&y), y < 0) PageUp(TRUE, result);
        *result = TRUE;
        return NOERROR;
    }
    Int32 action;
    ev->GetAction(&action);
    if (action == IMotionEvent::ACTION_DOWN) {
        if (mSelectingText) {
            *result = TRUE;
            return NOERROR; // discard press if copy in progress
        }
        mTrackballDown = TRUE;
        if (mNativeClass == 0) {
            *result = FALSE;
            return NOERROR;
        }
        if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "onTrackballEvent down ev=%p time=%d mLastCursorTime=%d",
                    ev, time, mLastCursorTime);
        }
        Boolean isInTouchMode = FALSE;
        if (mWebView->IsInTouchMode(&isInTouchMode), isInTouchMode) mWebView->RequestFocusFromTouch(result);
        *result = FALSE;
        return NOERROR; // let common code in onKeyDown at it
    }
    ev->GetAction(&action);
    if (action == IMotionEvent::ACTION_UP) {
        // LONG_PRESS_CENTER is set in common onKeyDown
        mPrivateHandler->RemoveMessages(LONG_PRESS_CENTER);
        mTrackballDown = FALSE;
        mTrackballUpTime = time;
        if (mSelectingText) {
            CopySelection(result);
            SelectionDone();
            *result = TRUE;
            return NOERROR; // discard press if copy in progress
        }
        if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "onTrackballEvent up ev=%p time=%d", ev, time);
        }
        *result = FALSE;
        return NOERROR; // let common code in onKeyUp at it
    }
    AutoPtr<IAccessibilityManager> manager;
    Boolean enabled;
    if ((mMapTrackballToArrowKeys && (ev->GetMetaState(&state), state & IKeyEvent::META_SHIFT_ON) == 0) ||
            (CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager),
                manager->IsEnabled(&enabled), enabled)) {
        if (DebugFlags::WEB_VIEW) Logger::V(LOGTAG, "onTrackballEvent gmail quit");
        *result = FALSE;
        return NOERROR;
    }
    if (mTrackballDown) {
        if (DebugFlags::WEB_VIEW) Logger::V(LOGTAG, "onTrackballEvent down quit");
        *result = TRUE;
        return NOERROR; // discard move if trackball is down
    }
    if (time - mTrackballUpTime < TRACKBALL_TIMEOUT) {
        if (DebugFlags::WEB_VIEW) Logger::V(LOGTAG, "onTrackballEvent up timeout quit");
        *result = TRUE;
        return NOERROR;
    }
    // TODO: alternatively we can do panning as touch does
    SwitchOutDrawHistory();
    if (time - mTrackballLastTime > TRACKBALL_TIMEOUT) {
        if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "onTrackballEvent time=%d last=%d",
                    time, mTrackballLastTime);
        }
        mTrackballFirstTime = time;
        mTrackballXMove = mTrackballYMove = 0;
    }
    mTrackballLastTime = time;
    if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "onTrackballEvent ev=%p time=%d", ev, time);
    }
    Float evX, evY;
    ev->GetX(&evX);
    ev->GetY(&evY);
    mTrackballRemainsX += evX;
    mTrackballRemainsY += evY;
    Int32 metaState;
    ev->GetMetaState(&metaState);
    DoTrackball(time, metaState);
    *result = TRUE;
    return NOERROR;
}

Int32 CWebViewClassic::ScaleTrackballX(
    /* [in] */ Float xRate,
    /* [in] */ Int32 width)
{
    Int32 xMove = (Int32) (xRate / TRACKBALL_SCALE * width);
    Int32 nextXMove = xMove;
    if (xMove > 0) {
        if (xMove > mTrackballXMove) {
            xMove -= mTrackballXMove;
        }
    }
    else if (xMove < mTrackballXMove) {
        xMove -= mTrackballXMove;
    }
    mTrackballXMove = nextXMove;
    return xMove;
}

Int32 CWebViewClassic::ScaleTrackballY(
    /* [in] */ Float yRate,
    /* [in] */ Int32 height)
{
    Int32 yMove = (Int32) (yRate / TRACKBALL_SCALE * height);
    Int32 nextYMove = yMove;
    if (yMove > 0) {
        if (yMove > mTrackballYMove) {
            yMove -= mTrackballYMove;
        }
    }
    else if (yMove < mTrackballYMove) {
        yMove -= mTrackballYMove;
    }
    mTrackballYMove = nextYMove;
    return yMove;
}

Int32 CWebViewClassic::KeyCodeToSoundsEffect(
    /* [in] */ Int32 keyCode)
{
    switch(keyCode) {
        case IKeyEvent::KEYCODE_DPAD_UP:
            return SoundEffectConstants::NAVIGATION_UP;
        case IKeyEvent::KEYCODE_DPAD_RIGHT:
            return SoundEffectConstants::NAVIGATION_RIGHT;
        case IKeyEvent::KEYCODE_DPAD_DOWN:
            return SoundEffectConstants::NAVIGATION_DOWN;
        case IKeyEvent::KEYCODE_DPAD_LEFT:
            return SoundEffectConstants::NAVIGATION_LEFT;
    }
    return 0;
}

void CWebViewClassic::DoTrackball(
    /* [in] */ Int64 time,
    /* [in] */ Int32 metaState)
{
    Int32 elapsed = (Int32) (mTrackballLastTime - mTrackballFirstTime);
    if (elapsed == 0) {
        elapsed = TRACKBALL_TIMEOUT;
    }
    Float xRate = mTrackballRemainsX * 1000 / elapsed;
    Float yRate = mTrackballRemainsY * 1000 / elapsed;
    Int32 viewWidth = GetViewWidth();
    Int32 viewHeight = GetViewHeight();
    Float ax = Elastos::Core::Math::Abs(xRate);
    Float ay = Elastos::Core::Math::Abs(yRate);
    Float maxA = Elastos::Core::Math::Max(ax, ay);
    if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "doTrackball elapsed=%d xRate=%f yRate=%f mTrackballRemainsX=%f mTrackballRemainsY=%f",
                elapsed, xRate, yRate, mTrackballRemainsX, mTrackballRemainsY);
    }
    Int32 width = mContentWidth - viewWidth;
    Int32 height = mContentHeight - viewHeight;
    if (width < 0) width = 0;
    if (height < 0) height = 0;
    ax = Elastos::Core::Math::Abs(mTrackballRemainsX * TRACKBALL_MULTIPLIER);
    ay = Elastos::Core::Math::Abs(mTrackballRemainsY * TRACKBALL_MULTIPLIER);
    maxA = Elastos::Core::Math::Max(ax, ay);
    Int32 count = Elastos::Core::Math::Max(0, (Int32) maxA);
    Int32 oldScrollX = GetScrollX();
    Int32 oldScrollY = GetScrollY();
    if (count > 0) {
        Int32 selectKeyCode = ax < ay ? mTrackballRemainsY < 0 ?
                IKeyEvent::KEYCODE_DPAD_UP : IKeyEvent::KEYCODE_DPAD_DOWN :
                mTrackballRemainsX < 0 ? IKeyEvent::KEYCODE_DPAD_LEFT :
                IKeyEvent::KEYCODE_DPAD_RIGHT;
        count = Elastos::Core::Math::Min(count, TRACKBALL_MOVE_COUNT);
        if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "doTrackball keyCode=%d count=%d mTrackballRemainsX=%f mTrackballRemainsY=%f",
                    selectKeyCode, count, mTrackballRemainsX, mTrackballRemainsY);
        }
        if (mNativeClass != 0) {
            for (Int32 i = 0; i < count; i++) {
                LetPageHandleNavKey(selectKeyCode, time, TRUE, metaState);
            }
            LetPageHandleNavKey(selectKeyCode, time, FALSE, metaState);
        }
        mTrackballRemainsX = mTrackballRemainsY = 0;
    }
    if (count >= TRACKBALL_SCROLL_COUNT) {
        Int32 xMove = ScaleTrackballX(xRate, width);
        Int32 yMove = ScaleTrackballY(yRate, height);
        if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "doTrackball pinScrollBy count=%d xMove=%d yMove=%d mScrollX-oldScrollX=%d mScrollY-oldScrollY=%d",
                    count, xMove, yMove, GetScrollX() - oldScrollX, GetScrollY()-oldScrollY);
        }
        if (Elastos::Core::Math::Abs(GetScrollX() - oldScrollX) > Elastos::Core::Math::Abs(xMove)) {
            xMove = 0;
        }
        if (Elastos::Core::Math::Abs(GetScrollY() - oldScrollY) > Elastos::Core::Math::Abs(yMove)) {
            yMove = 0;
        }
        if (xMove != 0 || yMove != 0) {
            PinScrollBy(xMove, yMove, TRUE, 0);
        }
    }
}

/**
 * Compute the maximum horizontal scroll position. Used by {@link OverScrollGlow}.
 * @return Maximum horizontal scroll position within real content
 */
Int32 CWebViewClassic::ComputeMaxScrollX()
{
    return Elastos::Core::Math::Max(ComputeRealHorizontalScrollRange() - GetViewWidth(), 0);
}

/**
 * Compute the maximum vertical scroll position. Used by {@link OverScrollGlow}.
 * @return Maximum vertical scroll position within real content
 */
Int32 CWebViewClassic::ComputeMaxScrollY()
{
    Int32 range = ComputeRealVerticalScrollRange();
    return Elastos::Core::Math::Max(range + GetTitleHeight()
                - GetViewHeightWithTitle(), 0);
}

Boolean CWebViewClassic::UpdateScrollCoordinates(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 oldX = GetScrollX();
    Int32 oldY = GetScrollY();
    SetScrollXRaw(x);
    SetScrollYRaw(y);
    if (oldX != GetScrollX() || oldY != GetScrollY()) {
        mWebViewPrivate->OnScrollChanged(GetScrollX(), GetScrollY(), oldX, oldY);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

ECode CWebViewClassic::FlingScroll(
    /* [in] */ Int32 vx,
    /* [in] */ Int32 vy)
{
    mScroller->Fling(GetScrollX(), GetScrollY(), vx, vy, 0, ComputeMaxScrollX(), 0,
            ComputeMaxScrollY(), mOverflingDistance, mOverflingDistance);
    Invalidate();
    return NOERROR;
}

void CWebViewClassic::DoFling()
{
    if (mVelocityTracker == NULL) {
        return;
    }
    Int32 maxX = ComputeMaxScrollX();
    Int32 maxY = ComputeMaxScrollY();

    mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumFling);
    Float x, y;
    mVelocityTracker->GetXVelocity(&x);
    mVelocityTracker->GetYVelocity(&y);
    Int32 vx = (Int32)x;
    Int32 vy = (Int32)y;

    Int32 scrollX = GetScrollX();
    Int32 scrollY = GetScrollY();
    Int32 overscrollDistance = mOverscrollDistance;
    Int32 overflingDistance = mOverflingDistance;

    // Use the layer's scroll data if applicable.
    if (mTouchMode == TOUCH_DRAG_LAYER_MODE) {
        mScrollingLayerRect->GetLeft(&scrollX);
        mScrollingLayerRect->GetTop(&scrollY);
        mScrollingLayerRect->GetRight(&maxX);
        mScrollingLayerRect->GetBottom(&maxY);
        // No overscrolling for layers.
        overscrollDistance = overflingDistance = 0;
    }
    else if (mTouchMode == TOUCH_DRAG_TEXT_MODE) {
        scrollX = GetTextScrollX();
        scrollY = GetTextScrollY();
        maxX = GetMaxTextScrollX();
        maxY = GetMaxTextScrollY();
        // No overscrolling for edit text.
        overscrollDistance = overflingDistance = 0;
    }

    if (mSnapScrollMode != SNAP_NONE) {
        if ((mSnapScrollMode & SNAP_X) == SNAP_X) {
            vy = 0;
        }
        else {
            vx = 0;
        }
    }
    if ((maxX == 0 && vy == 0) || (maxY == 0 && vx == 0)) {
        CWebViewCore::ResumePriority();
        if (!mSelectingText) {
            CWebViewCore::ResumeUpdatePicture(mWebViewCore);
        }
        if (mScroller->SpringBack(scrollX, scrollY, 0, maxX, 0, maxY)) {
            Invalidate();
        }
        return;
    }
    Float currentVelocity = mScroller->GetCurrVelocity();
    Float velocity = (Float) Elastos::Core::Math::Hypot(vx, vy);
    if (mLastVelocity > 0 && currentVelocity > 0 && velocity
            > mLastVelocity * MINIMUM_VELOCITY_RATIO_FOR_ACCELERATION) {
        Float deltaR = (Float) (Elastos::Core::Math::Abs(Elastos::Core::Math::Atan2(mLastVelY, mLastVelX)
                - Elastos::Core::Math::Atan2(vy, vx)));
        Float circle = (Float) (Elastos::Core::Math::DOUBLE_PI) * 2.0f;
        if (deltaR > circle * 0.9f || deltaR < circle * 0.1f) {
            vx += currentVelocity * mLastVelX / mLastVelocity;
            vy += currentVelocity * mLastVelY / mLastVelocity;
            velocity = (Float) Elastos::Core::Math::Hypot(vx, vy);
            if (DebugFlags::WEB_VIEW) {
                Logger::V(LOGTAG, "doFling vx=%d vy=%d", vx, vy);
            }
        }
        else if (DebugFlags::WEB_VIEW) {
            Logger::V(LOGTAG, "doFling missed %f", deltaR / circle);
        }
    }
    else if (DebugFlags::WEB_VIEW) {
        Logger::V(LOGTAG, "doFling start last=%f  current=%f vx=%d vy=%d maxX=%d maxY=%d scrollX=%d scrollY=%d layer=%d",
                mLastVelocity, currentVelocity, vx, vy, maxX, maxY, scrollX, scrollY, mCurrentScrollingLayerId);
    }

    // Allow sloppy flings without overscrolling at the edges.
    if ((scrollX == 0 || scrollX == maxX) && Elastos::Core::Math::Abs(vx) < Elastos::Core::Math::Abs(vy)) {
        vx = 0;
    }
    if ((scrollY == 0 || scrollY == maxY) && Elastos::Core::Math::Abs(vy) < Elastos::Core::Math::Abs(vx)) {
        vy = 0;
    }

    if (overscrollDistance < overflingDistance) {
        if ((vx > 0 && scrollX == -overscrollDistance) ||
                (vx < 0 && scrollX == maxX + overscrollDistance)) {
            vx = 0;
        }
        if ((vy > 0 && scrollY == -overscrollDistance) ||
                (vy < 0 && scrollY == maxY + overscrollDistance)) {
            vy = 0;
        }
    }

    mLastVelX = vx;
    mLastVelY = vy;
    mLastVelocity = velocity;

    // no horizontal overscroll if the content just fits
    mScroller->Fling(scrollX, scrollY, -vx, -vy, 0, maxX, 0, maxY,
            maxX == 0 ? 0 : overflingDistance, overflingDistance);

    Invalidate();
}

/**
 * See {@link WebView#getZoomControls()}
 */
ECode CWebViewClassic::GetZoomControls(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IWebSettings> settings;
    GetSettings((IWebSettings**)&settings);
    Boolean result = FALSE;
    if (settings->SupportZoom(&result), !result) {
        Logger::W(LOGTAG, "This WebView doesn't support zoom.");
        *view = NULL;
        return NOERROR;
    }
    AutoPtr<IView> v = mZoomManager->GetExternalZoomPicker();
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

void CWebViewClassic::DismissZoomControl()
{
    mZoomManager->DismissZoomPicker();
}

Float CWebViewClassic::GetDefaultZoomScale()
{
    return mZoomManager->GetDefaultScale();
}

/**
 * Return the overview scale of the WebView
 * @return The overview scale.
 */
Float CWebViewClassic::GetZoomOverviewScale()
{
    return mZoomManager->GetZoomOverviewScale();
}

/**
 * See {@link WebView#canZoomIn()}
 */
ECode CWebViewClassic::CanZoomIn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mZoomManager->CanZoomIn();
    return NOERROR;
}

/**
 * See {@link WebView#canZoomOut()}
 */
ECode CWebViewClassic::CanZoomOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mZoomManager->CanZoomOut();
    return NOERROR;
}

/**
 * See {@link WebView#zoomIn()}
 */
ECode CWebViewClassic::ZoomIn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mZoomManager->ZoomIn();
    return NOERROR;
}

/**
 * See {@link WebView#zoomOut()}
 */
ECode CWebViewClassic::ZoomOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mZoomManager->ZoomOut();
    return NOERROR;
}

/*
 * Return true if the rect (e.g. plugin) is fully visible and maximized
 * inside the WebView.
 */
Boolean CWebViewClassic::IsRectFitOnScreen(
    /* [in] */ IRect* rect)
{
    Int32 rectWidth;
    Int32 rectHeight;
    rect->GetWidth(&rectWidth);
    rect->GetHeight(&rectHeight);
    Int32 viewWidth = GetViewWidth();
    Int32 viewHeight = GetViewHeightWithTitle();
    Float scale = Elastos::Core::Math::Min((Float) viewWidth / rectWidth, (Float) viewHeight / rectHeight);
    scale = mZoomManager->ComputeScaleWithLimits(scale);
    Int32 left, right, top, bottom;
    rect->Get(&left, &top, &right, &bottom);
    return !mZoomManager->WillScaleTriggerZoom(scale)
            && ContentToViewX(left) >= GetScrollX()
            && ContentToViewX(right) <= GetScrollX() + viewWidth
            && ContentToViewY(top) >= GetScrollY()
            && ContentToViewY(bottom) <= GetScrollY() + viewHeight;
}

/*
 * Maximize and center the rectangle, specified in the document coordinate
 * space, inside the WebView. If the zoom doesn't need to be changed, do an
 * animated scroll to center it. If the zoom needs to be changed, find the
 * zoom center and do a smooth zoom transition. The rect is in document
 * coordinates
 */
void CWebViewClassic::CenterFitRect(
    /* [in] */ IRect* rect)
{
    Int32 rectWidth;
    Int32 rectHeight;
    rect->GetWidth(&rectWidth);
    rect->GetHeight(&rectHeight);
    Int32 viewWidth = GetViewWidth();
    Int32 viewHeight = GetViewHeightWithTitle();
    Float scale = Elastos::Core::Math::Min((Float) viewWidth / rectWidth, (Float) viewHeight
            / rectHeight);
    scale = mZoomManager->ComputeScaleWithLimits(scale);
    if (!mZoomManager->WillScaleTriggerZoom(scale)) {
        Int32 top, left;
        rect->GetLeft(&left);
        rect->GetTop(&top);
        PinScrollTo(ContentToViewX(left + rectWidth / 2) - viewWidth / 2,
                ContentToViewY(top + rectHeight / 2) - viewHeight / 2,
                TRUE, 0);
    }
    else {
        Int32 top, left;
        Float actualScale = mZoomManager->GetScale();
        rect->GetLeft(&left);
        Float oldScreenX = left * actualScale - GetScrollX();
        Float rectViewX = left * scale;
        Float rectViewWidth = rectWidth * scale;
        Float newMaxWidth = mContentWidth * scale;
        Float newScreenX = (viewWidth - rectViewWidth) / 2;
        // pin the newX to the WebView
        if (newScreenX > rectViewX) {
            newScreenX = rectViewX;
        }
        else if (newScreenX > (newMaxWidth - rectViewX - rectViewWidth)) {
            newScreenX = viewWidth - (newMaxWidth - rectViewX);
        }
        Float zoomCenterX = (oldScreenX * scale - newScreenX * actualScale)
                / (scale - actualScale);
        rect->GetTop(&top);
        Float oldScreenY = top * actualScale + GetTitleHeight()
                - GetScrollY();
        rect->GetTop(&top);
        Float rectViewY = top * scale + GetTitleHeight();
        Float rectViewHeight = rectHeight * scale;
        Float newMaxHeight = mContentHeight * scale + GetTitleHeight();
        Float newScreenY = (viewHeight - rectViewHeight) / 2;
        // pin the newY to the WebView
        if (newScreenY > rectViewY) {
            newScreenY = rectViewY;
        }
        else if (newScreenY > (newMaxHeight - rectViewY - rectViewHeight)) {
            newScreenY = viewHeight - (newMaxHeight - rectViewY);
        }
        Float zoomCenterY = (oldScreenY * scale - newScreenY * actualScale)
                / (scale - actualScale);
        mZoomManager->SetZoomCenter(zoomCenterX, zoomCenterY);
        mZoomManager->StartZoomAnimation(scale, FALSE);
    }
}

// Called by JNI to handle a touch on a node representing an email address,
// address, or phone number
void CWebViewClassic::OverrideLoading(
    /* [in] */ const String& url)
{
    mCallbackProxy->UiOverrideUrlLoading(url);
}

ECode CWebViewClassic::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Check if we are destroyed
    if (mWebViewCore == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    // FIXME: If a subwindow is showing find, and the user touches the
    // background window, it can steal focus.
    if (mFindIsUp) {
        *result = FALSE;
        return NOERROR;
    }
    mWebViewPrivate->Super_requestFocus(direction, previouslyFocusedRect, result);
    AutoPtr<IWebSettingsClassic> settings;
    mWebViewCore->GetSettings((IWebSettingsClassic**)&settings);
    Boolean touchMode;
    if (((WebSettingsClassic*)settings.Get())->GetNeedInitialFocus()
            && (mWebView->IsInTouchMode(&touchMode), !touchMode)) {
        // For cases such as GMail, where we gain focus from a direction,
        // we want to move to the first available link.
        // FIXME: If there are no visible links, we may not want to
        Int32 fakeKeyDirection = 0;
        switch(direction) {
            case IView::FOCUS_UP:
                fakeKeyDirection = IKeyEvent::KEYCODE_DPAD_UP;
                break;
            case IView::FOCUS_DOWN:
                fakeKeyDirection = IKeyEvent::KEYCODE_DPAD_DOWN;
                break;
            case IView::FOCUS_LEFT:
                fakeKeyDirection = IKeyEvent::KEYCODE_DPAD_LEFT;
                break;
            case IView::FOCUS_RIGHT:
                fakeKeyDirection = IKeyEvent::KEYCODE_DPAD_RIGHT;
                break;
            default:
                return NOERROR;
        }
        mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_INITIAL_FOCUS, fakeKeyDirection);
    }
    return NOERROR;
}

ECode CWebViewClassic::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 heightMode = Elastos::Droid::View::View::MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSize = Elastos::Droid::View::View::MeasureSpec::GetSize(heightMeasureSpec);
    Int32 widthMode = Elastos::Droid::View::View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize = Elastos::Droid::View::View::MeasureSpec::GetSize(widthMeasureSpec);

    Int32 measuredHeight = heightSize;
    Int32 measuredWidth = widthSize;

    // Grab the content size from WebViewCore.
    Int32 contentHeight = ContentToViewDimension(mContentHeight);
    Int32 contentWidth = ContentToViewDimension(mContentWidth);

//        Log.d(LOGTAG, "------- measure " + heightMode);

    if (heightMode != Elastos::Droid::View::View::MeasureSpec::EXACTLY) {
        mHeightCanMeasure = TRUE;
        measuredHeight = contentHeight;
        if (heightMode == Elastos::Droid::View::View::MeasureSpec::AT_MOST) {
            // If we are larger than the AT_MOST height, then our height can
            // no longer be measured and we should scroll internally.
            if (measuredHeight > heightSize) {
                measuredHeight = heightSize;
                mHeightCanMeasure = FALSE;
                measuredHeight |= IView::MEASURED_STATE_TOO_SMALL;
            }
        }
    }
    else {
        mHeightCanMeasure = FALSE;
    }
    if (mNativeClass != 0) {
        NativeSetHeightCanMeasure(mHeightCanMeasure);
    }
    // For the width, always use the given size unless unspecified.
    if (widthMode == Elastos::Droid::View::View::MeasureSpec::UNSPECIFIED) {
        mWidthCanMeasure = TRUE;
        measuredWidth = contentWidth;
    }
    else {
        if (measuredWidth < contentWidth) {
            measuredWidth |= IView::MEASURED_STATE_TOO_SMALL;
        }
        mWidthCanMeasure = FALSE;
    }

    {
        Mutex::Autolock lock(mLock);
        mWebViewPrivate->SetMeasuredDimension(measuredWidth, measuredHeight);
    }
    return NOERROR;
}

ECode CWebViewClassic::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mNativeClass == 0) {
        *result = FALSE;
        return NOERROR;
    }

    // don't scroll while in zoom animation. When it is done, we will adjust
    // the necessary components
    if (mZoomManager->IsFixedLengthAnimationInProgress()) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 viewLeft, viewTop, scrollX, scrollY;
    child->GetLeft(&viewLeft);
    child->GetScrollX(&scrollX);
    child->GetTop(&viewTop);
    child->GetScrollY(&scrollY);
    rect->Offset(viewLeft - scrollX, viewTop - scrollY);

    Int32 scrollbarWidth;
    mWebView->GetVerticalScrollbarWidth(&scrollbarWidth);
    AutoPtr<IRect> content;
    CRect::New(ViewToContentX(GetScrollX()),
            ViewToContentY(GetScrollY()),
            ViewToContentX(GetScrollX() + GetWidth() - scrollbarWidth),
            ViewToContentY(GetScrollY() + GetViewHeightWithTitle()),
            (IRect**)&content);

    Int32 contentTop, contentBottom;
    content->GetTop(&contentTop);
    content->GetBottom(&contentBottom);
    Int32 screenTop = ContentToViewY(contentTop);
    Int32 screenBottom = ContentToViewY(contentBottom);
    Int32 height = screenBottom - screenTop;
    Int32 scrollYDelta = 0;

    Int32 rectTop, rectBottom;
    rect->GetBottom(&rectBottom);
    rect->GetTop(&rectTop);
    if (rectBottom > screenBottom) {
        Int32 oneThirdOfScreenHeight = height / 3;
        Int32 rectHeight;
        rect->GetHeight(&rectHeight);
        if (rectHeight > 2 * oneThirdOfScreenHeight) {
            // If the rectangle is too tall to fit in the bottom two thirds
            // of the screen, place it at the top.
            rect->GetTop(&rectTop);
            scrollYDelta = rectTop - screenTop;
        }
        else {
            // If the rectangle will still fit on screen, we want its
            // top to be in the top third of the screen.
            rect->GetTop(&rectTop);
            scrollYDelta = rectTop - (screenTop + oneThirdOfScreenHeight);
        }
    }
    else if (rectTop < screenTop) {
        scrollYDelta = rectTop - screenTop;
    }

    Int32 contentLeft, contentRight;
    content->GetLeft(&contentLeft);
    content->GetRight(&contentRight);
    Int32 screenLeft = ContentToViewX(contentLeft);
    Int32 screenRight = ContentToViewX(contentRight);
    Int32 width = screenRight - screenLeft;
    Int32 scrollXDelta = 0;

    Int32 rectLeft, rectRight;
    rect->GetRight(&rectRight);
    rect->GetLeft(&rectLeft);
    if (rectRight > screenRight && rectLeft > screenLeft) {
        Int32 rectWidth;
        rect->GetWidth(&rectWidth);
        if (rectWidth > width) {
            scrollXDelta += (rectLeft - screenLeft);
        }
        else {
            scrollXDelta += (rectRight - screenRight);
        }
    }
    else if (rectLeft < screenLeft) {
        scrollXDelta -= (screenLeft - rectLeft);
    }

    if ((scrollYDelta | scrollXDelta) != 0) {
        return PinScrollBy(scrollXDelta, scrollYDelta, !immediate, 0);
    }

    *result = FALSE;
    return NOERROR;
}

void CWebViewClassic::ReplaceTextfieldText(
    /* [in] */ Int32 oldStart,
    /* [in] */ Int32 oldEnd,
    /* [in] */ const String& replace,
    /* [in] */ Int32 newStart,
    /* [in] */ Int32 newEnd)
{
    AutoPtr<CWebViewCore::ReplaceTextData> arg = new CWebViewCore::ReplaceTextData();
    arg->mReplace = replace;
    arg->mNewStart = newStart;
    arg->mNewEnd = newEnd;
    mTextGeneration++;
    arg->mTextGeneration = mTextGeneration;
    SendBatchableInputMessage(CWebViewCore::EventHub::REPLACE_TEXT, oldStart, oldEnd, (IInterface*)arg.Get());
}

void CWebViewClassic::PassToJavaScript(
    /* [in] */ const String& currentText,
    /* [in] */ IKeyEvent* event)
{
    // check if mWebViewCore has been destroyed
    if (mWebViewCore == NULL) {
        return;
    }
    AutoPtr<CWebViewCore::JSKeyData> arg = new CWebViewCore::JSKeyData();
    arg->mEvent = event;
    arg->mCurrentText = currentText;
    // Increase our text generation number, and pass it to webcore thread
    mTextGeneration++;
    mWebViewCore->SendMessage(CWebViewCore::EventHub::PASS_TO_JS, mTextGeneration, 0, arg.Get());
    // WebKit's document state is not saved until about to leave the page.
    // To make sure the host application, like Browser, has the up to date
    // document state when it goes to background, we force to save the
    // document state.
    mWebViewCore->RemoveMessages(CWebViewCore::EventHub::SAVE_DOCUMENT_STATE);
    mWebViewCore->SendMessageDelayed(CWebViewCore::EventHub::SAVE_DOCUMENT_STATE, NULL, 1000);
}

ECode CWebViewClassic::GetWebViewCore(
    /* [out] */ IWebViewCore** wvc)
{
    Mutex::Autolock lock(mLock);
    VALIDATE_NOT_NULL(wvc);
    *wvc = mWebViewCore;
    REFCOUNT_ADD(*wvc);
    return NOERROR;
}

Boolean CWebViewClassic::CanTextScroll(
    /* [in] */ Int32 directionX,
    /* [in] */ Int32 directionY)
{
    Int32 scrollX = GetTextScrollX();
    Int32 scrollY = GetTextScrollY();
    Int32 maxScrollX = GetMaxTextScrollX();
    Int32 maxScrollY = GetMaxTextScrollY();
    Boolean canScrollX = (directionX > 0)
            ? (scrollX < maxScrollX)
            : (scrollX > 0);
    Boolean canScrollY = (directionY > 0)
            ? (scrollY < maxScrollY)
            : (scrollY > 0);
    return canScrollX || canScrollY;
}

Int32 CWebViewClassic::GetTextScrollX()
{
    Int32 left;
    mEditTextContent->GetLeft(&left);
    return -left;
}

Int32 CWebViewClassic::GetTextScrollY()
{
    Int32 top;
    mEditTextContent->GetTop(&top);
    return -top;
}

Int32 CWebViewClassic::GetMaxTextScrollX()
{
    Int32 w1, w2;
    mEditTextContent->GetWidth(&w1);
    mEditTextContentBounds->GetWidth(&w2);
    return Elastos::Core::Math::Max(0, w1 - w2);
}

Int32 CWebViewClassic::GetMaxTextScrollY()
{
    Int32 h1, h2;
    mEditTextContent->GetHeight(&h1);
    mEditTextContentBounds->GetHeight(&h2);
    return Elastos::Core::Math::Max(0, h1 - h2);
}

void CWebViewClassic::AcquireWakeLock()
{
    if (mWakeLock == NULL) {
        AutoPtr<IPowerManager> pm;
        mContext->GetSystemService(IContext::POWER_SERVICE, (IInterface**)&pm);
        pm->NewWakeLock(IPowerManager::SCREEN_BRIGHT_WAKE_LOCK, String("flash_plugin"), (IPowerManagerWakeLock**)&mWakeLock);
        mWakeLock->AcquireLock();
    }
}

void CWebViewClassic::ReleaseWakeLock()
{
    Boolean held = FALSE;
    if(mWakeLock != NULL && (mWakeLock->IsHeld(&held), held)){
        mWakeLock->ReleaseLock();
        mWakeLock = NULL;
    }
}

void CWebViewClassic::SetHitTestTypeFromUrl(
    /* [in] */ const String& url)
{
    String substr;
    if (url.StartWith(SCHEME_GEO)) {
        mInitialHitTestResult->SetType(IWebViewHitTestResult::GEO_TYPE);
        substr = url.Substring(SCHEME_GEO.GetLength());
    }
    else if (url.StartWith(SCHEME_TEL)) {
        mInitialHitTestResult->SetType(IWebViewHitTestResult::PHONE_TYPE);
        substr = url.Substring(SCHEME_TEL.GetLength());
    }
    else if (url.StartWith(SCHEME_MAILTO)) {
        mInitialHitTestResult->SetType(IWebViewHitTestResult::EMAIL_TYPE);
        substr = url.Substring(SCHEME_MAILTO.GetLength());
    }
    else {
        mInitialHitTestResult->SetType(IWebViewHitTestResult::SRC_ANCHOR_TYPE);
        mInitialHitTestResult->SetExtra(url);
        return;
    }
    //try {
        mInitialHitTestResult->SetExtra(substr/*URLDecoder::Decode(substr, "UTF-8")*/);
    //} catch (Throwable e) {
    //    Log.w(LOGTAG, "Failed to decode URL! " + substr, e);
    //    mInitialHitTestResult.setType(HitTestResult.UNKNOWN_TYPE);
    //}
}

void CWebViewClassic::SetHitTestResult(
    /* [in] */ CWebViewCoreWebKitHitTest* hit)
{
    if (hit == NULL) {
        mInitialHitTestResult = NULL;
        return;
    }
    mInitialHitTestResult = NULL;
    CWebViewHitTestResult::New((IWebViewHitTestResult**)&mInitialHitTestResult);
    if (!hit->mLinkUrl.IsNull()) {
        SetHitTestTypeFromUrl(hit->mLinkUrl);
        Int32 type;
        if (!hit->mImageUrl.IsNull()
                && (mInitialHitTestResult->GetType(&type), type == IWebViewHitTestResult::SRC_ANCHOR_TYPE)) {
            mInitialHitTestResult->SetType(IWebViewHitTestResult::SRC_IMAGE_ANCHOR_TYPE);
            mInitialHitTestResult->SetExtra(hit->mImageUrl);
        }
    }
    else if (!hit->mImageUrl.IsNull()) {
        mInitialHitTestResult->SetType(IWebViewHitTestResult::IMAGE_TYPE);
        mInitialHitTestResult->SetExtra(hit->mImageUrl);
    }
    else if (hit->mEditable) {
        mInitialHitTestResult->SetType(IWebViewHitTestResult::EDIT_TEXT_TYPE);
    }
    else if (!hit->mIntentUrl.IsNull()) {
        SetHitTestTypeFromUrl(hit->mIntentUrl);
    }
}

Boolean CWebViewClassic::ShouldDrawHighlightRect()
{
    if (mFocusedNode == NULL || mInitialHitTestResult == NULL) {
        return FALSE;
    }
    Boolean isEmpty = FALSE;
    if (mTouchHighlightRegion->IsEmpty(&isEmpty), isEmpty) {
        return FALSE;
    }
    Boolean isInTouchMode = FALSE;
    if (mFocusedNode->mHasFocus && (mWebView->IsInTouchMode(&isInTouchMode), !isInTouchMode)) {
        return mDrawCursorRing && !mFocusedNode->mEditable;
    }
    if (mFocusedNode->mHasFocus && mFocusedNode->mEditable) {
        return FALSE;
    }
    return mShowTapHighlight;
}

Boolean CWebViewClassic::ShouldAnimateTo(
    /* [in] */ CWebViewCoreWebKitHitTest* hit)
{
    // TODO: Don't be annoying or throw out the animation entirely
    return FALSE;
}

void CWebViewClassic::SetTouchHighlightRects(
    /* [in] */ CWebViewCoreWebKitHitTest* hit)
{
    AutoPtr<FocusTransitionDrawable> transition;
    if (ShouldAnimateTo(hit)) {
        transition = new FocusTransitionDrawable(this);
    }
    AutoPtr< ArrayOf<IRect*> > rects = hit != NULL ? hit->mTouchRects : NULL;
    Boolean isEmpty = FALSE;
    if (mTouchHighlightRegion->IsEmpty(&isEmpty), !isEmpty) {
        AutoPtr<IRect> bounds;
        mTouchHighlightRegion->GetBounds((IRect**)&bounds);
        mWebView->Invalidate(bounds);
        if (transition != NULL) {
            AutoPtr<IRegion> r;
            CRegion::New(mTouchHighlightRegion, (IRegion**)&r);
            transition->mPreviousRegion = r;
        }
        mTouchHighlightRegion->SetEmpty();
    }
    if (rects != NULL) {
        mTouchHightlightPaint->SetColor(hit->mTapHighlightColor);
        for (Int32 i = 0; i < rects->GetLength(); i++) {
            AutoPtr<IRect> rect = (*rects)[i];
            AutoPtr<IRect> viewRect = ContentToViewRect(rect);
            // some sites, like stories in nytimes.com, set
            // mouse event handler in the top div. It is not
            // user friendly to highlight the div if it covers
            // more than half of the screen.
            Int32 width, height;
            viewRect->GetWidth(&width);
            viewRect->GetHeight(&height);
            if (width < GetWidth() >> 1
                    || height < GetHeight() >> 1) {
                Boolean result;
                mTouchHighlightRegion->Union(viewRect, &result);
            }
            else if (DebugFlags::WEB_VIEW) {
                String info;
                viewRect->ToString(&info);
                Logger::D(LOGTAG, "Skip the huge selection rect:%s", info.string());
            }
        }
        AutoPtr<IRect> bounds;
        mTouchHighlightRegion->GetBounds((IRect**)&bounds);
        mWebView->Invalidate(bounds);
        if (transition != NULL && transition->mPreviousRegion != NULL) {
            AutoPtr<IRegion> r;
            CRegion::New(mTouchHighlightRegion, (IRegion**)&r);
            transition->mNewRegion = r;
            mFocusTransition = transition;
            AutoPtr<IObjectAnimatorHelper> helper;
            CObjectAnimatorHelper::AcquireSingleton((IObjectAnimatorHelper**)&helper);
            AutoPtr< ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
            (*values)[0] = 0;
            AutoPtr<IObjectAnimator> animator;
            helper->OfFloat(mFocusTransition.Get(), String("progress"), values, (IObjectAnimator**)&animator);
            animator->Start();
        }
    }
}

/** Called by JNI when pages are swapped (only occurs with hardware
 * acceleration) */
void CWebViewClassic::PageSwapCallback(
    /* [in] */ Boolean notifyAnimationStarted)
{
    if (DebugFlags::MEASURE_PAGE_SWAP_FPS) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 now;
        system->GetCurrentTimeMillis(&now);
        Int64 diff = now - mLastSwapTime;
        mAverageSwapFps = ((1000.0 / diff) + mAverageSwapFps) / 2;
        Logger::D(LOGTAG, "page swap fps:%llf ", mAverageSwapFps);
        mLastSwapTime = now;
    }
    mWebViewCore->ResumeWebKitDraw();
    if (notifyAnimationStarted) {
        mWebViewCore->SendMessage(CWebViewCore::EventHub::NOTIFY_ANIMATION_STARTED);
    }
    if (IWebViewClassicPageSwapDelegate::Probe(mWebView) != NULL) {
        // This provides a hook for ProfiledWebView to observe the tile page swaps.
        IWebViewClassicPageSwapDelegate::Probe(mWebView)->OnPageSwapOccurred(notifyAnimationStarted);
    }
    if (mPictureListener != NULL) {
        // trigger picture listener for hardware layers. Software layers are
        // triggered in setNewPicture
        AutoPtr<IWebView> v;
        GetWebView((IWebView**)&v);
        AutoPtr<IPicture> p;
        CapturePicture((IPicture**)&p);
        mPictureListener->OnNewPicture(v, p);
    }
}

ECode CWebViewClassic::SetNewPicture(
    /* [in] */ CWebViewCoreDrawData* draw,
    /* [in] */ Boolean updateBaseLayer)
{
    if (mNativeClass == 0) {
        if (mDelaySetPicture != NULL) {
            //throw new IllegalStateException("Tried to setNewPicture with"
            //        + " a delay picture already set! (memory leak)");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        // Not initialized yet, delay set
        mDelaySetPicture = draw;
        return NOERROR;
    }
    AutoPtr<CWebViewCoreViewState> viewState = draw->mViewState;
    Boolean isPictureAfterFirstLayout = viewState != NULL;

    if (updateBaseLayer) {
        AutoPtr<IWebSettings> settings;
        GetSettings((IWebSettings**)&settings);
        Boolean result;
        IWebSettingsClassic::Probe(settings)->GetShowVisualIndicator(&result);
        SetBaseLayer(draw->mBaseLayer,
                result, isPictureAfterFirstLayout);
    }
    AutoPtr<IPoint> viewSize = draw->mViewSize;
    // We update the layout (i.e. request a layout from the
    // view system) if the last view size that we sent to
    // WebCore matches the view size of the picture we just
    // received in the fixed dimension.
    Int32 pointX, pointY;
    viewSize->GetX(&pointX);
    viewSize->GetY(&pointY);
    Boolean updateLayout = pointX == mLastWidthSent
            && pointY == mLastHeightSent;
    // Don't send scroll event for picture coming from webkit,
    // since the new picture may cause a scroll event to override
    // the saved history scroll position.
    mSendScrollEvent = FALSE;
    Int32 pX, pY;
    draw->mContentSize->GetX(&pX);
    draw->mContentSize->GetY(&pY);
    RecordNewContentSize(pX, pY, updateLayout);
    if (isPictureAfterFirstLayout) {
        // Reset the last sent data here since dealing with new page.
        mLastWidthSent = 0;
        mZoomManager->OnFirstLayout(draw);
        Int32 contentWidth;
        Int32 scrollX = viewState->mShouldStartScrolledRight
                ? (GetContentWidth(&contentWidth), contentWidth) : viewState->mScrollX;
        Int32 scrollY = viewState->mScrollY;
        ContentScrollTo(scrollX, scrollY, FALSE);
        if (!mDrawHistory) {
            // As we are on a new page, hide the keyboard
            HideSoftKeyboard();
        }
    }
    mSendScrollEvent = TRUE;

    Int32 functor = 0;
    Boolean forceInval = isPictureAfterFirstLayout;
    Elastos::Droid::View::View* view = reinterpret_cast<Elastos::Droid::View::View*>(mWebView->Probe(EIID_View));
    AutoPtr<ViewRootImpl> viewRoot = view->GetViewRootImpl();
    Boolean accelerated = FALSE;
    Int32 type;
    if ((mWebView->IsHardwareAccelerated(&accelerated), accelerated)
            && (mWebView->GetLayerType(&type), type != IView::LAYER_TYPE_SOFTWARE)
            && viewRoot != NULL) {
        functor = NativeGetDrawGLFunction(mNativeClass);
        if (functor != 0) {
            // force an invalidate if functor attach not successful
            forceInval |= !viewRoot->AttachFunctor(functor);
        }
    }

    Int32 layerType;
    if (functor == 0
            || forceInval
            || (mWebView->GetLayerType(&layerType), layerType != IView::LAYER_TYPE_NONE)) {
        // invalidate the screen so that the next repaint will show new content
        // TODO: partial invalidate
        mWebView->Invalidate();
    }

    // update the zoom information based on the new picture
    if (mZoomManager->OnNewPicture(draw)) {
        Invalidate();
    }

    if (isPictureAfterFirstLayout) {
        mViewManager->PostReadyToDrawAll();
    }
    ScrollEditWithCursor();

    if (mPictureListener != NULL) {
        Boolean accelerated = FALSE;
        if ((mWebView->IsHardwareAccelerated(&accelerated), !accelerated)
                || (mWebView->GetLayerType(&layerType), layerType == IView::LAYER_TYPE_SOFTWARE)) {
            // trigger picture listener for software layers. Hardware layers are
            // triggered in pageSwapCallback
            AutoPtr<IWebView> v;
            GetWebView((IWebView**)&v);
            AutoPtr<IPicture> p;
            CapturePicture((IPicture**)&p);
            mPictureListener->OnNewPicture(v, p);
        }
    }
    return NOERROR;
}

/**
 * Used when receiving messages for REQUEST_KEYBOARD_WITH_SELECTION_MSG_ID
 * and UPDATE_TEXT_SELECTION_MSG_ID.
 */
void CWebViewClassic::UpdateTextSelectionFromMessage(
    /* [in] */ Int32 nodePointer,
    /* [in] */ Int32 textGeneration,
    /* [in] */ CWebViewCoreTextSelectionData* data)
{
    if (textGeneration == mTextGeneration) {
        if (mInputConnection != NULL && mFieldPointer == nodePointer) {
            Boolean result;
            mInputConnection->SetSelection(data->mStart, data->mEnd, &result);
        }
    }
    NativeSetTextSelection(mNativeClass, data->mSelectTextPtr);

    if ((data->mSelectionReason == CWebViewCoreTextSelectionData::REASON_ACCESSIBILITY_INJECTOR)
            || (!mSelectingText && data->mStart != data->mEnd
                    && data->mSelectionReason != CWebViewCoreTextSelectionData::REASON_SELECT_WORD)) {
        SelectionDone();
        mShowTextSelectionExtra = TRUE;
        Invalidate();
        return;
    }

    if (data->mSelectTextPtr != 0 &&
            (data->mStart != data->mEnd ||
            (mFieldPointer == nodePointer && mFieldPointer != 0) ||
            (nodePointer == 0 && data->mStart == 0 && data->mEnd == 0))) {
        mIsEditingText = (mFieldPointer == nodePointer) && nodePointer != 0;
        mIsCaretSelection = (data->mStart == data->mEnd && nodePointer != 0);
        AutoPtr<IEditable> editable;
        Int32 len;
        if (mIsCaretSelection &&
                (mInputConnection == NULL ||
                (mInputConnection->GetEditable((IEditable**)&editable), editable->GetLength(&len), len == 0))) {
            // There's no text, don't show caret handle.
            SelectionDone();
        }
        else {
            if (!mSelectingText) {
                SetupWebkitSelect();
            }
            else {
                SyncSelectionCursors();
            }
            AnimateHandles();
            if (mIsCaretSelection) {
                ResetCaretTimer();
            }
        }
    }
    else {
        SelectionDone();
    }
    Invalidate();
}

void CWebViewClassic::ScrollEditText(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY)
{
    // Scrollable edit text. Scroll it.
    Float maxScrollX = GetMaxTextScrollX();
    Float scrollPercentX = ((Float)scrollX)/maxScrollX;
    mEditTextContent->OffsetTo(-scrollX, -scrollY);
    mWebViewCore->RemoveMessages(CWebViewCore::EventHub::SCROLL_TEXT_INPUT);
    AutoPtr<IFloat> scrollPercentXObj;
    CFloat::New(scrollPercentX, (IFloat**)&scrollPercentXObj);
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SCROLL_TEXT_INPUT, 0,
            scrollY, scrollPercentXObj);
    AnimateHandles();
}

void CWebViewClassic::BeginTextBatch()
{
    mIsBatchingTextChanges = TRUE;
}

void CWebViewClassic::CommitTextBatch()
{
    if (mWebViewCore != NULL) {
        mWebViewCore->SendMessages(mBatchedTextChanges);
    }
    mBatchedTextChanges.Clear();
    mIsBatchingTextChanges = FALSE;
}

void CWebViewClassic::SendBatchableInputMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    if (mWebViewCore == NULL) {
        return;
    }
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> message;
    helper->Obtain(NULL, what, arg1, arg2, obj, (IMessage**)&message);
    if (mIsBatchingTextChanges) {
        mBatchedTextChanges.PushBack(message);
    }
    else {
        mWebViewCore->SendMessage(message);
    }
}

/*
 * Request a dropdown menu for a listbox with multiple selection.
 *
 * @param array Labels for the listbox.
 * @param enabledArray  State for each element in the list.  See static
 *      integers in Container class.
 * @param selectedArray Which positions are initally selected.
 */
void CWebViewClassic::RequestListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ ArrayOf<Int32>* selectedArray)
{
    AutoPtr<IRunnable> r = new InvokeListBox(array, enabledArray, selectedArray, this);
    Boolean result;
    mPrivateHandler->Post(r, &result);
}

/*
 * Request a dropdown menu for a listbox with single selection or a single
 * <select> element.
 *
 * @param array Labels for the listbox.
 * @param enabledArray  State for each element in the list.  See static
 *      integers in Container class.
 * @param selection Which position is initally selected.
 */
void CWebViewClassic::RequestListBox(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ ArrayOf<Int32>* enabledArray,
    /* [in] */ Int32 selection)
{
    AutoPtr<IRunnable> r = new InvokeListBox(array, enabledArray, selection, this);
    Boolean result;
    mPrivateHandler->Post(r, &result);
}

Int32 CWebViewClassic::GetScaledMaxXScroll()
{
    Int32 width;
    if (mHeightCanMeasure == FALSE) {
        width = GetViewWidth() / 4;
    }
    else {
        AutoPtr<IRect> visRect;
        CRect::New((IRect**)&visRect);
        CalcOurVisibleRect(visRect);
        Int32 w;
        visRect->GetWidth(&w);
        width = w / 2;
    }
    // FIXME the divisor should be retrieved from somewhere
    return ViewToContentX(width);
}

Int32 CWebViewClassic::GetScaledMaxYScroll()
{
    Int32 height;
    if (mHeightCanMeasure == FALSE) {
        height = GetViewHeight() / 4;
    }
    else {
        AutoPtr<IRect> visRect;
        CRect::New((IRect**)&visRect);
        CalcOurVisibleRect(visRect);
        Int32 h;
        visRect->GetHeight(&h);
        height = h / 2;
    }
    // FIXME the divisor should be retrieved from somewhere
    // the closest thing today is hard-coded into ScrollView.java
    // (from ScrollView.java, line 363)   int maxJump = height/2;
    return Elastos::Core::Math::Round(height * mZoomManager->GetInvScale());
}

/**
 * Called by JNI to invalidate view
 */
void CWebViewClassic::ViewInvalidate()
{
    Invalidate();
}

/**
 * Pass the key directly to the page.  This assumes that
 * nativePageShouldHandleShiftAndArrows() returned true.
 */
void CWebViewClassic::LetPageHandleNavKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int64 time,
    /* [in] */ Boolean down,
    /* [in] */ Int32 metaState)
{
    Int32 keyEventAction;
    if (down) {
        keyEventAction = IKeyEvent::ACTION_DOWN;
    }
    else {
        keyEventAction = IKeyEvent::ACTION_UP;
    }

    AutoPtr<IKeyEvent> event;
    CKeyEvent::New(time, time, keyEventAction, keyCode,
            1, (metaState & IKeyEvent::META_SHIFT_ON)
            | (metaState & IKeyEvent::META_ALT_ON)
            | (metaState & IKeyEvent::META_SYM_ON)
            , IKeyCharacterMap::VIRTUAL_KEYBOARD, 0, 0, (IKeyEvent**)&event);
    SendKeyEvent(event);
}

void CWebViewClassic::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 direction = 0;
    Int32 code;
    event->GetKeyCode(&code);
    switch (code) {
    case IKeyEvent::KEYCODE_DPAD_DOWN:
        direction = IView::FOCUS_DOWN;
        break;
    case IKeyEvent::KEYCODE_DPAD_UP:
        direction = IView::FOCUS_UP;
        break;
    case IKeyEvent::KEYCODE_DPAD_LEFT:
        direction = IView::FOCUS_LEFT;
        break;
    case IKeyEvent::KEYCODE_DPAD_RIGHT:
        direction = IView::FOCUS_RIGHT;
        break;
    case IKeyEvent::KEYCODE_TAB:
        Boolean pressed = FALSE;
        event->IsShiftPressed(&pressed);
        direction = pressed ? IView::FOCUS_BACKWARD : IView::FOCUS_FORWARD;
        break;
    }
    AutoPtr<IView> view;
    if (direction != 0 && (mWebView->FocusSearch(direction, (IView**)&view), view == NULL)) {
        // Can't take focus in that direction
        direction = 0;
    }
    Int32 eventHubAction = CWebViewCore::EventHub::KEY_UP;
    Int32 action;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_DOWN) {
        eventHubAction = CWebViewCore::EventHub::KEY_DOWN;
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        Int32 sound = KeyCodeToSoundsEffect(keyCode);
        if (sound != 0) {
            mWebView->PlaySoundEffect(sound);
        }
    }
    SendBatchableInputMessage(eventHubAction, direction, 0, event);
}

/**
 * See {@link WebView#setBackgroundColor(int)}
 */
ECode CWebViewClassic::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    mBackgroundColor = color;
    mWebViewCore->SendMessage(CWebViewCore::EventHub::SET_BACKGROUND_COLOR, color);
    return NOERROR;
}

/**
 * Enable the communication b/t the webView and VideoViewProxy
 *
 * only used by the Browser
 */
ECode CWebViewClassic::SetHTML5VideoViewProxy(
    /* [in] */ HTML5VideoViewProxy* proxy)
{
    mHTML5VideoViewProxy = proxy;
    return NOERROR;
}

/**
 * Set the time to wait between passing touches to WebCore. See also the
 * TOUCH_SENT_INTERVAL member for further discussion.
 *
 * This is only used by the DRT test application.
 */
ECode CWebViewClassic::SetTouchInterval(
    /* [in] */ Int32 interval)
{
    mCurrentTouchInterval = interval;
    return NOERROR;
}

/**
 * Copy text into the clipboard. This is called indirectly from
 * WebViewCore.
 * @param text The text to put into the clipboard.
 */
void CWebViewClassic::CopyToClipboard(
    /* [in] */ const String& text)
{
    AutoPtr<IClipboardManager> cm;
    mContext->GetSystemService(IContext::CLIPBOARD_SERVICE, (IInterface**)&cm);
    AutoPtr<IClipDataHelper> helper;
    CClipDataHelper::AcquireSingleton((IClipDataHelper**)&helper);
    String title;
    GetTitle(&title);
    AutoPtr<ICharSequence> titleCs, textCs;
    CStringWrapper::New(title, (ICharSequence**)&titleCs);
    CStringWrapper::New(text, (ICharSequence**)&textCs);
    AutoPtr<IClipData> clip;
    helper->NewPlainText(titleCs, textCs, (IClipData**)&clip);
    cm->SetPrimaryClip(clip);
}

void CWebViewClassic::AutoFillForm(
    /* [in] */ Int32 autoFillQueryId)
{
    AutoPtr<IMessageHelper> helper;
    CMessageHelper::AcquireSingleton((IMessageHelper**)&helper);
    AutoPtr<IMessage> msg;
    helper->Obtain(mPrivateHandler, AUTOFILL_FORM, autoFillQueryId, 0, (IMessage**)&msg);
    msg->SendToTarget();
}

AutoPtr<ViewManager> CWebViewClassic::GetViewManager()
{
    return mViewManager;
}

/** send content invalidate */
void CWebViewClassic::ContentInvalidateAll()
{
    if (mWebViewCore != NULL && !mBlockWebkitViewMessages) {
        mWebViewCore->SendMessage(CWebViewCore::EventHub::CONTENT_INVALIDATE_ALL);
    }
}

/** discard all textures from tiles. Used in Profiled WebView */
ECode CWebViewClassic::DiscardAllTextures()
{
    NativeDiscardAllTextures();
    return NOERROR;
}

ECode CWebViewClassic::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint* paint)
{
    UpdateHwAccelerated();
    return NOERROR;
}

void CWebViewClassic::UpdateHwAccelerated()
{
    if (mNativeClass == 0) {
        return;
    }
    Boolean hwAccelerated = FALSE;
    Boolean accelerated = FALSE;
    Int32 type;
    if ((mWebView->IsHardwareAccelerated(&accelerated), accelerated) &&
        (mWebView->GetLayerType(&type), type != IView::LAYER_TYPE_SOFTWARE)) {
        hwAccelerated = TRUE;
    }

    // result is of type LayerAndroid::InvalidateFlags, non zero means invalidate/redraw
    Int32 result = NativeSetHwAccelerated(mNativeClass, hwAccelerated);
    if (mWebViewCore != NULL && !mBlockWebkitViewMessages && result != 0) {
        mWebViewCore->ContentDraw();
    }
}

/**
 * Begin collecting per-tile profiling data
 *
 * only used by profiling tests
 */
ECode CWebViewClassic::TileProfilingStart()
{
    NativeTileProfilingStart();
    return NOERROR;
}

/**
 * Return per-tile profiling data
 *
 * only used by profiling tests
 */
ECode CWebViewClassic::TileProfilingStop(
    /* [out] */ Float* stop)
{
    VALIDATE_NOT_NULL(stop);
    *stop = NativeTileProfilingStop();
    return NOERROR;
}

/** only used by profiling tests */
ECode CWebViewClassic::TileProfilingClear()
{
    NativeTileProfilingClear();
    return NOERROR;
}

/** only used by profiling tests */
ECode CWebViewClassic::TileProfilingNumFrames(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    *num = NativeTileProfilingNumFrames();
    return NOERROR;
}

/** only used by profiling tests */
ECode CWebViewClassic::TileProfilingNumTilesInFrame(
    /* [in] */ Int32 frame,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    *num = NativeTileProfilingNumTilesInFrame(frame);
    return NOERROR;
}

/** only used by profiling tests */
ECode CWebViewClassic::TileProfilingGetInt(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 tile,
    /* [in] */ const String& key,
    /* [out] */ Int32* Int)
{
    VALIDATE_NOT_NULL(Int);
    *Int = NativeTileProfilingGetInt(frame, tile, key);
    return NOERROR;
}

/** only used by profiling tests */
ECode CWebViewClassic::TileProfilingGetFloat(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 tile,
    /* [in] */ const String& key,
    /* [out] */ Float* gileFloat)
{
    VALIDATE_NOT_NULL(gileFloat);
    *gileFloat = NativeTileProfilingGetFloat(frame, tile, key);
    return NOERROR;
}

/**
 * Checks the focused content for an editable text field. This can be
 * text input or ContentEditable.
 * @return true if the focused item is an editable text field.
 */
Boolean CWebViewClassic::FocusCandidateIsEditableText()
{
    if (mFocusedNode != NULL) {
        return mFocusedNode->mEditable;
    }
    return FALSE;
}

// Called via JNI
void CWebViewClassic::PostInvalidate()
{
    mWebView->PostInvalidate();
}

// Note: must be called before first WebViewClassic is created.
void CWebViewClassic::SetShouldMonitorWebCoreThread()
{
    CWebViewCore::SetShouldMonitorWebCoreThread();
}

ECode CWebViewClassic::DumpViewHierarchyWithProperties(
    /* [in] */ IBufferedWriter* out,
    /* [in] */ Int32 level)
{
    Int32 layer = GetBaseLayer();
    if (layer != 0) {
        //try {
        AutoPtr<IByteArrayOutputStream> stream;
        CByteArrayOutputStream::New((IByteArrayOutputStream**)&stream);
        ViewStateSerializer::DumpLayerHierarchy(layer, stream, level);
        stream->Close();
        AutoPtr< ArrayOf<Byte> > buf;
        stream->ToByteArray((ArrayOf<Byte>**)&buf);
        String str((const char*)buf->GetPayload());
        out->WriteString(str);
        //} catch (IOException e) {}
    }
    return NOERROR;
}

ECode CWebViewClassic::FindHierarchyView(
    /* [in] */ const String& className,
    /* [in] */ Int32 hashCode,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    if (mNativeClass == 0) {
       *view = NULL;
       return NOERROR;
    }
    AutoPtr<IPicture> pic;
    CPicture::New((IPicture**)&pic);
    if (!NativeDumpLayerContentToPicture(mNativeClass, className, hashCode, pic)) {
        *view = NULL;
        return NOERROR;
    }
    AutoPtr<PictureWrapperView> pv = new PictureWrapperView(GetContext(), pic, mWebView);
    *view = (IView*)pv.Get();
    REFCOUNT_ADD(*view);
    return NOERROR;
}

void CWebViewClassic::NativeCreate(
    /* [in] */ Int32 viewImpl,
    /* [in] */ const String& drawableDir,
    /* [in] */ Boolean isHighEndGfx)
{
    IWeakReferenceSource* source = IWeakReferenceSource::Probe((IWeakReferenceSource *)this);
    AutoPtr<IWeakReference> wr;
    source->GetWeakReference((IWeakReference**)&wr);
    Elastos_WebView_nativeCreate((Int32)&CWebViewClassic::sElaWebViewCallback, wr, viewImpl, drawableDir, isHighEndGfx);
}

void CWebViewClassic::NativeDebugDump()
{}

void CWebViewClassic::NativeDestroy(
    /* [in] */ Int32 ptr)
{
    Elastos_WebView_nativeDestroy(ptr);
}

void CWebViewClassic::NativeDraw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* visible,
    /* [in] */ Int32 color,
    /* [in] */ Int32 extras)
{
    Handle32 native;
    canvas->GetNativeCanvas(&native);
    Int32 nativeCanvas = (Int32)native;

    Float l, t, r, b;
    visible->GetLeft(&l);
    visible->GetTop(&t);
    visible->GetRight(&r);
    visible->GetBottom(&b);
    Elastos_WebView_nativeDraw(mNativeClass, nativeCanvas,
            l, t, r, b, color, extras);
}

void CWebViewClassic::NativeDumpDisplayTree(
    /* [in] */ const String& jurl)
{
    Elastos_WebView_nativeDumpDisplayTree(mNativeClass, jurl);
}

Boolean CWebViewClassic::NativeEvaluateLayersAnimations(
    /* [in] */ Int32 nativeView)
{
    return Elastos_WebView_nativeEvaluateLayersAnimations(nativeView);
}

Int32 CWebViewClassic::NativeCreateDrawGLFunction(
    /* [in] */ Int32 nativeView,
    /* [in] */ IRect* jinvscreenrect,
    /* [in] */ IRect* jscreenrect,
    /* [in] */ IRectF* jvisiblecontentrect,
    /* [in] */ Float scale,
    /* [in] */ Int32 extras)
{
    Int32 L, T, R, B;
    Int32 invscreenrect[4];
    Int32 screenrect[4];
    Int32 *pinvscreenrect = NULL;
    Int32 *pscreenrect = NULL;

    if (jinvscreenrect)
    {
        jinvscreenrect->Get(&L, &T, &R, &B);
        invscreenrect[0] = L;
        invscreenrect[1] = T;
        invscreenrect[2] = R;
        invscreenrect[3] = B;
        pinvscreenrect = invscreenrect;
    }
    if (jscreenrect)
    {
        jscreenrect->Get(&L, &T, &R, &B);
        screenrect[0] = L;
        screenrect[1] = T;
        screenrect[2] = R;
        screenrect[3] = B;
        pscreenrect = screenrect;
    }
    Float l, t, r, b;
    jvisiblecontentrect->GetLeft(&l);
    jvisiblecontentrect->GetTop(&t);
    jvisiblecontentrect->GetRight(&r);
    jvisiblecontentrect->GetBottom(&b);
    Float visiblecontentrect[4] = { l, t, r, b };
    return Elastos_WebView_nativeCreateDrawGLFunction(nativeView, pinvscreenrect, pscreenrect, visiblecontentrect, scale, extras);
}

Int32 CWebViewClassic::NativeGetDrawGLFunction(
    /* [in] */ Int32 nativeView)
{
    return Elastos_WebView_nativeGetDrawGLFunction(nativeView);
}

void CWebViewClassic::NativeUpdateDrawGLFunction(
    /* [in] */ Int32 nativeView,
    /* [in] */ IRect* jinvscreenrect,
    /* [in] */ IRect* jscreenrect,
    /* [in] */ IRectF* jvisiblecontentrect,
    /* [in] */ Float scale)
{
    Int32 L, T, R, B;
    Int32 invscreenrect[4];
    Int32 screenrect[4];
    Int32 *pinvscreenrect = NULL;
    Int32 *pscreenrect = NULL;

    if (jinvscreenrect)
    {
        jinvscreenrect->Get(&L, &T, &R, &B);
        invscreenrect[0] = L;
        invscreenrect[1] = T;
        invscreenrect[2] = R;
        invscreenrect[3] = B;
        pinvscreenrect = invscreenrect;
    }
    if (jscreenrect)
    {
        jscreenrect->Get(&L, &T, &R, &B);
        screenrect[0] = L;
        screenrect[1] = T;
        screenrect[2] = R;
        screenrect[3] = B;
        pscreenrect = screenrect;
    }

    Float l, t, r, b;
    jvisiblecontentrect->GetLeft(&l);
    jvisiblecontentrect->GetTop(&t);
    jvisiblecontentrect->GetRight(&r);
    jvisiblecontentrect->GetBottom(&b);
    Float visiblecontentrect[4] = {l, t, r, b};
    Elastos_WebView_nativeUpdateDrawGLFunction(nativeView, pinvscreenrect, pscreenrect, visiblecontentrect, scale);
}

String CWebViewClassic::NativeGetSelection()
{
    return Elastos_WebView_nativeGetSelection(mNativeClass);
}

void CWebViewClassic::NativeSetHeightCanMeasure(
    /* [in] */ Boolean measure)
{
    Elastos_WebView_nativeSetHeightCanMeasure(mNativeClass, measure);
}

Boolean CWebViewClassic::NativeSetBaseLayer(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 layer,
    /* [in] */ Boolean showVisualIndicator,
    /* [in] */ Boolean isPictureAfterFirstLayout,
    /* [in] */ Int32 scrollingLayer)
{
    return Elastos_WebView_nativeSetBaseLayer(nativeView, layer, showVisualIndicator, isPictureAfterFirstLayout, scrollingLayer);
}

Int32 CWebViewClassic::NativeGetBaseLayer(
    /* [in] */ Int32 nativeView)
{
    return Elastos_WebView_nativeGetBaseLayer(nativeView);
}

void CWebViewClassic::NativeCopyBaseContentToPicture(
    /* [in] */ IPicture* picture)
{
    Handle32 native;
    picture->GetNativePicture(&native);
    Int32 nativePicture = (Int32)native;
    Elastos_WebView_nativeCopyBaseContentToPicture(mNativeClass, nativePicture);
}

Boolean CWebViewClassic::NativeDumpLayerContentToPicture(
    /* [in] */ Int32 instance,
    /* [in] */ const String& jclassName,
    /* [in] */ Int32 layerId,
    /* [in] */ IPicture* pict)
{
    Handle32 native;
    pict->GetNativePicture(&native);
    Int32 picture = (Int32)native;
    return Elastos_WebView_nativeDumpLayerContentToPicture(instance, jclassName, layerId, picture);
}

Boolean CWebViewClassic::NativeHasContent()
{
    return Elastos_WebView_nativeHasContent(mNativeClass);
}

void CWebViewClassic::NativeStopGL(
    /* [in] */ Int32 ptr)
{
    Elastos_WebView_nativeStopGL(ptr);
}

void CWebViewClassic::NativeDiscardAllTextures()
{
    Elastos_WebView_nativeDiscardAllTextures();
}

void CWebViewClassic::NativeTileProfilingStart()
{
    Elastos_WebView_nativeTileProfilingStart();
}

Float CWebViewClassic::NativeTileProfilingStop()
{
    return Elastos_WebView_nativeTileProfilingStop();
}

void CWebViewClassic::NativeTileProfilingClear()
{
    Elastos_WebView_nativeTileProfilingClear();
}

Int32 CWebViewClassic::NativeTileProfilingNumFrames()
{
    return Elastos_WebView_nativeTileProfilingNumFrames();
}

Int32 CWebViewClassic::NativeTileProfilingNumTilesInFrame(
    /* [in] */ Int32 frame)
{
    return Elastos_WebView_nativeTileProfilingNumTilesInFrame(frame);
}

Int32 CWebViewClassic::NativeTileProfilingGetInt(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 tile,
    /* [in] */ const String& jkey)
{
    return Elastos_WebView_nativeTileProfilingGetInt(frame, tile, jkey);
}

Float CWebViewClassic::NativeTileProfilingGetFloat(
    /* [in] */ Int32 frame,
    /* [in] */ Int32 tile,
    /* [in] */ const String& jkey)
{
    return Elastos_WebView_nativeTileProfilingGetFloat(frame, tile, jkey);
}

void CWebViewClassic::NativeUseHardwareAccelSkia(
    /* [in] */ Boolean enabled)
{
    Elastos_WebView_nativeUseHardwareAccelSkia(enabled);
}

// Returns a pointer to the scrollable LayerAndroid at the given point.
Int32 CWebViewClassic::NativeScrollableLayer(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IRect* rect,
    /* [in] */ IRect* bounds)
{
    Int32 pRect[4];
    Int32 pBounds[4];
    Int32 result = Elastos_WebView_nativeScrollableLayer(nativeView, x, y, pRect, pBounds);
    rect->Set(pRect[0],pRect[1],pRect[2],pRect[3]);
    bounds->Set(pBounds[0],pBounds[1], pBounds[2], pBounds[3]);
    return result;
}

/**
 * Scroll the specified layer.
 * @param nativeInstance Native WebView instance
 * @param layer Id of the layer to scroll, as determined by nativeScrollableLayer.
 * @param newX Destination x position to which to scroll.
 * @param newY Destination y position to which to scroll.
 * @return True if the layer is successfully scrolled.
 */
Boolean CWebViewClassic::NativeScrollLayer(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 layerId,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return Elastos_WebView_nativeScrollLayer(nativeView, layerId, x, y);
}

void CWebViewClassic::NativeSetIsScrolling(
    /* [in] */ Boolean isScrolling)
{
    Elastos_WebView_nativeSetIsScrolling(mNativeClass, isScrolling);
}

Int32 CWebViewClassic::NativeGetBackgroundColor(
    /* [in] */ Int32 nativeView)
{
    return Elastos_WebView_nativeGetBackgroundColor(nativeView);
}

Boolean CWebViewClassic::NativeSetProperty(
    /* [in] */ const String& jkey,
    /* [in] */ const String& jvalue)
{
    return Elastos_WebView_nativeSetProperty(jkey, jvalue);
}

String CWebViewClassic::NativeGetProperty(
    /* [in] */ const String& jkey)
{
    return Elastos_WebView_nativeGetProperty(jkey);
}

/**
 * See {@link ComponentCallbacks2} for the trim levels and descriptions
 */
void CWebViewClassic::NativeOnTrimMemory(
    /* [in] */ Int32 level)
{
    Elastos_WebView_nativeOnTrimMemory(level);
}

void CWebViewClassic::NativeSetPauseDrawing(
    /* [in] */ Int32 nativeView,
    /* [in] */ Boolean pause)
{
    Elastos_WebView_nativeSetPauseDrawing(nativeView, pause);
}

void CWebViewClassic::NativeSetTextSelection(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 selectionPtr)
{
    Elastos_WebView_nativeSetTextSelection(nativeView, selectionPtr);
}

Int32 CWebViewClassic::NativeGetHandleLayerId(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 handleIndex,
    /* [in] */ IPoint* cursorPoint,
    /* [in] */ QuadF* textQuad)
{
    Int32 pCursorPoint[2];
    Int32 result = Elastos_WebView_nativeGetHandleLayerId(nativeView, handleIndex, pCursorPoint, textQuad);
    cursorPoint->Set(pCursorPoint[0], pCursorPoint[1]);
    return result;
}

void CWebViewClassic::NativeMapLayerRect(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 layerId,
    /* [in] */ IRect* rect)
{
    Int32 pRect[4];
    Elastos_WebView_nativeMapLayerRect(nativeView, layerId, pRect);
    rect->Set(pRect[0], pRect[1], pRect[2], pRect[3]);
}

// Returns 1 if a layer sync is needed, else 0
Int32 CWebViewClassic::NativeSetHwAccelerated(
    /* [in] */ Int32 nativeView,
    /* [in] */ Boolean hwAccelerated)
{
    return Elastos_WebView_nativeSetHwAccelerated(nativeView, hwAccelerated);
}

void CWebViewClassic::NativeFindMaxVisibleRect(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 movingLayerId,
    /* [in] */ IRect* visibleContentRect)
{
    Int32 pVisibleContentRect[4];
    Elastos_WebView_nativeFindMaxVisibleRect(nativeView, movingLayerId, pVisibleContentRect);
    visibleContentRect->Set(pVisibleContentRect[0], pVisibleContentRect[1], pVisibleContentRect[2], pVisibleContentRect[3]);
}

Boolean CWebViewClassic::NativeIsHandleLeft(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 handleId)
{
    return Elastos_WebView_nativeIsHandleLeft(nativeView, handleId);
}

Boolean CWebViewClassic::NativeIsPointVisible(
    /* [in] */ Int32 nativeView,
    /* [in] */ Int32 layerId,
    /* [in] */ Int32 contentX,
    /* [in] */ Int32 contentY)
{
    return Elastos_WebView_nativeIsPointVisible(nativeView, layerId, contentX, contentY);
}

void CWebViewClassic::SetNativeClass(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 nativeClass)
{
    ((CWebViewClassic*)IWebViewClassic::Probe(obj))->mNativeClass = nativeClass;
}

Int32 CWebViewClassic::GetNativeClass(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewClassic*)IWebViewClassic::Probe(obj))->mNativeClass;
}

Boolean CWebViewClassic::ScrollBy(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 cx,
    /* [in] */ Int32 cy,
    /* [in] */ Boolean animate)
{
    return ((CWebViewClassic*)IWebViewClassic::Probe(obj))->SetContentScrollBy(cx, cy, animate);
}

Int32 CWebViewClassic::GetScaledMaxXScroll(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewClassic*)IWebViewClassic::Probe(obj))->GetScaledMaxXScroll();
}

Int32 CWebViewClassic::GetScaledMaxYScroll(
    /* [in] */ IInterface* obj)
{
    return ((CWebViewClassic*)IWebViewClassic::Probe(obj))->GetScaledMaxYScroll();
}

void CWebViewClassic::UpdateRectsForGL(
    /* [in] */ IInterface* obj)
{
    ((CWebViewClassic*)IWebViewClassic::Probe(obj))->UpdateRectsForGL();
}

void CWebViewClassic::ViewInvalidate(
    /* [in] */ IInterface* obj)
{
    ((CWebViewClassic*)IWebViewClassic::Probe(obj))->ViewInvalidate();
}

void CWebViewClassic::ViewInvalidateRect(
    /* [in] */ IInterface* obj,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    ((CWebViewClassic*)IWebViewClassic::Probe(obj))->ViewInvalidate(l, t, r, b);
}

void CWebViewClassic::PostInvalidateDelayed(
    /* [in] */ IInterface* obj,
    /* [in] */ Int64 delay,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    ((CWebViewClassic*)IWebViewClassic::Probe(obj))->ViewInvalidateDelayed(delay, l, t, r, b);
}

void CWebViewClassic::PageSwapCallback(
    /* [in] */ IInterface* obj,
    /* [in] */ Boolean notifyAnimationStarted)
{
    ((CWebViewClassic*)IWebViewClassic::Probe(obj))->PageSwapCallback(notifyAnimationStarted);
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos
