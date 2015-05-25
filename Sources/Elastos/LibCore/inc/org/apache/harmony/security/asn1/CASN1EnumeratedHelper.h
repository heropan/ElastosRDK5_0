
#ifndef __CASN1ENUMERATEDHELPER_H__
#define __CASN1ENUMERATEDHELPER_H__

#include "_CASN1EnumeratedHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1EnumeratedHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1Enumerated** instance);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1ENUMERATEDHELPER_H__
