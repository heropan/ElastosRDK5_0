#ifndef __FILEPATHTOURI_H__
#define __FILEPATHTOURI_H__

namespace Elastosx {
namespace Xml {
namespace Parsers {

class FilePathToURI
{

private:
    // which ASCII characters need to be escaped
    static Boolean gNeedEscaping[128];
    // the first hex character if a character needs to be escaped
    static char gAfterEscaping1[128];
    // the second hex character if a character needs to be escaped
    static char gAfterEscaping2[128];
    static char[] gHexChs = {'0', '1', '2', '3', '4', '5', '6', '7',
             '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
}

} // namespace Parsers
} // namespace Xml
} // namespace Elastosx
#endif