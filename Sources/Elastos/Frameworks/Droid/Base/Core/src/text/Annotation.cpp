#include "ext/frameworkdef.h"
#include "text/Annotation.h"

namespace Elastos {
namespace Droid {
namespace Text {

Annotation::Annotation()
{}

Annotation::Annotation(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    Init(key, value);
}

Annotation::Annotation(
    /* [in] */ IParcel* src)
{
    Init(src);
}

void Annotation::Init(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    mKey = key;
    mValue = value;
}

void Annotation::Init(
    /* [in] */ IParcel* src)
{
    ReadFromParcel(src);
}

Int32 Annotation::GetSpanTypeId()
{
    return ITextUtils::ANNOTATION;
}

Int32 Annotation::DescribeContents()
{
    return 0;
}

ECode Annotation::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadString(&mKey));
    FAIL_RETURN(source->ReadString(&mValue));
    return NOERROR;
}

ECode Annotation::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteString(mKey));
    FAIL_RETURN(dest->WriteString(mValue));
    return NOERROR;
}

String Annotation::GetKey()
{
    return mKey;
}

String Annotation::GetValue()
{
    return mValue;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos