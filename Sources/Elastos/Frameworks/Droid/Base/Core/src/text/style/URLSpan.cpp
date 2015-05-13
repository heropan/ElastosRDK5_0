#include "text/style/URLSpan.h"
#include "net/Uri.h"
#include "content/CIntent.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::Uri;
//using Elastos::Droid::Provider::IBrowser;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

URLSpan::URLSpan()
{}

URLSpan::URLSpan(
    /* [in] */ const String& url)
{
    Init(url);
}

URLSpan::URLSpan(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void URLSpan::Init(
    /* [in] */ const String& url)
{
    mURL = url;
}

void URLSpan::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 URLSpan::GetSpanTypeId()
{
    return ITextUtils::URL_SPAN;
}

Int32 URLSpan::DescribeContents()
{
    return 0;
}

ECode URLSpan::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return source->ReadString(&mURL);
}

ECode URLSpan::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteString(mURL);
}

String URLSpan::GetURL()
{
    return mURL;
}

ECode URLSpan::OnClick(
    /* [in] */ IView* widget)
{
    AutoPtr<IUri> uri;
    Uri::Parse(GetURL(), (IUri**)&uri);
    AutoPtr<IContext> context;
    widget->GetContext((IContext**)&context);
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_VIEW, uri, (IIntent**)&intent);
    String strPackageName;
    intent->/*PutExtra*/PutStringExtra(/*IBrowser::EXTRA_APPLICATION_ID*/String("com.android.browser.application_id"), (context->GetPackageName(&strPackageName), strPackageName));
    context->StartActivity(intent);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos