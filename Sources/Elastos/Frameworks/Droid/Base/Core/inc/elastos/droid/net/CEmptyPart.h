
#ifndef __CEMPTYPART_H__
#define __CEMPTYPART_H__

#include "_CEmptyPart.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/net/CPart.h"

class EmptyPart : public Part
{
public:
    virtual CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty)
    {
        VALIDATE_NOT_NULL(isEmpty);

        *isEmpty = TRUE;

        return NOERROR;
    }
};

CarClass(CEmptyPart), public EmptyPart
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
        /* [in] */ const String& value);

private:
};

#endif // __CEMPTYPART_H__
