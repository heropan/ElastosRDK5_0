
#ifndef __ELASTOS_UTILITY_CPACK200HELPER_H__
#define __ELASTOS_UTILITY_CPACK200HELPER_H__

#include "_Elastos_Utility_Jar_CPack200Helper.h"
#include "Singleton.h"

using Elastos::Utility::Jar::IPacker;
using Elastos::Utility::Jar::IUnpacker;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CPack200Helper)
    , public Singleton
    , public IPack200Helper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI NewPacker(
        /* [out] */ IPacker** packer);

    CARAPI NewUnpacker(
        /* [out] */ IUnpacker** unpacker);

private:
    // TODO: Add your private member variables here.
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_CPACK200HELPER_H__
