
#ifndef __ELASTOS_DROID_TRANSITION_CCHANGEIMAGETRANSFORM_H__
#define __ELASTOS_DROID_TRANSITION_CCHANGEIMAGETRANSFORM_H__

#include "_Elastos_Droid_Transition_CChangeImageTransform.h"
#include "elastos/droid/transition/Transition.h"

using Elastos::Droid::Animation::ITypeEvaluator;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Utility::IProperty;
using Elastos::Droid::Widget::IImageView;

namespace Elastos {
namespace Droid {
namespace Transition {

CarClass(CChangeImageTransform)
    , public Transition
{
private:
    class TypeEvaluator
        : public Object
        , public ITypeEvaluator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Evaluate(
            /* [in] */ Float fraction,
            /* [in] */ IInterface* startValue,
            /* [in] */ IInterface* endValue,
            /* [out] */ IInterface** result);
    };

    class MatrixProperty
        : public Object
        , public IProperty
    {
    public:
        CAR_INTERFACE_DECL()

        MatrixProperty(
            /* [in] */ String str);

        CARAPI IsReadOnly(
            /* [out] */ Boolean* readOnly);

        CARAPI Set(
            /* [in] */ IInterface* object,
            /* [in] */ IInterface* value);

        CARAPI Get(
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** result);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI GetType(
            /* [out] */ ClassID* id);

        CARAPI IsWriteOnly(
            /* [out] */ Boolean* writeOnly);

        CARAPI ForbiddenOperate(
            /* [out] */ Boolean* forbidden);

    private:
        String mName;
    };

public:
    CAR_OBJECT_DECL()

    CChangeImageTransform();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI CaptureStartValues(
        /* [in] */ ITransitionValues* transitionValues);

    CARAPI CaptureEndValues(
        /* [in] */ ITransitionValues* transitionValues);

    CARAPI GetTransitionProperties(
        /* [out] */ ArrayOf<String>** result);

    CARAPI CreateAnimator(
        /* [in] */ IViewGroup* sceneRoot,
        /* [in] */ ITransitionValues* startValues,
        /* [in] */ ITransitionValues* endValues,
        /* [out] */ IAnimator** result);

private:
    CARAPI_(void) CaptureValues(
        /* [in] */ ITransitionValues* transitionValues);

    CARAPI_(AutoPtr<IObjectAnimator>) CreateNullAnimator(
        /* [in] */ IImageView* imageView);

    CARAPI_(AutoPtr<IObjectAnimator>) CreateMatrixAnimator(
        /* [in] */ IImageView* imageView,
        /* [in] */ IMatrix* startMatrix,
        /* [in] */ IMatrix* endMatrix);

private:
    static String TAG;

    static String PROPNAME_MATRIX;
    static String PROPNAME_BOUNDS;

    static AutoPtr<ArrayOf<String> > sTransitionProperties;

    static AutoPtr<ITypeEvaluator> NULL_MATRIX_EVALUATOR;

    static AutoPtr<IProperty> ANIMATED_TRANSFORM_PROPERTY;
};

} // namespace Transition
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TRANSITION_CCHANGEIMAGETRANSFORM_H__
