#ifndef __INT32PROPERTYVALUESHOLDER_H__
#define __INT32PROPERTYVALUESHOLDER_H__

#include "PropertyValuesHolder.h"

using Elastos::Droid::Utility::IInt32Property;
using Elastos::Droid::Utility::EIID_IInt32Property;

namespace Elastos {
namespace Droid {
namespace Animation {

class Int32PropertyValuesHolder
    : public IInt32PropertyValuesHolder
    , public PropertyValuesHolder
    , public ElRefBase
{
public:

    Int32PropertyValuesHolder(
        /* [in] */ const String& propertyName,
        /* [in] */ IInt32KeyframeSet* keyframeSet);

    Int32PropertyValuesHolder(
        /* [in] */ IProperty* property,
        /* [in] */ IInt32KeyframeSet* keyframeSet);

    Int32PropertyValuesHolder(
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Int32>* values);

    Int32PropertyValuesHolder(
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Int32>* values);

    CAR_INTERFACE_DECL()

    CARAPI SetInt32Values(
        /* [in] */ ArrayOf<Int32>* values);

    CARAPI CalculateValue(
        /* [in] */ Float fraction);

    CARAPI_(AutoPtr<IInterface>) GetAnimatedValue();

    CARAPI Clone(
        /* [out] */ IPropertyValuesHolder** holder);

    CARAPI SetAnimatedValue(
        /* [in] */ IInterface* target);

    CARAPI SetupSetter(
        /* [in] */ IInterface* targetClass);


    CARAPI SetFloatValues(
        /* [in] */ ArrayOf<Float>* values);

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
    AutoPtr<IInt32KeyframeSet> mInt32KeyframeSet;
    Int32 mInt32AnimatedValue;
private:
    typedef HashMap<String, AutoPtr<IMethodInfo> > MethodMap;
    typedef typename MethodMap::Iterator MethodMapIterator;

    typedef HashMap<AutoPtr<IClassInfo>, AutoPtr<MethodMap> > ClassMethodMap;
    typedef typename ClassMethodMap::Iterator ClassMethodMapIterator;

    static ClassMethodMap sJNISetterPropertyMap;
    AutoPtr<IInt32Property> mInt32Property;

};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif
