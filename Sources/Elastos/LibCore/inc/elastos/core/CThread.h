
#ifndef __CTHREAD_H__
#define __CTHREAD_H__

#include "_Elastos_Core_CThread.h"
#include "Thread.h"

namespace Elastos {
namespace Core {

CarClass(CThread)
    , public Thread
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Core
} // namespace Elastos

#endif //__CTHREAD_H__
