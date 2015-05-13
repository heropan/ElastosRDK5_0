#include "ext/frameworkext.h"
#include "app/CContentProviderHolder.h"

namespace Elastos {
namespace Droid {
namespace App {

CContentProviderHolder::CContentProviderHolder()
    : mNoReleaseNeeded(FALSE)
{
}

ECode CContentProviderHolder::constructor()
{
    return NOERROR;
}

ECode CContentProviderHolder::constructor(
    /* [in] */ IProviderInfo* info)
{
    mInfo = info;
    return NOERROR;
}

ECode CContentProviderHolder::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInterfacePtr(mInfo.Get());
    dest->WriteInterfacePtr(mProvider.Get());
    dest->WriteInterfacePtr(mConnection.Get());
    dest->WriteBoolean(mNoReleaseNeeded);
    return NOERROR;
}

ECode CContentProviderHolder::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInterfacePtr((Handle32*)&mInfo);
    source->ReadInterfacePtr((Handle32*)&mProvider);
    // TODO:
//    provider = ContentProviderNative.asInterface(
//        source.readStrongBinder());
    source->ReadInterfacePtr((Handle32*)&mConnection);

    source->ReadBoolean(&mNoReleaseNeeded);
    return NOERROR;
}

ECode CContentProviderHolder::GetProviderInfo(
    /* [out] */ IProviderInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mInfo;
    INTERFACE_ADDREF(*info);
    return NOERROR;
}

ECode CContentProviderHolder::SetProviderInfo(
    /* [in] */ IProviderInfo* info)
{
    mInfo = info;
    return NOERROR;
}

ECode CContentProviderHolder::GetContentProvider(
    /* [out] */ IIContentProvider** provider)
{
    VALIDATE_NOT_NULL(provider);
    *provider = mProvider;
    INTERFACE_ADDREF(*provider);
    return NOERROR;
}

ECode CContentProviderHolder::SetContentProvider(
    /* [in] */ IIContentProvider* provider)
{
    mProvider = provider;
    return NOERROR;
}

ECode CContentProviderHolder::GetConnection(
    /* [out] */ IBinder** connection)
{
    VALIDATE_NOT_NULL(connection);
    *connection = mConnection;
    INTERFACE_ADDREF(*connection);
    return NOERROR;
}

ECode CContentProviderHolder::SetConnection(
    /* [in] */ IBinder* connection)
{
    mConnection = connection;
    return NOERROR;
}

ECode CContentProviderHolder::GetNoReleaseNeeded(
    /* [out] */ Boolean* noReleaseNeeded)
{
    VALIDATE_NOT_NULL(noReleaseNeeded);
    *noReleaseNeeded = mNoReleaseNeeded;
    return NOERROR;
}

ECode CContentProviderHolder::SetNoReleaseNeeded(
    /* [in] */ Boolean noReleaseNeeded)
{
    mNoReleaseNeeded = noReleaseNeeded;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos

