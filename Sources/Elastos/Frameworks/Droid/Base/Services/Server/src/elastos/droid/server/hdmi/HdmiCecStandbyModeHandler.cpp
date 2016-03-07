
#include "elastos/droid/server/hdmi/HdmiCecStandbyModeHandler.h"
#include "elastos/droid/server/hdmi/HdmiCecLocalDeviceTv.h"
#include <Elastos.Droid.Utility.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Hdmi {

//=============================================================================
// HdmiCecStandbyModeHandler::Bystander
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecStandbyModeHandler::Bystander, Object, IHdmiCecStandbyModeHandlerCecMessageHandler)

ECode HdmiCecStandbyModeHandler::Bystander::Handle(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                *result = TRUE;
                return NOERROR;

#endif
}

//=============================================================================
// HdmiCecStandbyModeHandler::Bypasser
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecStandbyModeHandler::Bypasser, Object, IHdmiCecStandbyModeHandlerCecMessageHandler)

ECode HdmiCecStandbyModeHandler::Bypasser::Handle(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                *result = TRUE;
                return NOERROR;

#endif
}

//=============================================================================
// HdmiCecStandbyModeHandler::Aborter
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecStandbyModeHandler::Aborter, Object, IHdmiCecStandbyModeHandlerCecMessageHandler)

HdmiCecStandbyModeHandler::Aborter::Aborter(
    /* [in] */ Int32 reason)
    : mReason(reason)
{}

ECode HdmiCecStandbyModeHandler::Aborter::Handle(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mService->MaySendFeatureAbortCommand(message, mReason);
                *result = TRUE;
                return NOERROR;

#endif
}

//=============================================================================
// HdmiCecStandbyModeHandler::AutoOnHandler
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecStandbyModeHandler::AutoOnHandler, Object, IHdmiCecStandbyModeHandlerCecMessageHandler)

ECode HdmiCecStandbyModeHandler::AutoOnHandler::Handle(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (!mTv->GetAutoWakeup()) {
                    mAborterRefused->Handle(message);
                    *result = TRUE;
                    return NOERROR;
                }
                *result = FALSE;
                return NOERROR;

#endif
}

//=============================================================================
// HdmiCecStandbyModeHandler::UserControlProcessedHandler
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecStandbyModeHandler::UserControlProcessedHandler, Object, IHdmiCecStandbyModeHandlerCecMessageHandler)

ECode HdmiCecStandbyModeHandler::UserControlProcessedHandler::Handle(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                // The power status here is always standby.
                if (HdmiCecLocalDevice::IsPowerOnOrToggleCommand(message)) {
                    *result = FALSE;
                    return NOERROR;
                } else if (HdmiCecLocalDevice::IsPowerOffOrToggleCommand(message)) {
                    *result = TRUE;
                    return NOERROR;
                }
                return mAborterIncorrectMode->Handle(message);

#endif
}

//=============================================================================
// HdmiCecStandbyModeHandler
//=============================================================================
HdmiCecStandbyModeHandler::HdmiCecStandbyModeHandler()
{
#if 0 // TODO: Translate codes below
    private SparseArray<CecMessageHandler> mCecMessageHandlers = new SparseArray<>();

    private CecMessageHandler mDefaultHandler = new Aborter(
            Constants::ABORT_UNRECOGNIZED_OPCODE);
    private CecMessageHandler mAborterIncorrectMode = new Aborter(
            Constants::ABORT_NOT_IN_CORRECT_MODE);
    private CecMessageHandler mAborterRefused = new Aborter(Constants::ABORT_REFUSED);
    private CecMessageHandler mAutoOnHandler = new AutoOnHandler();
    private CecMessageHandler mBypasser = new Bypasser();
    private CecMessageHandler mBystander = new Bystander();
    private UserControlProcessedHandler
            mUserControlProcessedHandler = new UserControlProcessedHandler();
#endif
}

ECode HdmiCecStandbyModeHandler::constructor(
    /* [in] */ IHdmiControlService* service,
    /* [in] */ HdmiCecLocalDeviceTv* tv)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mService = service;
        mTv = tv;
        AddHandler(Constants::MESSAGE_IMAGE_VIEW_ON, mAutoOnHandler);
        AddHandler(Constants::MESSAGE_TEXT_VIEW_ON, mAutoOnHandler);
        AddHandler(Constants::MESSAGE_ACTIVE_SOURCE, mBystander);
        AddHandler(Constants::MESSAGE_REQUEST_ACTIVE_SOURCE, mBystander);
        AddHandler(Constants::MESSAGE_ROUTING_CHANGE, mBystander);
        AddHandler(Constants::MESSAGE_ROUTING_INFORMATION, mBystander);
        AddHandler(Constants::MESSAGE_SET_STREAM_PATH, mBystander);
        AddHandler(Constants::MESSAGE_STANDBY, mBystander);
        AddHandler(Constants::MESSAGE_SET_MENU_LANGUAGE, mBystander);
        AddHandler(Constants::MESSAGE_DEVICE_VENDOR_ID, mBystander);
        AddHandler(Constants::MESSAGE_USER_CONTROL_RELEASED, mBystander);
        AddHandler(Constants::MESSAGE_REPORT_POWER_STATUS, mBystander);
        AddHandler(Constants::MESSAGE_FEATURE_ABORT, mBystander);
        AddHandler(Constants::MESSAGE_INACTIVE_SOURCE, mBystander);
        AddHandler(Constants::MESSAGE_SYSTEM_AUDIO_MODE_STATUS, mBystander);
        AddHandler(Constants::MESSAGE_REPORT_AUDIO_STATUS, mBystander);
        // If TV supports the following messages during power-on, ignore them and do nothing,
        // else reply with <Feature Abort>["Unrecognized Opcode"]
        // <Deck Status>, <Tuner Device Status>, <Tuner Cleared Status>, <Timer Status>
        AddHandler(Constants::MESSAGE_RECORD_STATUS, mBystander);
        // If TV supports the following messages during power-on, reply with <Feature Abort>["Not
        // in correct mode to respond"], else reply with <Feature Abort>["Unrecognized Opcode"]
        // <Give Tuner Device Status>, <Select Digital Service>, <Tuner Step Decrement>,
        // <Tuner Stem Increment>, <Menu Status>.
        AddHandler(Constants::MESSAGE_RECORD_TV_SCREEN, mAborterIncorrectMode);
        AddHandler(Constants::MESSAGE_INITIATE_ARC, mAborterIncorrectMode);
        AddHandler(Constants::MESSAGE_TERMINATE_ARC, mAborterIncorrectMode);
        AddHandler(Constants::MESSAGE_GIVE_PHYSICAL_ADDRESS, mBypasser);
        AddHandler(Constants::MESSAGE_GET_MENU_LANGUAGE, mBypasser);
        AddHandler(Constants::MESSAGE_REPORT_PHYSICAL_ADDRESS, mBypasser);
        AddHandler(Constants::MESSAGE_GIVE_DEVICE_VENDOR_ID, mBypasser);
        AddHandler(Constants::MESSAGE_GIVE_OSD_NAME, mBypasser);
        AddHandler(Constants::MESSAGE_SET_OSD_NAME, mBypasser);
        AddHandler(Constants::MESSAGE_USER_CONTROL_PRESSED, mUserControlProcessedHandler);
        AddHandler(Constants::MESSAGE_GIVE_DEVICE_POWER_STATUS, mBypasser);
        AddHandler(Constants::MESSAGE_ABORT, mBypasser);
        AddHandler(Constants::MESSAGE_GET_CEC_VERSION, mBypasser);
        AddHandler(Constants::MESSAGE_VENDOR_COMMAND_WITH_ID, mAborterIncorrectMode);
        AddHandler(Constants::MESSAGE_SET_SYSTEM_AUDIO_MODE, mAborterIncorrectMode);

#endif
}

ECode HdmiCecStandbyModeHandler::AddHandler(
    /* [in] */ Int32 opcode,
    /* [in] */ IHdmiCecStandbyModeHandlerCecMessageHandler* handler)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCecMessageHandlers->Put(opcode, handler);

#endif
}

ECode HdmiCecStandbyModeHandler::HandleCommand(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 opcode;
        message->GetOpcode(&opcode);
        AutoPtr<IInterface> obj;
        mCecMessageHandlers->Get(opcode, (IInterface**)&obj);
        CecMessageHandler handler = I::Probe(obj);
        if (handler != NULL) {
            return handler->Handle(message);
        }
        return mDefaultHandler->Handle(message);

#endif
}

} // namespace Hdmi
} // namespace Server
} // namespace Droid
} // namespace Elastos
