
#include "AbstractVerifier.h"
#include <elastos/Logger.h>
#include <elastos/utility/Arrays.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Utility::Arrays;
using Elastos::Utility::ICollection;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::IList;
using Elastos::Utility::ILinkedList;
using Elastos::Utility::CLinkedList;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace SSL {

static AutoPtr< ArrayOf<String> > InitBadCountry2lds()
{
    AutoPtr< ArrayOf<String> > stringArray = ArrayOf<String>::Alloc(14);
    (*stringArray)[0]("ac");
    (*stringArray)[1]("co");
    (*stringArray)[2]("com");
    (*stringArray)[3]("ed");
    (*stringArray)[4]("edu");
    (*stringArray)[5]("go");
    (*stringArray)[6]("gouv");
    (*stringArray)[7]("gov");
    (*stringArray)[8]("info");
    (*stringArray)[9]("lg");
    (*stringArray)[10]("ne");
    (*stringArray)[11]("net");
    (*stringArray)[12]("or");
    (*stringArray)[13]("org");
    Arrays::Sort(stringArray);
    return stringArray;

}
const AutoPtr< ArrayOf<String> > AbstractVerifier::BAD_COUNTRY_2LDS = InitBadCountry2lds();

CAR_INTERFACE_IMPL_3(AbstractVerifier, Object, IAbstractVerifier, IX509HostnameVerifier, IHostnameVerifier)

ECode AbstractVerifier::Verify(
    /* [in] */ const String& host,
    /* [in] */ ISSLSocket* ssl)
{
    if(host == NULL) {
        Logger::E("AbstractVerifier", "host to verify is null");
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<ISSLSession> session;
    ssl->GetSession((ISSLSession**)&session);
    AutoPtr< ArrayOf<ICertificate*> > certs;
    session->GetPeerCertificates((ArrayOf<ICertificate*>**)&certs);
    AutoPtr<IX509Certificate> x509 = IX509Certificate::Probe((*certs)[0]);
    return Verify(host, x509);
}

ECode AbstractVerifier::Verify(
    /* [in] */ const String& host,
    /* [in] */ ISSLSession* session,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    // try {
    AutoPtr< ArrayOf<ICertificate*> > certs;
    session->GetPeerCertificates((ArrayOf<ICertificate*>**)&certs);
    AutoPtr<IX509Certificate> x509 = IX509Certificate::Probe((*certs)[0]);
    FAIL_RETURN(Verify(host, x509))
    *result = TRUE;
    return NOERROR;
    // }
    // catch(SSLException e) {
    //     return false;
    // }
}

ECode AbstractVerifier::Verify(
    /* [in] */ const String& host,
    /* [in] */ IX509Certificate* cert)
{
    AutoPtr< ArrayOf<String> > cns = GetCNs(cert);
    AutoPtr< ArrayOf<String> > subjectAlts = GetDNSSubjectAlts(cert);
    return Verify(host, cns, subjectAlts);
}

ECode AbstractVerifier::Verify(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<String>* cns,
    /* [in] */ ArrayOf<String>* subjectAlts,
    /* [in] */ Boolean strictWithSubDomains)
{
    // Build the list of names we're going to check.  Our DEFAULT and
    // STRICT implementations of the HostnameVerifier only use the
    // first CN provided.  All other CNs are ignored.
    // (Firefox, wget, curl, Sun Java 1.4, 5, 6 all work this way).
    AutoPtr<ILinkedList> names;
    CLinkedList::New((ILinkedList**)&names);
    AutoPtr<ICollection> col = ICollection::Probe(names);
    if(cns != NULL && cns->GetLength() > 0 && !(*cns)[0].IsNull()) {
        AutoPtr<ICharSequence> cs;
        CString::New((*cns)[0], (ICharSequence**)&cs);
        col->Add(cs);
    }
    if(subjectAlts != NULL) {
        for (Int32 i = 0; i < subjectAlts->GetLength(); ++i) {
            String subjectAlt = (*subjectAlts)[i];
            if (!subjectAlt.IsNull()) {
                AutoPtr<ICharSequence> cs;
                CString::New(subjectAlt, (ICharSequence**)&cs);
                col->Add(cs);
            }
        }
    }

    Boolean isEmpty;
    if(names->IsEmpty(&isEmpty), isEmpty) {
        Logger::E("AbstractVerifier", "Certificate for <%s> doesn't contain CN or DNS subjectAlt", host.string());
        return E_SSL_EXCEPTION;
    }

    // StringBuffer for building the error message.
    StringBuilder buf;

    // We're can be case-insensitive when comparing the host we used to
    // establish the socket to the hostname in the certificate.
    String hostName = host.Trim().ToLowerCase(ILocale::ENGLISH);
    Boolean match = FALSE;
    AutoPtr<IIterable> iterable = IIterable::Probe(names);
    AutoPtr<IIterator> it;
    iterable->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<ICharSequence> cs;
        it->GetNext((IInterface**)&cs);
        String cn;
        cs->ToString(&cn);
        cn = cn.ToLowerCase(ILocale::ENGLISH);
        // Store CN in StringBuffer in case we need to report an error.
        buf.Append(" <");
        buf.Append(cn);
        buf.AppendChar('>');
        Boolean result;
        if(it->HasNext(&result), result) {
            buf.Append(" OR");
        }

        // The CN better have at least two dots if it wants wildcard
        // action.  It also can't be [*.co.uk] or [*.co.jp] or
        // [*.org.uk], etc...
        Boolean doWildcard = cn.StartsWith("*.") &&
                             cn.IndexOf('.', 2) != -1 &&
                             AcceptableCountryWildcard(cn) &&
                             !InetAddressUtils::IsIPv4Address(host);

        if(doWildcard) {
            match = hostName.EndsWith(cn.Substring(1));
            if(match && strictWithSubDomains) {
                // If we're in strict mode, then [*.foo.com] is not
                // allowed to match [a.b.foo.com]
                match = CountDots(hostName) == CountDots(cn);
            }
        }
        else {
            match = hostName.Equals(cn);
        }
        if(match) {
            break;
        }
    }
    if(!match) {
        Logger::E("AbstractVerifier", "hostname in certificate didn't match: <%s> !=%s", host.string(), buf.ToString().string());
        return E_SSL_EXCEPTION;
    }
    return NOERROR;
}

Boolean AbstractVerifier::AcceptableCountryWildcard(
    /* [in] */ const String& cn)
{
    Int32 cnLen = cn->GetLength();
    if(cnLen >= 7 && cnLen <= 9) {
        // Look for the '.' in the 3rd-last position:
        if(cn.GetChar(cnLen - 3) == '.') {
            // Trim off the [*.] and the [.XX].
            String s = cn.Substring(2, cnLen - 3);
            // And test against the sorted array of bad 2lds:
            Int32 x;
            Arrays::BinarySearch(BAD_COUNTRY_2LDS, s, &x);
            return x < 0;
        }
    }
    return TRUE;
}

AutoPtr< ArrayOf<String> > AbstractVerifier::GetCNs(
    /* [in] */ IX509Certificate* cert)
{
    AutoPtr<IX500Principal> principal;
    cert->GetSubjectX500Principal((IX500Principal**)&principal);
    AutoPtr<CDistinguishedNameParser> dnParser;
    CDistinguishedNameParser::NewByFrind(principal, (CDistinguishedNameParser**)&dnParser);
    AutoPtr<IList> cnList;
    dnParser->GetAllMostSpecificFirst(String("cn"), (IList**)&cnList);

    Boolean isEmpty;
    if(cnList->IsEmpty(&isEmpty), !isEmpty) {
        AutoPtr<ICollection> col = ICollection::Probe(cnList);
        Int32 size;
        col->GetSize(&size);
        AutoPtr< ArrayOf<String> > cns = ArrayOf<String>::Alloc(size);
        AutoPtr< ArrayOf<IInterface*> > array;
        col->ToArray((ArrayOf<IInterface*>**)&array);
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            AutoPtr<ICharSequence> cs = ICharSequence::Probe((*array)[i]);
            String str;
            cs->ToString(&str);
            (*cns)[i] = str;
        }
        return cns;
    }
    else {
        return null;
    }
}

AutoPtr< ArrayOf<String> > AbstractVerifier::GetDNSSubjectAlts(
    /* [in] */ IX509Certificate* cert)
{
    AutoPtr<ILinkedList> subjectAltList;
    CLinkedList::New((ILinkedList**)&subjectAltList);
    AutoPtr<ICollection> subjectAltCol = ICollection::Probe(subjectAltList);
    AutoPtr<ICollection> c;
    // try {
    if (FAILED(cert->GetSubjectAlternativeNames((ICollection**)&c))) {
        Logger::E("AbstractVerifier", "Error parsing certificate.");
    }
    // }
    // catch(CertificateParsingException cpe) {
    //     Logger.getLogger(AbstractVerifier.class.getName())
    //             .log(Level.FINE, "Error parsing certificate.", cpe);
    // }
    if(c != NULL) {
        AutoPtr<IIterable> iterable = IIterable::Probe(c);
        AutoPtr<IIterator> it;
        iterable->GetIterator((IIterator**)&it);
        Boolean hasNext;
        while (it->HasNext(&hasNext), hasNext) {
            AutoPtr<IList> aC;
            it->GetNext((IInterface**)&aC);
            AutoPtr<IInteger32> value;
            aC->Get(0, (IInterface**)&value);
            Int32 type;
            value->GetValue(&type);
            // If type is 2, then we've got a dNSName
            if (type == 2) {
                AutoPtr<ICharSequence> cs;
                aC->Get(1, (IInterface**)&cs);
                subjectAltCol->Add(cs);
            }
        }
    }
    Boolean isEmpty;
    if(subjectAltList->IsEmpty(&isEmpty), !isEmpty) {
        Int32 size;
        subjectAltCol->GetSize(&size);
        AutoPtr< ArrayOf<String> > subjectAlts = ArrayOf<String>::Alloc(size);
        AutoPtr< ArrayOf<IInterface*> > array;
        subjectAltCol->ToArray((ArrayOf<IInterface*>**)&array);
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            AutoPtr<ICharSequence> cs = ICharSequence::Probe((*array)[i]);
            String str;
            cs->ToString(&str);
            (*subjectAlts)[i] = str;
        }
        return subjectAlts;
    }
    else {
        return NULL;
    }
}

Int32 AbstractVerifier::CountDots(
    /* [in] */ const String& s)
{
    Int32 count = 0;
    for (Int32 i = 0; i < s.GetLength(); i++) {
        if(s.GetChar(i) == '.') {
            count++;
        }
    }
    return count;
}

} // namespace SSL
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org