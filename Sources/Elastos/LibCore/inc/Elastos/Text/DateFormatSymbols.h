#ifndef __DATEFORMATSYMBOLS_H__
#define __DATEFORMATSYMBOLS_H__

#include "Elastos.Core_server.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;
using Libcore::ICU::ILocale;

namespace Elastos {
namespace Text {

class DateFormatSymbols
{
public:
    DateFormatSymbols();

    virtual ~DateFormatSymbols();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI Init();

    CARAPI Init(
        /* [in] */ ILocale* locale);

    CARAPI_(AutoPtr<ArrayOf<IObjectContainer*> >) InternalZoneStrings();

    static CARAPI GetInstance(
        /* [out] */ IDateFormatSymbols** instance);

    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ IDateFormatSymbols** instance);

    static CARAPI GetAvailableLocales(
        /* [out, callee] */ ArrayOf<ILocale*>** arrayOfLocales);

    // @Override
    CARAPI Clone(
        /* [out] */ IDateFormatSymbols** instance);

//    @Override
    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* res);

//    @Override
//    public String toString();

    virtual CARAPI GetLongStandAloneMonths(
        /* [out, callee] */ ArrayOf<String>** longStandAloneMonths);

    virtual CARAPI GetShortStandAloneMonths(
        /* [out, callee] */ ArrayOf<String>** shortStandAloneMonths);

    virtual CARAPI GetLongStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String>** longStandAloneWeekdays);

    virtual CARAPI GetShortStandAloneWeekdays(
        /* [out, callee] */ ArrayOf<String>** shortStandAloneWeekdays);

    virtual CARAPI GetCustomZoneStrings(
        /* [out] */ Boolean* customZoneStrings);

    virtual CARAPI GetLocale(
        /* [out] */ ILocale** locale);

    virtual CARAPI GetAmPmStrings(
        /* [out, callee] */ ArrayOf<String> ** arrayOfStrings);

    virtual CARAPI GetEras(
        /* [out, callee] */ ArrayOf<String> ** arrayOfStrings);

    virtual CARAPI GetLocalPatternChars(
        /* [out] */ String* string);

    virtual CARAPI GetMonths(
        /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    virtual CARAPI GetShortMonths(
        /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    virtual CARAPI GetShortWeekdays(
        /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    virtual CARAPI GetWeekdays(
        /* [out, callee] */ ArrayOf<String>** arrayOfStrings);

    virtual CARAPI GetZoneStrings(
        /* [out, callee] */ ArrayOf<IObjectContainer*> ** zoneStrings);

//    @Override
//    public int hashCode();

    virtual CARAPI SetAmPmStrings(
        /* [in] */ const ArrayOf<String>& data);

    virtual CARAPI SetEras(
        /* [in] */ const ArrayOf<String>& data);

    virtual CARAPI SetLocalPatternChars(
        /* [in] */ const String& data);

    virtual CARAPI SetMonths(
        /* [in] */ const ArrayOf<String>& data);

    virtual CARAPI SetShortMonths(
        /* [in] */ const ArrayOf<String>& data);

    virtual CARAPI SetShortWeekdays(
        /* [in] */ const ArrayOf<String>& data);

    virtual CARAPI SetWeekdays(
        /* [in] */ const ArrayOf<String>& data);

    virtual CARAPI SetZoneStrings(
        /* [in] */ ArrayOf<IObjectContainer*>* zoneStrings);

private:
//    private void readObject(ObjectInputStream ois);

//    private void writeObject(ObjectOutputStream oos);

    static CARAPI_(Boolean) TimeZoneStringsEqual(
        /* [in] */ IDateFormatSymbols* lhs,
        /* [in] */ IDateFormatSymbols* rhs);

    CARAPI Clone2dStringArray(
        /* [in] */ ArrayOf<IObjectContainer*>* array,
        /* [out, callee] */ ArrayOf<IObjectContainer*>** stringArray);

public:
    AutoPtr<ArrayOf<String> > mAmpms, mEras, mMonths, mShortMonths, mShortWeekdays, mWeekdays;

    AutoPtr<ArrayOf<String> > mLongStandAloneMonths;

    AutoPtr<ArrayOf<String> > mShortStandAloneMonths;

    AutoPtr<ArrayOf<String> > mLongStandAloneWeekdays;

    AutoPtr<ArrayOf<String> > mShortStandAloneWeekdays;

    AutoPtr<ArrayOf<IObjectContainer*> > mZoneStrings;// String[][]

    Boolean mCustomZoneStrings;

    AutoPtr<ILocale> mLocale;

    Mutex mLock;

private:
    String mLocalPatternChars;
};

} // namespace Text
} // namespace Elastos

#endif //__DATEFORMATSYMBOLS_H__
