
#include "graphics/drawable/PictureDrawable.h"
#include "graphics/CRect.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

PictureDrawable::PictureDrawable()
{}

PictureDrawable::PictureDrawable(
    /* [in] */ IPicture* picture)
    : mPicture(picture)
{}

AutoPtr<IPicture> PictureDrawable::GetPicture()
{
    return mPicture;
}

ECode PictureDrawable::SetPicture(
    /* [in] */ IPicture* picture)
{
    mPicture = picture;
    return NOERROR;
}

ECode PictureDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mPicture != NULL) {
        AutoPtr<IRect> bounds = GetBounds();
        Int32 count;
        canvas->Save(&count);
        Boolean isNotEmpty;
        canvas->ClipRectEx3(bounds, &isNotEmpty);
        canvas->Translate(
                ((CRect*)bounds.Get())->mLeft, ((CRect*)bounds.Get())->mTop);
        canvas->DrawPicture(mPicture);
        canvas->Restore();
    }
    return NOERROR;
}

Int32 PictureDrawable::GetIntrinsicWidth()
{
    Int32 width = -1;
    if (mPicture != NULL) mPicture->GetWidth(&width);
    return width;
}

Int32 PictureDrawable::GetIntrinsicHeight()
{
    Int32 height = -1;
    if (mPicture != NULL) mPicture->GetHeight(&height);
    return height;
}

Int32 PictureDrawable::GetOpacity()
{
    // not sure, so be safe
    return IPixelFormat::TRANSLUCENT;
}

ECode PictureDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return NOERROR;
}

ECode PictureDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    return NOERROR;
}

ECode PictureDrawable::SetColorFilter(
    /* [in] */ IColorFilter* colorFilter)
{
    return NOERROR;
}

ECode PictureDrawable::SetAlpha(
    /* [in */ Int32 alpha)
{
    return NOERROR;
}

ECode PictureDrawable::Init(
    /* [in] */ IPicture* picture)
{
    mPicture = picture;
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
