#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sqlite3.h>
int userId,teacherId,studentId;
long int date;
char fullName[31],password[21],className[11];
char *locDb = "college.db";
sqlite3 *db;
char *zErrMsg = 0;
int rc = 1;
char sql[2000];
const char* data = "Callback function called";


int callback1(void *ref, int totalColumns,char **columnValue, char **columnName ){
    ref = (char *)ref;

    printf("\n\n%s\n\n",ref);
    int i=0;
        for(i = 0; i < totalColumns; i++){
            printf("%s | %s \n",columnName[i], columnValue[i] ? columnValue[i] : "EMPTY" );
        }
        return 0;
    
     if(!strcmp("userId",ref)){
        if(totalColumns==0)
            rc = 0;
        return 0;
    }
	if(!strcmp("exists",ref)){
        if(totalColumns==0)
            rc = 0;
        return 0;
    }
}


void query(char *doThis,void *inRef){
    sqlite3* db;
    char *error;
    printf("commands are : \n%s\n%s\n\n",doThis,locDb);
    printf("Db open code %d\n",sqlite3_open(locDb,&db));
    printf("Exec code is %d\n",sqlite3_exec(db,doThis,callback1,inRef,&error));
    sqlite3_free(error);
    sqlite3_close(db);
}

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}
static int callbackCheck(void *data,int argc,char **argv,char **azColName)
{
	 int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
	
	/*if(*argv[0]==1)
		return 0;
	return 1;*/
}
void getLoginDetails()
{
	printf("Enter User Id : ");
	scanf("%d",&userId);
	printf("Enter Password : ");
	scanf("%s",password);
}
void getAddData()
{
	printf("Enter name of the member : ");
	scanf("%s",fullName);
	printf("Enter class name of the member : ");
	scanf("%s",className);
	printf("Enter user id of member : ");
	scanf("%d",&userId);
	printf("Enter password of member : ");
	scanf("%s",password);
}
void adminProcess()
{
	getLoginDetails();
	sprintf(sql,"SELECT userId FROM loginData WHERE userId = %d AND type='admin';",userId);
	//rc = sqlite3_exec(db, sql, callback,(void*)data, &zErrMsg);
	query(sql, "userId");
	if( rc != 0)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		printf("Invalid User Id\n");
		//sqlite3_free(zErrMsg);
		getLoginDetails();
	} 
	else /*Right User Id*/
	{
		sprintf(sql,"SELECT COUNT(password) as exists FROM loginData WHERE password='%s' AND type='admin' AND userId=%d;",password,userId);
		//rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
		query(sql, "exists");
		if( rc != SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			printf("Invalid Password\n");
			//sqlite3_free(zErrMsg);
			getLoginDetails();
		} 
		else/*Right Password*/           //Code
		{
			int choiceAdd,noMembers;
			label2 : printf("1.Add Teachers :\n2.Add Students : \n3.Log out\nChoice : ");
			scanf("%d",&choiceAdd);
			
			if(choiceAdd<=3 && choiceAdd>0)
			{
				
				switch(choiceAdd)
				{
					
					case 1 :
							printf("How many teachers to add : ");
							scanf("%d",&noMembers);
							for(int i=1;i<=noMembers;i++)
							{
								getAddData();
								teacherId=userId;
								sprintf(sql,"INSERT INTO loginData (userId,password,type) "  \
												"VALUES (%d, '%s','teacher'); " \
												"INSERT INTO teacherData (teacherId,className,fullName) "  \
												"VALUES (%d, '%s','%s');",userId,password,teacherId,className,fullName);  
								rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
								if( rc != SQLITE_OK )
								{
									fprintf(stderr, "SQL error: %s\n", zErrMsg);
									sqlite3_free(zErrMsg);
									fprintf(stdout, "Unsuccessful Attempt of adding.\n");
								} 
								else
									fprintf(stdout, "Added Members Successfully\n");
								
							}
							goto label2;
					case 2 :
							printf("How many students to add : ");
							scanf("%d",&noMembers);
							for(int i=1;i<=noMembers;i++)
							{
								getAddData();
								studentId=userId;
								sprintf(sql,"INSERT INTO loginData (userId,password,type) "  \
												"VALUES (%d, '%s','student'); " \
												"INSERT INTO studentData (studentId,className,fullName) "  \
												"VALUES (%d, '%s','%s');",userId,password,studentId,className,fullName);  
								rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
								if( rc != SQLITE_OK )
								{
									fprintf(stderr, "SQL error: %s\n", zErrMsg);
									sqlite3_free(zErrMsg);
									fprintf(stdout, "Unsuccessful Attempt of adding.\n");
								} 
								else
									fprintf(stdout, "Added Members Successfully\n");
							}
							goto label2;
					case 3 : 
							exit(0);
				}
			}
			else
			{
				printf("\nInvalid Choice!\nPlease select again.\n");
				goto label2;
			}
			
		}
	}
}
void teacherProcess()
{
	getLoginDetails();
	sprintf(sql,"SELECT COUNT(userId) FROM loginData WHERE userId = %d AND type='teacher';",userId);
	rc = sqlite3_exec(db, sql, callbackCheck,(void*)data, &zErrMsg);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		printf("Invalid User Id\n");
		sqlite3_free(zErrMsg);
		getLoginDetails();
	} 
	else /*Right User Id*/
	{
		sprintf(sql,"SELECT COUNT(password) FROM loginData WHERE password='%s' AND type='teacher' AND userId=%d;",password,userId);
		rc = sqlite3_exec(db, sql, callbackCheck, (void*)data, &zErrMsg);
		if( rc != SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			printf("Invalid Password\n");
			sqlite3_free(zErrMsg);
			getLoginDetails();
		} 
		else/*Right Password*/           //Code
		{
			int choiceTeacher,noPresent;
			label3 : printf("1.Fill Attendance\n2.See Attendance\n3.Log out\nChoice : ");
			scanf("%d",&choiceTeacher);
			switch(choiceTeacher)
			{
				case 1 :
						printf("Enter date (DDMMYY): ");
						scanf("%li",&date);
						printf("Enter no.of present students : ");
						scanf("%d",&noPresent);
						
						/*sprintf(sql,"INSERT INTO attendance (userId,password,type) "  \
												"VALUES (%d, '%s','%s'); " \
												"INSERT INTO studentData (studentId,className,fullName) "  \
												"VALUES (%d, '%s','%s');",userId,password,type,studentId,className,fullName); 
						goto label3;*/
				case 2 :
					
						/*goto label3;*/
				case 3 :
						exit(0);
			}
		}
	
	}
}
void studentProcess()
{
	getLoginDetails();
	sprintf(sql,"SELECT userId FROM loginData WHERE userId = %d AND type='student';",userId);
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		printf("Invalid User Id\n");
		sqlite3_free(zErrMsg);
		getLoginDetails();
	} 
	else /*Right User Id*/
	{
		sprintf(sql,"SELECT password FROM loginData WHERE password='%s' AND type='student';",password);
		rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
		if( rc != SQLITE_OK )
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			printf("Invalid Password\n");
			sqlite3_free(zErrMsg);
			getLoginDetails();
		} 
		else/*Right Password*/           //Code
		{
			
			
		}
	
	}
}
int main(int argc, char* argv[]) 
{  
	
	rc = sqlite3_open("college.db", &db);
   
	if(rc) 
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      	return(0);
	} 
	else 
	{
		fprintf(stdout, "Opened database successfully\n");
	}
	
	int choice;
	printf("------------\n ! Welcome to Attendance Management System ! \n------------\n");
	label1 : printf("Please select type of user : ");
	printf("\n1.Admin\n2.Teacher\n3.Student\nChoice : ");
	scanf("%d",&choice);
	if(choice<=3 && choice>0)
	{
		switch(choice)
		{
			case 1 : adminProcess();
					 break;
			case 2 : teacherProcess();
					 break;
			case 3 : studentProcess();
		}
	}
	else
	{
		printf("\nInvalid Choice!\nPlease select again.\n");
		goto label1;
	}
	
	sqlite3_close(db);
	return 0;
}