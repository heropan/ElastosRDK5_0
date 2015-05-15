
#ifndef __CPACK200HELPER_H__
#define __CPACK200HELPER_H__

#include "_CPack200Helper.h"

using Elastos::Utility::Jar::IPacker;
using Elastos::Utility::Jar::IUnpacker;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CPack200Helper)
{
public:
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

#endif // __CPACK200HELPER_H__
