
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/net/wifi/CWifiConfigurationKeyMgmt.h"
#include "elastos/droid/net/wifi/CWifiConfigurationProtocol.h"
#include "elastos/droid/net/wifi/CWifiConfigurationPairwiseCipher.h"
#include "elastos/droid/net/wifi/CWifiConfigurationAuthAlgorithm.h"
#include "elastos/droid/net/wifi/CWifiConfigurationGroupCipher.h"
#include "elastos/droid/net/wifi/CWifiConfiguration.h"
#include "elastos/droid/net/wifi/CEnterpriseField.h"
#include "elastos/droid/net/wifi/CWifiSsid.h"
#include "elastos/droid/net/CLinkProperties.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::CBitSet;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

//================================================================
// CWifiConfiguration::EnterpriseField
//================================================================
CAR_INTERFACE_IMPL(CWifiConfiguration::EnterpriseField, IWifiConfigurationEnterpriseField)

ECode CWifiConfiguration::EnterpriseField::SetValue(
    /* [in] */ const String& value)
{
    mValue = value;
    return NOERROR;
}

ECode CWifiConfiguration::EnterpriseField::GetVarName(
    /* [out] */ String* varName)
{
    VALIDATE_NOT_NULL(varName);
    *varName = mVarName;
    return NOERROR;
}

ECode CWifiConfiguration::EnterpriseField::GetValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}

//================================================================
// CWifiConfiguration
//================================================================
CWifiConfiguration::CWifiConfiguration()
    : mNetworkId(0)
    , mStatus(0)
    , mDisableReason(0)
    , mWepTxKeyIndex(0)
    , mPriority(0)
    , mHiddenSSID(FALSE)
{
    mEap = new EnterpriseField(String("eap"));
    mPhase2 = new EnterpriseField(String("phase2"));
    mIdentity = new EnterpriseField(String("identity"));
    mAnonymousIdentity = new EnterpriseField(String("anonymous_identity"));
    mPassword = new EnterpriseField(String("password"));
    mClientCert = new EnterpriseField(String("client_cert"));
    mEngine = new EnterpriseField(String("engine"));
    mEngineId = new EnterpriseField(String("engine_id"));
    mKeyId = new EnterpriseField(String("key_id"));
    mCaCert = new EnterpriseField(String("ca_cert"));

    mEnterpriseFields = ArrayOf<IWifiConfigurationEnterpriseField*>::Alloc(10);
    mEnterpriseFields->Set(0, mEap);
    mEnterpriseFields->Set(1, mPhase2);
    mEnterpriseFields->Set(2, mIdentity);
    mEnterpriseFields->Set(3, mAnonymousIdentity);
    mEnterpriseFields->Set(4, mPassword);
    mEnterpriseFields->Set(5, mClientCert);
    mEnterpriseFields->Set(6, mEngine);
    mEnterpriseFields->Set(7, mEngineId);
    mEnterpriseFields->Set(8, mKeyId);
    mEnterpriseFields->Set(9, mCaCert);
}

ECode CWifiConfiguration::constructor()
{
    mNetworkId = IWifiConfiguration::INVALID_NETWORK_ID;
    mSSID = NULL;
    mBSSID = NULL;
    mPriority = 0;
    mHiddenSSID = FALSE;
    mDisableReason = IWifiConfiguration::DISABLED_UNKNOWN_REASON;
    CBitSet::New((IBitSet**)&mAllowedKeyManagement);
    CBitSet::New((IBitSet**)&mAllowedProtocols);
    CBitSet::New((IBitSet**)&mAllowedAuthAlgorithms);
    CBitSet::New((IBitSet**)&mAllowedPairwiseCiphers);
    CBitSet::New((IBitSet**)&mAllowedGroupCiphers);
    mWepKeys = ArrayOf<String>::Alloc(4);
    for (Int32 i = 0; i < mWepKeys->GetLength(); i++) {
        (*mWepKeys)[i] = NULL;
    }
    for (Int32 j = 0; j < mEnterpriseFields->GetLength(); ++j) {
        (*mEnterpriseFields)[j]->SetValue(String(NULL));
    }

    mIpAssignment = IpAssignment_UNASSIGNED;
    mProxySettings = ProxySettings_UNASSIGNED;
    return CLinkProperties::New((ILinkProperties**)&mLinkProperties);
}

ECode CWifiConfiguration::constructor(
    /* [in] */ IWifiConfiguration* source)
{
    if (source != NULL) {
        source->GetNetworkId(&mNetworkId);
        source->GetStatus(&mStatus);
        source->GetDisableReason(&mDisableReason);
        source->GetSSID(&mSSID);
        source->GetBSSID(&mBSSID);
        source->GetPreSharedKey(&mPreSharedKey);

        AutoPtr<ArrayOf<String> > wepKeys;
        source->GetWepKeys((ArrayOf<String>**)&wepKeys);
        mWepKeys = wepKeys->Clone();

        source->GetWepTxKeyIndex(&mWepTxKeyIndex);
        source->GetPriority(&mPriority);
        source->GetHiddenSSID(&mHiddenSSID);

        source->GetAllowedKeyManagement((IBitSet**)&mAllowedKeyManagement);
        source->GetAllowedProtocols((IBitSet**)&mAllowedProtocols);
        source->GetAllowedAuthAlgorithms((IBitSet**)&mAllowedAuthAlgorithms);
        source->GetAllowedPairwiseCiphers((IBitSet**)&mAllowedPairwiseCiphers);
        source->GetAllowedGroupCiphers((IBitSet**)&mAllowedGroupCiphers);

        AutoPtr<ArrayOf<IWifiConfigurationEnterpriseField*> > fields;
        source->GetEnterpriseFields((ArrayOf<IWifiConfigurationEnterpriseField*>**)&fields);
        for (Int32 i = 0; i < fields->GetLength(); i++) {
            String value;
            ((*fields)[i])->GetValue(&value);
            ((*mEnterpriseFields)[i])->SetValue(value);
        }

        source->GetIpAssignment(&mIpAssignment);
        source->GetProxySettings(&mProxySettings);

        AutoPtr<ILinkProperties> linkProperties;
        source->GetLinkProperties((ILinkProperties**)&linkProperties);

        CLinkProperties::New(linkProperties, (ILinkProperties**)&mLinkProperties);
    }

    return NOERROR;
}

ECode CWifiConfiguration::ToString(
    /* [out] */ String* pValue)
{
    StringBuilder sbuf;
    if (mStatus == IWifiConfigurationStatus::CURRENT) {
        sbuf.AppendCStr("* ");
    } else if (mStatus == IWifiConfigurationStatus::DISABLED) {
        sbuf.AppendCStr("- DSBLE: ");
        sbuf.AppendInt32(mDisableReason);
        sbuf.AppendCStr(" ");
    }
    sbuf.AppendCStr("ID: ");
    sbuf.AppendInt32(mNetworkId);
    sbuf.AppendCStr(" SSID: ");
    sbuf.AppendString(mSSID);
    sbuf.AppendCStr(" BSSID: ");
    sbuf.AppendString(mBSSID);
    sbuf.AppendCStr(" PRIO: ");
    sbuf.AppendInt32(mPriority);
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr(" KeyMgmt:");
    Int32 size;
    mAllowedKeyManagement->GetSize(&size);
    for (Int32 k = 0; k < size; k++) {
        Boolean value;
        if (mAllowedKeyManagement->Get(k, &value), value) {
            sbuf.AppendCStr(" ");
            if (k < CWifiConfigurationKeyMgmt::sStrings->GetLength()) {
                sbuf.AppendString((*CWifiConfigurationKeyMgmt::sStrings)[k]);
            } else {
                sbuf.AppendCStr("??");
            }
        }
    }
    sbuf.AppendCStr(" Protocols:");
    mAllowedProtocols->GetSize(&size);
    for (Int32 p = 0; p < size; p++) {
        Boolean value;
        if (mAllowedProtocols->Get(p, &value), value) {
            sbuf.AppendCStr(" ");
            if (p < CWifiConfigurationProtocol::sStrings->GetLength()) {
                sbuf.AppendString((*CWifiConfigurationProtocol::sStrings)[p]);
            } else {
                sbuf.AppendCStr("??");
            }
        }
    }
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr(" AuthAlgorithms:");
    mAllowedAuthAlgorithms->GetSize(&size);
    for (Int32 a = 0; a < size; a++) {
        Boolean value;
        if (mAllowedAuthAlgorithms->Get(a, &value), value) {
            sbuf.AppendCStr(" ");
            if (a < CWifiConfigurationAuthAlgorithm::sStrings->GetLength()) {
                sbuf.AppendString((*CWifiConfigurationAuthAlgorithm::sStrings)[a]);
            } else {
                sbuf.AppendCStr("??");
            }
        }
    }
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr(" PairwiseCiphers:");
    mAllowedPairwiseCiphers->GetSize(&size);
    for (Int32 pc = 0; pc < size; pc++) {
        Boolean value;
        if (mAllowedPairwiseCiphers->Get(pc, &value), value) {
            sbuf.AppendCStr(" ");
            if (pc < CWifiConfigurationPairwiseCipher::sStrings->GetLength()) {
                sbuf.AppendString((*CWifiConfigurationPairwiseCipher::sStrings)[pc]);
            } else {
                sbuf.AppendCStr("??");
            }
        }
    }
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr(" GroupCiphers:");
    mAllowedGroupCiphers->GetSize(&size);
    for (Int32 gc = 0; gc < size; gc++) {
        Boolean value;
        if (mAllowedGroupCiphers->Get(gc, &value), value) {
            sbuf.AppendCStr(" ");
            if (gc < CWifiConfigurationGroupCipher::sStrings->GetLength()) {
                sbuf.AppendString((*CWifiConfigurationGroupCipher::sStrings)[gc]);
            } else {
                sbuf.AppendCStr("??");
            }
        }
    }
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr(" PSK: ");
    if (mPreSharedKey != NULL) {
        sbuf.AppendCStr("*");
    }

    for (Int32 f = 0; f < mEnterpriseFields->GetLength(); f++) {
        AutoPtr<IWifiConfigurationEnterpriseField> field = (*mEnterpriseFields)[f];
        sbuf.AppendCStr("\n");
        String name;
        field->GetVarName(&name);
        sbuf.AppendString(String(" ") + name + ": ");
        String value;
        field->GetValue(&value);
        if (value != String(NULL)) sbuf.AppendString(value);
    }
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr("IP assignment: ");
    sbuf.AppendInt32(mIpAssignment);
    sbuf.AppendCStr("\n");
    sbuf.AppendCStr("Proxy settings: ");
    sbuf.AppendInt32(mProxySettings);
    sbuf.AppendCStr("\n");
    String s;
    mLinkProperties->ToString(&s);
    sbuf.AppendString(s);
    sbuf.AppendCStr("\n");

    *pValue = sbuf.ToString();
    return NOERROR;
}

ECode CWifiConfiguration::GetPrintableSsid(
    /* [out] */ String* ssid)
{
    VALIDATE_NOT_NULL(ssid);

    if (mSSID.IsNull()) {
        *ssid = "";
        return NOERROR;
    }

    AutoPtr<ArrayOf<Char32> > charArray = mSSID.GetChars();
    Int32 length = charArray->GetLength();
    if (length > 2 && ((*charArray)[0] == '"') && (*charArray)[length - 1] == '"') {
        *ssid = mSSID.Substring(1, length - 1);
        return NOERROR;
    }

    /** The ascii-encoded string format is P"<ascii-encoded-string>"
     * The decoding is implemented in the supplicant for a newly configured
     * network.
     */
    if (length > 3 && ((*charArray)[0] == 'P') && ((*charArray)[1] == '"') &&
            ((*charArray)[length - 1] == '"')) {
        AutoPtr<IWifiSsid> wifiSsid;
        CWifiSsid::CreateFromAsciiEncoded(mSSID.Substring(2, length - 1),
            (IWifiSsid**)&wifiSsid);
        return wifiSsid->ToString(ssid);
    }
    *ssid = mSSID;
    return NOERROR;
}

AutoPtr<IBitSet> CWifiConfiguration::ReadBitSet(
    /* [in] */ IParcel* src)
{
    Int32 cardinality;
    src->ReadInt32(&cardinality);

    AutoPtr<IBitSet> set;
    CBitSet::New((IBitSet**)&set);
    for (Int32 i = 0; i < cardinality; i++) {
        Int32 index;
        src->ReadInt32(&index);
        set->Set(index);
    }

    return set;
}

void CWifiConfiguration::WriteBitSet(
    /* [in] */ IParcel* dest,
    /* [in] */ IBitSet* set)
{
    Int32 nextSetBit = -1;

    Int32 number = 0;
    set->Cardinality(&number);
    dest->WriteInt32(number);

    while (set->NextSetBit(nextSetBit + 1, &nextSetBit), nextSetBit != -1) {
        dest->WriteInt32(nextSetBit);
    }
}

ECode CWifiConfiguration::GetAuthType(
    /* [out] */ Int32* authType)
{
    VALIDATE_NOT_NULL(authType);
    *authType = IWifiConfigurationKeyMgmt::NONE;

    Boolean temp;
    if (mAllowedKeyManagement->Get(IWifiConfigurationKeyMgmt::WPA_PSK, &temp), temp) {
        *authType = IWifiConfigurationKeyMgmt::WPA_PSK;
    }
    else if (mAllowedKeyManagement->Get(IWifiConfigurationKeyMgmt::WPA2_PSK, &temp), temp) {
        *authType = IWifiConfigurationKeyMgmt::WPA2_PSK;
    }
    else if (mAllowedKeyManagement->Get(IWifiConfigurationKeyMgmt::WPA_EAP, &temp), temp) {
        *authType = IWifiConfigurationKeyMgmt::WPA_EAP;
    }
    else if (mAllowedKeyManagement->Get(IWifiConfigurationKeyMgmt::IEEE8021X, &temp), temp) {
        *authType = IWifiConfigurationKeyMgmt::IEEE8021X;
    }

    return NOERROR;
}

ECode CWifiConfiguration::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mNetworkId);
    source->ReadInt32(&mStatus);
    source->ReadInt32(&mDisableReason);
    source->ReadString(&mSSID);
    source->ReadString(&mBSSID);
    source->ReadString(&mPreSharedKey);
    for (Int32 i = 0; i < mWepKeys->GetLength(); ++i) {
        String key;
        source->ReadString(&key);
        (*mWepKeys)[i] = key;
    }
    source->ReadInt32(&mWepTxKeyIndex);
    source->ReadInt32(&mPriority);
    source->ReadBoolean(&mHiddenSSID);

    mAllowedKeyManagement = ReadBitSet(source);
    mAllowedProtocols = ReadBitSet(source);
    mAllowedAuthAlgorithms = ReadBitSet(source);
    mAllowedPairwiseCiphers = ReadBitSet(source);
    mAllowedGroupCiphers = ReadBitSet(source);

    for (Int32 i = 0; i < mEnterpriseFields->GetLength(); ++i) {
        String value;
        source->ReadString(&value);
        ((*mEnterpriseFields)[i])->SetValue(value);
    }

    source->ReadInt32(&mIpAssignment);
    source->ReadInt32(&mProxySettings);

    return source->ReadInterfacePtr((Handle32*)(ILinkProperties**)&mLinkProperties);
}

ECode CWifiConfiguration::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mNetworkId);
    dest->WriteInt32(mStatus);
    dest->WriteInt32(mDisableReason);
    dest->WriteString(mSSID);
    dest->WriteString(mBSSID);
    dest->WriteString(mPreSharedKey);
    for (Int32 i = 0; i < mWepKeys->GetLength(); ++i) {
        dest->WriteString((*mWepKeys)[i]);
    }
    dest->WriteInt32(mWepTxKeyIndex);
    dest->WriteInt32(mPriority);
    dest->WriteBoolean(mHiddenSSID);

    WriteBitSet(dest, mAllowedKeyManagement);
    WriteBitSet(dest, mAllowedProtocols);
    WriteBitSet(dest, mAllowedAuthAlgorithms);
    WriteBitSet(dest, mAllowedPairwiseCiphers);
    WriteBitSet(dest, mAllowedGroupCiphers);

    for (Int32 i = 0; i < mEnterpriseFields->GetLength(); i++) {
        String value;
        (*mEnterpriseFields)[i]->GetValue(&value);
        dest->WriteString(value);
    }

    dest->WriteInt32(mIpAssignment);
    dest->WriteInt32(mProxySettings);
    return dest->WriteInterfacePtr(mLinkProperties);
}

ECode CWifiConfiguration::GetWepKeyVarName(
    /* [in] */ Int32 index,
    /* [out] */ String* wepKeyVarName)
{
    VALIDATE_NOT_NULL(wepKeyVarName);
    if (index < 0 || index > 3) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    if (index == 0) *wepKeyVarName = "wep_key0";
    else if (index == 1) *wepKeyVarName = "wep_key1";
    else if (index == 2) *wepKeyVarName = "wep_key2";
    else if (index == 3) *wepKeyVarName = "wep_key3";
    return NOERROR;
}

ECode CWifiConfiguration::GetEap(
    /* [out] */ IWifiConfigurationEnterpriseField** eap)
{
    VALIDATE_NOT_NULL(eap);
    *eap = mEap;
    REFCOUNT_ADD(*eap);
    return NOERROR;
}

ECode CWifiConfiguration::GetPhase2(
    /* [out] */ IWifiConfigurationEnterpriseField** phase2)
{
    VALIDATE_NOT_NULL(phase2);
    *phase2 = mPhase2;
    REFCOUNT_ADD(*phase2);
    return NOERROR;
}

ECode CWifiConfiguration::GetIdentity(
    /* [out] */ IWifiConfigurationEnterpriseField** identity)
{
    VALIDATE_NOT_NULL(identity);
    *identity = mIdentity;
    REFCOUNT_ADD(*identity);
    return NOERROR;
}

ECode CWifiConfiguration::GetAnonymousIdentity(
    /* [out] */ IWifiConfigurationEnterpriseField** anonymousIdentity)
{
    VALIDATE_NOT_NULL(anonymousIdentity);
    *anonymousIdentity = mAnonymousIdentity;
    REFCOUNT_ADD(*anonymousIdentity);
    return NOERROR;
}

ECode CWifiConfiguration::GetPassword(
    /* [out] */ IWifiConfigurationEnterpriseField** password)
{
    VALIDATE_NOT_NULL(password);
    *password = mPassword;
    REFCOUNT_ADD(*password);
    return NOERROR;
}

ECode CWifiConfiguration::GetClientCert(
    /* [out] */ IWifiConfigurationEnterpriseField** clientCert)
{
    VALIDATE_NOT_NULL(clientCert);
    *clientCert = mClientCert;
    REFCOUNT_ADD(*clientCert);
    return NOERROR;
}

ECode CWifiConfiguration::GetEngine(
    /* [out] */ IWifiConfigurationEnterpriseField** engine)
{
    VALIDATE_NOT_NULL(engine);
    *engine = mEngine;
    REFCOUNT_ADD(*engine);
    return NOERROR;
}

ECode CWifiConfiguration::GetEngineId(
    /* [out] */ IWifiConfigurationEnterpriseField** engineId)
{
    VALIDATE_NOT_NULL(engineId);
    *engineId = mEngineId;
    REFCOUNT_ADD(*engineId);
    return NOERROR;
}

ECode CWifiConfiguration::GetKeyId(
    /* [out] */ IWifiConfigurationEnterpriseField** keyId)
{
    VALIDATE_NOT_NULL(keyId);
    *keyId = mKeyId;
    REFCOUNT_ADD(*keyId);
    return NOERROR;
}

ECode CWifiConfiguration::GetCaCert(
    /* [out] */ IWifiConfigurationEnterpriseField** caCert)
{
    VALIDATE_NOT_NULL(caCert);
    *caCert = mCaCert;
    REFCOUNT_ADD(*caCert);
    return NOERROR;
}

ECode CWifiConfiguration::GetEnterpriseFields(
    /* [out, callee] */ ArrayOf<IWifiConfigurationEnterpriseField*>** fields)
{
    VALIDATE_NOT_NULL(fields);
    *fields = mEnterpriseFields;
    REFCOUNT_ADD(*fields);
    return NOERROR;
}

ECode CWifiConfiguration::GetNetworkId(
    /* [out] */ Int32* networkId)
{
    VALIDATE_NOT_NULL(networkId);
    *networkId = mNetworkId;
    return NOERROR;
}

ECode CWifiConfiguration::SetNetworkId(
    /* [in] */ Int32 networkId)
{
    mNetworkId = networkId;
    return NOERROR;
}

ECode CWifiConfiguration::GetStatus(
    /* [out] */ Int32* status)
{
    VALIDATE_NOT_NULL(status);
    *status = mStatus;
    return NOERROR;
}

ECode CWifiConfiguration::SetStatus(
    /* [in] */ Int32 status)
{
    mStatus = status;
    return NOERROR;
}

ECode CWifiConfiguration::GetDisableReason(
    /* [out] */ Int32* disableReason)
{
    VALIDATE_NOT_NULL(disableReason);
    *disableReason = mDisableReason;
    return NOERROR;
}

ECode CWifiConfiguration::SetDisableReason(
    /* [in] */ Int32 disableReason)
{
    mDisableReason = disableReason;
    return NOERROR;
}

ECode CWifiConfiguration::GetSSID(
    /* [out] */ String* SSID)
{
    VALIDATE_NOT_NULL(SSID);
    *SSID = mSSID;
    return NOERROR;
}

ECode CWifiConfiguration::SetSSID(
    /* [in] */ const String& SSID)
{
    mSSID = SSID;
    return NOERROR;
}

ECode CWifiConfiguration::GetBSSID(
    /* [out] */ String* BSSID)
{
    VALIDATE_NOT_NULL(BSSID);
    *BSSID = mBSSID;
    return NOERROR;
}

ECode CWifiConfiguration::SetBSSID(
    /* [in] */ const String& BSSID)
{
    mBSSID = BSSID;
    return NOERROR;
}

ECode CWifiConfiguration::GetPreSharedKey(
    /* [out] */ String* preSharedKey)
{
    VALIDATE_NOT_NULL(preSharedKey);
    *preSharedKey = mPreSharedKey;
    return NOERROR;
}

ECode CWifiConfiguration::SetPreSharedKey(
    /* [in] */ const String& preSharedKey)
{
    mPreSharedKey = preSharedKey;
    return NOERROR;
}

ECode CWifiConfiguration::GetWepKeys(
    /* [out, callee] */ ArrayOf<String>** wepKeys)
{
    VALIDATE_NOT_NULL(wepKeys);
    *wepKeys = mWepKeys;
    REFCOUNT_ADD(*wepKeys);
    return NOERROR;
}

ECode CWifiConfiguration::SetWepKeys(
    /* [in] */ ArrayOf<String>* wepKeys)
{
    mWepKeys = wepKeys;
    return NOERROR;
}

ECode CWifiConfiguration::GetWepTxKeyIndex(
    /* [out] */ Int32* wepTxKeyIndex)
{
    VALIDATE_NOT_NULL(wepTxKeyIndex);
    *wepTxKeyIndex = mWepTxKeyIndex;
    return NOERROR;
}

ECode CWifiConfiguration::SetWepTxKeyIndex(
    /* [in] */ Int32 wepTxKeyIndex)
{
    mWepTxKeyIndex = wepTxKeyIndex;
    return NOERROR;
}

ECode CWifiConfiguration::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

ECode CWifiConfiguration::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

ECode CWifiConfiguration::GetHiddenSSID(
    /* [out] */ Boolean* hiddenSSID)
{
    VALIDATE_NOT_NULL(hiddenSSID);
    *hiddenSSID = mHiddenSSID;
    return NOERROR;
}

ECode CWifiConfiguration::SetHiddenSSID(
    /* [in] */ Boolean hiddenSSID)
{
    mHiddenSSID = hiddenSSID;
    return NOERROR;
}

ECode CWifiConfiguration::GetAllowedKeyManagement(
    /* [out] */ IBitSet** allowedKeyManagement)
{
    VALIDATE_NOT_NULL(allowedKeyManagement);
    *allowedKeyManagement = mAllowedKeyManagement;
    REFCOUNT_ADD(*allowedKeyManagement);
    return NOERROR;
}

ECode CWifiConfiguration::SetAllowedKeyManagement(
    /* [in] */ IBitSet* allowedKeyManagement)
{
    mAllowedKeyManagement = allowedKeyManagement;
    return NOERROR;
}

ECode CWifiConfiguration::GetAllowedProtocols(
    /* [out] */ IBitSet** allowedProtocols)
{
    VALIDATE_NOT_NULL(allowedProtocols);
    *allowedProtocols = mAllowedProtocols;
    REFCOUNT_ADD(*allowedProtocols);
    return NOERROR;
}

ECode CWifiConfiguration::SetAllowedProtocols(
    /* [in] */ IBitSet* allowedProtocols)
{
    mAllowedProtocols = allowedProtocols;
    return NOERROR;
}

ECode CWifiConfiguration::GetAllowedAuthAlgorithms(
    /* [out] */ IBitSet** allowedAuthAlgorithms)
{
    VALIDATE_NOT_NULL(allowedAuthAlgorithms);
    *allowedAuthAlgorithms = mAllowedAuthAlgorithms;
    REFCOUNT_ADD(*allowedAuthAlgorithms);
    return NOERROR;
}

ECode CWifiConfiguration::SetAllowedAuthAlgorithms(
    /* [in] */ IBitSet* allowedAuthAlgorithms)
{
    mAllowedAuthAlgorithms = allowedAuthAlgorithms;
    return NOERROR;
}

ECode CWifiConfiguration::GetAllowedPairwiseCiphers(
    /* [out] */ IBitSet** allowedPairwiseCiphers)
{
    VALIDATE_NOT_NULL(allowedPairwiseCiphers);
    *allowedPairwiseCiphers = mAllowedPairwiseCiphers;
    REFCOUNT_ADD(*allowedPairwiseCiphers);
    return NOERROR;
}

ECode CWifiConfiguration::SetAllowedPairwiseCiphers(
    /* [in] */ IBitSet* allowedPairwiseCiphers)
{
    mAllowedPairwiseCiphers = allowedPairwiseCiphers;
    return NOERROR;
}

ECode CWifiConfiguration::GetAllowedGroupCiphers(
    /* [out] */ IBitSet** allowedGroupCiphers)
{
    VALIDATE_NOT_NULL(allowedGroupCiphers);
    *allowedGroupCiphers = mAllowedGroupCiphers;
    REFCOUNT_ADD(*allowedGroupCiphers);
    return NOERROR;
}

ECode CWifiConfiguration::SetAllowedGroupCiphers(
    /* [in] */ IBitSet* allowedGroupCiphers)
{
    mAllowedGroupCiphers = allowedGroupCiphers;
    return NOERROR;
}

ECode CWifiConfiguration::GetIpAssignment(
    /* [out] */ IpAssignment* ipAssignment)
{
    VALIDATE_NOT_NULL(ipAssignment);
    *ipAssignment = mIpAssignment;
    return NOERROR;
}

ECode CWifiConfiguration::SetIpAssignment(
    /* [in] */ IpAssignment ipAssignment)
{
    mIpAssignment = ipAssignment;
    return NOERROR;
}

ECode CWifiConfiguration::GetProxySettings(
    /* [out] */ ProxySettings* proxySettings)
{
    VALIDATE_NOT_NULL(proxySettings);
    *proxySettings = mProxySettings;
    return NOERROR;
}

ECode CWifiConfiguration::SetProxySettings(
    /* [in] */ ProxySettings proxySettings)
{
    mProxySettings = proxySettings;
    return NOERROR;
}

ECode CWifiConfiguration::GetLinkProperties(
    /* [out] */ ILinkProperties** linkProperties)
{
    VALIDATE_NOT_NULL(linkProperties);
    *linkProperties = mLinkProperties;
    REFCOUNT_ADD(*linkProperties);
    return NOERROR;
}

ECode CWifiConfiguration::SetLinkProperties(
    /* [in] */ ILinkProperties* linkProperties)
{
    mLinkProperties = linkProperties;
    return NOERROR;
}

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos
