
#include "animation/CKeyframeHelper.h"

namespace Elastos {
namespace Droid {
namespace Animation {

/**
 * Constructs a Keyframe object with the given time and value. The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 * @param value The value that the object will animate to as the animation time approaches
 * the time in this keyframe, and the the value animated from as the time passes the time in
 * this keyframe.
 */
ECode CKeyframeHelper::OfInt32(
    /* [in] */ Float fraction,
    /* [in] */ Int32 value,
    /* [out] */ IKeyframe** obj)
{
    return Keyframe::OfInt32(fraction, value, obj);
}

/**
 * Constructs a Keyframe object with the given time. The value at this time will be derived
 * from the target object when the animation first starts (note that this implies that keyframes
 * with no initial value must be used as part of an ObjectAnimator).
 * The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @sa ObjectAnimator
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 */
ECode CKeyframeHelper::OfInt32(
    /* [in] */ Float fraction,
    /* [out] */ IKeyframe** obj)
{
    return Keyframe::OfInt32(fraction, obj);
}

/**
 * Constructs a Keyframe object with the given time and value. The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 * @param value The value that the object will animate to as the animation time approaches
 * the time in this keyframe, and the the value animated from as the time passes the time in
 * this keyframe.
 */
ECode CKeyframeHelper::OfFloat(
    /* [in] */ Float fraction,
    /* [in] */ Float value,
    /* [out] */ IKeyframe** obj)
{
    return Keyframe::OfFloat(fraction, value, obj);
}

/**
 * Constructs a Keyframe object with the given time. The value at this time will be derived
 * from the target object when the animation first starts (note that this implies that keyframes
 * with no initial value must be used as part of an ObjectAnimator).
 * The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @sa ObjectAnimator
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 */
ECode CKeyframeHelper::OfFloat(
    /* [in] */ Float fraction,
    /* [out] */ IKeyframe** obj)
{
    return Keyframe::OfFloat(fraction, obj);
}

/**
 * Constructs a Keyframe object with the given time and value. The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 * @param value The value that the object will animate to as the animation time approaches
 * the time in this keyframe, and the the value animated from as the time passes the time in
 * this keyframe.
 */
ECode CKeyframeHelper::OfObject(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* value,
    /* [out] */ IKeyframe** obj)
{
    return Keyframe::OfObject(fraction, value, obj);
}

/**
 * Constructs a Keyframe object with the given time. The value at this time will be derived
 * from the target object when the animation first starts (note that this implies that keyframes
 * with no initial value must be used as part of an ObjectAnimator).
 * The time defines the
 * time, as a proportion of an overall animation's duration, at which the value will hold true
 * for the animation. The value for the animation between keyframes will be calculated as
 * an interpolation between the values at those keyframes.
 *
 * @sa ObjectAnimator
 *
 * @param fraction The time, expressed as a value between 0 and 1, representing the fraction
 * of time elapsed of the overall animation duration.
 */
ECode CKeyframeHelper::OfObject(
    /* [in] */ Float fraction,
    /* [out] */ IKeyframe** obj)
{
    return Keyframe::OfObject(fraction, obj);
}
}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
