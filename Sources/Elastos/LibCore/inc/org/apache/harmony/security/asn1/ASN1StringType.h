
#ifndef __ASN1STRINGTYPE_H__
#define __ASN1STRINGTYPE_H__

#include <elastos.h>
#include "ASN1Type.h"

_ELASTOS_NAMESPACE_USING

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

/**
 * This class is the super class for all string ASN.1 types
 *
 * @see <a href="http://asn1.elibel.tm.fr/en/standards/index.htm">ASN.1</a>
 */
class ASN1StringType
    : public ASN1Type {
public:
    static AutoPtr<IASN1Type> InitStatic();

    ASN1StringType(
        /* [in] */ Int32 tagNumber);

    CARAPI Init(
        /* [in] */ Int32 tagNumber);

    virtual CARAPI CheckTag(
        /* [in] */ Int32 identifier,
        /* [ou] */ Boolean* result);

    virtual CARAPI DecodeEx3(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** object);

    virtual CARAPI GetDecodedObject(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** object);

    virtual CARAPI EncodeASN(
        /* [in] */ IBerOutputStream* bos);

    virtual CARAPI EncodeContent(
        /* [in] */ IBerOutputStream* bos);

    virtual CARAPI SetEncodingContent(
        /* [in] */ IBerOutputStream* bos);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
private:
    class ASN1StringTypeNull;

    class ASN1StringTypeOfDerived;
public:
    // TODO: what about defining them as separate classes?
    // TODO: check decoded/encoded characters
    static AutoPtr<IASN1Type> sBMPSTRING;

    static AutoPtr<IASN1Type> sIA5STRING;

    static AutoPtr<IASN1Type> sGENERALSTRING;

    static AutoPtr<IASN1Type> sPRINTABLESTRING;

    static AutoPtr<IASN1Type> sTELETEXSTRING;

    static AutoPtr<IASN1Type> sUNIVERSALSTRING;

    static AutoPtr<IASN1Type> sUTF8STRING;
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ASN1STRINGTYPE_H__
