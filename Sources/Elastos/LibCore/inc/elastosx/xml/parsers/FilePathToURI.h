#ifndef __ELASTOSX_XML_PARSERS_FILEPATHTOURI_H__
#define __ELASTOSX_XML_PARSERS_FILEPATHTOURI_H__

#include "Object.h"

namespace Elastosx {
namespace Xml {
namespace Parsers {

#define HEXCHS_NUM 16

class FilePathToURI
{
public:
    static String Filepath2URI(
        /* [in] */ String path);

private:
    static Boolean Init();

    // which ASCII characters need to be escaped
    static Boolean gNeedEscaping[128];
    // the first hex character if a character needs to be escaped
    static char gAfterEscaping1[128];
    // the second hex character if a character needs to be escaped
    static char gAfterEscaping2[128];
    static char gHexChs[HEXCHS_NUM];

    static Boolean sIsInitStatic;
};

} // namespace Parsers
} // namespace Xml
} // namespace Elastosx

#endif
