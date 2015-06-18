
#ifndef __ABSTRACTDEVICEFACTORY_H__
#define __ABSTRACTDEVICEFACTORY_H__

#include <ext/frameworkext.h>
#include <elrefbase.h>

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Remote {

class AbstractDeviceFactory : public ElRefBase
{
public:
    AbstractDeviceFactory(
        /* [in] */ IContext* ctx);

protected:
    virtual CARAPI RunForResult(
        /* [in] */ String cmd,
        /* [out] */ String* result) = 0;

private:
    static const String TAG;
    static const Boolean DBG;

protected:
    AutoPtr<IContext> mContext;
};

} // Remote
} // Server
} // Droid
} // Elastos

#endif //__ABSTRACTDEVICEFACTORY_H__
