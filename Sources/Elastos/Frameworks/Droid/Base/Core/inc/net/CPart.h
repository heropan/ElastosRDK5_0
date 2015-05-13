
#ifndef __CPART_H__
#define __CPART_H__

#include "_CPart.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "net/Uri.h"

/**
 * Immutable wrapper of encoded and decoded versions of a URI part. Lazily
 * creates the encoded or decoded version from the other.
 */
class Part : public AbstractPart
{
public:
    /** A part with null values. */
    static AutoPtr<IPart> PartNULL;

    /** A part with empty strings for values. */
    static AutoPtr<IPart> EMPTY;

    virtual CARAPI Init(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded);

    virtual CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty)
    {
        VALIDATE_NOT_NULL(isEmpty);

        *isEmpty = FALSE;

        return NOERROR;
    }

    virtual CARAPI GetEncoded(
        /* [out] */ String* encoded);

    static CARAPI ReadFrom(
        /* [in] */ IParcel* parcel,
        /* [out] */ IPart** result);

    /**
     * Returns given part or {@link #NULL} if the given part is null.
     */
    static CARAPI NonNull(
        /* [in] */ IPart* part,
        /* [out] */ IPart** result);

    /**
     * Creates a part from the encoded string.
     *
     * @param encoded part string
     */
    static CARAPI FromEncoded(
        /* [in] */ const String& encoded,
        /* [out] */ IPart** result);

    /**
     * Creates a part from the decoded string.
     *
     * @param decoded part string
     */
    static CARAPI FromDecoded(
        /* [in] */ const String& decoded,
        /* [out] */ IPart** result);

private:
    /**
     * Creates a part from the encoded and decoded strings.
     *
     * @param encoded part string
     * @param decoded part string
     */
    static CARAPI From(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded,
        /* [out] */ IPart** result);
};

CarClass(CPart), public Part
{
public:
    CARAPI GetEncoded(
        /* [out] */ String* encoded);

    CARAPI GetDecoded(
        /* [out] */ String* decoded);

    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor(
        /* [in] */ const String& encoded,
        /* [in] */ const String& decoded);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPART_H__
