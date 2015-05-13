
#include "droid/test/EcoFile.h"
#include <elastos/Slogger.h>

using Elastos::Core::EIID_IComparable;
using Elastos::Utility::EIID_IEnumeration;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Test {

EcoFile::ClassInfo::ClassInfo(
    /* [in] */ IClassInfo* classInfo);

PInterface EcoFile::ClassInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IComparable *)this;
    }
    else if (riid == EIID_IComparable) {
        return (IComparable *)this;
    }
    else if (riid == EIID_IClassInfo) {
        return mClassInfo.Get();
    }
    return NULL;
}

UInt32 EcoFile::ClassInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 EcoFile::ClassInfo::Release()
{
    return ElLightRefBase::Release();
}

ECode EcoFile::ClassInfo::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IComparable *)this) {
        *iid = EIID_IComparable;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode EcoFile::ClassInfo::CompareTo(
    /* [in] */ IInterface* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    AutoPtr<ClassInfo> res = (ClassInfo*)another;
    if (res == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = !(this == res);
    return NOERROR;
}


EcoFile::EFEnum::EFEnum(
    /* [in] */ EcoFile* ef)
    : mIndex(0)
{
    mClassInfoList = EcoFile::GetClassInfoList(ef->mEcoModule);
}

CAR_INTERFACE_IMPL_LIGHT(EcoFile::EFEnum, IEnumeration)

ECode EcoFile::EFEnum::HasMoreElements(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIndex < mClassInfoList->GetLength();
    return NOERROR;
}

ECode EcoFile::EFEnum::NextElement(
    /* [out] */ IInterface** element)
{
    VALIDATE_NOT_NULL(element);
    *element = (*mClassInfoList)[mIndex++];
    INTERFACE_ADDREF(*element);
    return NOERROR;
}


const String EcoFile::TAG("EcoFile");

EcoFile::EcoFile()
{}

EcoFile::~EcoFile()
{
    Close();
}

ECode EcoFile::NewEcoFile(
    /* [in] */ const String& fileName,
    /* [out] */ EcoFile** ecoFile)
{
    AutoPtr<EcoFile> eco = new EcoFile();
    ECode ec = eco->Init(fileName);
    if (FAILED(ec)) {
        *ecoFile = NULL;
        return ec;
    }
    *ecoFile = eco;
    INTERFACE_ADDREF(*ecoFile);
    return NOERROR;
}

AutoPtr<IEnumeration> EcoFile::Entries()
{
    AutoPtr<IEnumeration> entries = new EFEnum(this);
    return entries;
}

ECode EcoFile::Close()
{
    //TODO:
    return NOERROR;
}

ECode EcoFile::Init(
    /* [in] */ const String& fileName)
{
    FAIL_RETURN(OpenEcoFile(fileName, (IModuleInfo**)&mEcoModule));
    mFileName = fileName;
    return NOERROR;
}

AutoPtr< ArrayOf<EcoFile::ClassInfo*> > EcoFile::GetClassInfoList(
    /* [in] */ IModuleInfo* ecoModule)
{
    AutoPtr< ArrayOf<ClassInfo*> > classInfoList;
    Int32 count;
    ecoModule->GetClassCount(&count);
    if (count <= 0) {
        classInfoList = ArrayOf<ClassInfo*>::Alloc(0);
        return classInfoList;
    }

    BufferOf<IClassInfo*>* classInfos = BufferOf<IClassInfo*>::Alloc(count);
    ecoModule->GetAllClassInfos(classInfos);
    classInfoList = ArrayOf<ClassInfo*>::Alloc(count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<ClassInfo> classInfo = new ClassInfo((*classInfos)[i]);
        classInfoList->Set(i, classInfo.Get());
        (*classInfos)[i]->Release();
        (*classInfos)[i] = NULL;
    }
    BufferOf<IClassInfo*>::Free(classInfos);
    return classInfoList;
}

ECode EcoFile::OpenEcoFile(
    /* [in] */ const String& sourceName,
    /* [out] */ IModuleInfo** ecoModule)
{
    String fileName = sourceName;
    Int32 start = fileName.LastIndexOf("/");
    Int32 end = fileName.LastIndexOf("-");
    fileName = fileName.Substring(start >= 0 ? start + 1 : 0, end >= 0 ? end : fileName.GetLength() - 4)  + String(".eco");
    String path = String("/data/elastos/") + fileName;
    AutoPtr<IModuleInfo> moduleInfo;
    if (FAILED(CReflector::AcquireModuleInfo(path.string(), (IModuleInfo**)&moduleInfo))) {
        *ecoModule = NULL;
        Slogger::E(TAG, "OpenEcoFile: Cann't Find the Instrumentation path is %s", path.string());
        return E_RUNTIME_EXCEPTION;
    }
    *ecoModule = moduleInfo;
    INTERFACE_ADDREF(*ecoModule);
    return NOERROR;
}

} // namespace Test
} // namespace Droid
} // namespace Elastos
