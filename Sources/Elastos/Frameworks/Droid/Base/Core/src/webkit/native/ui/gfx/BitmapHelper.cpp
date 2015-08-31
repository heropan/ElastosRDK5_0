// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

AutoPtr<IBitmap> BitmapHelper::CreateBitmap(
    /* in */ Int32 width,
    /* in */ Int32 height,
    /* in */ Int32 bitmapFormatValue)
{
    AutoPtr<IBitmap::Config> bitmapConfig = GetBitmapConfigForFormat(bitmapFormatValue);
    AutoPtr<IBitmap> bitmap;
    CBitmap::New(width, height, bitmapConfig, (IBitmap**)&bitmap);
    return bitmap;
}

AutoPtr<IBitmap> BitmapHelper::DecodeDrawableResource(
    /* in */ String name,
    /* in */ Int32 reqWidth,
    /* in */ Int32 reqHeight)
{
    AutoPtr<IResources> res;
    CResources::GetSystem((IResources**)&res);
    Int32 resId;
    res->GetIdentifier(name, String(""), String(""), &resId);
    if (resId == 0) return NULL;
    AutoPtr<IBitmapFactory::IOptions> options;
    CBitmapFactory::New((IBitmapFactory::IOptions**)&options);
    options->inJustDecodeBounds = TRUE;
    CBitmapFactory::DecodeResource(res, resId, options);
    options->InSampleSize = CalculateInSampleSize(options, reqWidth, reqHeight);
    options->InJustDecodeBounds = FALSE;
    options->InPreferredConfig = IBitmap::Config::ARGB_8888;
    AutoPtr<IBitmap> bitmap = CBitmapFactory::DecodeResource(res, resId, options);
    return bitmap;
}

Int32 BitmapHelper::CalculateInSampleSize(
    /* in */ IBitmapFactory::Options* options,
    /* in */ Int32 reqWidth,
    /* in */ Int32 reqHeight)
{
    const Int32 height = options->outHeight;
    const Int32 width = options->outWidth;
    Int32 inSampleSize = 1;
    if (height > reqHeight || width > reqWidth) {
        const Int32 heightRatio = Math::Round((float) height / (float) reqHeight);
        const Int32 widthRatio = Math::Round((float) width / (float) reqWidth);
        inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
    }
    return inSampleSize;
}

Int32 BitmapHelper::GetBitmapFormatForConfig(
    /* in */ IBitmap::Config* bitmapConfig)
{
    switch (bitmapConfig) {
        case ALPHA_8:
            return IBitmapFormat::FORMAT_ALPHA_8;
        case ARGB_4444:
            return IBitmapFormat::FORMAT_ARGB_4444;
        case ARGB_8888:
            return IBitmapFormat::FORMAT_ARGB_8888;
        case RGB_565:
            return IBitmapFormat::FORMAT_RGB_565;
        default:
            return IBitmapFormat::FORMAT_NO_CONFIG;
    }
}

AutoPtr<IBitmap::Config> BitmapHelper::GetBitmapConfigForFormat(
    /* in */ Int32 bitmapFormatValue)
{
    switch (bitmapFormatValue) {
        case IBitmapFormat::FORMAT_ALPHA_8:
            return IBitmap::Config::ALPHA_8;
        case IBitmapFormat::FORMAT_ARGB_4444:
            return IBitmap::Config::ARGB_4444;
        case IBitmapFormat::FORMAT_RGB_565:
            return IBitmap::Config::RGB_565;
        case IBitmapFormat::FORMAT_ARGB_8888:
        case IBitmapFormat::FORMAT_ARGB_8888:
        default:
            return IBitmap::Config::ARGB_8888;
    }
}

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

