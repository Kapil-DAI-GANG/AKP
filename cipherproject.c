#include<stdio.h>
#include<conio.h>
#include<windows.h>
//#include<stdlib.h>
#include<ctype.h>

void encrypt(int);
void decrypt(int);
void opten();
void optde();
COORD co={0,0};
int main()
{
	int c=72,e=1;
	system("cls");
	fflush(stdin);
	HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD max = GetLargestConsoleWindowSize( Screen );


    co.X=(max.X-8)/2;
    co.Y=max.Y/2;
    SetConsoleCursorPosition(Screen,co);
	printf("Encrypt");
	co.X=(max.X-8)/2;
	co.Y=max.Y/2+1;
	SetConsoleCursorPosition(Screen,co);
	printf("Decrypt");
	while(e!=4)
	{
	    if(c==27)//Esc
            break;

        if(c==13 && e==1)//13=enter
            opten();
        if(c==13 && e==2)
            optde();

		if(c==72)//UP
		{
            co.X=(max.X-8)/2-1;
            co.Y=max.Y/2+1;
		    SetConsoleCursorPosition(Screen, co);
		    printf(" ");
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2;
		    SetConsoleCursorPosition(Screen, co);
		    printf(">");
		    e=1;

		}
		if(c==80)//DOWN
		{
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2+1;
			SetConsoleCursorPosition(Screen, co);
		    printf(">");
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2;
		    SetConsoleCursorPosition(Screen, co);
		    printf(" ");
		    e=2;
		}
		if(c==75)//LEFT
		{

		}
		if(c==77)//Right
		{

		}
		c=getch();
	}
return 0;
}


void opten()
{
	int c=72,e=1;
	system("cls");
	fflush(stdin);
	HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD max = GetLargestConsoleWindowSize( Screen );


    co.X=(max.X-8)/2;
    co.Y=max.Y/2;
    SetConsoleCursorPosition(Screen,co);
	printf("Encrypt file");
	co.X=(max.X-8)/2;
	co.Y=max.Y/2+1;
	SetConsoleCursorPosition(Screen,co);
	printf("Encrypt Msg");
	while(e!=4)
	{

		if(c==27)//Esc
            break;
        if(c==13 && e==1)//13=enter
        {
            encrypt(e);
        }
        if(c==13 && e==2)
            encrypt(e);

		if(c==72)//UP
		{
            co.X=(max.X-8)/2-1;
            co.Y=max.Y/2+1;
		    SetConsoleCursorPosition(Screen, co);
		    printf(" ");
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2;
		    SetConsoleCursorPosition(Screen, co);
		    printf(">");
		    e=1;

		}
		if(c==80)//DOWN
		{
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2+1;
			SetConsoleCursorPosition(Screen, co);
		    printf(">");
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2;
		    SetConsoleCursorPosition(Screen, co);
		    printf(" ");
		    e=2;
		}
		if(c==75)//LEFT
		{

		}
		if(c==77)//Right
		{

		}
		c=getch();

	}
	exit(0);
}

void optde()
{
    int c=72,e=1;
	system("cls");
	fflush(stdin);
	HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD max = GetLargestConsoleWindowSize( Screen );


    co.X=(max.X-8)/2;
    co.Y=max.Y/2;
    SetConsoleCursorPosition(Screen,co);
	printf("Decrypt file");
	co.X=(max.X-8)/2;
	co.Y=max.Y/2+1;
	SetConsoleCursorPosition(Screen,co);
	printf("Decrypt Msg");
	while(e!=4)
	{

		if(c==27)//Esc
            break;
        if(c==13 && e==1)//13=enter
            decrypt(e);
        if(c==13 && e==2)
            decrypt(e);

		if(c==72)//UP
		{
            co.X=(max.X-8)/2-1;
            co.Y=max.Y/2+1;
		    SetConsoleCursorPosition(Screen, co);
		    printf(" ");
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2;
		    SetConsoleCursorPosition(Screen, co);
		    printf(">");
		    e=1;

		}
		if(c==80)//DOWN
		{
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2+1;
			SetConsoleCursorPosition(Screen, co);
		    printf(">");
		    co.X=(max.X-8)/2-1;
		    co.Y=max.Y/2;
		    SetConsoleCursorPosition(Screen, co);
		    printf(" ");
		    e=2;
		}
		if(c==75)//LEFT
		{

		}
		if(c==77)//Right
		{

		}
		c=getch();

	}
	exit(0);
}



void encrypt(int option){
    fflush(stdin);
    char msg[10000],path[1000],ch,temp;
    char key[1000];
    int cnt=0;
    FILE *fp;
    system("cls");
    if(option==2)
    {
        printf("Enter the messasge to encrypt: ");
        scanf("%[^\n]",msg);
    }
    else if(option==1)
    {
        printf("Enter the path of the file(eg; C:\\directory\\..\\filename):  ");
        scanf("%[^\n]",path);
        fp=fopen(path,"r");
        if(fp==NULL)
        {
            printf("Error opening file");
            main();
        }
        while((ch=fgetc(fp))!=EOF)
        {
            msg[cnt]=ch;
            cnt++;
        }
        fclose(fp);

    }
    printf("Enter a key: ");
    scanf("%c",&temp);
    scanf("%[^\n]s",key);
    strupr(msg);
    strupr(key);
    int msgLen = strlen(msg), keyLen = strlen(key), i, j;

    char newKey[msgLen], encryptedMsg[msgLen];


    //generating new key
    for(i = 0, j = 0; i < msgLen; ++i, ++j){
            if(j == keyLen)
                j = 0;

            if(isalpha(key[j]))
                newKey[i] = key[j];
            else
                i=i-1;
    }

    newKey[i] = '\0';

    //encryption
    for(i = 0; i < msgLen; ++i)
    {
        if(isalpha(msg[i]))
            encryptedMsg[i] = ((msg[i] + newKey[i]) % 26) + 'A';
        else
            encryptedMsg[i] = msg[i];
    }

    encryptedMsg[i] = '\0';

    if(option==2)
    {
    printf("Original Message: %s", msg);
    /*printf("\nKey: %s", key);
    printf("\nNew Generated Key: %s", newKey);*/
    printf("\nEncrypted Message: %s", encryptedMsg);
    }
    else if(option==1)
    {
        fp=fopen(path,"w");
        fwrite(encryptedMsg,sizeof(encryptedMsg),1,fp);
        fclose(fp);
        printf("Success!");
    }
    getch();
    fflush(stdin);
    main();
}



void decrypt(int option)
{
    fflush(stdin);
    char msg[10000],path[1000],ch,temp;
    char key[1000];
    int cnt=0;
    FILE *fp;
    system("cls");
    if(option==2)
    {
        printf("Enter the messasge to decrypt: ");
        scanf("%[^\n]",msg);
    }

    else if(option==1)
    {
        printf("Enter the path of the file(eg; C:\\directory\\..\\filename):  ");
        scanf("%[^\n]",path);
        fp=fopen(path,"r");
        if(fp==NULL)
        {
            printf("Error opening file");
            main();
        }
        while((ch=fgetc(fp))!=EOF)
        {
            msg[cnt]=ch;
            cnt++;
        }
        fclose(fp);

    }
    printf("Enter the key: ");
    scanf("%c",&temp);
    scanf("%[^\n]",key);
    strupr(msg);
    strupr(key);
    int msgLen = strlen(msg), keyLen = strlen(key), i, j;

    char newKey[msgLen], decryptedMsg[msgLen];


    //generating new key
    for(i = 0, j = 0; i < msgLen; ++i, ++j){
            if(j == keyLen)
                j = 0;

            if(isalpha(key[j]))
                newKey[i] = key[j];
            else
                i=i-1;
    }

    newKey[i] = '\0';

    //decryption
    for(i = 0; i < msgLen; ++i)
    {
        if(isalpha(msg[i]))
            decryptedMsg[i] = (((msg[i] - newKey[i]) + 26) % 26) + 'A';
        else
            decryptedMsg[i]=msg[i];
    }

    decryptedMsg[i] = '\0';

    if(option==2)
    {
    printf("Original Message: %s", msg);
    /*printf("\nKey: %s", key);
    printf("\nNew Generated Key: %s", newKey);*/
    printf("\nDecrypted Message: %s", decryptedMsg);
    }
    else if(option==1)
    {
        fp=fopen(path,"w");
        fwrite(decryptedMsg,sizeof(decryptedMsg),1,fp);
        fclose(fp);
        printf("Success!");
    }
    getch();
    fflush(stdin);
    main();
}
