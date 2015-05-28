#ifndef __CSTRUCTGROUPSOURCEREQ_H__
#define __CSTRUCTGROUPSOURCEREQ_H__

#include "coredef.h"
#include "core/Object.h"
#include "_Elastos_Droid_System_CStructGroupSourceReq.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace System {

CarClass(CStructGroupSourceReq) , public Object
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ Int32 itf,
        /* [in] */ IInetAddress* group,
        /* [in] */ IInetAddress* source);

    CARAPI GetInterface(
        /* [out] */ Int32* itf);

    CARAPI GetGroup(
        /* [out] */ IInetAddress** group);

    CARAPI GetSource(
        /* [out] */ IInetAddress** source);

private:
    Int32 gsr_interface;
    AutoPtr<IInetAddress> gsr_group;
    AutoPtr<IInetAddress> gsr_source;
};

}// namespace System
}// namespace Droid
}// namespace Elastos


#endif