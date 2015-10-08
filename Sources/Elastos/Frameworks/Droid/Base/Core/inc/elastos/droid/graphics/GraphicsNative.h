#ifndef __ELASTOS_DROID_GRAPHICS_GRAPHICSNATIVE_H__
#define __ELASTOS_DROID_GRAPHICS_GRAPHICSNATIVE_H__

#include "graphics/CBitmap.h"
#include <skia/core/SkBitmap.h>
#include <skia/core/SkRect.h>
#include <skia/core/SkMallocPixelRef.h>
// #include <skia/images/SkBitmapRegionDecoder.h>


namespace Elastos {
namespace Droid {
namespace Graphics {

class GraphicsNative
{
public:
    enum BitmapCreateFlags {
        kBitmapCreateFlag_None = 0x0,
        kBitmapCreateFlag_Mutable = 0x1,
        kBitmapCreateFlag_Premultiplied = 0x2,
    };

    // class DroidPixelRef : public SkMallocPixelRef
    // {
    // public:
    //     DroidPixelRef(
    //         /* [in] */ void* storage,
    //         /* [in] */ size_t size,
    //         /* [in] */ ArrayOf<Byte>* storageObj,
    //         /* [in] */ SkColorTable* ctable);

    //     virtual ~DroidPixelRef();

    //     AutoPtr< ArrayOf<Byte> > getStorageObj() { return mStorageObj; }

    //     void setLocalRef(
    //         /* [in] */ ArrayOf<Byte>* arr);

        /** Used to hold a ref to the pixels when the Java bitmap may be collected.
         *  If specified, 'localref' is a valid JNI local reference to the byte array
         *  containing the pixel data.
         *
         *  'localref' may only be NULL if setLocalJNIRef() was already called with
         *  a JNI local ref that is still valid.
         */
    //     virtual void globalRef(
    //         /* [in] */ void* localref=NULL);

    //     /** Release a ref that was acquired using globalRef(). */
    //     virtual void globalUnref();

    // private:
    //     bool mOnDroidHeap; // If true, the memory was allocated on the Java heap

    //     AutoPtr< ArrayOf<Byte> > mStorageObj; // The Java byte[] object used as the bitmap backing store
    //     bool mHasGlobalRef; // If true, fStorageObj holds a JNI global ref

    //     mutable int32_t mGlobalRefCnt;
    // };

    class DroidPixelAllocator : public SkBitmap::Allocator
    {
    public:
        DroidPixelAllocator();

        // overrides
        virtual bool allocPixelRef(
            /* [in] */ SkBitmap* bitmap,
            /* [in] */ SkColorTable* ctable);

        /** Return the Java array object created for the last allocation.
         *  This returns a local JNI reference which the caller is responsible
         *  for storing appropriately (usually by passing it to the Bitmap
         *  constructor).
         */
        AutoPtr< ArrayOf<Byte> > getStorageObj() { return mStorageObj; }

        /** Same as getStorageObj(), but also resets the allocator so that it
         *  can allocate again.
         */
        AutoPtr< ArrayOf<Byte> > getStorageObjAndReset()
        {
            AutoPtr< ArrayOf<Byte> > result = mStorageObj;
            mStorageObj = NULL;
            mAllocCount = 0;
            return result;
        }

    private:
        AutoPtr< ArrayOf<Byte> > mStorageObj;
        Int32 mAllocCount;
    };

public:
    static CARAPI_(SkRect*) IRect2SkRect(
        /* [in] */ IRect* obj,
        /* [in] */ SkRect* r);

    static CARAPI_(SkIRect*) IRect2SkIRect(
        /* [in] */ IRect* obj,
        /* [in] */ SkIRect* sr);

    static CARAPI_(void) SkIRect2IRect(
        /* [in] */ const SkIRect& ir,
        /* [in] */ IRect* obj);

    static CARAPI_(SkRect*) IRectF2SkRect(
        /* [in] */ IRectF* obj,
        /* [in] */ SkRect* r);

    static CARAPI_(void) SkRect2IRectF(
        /* [in] */ const SkRect& r,
        /* [in] */ IRectF* obj);

    static CARAPI_(void) SkIPoint2IPoint(
        /* [in] */ const SkIPoint& point,
        /* [in] */ IPoint* obj);

    static CARAPI_(SkBitmap*) GetNativeBitmap(
        /* [in] */ IBitmap* bitmap);

    static CARAPI_(SkBitmap::Config) GetNativeBitmapConfig(
        /* [in] */ BitmapConfig config);

    /*
     *  LegacyBitmapConfig is the old enum in Skia that matched the enum int values
     *  in Bitmap.Config. Skia no longer supports this config, but has replaced it
     *  with SkColorType. These routines convert between the two.
     */
    static CARAPI_(SkColorType) LegacyBitmapConfigToColorType(
        /* [in] */ Int32 legacyConfig);

    static CARAPI_(Int32) ColorTypeToLegacyBitmapConfig(
        /* [in] */ SkColorType colorType);

    static CARAPI_(SkColorType) GetNativeBitmapColorType(
        /* [in] */ BitmapConfig config);

    static CARAPI CreateBitmap(
        /* [in] */ SkBitmap* dst,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninepatch,
        /* [in] */ ArrayOf<Int32>* layoutbounds,
        /* [in] */ Int32 density,
        /* [out] */ CBitmap** bitmap);

    static CARAPI CreateBitmap(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 bitmapCreateFlags,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ INinePatchInsetStruct* ninePatchInsets,
        /* [in] */ Int32 density,
        /* [in, out] */ CBitmap* bitmapObj);

    // static CARAPI CreateBitmapRegionDecoder(
    //     /* [in] */ SkBitmapRegionDecoder* bitmap,
    //     /* [out] */ IBitmapRegionDecoder** decoder);

    static CARAPI AllocateDroidPixelRef(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ SkColorTable* ctable,
        /* [out] */ ArrayOf<Byte>** pixelRef);

    static CARAPI_(Boolean) SetPixels(
        /* [in] */ ArrayOf<Int32>* srcColors,
        /* [in] */ Int32 srcOffset,
        /* [in] */ Int32 srcStride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ const SkBitmap& dstBitmap);
};

} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_GRAPHICSNATIVE_H__
