
#include "elastos/droid/text/Html.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/text/AndroidBidi.h"
#include "elastos/droid/text/CSpannableStringBuilder.h"
#include "elastos/droid/text/style/CQuoteSpan.h"
#include "elastos/droid/text/style/CStyleSpan.h"
#include "elastos/droid/text/style/CTypefaceSpan.h"
#include "elastos/droid/text/style/CSuperscriptSpan.h"
#include "elastos/droid/text/style/CSubscriptSpan.h"
#include "elastos/droid/text/style/CUnderlineSpan.h"
#include "elastos/droid/text/style/CURLSpan.h"
#include "elastos/droid/text/style/CImageSpan.h"
#include "elastos/droid/text/style/CForegroundColorSpan.h"
#include "elastos/droid/text/style/CRelativeSizeSpan.h"
#include "elastos/droid/text/style/CTextAppearanceSpan.h"
#include "content/res/CResourcesHelper.h"
#include "utility/ArrayUtils.h"
#include "utility/XmlUtils.h"
#include "R.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/Character.h>

using Org::Xml::Sax::EIID_IContentHandler;
using Org::Xml::Sax::IInputSource;
using Org::Xml::Sax::CInputSource;
using Elastos::Core::StringUtils;
using Elastos::Core::Character;
using Elastos::Core::CString;
using Elastos::IO::CStringReader;
using Elastos::IO::IStringReader;
using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Utility::XmlUtils;
using Elastos::Droid::Content::Res::IResourcesHelper;
using Elastos::Droid::Content::Res::CResourcesHelper;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IColorStateList;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::Style::IParagraphStyle;
using Elastos::Droid::Text::Style::EIID_IParagraphStyle;
using Elastos::Droid::Text::Style::IAlignmentSpan;
using Elastos::Droid::Text::Style::IQuoteSpan;
using Elastos::Droid::Text::Style::CQuoteSpan;
using Elastos::Droid::Text::Style::EIID_IQuoteSpan;
using Elastos::Droid::Text::Style::ICharacterStyle;
using Elastos::Droid::Text::Style::EIID_ICharacterStyle;
using Elastos::Droid::Text::Style::IStyleSpan;
using Elastos::Droid::Text::Style::CStyleSpan;
using Elastos::Droid::Text::Style::ITypefaceSpan;
using Elastos::Droid::Text::Style::CTypefaceSpan;
using Elastos::Droid::Text::Style::ISuperscriptSpan;
using Elastos::Droid::Text::Style::CSuperscriptSpan;
using Elastos::Droid::Text::Style::ISubscriptSpan;
using Elastos::Droid::Text::Style::CSubscriptSpan;
using Elastos::Droid::Text::Style::IUnderlineSpan;
using Elastos::Droid::Text::Style::CUnderlineSpan;
using Elastos::Droid::Text::Style::IStrikethroughSpan;
using Elastos::Droid::Text::Style::IURLSpan;
using Elastos::Droid::Text::Style::CURLSpan;
using Elastos::Droid::Text::Style::IImageSpan;
using Elastos::Droid::Text::Style::CImageSpan;
using Elastos::Droid::Text::Style::IAbsoluteSizeSpan;
using Elastos::Droid::Text::Style::IForegroundColorSpan;
using Elastos::Droid::Text::Style::CForegroundColorSpan;
using Elastos::Droid::Text::Style::IRelativeSizeSpan;
using Elastos::Droid::Text::Style::CRelativeSizeSpan;
using Elastos::Droid::Text::Style::ITextAppearanceSpan;
using Elastos::Droid::Text::Style::CTextAppearanceSpan;

namespace Elastos {
namespace Droid {
namespace Text {

/**************************Html::HtmlParser*************************/
//AutoPtr<IHTMLSchema> Html::HtmlParser::schema;// = new HTMLSchema();

/**************************Html*************************/
Html::Html()
{ }

AutoPtr<ISpanned> Html::FromHtml(
    /* [in] */ const String& source)
{
    return FromHtml(source, NULL, NULL);
}

AutoPtr<ISpanned> Html::FromHtml(
    /* [in] */ const String& source,
    /* [in] */ IHtmlImageGetter* imageGetter,
    /* [in] */ IHtmlTagHandler* tagHandler)
{
//    AutoPtr<IParser> parser;// = new Parser();
//    try {
//        parser.setProperty(Parser.schemaProperty, HtmlParser.schema);
//    } catch (org.xml.sax.SAXNotRecognizedException e) {
//        // Should not happen.
//        throw new RuntimeException(e);
//    } catch (org.xml.sax.SAXNotSupportedException e) {
//        // Should not happen.
//        throw new RuntimeException(e);
//    }

    AutoPtr<HtmlToSpannedConverter> converter =
            new HtmlToSpannedConverter(source, imageGetter, tagHandler,
                    /*parser*/NULL);
    return converter->Convert();
}

String Html::ToHtml(
    /* [in] */ ISpanned* text)
{
    StringBuilder out;
    WithinHtml(&out, text);
    return out.ToString();
}

String Html::EscapeHtml(
    /* [in] */ ICharSequence* text)
{
    StringBuilder out;
    Int32 csLen;
    WithinStyle(&out, text, 0, (text->GetLength(&csLen), csLen) );
    return out.ToString();
}

void Html::WithinHtml(
    /* [in] */ StringBuilder* out,
    /* [in] */ ISpanned* text)
{
    Int32 len;
    text->GetLength(&len);

    Int32 next, textLen;
    for (Int32 i = 0; i < (text->GetLength(&textLen), textLen); i = next) {
        text->NextSpanTransition(i, len, EIID_IParagraphStyle, &next);
        AutoPtr< ArrayOf<IParagraphStyle*> > style;
        text->GetSpans(i, next, EIID_IParagraphStyle, (ArrayOf<IInterface*>**)&style);
        String elements(" ");
        Boolean needDiv = FALSE;

        for(Int32 j = 0; j < style->GetLength(); j++) {
            AutoPtr<IAlignmentSpan> alignmentSpan = IAlignmentSpan::Probe((*style)[j]);
            if (alignmentSpan != NULL) {
                LayoutAlignment align;
                ((IAlignmentSpan*) (*style)[j])->GetAlignment(&align);
                needDiv = TRUE;
                if (align == /*Layout.Alignment.ALIGN_CENTER*/ALIGN_CENTER) {
                    elements = String("align=\"center\" ") + elements;
                }
                else if (align == /*Layout.Alignment.ALIGN_OPPOSITE*/ALIGN_OPPOSITE) {
                    elements = String("align=\"right\" ") + elements;
                }
                else {
                    elements = String("align=\"left\" ") + elements;
                }
            }
        }
        if (needDiv) {
            out->Append(("<div ") + elements + String(">"));
        }

        WithinDiv(out, text, i, next);

        if (needDiv) {
            out->Append("</div>");
        }
    }
}

void Html::WithinDiv(
    /* [in] */ StringBuilder* out,
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 next;
    for (Int32 i = start; i < end; i = next) {
        text->NextSpanTransition(i, end, EIID_IQuoteSpan, &next);
        AutoPtr< ArrayOf<IQuoteSpan> > quotes;
        text->GetSpans(i, next, EIID_IQuoteSpan, (ArrayOf<IInterface*>**)&quotes);

        for(Int32 j = 0; j < quotes->GetLength(); j++) {
            out->Append("<blockquote>");
        }

        WithinBlockquote(out, text, i, next);

        for(Int32 j = 0; j < quotes->GetLength(); j++) {
            out->Append("</blockquote>\n");
        }
    }
}

String Html::GetOpenParaTagWithDirection(
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 len = end - start;
    AutoPtr< ArrayOf<Byte> > levels = ArrayUtils::NewUnpaddedByteArray(len);
    AutoPtr< ArrayOf<Char32> > buffer = TextUtils::Obtain(len);
    TextUtils::GetChars(text, start, end, buffer, 0);

    Int32 paraDir = AndroidBidi::Bidi(ILayout::DIR_REQUEST_DEFAULT_LTR, buffer, levels, len,
                FALSE /* no info */);
    switch(paraDir) {
        case ILayout::DIR_RIGHT_TO_LEFT:
                return String("<p dir=\"rtl\">");
        case ILayout::DIR_LEFT_TO_RIGHT:
        default:
                return String("<p dir=\"ltr\">");
    }
}

void Html::WithinBlockquote(
    /* [in] */ StringBuilder* out,
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    out->AppendString(GetOpenParaTagWithDirection(text, start, end));

    Int32 next;
    for (Int32 i = start; i < end; i = next) {
        next = TextUtils::IndexOf(text, '\n', i, end);
        if (next < 0) {
            next = end;
        }

        Int32 nl = 0;

        Char32 textChar;
        while (next < end && (text->GetCharAt(next, &textChar), textChar) == '\n') {
            nl++;
            next++;
        }

        WithinParagraph(out, text, i, next - nl, nl, next == end);
    }

    out->Append("</p>\n");
}

void Html::WithinParagraph(
    /* [in] */ StringBuilder* out,
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 nl,
    /* [in] */ Boolean last)
{
    Int32 next;
    for (Int32 i = start; i < end; i = next) {
        text->NextSpanTransition(i, end, EIID_ICharacterStyle, &next);
        AutoPtr< ArrayOf<ICharacterStyle*> > style;
        text->GetSpans(i, next, EIID_ICharacterStyle, (ArrayOf<IInterface*>**)&style);

        for (Int32 j = 0; j < style->GetLength(); j++) {
            AutoPtr<IStyleSpan> styleSpan = IStyleSpan::Probe((*style)[j]);
            if (styleSpan != NULL) {
                Int32 s;
                ((IStyleSpan*) (*style)[j])->GetStyle(&s);

                if ((s & ITypeface::BOLD) != 0) {
                    out->Append("<b>");
                }
                if ((s & ITypeface::ITALIC) != 0) {
                    out->Append("<i>");
                }
            }
            AutoPtr<ITypefaceSpan> typefaceSpan = ITypefaceSpan::Probe((*style)[j]);
            if (typefaceSpan != NULL) {
                String s;
                ((ITypefaceSpan*) (*style)[j])->GetFamily(&s);

                if (s.Equals("monospace")) {
                    out->Append("<tt>");
                }
            }
            AutoPtr<ISuperscriptSpan> superscriptSpan = ISuperscriptSpan::Probe((*style)[j]);
            if (superscriptSpan != NULL) {
                out->Append("<sup>");
            }
            AutoPtr<ISubscriptSpan> subscriptSpan = ISubscriptSpan::Probe((*style)[j]);
            if (subscriptSpan != NULL) {
                out->Append("<sub>");
            }
            AutoPtr<IUnderlineSpan> underlineSpan = IUnderlineSpan::Probe((*style)[j]);
            if (underlineSpan != NULL) {
                out->Append("<u>");
            }
            AutoPtr<IStrikethroughSpan> strikethroughSpan = IStrikethroughSpan::Probe((*style)[j]);
            if (strikethroughSpan != NULL) {
                out->Append("<strike>");
            }
            AutoPtr<IURLSpan> urlSpan = IURLSpan::Probe((*style)[j]);
            if (urlSpan != NULL) {
                out->Append("<a href=\"");
                String strUrl;
                out->AppendString((((IURLSpan*) (*style)[j])->GetURL(&strUrl), strUrl));
                out->Append("\">");
            }
            AutoPtr<IImageSpan> imageSpan = IImageSpan::Probe((*style)[j]);
            if (imageSpan != NULL) {
                out->Append("<img src=\"");
                String strSource;
                out->AppendString((((IImageSpan*) (*style)[j])->GetSource(&strSource), strSource));
                out->Append("\">");

                // Don't output the dummy character underlying the image.
                i = next;
            }
            AutoPtr<IAbsoluteSizeSpan> absoluteSizeSpan = IAbsoluteSizeSpan::Probe((*style)[j]);
            if (absoluteSizeSpan != NULL) {
                out->Append("<font size =\"");
                Int32 nSize;
                out->AppendInt32((((IAbsoluteSizeSpan*) (*style)[j])->GetSize(&nSize), nSize) / 6);
                out->Append("\">");
            }
            AutoPtr<IForegroundColorSpan> foregroundColorSpan = IForegroundColorSpan::Probe((*style)[j]);
            if (foregroundColorSpan != NULL) {
                out->Append("<font color =\"#");
                Int32 foregroundColor;
                String color = StringUtils::Int32ToHexString(
                    (((IForegroundColorSpan*)(*style)[j])->GetForegroundColor(&foregroundColor), foregroundColor) + 0x01000000);
                while (color.GetLength() < 6) {
                    color = String("0") + color;
                }
                out->AppendString(color);
                out->Append("\">");
            }
        }

        WithinStyle(out, text, i, next);

        for (Int32 j = style->GetLength() - 1; j >= 0; j--) {
            AutoPtr<IForegroundColorSpan> foregroundColorSpan = IForegroundColorSpan::Probe((*style)[j]);
            if (foregroundColorSpan != NULL) {
                out->Append("</font>");
            }
            AutoPtr<IAbsoluteSizeSpan> absoluteSizeSpan = IAbsoluteSizeSpan::Probe((*style)[j]);
            if (absoluteSizeSpan != NULL) {
                out->Append("</font>");
            }
            AutoPtr<IURLSpan> urlSpan = IURLSpan::Probe((*style)[j]);
            if (urlSpan != NULL) {
                out->Append("</a>");
            }
            AutoPtr<IStrikethroughSpan> strikethroughSpan = IStrikethroughSpan::Probe((*style)[j]);
            if (strikethroughSpan != NULL) {
                out->Append("</strike>");
            }
            AutoPtr<IUnderlineSpan> underlineSpan = IUnderlineSpan::Probe((*style)[j]);
            if (underlineSpan != NULL) {
                out->Append("</u>");
            }
            AutoPtr<ISubscriptSpan> subscriptSpan = ISubscriptSpan::Probe((*style)[j]);
            if (subscriptSpan != NULL) {
                out->Append("</sub>");
            }
            AutoPtr<ISuperscriptSpan> superscriptSpan = ISuperscriptSpan::Probe((*style)[j]);
            if (superscriptSpan != NULL) {
                out->Append("</sup>");
            }
            AutoPtr<ITypefaceSpan> typefaceSpan = ITypefaceSpan::Probe((*style)[j]);
            if (typefaceSpan != NULL) {
                String s;
                ((ITypefaceSpan*) (*style)[j])->GetFamily(&s);

                if (s.Equals("monospace")) {
                    out->Append("</tt>");
                }
            }
            AutoPtr<IStyleSpan> styleSpan = IStyleSpan::Probe((*style)[j]);
            if (styleSpan != NULL) {
                Int32 s;
                ((IStyleSpan*) (*style)[j])->GetStyle(&s);

                if ((s & ITypeface::BOLD) != 0) {
                    out->Append("</b>");
                }
                if ((s & ITypeface::ITALIC) != 0) {
                    out->Append("</i>");
                }
            }
        }
    }

    String p = last ? String("") : String("</p>\n") + GetOpenParaTagWithDirection(text, start, end);

    if (nl == 1) {
        out->Append("<br>\n");
    }
    else if (nl == 2) {
        out->AppendString(p);
    }
    else {
        for (Int32 i = 2; i < nl; i++) {
            out->Append("<br>");
        }
        out->AppendString(p);
    }
}

void Html::WithinStyle(
    /* [in] */ StringBuilder* out,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    for (Int32 i = start; i < end; i++) {
        Char32 c;
        text->GetCharAt(i, &c);

        if (c == '<') {
            out->Append("&lt;");
        }
        else if (c == '>') {
            out->Append("&gt;");
        }
        else if (c == '&') {
            out->Append("&amp;");
        }

        else if (c >= 0xD800 && c <= 0xDFFF) {
            if (c < 0xDC00 && i + 1 < end) {
                char d = text.charAt(i + 1);
                if (d >= 0xDC00 && d <= 0xDFFF) {
                    i++;
                    int codepoint = 0x010000 | (int) c - 0xD800 << 10 | (int) d - 0xDC00;
                    out->Append("&#");
                    out->Append(codepoint);
                    out->Append(";");
                }
            }
        }
        else if (c > 0x7E || c < ' ') {
            out->Append("&#";
            out->Append((Int32)c);
            out->Append(";");
        }
        else if (c == ' ') {
            Char32 textChar;
            while (i + 1 < end && (text->GetCharAt(i + 1, &textChar), textChar) == ' ') {
                out->Append("&nbsp;");
                i++;
            }

            out->AppendChar(' ');
        }
        else {
            out->AppendChar(c);
        }
    }
}

/**************************HtmlToSpannedConverter::Font*************************/
HtmlToSpannedConverter::Font::Font(
    /* [in] */ const String& color,
    /* [in] */ const String& face)
{
    mColor = color;
    mFace = face;
}

/**************************HtmlToSpannedConverter::Href*************************/
HtmlToSpannedConverter::Href::Href(
    /* [in] */ const String& href)
{
    mHref = href;
}

/**************************HtmlToSpannedConverter::Header*************************/
HtmlToSpannedConverter::Header::Header(
    /* [in] */ Int32 level)
{
    mLevel = level;
}

/**************************HtmlToSpannedConverter*************************/
AutoPtr< ArrayOf<Float> > HtmlToSpannedConverter::HEADER_SIZES; // = { 1.5f, 1.4f, 1.3f, 1.2f, 1.1f, 1f, };

AutoPtr< ArrayOf<Float> > HtmlToSpannedConverter::InitStaticHeaderSizes()
{
    AutoPtr< ArrayOf<Float> > headerSizes = ArrayOf<Float>::Alloc(7);
    (*headerSizes)[0] = 1.5f;
    (*headerSizes)[1] = 1.4f;
    (*headerSizes)[2] = 1.3f;
    (*headerSizes)[3] = 1.2f;
    (*headerSizes)[4] = 1.1f;
    (*headerSizes)[5] = 1.0f;
    (*headerSizes)[6] = 0.0f;
    return headerSizes;
}

PInterface HtmlToSpannedConverter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IContentHandler*)this;
    }
    else if (riid == EIID_IContentHandler) {
        return (IContentHandler*)this;
    }
    return NULL;
}

UInt32 HtmlToSpannedConverter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 HtmlToSpannedConverter::Release()
{
    return ElRefBase::Release();
}

ECode HtmlToSpannedConverter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (object == (IInterface*)(IContentHandler*)this) {
        *iID = EIID_IContentHandler;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

HtmlToSpannedConverter::HtmlToSpannedConverter(
    /* [in] */ const String& source,
    /* [in] */ IHtmlImageGetter* imageGetter,
    /* [in] */ IHtmlTagHandler* tagHandler,
    /* [in] */ /*IParser*/IInterface* parser)
{
    mSource = source;
    CSpannableStringBuilder::New((ISpannableStringBuilder**)&mSpannableStringBuilder);
    mImageGetter = imageGetter;
    mTagHandler = tagHandler;
    mReader = IXMLReader::Probe(parser);
}

AutoPtr<ISpanned> HtmlToSpannedConverter::Convert()
{
    mReader->SetContentHandler((IContentHandler*)this);
    //try {
        AutoPtr<IStringReader> sr;
        CStringReader::New(mSource, (IStringReader**)&sr);
        AutoPtr<IInputSource> is;
        CInputSource::New((IInputSource**)&is);
        mReader->Parse(is);
    //} catch (IOException e) {
        // We are reading from a string. There should not be IO problems.
    //    throw new RuntimeException(e);
    //} catch (SAXException e) {
        // TagSoup doesn't throw parse exceptions.
    //    throw new RuntimeException(e);
    //}

    // Fix flags and range for paragraph-type markup.
    AutoPtr< ArrayOf<IInterface*> > obj;
    Int32 ssbLen;
    mSpannableStringBuilder->GetSpans(0, (mSpannableStringBuilder->GetLength(&ssbLen), ssbLen), EIID_IParagraphStyle, (ArrayOf<IInterface*>**)&obj);
    for (Int32 i = 0; i < obj->GetLength(); i++) {
        Int32 start;
        mSpannableStringBuilder->GetSpanStart((*obj)[i], &start);
        Int32 end;
        mSpannableStringBuilder->GetSpanEnd((*obj)[i], &end);

        // If the last line of the range is blank, back off by one.
        if (end - 2 >= 0) {
            Char32 ssbChar;
            if ((mSpannableStringBuilder->GetCharAt(end - 1, &ssbChar), ssbChar) == '\n' &&
                    (mSpannableStringBuilder->GetCharAt(end - 2, &ssbChar), ssbChar) == '\n') {
                end--;
            }
        }

        if (end == start) {
            mSpannableStringBuilder->RemoveSpan((*obj)[i]);
        }
        else {
            mSpannableStringBuilder->SetSpan((*obj)[i], start, end, ISpanned::SPAN_PARAGRAPH);
        }
    }

    AutoPtr<ISpanned> sRet = (ISpanned*)(ISpannable*)(IEditable*)(mSpannableStringBuilder.Get());
    return sRet;
}

void HtmlToSpannedConverter::HandleStartTag(
    /* [in] */ const String& tag,
    /* [in] */ IAttributes* attributes)
{
    if (tag.EqualsIgnoreCase("br")) {
            // We don't need to handle this. TagSoup will ensure that there's a </br> for each <br>
            // so we can safely emite the linebreaks when we handle the close tag.
    }
    else if (tag.EqualsIgnoreCase("p")) {
        HandleP(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("div")) {
        HandleP(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("strong")) {
        AutoPtr<Bold> param = new Bold();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("b")) {
        AutoPtr<Bold> param = new Bold();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("em")) {
        AutoPtr<Italic> param = new Italic();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("cite")) {
        AutoPtr<Italic> param = new Italic();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("dfn")) {
        AutoPtr<Italic> param = new Italic();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("i")) {
        AutoPtr<Italic> param = new Italic();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("big")) {
        AutoPtr<Big> param = new Big();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("small")) {
        AutoPtr<Small> param = new Small();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("font")) {
        StartFont(mSpannableStringBuilder, attributes);
    }
    else if (tag.EqualsIgnoreCase("blockquote")) {
        HandleP(mSpannableStringBuilder);
        AutoPtr<Blockquote> param = new Blockquote();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("tt")) {
        AutoPtr<Monospace> param = new Monospace();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("a")) {
        StartA(mSpannableStringBuilder, attributes);
    }
    else if (tag.EqualsIgnoreCase("u")) {
        AutoPtr<Underline> param = new Underline();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("sup")) {
        AutoPtr<Super> param = new Super();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("sub")) {
        AutoPtr<Sub> param = new Sub();
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.GetLength() == 2 &&
        Character::ToLowerCase(tag.GetChar(0)) == 'h' &&
        tag.GetChar(1) >= '1' && tag.GetChar(1) <= '6') {
        HandleP(mSpannableStringBuilder);
        AutoPtr<Header> param = new Header(tag.GetChar(1) - '1');
        Start(mSpannableStringBuilder, (IInterface*)(param.Get()));
    }
    else if (tag.EqualsIgnoreCase("img")) {
        StartImg(mSpannableStringBuilder, attributes, mImageGetter);
    }
    else if (mTagHandler != NULL) {
        mTagHandler->HandleTag(TRUE, tag, mSpannableStringBuilder, mReader);
    }
}

void HtmlToSpannedConverter::HandleEndTag(
    /* [in] */ const String& tag)
{
    if (tag.EqualsIgnoreCase("br")) {
        HandleBr(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("p")) {
        HandleP(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("div")) {
        HandleP(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("strong")) {
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::BOLD, (IStyleSpan**)&styleSpan);
        End(mSpannableStringBuilder, /*Bold.class*/EIID_IInterface, styleSpan);
    }
    else if (tag.EqualsIgnoreCase("b")) {
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::BOLD, (IStyleSpan**)&styleSpan);
        End(mSpannableStringBuilder, /*Bold.class*/EIID_IInterface, styleSpan);
    }
    else if (tag.EqualsIgnoreCase("em")) {
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::ITALIC, (IStyleSpan**)&styleSpan);
        End(mSpannableStringBuilder, /*Italic.class*/EIID_IInterface, styleSpan);
    }
    else if (tag.EqualsIgnoreCase("cite")) {
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::ITALIC, (IStyleSpan**)&styleSpan);
        End(mSpannableStringBuilder, /*Italic.class*/EIID_IInterface, styleSpan);
    }
    else if (tag.EqualsIgnoreCase("dfn")) {
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::ITALIC, (IStyleSpan**)&styleSpan);
        End(mSpannableStringBuilder, /*Italic.class*/EIID_IInterface, styleSpan);
    }
    else if (tag.EqualsIgnoreCase("i")) {
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::ITALIC, (IStyleSpan**)&styleSpan);
        End(mSpannableStringBuilder, /*Italic.class*/EIID_IInterface, styleSpan);
    }
    else if (tag.EqualsIgnoreCase("big")) {
        AutoPtr<IRelativeSizeSpan> relativeSizeSpan;
        CRelativeSizeSpan::New(1.25f, (IRelativeSizeSpan**)&relativeSizeSpan);
        End(mSpannableStringBuilder, /*Big.class*/EIID_IInterface, relativeSizeSpan);
    }
    else if (tag.EqualsIgnoreCase("small")) {
        AutoPtr<IRelativeSizeSpan> relativeSizeSpan;
        CRelativeSizeSpan::New(0.8f, (IRelativeSizeSpan**)&relativeSizeSpan);
        End(mSpannableStringBuilder, /*Small.class*/EIID_IInterface, relativeSizeSpan);
    }
    else if (tag.EqualsIgnoreCase("font")) {
        EndFont(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("blockquote")) {
        HandleP(mSpannableStringBuilder);
        AutoPtr<IQuoteSpan> quoteSpan;
        CQuoteSpan::New((IQuoteSpan**)&quoteSpan);
        End(mSpannableStringBuilder, /*Blockquote.class*/EIID_IInterface, quoteSpan);
    }
    else if (tag.EqualsIgnoreCase("tt")) {
        AutoPtr<ITypefaceSpan> typefaceSpan;
        CTypefaceSpan::New(String("monospace"), (ITypefaceSpan**)&typefaceSpan);
        End(mSpannableStringBuilder, /*Monospace.class*/EIID_IInterface,
                    typefaceSpan);
    }
    else if (tag.EqualsIgnoreCase("a")) {
        EndA(mSpannableStringBuilder);
    }
    else if (tag.EqualsIgnoreCase("u")) {
        AutoPtr<IUnderlineSpan> underlineSpan;
        CUnderlineSpan::New((IUnderlineSpan**)&underlineSpan);
        End(mSpannableStringBuilder, /*Underline.class*/EIID_IInterface, underlineSpan);
    }
    else if (tag.EqualsIgnoreCase("sup")) {
        AutoPtr<ISuperscriptSpan> superscriptSpan;
        CSuperscriptSpan::New((ISuperscriptSpan**)&superscriptSpan);
        End(mSpannableStringBuilder, /*Super.class*/EIID_IInterface, superscriptSpan);
    }
    else if (tag.EqualsIgnoreCase("sub")) {
        AutoPtr<ISubscriptSpan> subscriptSpan;
        CSubscriptSpan::New((ISubscriptSpan**)&subscriptSpan);
        End(mSpannableStringBuilder, /*Sub.class*/EIID_IInterface, subscriptSpan);
    }
    else if (tag.GetLength() == 2 &&
                Character::ToLowerCase(tag.GetChar(0)) == 'h' &&
                tag.GetChar(1) >= '1' && tag.GetChar(1) <= '6') {
        HandleP(mSpannableStringBuilder);
        EndHeader(mSpannableStringBuilder);
    }
    else if (mTagHandler != NULL) {
        mTagHandler->HandleTag(FALSE, tag, mSpannableStringBuilder, mReader);
    }
}

void HtmlToSpannedConverter::HandleP(
    /* [in] */ ISpannableStringBuilder* text)
{
    Int32 len;
    text->GetLength(&len);

    Char32 textChar;
    if (len >= 1 && (text->GetCharAt(len - 1, &textChar), textChar) == '\n') {
        if (len >= 2 && (text->GetCharAt(len - 2, &textChar), textChar) == '\n') {
            return;
        }

        AutoPtr<ICharSequence> cs;
        CString::New(String("\n"), (ICharSequence**)&cs);
        text->Append(cs);
        return;
    }

    if (len != 0) {
        AutoPtr<ICharSequence> cs;
        CString::New(String("\n\n"), (ICharSequence**)&cs);
        text->Append(cs);
    }
}

void HtmlToSpannedConverter::HandleBr(
    /* [in] */ ISpannableStringBuilder* text)
{
    AutoPtr<ICharSequence> cs;
    CString::New(String("\n"), (ICharSequence**)&cs);
    text->Append(cs);
}

AutoPtr<IInterface> HtmlToSpannedConverter::GetLast(
    /* [in] */ ISpanned* text,
    /* [in] */ InterfaceID kind)
{
    /*
     * This knows that the last returned object from getSpans()
     * will be the most recently added.
     */
    AutoPtr< ArrayOf<IInterface*> > objs;
    Int32 textLen;
    text->GetSpans(0, (text->GetLength(&textLen), textLen), kind, (ArrayOf<IInterface*>**)&objs);

    if (objs->GetLength() == 0) {
        return NULL;
    }
    else {
        AutoPtr<IInterface> ret = (*objs)[objs->GetLength() - 1];
        return ret;
    }
}

void HtmlToSpannedConverter::Start(
    /* [in] */ ISpannableStringBuilder* text,
    /* [in] */ IInterface* mark)
{
    Int32 len;
    text->GetLength(&len);
    text->SetSpan(mark, len, len, ISpanned::SPAN_MARK_MARK);
}

void HtmlToSpannedConverter::End(
    /* [in] */ ISpannableStringBuilder* text,
    /* [in] */ InterfaceID kind,
    /* [in] */ IInterface* repl)
{
    Int32 len;
    text->GetLength(&len);
    AutoPtr<IInterface> obj = GetLast(text, kind);
    Int32 where;
    text->GetSpanStart(obj, &where);

    text->RemoveSpan(obj);

    if (where != len) {
        text->SetSpan(repl, where, len, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    }

    return;
}

void HtmlToSpannedConverter::StartImg(
    /* [in] */ ISpannableStringBuilder* text,
    /* [in] */ IAttributes* attributes,
    /* [in] */ IHtmlImageGetter* img)
{
    String src;
    attributes->GetValue(String(""), String("src"), &src);
    AutoPtr<IDrawable> d = NULL;

    if (img != NULL) {
        img->GetDrawable(src, (IDrawable**)&d);
    }

    if (d == NULL) {
        AutoPtr<IResourcesHelper> rh;
        CResourcesHelper::AcquireSingleton((IResourcesHelper**)&rh);
        AutoPtr<IResources> r;
        rh->GetSystem((IResources**)&r);
        r->GetDrawable(R::drawable::unknown_image, (IDrawable**)&d);
        Int32 width, height;
        d->SetBounds(0, 0, (d->GetIntrinsicWidth(&width), width), (d->GetIntrinsicHeight(&height), height));
    }

    Int32 len;
    text->GetLength(&len);
    AutoPtr<ICharSequence> cs;
    CString::New(String("\uFFFC"), (ICharSequence**)&cs);
    text->Append(cs);

    AutoPtr<IImageSpan> imageSpan;
    CImageSpan::New(d, src, (IImageSpan**)&imageSpan);

    Int32 textLen;
    text->SetSpan(imageSpan, len, (text->GetLength(&textLen), textLen),
                     ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
}

void HtmlToSpannedConverter::StartFont(
    /* [in] */ ISpannableStringBuilder* text,
    /* [in] */ IAttributes* attributes)
{
    String color;
    attributes->GetValue(String(""), String("color"), &color);
    String face;
    attributes->GetValue(String(""), String("face"), &face);

    Int32 len;
    text->GetLength(&len);
    AutoPtr<Font> font = new Font(color, face);
    text->SetSpan((IInterface*)(font.Get()), len, len, ISpanned::SPAN_MARK_MARK);
}

void HtmlToSpannedConverter::EndFont(
    /* [in] */ ISpannableStringBuilder* text)
{
    Int32 len;
    text->GetLength(&len);
    AutoPtr<IInterface> obj = GetLast(text, /*Font.class*/EIID_IInterface);
    Int32 where;
    text->GetSpanStart(obj, &where);

    text->RemoveSpan(obj);

    if (where != len) {
        AutoPtr<Font> f = (Font*) (obj.Get());

        AutoPtr<ICharSequence> cs;
        CString::New(f->mColor, (ICharSequence**)&cs);
        if (!TextUtils::IsEmpty(cs)) {
            if ((f->mColor).StartWith("@")) {
                AutoPtr<IResourcesHelper> rh;
                CResourcesHelper::AcquireSingleton((IResourcesHelper**)&rh);
                AutoPtr<IResources> res;
                rh->GetSystem((IResources**)&res);
                String name = (f->mColor).Substring(1);
                Int32 colorRes;
                res->GetIdentifier(name, String("color"), String("android"), &colorRes);
                if (colorRes != 0) {
                    AutoPtr<IColorStateList> colors;
                    res->GetColorStateList(colorRes, (IColorStateList**)&colors);
                    AutoPtr<ITextAppearanceSpan> textAppearanceSpan;
                    CTextAppearanceSpan::New(String(""), 0, 0, colors, NULL, (ITextAppearanceSpan**)&textAppearanceSpan);
                    text->SetSpan(textAppearanceSpan,
                                where, len,
                                ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
                }
            }
            else {
                Int32 c = Color::GetHtmlColor(f->mColor);
                if (c != -1) {
                    AutoPtr<IForegroundColorSpan> foregroundColorSpan;
                    CForegroundColorSpan::New(c | 0xFF000000, (IForegroundColorSpan**)&foregroundColorSpan);
                    text->SetSpan(foregroundColorSpan,
                                where, len,
                                ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
                }
            }
        }

        if ((f->mFace) != NULL) {
            AutoPtr<ITypefaceSpan> typefaceSpan;
            CTypefaceSpan::New(f->mFace, (ITypefaceSpan**)&typefaceSpan);
            text->SetSpan(typefaceSpan, where, len,
                             ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
        }
    }
}

void HtmlToSpannedConverter::StartA(
    /* [in] */ ISpannableStringBuilder* text,
    /* [in] */ IAttributes* attributes)
{
    String href;
    attributes->GetValue(String(""), String("href"), &href);

    Int32 len;
    text->GetLength(&len);
    AutoPtr<Href> paraHref = new Href(href);
    text->SetSpan((IInterface*)(paraHref.Get()), len, len, ISpanned::SPAN_MARK_MARK);
}

void HtmlToSpannedConverter::EndA(
    /* [in] */ ISpannableStringBuilder* text)
{
    Int32 len;
    text->GetLength(&len);
    AutoPtr<IInterface> obj = GetLast(text, /*Href.class*/EIID_IInterface);
    Int32 where;
    text->GetSpanStart(obj, &where);

    text->RemoveSpan(obj);

    if (where != len) {
        AutoPtr<Href> h = (Href*) (obj.Get());

        if (!((h->mHref).IsNullOrEmpty())) {
            AutoPtr<IURLSpan> urlSpan;
            CURLSpan::New(h->mHref, (IURLSpan**)&urlSpan);

            text->SetSpan(urlSpan, where, len,
                             ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
        }
    }
}

void HtmlToSpannedConverter::EndHeader(
    /* [in] */ ISpannableStringBuilder* text)
{
    Int32 len;
    text->GetLength(&len);
    AutoPtr<IInterface> obj = GetLast(text, /*Header.class*/EIID_IInterface);

    Int32 where;
    text->GetSpanStart(obj, &where);

    text->RemoveSpan(obj);

    // Back off not to change only the text, not the blank line.
    Char32 textChar;
    while (len > where && (text->GetCharAt(len - 1, &textChar), textChar) == '\n') {
        len--;
    }

    if (where != len) {
        AutoPtr<Header> h = (Header*) (obj.Get());

        AutoPtr<IRelativeSizeSpan> relativeSizeSpan;
        CRelativeSizeSpan::New((*HEADER_SIZES)[h->mLevel], (IRelativeSizeSpan**)&relativeSizeSpan);
        text->SetSpan(relativeSizeSpan,
                         where, len, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
        AutoPtr<IStyleSpan> styleSpan;
        CStyleSpan::New(ITypeface::BOLD, (IStyleSpan**)&styleSpan);
        text->SetSpan(styleSpan,
                         where, len, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
    }
}

ECode HtmlToSpannedConverter::SetDocumentLocator(
    /* [in] */ ILocator* locator)
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::StartDocument()// throws SAXException
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::EndDocument()// throws SAXException
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::StartPrefixMapping(
    /* [in] */ const String& prefix,
    /* [in] */ const String& uri)// throws SAXException
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::EndPrefixMapping(
    /* [in] */ const String& prefix)// throws SAXException
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::StartElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName,
    /* [in] */ IAttributes* attributes)// throws SAXException
{
    HandleStartTag(localName, attributes);
    return NOERROR;
}

ECode HtmlToSpannedConverter::EndElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName)// throws SAXException
{
    HandleEndTag(localName);
    return NOERROR;
}

ECode HtmlToSpannedConverter::Characters(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)// throws SAXException
{
    StringBuilder sb;

    /*
     * Ignore whitespace that immediately follows other whitespace;
     * newlines count as spaces.
     */

    for (Int32 i = 0; i < length; i++) {
        Char32 c = (*ch)[i + start];

        if (c == ' ' || c == '\n') {
            Char32 pred;
            Int32 len;
            sb.GetLength(&len);

            if (len == 0) {
                mSpannableStringBuilder->GetLength(&len);

                if (len == 0) {
                    pred = '\n';
                }
                else {
                    mSpannableStringBuilder->GetCharAt(len - 1, &pred);
                }
            }
            else {
                sb.GetCharAt(len - 1, &pred);
            }

            if (pred != ' ' && pred != '\n') {
                sb.AppendChar(' ');
            }
        }
        else {
            sb.AppendChar(c);
        }
    }
    AutoPtr<ICharSequence> cs;
    CString::New(sb.ToString(), (ICharSequence**)&cs);
    mSpannableStringBuilder->Append(cs);
    return NOERROR;
}

ECode HtmlToSpannedConverter::IgnorableWhitespace(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)// throws SAXException
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::ProcessingInstruction(
    /* [in] */ const String& target,
    /* [in] */ const String& data)// throws SAXException
{
    return NOERROR;
}

ECode HtmlToSpannedConverter::SkippedEntity(
    /* [in] */ const String& name)// throws SAXException
{
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
