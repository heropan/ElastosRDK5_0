
#include "ZoneInfoDB.h"
#include "CSystem.h"
#include "Arrays.h"
#include "CStringWrapper.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Utility::Arrays;

namespace Libcore {
namespace Utility {

//===============================================================
// ZoneInfoDB
//===============================================================
AutoPtr<ZoneInfoDB::TzData> InitDATA()
{
    AutoPtr<CSystem> system;
    CSystem::AcquireSingletonByFriend((CSystem**)&system);
    String data, root;
    system->GetEnv(String("ANDROID_DATA"), &data);
    system->GetEnv(String("ANDROID_ROOT"), &root);
    data += "/misc/zoneinfo/tzdata";
    root += "/usr/share/zoneinfo/tzdata";

    AutoPtr<ArrayOf<String> > paths = ArrayOf<String>::Alloc(2);
    paths->Set(0, data);
    paths->Set(1, root);
    AutoPtr<ZoneInfoDB::TzData> tz = new ZoneInfoDB::TzData();
    tz->Init(paths);
    return tz;
}

AutoPtr<ZoneInfoDB::TzData> ZoneInfoDB::DATA = InitDATA();

AutoPtr<ZoneInfoDB::TzData> ZoneInfoDB::GetInstance()
{
    return DATA;
}

//===============================================================
// ZoneInfoDB::TzData::MyBasicLruCache
//===============================================================

AutoPtr<IZoneInfo> ZoneInfoDB::TzData::MyBasicLruCache::Create(
    /* [in] */ const String& id)
{
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(id, (ICharSequence**)&csq);
    AutoPtr<IInterface> obj = Create(TO_IINTERFACE(csq));
    AutoPtr<IZoneInfo> zi = IZoneInfo::Probe(obj);
    return zi;
}

AutoPtr<IInterface> ZoneInfoDB::TzData::MyBasicLruCache::Create(
    /* [in] */ IInterface* key)
{
    // // Work out where in the big data file this time zone is.
    // Int32 index = Arrays::BinarySearch(mIds, id);
    // if (index < 0) {
    //   return NULL;
    // }

    // BufferIterator it = mappedFile.bigEndianIterator();
    // it.skip(byteOffsets[index]);

    // return ZoneInfo.makeTimeZone(id, it);
}

//===============================================================
// ZoneInfoDB::TzData
//===============================================================
const Int32 ZoneInfoDB::TzData::CACHE_SIZE;

ZoneInfoDB::TzData::TzData()
{

}

void ZoneInfoDB::TzData::Init(
    /* [in] */ ArrayOf<String>* paths)
{
    for (Int32 i = 0; i < paths->GetLength(); ++i) {
        if (LoadData((*paths)[i])) {
            return;
        }
    }

    // // We didn't find any usable tzdata on disk, so let's just hard-code knowledge of "GMT".
    // // This is actually implemented in TimeZone itself, so if this is the only time zone
    // // we report, we won't be asked any more questions.
    ALOGE("Couldn't find any tzdata!");
    mVersion = String("missing");
    mZoneTab = String("# Emergency fallback data.\n");
    mIds = ArrayOf<String>::Alloc(1);
    mIds->Set(0, String("GMT"));
    mByteOffsets = ArrayOf<Int32>::Alloc(1);
    mRawUtcOffsetsCache = mByteOffsets;
}

Boolean ZoneInfoDB::TzData::LoadData(
    /* [in] */ const String& path)
{
    // try {
    // mappedFile = MemoryMappedFile.mmapRO(path);
    // } catch (ErrnoException errnoException) {
    // return false;
    // }
    // try {
    // readHeader();
    // return true;
    // } catch (Exception ex) {
    // // Something's wrong with the file.
    // // Log the problem and return false so we try the next choice.
    // System.logE("tzdata file \"" + path + "\" was present but invalid!", ex);
    // return false;
    // }
}

ECode ZoneInfoDB::TzData::ReadHeader()
{
    // // byte[12] tzdata_version  -- "tzdata2012f\0"
    // // int index_offset
    // // int data_offset
    // // int zonetab_offset
    // BufferIterator it = mappedFile.bigEndianIterator();

    // byte[] tzdata_version = new byte[12];
    // it.readByteArray(tzdata_version, 0, tzdata_version.length);
    // String magic = new String(tzdata_version, 0, 6, StandardCharsets.US_ASCII);
    // if (!magic.equals("tzdata") || tzdata_version[11] != 0) {
    // throw new RuntimeException("bad tzdata magic: " + Arrays.toString(tzdata_version));
    // }
    // version = new String(tzdata_version, 6, 5, StandardCharsets.US_ASCII);

    // int index_offset = it.readInt();
    // int data_offset = it.readInt();
    // int zonetab_offset = it.readInt();

    // readIndex(it, index_offset, data_offset);
    // readZoneTab(it, zonetab_offset, (int) mappedFile.size() - zonetab_offset);
}

ECode ZoneInfoDB::TzData::ReadZoneTab(
    /* [in] */ IBufferIterator* it,
    /* [in] */ Int32 zoneTabOffset,
    /* [in] */ Int32 zoneTabSize)
{
    // byte[] bytes = new byte[zoneTabSize];
    // it.seek(zoneTabOffset);
    // it.readByteArray(bytes, 0, bytes.length);
    // zoneTab = new String(bytes, 0, bytes.length, StandardCharsets.US_ASCII);
}

ECode ZoneInfoDB::TzData::ReadIndex(
    /* [in] */ IBufferIterator* it,
    /* [in] */ Int32 indexOffset,
    /* [in] */ Int32 dataOffset)
{
    // it.seek(indexOffset);

    // // The database reserves 40 bytes for each id.
    // final int SIZEOF_TZNAME = 40;
    // // The database uses 32-bit (4 byte) integers.
    // final int SIZEOF_TZINT = 4;

    // byte[] idBytes = new byte[SIZEOF_TZNAME];
    // int indexSize = (dataOffset - indexOffset);
    // int entryCount = indexSize / (SIZEOF_TZNAME + 3*SIZEOF_TZINT);

    // char[] idChars = new char[entryCount * SIZEOF_TZNAME];
    // int[] idEnd = new int[entryCount];
    // int idOffset = 0;

    // byteOffsets = new int[entryCount];

    // for (int i = 0; i < entryCount; i++) {
    // it.readByteArray(idBytes, 0, idBytes.length);

    // byteOffsets[i] = it.readInt();
    // byteOffsets[i] += dataOffset; // TODO: change the file format so this is included.

    // int length = it.readInt();
    // if (length < 44) {
    //   throw new AssertionError("length in index file < sizeof(tzhead)");
    // }
    // it.skip(4); // Skip the unused 4 bytes that used to be the raw offset.

    // // Don't include null chars in the String
    // int len = idBytes.length;
    // for (int j = 0; j < len; j++) {
    //   if (idBytes[j] == 0) {
    //     break;
    //   }
    //   idChars[idOffset++] = (char) (idBytes[j] & 0xFF);
    // }

    // idEnd[i] = idOffset;
    // }

    // // We create one string containing all the ids, and then break that into substrings.
    // // This way, all ids share a single char[] on the heap.
    // String allIds = new String(idChars, 0, idOffset);
    // ids = new String[entryCount];
    // for (int i = 0; i < entryCount; i++) {
    // ids[i] = allIds.substring(i == 0 ? 0 : idEnd[i - 1], idEnd[i]);
    // }
}

AutoPtr<ArrayOf<String> > ZoneInfoDB::TzData::GetAvailableIDs()
{
    //return ids.clone();
}

AutoPtr<ArrayOf<String> > ZoneInfoDB::TzData::GetAvailableIDs(
    /* [in] */ Int32 rawUtcOffset)
{
      // List<String> matches = new ArrayList<String>();
      // int[] rawUtcOffsets = getRawUtcOffsets();
      // for (int i = 0; i < rawUtcOffsets.length; ++i) {
      //   if (rawUtcOffsets[i] == rawUtcOffset) {
      //     matches.add(ids[i]);
      //   }
      // }
      // return matches.toArray(new String[matches.size()]);
}

AutoPtr<ArrayOf<Int32> > ZoneInfoDB::TzData::GetRawUtcOffsets()
{
    // synchronized(this) {
    //     if (rawUtcOffsetsCache != null) {
    //     return rawUtcOffsetsCache;
    //     }
    //     rawUtcOffsetsCache = new int[ids.length];
    //     for (int i = 0; i < ids.length; ++i) {
    //     // This creates a TimeZone, which is quite expensive. Hence the cache.
    //     // Note that icu4c does the same (without the cache), so if you're
    //     // switching this code over to icu4j you should check its performance.
    //     // Telephony shouldn't care, but someone converting a bunch of calendar
    //     // events might.
    //     rawUtcOffsetsCache[i] = cache.get(ids[i]).getRawOffset();
    //     }
    // }
    // return rawUtcOffsetsCache;
}

String ZoneInfoDB::TzData::GetVersion()
{
    return mVersion;
}

String ZoneInfoDB::TzData::GetZoneTab()
{
    return mZoneTab;
}

AutoPtr<IZoneInfo> ZoneInfoDB::TzData::MakeTimeZone(
    /* [in] */ const String& id)
{
      // ZoneInfo zoneInfo = cache.get(id);
      // // The object from the cache is cloned because TimeZone / ZoneInfo are mutable.
      // return zoneInfo == null ? null : (ZoneInfo) zoneInfo.clone();
    return NULL;
}

} // Utility
} // Libcore
