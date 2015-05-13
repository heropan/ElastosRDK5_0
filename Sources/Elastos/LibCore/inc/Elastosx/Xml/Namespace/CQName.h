
#ifndef __CQNAME_H__
#define __CQNAME_H__

#include "_CQName.h"

namespace Elastosx {
namespace Xml {
namespace Namespace {

CarClass(CQName)
{
public:
    CARAPI GetNamespaceURI(
        /* [out] */ String * pUri);

    CARAPI GetLocalPart(
        /* [out] */ String * pPart);

    CARAPI GetPrefix(
        /* [out] */ String * pPrefix);

    CARAPI Equals(
        /* [in] */ IInterface * pObjectToTest,
        /* [out] */ Boolean * pIsEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI constructor(
        /* [in] */ const String& namespaceURI,
        /* [in] */ const String& localPart);

    CARAPI constructor(
        /* [in] */ const String& namespaceURI,
        /* [in] */ const String& localPart,
        /* [in] */ const String& prefix);

    CARAPI constructor(
        /* [in] */ const String& localPart);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CQNAME_H__
