
#include "CDeleteOnExit.h"
#include <cmdef.h>

using Elastos::Core::ICharSequence;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Core::CStringWrapper;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Luni {
namespace Util {

AutoPtr<IDeleteOnExit> CDeleteOnExit::sInstance;
Mutex CDeleteOnExit::sLockLock;

ECode CDeleteOnExit::GetInstance(
    /* [out] */ IDeleteOnExit** instance)
{
    VALIDATE_NOT_NULL(instance)
    Mutex::Autolock lock(sLockLock);
    if (NULL == sInstance) {
        sInstance = new CDeleteOnExit();
        //Runtime.getRuntime().addShutdownHook(instance);
    }
    *instance = sInstance;
    return NOERROR;
}

ECode CDeleteOnExit::AddFile(
    /* [in] */ const String& filename)
{
    Mutex::Autolock lock(mLockLock);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(filename, (ICharSequence**)&cs);
    Boolean isContained;
    mFiles->Contains(cs.Get(), &isContained);
    if (!isContained) {
        mFiles->Add(cs.Get(), &isContained);
    }
    return NOERROR;
}

CDeleteOnExit::CDeleteOnExit()
{
    CArrayList::New((IArrayList**)&mFiles);
}

ECode CDeleteOnExit::Run()
{
    AutoPtr<ICollections> col;
    CCollections::AcquireSingleton((ICollections**)&col);
    col->Sort(mFiles.Get());
    Int32 size;
    mFiles->GetSize(&size);
    for (Int32 i = size - 1; i >= 0; i--) {
        AutoPtr<IInterface> elem;
        mFiles->Get(i, (IInterface**)&elem);
        String fileName;
        ICharSequence::Probe(elem)->ToString(&fileName);
        AutoPtr<IFile> file;
        CFile::New(fileName, (IFile**)&file);
        Boolean isDeleted;
        FAIL_RETURN(file->Delete(&isDeleted))
    }
    return NOERROR;
}

PInterface CDeleteOnExit::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IDeleteOnExit *)this;
    }
    else if ( riid == EIID_IDeleteOnExit) {
        return (IDeleteOnExit *)this;
    }
    else if ( riid == EIID_IThread ) {
        return (IThread *)(ThreadBase *)this;
    }
    return NULL;
}

UInt32 CDeleteOnExit::AddRef()
{
    return _CDeleteOnExit::AddRef();
}

UInt32 CDeleteOnExit::Release()
{
    return _CDeleteOnExit::Release();
}

ECode CDeleteOnExit::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid)
    if (object == (IInterface*)(IDeleteOnExit*)this) {
        *iid = EIID_IDeleteOnExit;
    }
    else if (object == (IInterface*)(IThread*)this) {
        *iid = EIID_IThread;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

} // namespace Util
} // namespace Luni
} // namespace Harmony
} // namespace Apache
} // namespace Org

