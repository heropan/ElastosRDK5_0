
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/CVpnProfile.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Text::TextUtils;

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Net::CInetAddressHelper;
using Elastos::Net::IInetAddress;
using Elastos::Net::IInetAddressHelper;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

const String CVpnProfile::TAG("VpnProfile");

CVpnProfile::CVpnProfile()
    : mName("")
    , mType(IVpnProfile::TYPE_PPTP)
    , mServer("")
    , mUsername("")
    , mPassword("")
    , mDnsServers("")
    , mSearchDomains("")
    , mRoutes("")
    , mMppe(TRUE)
    , mL2tpSecret("")
    , mIpsecIdentifier("")
    , mIpsecSecret("")
    , mIpsecUserCert("")
    , mIpsecCaCert("")
    , mIpsecServerCert("")
    , mSaveLogin(FALSE)
{}

ECode CVpnProfile::constructor()
{
    return NOERROR;
}

ECode CVpnProfile::constructor(
    /* [in] */ const String& key)
{
    mKey = key;
    return NOERROR;
}

ECode CVpnProfile::Encode(
    /* [out, callee] */ ArrayOf<Byte>** value)
{
    StringBuilder builder(mName);
    builder.AppendCStr("\0");
    builder.AppendInt32(mType);
    builder.AppendCStr("\0");
    builder.AppendString(mServer);
    builder.AppendCStr("\0");
    builder.AppendString(mSaveLogin ? mUsername : String(""));
    builder.AppendCStr("\0");
    builder.AppendString(mSaveLogin ? mPassword : String(""));
    builder.AppendCStr("\0");
    builder.AppendString(mDnsServers);
    builder.AppendCStr("\0");
    builder.AppendString(mSearchDomains);
    builder.AppendCStr("\0");
    builder.AppendString(mRoutes);
    builder.AppendCStr("\0");
    builder.AppendBoolean(mMppe);
    builder.AppendCStr("\0");
    builder.AppendString(mL2tpSecret);
    builder.AppendCStr("\0");
    builder.AppendString(mIpsecIdentifier);
    builder.AppendCStr("\0");
    builder.AppendString(mIpsecSecret);
    builder.AppendCStr("\0");
    builder.AppendString(mIpsecUserCert);
    builder.AppendCStr("\0");
    builder.AppendString(mIpsecCaCert);
    builder.AppendCStr("\0");
    builder.AppendString(mIpsecServerCert);
    // TODO:
    // AutoPtr<ICharset> charsets, utf
    // CCharsets::AcquireSingleton((ICharset**)&charsets);
    // charsets->GetUTF_8((ICharset**)&utf);
    // return builder.ToString().getBytes(utf);
    return E_NOT_IMPLEMENTED;
}

/**
 * Test if profile is valid for lockdown, which requires IPv4 address for
 * both server and DNS. Server hostnames would require using DNS before
 * connection.
 */
ECode CVpnProfile::IsValidLockdownProfile(
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);
    *isValid = FALSE;
    // try {
    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
    AutoPtr<IInetAddress> address;
    FAIL_RETURN(helper->ParseNumericAddress(mServer, (IInetAddress**)&address));

    AutoPtr<ArrayOf<String> > dnsArray;
    StringUtils::Split(mDnsServers, String(" +"), (ArrayOf<String>**)&dnsArray);
    for (Int32 i = 0; i < dnsArray->GetLength(); i++) {
        String dnsServer = (*dnsArray)[i];
        address = NULL;
        FAIL_RETURN(helper->ParseNumericAddress(mDnsServers, (IInetAddress**)&address));
    }
    if (TextUtils::IsEmpty(mDnsServers)) {
        Logger::W(TAG, "DNS required");
        *isValid = FALSE;
        return NOERROR;
    }

    // Everything checked out above
    *isValid = TRUE;
    return NOERROR;

    // } catch (IllegalArgumentException e) {
    //     Log.w(TAG, "Invalid address", e);
    //     return false;
    // }
}

ECode CVpnProfile::GetKey(
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);
    *key = mKey;
    return NOERROR;
}

ECode CVpnProfile::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode CVpnProfile::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode CVpnProfile::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}

ECode CVpnProfile::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CVpnProfile::GetServer(
    /* [out] */ String* server)
{
    VALIDATE_NOT_NULL(server);
    *server = mServer;
    return NOERROR;
}

ECode CVpnProfile::SetServer(
    /* [in] */ const String& server)
{
    mServer = server;
    return NOERROR;
}

ECode CVpnProfile::GetUsername(
    /* [out] */ String* username)
{
    VALIDATE_NOT_NULL(username);
    *username = mUsername;
    return NOERROR;
}

ECode CVpnProfile::SetUsername(
    /* [in] */ const String& username)
{
    mUsername = username;
    return NOERROR;
}

ECode CVpnProfile::GetPassword(
    /* [out] */ String* password)
{
    VALIDATE_NOT_NULL(password);
    *password = mPassword;
    return NOERROR;
}

ECode CVpnProfile::SetPassword(
    /* [in] */ const String& password)
{
    mPassword = password;
    return NOERROR;
}

ECode CVpnProfile::GetDnsServers(
    /* [out] */ String* dnsServers)
{
    VALIDATE_NOT_NULL(dnsServers);
    *dnsServers = mDnsServers;
    return NOERROR;
}

ECode CVpnProfile::SetDnsServers(
    /* [in] */ const String& dnsServers)
{
    mDnsServers = dnsServers;
    return NOERROR;
}

ECode CVpnProfile::GetSearchDomains(
    /* [out] */ String* searchDomains)
{
    VALIDATE_NOT_NULL(searchDomains);
    *searchDomains = mSearchDomains;
    return NOERROR;
}

ECode CVpnProfile::SetSearchDomains(
    /* [in] */ const String& searchDomains)
{
    mSearchDomains = searchDomains;
    return NOERROR;
}

ECode CVpnProfile::GetRoutes(
    /* [out] */ String* routes)
{
    VALIDATE_NOT_NULL(routes);
    *routes = mRoutes;
    return NOERROR;
}

ECode CVpnProfile::SetRoutes(
    /* [in] */ const String& routes)
{
    mRoutes = routes;
    return NOERROR;
}

ECode CVpnProfile::GetMppe(
    /* [out] */ Boolean* mppe)
{
    VALIDATE_NOT_NULL(mppe);
    *mppe = mMppe;
    return NOERROR;
}

ECode CVpnProfile::SetMppe(
    /* [in] */ Boolean mppe)
{
    mMppe = mppe;
    return NOERROR;
}

ECode CVpnProfile::Get12tpSecret(
    /* [out] */ String* l2tpSecret)
{
    VALIDATE_NOT_NULL(l2tpSecret);
    *l2tpSecret = mL2tpSecret;
    return NOERROR;
}

ECode CVpnProfile::Set12tpSecret(
    /* [in] */ const String& l2tpSecret)
{
    mL2tpSecret = l2tpSecret;
    return NOERROR;
}

ECode CVpnProfile::GetIpsecIdentifier(
    /* [out] */ String* ipsecIdentifier)
{
    VALIDATE_NOT_NULL(ipsecIdentifier);
    *ipsecIdentifier = mIpsecIdentifier;
    return NOERROR;
}

ECode CVpnProfile::SetIpsecIdentifier(
    /* [in] */ const String& ipsecIdentifier)
{
    mIpsecIdentifier = ipsecIdentifier;
    return NOERROR;
}

ECode CVpnProfile::GetIpsecSecret(
    /* [out] */ String* ipsecSecret)
{
    VALIDATE_NOT_NULL(ipsecSecret);
    *ipsecSecret = mIpsecSecret;
    return NOERROR;
}

ECode CVpnProfile::SetIpsecSecret(
    /* [in] */ const String& ipsecSecret)
{
    mIpsecSecret = ipsecSecret;
    return NOERROR;
}

ECode CVpnProfile::GetIpsecUserCert(
    /* [out] */ String* ipsecUserCert)
{
    VALIDATE_NOT_NULL(ipsecUserCert);
    *ipsecUserCert = mIpsecUserCert;
    return NOERROR;
}

ECode CVpnProfile::SetIpsecUserCert(
    /* [in] */ const String& ipsecUserCert)
{
    mIpsecUserCert = ipsecUserCert;
    return NOERROR;
}

ECode CVpnProfile::GetIpsecCaCert(
    /* [out] */ String* ipsecCaCert)
{
    VALIDATE_NOT_NULL(ipsecCaCert);
    *ipsecCaCert = mIpsecCaCert;
    return NOERROR;
}

ECode CVpnProfile::SetIpsecCaCert(
    /* [in] */ const String& ipsecCaCert)
{
    mIpsecCaCert = ipsecCaCert;
    return NOERROR;
}

ECode CVpnProfile::GetIpsecServerCert(
    /* [out] */ String* ipsecServerCert)
{
    VALIDATE_NOT_NULL(ipsecServerCert);
    *ipsecServerCert = mIpsecServerCert;
    return NOERROR;
}

ECode CVpnProfile::SetIpsecServerCert(
    /* [in] */ const String& ipsecServerCert)
{
    mIpsecServerCert = ipsecServerCert;
    return NOERROR;
}

ECode CVpnProfile::GetSaveLogin(
    /* [out] */ Boolean* saveLogin)
{
    VALIDATE_NOT_NULL(saveLogin);
    *saveLogin = mSaveLogin;
    return NOERROR;
}

ECode CVpnProfile::SetSaveLogin(
    /* [in] */ Boolean saveLogin)
{
    mSaveLogin = saveLogin;
    return NOERROR;
}

ECode CVpnProfile::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);

    source->ReadString(&mKey);
    source->ReadString(&mName);
    source->ReadInt32(&mType);
    source->ReadString(&mServer);
    source->ReadString(&mUsername);
    source->ReadString(&mPassword);
    source->ReadString(&mDnsServers);
    source->ReadString(&mSearchDomains);
    source->ReadString(&mRoutes);
    source->ReadBoolean(&mMppe);
    source->ReadString(&mL2tpSecret);
    source->ReadString(&mIpsecIdentifier);
    source->ReadString(&mIpsecSecret);
    source->ReadString(&mIpsecUserCert);
    source->ReadString(&mIpsecCaCert);
    source->ReadString(&mIpsecServerCert);
    source->ReadBoolean(&mSaveLogin);
    return NOERROR;
}

ECode CVpnProfile::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);

    dest->WriteString(mKey);
    dest->WriteString(mName);
    dest->WriteInt32(mType);
    dest->WriteString(mServer);
    dest->WriteString(mUsername);
    dest->WriteString(mPassword);
    dest->WriteString(mDnsServers);
    dest->WriteString(mSearchDomains);
    dest->WriteString(mRoutes);
    dest->WriteBoolean(mMppe);
    dest->WriteString(mL2tpSecret);
    dest->WriteString(mIpsecIdentifier);
    dest->WriteString(mIpsecSecret);
    dest->WriteString(mIpsecUserCert);
    dest->WriteString(mIpsecCaCert);
    dest->WriteString(mIpsecServerCert);
    dest->WriteBoolean(mSaveLogin);
    return NOERROR;
}

ECode CVpnProfile::Decode(
    /* [in] */ const String& key,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ IVpnProfile** profile)
{
    VALIDATE_NOT_NULL(profile);
    *profile = NULL;
    // try {
    if (key.IsNull()) {
        return NOERROR;
    }

    // TODO:
    //     String[] values = String(value, Charsets.UTF_8).split("\0", -1);
    //     // There can be 14 or 15 values in ICS MR1.
    //     if (values.length < 14 || values.length > 15) {
    //         return null;
    //     }

    //     VpnProfile profile = new VpnProfile(key);
    //     profile.name = values[0];
    //     profile.type = Integer.valueOf(values[1]);
    //     if (profile.type < 0 || profile.type > TYPE_MAX) {
    //         return null;
    //     }
    //     profile.server = values[2];
    //     profile.username = values[3];
    //     profile.password = values[4];
    //     profile.dnsServers = values[5];
    //     profile.searchDomains = values[6];
    //     profile.routes = values[7];
    //     profile.mppe = Boolean.valueOf(values[8]);
    //     profile.l2tpSecret = values[9];
    //     profile.ipsecIdentifier = values[10];
    //     profile.ipsecSecret = values[11];
    //     profile.ipsecUserCert = values[12];
    //     profile.ipsecCaCert = values[13];
    //     profile.ipsecServerCert = (values.length > 14) ? values[14] : "";

    //     profile.saveLogin = !profile.username.isEmpty() || !profile.password.isEmpty();
    //     return profile;
    // } catch (Exception e) {
    //     // ignore
    // }
    // return null;
    return E_NOT_IMPLEMENTED;
}

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos
