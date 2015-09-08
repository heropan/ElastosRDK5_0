
#ifndef __ELASTOS_SECURITY_CPROVIDERSERVICE_H__
#define __ELASTOS_SECURITY_CPROVIDERSERVICE_H__

#include "coredef.h"
#include "_Elastos_Security_CProviderService.h"
#include "core/Object.h"

using Elastos::Core::Object;
using Elastos::Utility::IList;
using Elastos::Utility::IMap;

namespace Elastos {
namespace Security {

CarClass(CProviderService)
    , public Object
    , public IProviderService
{
public:
    CARAPI constructor(
        /* [in] */ IProvider* provider,
        /* [in] */ const String& type,
        /* [in] */ const String& algorithm,
        /* [in] */ const String& className,
        /* [in] */ IList* aliases,
        /* [in] */ IMap* attributes);

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /**
     * Adds an alias.
     *
     * @param alias the alias to add
     */
    CARAPI AddAlias(
        /* [in] */ const String& alias);

    /**
     * Puts a new attribute mapping.
     *
     * @param name the attribute name.
     * @param value the attribute value.
     */
    CARAPI PutAttribute(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    /**
     * Returns the type of this {@code Service}. For example {@code
     * KeyPairGenerator}.
     *
     * @return the type of this {@code Service}.
     */
    CARAPI GetType(
        /* [out] */ String* type);

    /**
     * Returns the name of the algorithm implemented by this {@code
     * Service}.
     *
     * @return the name of the algorithm implemented by this {@code
     *         Service}.
     */
    CARAPI GetAlgorithm(
        /* [out] */ String* algorithm);

    /**
     * Returns the {@code Provider} this {@code Service} belongs to.
     *
     * @return the {@code Provider} this {@code Service} belongs to.
     */
    CARAPI GetProvider(
        /* [out] */ IProvider** provider);

    /**
     * Returns the name of the class implementing this {@code Service}.
     *
     * @return the name of the class implementing this {@code Service}.
     */
    CARAPI GetClassName(
        /* [out] */ String* className);

    CARAPI SetClassName(
        /* [in] */ const String& className);

    /**
     * Returns the value of the attribute with the specified {@code name}.
     *
     * @param name
     *            the name of the attribute.
     * @return the value of the attribute, or {@code null} if no attribute
     *         with the given name is set.
     * @throws NullPointerException
     *             if {@code name} is {@code null}.
     */
    CARAPI GetAttribute(
        /* [in] */ const String& name,
        /* [out] */ String* attrib);

    CARAPI GetAliases(
        /* [out] */ IList** aliases);

    /**
     * Creates and returns a new instance of the implementation described by
     * this {@code Service}.
     *
     * @param constructorParameter
     *            the parameter that is used by the constructor, or {@code
     *            null} if the implementation does not declare a constructor
     *            parameter.
     * @return a new instance of the implementation described by this
     *         {@code Service}.
     * @throws NoSuchAlgorithmException
     *             if the instance could not be constructed.
     * @throws InvalidParameterException
     *             if the implementation does not support the specified
     *             {@code constructorParameter}.
     */
    CARAPI NewInstance(
        /* [in] */ IInterface* constructorParameter,
        /* [out] */ IInterface** instance);

    /**
     * Indicates whether this {@code Service} supports the specified
     * constructor parameter.
     *
     * @param parameter
     *            the parameter to test.
     * @return {@code true} if this {@code Service} supports the specified
     *         constructor parameter, {@code false} otherwise.
     */
    CARAPI SupportsParameter(
        /* [in] */ IInterface* parameter,
        /* [out] */ Boolean* isSupported);

    CARAPI GetAttributes(
        /* [out] */ IMap** attributes);
};

} // namespace Security
} // namespace Elastos

#endif //__ELASTOS_SECURITY_CPROVIDERSERVICE_H__
