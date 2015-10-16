
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/Movie.h"
#include "elastos/droid/graphics/Canvas.h"
#include "elastos/droid/graphics/Paint.h"
#include "elastos/droid/graphics/CreateOutputStreamAdaptor.h"
#include <skia/images/SkMovie.h>

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_INTERFACE_IMPL(Movie, Object, IMovie);
Movie::Movie(
    /* [in] */ Int64 nativeMovie)
{
    if (nativeMovie == 0) {
        // throw new RuntimeException("native movie creation failed");
        assert(0);
        return;
    }
    mNativeMovie = nativeMovie;
}

Movie::~Movie()
{
    NativeDestructor(mNativeMovie);
}

ECode Movie::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = ((SkMovie*)mNativeMovie)->width();
    return NOERROR;
}

ECode Movie::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = ((SkMovie*)mNativeMovie)->height();
    return NOERROR;
}

ECode Movie::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);

    *isOpaque = ((SkMovie*)mNativeMovie)->isOpaque();
    return NOERROR;
}

ECode Movie::GetDuration(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = ((SkMovie*)mNativeMovie)->duration();
    return NOERROR;
}


ECode Movie::SetTime(
    /* [in] */ Int32 relativeMilliseconds,
    /* [out] */ Boolean* isSet)
{
    VALIDATE_NOT_NULL(isSet);

    *isSet = ((SkMovie*)mNativeMovie)->setTime(relativeMilliseconds);
    return NOERROR;
}

ECode Movie::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(canvas != NULL);
    // its OK for paint to be null

    assert(0 && "TODO");
    // NPE_CHECK_RETURN_VOID(env, movie);
    // NPE_CHECK_RETURN_VOID(env, canvas);
    // // its OK for paint to be null

    // SkMovie* m = J2Movie(env, movie);
    // SkCanvas* c = GraphicsJNI::getNativeCanvas(env, canvas);
    // const SkBitmap& b = m->bitmap();
    // const SkPaint* p = jpaint ? GraphicsJNI::getNativePaint(env, jpaint) : NULL;

    // c->drawBitmap(b, fx, fy, p);
    return NOERROR;
}

ECode Movie::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return Draw(canvas, x, y, NULL);
}

static ECode CreateMovie(
    /* [in] */ SkMovie* moov,
    /* [out] */ IMovie** movie)
{
    if (NULL == moov) {
        *movie = NULL;
        return NOERROR;
    }

    *movie = new Movie((Int32)moov);
    REFCOUNT_ADD(*movie);
    return NOERROR;
}

ECode Movie::DecodeStream(
    /* [in] */ IInputStream* is,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    if (is == NULL) {
        *movie = NULL;
        return NOERROR;
    }

    assert(0 && "TODO");
    // if (is instanceof AssetManager.AssetInputStream) {
    //     final long asset = ((AssetManager.AssetInputStream) is).getNativeAsset();
    //     return nativeDecodeAsset(asset);
    // }

    *movie = NativeDecodeStream(is);
    REFCOUNT_ADD(*movie);
    return NOERROR;
}

ECode Movie::DecodeByteArray(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    Int32 totalLength = data->GetLength();
    if ((offset | length) < 0 || offset + length > totalLength) {
        // doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    SkMovie* moov = SkMovie::DecodeMemory(data->GetPayload() + offset, length);
    return CreateMovie(moov, movie);
}

ECode Movie::DecodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    AutoPtr<IInputStream> is;
    // try {
    FAIL_RETURN(CFileInputStream::New(pathName, (IFileInputStream**)&is));
    // }
    // catch (java.io.FileNotFoundException e) {
    //     return null;
    // }
    return DecodeTempStream(is, movie);
}

ECode Movie::DecodeTempStream(
    /* [in] */ IInputStream* is,
    /* [out] */IMovie** movie)
{
    // try {
    ECode ec = DecodeStream(is, movie);
    is->Close();
    // }
    // catch (java.io.IOException e) {
        // /*  do nothing.
            // If the exception happened on open, moov will be null.
            // If it happened on close, moov is still valid.
        // */
    // }
    return ec;
}

void Movie::NativeDestructor(
    /* [in] */ Int64 nativeMovie)
{
    SkMovie* movie = (SkMovie*) nativeMovie;
    delete movie;
}

AutoPtr<IMovie> Movie::NativeDecodeAsset(
    /* [in] */ Int64 asset)
{
    assert(0 && "TODO");
    // android::Asset* asset = reinterpret_cast<android::Asset*>(native_asset);
    // if (asset == NULL) return NULL;
    // SkAutoTUnref<SkStreamRewindable> stream (new android::AssetStreamAdaptor(asset,
    //         android::AssetStreamAdaptor::kNo_OwnAsset,
    //         android::AssetStreamAdaptor::kNo_HasMemoryBase));
    // SkMovie* moov = SkMovie::DecodeStream(stream.get());
    // return create_jmovie(env, moov);
    return NULL;
}

AutoPtr<IMovie> Movie::NativeDecodeStream(
    /* [in] */ IInputStream* is)
{
    assert(0 && "TODO");
    // NPE_CHECK_RETURN_ZERO(env, istream);

    // jbyteArray byteArray = env->NewByteArray(16*1024);
    // ScopedLocalRef<jbyteArray> scoper(env, byteArray);
    // SkStream* strm = CreateJavaInputStreamAdaptor(env, istream, byteArray);
    // if (NULL == strm) {
    //     return 0;
    // }

    // // Need to buffer enough input to be able to rewind as much as might be read by a decoder
    // // trying to determine the stream's format. The only decoder for movies is GIF, which
    // // will only read 6.
    // // FIXME: Get this number from SkImageDecoder
    // SkAutoTUnref<SkStreamRewindable> bufferedStream(SkFrontBufferedStream::Create(strm, 6));
    // SkASSERT(bufferedStream.get() != NULL);

    // SkMovie* moov = SkMovie::DecodeStream(bufferedStream);
    // strm->unref();
    // return create_jmovie(env, moov);
    return NULL;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
