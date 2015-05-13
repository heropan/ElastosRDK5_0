
#include "net/CEmptyPart.h"

ECode CEmptyPart::GetEncoded(
    /* [out] */ String* encoded)
{
    return EmptyPart::GetEncoded(encoded);
}

ECode CEmptyPart::GetDecoded(
    /* [out] */ String* decoded)
{
    return EmptyPart::GetDecoded(decoded);
}

ECode CEmptyPart::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    return EmptyPart::IsEmpty(isEmpty);
}

ECode CEmptyPart::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CEmptyPart::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return AbstractPart::WriteTo(dest);
}

ECode CEmptyPart::constructor(
    /* [in] */ const String& value)
{
    return Part::Init(value, value);
}

