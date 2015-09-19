
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Chromium {
namespace Chromium {

//@Override
ECode WebIconDatabaseAdapter::Open(
    /* [in] */ String path)
{
    AwContents::SetShouldDownloadFavicons();
    return NOERROR;
}

//@Override
ECode WebIconDatabaseAdapter::Close()
{
    // Intentional no-op.
    return NOERROR;
}

//@Override
ECode WebIconDatabaseAdapter::RemoveAllIcons()
{
    // Intentional no-op: we have no database so nothing to remove.
    return NOERROR;
}

//@Override
ECode WebIconDatabaseAdapter::RequestIconForPageUrl(
    /* [in] */ String url,
    /* [in] */ IIconListener* listener)
{
    // Intentional no-op.
    return NOERROR;
}

//@Override
ECode WebIconDatabaseAdapter::BulkRequestIconForPageUrl(
    /* [in] */ IContentResolver* cr,
    /* [in] */ String where,
    /* [in] */ IIconListener* listener)
{
    // Intentional no-op: hidden in base class.
    return NOERROR;
}

//@Override
ECode WebIconDatabaseAdapter::RetainIconForPageUrl(
    /* [in] */ String url)
{
    // Intentional no-op.
    return NOERROR;
}

//@Override
ECode WebIconDatabaseAdapter::ReleaseIconForPageUrl(
    /* [in] */ String url)
{
    // Intentional no-op.
    return NOERROR;
}

} // namespace Chromium
} // namespace Chromium
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
