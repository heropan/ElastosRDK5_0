
#ifndef  __OBJECTANIMATOR_H__
#define  __OBJECTANIMATOR_H__

#include "animation/ValueAnimator.h"

using Elastos::Droid::Utility::IProperty;

namespace Elastos {
namespace Droid {
namespace Animation {

class ObjectAnimator : public ValueAnimator
{
public:
    ObjectAnimator();

    /**
     * Sets the name of the property that will be animated. This name is used to derive
     * a setter function that will be called to set animated values.
     * For example, a property name of <code>foo</code> will result
     * in a call to the function <code>setFoo()</code> on the target object. If either
     * <code>valueFrom</code> or <code>valueTo</code> is NULL, then a getter function will
     * also be derived and called.
     *
     * <p>For best performance of the mechanism that calls the setter function determined by the
     * name of the property being animated, use <code>Float</code> or <code>Int32</code> typed values,
     * and make the setter function for those properties have a <code>void</code> return value. This
     * will cause the code to take an optimized path for these constrained circumstances. Other
     * property types and return types will work, but will have more overhead in processing
     * the requests due to normal reflection mechanisms.</p>
     *
     * <p>Note that the setter function derived from this property name
     * must take the same parameter type as the
     * <code>valueFrom</code> and <code>valueTo</code> properties, otherwise the call to
     * the setter function will fail.</p>
     *
     * <p>If this ObjectAnimator has been set up to animate several properties together,
     * using more than one PropertyValuesHolder objects, then setting the propertyName simply
     * sets the propertyName in the first of those PropertyValuesHolder objects.</p>
     *
     * @param propertyName The name of the property being animated. Should not be NULL.
     */
    virtual CARAPI SetPropertyName(
        /* [in] */ const String& propertyName);

    /**
     * Sets the property that will be animated. Property objects will take precedence over
     * properties specified by the {@link #setPropertyName(String)} method. Animations should
     * be set up to use one or the other, not both.
     *
     * @param property The property being animated. Should not be NULL.
     */
    virtual CARAPI SetProperty(
        /* [in] */ IProperty* property);

    /**
     * Gets the name of the property that will be animated. This name will be used to derive
     * a setter function that will be called to set animated values.
     * For example, a property name of <code>foo</code> will result
     * in a call to the function <code>setFoo()</code> on the target object. If either
     * <code>valueFrom</code> or <code>valueTo</code> is null, then a getter function will
     * also be derived and called.
     */
    virtual CARAPI_(String) GetPropertyName();

    //@Override
    virtual CARAPI SetInt32Values(
        /* [in] */ ArrayOf<Int32>* values);

    //@Override
    virtual CARAPI SetFloatValues(
        /* [in] */ ArrayOf<Float>* values);

    //@Override
    virtual CARAPI SetObjectValues(
        /* [in] */ ArrayOf<IInterface*>* values);

    //@Override
    virtual CARAPI Start();

    /**
     * This function is called immediately before processing the first animation
     * frame of an animation. If there is a nonzero <code>startDelay</code>, the
     * function is called after that delay ends.
     * It takes care of the final initialization steps for the
     * animation. This includes setting mEvaluator, if the user has not yet
     * set it up, and the setter/getter methods, if the user did not supply
     * them.
     *
     *  <p>Overriders of this method should call the superclass method to cause
     *  internal mechanisms to be set up correctly.</p>
     */
    //@Override
    virtual CARAPI InitAnimation();

    /**
     * Sets the length of the animation. The default duration is 300 milliseconds.
     *
     * @param duration The length of the animation, in milliseconds.
     * @return ObjectAnimator The object called with setDuration(). This return
     * value makes it easier to compose statements together that construct and then set the
     * duration, as in
     * <code>ObjectAnimator.ofInt(target, propertyName, 0, 10).setDuration(500).start()</code>.
     */
    //@Override
    virtual CARAPI SetDuration(
        /* [in] */ Int64 duration);

    /**
     * The target object whose property will be animated by this animation
     *
     * @return The object being animated
     */
    CARAPI_(AutoPtr<IInterface>) GetTarget();

    /**
     * Sets the target object whose property will be animated by this animation
     *
     * @param target The object being animated
     */
    //@Override
    virtual CARAPI SetTarget(
        /*[in] */ IInterface* target);

    //@Override
    virtual CARAPI SetupStartValues();

    //@Override
    virtual CARAPI SetupEndValues();

    //@Override
    virtual CARAPI AnimateValue(
        /* [in] */ Float fraction);

    virtual CARAPI Clone(
        /* [out] */ IAnimator** object);
protected:
    /**
     * Private utility constructor that initializes the target object and name of the
     * property being animated.
     *
     * @param target The object whose property is to be animated. This object should
     * have a public method on it called <code>setName()</code>, where <code>name</code> is
     * the value of the <code>propertyName</code> parameter.
     * @param propertyName The name of the property being animated.
     */
    CARAPI Init(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName);

    /**
     * Private utility constructor that initializes the target object and property being animated.
     *
     * @param target The object whose property is to be animated.
     * @param property The property being animated.
     */
    CARAPI Init(
        /* [in] */ IInterface* target,
        /* [in] */ AutoPtr<IProperty> property);

private:

    // The target object on which the property exists, set in the constructor
    AutoPtr<IInterface> mTarget;

    String mPropertyName;

    AutoPtr<IProperty> mProperty;
};


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos

#endif  //__OBJECTANIMATOR_H__
