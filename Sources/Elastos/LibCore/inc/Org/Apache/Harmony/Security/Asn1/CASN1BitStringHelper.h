
#ifndef __CASN1BITSTRINGHELPER_H__
#define __CASN1BITSTRINGHELPER_H__

#include "_CASN1BitStringHelper.h"
#include "CASN1BitString.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1BitStringHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1BitString** instance);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1BITSTRINGHELPER_H__
