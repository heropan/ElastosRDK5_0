
#include "CCheckedOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_INTERFACE_IMPL(CheckedOutputStream, FilterOutputStream, ICheckedOutputStream)

ECode CheckedOutputStream::GetChecksum(
    /* [out] */ IChecksum** checksum)
{
    VALIDATE_NOT_NULL(checksum);
    *checksum = mCheck;
    REFCOUNT_ADD(*checksum);
    return NOERROR;
}

ECode CheckedOutputStream::Write(
    /* [in] */ Int32 val)
{
    FAIL_RETURN(mOut->Write(val));
    return mCheck->Update(val);
}

ECode CheckedOutputStream::Write(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    FAIL_RETURN(mOut->Write(buf, off, nbytes));
    return mCheck->Update(buf, off, nbytes);
}

ECode CheckedOutputStream::Close()
{
    ECode ec = Flush();
    AutoPtr<ICloseable> cls = (ICloseable*)mOut->Probe(Elastos::IO::EIID_ICloseable);
    ECode ec2 = cls->Close();
    if(ec != NOERROR || ec2 != NOERROR){
        return ec != NOERROR ? ec : ec2;
    }
    return NOERROR;
}

ECode CheckedOutputStream::Flush()
{
    return E_NOT_IMPLEMENTED;
    //return mOut->Flush();
}

ECode CheckedOutputStream::Write(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return Write(buffer, 0, buffer.GetLength());
}

ECode CheckedOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    *hasError = FALSE;
    return NOERROR;
}

ECode CheckedOutputStream::Init(
    /* [in] */ IOutputStream* os,
    /* [in] */ IChecksum* cs)
{
    FilterOutputStream::constructor(os);
    mCheck = cs;
    return NOERROR;
}

//======================================================
CAR_OBJECT_IMPL(CCheckedOutputStream)

ECode CCheckedOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IChecksum* cs)
{
    return CheckedOutputStream::Init(os, cs);
}

ECode CCheckedOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj;// = CheckedOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
