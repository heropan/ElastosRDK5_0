
#include "ext/frameworkext.h"
#include "view/CMotionEventHelper.h"
#include "view/CMotionEvent.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CMotionEventHelper::Obtain(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ ArrayOf<IPointerProperties*>* pointerProperties,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 buttonState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [in] */ Int32 source,
    /* [in] */ Int32 flags,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::Obtain(
        downTime, eventTime, action, pointerCount,
        pointerProperties, pointerCoords,
        metaState, buttonState, xPrecision, yPrecision,
        deviceId, edgeFlags, source, flags, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ObtainEx(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ ArrayOf<Int32>* pointerIds,
    /* [in] */ ArrayOf<IPointerCoords*>* pointerCoords,
    /* [in] */ Int32 metaState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [in] */ Int32 source,
    /* [in] */ Int32 flags,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::Obtain(
        downTime, eventTime, action, pointerCount, pointerIds,
        pointerCoords, metaState, xPrecision, yPrecision,
        deviceId, edgeFlags, source, flags, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ObtainEx2(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size,
    /* [in] */ Int32 metaState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::Obtain(
        downTime, eventTime, action, x, y, pressure, size, metaState,
        xPrecision, yPrecision, deviceId, edgeFlags, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ObtainEx3(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float pressure,
    /* [in] */ Float size,
    /* [in] */ Int32 metaState,
    /* [in] */ Float xPrecision,
    /* [in] */ Float yPrecision,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 edgeFlags,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::Obtain(
        downTime, eventTime, action, pointerCount, x, y, pressure, size,
        metaState, xPrecision, yPrecision, deviceId, edgeFlags, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ObtainEx4(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 metaState,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::Obtain(
        downTime, eventTime, action, x, y, metaState, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ObtainEx5(
    /* [in] */ IMotionEvent* o,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::Obtain((CMotionEvent*)o, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ObtainNoHistory(
    /* [in] */ IMotionEvent* o,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::ObtainNoHistory((CMotionEvent*)o, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

ECode CMotionEventHelper::ActionToString(
    /* [in] */ Int32 action,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CMotionEvent::ActionToString(action);

    return NOERROR;
}

ECode CMotionEventHelper::AxisToString(
    /* [in] */ Int32 axis,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CMotionEvent::AxisToString(axis);

    return NOERROR;
}

ECode CMotionEventHelper::AxisFromString(
    /* [in] */ const String& symbolicName,
    /* [out] */ Int32* axis)
{
    VALIDATE_NOT_NULL(axis);
    return CMotionEvent::AxisFromString(symbolicName, axis);
}

ECode CMotionEventHelper::ButtonStateToString(
    /* [in] */ Int32 buttonState,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CMotionEvent::ButtonStateToString(buttonState);

    return NOERROR;
}

ECode CMotionEventHelper::ToolTypeToString(
    /* [in] */ Int32 toolType,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = CMotionEvent::ToolTypeToString(toolType);

    return NOERROR;
}

ECode CMotionEventHelper::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ IMotionEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    AutoPtr<IMotionEvent> ev;
    FAIL_RETURN(CMotionEvent::CreateFromParcelBody(in, (CMotionEvent**)&ev));

    *event = ev;
    INTERFACE_ADDREF(*event);
    return NOERROR;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
