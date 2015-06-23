
#include "hardware/CSerialManager.h"
#include "hardware/CSerialPort.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Hardware {

String CSerialManager::TAG("SerialManager");

ECode CSerialManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IISerialManager* service)
{
    mContext = context;
    mService = service;
    return NOERROR;
}

ECode CSerialManager::GetSerialPorts(
    /* [out, callee] */ ArrayOf<String>** ports)
{
    // try {
    return mService->GetSerialPorts(ports);
    // } catch (RemoteException e) {
    //     Log.e(TAG, "RemoteException in getSerialPorts", e);
    //     return null;
    // }
}

ECode CSerialManager::OpenSerialPort(
    /* [in] */ const String& name,
    /* [in] */ Int32 speed,
    /* [out] */ ISerialPort** port)
{
    assert(port != NULL);
    // try {
    AutoPtr<IParcelFileDescriptor> pfd;
    FAIL_RETURN(mService->OpenSerialPort(name, (IParcelFileDescriptor**)&pfd));
    if (pfd != NULL) {
        CSerialPort::New(name, port);
        (*port)->Open(pfd, speed);
        return NOERROR;
    } else {
        // throw new IOException("Could not open serial port " + name);
        return E_IO_EXCEPTION;
    }
    // } catch (RemoteException e) {
    //     Log.e(TAG, "exception in UsbManager.openDevice", e);
    // }
    *port = NULL;
    return NOERROR;
}


} // namespace Hardware
} // namespace Droid
} // namespace Elastos

