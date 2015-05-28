#include "CStructGroupSourceReq.h"

namespace Elastos {
namespace Droid {
namespace System {

CAR_OBJECT_IMPL(CStructGroupSourceReq)

CAR_INTERFACE_IMPL(CStructGroupSourceReq, Object, IStructGroupSourceReq)

ECode CStructGroupSourceReq::constructor(
    /* [in] */ Int32 itf,
    /* [in] */ IInetAddress* group,
    /* [in] */ IInetAddress* source)
{
    gsr_interface = itf;
    gsr_group = group;
    gsr_source = source;
    return NOERROR;
}

ECode CStructGroupSourceReq::GetInterface(
    /* [out] */ Int32* itf)
{
    *itf = gsr_interface;
    return NOERROR;
}

ECode CStructGroupSourceReq::GetGroup(
    /* [out] */ IInetAddress** group)
{
    *group = gsr_group;
    REFCOUNT_ADD(*group)
    return NOERROR;
}

ECode CStructGroupSourceReq::GetSource(
    /* [out] */ IInetAddress** source)
{
    *source = gsr_source;
    REFCOUNT_ADD(*source)
    return NOERROR;
}

}// namespace System
}// namespace Droid
}// namespace Elastos
