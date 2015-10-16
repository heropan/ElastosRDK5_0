
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/CPicture.h"
#include "elastos/droid/graphics/SkiaIOStreamAdaptor.h"
#include <skia/core/SkPicture.h>
#include <skia/core/SkCanvas.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CPicture::RecordingCanvas::RecordingCanvas(
    /* [in] */ CPicture* pict,
    /* [in] */ Int64 nativeCanvas)
{
    Canvas::constructor(nativeCanvas);
    mPicture = pict;
}

ECode CPicture::RecordingCanvas::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
//    throw new RuntimeException(
//            "Cannot call setBitmap on a picture canvas");
    return E_RUNTIME_EXCEPTION;
}

ECode CPicture::RecordingCanvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    if (mPicture == picture) {
//        throw new RuntimeException(
//                    "Cannot draw a picture into its recording canvas");
        return E_RUNTIME_EXCEPTION;
    }
    return Canvas::DrawPicture(picture);
}

////////////////////////////////////////////////////////////
// CPicture
const Int32 CPicture::WORKING_STREAM_STORAGE = 16 * 1024;

CAR_OBJECT_IMPL(CPicture);
CAR_INTERFACE_IMPL(CPicture, Object, IPicture);
CPicture::CPicture()
    : mNativePicture(0)
{}

CPicture::~CPicture()
{
    NativeDestructor(mNativePicture);
}

ECode CPicture::constructor()
{
    return constructor(NativeConstructor(0));
}

ECode CPicture::constructor(
    /* [in] */ IPicture* src)
{
    return constructor(NativeConstructor(
            src != NULL ? ((CPicture*)src)->mNativePicture : NULL));
}

ECode CPicture::constructor(
    /* [in] */ Int64 nativePicture)
{
    if (nativePicture == 0) {
//        throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    mNativePicture = nativePicture;
    return NOERROR;
}

ECode CPicture::BeginRecording(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    Int64 ni = NativeBeginRecording(mNativePicture, width, height);
    mRecordingCanvas = new RecordingCanvas(this, ni);
    *canvas = mRecordingCanvas;
    REFCOUNT_ADD(*canvas);
    return NOERROR;
}

ECode CPicture::EndRecording()
{
    if (mRecordingCanvas != NULL) {
        mRecordingCanvas = NULL;
        NativeEndRecording(mNativePicture);
    }
    return NOERROR;
}

ECode CPicture::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = NativeGetWidth(mNativePicture);
    return NOERROR;
}

ECode CPicture::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = NativeGetHeight(mNativePicture);
    return NOERROR;
}

ECode CPicture::Draw(
    /* [in] */ ICanvas* canvas)
{
    Boolean is = FALSE;
    if (canvas->IsHardwareAccelerated(&is), is) {
        // throw new IllegalArgumentException(
        //         "Picture playback is only supported on software canvas.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mRecordingCanvas != NULL) {
        EndRecording();
    }
    NativeDraw(((Canvas*)(ICanvas*)canvas->Probe(EIID_Canvas))->mNativeCanvasWrapper,
            mNativePicture);
    return NOERROR;
}

AutoPtr<IPicture> CPicture::CreateFromStream(
    /* [in] */ IInputStream* stream)
{
    AutoPtr<IPicture> picture;
    AutoPtr< ArrayOf<Byte> > storage = ArrayOf<Byte>::Alloc(WORKING_STREAM_STORAGE);
    Int64 ni = NativeCreateFromStream(stream, storage.Get());
    assert(0 && "TODO");
    // CPicture::New((Int64)ni, (IPicture**)&picture);
    return picture;
}

ECode CPicture::WriteToStream(
    /* [in] */ IOutputStream* stream)
{
    // do explicit check before calling the native method
    if (stream == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr< ArrayOf<Byte> > storage = ArrayOf<Byte>::Alloc(WORKING_STREAM_STORAGE);
    if (!NativeWriteToStream(mNativePicture, stream, storage.Get())) {
//        throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CPicture::GetNativePicture(
    /* [out] */ Handle32* nativePicture)
{
    VALIDATE_NOT_NULL(nativePicture);
    *nativePicture = mNativePicture;
    return NOERROR;
}

Int64 CPicture::NativeConstructor(
    /* [in] */ Int64 nativeSrcOr0)
{
    if (nativeSrcOr0) {
        return (Int64)new SkPicture(*(SkPicture*)nativeSrcOr0);
    }
    else {
        return (Int64)new SkPicture;
    }
}

Int64 CPicture::NativeCreateFromStream(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    assert(0 && "TODO");
    // Picture* picture = NULL;
    // SkStream* strm = CreateSkiaInputStreamAdaptor(env, jstream, jstorage);
    // if (strm) {
    //     picture = Picture::CreateFromStream(strm);
    //     delete strm;
    // }
    // return reinterpret_cast<Int64>(picture);
    return NOERROR;
}

Int32 CPicture::NativeGetWidth(
    /* [in] */ Int64 nativePicture)
{
    assert(0 && "TODO");
    // Picture* pict = reinterpret_cast<Picture*>(nativePicture);
    // return static_cast<Int32>(pict->width());
    return -1;
}

Int32 CPicture::NativeGetHeight(
    /* [in] */ Int64 nativePicture)
{
    assert(0 && "TODO");
    // Picture* pict = reinterpret_cast<Picture*>(nativePicture);
    // return static_cast<Int32>(pict->height());
    return -1;
}

Int64 CPicture::NativeBeginRecording(
    /* [in] */ Int64 nativePicture,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    assert(0 && "TODO");
    // Picture* pict = reinterpret_cast<Picture*>(nativePicture);
    // Canvas* canvas = pict->beginRecording(w, h);
    // return reinterpret_cast<Int64>(canvas);
    return -1;
}

void CPicture::NativeEndRecording(
    /* [in] */ Int64 nativePicture)
{
    assert(0 && "TODO");
    // Picture* pict = reinterpret_cast<Picture*>(nativePicture);
    // pict->endRecording();
}

void CPicture::NativeDraw(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Int64 nativePicture)
{
    assert(0 && "TODO");
    // Canvas* canvas = reinterpret_cast<Canvas*>(nativeCanvas);
    // Picture* picture = reinterpret_cast<Picture*>(nativePicture);
    // SkASSERT(canvas);
    // SkASSERT(picture);
    // picture->draw(canvas);
}

Boolean CPicture::NativeWriteToStream(
    /* [in] */ Int64 nativePicture,
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    assert(0 && "TODO");
    // Picture* picture = reinterpret_cast<Picture*>(nativePicture);
    // SkWStream* strm = CreateSkiaOutputStreamAdaptor(stream, storage);

    // if (NULL != strm) {
    //     picture->serialize(strm);
    //     delete strm;
    //     return TRUE;
    // }
    return FALSE;
}

void CPicture::NativeDestructor(
    /* [in] */ Int64 nativePicture)
{
    assert(0 && "TODO");
    // Picture* picture = reinterpret_cast<Picture*>(nativePicture);
    // SkASSERT(picture);
    // delete picture;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
