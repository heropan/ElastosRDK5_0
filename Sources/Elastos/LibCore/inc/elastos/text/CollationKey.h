#ifndef __ELASTOS_TEXT_COLLATIONKEY_H__
#define __ELASTOS_TEXT_COLLATIONKEY_H__

#include <elastos.h>
#include "Elastos.CoreLibrary_server.h"

namespace Elastos {
namespace Text {

class CollationKey
{
public:
    virtual CARAPI CompareTo(
        /* [in] */ IInterface* value,
        /* [out] */ Int32* result) = 0;

    virtual CARAPI GetSourceString(
        /* [out] */ String* sourceString);

    virtual CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** array) = 0;

protected:
    CARAPI Init(
        /* [in] */ const String& source);

private:
    String source;
};

} // namespace Text
} // namespace Elastos

#endif //__ELASTOS_TEXT_COLLATIONKEY_H__
