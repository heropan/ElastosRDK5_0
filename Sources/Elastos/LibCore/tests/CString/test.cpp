
#include <elastos.h>
#include <stdio.h>
#include <new>

using namespace Elastos;

CString f1()
{
    CString tmp("Hello World");
    return tmp;
}

void f2(const CString& s)
{
    CString s1 = s;
    s1 = "Hello World!";
}

void f3(CString& s)
{
    s = "Hello World!";
}

void f4(CString s)
{
    s = "Hello World!";
}

void f5(const char* p)
{
    p = "Hello World..";
    printf("%s\n", p);
}

int main(int argc, char *argv[])
{
    CString s1(NULL);
    CString s2;
    CString s3("Hello World");

    printf("s2:%s\n", (const char*)s2);
    printf("s3:%s\n", (const char*)s3);

    CString s4 = CString::Duplicate(s3);
    printf("s4:%s\n", (const char*)s4);

    CString::Free(s4);

    String s6("    Hello World!    ");
    printf("s6 trimStart :%s\n", (const char*)s6.TrimStart());
    printf("s6 trimEnd :%s\n", (const char*)s6.TrimEnd());
    printf("s6 trim:%s\n", (const char*)s6.Trim());

    CString s7 = f1();
    printf("s7:%s\n", (const char*)s7);

    CString s8;
    f2(s8);
    printf("s8:%s\n", (const char*)s8);

    String s9;
    s9 = s3;
    printf("s9:%s\n", (const char*)s9);

    CString s10(s9.string());
    printf("s10:%s\n", (const char*)s10);

    CString s11;
    s11 = s9.string();
    printf("s11:%s\n", (const char*)s11);

    CString s12 = s9;
    printf("s12:%s\n", (const char*)s12);

    CString s13;
    s13 = s12;
    printf("s13:%s\n", (const char*)s13);

    String s14(s13);
    printf("s14:%s\n", (const char*)s14);

    String s15 = String(s13);
    printf("s15:%s\n", (const char*)s15);

    f2(s15);
    f4(s15);
    f4("Hello");

    String s16;
    s16 = s13;
    printf("s16:%s\n", (const char*)s16);

    CString s17(s15.string());
    f3(s17);

    f5("world");

    return 0;
}
