
#ifndef __CJDBCDRIVER_H__
#define __CJDBCDRIVER_H__

#include "_Elastos_Sql_SQLite_CJDBCDriver.h"
#include "Object.h"

using Elastos::Utility::IProperties;
using Elastos::Sql::IConnection;
using Elastos::Sql::IDriverPropertyInfo;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CJDBCDriver)
    , public Object
    , public IDriver
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI AcceptsURL(
        /* [in] */ const String& url,
        /* [out] */ Boolean * canOpen);

    CARAPI GetMajorVersion(
        /* [out] */ Int32 * ver);

    CARAPI GetMinorVersion(
        /* [out] */ Int32 * ver);

    CARAPI JdbcCompliant(
        /* [out] */ Boolean * isCompliant);

    CARAPI constructor();

    CARAPI Connect(
        /* [in] */ const String& url,
        /* [in] */ IProperties* info,
        /* [out] */ IConnection** connection);

    CARAPI GetPropertyInfo(
        /* [in] */ const String& url,
        /* [in] */ IProperties* info,
        /* [out] */ ArrayOf<IDriverPropertyInfo*>** array);

public:
    const static Int32 MAJORVERSION;
    static Boolean sharedCache;
    static String vfs;
    AutoPtr<IConnection> conn;

private:
    //static java.lang.reflect.Constructor makeConn = null;
    static CARAPI_(Boolean) init();

private:
    static Boolean isinit;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CJDBCDRIVER_H__
