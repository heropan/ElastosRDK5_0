
#include "DeleteOnExit.h"
#include "CStringWrapper.h"
#include "Autolock.h"

using Elastos::IO::IFile;
// using Elastos::IO::CFile;
using Elastos::Utility::IList;
using Elastos::Utility::ICollections;
// using Elastos::Utility::CCollections;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;

namespace Libcore {
namespace IO {

CAR_INTERFACE_IMPL(DeleteOnExit, Thread, IDeleteOnExit)

AutoPtr<IDeleteOnExit> DeleteOnExit::sInstance;
Object DeleteOnExit::sLockLock;

ECode DeleteOnExit::GetInstance(
    /* [out] */ IDeleteOnExit** instance)
{
    VALIDATE_NOT_NULL(instance)
    Autolock lock(sLockLock);
    if (NULL == sInstance) {
        sInstance = new DeleteOnExit();
    }
    *instance = sInstance;
    REFCOUNT_ADD(*instance)
    return NOERROR;
}

ECode DeleteOnExit::AddFile(
    /* [in] */ const String& filename)
{
    Autolock lock(this);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(filename, (ICharSequence**)&cs);
    Boolean isContained;
    AutoPtr<ICollection> collection = ICollection::Probe(mFiles);
    collection->Contains(cs, &isContained);
    if (!isContained) {
        collection->Add(cs, &isContained);
    }
    return NOERROR;
}

DeleteOnExit::DeleteOnExit()
{
    CArrayList::New((IArrayList**)&mFiles);
}

ECode DeleteOnExit::Run()
{
    AutoPtr<ICollections> col;
    // CCollections::AcquireSingleton((ICollections**)&col);
    AutoPtr<IList> list = IList::Probe(mFiles);
    col->Sort(list);
    Int32 size;
    AutoPtr<ICollection> collection = ICollection::Probe(mFiles);
    collection->GetSize(&size);
    for (Int32 i = size - 1; i >= 0; i--) {
        AutoPtr<IInterface> elem;
        list->Get(i, (IInterface**)&elem);
        String fileName;
        ICharSequence::Probe(elem)->ToString(&fileName);
        AutoPtr<IFile> file;
        // CFile::New(fileName, (IFile**)&file);
        Boolean isDeleted;
        FAIL_RETURN(file->Delete(&isDeleted))
    }
    return NOERROR;
}

} // namespace IO
} // namespace Libcore


