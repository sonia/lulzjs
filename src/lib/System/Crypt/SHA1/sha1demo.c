#include <stdlib.h>
#include <stdio.h>

#include "sha1.h"

int main (int argc, char** argv)
{
    int i;
    SHA1_ctx ctx;
    char digest[41];

    const char *res_vectors[] = {
        "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709",
        "A9993E364706816ABA3E25717850C26C9CD0D89D",
        "84983E441C3BD26EBAAE4AA1F95129E5E54670F1",
        "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F" };

    char *test_vectors[4] = {
        "", "abc", "abcdbcdecdefdefgefghfghighijhijki"
        "jkljklmklmnlmnomnopnopq" };

    test_vectors[3] = (char *) malloc(sizeof(char) * 1000000);
    for (i = 0; i < 1000000; i++)
        test_vectors[3][i] = 'a';
    
    printf("Test Vectors (from FIPS PUB 180-1)\n"
           "<http://www.itl.nist.gov/fipspubs/fip180-1.htm>\n");

    for (i = 0; i < 4; i++) {
        putchar('\n');
        SHA1_init(&ctx);
        SHA1_update(&ctx, (uint8_t *) test_vectors[i], strlen(test_vectors[i]));
        SHA1_to_str(&ctx, digest);
        printf("%s\n%s\n",digest,res_vectors[i]);
    }

    putchar('\n');
    exit(0);
}
