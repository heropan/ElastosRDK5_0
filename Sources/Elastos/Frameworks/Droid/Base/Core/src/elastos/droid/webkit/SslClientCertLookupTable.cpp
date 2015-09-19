
#include "webkit/SslClientCertLookupTable.h"

using Elastos::Utility::CObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Webkit {

AutoPtr<SslClientCertLookupTable> SslClientCertLookupTable::sTable;
Mutex SslClientCertLookupTable::sLock;

SslClientCertLookupTable::SslClientCertLookupTable()
{
    CObjectStringMap::New((IObjectStringMap**)&privateKeys);
    CObjectStringMap::New((IObjectStringMap**)&certificateChains);
}

//synchronized
AutoPtr<SslClientCertLookupTable> SslClientCertLookupTable::GetInstance()
{
    AutoLock lock(sLock);
    if (sTable == NULL) {
        sTable = new SslClientCertLookupTable();
    }
    return sTable;
}

//void SslClientCertLookupTable::Allow(
//    /* [in] */ const String& host_and_port,
//    /* [in] */ IPrivateKey privateKey*,
//    /* [in] */ ArrayOf< AutoPtr< ArrayOf<Byte> > >* chain)
//{}

void SslClientCertLookupTable::Deny(
    /* [in] */ const String& host_and_port)
{
    privateKeys->Remove(host_and_port);
    certificateChains->Remove(host_and_port);
    denied.Insert(host_and_port);
}

Boolean SslClientCertLookupTable::IsAllowed(
    /* [in] */ const String& host_and_port)
{
    Boolean bFlag = FALSE;
    privateKeys->ContainsKey(host_and_port, &bFlag);
    return bFlag;
}

Boolean SslClientCertLookupTable::IsDenied(
    /* [in] */ const String& host_and_port)
{
    Set<String>::Iterator iter = denied.Find(host_and_port);
    return iter != denied.End();
}

//AutoPtr<IPrivateKey> SslClientCertLookupTable::PrivateKey(
//    /* [in] */ const String& host_and_port)
//{}

AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > > SslClientCertLookupTable::CertificateChain(
    /* [in] */ const String& host_and_port)
{
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<Byte> > > > array;
    certificateChains->Get(host_and_port, (IInterface**)&array);
    return array;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos