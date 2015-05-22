#ifndef __UTILITY_ZONEINFODB_H__
#define __UTILITY_ZONEINFODB_H__

#include <Elastos.CoreLibrary_server.h>
#include <elastos/Mutex.h>

using Elastos::IO::IByteBuffer;
using Elastos::Core::Threading::Mutex;

namespace Elastos{
namespace Utility{

class ZoneInfoDB
{
public:
    static AutoPtr<ArrayOf<String> > GetAvailableIDs();

    static AutoPtr<ArrayOf<String> > GetAvailableIDs(
        /* [in] */ Int32 rawOffset);

    static AutoPtr<ITimeZone> GetSystemDefault();

    static AutoPtr<ITimeZone> GetTimeZone(
        /* [in] */ const String& id);

    static String GetVersion();

    static AutoPtr<ITimeZone> MakeTimeZone(
        /* [in] */ const String& id);

    static CARAPI Init();

private:
    static String ReadVersion();

    static void ReadIndex();

    static AutoPtr<IByteBuffer> MapData();

    static String GetDirectoryName();

private:
    /**
      * The directory containing the time zone database files.
      */
    static String ZONE_DIRECTORY_NAME;

    /**
     * The name of the file containing the concatenated time zone records.
     */
    static String ZONE_FILE_NAME;

    /**
     * The name of the file containing the index to each time zone record within
     * the zoneinfo.dat file.
     */
    static String INDEX_FILE_NAME;

    static Mutex sLock;

    static String VERSION;

    /**
     * The 'ids' array contains time zone ids sorted alphabetically, for binary searching.
     * The other two arrays are in the same order. 'byteOffsets' gives the byte offset
     * into "zoneinfo.dat" of each time zone, and 'rawUtcOffsets' gives the time zone's
     * raw UTC offset.
     */
    static AutoPtr<ArrayOf<String> > mIDs;
    static AutoPtr<ArrayOf<Int32> > mByteOffsets;
    static AutoPtr<ArrayOf<Int32> > mRawUtcOffsets;
    static AutoPtr<IByteBuffer> mMappedData;
};

} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_ZONEINFODB_H__
