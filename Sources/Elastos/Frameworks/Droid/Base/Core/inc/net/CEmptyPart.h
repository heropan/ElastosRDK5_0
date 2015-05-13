
#ifndef __CEMPTYPART_H__
#define __CEMPTYPART_H__

#include "_CEmptyPart.h"
#include "ext/frameworkext.h"
#include "net/Uri.h"
#include "net/CPart.h"

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
    // TODO: Add your private member variables here.
};

#endif // __CEMPTYPART_H__
