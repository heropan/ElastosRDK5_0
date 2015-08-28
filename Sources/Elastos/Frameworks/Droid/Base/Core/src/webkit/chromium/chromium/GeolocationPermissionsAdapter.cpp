
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Chromium {
namespace Chromium {

AwGeolocationPermissions::GeolocationPermissionsAdapter(
    /* [in] */ AwGeolocationPermissions* chromeGeolocationPermissions)
    : mChromeGeolocationPermissions(chromeGeolocationPermissions)
{
}

//@Override
ECode AwGeolocationPermissions::Allow(
    /* [in] */ String origin)
{
    mChromeGeolocationPermissions->Allow(origin);
    return NOERROR;
}

//@Override
ECode AwGeolocationPermissions::Clear(
    /* [in] */ String origin)
{
    mChromeGeolocationPermissions->Clear(origin);
    return NOERROR;
}

//@Override
ECode AwGeolocationPermissions::ClearAll()
{
    mChromeGeolocationPermissions->ClearAll();
    return NOERROR;
}

//@Override
ECode AwGeolocationPermissions::GetAllowed(
    /* [in] */ String origin,
    /* [in] */ IValueCallback* callback)
{
    mChromeGeolocationPermissions->GetAllowed(origin, callback);
    return NOERROR;
}

//@Override
ECode AwGeolocationPermissions::GetOrigins(
    /* [in] */ IValueCallback* callback)
{
    mChromeGeolocationPermissions->GetOrigins(callback);
    return NOERROR;
}

} // namespace Chromium
} // namespace Chromium
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
