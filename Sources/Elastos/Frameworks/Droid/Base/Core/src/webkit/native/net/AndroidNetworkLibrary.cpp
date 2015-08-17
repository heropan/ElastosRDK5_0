
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
//                  AndroidNetworkLibrary
//===============================================================
const String AndroidNetworkLibrary::TAG("AndroidNetworkLibrary");

Boolean AndroidNetworkLibrary::StoreKeyPair(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<Byte>* publicKey,
    /* [in] */ ArrayOf<Byte>* privateKey)
{
    //try {
        AutoPtr<IIntent> intent;
        //from qiuwenrong init "intent"
        //AutoPtr<IKeyChainHelper> helper;
        //CKeyChainHelper::AcquireSingleton((IKeyChainHelper**)&helper);
        //hepler->CreateInstallIntent((IIntent**)&intent);

        //self init "intent"
        CIntent::New((IIntent**)&intent);
        intent->PutExtra(String("PKEY"), privateKey);
        intent->PutExtra(String("KEY"), publicKey);
        intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
        context->StartActivity(intent);
        return TRUE;
    //} catch (ActivityNotFoundException e) {
    //    Log.w(TAG, "could not store key pair: " + e);
    //}
    //return FALSE;
}

Boolean AndroidNetworkLibrary::StoreCertificate(
    /* [in] */ IContext* context,
    /* [in] */ Int32 certType,
    /* [in] */ ArrayOf<Byte>* data)
{
    //try {
        AutoPtr<IIntent> intent;
        CIntent::New((IIntent**)&intent);
        intent->AddFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);

        switch (certType) {
            case ICertificateMimeType::X509_USER_CERT:
            case ICertificateMimeType::X509_CA_CERT:
                intent->PutExtra(IKeyChain::EXTRA_CERTIFICATE, data);
                break;

            case ICertificateMimeType::PKCS12_ARCHIVE:
                intent->PutExtra(IKeyChain::EXTRA_PKCS12, data);
                break;

            default:
                //Log.w(TAG, "invalid certificate type: " + certType);
                return FALSE;
        }
        context->StartActivity(intent);
        return TRUE;
    //} catch (ActivityNotFoundException e) {
    //    Log.w(TAG, "could not store crypto file: " + e);
    //}
    //return FALSE;
}

String AndroidNetworkLibrary::GetMimeTypeFromExtension(
    /* [in] */ String extension)
{
    return URLConnection::GuessContentTypeFromName(String("foo.") + extension);
}

Boolean AndroidNetworkLibrary::HaveOnlyLoopbackAddresses()
{
    Enumeration< AutoPtr<INetworkInterface> > list;
    //try {
        AutoPtr<INetworkInterfaceHelper> helper;
        CNetworkInterfaceHelper::AcquireSingleton((INetworkInterfaceHelper**)&helper);
        helper->GetNetworkInterfaces(&list);
        if (list.IsEmpty())
            return FALSE;
    //} catch (Exception e) {
    //    Log.w(TAG, "could not get network interfaces: " + e);
    //    return false;
    //}

    while (list.HasMoreElements()) {
        AutoPtr<INetworkInterface> netIf = list.NextElement();
        //try {
            if (netIf->IsUp() && !netIf->IsLoopback())
                return FALSE;
        //} catch (SocketException e) {
        //    continue;
        //}
    }
    return TRUE;
}

String AndroidNetworkLibrary::GetNetworkList()
{
    // question: how to initlize Enumeration and how to free, using AutoPtr?
    Enumeration< AutoPtr<INetworkInterface> > lists;
    //try {
        AutoPtr<INetworkInterfaceHelper> helper;
        CNetworkInterfaceHelper::AcquireSingleton((INetworkInterfaceHelper**)&helper);
        helper->GetNetworkInterfaces(&list);
        if (list.IsEmpty())
            return String("");
    //} catch (SocketException e) {
    //    Log.w(TAG, "Unable to get network interfaces: " + e);
    //    return "";
    //}

    StringBuilder result;
    while (list.HasMoreElements()) {
        AutoPtr<INetworkInterface> netIf = list.NextElement();
        //try {
            // Skip loopback interfaces, and ones which are down.
            if (!netIf->IsUp() || netIf->IsLoopback())
                continue;

            List< AutoPtr<IInterfaceAddress> > interfaceAddrs;
            netIf->GetInterfaceAddresses(&interfaceAddrs);
            for (Int32 i=0; i<interfaceAddrs.GetLength(); ++i) {
                   AutoPtr<IInterfaceAddress> interfaceAddress = interfaceAddrs[i];
                AutoPtr<IInetAddress> address;
                CInetAddress::New((IInetAddress**)&address);
                interfaceAddress->GetAddress(&address);

                // Skip loopback addresses configured on non-loopback interfaces.
                if (address->IsLoopbackAddress())
                    continue;

                StringBuilder addressString;
                String netName;
                netIf->GetName(&netName);
                addressString.Append(netName);
                addressString.Append(String("\t"));

                String ipAddress;
                address->GetHostAddress(&ipAddress);
                AutoPtr<IInet6Address> inet6Addr = (IInet6Address*)address->Probe(EIID_IInet6Address);
                if (inet6Addr != NULL && ipAddress.Contains(String("%"))) {
                    ipAddress = ipAddress.Substring(0, ipAddress.LastIndexOf(String("%")));
                }
                addressString.Append(ipAddress);
                addressString.Append(String("/"));

                Int16 prefixLength = 0;
                interfaceAddress->GetNetworkPrefixLength(&prefixLength);

                // question: Int16 to String
                addressString.Append(prefixLength.ToString());
                addressString.Append(String("\t"));

                // TODO(vitalybuka): use netIf.getIndex() when API level 19 is availible.
                addressString->Append(String("0"));

                if (result.GetLength() != 0)
                    result.Append(String("\n"));
                result.Append(addressString->ToString());
            }
        //} catch (SocketException e) {
        //    continue;
        //}
    }
    return result.ToString();
}

AutoPtr<AndroidCertVerifyResult> AndroidNetworkLibrary::VerifyServerCertificates(
    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* certChain,
    /* [in] */ String authType,
    /* [in] */ String host)
{
    //try {
        return X509Util::VerifyServerCertificates(certChain, authType, host);
    //} catch (KeyStoreException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    //} catch (NoSuchAlgorithmException e) {
    //    return new AndroidCertVerifyResult(CertVerifyStatusAndroid.VERIFY_FAILED);
    //}
}

ECode AndroidNetworkLibrary::AddTestRootCertificate(
    /* [in] */ ArrayOf<Byte>* rootCert)
{
    VALIDATE_NOT_NULL(rootCert);
    X509Util::AddTestRootCertificate(rootCert);
    return NOERROR;
}

ECode AndroidNetworkLibrary::ClearTestRootCertificates()
{
    X509Util::ClearTestRootCertificates();
    return NOERROR;
}

String AndroidNetworkLibrary::GetNetworkCountryIso(
    /* [in] */ IContext* context)
{
    AutoPtr<ITelephonyManager> telephonyManager;
    CTelephonyManager::New((ITelephonyManager**)&telephonyManager);
    context->GetSystemService(IContext::TELEPHONY_SERVICE, (ITelephonyManager**)&telephonyManager);
    String result("");
    if (telephonyManager != NULL) {
        telephonyManager->GetNetworkCountryIso(&result);
        return result;
    }
    return result;
}


String AndroidNetworkLibrary::GetNetworkOperator(
    /* [in] */ IContext* context)
{
    AutoPtr<ITelephonyManager> telephonyManager;
    CTelephonyManager::New((ITelephonyManager**)&telephonyManager);
    context->GetSystemService(IContext::TELEPHONY_SERVICE, (ITelephonyManager**)&telephonyManager);
    String result("");
    if (telephonyManager != NULL) {
        telephonyManager->GetNetworkOperator(&result);
        return result;
    }
    return result;
}

} // namespace  Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
