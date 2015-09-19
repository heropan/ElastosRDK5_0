
#include "ext/frameworkext.h"
#include "graphics/CPicture.h"
#include "graphics/SkiaIOStreamAdaptor.h"
#include <skia/core/SkPicture.h>
#include <skia/core/SkCanvas.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CPicture::_RecordingCanvas::_RecordingCanvas(
    /* [in] */ CPicture* pict,
    /* [in] */ Int32 nativeCanvas)
{
    Canvas::Init(nativeCanvas);
    mPicture = pict;
}

ECode CPicture::_RecordingCanvas::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
//    throw new RuntimeException(
//            "Cannot call setBitmap on a picture canvas");
    return E_RUNTIME_EXCEPTION;
}

ECode CPicture::_RecordingCanvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    if (mPicture == picture) {
//        throw new RuntimeException(
//                    "Cannot draw a picture into its recording canvas");
        return E_RUNTIME_EXCEPTION;
    }
    return Canvas::DrawPicture(picture);
}

CPicture::RecordingCanvas::RecordingCanvas(
    /* [in] */ CPicture* pict,
    /* [in] */ Int32 nativeCanvas)
    : _RecordingCanvas(pict, nativeCanvas)
{
}

ICANVAS_METHODS_IMPL(CPicture::RecordingCanvas, CPicture::_RecordingCanvas);

PInterface CPicture::RecordingCanvas::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICanvas*)this;
    }
    else if (riid == EIID_Canvas) {
        return reinterpret_cast<PInterface>((Canvas*)this);
    }
    else if (riid == EIID_ICanvas) {
        return (ICanvas*)this;
    }

    return NULL;
}

UInt32 CPicture::RecordingCanvas::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CPicture::RecordingCanvas::Release()
{
    return ElRefBase::Release();
}

ECode CPicture::RecordingCanvas::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICanvas*)this) {
        *pIID = EIID_ICanvas;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

////////////////////////////////////////////////////////////
// CPicture
const Int32 CPicture::WORKING_STREAM_STORAGE;

CPicture::CPicture()
    : mCreatedFromStream(FALSE)
    , mNativePicture(0)
{}

CPicture::~CPicture()
{
    NativeDestructor(mNativePicture);
}

ECode CPicture::constructor()
{
    return constructor(NativeConstructor(0), FALSE);
}

ECode CPicture::constructor(
    /* [in] */ IPicture* src)
{
    return constructor(NativeConstructor(
            src != NULL ? ((CPicture*)src)->mNativePicture : NULL), FALSE);
}

ECode CPicture::constructor(
    /* [in] */ Int32 nativePicture,
    /* [in] */ Boolean fromStream)
{
    if (nativePicture == 0) {
//        throw new RuntimeException();
        return E_RUNTIME_EXCEPTION;
    }
    mNativePicture = nativePicture;
    mCreatedFromStream = fromStream;
    return NOERROR;
}

ECode CPicture::BeginRecording(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);
    Int32 ni = NativeBeginRecording(mNativePicture, width, height);
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
    *width = ((SkPicture*)mNativePicture)->width();
    return NOERROR;
}

ECode CPicture::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = ((SkPicture*)mNativePicture)->height();
    return NOERROR;
}

ECode CPicture::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mRecordingCanvas != NULL) {
        EndRecording();
    }
    NativeDraw(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas,
            mNativePicture);
    return NOERROR;
}

AutoPtr<IPicture> CPicture::CreateFromStream(
    /* [in] */ IInputStream* stream)
{
    AutoPtr<IPicture> picture;
    AutoPtr< ArrayOf<Byte> > storage = ArrayOf<Byte>::Alloc(WORKING_STREAM_STORAGE);
    Int32 ni = NativeCreateFromStream(stream, storage.Get());
    CPicture::New(ni, TRUE, (IPicture**)&picture);
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

Int32 CPicture::Ni()
{
    return mNativePicture;
}

Int32 CPicture::NativeConstructor(
    /* [in] */ Int32 nativeSrcOr0)
{
    if (nativeSrcOr0) {
        return (Int32)new SkPicture(*(SkPicture*)nativeSrcOr0);
    }
    else {
        return (Int32)new SkPicture;
    }
}

Int32 CPicture::NativeCreateFromStream(
    /* [in] */ IInputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    SkPicture* picture = NULL;
    SkStream* strm = CreateSkiaInputStreamAdaptor(stream, storage);
    if (strm) {
        picture = new SkPicture(strm);
        delete strm;
    }
    return (Int32)picture;
}

Int32 CPicture::NativeBeginRecording(
    /* [in] */ Int32 nativePicture,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    // beginRecording does not ref its return value, it just returns it.
    SkCanvas* canvas = ((SkPicture*)nativePicture)->beginRecording(w, h);
    // the java side will wrap this guy in a Canvas.java, which will call
    // unref in its finalizer, so we have to ref it here, so that both that
    // Canvas.java and our picture can both be owners
    canvas->ref();
    return (Int32)canvas;
}

void CPicture::NativeEndRecording(
    /* [in] */ Int32 nativePicture)
{
    ((SkPicture*)nativePicture)->endRecording();
}

void CPicture::NativeDraw(
    /* [in] */ Int32 nativeCanvas,
    /* [in] */ Int32 nativePicture)
{
    SkASSERT(nativeCanvas);
    SkASSERT(nativePicture);
    ((SkPicture*)nativePicture)->draw((SkCanvas*)nativeCanvas);
}

Boolean CPicture::NativeWriteToStream(
    /* [in] */ Int32 nativePicture,
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* storage)
{
    SkWStream* strm = CreateSkiaOutputStreamAdaptor(stream, storage);

    if (NULL != strm) {
        ((SkPicture*)nativePicture)->serialize(strm);
        delete strm;
        return TRUE;
    }
    return FALSE;
}

void CPicture::NativeDestructor(
    /* [in] */ Int32 nativePicture)
{
    SkASSERT(nativePicture);
    delete (SkPicture*)nativePicture;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
