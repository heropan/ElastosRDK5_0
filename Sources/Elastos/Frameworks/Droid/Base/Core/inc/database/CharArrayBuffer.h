#ifndef __ELASTOS_DROID_DATABASE_CHARARRAYBUFFER_H__
#define __ELASTOS_DROID_DATABASE_CHARARRAYBUFFER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Database {
/**
 * This is used for {@link Cursor#copyStringToBuffer}
 */
class CharArrayBuffer
{
public:
    CharArrayBuffer();

    CARAPI_(void) Init(
        /* [in] */ Int32 size);

    CARAPI_(void) Init(
        /* [in] */ ArrayOf<Char32>* buf);

    virtual CARAPI GetData(
        /* [out,callee] */ ArrayOf<Char32>** data);

    virtual CARAPI SetData(
        /* [in] */ ArrayOf<Char32>* data);

    virtual CARAPI GetSizeCopied(
        /* [out] */ Int32* sizeCopied);

    virtual CARAPI SetSizeCopied(
        /* [in] */ Int32 sizeCopied);

public:
    AutoPtr< ArrayOf<Char32> > mData;
    Int32 mSizeCopied;
};

} //Database
} //Droid
} //Elastoss

#endif //_CHARARRAYBUFFER_H_
