
#include "elastos/droid/webkit/native/ui/gfx/BitmapHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

//=====================================================================
//                             BitmapHelper
//=====================================================================
AutoPtr<IBitmap> BitmapHelper::CreateBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 bitmapFormatValue)
{
    // ==================before translated======================
    // Bitmap.Config bitmapConfig = getBitmapConfigForFormat(bitmapFormatValue);
    // return Bitmap.createBitmap(width, height, bitmapConfig);
    assert(0);
    AutoPtr<IBitmap> empty;
    return empty;
}

AutoPtr<IBitmap> BitmapHelper::DecodeDrawableResource(
    /* [in] */ const String& name,
    /* [in] */ Int32 reqWidth,
    /* [in] */ Int32 reqHeight)
{
    // ==================before translated======================
    // Resources res = Resources.getSystem();
    // int resId = res.getIdentifier(name, null, null);
    // if (resId == 0) return null;
    //
    // final BitmapFactory.Options options = new BitmapFactory.Options();
    // options.inJustDecodeBounds = true;
    // BitmapFactory.decodeResource(res, resId, options);
    //
    // options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);
    // options.inJustDecodeBounds = false;
    // options.inPreferredConfig = Bitmap.Config.ARGB_8888;
    // return BitmapFactory.decodeResource(res, resId, options);
    assert(0);
    AutoPtr<IBitmap> empty;
    return empty;
}

Int32 BitmapHelper::CalculateInSampleSize(
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ Int32 reqWidth,
    /* [in] */ Int32 reqHeight)
{
    // ==================before translated======================
    // // Raw height and width of image
    // final int height = options.outHeight;
    // final int width = options.outWidth;
    // int inSampleSize = 1;
    //
    // if (height > reqHeight || width > reqWidth) {
    //
    //     // Calculate ratios of height and width to requested height and width
    //     final int heightRatio = Math.round((float) height / (float) reqHeight);
    //     final int widthRatio = Math.round((float) width / (float) reqWidth);
    //
    //     // Choose the smallest ratio as inSampleSize value, this will guarantee
    //     // a final image with both dimensions larger than or equal to the
    //     // requested height and width.
    //     inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
    // }
    //
    // return inSampleSize;
    assert(0);
    return 0;
}

Int32 BitmapHelper::GetBitmapFormatForConfig(
    /* [in] */ BitmapConfig bitmapConfig)
{
    // ==================before translated======================
    // switch (bitmapConfig) {
    //     case ALPHA_8:
    //         return BitmapFormat.FORMAT_ALPHA_8;
    //     case ARGB_4444:
    //         return BitmapFormat.FORMAT_ARGB_4444;
    //     case ARGB_8888:
    //         return BitmapFormat.FORMAT_ARGB_8888;
    //     case RGB_565:
    //         return BitmapFormat.FORMAT_RGB_565;
    //     default:
    //         return BitmapFormat.FORMAT_NO_CONFIG;
    // }
    assert(0);
    return 0;
}

BitmapConfig GetBitmapConfigForFormat(
    /* [in] */ Int32 bitmapFormatValue)
{
    // ==================before translated======================
    // switch (bitmapFormatValue) {
    //     case BitmapFormat.FORMAT_ALPHA_8:
    //         return Bitmap.Config.ALPHA_8;
    //     case BitmapFormat.FORMAT_ARGB_4444:
    //         return Bitmap.Config.ARGB_4444;
    //     case BitmapFormat.FORMAT_RGB_565:
    //         return Bitmap.Config.RGB_565;
    //     case BitmapFormat.FORMAT_ARGB_8888:
    //     default:
    //         return Bitmap.Config.ARGB_8888;
    // }
    assert(0);
    BitmapConfig empty;
    return empty;
}

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


