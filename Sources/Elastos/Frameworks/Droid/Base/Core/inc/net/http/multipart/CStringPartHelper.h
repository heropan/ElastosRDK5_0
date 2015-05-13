
#ifndef __CSTRINGPARTHELPER_H__
#define __CSTRINGPARTHELPER_H__

#include "_CStringPartHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

CarClass(CStringPartHelper)
{
public:
    CARAPI GetBoundary(
        /* [out] */ String* boundary);

    CARAPI SendParts(
        /* [in] */ Elastos::IO::IOutputStream* outStream,
        /* [in] */ ArrayOf<IPart *>* iparts,
        /* [in] */ ArrayOf<Byte>* partBoundary);

    CARAPI GetLengthOfParts(
        /* [in] */ ArrayOf<IPart *>* iparts,
        /* [out] */ Int64* length);

    CARAPI GetLengthOfPartsEx(
        /* [in] */ ArrayOf<IPart *>* iparts,
        /* [in] */ ArrayOf<Byte>* partBoundary,
        /* [out] */ Int64* length);
};

}
}
}
}
}
}

#endif // __CSTRINGPARTHELPER_H__
