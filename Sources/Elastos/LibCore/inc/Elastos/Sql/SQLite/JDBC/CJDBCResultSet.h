
#ifndef __CJDBCRESULTSET_H__
#define __CJDBCRESULTSET_H__

#include "_CJDBCResultSet.h"
#include "CTableResult.h"
#include "Elastos.Core_server.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IReader;
using Elastos::Math::IBigDecimal;
using Elastos::Net::IURL;
using Elastos::Utility::ICalendar;
using Elastos::Utility::IObjectStringMap;
using Elastos::Sql::ISQLWarning;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger16;
using Elastos::Core::IInteger64;
using Elastos::Core::IDouble;
using Elastos::Core::IFloat;

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CarClass(CJDBCResultSet)
{
public:
    CARAPI Absolute(
        /* [in] */ Int32 row,
        /* [out] */ Boolean * value);

    CARAPI AfterLast();

    CARAPI BeforeFirst();

    CARAPI CancelRowUpdates();

    CARAPI ClearWarnings();

    CARAPI Close();

    CARAPI DeleteRow();

    CARAPI FindColumn(
        /* [in] */ const String& colName);

    CARAPI First(
        /* [out] */ Boolean *value);

    CARAPI GetAsciiStream(
        /* [in] */ Int32 colIndex,
        /* [out] */ IInputStream ** iinputstream);

    CARAPI GetAsciiStreamEx(
        /* [in] */ const String& colName,
        /* [out] */ IInputStream ** iinputstream);

    CARAPI GetBigDecimal(
        /* [in] */ Int32 colIndex,
        /* [out] */ IBigDecimal ** bigdecimal);

    CARAPI GetBigDecimalEx(
        /* [in] */ Int32 colIndex,
        /* [in] */ Int32 scale,
        /* [out] */ IBigDecimal ** bigdecimal);

    CARAPI GetBigDecimalEx2(
        /* [in] */ const String& colName,
        /* [out] */ IBigDecimal ** bigdecimal);

    CARAPI GetBigDecimalEx3(
        /* [in] */ const String& colName,
        /* [in] */ Int32 scale,
        /* [out] */ IBigDecimal ** bigdecimal);

    CARAPI GetBinaryStream(
        /* [in] */ Int32 colIndex,
        /* [out] */ IInputStream ** iinputstream);

    CARAPI GetBinaryStreamEx(
        /* [in] */ const String& colName,
        /* [out] */ IInputStream ** iinputstream);

    CARAPI GetBlob(
        /* [in] */ Int32 colIndex,
        /* [out] */ Elastos::Sql::IBlob ** oblob);

    CARAPI GetBlobEx(
        /* [in] */ const String& colName,
        /* [out] */ Elastos::Sql::IBlob ** oblob);

    CARAPI GetBoolean(
        /* [in] */ Int32 colIndex,
        /* [out] */ Boolean * value);

    CARAPI GetBooleanEx(
        /* [in] */ const String& colName,
        /* [out] */ Boolean * value);

    CARAPI GetByte(
        /* [in] */ Int32 colIndex,
        /* [out] */ Byte * value);

    CARAPI GetByteEx(
        /* [in] */ const String& colName,
        /* [out] */ Byte * value);

    CARAPI GetBytes(
        /* [in] */ Int32 colIndex,
        /* [out, callee] */ ArrayOf<Byte> ** oarray);

    CARAPI GetBytesEx(
        /* [in] */ const String& colName,
        /* [out, callee] */ ArrayOf<Byte> ** oarray);

    CARAPI GetCharacterStream(
        /* [in] */ Int32 colIndex,
        /* [out] */ IReader ** reader);

    CARAPI GetCharacterStreamEx(
        /* [in] */ const String& colName,
        /* [out] */ IReader ** reader);

    CARAPI GetClob(
        /* [in] */ Int32 colIndex,
        /* [out] */ IClob ** oclob);

    CARAPI GetClobEx(
        /* [in] */ const String& colName,
        /* [out] */ IClob ** oclob);

    CARAPI GetConcurrency(
        /* [out] */ Int32 * mode);

    CARAPI GetCursorName(
        /* [out] */ String * name);

    CARAPI GetDouble(
        /* [in] */ Int32 colIndex,
        /* [out] */ Double * value);

    CARAPI GetDoubleEx(
        /* [in] */ const String& colName,
        /* [out] */ Double * value);

    CARAPI GetFetchDirection(
        /* [out] */ Int32 * value);

    CARAPI GetFetchSize(
        /* [out] */ Int32 * size);

    CARAPI GetFloat(
        /* [in] */ Int32 colIndex,
        /* [out] */ Float * value);

    CARAPI GetFloatEx(
        /* [in] */ const String& colName,
        /* [out] */ Float * value);

    CARAPI GetInt(
        /* [in] */ Int32 colIndex,
        /* [out] */ Int32 * value);

    CARAPI GetIntEx(
        /* [in] */ const String& colName,
        /* [out] */ Int32 * value);

    CARAPI GetLong(
        /* [in] */ Int32 colIndex,
        /* [out] */ Int64 * value);

    CARAPI GetLongEx(
        /* [in] */ const String& colName,
        /* [out] */ Int64 * value);

    CARAPI GetMetaData(
        /* [out] */ IResultSetMetaData ** metaData);

    CARAPI GetObject(
        /* [in] */ Int32 colIndex,
        /* [out] */ IInterface ** obj);

    CARAPI GetRef(
        /* [in] */ Int32 colIndex,
        /* [out] */ IRef ** ref);

    CARAPI GetRefEx(
        /* [in] */ const String& colName,
        /* [out] */ IRef ** ref);

    CARAPI GetRow(
        /* [out] */ Int32 * value);

    CARAPI GetShort(
        /* [in] */ Int32 colIndex,
        /* [out] */ Int16 * value);

    CARAPI GetShortEx(
        /* [in] */ const String& colName,
        /* [out] */ Int16 * value);

    CARAPI GetStatement(
        /* [out] */ IStatement ** ppStatement);

    CARAPI GetString(
        /* [in] */ Int32 colIndex,
        /* [out] */ String * value);

    CARAPI GetStringEx(
        /* [in] */ const String& colName,
        /* [out] */ String * value);

    CARAPI GetTime(
        /* [in] */ Int32 colIndex,
        /* [out] */ ITime ** sqltime);

    CARAPI GetTimeEx(
        /* [in] */ Int32 colIndex,
        /* [in] */ ICalendar * pCal,
        /* [out] */ ITime ** sqltime);

    CARAPI GetTimeEx2(
        /* [in] */ const String& colName,
        /* [out] */ ITime ** sqltime);

    CARAPI GetTimeEx3(
        /* [in] */ const String& colName,
        /* [in] */ ICalendar * pCal,
        /* [out] */ ITime ** sqltime);

    CARAPI GetTimestamp(
        /* [in] */ Int32 colIndex,
        /* [out] */ ITimestamp ** timemestamp);

    CARAPI GetTimestampEx(
        /* [in] */ Int32 colIndex,
        /* [in] */ ICalendar * pCal,
        /* [out] */ ITimestamp ** timemestamp);

    CARAPI GetTimestampEx2(
        /* [in] */ const String& colName,
        /* [out] */ ITimestamp ** timemestamp);

    CARAPI GetTimestampEx3(
        /* [in] */ const String& colName,
        /* [in] */ ICalendar * pCal,
        /* [out] */ ITimestamp ** timemestamp);

    CARAPI GetType(
        /* [out] */ Int32 * value);

    CARAPI GetUnicodeStream(
        /* [in] */ Int32 colIndex,
        /* [out] */ IInputStream ** iinputstream);

    CARAPI GetUnicodeStreamEx(
        /* [in] */ const String& colName,
        /* [out] */ IInputStream ** iinputstream);

    CARAPI GetWarnings(
        /* [out] */ ISQLWarning ** sqlwarning);

    CARAPI InsertRow();

    CARAPI IsAfterLast(
        /* [out] */ Boolean * value);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean * value);

    CARAPI IsFirst(
        /* [out] */ Boolean * value);

    CARAPI IsLast(
        /* [out] */ Boolean * value);

    CARAPI Last(
        /* [out] */ Boolean * value);

    CARAPI MoveToCurrentRow();

    CARAPI MoveToInsertRow();

    CARAPI Next(
        /* [out] */ Boolean * value);

    CARAPI Previous(
        /* [out] */ Boolean * value);

    CARAPI RefreshRow();

    CARAPI Relative(
        /* [in] */ Int32 rows,
        /* [out] */ Boolean * value);

    CARAPI RowDeleted(
        /* [out] */ Boolean * value);

    CARAPI RowInserted(
        /* [out] */ Boolean * value);

    CARAPI RowUpdated(
        /* [out] */ Boolean * value);

    CARAPI SetFetchDirection(
        /* [in] */ Int32 direction);

    CARAPI SetFetchSize(
        /* [in] */ Int32 rows);

    CARAPI UpdateArray(
        /* [in] */ Int32 colIndex,
        /* [in] */ IArray * x);

    CARAPI UpdateArrayEx(
        /* [in] */ const String& colName,
        /* [in] */ IArray * x);

    CARAPI UpdateAsciiStream(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int32 length);

    CARAPI UpdateAsciiStreamEx(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int32 length);

    CARAPI UpdateBigDecimal(
        /* [in] */ Int32 colIndex,
        /* [in] */ IBigDecimal * x);

    CARAPI UpdateBigDecimalEx(
        /* [in] */ const String& colName,
        /* [in] */ IBigDecimal * x);

    CARAPI UpdateBinaryStream(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int32 length);

    CARAPI UpdateBinaryStreamEx(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int32 length);

    CARAPI UpdateBlob(
        /* [in] */ Int32 colIndex,
        /* [in] */ Elastos::Sql::IBlob * x);

    CARAPI UpdateBlobEx(
        /* [in] */ const String& colName,
        /* [in] */ Elastos::Sql::IBlob * x);

    CARAPI UpdateBoolean(
        /* [in] */ Int32 colIndex,
        /* [in] */ Boolean x);

    CARAPI UpdateBooleanEx(
        /* [in] */ const String& colName,
        /* [in] */ Boolean x);

    CARAPI UpdateByte(
        /* [in] */ Int32 colIndex,
        /* [in] */ Byte x);

    CARAPI UpdateByteEx(
        /* [in] */ const String& colName,
        /* [in] */ Byte x);

    CARAPI UpdateBytes(
        /* [in] */ Int32 colIndex,
        /* [in] */ const ArrayOf<Byte> & x);

    CARAPI UpdateBytesEx(
        /* [in] */ const String& colName,
        /* [in] */ const ArrayOf<Byte> & x);

    CARAPI UpdateCharacterStream(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int32 length);

    CARAPI UpdateCharacterStreamEx(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader,
        /* [in] */ Int32 length);

    CARAPI UpdateClob(
        /* [in] */ Int32 colIndex,
        /* [in] */ IClob * x);

    CARAPI UpdateClobEx(
        /* [in] */ const String& colName,
        /* [in] */ IClob * x);

    CARAPI UpdateDate(
        /* [in] */ Int32 colIndex,
        /* [in] */ IDate * x);

    CARAPI UpdateDateEx(
        /* [in] */ const String& colName,
        /* [in] */ IDate * x);

    CARAPI UpdateDouble(
        /* [in] */ Int32 colIndex,
        /* [in] */ Double x);

    CARAPI UpdateDoubleEx(
        /* [in] */ const String& colName,
        /* [in] */ Double x);

    CARAPI UpdateFloat(
        /* [in] */ Int32 colIndex,
        /* [in] */ Float x);

    CARAPI UpdateFloatEx(
        /* [in] */ const String& colName,
        /* [in] */ Float x);

    CARAPI UpdateInt(
        /* [in] */ Int32 colIndex,
        /* [in] */ Int32 x);

    CARAPI UpdateIntEx(
        /* [in] */ const String& colName,
        /* [in] */ Int32 x);

    CARAPI UpdateLong(
        /* [in] */ Int32 colIndex,
        /* [in] */ Int64 x);

    CARAPI UpdateLongEx(
        /* [in] */ const String& colName,
        /* [in] */ Int64 x);

    CARAPI UpdateNull(
        /* [in] */ Int32 colIndex);

    CARAPI UpdateNullEx(
        /* [in] */ const String& colName);

    CARAPI UpdateObject(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInterface * x);

    CARAPI UpdateObjectEx(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInterface * x,
        /* [in] */ Int32 scale);

    CARAPI UpdateObjectEx2(
        /* [in] */ const String& colName,
        /* [in] */ IInterface * x);

    CARAPI UpdateObjectEx3(
        /* [in] */ const String& colName,
        /* [in] */ IInterface * x,
        /* [in] */ Int32 scale);

    CARAPI UpdateRef(
        /* [in] */ Int32 colIndex,
        /* [in] */ IRef * x);

    CARAPI UpdateRefEx(
        /* [in] */ const String& colName,
        /* [in] */ IRef * x);

    CARAPI UpdateRow();

    CARAPI UpdateShort(
        /* [in] */ Int32 colIndex,
        /* [in] */ Int16 x);

    CARAPI UpdateShortEx(
        /* [in] */ const String& colName,
        /* [in] */ Int16 x);

    CARAPI UpdateString(
        /* [in] */ Int32 colIndex,
        /* [in] */ const String& x);

    CARAPI UpdateStringEx(
        /* [in] */ const String& colName,
        /* [in] */ const String& x);

    CARAPI UpdateTime(
        /* [in] */ Int32 colIndex,
        /* [in] */ ITime * x);

    CARAPI UpdateTimeEx(
        /* [in] */ const String& colName,
        /* [in] */ ITime * x);

    CARAPI UpdateTimestamp(
        /* [in] */ Int32 colIndex,
        /* [in] */ ITimestamp * x);

    CARAPI UpdateTimestampEx(
        /* [in] */ const String& colName,
        /* [in] */ ITimestamp * x);

    CARAPI WasNull(
        /* [out] */ Boolean * pIsNull);

    CARAPI GetRowId(
        /* [in] */ Int32 colIndex,
        /* [out] */ IRowId ** rowid);

    CARAPI GetRowIdEx(
        /* [in] */ const String& colName,
        /* [out] */ IRowId ** rowid);

    CARAPI UpdateRowId(
        /* [in] */ Int32 colIndex,
        /* [in] */ IRowId * value);

    CARAPI UpdateRowIdEx(
        /* [in] */ const String& colName,
        /* [in] */ IRowId * value);

    CARAPI GetHoldability(
        /* [out] */ Int32 * value);

    CARAPI IsClosed(
        /* [out] */ Boolean * value);

    CARAPI UpdateNString(
        /* [in] */ Int32 colIndex,
        /* [in] */ const String& nString);

    CARAPI UpdateNStringEx(
        /* [in] */ const String& colName,
        /* [in] */ const String& nString);

    CARAPI UpdateNClob(
        /* [in] */ Int32 colIndex,
        /* [in] */ INClob * nclob);

    CARAPI UpdateNClobEx(
        /* [in] */ const String& colName,
        /* [in] */ INClob * nclob);

    CARAPI GetNClob(
        /* [in] */ Int32 colIndex,
        /* [out] */ INClob ** nclob);

    CARAPI GetNClobEx(
        /* [in] */ const String& colName,
        /* [out] */ INClob ** nclob);

    CARAPI GetSQLXML(
        /* [in] */ Int32 colIndex,
        /* [out] */ ISQLXML ** sqlxml);

    CARAPI GetSQLXMLEx(
        /* [in] */ const String& colName,
        /* [out] */ ISQLXML ** sqlxml);

    CARAPI UpdateSQLXML(
        /* [in] */ Int32 colIndex,
        /* [in] */ ISQLXML * xmlObject);

    CARAPI UpdateSQLXMLEx(
        /* [in] */ const String& colName,
        /* [in] */ ISQLXML * xmlObject);

    CARAPI GetNString(
        /* [in] */ Int32 colIndex,
        /* [out] */ String * nstr);

    CARAPI GetNStringEx(
        /* [in] */ const String& colName,
        /* [out] */ String * nstr);

    CARAPI GetNCharacterStream(
        /* [in] */ Int32 colIndex,
        /* [out] */ IReader ** reader);

    CARAPI GetNCharacterStreamEx(
        /* [in] */ const String& colName,
        /* [out] */ IReader ** reader);

    CARAPI UpdateNCharacterStream(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int64 length);

    CARAPI UpdateNCharacterStreamEx(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI UpdateAsciiStreamEx2(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 length);

    CARAPI UpdateBinaryStreamEx2(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 length);

    CARAPI UpdateCharacterStreamEx2(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int64 length);

    CARAPI UpdateAsciiStreamEx3(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 length);

    CARAPI UpdateBinaryStreamEx3(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 length);

    CARAPI UpdateCharacterStreamEx3(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI UpdateBlobEx2(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * pInputStream,
        /* [in] */ Int64 length);

    CARAPI UpdateBlobEx3(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * pInputStream,
        /* [in] */ Int64 length);

    CARAPI UpdateClobEx2(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI UpdateClobEx3(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI UpdateNClobEx2(
        /* [in] */ Int64 colIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI UpdateNClobEx3(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI UpdateNCharacterStreamEx2(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * x);

    CARAPI UpdateNCharacterStreamEx3(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader);

    CARAPI UpdateAsciiStreamEx4(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * x);

    CARAPI UpdateBinaryStreamEx4(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * x);

    CARAPI UpdateCharacterStreamEx4(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * px);

    CARAPI UpdateAsciiStreamEx5(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * x);

    CARAPI UpdateBinaryStreamEx5(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * x);

    CARAPI UpdateCharacterStreamEx5(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader);

    CARAPI UpdateBlobEx4(
        /* [in] */ Int32 colIndex,
        /* [in] */ IInputStream * pInputStream);

    CARAPI UpdateBlobEx5(
        /* [in] */ const String& colName,
        /* [in] */ IInputStream * pInputStream);

    CARAPI UpdateClobEx4(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * reader);

    CARAPI UpdateClobEx5(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader);

    CARAPI UpdateNClobEx4(
        /* [in] */ Int32 colIndex,
        /* [in] */ IReader * reader);

    CARAPI UpdateNClobEx5(
        /* [in] */ const String& colName,
        /* [in] */ IReader * reader);

    CARAPI constructor(
        /* [in] */ ITableResult * pTr,
        /* [in] */ IJDBCStatement * pS);

    CARAPI IsWrapperFor(
        /* [in] */ IInterface * iface,
        /* [out] */ Boolean * value);

    CARAPI GetArray(
        /* [in] */ Int32 colIndex,
        /* [out] */ IArray ** iarray);

    CARAPI GetArrayEx(
        /* [in] */ const String& colName,
        /* [out] */ IArray ** iarray);

    CARAPI GetDate(
        /* [in] */ Int32 colIndex,
        /* [out] */ IDate ** idate);

    CARAPI GetDateEx(
        /* [in] */ Int32 colIndex,
        /* [in] */ ICalendar * cal,
        /* [out] */ IDate ** idate);

    CARAPI GetDateEx2(
        /* [in] */ const String& colName,
        /* [out] */ IDate ** idate);

    CARAPI GetDateEx3(
        /* [in] */ const String& colName,
        /* [in] */ ICalendar * cal,
        /* [out] */ IDate ** idate);

    CARAPI GetObjectEx(
        /* [in] */ Int32 colIndex,
        /* [in] */ IObjectStringMap * map,
        /* [out] */ IInterface ** obj);

    CARAPI GetObjectEx2(
        /* [in] */ const String& colName,
        /* [out] */ IInterface** obj);

    CARAPI GetObjectEx3(
        /* [in] */ const String& colName,
        /* [in] */ IObjectStringMap * map,
        /* [out] */ IInterface** obj);

    CARAPI GetURL(
        /* [in] */ Int32 colIndex,
        /* [out] */ IURL** url);

    CARAPI GetURLEx(
        /* [in] */ const String& colName,
        /* [out] */ IURL** url);

    CARAPI Unwrap(
        /* [in] */ PInterface iface,
        /* [out] */ IInterface ** oface);

    CARAPI IsUpdatable(
        /* [out] */ Boolean * value);

    CARAPI FillRowbuf();

    CARAPI FindColumnEx(
        /* [in] */ const String& colName,
        /* [out] */ Int32 * value);

private:
    CARAPI_(AutoPtr<IInteger32>) InternalGetInt(
        /* [in] */ Int32 colIndex);

    CARAPI_(AutoPtr<IInteger16>) InternalGetShort(
        /* [in] */ Int32 colIndex);

    CARAPI_(AutoPtr<ITime>) InternalGetTime(
        /* [in] */ Int32 colIndex,
        /* [in] */ ICalendar * cal);

    CARAPI_(AutoPtr<ITimestamp>) InternalGetTimestamp(
        /* [in] */ Int32 colIndex,
        /* [in] */ ICalendar * cal);

    CARAPI_(AutoPtr<IDate>) InternalGetDate(
        /* [in] */ Int32 colIndex,
        /* [in] */ ICalendar * cal);

    CARAPI_(AutoPtr<IDouble>) InternalGetDouble(
        /* [in] */ Int32 colIndex);

    CARAPI_(AutoPtr<IFloat>) InternalGetFloat(
        /* [in] */ Int32 colIndex);

    CARAPI_(AutoPtr<IInteger64>) InternalGetLong(
        /* [in] */ Int32 colIndex);

public:
    AutoPtr<CTableResult> tr;

private:
    Int32 row;
    AutoPtr<IJDBCStatement> s;
    AutoPtr<IJDBCResultSetMetaData> md;
    String lastg;
    Int32 updatable;
    String uptable;
    AutoPtr<ArrayOf<String> > pkcols;
    AutoPtr<ArrayOf<Int32> > pkcoli;
    const static Int32 UPD_UNKNOWN = -1;
    const static Int32 UPD_NO = 0;
    const static Int32 UPD_INS = 1;
    const static Int32 UPD_INSUPDDEL = 2;
    Boolean oninsrow;
    AutoPtr<ArrayOf<String> > rowbuf;
    const static Boolean nullrepl;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CJDBCRESULTSET_H__
