
#include "CStringEncoderHelper.h"
#include "StringEncoder.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_SINGLETON_IMPL(CStringEncoderHelper, Object, IStringEncoderHelper)

ECode CStringEncoderHelper::Encode(
    /* [in] */ const ArrayOf<Byte>& a,
    /* [out] */ String * str)
{
    *str = StringEncoder::Encode(const_cast<ArrayOf<Byte>* >(&a));
    return NOERROR;
}

ECode CStringEncoderHelper::Decode(
    /* [in] */ const String& s,
    /* [out,callee] */ ArrayOf<Byte> ** outarray)
{
    *outarray = StringEncoder::Decode(s);
    return NOERROR;
}

ECode CStringEncoderHelper::EncodeX(
    /* [in] */ const ArrayOf<Byte>& a,
    /* [out] */ String * str)
{
    *str = StringEncoder::EncodeX(const_cast<ArrayOf<Byte>* >(&a));
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
