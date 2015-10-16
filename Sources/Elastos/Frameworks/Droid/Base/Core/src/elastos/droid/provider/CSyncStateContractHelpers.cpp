
#include "elastos/droid/provider/CSyncStateContractHelpers.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CSyncStateContractHelpers::Get(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ IAccount* account,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);

    return SyncStateContractHelpers::Get(provider, uri, account, value);
}

ECode CSyncStateContractHelpers::Set(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data)
{
    return SyncStateContractHelpers::Set(provider, uri, account, data);
}

ECode CSyncStateContractHelpers::Insert(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [out] */ IUri** retUri)
{
    VALIDATE_NOT_NULL(retUri);

    return SyncStateContractHelpers::Insert(provider, uri, account, data, retUri);
}

ECode CSyncStateContractHelpers::Update(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<Byte>& data)
{
    return SyncStateContractHelpers::Update(provider, uri, data);
}

ECode CSyncStateContractHelpers::NewSetOperation(
        /* [in] */ IUri* uri,
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [out] */ IContentProviderOperation** operation)
{
    VALIDATE_NOT_NULL(operation);

    return SyncStateContractHelpers::NewSetOperation(uri, account, data, operation);
}

ECode CSyncStateContractHelpers::NewUpdateOperation(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [out] */ IContentProviderOperation** operation)
{
    VALIDATE_NOT_NULL(operation);

    return SyncStateContractHelpers::NewUpdateOperation(uri, data, operation);
}

} //Provider
} //Droid
} //Elastos