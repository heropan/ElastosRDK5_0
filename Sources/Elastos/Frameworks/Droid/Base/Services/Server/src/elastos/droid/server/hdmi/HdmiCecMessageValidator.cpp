
#include "elastos/droid/server/hdmi/HdmiCecMessageValidator.h"
#include <Elastos.Droid.Utility.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Hdmi {

//=============================================================================
// HdmiCecMessageValidator::ValidationInfo
//=============================================================================
HdmiCecMessageValidator::ValidationInfo::ValidationInfo(
    /* [in] */ IHdmiCecMessageValidatorParameterValidator* validator,
    /* [in] */ Int32 type)
    : mParameterValidator(validator)
    , mAddressType(type)
{}

//=============================================================================
// HdmiCecMessageValidator::FixedLengthValidator
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecMessageValidator::FixedLengthValidator, Object, IHdmiCecMessageValidatorParameterValidator)

HdmiCecMessageValidator::FixedLengthValidator::FixedLengthValidator(
    /* [in] */ Int32 length)
    : mLength(length)
{}

ECode HdmiCecMessageValidator::FixedLengthValidator::IsValid(
    /* [in] */ ArrayOf<Byte>* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return params->GetLength() == mLength;

#endif
}

//=============================================================================
// HdmiCecMessageValidator::VariableLengthValidator
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecMessageValidator::VariableLengthValidator, Object, IHdmiCecMessageValidatorParameterValidator)

HdmiCecMessageValidator::VariableLengthValidator::VariableLengthValidator(
    /* [in] */ Int32 minLength,
    /* [in] */ Int32 maxLength)
    : mMinLength(minLength)
    , mMaxLength(maxLength)
{}

ECode HdmiCecMessageValidator::VariableLengthValidator::IsValid(
    /* [in] */ ArrayOf<Byte>* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                return params->GetLength() >= mMinLength && params->GetLength() <= mMaxLength;

#endif
}

//=============================================================================
// HdmiCecMessageValidator::PhysicalAddressValidator
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecMessageValidator::PhysicalAddressValidator, Object, IHdmiCecMessageValidatorParameterValidator)

ECode HdmiCecMessageValidator::PhysicalAddressValidator::IsValid(
    /* [in] */ ArrayOf<Byte>* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (params->GetLength() != 2) {
                    *result = FALSE;
                    return NOERROR;
                }
                return IsValidPhysicalAddress(params, 0);

#endif
}

//=============================================================================
// HdmiCecMessageValidator::ReportPhysicalAddressValidator
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecMessageValidator::ReportPhysicalAddressValidator, Object, IHdmiCecMessageValidatorParameterValidator)

ECode HdmiCecMessageValidator::ReportPhysicalAddressValidator::IsValid(
    /* [in] */ ArrayOf<Byte>* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (params->GetLength() != 3) {
                    *result = FALSE;
                    return NOERROR;
                }
                return IsValidPhysicalAddress(params, 0) && IsValidType((*params)[2]);

#endif
}

//=============================================================================
// HdmiCecMessageValidator::RoutingChangeValidator
//=============================================================================
CAR_INTERFACE_IMPL(HdmiCecMessageValidator::RoutingChangeValidator, Object, IHdmiCecMessageValidatorParameterValidator)

ECode HdmiCecMessageValidator::RoutingChangeValidator::IsValid(
    /* [in] */ ArrayOf<Byte>* params,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                if (params->GetLength() != 4) {
                    *result = FALSE;
                    return NOERROR;
                }
                return IsValidPhysicalAddress(params, 0) && IsValidPhysicalAddress(params, 2);

#endif
}

//=============================================================================
// HdmiCecMessageValidator
//=============================================================================
const String HdmiCecMessageValidator::TAG("HdmiCecMessageValidator");
const Int32 HdmiCecMessageValidator::OK = 0;
const Int32 HdmiCecMessageValidator::ERROR_SOURCE = 1;
const Int32 HdmiCecMessageValidator::ERROR_DESTINATION = 2;
const Int32 HdmiCecMessageValidator::ERROR_PARAMETER = 3;
const Int32 HdmiCecMessageValidator::DEST_DIRECT = 1 << 0;
const Int32 HdmiCecMessageValidator::DEST_BROADCAST = 1 << 1;
const Int32 HdmiCecMessageValidator::DEST_ALL = DEST_DIRECT | DEST_BROADCAST;
const Int32 HdmiCecMessageValidator::SRC_UNREGISTERED = 1 << 2;

HdmiCecMessageValidator::HdmiCecMessageValidator()
{
#if 0 // TODO: Translate codes below
    mValidationInfo = new SparseArray<>();
#endif
}

ECode HdmiCecMessageValidator::constructor(
    /* [in] */ IHdmiControlService* service)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mService = service;
        // Messages related to the physical address.
        PhysicalAddressValidator physicalAddressValidator = new PhysicalAddressValidator();
        AddValidationInfo(Constants::MESSAGE_ACTIVE_SOURCE,
                physicalAddressValidator, DEST_BROADCAST | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_INACTIVE_SOURCE, physicalAddressValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REPORT_PHYSICAL_ADDRESS,
                new ReportPhysicalAddressValidator(), DEST_BROADCAST | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_ROUTING_CHANGE,
                new RoutingChangeValidator(), DEST_BROADCAST | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_ROUTING_INFORMATION,
                physicalAddressValidator, DEST_BROADCAST | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_SET_STREAM_PATH,
                physicalAddressValidator, DEST_BROADCAST);
        AddValidationInfo(Constants::MESSAGE_SYSTEM_AUDIO_MODE_REQUEST,
                physicalAddressValidator, DEST_DIRECT);
        // Messages have no parameter.
        FixedLengthValidator noneValidator = new FixedLengthValidator(0);
        AddValidationInfo(Constants::MESSAGE_ABORT, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_GET_CEC_VERSION, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_GET_MENU_LANGUAGE,
                noneValidator, DEST_DIRECT | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_GIVE_AUDIO_STATUS, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_GIVE_DEVICE_POWER_STATUS, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_GIVE_DEVICE_VENDOR_ID,
                noneValidator, DEST_DIRECT | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_GIVE_OSD_NAME, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_GIVE_PHYSICAL_ADDRESS,
                noneValidator, DEST_DIRECT | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_GIVE_SYSTEM_AUDIO_MODE_STATUS,
                noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_IMAGE_VIEW_ON, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_INITIATE_ARC, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_RECORD_OFF, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_RECORD_TV_SCREEN, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REPORT_ARC_INITIATED, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REPORT_ARC_TERMINATED, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REQUEST_ARC_INITIATION, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REQUEST_ARC_TERMINATION, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REQUEST_ACTIVE_SOURCE,
                noneValidator, DEST_BROADCAST | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_STANDBY, noneValidator, DEST_ALL | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_TERMINATE_ARC, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_TEXT_VIEW_ON, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_TUNER_STEP_DECREMENT, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_TUNER_STEP_INCREMENT, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_USER_CONTROL_RELEASED, noneValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_VENDOR_REMOTE_BUTTON_UP, noneValidator, DEST_ALL);
        // TODO: Validate more than length for the following messages.
        // Messages for the One Touch Record.
        FixedLengthValidator oneByteValidator = new FixedLengthValidator(1);
        AddValidationInfo(Constants::MESSAGE_RECORD_ON,
                new VariableLengthValidator(1, 8), DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_RECORD_STATUS, oneByteValidator, DEST_DIRECT);
        // TODO: Handle messages for the Timer Programming.
        // Messages for the System Information.
        AddValidationInfo(Constants::MESSAGE_CEC_VERSION, oneByteValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_SET_MENU_LANGUAGE,
                new FixedLengthValidator(3), DEST_BROADCAST);
        // TODO: Handle messages for the Deck Control.
        // TODO: Handle messages for the Tuner Control.
        // Messages for the Vendor Specific Commands.
        VariableLengthValidator maxLengthValidator = new VariableLengthValidator(0, 14);
        AddValidationInfo(Constants::MESSAGE_DEVICE_VENDOR_ID,
                new FixedLengthValidator(3), DEST_BROADCAST);
        // Allow unregistered source for all vendor specific commands, because we don't know
        // how to use the commands at this moment.
        AddValidationInfo(Constants::MESSAGE_VENDOR_COMMAND,
                maxLengthValidator, DEST_DIRECT | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_VENDOR_COMMAND_WITH_ID,
                maxLengthValidator, DEST_ALL | SRC_UNREGISTERED);
        AddValidationInfo(Constants::MESSAGE_VENDOR_REMOTE_BUTTON_DOWN,
                maxLengthValidator, DEST_ALL | SRC_UNREGISTERED);
        // Messages for the OSD.
        AddValidationInfo(Constants::MESSAGE_SET_OSD_STRING, maxLengthValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_SET_OSD_NAME, maxLengthValidator, DEST_DIRECT);
        // Messages for the Device Menu Control.
        AddValidationInfo(Constants::MESSAGE_MENU_REQUEST, oneByteValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_MENU_STATUS, oneByteValidator, DEST_DIRECT);
        // Messages for the Remote Control Passthrough.
        // TODO: Parse the first parameter and determine if it can have the next parameter.
        AddValidationInfo(Constants::MESSAGE_USER_CONTROL_PRESSED,
                new VariableLengthValidator(1, 2), DEST_DIRECT);
        // Messages for the Power Status.
        AddValidationInfo(Constants::MESSAGE_REPORT_POWER_STATUS, oneByteValidator, DEST_DIRECT);
        // Messages for the General Protocol.
        AddValidationInfo(Constants::MESSAGE_FEATURE_ABORT,
                new FixedLengthValidator(2), DEST_DIRECT);
        // Messages for the System Audio Control.
        AddValidationInfo(Constants::MESSAGE_REPORT_AUDIO_STATUS, oneByteValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REPORT_SHORT_AUDIO_DESCRIPTOR,
                new FixedLengthValidator(3), DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_REQUEST_SHORT_AUDIO_DESCRIPTOR,
                oneByteValidator, DEST_DIRECT);
        AddValidationInfo(Constants::MESSAGE_SET_SYSTEM_AUDIO_MODE, oneByteValidator, DEST_ALL);
        AddValidationInfo(Constants::MESSAGE_SYSTEM_AUDIO_MODE_STATUS,
                oneByteValidator, DEST_DIRECT);
        // Messages for the Audio Rate Control.
        AddValidationInfo(Constants::MESSAGE_SET_AUDIO_RATE, oneByteValidator, DEST_DIRECT);
        // All Messages for the ARC have no parameters.
        // Messages for the Capability Discovery and Control.
        AddValidationInfo(Constants::MESSAGE_CDC_MESSAGE, maxLengthValidator,
                DEST_BROADCAST | SRC_UNREGISTERED);

#endif
}

ECode HdmiCecMessageValidator::AddValidationInfo(
    /* [in] */ Int32 opcode,
    /* [in] */ IHdmiCecMessageValidatorParameterValidator* validator,
    /* [in] */ Int32 addrType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mValidationInfo->Append(opcode, new ValidationInfo(validator, addrType));

#endif
}

ECode HdmiCecMessageValidator::IsValid(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 opcode;
        message->GetOpcode(&opcode);
        AutoPtr<IInterface> obj;
        mValidationInfo->Get(opcode, (IInterface**)&obj);
        ValidationInfo info = I::Probe(obj);
        if (info == NULL) {
            HdmiLogger::Warning("No validation information for the message: " + message);
            return OK;
        }
        // Check the source field.
        Int32 srcAddr;
        message->GetSource(&srcAddr);
        if (srcAddr == Constants::ADDR_UNREGISTERED &&
                (info->mAddressType & SRC_UNREGISTERED) == 0) {
            HdmiLogger::Warning("Unexpected source: " + message);
            return ERROR_SOURCE;
        }
        // Check the destination field.
        Int32 dest;
        message->GetDestination(&dest);
        if (dest == Constants::ADDR_BROADCAST) {
            if ((info->mAddressType & DEST_BROADCAST) == 0) {
                HdmiLogger::Warning("Unexpected broadcast message: " + message);
                return ERROR_DESTINATION;
            }
        } else {  // Direct addressing.
            if ((info->mAddressType & DEST_DIRECT) == 0) {
                HdmiLogger::Warning("Unexpected direct message: " + message);
                return ERROR_DESTINATION;
            }
        }
        // Check the parameter type.
        AutoPtr<ArrayOf<Byte> > params;
        message->GetParams((ArrayOf<Byte>**)&params);
        if (!info->mParameterValidator->IsValid(params)) {
            HdmiLogger::Warning("Unexpected parameters: " + message);
            return ERROR_PARAMETER;
        }
        return OK;

#endif
}

ECode HdmiCecMessageValidator::IsValidPhysicalAddress(
    /* [in] */ ArrayOf<Byte>* params,
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // TODO: Add more logic like validating 1.0.1.0.
        Boolean isTvDevice;
        mService->IsTvDevice(&isTvDevice);
        if (!isTvDevice) {
            // If the device is not TV, we can't convert path to port-id, so stop here.
            *result = TRUE;
            return NOERROR;
        }
        Int32 path = HdmiUtils::TwoBytesToInt32(params, offset);
        Int32 physicalAddr;
        mService->GetPhysicalAddress(&physicalAddr);
        if (path != Constants::INVALID_PHYSICAL_ADDRESS && path == physicalAddr) {
            *result = TRUE;
            return NOERROR;
        }
        Int32 portId;
        mService->PathToPortId(path, &portId);
        if (portId == Constants::INVALID_PORT_ID) {
            *result = FALSE;
            return NOERROR;
        }
        *result = TRUE;
        return NOERROR;

#endif
}

ECode HdmiCecMessageValidator::IsValidType(
    /* [in] */ Int32 type,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (IHdmiDeviceInfo::DEVICE_TV <= type
                && type <= IHdmiDeviceInfo::DEVICE_VIDEO_PROCESSOR)
                && type != IHdmiDeviceInfo::DEVICE_RESERVED;

#endif
}

} // namespace Hdmi
} // namespace Server
} // namespace Droid
} // namespace Elastos
