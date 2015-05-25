
#ifndef __CSHA1PRNG_SECURERANDOMIMPL_H__
#define __CSHA1PRNG_SECURERANDOMIMPL_H__

#include "_CSHA1PRNG_SecureRandomImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Provider {
namespace Crypto {

CarClass(CSHA1PRNG_SecureRandomImpl)
{
public:
    CARAPI EngineSetSeed(
        /* [in] */ ArrayOf<Byte> * pSeed);

    CARAPI EngineNextBytes(
        /* [in] */ ArrayOf<Byte> * pBytes);

    CARAPI EngineGenerateSeed(
        /* [in] */ Int32 numBytes,
        /* [out, callee] */ ArrayOf<Byte> ** ppSeed);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSHA1PRNG_SECURERANDOMIMPL_H__
