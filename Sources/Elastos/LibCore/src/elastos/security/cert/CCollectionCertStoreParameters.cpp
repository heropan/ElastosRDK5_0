
#include "CCollectionCertStoreParameters.h"
#include "StringBuilder.h"
#include "elastos/ObjectUtils.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Security {
namespace Cert {

const AutoPtr<ICollection> CCollectionCertStoreParameters::mDefaultCollection; // = Collections.EMPTY_SET;

ECode CCollectionCertStoreParameters::Clone(
    /* [out] */ IInterface **object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<ICollectionCertStoreParameters> obj;
    FAIL_RETURN(CCollectionCertStoreParameters::New((ICollectionCertStoreParameters**)&obj))
    *object = obj.Get();
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CCollectionCertStoreParameters::GetCollection(
    /* [out] */ ICollection **collection)
{
    VALIDATE_NOT_NULL(collection)
    *collection = mCollection;
    REFCOUNT_ADD(*collection)
    return NOERROR;
}

ECode CCollectionCertStoreParameters::ToString(
    /* [out] */ String *str)
{
    StringBuilder sb("CollectionCertStoreParameters: [\ncollection: ");
    AutoPtr<ICollection> coll;
    GetCollection((ICollection**)&coll);
    String strCol = ObjectUtils::ToString(coll.Get());
    sb.AppendString(strCol);
    sb.AppendCStr("\n]");
    return sb.ToString(str);
}

ECode CCollectionCertStoreParameters::constructor()
{
    mCollection = mDefaultCollection;
    return NOERROR;
}

ECode CCollectionCertStoreParameters::constructor(
    /* [in] */ ICollection *collection)
{
    if (collection == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mCollection = collection;
    return NOERROR;
}

}
}
}

