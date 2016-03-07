
#include "elastos/droid/server/hdmi/ActiveSourceHandler.h"
#include "elastos/droid/server/hdmi/HdmiCecLocalDeviceTv.h"
#include "elastos/droid/server/hdmi/HdmiCecMessageBuilder.h"
#include "elastos/droid/server/hdmi/HdmiControlService.h"
#include "elastos/droid/server/hdmi/RoutingControlAction.h"
#include <elastos/droid/net/ReturnOutValue.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Hardware::Hdmi::IHdmiControlManager;
using Elastos::Droid::Hardware::Hdmi::IHdmiDeviceInfo;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Hdmi {

const String ActiveSourceHandler::TAG("ActiveSourceHandler");

ECode ActiveSourceHandler::Create(
    /* [in] */ HdmiCecLocalDeviceTv* source,
    /* [in] */ IIHdmiControlCallback* callback,
    /* [out] */ ActiveSourceHandler** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    if (source == NULL) {
        Slogger::E(TAG, "Wrong arguments");
        *result = NULL;
        return NOERROR;
    }
    *result = new ActiveSourceHandler(source, callback);
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ActiveSourceHandler::ActiveSourceHandler(
    /* [in] */ HdmiCecLocalDeviceTv* source,
    /* [in] */ IIHdmiControlCallback* callback)
    : mSource(source)
    , mCallback(callback)
{
    mSource->GetService((IHdmiControlService**)&mService);
}

ECode ActiveSourceHandler::Process(
    /* [in] */ IHdmiCecLocalDeviceActiveSource* newActive)
{
    // Seq #17
    AutoPtr<HdmiCecLocalDeviceTv> tv = mSource;
    AutoPtr<IHdmiCecLocalDeviceActiveSource> activeSource;
    tv->GetActiveSource((IHdmiCecLocalDeviceActiveSource**)&activeSource);
    Boolean isEquals;
    IObject::Probe(activeSource)->Equals(newActive, &isEquals);
    if (isEquals) {
        InvokeCallback(IHdmiControlManager::RESULT_SUCCESS);
        return NOERROR;
    }
    AutoPtr<IHdmiDeviceInfo> device;
    ((HdmiControlService*)mService.Get())->GetDeviceInfo(((HdmiCecLocalDevice::ActiveSource*)newActive)->mLogicalAddress, (IHdmiDeviceInfo**)&device);
    if (device == NULL) {
        tv->StartNewDeviceAction(newActive);
    }
    if (!Ptr(tv)->Func(tv->IsProhibitMode)) {
        tv->UpdateActiveSource(newActive);
        Boolean notifyInputChange = (mCallback == NULL);
        tv->UpdateActiveInput(((HdmiCecLocalDevice::ActiveSource*)newActive)->mPhysicalAddress, notifyInputChange);
        InvokeCallback(IHdmiControlManager::RESULT_SUCCESS);
    } else {
        // TV is in a mode that should keep its current source/input from
        // being changed for its operation. Reclaim the active source
        // or switch the port back to the one used for the current mode.
        AutoPtr<IHdmiCecLocalDeviceActiveSource> current;
        tv->GetActiveSource((IHdmiCecLocalDeviceActiveSource**)&current);
        if (((HdmiCecLocalDevice::ActiveSource*)current.Get())->mLogicalAddress == Ptr(this)->Func(this->GetSourceAddress)) {
            AutoPtr<IHdmiCecMessage> activeSourceCommand;
            HdmiCecMessageBuilder::BuildActiveSource(
                    ((HdmiCecLocalDevice::ActiveSource*)current.Get())->mLogicalAddress, ((HdmiCecLocalDevice::ActiveSource*)current.Get())->mPhysicalAddress, (IHdmiCecMessage**)&activeSourceCommand);
            ((HdmiControlService*)mService.Get())->SendCecCommand(activeSourceCommand);
            tv->UpdateActiveSource(current);
            InvokeCallback(IHdmiControlManager::RESULT_SUCCESS);
        } else {
            AutoPtr<IHdmiCecMessage> routingChange;
            Int32 address;
            GetSourceAddress(&address);
            HdmiCecMessageBuilder::BuildRoutingChange(
                    address, ((HdmiCecLocalDevice::ActiveSource*)newActive)->mPhysicalAddress, ((HdmiCecLocalDevice::ActiveSource*)current.Get())->mPhysicalAddress, (IHdmiCecMessage**)&routingChange);
            ((HdmiControlService*)mService.Get())->SendCecCommand(routingChange);
            AutoPtr<RoutingControlAction> action = new RoutingControlAction();
            action->constructor(tv, ((HdmiCecLocalDevice::ActiveSource*)current.Get())->mPhysicalAddress, TRUE, mCallback);
            tv->AddAndStartAction(action.Get());
        }
    }
    return NOERROR;
}

ECode ActiveSourceHandler::GetSourceAddress(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IHdmiDeviceInfo> info;
    mSource->GetDeviceInfo((IHdmiDeviceInfo**)&info);
    return info->GetLogicalAddress(result);
}

ECode ActiveSourceHandler::InvokeCallback(
    /* [in] */ Int32 result)
{
    if (mCallback == NULL) {
        return NOERROR;
    }
    // try {
    ECode ec = mCallback->OnComplete(result);
    // } catch (RemoteException e) {
    if (FAILED(ec)) {
        if ((ECode) E_REMOTE_EXCEPTION == ec) {
            Slogger::E(TAG, "Callback failed: %d", ec);
        }
        else
            return ec;
    }
    // }
    return NOERROR;
}

} // namespace Hdmi
} // namespace Server
} // namespace Droid
} // namespace Elastos
