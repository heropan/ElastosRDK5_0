
#ifndef  __ELASTOS_DROID_ANIMATION_COBJECTANIMATOR_H__
#define  __ELASTOS_DROID_ANIMATION_COBJECTANIMATOR_H__

#include "_Elastos_Droid_Animation_CObjectAnimator.h"
#include "animation/ObjectAnimator.h"

using Elastos::Droid::Utility::IProperty;

namespace Elastos {
namespace Droid {
namespace Animation {

/**
 * This subclass of {@link ValueAnimator} provides support for animating properties on target objects.
 * The constructors of this class take parameters to define the target object that will be animated
 * as well as the name of the property that will be animated. Appropriate set/get functions
 * are then determined internally and the animation will call these functions as necessary to
 * animate the property.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about animating with {@code ObjectAnimator}, read the
 * <a href="{@docRoot}guide/topics/graphics/prop-animation.html#object-animator">Property
 * Animation</a> developer guide.</p>
 * </div>
 *
 * @see #setPropertyName(String)
 *
 */
CarClass(CObjectAnimator) , public ObjectAnimator
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    /**
     * Constructs and returns an ObjectAnimator that animates between Int32 values. A single
     * value implies that that value is the one being animated to. Two values imply a starting
     * and ending values. More than two values imply a starting value, values to animate through
     * along the way, and an ending value (these values will be distributed evenly across
     * the duration of the animation).
     *
     * @param target The object whose property is to be animated. This object should
     * have a public method on it called <code>setName()</code>, where <code>name</code> is
     * the value of the <code>propertyName</code> parameter.
     * @param propertyName The name of the property being animated.
     * @param values A set of values that the animation will animate between over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfInt32(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Int32>* values);

    /**
     * Constructs and returns an ObjectAnimator that animates between int values. A single
     * value implies that that value is the one being animated to. Two values imply a starting
     * and ending values. More than two values imply a starting value, values to animate through
     * along the way, and an ending value (these values will be distributed evenly across
     * the duration of the animation).
     *
     * @param target The object whose property is to be animated.
     * @param property The property being animated.
     * @param values A set of values that the animation will animate between over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfInt32(
        /* [in] */ IInterface* target,
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Int32>* values);

    /**
     * Constructs and returns an ObjectAnimator that animates between Float values. A single
     * value implies that that value is the one being animated to. Two values imply a starting
     * and ending values. More than two values imply a starting value, values to animate through
     * along the way, and an ending value (these values will be distributed evenly across
     * the duration of the animation).
     *
     * @param target The object whose property is to be animated. This object should
     * have a public method on it called <code>setName()</code>, where <code>name</code> is
     * the value of the <code>propertyName</code> parameter.
     * @param propertyName The name of the property being animated.
     * @param values A set of values that the animation will animate between over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfFloat(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Constructs and returns an ObjectAnimator that animates between float values. A single
     * value implies that that value is the one being animated to. Two values imply a starting
     * and ending values. More than two values imply a starting value, values to animate through
     * along the way, and an ending value (these values will be distributed evenly across
     * the duration of the animation).
     *
     * @param target The object whose property is to be animated.
     * @param property The property being animated.
     * @param values A set of values that the animation will animate between over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfFloat(
        /* [in] */ IInterface* target,
        /* [in] */ IProperty* propertyName,
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Constructs and returns an ObjectAnimator that animates between Object values. A single
     * value implies that that value is the one being animated to. Two values imply a starting
     * and ending values. More than two values imply a starting value, values to animate through
     * along the way, and an ending value (these values will be distributed evenly across
     * the duration of the animation).
     *
     * @param target The object whose property is to be animated. This object should
     * have a public method on it called <code>setName()</code>, where <code>name</code> is
     * the value of the <code>propertyName</code> parameter.
     * @param propertyName The name of the property being animated.
     * @param evaluator A TypeEvaluator that will be called on each animation frame to
     * provide the necessary interpolation between the Object values to derive the animated
     * value.
     * @param values A set of values that the animation will animate between over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfObject(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName,
        /* [in] */ ITypeEvaluator* evaluator,
        /* [in] */ ArrayOf<IInterface*>* values);

    /**
     * Constructs and returns an ObjectAnimator that animates between Object values. A single
     * value implies that that value is the one being animated to. Two values imply a starting
     * and ending values. More than two values imply a starting value, values to animate through
     * along the way, and an ending value (these values will be distributed evenly across
     * the duration of the animation).
     *
     * @param target The object whose property is to be animated.
     * @param property The property being animated.
     * @param evaluator A TypeEvaluator that will be called on each animation frame to
     * provide the necessary interpolation between the Object values to derive the animated
     * value.
     * @param values A set of values that the animation will animate between over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfObject(
        /* [in] */ IInterface* target,
        /* [in] */ IProperty* property,
        /* [in] */ ITypeEvaluator* evaluator,
        /* [in] */ ArrayOf<IInterface*>* values);

    /**
     * Constructs and returns an ObjectAnimator that animates between the sets of values specified
     * in <code>PropertyValueHolder</code> objects. This variant should be used when animating
     * several properties at once with the same ObjectAnimator, since PropertyValuesHolder allows
     * you to associate a set of animation values with a property name.
     *
     * @param target The object whose property is to be animated. Depending on how the
     * PropertyValuesObjects were constructed, the target object should either have the {@link
     * android.util.Property} objects used to construct the PropertyValuesHolder objects or (if the
     * PropertyValuesHOlder objects were created with property names) the target object should have
     * public methods on it called <code>setName()</code>, where <code>name</code> is the name of
     * the property passed in as the <code>propertyName</code> parameter for each of the
     * PropertyValuesHolder objects.
     * @param values A set of PropertyValuesHolder objects whose values will be animated between
     * over time.
     * @return An ObjectAnimator object that is set up to animate between the given values.
     */
    static AutoPtr<IObjectAnimator> OfPropertyValuesHolder(
        /* [in] */ IInterface* target,
        /* [in] */ ArrayOf<IPropertyValuesHolder*>* values);

public:
    CARAPI constructor();
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_ANIMATION_COBJECTANIMATOR_H__
