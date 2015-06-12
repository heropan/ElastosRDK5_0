#ifndef __ELASTOS_TEXT_COLLATIONKEY_H__
#define __ELASTOS_TEXT_COLLATIONKEY_H__

#include <elastos.h>
#include <Object.h>
#include "Elastos.CoreLibrary_server.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Text {

class CollationKey
    : public Object
    , public ICollationKey
{
public:
    CAR_INTERFACE_DECL()

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
