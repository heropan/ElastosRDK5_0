
#ifndef __ELASTOS_DROID_GRAPHICS_CBITMAPFACTORY_H__
#define __ELASTOS_DROID_GRAPHICS_CBITMAPFACTORY_H__

#include "_Elastos_Droid_Graphics_CBitmapFactory.h"
#include <Elastos.CoreLibrary.h>
#include <elastos/core/Singleton.h>

using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Core::Singleton;
using Elastos::IO::IInputStream;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CBitmapFactory)
    , public Singleton
    , public IBitmapFactory
{
public:
    CAR_INTERFACE_DECL();

    CAR_SINGLETON_DECL();

    CARAPI SetDefaultDensity(
        /* [in] */ Int32 density);

    CARAPI CreateScaledBitmap(
        /* [in] */ IBitmap* src,
        /* [in] */ Int32 dstWidth,
        /* [in] */ Int32 dstHeight,
        /* [in] */ Boolean filter,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns an immutable bitmap from the source bitmap. The new bitmap may
     * be the same object as source, or a copy may have been made.  It is
     * initialized with the same density as the original bitmap.
     */
    CARAPI CreateBitmap(
        /* [in] */ IBitmap* source,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns an immutable bitmap from the specified subset of the source
     * bitmap. The new bitmap may be the same object as source, or a copy may
     * have been made. It is initialized with the same density as the original
     * bitmap.
     *
     * @param source   The bitmap we are subsetting
     * @param x        The x coordinate of the first pixel in source
     * @param y        The y coordinate of the first pixel in source
     * @param width    The number of pixels in each row
     * @param height   The number of rows
     * @return A copy of a subset of the source bitmap or the source bitmap itself.
     */
    CARAPI CreateBitmap(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns an immutable bitmap from subset of the source bitmap,
     * transformed by the optional matrix. The new bitmap may be the
     * same object as source, or a copy may have been made. It is
     * initialized with the same density as the original bitmap.
     *
     * If the source bitmap is immutable and the requested subset is the
     * same as the source bitmap itself, then the source bitmap is
     * returned and no new bitmap is created.
     *
     * @param source   The bitmap we are subsetting
     * @param x        The x coordinate of the first pixel in source
     * @param y        The y coordinate of the first pixel in source
     * @param width    The number of pixels in each row
     * @param height   The number of rows
     * @param m        Optional matrix to be applied to the pixels
     * @param filter   true if the source should be filtered.
     *                   Only applies if the matrix contains more than just
     *                   translation.
     * @return A bitmap that represents the specified subset of source
     * @throws IllegalArgumentException if the x, y, width, height values are
     *         outside of the dimensions of the source bitmap.
     */
    CARAPI CreateBitmap(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IMatrix* m,
        /* [in] */ Boolean filter,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a mutable bitmap with the specified width and height.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create.
     * @throws IllegalArgumentException if the width or height are <= 0
     */
    CARAPI CreateBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param colors   Array of {@link Color} used to initialize the pixels.
     * @param offset   Number of values to skip before the first color in the
     *                 array of colors.
     * @param stride   Number of colors in the array between rows (must be >=
     *                 width or <= -width).
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    CARAPI CreateBitmap(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param colors   Array of {@link Color} used to initialize the pixels.
     *                 This array must be at least as large as width * height.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    CARAPI CreateBitmap(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a mutable bitmap with the specified width and height.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create.
     * @throws IllegalArgumentException if the width or height are <= 0
     */
    CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param colors   Array of {@link Color} used to initialize the pixels.
     * @param offset   Number of values to skip before the first color in the
     *                 array of colors.
     * @param stride   Number of colors in the array between rows (must be >=
     *                 width or <= -width).
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param colors   Array of {@link Color} used to initialize the pixels.
     *                 This array must be at least as large as width * height.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    CARAPI ScaleFromDensity(
        /* [in] */ Int32 size,
        /* [in] */ Int32 sdensity,
        /* [in] */ Int32 tdensity,
        /* [out] */ Int32* retSize);

    CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeResourceStream(
        /* [in] */ IResources* res,
        /* [in] */ ITypedValue* value,
        /* [in] */ IInputStream* is,
        /* [in] */ IRect* pad,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeResource(
        /* [in] */ IResources* res,
        /* [in] */ Int32 id,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeResource(
        /* [in] */ IResources* res,
        /* [in] */ Int32 id,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    /**
     * Decode an input stream into a bitmap. If the input stream is null, or
     * cannot be used to decode a bitmap, the function returns null.
     * The stream's position will be where ever it was after the encoded data
     * was read.
     *
     * @param is The input stream that holds the raw data to be decoded into a
     *           bitmap.
     * @return The decoded bitmap, or null if the image data could not be decoded.
     */
    CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeFileDescriptor(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeFileDescriptor(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ IBitmap** bitmap);

private:
    static CARAPI_(AutoPtr<IBitmap>) FinishDecode(
        /* [in] */ IBitmap* bm,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* opts);

    static CARAPI_(AutoPtr<IBitmap>) NativeDecodeStream(
        /* [in] */ IInputStream* is,
        /* [in] */ ArrayOf<Byte>* storage,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts);

    static CARAPI_(AutoPtr<IBitmap>) NativeDecodeStream(
        /* [in] */ IInputStream* is,
        /* [in] */ ArrayOf<Byte>* storage,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [in] */ Boolean applyScale,
        /* [in] */ Float scale);

    static CARAPI_(AutoPtr<IBitmap>) NativeDecodeFileDescriptor(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts);

    static CARAPI_(AutoPtr<IBitmap>) NativeDecodeAsset(
        /* [in] */ Int32 asset,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts);

    static CARAPI_(AutoPtr<IBitmap>) NativeDecodeAsset(
        /* [in] */ Int32 asset,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [in] */ Boolean applyScale,
        /* [in] */ Float scale);

    static CARAPI_(AutoPtr<IBitmap>) NativeDecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IBitmapFactoryOptions* opts);

    static CARAPI_(void) NativeScaleNinePatch(
        /* [in] */ const ArrayOf<Byte>& chunkObject,
        /* [in] */ Float scale,
        /* [in] */ IRect* padding);

    static CARAPI_(Boolean) NativeIsSeekable(
        /* [in] */ IFileDescriptor* fd);

private:
    static const Int32 DECODE_BUFFER_SIZE;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CBITMAPFACTORY_H__

