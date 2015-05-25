
#include "cmdef.h"
#include "CStructGroupReq.h"
#include "StringUtils.h"

using Elastos::Core::StringUtils;

namespace Libcore {
namespace IO {

ECode CStructGroupReq::constructor(
    /* [in] */ Int32 gr_interface,
    /* [in] */ IInetAddress* gr_group)
{
    mGr_interface = gr_interface;
    mGr_group = gr_group;
    return NOERROR;
}

ECode CStructGroupReq::GetGrInterface(
    /* [out] */ Int32* grInterface)
{
    VALIDATE_NOT_NULL(grInterface)

    *grInterface = mGr_interface;
    return NOERROR;
}

ECode CStructGroupReq::GetGrGroup(
    /* [out] */ IInetAddress** grGroup)
{
    VALIDATE_NOT_NULL(grGroup)

    *grGroup = mGr_group;
    INTERFACE_ADDREF(*grGroup)
    return NOERROR;
}

ECode CStructGroupReq::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    String outstr;
    mGr_group->ToString(&outstr);
    *str = String("StructGroupReq[gr_interface=")
            + StringUtils::Int32ToString(mGr_interface)
            + String(",gr_group=")
            + outstr
            + String("]");
    return NOERROR;
}

} // namespace IO
} // namespace Libcore
