
#include "CParserFactory.h"
#include "NewInstanceHelper.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

ECode CParserFactory::MakeParser(
    /* [out] */ IParser** parser)
{
    return ParserFactory::MakeParser(parser);
}

ECode CParserFactory::MakeParser(
    /* [in] */ const String& className,
    /* [out] */ IParser** parser)
{
    return ParserFactory::MakeParser(className, parser);
}

ECode ParserFactory::MakeParser(
    /* [out] */ IParser** parser)
{
    VALIDATE_NOT_NULL(parser);

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String className;
    system->GetProperty(String("org.xml.sax.parser"), &className);
    if (className.IsNullOrEmpty()) {
//        throw new NullPointerException("No value for sax.parser property");
        return E_NULL_POINTER_EXCEPTION;
    } else {
        return ParserFactory::MakeParser(className, parser);
    }

    *parser = NULL;
    return NOERROR;
}

ECode ParserFactory::MakeParser(
    /* [in] */ const String& className,
    /* [out] */ IParser** parser)
{
    VALIDATE_NOT_NULL(parser);

    AutoPtr<IClassLoader> icl = NewInstanceHelper::GetClassLoader();
    AutoPtr<IInterface> ointer = NewInstanceHelper::NewInstance(icl, className);
    *parser = IParser::Probe(ointer.Get());
    INTERFACE_ADDREF(*parser)

    return NOERROR;
}

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org
