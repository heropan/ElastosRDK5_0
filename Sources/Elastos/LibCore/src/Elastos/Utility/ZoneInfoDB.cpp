
#include "cmdef.h"
#include "ZoneInfoDB.h"
#include "CSystem.h"
#include <elastos/List.h>
#include <elastos/StringBuilder.h>
#include "TimeZone.h"
#include "CZoneInfo.h"
#include "TimeZoneGetter.h"
#include "CRandomAccessFile.h"
#ifdef ELASTOS_CORE
#include <elastos/CSystem.h>
#endif

namespace Elastos {
namespace Utility {

using Elastos::Core::StringBuilder;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::List;
using Elastos::IO::IRandomAccessFile;
using Elastos::IO::CRandomAccessFile;
using Elastos::IO::IDataInput;
using Elastos::IO::IMappedByteBuffer;
using Elastos::IO::Channels::IFileChannel;
using Elastos::Utility::TimeZoneGetter;

String ZoneInfoDB::ZONE_DIRECTORY_NAME;
String ZoneInfoDB::ZONE_FILE_NAME;
String ZoneInfoDB::INDEX_FILE_NAME;
String ZoneInfoDB::VERSION;

AutoPtr<ArrayOf<String> > ZoneInfoDB::mIDs;
AutoPtr<ArrayOf<Int32> > ZoneInfoDB::mRawUtcOffsets;
AutoPtr<ArrayOf<Int32> > ZoneInfoDB::mByteOffsets;
AutoPtr<IByteBuffer> ZoneInfoDB::mMappedData;
Mutex ZoneInfoDB::sLock;

ECode ZoneInfoDB::Init()
{
    if (mIDs == NULL) {
        Mutex::Autolock lock(sLock);
        if (mIDs == NULL) {
            ZONE_DIRECTORY_NAME = GetDirectoryName();
            ZONE_FILE_NAME = ZONE_DIRECTORY_NAME + "zoneinfo.dat";
            INDEX_FILE_NAME = ZONE_DIRECTORY_NAME + "zoneinfo.idx";

            mMappedData = MapData();

            ReadIndex();

            VERSION = ReadVersion();
        }
    }
    return NOERROR;
}

String ZoneInfoDB::GetDirectoryName()
{
    AutoPtr<ISystem> system;
#ifdef ELASTOS_CORE
    AutoPtr<CSystem> cs;
    CSystem::AcquireSingleton((CSystem**)&cs);
    system = (ISystem*)cs.Get();
#else
    CSystem::AcquireSingleton((ISystem**)&system);
#endif

    String str;
    system->GetEnv(String("ANDROID_ROOT"), &str);
    StringBuilder sb(128);
    sb += str;
    sb += "/usr/share/zoneinfo/";
    return sb.ToString();
}

String ZoneInfoDB::ReadVersion()
{
    String path = ZONE_DIRECTORY_NAME + "zoneinfo.version";
    AutoPtr<CRandomAccessFile> obj;
    ECode ec = CRandomAccessFile::NewByFriend(path, String("r"), (CRandomAccessFile **)&obj);
    if (FAILED(ec) || obj == NULL) {
        return String("");
    }

    AutoPtr<IRandomAccessFile> versionFile = (IRandomAccessFile*)obj.Get();

    Int32 len;
    versionFile->GetLength((Int64 *)&len);
    AutoPtr<IDataInput> input;
    input = IDataInput::Probe(versionFile);

    AutoPtr<ArrayOf<Byte> > buf = ArrayOf<Byte>::Alloc(len);
    ec = input->ReadFully(buf);
    versionFile->Close();
    return String((const char *)buf.Get(), buf->GetLength()).Trim();
}

void ZoneInfoDB::ReadIndex()
{
    AutoPtr<CRandomAccessFile> obj;
    ECode ec = CRandomAccessFile::NewByFriend(INDEX_FILE_NAME, String("r"), (CRandomAccessFile **)&obj);
    if (FAILED(ec) || obj == NULL) {
        return;
    }

    AutoPtr<IRandomAccessFile> indexFile = (IRandomAccessFile*)obj.Get();

    AutoPtr<IDataInput> input;
    input = IDataInput::Probe(indexFile);
    assert(input != NULL);

    // The database reserves 40 bytes for each id.
    const Int32 SIZEOF_TZNAME = 40;
    // The database uses 32-bit (4 byte) integers.
    const Int32 SIZEOF_TZINT = 4;

    Int64 len;
    indexFile->GetLength(&len);

    AutoPtr<ArrayOf<Byte> > idBytes = ArrayOf<Byte>::Alloc(SIZEOF_TZNAME);
    Int32 numEntries = (Int32) (len / (SIZEOF_TZNAME + 3*SIZEOF_TZINT));

    AutoPtr<ArrayOf<Char32> > idChars = ArrayOf<Char32>::Alloc(numEntries * SIZEOF_TZNAME);
    AutoPtr<ArrayOf<Int32> > idEnd = ArrayOf<Int32>::Alloc(numEntries);
    Int32 idOffset = 0;

    mByteOffsets = ArrayOf<Int32>::Alloc(numEntries);
    mRawUtcOffsets = ArrayOf<Int32>::Alloc(numEntries);

    Int32 length;
    for (Int32 i = 0; i < numEntries; i++) {
        input->ReadFully(idBytes);
        Int32 offset;
        input->ReadInt32(&offset);
        (*mByteOffsets)[i] = offset;

        input->ReadInt32(&length);
        if (length < 44) {
            indexFile->Close();
            return;
        }

        input->ReadInt32(&offset);
        (*mRawUtcOffsets)[i] = offset;

        // Don't include null chars in the String
        Int32 len = idBytes->GetLength();
        Int32 value;
        for (Int32 j = 0; j < len; j++) {
            if ((*idBytes)[j] == 0) {
                break;
            }
            value = (*idBytes)[j] & 0xFF;
            (*idChars)[idOffset++] = *((Char32 *)&value);
        }

        (*idEnd)[i] = idOffset;
    }

    // We create one string containing all the ids, and then break that into substrings.
    // This way, all ids share a single char[] on the heap.
    String allIds = String(*idChars ,0 ,idOffset);
    mIDs = ArrayOf<String>::Alloc(numEntries);
    for (Int32 i = 0; i < numEntries; i++) {
        Int32 substart =  i == 0 ? 0 : (*idEnd)[i - 1];
        (*mIDs)[i] = allIds.Substring(substart, (*idEnd)[i]);
    }

    indexFile->Close();
}

AutoPtr<IByteBuffer> ZoneInfoDB::MapData()
{
    AutoPtr<CRandomAccessFile> obj;
    ECode ec = CRandomAccessFile::NewByFriend(ZONE_FILE_NAME, String("r"), (CRandomAccessFile **)&obj);
    if (FAILED(ec) || obj == NULL) {
        return NULL;
    }
    AutoPtr<IRandomAccessFile> file = (IRandomAccessFile*)obj.Get();

    AutoPtr<IDataInput> input = IDataInput::Probe(file);
    assert(input != NULL);

    AutoPtr<IFileChannel> channel;
    file->GetChannel((IFileChannel**)&channel);

    AutoPtr<IByteBuffer> buf;
    Int64 size;
    channel->GetSize(&size);
    channel->Map(Elastos::IO::Channels::FileChannelMapMode_READ_ONLY, 0, size, (IMappedByteBuffer **)&buf);
    file->Close();
    return buf;
}

AutoPtr<ITimeZone> ZoneInfoDB::MakeTimeZone(
    /* [in] */ const String& id)
{
    Init();

    assert(mIDs != NULL);
    assert(mMappedData != NULL);

    Int32 index = mIDs->BinarySearch(id);
    if (index < 0) {
       return NULL;
    }

    Int32 start = (*mByteOffsets)[index];

    // We duplicate the ByteBuffer to allow unsynchronized access to this shared data,
    // despite Buffer's implicit position.
    AutoPtr<IByteBuffer> data;
    mMappedData->Duplicate((IByteBuffer **)&data);

    data->SetPosition(start);
    // Variable names beginning tzh_ correspond to those in "tzfile.h".
    // Check tzh_magic.
    Int32 value;
    Byte bValue;
    data->GetInt32(&value);
    if (value != 0x545a6966) { // "TZif"
       return NULL;
    }

    // Skip the uninteresting part of the header.
    data->SetPosition(start + 32);

    // Read the sizes of the arrays we're about to read.
    Int32 tzh_timecnt;
    Int32 tzh_typecnt;
    Int32 skip;

    data->GetInt32(&tzh_timecnt);
    data->GetInt32(&tzh_typecnt);
    data->GetInt32(&skip);          // skip(4)

    AutoPtr<ArrayOf<Int32> > transitions = ArrayOf<Int32>::Alloc(tzh_timecnt);
    if (transitions == NULL) {
        return NULL;
    }

    for (Int32 i = 0; i < tzh_timecnt; ++i) {
       data->GetInt32(&value);
       (*transitions)[i] = value;
    }

    AutoPtr<ArrayOf<Byte> > type = ArrayOf<Byte>::Alloc(tzh_timecnt);
    data->GetBytes(type);

    AutoPtr<ArrayOf<Int32> > gmtOffsets = ArrayOf<Int32>::Alloc(tzh_typecnt);
    AutoPtr<ArrayOf<Byte> > isDsts = ArrayOf<Byte>::Alloc(tzh_typecnt);
    for (Int32 i = 0; i < tzh_typecnt; ++i) {
       data->GetInt32(&value);
       (*gmtOffsets)[i] = value;
       data->GetByte(&bValue);
       (*isDsts)[i] = bValue;
       data->GetByte(&bValue);      // skip(1)
    }

    AutoPtr<CZoneInfo> timeZone;
    CZoneInfo::NewByFriend(id, transitions, type, gmtOffsets, isDsts , (CZoneInfo **)&timeZone);

    return (ITimeZone*)timeZone.Get();
}

AutoPtr<ArrayOf<String> > ZoneInfoDB::GetAvailableIDs()
{
    Init();

    assert(mIDs != NULL);
    return mIDs->Clone();
}

AutoPtr<ArrayOf<String> > ZoneInfoDB::GetAvailableIDs(
    /* [in] */ Int32 rawOffset)
{
    Init();

    assert(mIDs != NULL);
    List<String> matches;

    for (Int32 i = 0, end = mRawUtcOffsets->GetLength(); i < end; i++) {
        if ((*mRawUtcOffsets)[i] == rawOffset) {
            matches.PushBack((*mIDs)[i]);
        }
    }

    AutoPtr<ArrayOf<String> > arrayOfStr = ArrayOf<String>::Alloc(matches.GetSize());
    List<String>::Iterator it = matches.Begin();
    for (Int32 i = 0; it != matches.End(); ++i, ++it) {
        (*arrayOfStr)[i] = *it;
    }

    return arrayOfStr;
}

AutoPtr<ITimeZone> ZoneInfoDB::GetSystemDefault()
{
    Init();

    Mutex::Autolock lock(sLock);
    String zoneName;
    AutoPtr<ITimeZoneGetter> tzGetter = TimeZoneGetter::GetInstance();
    if (tzGetter != NULL) {
        tzGetter->GetId(&zoneName);
    }

    if (!zoneName.IsNull()) {
        zoneName = zoneName.Trim();
    }

    if (zoneName.IsNullOrEmpty()) {
       // use localtime for the simulator
       // TODO: what does that correspond to?
       zoneName = String("localtime");
    }

    AutoPtr<ITimeZone> tz;
    TimeZone::GetTimeZone(zoneName, (ITimeZone**)&tz);
    return tz;
}

AutoPtr<ITimeZone> ZoneInfoDB::GetTimeZone(
    /* [in] */ const String& id)
{
    if (id.IsNull()) {
        return NULL;
    }
    return MakeTimeZone(id);
}

String ZoneInfoDB::GetVersion()
{
    Init();

    return VERSION;
}

} // Utility
} // Elastos
