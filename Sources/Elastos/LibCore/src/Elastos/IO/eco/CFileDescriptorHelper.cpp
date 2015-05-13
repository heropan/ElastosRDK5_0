
#include "cmdef.h"
#include "CFileDescriptorHelper.h"
#include "CFileDescriptor.h"

namespace Elastos {
namespace IO {

ECode CFileDescriptorHelper::GetInFile(
    /* [out] */ IFileDescriptor** inFile)
{
    VALIDATE_NOT_NULL(inFile);

    *inFile = CFileDescriptor::IN;
    INTERFACE_ADDREF(*inFile);
    return NOERROR;
}

ECode CFileDescriptorHelper::GetOutFile(
    /* [out] */ IFileDescriptor** outFile)
{
    VALIDATE_NOT_NULL(outFile);

    *outFile = CFileDescriptor::OUT;
    INTERFACE_ADDREF(*outFile);
    return NOERROR;
}

ECode CFileDescriptorHelper::GetErrFile(
    /* [out] */ IFileDescriptor** errFile)
{
    VALIDATE_NOT_NULL(errFile);

    *errFile = CFileDescriptor::ERR;
    INTERFACE_ADDREF(*errFile);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
