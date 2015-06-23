
#include "animation/PropertyValuesHolderImpl.h"


namespace Elastos {
namespace Droid {
namespace Animation {

PropertyValuesHolderImpl::PropertyValuesHolderImpl(
    /* [in] */ const String& propertyName)
{
    InitProperty(propertyName);
}

PropertyValuesHolderImpl::PropertyValuesHolderImpl(
    /* [in] */ IProperty* property)
{
    InitProperty(property);
}

UInt32 PropertyValuesHolderImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PropertyValuesHolderImpl::Release()
{
    return ElRefBase::Release();
}

ECode PropertyValuesHolderImpl::GetInterfaceID(
    /* [in] */ IInterface *object,
    /* [out] */ InterfaceID *pIID)
{
    if (object == (IInterface*)(IPropertyValuesHolder *)this) {
        *pIID = EIID_IPropertyValuesHolder;
    } else if (object == reinterpret_cast<PInterface>((PropertyValuesHolder*)this)) {
        *pIID = EIID_PropertyValuesHolder;
    } else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

PInterface PropertyValuesHolderImpl::Probe(
    /* [in] */ REIID riid)
{
    if(riid == EIID_PropertyValuesHolder) {
        return reinterpret_cast<PInterface>((PropertyValuesHolder*)this);
    } else if (riid == EIID_IInterface) {
        return (IInterface*)(IPropertyValuesHolder*)this;
    } else if (riid == EIID_IPropertyValuesHolder) {
        return (IPropertyValuesHolder*)this;
    } else {
        return NULL;
    }
}
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
ECode PropertyValuesHolderImpl::SetInt32Values(
    /* [in] */ ArrayOf<Int32>* values)
{
    return PropertyValuesHolder::SetInt32Values(values);
}

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
ECode PropertyValuesHolderImpl::SetFloatValues(
    /* [in] */ ArrayOf<Float>* values)
{
    return PropertyValuesHolder::SetFloatValues(values);
}

/**
 * Set the animated values for this object to this set of Keyframes.
 *
 * @param values One or more values that the animation will animate between.
 */
ECode PropertyValuesHolderImpl::SetKeyframes(
    /* [in] */ ArrayOf<IKeyframe*>* values)
{
    return PropertyValuesHolder::SetKeyframes(values);
}

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
ECode PropertyValuesHolderImpl::SetObjectValues(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    return PropertyValuesHolder::SetObjectValues(values);
}

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
ECode PropertyValuesHolderImpl::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    return PropertyValuesHolder::SetEvaluator(evaluator);
}

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
ECode PropertyValuesHolderImpl::SetPropertyName(
    /* [in] */ const String& propertyName)
{
    return PropertyValuesHolder::SetPropertyName(propertyName);
}

/**
 * Sets the property that will be animated.
 *
 * <p>Note that if this PropertyValuesHolder object is used with ObjectAnimator, the property
 * must exist on the target object specified in that ObjectAnimator.</p>
 *
 * @param property The property being animated.
 */
ECode PropertyValuesHolderImpl::SetProperty(
    /* [in] */ IProperty* property)
{
    return PropertyValuesHolder::SetProperty(property);
}

/**
 * Gets the name of the property that will be animated. This name will be used to derive
 * a setter function that will be called to set animated values.
 * For example, a property name of <code>foo</code> will result
 * in a call to the function <code>setFoo()</code> on the target object. If either
 * <code>valueFrom</code> or <code>valueTo</code> is NULL, then a getter function will
 * also be derived and called.
 */
ECode PropertyValuesHolderImpl::GetPropertyName(
    /* [out] */ String* name)
{
    return PropertyValuesHolder::GetPropertyName(name);
}

ECode PropertyValuesHolderImpl::Clone(
    /* [out] */ IPropertyValuesHolder** holder)
{
    AutoPtr<PropertyValuesHolderImpl> v = new PropertyValuesHolderImpl(mPropertyName);
    PropertyValuesHolder::CloneSuperData(v);
    *holder = v;
    REFCOUNT_ADD(*holder)
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
