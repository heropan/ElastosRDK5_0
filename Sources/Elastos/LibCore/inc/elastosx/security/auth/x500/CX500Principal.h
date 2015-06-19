
#ifndef __CX500PRINCIPAL_H__
#define __CX500PRINCIPAL_H__

#include "_CX500Principal.h"
#include "elastos/Object.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectInputStream;
using Elastos::Utility::IMap;

namespace Elastosx {
namespace Security {
namespace Auth {
namespace X500 {

CarClass(CX500Principal)
{
public:
    /**
     * Returns an ASN.1 DER-encoded representation of the distinguished name
     * contained in this X.500 principal.
     *
     * @return the ASN.1 DER-encoded representation
     */
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> **encodedName);

    /**
     * Returns a string representation of the distinguished name contained in
     * this X.500 principal. The format of the representation can be chosen.
     * Valid arguments are {@link #RFC1779}, {@link #RFC2253}, and
     * {@link #CANONICAL}. The representations are specified in RFC 1779 and RFC
     * 2253, respectively. The canonical form is based on RFC 2253, but adds
     * some canonicalizing operations like removing leading and trailing
     * whitespace, lower-casing the whole name, and bringing it into a
     * normalized Unicode representation.
     *
     * @param format
     *            the name of the format to use for the representation
     *
     * @return the string representation
     *
     * @throws IllegalArgumentException
     *             if the {@code format} argument is not one of the three
     *             mentioned above
     */
    CARAPI GetNameEx(
        /* [in] */ const String& format,
        /* [out] */ String *name);

    CARAPI GetNameEx2(
        /* [in] */ const String& format,
        /* [in] */ IMap *oidMap,
        /* [out] */ String *name);

    CARAPI Equals(
        /* [in] */ IInterface *obj,
        /* [out] */ Boolean *result);

    /**
     * Returns a human-readable string representation of the distinguished name
     * contained in this X.500 principal.
     *
     * @return the string representation
     */
    CARAPI GetName(
        /* [out] */ String *name);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI ToString(
        /* [out] */ String *str);

    /**
     * Creates a new X500Principal from a given ASN.1 DER encoding of a
     * distinguished name.
     *
     * @param name
     *            the ASN.1 DER-encoded distinguished name
     *
     * @throws IllegalArgumentException
     *             if the ASN.1 DER-encoded distinguished name is incorrect
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> *name);

    /**
     * Creates a new X500Principal from a given ASN.1 DER encoding of a
     * distinguished name.
     *
     * @param in
     *            an {@code InputStream} holding the ASN.1 DER-encoded
     *            distinguished name
     *
     * @throws IllegalArgumentException
     *             if the ASN.1 DER-encoded distinguished name is incorrect
     */
    CARAPI constructor(
        /* [in] */ IInputStream *is);

    /**
     * Creates a new X500Principal from a string representation of a
     * distinguished name.
     *
     * @param name
     *            the string representation of the distinguished name
     *
     * @throws IllegalArgumentException
     *             if the string representation of the distinguished name is
     *             incorrect
     */
    CARAPI constructor(
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ IMap *keywordMap);

private:
    CARAPI GetCanonicalName(
        /* [out] */ String *name);

    CARAPI ReadObject(
        /* [in] */ IObjectInputStream *in);

    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream *out);

    CARAPI SubstituteNameFromMap(
        /* [in] */ const String& name,
        /* [in] */ IMap *keywordMap,
        /* [out] */ String *nm);

private:
    static const Int64 serialVersionUID;

    //Apache...Todo later
    //Distinguished Name
    //Name dn;

    String mCanonicalName;

    Object mLock;
};

}
}
}
}

#endif // __CX500PRINCIPAL_H__
