
#include "droid/test/ClassPathPackageInfo.h"

using Elastos::Core::ICharSequence;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::CHashSet;
using Elastos::Utility::IIterator;

namespace Elastos {
namespace Droid {
namespace Test {

ClassPathPackageInfo::ClassPathPackageInfo(
    /* [in] */ ClassPathPackageInfoSource* source,
    /* [in] */ const String& packageName,
    /* [in] */ ISet* subpackageNames,
    /* [in] */ ISet* topLevelClasses)
    : mSource(source)
    , mPackageName(packageName)
{
    AutoPtr<ICollections> collections;
    CCollections::AcquireSingleton((ICollections**)&collections);
    collections->NewUnmodifiableSet(subpackageNames, (ISet**)&mSubpackageNames);
    collections->NewUnmodifiableSet(topLevelClasses, (ISet**)&mTopLevelClasses);
}

CAR_INTERFACE_IMPL(ClassPathPackageInfo, IInterface)

AutoPtr<ISet> ClassPathPackageInfo::GetSubpackages()
{
    AutoPtr<ISet> info;
    CHashSet::New((ISet**)&info);
    AutoPtr<IIterator> it;
    mSubpackageNames->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        it->Next((IInterface**)&obj);
        String name;
        ICharSequence::Probe(obj)->ToString(&name);
        AutoPtr<ClassPathPackageInfo> cppi = mSource->GetPackageInfo(name);
        Boolean result;
        info->Add((IInterface*)cppi.Get(), &result);
    }
    return info;
}

AutoPtr<ISet> ClassPathPackageInfo::GetTopLevelClassesRecursive()
{
    AutoPtr<ISet> set;
    CHashSet::New((ISet**)&set);
    AddTopLevelClassesTo(set);
    return set;
}

void ClassPathPackageInfo::AddTopLevelClassesTo(
    /* [in] */ ISet* set)
{
    Boolean result;
    set->AddAll(mTopLevelClasses, &result);
    AutoPtr<ISet> infos =  GetSubpackages();
    AutoPtr<IIterator> it;
    infos->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        it->Next((IInterface**)&obj);
        ClassPathPackageInfo* info = (ClassPathPackageInfo*)obj.Get();
        info->AddTopLevelClassesTo(set);
    }
}

} // namespace Test
} // namespace Droid
} // namespace Elastos
