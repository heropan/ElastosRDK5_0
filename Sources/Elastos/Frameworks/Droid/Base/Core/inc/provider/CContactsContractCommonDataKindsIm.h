
#ifndef __CCONTACTSCONTRACTCOMMONDATAKINDSIM_H__
#define __CCONTACTSCONTRACTCOMMONDATAKINDSIM_H__

#include "_CContactsContractCommonDataKindsIm.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractCommonDataKindsIm)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * Return the string resource that best describes the given
     * {@link #TYPE}. Will always return a valid resource.
     */
    CARAPI GetTypeLabelResource(
        /* [in] */ Int32 type,
        /* [out] */ Int32* resource);

    /**
     * Return a {@link CharSequence} that best describes the given type,
     * possibly substituting the given {@link #LABEL} value
     * for {@link #TYPE_CUSTOM}.
     */
    CARAPI GetTypeLabel(
        /* [in] */ IResources* res,
        /* [in] */ Int32 type,
        /* [in] */ ICharSequence* label,
        /* [out] */ ICharSequence** lb);

    /**
     * Return the string resource that best describes the given
     * {@link #PROTOCOL}. Will always return a valid resource.
     */
    CARAPI GetProtocolLabelResource(
        /* [in] */ Int32 type,
        /* [out] */ Int32* resource);

    /**
     * Return a {@link CharSequence} that best describes the given
     * protocol, possibly substituting the given
     * {@link #CUSTOM_PROTOCOL} value for {@link #PROTOCOL_CUSTOM}.
     */
    CARAPI GetProtocolLabel(
        /* [in] */ IResources* res,
        /* [in] */ Int32 type,
        /* [in] */ ICharSequence* label,
        /* [out] */ ICharSequence** lb);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTCOMMONDATAKINDSIM_H__