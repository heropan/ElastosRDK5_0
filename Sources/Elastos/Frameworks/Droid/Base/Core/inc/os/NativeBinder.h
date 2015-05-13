#ifndef __NATIVEBINDER_H__
#define __NATIVEBINDER_H__

#include "Elastos.Droid.Core_server.h"
#include <binder/IBinder.h>

namespace Elastos {
namespace Droid {
namespace Os {

extern AutoPtr<IBinder> DroidObjectForIBinder(const android::sp<android::IBinder>& val);

extern android::sp<android::IBinder> IBinderForDroidObject(IBinder* obj);

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__NATIVEBINDER_H__
