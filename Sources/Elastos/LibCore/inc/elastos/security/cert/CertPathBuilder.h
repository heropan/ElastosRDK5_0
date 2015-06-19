
#ifndef __CERTPATHBUILDER_H__
#define __CERTPATHBUILDER_H__



using Elastos::Security::IProvider;

namespace Elastos {
namespace Security {
namespace Cert {

class CertPathBuilder
{
public:
    static CARAPI GetInstance(
    /* [in] */ const String& algorithm,
    /* [out] */ ICertPathBuilder **builder);

    static CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [in] */ const String& provider,
        /* [out] */ ICertPathBuilder **builder);

    static CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [in] */ IProvider *provider,
        /* [out] */ ICertPathBuilder **builder);

    static CARAPI GetDefaultType(
        /* [out] */ String *type);

    CARAPI GetAlgorithm(
        /* [out] */ String *algorithm);

    CARAPI GetProvider(
        /* [out] */ IProvider **provider);

    CARAPI Build(
        /* [in] */ ICertPathParameters* params,
        /* [out] */ ICertPathBuilderResult** result);

protected:
    CertPathBuilder(
        /* [in] */ ICertPathBuilderSpi *builderSpi,
        /* [in] */ IProvider *provider,
        /* [in] */ const String& algorithm);

private:
    // Store CertPathBuilder service name
    static const String sSERVICE;

    //apache...todo
    // Used to access common engine functionality
    //static const Engine ENGINE = new Engine(SERVICE);

    // Store default property name
    static const String sPROPERTYNAME;

    // Default value of CertPathBuilder type. It returns if certpathbuild.type
    // property is not defined in java.security file
    static const String sDEFAULTPROPERTY;

    // Store used provider
    const AutoPtr<IProvider> mProvider;

    // Store spi implementation
    const AutoPtr<ICertPathBuilderSpi> mSpiImpl;

    // Store algorithm name
    const String mAlgorithm;
};

}
}
}

#endif // __CERTPATHBUILDER_H__
