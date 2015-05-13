
#include "text/style/CImageSpan.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ICHARACTERSTYLE_METHODS_IMPL(CImageSpan, ImageSpan, ImageSpan)
IMETRICAFFECTINGSPAN_METHODS_IMPL(CImageSpan, ImageSpan, ImageSpan)
//IREPLACEMENTSPAN_METHODS_IMPL(CImageSpan, ImageSpan, ImageSpan)
//IDynamicDrawableSpan_METHODS_IMPL(CImageSpan, ImageSpan, ImageSpan)

PInterface CImageSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CImageSpan::Probe(riid);
}

ECode CImageSpan::GetSize(
    /* [in] */ IPaint* paint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaintFontMetricsInt* fm,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size=ImageSpan::GetSize(paint, text, start, end, fm);
    return NOERROR;
}

ECode CImageSpan::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Int32 top,
    /* [in] */ Int32 y,
    /* [in] */ Int32 bottom,
    /* [in] */ IPaint* paint)
{
    ImageSpan::Draw(canvas, text, start, end, x, top, y, bottom, paint);
    return NOERROR;
}

ECode CImageSpan::GetVerticalAlignment(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=ImageSpan::GetVerticalAlignment();
    return NOERROR;
}

ECode CImageSpan::GetDrawable(
    /* [out] */ IDrawable** ret)
{
    VALIDATE_NOT_NULL(ret);
    AutoPtr<IDrawable> d = ImageSpan::GetDrawable();
    *ret = d;
    INTERFACE_ADDREF(*ret);
    return NOERROR;
}

ECode CImageSpan::GetSource(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret=ImageSpan::GetSource();
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IBitmap* b)
{
    Init(b);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 verticalAlignment)
{
    Init(b, verticalAlignment);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* b)
{
    Init(context, b);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* b,
    /* [in] */ Int32 verticalAlignment)
{
    Init(context, b, verticalAlignment);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IDrawable* d)
{
    Init(d);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IDrawable* d,
    /* [in] */ Int32 verticalAlignment)
{
    Init(d, verticalAlignment);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IDrawable* d,
    /* [in] */ const String& source)
{
    Init(d, source);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IDrawable* d,
    /* [in] */ const String& source,
    /* [in] */ Int32 verticalAlignment)
{
    Init(d, source, verticalAlignment);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri)
{
    Init(context, uri);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 verticalAlignment)
{
    Init(context, uri, verticalAlignment);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId)
{
    Init(context, resourceId);
    return NOERROR;
}

ECode CImageSpan::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceId,
    /* [in] */ Int32 verticalAlignment)
{
    Init(context, resourceId, verticalAlignment);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

