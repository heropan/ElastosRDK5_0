
#include "CCdmaSmsCbProgramResults.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Telephony {

ECode CCdmaSmsCbProgramResults::constructor()
{
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::constructor(
    /* [in] */ Int32 category,
    /* [in] */ Int32 language,
    /* [in] */ Int32 categoryResult)
{
    mCategory = category;
    mLanguage = language;
    mCategoryResult = categoryResult;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mCategory);
    source->ReadInt32(&mLanguage);
    source->ReadInt32(&mCategoryResult);
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mCategory);
    dest->WriteInt32(mLanguage);
    dest->WriteInt32(mCategoryResult);
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::GetCategory(
    /* [out] */ Int32* category)
{
    VALIDATE_NOT_NULL(category);
    *category = mCategory;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::GetLanguage(
    /* [out] */ Int32* language)
{
    VALIDATE_NOT_NULL(language);
    *language = mLanguage;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::GetCategoryResult(
    /* [out] */ Int32* categoryresult)
{
    VALIDATE_NOT_NULL(categoryresult);
    *categoryresult = mCategoryResult;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::SetCategory(
    /* [in] */ Int32 category)
{
    mCategory = category;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::SetLanguage(
    /* [in] */ Int32 language)
{
    mLanguage = language;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::SetCategoryResult(
    /* [in] */ Int32 categoryresult)
{
    mCategoryResult = categoryresult;
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb;
    sb.AppendCStr("CdmaSmsCbProgramResults{category=");
    sb.AppendInt32(mCategory);
    sb.AppendCStr(", language=");
    sb.AppendInt32(mLanguage);
    sb.AppendCStr(", result=");
    sb.AppendInt32(mCategoryResult);
    sb.AppendCStr("}");
    *str = sb.ToString();
    return NOERROR;
}

ECode CCdmaSmsCbProgramResults::DescribeContents(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    *val = 0;
    return NOERROR;
}

} // namespace Telephony
} // namespace Droid
} // namespace Elastos

