
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content_Public {

Referrer::Referrer(
    /* [in] */ String url,
    /* [in] */ Int32 policy)
    : mUrl(url)
    , mPolicy(policy)
{
}

String Referrer::GetUrl()
{
    return mUrl;
}

Int32 Referrer::GetPolicy()
{
    return mPolicy;
}

} // namespace Content_Public
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
