
#include "text/CHtml.h"
#include "text/Html.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CHtml::FromHtml(
    /* [in] */ const String& source,
    /* [out] */ ISpanned** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ISpanned> spanned = Html::FromHtml(source);
    *ret = spanned;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CHtml::FromHtmlEx(
    /* [in] */ const String& source,
    /* [in] */ IHtmlImageGetter* imageGetter,
    /* [in] */ IHtmlTagHandler* tagHandler,
    /* [out] */ ISpanned** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<ISpanned> spanned = Html::FromHtml(source, imageGetter, tagHandler);
    *ret = spanned;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CHtml::ToHtml(
    /* [in] */ ISpanned* text,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Html::ToHtml(text);
    return NOERROR;
}

ECode CHtml::EscapeHtml(
    /* [in] */ ICharSequence* text,
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Html::EscapeHtml(text);
    return NOERROR;
}

} // namespace Text
} // namespace Droid
} // namespace Elastos

