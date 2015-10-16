#include "CAccessibilityServiceInfo.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <Elastos.CoreLibrary.h>
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/view/accessibility/CAccessibilityEventHelper.h"
#include "elastos/droid/utility/Xml.h"
#include "elastos/droid/R.h"

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Org::Xmlpull::V1::IXmlPullParser;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityEventHelper;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const String CAccessibilityServiceInfo::TAG_ACCESSIBILITY_SERVICE = String("accessibility-service");

CAccessibilityServiceInfo::CAccessibilityServiceInfo()
    : mEventTypes(-1)
    , mFeedbackType(-1)
    , mNotificationTimeout(0)
    , mFlags(-1)
    , mId(NULL)
    , mSettingsActivityName(NULL)
    , mCanRetrieveWindowContent(FALSE)
    , mDescriptionResId(-1)
    , mNonLocalizedDescription(NULL)
{
}

CAccessibilityServiceInfo::~CAccessibilityServiceInfo()
{
}

ECode CAccessibilityServiceInfo::constructor()
{
    return NOERROR;
}

ECode CAccessibilityServiceInfo::constructor(
    /* [in] */ IResolveInfo* resolveInfo,
    /* [in] */ IContext* context)
{
    AutoPtr<IServiceInfo> serviceInfo;
    resolveInfo->GetServiceInfo((IServiceInfo**)&serviceInfo);
    String pkgName, name;
    serviceInfo->GetPackageName(&pkgName);
    serviceInfo->GetName(&name);
    AutoPtr<IComponentName> cname;
    CComponentName::New(pkgName, name, (IComponentName**)&cname);
    cname->FlattenToShortString(&mId);
    mResolveInfo = resolveInfo;

    AutoPtr<IXmlResourceParser> parser;

    // try {
        AutoPtr<IPackageManager> packageManager;
        context->GetPackageManager((IPackageManager**)&packageManager);
        serviceInfo->LoadXmlMetaData(packageManager,
            IAccessibilityService::SERVICE_META_DATA,
            (IXmlResourceParser**)&parser);
        if (parser == NULL) {
            return NOERROR;
        }

        Int32 type = 0;
        while (type != IXmlPullParser::END_DOCUMENT && type != IXmlPullParser::START_TAG) {
            parser->Next(&type);
        }

        String nodeName;
        parser->GetName(&nodeName);
        if (!nodeName.Equals(TAG_ACCESSIBILITY_SERVICE)) {
            // throw new XmlPullParserException( "Meta-data does not start with"
            //         + TAG_ACCESSIBILITY_SERVICE + " tag");
            return E_XML_PULL_PARSER_EXCEPTION;
        }

        AutoPtr<IAttributeSet> allAttributes = Xml::AsAttributeSet(parser);

        AutoPtr<IResources> resources;
        AutoPtr<IApplicationInfo> appInfo;
        serviceInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        packageManager->GetResourcesForApplication(appInfo, (IResources**)&resources);

        AutoPtr<ITypedArray> asAttributes;
        AutoPtr<ArrayOf<Int32> > vals = ArrayOf<Int32>::Alloc(8);
        for (Int32 i = 0; i < 8; i++) {
            (*vals)[i] = Elastos::Droid::R::styleable::AccessibilityService[i];
        }

        resources->ObtainAttributes(allAttributes,
                vals, (ITypedArray**)&asAttributes);
        asAttributes->GetInt32(
                Elastos::Droid::R::styleable::AccessibilityService_accessibilityEventTypes,
                0, &mEventTypes);
        String packageNamez;
        asAttributes->GetString(
                Elastos::Droid::R::styleable::AccessibilityService_packageNames, &packageNamez);
        if (packageNamez != NULL) {
            mPackageNames = NULL;
            AutoPtr< ArrayOf<String> > events;
            StringUtils::Split(packageNamez, String("(\\s)*,(\\s)*"), (ArrayOf<String>**)&mPackageNames);
        }
        asAttributes->GetInt32(
                Elastos::Droid::R::styleable::AccessibilityService_accessibilityFeedbackType,
                0, &mFeedbackType);
        asAttributes->GetInt32(
                Elastos::Droid::R::styleable::AccessibilityService_notificationTimeout,
                0, (Int32*)&mNotificationTimeout);
        asAttributes->GetInt32(
                Elastos::Droid::R::styleable::AccessibilityService_accessibilityFlags, 0, &mFlags);
        asAttributes->GetString(
                Elastos::Droid::R::styleable::AccessibilityService_settingsActivity, &mSettingsActivityName);
        asAttributes->GetBoolean(
                Elastos::Droid::R::styleable::AccessibilityService_canRetrieveWindowContent,
                FALSE, &mCanRetrieveWindowContent);
        AutoPtr<ITypedValue> peekedValue;
        asAttributes->PeekValue(
                Elastos::Droid::R::styleable::AccessibilityService_description,
                (ITypedValue**)&peekedValue);
        if (peekedValue != NULL) {
            peekedValue->GetResourceId(&mDescriptionResId);
            AutoPtr<ICharSequence> nonLocalizedDescription;
            peekedValue->CoerceToString((ICharSequence**)&nonLocalizedDescription);
            if (nonLocalizedDescription != NULL) {
                String des;
                nonLocalizedDescription->ToString(&des);
                mNonLocalizedDescription = des.Trim();
            }
        }
        asAttributes->Recycle();
    // } catch (NameNotFoundException e) {
    //     throw new XmlPullParserException( "Unable to create context for: "
    //             + serviceInfo.packageName);
    // } finally {
        if (parser != NULL) {
            parser->Close();
        }
    // }
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetEventTypes (
    /* [in] */ Int32 types)
{
    mEventTypes = types;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetEventTypes (
    /* [out] */ Int32* types)
{
    VALIDATE_NOT_NULL(types);
    *types = mEventTypes;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetPackageNames (
    /* [in] */ ArrayOf<String>* names)
{
    mPackageNames = names;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetPackageNames (
    /* [out, callee] */ ArrayOf<String>** names)
{
    VALIDATE_NOT_NULL(names);
    *names = mPackageNames;
    REFCOUNT_ADD(*names);
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetFeedbackType (
    /* [in] */ Int32 type)
{
    mFeedbackType = type;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetFeedbackType (
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mFeedbackType;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetNotificationTimeout (
    /* [in] */ Int64 timeout)
{
    mNotificationTimeout = timeout;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetNotificationTimeout (
    /* [out] */ Int64* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mNotificationTimeout;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::SetFlags (
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::GetFlags (
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

/**
 * Updates the properties that an AccessibilitySerivice can change dynamically.
 *
 * @param other The info from which to update the properties.
 *
 * @hide
 */
ECode CAccessibilityServiceInfo::UpdateDynamicallyConfigurableProperties(
    /* [in] */ IAccessibilityServiceInfo* other)
{
    other->GetEventTypes(&mEventTypes);
    mPackageNames = NULL;
    other->GetPackageNames((ArrayOf<Elastos::String>**)&mPackageNames);
    other->GetFeedbackType(&mFeedbackType);
    other->GetNotificationTimeout(&mNotificationTimeout);
    other->GetFlags(&mFlags);
    return NOERROR;
}

/**
 * The accessibility service id.
 * <p>
 *   <strong>Generated by the system.</strong>
 * </p>
 * @return The id.
 */
ECode CAccessibilityServiceInfo::GetId(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mId;
    return NOERROR;
}

/**
 * The service {@link ResolveInfo}.
 * <p>
 *   <strong>Generated by the system.</strong>
 * </p>
 * @return The info.
 */
ECode CAccessibilityServiceInfo::GetResolveInfo(
    /* [out] */ IResolveInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = mResolveInfo;
    REFCOUNT_ADD(*info);
    return NOERROR;
}

/**
 * The settings activity name.
 * <p>
 *    <strong>lly set from
 *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
 * </p>
 * @return The settings activity name.
 */
ECode CAccessibilityServiceInfo::GetSettingsActivityName(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mSettingsActivityName;
    return NOERROR;
}

/**
 * Whether this service can retrieve the current window's content.
 * <p>
 *    <strong>lly set from
 *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
 * </p>
 * @return True if window content can be retrieved.
 */
ECode CAccessibilityServiceInfo::GetCanRetrieveWindowContent(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCanRetrieveWindowContent;
    return NOERROR;
}

/**
 * Gets the non-localized description of the accessibility service.
 * <p>
 *    <strong>lly set from
 *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
 * </p>
 * @return The description.
 *
 * @deprecated Use {@link #loadDescription(PackageManager)}.
 */
ECode CAccessibilityServiceInfo::GetDescription(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = mNonLocalizedDescription;
    return NOERROR;
}

/**
 * The localized description of the accessibility service.
 * <p>
 *    <strong>lly set from
 *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
 * </p>
 * @return The localized description.
 */
ECode CAccessibilityServiceInfo::LoadDescription(
    /* [in] */ IPackageManager* packageManager,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    if (mDescriptionResId == 0) {
        *str = mNonLocalizedDescription;
        return NOERROR;
    }
    AutoPtr<IServiceInfo> serviceInfo;
    mResolveInfo->GetServiceInfo((IServiceInfo**)&serviceInfo);
    AutoPtr<ICharSequence> description;
    AutoPtr<IApplicationInfo> appInfo;
    serviceInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String pkgName;
    serviceInfo->GetPackageName(&pkgName);
    packageManager->GetText(pkgName, mDescriptionResId, appInfo, (ICharSequence**)&description);
    if (description != NULL) {
        String des;
        description->ToString(&des);
        *str = des.Trim();
        return NOERROR;
    }
    *str = String(NULL);
    return NOERROR;
}

/**
 * {@inheritDoc}
 */
ECode CAccessibilityServiceInfo::DescribeContents(
    /* [out] */ Int32* val)
{
    VALIDATE_NOT_NULL(val);
    *val = 0;
    return NOERROR;
}

ECode CAccessibilityServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return E_NOT_IMPLEMENTED;
}

ECode CAccessibilityServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

void CAccessibilityServiceInfo::InitFromParcel(
    /* [in] */ IParcel* parcel)
{
    ReadFromParcel(parcel);
}

void CAccessibilityServiceInfo::AppendFeedbackTypes(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 feedbackTypes)
{
    using Elastos::Core::Math;

    (*stringBuilder) += "feedbackTypes:";
    (*stringBuilder) += "[";

    while (feedbackTypes != 0) {
        Int32 feedbackTypeBit = (1 << Math::NumberOfTrailingZeros(feedbackTypes));
        String s;
        FeedbackTypeToString(feedbackTypeBit, &s);
        (*stringBuilder) += s;
        feedbackTypes &= ~feedbackTypeBit;
        if (feedbackTypes != 0) {
            (*stringBuilder) += ", ";
        }
    }
    (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendPackageNames(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ ArrayOf<String>* packageNames)
{
    (*stringBuilder) += "packageNames:";
    (*stringBuilder) += "[";
    if (packageNames != NULL) {
        Int32 packageNameCount = packageNames->GetLength();
        for (Int32 i = 0; i < packageNameCount; i++) {
            (*stringBuilder) += (*packageNames)[i];
            if (i < packageNameCount - 1) {
                (*stringBuilder) += ", ";
            }
        }
    }
    (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendEventTypes(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 eventTypes)
{
    using Elastos::Core::Math;
    String info;
    Int32 eventTypeBit;
    AutoPtr<IAccessibilityEventHelper> helper;
    CAccessibilityEventHelper::AcquireSingleton((IAccessibilityEventHelper**)&helper);

    (*stringBuilder) += "eventTypes:";
    (*stringBuilder) += "[";
    while (eventTypes != 0) {
        eventTypeBit = (1 << Math::NumberOfTrailingZeros(eventTypes));
        helper->EventTypeToString(eventTypeBit, &info);
        (*stringBuilder) += info;
        eventTypes &= ~eventTypeBit;
        if (eventTypes != 0) {
            (*stringBuilder) += ", ";
        }
    }
    (*stringBuilder) += "]";
}

void CAccessibilityServiceInfo::AppendFlags(
    /* [in] */ StringBuilder* stringBuilder,
    /* [in] */ Int32 flags)
{
    using Elastos::Core::Math;

    (*stringBuilder) += "flags:";
    (*stringBuilder) += "[";
    while (flags != 0) {
        Int32 flagBit = (1 << Math::NumberOfTrailingZeros(flags));
        String s;
        FlagToString(flagBit, &s);
        (*stringBuilder) += s;
        flags &= ~flagBit;
        if (flags != 0) {
            (*stringBuilder) += ", ";
        }
    }
    (*stringBuilder) += "]";
}

/**
 * Returns the string representation of a feedback type. For example,
 * {@link #FEEDBACK_SPOKEN} is represented by the string FEEDBACK_SPOKEN.
 *
 * @param feedbackType The feedback type.
 * @return The string representation.
 */
ECode CAccessibilityServiceInfo::FeedbackTypeToString(
    /* [in] */ Int32 feedbackType,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    using Elastos::Core::Math;

    StringBuilder builder("[");
    while (feedbackType != 0) {
        Int32 feedbackTypeFlag = 1 << Math::NumberOfTrailingZeros(feedbackType);
        feedbackType &= ~feedbackTypeFlag;
        switch (feedbackTypeFlag) {
            case IAccessibilityServiceInfo::FEEDBACK_AUDIBLE:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_AUDIBLE";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_HAPTIC:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_HAPTIC";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_GENERIC:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_GENERIC";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_SPOKEN:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_SPOKEN";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_VISUAL:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_VISUAL";
                break;
            case IAccessibilityServiceInfo::FEEDBACK_BRAILLE:
                if (builder.GetLength() > 1) {
                    builder += ", ";
                }
                builder += "FEEDBACK_BRAILLE";
                break;
        }
    }
    builder += "]";
    *str = builder.ToString();
    return NOERROR;
}

/**
 * Returns the string representation of a flag. For example,
 * {@link #DEFAULT} is represented by the string DEFAULT.
 *
 * @param flag The flag.
 * @return The string representation.
 */
ECode CAccessibilityServiceInfo::FlagToString(
    /* [in] */ Int32 flag,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    switch (flag) {
        case IAccessibilityServiceInfo::DEFAULT:
            *str = "DEFAULT";
            break;
        case IAccessibilityServiceInfo::FLAG_INCLUDE_NOT_IMPORTANT_VIEWS:
            *str = "FLAG_INCLUDE_NOT_IMPORTANT_VIEWS";
            break;
        case IAccessibilityServiceInfo::FLAG_REQUEST_TOUCH_EXPLORATION_MODE:
            *str = "FLAG_REQUEST_TOUCH_EXPLORATION_MODE";
            break;
        default:
            *str = String(NULL);
            break;
        }
    return NOERROR;
}

}
}
}
