
#ifndef __ORG_APACHE_HARMONY_SECURITY_ASN1_BERINPUTSTREAM_H__
#define __ORG_APACHE_HARMONY_SECURITY_ASN1_BERINPUTSTREAM_H__

using Elastos::IO::IInputStream;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

class BerInputStream
{
public:
    static CARAPI GetLength(
        /* [in] */ ArrayOf<Byte>* encoding,
        /* [out] */ Int32* length);

    CARAPI SetTag(
        /* [in] */ Int32 tag);

    CARAPI GetTag(
        /* [out] */ Int32* tag);

    CARAPI SetLength(
        /* [in] */ Int32 length);

    CARAPI SetContent(
        /* [in] */ IInterface* content);

    CARAPI GetContent(
        /* [out] */ IInterface** content);

    CARAPI GetTagOffSet(
        /* [out] */ Int32* tagOffset);

    CARAPI SetTagOffSet(
        /* [in] */ Int32 tagOffset);

    CARAPI GetContentOffset(
        /* [out] */ Int32* contentOffset);

    CARAPI SetContentOffset(
        /* [in] */ Int32 contentOffset);

    virtual CARAPI Reset(
        /* [in] */ ArrayOf<Byte>* encoded);

    virtual CARAPI Next(
        /* [out] */ Int32* next);

    virtual CARAPI ReadBitString();

    virtual CARAPI ReadEnumerated();

    virtual CARAPI ReadBoolean();

    CARAPI SetChoiceIndex(
        /* [in] */ Int32 choiceIndex);

    CARAPI GetChoiceIndex(
        /* [out] */ Int32* choiceIndex);

    CARAPI SetTimes(
        /* [in] */ ArrayOf<Int32>* times);

    CARAPI GetTimes(
        /* [out, callee] */ ArrayOf<Int32>** times);

    virtual CARAPI ReadGeneralizedTime();

    virtual CARAPI ReadUTCTime();

    virtual CARAPI ReadInteger();

    virtual CARAPI ReadOctetString();

    CARAPI SetOidElement(
        /* [in] */ Int32 oidElement);

    CARAPI GetOidElement(
        /* [out] */ Int32* oidElement);

    virtual CARAPI ReadOID();

    virtual CARAPI ReadSequence(
        /* [out] */ IASN1Sequence** sequence);

    virtual CARAPI ReadSequenceOf(
        /* [in] */ IASN1SequenceOf* sequenceOf);

    virtual CARAPI ReadSet(
        /* [in] */ IASN1Set* set);

    virtual CARAPI ReadSetOf(
        /* [in] */ IASN1SetOf* setOf);

    virtual CARAPI ReadString(
        /* [in] */ IASN1Type* type);

    virtual CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    virtual CARAPI GetBuffer(
        /* [out, callee] */ ArrayOf<Byte>** buffer);

    virtual CARAPI GetLength(
        /* [out] */ Int32* length);

    virtual CARAPI GetOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI GetEndOffset(
        /* [out] */ Int32* endOffset);

    virtual CARAPI GetTagOffset(
        /* [out] */ Int32* tagOffset);

    virtual CARAPI SetVerify();

    virtual CARAPI ReadContent();

    virtual CARAPI CompactBuffer();

    virtual CARAPI Put(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* entry);

    virtual CARAPI Get(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** entry);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 expectedLength);

    CARAPI constructor(
        /* [in] */ IInputStream* is);

    CARAPI constructor(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 initialSize);

protected:
    virtual CARAPI Read(
        /* [out] */ Int32* ret);

private:
    CARAPI InnerConstructor(
        /* [in] */ ArrayOf<Byte>* encoded,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 expectedLength);

    CARAPI InnerConstructorEx(
        /* [in] */ IInputStream* is,
        /* [in] */ Int32 initialSize);

    CARAPI StrToInt(
        /* [in] */ Int32 off,
        /* [in] */ Int32 count,
        /* [out] */ Int32* ret);

    CARAPI DecodeValueCollection(
        /* [in] */ IASN1ValueCollection* collection);

public:
    /** Current decoded tag */
    Int32 mTag;

    /** Current decoded content */
    AutoPtr<IInterface> mContent;

    /** The last choice index */
    Int32 mChoiceIndex;

    /** Keeps last decoded: year, month, day, hour, minute, second, millisecond */
    AutoPtr<ArrayOf<Int32> > mTimes;

    Int32 mOidElement;

protected:
    /** Indicates indefinite length of the current type */
    static const Int32 INDEFINIT_LENGTH = -1;

    AutoPtr<ArrayOf<Byte> > mBuffer;

    /**
    * The position in the buffer.
    * Next read must place data into the buffer from this offset
     */
    Int32 mOffset;// = 0;

    /** Current decoded length */
    Int32 mLength;

    /** Current decoded tag offset */
    Int32 mTagOffset;

    /** Current decoded content offset */
    Int32 mContentOffset;

    /**
     * Indicates defined or indefined reading mode for associated InputStream.
     *
     * This mode is defined by reading a length
     * for a first ASN.1 type from InputStream.
     */
    Boolean mIsIndefinedLength;

    /**
     * Indicates verify or store mode.
     *
     * In store mode a decoded content is stored in a newly allocated
     * appropriate object. The <code>content</code> variable holds
     * a reference to the last created object.
     *
     * In verify mode a decoded content is not stored.
     */
    // FIXME it is used only for one case
    // decoding PCKS#8 Private Key Info notation
    // remove this option because it does decoding more complex
    Boolean mIsVerify;
private:
    AutoPtr<IInputStream> mIn;

    /**
    * The buffer increment size.
    * Must be reasonable big to reallocate memory not to often.
    * Primary is used for decoding indefinite length encoding
     */
    static const Int32 BUF_INCREASE_SIZE = 1024* 16;

    AutoPtr<ArrayOf<IArrayOf*> > mPool;
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CBERINPUTSTREAM_H__
