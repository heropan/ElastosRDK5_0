
#include "ext/frameworkdef.h"
#include "content/CClipDataItem.h"
#include "text/CSpannableStringBuilder.h"
#include "text/Html.h"
#include "text/style/CURLSpan.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::ICloseable;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::CInputStreamReader;
using Elastos::Droid::Text::CSpannableStringBuilder;
using Elastos::Droid::Text::ISpanned;
using Elastos::Droid::Text::ISpannableStringBuilder;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::Html;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::Style::IURLSpan;
using Elastos::Droid::Text::Style::CURLSpan;
using Elastos::Droid::Content::Res::IAssetFileDescriptor;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Content {

ECode CClipDataItem::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CClipDataItem::GetHtmlText(
    /* [out] */ String* htmlText)
{
    VALIDATE_NOT_NULL(htmlText)
    *htmlText = mHtmlText;
    return NOERROR;
}

ECode CClipDataItem::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = mIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode CClipDataItem::GetUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = mUri;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode CClipDataItem::CoerceToText(
    /* [in] */ IContext* context,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    // If this Item has an explicit textual value, simply return that.
    AutoPtr<ICharSequence> cs = mText;
    if (cs != NULL) {
        *text = cs;
        REFCOUNT_ADD(*text);
        return NOERROR;
    }

    // If this Item has a URI value, try using that.
    AutoPtr<IUri> uri = mUri;
    if (uri != NULL) {
        // First see if the URI can be opened as a plain text stream
        // (of any sub-type).  If so, this is the best textual
        // representation for it.
        AutoPtr<IFileInputStream> stream;
        // try {
        // Ask for a stream of the desired type.
        AutoPtr<IContentResolver> resolver;
        context->GetContentResolver((IContentResolver**)&resolver);
        AutoPtr<IAssetFileDescriptor> descr;
        ECode ec = resolver->OpenTypedAssetFileDescriptor(uri, String("text/*"),
                NULL, (IAssetFileDescriptor**)&descr);
        if (ec == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
            String tmp;
            uri->ToString(&tmp);
            return CStringWrapper::New(tmp, text);
        }

        FAIL_RETURN(descr->CreateInputStream((IFileInputStream**)&stream));
        AutoPtr<IInputStreamReader> reader;
        ec = CInputStreamReader::New(stream, String("UTF-8"), (IInputStreamReader**)&reader);
        if (FAILED(ec)) {
            if (stream != NULL) {
                stream->Close();
            }
            return ec;
        }

        // Got it...  copy the stream into a local string and return it.
        StringBuilder builder(128);
        AutoPtr< ArrayOf<Char32> > buffer = ArrayOf<Char32>::Alloc(8192);
        Int32 len;
        while (reader->ReadChars(buffer, &len), len > 0) {
            builder.AppendChars(*buffer, 0, len);
        }
        String str;
        builder.ToString(&str);
        AutoPtr<ICharSequence> seq = builder.ToCharSequence();
        *text = seq;
        REFCOUNT_ADD(*text);

        if (stream != NULL) {
            stream->Close();
        }
        // } catch (FileNotFoundException e) {
            // Unable to open content URI as text...  not really an
            // error, just something to ignore.
        // } catch (IOException e) {
            // Something bad has happened.
            // Log.w("ClippedData", "Failure loading text", e);
            // return e.toString();
        // } finally {
            // if (stream != null) {
                // try {
                    // stream.close();
                // } catch (IOException e) {
                // }
            // }
        // }
        return NOERROR;
    }

    // Finally, if all we have is an Intent, then we can just turn that
    // into text.  Not the most user-friendly thing, but it's something.
    AutoPtr<IIntent> intent = mIntent;
    if (intent != NULL) {
        String str;
        intent->ToUri(IIntent::URI_INTENT_SCHEME, &str);
        return CStringWrapper::New(str, text);
    }

    // Shouldn't get here, but just in case...
    return CStringWrapper::New(String(""), text);
}

ECode CClipDataItem::CoerceToStyledText(
    /* [in] */ IContext* context,
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    AutoPtr<ICharSequence> cs = mText;
    if (ISpanned::Probe(cs) != NULL) {
        *text = cs;
        REFCOUNT_ADD(*text);
        return NOERROR;
    }

    String htmlText = mHtmlText;
    if (!htmlText.IsNull()) {
        // try {
            AutoPtr<ICharSequence> newText;
            assert(0);
//            newText = Html.fromHtml(htmlText);
            if (newText != NULL) {
                *text = newText;
                REFCOUNT_ADD(*text);
                return NOERROR;
            }
        // } catch (RuntimeException e) {
            // If anything bad happens, we'll fall back on the plain text.
        // }
    }

    if (cs != NULL) {
        *text = cs;
        REFCOUNT_ADD(*text);
        return NOERROR;
    }

    CoerceToHtmlOrStyledText(context, TRUE, text);
    return NOERROR;
}

ECode CClipDataItem::CoerceToHtmlText(
    /* [in] */ IContext* context,
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);
    // If the item has an explicit HTML value, simply return that.
    String htmlText = mHtmlText;
    if (!htmlText.IsNull()) {
        *text = htmlText;
        return NOERROR;
    }

    // If this Item has a plain text value, return it as HTML.
    AutoPtr<ICharSequence> cs = mText;
    if (cs != NULL) {
        if (ISpanned::Probe(cs) != NULL) {
            // return Html.toHtml((Spanned)cs);
            assert(0);
            return E_NOT_IMPLEMENTED;
        }
        // return Html.escapeHtml(cs);
        assert(0);
        return E_NOT_IMPLEMENTED;
    }

    CoerceToHtmlOrStyledText(context, FALSE, (ICharSequence**)&cs);
    if (cs != NULL) {
        return cs->ToString(text);
    }
    *text = NULL;
    return NOERROR;
}

ECode CClipDataItem::CoerceToHtmlOrStyledText(
    /* [in] */ IContext* context,
    /* [in] */ Boolean styled,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);
    *cs = NULL;
    // If this Item has a URI value, try using that.
    if (mUri != NULL) {
        // Check to see what data representations the content
        // provider supports.  We would like HTML text, but if that
        // is not possible we'll live with plan text.
        AutoPtr<IContentResolver> resolver;
        context->GetContentResolver((IContentResolver**)&resolver);
        AutoPtr< ArrayOf<String> > types;
        resolver->GetStreamTypes(mUri, String("text/*"), (ArrayOf<String>**)&types);
        Boolean hasHtml = FALSE;
        Boolean hasText = FALSE;
        if (types != NULL) {
            for (Int32 i = 0; i < types->GetLength(); ++i) {
                if (String("text/html").Equals((*types)[i])) {
                    hasHtml = TRUE;
                }
                else if ((*types)[i].StartWith("text/")) {
                    hasText = TRUE;
                }
            }
        }

        // If the provider can serve data we can use, open and load it.
        if (hasHtml || hasText) {
            AutoPtr<IFileInputStream> stream;
            // BEGIN FROM THIS
            // try {
            // Ask for a stream of the desired type.
            AutoPtr<IAssetFileDescriptor> descr;
            AutoPtr<IInputStreamReader> reader;
            // Got it...  copy the stream into a local string and return it.
            AutoPtr<IStringBuilder> builder = new StringBuilder(128);
            AutoPtr<ArrayOf<Char32> > buffer = ArrayOf<Char32>::Alloc(8192);
            Int32 len = 0;
            String text;
            AutoPtr<ICharSequence> newText;
            ECode ec = resolver->OpenTypedAssetFileDescriptor(mUri, hasHtml ? String("text/html") : String("text/plain"),
                (IBundle*)NULL, (IAssetFileDescriptor**)&descr);
            FAIL_GOTO(ec, EXIT);
            ec = descr->CreateInputStream((IFileInputStream**)&stream);
            FAIL_GOTO(ec, EXIT);
            ec = CInputStreamReader::New(stream, String("UTF-8"), (IInputStreamReader**)&reader);
            FAIL_GOTO(ec, EXIT);

            while ((reader->ReadChars(buffer, &len), len) > 0) {
                ec = builder->AppendChars(*buffer, 0, len);
                FAIL_GOTO(ec, EXIT);
            }

            FAIL_GOTO(builder->ToString(&text), EXIT);

            if (hasHtml) {
                if (styled) {
                    // We loaded HTML formatted text and the caller
                    // want styled text, convert it.
                    newText = Html::FromHtml(text);

                    if (newText != NULL) {
                        *cs = newText;
                        REFCOUNT_ADD(*cs);
                        if (stream != NULL) {
                            FAIL_RETURN(ICloseable::Probe(stream)->Close());
                        }
                        return NOERROR;
                    }
                    else {
                        return CStringWrapper::New(text, cs);
                    }
                }
                else {
                    // We loaded HTML formatted text and that is what
                    // the caller wants, just return it.
                    return CStringWrapper::New(text, cs);
                }
            }

            if (styled) {
                // We loaded plain text and the caller wants styled
                // text, that is all we have so return it.
                return CStringWrapper::New(text, cs);
            }
            else {
                // We loaded plain text and the caller wants HTML
                // text, escape it for HTML.
                if (stream != NULL) {
                    FAIL_RETURN(ICloseable::Probe(stream)->Close());
                }

                AutoPtr<ICharSequence> tmp;
                CStringWrapper::New(text, (ICharSequence**)&tmp);
                String value = Html::EscapeHtml(tmp);
                return CStringWrapper::New(value, cs);
            }

            // } catch (FileNotFoundException e) {
            //     // Unable to open content URI as text...  not really an
            //     // error, just something to ignore.

            // } catch (IOException e) {
            //     // Something bad has happened.
            //     Log.w("ClippedData", "Failure loading text", e);
            //     return Html.escapeHtml(e.toString());

            // } finally {
            //     if (stream != null) {
            //         try {
            //             stream.close();
            //         } catch (IOException e) {
            //         }
            //     }
            // }
EXIT:
            if (stream != NULL) {
                FAIL_RETURN(ICloseable::Probe(stream)->Close());
            }
            if (ec == (ECode)E_IO_EXCEPTION) {
                // Something bad has happened.
//                Html.escapeHtml(e.toString());
                return E_NOT_IMPLEMENTED;
            }
            else if (ec == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
                // Unable to open content URI as text...  not really an
                // error, just something to ignore.
            }
            else {
                return ec;
            }

        }

        // If we couldn't open the URI as a stream, then we can build
        // some HTML text with the URI itself.
        // probably serves fairly well as a textual representation.
        String str;
        FAIL_RETURN(mUri->ToString(&str));
        if (styled) {
            return UriToStyledText(str, cs);
        }
        else {
            String htmlText;
            FAIL_RETURN(UriToHtml(str, &htmlText));
            return CStringWrapper::New(htmlText, cs);
        }
    }

    // Finally, if all we have is an Intent, then we can just turn that
    // into text.  Not the most user-friendly thing, but it's something.
    if (mIntent != NULL) {
        String uri;
        FAIL_RETURN(mIntent->ToUri(IIntent::URI_INTENT_SCHEME, &uri));
        if (styled) {
            return UriToStyledText(uri, cs);
        }
        else {
            String htmlText;
            FAIL_RETURN(UriToHtml(uri, &htmlText));
            return CStringWrapper::New(htmlText, cs);
        }
    }

    // Shouldn't get here, but just in case...
    return CStringWrapper::New(String(""), cs);
}

ECode CClipDataItem::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> b = new StringBuilder(128);
    b->AppendString(String("ClipData.Item { "));
    FAIL_RETURN(ToShortString(b));
    b->AppendString(String(" }"));
    return b->ToString(str);
}

ECode CClipDataItem::ToShortString(
    /* [in] */ IStringBuilder* sb)
{
    if (!mHtmlText.IsNull()) {
        sb->AppendString(String("H:"));
        sb->AppendString(mHtmlText);
    }
    else if (mText != NULL) {
        sb->AppendString(String("T:"));
        String str;
        FAIL_RETURN(mText->ToString(&str));
        sb->AppendString(str);
    }
    else if (mUri != NULL) {
        sb->AppendString(String("U:"));
        String str;
        FAIL_RETURN(mUri->ToString(&str));
        sb->AppendString(str);
    }
    else if (mIntent != NULL) {
        sb->AppendString(String("I:"));
        FAIL_RETURN(mIntent->ToShortString(sb, TRUE, TRUE, TRUE, TRUE));
    }
    else {
        sb->AppendString(String("NULL"));
    }

    return NOERROR;
}

ECode CClipDataItem::constructor(
    /* [in] */ ICharSequence* text)
{
    mText = text;
    mHtmlText = NULL;
    mIntent = NULL;
    mUri = NULL;
    return NOERROR;
}

ECode CClipDataItem::constructor(
    /* [in] */ ICharSequence* text,
    /* [in] */ const String& htmlText)
{
    mText = text;
    mHtmlText = htmlText;
    mIntent = NULL;
    mUri = NULL;
    return NOERROR;
}

ECode CClipDataItem::constructor(
    /* [in] */ IIntent* intent)
{
    mText = NULL;
    mHtmlText = NULL;
    mIntent = intent;
    mUri = NULL;
    return NOERROR;
}

ECode CClipDataItem::constructor(
    /* [in] */ IUri* uri)
{
    mText = NULL;
    mHtmlText = NULL;
    mIntent = NULL;
    mUri = uri;
    return NOERROR;
}

ECode CClipDataItem::constructor(
    /* [in] */ ICharSequence* text,
    /* [in] */ IIntent* intent,
    /* [in] */ IUri* uri)
{
    mText = text;
    mHtmlText = NULL;
    mIntent = intent;
    mUri = uri;
    return NOERROR;
}

ECode CClipDataItem::constructor(
    /* [in] */ ICharSequence* text,
    /* [in] */ const String& htmlText,
    /* [in] */ IIntent* intent,
    /* [in] */ IUri* uri)
{
    if (!htmlText.IsNull() && NULL == text) {
        //throw new IllegalArgumentException(
        //        "Plain text must be supplied if HTML text is supplied");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mText = text;
    mHtmlText = htmlText;
    mIntent = intent;
    mUri = uri;
    return NOERROR;
}

ECode CClipDataItem::UriToHtml(
    /* [in] */ const String& uri,
    /* [out] */ String* htmlText)
{
    VALIDATE_NOT_NULL(htmlText)
    AutoPtr<ICharSequence> uriSeq;
    CStringWrapper::New(uri, (ICharSequence**)&uriSeq);
    StringBuilder builder(256);
    builder += "<a href=\"";
    builder += Html::EscapeHtml(uriSeq);
    builder += uri;
    builder += "\">";
    builder += Html::EscapeHtml(uriSeq);
    builder += uri;
    builder += "</a>";
    return builder.ToString(htmlText);
}

ECode CClipDataItem::UriToStyledText(
    /* [in] */ const String& uri,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs)
    AutoPtr<ISpannableStringBuilder> builder;
    FAIL_RETURN(CSpannableStringBuilder::New((ISpannableStringBuilder**)&builder));
    AutoPtr<ICharSequence> tmpUri;
    FAIL_RETURN(CStringWrapper::New(uri, (ICharSequence**)&tmpUri));
    FAIL_RETURN(builder->Append(tmpUri));

    AutoPtr<IURLSpan> span;
    CURLSpan::New(uri, (IURLSpan**)&span);
    Int32 len = 0;
    builder->GetLength(&len);
    builder->SetSpan(span, 0, len, ISpannable::SPAN_EXCLUSIVE_EXCLUSIVE);
    String str;
    FAIL_RETURN(builder->ToString(&str));
    *cs = ICharSequence::Probe(builder.Get());
    REFCOUNT_ADD(*cs);
    return NOERROR;
}

}
}
}

