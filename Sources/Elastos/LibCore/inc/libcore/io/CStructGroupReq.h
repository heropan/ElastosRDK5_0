
#ifndef __CSTRUCTGROUPREQ_H__
#define __CSTRUCTGROUPREQ_H__

#include "_CStructGroupReq.h"

using Elastos::Net::IInetAddress;

namespace Libcore {
namespace IO {

/**
 * Corresponds to C's {@code struct group_req}.
 */
CarClass(CStructGroupReq)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 gr_interface,
        /* [in] */ IInetAddress* gr_group);

    CARAPI GetGrInterface(
        /* [out] */ Int32* grInterface);

    CARAPI GetGrGroup(
        /* [out] */ IInetAddress** grGroup);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    Int32 mGr_interface;
    AutoPtr<IInetAddress> mGr_group;
};

} // namespace IO
} // namespace Libcore

#endif // __CSTRUCTGROUPREQ_H__
