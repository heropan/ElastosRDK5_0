
#ifndef  __CSERIALPORT_H__
#define  __CSERIALPORT_H__

#include "_CSerialPort.h"

using Elastos::IO::IByteBuffer;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Os::IParcelFileDescriptor;


namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CSerialPort)
{
public:
    CSerialPort();

    ~CSerialPort();

    /**
     * SerialPort should only be instantiated by SerialManager
     * @hide
     */
    CARAPI constructor(
        /* [in] */ const String& name);

    /**
     * SerialPort should only be instantiated by SerialManager
     * Speed must be one of 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600,
     * 19200, 38400, 57600, 115200, 230400, 460800, 500000, 576000, 921600, 1000000, 1152000,
     * 1500000, 2000000, 2500000, 3000000, 3500000, 4000000
     *
     * @hide
     */
    CARAPI Open(
        /* [in] */ IParcelFileDescriptor* pfd,
        /* [in] */ Int32 speed);

    /**
     * Closes the serial port
     */
    CARAPI Close();

    /**
     * Returns the name of the serial port
     *
     * @return the serial port's name
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Reads data into the provided buffer
     *
     * @param buffer to read into
     * @return number of bytes read
     */
    CARAPI Read(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* num);

    /**
     * Writes data from provided buffer
     *
     * @param buffer to write
     * @param length number of bytes to write
     */
    CARAPI Write(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 length);

    /**
     * Sends a stream of zero valued bits for 0.25 to 0.5 seconds
     */
    CARAPI SendBreak();

private:
    //java native methods.
    void native_open(
        /* [in] */ IFileDescriptor* pfd,
        /* [in] */ Int32 speed);

    void native_close();

    Int32 native_read_array(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 length);

    Int32 native_read_direct(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 length);

    void native_write_array(
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 length);

    void native_write_direct(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 length);

    void native_send_break();

private:
    static String TAG;

    // used by the JNI code
    Int32 mNativeContext;
    String mName;
    AutoPtr<IParcelFileDescriptor> mFileDescriptor;
};


} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__CSERIALPORT_H__
