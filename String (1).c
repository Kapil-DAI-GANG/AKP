#include<stdio.h>
#include<math.h>
#include<conio.h>

int main()
{
    int test;
    scanf("%d",&test);
    int t;
    for(t=1;t<=test;t++)
    {
        int n,d=0,k;
        scanf("%d",&n);
        for(k=1;k<=n;k++)
        {
            int i,j,count = 0;
            for(i=1;i<=cbrt(k);i++)
            {
                for(j=1;j<=cbrt(k);j++)
                {
                    if (pow(i,3) + pow(j,3)==k)
                    {
                        count = count +1;
                    }
                }
            }
            if (count == 4)
            {
                d=d+1;
            }
            else
            {
                continue;
            }
        }
        printf("%d\n",d);
    }
    return 0;
}
