
#include "elastos/droid/webkit/L10nUtils.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Webkit {

// These array elements must be kept in sync with those defined in
// external/chromium/android/app/l10n_utils.h
Int32 L10nUtils::mIdsArray[] = {
    R::string::autofill_address_name_separator,      // IDS_AUTOFILL_DIALOG_ADDRESS_NAME_SEPARATOR
    R::string::autofill_address_summary_name_format, // IDS_AUTOFILL_DIALOG_ADDRESS_SUMMARY_NAME_FORMAT
    R::string::autofill_address_summary_separator,   // IDS_AUTOFILL_DIALOG_ADDRESS_SUMMARY_SEPARATOR
    R::string::autofill_address_summary_format,      // IDS_AUTOFILL_DIALOG_ADDRESS_SUMMARY_FORMAT
    R::string::autofill_attention_ignored_re,        // IDS_AUTOFILL_ATTENTION_IGNORED_RE
    R::string::autofill_region_ignored_re,           // IDS_AUTOFILL_REGION_IGNORED_RE
    R::string::autofill_company_re,                  // IDS_AUTOFILL_COMPANY_RE
    R::string::autofill_address_line_1_re,           // IDS_AUTOFILL_ADDRESS_LINE_1_RE
    R::string::autofill_address_line_1_label_re,     // IDS_AUTOFILL_ADDRESS_LINE_1_LABEL_RE
    R::string::autofill_address_line_2_re,           // IDS_AUTOFILL_ADDRESS_LINE_2_RE
    R::string::autofill_address_line_3_re,           // IDS_AUTOFILL_ADDRESS_LINE_3_RE
    R::string::autofill_country_re,                  // IDS_AUTOFILL_COUNTRY_RE
    R::string::autofill_zip_code_re,                 // IDS_AUTOFILL_ZIP_CODE_RE
    R::string::autofill_zip_4_re,                    // IDS_AUTOFILL_ZIP_4_RE
    R::string::autofill_city_re,                     // IDS_AUTOFILL_CITY_RE
    R::string::autofill_state_re,                    // IDS_AUTOFILL_STATE_RE
    R::string::autofill_address_type_same_as_re,     // IDS_AUTOFILL_SAME_AS_RE
    R::string::autofill_address_type_use_my_re,      // IDS_AUTOFILL_USE_MY_RE
    R::string::autofill_billing_designator_re,       // IDS_AUTOFILL_BILLING_DESIGNATOR_RE
    R::string::autofill_shipping_designator_re,      // IDS_AUTOFILL_SHIPPING_DESIGNATOR_RE
    R::string::autofill_email_re,                    // IDS_AUTOFILL_EMAIL_RE
    R::string::autofill_username_re,                 // IDS_AUTOFILL_USERNAME_RE
    R::string::autofill_name_re,                     // IDS_AUTOFILL_NAME_RE
    R::string::autofill_name_specific_re,            // IDS_AUTOFILL_NAME_SPECIFIC_RE
    R::string::autofill_first_name_re,               // IDS_AUTOFILL_FIRST_NAME_RE
    R::string::autofill_middle_initial_re,           // IDS_AUTOFILL_MIDDLE_INITIAL_RE
    R::string::autofill_middle_name_re,              // IDS_AUTOFILL_MIDDLE_NAME_RE
    R::string::autofill_last_name_re,                // IDS_AUTOFILL_LAST_NAME_RE
    R::string::autofill_phone_re,                    // IDS_AUTOFILL_PHONE_RE
    R::string::autofill_area_code_re,                // IDS_AUTOFILL_AREA_CODE_RE
    R::string::autofill_phone_prefix_re,             // IDS_AUTOFILL_PHONE_PREFIX_RE
    R::string::autofill_phone_suffix_re,             // IDS_AUTOFILL_PHONE_SUFFIX_RE
    R::string::autofill_phone_extension_re,          // IDS_AUTOFILL_PHONE_EXTENSION_RE
    R::string::autofill_name_on_card_re,             // IDS_AUTOFILL_NAME_ON_CARD_RE
    R::string::autofill_name_on_card_contextual_re,  // IDS_AUTOFILL_NAME_ON_CARD_CONTEXTUAL_RE
    R::string::autofill_card_cvc_re,                 // IDS_AUTOFILL_CARD_CVC_RE
    R::string::autofill_card_number_re,              // IDS_AUTOFILL_CARD_NUMBER_RE
    R::string::autofill_expiration_month_re,         // IDS_AUTOFILL_EXPIRATION_MONTH_RE
    R::string::autofill_expiration_date_re,          // IDS_AUTOFILL_EXPIRATION_DATE_RE
    R::string::autofill_card_ignored_re,             // IDS_AUTOFILL_CARD_IGNORED_RE
    R::string::autofill_fax_re,                      // IDS_AUTOFILL_FAX_RE
    R::string::autofill_country_code_re,             // IDS_AUTOFILL_COUNTRY_CODE_RE
    R::string::autofill_area_code_notext_re,         // IDS_AUTOFILL_AREA_CODE_NOTEXT_RE
    R::string::autofill_phone_prefix_separator_re,   // IDS_AUTOFILL_PHONE_PREFIX_SEPARATOR_RE
    R::string::autofill_phone_suffix_separator_re,   // IDS_AUTOFILL_PHONE_SUFFIX_SEPARATOR_RE
    R::string::autofill_province,                    // IDS_AUTOFILL_DIALOG_PROVINCE
    R::string::autofill_postal_code,                 // IDS_AUTOFILL_DIALOG_POSTAL_CODE
    R::string::autofill_state,                       // IDS_AUTOFILL_DIALOG_STATE
    R::string::autofill_zip_code,                    // IDS_AUTOFILL_DIALOG_ZIP_CODE
    R::string::autofill_county,                      // IDS_AUTOFILL_DIALOG_COUNTY
    R::string::autofill_island,                      // IDS_AUTOFILL_DIALOG_ISLAND
    R::string::autofill_district,                    // IDS_AUTOFILL_DIALOG_DISTRICT
    R::string::autofill_department,                  // IDS_AUTOFILL_DIALOG_DEPARTMENT
    R::string::autofill_prefecture,                  // IDS_AUTOFILL_DIALOG_PREFECTURE
    R::string::autofill_parish,                      // IDS_AUTOFILL_DIALOG_PARISH
    R::string::autofill_area,                        // IDS_AUTOFILL_DIALOG_AREA
    R::string::autofill_emirate                      // IDS_AUTOFILL_DIALOG_EMIRATE
};

AutoPtr<IContext> L10nUtils::mApplicationContext;
HashMap<Int32, String> L10nUtils::mStrings;

void L10nUtils::SetApplicationContext(
    /* [in] */ IContext* applicationContext)
{
    mApplicationContext = NULL;
    applicationContext->GetApplicationContext((IContext**)&mApplicationContext);
}

String L10nUtils::GetLocalisedString(
    /* [in] */ Int32 id)
{
    // if (mStrings == NULL) {
    //     // This is the first time we need a localised string.
    //     // loadString will create the Map.
    //     return LoadString(id);
    // }

    // SoftReference<String> ref = mStrings.get(id);
    // boolean needToLoad = ref == null || ref.get() == null;
    // return needToLoad ? loadString(id) : ref.get();

    // Don't use SoftReference for value of HashMap
    String value = mStrings[id];
    if (value.IsNullOrEmpty()) {
        return LoadString(id);
    }
    else {
        return value;
    }
}

String L10nUtils::LoadString(
    /* [in] */ Int32 id)
{
    // if (mStrings == null) {
    //     mStrings = new HashMap<Integer, SoftReference<String> >(mIdsArray.length);
    // }

    // String localisedString = mApplicationContext.getResources().getString(mIdsArray[id]);
    // mStrings.put(id, new SoftReference<String>(localisedString));
    // return localisedString;

    // Don't use SoftReference for value of HashMap
    AutoPtr<IResources> res;
    mApplicationContext->GetResources((IResources**)&res);
    String localisedString;
    res->GetString(mIdsArray[id], &localisedString);
    mStrings[id] = localisedString;

    return localisedString;
}

ECode L10nUtils::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "L10nUtils";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos