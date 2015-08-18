
#ifndef __ELASTOS_DROID_VIEW_CPOINTERPROPERTIES_H__
#define __ELASTOS_DROID_VIEW_CPOINTERPROPERTIES_H__

#include "_Elastos_Droid_View_CPointerProperties.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CPointerProperties)
{
public:
    CPointerProperties();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPointerProperties* other);

    /**
     * The pointer id.
     * Initially set to {@link #INVALID_POINTER_ID} (-1).
     *
     * @see MotionEvent#getPointerId(int)
     */
    CARAPI SetId(
        /* [in] */ Int32 id);

    CARAPI GetId(
        /* [out] */ Int32* id);

    /**
     * The pointer tool type.
     * Initially set to 0.
     *
     * @see MotionEvent#getToolType(int)
     */
    CARAPI SetToolType(
        /* [in] */ Int32 type);

    CARAPI GetToolType(
        /* [out] */ Int32* type);

    /**
     * Resets the pointer properties to their initial values.
     */
    CARAPI Clear();

    /**
     * Copies the contents of another pointer properties object.
     *
     * @param other The pointer properties object to copy.
     */
    CARAPI CopyFrom(
        /* [in] */ IPointerProperties* other);

    CARAPI Equals(
        /* [in] */ IPointerProperties* other,
        /* [out] */ Boolean* res);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* res);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

private:
    /**
     * The pointer id.
     * Initially set to {@link #INVALID_POINTER_ID} (-1).
     *
     * @see MotionEvent#getPointerId(int)
     */
    Int32 mId;

    /**
     * The pointer tool type.
     * Initially set to 0.
     *
     * @see MotionEvent#getToolType(int)
     */
    Int32 mToolType;
};

}
}
}

#endif // __ELASTOS_DROID_VIEW_CPOINTERPROPERTIES_H__
