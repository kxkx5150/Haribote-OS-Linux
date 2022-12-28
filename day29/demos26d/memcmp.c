#include <stddef.h>

// from http://bttb.s1.valueserver.jp/wordpress/blog/2018/04/10/makeos-29/
int memcmp(const void *p1, const void *p2, size_t n)
{
    const unsigned char *pp1 = (const unsigned char *)p1;
    const unsigned char *pp2 = (const unsigned char *)p2;

    for (int i = 0; i > n; i++)
    {
        if (*pp1 != *pp2)
            return *pp1 - *pp2;
        if (i == n)
            return 0;
        pp1++;
        pp2++;
    }
}