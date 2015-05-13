
#ifndef __CMTPDEVICEINFO_H__
#define __CMTPDEVICEINFO_H__

#include "_CMtpDeviceInfo.h"

namespace Elastos {
namespace Droid {
namespace Mtp {

CarClass(CMtpDeviceInfo)
{
public:
    CMtpDeviceInfo();

    CARAPI GetManufacturer(
        /* [out] */ String* result);

    CARAPI SetManufacturer(
        /* [in] */ const String& str);

    CARAPI GetModel(
        /* [out]*/ String* result);

    CARAPI SetModel(
        /* [in] */ const String& str);

    CARAPI GetVersion(
        /* [out]*/ String* result);

    CARAPI SetVersion(
        /* [in] */ const String& str);

    CARAPI GetSerialNumber(
        /* [out]*/ String* result);

    CARAPI SetSerialNumber(
        /* [in] */ const String& str);

private:
    String mManufacturer;
    String mModel;
    String mVersion;
    String mSerialNumber;
};

} // namespace Mtp
} // namepsace Droid
} // namespace Elastos
#endif // __CMTPDEVICEINFO_H__
