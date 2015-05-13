
#ifndef  __PROPERTYVALUESHOLDER_H__
#define  __PROPERTYVALUESHOLDER_H__


#include "animation/Int32KeyframeSet.h"
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

using Elastos::Core::INumber;
using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Mutex;
using Elastos::Core::IInteger32;
using Elastos::Droid::Utility::IProperty;


namespace Elastos {
namespace Droid {
namespace Animation {

extern "C" const InterfaceID EIID_PropertyValuesHolder;

class PropertyValuesHolder
{
public:
    typedef HashMap< String, AutoPtr<IMethodInfo> > MethodMap;
    typedef HashMap< AutoPtr<IClassInfo>, AutoPtr<PropertyValuesHolder::MethodMap > > ClassMethodMap;

    PropertyValuesHolder();

    /**
     * Constructs and returns a PropertyValuesHolder with a given property name and
     * set of Int32 values.
     * @param propertyName The name of the property being animated.
     * @param values The values that the named property will animate between.
     * @return PropertyValuesHolder The constructed PropertyValuesHolder object.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfInt32(
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Int32>* values);

    /**
     * Constructs and returns a PropertyValuesHolder with a given property and
     * set of Int32 values.
     * @param property The property being animated. Should not be NULL.
     * @param values The values that the property will animate between.
     * @return PropertyValuesHolder The constructed PropertyValuesHolder object.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfInt32(
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Int32>* values);

    /**
     * Constructs and returns a PropertyValuesHolder with a given property name and
     * set of Float values.
     * @param propertyName The name of the property being animated.
     * @param values The values that the named property will animate between.
     * @return PropertyValuesHolder The constructed PropertyValuesHolder object.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfFloat(
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Constructs and returns a PropertyValuesHolder with a given property and
     * set of Float values.
     * @param property The property being animated. Should not be NULL.
     * @param values The values that the property will animate between.
     * @return PropertyValuesHolder The constructed PropertyValuesHolder object.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfFloat(
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Constructs and returns a PropertyValuesHolder with a given property name and
     * set of Object values. This variant also takes a TypeEvaluator because the system
     * cannot automatically interpolate between objects of unknown type.
     *
     * @param propertyName The name of the property being animated.
     * @param evaluator A TypeEvaluator that will be called on each animation frame to
     * provide the necessary interpolation between the Object values to derive the animated
     * value.
     * @param values The values that the named property will animate between.
     * @return PropertyValuesHolder The constructed PropertyValuesHolder object.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfObject(
        /* [in] */ const String& propertyName,
        /* [in] */ ITypeEvaluator* evaluator,
        /* [in] */ ArrayOf<IInterface*>* values);

    /**
     * Constructs and returns a PropertyValuesHolder with a given property and
     * set of Object values. This variant also takes a TypeEvaluator because the system
     * cannot automatically interpolate between objects of unknown type.
     *
     * @param property The property being animated. Should not be NULL.
     * @param evaluator A TypeEvaluator that will be called on each animation frame to
     * provide the necessary interpolation between the Object values to derive the animated
     * value.
     * @param values The values that the property will animate between.
     * @return PropertyValuesHolder The constructed PropertyValuesHolder object.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfObject(
        /* [in] */ IProperty* property,
        /* [in] */ ITypeEvaluator* evaluator,
        /* [in] */ ArrayOf<IInterface*>* values);

    /**
     * Constructs and returns a PropertyValuesHolder object with the specified property name and set
     * of values. These values can be of any type, but the type should be consistent so that
     * an appropriate {@link android.animation.TypeEvaluator} can be found that matches
     * the common type.
     * <p>If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * {@link ObjectAnimator}, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     * @param propertyName The name of the property associated with this set of values. This
     * can be the actual property name to be used when using a ObjectAnimator object, or
     * just a name used to get animated values, such as if this object is used with an
     * ValueAnimator object.
     * @param values The set of values to animate between.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfKeyframe(
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<IKeyframe*>* values);

    /**
     * Constructs and returns a PropertyValuesHolder object with the specified property and set
     * of values. These values can be of any type, but the type should be consistent so that
     * an appropriate {@link android.animation.TypeEvaluator} can be found that matches
     * the common type.
     * <p>If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling the property's
     * {@link android.util.Property#get(Object)} function.
     * Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction with
     * {@link ObjectAnimator}, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     * @param property The property associated with this set of values. Should not be NULL.
     * @param values The set of values to animate between.
     */
    static CARAPI_(AutoPtr<IPropertyValuesHolder>) OfKeyframe(
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<IKeyframe*>* values);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Set the animated values for this object to this set of ints.
     * If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * {@link ObjectAnimator}, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     *
     * @param values One or more values that the animation will animate between.
     */
    virtual CARAPI SetInt32Values(
        /* [in] */ ArrayOf<Int32>* values);

    /**
     * Set the animated values for this object to this set of floats.
     * If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * {@link ObjectAnimator}, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     *
     * @param values One or more values that the animation will animate between.
     */
    virtual CARAPI SetFloatValues(
        /* [in] */ ArrayOf<Float>* values);

    /**
     * Set the animated values for this object to this set of Keyframes.
     *
     * @param values One or more values that the animation will animate between.
     */
    virtual CARAPI SetKeyframes(
        /* [in] */ ArrayOf<IKeyframe*>* values);

    /**
     * Set the animated values for this object to this set of Objects.
     * If there is only one value, it is assumed to be the end value of an animation,
     * and an initial value will be derived, if possible, by calling a getter function
     * on the object. Also, if any value is NULL, the value will be filled in when the animation
     * starts in the same way. This mechanism of automatically getting NULL values only works
     * if the PropertyValuesHolder object is used in conjunction
     * {@link ObjectAnimator}, and with a getter function
     * derived automatically from <code>propertyName</code>, since otherwise PropertyValuesHolder has
     * no way of determining what the value should be.
     *
     * @param values One or more values that the animation will animate between.
     */
    virtual CARAPI SetObjectValues(
        /* [in] */ ArrayOf<IInterface*>* values);

    /**
     * Utility function to get the setter from targetClass
     * @param targetClass The Class on which the requested method should exist.
     */
    virtual CARAPI SetupSetter(
        /* [in] */ IInterface* targetClass);

    /**
     * Internal function (called from ObjectAnimator) to set up the setter and getter
     * prior to running the animation. If the setter has not been manually set for this
     * object, it will be derived automatically given the property name, target object, and
     * types of values supplied. If no getter has been set, it will be supplied iff any of the
     * supplied values was NULL. If there is a NULL value, then the getter (supplied or derived)
     * will be called to set those NULL values to the current value of the property
     * on the target object.
     * @param target The object on which the setter (and possibly getter) exist.
     */
    virtual CARAPI SetupSetterAndGetter(
        /* [in] */ IInterface* target);

    /**
     * This function is called by ObjectAnimator when setting the start values for an animation.
     * The start values are set according to the current values in the target object. The
     * property whose value is extracted is whatever is specified by the propertyName of this
     * PropertyValuesHolder object.
     *
     * @param target The object which holds the start values that should be set.
     */
    virtual CARAPI SetupStartValue(
        /* [in] */ IInterface* target);

    /**
     * This function is called by ObjectAnimator when setting the end values for an animation.
     * The end values are set according to the current values in the target object. The
     * property whose value is extracted is whatever is specified by the propertyName of this
     * PropertyValuesHolder object.
     *
     * @param target The object which holds the start values that should be set.
     */
    virtual CARAPI SetupEndValue(
        /* [in] */ IInterface* target);

    CARAPI CloneSuperData(
        /* [in] */ PropertyValuesHolder* holder);
    /**
     * Internal function to set the value on the target object, using the setter set up
     * earlier on this PropertyValuesHolder object. This function is called by ObjectAnimator
     * to handle turning the value calculated by ValueAnimator into a value set on the object
     * according to the name of the property.
     * @param target The target object on which the value is set
     */
    virtual CARAPI SetAnimatedValue(
        /* [in] */ IInterface* target);

    /**
     * Internal function, called by ValueAnimator, to set up the TypeEvaluator that will be used
     * to calculate animated values.
     */
    virtual CARAPI Init();

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
    virtual CARAPI SetEvaluator(
        /* [in] */ ITypeEvaluator* evaluator);

    /**
     * Function used to calculate the value according to the evaluator set up for
     * this PropertyValuesHolder object. This function is called by ValueAnimator.animateValue().
     *
     * @param fraction The elapsed, interpolated fraction of the animation.
     */
    virtual CARAPI CalculateValue(
        /* [in] */ Float fraction);

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
    virtual CARAPI SetPropertyName(
        /* [in] */ const String& propertyName);

    /**
     * Sets the property that will be animated.
     *
     * <p>Note that if this PropertyValuesHolder object is used with ObjectAnimator, the property
     * must exist on the target object specified in that ObjectAnimator.</p>
     *
     * @param property The property being animated.
     */
    virtual CARAPI SetProperty(
        /* [in] */ IProperty* property);

    /**
     * Gets the name of the property that will be animated. This name will be used to derive
     * a setter function that will be called to set animated values.
     * For example, a property name of <code>foo</code> will result
     * in a call to the function <code>setFoo()</code> on the target object. If either
     * <code>valueFrom</code> or <code>valueTo</code> is NULL, then a getter function will
     * also be derived and called.
     */
    virtual CARAPI GetPropertyName(
        /* [out] */ String* name);

    /**
     * Internal function, called by ValueAnimator and ObjectAnimator, to retrieve the value
     * most recently calculated in calculateValue().
     * @return
     */
    virtual CARAPI_(AutoPtr<IInterface>) GetAnimatedValue();

    //TODO
    // @Override
    // public String toString() {
    //     return mPropertyName + ": " + mKeyframeSet.toString();
    // }

    /**
     * Utility method to derive a setter/getter method name from a property name, where the
     * prefix is typically "set" or "get" and the first letter of the property name is
     * capitalized.
     *
     * @param prefix The precursor to the method name, before the property name begins, typically
     * "set" or "get".
     * @param propertyName The name of the property that represents the bulk of the method name
     * after the prefix. The first letter of this word will be capitalized in the resulting
     * method name.
     * @return String the property name converted to a method name according to the conventions
     * specified above.
     */
    static CARAPI_(String) GetMethodName(
        /* [in] */ const String& prefix,
        /* [in] */ const String& propertyName);

protected:
    /**
     * Internal utility constructor, used by the factory methods to set the property name.
     * @param propertyName The name of the property for this holder.
     */
    CARAPI InitProperty(
        /* [in] */ const String& propertyName);

    /**
     * Internal utility constructor, used by the factory methods to set the property.
     * @param property The property for this holder.
     */
    CARAPI InitProperty(
        /* [in] */ IProperty* property);

    //The function will help you transform from a object to classInfo

     CARAPI_(AutoPtr<IClassInfo>) TransformClassInfo(
         /* [in] */ IInterface* o);
private:
    /**
     * Determine the setter or getter function using the JavaBeans convention of setFoo or
     * getFoo for a property named 'foo'. This function figures out what the name of the
     * function should be and uses reflection to find the Method with that name on the
     * target object.
     *
     * @param targetClass The class to search for the method
     * @param prefix "set" or "get", depending on whether we need a setter or getter.
     * @param valueType The type of the parameter (in the case of a setter). This type
     * is derived from the values set on this PropertyValuesHolder. This type is used as
     * a first guess at the parameter type, but we check for methods with several different
     * types to avoid problems with slight mis-matches between supplied values and actual
     * value types used on the setter.
     * @return Method the method associated with mPropertyName.
     */

    //TODO
     AutoPtr<IMethodInfo> GetPropertyFunction(
         /* [in] */ IClassInfo* targetClass,
         /* [in] */ const String& prefix);

    /**
     * Returns the setter or getter requested. This utility function checks whether the
     * requested method exists in the propertyMapMap cache. If not, it calls another
     * utility function to request the Method from the targetClass directly.
     * @param targetClass The Class on which the requested method should exist.
     * @param propertyMapMap The cache of setters/getters derived so far.
     * @param prefix "set" or "get", for the setter or getter.
     * @param valueType The type of parameter passed into the method (NULL for getter).
     * @return Method the method associated with mPropertyName.
     */


     AutoPtr<IMethodInfo> SetupSetterOrGetter(
         /* [in] */ IClassInfo* targetClass,
         /* [in] */ PropertyValuesHolder::ClassMethodMap * propertyMapMap,
         /* [in] */ const String& prefix);

    /**
     * Utility function to get the getter from targetClass
     */

    //TODO
     CARAPI SetupGetter(
         /* [in] */ IInterface* target);

    /**
     * Utility function to set the value stored in a particular Keyframe. The value used is
     * whatever the value is for the property name specified in the keyframe on the target object.
     *
     * @param target The target object from which the current value should be extracted.
     * @param kf The keyframe which holds the property name and value.
     */

    //TODO
     CARAPI SetupValue(
         /* [in] */ IInterface* target,
         /* [in] */ IKeyframe* kf);

protected:
    /**
     * The name of the property associated with the values. This need not be a real property,
     * unless this object is being used with ObjectAnimator. But this is the name by which
     * aniamted values are looked up with getAnimatedValue(String) in ValueAnimator.
     */
    String mPropertyName;

    /**
     * @hide
     */
    AutoPtr<IProperty> mProperty;

    /**
     * The setter function, if needed. ObjectAnimator hands off this functionality to
     * PropertyValuesHolder, since it holds all of the per-property information. This
     * property is automatically
     * derived when the animation starts in setupSetterAndGetter() if using ObjectAnimator.
     */
    AutoPtr<IMethodInfo> mSetter;

    /**
     * The type of values supplied. This information is used both in deriving the setter/getter
     * functions and in deriving the type of TypeEvaluator.
     */
    ClassID mValueType;

    /**
     * The set of keyframes (time/value pairs) that define this animation.
     */
    AutoPtr<IKeyframeSet> mKeyframeSet;

    // This lock is used to ensure that only one thread is accessing the property maps
    // at a time.
    //ReentrantReadWriteLock mPropertyMapLock = new ReentrantReadWriteLock();
    Mutex mPropertyMapLock;

    // Used to pass single value to varargs parameter in setter invocation
    //final Object[] mTmpValueArray = new Object[1];
    AutoPtr<ArrayOf<IInterface*> > mTmpValueArray;

private:
    /**
     * The getter function, if needed. ObjectAnimator hands off this functionality to
     * PropertyValuesHolder, since it holds all of the per-property information. This
     * property is automatically
     * derived when the animation starts in setupSetterAndGetter() if using ObjectAnimator.
     * The getter is only derived and used if one of the values is NULL.
     */
    AutoPtr<IMethodInfo> mGetter;

    // type evaluators for the primitive types handled by this implementation
    static AutoPtr<ITypeEvaluator> sIntEvaluator;
    static AutoPtr<ITypeEvaluator> sFloatEvaluator;

    // We try several different types when searching for appropriate setter/getter functions.
    // The caller may have supplied values in a type that does not match the setter/getter
    // functions (such as the integers 0 and 1 to represent floating point values for alpha).
    // Also, the use of generics in constructors means that we end up with the Object versions
    // of primitive types (Float vs. Float). But most likely, the setter/getter functions
    // will take primitive types instead.
    // So we supply an ordered array of other types to try before giving up.

    //TODO
    // These maps hold all property entries for a particular class. This map
    // is used to speed up property/setter/getter lookups for a given class/property
    // combination. No need to use reflection on the combination more than once.
    //TODO
    static AutoPtr< ClassMethodMap > sSetterPropertyMap;
    static AutoPtr< ClassMethodMap > sGetterPropertyMap;

    /**
     * The type evaluator used to calculate the animated values. This evaluator is determined
     * automatically based on the type of the start/end objects passed into the constructor,
     * but the system only knows about the primitive types Int32 and Float. Any other
     * type will need to set the evaluator to a custom evaluator for that type.
     */
    AutoPtr<ITypeEvaluator> mEvaluator;

    /**
     * The value most recently calculated by calculateValue(). This is set during
     * that function and might be retrieved later either by ValueAnimator.animatedValue() or
     * by the property-setting logic in ObjectAnimator.animatedValue().
     */
    AutoPtr<IInterface> mAnimatedValue;
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif  //__PROPERTYVALUESHOLDER_H__
