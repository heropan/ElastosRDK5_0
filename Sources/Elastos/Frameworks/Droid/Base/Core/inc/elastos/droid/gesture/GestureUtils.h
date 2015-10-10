#ifndef __ELASTOS_DROID_GESTURE_GESTUREUTILS_H__
#define __ELASTOS_DROID_GESTURE_GESTUREUTILS_H__

#include "elastos/droid/ext/frameworkext.h"
#include "gesture/CGesture.h"
#include <elastos/utility/etl/List.h>

using Elastos::IO::ICloseable;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Gesture {
/**
 * Utility functions for gesture processing & analysis, including methods for:
 * <ul>
 * <li>feature extraction (e.g., samplers and those for calculating bounding
 * boxes and gesture path lengths);
 * <li>geometric transformation (e.g., translation, rotation and scaling);
 * <li>gesture similarity comparison (e.g., calculating Euclidean or Cosine
 * distances between two gestures).
 * </ul>
 */
class GestureUtils
{
public:
    /**
     * Samples the gesture spatially by rendering the gesture into a 2D
     * grayscale bitmap. Scales the gesture to fit the size of the bitmap.
     * The scaling does not necessarily keep the aspect ratio of the gesture.
     *
     * @param gesture the gesture to be sampled
     * @param bitmapSize the size of the bitmap
     * @return a bitmapSize x bitmapSize grayscale bitmap that is represented
     *         as a 1D array. The float at index i represents the grayscale
     *         value at pixel [i%bitmapSize, i/bitmapSize]
     */
    static CARAPI_(AutoPtr<ArrayOf<Float> >) SpatialSampling(
        /* [in] */ IGesture *gesture,
        /* [in] */ Int32 bitmapSize);

    /**
     * Samples the gesture spatially by rendering the gesture into a 2D
     * grayscale bitmap. Scales the gesture to fit the size of the bitmap.
     *
     * @param gesture the gesture to be sampled
     * @param bitmapSize the size of the bitmap
     * @param keepAspectRatio if the scaling should keep the gesture's
     *        aspect ratio
     *
     * @return a bitmapSize x bitmapSize grayscale bitmap that is represented
     *         as a 1D array. The float at index i represents the grayscale
     *         value at pixel [i%bitmapSize, i/bitmapSize]
     */
    static CARAPI_(AutoPtr<ArrayOf<Float> >) SpatialSampling(
        /* [in] */ IGesture *gesture,
        /* [in] */ Int32 bitmapSize,
        /* [in] */ Boolean keepAspectRatio);

    /**
     * Samples a stroke temporally into a given number of evenly-distributed
     * points.
     *
     * @param stroke the gesture stroke to be sampled
     * @param numPoints the number of points
     * @return the sampled points in the form of [x1, y1, x2, y2, ..., xn, yn]
     */
    static CARAPI_(AutoPtr<ArrayOf<Float> >) TemporalSampling(
        /* [in] */ IGestureStroke *stroke,
        /* [in] */ Int32 numPoints);

    /**
     * Computes an oriented, minimum bounding box of a set of points.
     *
     * @param originalPoints
     * @return an oriented bounding box
     */
    static CARAPI_(AutoPtr<IOrientedBoundingBox>) ComputeOrientedBoundingBox(
        /* [in] */ List<IGesturePoint *> * originalPoints);

    /**
     * Computes an oriented, minimum bounding box of a set of points.
     *
     * @param originalPoints
     * @return an oriented bounding box
     */
    static CARAPI_(AutoPtr<IOrientedBoundingBox>) ComputeOrientedBoundingBox(
        /* [in] */ ArrayOf<Float> *originalPoints);

public:
    /**
     * Closes the specified stream.
     *
     * @param stream The stream to close.
     */
    static CARAPI_(void) CloseStream(
        /* [in] */ ICloseable *stream);

    /**
     * Calculates the centroid of a set of points.
     *
     * @param points the points in the form of [x1, y1, x2, y2, ..., xn, yn]
     * @return the centroid
     */
    static CARAPI_(AutoPtr<ArrayOf<Float> >) ComputeCentroid(
        /* [in] */ ArrayOf<Float> * points);

    static CARAPI_(Float) ComputeTotalLength(
        /* [in] */ ArrayOf<Float> * points);

    static CARAPI_(Float) ComputeStraightness(
        /* [in] */ ArrayOf<Float> * points);

    static CARAPI_(Float) ComputeStraightness(
        /* [in] */ ArrayOf<Float> * points,
        /* [in] */ Float totalLen);

    /**
     * Calculates the squared Euclidean distance between two vectors.
     *
     * @param vector1
     * @param vector2
     * @return the distance
     */
    static CARAPI_(Float) SquaredEuclideanDistance(
        /* [in] */ ArrayOf<Float> * vector1,
        /* [in] */ ArrayOf<Float> * vector2);

    /**
     * Calculates the cosine distance between two instances.
     *
     * @param vector1
     * @param vector2
     * @return the distance between 0 and Math.PI
     */
    static CARAPI_(Float) CosineDistance(
        /* [in] */ ArrayOf<Float> * vector1,
        /* [in] */ ArrayOf<Float> * vector2);

    /**
     * Calculates the "minimum" cosine distance between two instances.
     *
     * @param vector1
     * @param vector2
     * @param numOrientations the maximum number of orientation allowed
     * @return the distance between the two instances (between 0 and Math.PI)
     */
    static CARAPI_(Float) MinimumCosineDistance(
        /* [in] */ ArrayOf<Float> * vector1,
        /* [in] */ ArrayOf<Float> * vector2,
        /* [in] */ Int32 numOrientations);

    static CARAPI_(AutoPtr<ArrayOf<Float> >) Rotate(
        /* [in] */ ArrayOf<Float> * points,
        /* [in] */ Float angle);

    static CARAPI_(AutoPtr<ArrayOf<Float> >) Translate(
        /* [in] */ ArrayOf<Float> * points,
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    static CARAPI_(AutoPtr<ArrayOf<Float> >) Scale(
        /* [in] */ ArrayOf<Float> * points,
        /* [in] */ Float sx,
        /* [in] */ Float sy);

private:
    /* cannot be instantiated */
    GestureUtils();

    static CARAPI_(void) Plot(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ ArrayOf<Float> *sample,
        /* [in] */ Int32 sampleSize);

    /**
     * Calculates the variance-covariance matrix of a set of points.
     *
     * @param points the points in the form of [x1, y1, x2, y2, ..., xn, yn]
     * @return the variance-covariance matrix
     */
    static CARAPI_(AutoPtr<ArrayOf<FloatArray> >) ComputeCoVariance(
        /* [in] */ ArrayOf<Float> * points);

    static CARAPI_(AutoPtr<IOrientedBoundingBox>) ComputeOrientedBoundingBox(
        /* [in] */ ArrayOf<Float> * points,
        /* [in] */ ArrayOf<Float> * centroid);

    static CARAPI_(AutoPtr<ArrayOf<Float> >) ComputeOrientation(
        /* [in] */ ArrayOf<FloatArray>* covarianceMatrix);

private:
    const static Float SCALING_THRESHOLD;
    const static Float NONUNIFORM_SCALE;
    const static Float MIN_VALUE;
    const static Float MAX_VALUE;

};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
#endif //__ELASTOS_DROID_GESTURE_GESTUREUTILS_H__
