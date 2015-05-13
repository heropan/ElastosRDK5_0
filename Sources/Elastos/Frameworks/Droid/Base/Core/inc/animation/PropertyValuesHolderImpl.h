
#ifndef  __PROPERTYVALUESHOLDERIMPL_H__
#define  __PROPERTYVALUESHOLDERIMPL_H__

#include "animation/PropertyValuesHolder.h"

using Elastos::Droid::Utility::IProperty;

namespace Elastos {
namespace Droid {
namespace Animation {

class PropertyValuesHolderImpl
    : public IPropertyValuesHolder
    , public PropertyValuesHolder
    , public ElRefBase
{
public:
    PropertyValuesHolderImpl(
        /* [in] */ const String& propertyName);

    PropertyValuesHolderImpl(
        /* [in] */ IProperty* property);

    CAR_INTERFACE_DECL()

    /**
     * Set the animated values for this object to this set of ints.
     * If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * ObjectAnimator, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     *
     * @sa ObjectAnimator
     *
     * @param values One or more values that the animation will animate between.
     */
    CARAPI SetInt32Values(
        /* [in] */ ArrayOf<Int32>* values);

    /**
     * Set the animated values for this object to this set of floats.
     * If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * ObjectAnimator, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     *
     * @sa ObjectAnimator
     *
     * @param values One or more values that the animation will animate between.
     */
    CARAPI SetFloatValues(
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Set the animated values for this object to this set of Keyframes.
     *
     * @param values One or more values that the animation will animate between.
     */
    CARAPI SetKeyframes(
        /* [in] */ ArrayOf<IKeyframe*>* values);

    /**
     * Set the animated values for this object to this set of Objects.
     * If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * ObjectAnimator, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     *
     * @sa ObjectAnimator
     *
     * @param values One or more values that the animation will animate between.
     */
    CARAPI SetObjectValues(
        /* [in] */ ArrayOf<IInterface*>* values);

    /**
     * The TypeEvaluator will the automatically determined based on the type of values
     * supplied to PropertyValuesHolder. The evaluator can be manually set, however, if so
     * desired. This may be important in cases where either the type of the values supplied
     * do not match the way that they should be interpolated between, or if the values
     * are of a custom type or one not currently understood by the animation system. Currently,
     * only values of type Float and Int32 (and their Object equivalents: Float
     * and Integer) are  correctly interpolated; all other types require setting a TypeEvaluator.
     * @param evaluator
     */
    CARAPI SetEvaluator(
        /* [in] */ ITypeEvaluator* evaluator);

    /**
     * Sets the name of the property that will be animated. This name is used to derive
     * a setter function that will be called to set animated values.
     * For example, a property name of <code>foo</code> will result
     * in a call to the function <code>setFoo()</code> on the target object. If either
     * <code>valueFrom</code> or <code>valueTo</code> is NULL, then a getter function will
     * also be derived and called.
     *
     * <p>Note that the setter function derived from this property name
     * must take the same parameter type as the
     * <code>valueFrom</code> and <code>valueTo</code> properties, otherwise the call to
     * the setter function will fail.</p>
     *
     * @param propertyName The name of the property being animated.
     */
    CARAPI SetPropertyName(
        /* [in] */ const String& propertyName);

    /**
     * Sets the property that will be animated.
     *
     * <p>Note that if this PropertyValuesHolder object is used with ObjectAnimator, the property
     * must exist on the target object specified in that ObjectAnimator.</p>
     *
     * @param property The property being animated.
     */
    CARAPI SetProperty(
        /* [in] */ IProperty* property);

    /**
     * Gets the name of the property that will be animated. This name will be used to derive
     * a setter function that will be called to set animated values.
     * For example, a property name of <code>foo</code> will result
     * in a call to the function <code>setFoo()</code> on the target object. If either
     * <code>valueFrom</code> or <code>valueTo</code> is NULL, then a getter function will
     * also be derived and called.
     */
    CARAPI GetPropertyName(
        /* [out] */ String* name);

    CARAPI Clone(
        /* [out] */ IPropertyValuesHolder** holder);
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif  //__PropertyValuesHolderImpl_H__
