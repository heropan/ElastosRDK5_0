
#ifndef __CSTRINGENCODERHELPER_H__
#define __CSTRINGENCODERHELPER_H__

#include "_Elastos_Sql_SQLite_CStringEncoderHelper.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CStringEncoderHelper)
    , public Object
    , public IStringEncoderHelper
{
public:
    CAR_INTERFACE_DECL();

    CAR_SINGLETON_DECL();

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