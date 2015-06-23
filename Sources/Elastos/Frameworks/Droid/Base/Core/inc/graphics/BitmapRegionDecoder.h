
#ifndef __BITMAPREGIONDECODER_H__
#define __BITMAPREGIONDECODER_H__

#include "Elastos.Droid.Core_server.h"
#include <Elastos.CoreLibrary.h>

using Elastos::Core::Mutex;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

class BitmapRegionDecoder
    : public ElRefBase
    , public IBitmapRegionDecoder
{
    friend class GraphicsNative;

public:
    ~BitmapRegionDecoder();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID);

    /**
     * Create a BitmapRegionDecoder from the specified byte array.
     * Currently only the JPEG and PNG formats are supported.
     *
     * @param data byte array of compressed image data.
     * @param offset offset into data for where the decoder should begin
     *               parsing.
     * @param length the number of bytes, beginning at offset, to parse
     * @param isShareable If this is true, then the BitmapRegionDecoder may keep a
     *                    shallow reference to the input. If this is false,
     *                    then the BitmapRegionDecoder will explicitly make a copy of the
     *                    input data, and keep that. Even if sharing is allowed,
     *                    the implementation may still decide to make a deep
     *                    copy of the input data. If an image is progressively encoded,
     *                    allowing sharing may degrade the decoding speed.
     * @return BitmapRegionDecoder, or null if the image data could not be decoded.
     * @throws IOException if the image format is not supported or can not be decoded.
     */
    static CARAPI NewInstance(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NewInstance(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NewInstance(
        /* [in] */ IInputStream* is,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NewInstance(
        /* [in] */ const String& pathName,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    /**
     * Decodes a rectangle region in the image specified by rect.
     *
     * @param rect The rectangle that specified the region to be decode.
     * @param options null-ok; Options that control downsampling.
     *             inPurgeable is not supported.
     * @return The decoded bitmap, or null if the image data could not be
     *         decoded.
     */
    CARAPI DecodeRegion(
        /* [in] */ IRect* rect,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    /** Returns the original image's width */
    CARAPI GetWidth(
        /* [out] */ Int32* width);

    /** Returns the original image's height */
    CARAPI GetHeight(
        /* [out] */ Int32* height);

    /**
     * Frees up the memory associated with this region decoder, and mark the
     * region decoder as "dead", meaning it will throw an exception if decodeRegion(),
     * getWidth() or getHeight() is called.
     *
     * <p>This operation cannot be reversed, so it should only be called if you are
     * sure there are no further uses for the region decoder. This is an advanced call,
     * and normally need not be called, since the normal GC process will free up this
     * memory when there are no more references to this region decoder.
     */
    CARAPI Recycle();

    /**
     * Returns true if this region decoder has been recycled.
     * If so, then it is an error to try use its method.
     *
     * @return true if the region decoder has been recycled
     */

    CARAPI IsRecycled(
        /* [out] */ Boolean* result);

private:
    BitmapRegionDecoder(
        /* [in] */ Int32 decoder);

    CARAPI CheckRecycled(
        /* [in] */ CString errorMessage);

    static CARAPI NativeDecodeRegion(
        /* [in] */ Int32 brd,
        /* [in] */ Int32 start_x,
        /* [in] */ Int32 start_y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    static CARAPI_(Int32) NativeGetWidth(
        /* [in] */ Int32 brd);

    static CARAPI_(Int32) NativeGetHeight(
        /* [in] */ Int32 brd);

    static CARAPI_(void) NativeClean(
        /* [in] */ Int32 brd);

    static CARAPI NativeNewInstance(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NativeNewInstance(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NativeNewInstance(
        /* [in] */ IInputStream* is,
        /* [in] */ ArrayOf<Byte>* storage,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI NativeNewInstance(
        /* [in] */ Int32 nativeAsset,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

private:
    Int32 mNativeBitmapRegionDecoder;
    Boolean mRecycled;
    Mutex mNativeLock;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __BITMAPREGIONDECODER_H__
