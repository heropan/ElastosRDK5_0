
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/app/CPendingIntentHelper.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/net/CVpnConfig.h"

using Elastos::Droid::App::CPendingIntentHelper;
using Elastos::Droid::App::IPendingIntentHelper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CVpnConfig::CVpnConfig()
    : mMtu(-1)
    , mStartTime(-1)
    , mLegacy(FALSE)
{}

ECode CVpnConfig::constructor()
{
    return NOERROR;
}

ECode CVpnConfig::GetUser(
    /* [out] */ String* user)
{
    VALIDATE_NOT_NULL(user);
    *user = mUser;
    return NOERROR;
}

ECode CVpnConfig::SetUser(
    /* [in] */ const String& user)
{
    mUser = user;
    return NOERROR;
}

ECode CVpnConfig::GetInterfaze(
    /* [out] */ String* interfaze)
{
    VALIDATE_NOT_NULL(interfaze);
    *interfaze = mInterfaze;
    return NOERROR;
}

ECode CVpnConfig::SetInterfaze(
    /* [in] */ const String& interfaze)
{
    mInterfaze = interfaze;
    return NOERROR;
}

ECode CVpnConfig::GetSession(
    /* [out] */ String* session)
{
    VALIDATE_NOT_NULL(session);
    *session = mSession;
    return NOERROR;
}

ECode CVpnConfig::SetSession(
    /* [in] */ const String& session)
{
    mSession = session;
    return NOERROR;
}

ECode CVpnConfig::GetMtu(
    /* [out] */ Int32* mtu)
{
    VALIDATE_NOT_NULL(mtu);
    *mtu = mMtu;
    return NOERROR;
}

ECode CVpnConfig::SetMtu(
    /* [in] */ Int32 mtu)
{
    mMtu = mtu;
    return NOERROR;
}

ECode CVpnConfig::GetAddresses(
    /* [out] */ String* addresses)
{
    VALIDATE_NOT_NULL(addresses);
    *addresses = mAddresses;
    return NOERROR;
}

ECode CVpnConfig::SetAddresses(
    /* [in] */ const String& addresses)
{
    mAddresses = addresses;
    return NOERROR;
}

ECode CVpnConfig::GetRoutes(
    /* [out] */ String* routes)
{
    VALIDATE_NOT_NULL(routes);
    *routes = mRoutes;
    return NOERROR;
}

ECode CVpnConfig::SetRoutes(
    /* [in] */ const String& routes)
{
    mRoutes = routes;
    return NOERROR;
}

ECode CVpnConfig::GetDnsServers(
    /* [out] */ IObjectContainer** dnsServers)
{
    VALIDATE_NOT_NULL(dnsServers);

    *dnsServers = mDnsServers;
    REFCOUNT_ADD(*dnsServers);
    return NOERROR;
}

ECode CVpnConfig::SetDnsServers(
    /* [in] */ IObjectContainer* dnsServers)
{
    mDnsServers = dnsServers;
    return NOERROR;
}

ECode CVpnConfig::GetSearchDomains(
    /* [out] */ IObjectContainer** searchDomains)
{
    VALIDATE_NOT_NULL(searchDomains);

    *searchDomains = mSearchDomains;
    REFCOUNT_ADD(*searchDomains);
    return NOERROR;
}

ECode CVpnConfig::SetSearchDomains(
    /* [in] */ IObjectContainer* searchDomains)
{
    mSearchDomains = searchDomains;
    return NOERROR;
}

ECode CVpnConfig::GetConfigureIntent(
    /* [out] */ IPendingIntent** configureIntent)
{
    VALIDATE_NOT_NULL(configureIntent);

    *configureIntent = mConfigureIntent;
    REFCOUNT_ADD(*configureIntent);
    return NOERROR;
}

ECode CVpnConfig::SetConfigureIntent(
    /* [in] */ IPendingIntent* configureIntent)
{
    mConfigureIntent = configureIntent;
    return NOERROR;
}

ECode CVpnConfig::GetStartTime(
    /* [out] */ Int64* startTime)
{
    VALIDATE_NOT_NULL(startTime);

    *startTime = mStartTime;
    return NOERROR;
}

ECode CVpnConfig::SetStartTime(
    /* [in] */ Int64 startTime)
{
    mStartTime = startTime;
    return NOERROR;
}

ECode CVpnConfig::GetLegacy(
    /* [out] */ Boolean* legacy)
{
    VALIDATE_NOT_NULL(legacy);

    *legacy = mLegacy;
    return NOERROR;
}

ECode CVpnConfig::SetLegacy(
    /* [in] */ Boolean legacy)
{
    mLegacy = legacy;
    return NOERROR;
}

ECode CVpnConfig::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mUser);
    source->ReadString(&mInterfaze);
    source->ReadString(&mSession);
    source->ReadInt32(&mMtu);
    source->ReadString(&mAddresses);
    source->ReadString(&mRoutes);
    mDnsServers = NULL;
    source->ReadInterfacePtr((Handle32*)&mDnsServers);
    mSearchDomains = NULL;
    source->ReadInterfacePtr((Handle32*)&mSearchDomains);
    mConfigureIntent = NULL;
    source->ReadInterfacePtr((Handle32*)&mConfigureIntent);
    source->ReadInt64(&mStartTime);
    Int32 value;
    source->ReadInt32(&value);
    mLegacy = value != 0;
    return NOERROR;
}

ECode CVpnConfig::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mUser);
    dest->WriteString(mInterfaze);
    dest->WriteString(mSession);
    dest->WriteInt32(mMtu);
    dest->WriteString(mAddresses);
    dest->WriteString(mRoutes);
    dest->WriteInterfacePtr(mDnsServers);
    dest->WriteInterfacePtr(mSearchDomains);
    dest->WriteInterfacePtr(mConfigureIntent);
    dest->WriteInt64(mStartTime);
    dest->WriteInt32(mLegacy ? 1 : 0);
    return NOERROR;
}

ECode CVpnConfig::GetIntentForConfirmation(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = NULL;

    AutoPtr<IIntent> newintent;
    CIntent::New((IIntent**)&newintent);
    newintent->SetClassName(IVpnConfig::DIALOGS_PACKAGE, IVpnConfig::DIALOGS_PACKAGE + ".ConfirmDialog");
    *intent = newintent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode CVpnConfig::GetIntentForStatusPanel(
    /* [in] */ IContext* context,
    /* [in] */ IVpnConfig* config,
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = NULL;
    // TODO:
    // Preconditions.checkNotNull(config);

    AutoPtr<IIntent> newintent;
    CIntent::New((IIntent**)&newintent);
    newintent->SetClassName(IVpnConfig::DIALOGS_PACKAGE, IVpnConfig::DIALOGS_PACKAGE + ".ManageDialog");
    newintent->PutExtra(String("config"), IParcelable::Probe(config));
    newintent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK | IIntent::FLAG_ACTIVITY_NO_HISTORY |
            IIntent::FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    return helper->GetActivity(context, 0, newintent, IPendingIntent::FLAG_CANCEL_CURRENT, intent);
}

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos
