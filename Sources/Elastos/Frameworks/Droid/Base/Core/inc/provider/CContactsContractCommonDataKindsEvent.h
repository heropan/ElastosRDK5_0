
#ifndef __CCONTACTSCONTRACTCOMMONDATAKINDSEVENT_H__
#define __CCONTACTSCONTRACTCOMMONDATAKINDSEVENT_H__

#include "_CContactsContractCommonDataKindsEvent.h"

using Elastos::Core::IInteger32;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractCommonDataKindsEvent)
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
    CARAPI GetTypeResource(
        /* [in] */ IInteger32* type,
        /* [out] */ Int32* resource);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTCOMMONDATAKINDSEVENT_H__