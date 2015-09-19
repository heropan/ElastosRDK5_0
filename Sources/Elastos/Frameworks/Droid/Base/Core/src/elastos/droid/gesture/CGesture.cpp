
#include "ext/frameworkext.h"
#include "gesture/CGesture.h"
#include "gesture/GestureUtils.h"
#include "gesture/CGestureStroke.h"
#include "gesture/GestureConstants.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CCanvas.h"
#include "graphics/CPaint.h"
#include "graphics/CPath.h"
#include "graphics/CRectF.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CObjectContainer;
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

Int64 CGesture::GESTURE_ID_BASE = -1;
const Boolean CGesture::BITMAP_RENDERING_ANTIALIAS = TRUE;
const Boolean CGesture::BITMAP_RENDERING_DITHER = TRUE;
AutoPtr<IAtomicInteger32> CGesture::sGestureCount = CGesture::InitGestureCount();

AutoPtr<IAtomicInteger32> CGesture::InitGestureCount()
{
    AutoPtr<IAtomicInteger32> temp;
    CAtomicInteger32::New(0, (IAtomicInteger32**)&temp);
    return temp;
}

CGesture::CGesture()
    : mGestureID(0)
{
    CRectF::New((IRectF**)&mBoundingBox);
    mStrokes = new List<AutoPtr<IGestureStroke> >();

}

ECode CGesture::constructor()
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

ECode CGesture::GetStrokes(
    /* [out] */ IObjectContainer **container)
{
    CObjectContainer::New(container);
    List<AutoPtr<IGestureStroke> >::Iterator it;
    for (it = mStrokes->Begin(); it != mStrokes->End(); ++it) {
        (*container)->Add(*it);
    }
    return NOERROR;
}

ECode CGesture::GetStrokesCount(
    /* [out] */ Int32 *count)
{
    VALIDATE_NOT_NULL(count);

    *count = mStrokes->GetSize();
    return NOERROR;
}

ECode CGesture::AddStroke(
    /* [in] */ IGestureStroke *stroke)
{
    mStrokes->PushBack(stroke);
    AutoPtr<IRectF> r;
    stroke->GetBoundingBox((IRectF**)&r);
    mBoundingBox->Union(r);
    return NOERROR;
}

ECode CGesture::GetLength(
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

ECode CGesture::GetBoundingBox(
    /* [out] */ IRectF **box)
{
    VALIDATE_NOT_NULL(box);
    *box = mBoundingBox;
    REFCOUNT_ADD(*box);
    return NOERROR;
}

ECode CGesture::ToPath(
    /* [out] */ IPath **outPath)
{
    return ToPath(NULL, outPath);
}

ECode CGesture::ToPath(
    /* [in] */ IPath *path,
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);

    if (path == NULL) CPath::New((IPath**)&path);

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

ECode CGesture::ToPath(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath **outPath)
{
    return ToPath(NULL, width, height, edge, numSample, outPath);
}

ECode CGesture::ToPath(
    /* [in] */ IPath *path,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);

    if (path == NULL) CPath::New((IPath**)&path);

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

ECode CGesture::SetID(
    /* [in] */ Int64 id)
{
    mGestureID = id;
    return NOERROR;
}

ECode CGesture::GetID(
    /* [out] */ Int64 *id)
{
    VALIDATE_NOT_NULL(id);
    *id = mGestureID;
    return NOERROR;
}

ECode CGesture::ToBitmap(
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

ECode CGesture::ToBitmap(
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

ECode CGesture::Serialize(
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

ECode CGesture::WriteToParcel(
    /* [in] */ IParcel* out)
{
    out->WriteInt64(mGestureID);

    Boolean result = FALSE;
    AutoPtr<IByteArrayOutputStream> byteStream;
    CByteArrayOutputStream::New(GestureConstants::IO_BUFFER_SIZE, (IByteArrayOutputStream**)&byteStream);
    AutoPtr<IDataOutputStream> outStream;
    CDataOutputStream::New(byteStream, (IDataOutputStream**)&outStream);

    // try {
    ECode ec = Serialize(outStream);
    if (FAILED(ec)) Logger::E(GestureConstants::LOG_TAG, "Error writing Gesture to parcel:0x%x", ec);
    result = TRUE;
    // } catch (IOException e) {
    // Log.e(GestureConstants.LOG_TAG, "Error writing Gesture to parcel:", e);
    // } finally {
    GestureUtils::CloseStream(outStream);
    GestureUtils::CloseStream(byteStream);
    // }

    if (result) {
        AutoPtr<ArrayOf<Byte> > streamBytes;
        byteStream->ToByteArray((ArrayOf<Byte>**)&streamBytes);
        out->WriteArrayOf((Handle32)streamBytes.Get());
    }
    return NOERROR;
}

ECode CGesture::ReadFromParcel(
    /* [in] */ IParcel* in)
{
    Int64 gestureID;
    in->ReadInt64(&gestureID);

    AutoPtr<ArrayOf<Byte> > bytes;
// TODO:
    // in->CreateByteArray((ArrayOf<Byte>**)&bytes);
    AutoPtr<IByteArrayInputStream> byteStream;
    CByteArrayInputStream::New(bytes, (IByteArrayInputStream**)&byteStream);
    AutoPtr<IDataInputStream> inStream;
    CDataInputStream::New(byteStream, (IDataInputStream**)&inStream);

    // try {
    ECode ec1 = IDataInput::Probe(inStream)->ReadInt64(&mGestureID);
    Int32 count;
    ECode ec2 = IDataInput::Probe(inStream)->ReadInt32(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IGestureStroke> stroke;
        CGestureStroke::Deserialize(inStream, (IGestureStroke**)&stroke);
        AddStroke(stroke);
    }
    // } catch (IOException e) {
    if (FAILED(ec1) || FAILED(ec2)) Logger::E(GestureConstants::LOG_TAG, "Error reading Gesture from parcel:0x%x || 0x%x", ec1, ec2);
    // } finally {
    GestureUtils::CloseStream(inStream);
    // }

    if (FAILED(ec1) || FAILED(ec2)) mGestureID = gestureID;

    return NOERROR;
}

ECode CGesture::Deserialize(
    /* [in] */ IDataInputStream *in,
    /* [out] */ IGesture** gesture)
{
    CGesture::New(gesture);

    // Gesture ID
    Int64 gestureID;
    FAIL_RETURN(IDataInput::Probe(in)->ReadInt64(&gestureID));
    ((CGesture*)(*gesture))->SetID(gestureID);
    // Number of strokes
    Int32 count;
    FAIL_RETURN(IDataInput::Probe(in)->ReadInt32(&count));

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IGestureStroke> stroke;
        CGestureStroke::Deserialize(in, (IGestureStroke**)&stroke);
        (*gesture)->AddStroke(stroke);
    }

    return NOERROR;
}

ECode CGesture::WriteToParcel(
    /* [in] */ IParcel *out,
    /* [in] */ Int32 flags)
{
    VALIDATE_NOT_NULL(out);

    out->WriteInt64(mGestureID);

    AutoPtr<IByteArrayOutputStream> byteStream;
    CByteArrayOutputStream::New(GestureConstants::IO_BUFFER_SIZE, (IByteArrayOutputStream**)&byteStream);
    AutoPtr<IDataOutputStream> outStream;
    CDataOutputStream::New(byteStream, (IDataOutputStream**)&outStream);

    //try {
    ECode ec = Serialize(outStream);
    //} catch (IOException e) {
    if (FAILED(ec)) Logger::E(GestureConstants::LOG_TAG, "Error writing Gesture to parcel:0x%x", ec);
    //} finally {
    GestureUtils::CloseStream(outStream);
    GestureUtils::CloseStream(byteStream);
    //}

    if (SUCCEEDED(ec)) {
        AutoPtr<ArrayOf<Byte> > bytes;
        byteStream->ToByteArray((ArrayOf<Byte>**)&bytes);
        out->WriteArrayOf((Handle32)bytes.Get());
    }

    return ec;
}

ECode CGesture::DescribeContents(
    /* [out] */ Int32 *result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;
    return NOERROR;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
