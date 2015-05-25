
#ifndef __ASN1TYPE_H__
#define __ASN1TYPE_H__

#include <elastos.h>
#include <Org.Apache.Harmony_server.h>

_ELASTOS_NAMESPACE_USING

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
class ASN1Type {
public:
    ASN1Type(
        /* [in] */ Int32 tagNumber);

    Init(
        /* [in] */ Int32 tagNumber);

    ASN1Type(
        /* [in] */ Int32 tagClass,
        /* [in] */ Int32 tagNumber);

    Init(
        /* [in] */ Int32 tagClass,
        /* [in] */ Int32 tagNumber);

    CARAPI GetId(
        /* [out] */ Int32* id);

    /** Integer representation of constructed identifier. */
    CARAPI GetConstrId(
        /* [out] */ Int32* constrId);

    virtual CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [out] */ IInterface** object);

    virtual CARAPI DecodeEx(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 encodingLen,
        /* [out] */ IInterface** object);

    virtual CARAPI DecodeEx2(
        /* [in] */ IInputStream* is,
        /* [out] */ IInterface** object);

    virtual CARAPI Verify(
        /* [in] */ ArrayOf<Byte>* encoded);

    virtual CARAPI VerifyEx(
        /* [in] */ IInputStream* is);

    virtual CARAPI Encode(
        /* [in] */ IInterface* object,
        /* [out, callee] */ ArrayOf<Byte>** encode);

    virtual CARAPI DecodeEx3(
        /* [in] */ IBerInputStream* bis,
        /* [out] */ IInterface** object) = 0;

    /**
     * Tests provided identifier.
     *
     * @param identifier identifier to be verified
     * @return true if identifier is associated with this ASN.1 type
    */
    virtual CARAPI CheckTag(
        /* [in] */ Int32 identifier,
        /* [out] */ Boolean* checkTag) = 0;

    virtual CARAPI EncodeASN(
        /* [in] */ IBerOutputStream* bos) = 0;

    virtual CARAPI EncodeContent(
        /* [in] */ IBerOutputStream* bos) = 0;

    virtual CARAPI SetEncodingContent(
        /* [in] */ IBerOutputStream* bos) = 0;

    virtual CARAPI GetEncodedLength(
        /* [in] */ IBerOutputStream* bos,
        /* [out] */ Int32* length);

    virtual CARAPI ToString(
        /* [out] */ String* result);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

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

private:
    CARAPI InitEx(
        /* [in] */ Int32 tagClass,
        /* [in] */ Int32 tagNumber);

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

#endif // __ASN1TYPE_H__
