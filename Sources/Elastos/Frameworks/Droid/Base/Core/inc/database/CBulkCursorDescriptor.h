#ifndef __CSQLITEBULKCURSORDESCRIPTOR_H__
#define __CSQLITEBULKCURSORDESCRIPTOR_H__

#include "ext/frameworkext.h"
#include "_CBulkCursorDescriptor.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CBulkCursorDescriptor)
{
public:
    CBulkCursorDescriptor();

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* writeout);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* readin);

    CARAPI constructor();

public:
    AutoPtr<IBulkCursor> mCursor;
    AutoPtr< ArrayOf<String> > mColumnNames;
    Boolean mWantsAllOnMoveCalls;
    Int32 mCount;
    AutoPtr<ICursorWindow> mWindow;
};

} //Database
} //Droid
} //Elastos
#endif //__CSQLITEBULKCURSORDESCRIPTOR_H__
