#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}
int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
 char *sql;
   rc = sqlite3_open("collegeams.db", &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
 /* sql = "CREATE TABLE loginData("  \
         "userId int UNIQUE NOT NULL PRIMARY KEY," \
         "password VARCHAR (255) NOT NULL COLLATE BINARY," \
         "type  VARCHAR (10)  NOT NULL," \
         "fullName VARCHAR (30)  NOT NULL);";
        

  /* Execute SQL statement 
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
   
   sql="CREATE TABLE studentData ("\
		"studentId int UNIQUE 	NOT NULL    PRIMARY KEY,"\
        "className VARCHAR (10) NOT NULL);";
	 rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
   
	sql="CREATE TABLE teacherData ("\
    "teacherId int PRIMARY KEY NOT NULL UNIQUE,"\
    "className VARCHAR (10) NOT NULL);";	
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }*/
	 /* sql="INSERT INTO loginData (userId,password,type,fullName) VALUES(1,'admin1','admin','first admin')";
	  	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);*/
  /*sql="CREATE TABLE attendance ("\
   "id        INTEGER      PRIMARY KEY AUTOINCREMENT,"\
    "date      DATE         NOT NULL,"\
    "studentId INTEGER      NOT NULL,"\
    "presenty  BOOLEAN      NOT NULL);";*/
	//sql="DROP TABLE attendance;";
	sql="INSERT INTO attendance(date,studentId,presenty) VALUES('15-4-2018',3000,1);INSERT INTO attendance(date,studentId,presenty) VALUES('14-4-2018',3000,1);INSERT INTO attendance(date,studentId,presenty) VALUES('13-4-2018',3000,0);";
	
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table attendance created successfully\n");
   }
      sqlite3_close(db);
}