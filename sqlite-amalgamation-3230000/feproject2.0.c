#include <stdio.h>
#include <sqlite3.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>

int rc = 0;
char sqlStmt[200] = "";
char data[200] = "";

int userId,teacherId,studentId;
char password[21],firstName[11],lastName[11],fullName[31],className[11];
int presentOrAbsent;
int studentIds[100];
int presenty[100];
int counter=0;
char cpy[150];


static int callback(void *data, int argc, char **argv, char **azColName);
int query(char sqlStmt[], void *data);
void reset();
int doValidate(char typePass[]);
void addMember(char typePass[]);
void callAdmin();
void callTeacher();
void callStudent();
void attendance(char datePass[]);
void markPresenty(int,int);
void viewAttendance();


int main(int argc, char* argv[]) {
   	int choice;
	label2 : printf("\n-------------------------------------------------\n\n   ! Welcome to Attendance Management System ! \n\n-------------------------------------------------\n\n");
	printf("Please select type of user : ");
	printf("\n1.Admin\n2.Teacher\n3.Student\n\nChoice : ");
	scanf("%d",&choice);
	if(choice<=3 && choice>0)
	{	
		printf("\nEnter user id number : ");
		scanf("%d",&userId);
		printf("Enter password : ");
		scanf("%s",password);
		switch(choice)
		{
			int checklog;
			case 1 : 	checklog=doValidate("admin");
						if(checklog==0)
							callAdmin();
						else{
							printf("\nInvalid Credentials!\nEnter Again\n\n");
							goto label2;
						}
						break;
			case 2 : 	checklog=doValidate("teacher");
						if(checklog==0)
							callTeacher();
						else{
							printf("\nInvalid Credentials!\nEnter Again\n\n");
							goto label2;
						}
						break;
			case 3 :	checklog=doValidate("student");
						if(checklog==0)
							callStudent();
						else{
							printf("\nInvalid Credentials!\nEnter Again\n\n");
							goto label2;
						}
		}
	}
	else
	{
		printf("\nInvalid Choice!\nPlease select again\n");
		goto label2;
	}
   //sprintf(str1,"SELECT * from COMPANY where age < %d;",n);
   //int insertResult;
   //insertResult = query(str1);
   //printf("%d", insertResult);
   
}
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   data = (char *)data;
   //printf("%s\n", data);
   if(!strcmp(data,"checkLogin")){
		//printf("%c",*argv[0]);
		if(*argv[0]=='1')
			return 0;
	}
	/*if(!strcmp(data,"Add")){
		printf("\nAdded Members Successfully!\n");
		return 0;
	}*/
  /* for(i = 0; i<argc; i++) {
      printf("%s | %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");*/
   else if(!strcmp(data,"attendance")){
		printf("\n------------Mark 1 if present and 0 if absent------------\n");
		for(i = 0; i<argc; i++) {
			printf("%s\t", argv[i]);
		}
		char str[100];
		strcpy(str,argv[0]);
		studentIds[counter] = atoi(str);
		
		scanf("%d",&presentOrAbsent);
		presenty[counter] = presentOrAbsent;
		counter++;
		return 0;
   }
   else if(!strcmp(data,"viewAttendance")){
		for(i = 0; i<argc; i++) {
			if(i==1){
				if(*argv[i]=='1')
					strcpy(argv[i],"Present");
				else
					strcpy(argv[i],"Absent ");
			}
			printf("|%s\t",argv[i] ? argv[i] : "NULL");
		}
		printf("|\n------------------------------------------------------------------------------");
		return 0;
   }
   
}

int query(char sqlStmt[], void *data) {
	sqlite3 *db;
	char *zErrMsg = 0;
	rc = sqlite3_open("collegeams.db", &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
   }/* else {
		fprintf(stderr, "Opened database successfully\n");
   }*/
  // printf("%s",sqlStmt);
   rc = sqlite3_exec(db, sqlStmt, callback, data, &zErrMsg);
   sqlite3_close(db);
   return rc;
}

void reset() {
	rc =0;
	strcpy(sqlStmt," ");
	strcpy(data," ");
}

int doValidate(char typePass[]){
	sprintf(sqlStmt,"SELECT COUNT(*) FROM loginData WHERE userId=%d and password='%s' and type='%s';",userId,password,typePass);
	int check = query(sqlStmt,"checkLogin");
	return check;
}

void addMember(char typePass[]){
	char ask[150];
	int i,n,choiceBack;
	label4 : sprintf(ask,"How many '%s' to add ?  ",typePass);
	printf("1.Continue  \n");
	printf("2.Back\n\nChoice : ");
	scanf("%d",&choiceBack);
	if(choiceBack==1){
		printf("\n%s",ask);
		scanf("%d",&n);
		for(i=1;i<=n;i++){
			sprintf(ask,"%d.Enter user id number of '%s' : ",i,typePass);
			printf("%s",ask);
			scanf("%d",&userId);
			sprintf(ask,"   Enter password of '%s' : ",typePass);
			printf("%s",ask);
			scanf("%s",password);
			sprintf(ask,"   Enter first name of '%s' : ",typePass);
			printf("%s",ask);
			scanf("%s",firstName);
			sprintf(ask,"	Enter last name of '%s' : ",typePass);
			printf("%s",ask);
			scanf("%s",lastName);
			sprintf(ask,"	Enter class name of '%s' : ",typePass);
			printf("%s",ask);
			scanf("%s",className);
			strcpy(firstName,strcat(firstName," "));
			strcpy(fullName,strcat(firstName,lastName));
			sprintf(sqlStmt,"INSERT INTO loginData VALUES(%d,'%s','%s','%s');",userId,password,typePass,fullName);
			int checkifAdded = query(sqlStmt,"Add");
			if(checkifAdded==0){
				if(!strcmp(typePass,"teacher")){
					teacherId=userId;
					sprintf(sqlStmt,"INSERT INTO teacherData VALUES(%d,'%s');",teacherId,className);
					query(sqlStmt,"Add");
				}
				if(!strcmp(typePass,"student")){
					studentId=userId;
					sprintf(sqlStmt,"INSERT INTO studentData VALUES(%d,'%s');",studentId,className);
					query(sqlStmt,"Add");
				}
				printf("\n\nAdded Members Successfully!\n\n");
			}
			else{
				printf("\n\nAttempt of adding members unsuccessful");
				goto label4;
			}
		}
	}
	else if(choiceBack==2)
		callAdmin();
	else{
		printf("\nInvalid choice\nPlease select again\n");
		goto label4; 
	}
}


void callAdmin(){
	//printf("\nCALLED");
	int choiceAdd;
	label3 : printf("\n1.Add teachers\n2.Add students\n3.Log out\n\nChoice : ");
	scanf("%d",&choiceAdd);
	if(choiceAdd<=3 && choiceAdd>0){
		switch(choiceAdd){
			case 1 :	addMember("teacher");
						goto label3;
						break;
			case 2 :	addMember("student");
						goto label3;
						break;
			case 3 : 	exit(0);
		}
	}
	else{
		printf("\nInvalid Choice!\nPlease select again\n");
		goto label3;
	}
}


void attendance(char datePass[]){
	if(!strcmp(datePass,"today")){
		//printf("today");
		sprintf(sqlStmt,"SELECT ld.userId as userId,ld.fullName as fullName FROM loginData ld INNER JOIN studentData sd on sd.studentId=ld.userId AND sd.className = (SELECT className FROM teacherData WHERE teacherId=%d);",userId);
		query(sqlStmt,"attendance");
		int i;
		for(i=0;i<counter;i++){
			printf("%d\t",studentIds[i]);
			printf("%d\n",presenty[i]);
			//markPresenty(studentIds[i],presenty[i]);
			time_t t;
	char day[50];
	char month[50];
	char year[50];
	char combineDate[200];
    struct tm *local;
	t=time(NULL);
    local = localtime(&t);
	//printf("%d",local->tm_mday);
	//printf("%d",local->tm_mon+1);
	//printf("%d",local->tm_year+1900);
	sprintf(day, "%d", local->tm_mday);
	sprintf(month, "%d", local->tm_mon+1);
	sprintf(year, "%d", local->tm_year+1900);
	sprintf(combineDate,"%s-%s-%s",day,month,year);
	sprintf(sqlStmt,"INSERT INTO attendance(date,studentId,presenty) VALUES('%s',%d,%d);",combineDate,studentIds[i],presenty[i]);
	query(sqlStmt,"add");
		}
		/*for(i=0;i<counter;i++){
			printf("%s\n",chararray[i]);
			printf("Mark 1 if present and 0 if absent : ");
			scanf("%d",&presentOrAbsent);
			char *token= strtok(chararray[i], "-");
   
			// Keep printing tokens while one of the
				// delimiters present in str[].
			while (token != NULL)
			{
				printf("%s\n", token);
				strcpy(cpy,token);
				break;
			}
			;
			printf("\n");
		}*/
	}
	else if(!strcmp(datePass,"givenDate")){
		//printf("notoday");
	}
}
void markPresenty(int studentId, int isPresent){
	
}

void callTeacher(){
	int choiceTeacher;
	//printf("\nCALLED Teacher");
	label5 : printf("\n1.Mark Attendance For Today\n2.Log out\n\nChoice : ");
	scanf("%d",&choiceTeacher);
	switch(choiceTeacher){
		case 1 : 	attendance("today");
					goto label5;
					break;
		/*case 2 : 	attendance("givenDate");
					goto label5;
					break;*/
		case 2 : 	exit(0);
		default :	printf("\nInvalid Choice!\nPlease select again\n");
					goto label5;
	}
}

void viewAttendance(){
	sprintf(sqlStmt,"SELECT date,presenty FROM attendance WHERE studentId=%d ORDER BY date;",userId);
	printf("\n------------------------------------------------------------------------------\n");
	printf("|Date\t\t  |Presenty\t|\n------------------------------------------------------------------------------\n");
	query(sqlStmt,"viewAttendance");
}

void callStudent(){
	//printf("\nCALLED student");
	int choiceStudent;
	label6 : printf("\n\n1.View attendance\n2.Log out\n\nChoice : ");
	scanf("%d",&choiceStudent);
	switch(choiceStudent){
		case 1 :	viewAttendance();
					goto label6;
					break;
		case 2 : 	exit(0);
		default :	printf("\nInvalid Choice!\nPlease select again\n");
					goto label6;
	}
}