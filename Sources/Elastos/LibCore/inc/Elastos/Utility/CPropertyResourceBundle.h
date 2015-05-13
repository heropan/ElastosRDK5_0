#ifndef __UTILITY_CPROPERTY_RESOURCE_BUNDLE_H__
#define __UTILITY_CPROPERTY_RESOURCE_BUNDLE_H__

#include "_CPropertyResourceBundle.h"
#include "cmdef.h"
#include "ResourceBundle.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IReader;

namespace Elastos {
namespace Utility {

CarClass(CPropertyResourceBundle), public ResourceBundle
{
private:
    class _Enumeration
        : public IEnumeration
        , public ElRefBase
    {
    public:
        _Enumeration(
            /* [in] */ CPropertyResourceBundle* host);

        CAR_INTERFACE_DECL()

        /**
         * Returns whether this {@code Enumeration} has more elements.
         *
         * @return {@code true} if there are more elements, {@code false} otherwise.
         * @see #nextElement
         */
        CARAPI HasMoreElements(
            /* [out] */ Boolean * value);

        /**
         * Returns the next element in this {@code Enumeration}.
         *
         * @return the next element..
         * @throws NoSuchElementException
         *             if there are no more elements.
         * @see #hasMoreElements
         */
        CARAPI NextElement(
            /* [out] */ IInterface ** inter);

    private:
        CARAPI_(Boolean) FindNext();

    private:
        AutoPtr<IEnumeration> mLocal; // = getLocalKeys();

        AutoPtr<IEnumeration> mPEnum; // = parent.getKeys();

        String mNextElement;

        AutoPtr<CPropertyResourceBundle> mHost;
    };
public:
    CARAPI constructor();

    /**
     * Constructs a new instance of {@code PropertyResourceBundle} and loads the
     * properties file from the specified {@code InputStream}.
     *
     * @param stream
     *            the {@code InputStream}.
     * @throws IOException
     *             if an error occurs during a read operation on the
     *             {@code InputStream}.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* stream);

    /**
     * Constructs a new resource bundle with properties read from {@code reader}.
     *
     * @param reader the {@code Reader}
     * @throws IOException
     * @since 1.6
     */
    CARAPI constructor(
        /* [in] */ IReader* reader);

    /**
     * Returns the names of the resources contained in this {@code ResourceBundle}.
     *
     * @return an {@code Enumeration} of the resource names.
     */
    CARAPI GetKeys(
        /* [out] */ IEnumeration** enu);

    /**
     * Gets the {@code Locale} of this {@code ResourceBundle}. In case a bundle was not
     * found for the requested {@code Locale}, this will return the actual {@code Locale} of
     * this resource bundle that was found after doing a fallback.
     *
     * @return the {@code Locale} of this {@code ResourceBundle}.
     */
    CARAPI GetLocale(
        /* [out] */ ILocale** loc);

    /**
     * Returns the named resource from this {@code ResourceBundle}. If the resource
     * cannot be found in this bundle, it falls back to the parent bundle (if
     * it's not null) by calling the {@link #handleGetObject} method. If the resource still
     * can't be found it throws a {@code MissingResourceException}.
     *
     * @param key
     *            the name of the resource.
     * @return the resource object.
     * @throws MissingResourceException
     *                if the resource is not found.
     */
    CARAPI GetObject(
        /* [in] */ const String& key,
        /* [out] */ IInterface** outface);

    /**
     * Returns the named string resource from this {@code ResourceBundle}.
     *
     * @param key
     *            the name of the resource.
     * @return the resource string.
     * @throws MissingResourceException
     *                if the resource is not found.
     * @throws ClassCastException
     *                if the resource found is not a string.
     * @see #getObject(String)
     */
    CARAPI GetString(
        /* [in] */ const String& key,
        /* [out] */ String* str);

    /**
     * Returns the named resource from this {@code ResourceBundle}.
     *
     * @param key
     *            the name of the resource.
     * @return the resource string array.
     * @throws MissingResourceException
     *                if the resource is not found.
     * @throws ClassCastException
     *                if the resource found is not an array of strings.
     * @see #getObject(String)
     */
    CARAPI GetStringArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<String>** arrstr);

    CARAPI ContainsKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* value);

    CARAPI HandleGetObject(
        /* [in] */ const String& key,
        /* [out] */ IInterface** outface);

protected:
    CARAPI HandleKeySet(
        /* [out] */ ISet** outset);

private:
    AutoPtr<IEnumeration> GetLocalKeys();

private:
    AutoPtr<IProperties> mResources;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CPROPERTY_RESOURCE_BUNDLE_H__