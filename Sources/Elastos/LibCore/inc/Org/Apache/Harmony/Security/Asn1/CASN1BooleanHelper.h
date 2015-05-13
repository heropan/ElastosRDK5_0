
#ifndef __CASN1BOOLEANHELPER_H__
#define __CASN1BOOLEANHELPER_H__

#include "_CASN1BooleanHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1BooleanHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1Type** instance);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1BOOLEANHELPER_H__
