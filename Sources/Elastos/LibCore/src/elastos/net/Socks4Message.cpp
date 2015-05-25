
#include "Socks4Message.h"
#include <elastos/Math.h>

namespace Elastos {
namespace Net {

Socks4Message::Socks4Message()
{
    Init();
}

ECode Socks4Message::Init()
{
    mBuffer = ArrayOf<Byte>::Alloc(BUFFER_LENGTH);
    SetVersionNumber(SOCKS_VERSION);
    return NOERROR;
}

Int32 Socks4Message::GetCommandOrResult()
{
    return (*mBuffer)[INDEX_COMMAND];
}

void Socks4Message::SetCommandOrResult(
    /* [in] */ Int32 command)
{
    (*mBuffer)[INDEX_COMMAND] = (Byte) command;
}

Int32 Socks4Message::GetPort()
{
    return GetInt16(INDEX_PORT);
}

void Socks4Message::SetPort(
    /* [in] */ Int32 port)
{
    SetInt16(INDEX_PORT, port);
}

Int32 Socks4Message::GetIP()
{
    return GetInt32(INDEX_IP);
}

void Socks4Message::SetIP(
    /* [in] */ const ArrayOf<Byte>& ip)
{
    (*mBuffer)[INDEX_IP] = ip[0];
    (*mBuffer)[INDEX_IP + 1] = ip[1];
    (*mBuffer)[INDEX_IP + 2] = ip[2];
    (*mBuffer)[INDEX_IP + 3] = ip[3];
}

String Socks4Message::GetUserId()
{
    return GetString(INDEX_USER_ID, MAX_USER_ID_LENGTH);
}

void Socks4Message::SetUserId(
    /* [in] */ const String& id)
{
    SetString(INDEX_USER_ID, MAX_USER_ID_LENGTH, id);
}

String Socks4Message::ToString()
{
}

Int32 Socks4Message::GetLength()
{
    Int32 index = 0;

    // Look for the end of the user id.
    for (index = INDEX_USER_ID; (*mBuffer)[index] != 0; index++) {
    /*
     * Finds the end of the user id by searching for the null
     * termination of the user id string.
     */
    }

    // Increment the index to include the NULL character in the length;
    index++;
    return index;
}

String Socks4Message::GetErrorString(
    /* [in] */ Int32 error)
{
}

AutoPtr<ArrayOf<Byte> > Socks4Message::GetBytes()
{
    return mBuffer;
}
//private-------------------------------------
Int32 Socks4Message::GetInt16(
    /* [in] */ Int32 offset)
{
    return ((((*mBuffer)[offset] & 0xFF) << 8) + ((*mBuffer)[offset + 1] & 0xFF));
}

Int32 Socks4Message::GetInt32(
    /* [in] */ Int32 offset)
{
    return (((*mBuffer)[offset + 3] & 0xFF)
            + (((*mBuffer)[offset + 2] & 0xFF) << 8)
            + (((*mBuffer)[offset + 1] & 0xFF) << 16) + (((*mBuffer)[offset + 0] & 0xFF) << 24));

}

String Socks4Message::GetString(
     /* [in] */ Int32 offset,
     /* [in] */ Int32 maxLength)
{
   Int32 index = offset;
   Int32 lastIndex = index + maxLength;
   while (index < lastIndex && ((*mBuffer)[index] != 0)) {
       index++;
   }
   AutoPtr< ArrayOf<Char32> > outchar = ArrayOf<Char32>::Alloc(index - offset);
   for (Int32 i = offset; i < index - offset; ++i) {
       (*outchar)[i] = (*mBuffer)[i];
   }
   return String(*outchar, 0, index - offset/*, Charsets.ISO_8859_1*/);
}

Int32 Socks4Message::GetVersionNumber()
{
    return (*mBuffer)[INDEX_VERSION];
}

void Socks4Message::SetInt16(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 value)
{
    (*mBuffer)[offset] = (Byte) (value >> 8 & 0xFF);
    (*mBuffer)[offset + 1] = (Byte) (value & 0xFF);
}

void Socks4Message::SetString(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 maxLength,
    /* [in] */ const String& theString)
{

    AutoPtr< ArrayOf<Char32> > stringBytes = theString.GetChars(/*Charsets.ISO_8859_1*/);
    Int32 length = Elastos::Core::Math::Min(stringBytes->GetLength(), maxLength);
    for (Int32 i = 0; i < length; ++i) {
        (*mBuffer)[offset + i] = (*stringBytes)[i] & 0xFF;
    }
    (*mBuffer)[offset + length] = 0;
}

void Socks4Message::SetVersionNumber(int number)
{
    (*mBuffer)[INDEX_VERSION] = (Byte) number;
}

} // namespace Net
} // namespace Elastos
