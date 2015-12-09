
#ifndef __ORG_APACHE_HARMONY_SECURITY_ASN1_ASN1TYPE_H__
#define __ORG_APACHE_HARMONY_SECURITY_ASN1_ASN1TYPE_H__

#include "Elastos.CoreLibrary.Apache.h"
#include "core/Object.h"

using Elastos::Core::Object;
using Elastos::IO::IInputStream;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {


/**
 * This abstract class is the super class for all ASN.1 types
 *
 * @see <a href="http://asn1.elibel.tm.fr/en/standards/index.htm">ASN.1</a>
 */
class ASN1Type
    : public Object
    , public IASN1Type
    , public IASN1Constants
{
public:
    ASN1Type();

    CARAPI constructor(
        /* [in] */ Int32 tagNumber);

    CARAPI constructor(
        /* [in] */ Int32 tagClass,
        /* [in] */ Int32 tagNumber);

    CAR_INTERFACE_DECL()

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [out] */ IInterface** object);

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 encodingLen,
        /* [out] */ IInterface** object);

    CARAPI Decode(
        /* [in] */ IInputStream* is,
        /* [out] */ IInterface** object);

    CARAPI Verify(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI Verify(
        /* [in] */ IInputStream* is);

    CARAPI Encode(
        /* [in] */ IInterface* object,
        /* [out, callee] */ ArrayOf<Byte>** encode);

    using IASN1Type::Decode;

    using IASN1Type::CheckTag;

    using IASN1Type::EncodeASN;

    using IASN1Type::EncodeContent;

    using IASN1Type::SetEncodingContent;

    CARAPI GetEncodedLength(
        /* [in] */ IBerOutputStream* bos,
        /* [out] */ Int32* length);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetId(
        /* [out] */ Int32* id);

    CARAPI GetConstrId(
        /* [out] */ Int32* constrId);

protected:
        /**
     * Creates decoded object.
     *
     * Derived classes should override this method to provide creation for a
     * selected class of objects during decoding.
     *
     * The default implementation returns an object created by decoding stream.
    */
    virtual CARAPI GetDecodedObject(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** object);

public:
    /** Integer representation of primitive identifier. */
    Int32 mId;

    /** Integer representation of constructed identifier. */
    Int32 mConstrId;
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_ASN1_ASN1TYPE_H__
