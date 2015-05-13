
#ifndef __CPRIVACYSETTINGSSTUB_H__
#define __CPRIVACYSETTINGSSTUB_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Privacy {

class PrivacySettingsStub
    : public ElRefBase
    , public IInterface
{
public:
    CAR_INTERFACE_DECL();

    CARAPI_(Boolean) IsStub();

private:
    static const Boolean sIsStub;
};

} // namespace Privacy
} // namespace Droid
} // namespace Elastos

#endif //__CPRIVACYSETTINGSSTUB_H__