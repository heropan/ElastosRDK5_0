
#include "elastos/droid/location/SettingInjectorService.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/CMessage.h"
#include "elastos/droid/os/CMessageHelper.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::App::IService;
using Elastos::Droid::Os::CMessageHelper;
using Elastos::Droid::Os::IMessageHelper;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CMessage;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IMessenger;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Location {

const String SettingInjectorService::TAG("SettingInjectorService");

// CAR_INTERFACE_IMPL(SettingInjectorService, Service, ISettingInjectorService)
CAR_INTERFACE_IMPL(SettingInjectorService, Object, ISettingInjectorService)

SettingInjectorService::SettingInjectorService(
    /* [in] */ const String& name)
    : mName(name)
{
}

ECode SettingInjectorService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder)
    *binder = NULL;
    return NOERROR;
}

ECode SettingInjectorService::OnStart(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    assert(0);
    // Service::OnStart(intent, startId);
    return NOERROR;
}

ECode SettingInjectorService::OnStartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    OnHandleIntent(intent);
    assert(0);
    // Service::StopSelf(startId);
    *result = IService::START_NOT_STICKY;
    return NOERROR;
}

ECode SettingInjectorService::OnHandleIntent(
    /* [in] */ IIntent* intent)
{
    Boolean enabled = FALSE;
    ECode ec = OnGetEnabled(&enabled);
    if (FAILED(ec)) {
        SendStatus(intent, TRUE);
        return E_RUNTIME_EXCEPTION;
    }
    SendStatus(intent, enabled);
    return NOERROR;
}

ECode SettingInjectorService::SendStatus(
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean enabled)
{
    AutoPtr<IMessage> message = CMessage::Obtain();
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    bundle->PutBoolean(ISettingInjectorService::ENABLED_KEY, enabled);
    message->SetData(bundle);

    if (Logger::IsLoggable(TAG, Logger::___DEBUG)) {
         #if 0
         Log.d(TAG, mName + ": received " + intent
                 + ", enabled=" + enabled + ", sending message: " + message);
        #endif
    }

    AutoPtr<IParcelable> par;
    intent->GetParcelableExtra(ISettingInjectorService::MESSENGER_KEY, (IParcelable**)&par);
    AutoPtr<IMessenger> messenger = IMessenger::Probe(par);
    ECode ec = messenger->Send(message);
    if (FAILED(ec)) {
        StringBuilder sb(mName);
        sb += ": sending dynamic status failed";
        Logger::E(TAG, sb.ToString());
    }
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
