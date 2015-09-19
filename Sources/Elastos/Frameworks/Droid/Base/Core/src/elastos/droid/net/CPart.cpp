
#include "net/CPart.h"
#include "net/CEmptyPart.h"

class CStatic
{
public:

    CStatic()
    {
        // CEmptyPart::NewByFriend(String(NULL), (IPart**)&Part::PartNULL);
        // CEmptyPart::NewByFriend(String(""), (IPart**)&Part::EMPTY);
    }
};

static CStatic _static;

/** A part with null values. */
AutoPtr<IPart> Part::PartNULL;

/** A part with empty strings for values. */
AutoPtr<IPart> Part::EMPTY;

ECode Part::Init(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    return AbstractPart::Init(encoded, decoded);
}

ECode Part::GetEncoded(
    /* [out] */ String* encoded)
{
    VALIDATE_NOT_NULL(encoded);

    // @SuppressWarnings("StringEquality")
    Boolean hasEncoded = !mEncoded.Equals(Uri::NOT_CACHED);
    if (!hasEncoded) FAIL_RETURN(Uri::Encode(mDecoded, &mEncoded));
    *encoded = mEncoded;

    return NOERROR;
}

ECode Part::ReadFrom(
    /* [in] */ IParcel* parcel,
    /* [out] */ IPart** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 representation;
    FAIL_RETURN(parcel->ReadInt32(&representation));

    switch (representation) {
        case Representation::BOTH: {
            String str1;
            FAIL_RETURN(parcel->ReadString(&str1));

            String str2;
            FAIL_RETURN(parcel->ReadString(&str2));

            return From(str1, str2, result);
        }
        case Representation::ENCODED: {
            String str1;
            FAIL_RETURN(parcel->ReadString(&str1));

            return FromEncoded(str1, result);
        }
        case Representation::DECODED: {
            String str1;
            FAIL_RETURN(parcel->ReadString(&str1));

            return FromDecoded(str1, result);
        }
        default: {
            // throw new AssertionError();
            return E_ASSERTION_ERROR;
        }
    }
}

ECode Part::NonNull(
    /* [in] */ IPart* part,
    /* [out] */ IPart** result)
{
    VALIDATE_NOT_NULL(result);

    *result = (part == NULL) ? (IPart*)Part::PartNULL : part;
    REFCOUNT_ADD(*result);

    return NOERROR;
}

ECode Part::FromEncoded(
    /* [in] */ const String& encoded,
    /* [out] */ IPart** result)
{
    return From(encoded, Uri::NOT_CACHED, result);
}

ECode Part::FromDecoded(
    /* [in] */ const String& decoded,
    /* [out] */ IPart** result)
{
    return From(Uri::NOT_CACHED, decoded, result);
}

ECode Part::From(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded,
    /* [out] */ IPart** result)
{
    VALIDATE_NOT_NULL(result);

    // We have to check both encoded and decoded in case one is
    // NOT_CACHED.
    if (encoded.IsNull()) {
        *result = Part::PartNULL;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    if (encoded.IsEmpty()) {
        *result = Part::EMPTY;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }

    if (decoded.IsNull()) {
        *result = Part::PartNULL;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    if (decoded.IsEmpty()) {
        *result = Part::EMPTY;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }

    return CPart::New(encoded, decoded, result);
}

ECode CPart::GetEncoded(
    /* [out] */ String* encoded)
{
    return Part::GetEncoded(encoded);
}

ECode CPart::GetDecoded(
    /* [out] */ String* decoded)
{
    return Part::GetDecoded(decoded);
}

ECode CPart::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    return Part::IsEmpty(isEmpty);
}

ECode CPart::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CPart::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return AbstractPart::WriteTo(dest);
}

ECode CPart::constructor(
    /* [in] */ const String& encoded,
    /* [in] */ const String& decoded)
{
    return Part::Init(encoded, decoded);
}

