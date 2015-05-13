
#include "app/CAlarmManager.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CAlarmManager::constructor(
    /* [in] */ IIAlarmManager* service)
{
    mService = service;
    return NOERROR;
}

ECode CAlarmManager::Set(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ IPendingIntent* operation)
{
    //try {
    return mService->Set(type, triggerAtMillis, operation);
    //} catch (RemoteException ex) {
    //}
}

ECode CAlarmManager::SetRepeating(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ Int64 intervalMillis,
    /* [in] */ IPendingIntent* operation)
{
    //try {
    return mService->SetRepeating(type, triggerAtMillis, intervalMillis, operation);
    //} catch (RemoteException ex) {
    //}
}

ECode CAlarmManager::SetInexactRepeating(
    /* [in] */ Int32 type,
    /* [in] */ Int64 triggerAtMillis,
    /* [in] */ Int64 intervalMillis,
    /* [in] */ IPendingIntent* operation)
{
    //try {
    return mService->SetInexactRepeating(type, triggerAtMillis, intervalMillis, operation);
    //} catch (RemoteException ex) {
    //}
}

ECode CAlarmManager::Cancel(
    /* [in] */ IPendingIntent* operation)
{
    //try {
    return mService->Remove(operation);
    //} catch (RemoteException ex) {
    //}
}

ECode CAlarmManager::SetTime(
    /* [in] */ Int64 millis)
{
    //try {
    return mService->SetTime(millis);
    //} catch (RemoteException ex) {
    //}
}

/**
 * Set the system default time zone.
 * Requires the permission android.permission.SET_TIME_ZONE.
 *
 * @param timeZone in the format understood by {@link java.util.TimeZone}
 */
ECode CAlarmManager::SetTimeZone(
    /* [in] */ const String& timeZone)
{
    //try {
    return mService->SetTimeZone(timeZone);
    //} catch (RemoteException ex) {
    //}
}

}
}
}