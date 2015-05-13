
#ifndef __HH_NINEPATCH_H
#define __HH_NINEPATCH_H

#include "ext/frameworkext.h"
#include "Elastos.Droid.Core_server.h"


namespace Elastos {
namespace Droid {
namespace Graphics {

class NinePatch
{
public:
    NinePatch();

    virtual CARAPI SetPaint(
        /* [in] */ IPaint* p);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRectF* location);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location);

    /**
     * Draw a bitmap of nine patches.
     *
     * @param canvas    A container for the current matrix and clip used to draw the bitmap.
     * @param location  Where to draw the bitmap.
     * @param paint     The Paint to draw through.
     */
    virtual CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* location,
        /* [in] */ IPaint* paint);

    /**
     * Return the underlying bitmap's density, as per
     * {@link Bitmap#getDensity() Bitmap.getDensity()}.
     */
    virtual CARAPI GetDensity(
        /* [out] */ Int32* density);

    virtual CARAPI GetWidth(
        /* [out] */ Int32* width);

    virtual CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI GetTransparentRegion(
        /* [in] */ IRect* location,
        /* [out] */ IRegion** region);

    static CARAPI_(Boolean) IsNinePatchChunk(
        /* [in] */ ArrayOf<Byte>* chunk);

protected:
    CARAPI Init(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ const String& srcName);

    CARAPI Init(
        /* [in] */ NinePatch* patch);

private:
    static CARAPI ValidateNinePatchChunk(
        /* [in] */ Int32 bitmap,
        /* [in] */ ArrayOf<Byte>* chunk);

    static CARAPI NativeDraw(
        /* [in] */ Int32 canvas,
        /* [in] */ IRectF* loc,
        /* [in] */ Int32 bitmap,
        /* [in] */ ArrayOf<Byte>* c,
        /* [in] */ Int32 paint, // may be null
        /* [in] */ Int32 destDensity,
        /* [in] */ Int32 srcDensity);

    static CARAPI NativeDraw(
        /* [in] */ Int32 canvas,
        /* [in] */ IRect* boundsRect,
        /* [in] */ Int32 bitmap,
        /* [in] */ ArrayOf<Byte>* c,
        /* [in] */ Int32 paint, // may be null
        /* [in] */ Int32 destDensity,
        /* [in] */ Int32 srcDensity);

    static CARAPI_(Int32) NativeGetTransparentRegion(
        /* [in] */ Int32 bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* boundsRect);

private:
    AutoPtr<IBitmap> mBitmap;
    AutoPtr< ArrayOf<Byte> > mChunk;
    AutoPtr<IPaint> mPaint;
    String mSrcName;  // Useful for debugging
    AutoPtr<IRectF> mRect;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __HH_NINEPATCH_H
