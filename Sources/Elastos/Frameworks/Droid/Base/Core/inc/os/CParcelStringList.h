#ifndef __ELASTOS_DROID_OS_CPARCELSTRINGLIST_H__
#define __ELASTOS_DROID_OS_CPARCELSTRINGLIST_H__

#include "_CParcelStringList.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CParcelStringList)
{
public:
    CARAPI constructor(
        /* [in] */ ArrayOf<String>* list);

    CARAPI GetStringList(
        /* [out, callee] */ ArrayOf<String>** list);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    AutoPtr<ArrayOf<String> > mStringList;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CPARCELSTRINGLIST_H__
