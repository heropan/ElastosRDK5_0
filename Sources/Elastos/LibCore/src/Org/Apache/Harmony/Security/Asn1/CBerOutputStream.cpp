
#include "CBerOutputStream.h"
#include <cmdef.h>

using Elastos.Core.IBoolean;
using Elastos.Core.IArrayOf;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {


ECode CBerOutputStream::SetEncoded(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return BerOutputStream::SetEncoded(encoded);
}

ECode CBerOutputStream::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    return BerOutputStream::GetEncoded(encoded);
}

ECode CBerOutputStream::SetLength(
    /* [in] */ Int32 length)
{
    return BerOutputStream::SetLength(length);
}

ECode CBerOutputStream::GetLength(
    /* [out] */ Int32* length)
{
    return BerOutputStream::GetLength(length);
}

ECode CBerOutputStream::SetContent(
    /* [in] */ IInterface* content)
{
    return BerOutputStream::SetContent(content);
}

ECode CBerOutputStream::GetContent(
    /* [out] */ IInterface** content)
{
    return BerOutputStream::GetContent(content);
}

ECode CBerOutputStream::EncodeTag(
    /* [in] */ Int32 tag)
{
    return BerOutputStream::EncodeTag(tag);
}

ECode CBerOutputStream::EncodeANY()
{
    return BerOutputStream::EncodeANY();
}

ECode CBerOutputStream::EncodeBitString()
{
    return BerOutputStream::EncodeBitString();
}

ECode CBerOutputStream::EncodeBoolean()
{
    return BerOutputStream::EncodeBoolean();
}

ECode CBerOutputStream::EncodeChoice(
    /* [in] */ IASN1Choice* choice)
{
    return BerOutputStream::EncodeChoice(choice);
}

ECode CBerOutputStream::EncodeExplicit(
    /* [in] */ IASN1Type* xplicit)
{
    return BerOutputStream::EncodeExplicit(xplicit);
}

ECode CBerOutputStream::EncodeGeneralizedTime()
{
    return BerOutputStream::EncodeGeneralizedTime();
}

ECode CBerOutputStream::EncodeUTCTime()
{
    return BerOutputStream::EncodeUTCTime();
}

ECode CBerOutputStream::EncodeInteger()
{
    return BerOutputStream::EncodeInteger();
}

ECode CBerOutputStream::EncodeOctetString()
{
    return BerOutputStream::EncodeOctetString();
}

ECode CBerOutputStream::EncodeOID()
{
    return BerOutputStream::EncodeOID();
}

ECode CBerOutputStream::EncodeSequence(
    /* [in] */ IASN1Sequence* sequence)
{
    return BerOutputStream::EncodeSequence(sequence);
}

ECode CBerOutputStream::EncodeSequenceOf(
    /* [in] */ IASN1SequenceOf* sequenceOf)
{
    return BerOutputStream::EncodeSequenceOf(sequenceOf);
}

ECode CBerOutputStream::EncodeSet(
    /* [in] */ IASN1Set* set)
{
    return BerOutputStream::EncodeSet(set);
}

ECode CBerOutputStream::EncodeSetOf(
    /* [in] */ IASN1SetOf* setOf)
{
    return BerOutputStream::EncodeSetOf(setOf);
}

ECode CBerOutputStream::EncodeString()
{
    return BerOutputStream::EncodeString();
}

ECode CBerOutputStream::GetChoiceLength(
    /* [in] */ IASN1Choice* choice)
{
    return BerOutputStream::GetChoiceLength(choice);
}

ECode CBerOutputStream::GetExplicitLength(
    /* [in] */ IASN1Type* sequence)
{
    return BerOutputStream::GetExplicitLength(sequence);
}

ECode CBerOutputStream::GetSequenceLength(
    /* [in] */ IASN1Sequence* sequence)
{
    return BerOutputStream::GetSequenceLength(sequence);
}

ECode CBerOutputStream::GetSequenceOfLength(
    /* [in] */ IASN1SequenceOf* sequence)
{
    return BerOutputStream::GetSequenceOfLength(sequence);
}

ECode CBerOutputStream::GetSetLength(
    /* [in] */ IASN1Set* set)
{
    return BerOutputStream::GetSetLength(set);
}

ECode CBerOutputStream::GetSetOfLength(
    /* [in] */ IASN1SetOf* setOf)
{
    return BerOutputStream::GetSetOfLength(setOf);
}

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

