#ifndef __MESSAGEFORMAT_H__
#define __MESSAGEFORMAT_H__
#include "Elastos.CoreLibrary_server.h"
#include <elastos.h>
#include <elastos/StringBuffer.h>
#include <elastos/utility/etl/List.h>
#include <elastos/Character.h>
#include "Format.h"
#include "NumberFormat.h"
#include "DateFormat.h"

using Elastos::Utility::Etl::List;
using Elastos::Text::IAttributedCharacterIteratorAttribute;
using Elastos::Text::IFieldPosition;
using Elastos::Text::IParsePosition;

namespace Elastos {
namespace Text {

class MessageFormat : public Format
{
public:

    class MessageFormatField : public Format::Field
    {
        protected:
            CARAPI Init(
            /* [in] */ const String& fieldName);

        public:
            //public static final Field ARGUMENT = new Field("message argument field");
            const static AutoPtr<IMessageFormatField> ARGUMENT;

        private:
            //const static Int64 serialVersionUID = 7899943957617360810L;
    };

private:
    /**
     * An inner class to store attributes, values, start and end indices.
     * Instances of this inner class are used as elements for the fields vector
     */
    /*static*/ class FieldContainer : public ElRefBase {
    public:

        FieldContainer(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
            /* [in] */ IInterface* value);

    public:
        Int32 mStart;
        Int32 mEnd;
        AutoPtr<IAttributedCharacterIteratorAttribute> mAttribute;
        AutoPtr<IInterface> mValue;
    };

public:

    virtual ~MessageFormat();

    CARAPI Init(
        /* [in] */ const String& tem,
        /* [in] */ ILocale* locale);

    CARAPI Init(
        /* [in] */ const String& tem);

    virtual CARAPI ApplyPattern(
        /* [in] */ const String& tem);

//    @Override
//    public Object clone();

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    //@Override
    CARAPI FormatToCharacterIterator(
        /* [in] */ IInterface* object,
        /* [out] */ IAttributedCharacterIterator** characterIterator);

    CARAPI FormatObjects(
        /* [in] */ ArrayOf< IInterface* >* objects,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

    //@Override
    CARAPI FormatObject(
        /* [in] */ IInterface* object,
        /* [in] */ IStringBuffer * buffer,
        /* [in] */ IFieldPosition* field,
        /* [out] */ IStringBuffer ** value);

//    public static String format(String format, Object... args);

    virtual CARAPI GetFormats(
        /* [out] */ ArrayOf< IFormat* >** arrayOfInstances);

    virtual CARAPI GetFormatsByArgumentIndex(
        /* [out] */ ArrayOf< IFormat* >** arrayOfInstances);

    virtual CARAPI SetFormatByArgumentIndex(
        /* [in] */ Int32 argIndex,
        /* [in] */ IFormat* format);

    virtual CARAPI SetFormatsByArgumentIndex(
        /* [in] */ ArrayOf< IFormat* >* formats);

    virtual CARAPI GetLocale(
        /* [out] */ ILocale** locale);

//    @Override
//    public int hashCode();

    virtual CARAPI Parse(
        /* [in] */ const String& string,
        /* [out, callee] */ ArrayOf< IInterface* >** results);

    virtual CARAPI Parse(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out, callee] */ ArrayOf< IInterface* >** result);

    //@Override
    CARAPI ParseObject(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IInterface** arrayOfObjects);

    virtual CARAPI SetFormat(
        /* [in] */ Int32 offset,
        /* [in] */ IFormat* format);

    virtual CARAPI SetFormats(
        /* [in] */ ArrayOf< IFormat* >* formats);

    virtual CARAPI SetLocale(
        /* [in] */ ILocale* locale);

    virtual CARAPI ToPattern(
        /* [out] */ String* value);

    CARAPI GetMaxOffset(
        /* [out] */ Int32* maxOffset);

    CARAPI GetArgumentNumbers(
        /* [out,callee] */ ArrayOf<Int32>** argumentNumbers);

    CARAPI GetStrings(
        /* [out,callee] */ ArrayOf<String>** strings);

private:

    CARAPI FormatImpl(
        /* [in] */ ArrayOf< IInterface* >* objects,
        /* [in] */ IStringBuffer* inbuffer,
        /* [in] */ IFieldPosition* field,
        /* [in] */ List<AutoPtr<FieldContainer> >* fields,
        /* [out] */ IStringBuffer ** value);

    CARAPI HandleArgumentField(
        /* [in] */ Int32 begin,
        /* [in] */ Int32 end,
        /* [in] */ Int32 argIndex,
        /* [in] */ IFieldPosition* position,
        /* [in] */ List<AutoPtr<FieldContainer> >* fields);

    CARAPI HandleFormat(
        /* [in] */ IFormat* format,
        /* [in] */ IInterface* arg,
        /* [in] */ Int32 begin,
        /* [in] */ List<AutoPtr<FieldContainer> >* fields);

    CARAPI Match(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [in] */ Boolean last,
        /* [in] */ ArrayOf<String>* tokens,
        /* [out] */ Int32* value);

    CARAPI ParseVariable(
        /* [in] */ const String& string,
        /* [in] */ IParsePosition* position,
        /* [out] */ IFormat** value);

    CARAPI DecodeDecimalFormat(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFormat* format,
        /* [out] */ String* value);

    CARAPI DecodeSimpleDateFormat(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ IFormat* format,
        /* [out] */ String* value);

    CARAPI AppendQuoted(
        /* [in] */ StringBuffer* buffer,
        /* [in] */ const String& string);

/*    private static final ObjectStreamField[] serialPersistentFields = {
            new ObjectStreamField("argumentNumbers", int[].class),
            new ObjectStreamField("formats", Format[].class),
            new ObjectStreamField("locale", Locale.class),
            new ObjectStreamField("maxOffset", int.class),
            new ObjectStreamField("offsets", int[].class),
            new ObjectStreamField("pattern", String.class), };*/

    // private void writeObject(ObjectOutputStream stream);

    // private void readObject(ObjectInputStream stream);

private:
    AutoPtr<ILocale> mLocale;

    mutable AutoPtr<ArrayOf<String> > mStrings;

    AutoPtr<ArrayOf<Int32> > mArgumentNumbers;

    AutoPtr<ArrayOf<IFormat*> > mFormats;

    Int32 mMaxOffset;

    mutable Int32 mMaxArgumentIndex;
};

} // namespace Text
} // namespace Elastos

#endif //__MESSAGEFORMAT_H__
