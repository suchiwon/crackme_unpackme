#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _FIELD1     0x1d5
#define _FIELD2     0x1b2
#define SPLITKEY    0x01

FILE *fp;

int main(int argc, char *argv[])
{
    unsigned int nKey, nSum;
    int          nCount, nPos;
    
    char  szTmp[128] = {0,};
    
    printf("==============================================\n");
    printf("     Crackme 2 Solution... by ryang...\n");
    printf("==============================================\n\n");

    nKey   = 0; 
    nCount = 0;
    nPos   = 0;
    nSum   = _FIELD1;
    
    srand(time(NULL));
    while (1) {
        nKey = (unsigned int)(rand() % 30 + 31);
        
        if (nCount < 8 && nSum < nKey)
           continue;
        
        if (nCount >= 8 && nSum < nKey)
           nKey = nSum;   
            
        szTmp[nCount++] = nKey;
        nSum -= nKey;
        
        if (nSum == 0)
           break;
    }
    szTmp[nCount++] = SPLITKEY;
    szTmp[nCount++] = SPLITKEY;
    
    nPos   = nCount;
    nKey   = 0;
    nCount = 0;
    nSum   = _FIELD2;
    
    while (1) {
        nKey = (unsigned int)(rand() % 62 + 2);
        
        if (nCount < 8 && nSum < nKey)
           continue;
        
        if (nCount >= 8 && nSum < nKey)
           nKey = nSum;   
        
        nCount++;    
        szTmp[nPos++] = nKey;
        nSum -= nKey;
        
        if (nSum == 0)
           break;
    }
  
    // Write code..
    fp = fopen("due-cm2.dat", "wt");
    if (!fp) {
        printf("파일을 열 수 없습니다..\n");
        return 1;         
    }
    
    fprintf(fp, "%s", szTmp);
    fclose(fp);
    
    printf("Keygen.. Complete...\n"
           "Enjoy plming~~ :)...\n\n");    

    system("PAUSE");
    return 0;
}
