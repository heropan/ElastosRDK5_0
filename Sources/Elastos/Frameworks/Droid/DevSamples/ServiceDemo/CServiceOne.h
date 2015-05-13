
#ifndef __CSERVICEONE_H__
#define __CSERVICEONE_H__

#include "_CServiceOne.h"
#include "app/Service.h"

using Elastos::Droid::App::Service;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace ServiceDemo {

class CServiceOne : public Elastos::Droid::App::Service
{
public:

protected:
    CARAPI OnCreate();

    CARAPI OnStart(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 startId);

    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

    CARAPI OnUnbind(
        /* [in] */ IIntent* intent,
        /* [out] */ Boolean* succeeded);

private:
    // TODO: Add your private member variables here.
};

} // namespace ServiceDemo
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif // __CSERVICEONE_H__
