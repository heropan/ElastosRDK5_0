
#include "CCheckedOutputStream.h"

namespace Elastos {
namespace Utility {
namespace Zip {

ECode CheckedOutputStream::GetChecksum(
    /* [out] */ IChecksum** checksum)
{
    VALIDATE_NOT_NULL(checksum);
    *checksum = mCheck;
    INTERFACE_ADDREF(*checksum);
    return NOERROR;
}

ECode CheckedOutputStream::Write(
    /* [in] */ Int32 val)
{
    FAIL_RETURN(mOut->Write(val));
    return mCheck->Update(val);
}

ECode CheckedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    FAIL_RETURN(mOut->WriteBytes(buf, off, nbytes));
    return mCheck->Update(buf, off, nbytes);
}

ECode CheckedOutputStream::Close()
{
    ECode ec = Flush();
    ECode ec2 = mOut->Close();
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

ECode CheckedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return WriteBytes(buffer, 0, buffer.GetLength());
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
    FilterOutputStream::Init(os);
    mCheck = cs;
    return NOERROR;
}

//======================================================
ECode CCheckedOutputStream::GetChecksum(
    /* [out] */ IChecksum** checksum)
{
    return CheckedOutputStream::GetChecksum(checksum);
}

ECode CCheckedOutputStream::Write(
    /* [in] */ Int32 val)
{
    return CheckedOutputStream::Write(val);
}

ECode CCheckedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes)
{
    return CheckedOutputStream::WriteBytes(buf, off, nbytes);
}

ECode CCheckedOutputStream::Close()
{
    return CheckedOutputStream::Close();
}

ECode CCheckedOutputStream::Flush()
{
    return CheckedOutputStream::Flush();
}

ECode CCheckedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    return CheckedOutputStream::WriteBytes(buffer);
}

ECode CCheckedOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    return CheckedOutputStream::CheckError(hasError);
}

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

    AutoPtr<IInterface> obj = CheckedOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

PInterface CCheckedOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CCheckedOutputStream::Probe(riid);
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
