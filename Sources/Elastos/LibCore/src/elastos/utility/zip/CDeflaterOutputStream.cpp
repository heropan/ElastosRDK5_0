
#include "CDeflaterOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CDeflaterOutputStream)

ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    return DeflaterOutputStream::Init(os);
}

ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def)
{
    return DeflaterOutputStream::Init(os, def);
}

ECode CDeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize)
{
    return DeflaterOutputStream::Init(os, def, bsize);
}

ECode CDeflaterOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj;// = DeflaterOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
