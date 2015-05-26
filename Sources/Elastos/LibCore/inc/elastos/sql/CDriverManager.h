
#ifndef __CDRIVERMANAGER_H__
#define __CDRIVERMANAGER_H__

#include "_CDriverManager.h"
#include <coredef.h>
#include <elastos.h>
#include <elastos/List.h>
#include <elastos/Mutex.h>
#include "CPrintStream.h"
#include "CPrintWriter.h"
#include "CProperties.h"

using Elastos::IO::IPrintStream;
using Elastos::IO::IPrintWriter;
using Elastos::IO::CPrintStream;
using Elastos::IO::CPrintWriter;
using Elastos::Utility::IProperties;
using Elastos::Utility::IEnumeration;
using Elastos::Utility::List;
using Elastos::Core::Mutex;
using Elastos::Core::IClassLoader;

namespace Elastos {
namespace Sql {

CarClass(CDriverManager)
    , public Object
    , public IDriverManagerHelper
{
public:
    CAR_SINGLETON_DECL();

    CARAPI DeregisterDriver(
        /* [in] */ IDriver * driver);

    CARAPI GetConnection(
        /* [in] */ const String& url,
        /* [out] */ IConnection ** conn);

    CARAPI GetConnection(
        /* [in] */ const String& url,
        /* [in] */ IProperties * info,
        /* [out] */ IConnection ** conn);

    CARAPI GetConnection(
        /* [in] */ const String& url,
        /* [in] */ const String& user,
        /* [in] */ const String& password,
        /* [out] */ IConnection ** conn);

    CARAPI GetDriver(
        /* [in] */ const String& url,
        /* [out] */ IDriver ** driver);

    CARAPI GetDrivers(
        /* [out] */ IEnumeration ** enumeration);

    CARAPI GetLoginTimeout(
        /* [out] */ Int32 * value);

    CARAPI GetLogStream(
        /* [out] */ IPrintStream ** pntstream);

    CARAPI GetLogWriter(
        /* [out] */ IPrintWriter ** pntwriter);

    CARAPI Println(
        /* [in] */ const String& message);

    CARAPI RegisterDriver(
        /* [in] */ IDriver * driver);

    CARAPI SetLoginTimeout(
        /* [in] */ Int32 seconds);

    CARAPI SetLogStream(
        /* [in] */ IPrintStream * pntstream);

    CARAPI SetLogWriter(
        /* [in] */ IPrintWriter * pntwriter);

    CDriverManager();

private:

    static CARAPI_(Boolean) LoadInitialDrivers();

    static CARAPI_(Boolean) IsClassFromClassLoader(
        /* [in] */ IInterface * theObject,
        /* [in] */ IClassLoader * theClassLoader);

private:
    /*
     * Facilities for logging. The Print Stream is deprecated but is maintained
     * here for compatibility.
     */
    static AutoPtr<IPrintStream> thePrintStream;

    static AutoPtr<IPrintWriter> thePrintWriter;

    // Login timeout value - by default set to 0 -> "wait forever"
    static Int32 loginTimeout;

    /*
     * Set to hold Registered Drivers - initial capacity 10 drivers (will expand
     * automatically if necessary.
     */
    static List<AutoPtr<IDriver> > theDrivers;

    // Permission for setting log
    // const static SQLPermission logPermission = new SQLPermission("setLog");

    Mutex mLock;

public:
    static Boolean isinitflag;
};

} // namespace Sql
} // namespace Elastos

#endif // __CDRIVERMANAGER_H__
