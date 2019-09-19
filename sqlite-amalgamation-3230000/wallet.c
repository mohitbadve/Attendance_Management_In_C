#include<stdio.h>
#include<sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


char *locDb = "wallet.db";
int loginFlag = 1,cUsernameFlag = 1, cMobFlag = 1;
char balanceOfMainUser[20],balanceOfFriend[20];
void clear (){
    while(getchar()!='\n');
}

int callback(void *ref, int totalColumns,char **columnValue, char **columnName ){
    ref = (char *)ref;

    printf("\n\n%s\n\n",ref);
    int i=0;
    if(!strcmp("Display!",ref)){
        for(i = 0; i < totalColumns; i++){
            printf("%s | %s \n",columnName[i], columnValue[i] ? columnValue[i] : "EMPTY" );
        }
        return 0;
    }
    if(!strcmp("check",ref)){
        if(totalColumns==0)
            loginFlag = 0;
        return 0;
    }
    if(!strcmp("checkBal",ref)){
        //printf("Your balance is Rs. %s\n",columnValue[0]);
        strcpy(balanceOfMainUser,columnValue[0]);
        printf("Done\n");
        return 0;
    }
    if(!strcmp("add",ref)){
        printf("Account created successfully!\n");
        return 0;
    }
    if(!strcmp("addM",ref)){
        printf("Money added successfully!\n");
        return 0;
    }
    if(!strcmp("checkU",ref)){
        if(totalColumns==0)
            cUsernameFlag = 0;
        return 0;
    }
    if(!strcmp("checkM",ref)){
        if(totalColumns==0)
            cMobFlag = 0;
        return 0;
    }
    if(!strcmp("checkFbal",ref)){
        strcpy(balanceOfFriend,columnValue[0]);
        printf("Bal F : %s\n\n",balanceOfFriend);
        printf("Done\n");
        return 0;
    }
    if(!strcmp("update",ref)){
        return 0;
    }
}

void query(char *doThis,void *inRef){
    sqlite3* db;
    char *error;
    printf("commands are : \n%s\n%s\n\n",doThis,locDb);
    printf("Db open code %d\n",sqlite3_open(locDb,&db));
    printf("Exec code is %d\n",sqlite3_exec(db,doThis,callback,inRef,&error));
    sqlite3_free(error);
    sqlite3_close(db);
}

int login(char *username, char *password){
    char login[200];
    sprintf(login,"SELECT * FROM accinfo WHERE username = '%s' AND password = '%s';",username,password);
    query(login,"check");
    if(loginFlag == 1){
        return 0;
    }
    else{
        loginFlag = 1;
        return 1;
    }
}

void signUp(){
    char mob[11],name[30],surname[30],username[30],password[30],secQ[100],answer[20],accinfo[1000],security[1000],amount[200],transaction[1000];
    time_t t;
    struct tm *local;

    printf("Enter name :");
    gets(name);
    printf("Enter surname :");
    gets(surname);
    printf("Enter username :");
    gets(username);
    printf("Enter password :");
    gets(password);
    printf("Enter mobile number : ");
    gets(mob);
    printf("Enter security question : ");
    gets(secQ);
    printf("Enter answer :");
    gets(answer);
    sprintf(accinfo,"INSERT INTO accinfo (name, surname, username, password, mob) VALUES ('%s', '%s', '%s', '%s', '%s')",name,surname,username,password,mob);
    sprintf(security,"INSERT INTO security (username, question, answer) VALUES ('%s', '%s', '%s')",username, secQ, answer);
    sprintf(amount,"INSERT INTO amount (username) VALUES ('%s')",username);
    t=time(NULL);
    local = localtime(&t);
    sprintf(transaction,"INSERT INTO transactions (from_username, type, time, date) VALUES ('%s','Account created.','%d:%02d:%02d','%d-%d-%d');",
    username,local->tm_hour,local->tm_min,local->tm_sec,local->tm_mday,local->tm_mon+1,local->tm_year+1900);
    query(accinfo,"add");
    query(security,"add");
    query(amount,"add");
    query(transaction,"add");
    //clear();
}

int checkMobNo(char *mobileNo1){
    char checkMob[200];
    sprintf(checkMob,"SELECT mob FROM accinfo WHERE mob = '%s';",mobileNo1);
    query(checkMob,"checkM");
    if(cMobFlag==1){
        return 0;
        printf("Entered mobile number exists.\n" );
    }
    else{
        cMobFlag = 1;
        printf("Entered mobile number does not exist.\n" );
        return 1;
    }
}

int checkUsername(char *username1){
    char checkUsername[200];
    sprintf(checkUsername,"SELECT username FROM accinfo WHERE username = '%s';",username1);
    query(checkUsername,"checkU");
    if(cUsernameFlag==1){
        return 0;
        printf("Entered username exists.\n" );
    }
    else{
        cUsernameFlag = 1;
        printf("Entered username does not exist.\n" );
        return 1;
    }
}


void afterLogin(char *username){
    char balance[200],addMoney[200], transaction[1000],username1[30],mobileNo1[11],transaction1[1000],removeMoney[200];
    int choice,myBal,moneyToAdd,totalBalMainUser,friendBal,totalFriendBal, mFlag=1, uFlag=1, choice1;
    time_t t;
    struct tm *local;
    while(1){
        sprintf(balance, "SELECT balance FROM amount WHERE username = '%s';",username);
        query(balance, "checkBal");
        myBal = atoi(balanceOfMainUser);
        printf("1. View balance\n2. Add money\n3. Send money to friend\n");
        printf("Enter choice : ");
        scanf("%d",&choice);
        switch (choice) {
            case 1:


                printf("%d\n",myBal);
                break;
            case 2:
                printf("How much money do you want to add : ");
                scanf("%d",&moneyToAdd);
                totalBalMainUser = myBal + moneyToAdd;
                sprintf(addMoney,"UPDATE amount SET balance = %d WHERE username = '%s';",totalBalMainUser, username);
                t=time(NULL);
                local = localtime(&t);
                sprintf(transaction,"INSERT INTO transactions (from_username, type, time, date) VALUES ('%s','Added money Rs. %d.','%d:%02d:%02d','%d-%d-%d');",
                username,moneyToAdd,local->tm_hour,local->tm_min,local->tm_sec,local->tm_mday,local->tm_mon+1,local->tm_year+1900);
                query(addMoney,"update");
                query(balance,"checkBal");
                query(transaction,"update");
                myBal = atoi(balanceOfMainUser);
                break;
            case 3:

                printf("Send money by :\n1. Username\n2. Mobile No.\n3. Exit\nEnter choice : ");
                scanf("%d",&choice1);
                clear();
                switch (choice1) {
                    case 1:
                        printf("Enter username : " );
                        gets(username1);
                        uFlag = checkUsername(username1);
                        if(uFlag == 0){
                            sprintf(balance,"SELECT balance FROM amount WHERE username = '%s';",username1);
                            query(balance, "checkFbal");
                            friendBal = atoi(balanceOfFriend);
                        }
                        break;
                    case 2:
                        printf("Enter mobile number : ");
                        gets(mobileNo1);
                        mFlag = checkMobNo(mobileNo1);
                        if(mFlag == 0){
                            sprintf(balance,"SELECT balance FROM amount WHERE mob = '%s';",mobileNo1);
                            query(balance,"checkFbal");
                            friendBal = atoi(balanceOfFriend);
                        }
                        break;
                    case 3:
                        break;
                    default:
                        printf("Wrong input.\n");



                }
                if(mFlag == 0 || uFlag == 0){
                    printf("Enter the amount you want to send : ");
                    scanf("%d",&moneyToAdd);
                    totalFriendBal = friendBal + moneyToAdd;
                    totalBalMainUser = myBal - moneyToAdd;
                    if(choice1 == 1)
                        sprintf(addMoney,"UPDATE amount SET balance = %d WHERE username = '%s';",totalFriendBal,username1);
                    else if(choice1 == 2 )
                        sprintf(addMoney,"UPDATE amount SET balance = %d WHERE mob = '%s';",totalFriendBal,mobileNo1);
                    sprintf(removeMoney,"UPDATE amount SET balance = %d WHERE username = '%s';",totalBalMainUser,username);
                    t=time(NULL);
                    local = localtime(&t);

                    sprintf(transaction,"INSERT INTO transactions (from_username,to_username, type, time, date) VALUES ('%s', '%s','Sent Rs. %d.','%d:%02d:%02d','%d-%d-%d');",
                    username,username1,moneyToAdd,local->tm_hour,local->tm_min,local->tm_sec,local->tm_mday,local->tm_mon+1,local->tm_year+1900);

                    sprintf(transaction1,"INSERT INTO transactions (from_username,to_username, type, time, date) VALUES ('%s','%s','Received Rs. %d.','%d:%02d:%02d','%d-%d-%d');",
                    username,username1,moneyToAdd,local->tm_hour,local->tm_min,local->tm_sec,local->tm_mday,local->tm_mon+1,local->tm_year+1900);

                    query(addMoney,"update");
                    query(removeMoney,"update");
                    query(transaction,"update");
                    query(transaction1,"update");


                }

        }
    }
}


void main(){
    char username[30],password[30];
    int flag,choice;

    if(flag == 0)
        printf("Login successful\n");
    while(1){
        printf("1. Sign Up / Create an account\n2. Login\n");
        printf("Enter your choice : ");
        scanf("%d",&choice);
        clear();
        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                printf("Enter username :");
                gets(username);
                printf("Enter password :");
                gets(password);
                flag = login(username,password);
                if(flag == 0){
                    printf("Login successful!\n");
                    afterLogin(username);
                }
                break;
            default:
                printf("Wrong input.\n" );
        }
    }
}
