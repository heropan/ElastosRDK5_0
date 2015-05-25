
#include "cmdef.h"
#include "CFileDescriptor.h"
#include "IoUtils.h"
#include "COsConstants.h"
#include "CLibcore.h"

using Libcore::IO::IOsConstants;
using Libcore::IO::COsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;

namespace Elastos {
namespace IO {

static AutoPtr<IFileDescriptor> CreateSTDIN()
{
    AutoPtr<COsConstants> osConstans;
    COsConstants::AcquireSingletonByFriend((COsConstants**)&osConstans);
    Int32 fileno;
    osConstans->GetOsConstant(String("STDIN_FILENO"), &fileno);
    AutoPtr<CFileDescriptor> fd;
    CFileDescriptor::NewByFriend((CFileDescriptor**)&fd);
    fd->SetDescriptor(fileno);
    return (IFileDescriptor*)fd.Get();
}

static AutoPtr<IFileDescriptor> CreateSTDOUT()
{
    AutoPtr<COsConstants> osConstans;
    COsConstants::AcquireSingletonByFriend((COsConstants**)&osConstans);
    Int32 fileno;
    osConstans->GetOsConstant(String("STDOUT_FILENO"), &fileno);
    AutoPtr<CFileDescriptor> fd;
    CFileDescriptor::NewByFriend((CFileDescriptor**)&fd);
    fd->SetDescriptor(fileno);
    return (IFileDescriptor*)fd.Get();
}

static AutoPtr<IFileDescriptor> CreateSTDERR()
{
    AutoPtr<COsConstants> osConstans;
    COsConstants::AcquireSingletonByFriend((COsConstants**)&osConstans);
    Int32 fileno;
    osConstans->GetOsConstant(String("STDERR_FILENO"), &fileno);
    AutoPtr<CFileDescriptor> fd;
    CFileDescriptor::NewByFriend((CFileDescriptor**)&fd);
    fd->SetDescriptor(fileno);
    return (IFileDescriptor*)fd.Get();
}

const AutoPtr<IFileDescriptor> CFileDescriptor::IN = CreateSTDIN();
const AutoPtr<IFileDescriptor> CFileDescriptor::OUT = CreateSTDOUT();
const AutoPtr<IFileDescriptor> CFileDescriptor::ERR = CreateSTDERR();

CFileDescriptor::CFileDescriptor()
    : mDescriptor(-1)
{}

ECode CFileDescriptor::Sync()
{
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    Boolean isAtty;
    if (os->Isatty(mDescriptor, &isAtty), isAtty) {
        return IoUtils::Libcore2IoECode(os->Tcdrain(mDescriptor));
    }
    else {
        return IoUtils::Libcore2IoECode(os->Fsync(mDescriptor));
    }
    // } catch (ErrnoException errnoException) {
    //     SyncFailedException sfe = new SyncFailedException(errnoException.getMessage());
    //     sfe.initCause(errnoException);
    //     throw sfe;
    // }
}

ECode CFileDescriptor::Valid(
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);

    *isValid = mDescriptor != -1;
    return NOERROR;
}

ECode CFileDescriptor::GetDescriptor(
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = mDescriptor;
    return NOERROR;
}

ECode CFileDescriptor::SetDescriptor(
    /* [in] */ Int32 fd)
{
    mDescriptor = fd;
    return NOERROR;
}

ECode CFileDescriptor::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("FileDescriptor[");
    str->AppendFormat("%d]", mDescriptor);

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
