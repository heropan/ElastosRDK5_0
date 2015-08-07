
#ifndef __ELASTOS_DROID_ANIMATION_FloatPropertyValuesHolder_H__
#define __ELASTOS_DROID_ANIMATION_FloatPropertyValuesHolder_H__

#include "PropertyValuesHolder.h"

using Elastos::Droid::Utility::IFloatProperty;

namespace Elastos {
namespace Droid {
namespace Animation {

class FloatPropertyValuesHolder
    : public IFloatPropertyValuesHolder
    , public PropertyValuesHolder
    , public ElRefBase
{
public:

    FloatPropertyValuesHolder(
        /* [in] */ const String& propertyName,
        /* [in] */ IFloatKeyframeSet* keyframeSet);

    FloatPropertyValuesHolder(
        /* [in] */ IProperty* property,
        /* [in] */ IFloatKeyframeSet* keyframeSet);

    FloatPropertyValuesHolder(
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Float>* values);

    FloatPropertyValuesHolder(
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Float>* values);

    CAR_INTERFACE_DECL()

    CARAPI SetFloatValues(
        /* [in] */ ArrayOf<Float>* values);

    CARAPI CalculateValue(
        /* [in] */ Float fraction);

    virtual CARAPI GetAnimatedValue(
        /* [out] */ IInterface** value);

    CARAPI Clone(
        /* [out] */ IPropertyValuesHolder** holder);

    CARAPI SetAnimatedValue(
        /* [in] */ IInterface* target);

    CARAPI SetupSetter(
            /* [in] */ IInterface* targetClass);

    CARAPI SetInt32Values(
        /* [in] */ ArrayOf<Int32>* values);

    CARAPI SetKeyframes(
        /* [in] */ ArrayOf<IKeyframe*>* values);

    CARAPI SetObjectValues(
        /* [in] */ ArrayOf<IInterface*>* values);

    CARAPI SetEvaluator(
        /* [in] */ ITypeEvaluator* evaluator);

    CARAPI SetPropertyName(
        /* [in] */ const String& propertyName);

    CARAPI SetProperty(
        /* [in] */ IProperty* property);

    CARAPI GetPropertyName(
        /* [out] */ String* name);
protected:

    AutoPtr<IMethodInfo> mJniSetter;
    AutoPtr<IFloatKeyframeSet> mFloatKeyframeSet;
    Float mFloatAnimatedValue;
private:
    typedef HashMap<String, AutoPtr<IMethodInfo> > MethodMap;
    typedef typename MethodMap::Iterator MethodMapIterator;

    typedef HashMap<AutoPtr<IClassInfo>, AutoPtr<MethodMap> > ClassMethodMap;
    typedef typename ClassMethodMap::Iterator ClassMethodMapIterator;

    static ClassMethodMap sJNISetterPropertyMap;
    AutoPtr<IFloatProperty> mFloatProperty;

};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_ANIMATION_FloatPropertyValuesHolder_H__
