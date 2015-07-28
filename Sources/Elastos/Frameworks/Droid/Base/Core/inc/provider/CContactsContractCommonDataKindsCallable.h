
#ifndef __CCONTACTSCONTRACTCOMMONDATAKINDSCALLABLE_H__
#define __CCONTACTSCONTRACTCOMMONDATAKINDSCALLABLE_H__

#include "_CContactsContractCommonDataKindsCallable.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractCommonDataKindsCallable)
{
public:
    CARAPI constructor();

    /**
     * Similar to {@link Phone#CONTENT_URI}, but returns callable data instead of only
     * phone numbers.
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
    /**
     * Similar to {@link Phone#CONTENT_FILTER_URI}, but allows users to filter callable
     * data.
     */
    CARAPI GetCONTENT_FILTER_URI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTCOMMONDATAKINDSCALLABLE_H__