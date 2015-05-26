
#include "CDriverManager.h"
#include "CSystem.h"
#include "Pattern.h"

using Elastos::Core::ISystem;
using Elastos::Utility::Regex::Pattern;
using Elastos::Utility::IProperties;
using Elastos::Utility::CProperties;

namespace Elastos {
namespace Sql {

List<AutoPtr<IDriver> > CDriverManager::theDrivers;
Int32 CDriverManager::loginTimeout = 0;
AutoPtr<IPrintStream> CDriverManager::thePrintStream;
AutoPtr<IPrintWriter> CDriverManager::thePrintWriter;
Boolean CDriverManager::isinitflag = FALSE;

CAR_SINGLETON_IMPL(CDriverManager, Object, IDriverManagerHelper);

CDriverManager::CDriverManager()
{
    // pls use lock
    if (isinitflag == FALSE) {
        LoadInitialDrivers();
        isinitflag = TRUE;
    }
}

Boolean CDriverManager::LoadInitialDrivers()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String theDriverList;
    system->GetProperty(String("jdbc.drivers"), String(NULL), &theDriverList);
    if (theDriverList.IsNull()) {
        return FALSE;
    }

    /*
     * Get the names of the drivers as an array of Strings from the system
     * property by splitting the property at the separator character ':'
     */
    AutoPtr<Pattern> pat = new Pattern();
    AutoPtr<ArrayOf<String> > theDriverNames;
    pat->Split(String(":"), (ArrayOf<String> **)&theDriverNames);

    // for (String element : theDriverNames) {
    //     try {
    //         // Load the driver class
    //         Class
    //                 .forName(element, true, ClassLoader
    //                         .getSystemClassLoader());
    //     } catch (Throwable t) {
    //         // Ignored
    //     }
    // }
    return TRUE;
}

ECode CDriverManager::DeregisterDriver(
    /* [in] */ IDriver * driver)
{
    if (driver == NULL) {
        return NOERROR;
    }
    AutoPtr<IClassLoader> callerClassLoader ;// = VMStack.getCallingClassLoader();
    assert(0 && "TODO");
    if (!IsClassFromClassLoader(driver, callerClassLoader)) {
        return E_SQL_SECURITY_EXCEPTION;
    }
    Mutex::Autolock lock(mLock);
    theDrivers.Remove((IDriver *)driver);

    return NOERROR;
}

ECode CDriverManager::GetConnection(
    /* [in] */ const String& url,
    /* [out] */ IConnection ** conn)
{
    AutoPtr<IProperties> mprope = NULL;
    CProperties::New((IProperties **)&mprope);
    return GetConnection(url, mprope , conn);
}

ECode CDriverManager::GetConnection(
    /* [in] */ const String& url,
    /* [in] */ IProperties * info,
    /* [out] */ IConnection ** conn)
{
    // 08 - connection exception
    // 001 - SQL-client unable to establish SQL-connection
    String sqlState = String("08001");
    if (url.IsNull()) {
        return E_SQL_EXCEPTION;
    }
    {
        Mutex::Autolock lock(mLock);
        /*
         * Loop over the drivers in the DriverSet checking to see if one can
         * open a connection to the supplied URL - return the first
         * connection which is returned
         */
        List<AutoPtr<IDriver> >::Iterator iter = theDrivers.Begin();
        while (iter != theDrivers.End()) {
            AutoPtr<IConnection> theConnection;
            (*iter)->Connect(url, info, (IConnection **)&theConnection);
            if (theConnection != NULL) {
                *conn = theConnection;
                return NOERROR;
            }
        }
    }
    // If we get here, none of the drivers are able to resolve the URL
    return E_SQL_EXCEPTION;
}

ECode CDriverManager::GetConnection(
    /* [in] */ const String& url,
    /* [in] */ const String& user,
    /* [in] */ const String& password,
    /* [out] */ IConnection ** conn)
{
    AutoPtr<IProperties> theProperties;
    CProperties::New((IProperties **)&theProperties);

    String temp;
    if (!user.IsNull()) {
        theProperties->SetProperty(String("user"), user, &temp);
    }
    if (!password.IsNull()) {
        theProperties->SetProperty(String("password"), password, &temp);
    }
    return GetConnection(url, theProperties,conn);
}

ECode CDriverManager::GetDriver(
    /* [in] */ const String& url,
    /* [out] */ IDriver ** driver)
{
    AutoPtr<IClassLoader> callerClassLoader  ;//= VMStack.getCallingClassLoader();
    assert(0 && "TODO");
    {
        Mutex::Autolock lock(mLock);
        /*
         * Loop over the drivers in the DriverSet checking to see if one
         * does understand the supplied URL - return the first driver which
         * does understand the URL
         */
        List<AutoPtr<IDriver> >::Iterator iter = theDrivers.Begin();
        while (iter != theDrivers.End()) {
            Boolean isiterflag = FALSE;
            (*iter)->AcceptsURL(url,&isiterflag);
            if (isiterflag &&
                IsClassFromClassLoader(*iter, callerClassLoader)) {
                *driver = *iter;
                return NOERROR;
            }
        }
    }
    // If no drivers understand the URL, throw an SQLException
    // SQLState: 08 - connection exception
    // 001 - SQL-client unable to establish SQL-connection
    return E_SQL_EXCEPTION;
}

ECode CDriverManager::GetDrivers(
    /* [out] */ IEnumeration ** enumeration)
{
    VALIDATE_NOT_NULL(enumeration);

    /*
     * Synchronize to avoid clashes with additions and removals of drivers
     * in the DriverSet
     */
    AutoPtr<IClassLoader> callerClassLoader ; //= VMStack.getCallingClassLoader();
    assert(0 && "TODO");
    {
        Mutex::Autolock lock(mLock);
        List<AutoPtr<IDriver> > result;
        List<AutoPtr<IDriver> >::Iterator iter = theDrivers.Begin();
        while (iter != theDrivers.End()) {
            if (IsClassFromClassLoader(*iter, callerClassLoader)) {
                result.PushBack(*iter);
            }
        }
        assert(0 && "TODO");
        // return Collections.enumeration(result);
    }
    return NOERROR;
}

ECode CDriverManager::GetLoginTimeout(
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value);
    *value = loginTimeout;
    return NOERROR;
}

ECode CDriverManager::GetLogStream(
    /* [out] */ IPrintStream ** pntstream)
{
    VALIDATE_NOT_NULL(pntstream);
    *pntstream = thePrintStream;
    return NOERROR;
}

ECode CDriverManager::GetLogWriter(
    /* [out] */ IPrintWriter ** pntwriter)
{
    VALIDATE_NOT_NULL(pntwriter);
    *pntwriter = thePrintWriter;
    return NOERROR;
}

ECode CDriverManager::Println(
    /* [in] */ const String& message)
{
    if (thePrintWriter != NULL) {
        thePrintWriter->PrintString(message);
        // thePrintWriter->Flush();
    } else if (thePrintStream != NULL) {
        thePrintStream->PrintString(message);
        // thePrintStream->Flush();
    }
    /*
     * If neither the PrintWriter not the PrintStream are set, then silently
     * do nothing the message is not recorded and no exception is generated.
     */
    return NOERROR;
}

ECode CDriverManager::RegisterDriver(
    /* [in] */ IDriver * driver)
{
    if (driver == NULL) {
        return E_SQL_NULL_POINTER_EXCEPTION;
    }
    {
        Mutex::Autolock lock(mLock);
        theDrivers.PushBack(driver);
    }
    return NOERROR;
}

ECode CDriverManager::SetLoginTimeout(
    /* [in] */ Int32 seconds)
{
    loginTimeout = seconds;
    return NOERROR;
}

ECode CDriverManager::SetLogStream(
    /* [in] */ IPrintStream * pntstream)
{
    thePrintStream = pntstream;
    return NOERROR;
}

ECode CDriverManager::SetLogWriter(
    /* [in] */ IPrintWriter * pntwriter)
{
    thePrintWriter = pntwriter;
    return NOERROR;
}

Boolean CDriverManager::IsClassFromClassLoader(
    /* [in] */ IInterface * theObject,
    /* [in] */ IClassLoader * theClassLoader)
{
    if ((theObject == NULL) || (theClassLoader == NULL)) {
        return FALSE;
    }

    // Class<?> objectClass = theObject.getClass();

    // try {
    //     Class<?> checkClass = Class.forName(objectClass.getName(), true,
    //             theClassLoader);
    //     if (checkClass == objectClass) {
    //         return true;
    //     }
    // } catch (Throwable t) {
    //     // Empty
    // }
    return FALSE;
}

} // namespace Sql
} // namespace Elastos
