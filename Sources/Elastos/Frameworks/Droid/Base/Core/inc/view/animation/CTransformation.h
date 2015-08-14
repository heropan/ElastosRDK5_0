
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CTRANSFORMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CTRANSFORMATION_H__

#include "_CTransformation.h"
#include "ext/frameworkext.h"
#include "graphics/CMatrix.h"

using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CMatrix;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CTransformation)
{
public:
    CARAPI constructor();

    CARAPI Clear();

    CARAPI GetTransformationType(
        /* [out] */ Int32* type);

    CARAPI SetTransformationType(
        /* [in] */ Int32 transformationType);

    CARAPI Set(
        /* [in] */ ITransformation* t);

    CARAPI Compose(
        /* [in] */ ITransformation* t);

    CARAPI PostCompose(
        /* [in] */ ITransformation* t);

    CARAPI GetMatrix(
        /* [out] */ IMatrix** matrix);

    CARAPI SetAlpha(
        /* [in] */ Float alpha);

    CARAPI GetAlpha(
        /* [out] */ Float* pAlpha);

protected:
    AutoPtr<IMatrix> mMatrix;
    Float mAlpha;
    Int32 mTransformationType;
};


}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CTRANSFORMATION_H__
