
#include "TimeZones.h"
#include "ElStringByteSink.h"
#include "CLocaleHelper.h"
#include "CArrayOf.h"
#include "CStringWrapper.h"
#include <elastos/UniquePtr.h>
#include <elastos/Pair.h>
#include <elastos/Vector.h>
#include <elastos/Map.h>
#include <unicode/smpdtfmt.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef ELASTOS_CORELIBRARY
#include <elastos/CSystem.h>
#endif

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::UniquePtr;
using Elastos::Core::CArrayOf;
using Elastos::Core::ICharSequence;
using Elastos::Core::EIID_ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Pair;
using Elastos::Utility::Vector;
using Elastos::Utility::Map;

_ETL_NAMESPACE_BEGIN
const RBTreeColorType S_RBTreeRed = FALSE;
const RBTreeColorType S_RBTreeBlack = TRUE;
_ETL_NAMESPACE_END

namespace Libcore {
namespace ICU {

const static Int32 TimeZone_LONG = 1; // Itimezone::LONG

Int32 ReadFully(Int32 file , ArrayOf<Byte>* buffer)
{
    Int32 number;
    Int32 offset = 0;
    Int32 length = buffer->GetLength();
    while (length > 0) {
        number = read(file , buffer->GetPayload() + offset , length);
        if (number < 0) {
            return -1;
        }
        offset += number;
        length -= number;
    }
    return number;
}

Int32 ReadInt32(Int32 file)
{
    AutoPtr<ArrayOf<Byte> > scratch = ArrayOf<Byte>::Alloc(4);
    Int32 num = ReadFully(file , scratch);
    if (num == -1) {
        return -1;
    }
    Int32 result = (((*scratch)[0] & 0xff) << 24) + (((*scratch)[1] & 0xff) << 16)
                    + (((*scratch)[2] & 0xff) << 8) + ((*scratch)[3] & 0xff);
    return result;
}

AutoPtr<ArrayOf<String> > TimeZones::sAvailableTimeZones;
AutoPtr<TimeZones::ZoneStringsCache> TimeZones::sCachedZoneStrings = new TimeZones::ZoneStringsCache();

AutoPtr< ArrayOf<String> > TimeZones::GetAvailableTimeZones()
{
    if (sAvailableTimeZones != NULL) {
        return sAvailableTimeZones;
    }

    AutoPtr<ISystem> system;
#ifdef ELASTOS_CORELIBRARY
    AutoPtr<Elastos::Core::CSystem> cs;
    Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&cs);
    system = (ISystem*)cs.Get();
#else
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
#endif

    String path;
    system->GetEnv(String("ANDROID_ROOT"), &path);
    path += "/usr/share/zoneinfo/zoneinfo.idx";
    struct stat statbuff;
    if (stat(path.string(), &statbuff) < 0){
        return NULL;
    }
    Int32 indexFile = open(path.string() , O_RDONLY);

    // The database reserves 40 bytes for each id.
    const Int32 SIZEOF_TZNAME = 40;
    // The database uses 32-bit (4 byte) integers.
    const Int32 SIZEOF_TZINT = 4;

    Int64 len = statbuff.st_size;
    AutoPtr<ArrayOf<Byte> > idBytes = ArrayOf<Byte>::Alloc(SIZEOF_TZNAME);
    Int32 numEntries = (Int32) (len / (SIZEOF_TZNAME + 3 * SIZEOF_TZINT));

    AutoPtr<ArrayOf<Char32> > idChars = ArrayOf<Char32>::Alloc(numEntries * SIZEOF_TZNAME);
    AutoPtr<ArrayOf<Int32> > idEnd = ArrayOf<Int32>::Alloc(numEntries);
    Int32 idOffset = 0;

    AutoPtr<ArrayOf<Int32> > mByteOffsets = ArrayOf<Int32>::Alloc(numEntries);
    AutoPtr<ArrayOf<Int32> > mRawUtcOffsets = ArrayOf<Int32>::Alloc(numEntries);

    Int32 length = 0;
    for (Int32 i = 0; i < numEntries; i++) {
        if (ReadFully(indexFile, idBytes) == -1)
        {
            close(indexFile);
            return NULL;
        }
        Int32 offset = ReadInt32(indexFile);
        (*mByteOffsets)[i] = offset;
        length = ReadInt32(indexFile);
        if (length < 44) {
            close(indexFile);
            return NULL;
        }
        offset = ReadInt32(indexFile);
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
    AutoPtr<ArrayOf<String> > ids = ArrayOf<String>::Alloc(numEntries);
    for (Int32 i = 0; i < numEntries; i++) {
        Int32 substart =  i == 0 ? 0 : (*idEnd)[i - 1];
        (*ids)[i] = allIds.Substring(substart, (*idEnd)[i]);
    }

    close(indexFile);
    sAvailableTimeZones = ids;
    return sAvailableTimeZones;
}

TimeZones::ZoneStringsCache::ZoneStringsCache()
{}

TimeZones::ZoneStringsCache::~ZoneStringsCache()
{
    mInternTable.Clear();
}

AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > > TimeZones::ZoneStringsCache::Create(
    /* [in] */ AutoPtr<ILocale> locale)
{
    AutoPtr<ISystem> system;
#ifdef ELASTOS_CORELIBRARY
    AutoPtr<Elastos::Core::CSystem> cs;
    Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&cs);
    system = (ISystem*)cs.Get();
#else
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
#endif

    Int64 start = 0, nativeStart = 0;
    system->GetCurrentTimeMillis(&start);
    nativeStart = start;
    String locname;
    locale->ToString(&locname);
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > > result = GetZoneStringsImpl(locname, GetAvailableTimeZones());
    Int64 nativeEnd;
    system->GetCurrentTimeMillis(&nativeEnd);
    InternStrings(result);
    // Ending up in this method too often is an easy way to make your app slow, so we ensure
    // it's easy to tell from the log (a) what we were doing, (b) how long it took, and
    // (c) that it's all ICU's fault.
    Int64 end;
    system->GetCurrentTimeMillis(&end);
    Int64 duration = end - start;
    Int64 nativeDuration = nativeEnd - nativeStart;
    return result;
}

void TimeZones::ZoneStringsCache::InternStrings(
    /* [in] */ ArrayOf<StringArray>* result)
{
    Mutex::Autolock lock(mlock);
    for (Int32 i = 0; i < result->GetLength(); ++i) {
        for (Int32 j = 1; j < ITimeZones::NAME_COUNT; ++j) {
            String original = (*((*result)[i]))[j];
            String nonDuplicate = mInternTable[original];
            if (nonDuplicate.IsNull()) {
                mInternTable[original] = original;
            }
            else {
                (*((*result)[i]))[j] = nonDuplicate;
            }
        }
    }
}

ECode TimeZones::GetDisplayName(
    /* [in] */ ArrayOf<IArrayOf*>*  zoneStrings,
    /* [in] */ const String& id,
    /* [in] */ Boolean daylight,
    /* [in] */ Int32 style ,
    /* [out] */ String * str)
{
    VALIDATE_NOT_NULL(zoneStrings)
    VALIDATE_NOT_NULL(str)

    Int32 length = zoneStrings->GetLength();
    AutoPtr< ArrayOf<String> > arrstr = ArrayOf<String>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<IArrayOf> row = (*zoneStrings)[i];
        AutoPtr<IInterface> outface;
        row->Get(0, (IInterface**)&outface);
        String outstr;
        if (ICharSequence::Probe(outface) != NULL) {
            ICharSequence::Probe(outface)->ToString(&outstr);
        }
        (*arrstr)[i] = outstr;
    }
    Int32 index = arrstr->BinarySearch(id);
    if (index > 0) {
        AutoPtr<IArrayOf> row = (*zoneStrings)[index];
        AutoPtr<IInterface> outface;
        if (daylight) {
            if (style == TimeZone_LONG) {
                row->Get(ITimeZones::LONG_NAME_DST, (IInterface**)&outface);
            }
            else {
                row->Get(ITimeZones::SHORT_NAME_DST, (IInterface**)&outface);
            }
        }
        else {
            if (style == TimeZone_LONG) {
                row->Get(ITimeZones::LONG_NAME, (IInterface**)&outface);
            }
            else {
                row->Get(ITimeZones::SHORT_NAME, (IInterface**)&outface);
            }
        }
        if (ICharSequence::Probe(outface) != NULL) {
            return ICharSequence::Probe(outface)->ToString(str);
        }
    }
    *str = String(NULL);
    return NOERROR;
}

ECode TimeZones::GetZoneStrings(
    /* [in] */ ILocale * locale,
    /* [out,callee] */ ArrayOf<IArrayOf*> ** outarray)
{
    VALIDATE_NOT_NULL(outarray)

    if (locale == NULL) {
        AutoPtr<ILocaleHelper> lh;
        CLocaleHelper::AcquireSingleton((ILocaleHelper **)&lh);
        lh->GetDefault((ILocale **)&locale);
    }
    AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > > result = sCachedZoneStrings->Create(locale);
    Int32 length = result->GetLength();
    AutoPtr< ArrayOf<IArrayOf*> > resarr = ArrayOf<IArrayOf*>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr< ArrayOf<String> > minarr = (*result)[i];
        Int32 midlen = minarr->GetLength();
        AutoPtr<IArrayOf> arrof;
        FAIL_RETURN(CArrayOf::New(EIID_ICharSequence, midlen, (IArrayOf**)&arrof));
        for (Int32 j = 0; j < midlen; ++j) {
            AutoPtr<ICharSequence> sq;
            CStringWrapper::New((*minarr)[j], (ICharSequence**)&sq);
            arrof->Put(j, sq);
        }
        resarr->Set(i, arrof);
    }
    *outarray = resarr;
    REFCOUNT_ADD(*outarray);

    return NOERROR;
}

ECode TimeZones::ForLocale(
    /* [in] */ ILocale * locale ,
    /* [out,callee] */ ArrayOf<String> ** outarray)
{
    VALIDATE_NOT_NULL(locale)
    VALIDATE_NOT_NULL(outarray)

    String country;
    locale->GetCountry(&country);
    AutoPtr<ArrayOf<String> > res = ForCountryCode(country);
    *outarray = res;
    REFCOUNT_ADD(*outarray);
    return NOERROR;
}

AutoPtr<ArrayOf<String> > TimeZones::ForCountryCode(
    /* [in] */ const String& countryCode)
{
    if (countryCode.IsNull()) {
        return NULL;
    }

    UniquePtr<StringEnumeration> ids(TimeZone::createEnumeration(countryCode.string()));
    if (ids.get() == NULL) {
        return NULL;
    }
    UErrorCode status = U_ZERO_ERROR;
    int32_t idCount = ids->count(status);
    if (!U_SUCCESS(status)) {
        return NULL;
    }

    AutoPtr<ArrayOf<String> > result = ArrayOf<String>::Alloc(idCount);
    for (int32_t i = 0; i < idCount; ++i) {
        const UnicodeString* id = ids->snext(status);
        if (!U_SUCCESS(status)) {
            return NULL;
        }
        String str((const char*) id->getBuffer() , id->length());
        (*result)[i] = str;
    }
    return result;
}

struct TimeZoneNames {
    TimeZone* tz;

    UnicodeString longStd;
    UnicodeString shortStd;
    UnicodeString longDst;
    UnicodeString shortDst;

    UDate standardDate;
    UDate daylightSavingDate;
};

static bool isUtc(const UnicodeString& id) {
    static UnicodeString etcUct("Etc/UCT", 7, US_INV);
    static UnicodeString etcUtc("Etc/UTC", 7, US_INV);
    static UnicodeString etcUniversal("Etc/Universal", 13, US_INV);
    static UnicodeString etcZulu("Etc/Zulu", 8, US_INV);

    static UnicodeString uct("UCT", 3, US_INV);
    static UnicodeString utc("UTC", 3, US_INV);
    static UnicodeString universal("Universal", 9, US_INV);
    static UnicodeString zulu("Zulu", 4, US_INV);

    return id == etcUct || id == etcUtc || id == etcUniversal || id == etcZulu ||
            id == uct || id == utc || id == universal || id == zulu;
}

AutoPtr< ArrayOf< AutoPtr< ArrayOf<String> > > > TimeZones::GetZoneStringsImpl(
    /* [in] */ const String& localeName,
    /* [in] */ ArrayOf<String>* timeZoneIds)
{
    Locale locale = Locale::createFromName(localeName);

    // We could use TimeZone::getDisplayName, but that's even slower
    // because it creates a new SimpleDateFormat each time.
    // We're better off using SimpleDateFormat directly.

    // We can't use DateFormatSymbols::getZoneStrings because that
    // uses its own set of time zone ids and contains empty strings
    // instead of GMT offsets (a pity, because it's a bit faster than this code).

    UErrorCode status = U_ZERO_ERROR;
    UnicodeString longPattern("zzzz", 4, US_INV);
    SimpleDateFormat longFormat(longPattern, locale, status);
    // 'z' only uses "common" abbreviations. 'V' allows all known abbreviations.
    // For example, "PST" is in common use in en_US, but "CET" isn't.
    UnicodeString commonShortPattern("z", 1, US_INV);
    SimpleDateFormat shortFormat(commonShortPattern, locale, status);
    UnicodeString allShortPattern("V", 1, US_INV);
    SimpleDateFormat allShortFormat(allShortPattern, locale, status);

    UnicodeString utc("UTC", 3, US_INV);

    // TODO: use of fixed dates prevents us from using the correct historical name when formatting dates.
    // TODO: use of dates not in the current year could cause us to output obsoleted names.
    // 15th January 2008
    UDate date1 = 1203105600000.0;
    // 15th July 2008
    UDate date2 = 1218826800000.0;

    // In the first pass, we get the long names for the time zone.
    // We also get any commonly-used abbreviations.
    Vector<TimeZoneNames> table;
    typedef Map<UnicodeString, UnicodeString*> AbbreviationMap;
    AbbreviationMap usedAbbreviations;
    size_t idCount = timeZoneIds->GetLength();
    for (size_t i = 0; i < idCount; ++i) {
        UnicodeString id = UnicodeString::fromUTF8((*timeZoneIds)[i].string());

        TimeZoneNames row;
        if (isUtc(id)) {
            // ICU doesn't have names for the UTC zones; it just says "GMT+00:00" for both
            // long and short names. We don't want this. The best we can do is use "UTC"
            // for everything (since we don't know how to say "Universal Coordinated Time").
            row.tz = NULL;
            row.longStd = row.shortStd = row.longDst = row.shortDst = utc;
            table.PushBack(row);
            usedAbbreviations[utc] = &utc;
            continue;
        }

        row.tz = TimeZone::createTimeZone(id);
        longFormat.setTimeZone(*row.tz);
        shortFormat.setTimeZone(*row.tz);

        int32_t daylightOffset;
        int32_t rawOffset;
        row.tz->getOffset(date1, false, rawOffset, daylightOffset, status);
        if (daylightOffset != 0) {
            // The TimeZone is reporting that we are in daylight time for the winter date.
            // The dates are for the wrong hemisphere, so swap them.
            row.standardDate = date2;
            row.daylightSavingDate = date1;
        }
        else {
            row.standardDate = date1;
            row.daylightSavingDate = date2;
        }

        longFormat.format(row.standardDate, row.longStd);
        shortFormat.format(row.standardDate, row.shortStd);
        if (row.tz->useDaylightTime()) {
            longFormat.format(row.daylightSavingDate, row.longDst);
            shortFormat.format(row.daylightSavingDate, row.shortDst);
        }
        else {
            row.longDst = row.longStd;
            row.shortDst = row.shortStd;
        }

        table.PushBack(row);
        usedAbbreviations[row.shortStd] = &row.longStd;
        usedAbbreviations[row.shortDst] = &row.longDst;
    }

    // In the second pass, we create the Java String[][].
    // We also look for any uncommon abbreviations that don't conflict with ones we've already seen.
    AutoPtr<ArrayOf<AutoPtr<ArrayOf<String> > > > result = ArrayOf<AutoPtr<ArrayOf<String> > >::Alloc(idCount);
    UnicodeString gmt("GMT", 3, US_INV);
    for (size_t i = 0; i < table.GetSize(); ++i) {
        TimeZoneNames& row(table[i]);
        // Did we get a GMT offset instead of an abbreviation?
        if (row.shortStd.length() > 3 && row.shortStd.startsWith(gmt)) {
            // See if we can do better...
            UnicodeString uncommonStd, uncommonDst;
            allShortFormat.setTimeZone(*row.tz);
            allShortFormat.format(row.standardDate, uncommonStd);
            if (row.tz->useDaylightTime()) {
                allShortFormat.format(row.daylightSavingDate, uncommonDst);
            }
            else {
                uncommonDst = uncommonStd;
            }

            // If this abbreviation isn't already in use, we can use it.
            AbbreviationMap::Iterator it = usedAbbreviations.Find(uncommonStd);
            if (it == usedAbbreviations.End() || *(usedAbbreviations[uncommonStd]) == row.longStd) {
                row.shortStd = uncommonStd;
                usedAbbreviations[row.shortStd] = &row.longStd;
            }
            it = usedAbbreviations.Find(uncommonDst);
            if (it == usedAbbreviations.End() || *(usedAbbreviations[uncommonDst]) == row.longDst) {
                row.shortDst = uncommonDst;
                usedAbbreviations[row.shortDst] = &row.longDst;
            }
        }
        // Fill in whatever we got.
        AutoPtr<ArrayOf<String> > midarray = ArrayOf<String>::Alloc(5);
        (*midarray)[0] = (*timeZoneIds)[i];
        String str("");
        ElStringByteSink sink(&str);
        row.longStd.toUTF8(sink);
        (*midarray)[1] = str;
        str = "";
        row.shortStd.toUTF8(sink);
        (*midarray)[2] = str;
        str = "";
        row.longDst.toUTF8(sink);
        (*midarray)[3] = str;
        str = "";
        row.shortDst.toUTF8(sink);
        (*midarray)[4] = str;
        result->Set(i , midarray);
        delete row.tz;
    }

    return result;
}

} // namespace ICU
} // namespace Libcore
