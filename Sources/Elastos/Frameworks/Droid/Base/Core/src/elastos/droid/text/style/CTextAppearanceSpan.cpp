
#include "elastos/droid/text/style/CTextAppearanceSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CTextAppearanceSpan, TextAppearanceSpan, TextAppearanceSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CTextAppearanceSpan, TextAppearanceSpan, TextAppearanceSpan)
IPARCELABLESPAN_METHODS_IMPL(CTextAppearanceSpan, TextAppearanceSpan, TextAppearanceSpan)

PInterface CTextAppearanceSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CTextAppearanceSpan::Probe(riid);
}

ECode CTextAppearanceSpan::GetFamily(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextAppearanceSpan::GetFamily();
    return NOERROR;
}

ECode CTextAppearanceSpan::GetTextColor(
    /* [out] */ IColorStateList** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IColorStateList> csl = TextAppearanceSpan::GetTextColor();
    *ret = csl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextAppearanceSpan::GetLinkTextColor(
    /* [out] */ IColorStateList** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IColorStateList> csl = TextAppearanceSpan::GetLinkTextColor();
    *ret = csl;
    REFCOUNT_ADD(*ret);
    return NOERROR;
}

ECode CTextAppearanceSpan::GetTextSize(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextAppearanceSpan::GetTextSize();
    return NOERROR;
}

ECode CTextAppearanceSpan::GetTextStyle(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = TextAppearanceSpan::GetTextStyle();
    return NOERROR;
}

ECode CTextAppearanceSpan::constructor()
{
    return NOERROR;
}

ECode CTextAppearanceSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 appearance)
{
    Init(context, appearance);
    return NOERROR;
}

ECode CTextAppearanceSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 appearance,
    /* [in] */ Int32 colorList)
{
    Init(context, appearance, colorList);
    return NOERROR;
}

ECode CTextAppearanceSpan::constructor(
    /* [in] */ const String& family,
    /* [in] */ Int32 style,
    /* [in] */ Int32 size,
    /* [in] */ IColorStateList* color,
    /* [in] */ IColorStateList* linkColor)
{
    Init(family, style, size, color, linkColor);
    return NOERROR;
}

ECode CTextAppearanceSpan::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos


