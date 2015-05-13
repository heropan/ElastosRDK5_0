#ifndef __COLLATIONKEY_H__
#define __COLLATIONKEY_H__

#include <elastos.h>
#include <cmdef.h>
#include "Elastos.Core_server.h"

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

#endif //__COLLATIONKEY_H__
