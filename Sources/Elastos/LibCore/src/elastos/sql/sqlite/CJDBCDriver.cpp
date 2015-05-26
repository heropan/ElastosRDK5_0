
#include "CJDBCDriver.h"
#include "CDriverPropertyInfo.h"

using Elastos::Sql::CDriverPropertyInfo;

namespace Elastos {
namespace Sql {
namespace SQLite {

const Int32 CJDBCDriver::MAJORVERSION =1;

Boolean CJDBCDriver::sharedCache = FALSE;

String CJDBCDriver::vfs = String(NULL);

CAR_OBJECT_IMPL(CJDBCDriver);

CAR_INTERFACE_IMPL(CJDBCDriver, Object, IDriver);

Boolean CJDBCDriver::init()
{
    if (!isinit)
    {
        isinit = true;
        // try {
        //     Class connClass = null;
        //     Class args[] = new Class[5];
        //     args[0] = Class.forName("java.lang.String");
        //     args[1] = args[0];
        //     args[2] = args[0];
        //     args[3] = args[0];
        //     args[4] = args[0];
        //     String jvers = java.lang.System.getProperty("java.version");
        //     String cvers;
        //     if (jvers == null || jvers.startsWith("1.0")) {
        //     throw new java.lang.Exception("unsupported java version");
        //     } else if (jvers.startsWith("1.1")) {
        //     cvers = "SQLite.JDBC1.JDBCConnection";
        //     } else if (jvers.startsWith("1.2") || jvers.startsWith("1.3")) {
        //     cvers = "SQLite.JDBC2.JDBCConnection";
        //     } else if (jvers.startsWith("1.4")) {
        //     cvers = "SQLite.JDBC2x.JDBCConnection";
        //     } else if (jvers.startsWith("1.5")) {
        //     cvers = "SQLite.JDBC2y.JDBCConnection";
        //     try {
        //         Class.forName(cvers);
        //     } catch (java.lang.Exception e) {
        //         cvers = "SQLite.JDBC2x.JDBCConnection";
        //     }
        //     } else {
        //     cvers = "SQLite.JDBC2z.JDBCConnection";
        //     try {
        //         Class.forName(cvers);
        //     } catch (java.lang.Exception e) {
        //         cvers = "SQLite.JDBC2y.JDBCConnection";
        //         try {
        //         Class.forName(cvers);
        //         } catch (java.lang.Exception ee) {
        //         cvers = "SQLite.JDBC2x.JDBCConnection";
        //         }
        //     }
        //     }
        //     connClass = Class.forName(cvers);
        //     makeConn = connClass.getConstructor(args);
        //     java.sql.DriverManager.registerDriver(new JDBCDriver());
        //     try {
        //     String shcache =
        //         java.lang.System.getProperty("SQLite.sharedcache");
        //     if (shcache != null &&
        //         (shcache.startsWith("y") || shcache.startsWith("Y"))) {
        //         sharedCache = SQLite.Database._enable_shared_cache(true);
        //     }
        //     } catch (java.lang.Exception e) {
        //     }
        //     try {
        //     String tvfs =
        //         java.lang.System.getProperty("SQLite.vfs");
        //     if (tvfs != null) {
        //         vfs = tvfs;
        //     }
        //     } catch (java.lang.Exception e) {
        //     }
        // }
        // catch (java.lang.Exception e) {
        // System.err.println(e);
        // }
    }

    return isinit;
}

Boolean CJDBCDriver::isinit = init();

ECode CJDBCDriver::AcceptsURL(
    /* [in] */ const String& url,
    /* [out] */ Boolean * canOpen)
{
    *canOpen = url.StartWith("sqlite:/") ||
               url.StartWith("jdbc:sqlite:/");
    return NOERROR;
}

ECode CJDBCDriver::GetMajorVersion(
    /* [out] */ Int32 * ver)
{
    *ver = MAJORVERSION;
    return NOERROR;
}

ECode CJDBCDriver::GetMinorVersion(
    /* [out] */ Int32 * ver)
{
    *ver = IConstants::drv_minor;
    return NOERROR;
}

ECode CJDBCDriver::JdbcCompliant(
    /* [out] */ Boolean * isCompliant)
{
    *isCompliant = FALSE;
    return NOERROR;
}

ECode CJDBCDriver::constructor()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CJDBCDriver::Connect(
    /* [in] */ const String& url,
    /* [in] */ IProperties* info,
    /* [out] */ IConnection** connection)
{
    // if (!acceptsURL(url)) {
    //     return null;
    // }
    // Object args[] = new Object[5];
    // args[0] = url;
    // if (info != null) {
    //     args[1] = info.getProperty("encoding");
    //     args[2] = info.getProperty("password");
    //     args[3] = info.getProperty("daterepr");
    //     args[4] = info.getProperty("vfs");
    // }
    // if (args[1] == null) {
    //     args[1] = java.lang.System.getProperty("SQLite.encoding");
    // }
    // if (args[4] == null) {
    //     args[4] = vfs;
    // }
    // try {
    //     conn = (Connection) makeConn.newInstance(args);
    // } catch (java.lang.reflect.InvocationTargetException ie) {
    //     throw new SQLException(ie.getTargetException().toString());
    // } catch (java.lang.Exception e) {
    //     throw new SQLException(e.toString());
    // }
    // return conn;
    return NOERROR;
}

ECode CJDBCDriver::GetPropertyInfo(
     /* [in] */ const String& url,
     /* [in] */ IProperties* info,
     /* [out] */ ArrayOf<IDriverPropertyInfo*>** array)
{
    AutoPtr<ArrayOf<IDriverPropertyInfo *> > p = ArrayOf<IDriverPropertyInfo *>::Alloc(4);
    AutoPtr<IDriverPropertyInfo> pp;
    FAIL_RETURN(CDriverPropertyInfo::New(String("encoding"), String(""),(IDriverPropertyInfo **)&pp));
    (*p)[0] = pp;
    pp = NULL;
    FAIL_RETURN(CDriverPropertyInfo::New(String("password"), String(""),(IDriverPropertyInfo **)&pp));
    (*p)[1] = pp;
    pp = NULL;
    FAIL_RETURN(CDriverPropertyInfo::New(String("daterepr"), String("normal"),(IDriverPropertyInfo **)&pp));
    (*p)[2] = pp;
    pp = NULL;
    FAIL_RETURN(CDriverPropertyInfo::New(String("vfs"), vfs,(IDriverPropertyInfo **)&pp));
    (*p)[3] = pp;
    *array = p;
    return NOERROR;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
