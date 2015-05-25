
#ifndef __CALGNAMEMAPPERHELPER_H__
#define __CALGNAMEMAPPERHELPER_H__

#include "_CAlgNameMapperHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

CarClass(CAlgNameMapperHelper)
{
public:
    CARAPI Map2OID(
        /* [in] */ const String& algName,
        /* [out] */ String * pOid);

    CARAPI Map2AlgName(
        /* [in] */ const String& oid,
        /* [out] */ String * pAlgName);

    CARAPI GetStandardName(
        /* [in] */ const String& algName,
        /* [out] */ String * pStandardName);

    CARAPI IsOID(
        /* [in] */ const String& alias,
        /* [out] */ Boolean * pIsOid);

    CARAPI Normalize(
        /* [in] */ const String& oid,
        /* [out] */ String * pNormalString);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CALGNAMEMAPPERHELPER_H__
