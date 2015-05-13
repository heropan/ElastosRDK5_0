#include "text/style/DynamicDrawableSpan.h"
#include "graphics/CRect.h"

using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

const CString DynamicDrawableSpan::TAG = "DynamicDrawableSpan";

DynamicDrawableSpan::DynamicDrawableSpan()
{
    Init();
}

void DynamicDrawableSpan::Init()
{
    mVerticalAlignment = IDynamicDrawableSpan::ALIGN_BOTTOM;
}

DynamicDrawableSpan::DynamicDrawableSpan(
    /* [in] */ Int32 verticalAlignment)
{
    Init(verticalAlignment);
}

void DynamicDrawableSpan::Init(
    /* [in] */ Int32 verticalAlignment)
{
    mVerticalAlignment = verticalAlignment;
}

Int32 DynamicDrawableSpan::GetVerticalAlignment()
{
    return mVerticalAlignment;
}

Int32 DynamicDrawableSpan::GetSize(
    /* [in] */ IPaint* paint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaintFontMetricsInt* fm)
{
    AutoPtr<IDrawable> d = GetCachedDrawable();
    AutoPtr<IRect> rect;
    d->GetBounds( (IRect**)&rect );

    if (fm != NULL) {
        Int32 bottom;
        fm->SetAscent(-(rect->GetBottom(&bottom), bottom));
        fm->SetDescent(0);

        Int32 ascent;
        fm->SetTop((fm->GetAscent(&ascent), ascent));
        fm->SetBottom(0);
    }
    Int32 right;
    return (rect->GetRight(&right), right);
}

ECode DynamicDrawableSpan::Draw(
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
    AutoPtr<IDrawable> b = GetCachedDrawable();
    Int32 saveRet;
    canvas->Save(&saveRet);

    AutoPtr<IRect> rect;
    Int32 rbottom;
    Int32 transY = bottom - ((b->GetBounds( (IRect**)&rect ), rect)->GetBottom(&rbottom), rbottom);
    if (mVerticalAlignment == IDynamicDrawableSpan::ALIGN_BASELINE) {
        AutoPtr<IPaintFontMetricsInt> fm;
        paint->GetFontMetricsIntEx((IPaintFontMetricsInt**)&fm);
        Int32 descent;
        fm->GetDescent(&descent);
        transY -= descent;
    }

    canvas->Translate(x, transY);
    b->Draw(canvas);
    canvas->Restore();
    return NOERROR;
}

AutoPtr<IDrawable> DynamicDrawableSpan::GetCachedDrawable()
{
    AutoPtr<IDrawable> wr = mDrawableRef;
    AutoPtr<IDrawable> d = NULL;

    if (wr != NULL) {
        d = wr.Get();
    }

    if (d == NULL) {
        d = GetDrawable();
        mDrawableRef = d;
    }

    return d;
}


} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos