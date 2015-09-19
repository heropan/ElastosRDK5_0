
#include "provider/SyncStateContractHelpers.h"
#include "content/CContentValues.h"
#include "content/CContentProviderOperationHelper.h"

using Elastos::Core::IArrayOf;
using Elastos::Core::CArrayOf;
using Elastos::Core::EIID_IByte;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::IContentProviderOperationHelper;
using Elastos::Droid::Content::CContentProviderOperationHelper;
using Elastos::Droid::Content::IContentProviderOperationBuilder;

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<ArrayOf<String> > initDATAPROJECTION()
{
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(2);
    (*args)[0] = ISyncStateContractColumns::DATA;
    (*args)[0] = IBaseColumns::ID;
    return args;
}

AutoPtr<ArrayOf<String> > SyncStateContractHelpers::DATA_PROJECTION = initDATAPROJECTION();

const String SyncStateContractHelpers::SELECT_BY_ACCOUNT = ISyncStateContractColumns::ACCOUNT_NAME + String("=? AND ")
                                                    + ISyncStateContractColumns::ACCOUNT_TYPE + String("=?");

ECode SyncStateContractHelpers::Get(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ IAccount* account,
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    VALIDATE_NOT_NULL(value);

    AutoPtr<ICursor> c;
    AutoPtr<ArrayOf<String> > args = ArrayOf<String>::Alloc(2);
    FAIL_RETURN(account->GetName(&(*args)[0]))
    FAIL_RETURN(account->GetType(&(*args)[1]))
    FAIL_RETURN(provider->Query(uri, DATA_PROJECTION, SELECT_BY_ACCOUNT, args, String(NULL), (ICursor**)&c))

    // Unable to query the provider
    if (c == NULL) {
        //throw new RemoteException();
        return E_REMOTE_EXCEPTION;
    }

    //try {
    Boolean result;
    FAIL_GOTO(c->MoveToNext(&result), EXIT)
    if (result) {
        Int32 columnIndex;
        FAIL_GOTO(c->GetColumnIndexOrThrow(ISyncStateContractColumns::DATA, &columnIndex), EXIT)
        FAIL_GOTO(c->GetBlob(columnIndex, value), EXIT)
        return c->Close();
    }
    //} finally {
EXIT:
    FAIL_RETURN(c->Close())
    //}
    *value = NULL;
    return NOERROR;
}

ECode SyncStateContractHelpers::Set(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data)
{
    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    AutoPtr<IArrayOf> array;
    FAIL_RETURN(CArrayOf::New(EIID_IByte, data.GetLength(), (IArrayOf**)&array))
    FAIL_RETURN(values->PutBytes(ISyncStateContractColumns::DATA, array))
    String name, type;
    FAIL_RETURN(account->GetName(&name))
    FAIL_RETURN(account->GetType(&type))
    AutoPtr<ICharSequence> cname, ctype;
    FAIL_RETURN(CString::New(name, (ICharSequence**)&cname))
    FAIL_RETURN(CString::New(type, (ICharSequence**)&ctype))
    FAIL_RETURN(values->PutString(ISyncStateContractColumns::ACCOUNT_NAME, cname))
    FAIL_RETURN(values->PutString(ISyncStateContractColumns::ACCOUNT_TYPE, ctype))
    AutoPtr<IUri> _uri;
    return provider->Insert(uri, values, (IUri**)&_uri);
}

ECode SyncStateContractHelpers::Insert(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<Byte>& data,
    /* [out] */ IUri** retUri)
{
    VALIDATE_NOT_NULL(retUri);

    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    AutoPtr<IArrayOf> array;
    FAIL_RETURN(CArrayOf::New(EIID_IByte, data.GetLength(), (IArrayOf**)&array))
    FAIL_RETURN(values->PutBytes(ISyncStateContractColumns::DATA, array))
    String name, type;
    FAIL_RETURN(account->GetName(&name))
    FAIL_RETURN(account->GetType(&type))
    AutoPtr<ICharSequence> cname, ctype;
    FAIL_RETURN(CString::New(name, (ICharSequence**)&cname))
    FAIL_RETURN(CString::New(type, (ICharSequence**)&ctype))
    FAIL_RETURN(values->PutString(ISyncStateContractColumns::ACCOUNT_NAME, cname))
    FAIL_RETURN(values->PutString(ISyncStateContractColumns::ACCOUNT_TYPE, ctype))
    return provider->Insert(uri, values, retUri);
}

ECode SyncStateContractHelpers::Update(
    /* [in] */ IContentProviderClient* provider,
    /* [in] */ IUri* uri,
    /* [in] */ const ArrayOf<Byte>& data)
{
    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    AutoPtr<IArrayOf> array;
    FAIL_RETURN(CArrayOf::New(EIID_IByte, data.GetLength(), (IArrayOf**)&array))
    FAIL_RETURN(values->PutBytes(ISyncStateContractColumns::DATA, array))
    Int32 rowsAffected;
    return provider->Update(uri, values, String(NULL), NULL, &rowsAffected);
}

ECode SyncStateContractHelpers::NewSetOperation(
        /* [in] */ IUri* uri,
        /* [in] */ IAccount* account,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [out] */ IContentProviderOperation** operation)
{
    VALIDATE_NOT_NULL(operation);

    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    AutoPtr<IArrayOf> array;
    FAIL_RETURN(CArrayOf::New(EIID_IByte, data.GetLength(), (IArrayOf**)&array))
    FAIL_RETURN(values->PutBytes(ISyncStateContractColumns::DATA, array))

    AutoPtr<IContentProviderOperationHelper> helper;
    FAIL_RETURN(CContentProviderOperationHelper::AcquireSingleton((IContentProviderOperationHelper**)&helper))
    AutoPtr<IContentProviderOperationBuilder> builder;
    FAIL_RETURN(helper->NewInsert(uri, (IContentProviderOperationBuilder**)&builder))

    String name, type;
    FAIL_RETURN(account->GetName(&name))
    FAIL_RETURN(account->GetType(&type))
    AutoPtr<ICharSequence> cname, ctype;
    FAIL_RETURN(CString::New(name, (ICharSequence**)&cname))
    FAIL_RETURN(CString::New(type, (ICharSequence**)&ctype))
    FAIL_RETURN(builder->WithValue(ISyncStateContractColumns::ACCOUNT_NAME, (IInterface*)cname))
    FAIL_RETURN(builder->WithValue(ISyncStateContractColumns::ACCOUNT_TYPE, (IInterface*)ctype))
    return builder->Build(operation);
}

ECode SyncStateContractHelpers::NewUpdateOperation(
        /* [in] */ IUri* uri,
        /* [in] */ const ArrayOf<Byte>& data,
        /* [out] */ IContentProviderOperation** operation)
{
    VALIDATE_NOT_NULL(operation);

    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))

    AutoPtr<IArrayOf> array;
    FAIL_RETURN(CArrayOf::New(EIID_IByte, data.GetLength(), (IArrayOf**)&array))
    FAIL_RETURN(values->PutBytes(ISyncStateContractColumns::DATA, array))

    AutoPtr<IContentProviderOperationHelper> helper;
    FAIL_RETURN(CContentProviderOperationHelper::AcquireSingleton((IContentProviderOperationHelper**)&helper))
    AutoPtr<IContentProviderOperationBuilder> builder;
    FAIL_RETURN(helper->NewUpdate(uri, (IContentProviderOperationBuilder**)&builder))
    FAIL_RETURN(builder->WithValues(values))
    return builder->Build(operation);
}

} //Provider
} //Droid
} //Elastos