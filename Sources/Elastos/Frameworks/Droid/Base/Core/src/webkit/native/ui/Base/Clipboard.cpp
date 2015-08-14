
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

Clipboard::Clipboard(
    /* in */ const IContext* context)
    : mContext(context)
{
    context->GetSystemService(IContext::CLIPBOARD_SERVICE, (IClipboardManager**)&mClipboardManager);
}

ECode Clipboard::SetText(
    /* in */ const String& label,
    /* in */ const String& text)
{
    AutoPtr<IPlainText> plainText;
    CClipData::New(label, text, (IPlainText**)&plainText);
    SetPrimaryClipNoException(plainText);
    return NOERROR;
}

//@CalledByNative
ECode Clipboard::SetText(
    /* in */ const String& text)
{
    return SetText(String(""), text);
}

ECode Clipboard::SetHTMLText(
    /* in */ const String& html,
    /* in */ const String& label,
    /* in */ const String& text)
{
    if (IsHTMLClipboardSupported()) {
        AutoPtr<IHtmlText> htmlText;
        CClipData::New(label, text, html, (IHtmlText**)&htmlText);
        SetPrimaryClipNoException(htmlText);
    }
    return NOERROR;
}

//@CalledByNative
ECode Clipboard::SetHTMLText(
    /* in */ const String& html,
    /* in */ const String& text)
{
    return setHTMLText(html, String(""), text);
}

//@CalledByNative
AutoPtr<IClipboard> Clipboard::Create(
    /* in */ const IContext* context)
{
    AutoPtr<IClipBoard> clipBoard;
    CClipBoard::New(context, (IClipBoard**)&clipBoard);
    return clipBoard;
}

//@SuppressWarnings("javadoc")
//@CalledByNative
String Clipboard::GetCoercedText()
{
    AutoPtr<IClipData> clip;
    mClipboardManager->GetPrimaryClip((IClipData**)&clip);
    Int32 itemCount;
    clip->GetItemCount(&itemCount);
    if (clip != NULL && itemCount > 0) {
        AutoPtr<IClipDataItem> clipDataItem;
        clip->GetItemAt(0, (IClipDataItem**)&clipDataItem);

        AutoPtr<ICharSequence> sequence;
        clipDataItem->CoerceToText(mContext, (ICharSequence**)&sequence);
        if (sequence != NULL) {
            String result;
            sequence->ToString(&result);
            return result;
        }
    }
    return String("");
}

//@CalledByNative
String Clipboard::GetHTMLText()
{
    if (IsHTMLClipboardSupported()) {
        AutoPtr<IClipData> clip;
        mClipboardManager->GetPrimaryClip((IClipData**)&clip);
        Int32 itemCount;
        clip->GetItemCount(&itemCount);
        if (clip != NULL && itemCount > 0) {
            AutoPtr<IClipDataItem> clipDataItem;
            clip->GetItemAt(0, (IClipDataItem**)&clipDataItem);

            AutoPtr<ICharSequence> sequence;
            clipDataItem->GetHtmlText(mContext, (ICharSequence**)&sequence);
            String result;
            sequence->ToString(&result);
c
            return result;
        }
    }
    return String("");
}

//@CalledByNative
Boolean Clipboard::IsHTMLClipboardSupported()
{
    return ApiCompatibilityUtils::IsHTMLClipboardSupported();
}

ECode Clipboard::SetPrimaryClipNoException(
    /* in */ IClipData* clip)
{
    VALIDATE_NOT_NULL(clip);
    //try {
        mClipboardManager->SetPrimaryClip(clip);
    //} catch (Exception ex) {
        // Ignore any exceptions here as certain devices have bugs and will fail.
    //    String text = mContext.getString(R.string.copy_to_clipboard_failure_message);
    //    Toast.makeText(mContext, text, Toast.LENGTH_SHORT).show();
    //}
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

