
#include "elastos/droid/net/SntpClient.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(SntpClient, Object, ISntpClient)

const String SntpClient::TAG("SntpClient");
const Int32 SntpClient::REFERENCE_TIME_OFFSET = 16;
const Int32 SntpClient::ORIGINATE_TIME_OFFSET = 24;
const Int32 SntpClient::RECEIVE_TIME_OFFSET = 32;
const Int32 SntpClient::TRANSMIT_TIME_OFFSET = 40;
const Int32 SntpClient::NTP_PACKET_SIZE = 48;
const Int32 SntpClient::NTP_PORT = 123;
const Int32 SntpClient::NTP_MODE_CLIENT = 3;
const Int32 SntpClient::NTP_VERSION = 3;
const Int64 SntpClient::OFFSET_1900_TO_1970 = ((365L * 70LL) + 17LL) * 24LL * 60LL * 60LL;

ECode SntpClient::constructor()
{
    return NOERROR;
}

ECode SntpClient::RequestTime(
    /* [in] */ const String& host,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DatagramSocket socket = NULL;
        try {
            socket = new DatagramSocket();
            socket.setSoTimeout(timeout);
            InetAddress address = InetAddress.getByName(host);
            byte[] buffer = new byte[NTP_PACKET_SIZE];
            DatagramPacket request = new DatagramPacket(buffer, buffer.length, address, NTP_PORT);
            // set mode = 3 (client) and version = 3
            // mode is in low 3 bits of first byte
            // version is in bits 3-5 of first byte
            buffer[0] = NTP_MODE_CLIENT | (NTP_VERSION << 3);
            // get current time and write it to the request packet
            long requestTime = System.currentTimeMillis();
            long requestTicks = SystemClock.elapsedRealtime();
            writeTimeStamp(buffer, TRANSMIT_TIME_OFFSET, requestTime);
            socket.send(request);
            // read the response
            DatagramPacket response = new DatagramPacket(buffer, buffer.length);
            socket.receive(response);
            long responseTicks = SystemClock.elapsedRealtime();
            long responseTime = requestTime + (responseTicks - requestTicks);
            // extract the results
            long originateTime = readTimeStamp(buffer, ORIGINATE_TIME_OFFSET);
            long receiveTime = readTimeStamp(buffer, RECEIVE_TIME_OFFSET);
            long transmitTime = readTimeStamp(buffer, TRANSMIT_TIME_OFFSET);
            long roundTripTime = responseTicks - requestTicks - (transmitTime - receiveTime);
            // receiveTime = originateTime + transit + skew
            // responseTime = transmitTime + transit - skew
            // clockOffset = ((receiveTime - originateTime) + (transmitTime - responseTime))/2
            //             = ((originateTime + transit + skew - originateTime) +
            //                (transmitTime - (transmitTime + transit - skew)))/2
            //             = ((transit + skew) + (transmitTime - transmitTime - transit + skew))/2
            //             = (transit + skew - transit + skew)/2
            //             = (2 * skew)/2 = skew
            long clockOffset = ((receiveTime - originateTime) + (transmitTime - responseTime))/2;
            // if (false) Log.d(TAG, "round trip: " + roundTripTime + " ms");
            // if (false) Log.d(TAG, "clock offset: " + clockOffset + " ms");
            // save our results - use the times on this side of the network latency
            // (response rather than request time)
            mNtpTime = responseTime + clockOffset;
            mNtpTimeReference = responseTicks;
            mRoundTripTime = roundTripTime;
        } catch (Exception e) {
            if (FALSE) Logger::D(TAG, "request time failed: " + e);
            return FALSE;
        } finally {
            if (socket != NULL) {
                socket.close();
            }
        }
        return TRUE;
#endif
}

ECode SntpClient::GetNtpTime(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mNtpTime;
#endif
}

ECode SntpClient::GetNtpTimeReference(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mNtpTimeReference;
#endif
}

ECode SntpClient::GetRoundTripTime(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mRoundTripTime;
#endif
}

ECode SntpClient::Read32(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        byte b0 = buffer[offset];
        byte b1 = buffer[offset+1];
        byte b2 = buffer[offset+2];
        byte b3 = buffer[offset+3];
        // convert signed bytes to unsigned values
        Int32 i0 = ((b0 & 0x80) == 0x80 ? (b0 & 0x7F) + 0x80 : b0);
        Int32 i1 = ((b1 & 0x80) == 0x80 ? (b1 & 0x7F) + 0x80 : b1);
        Int32 i2 = ((b2 & 0x80) == 0x80 ? (b2 & 0x7F) + 0x80 : b2);
        Int32 i3 = ((b3 & 0x80) == 0x80 ? (b3 & 0x7F) + 0x80 : b3);
        return ((long)i0 << 24) + ((long)i1 << 16) + ((long)i2 << 8) + (long)i3;
#endif
}

ECode SntpClient::ReadTimeStamp(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        long seconds = read32(buffer, offset);
        long fraction = read32(buffer, offset + 4);
        return ((seconds - OFFSET_1900_TO_1970) * 1000) + ((fraction * 1000L) / 0x100000000L);
#endif
}

ECode SntpClient::WriteTimeStamp(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int64 time)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        long seconds = time / 1000L;
        long milliseconds = time - seconds * 1000L;
        seconds += OFFSET_1900_TO_1970;
        // write seconds in big endian format
        buffer[offset++] = (byte)(seconds >> 24);
        buffer[offset++] = (byte)(seconds >> 16);
        buffer[offset++] = (byte)(seconds >> 8);
        buffer[offset++] = (byte)(seconds >> 0);
        long fraction = milliseconds * 0x100000000L / 1000L;
        // write fraction in big endian format
        buffer[offset++] = (byte)(fraction >> 24);
        buffer[offset++] = (byte)(fraction >> 16);
        buffer[offset++] = (byte)(fraction >> 8);
        // low order bits should be random data
        buffer[offset++] = (byte)(Math.random() * 255.0);
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CSntpClient.cpp
#include "elastos/droid/net/CSntpClient.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include <Elastos.CoreLibrary.h>
#include <os/SystemClock.h>

using Elastos::Core::Math;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::Logging::Slogger;
using Elastos::Net::IDatagramPacket;
using Elastos::Net::CDatagramPacket;
using Elastos::Net::IDatagramSocket;
using Elastos::Net::CDatagramSocket;
using Elastos::Net::IInetAddress;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Droid::Os::SystemClock;


namespace Elastos {
namespace Droid {
namespace Net {

const Boolean CSntpClient::DEBUG = FALSE;
const String CSntpClient::TAG("SntpClient");
const Int32 CSntpClient::REFERENCE_TIME_OFFSET = 16;
const Int32 CSntpClient::ORIGINATE_TIME_OFFSET = 24;
const Int32 CSntpClient::RECEIVE_TIME_OFFSET = 32;
const Int32 CSntpClient::TRANSMIT_TIME_OFFSET = 40;
const Int32 CSntpClient::NTP_PACKET_SIZE = 48;
const Int32 CSntpClient::NTP_PORT = 123;
const Int32 CSntpClient::NTP_MODE_CLIENT = 3;
const Int32 CSntpClient::NTP_VERSION = 3;
const Int64 CSntpClient::OFFSET_1900_TO_1970 = (((365LL * 70LL) + 17LL) * 24LL * 60LL * 60LL);

CSntpClient::CSntpClient()
    : mNtpTime(0)
    , mNtpTimeReference(0)
    , mRoundTripTime(0)
{
}

ECode CSntpClient::RequestTime(
    /* [in] */ const String& host,
    /* [in] */ Int32 timeout,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IDatagramSocket> socket;
    //try {
    ECode ec = CDatagramSocket::New((IDatagramSocket**)&socket);
    if (FAILED(ec) || socket == NULL) {
        Slogger::E(TAG, "Failed to create CDatagramPacket!");
        return ec;
    }

    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);

    Int64 requestTime, requestTicks, responseTicks, responseTime;
    Int64 originateTime, receiveTime, transmitTime, roundTripTime, clockOffset;
    Int32  length = NTP_PACKET_SIZE;
    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(length);

    AutoPtr<IDatagramPacket> response;
    AutoPtr<IInetAddress> address;
    AutoPtr<IDatagramPacket> request;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    ec = socket->SetSoTimeout(timeout);
    FAIL_GOTO(ec, _EXIT_)

    ec = helper->GetByName(host, (IInetAddress**)&address);
    FAIL_GOTO(ec, _EXIT_)

    ec = CDatagramPacket::New(buffer, length, address, NTP_PORT, (IDatagramPacket**)&request);
    FAIL_GOTO(ec, _EXIT_)

    // set mode = 3 (client) and version = 3
    // mode is in low 3 bits of first byte
    // version is in bits 3-5 of first byte
    (*buffer)[0] = NTP_MODE_CLIENT | (NTP_VERSION << 3);

    // get current time and write it to the request packet
    system->GetCurrentTimeMillis(&requestTime);
    requestTicks = SystemClock::GetElapsedRealtime();
    WriteTimeStamp(buffer, TRANSMIT_TIME_OFFSET, requestTime);

    ec = socket->Send(request);
    FAIL_GOTO(ec, _EXIT_)

    // read the response
    ec = CDatagramPacket::New(buffer, length, (IDatagramPacket**)&response);
    FAIL_GOTO(ec, _EXIT_)

    ec = socket->Receive(response);
    FAIL_GOTO(ec, _EXIT_)

    responseTicks = SystemClock::GetElapsedRealtime();
    responseTime = requestTime + (responseTicks - requestTicks);

    // extract the results
    originateTime = ReadTimeStamp(buffer, ORIGINATE_TIME_OFFSET);
    receiveTime = ReadTimeStamp(buffer, RECEIVE_TIME_OFFSET);
    transmitTime = ReadTimeStamp(buffer, TRANSMIT_TIME_OFFSET);
    roundTripTime = responseTicks - requestTicks - (transmitTime - receiveTime);
    // receiveTime = originateTime + transit + skew
    // responseTime = transmitTime + transit - skew
    // clockOffset = ((receiveTime - originateTime) + (transmitTime - responseTime))/2
    //             = ((originateTime + transit + skew - originateTime) +
    //                (transmitTime - (transmitTime + transit - skew)))/2
    //             = ((transit + skew) + (transmitTime - transmitTime - transit + skew))/2
    //             = (transit + skew - transit + skew)/2
    //             = (2 * skew)/2 = skew
    clockOffset = ((receiveTime - originateTime) + (transmitTime - responseTime))/2;
    // if (false) Log.d(TAG, "round trip: " + roundTripTime + " ms");
    // if (false) Log.d(TAG, "clock offset: " + clockOffset + " ms");

    // save our results - use the times on this side of the network latency
    // (response rather than request time)
    mNtpTime = responseTime + clockOffset;
    mNtpTimeReference = responseTicks;
    mRoundTripTime = roundTripTime;

    if (DEBUG) {
        Slogger::D(TAG, " >> RequestTime: mNtpTime: %lld, mNtpTimeReference: %lld, mRoundTripTime: %lld",
            mNtpTime, mNtpTimeReference, mRoundTripTime);
    }
    *result = TRUE;

_EXIT_:
    //finally...
    socket->Close();

    if (FAILED(ec) && DEBUG) {
        Slogger::E(TAG, "request time failed: %08x", ec);
    }

    return ec;
}

/**
 * Returns the time computed from the NTP transaction.
 *
 * @return time value computed from NTP server response.
 */
ECode CSntpClient::GetNtpTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mNtpTime;
    if (DEBUG) {
        Slogger::D(TAG, " >> in GetNtpTime: %lld, %lld", mNtpTime, *time);
    }
    return NOERROR;
}

/**
 * Returns the reference clock value (value of SystemClock.elapsedRealtime())
 * corresponding to the NTP time.
 *
 * @return reference clock corresponding to the NTP time.
 */
ECode CSntpClient::GetNtpTimeReference(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mNtpTimeReference;
    if (DEBUG) {
        Slogger::D(TAG, " >> in GetNtpTimeReference: %lld, %lld", mNtpTimeReference, *time);
    }
    return NOERROR;
}

/**
 * Returns the round trip time of the NTP transaction
 *
 * @return round trip time in milliseconds.
 */
ECode CSntpClient::GetRoundTripTime(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    *time = mRoundTripTime;
    if (DEBUG) {
        Slogger::D(TAG, " >> in GetRoundTripTime: %lld, %lld", mRoundTripTime, *time);
    }
    return NOERROR;
}

/**
 * Reads an unsigned 32 bit big endian number from the given offset in the buffer.
 */
Int64 CSntpClient::Read32(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset)
{
    Byte b0 = (*buffer)[offset];
    Byte b1 =  (*buffer)[offset+1];
    Byte b2 =  (*buffer)[offset+2];
    Byte b3 =  (*buffer)[offset+3];

    // convert signed bytes to unsigned values
    Int32 i0 = ((b0 & 0x80) == 0x80 ? (b0 & 0x7F) + 0x80 : b0);
    Int32 i1 = ((b1 & 0x80) == 0x80 ? (b1 & 0x7F) + 0x80 : b1);
    Int32 i2 = ((b2 & 0x80) == 0x80 ? (b2 & 0x7F) + 0x80 : b2);
    Int32 i3 = ((b3 & 0x80) == 0x80 ? (b3 & 0x7F) + 0x80 : b3);

    return ((Int64)i0 << 24) + ((Int64)i1 << 16) + ((Int64)i2 << 8) + (Int64)i3;
}

/**
 * Reads the NTP time stamp at the given offset in the buffer and returns
 * it as a system time (milliseconds since January 1, 1970).
 */
Int64 CSntpClient::ReadTimeStamp(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset)
{
    Int64 seconds = Read32(buffer, offset);
    Int64 fraction = Read32(buffer, offset + 4);
    return ((seconds - OFFSET_1900_TO_1970) * 1000) + ((fraction * 1000L) / 0x100000000L);
}

/**
 * Writes system time (milliseconds since January 1, 1970) as an NTP time stamp
 * at the given offset in the buffer.
 */
void CSntpClient::WriteTimeStamp(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int64 time)
{
    using Elastos::Core::Math;

    Int64 seconds = time / 1000L;
    Int64 milliseconds = time - seconds * 1000L;
    seconds += OFFSET_1900_TO_1970;

    // write seconds in big endian format
    (*buffer)[offset++] = (Byte)(seconds >> 24);
    (*buffer)[offset++] = (Byte)(seconds >> 16);
    (*buffer)[offset++] = (Byte)(seconds >> 8);
    (*buffer)[offset++] = (Byte)(seconds >> 0);

    Int64 fraction = milliseconds * 0x100000000L / 1000L;
    // write fraction in big endian format
    (*buffer)[offset++] = (Byte)(fraction >> 24);
    (*buffer)[offset++] = (Byte)(fraction >> 16);
    (*buffer)[offset++] = (Byte)(fraction >> 8);
    // low order bits should be random data
    (*buffer)[offset++] = (Byte)(Math::Random() * 255.0);
}


} //namespace Net
} //namespace Droid
} //namespace Elastos
#endif