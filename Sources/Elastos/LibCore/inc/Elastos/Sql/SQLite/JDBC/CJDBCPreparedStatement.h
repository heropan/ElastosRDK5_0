
#ifndef __CJDBCPREPAREDSTATEMENT_H__
#define __CJDBCPREPAREDSTATEMENT_H__

#include "_CJDBCPreparedStatement.h"
#include "cmdef.h"
#include "JDBCStatement.h"
#include "CJDBCStatement.h"
#include "JDBCStatementMacro.h"
#include <elastos/List.h>

using Elastos::Net::IURL;
using Elastos::IO::IReader;
using Elastos::IO::IInputStream;
using Elastos::Math::IBigDecimal;
using Elastos::Utility::ICalendar;
using Elastos::Utility::IObjectMap;

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

class BatchArg : public ElLightRefBase {
public:
    BatchArg(String iarg, Boolean iblob) {
        if (iarg.IsNull()) {
            arg = String(NULL);
        } else {
            arg = iarg;
        }
        blob = iblob;
    }

public:
    String arg;
    Boolean blob;
};

CarClass(CJDBCPreparedStatement) , public JDBCStatement
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    JDBCSTATEMENT_METHODS_DECL()

    ~CJDBCPreparedStatement();

    CARAPI constructor(
        /* [in] */ IJDBCConnection * pConn,
        /* [in] */ const String& sql);

    CARAPI AddBatchEx();

    CARAPI ExecuteBatchEx(
        /* [out] */ ArrayOf<Int32> ** outarray);

    CARAPI ClearBatchEx();

    CARAPI CloseEx();

    CARAPI ClearParameters();

    CARAPI ExecuteEx4(
        /* [out] */ Boolean * value);

    CARAPI ExecuteQueryEx(
        /* [out] */ IResultSet ** resultset);

    CARAPI ExecuteUpdateEx4(
        /* [out] */ Int32 * value);

    CARAPI GetMetaData(
        /* [out] */ IResultSetMetaData ** resultsetmeta);

    CARAPI GetParameterMetaData(
        /* [out] */ IParameterMetaData ** parametermeta);

    CARAPI SetArray(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IArray * theArray);

    CARAPI SetAsciiStream(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * theInputStream,
        /* [in] */ Int32 length);

    CARAPI SetBigDecimal(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IBigDecimal * theBigDecimal);

    CARAPI SetBinaryStream(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * theInputStream,
        /* [in] */ Int32 length);

    CARAPI SetBlob(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Elastos::Sql::IBlob * theBlob);

    CARAPI SetBoolean(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Boolean theBoolean);

    CARAPI SetByte(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Byte theByte);

    CARAPI SetBytes(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ const ArrayOf<Byte>& theBytes);

    CARAPI SetCharacterStream(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int32 length);

    CARAPI SetClob(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IClob * theClob);

    CARAPI SetDate(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IDate * theDate);

    CARAPI SetDateEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IDate * theDate,
        /* [in] */ ICalendar * cal);

    CARAPI SetDouble(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Double theDouble);

    CARAPI SetFloat(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Float theFloat);

    CARAPI SetInt(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Int32 theInt);

    CARAPI SetLong(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Int64 theLong);

    CARAPI SetNull(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Int32 sqlType);

    CARAPI SetNullEx(
        /* [in] */ Int32 paramIndex,
        /* [in] */ Int32 sqlType,
        /* [in] */ const String& typeName);

    CARAPI SetObject(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IObject * theObject);

    CARAPI SetObjectEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IObject * theObject,
        /* [in] */ Int32 targetSqlType);

    CARAPI SetObjectEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IObject * theObject,
        /* [in] */ Int32 targetSqlType,
        /* [in] */ Int32 scale);

    CARAPI SetRef(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IRef * theRef);

    CARAPI SetShort(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ Int16 theShort);

    CARAPI SetString(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ const String& theString);

    CARAPI SetTime(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ITime * theTime);

    CARAPI SetTimeEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ITime * theTime,
        /* [in] */ ICalendar * cal);

    CARAPI SetTimestamp(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ITimestamp * theTimestamp);

    CARAPI SetTimestampEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ITimestamp * theTimestamp,
        /* [in] */ ICalendar * cal);

    CARAPI SetUnicodeStream(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * theInputStream,
        /* [in] */ Int32 length);

    CARAPI SetURL(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IURL * theURL);

    CARAPI SetRowId(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IRowId * theRowId);

    CARAPI SetNString(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ const String& theString);

    CARAPI SetNCharacterStream(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI SetNClob(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ INClob * value);

    CARAPI SetClobEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI SetBlobEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * inputStream,
        /* [in] */ Int64 length);

    CARAPI SetNClobEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI SetSQLXML(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ISQLXML * xmlObject);

    CARAPI SetAsciiStreamEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * inputStream,
        /* [in] */ Int64 length);

    CARAPI SetBinaryStreamEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * inputStream,
        /* [in] */ Int64 length);

    CARAPI SetCharacterStreamEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader,
        /* [in] */ Int64 length);

    CARAPI SetAsciiStreamEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * inputStream);

    CARAPI SetBinaryStreamEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * inputStream);

    CARAPI SetCharacterStreamEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader);

    CARAPI SetNCharacterStreamEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader);

    CARAPI SetClobEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader);

    CARAPI SetBlobEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * inputStream);

    CARAPI SetNClobEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * reader);

    CARAPI RegisterOutputParameter(
        /* [in] */ const String& parameterName,
        /* [in] */ Int32 sqlType);

    CARAPI RegisterOutputParameterEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Int32 sqlType,
        /* [in] */ Int32 scale);

    CARAPI RegisterOutputParameterEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ Int32 sqlType,
        /* [in] */ const String& typeName);

    CARAPI GetURL(
        /* [in] */ Int32 parameterIndex,
        /* [out] */ IURL ** url);

    CARAPI SetNullEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ Int32 sqlType);

    CARAPI SetBooleanEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Boolean val);

    CARAPI SetByteEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Byte val);

    CARAPI SetShortEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Int16 val);

    CARAPI SetIntEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Int32 val);

    CARAPI SetLongEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Int64 val);

    CARAPI SetFloatEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Float val);

    CARAPI SetDoubleEx(
        /* [in] */ const String& parameterName,
        /* [in] */ Double val);

    CARAPI SetBigDecimalEx(
        /* [in] */ const String& parameterName,
        /* [in] */ IBigDecimal * val);

    CARAPI SetStringEx(
        /* [in] */ const String& parameterName,
        /* [in] */ const String& val);

    CARAPI SetBytesEx(
        /* [in] */ const String& parameterName,
        /* [in] */ const ArrayOf<Byte>& val);

    CARAPI SetDateEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ IDate * val);

    CARAPI SetTimeEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ ITime * val);

    CARAPI SetTimestampEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ ITimestamp * val);

    CARAPI SetAsciiStreamEx6(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * s,
        /* [in] */ Int32 length);

    CARAPI SetBinaryStreamEx6(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * s,
        /* [in] */ Int32 length);

    CARAPI SetObjectEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ IObject * val,
        /* [in] */ Int32 tarGetSqlType,
        /* [in] */ Int32 scale);

    CARAPI SetObjectEx4(
        /* [in] */ const String& parameterName,
        /* [in] */ IObject * val,
        /* [in] */ Int32 tarGetSqlType);

    CARAPI SetObjectEx5(
        /* [in] */ const String& parameterName,
        /* [in] */ IObject * val);

    CARAPI SetCharacterStreamEx7(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * r,
        /* [in] */ Int32 length);

    CARAPI SetDateEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ IDate * val,
        /* [in] */ ICalendar * cal);

    CARAPI SetTimeEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ ITime * val,
        /* [in] */ ICalendar * cal);

    CARAPI SetTimestampEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ ITimestamp * val,
        /* [in] */ ICalendar * cal);

    CARAPI SetNullEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ Int32 sqlType,
        /* [in] */ const String& typeName);

    CARAPI GetString(
        /* [in] */ const String& parameterName,
        /* [out] */ String * str);

    CARAPI GetBoolean(
        /* [in] */ const String& parameterName,
        /* [out] */ Boolean * value);

    CARAPI GetByte(
        /* [in] */ const String& parameterName,
        /* [out] */ Byte * outbyte);

    CARAPI GetShort(
        /* [in] */ const String& parameterName,
        /* [out] */ Int16 * value);

    CARAPI GetInt(
        /* [in] */ const String& parameterName,
        /* [out] */ Int32 * value);

    CARAPI GetLong(
        /* [in] */ const String& parameterName,
        /* [out] */ Int64 * value);

    CARAPI GetFloat(
        /* [in] */ const String& parameterName,
        /* [out] */ Float * value);

    CARAPI GetDouble(
        /* [in] */ const String& parameterName,
        /* [out] */ Double * value);

    CARAPI GetBytes(
        /* [in] */ const String& parameterName,
        /* [out,callee] */ ArrayOf<Byte> ** outarray);

    CARAPI GetDate(
        /* [in] */ const String& parameterName,
        /* [out] */ IDate ** thedate);

    CARAPI GetTime(
        /* [in] */ const String& parameterName,
        /* [out] */ ITime ** thetime);

    CARAPI GetTimestamp(
        /* [in] */ const String& parameterName,
        /* [out] */ ITimestamp ** timestamp);

    CARAPI GetObject(
        /* [in] */ const String& parameterName,
        /* [out] */ IObject ** theobject);

    CARAPI GetObjectEx(
        /* [in] */ Int32 parameterIndex,
        /* [out] */ IObject ** theobject);

    CARAPI GetBigDecimal(
        /* [in] */ const String& parameterName,
        /* [out] */ IBigDecimal ** bigdecimal);

    CARAPI GetObjectEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ IObjectMap * map,
        /* [out] */ IObject ** theobject);

    CARAPI GetObjectEx3(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IObjectMap * map,
        /* [out] */ IObject ** theobject);

    CARAPI GetRef(
        /* [in] */ Int32 parameterIndex,
        /* [out] */ IRef ** iref);

    CARAPI GetRefEx(
        /* [in] */ const String& parameterName,
        /* [out] */ IRef ** iref);

    CARAPI GetBlob(
        /* [in] */ const String& parameterName,
        /* [out] */ Elastos::Sql::IBlob ** blob);

    CARAPI GetBlobEx(
        /* [in] */ Int32 parameterIndex,
        /* [out] */ Elastos::Sql::IBlob ** blob);

    CARAPI GetClob(
        /* [in] */ const String& parameterName,
        /* [out] */ IClob ** clob);

    CARAPI GetClobEx(
        /* [in] */ Int32 parameterIndex,
        /* [out] */ IClob ** clob);

    CARAPI GetArray(
        /* [in] */ const String& parameterName,
        /* [out] */ IArray ** outarray);

    CARAPI GetArrayEx(
        /* [in] */ Int32 parameterIndex,
        /* [out] */ IArray ** outarray);

    CARAPI GetDateEx(
        /* [in] */ const String& parameterName,
        /* [in] */ ICalendar * cal,
        /* [out] */ IDate ** thedate);

    CARAPI GetDateEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ICalendar * cal,
        /* [out] */ IDate ** thedate);

    CARAPI GetTimeEx(
        /* [in] */ const String& parameterName,
        /* [in] */ ICalendar * cal,
        /* [out] */ ITime ** thetime);

    CARAPI GetTimeEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ICalendar * cal,
        /* [out] */ ITime ** thetime);

    CARAPI GetTimestampEx(
        /* [in] */ const String& parameterName,
        /* [in] */ ICalendar * cal,
        /* [out] */ ITimestamp ** timestamp);

    CARAPI GetTimestampEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ICalendar * cal,
        /* [out] */ ITimestamp ** timestamp);

    CARAPI GetURLEx(
        /* [in] */ const String& parameterName,
        /* [out] */ IURL ** url);

    CARAPI SetRowIdEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IRowId * x);

    CARAPI SetRowIdEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ IRowId * x);

    CARAPI SetNStringEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ const String& value);

    CARAPI SetNStringEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ const String& value);

    CARAPI SetNCharacterStreamEx2(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetNCharacterStreamEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetNClobEx8(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ INClob * value);

    CARAPI SetNClobEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ INClob * value);

    CARAPI SetClobEx3(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetClobEx4(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetBlobEx3(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 len);

    CARAPI SetBlobEx4(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 len);

    CARAPI SetNClobEx4(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetNClobEx5(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetSQLXMLEx(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ ISQLXML * xml);

    CARAPI SetSQLXMLEx2(
        /* [in] */ const String& parameterName,
        /* [in] */ ISQLXML * xml);

    CARAPI SetAsciiStreamEx7(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 len);

    CARAPI SetAsciiStreamEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 len);

    CARAPI SetBinaryStreamEx7(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 len);

    CARAPI SetBinaryStreamEx3(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * x,
        /* [in] */ Int64 len);

    CARAPI SetCharacterStreamEx3(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetCharacterStreamEx4(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x,
        /* [in] */ Int64 len);

    CARAPI SetAsciiStreamEx4(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * x);

    CARAPI SetAsciiStreamEx5(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * x);

    CARAPI SetBinaryStreamEx4(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * x);

    CARAPI SetBinaryStreamEx5(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * x);

    CARAPI SetCharacterStreamEx5(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x);

    CARAPI SetCharacterStreamEx6(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x);

    CARAPI SetNCharacterStreamEx4(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x);

    CARAPI SetNCharacterStreamEx5(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x);

    CARAPI SetClobEx5(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x);

    CARAPI SetClobEx6(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x);

    CARAPI SetBlobEx5(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IInputStream * x);

    CARAPI SetBlobEx6(
        /* [in] */ const String& parameterName,
        /* [in] */ IInputStream * x);

    CARAPI SetNClobEx6(
        /* [in] */ Int32 parameterIndex,
        /* [in] */ IReader * x);

    CARAPI SetNClobEx7(
        /* [in] */ const String& parameterName,
        /* [in] */ IReader * x);

    CARAPI_(void) Init(
        /* [in] */ IJDBCConnection * pConn,
        /* [in] */ const String& isql);

private:
    CARAPI_(String) Fixup(
        /* [in] */ const String& sql);

    CARAPI_(String) Fixup2(
        /* [in] */ const String& sql);

private:
    String sql;
    AutoPtr<ArrayOf<String> > mArgs;
    AutoPtr<ArrayOf<Boolean> > mBlobs;
    AutoPtr<List<AutoPtr<BatchArg> > > mBatch;
    const static Boolean mNullrepl;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CJDBCPREPAREDSTATEMENT_H__
