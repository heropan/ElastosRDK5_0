// wuweizuo automatic build .cpp file from .java file.

#include "Clipboard.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Base {

//=====================================================================
//                              Clipboard
//=====================================================================
Clipboard::Clipboard(
    /* [in] */ const IContext* context)
{
    // ==================before translated======================
    // mContext = context;
    // mClipboardManager = (ClipboardManager)
    //         context.getSystemService(Context.CLIPBOARD_SERVICE);
}

ECode Clipboard::SetText(
    /* [in] */ const String& label,
    /* [in] */ const String& text)
{
    // ==================before translated======================
    // setPrimaryClipNoException(ClipData.newPlainText(label, text));
    assert(0);
    return NOERROR;
}

ECode Clipboard::SetText(
    /* [in] */ const String& text)
{
    // ==================before translated======================
    // setText(null, text);
    assert(0);
    return NOERROR;
}

ECode Clipboard::SetHTMLText(
    /* [in] */ const String& html,
    /* [in] */ const String& label,
    /* [in] */ const String& text)
{
    // ==================before translated======================
    // if (isHTMLClipboardSupported()) {
    //     setPrimaryClipNoException(ClipData.newHtmlText(label, text, html));
    // }
    assert(0);
    return NOERROR;
}

ECode Clipboard::SetHTMLText(
    /* [in] */ const String& html,
    /* [in] */ const String& text)
{
    // ==================before translated======================
    // setHTMLText(html, null, text);
    assert(0);
    return NOERROR;
}

AutoPtr<Clipboard> Clipboard::Create(
    /* [in] */ const IContext* context)
{
    // ==================before translated======================
    // return new Clipboard(context);
    assert(0);
    AutoPtr<Clipboard> empty;
    return empty;
}

String Clipboard::GetCoercedText()
{
    // ==================before translated======================
    // final ClipData clip = mClipboardManager.getPrimaryClip();
    // if (clip != null && clip.getItemCount() > 0) {
    //     final CharSequence sequence = clip.getItemAt(0).coerceToText(mContext);
    //     if (sequence != null) {
    //         return sequence.toString();
    //     }
    // }
    // return null;
    assert(0);
    return String("");
}

String Clipboard::GetHTMLText()
{
    // ==================before translated======================
    // if (isHTMLClipboardSupported()) {
    //     final ClipData clip = mClipboardManager.getPrimaryClip();
    //     if (clip != null && clip.getItemCount() > 0) {
    //         return clip.getItemAt(0).getHtmlText();
    //     }
    // }
    // return null;
    assert(0);
    return String("");
}

Boolean Clipboard::IsHTMLClipboardSupported()
{
    // ==================before translated======================
    // return ApiCompatibilityUtils.isHTMLClipboardSupported();
    assert(0);
    return FALSE;
}

ECode Clipboard::SetPrimaryClipNoException(
    /* [in] */ IClipData* clip)
{
    VALIDATE_NOT_NULL(clip);
    // ==================before translated======================
    // try {
    //     mClipboardManager.setPrimaryClip(clip);
    // } catch (Exception ex) {
    //     // Ignore any exceptions here as certain devices have bugs and will fail.
    //     String text = mContext.getString(R.string.copy_to_clipboard_failure_message);
    //     Toast.makeText(mContext, text, Toast.LENGTH_SHORT).show();
    // }
    assert(0);
    return NOERROR;
}

} // namespace Base
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


