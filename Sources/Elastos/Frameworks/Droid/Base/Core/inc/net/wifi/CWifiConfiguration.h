
#ifndef __CWIFICONFIGURATION_H__
#define __CWIFICONFIGURATION_H__

#include "ext/frameworkdef.h"
#include "_CWifiConfiguration.h"

using Elastos::Utility::IBitSet;
using Elastos::Droid::Net::ILinkProperties;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfiguration)
{
public:
    /** {@hide} */
    class EnterpriseField
        : public ElRefBase
        , public IWifiConfigurationEnterpriseField
    {
        friend class CWifiConfiguration;

    public:
        CAR_INTERFACE_DECL();

        CARAPI SetValue(
            /* [in] */ const String& value);

        CARAPI GetVarName(
            /* [out] */ String* varName);

        CARAPI GetValue(
            /* [out] */ String* value);

    private:
        EnterpriseField(
            /* [in] */ const String& varName)
            : mVarName(varName)
        {}

    private:
        String mVarName;
        String mValue;
    };

public:
    CWifiConfiguration();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IWifiConfiguration* source);

    static CARAPI GetWepKeyVarName(
        /* [in] */ Int32 index,
        /* [out] */ String* wepKeyVarName);

    CARAPI GetEap(
        /* [out] */ IWifiConfigurationEnterpriseField** eap);

    CARAPI GetPhase2(
        /* [out] */ IWifiConfigurationEnterpriseField** phase2);

    CARAPI GetIdentity(
        /* [out] */ IWifiConfigurationEnterpriseField** identity);

    CARAPI GetAnonymousIdentity(
        /* [out] */ IWifiConfigurationEnterpriseField** anonymousIdentity);

    CARAPI GetPassword(
        /* [out] */ IWifiConfigurationEnterpriseField** password);

    CARAPI GetClientCert(
        /* [out] */ IWifiConfigurationEnterpriseField** clientCert);

    CARAPI GetEngine(
        /* [out] */ IWifiConfigurationEnterpriseField** engine);

    CARAPI GetEngineId(
        /* [out] */ IWifiConfigurationEnterpriseField** engineId);

    CARAPI GetKeyId(
        /* [out] */ IWifiConfigurationEnterpriseField** keyId);

    CARAPI GetCaCert(
        /* [out] */ IWifiConfigurationEnterpriseField** caCert);

    CARAPI GetEnterpriseFields(
        /* [out, callee] */ ArrayOf<IWifiConfigurationEnterpriseField*>** fields);

    CARAPI GetNetworkId(
        /* [out] */ Int32* networkId);

    CARAPI SetNetworkId(
        /* [in] */ Int32 networkId);

    CARAPI GetStatus(
        /* [out] */ Int32* status);

    CARAPI SetStatus(
        /* [in] */ Int32 status);

    CARAPI GetDisableReason(
        /* [out] */ Int32* disableReason);

    CARAPI SetDisableReason(
        /* [in] */ Int32 disableReason);

    CARAPI GetSSID(
        /* [out] */ String* SSID);

    CARAPI SetSSID(
        /* [in] */ const String& SSID);

    CARAPI GetBSSID(
        /* [out] */ String* BSSID);

    CARAPI SetBSSID(
        /* [in] */ const String& BSSID);

    CARAPI GetPreSharedKey(
        /* [out] */ String* preSharedKey);

    CARAPI SetPreSharedKey(
        /* [in] */ const String& preSharedKey);

    CARAPI GetWepKeys(
        /* [out, callee] */ ArrayOf<String>** wepKeys);

    CARAPI SetWepKeys(
        /* [in] */ ArrayOf<String>* wepKeys);

    CARAPI GetWepTxKeyIndex(
        /* [out] */ Int32* wepTxKeyIndex);

    CARAPI SetWepTxKeyIndex(
        /* [in] */ Int32 wepTxKeyIndex);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI GetHiddenSSID(
        /* [out] */ Boolean* hiddenSSID);

    CARAPI SetHiddenSSID(
        /* [in] */ Boolean hiddenSSID);

    CARAPI GetAllowedKeyManagement(
        /* [out] */ IBitSet** allowedKeyManagement);

    CARAPI SetAllowedKeyManagement(
        /* [in] */ IBitSet* allowedKeyManagement);

    CARAPI GetAllowedProtocols(
        /* [out] */ IBitSet** allowedProtocols);

    CARAPI SetAllowedProtocols(
        /* [in] */ IBitSet* allowedProtocols);

    CARAPI GetAllowedAuthAlgorithms(
        /* [out] */ IBitSet** allowedAuthAlgorithms);

    CARAPI SetAllowedAuthAlgorithms(
        /* [in] */ IBitSet* allowedAuthAlgorithms);

    CARAPI GetAllowedPairwiseCiphers(
        /* [out] */ IBitSet** allowedPairwiseCiphers);

    CARAPI SetAllowedPairwiseCiphers(
        /* [in] */ IBitSet* allowedPairwiseCiphers);

    CARAPI GetAllowedGroupCiphers(
        /* [out] */ IBitSet** allowedGroupCiphers);

    CARAPI SetAllowedGroupCiphers(
        /* [in] */ IBitSet* allowedGroupCiphers);

    CARAPI GetIpAssignment(
        /* [out] */ IpAssignment* ipAssignment);

    CARAPI SetIpAssignment(
        /* [in] */ IpAssignment ipAssignment);

    CARAPI GetProxySettings(
        /* [out] */ ProxySettings* proxySettings);

    CARAPI SetProxySettings(
        /* [in] */ ProxySettings proxySettings);

    CARAPI GetLinkProperties(
        /* [out] */ ILinkProperties** linkProperties);

    CARAPI SetLinkProperties(
        /* [in] */ ILinkProperties* linkProperties);

    CARAPI ToString(
        /* [out] */ String* value);

    CARAPI GetPrintableSsid(
        /* [out] */ String* printableSsid);

    CARAPI GetAuthType(
        /* [out] */ Int32* authType);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    static CARAPI_(AutoPtr<IBitSet>) ReadBitSet(
        /* [in] */ IParcel* src);

    static CARAPI_(void) WriteBitSet(
        /* [in] */ IParcel* dest,
        /* [in] */ IBitSet* set);

public:
    AutoPtr<EnterpriseField> mEap;
    AutoPtr<EnterpriseField> mPhase2;
    AutoPtr<EnterpriseField> mIdentity;
    AutoPtr<EnterpriseField> mAnonymousIdentity;
    AutoPtr<EnterpriseField> mPassword;
    AutoPtr<EnterpriseField> mClientCert;
    AutoPtr<EnterpriseField> mEngine;
    AutoPtr<EnterpriseField> mEngineId;
    AutoPtr<EnterpriseField> mKeyId;
    AutoPtr<EnterpriseField> mCaCert;
    AutoPtr< ArrayOf<IWifiConfigurationEnterpriseField*> > mEnterpriseFields;

    Int32 mNetworkId;
    Int32 mStatus;
    Int32 mDisableReason;
    String mSSID;
    String mBSSID;
    String mPreSharedKey;
    AutoPtr< ArrayOf<String> > mWepKeys;
    Int32 mWepTxKeyIndex;
    Int32 mPriority;
    Boolean mHiddenSSID;

    AutoPtr<IBitSet> mAllowedKeyManagement;
    AutoPtr<IBitSet> mAllowedProtocols;
    AutoPtr<IBitSet> mAllowedAuthAlgorithms;
    AutoPtr<IBitSet> mAllowedPairwiseCiphers;
    AutoPtr<IBitSet> mAllowedGroupCiphers;

    IpAssignment mIpAssignment;
    ProxySettings mProxySettings;
    AutoPtr<ILinkProperties> mLinkProperties;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __CWIFICONFIGURATION_H__
