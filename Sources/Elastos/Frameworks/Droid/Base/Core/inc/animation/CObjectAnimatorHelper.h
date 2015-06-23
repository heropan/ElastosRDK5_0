#ifndef  __COBJECTANIMATORHELPER_H__
#define  __COBJECTANIMATORHELPER_H__

#include "_CObjectAnimatorHelper.h"

using Elastos::Droid::Utility::IProperty;

namespace Elastos {
namespace Droid {
namespace Animation {

CarClass(CObjectAnimatorHelper) {
public:
    CARAPI OfInt32(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Int32>* values,
        /* [out] */ IObjectAnimator** anim);

    CARAPI OfInt32(
        /* [in] */ IInterface* target,
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Int32>* values,
        /* [out] */ IObjectAnimator** anim);

    CARAPI OfFloat(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName,
        /* [in] */ ArrayOf<Float>* values,
        /* [out] */ IObjectAnimator** anim);

    CARAPI OfFloat(
        /* [in] */ IInterface* target,
        /* [in] */ IProperty* property,
        /* [in] */ ArrayOf<Float>* values,
        /* [out] */ IObjectAnimator** anim);

    CARAPI OfObject(
        /* [in] */ IInterface* target,
        /* [in] */ const String& propertyName,
        /* [in] */ ITypeEvaluator* evaluator,
        /* [in] */ ArrayOf<IInterface*>* values,
        /* [out] */ IObjectAnimator** anim);

    CARAPI OfObject(
        /* [in] */ IInterface* target,
        /* [in] */ IProperty* property,
        /* [in] */ ITypeEvaluator* evaluator,
        /* [in] */ ArrayOf<IInterface*>* values,
        /* [out] */ IObjectAnimator** anim);

    CARAPI OfPropertyValuesHolder(
        /* [in] */ IInterface* target,
        /* [in] */ ArrayOf<IPropertyValuesHolder*>* values,
        /* [out] */ IObjectAnimator** anim);
};

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

#endif //__COBJECTANIMATORHELPER_H__
