
#ifndef __CINTENTFILTERCOMPARISON_H__
#define __CINTENTFILTERCOMPARISON_H__

#include "_CIntentFilterComparison.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CIntentFilterComparison)
{
public:
    CIntentFilterComparison();

    ~CIntentFilterComparison();

    /**
     * Return the Intent that this FilterComparison represents.
     * @return Returns the Intent held by the FilterComparison.  Do
     * not modify!
     */
    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI constructor(
        /* [in] */ IIntent* intent);

private:
    AutoPtr<IIntent> mIntent;
    Int32 mHashCode;

};

}
}
}

#endif // __CINTENTFILTERCOMPARISON_H__
