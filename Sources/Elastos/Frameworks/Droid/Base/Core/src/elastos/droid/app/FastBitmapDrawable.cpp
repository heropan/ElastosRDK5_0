#include "elastos/droid/app/FastBitmapDrawable.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CPorterDuffXfermode.h"

using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IPorterDuffXfermode;
using Elastos::Droid::Graphics::CPorterDuffXfermode;
using Elastos::Droid::Graphics::IPixelFormat;

namespace Elastos{
namespace Droid{
namespace App{

FastBitmapDrawable::FastBitmapDrawable()
    : mWidth(0)
    , mHeight(0)
    , mDrawLeft(0)
    , mDrawTop(0)
{
}

ECode FastBitmapDrawable::Init(
    /* [in] */ IBitmap* bitmap)
{
    mBitmap = bitmap;
    bitmap->GetWidth(&mWidth);
    bitmap->GetHeight(&mHeight);

    SetBounds(0, 0, mWidth, mHeight);

    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
    AutoPtr<IPorterDuffXfermode> mode;
    ASSERT_SUCCEEDED(CPorterDuffXfermode::New(Elastos::Droid::Graphics::PorterDuffMode_SRC,
            (IPorterDuffXfermode**)&mode));
    mPaint->SetXfermode(mode);
    return NOERROR;
}

ECode FastBitmapDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    return canvas->DrawBitmap(mBitmap, (Float)mDrawLeft, (Float)mDrawTop, mPaint);
}

Int32 FastBitmapDrawable::GetOpacity()
{
    return IPixelFormat::OPAQUE;
}

ECode FastBitmapDrawable::SetBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mDrawLeft = left + (right-left - mWidth) / 2;
    mDrawTop = top + (bottom-top - mHeight) / 2;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
