
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//                 ClientCertLookupTable::Cert
//===============================================================

/**
 * A container for the certificate data.
 */
public static class Cert {
    AndroidPrivateKey privateKey;
    byte[][] certChain;
ClientCertLookupTable::Cert::Cert(
    /* [in] */ AndroidPrivateKey* privateKey,
    /* [in] */ ArrayOf< ArrayOf<Byte> >* certChain)
{
    this->privateKey = privateKey;
    Int32 length = newChain->GetLenght();
    AutoPtr< ArrayOf< ArrayOf<Byte> > > newChain = ArrayOf< ArrayOf<Byte> >::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        newChain[i] = Arrays.copyOf(certChain[i], certChain[i].length);
    }
    this->certChain = newChain;
}
};

//===============================================================
//                     ClientCertLookupTable
//===============================================================

// Clear client certificate preferences
void ClientCertLookupTable::Clear()
{
    mCerts.clear();
    mDenieds.clear();
}

ClientCertLookupTable::ClientCertLookupTable()
{
    mCerts = new HashMap<String, Cert>();
    mDenieds = new HashSet<String>();
}

void ClientCertLookupTable::Allow(
    /* [in] */ const String& host,
    /* [in] */ Int32 port,
    /* [in] */ AndroidPrivateKey* privateKey,
    /* [in] */ ArrayOf< ArrayOf<Byte> >* chain)
{
    String host_and_port = HostAndPort(host, port);
    mCerts.put(host_and_port, new Cert(privateKey, chain));
    mDenieds.remove(host_and_port);
}

void ClientCertLookupTable::Deny(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    String host_and_port = HostAndPort(host, port);
    mCerts.remove(host_and_port);
    mDenieds.add(host_and_port);
}

AutoPtr<Cert> ClientCertLookupTable::GetCertData(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return mCerts->Get(HostAndPort(host, port));
}

Boolean ClientCertLookupTable::IsDenied(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    return mDenieds.contains(HostAndPort(host, port));
}

// TODO(sgurun) add a test for this. Not separating host and pair properly will be
// a security issue.
String ClientCertLookupTable::HostAndPort(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    String retStr(host);
    retStr += ":";
    retStr += port;
    return retStr;
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
