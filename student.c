#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"

char currentUser[50];
char currentRole[20];

int login() {
    char u[50], p[50], r[20], dob[20], mob[20];
    char inUser[50], inPass[50], inDOB[20], inMob[20];
    int attempts = 3; // username attempts

RETRY_LOGIN:

    printf("USERNAME: ");
    scanf("%s", inUser);

    printf("PASSWORD: ");

    int i = 0;
    char ch;
    while ((ch = getch()) != 13) {
        if (ch == 8 && i > 0) {
            printf("\b \b");
            i--;
        } else if (ch != 8) {
            inPass[i++] = ch;
            printf("*");
        }
    }
    inPass[i] = '\0';
    printf("\n");

    FILE *fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf("Credential file missing!\n");
        return 0;
    }

    int userFound = 0;

    while (fscanf(fp, "%s %s %s %s %s", u, p, r, dob, mob) == 5) {
        if (strcmp(inUser, u) == 0) {
            userFound = 1;

            if (strcmp(inPass, p) == 0) {
                strcpy(currentUser, u);
                strcpy(currentRole, r);
                fclose(fp);
                return 1;
            } else {
                printf("\nWrong password!\n");
                printf("Enter your Mobile Number: ");
                scanf("%s", inMob);

                if (strcmp(inMob, mob) == 0) {
                    printf("\nMobile matched! Logging in directly.\n");
                    strcpy(currentUser, u);
                    strcpy(currentRole, r);
                    fclose(fp);
                    return 1;
                }

                printf("Mobile number incorrect!\n");
                printf("Enter your Date of Birth (dd-mm-yyyy): ");
                scanf("%s", inDOB);

                if (strcmp(inDOB, dob) == 0) {
                    printf("\nDOB matched! Logging in.\n");
                    strcpy(currentUser, u);
                    strcpy(currentRole, r);
                    fclose(fp);
                    return 1;
                }

                printf("DOB also incorrect! Access Denied.\n");
                fclose(fp);
                return 0;
            }
        }
    }

    fclose(fp);

    // Username not found
    attempts--;
    if (attempts == 0) {
        printf("\nNo attempts left! Logging out.\n");
        return 0;
    }

    printf("\nINVALID USERNAME!\nYou have %d attempts left.\n", attempts);
    goto RETRY_LOGIN;
}

void addStudent() {
    int roll;
    char name[50];
    float mark;

    printf("Roll Number: ");
    scanf("%d", &roll);
    printf("Name: ");
    scanf(" %[^\n]", name);
    printf("Mark: ");
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);

    printf("Student Details added!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf("Roll Number\tName\tMarks\n");
    printf("----\t----\t----\n");
    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        printf("%d\t%s\t%.2f\n", roll, name, mark);
    }

    fclose(fp);
}

void searchStudent() {
    int find, roll;
    char name[50];
    float mark;

    printf("Enter roll number to search: ");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == find) {
            printf("Found: %d %s %.2f\n", roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Student Details not found!\n");
}

void deleteStudent() {
    int delRoll;
    printf("Enter roll to delete: ");
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student deleted!\n");
    else printf("Roll Number not found!\n");
}

void updateStudent() {
    int updateRoll;
    printf("Enter roll number to update: ");
    scanf("%d", &updateRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            char newName[50];
            float newMark;

            printf("New Name: ");
            scanf(" %[^\n]", newName);
            printf("New Mark: ");
            scanf("%f", &newMark);

            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student Details updated!\n");
    else printf("Roll Numbernot found!\n");
}

void adminMenu() {
    int c;
    while (1) {
        printf("\nADMIN MENU\n");
        printf("1.Add Student Details\n2.Display Student Details\n3.Search Student Details\n4.Update Student Details\n5.Delete Student Details\n6.Logout\n");
        scanf("%d",&c);
        if(c==1)addStudent();
        else if(c==2)displayStudents();
        else if(c==3)searchStudent();
        else if(c==4)updateStudent();
        else if(c==5)deleteStudent();
        else return;
    }
}

void staffMenu() {
    int c;
    while (1) {
        printf("\nSTAFF MENU\n");
        printf("1.Add Student Details\n2.Display Student Details\n3.Search Student Details\n4.Update Student Details\n5.Delete Student Details\n6.Logout\n");
        scanf("%d",&c);
        if(c==1)addStudent();
        else if(c==2)displayStudents();
        else if(c==3)searchStudent();
        else if(c==4)updateStudent();
        else return;
    }
}

void guestMenu() {
    int c;
    while (1) {
        printf("\nGUEST MENU\n");
        printf("1.Display\n2.Search\n3.Logout\n");
        scanf("%d",&c);
        if(c==1)displayStudents();
        else if(c==2)searchStudent();
        else return;
    }
}

int main() {
    if (!login()) {
        printf("Invalid login!\n");
        return 0;
    }

    printf("Logged in as: %s\n", currentRole);

    if (strcmp(currentRole,"admin")==0) adminMenu();
    else if (strcmp(currentRole,"staff")==0) staffMenu();
    else guestMenu();

    return 0;
}