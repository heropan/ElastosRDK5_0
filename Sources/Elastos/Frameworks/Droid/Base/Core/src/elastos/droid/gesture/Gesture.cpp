
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/gesture/Gesture.h"
#include "elastos/droid/gesture/GestureUtils.h"
#include "elastos/droid/gesture/CGestureStroke.h"
#include "elastos/droid/gesture/GestureConstants.h"
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CPath.h"
#include "elastos/droid/graphics/CRectF.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;

using Elastos::IO::IDataOutput;
using Elastos::IO::IDataInput;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::CByteArrayInputStream;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::Graphics::PaintStyle_STROKE;
using Elastos::Droid::Graphics::PaintJoin_ROUND;
using Elastos::Droid::Graphics::PaintCap_ROUND;
using Elastos::Droid::Graphics::CPath;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Gesture {

CAR_INTERFACE_IMPL_2(Gesture, Object, IGesture, IParcelable);

Int64 Gesture::GESTURE_ID_BASE = -1;
const Boolean Gesture::BITMAP_RENDERING_ANTIALIAS = TRUE;
const Boolean Gesture::BITMAP_RENDERING_DITHER = TRUE;
AutoPtr<IAtomicInteger32> Gesture::sGestureCount = Gesture::InitGestureCount();

AutoPtr<IAtomicInteger32> Gesture::InitGestureCount()
{
    AutoPtr<IAtomicInteger32> temp;
    CAtomicInteger32::New(0, (IAtomicInteger32**)&temp);
    return temp;
}

Gesture::Gesture()
    : mGestureID(0)
{
    CRectF::New((IRectF**)&mBoundingBox);
    mStrokes = new List<AutoPtr<IGestureStroke> >();

}

Gesture::~Gesture()
{}

ECode Gesture::constructor()
{
    if (GESTURE_ID_BASE < 0) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        system->GetCurrentTimeMillis(&GESTURE_ID_BASE);
    }

    Int32 value;
    sGestureCount->IncrementAndGet(&value);
    mGestureID = GESTURE_ID_BASE + value;
    return NOERROR;
}

ECode Gesture::GetStrokes(
    /* [out] */ IObjectContainer **container)
{
    CObjectContainer::New(container);
    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        (*container)->Add(*it);
    }
    return NOERROR;
}

ECode Gesture::GetStrokesCount(
    /* [out] */ Int32 *count)
{
    VALIDATE_NOT_NULL(count);

    *count = mStrokes->GetSize();
    return NOERROR;
}

ECode Gesture::AddStroke(
    /* [in] */ IGestureStroke *stroke)
{
    mStrokes->PushBack(stroke);
    AutoPtr<IRectF> r;
    stroke->GetBoundingBox((IRectF**)&r);
    mBoundingBox->Union(r);
    return NOERROR;
}

ECode Gesture::GetLength(
    /* [out] */ Float *len)
{
    VALIDATE_NOT_NULL(len);
    *len = 0;

    AutoPtr<List<AutoPtr<IGestureStroke> > > strokes = mStrokes;
    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        Float length = 0;
        (*it)->GetLength(&length);
        *len += length;
    }

    return NOERROR;
}

ECode Gesture::GetBoundingBox(
    /* [out] */ IRectF **box)
{
    VALIDATE_NOT_NULL(box);
    *box = mBoundingBox;
    REFCOUNT_ADD(*box);
    return NOERROR;
}

ECode Gesture::ToPath(
    /* [out] */ IPath **outPath)
{
    return ToPath(NULL, outPath);
}

ECode Gesture::ToPath(
    /* [in] */ IPath *path,
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);

    if (path == NULL)
        CPath::New((IPath**)&path);

    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        AutoPtr<IPath> tempPath;
        (*it)->GetPath((IPath**)&tempPath);
        path->AddPath(tempPath);
    }

    *outPath = path;
    REFCOUNT_ADD(*outPath);
    return NOERROR;
}

ECode Gesture::ToPath(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath **outPath)
{
    return ToPath(NULL, width, height, edge, numSample, outPath);
}

ECode Gesture::ToPath(
    /* [in] */ IPath *path,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);

    if (path == NULL)
        CPath::New((IPath**)&path);

    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        AutoPtr<IPath> tempPath;
        (*it)->ToPath(width - 2 * edge, height - 2 * edge, numSample, (IPath**)&tempPath);
        path->AddPath(tempPath);
    }
    *outPath = path;
    REFCOUNT_ADD(*outPath);
    return NOERROR;
}

ECode Gesture::SetID(
    /* [in] */ Int64 id)
{
    mGestureID = id;
    return NOERROR;
}

ECode Gesture::GetID(
    /* [out] */ Int64 *id)
{
    VALIDATE_NOT_NULL(id);
    *id = mGestureID;
    return NOERROR;
}

ECode Gesture::ToBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [in] */ Int32 color,
    /* [out] */ IBitmap **bm)
{
    VALIDATE_NOT_NULL(bm);

    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> bitmap;
    bitmapFactory->CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&bitmap);

    AutoPtr<ICanvas> canvas;
    CCanvas::New(bitmap, (ICanvas**)&canvas);
    canvas->Translate(edge, edge);

    AutoPtr<IPaint> paint;
    CPaint::New((IPaint**)&paint);
    paint->SetAntiAlias(BITMAP_RENDERING_ANTIALIAS);
    paint->SetDither(BITMAP_RENDERING_DITHER);
    paint->SetColor(color);
    paint->SetStyle(PaintStyle_STROKE);
    paint->SetStrokeJoin(PaintJoin_ROUND);
    paint->SetStrokeCap(PaintCap_ROUND);
    paint->SetStrokeWidth(BITMAP_RENDERING_WIDTH);

    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        AutoPtr<IPath> path;
        (*it)->ToPath(width - 2 * edge, height - 2 * edge, numSample, (IPath**)&path);
        canvas->DrawPath(path, paint);
    }

    *bm = bitmap;
    REFCOUNT_ADD(*bm);
    return NOERROR;
}

ECode Gesture::ToBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 inset,
    /* [in] */ Int32 color,
    /* [out] */ IBitmap **bm)
{
    VALIDATE_NOT_NULL(bm);

    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> bitmap;
    bitmapFactory->CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&bitmap);

    AutoPtr<ICanvas> canvas;
    CCanvas::New(bitmap, (ICanvas**)&canvas);

    AutoPtr<IPaint> paint;
    CPaint::New((IPaint**)&paint);

    paint->SetAntiAlias(BITMAP_RENDERING_ANTIALIAS);
    paint->SetDither(BITMAP_RENDERING_DITHER);
    paint->SetColor(color);
    paint->SetStyle(PaintStyle_STROKE);
    paint->SetStrokeJoin(PaintJoin_ROUND);
    paint->SetStrokeCap(PaintCap_ROUND);
    paint->SetStrokeWidth(BITMAP_RENDERING_WIDTH);

    AutoPtr<IPath> path;
    ToPath((IPath**)&path);
    AutoPtr<IRectF> bounds;
    CRectF::New((IRectF**)&bounds);
    path->ComputeBounds((IRectF*)bounds, TRUE);

    Float w, h;
    bounds->GetWidth(&w);
    bounds->GetHeight(&h);
    const Float sx = (width - 2 * inset) / w;
    const Float sy = (height - 2 * inset) / h;
    const Float scale = sx > sy ? sy : sx;
    paint->SetStrokeWidth(2.0f / scale);

    Float l, t;
    bounds->GetLeft(&l);
    bounds->GetTop(&t);
    path->Offset(-l + (width - w * scale) / 2.0f,
            -t + (height - h * scale) / 2.0f);

    canvas->Translate(inset, inset);
    canvas->Scale(scale, scale);
    canvas->DrawPath(path, paint);

    *bm = bitmap;
    REFCOUNT_ADD(*bm);
    return NOERROR;
}

ECode Gesture::Serialize(
    /* [in] */ IDataOutputStream *out)
{
    VALIDATE_NOT_NULL(out);

    Int32 count = mStrokes->GetSize();

    // Write gesture ID
    IDataOutput::Probe(out)->WriteInt64(mGestureID);
    // Write number of strokes
    IDataOutput::Probe(out)->WriteInt32(count);

    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        (*it)->Serialize(out);
    }

    return NOERROR;
}

ECode Gesture::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt64(mGestureID);

    Boolean result = FALSE;
    AutoPtr<IByteArrayOutputStream> byteStream;
    CByteArrayOutputStream::New(GestureConstants::IO_BUFFER_SIZE, (IByteArrayOutputStream**)&byteStream);
    AutoPtr<IDataOutputStream> outStream;
    CDataOutputStream::New(byteStream, (IDataOutputStream**)&outStream);

    ECode ec = Serialize(outStream);
    if (FAILED(ec))
        Logger::E(GestureConstants::LOG_TAG, "Error writing Gesture to parcel:0x%x", ec);
    result = TRUE;

    GestureUtils::CloseStream(outStream);
    GestureUtils::CloseStream(byteStream);

    if (result) {
        AutoPtr<ArrayOf<Byte> > streamBytes;
        byteStream->ToByteArray((ArrayOf<Byte>**)&streamBytes);
        out->WriteArrayOf((Handle32)streamBytes.Get());
    }
    return NOERROR;
}

ECode Gesture::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    Int64 gestureID;
    in->ReadInt64(&gestureID);

    AutoPtr<ArrayOf<Byte> > bytes;

    bytes = ArrayOf<Byte>::Alloc(in->dataCapacity);
    if (bytes == NULL)
        Logger::E(GestureConstants::LOG_TAG, "No Memory");

    in->read(bytes->GetPayload(), in->dataCapacity);

    AutoPtr<IByteArrayInputStream> byteStream;
    CByteArrayInputStream::New(bytes, (IByteArrayInputStream**)&byteStream);
    AutoPtr<IDataInputStream> inStream;
    CDataInputStream::New(byteStream, (IDataInputStream**)&inStream);

    AutoPtr<IDataInput> ins = IDataInput::Probe(inStream);
    ECode ec1 = ins->ReadInt64(&mGestureID);
    Int32 count;
    ECode ec2 = ins->ReadInt32(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IGestureStroke> stroke;
        CGestureStroke::Deserialize(inStream, (IGestureStroke**)&stroke);
        AddStroke(stroke);
    }

    if (FAILED(ec1) || FAILED(ec2))
        Logger::E(GestureConstants::LOG_TAG, "Error reading Gesture from parcel:0x%x || 0x%x", ec1, ec2);

    GestureUtils::CloseStream(inStream);


    if (FAILED(ec1) || FAILED(ec2))
        mGestureID = gestureID;

    return NOERROR;
}

ECode Gesture::Deserialize(
    /* [in] */ IDataInputStream *in,
    /* [out] */ IGesture** gesture)
{
    Gesture::New(gesture);

    Int64 gestureID;
    FAIL_RETURN(IDataInput::Probe(in)->ReadInt64(&gestureID));
    ((Gesture*)(*gesture))->SetID(gestureID);

    Int32 count;
    FAIL_RETURN(IDataInput::Probe(in)->ReadInt32(&count));

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IGestureStroke> stroke;
        CGestureStroke::Deserialize(in, (IGestureStroke**)&stroke);
        (*gesture)->AddStroke(stroke);
    }

    return NOERROR;
}

ECode Gesture::WriteToParcel(
    /* [in] */ IParcel *out,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(out);

    out->WriteInt64(mGestureID);

    AutoPtr<IByteArrayOutputStream> byteStream;
    CByteArrayOutputStream::New(GestureConstants::IO_BUFFER_SIZE, (IByteArrayOutputStream**)&byteStream);
    AutoPtr<IDataOutputStream> outStream;
    CDataOutputStream::New(byteStream, (IDataOutputStream**)&outStream);

    ECode ec = Serialize(outStream);
    if (FAILED(ec))
        Logger::E(GestureConstants::LOG_TAG, "Error writing Gesture to parcel:0x%x", ec);

    GestureUtils::CloseStream(outStream);
    GestureUtils::CloseStream(byteStream);

    if (SUCCEEDED(ec)) {
        AutoPtr<ArrayOf<Byte> > bytes;
        byteStream->ToByteArray((ArrayOf<Byte>**)&bytes);
        out->WriteArrayOf((Handle32)bytes.Get());
    }

    return ec;
}

ECode Gesture::DescribeContents(
    /* [out] */ Int32 *result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

} // namespace Gesture
} // namespace Droid
} // namespace Elastos
