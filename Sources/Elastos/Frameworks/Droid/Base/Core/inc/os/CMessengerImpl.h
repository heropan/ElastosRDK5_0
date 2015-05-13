
#ifndef __CMESSENGERIMPL_H__
#define __CMESSENGERIMPL_H__

#include "_CMessengerImpl.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CMessengerImpl)
{
public:
    CARAPI constructor(
        /* [in] */ IHandler* handler);

    CARAPI Send(
        /* [in] */ IMessage* message);

private:
    IHandler* mOwner;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__CMESSENGERIMPL_H__
