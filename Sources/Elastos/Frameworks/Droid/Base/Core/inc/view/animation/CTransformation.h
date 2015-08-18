
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CTRANSFORMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CTRANSFORMATION_H__

#include "_Elastos_Droid_View_Animation_CTransformation.h"
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
    CTransformation();

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

    /**
     * Sets the current Transform's clip rect
     * @hide
     */
    CARAPI SetClipRect(
        /* [in] */ IRect* r);

    /**
     * Sets the current Transform's clip rect
     * @hide
     */
    CARAPI SetClipRect(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);

    /**
     * Returns the current Transform's clip rect
     * @hide
     */
    CARAPI GetClipRect(
        /* [out] */ IRect** cr);

    /**
     * Returns whether the current Transform's clip rect is set
     * @hide
     */
    CARAPI HasClipRect(
        /* [out] */ Boolean* has);

    CARAPI GetAlpha(
        /* [out] */ Float* pAlpha);

protected:
    AutoPtr<IMatrix> mMatrix;
    Float mAlpha;
    Int32 mTransformationType;

    Boolean mHasClipRect;
    AutoPtr<IRect> mClipRect;
};


}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CTRANSFORMATION_H__
