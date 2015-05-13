#ifndef __CEVENTLOGTAGSDESCRIPTION_H__
#define __CEVENTLOGTAGSDESCRIPTION_H__

#include "_CEventLogTagsDescription.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CEventLogTagsDescription)
{
public:
    CEventLogTagsDescription();

    ~CEventLogTagsDescription();

    CARAPI constructor(
        /* [in] */ Int32 * tag,
        /* [in] */ const String& * name);

public:
    Int32 mTag;
    String mName;
};

}
}
}

#endif // __CEVENTLOGTAGSDESCRIPTION_H__
