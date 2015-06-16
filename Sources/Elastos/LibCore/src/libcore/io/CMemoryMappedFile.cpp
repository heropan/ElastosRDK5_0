#include "CMemoryMappedFile.h"
#include "CLibcore.h"
#include "droid/system/OsConstants.h"
#include "nio/CByteOrderHelper.h"
#include "NioBufferIterator.h"
#include "CLibcore.h"

using Elastos::IO::CByteOrderHelper;
using Elastos::IO::IByteOrderHelper;
using Elastos::IO::ByteOrder;
using Elastos::IO::ByteOrder_BIG_ENDIAN;
using Elastos::IO::ByteOrder_LITTLE_ENDIAN;
using Elastos::IO::EIID_ICloseable;
using Elastos::Droid::System::OsConstants;
using Elastos::Droid::System::IStructStat;
using Elastos::IO::IFileDescriptor;

namespace Libcore {
namespace IO {

CAR_OBJECT_IMPL(CMemoryMappedFile)

CAR_INTERFACE_IMPL_2(CMemoryMappedFile, Object, IMemoryMappedFile, ICloseable)

ECode CMemoryMappedFile::constructor(
    /* [in] */ Int64 address,
    /* [in] */ Int64 size)
{
    mAddress = address;
    mSize = size;
    return NOERROR;
}

/**
 * Returns a new iterator that treats the mapped data as big-endian.
 */
ECode CMemoryMappedFile::BigEndianIterator(
    /* [out] */ IBufferIterator** it)
{
    AutoPtr<IByteOrderHelper> helper;
    CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper);
    ByteOrder order;
    helper->GetNativeOrder(&order);
    *it = new NioBufferIterator(mAddress, (Int32) mSize, order != ByteOrder_BIG_ENDIAN);
    REFCOUNT_ADD(*it)
    return NOERROR;
}

/**
 * Returns a new iterator that treats the mapped data as little-endian.
 */
ECode CMemoryMappedFile::LittleEndianIterator(
    /* [out] */ IBufferIterator** it)
{
    AutoPtr<IByteOrderHelper> helper;
    CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper);
    ByteOrder order;
    helper->GetNativeOrder(&order);
    *it = new NioBufferIterator(mAddress, (Int32) mSize, order != ByteOrder_LITTLE_ENDIAN);
    REFCOUNT_ADD(*it)
    return NOERROR;
}

/**
 * Returns the size in bytes of the memory-mapped region.
 */
ECode CMemoryMappedFile::Size(
    /* [out] */ Int64* size)
{
    return NOERROR;
}

ECode CMemoryMappedFile::Close()
{
    Autolock lock(this);
    if (mAddress != 0) {
        CLibcore::sOs->Munmap(mAddress, mSize);
        mAddress = 0;
    }
    return NOERROR;
}

AutoPtr<IMemoryMappedFile> CMemoryMappedFile::MmapRO(
    /* [in] */ String path)
{
    AutoPtr<IFileDescriptor> fd;
    AutoPtr<IOs> os = CLibcore::sOs;
    os->Open(path, OsConstants::_O_RDONLY, 0, (IFileDescriptor**)&fd);
    AutoPtr<IStructStat> structState;
    os->Fstat(fd, (IStructStat**)&structState);
    Int64 size;
    structState->GetSize(&size);
    Int64 address;
    os->Mmap(0L, size, OsConstants::_PROT_READ, OsConstants::_MAP_SHARED, fd, 0, &address);
    os->Close(fd);
    AutoPtr<IMemoryMappedFile> rst;
    New(address, size, (IMemoryMappedFile**)&rst);
    return rst;
}

} // namespace IO
} // namespace Libcore