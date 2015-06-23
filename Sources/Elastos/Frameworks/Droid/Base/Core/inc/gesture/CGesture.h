#ifndef __CGESTURE_H__
#define __CGESTURE_H__

#include "ext/frameworkext.h"
#include "_CGesture.h"
#include <elastos/utility/etl/List.h>

using Elastos::IO::IDataOutputStream;
using Elastos::IO::IDataInputStream;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPath;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CGesture)
{
public:
    static CARAPI_(AutoPtr<IAtomicInteger32>) InitGestureCount();

    CGesture();

    CARAPI constructor();

    /**
     * @return all the strokes of the gesture
     */
    CARAPI GetStrokes(
        /* [out] */ IObjectContainer **container);

    /**
     * @return the number of strokes included by this gesture
     */
    CARAPI GetStrokesCount(
        /* [out] */ Int32 *count);

    /**
     * Adds a stroke to the gesture.
     *
     * @param stroke
     */
    CARAPI AddStroke(
        /* [in] */ IGestureStroke *stroke);

    /**
     * Calculates the total length of the gesture. When there are multiple strokes in
     * the gesture, this returns the sum of the lengths of all the strokes.
     *
     * @return the length of the gesture
     */
    CARAPI GetLength(
        /* [out] */ Float *length);

    /**
     * @return the bounding box of the gesture
     */
    CARAPI GetBoundingBox(
        /* [out] */ IRectF **box);

    CARAPI ToPath(
        /* [out] */ IPath **outPath);

    CARAPI ToPath(
        /* [in] */ IPath *path,
        /* [out] */ IPath **outPath);

    CARAPI ToPath(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 edge,
        /* [in] */ Int32 numSample,
        /* [out] */ IPath **outPath);

    CARAPI ToPath(
        /* [in] */ IPath *path,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 edge,
        /* [in] */ Int32 numSample,
        /* [out] */ IPath **outPath);

    /**
     * @return the id of the gesture
     */
    CARAPI GetID(
        /* [out] */ Int64 *id);

    /**
     * Creates a bitmap of the gesture with a transparent background.
     *
     * @param width width of the target bitmap
     * @param height height of the target bitmap
     * @param edge the edge
     * @param numSample
     * @param color
     * @return the bitmap
     */
    CARAPI ToBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 edge,
        /* [in] */ Int32 numSample,
        /* [in] */ Int32 color,
        /* [out] */ IBitmap **bm);

    /**
     * Creates a bitmap of the gesture with a transparent background.
     *
     * @param width
     * @param height
     * @param inset
     * @param color
     * @return the bitmap
     */
    CARAPI ToBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 inset,
        /* [in] */ Int32 color,
        /* [out] */ IBitmap **bm);

/*    public static final Parcelable.Creator<Gesture> CREATOR = new Parcelable.Creator<Gesture>() {
        public Gesture createFromParcel(Parcel in) {
            Gesture gesture = null;
            final long gestureID = in.readLong();

            final DataInputStream inStream = new DataInputStream(
                    new ByteArrayInputStream(in.createByteArray()));

            try {
                gesture = deserialize(inStream);
            } catch (IOException e) {
                Log.e(GestureConstants.LOG_TAG, "Error reading Gesture from parcel:", e);
            } finally {
                GestureUtils.closeStream(inStream);
            }

            if (gesture != null) {
                gesture.mGestureID = gestureID;
            }

            return gesture;
        }

        public Gesture[] newArray(int size) {
            return new Gesture[size];
        }
    };*/

    CARAPI WriteToParcel(
        /* [in] */ IParcel *out,
        /* [in] */ Int32 flags);

    CARAPI DescribeContents(
        /* [out] */ Int32 *result);

    /**
     * Sets the id of the gesture.
     *
     * @param id
     */
    CARAPI SetID(
        /* [in] */ Int64 id);

    CARAPI Serialize(
        /* [in] */ IDataOutputStream *out);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);


public:
    static CARAPI Deserialize(
        /* [in] */ IDataInputStream *in,
        /* [out] */ IGesture** gesture);

private:
    static CARAPI_(AutoPtr<IRectF>) InitBoundingBox();
    static CARAPI_(AutoPtr<IObjectContainer>) InitObjectContainer();

private:
    static Int64 GESTURE_ID_BASE;

    const static Int32 BITMAP_RENDERING_WIDTH = 2;

    const static Boolean BITMAP_RENDERING_ANTIALIAS;
    const static Boolean BITMAP_RENDERING_DITHER;

    static AutoPtr<IAtomicInteger32> sGestureCount;

    AutoPtr<IRectF> mBoundingBox;

    // the same as its instance ID
    Int64 mGestureID;

    AutoPtr<List<AutoPtr<IGestureStroke> > > mStrokes;

};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos

#endif //__GESTURE_H__
