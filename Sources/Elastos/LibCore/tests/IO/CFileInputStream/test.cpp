
#include <elastos.h>
#include <stdio.h>
#include <elautoptr.h>
#include "Elastos.IO2.h"

using namespace Elastos;

int main(int argc, char *argv[])
{
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    String fp = String("/data/data/com.elastos.runtime/elastos/fostest.txt");
    AutoPtr<IFileInputStream> fis;
    CFileInputStream::New(fp, (IFileInputStream**)&fis);
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);
    ArrayOf_<Byte, 6> bytes;
	Int32 number;
    fis->ReadBufferEx(0, 6, &bytes, &number);
    printf("==== File: %s, Line: %d, Num: %d ====\n", __FILE__, __LINE__, number);
	for (Int32 i = 0; i < number; i++) {
		printf("%d ", bytes[i]);
	}
	printf("\n");
    return 0;
}
