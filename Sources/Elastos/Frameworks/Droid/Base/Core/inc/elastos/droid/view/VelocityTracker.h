
#ifndef __ELASTOS_DROID_VIEW_VELOCITYTRACKER_H__
#define __ELASTOS_DROID_VIEW_VELOCITYTRACKER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/utility/Pools.h"
#include "elastos/droid/utility/Config.h"
#include "elastos/droid/utility/Pools.h"

using Elastos::Droid::Utility::Pools;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IVelocityTracker;
using Elastos::Droid::View::EIID_IVelocityTracker;

/**
 * Helper for tracking the velocity of touch events, for implementing
 * flinging and other such gestures.  Use {@link #obtain} to retrieve a
 * new instance of the class when you are going to begin tracking, put
 * the motion events you receive into it with {@link #addMovement(MotionEvent)},
 * and when you want to determine the velocity call
 * {@link #computeCurrentVelocity(Int32)} and then {@link #getXVelocity()}
 * and {@link #getXVelocity()}.
 */
namespace Elastos {
namespace Droid {
namespace View {

//class VelocityTrackerState;

extern "C" const InterfaceID EIID_VelocityTracker;

class VelocityTracker
    : public Object
    , public IVelocityTracker
{
public:
    class Estimator
        : public Object
    {
        friend class VelocityTracker;
    private:
        //Must match VelocityTracker::Estimator::MAX_DEGREE
        static const Int32 MAX_DEGREE = 4;

    public:
        /**
         * Polynomial coefficients describing motion in X.
         */
        AutoPtr<ArrayOf<Float> > mXCoeff;// = new Float[MAX_DEGREE + 1];

        /**
         * Polynomial coefficients describing motion in Y.
         */
        AutoPtr<ArrayOf<Float> > mYCoeff;// = new Float[MAX_DEGREE + 1];

        /**
         * Polynomial degree, or zero if only position information is available.
         */
        Int32 mDegree;

        /**
         * Confidence (coefficient of determination), between 0 (no fit) and 1 (perfect fit).
         */
        Float mConfidence;

    public:

        Estimator();
        /**
         * Gets an estimate of the X position of the pointer at the specified time point.
         * @param time The time point in seconds, 0 is the last recorded time.
         * @return The estimated X coordinate.
         */
        CARAPI_(Float) EstimateX(
            /* [in]  */ Float time);

        /**
         * Gets an estimate of the Y position of the pointer at the specified time point.
         * @param time The time point in seconds, 0 is the last recorded time.
         * @return The estimated Y coordinate.
         */
        CARAPI_(Float) EstimateY(
            /* [in]  */ Float time);

        /**
         * Gets the X coefficient with the specified index.
         * @param index The index of the coefficient to return.
         * @return The X coefficient, or 0 if the index is greater than the degree.
         */
        CARAPI_(Float) GetXCoeff(
            /* [in]  */ Int32 index);

        /**
         * Gets the Y coefficient with the specified index.
         * @param index The index of the coefficient to return.
         * @return The Y coefficient, or 0 if the index is greater than the degree.
         */
        CARAPI_(Float) GetYCoeff(
            /* [in]  */ Int32 index);

    private:
        CARAPI_(Float) Estimate(
            /* [in]  */ Float time,
            /* [in]  */ ArrayOf<Float>* c);
    };

private:

    // static CARAPI_(AutoPtr<VelocityTrackerState>) NativeInitialize(
    //     /* [in] */ const String& strategy);

    // static CARAPI_(void) NativeClear(
    //     /* [in] */ VelocityTrackerState* ptr);

    // static CARAPI_(void) NativeAddMovement(
    //     /* [in] */ VelocityTrackerState* ptr,
    //     /* [in] */ IMotionEvent* event);

    // static CARAPI_(void) NativeComputeCurrentVelocity(
    //     /* [in] */ VelocityTrackerState* ptr,
    //     /* [in] */ Int32 units,
    //     /* [in] */ Float maxVelocity);

    // static CARAPI_(Float) NativeGetXVelocity(
    //     /* [in] */ VelocityTrackerState* ptr,
    //     /* [in] */ Int32 id);

    // static CARAPI_(Float) NativeGetYVelocity(
    //     /* [in] */ VelocityTrackerState* ptr,
    //     /* [in] */ Int32 id);

    // static CARAPI_(Boolean) NativeGetEstimator(
    //     /* [in] */ VelocityTrackerState* ptr,
    //     /* [in] */ Int32 id,
    //     /* [in] */ Estimator* outEstimator);

public:

    /**
     * Retrieve a new VelocityTracker object to watch the velocity of a
     * motion.  Be sure to call {@link #recycle} when done.  You should
     * generally only maintain an active object while tracking a movement,
     * so that the VelocityTracker can be re-used elsewhere.
     *
     * @return Returns a new VelocityTracker.
     */

    static CARAPI_(AutoPtr<VelocityTracker>) Obtain();

    /**
     * Obtains a velocity tracker with the specified strategy.
     * For testing and comparison purposes only.
     *
     * @param strategy The strategy, or null to use the default.
     * @return The velocity tracker.
     *
     * @hide
     */

    static CARAPI_(AutoPtr<VelocityTracker>) Obtain(
                    /* [in] */ const String& strategy);

public:
    CAR_INTERFACE_DECL()

    /**
     * Return a VelocityTracker object back to be re-used by others.  You must
     * not touch the object after calling this function.
     */
    CARAPI Recycle();

    CARAPI Clear();

    CARAPI AddMovement(
        /* [in] */ IMotionEvent* ev);

    CARAPI ComputeCurrentVelocity(
        /* [in] */ Int32 units);

    CARAPI ComputeCurrentVelocity(
        /* [in] */ Int32 units,
        /* [in] */ Float maxVelocity);

    CARAPI GetXVelocity(
        /* [out] */ Float* x);

    CARAPI GetYVelocity(
        /* [out] */ Float* y);

    CARAPI GetXVelocity(
        /* [in] */ Int32 id,
        /* [out] */ Float* x);

    CARAPI GetYVelocity(
        /* [in] */ Int32 id,
        /* [out] */ Float* y);

    CARAPI GetEstimator(
        /* [in] */ Int32 id,
        /* [in] */ Estimator* outEstimatorObj,
        /* [out] */ Boolean* result);

private:
    VelocityTracker(
        /* [in] */ const String& strategy);

public:
    ~VelocityTracker();

private:
    static const Int32 ACTIVE_POINTER_ID = -1;
    static AutoPtr<Pools::SynchronizedPool<IVelocityTracker> > sPool;

private:
//    AutoPtr<VelocityTrackerState> mPtr;
    String mStrategy;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_VELOCITYTRACKER_H__
