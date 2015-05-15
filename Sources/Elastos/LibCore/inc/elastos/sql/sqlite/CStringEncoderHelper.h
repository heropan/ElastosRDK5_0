
#ifndef __CSTRINGENCODERHELPER_H__
#define __CSTRINGENCODERHELPER_H__

#include "_CStringEncoderHelper.h"
#include "StringEncoder.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CStringEncoderHelper) , public StringEncoder
{
public:
    CARAPI Encode(
        /* [in] */ const ArrayOf<Byte>& a,
        /* [out] */ String * str);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out,callee] */ ArrayOf<Byte> ** outarray);

    CARAPI EncodeX(
        /* [in] */ const ArrayOf<Byte>& a,
        /* [out] */ String * str);
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CSTRINGENCODERHELPER_H__