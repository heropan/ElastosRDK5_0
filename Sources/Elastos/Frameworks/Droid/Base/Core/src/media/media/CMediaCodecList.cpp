
#include "ext/frameworkext.h"
#include "media/CMediaCodecList.h"
#include "media/CCodecCapabilities.h"
#include "media/CCodecProfileLevel.h"
#include "media/CCodecProfileLevel.h"
#include "media/CMediaCodecInfo.h"
#include <media/stagefright/MediaCodecList.h>
#include <media/stagefright/foundation/AString.h>
#include <utils/Vector.h>
#include <elastos/Slogger.h>
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMediaCodecList::TAG("CMediaCodecList");

ECode CMediaCodecList::GetCodecCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = android::MediaCodecList::getInstance()->countCodecs();
    return NOERROR;
}

ECode CMediaCodecList::GetCodecInfoAt(
    /* [in] */ Int32 index,
    /* [out] */ IMediaCodecInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Int32 temp;
    GetCodecCount(&temp);
    if (index < 0 || index > temp) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<CMediaCodecInfo> info;
    FAIL_RETURN(CMediaCodecInfo::NewByFriend(index, (CMediaCodecInfo**)&info));
    *result = (IMediaCodecInfo*)info.Get();
    return NOERROR;
}

ECode CMediaCodecList::GetCodecName(
    /* [in] */ Int32 index,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    const char *name = android::MediaCodecList::getInstance()->getCodecName(index);
    if (name == NULL) {
        Slogger::E(TAG, "CMediaCodecList::GetCodecName error");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = String(name);
    return NOERROR;
}

ECode CMediaCodecList::IsEncoder(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = android::MediaCodecList::getInstance()->isEncoder(index);
    return NOERROR;
}

ECode CMediaCodecList::GetSupportedTypes(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    android::Vector<android::AString> types;
    android::status_t err =
        android::MediaCodecList::getInstance()->getSupportedTypes(index, &types);

    if (err != android::OK) {
        Slogger::E(TAG, "CMediaCodecList::GetSupportedTypes error");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(types.size());
    for (size_t i = 0; i < types.size(); ++i) {
        (*array)[i] = String(types.itemAt(i).c_str());
    }

    *result = array;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CMediaCodecList::GetCodecCapabilities(
  /* [in] */ Int32 index,
  /* [in] */ const String& type,
  /* [out] */ ICodecCapabilities** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    if (type.IsNull()) {
        return NOERROR;
    }

    android::Vector<android::MediaCodecList::ProfileLevel> profileLevels;
    android::Vector<uint32_t> colorFormats;
    const char *typeStr = type.string();

    android::status_t err = android::MediaCodecList::getInstance()->getCodecCapabilities(
        index, typeStr, &profileLevels, &colorFormats);
    if (err != android::OK) {
        Slogger::E(TAG, "CMediaCodecList::GetCodecCapabilities error");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<CCodecCapabilities> caps;
    CCodecCapabilities::NewByFriend((CCodecCapabilities**)&caps);

    AutoPtr<ArrayOf<ICodecProfileLevel*> > profileLevelArray;
    profileLevelArray = ArrayOf<ICodecProfileLevel*>::Alloc(profileLevels.size());
    for (size_t i = 0; i < profileLevels.size(); ++i) {
        const android::MediaCodecList::ProfileLevel &src = profileLevels.itemAt(i);

        AutoPtr<CCodecProfileLevel> profileLevelObj;
        CCodecProfileLevel::NewByFriend((CCodecProfileLevel**)&profileLevelObj);
        profileLevelObj->SetProfile(src.mProfile);
        profileLevelObj->SetLevel(src.mLevel);

        ICodecProfileLevel* cpl = (ICodecProfileLevel*)profileLevelObj.Get();
        profileLevelArray->Set(i, cpl);
    }
    caps->SetProfileLevels(profileLevelArray);

    AutoPtr<ArrayOf<Int32> > colorFormatsArray;
    colorFormatsArray = ArrayOf<Int32>::Alloc(colorFormats.size());
    for (size_t i = 0; i < colorFormats.size(); ++i) {
        Int32 val = colorFormats.itemAt(i);
        colorFormatsArray->Set(i, val);
    }
    caps->SetColorFormats(colorFormatsArray);

    *result = (ICodecCapabilities*)caps.Get();
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

void CMediaCodecList::NativeInit()
{
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
