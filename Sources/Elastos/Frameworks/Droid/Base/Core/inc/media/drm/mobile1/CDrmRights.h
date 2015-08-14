
#ifndef __ELASTOS_DROID_DRM_MOBILE1_CDRMRIGHTS_H__
#define __ELASTOS_DROID_DRM_MOBILE1_CDRMRIGHTS_H__

#include "_CDrmRights.h"
#include "media/drm/mobile1/CDrmConstraintInfo.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Drm::Mobile1::IDrmConstraintInfo;
using Elastos::Droid::Drm::Mobile1::CDrmConstraintInfo;

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

CarClass(CDrmRights)
{
public:
    CDrmRights();

    CARAPI GetConstraint(
        /*[in] */ Int32 permission,
        /*[out] */ IDrmConstraintInfo** constraint);

    CARAPI ConsumeRights(
        /* [in] */ Int32 permission,
        /* [out] */ Boolean* result);

    CARAPI GetRoId(
        /* [out] */ String* id);

private:
    /**
     * native method: get the constraint information of the given permission.
     *
     * @param permission    the given permission.
     * @param constraint    the instance of constraint.
     *
     * @return #JNI_DRM_SUCCESS if succeed.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeGetConstraintInfo(
        /* [in] */ Int32 permission,
        /* [in] */ CDrmConstraintInfo* constraint);

    CARAPI_(Int32) NativeConsumeRights(
        /* [in] */ Int32 permission);

public:
    /**
     * Successful operation.
     */
    static const Int32 JNI_DRM_SUCCESS = 0;

    /**
     * General failure.
     */
    static const Int32 JNI_DRM_FAILURE = -1;

    /**
     * The uid of this rights object.
     */
    String mRoId;
};

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_DRM_MOBILE1_CDRMRIGHTS_H__
