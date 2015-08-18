
#ifndef __ORG_APACHE_HARMONY_SECURITY_PKCS7_CCONTENTINFO_H__
#define __ORG_APACHE_HARMONY_SECURITY_PKCS7_CCONTENTINFO_H__

#include "_Org_Apache_Harmony_Security_Pkcs7_CContentInfo.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs7 {

CarClass(CContentInfo)
{
private:
    friend class ASN1SequenceDerived;
    class ASN1SequenceDerived
        : public ASN1Sequence
        , public IASN1Sequence
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()
        ASN1TYPE_METHODS_DECL()
        CARAPI SetType(
        /* [in] */ ArrayOf<IASN1Type *>* type);

        CARAPI GetType(
            /* [out, callee] */ ArrayOf<IASN1Type *>** type);

        CARAPI SetOPTIONAL(
            /* [in] */ ArrayOf<Boolean>* optional);

        CARAPI GetOPTIONAL(
            /* [out, callee] */ ArrayOf<Boolean>** optional);

        CARAPI SetDEFAULT(
            /* [in] */ ArrayOf<IInterface *>* def);

        CARAPI GetDEFAULT(
            /* [out, callee] */ ArrayOf<IInterface *>** def);

        CARAPI GetValues(
            /* [in] */ IInterface* object,
            /* [in] */ ArrayOf<IInterface*>* values);
        ASN1SequenceDerived(
            /* [in] */ ArrayOf<IASN1Type *>* type);
    };
public:
    static CARAPI_(AutoPtr<ArrayOf<Int32> >) InitStatic();

    CContentInfo();

    CARAPI GetSignedData(
        /* [out] */ ISignedData** signedData);

    CARAPI GetContent(
        /* [out] */ IInterface** content);

    CARAPI GetContentType(
        /* [out, callee] */ ArrayOf<Int32>** contentType);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    CARAPI ToString(
        /* [out] */ String* str);

    CContentInfo(
        /* [in] */ ArrayOf<Int32>* oid,
        /* [in] */ IInterface* content,
        /* [in] */ ArrayOf<Byte>* encoding);


public:
    // OIDs
    static AutoPtr<ArrayOf<Int32> > DATA;//= new int[] {1, 2, 840, 113549, 1, 7, 1};
    static AutoPtr<ArrayOf<Int32> > SIGNED_DATA;//= new int[] {1, 2, 840, 113549, 1, 7, 2};
    static AutoPtr<ArrayOf<Int32> > ENVELOPED_DATA;//= new int[] {1, 2, 840, 113549, 1, 7, 3};
    static AutoPtr<ArrayOf<Int32> > SIGNED_AND_ENVELOPED_DATA;//= new int[] {1, 2, 840, 113549, 1, 7, 4};
    static AutoPtr<ArrayOf<Int32> > DIGESTED_DATA;//= new int[] {1, 2, 840, 113549, 1, 7, 5};
    static AutoPtr<ArrayOf<Int32> > ENCRYPTED_DATA;//= new int[] {1, 2, 840, 113549, 1, 7, 6};
    static AutoPtr<IASN1Sequence> ASN1;

    AutoPtr<ArrayOf<Int32> > mOid;
    AutoPtr<IInterface> mContent;
    AutoPtr<ArrayOf<Byte> > mEncoding;
};

} // namespace Pkcs7
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_PKCS7_CCONTENTINFO_H__
