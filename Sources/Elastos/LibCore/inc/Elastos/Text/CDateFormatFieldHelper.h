
#ifndef __CDATEFORMATFIELDHELPER_H__
#define __CDATEFORMATFIELDHELPER_H__

#include "_CDateFormatFieldHelper.h"

namespace Elastos {
namespace Text {

CarClass(CDateFormatFieldHelper)
{
public:

    CARAPI GetDateFormatFieldByName(
        /* [in] */ const String& name,
        /* [out] */ IDateFormatField** field);

    CARAPI OfCalendarField(
        /* [in] */ Int32 calendarField,
        /* [out] */ IDateFormatField** field);

};

} // namespace Text
} // namespace Elastos

#endif // __CDATEFORMATFIELDHELPER_H__
