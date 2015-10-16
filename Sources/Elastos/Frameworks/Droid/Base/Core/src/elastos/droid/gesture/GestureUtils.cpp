
#include "elastos/droid/gesture/GestureUtils.h"
#include "elastos/droid/gesture/COrientedBoundingBox.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Graphics::IRectF;

namespace Elastos {
namespace Droid {
namespace Gesture {

const Float GestureUtils::SCALING_THRESHOLD = 0.26f;
const Float GestureUtils::NONUNIFORM_SCALE = (Float) Elastos::Core::Math::Sqrt(2);
const Float GestureUtils::MIN_VALUE = 1.40129846432481707e-25f;
const Float GestureUtils::MAX_VALUE = 3.40282346638528860e38f;

GestureUtils::GestureUtils(){}

void GestureUtils::CloseStream(
    /* [in] */ ICloseable *stream) {
    if (stream != NULL) {
        stream->Close();
    }
}

AutoPtr<ArrayOf<Float> > GestureUtils::SpatialSampling(
    /* [in] */ IGesture *gesture,
    /* [in] */ Int32 bitmapSize)
{
    return SpatialSampling(gesture, bitmapSize, FALSE);
}

AutoPtr<ArrayOf<Float> > GestureUtils::SpatialSampling(
    /* [in] */ IGesture *gesture,
    /* [in] */ Int32 bitmapSize,
    /* [in] */ Boolean keepAspectRatio)
{
    const Float targetPatchSize = bitmapSize - 1;
    AutoPtr<ArrayOf<Float> > sample = ArrayOf<Float>::Alloc(bitmapSize * bitmapSize);
    for (Int32 i = 0; i < sample->GetLength(); ++i) {
        (*sample)[i] = 0;
    }

    AutoPtr<IRectF> rect;
    gesture->GetBoundingBox((IRectF**)&rect);
    Float gestureWidth = 0;
    rect->GetWidth(&gestureWidth);
    Float gestureHeight = 0;
    rect->GetHeight(&gestureHeight);
    Float sx = targetPatchSize / gestureWidth;
    Float sy = targetPatchSize / gestureHeight;

    if (keepAspectRatio) {
        Float scale = sx < sy ? sx : sy;
        sx = scale;
        sy = scale;
    } else {

        Float aspectRatio = gestureWidth / gestureHeight;
        if (aspectRatio > 1) {
            aspectRatio = 1 / aspectRatio;
        }
        if (aspectRatio < SCALING_THRESHOLD) {
            Float scale = sx < sy ? sx : sy;
            sx = scale;
            sy = scale;
        } else {
            if (sx > sy) {
                Float scale = sy * NONUNIFORM_SCALE;
                if (scale < sx) {
                    sx = scale;
                }
            } else {
                Float scale = sx * NONUNIFORM_SCALE;
                if (scale < sy) {
                    sy = scale;
                }
            }
        }
    }
    Float fx, fy;
    rect->GetCenterX(&fx);
    Float preDx = -fx;
    rect->GetCenterY(&fy);
    Float preDy = -fy;
    Float postDx = targetPatchSize / 2;
    Float postDy = targetPatchSize / 2;
    AutoPtr<IObjectContainer> strokes;
    gesture->GetStrokes((IObjectContainer**)&strokes);
    Int32 count;
    strokes->GetObjectCount(&count);
    AutoPtr<IObjectEnumerator> enumerator;
    strokes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Int32 size;
    Float xpos;
    Float ypos;
    Boolean hasNext = FALSE;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> item;
        enumerator->Current((IInterface**)&item);
        AutoPtr<IGestureStroke> stroke = IGestureStroke::Probe(item);
        AutoPtr<ArrayOf<Float> > strokepoints;
        stroke->GetPoints((ArrayOf<Float>**)&strokepoints);
        size = strokepoints->GetLength();
        AutoPtr<ArrayOf<Float> > pts = ArrayOf<Float>::Alloc(size);
        for (Int32 i = 0; i < size; i += 2) {
            (*pts)[i] = ((*strokepoints)[i] + preDx) * sx + postDx;
            (*pts)[i + 1] = ((*strokepoints)[i + 1] + preDy) * sy + postDy;
        }
        Float segmentEndX = -1;
        Float segmentEndY = -1;
        for (Int32 i = 0; i < size; i += 2) {
            Float segmentStartX = (*pts)[i] < 0 ? 0 : (*pts)[i];
            Float segmentStartY = (*pts)[i + 1] < 0 ? 0 : (*pts)[i + 1];
            if (segmentStartX > targetPatchSize) {
                segmentStartX = targetPatchSize;
            }
            if (segmentStartY > targetPatchSize) {
                segmentStartY = targetPatchSize;
            }
            Plot(segmentStartX, segmentStartY, sample, bitmapSize);
            if (segmentEndX != -1) {
                // Evaluate horizontally
                if (segmentEndX > segmentStartX) {
                    xpos = (Float) Elastos::Core::Math::Ceil(segmentStartX);
                    Float slope = (segmentEndY - segmentStartY) /
                                  (segmentEndX - segmentStartX);
                    while (xpos < segmentEndX) {
                        ypos = slope * (xpos - segmentStartX) + segmentStartY;
                        Plot(xpos, ypos, sample, bitmapSize);
                        xpos++;
                    }
                } else if (segmentEndX < segmentStartX){
                    xpos = (Float) Elastos::Core::Math::Ceil(segmentEndX);
                    Float slope = (segmentEndY - segmentStartY) /
                                  (segmentEndX - segmentStartX);
                    while (xpos < segmentStartX) {
                        ypos = slope * (xpos - segmentStartX) + segmentStartY;
                        Plot(xpos, ypos, sample, bitmapSize);
                        xpos++;
                    }
                }
                // Evaluate vertically
                if (segmentEndY > segmentStartY) {
                    ypos = (Float) Elastos::Core::Math::Ceil(segmentStartY);
                    Float invertSlope = (segmentEndX - segmentStartX) /
                                        (segmentEndY - segmentStartY);
                    while (ypos < segmentEndY) {
                        xpos = invertSlope * (ypos - segmentStartY) + segmentStartX;
                        Plot(xpos, ypos, sample, bitmapSize);
                        ypos++;
                    }
                } else if (segmentEndY < segmentStartY) {
                    ypos = (Float) Elastos::Core::Math::Ceil(segmentEndY);
                    Float invertSlope = (segmentEndX - segmentStartX) /
                                        (segmentEndY - segmentStartY);
                    while (ypos < segmentStartY) {
                        xpos = invertSlope * (ypos - segmentStartY) + segmentStartX;
                        Plot(xpos, ypos, sample, bitmapSize);
                        ypos++;
                    }
                }
            }
            segmentEndX = segmentStartX;
            segmentEndY = segmentStartY;
        }
    }
    return sample;
}

void GestureUtils::Plot(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ ArrayOf<Float> *sample,
    /* [in] */ Int32 sampleSize)
{
    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;
    Int32 xFloor = (Int32) Elastos::Core::Math::Floor(x);
    Int32 xCeiling = (Int32) Elastos::Core::Math::Ceil(x);
    Int32 yFloor = (Int32) Elastos::Core::Math::Floor(y);
    Int32 yCeiling = (Int32) Elastos::Core::Math::Ceil(y);

    // if it's an integer
    if (x == xFloor && y == yFloor) {
        Int32 index = yCeiling * sampleSize + xCeiling;
        if ((*sample)[index] < 1) {
            (*sample)[index] = 1;
        }
    } else {
        const Double xFloorSq = Elastos::Core::Math::Pow(xFloor - x, 2);
        const Double yFloorSq = Elastos::Core::Math::Pow(yFloor - y, 2);
        const Double xCeilingSq = Elastos::Core::Math::Pow(xCeiling - x, 2);
        const Double yCeilingSq = Elastos::Core::Math::Pow(yCeiling - y, 2);
        Float topLeft = (Float) Elastos::Core::Math::Sqrt(xFloorSq + yFloorSq);
        Float topRight = (Float) Elastos::Core::Math::Sqrt(xCeilingSq + yFloorSq);
        Float btmLeft = (Float) Elastos::Core::Math::Sqrt(xFloorSq + yCeilingSq);
        Float btmRight = (Float) Elastos::Core::Math::Sqrt(xCeilingSq + yCeilingSq);
        Float sum = topLeft + topRight + btmLeft + btmRight;

        Float value = topLeft / sum;
        Int32 index = yFloor * sampleSize + xFloor;
        if (value > (*sample)[index]) {
            (*sample)[index] = value;
        }

        value = topRight / sum;
        index = yFloor * sampleSize + xCeiling;
        if (value > (*sample)[index]) {
            (*sample)[index] = value;
        }

        value = btmLeft / sum;
        index = yCeiling * sampleSize + xFloor;
        if (value > (*sample)[index]) {
            (*sample)[index] = value;
        }

        value = btmRight / sum;
        index = yCeiling * sampleSize + xCeiling;
        if (value > (*sample)[index]) {
            (*sample)[index] = value;
        }
    }
}

AutoPtr<ArrayOf<Float> > GestureUtils::TemporalSampling(
    /* [in] */ IGestureStroke *stroke,
    /* [in] */ Int32 numPoints)
{
    Float length;
    stroke->GetLength(&length);
    const Float increment = length / (numPoints - 1);
    Int32 vectorLength = numPoints * 2;
    AutoPtr<ArrayOf<Float> >vector = ArrayOf<Float>::Alloc(vectorLength);
    Float distanceSoFar = 0;
    AutoPtr<ArrayOf<Float> > pts;
    stroke->GetPoints((ArrayOf<Float>**)&pts);
    Float lstPointX = (*pts)[0];
    Float lstPointY = (*pts)[1];
    Int32 index = 0;
    Float currentPointX = MIN_VALUE;//Float.MIN_VALUE;
    Float currentPointY = MIN_VALUE;//Float.MIN_VALUE;
    (*vector)[index] = lstPointX;
    index++;
    (*vector)[index] = lstPointY;
    index++;
    Int32 i = 0;
    Int32 count = pts->GetLength() / 2;
    while (i < count) {
        if (currentPointX == MIN_VALUE) {
            i++;
            if (i >= count) {
                break;
            }
            currentPointX = (*pts)[i * 2];
            currentPointY = (*pts)[i * 2 + 1];
        }
        Float deltaX = currentPointX - lstPointX;
        Float deltaY = currentPointY - lstPointY;
        Float distance = (Float) Elastos::Core::Math::Sqrt(deltaX * deltaX + deltaY * deltaY);
        if (distanceSoFar + distance >= increment) {
            Float ratio = (increment - distanceSoFar) / distance;
            Float nx = lstPointX + ratio * deltaX;
            Float ny = lstPointY + ratio * deltaY;
            (*vector)[index] = nx;
            index++;
            (*vector)[index] = ny;
            index++;
            lstPointX = nx;
            lstPointY = ny;
            distanceSoFar = 0;
        } else {
            lstPointX = currentPointX;
            lstPointY = currentPointY;
            currentPointX = MIN_VALUE;
            currentPointY = MIN_VALUE;
            distanceSoFar += distance;
        }
    }

    for (i = index; i < vectorLength; i += 2) {
        (*vector)[i] = lstPointX;
        (*vector)[i + 1] = lstPointY;
    }
    return vector;
}

AutoPtr<ArrayOf<Float> > GestureUtils::ComputeCentroid(
        /* [in] */ ArrayOf<Float> * points)
{
    Float centerX = 0;
    Float centerY = 0;
    Int32 count = points->GetLength();
    for (Int32 i = 0; i < count; i++) {
        centerX += (*points)[i];
        i++;
        centerY += (*points)[i];
    }
    AutoPtr<ArrayOf<Float> > center = ArrayOf<Float>::Alloc(2);
    (*center)[0] = 2 * centerX / count;
    (*center)[1] = 2 * centerY / count;

    return center;
}

AutoPtr<ArrayOf<FloatArray> > GestureUtils::ComputeCoVariance(
    /* [in] */ ArrayOf<Float> * points)
{
    AutoPtr<ArrayOf<FloatArray> > array = ArrayOf<FloatArray>::Alloc(2);
    AutoPtr<ArrayOf<Float> > array0 = ArrayOf<Float>::Alloc(2);
    AutoPtr<ArrayOf<Float> > array1 = ArrayOf<Float>::Alloc(2);

    array->Set(0, array0);
    array->Set(1, array1);

    (*array0)[0] = 0;
    (*array0)[1] = 0;
    (*array1)[0] = 0;
    (*array1)[1] = 0;
    Int32 count = points->GetLength();
    for (Int32 i = 0; i < count; i++) {
        Float x = (*points)[i];
        i++;
        Float y = (*points)[i];
        (*array0)[0] += x * x;
        (*array0)[1] += x * y;
        (*array1)[0] = (*array0)[1];
        (*array1)[1] += y * y;
    }
    (*array0)[0] /= (count / 2);
    (*array0)[1] /= (count / 2);
    (*array1)[0] /= (count / 2);
    (*array1)[1] /= (count / 2);

    return array;
}

Float GestureUtils::ComputeTotalLength(
    /* [in] */ ArrayOf<Float> * points)
{
    Float sum = 0;
    Int32 count = points->GetLength() - 4;
    for (Int32 i = 0; i < count; i += 2) {
        Float dx = (*points)[i + 2] - (*points)[i];
        Float dy = (*points)[i + 3] - (*points)[i + 1];
        sum += Elastos::Core::Math::Sqrt(dx * dx + dy * dy);
    }
    return sum;
}

Float GestureUtils::ComputeStraightness(
    /* [in] */ ArrayOf<Float> * points)
{
    Float totalLen = ComputeTotalLength(points);
    Float dx = (*points)[2] - (*points)[0];
    Float dy = (*points)[3] - (*points)[1];
    return (Float) Elastos::Core::Math::Sqrt(dx * dx + dy * dy) / totalLen;
}

Float GestureUtils::ComputeStraightness(
    /* [in] */ ArrayOf<Float> * points,
    /* [in] */ Float totalLen)
{
    Float dx = (*points)[2] - (*points)[0];
    Float dy = (*points)[3] - (*points)[1];
    return (Float) Elastos::Core::Math::Sqrt(dx * dx + dy * dy) / totalLen;
}

Float GestureUtils::SquaredEuclideanDistance(
    /* [in] */ ArrayOf<Float> * vector1,
    /* [in] */ ArrayOf<Float> * vector2)
{
    Float squaredDistance = 0;
    Int32 size = vector1->GetLength();
    for (Int32 i = 0; i < size; i++) {
        Float difference = (*vector1)[i] - (*vector2)[i];
        squaredDistance += difference * difference;
    }
    return squaredDistance / size;
}

Float GestureUtils::CosineDistance(
    /* [in] */ ArrayOf<Float> * vector1,
    /* [in] */ ArrayOf<Float> * vector2)
{
    Float sum = 0;
    Int32 len = vector1->GetLength();
    for (Int32 i = 0; i < len; i++) {
        sum += (*vector1)[i] * (*vector2)[i];
    }
    return (Float) Elastos::Core::Math::Acos(sum);
}

Float GestureUtils::MinimumCosineDistance(
    /* [in] */ ArrayOf<Float> * vector1,
    /* [in] */ ArrayOf<Float> * vector2,
    /* [in] */ Int32 numOrientations)
{
    const Int32 len = vector1->GetLength();
    Float a = 0;
    Float b = 0;
    for (Int32 i = 0; i < len; i += 2) {
        a += (*vector1)[i] * (*vector2)[i] + (*vector1)[i + 1] * (*vector2)[i + 1];
        b += (*vector1)[i] * (*vector2)[i + 1] - (*vector1)[i + 1] * (*vector2)[i];
    }
    if (a != 0) {
        const Float tan = b/a;
        const Double angle = Elastos::Core::Math::Atan(tan);
        if (numOrientations > 2 && Elastos::Core::Math::Abs(angle) >= Elastos::Core::Math::DOUBLE_PI / numOrientations) {
            return (Float) Elastos::Core::Math::Acos(a);
        } else {
            const Double cosine = Elastos::Core::Math::Cos(angle);
            const Double sine = cosine * tan;
            return (Float) Elastos::Core::Math::Acos(a * cosine + b * sine);
        }
    } else {
        return (Float) Elastos::Core::Math::DOUBLE_PI / 2;
    }
}

AutoPtr<IOrientedBoundingBox> GestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ List<IGesturePoint *> *originalPoints)
{
    const Int32 count = originalPoints->GetSize();
    AutoPtr<ArrayOf<Float> > points = ArrayOf<Float>::Alloc(count * 2);
    List<IGesturePoint *>::Iterator it = originalPoints->Begin();
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IGesturePoint> point = *it;
        Int32 index = i * 2;
        point->GetX(&(*points)[index]);
        point->GetY(&(*points)[index + 1]);
        if (it != originalPoints->End()) {
            ++it;
        }
    }
    AutoPtr<ArrayOf<Float> > meanVector = ComputeCentroid(points);
    return ComputeOrientedBoundingBox(points, meanVector);
}

AutoPtr<IOrientedBoundingBox> GestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ ArrayOf<Float> *originalPoints)
{
    Int32 size = originalPoints->GetLength();
    AutoPtr<ArrayOf<Float> > points = ArrayOf<Float>::Alloc(size);
    for (Int32 i = 0; i < size; i++) {
        (*points)[i] = (*originalPoints)[i];
    }
    AutoPtr<ArrayOf<Float> > meanVector = ComputeCentroid(points);
    return ComputeOrientedBoundingBox(points, meanVector);
}

AutoPtr<IOrientedBoundingBox> GestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ ArrayOf<Float> * points,
    /* [in] */ ArrayOf<Float> * centroid)
{
    Translate(points, -(*centroid)[0], -(*centroid)[1]);

    AutoPtr<ArrayOf<FloatArray> > array = ComputeCoVariance(points);
    AutoPtr<ArrayOf<Float> > targetVector = ComputeOrientation(array);

    Float angle;
    if ((*targetVector)[0] == 0 && (*targetVector)[1] == 0) {
        angle = (Float) -Elastos::Core::Math::DOUBLE_PI/2;
    } else { // -PI<alpha<PI
        angle = (Float) Elastos::Core::Math::Atan2((*targetVector)[1], (*targetVector)[0]);
        Rotate(points, -angle);
    }

    Float minx = MAX_VALUE;
    Float miny = MAX_VALUE;
    Float maxx = MIN_VALUE;
    Float maxy = MIN_VALUE;
    Int32 count = points->GetLength();
    for (Int32 i = 0; i < count; i++) {
        if ((*points)[i] < minx) {
            minx = (*points)[i];
        }
        if ((*points)[i] > maxx) {
            maxx = (*points)[i];
        }
        i++;
        if ((*points)[i] < miny) {
            miny = (*points)[i];
        }
        if ((*points)[i] > maxy) {
            maxy = (*points)[i];
        }
    }
    AutoPtr<IOrientedBoundingBox> box;
    COrientedBoundingBox::New((Float) (angle * 180 / Elastos::Core::Math::DOUBLE_PI),
            (*centroid)[0], (*centroid)[1], maxx - minx,
            maxy - miny, (IOrientedBoundingBox**)&box);
    return box;
}

AutoPtr<ArrayOf<Float> >  GestureUtils::ComputeOrientation(
        /* [in] */ ArrayOf<FloatArray>* covarianceMatrix)
{
    AutoPtr<ArrayOf<Float> > covarianceMatrix0 = (*covarianceMatrix)[0];
    AutoPtr<ArrayOf<Float> > covarianceMatrix1 = (*covarianceMatrix)[1];
    AutoPtr<ArrayOf<Float> > targetVector = ArrayOf<Float>::Alloc(2);
    if ((*covarianceMatrix0)[1] == 0 || (*covarianceMatrix1)[0] == 0) {
        (*targetVector)[0] = 1;
        (*targetVector)[1] = 0;
    }

    Float a = -(*covarianceMatrix0)[0] - (*covarianceMatrix1)[1];
    Float b = (*covarianceMatrix0)[0] * (*covarianceMatrix1)[1] - (*covarianceMatrix0)[1]
            * (*covarianceMatrix1)[0];
    Float value = a / 2;
    Float rightside = (Float) Elastos::Core::Math::Sqrt(Elastos::Core::Math::Pow(value, 2) - b);
    Float lambda1 = -value + rightside;
    Float lambda2 = -value - rightside;
    if (lambda1 == lambda2) {
        (*targetVector)[0] = 0;
        (*targetVector)[1] = 0;
    } else {
        Float lambda = lambda1 > lambda2 ? lambda1 : lambda2;
        (*targetVector)[0] = 1;
        (*targetVector)[1] = (lambda - (*covarianceMatrix0)[0]) / (*covarianceMatrix0)[1];
    }
    return targetVector;
}

AutoPtr<ArrayOf<Float> > GestureUtils::Rotate(
    /* [in] */ ArrayOf<Float> * points,
    /* [in] */ Float angle)
{
    Float cos = (Float) Elastos::Core::Math::Cos(angle);
    Float sin = (Float) Elastos::Core::Math::Sin(angle);
    Int32 size = points->GetLength();
    for (Int32 i = 0; i < size; i += 2) {
        Float x = (*points)[i] * cos - (*points)[i + 1] * sin;
        Float y = (*points)[i] * sin + (*points)[i + 1] * cos;
        (*points)[i] = x;
        (*points)[i + 1] = y;
    }
    AutoPtr<ArrayOf<Float> > result = points;
    return result;
}

AutoPtr<ArrayOf<Float> > GestureUtils::Translate(
    /* [in] */ ArrayOf<Float> * points,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    Int32 size = points->GetLength();
    for (Int32 i = 0; i < size; i += 2) {
        (*points)[i] += dx;
        (*points)[i + 1] += dy;
    }
    AutoPtr<ArrayOf<Float> > result = points;
    return result;
}

AutoPtr<ArrayOf<Float> > GestureUtils::Scale(
    /* [in] */ ArrayOf<Float> * points,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    Int32 size = points->GetLength();
    for (Int32 i = 0; i < size; i += 2) {
        (*points)[i] *= sx;
        (*points)[i + 1] *= sy;
    }
    AutoPtr<ArrayOf<Float> > result = points;
    return result;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
