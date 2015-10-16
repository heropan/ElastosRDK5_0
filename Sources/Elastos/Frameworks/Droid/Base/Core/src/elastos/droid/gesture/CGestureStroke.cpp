
#include "elastos/droid/gesture/CGestureStroke.h"
#include "elastos/droid/gesture/GestureUtils.h"
#include "elastos/droid/gesture/CGesturePoint.h"
#include "elastos/droid/graphics/CRectF.h"
#include "elastos/droid/graphics/CPath.h"
#include <elastos/core/Math.h>

using Elastos::IO::IDataInput;
using Elastos::IO::IDataOutput;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::CPath;

namespace Elastos {
namespace Droid {
namespace Gesture {

const Float CGestureStroke::TOUCH_TOLERANCE = 3;

CGestureStroke::CGestureStroke()
    : mLength(0)
{}

ECode CGestureStroke::constructor(
    /* [in] */ IObjectContainer *points)
{
    Int32 count;
    points->GetObjectCount(&count);
    AutoPtr<ArrayOf<Float> > tmpPoints = ArrayOf<Float>::Alloc(count * 2);
    AutoPtr<ArrayOf<Int64> > times = ArrayOf<Int64>::Alloc(count);

    AutoPtr<IRectF> bx = NULL;
    Float len = 0;
    Int32 index = 0;

    AutoPtr<IObjectEnumerator> pointsIt;
    points->GetObjectEnumerator((IObjectEnumerator**)&pointsIt);
    Boolean hasNext;
    Int32 i = 0;
    while (pointsIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> item;
        pointsIt->Current((IInterface**)&item);
        AutoPtr<IGesturePoint> p = IGesturePoint::Probe(item);
        p->GetX(&(*tmpPoints)[i * 2]);
        p->GetY(&(*tmpPoints)[i * 2 + 1]);
        p->GetTimestamp(&(*times)[index]);

        Float x, y;
        p->GetX(&x);
        p->GetY(&y);
        if (bx == NULL) {
            CRectF::New(y, x, x, y, (IRectF**)&bx);
            len = 0;
        } else {
            len += Elastos::Core::Math::Sqrt(Elastos::Core::Math::Pow(x - (*tmpPoints)[(i - 1) * 2], 2)
                    + Elastos::Core::Math::Pow(y - (*tmpPoints)[(i -1 ) * 2 + 1], 2));
            bx->Union(x, y);
        }
        index++;
        i++;
    }

    mTimestamps = times;
    mPoints = tmpPoints;
    mBoundingBox = bx;
    mLength = len;
    return NOERROR;
}

CGestureStroke::CGestureStroke(
    /* [in] */ IRectF *bbx,
    /* [in] */ Float len,
    /* [in] */ ArrayOf<Float> *pts,
    /* [in] */ ArrayOf<Int64> *times)
    : mLength(len)
    , mPoints(pts)
    , mTimestamps(times)
{
    AutoPtr<IParcel> source;
    IParcelable::Probe(bbx)->WriteToParcel(source);
    CRectF::New((IRectF**)&mBoundingBox);
    IParcelable::Probe(mBoundingBox)->ReadFromParcel(source);
}

ECode CGestureStroke::Draw(
    /* [in] */ ICanvas *canvas,
    /* [in] */ IPaint *paint)
{
    if (mCachedPath == NULL) {
        MakePath();
    }

    canvas->DrawPath(mCachedPath, paint);
    return NOERROR;
}

ECode CGestureStroke::GetPath(
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);
    if (mCachedPath == NULL) {
        MakePath();
    }
    *outPath = mCachedPath;
    REFCOUNT_ADD(*outPath);
    return NOERROR;
}

void CGestureStroke::MakePath()
{
    AutoPtr<ArrayOf<Float> > localPoints = mPoints;
    const Int32 count = localPoints->GetLength();

    AutoPtr<IPath> path;

    Float mX = 0;
    Float mY = 0;

    for (Int32 i = 0; i < count; i += 2) {
        Float x = (*localPoints)[i];
        Float y = (*localPoints)[i + 1];
        if (path == NULL) {
            CPath::New((IPath**)&path);
            path->MoveTo(x, y);
            mX = x;
            mY = y;
        } else {
            Float dx = Elastos::Core::Math::Abs(x - mX);
            Float dy = Elastos::Core::Math::Abs(y - mY);
            if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
                path->QuadTo(mX, mY, (x + mX) / 2, (y + mY) / 2);
                mX = x;
                mY = y;
            }
        }
    }

    mCachedPath = path;
}

ECode CGestureStroke::ToPath(
    /* [in] */ Float width,
    /* [in] */ Float height,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);

    AutoPtr<IGestureStroke> stroke = THIS_PROBE(IGestureStroke);
    AutoPtr<ArrayOf<Float> > pts = GestureUtils::TemporalSampling(stroke, numSample);
    AutoPtr<IRectF> rect = mBoundingBox;

    Float recL, recT;
    rect->GetLeft(&recL);
    rect->GetTop(&recT);
    GestureUtils::Translate(pts, -recL, -recT);

    Float recW, recH;
    rect->GetWidth(&recW);
    rect->GetHeight(&recH);
    Float sx = width / recW;
    Float sy = height / recH;
    Float scale = sx > sy ? sy : sx;
    GestureUtils::Scale(pts, scale, scale);

    Float mX = 0;
    Float mY = 0;

    AutoPtr<IPath> path;

    const Int32 count = pts->GetLength();

    for (Int32 i = 0; i < count; i += 2) {
        Float x = (*pts)[i];
        Float y = (*pts)[i + 1];
        if (path == NULL) {
            CPath::New((IPath**)&path);
            path->MoveTo(x, y);
            mX = x;
            mY = y;
        } else {
            Float dx = Elastos::Core::Math::Abs(x - mX);
            Float dy = Elastos::Core::Math::Abs(y - mY);
            if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
                path->QuadTo(mX, mY, (x + mX) / 2, (y + mY) / 2);
                mX = x;
                mY = y;
            }
        }
    }
    *outPath = path;
    REFCOUNT_ADD(*outPath);
    return NOERROR;
}

ECode CGestureStroke::Serialize(
    /* [in] */ IDataOutputStream *out)
{
    AutoPtr<ArrayOf<Float> >pts = mPoints.Get();
    AutoPtr<ArrayOf<Int64> > times = mTimestamps.Get();
    const Int32 count = mPoints->GetLength();

    // Write number of points
    IDataOutput::Probe(out)->WriteInt32(count / 2);

    for (Int32 i = 0; i < count; i += 2) {
        // Write X
        IDataOutput::Probe(out)->WriteFloat((*pts)[i]);
        // Write Y
        IDataOutput::Probe(out)->WriteFloat((*pts)[i + 1]);
        // Write timestamp
        IDataOutput::Probe(out)->WriteInt64((*times)[i / 2]);
    }
    return NOERROR;
}

ECode CGestureStroke::Deserialize(
    /* [in] */ IDataInputStream *in,
    /* [out] */ IGestureStroke **stroke)
{
    VALIDATE_NOT_NULL(stroke);

    // Number of points
    Int32 count = 0;
    IDataInput::Probe(in)->ReadInt32(&count);

    AutoPtr<IObjectContainer> points;
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IGesturePoint> point;
        CGesturePoint::Deserialize(in, (IGesturePoint**)&point);
        points->Add((IGesturePoint *)point);
    }
    CGestureStroke::New(points, stroke);
    return NOERROR;
}

ECode CGestureStroke::ClearPath()
{
    if (mCachedPath != NULL) {
        mCachedPath->Rewind();
    }
    return NOERROR;
}

ECode CGestureStroke::ComputeOrientedBoundingBox(
    /* [out] */ IOrientedBoundingBox **box)
{
    VALIDATE_NOT_NULL(box);
    AutoPtr<IOrientedBoundingBox> temp = GestureUtils::ComputeOrientedBoundingBox(mPoints);
    *box = temp;
    REFCOUNT_ADD(*box);
    return NOERROR;
}

ECode CGestureStroke::GetBoundingBox(
    /* [out] */ IRectF **box)
{
    VALIDATE_NOT_NULL(box);
    *box = mBoundingBox;
    REFCOUNT_ADD(*box);
    return NOERROR;
}

ECode CGestureStroke::GetLength(
    /* [out] */ Float *length)
{
    VALIDATE_NOT_NULL(length);

    *length = mLength;
    return NOERROR;
}

ECode CGestureStroke::GetPoints(
    /* [out, callee] */ ArrayOf<Float> **points)
{
    VALIDATE_NOT_NULL(points);
    *points = mPoints;
    REFCOUNT_ADD(*points);
    return NOERROR;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
