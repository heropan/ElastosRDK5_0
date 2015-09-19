
#include "ext/frameworkext.h"
#include "graphics/Movie.h"
#include "graphics/Canvas.h"
#include "graphics/Paint.h"
#include "graphics/CreateOutputStreamAdaptor.h"
#include <skia/images/SkMovie.h>


using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

Movie::Movie(
    /* [in] */ Int32 nativeMovie)
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

PInterface Movie::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IMovie) {
        return (IMovie*)this;
    }
    return NULL;
}

UInt32 Movie::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Movie::Release()
{
    return ElRefBase::Release();
}

ECode Movie::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
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

    SkMovie* m = (SkMovie*)mNativeMovie;
    SkCanvas* c = (SkCanvas*)((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas;
    SkScalar sx = SkFloatToScalar(x);
    SkScalar sy = SkFloatToScalar(y);
    const SkBitmap& b = m->bitmap();
    const SkPaint* p = paint ? (SkPaint*)((Paint*)paint->Probe(EIID_Paint))->mNativePaint : NULL;

    c->drawBitmap(b, sx, sy, p);
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
        return E_NULL_POINTER_EXCEPTION;
    }
    // what is the lifetime of the array? Can the skstream hold onto it?
    AutoPtr< ArrayOf<Byte> > adaptor = ArrayOf<Byte>::Alloc(16 * 1024);
    SkStream* strm = CreateInputStreamAdaptor(is, adaptor.Get());

    if (NULL == strm) {
        *movie = NULL;
        return NOERROR;
    }

    SkMovie* moov = SkMovie::DecodeStream(strm);
    strm->unref();
    return CreateMovie(moov, movie);
}

ECode Movie::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    Int32 totalLength = data.GetLength();
    if ((offset | length) < 0 || offset + length > totalLength) {
        // doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    SkMovie* moov = SkMovie::DecodeMemory(data.GetPayload() + offset, length);
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
    /* [in] */ Int32 nativeMovie)
{
    delete (SkMovie*)nativeMovie;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
