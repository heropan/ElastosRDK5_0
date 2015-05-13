
#ifndef __CCOPYONWRITEARRAYLISTHELPER_H__
#define __CCOPYONWRITEARRAYLISTHELPER_H__

#include "_CCopyOnWriteArrayListHelper.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CCopyOnWriteArrayListHelper)
{
public:
    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [in] */ const ArrayOf<IInterface*>& snapshot,
        /* [in] */ Int32 from,
        /* [in] */ Int32 to,
        /* [out] */ Boolean* value);

    CARAPI LastIndexOf(
        /* [in] */ IInterface* o,
        /* [in] */ const ArrayOf<IInterface*>& data,
        /* [in] */ Int32 from,
        /* [in] */ Int32 to,
        /* [out] */ Int32* value);

    CARAPI IndexOf(
        /* [in] */ IInterface* o,
        /* [in] */ const ArrayOf<IInterface*>& data,
        /* [in] */ Int32 from,
        /* [in] */ Int32 to,
        /* [out] */ Int32* value);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CCOPYONWRITEARRAYLISTHELPER_H__
