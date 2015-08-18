#ifndef __ELASTOS_DROID_DATABASE_CCHARARRAYBUFFER_H__
#define __ELASTOS_DROID_DATABASE_CCHARARRAYBUFFER_H__

#include "_Elastos_Droid_Database_CCharArrayBuffer.h"
#include "database/CharArrayBuffer.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CCharArrayBuffer), public CharArrayBuffer
{
public:
    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Char32>** data);

    CARAPI SetData(
        /* [in] */ ArrayOf<Char32>* data);

    CARAPI GetSizeCopied(
        /* [out] */ Int32* sizeCopied);

    CARAPI SetSizeCopied(
        /* [in] */ Int32 sizeCopied);

    CARAPI constructor(
        /* [in] */ Int32 size);

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32>* buf);

private:
    // TODO: Add your private member variables here.
};

} //Database
} //Droid
} //Elastos

#endif // __ELASTOS_DROID_DATABASE_CCHARARRAYBUFFER_H__
