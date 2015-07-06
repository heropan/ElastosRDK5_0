
#include "MappedByteBuffer.h"
//#include "ReadOnlyDirectByteBuffer.h"
//#include "ReadWriteDirectByteBuffer.h"
// #include "CLibcore.h"
// #include "COsConstants.h"

using Elastos::IO::Channels::FileChannelMapMode_READ_ONLY;
using Elastos::IO::Channels::FileChannelMapMode_READ_WRITE;
// using Libcore::IO::CLibcore;
// using Libcore::IO::COsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::IOs;
// using Elastos::Droid::System::IOsConstants;

namespace Elastos {
namespace IO {

// {E6FFB679-4B6D-E9D8-668A-C93B2E0819A8}
extern "C" const InterfaceID EIID_MappedByteBuffer =
    { 0xe6ffb679, 0x4b6d, 0xe9d8, { 0x66, 0x8a, 0xc9, 0x3b, 0x2e, 0x08, 0x19, 0xa8 } };

MappedByteBuffer::MappedByteBuffer()
{}

MappedByteBuffer::MappedByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ FileChannelMapMode mapMode,
    /* [in] */ Int64 effectiveDirectAddress)
    : ByteBuffer(capacity, effectiveDirectAddress)
    , mBlock(block)
    , mMapMode(mapMode)
{
}

ECode MappedByteBuffer::IsLoaded(
    /* [out] */ Boolean* isLoaded)
{
    VALIDATE_NOT_NULL(isLoaded)
    Int64 address = mBlock->ToInt64();
    Int64 size = mBlock->GetSize();
    if (size == 0) {
        *isLoaded = TRUE;
        return NOERROR;
    }

    // try {
    *isLoaded = FALSE;
    Int32 pageSize = 0;
    Int64 tmpSize = 0;
    AutoPtr<ILibcore> libcore;
    AutoPtr<IOs> os;
    assert(0 && "TODO");
    // ASSERT_SUCCEEDED(CLibcore::AcquireSingleton((ILibcore**)&libcore));
    libcore->GetOs((IOs**)&os);
    // AutoPtr<IOsConstants> osConstans;
    assert(0 && "TODO");
    // ASSERT_SUCCEEDED(COsConstants::AcquireSingleton((IOsConstants**)&osConstans))
    Int32 SC_PAGE_SIZE = 0;
    assert(0 && "TODO");
    // FAIL_RETURN(osConstans->GetOsConstant(String("_SC_PAGE_SIZE"), &SC_PAGE_SIZE))
    FAIL_RETURN(os->Sysconf(SC_PAGE_SIZE, &tmpSize))
    pageSize = (Int32) tmpSize;
    Int32 pageOffset = (Int32) (address % pageSize);
    address -= pageOffset;
    size += pageOffset;
    Int32 pageCount = (Int32) ((size + pageSize - 1) / pageSize);
    AutoPtr< ArrayOf<Byte> > vector = ArrayOf<Byte>::Alloc(pageCount);
    FAIL_RETURN(os->Mincore(address, size, vector))
    for (Int32 i = 0; i < vector->GetLength(); ++i) {
        if (((*vector)[i] & 1) != 1) {
            *isLoaded = FALSE;
            return NOERROR;
        }
    }
    *isLoaded = TRUE;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode MappedByteBuffer::Load()
{
    // try {
    AutoPtr<ILibcore> libcore;
    AutoPtr<IOs> os;
    assert(0 && "TODO");
    // ASSERT_SUCCEEDED(CLibcore::AcquireSingleton((ILibcore**)&libcore))
    libcore->GetOs((IOs**)&os);
    ASSERT_SUCCEEDED(os->Mlock(mBlock->ToInt64(), mBlock->GetSize()))
    ASSERT_SUCCEEDED(os->Munlock(mBlock->ToInt64(), mBlock->GetSize()))
    // } catch (ErrnoException ignored) {
    // }
    return NOERROR;
}

ECode MappedByteBuffer::Force()
{
    if (mMapMode == FileChannelMapMode_READ_WRITE) {
    //     try {
        AutoPtr<ILibcore> libcore;
        AutoPtr<IOs> os;
        assert(0 && "TODO");
        // ASSERT_SUCCEEDED(CLibcore::AcquireSingleton((ILibcore**)&libcore))
        libcore->GetOs((IOs**)&os);
        // AutoPtr<IOsConstants> osConstans;
        assert(0 && "TODO");
        // ASSERT_SUCCEEDED(COsConstants::AcquireSingleton((IOsConstants**)&osConstans))
        Int32 MS_SYNC = 0;
        assert(0 && "TODO");
        // FAIL_RETURN(osConstans->GetOsConstant(String("MS_SYNC"), &MS_SYNC))
        FAIL_RETURN(os->Msync(mBlock->ToInt64(), mBlock->GetSize(), MS_SYNC))
    //     } catch (ErrnoException errnoException) {
    //         // The RI doesn't throw, presumably on the assumption that you can't get into
    //         // a state where msync(2) could return an error.
    //         throw new AssertionError(errnoException);
    //     }
    }
    return NOERROR;
}

ECode MappedByteBuffer::CheckIsMapped()
{
    if (mMapMode == NULL) {
        // throw new UnsupportedOperationException();
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
