#include <ctype.h>
#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_PATIENTS 100
#define MAX_RECORDS 100

typedef struct {
    char cardID[10];//
    char name[50];
    char phone[15];
    double debt;
    int visitDays;
}Patient;

typedef struct {
    char recID[20];
    char cardId[10];
    char date[20];
    char status[20];
}Record;

Patient patients[MAX_PATIENTS];
Record records[MAX_RECORDS];
int patientCount = 0;//GLOBAL VARIABLE
int recordCount = 0;//GLOBAL VARIABLE

void printMenu() {
    printf("|------------------------------------------------------------|\n");
    printf("|                  PATIENT MANAGEMENT MENU                   |\n");
    printf("|------------------------------------------------------------|\n");
    printf("|%-60s|\n","1.Create a new patient's Information");
    printf("|%-60s|\n","2.Update a patient's Information");
    printf("|%-60s|\n","3.Discharge a patient from the hospital");
    printf("|%-60s|\n","4.Show current patients");
    printf("|%-60s|\n","5.Search for a patient in the hospital");
    printf("|%-60s|\n","6.Sort patients by debt");
    printf("|%-60s|\n","7.Create a record - medical check");
    printf("|%-60s|\n","8.View medical history");
    printf("|%-60s|\n","9.Exit");
    printf("|------------------------------------------------------------|\n");
    printf("|               PLEASE ENTER YOUR CHOICE ~^^~                |\n");
    printf("|------------------------------------------------------------|\n");
}

void removeNewline(char *str) {
    int len = strlen(str);
    if (str[len - 1] == '\n' && len > 0) {
        str[len - 1] = '\0';
    }
}
int isStringEqualIgnoreCase(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        // Chuyển cả 2 về chữ thường rồi so sánh
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 0; // Khác nhau
        }
        s1++;
        s2++;
    }
    // Kiểm tra xem cả 2 chuỗi có kết thúc cùng lúc không (độ dài bằng nhau)
    return *s1 == *s2;
}
int findIndexPatient(char *id) {//
    for ( int i = 0; i < patientCount; i++ ) {
        if (isStringEqualIgnoreCase(patients[i].cardID, id) == 0) {
            return i;
        }
    }
    return -1;
}
int isNumeric(const char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}
int isValidDate(char *date) {
    if (strlen(date) != 10) {
        return 0;
    }
    if (date[2] != '/'|| date[5] != '/') {
        return 0;
    }
    char dayStr[3], monthStr[3], yearStr[5];
    strncpy(dayStr, date, 2);
    dayStr[2] = '\0';
    strncpy(monthStr, date + 3, 2);
    monthStr[2] = '\0';
    strncpy(yearStr, date + 6, 4);
    yearStr[4] = '\0';
    if (!isNumeric(dayStr) || !isNumeric(monthStr) || !isNumeric(yearStr)) {
        return 0;
    }
    int d = atoi(dayStr);
    int m = atoi(monthStr);
    int y = atoi(yearStr);
    if (y < 1900 || y > 2100) {
        return 0;
    }
    if (m < 1 || m > 12) {
        return 0;
    }
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {// Ktra nam nhuan
        daysInMonth[1] = 29;
    }

    if (d < 1 || d > daysInMonth[m]) {
        return 0;
    }
    return 1; // Hop le
}

// #1
void addPatient() {
    if ( patientCount >= MAX_PATIENTS || recordCount >= MAX_RECORDS ) {
        printf("Error: Memory full (Patients or Records).\n");
        return;
    }
    Patient p;
    printf("\n--- ADD NEW PATIENT ---\n");
    //Input card ID
    while (1) {
        printf("Enter card ID ( cannot be empty):");
        fflush(stdin);
        fgets(p.cardID,sizeof(p.cardID),stdin);
        removeNewline(p.cardID);
        if (strlen(p.cardID) == 0) { // Empty
            printf("Error! Patient card ID cannot be empty.\n");
            continue;
        }
        if (p.cardID[0] == ' ') {// Start with space
            printf("Error: Card ID cannot start with a space.\n");
            continue;
        }
        if (findIndexPatient(p.cardID) != -1) {//Duplicate
            printf("Error! Card ID '%s' already exists.\n ",p.cardID);
        }else {
            break;
        }
    }
    //Input Name
    while (1) {
        printf("Enter name ( cannot be empty):");
        fflush(stdin);
        fgets(p.name,sizeof(p.name),stdin);
        removeNewline(p.name);
        if (strlen(p.name) == 0) {
            printf("Error! Name cannot be empty.\n");
            continue;
        }
        if (p.name[0] == ' ') {
            printf("Error: Name cannot start with a space.\n");
            continue;
        }
        break;
    }
    //Input phone number
    while (1) {
        printf("Enter phone number (min 9 digits,max 12 digits, number only):");
        fflush(stdin);
        fgets(p.phone,sizeof(p.phone),stdin);
        removeNewline(p.phone);
        if (strlen(p.phone) == 0) {
            printf("Error! Phone number cannot be empty.\n");
            continue;
        }
        if (p.phone[0] == ' ') {
            printf("Error! Phone number cannot start with a space.\n");
            continue;
        }
        int isNumberValid = 1;
        for ( int i = 0 ; i < strlen(p.phone) ; i++ ) {
            if (!isdigit((unsigned char)p.phone[i])) {
                isNumberValid = 0;
                break;
            }
        }
        if (!isNumberValid) {
            printf("Error!Phone number can only contain number digits.\n");
            continue;
        }
        if (strlen(p.phone) >= 9 && strlen(p.phone) <= 12 ) {//TRUE
            break;
        }
        printf("Error!Invalid phone number ( must be minimum 9 digits, maximum 12 digits and no symbols).\n");
    }
    //Input Debt
    char debtBuffer[50];
    while (1) {
        printf("Enter Initial Debt (> 0, no spaces): ");
        fflush(stdin);
        if (fgets(debtBuffer, sizeof(debtBuffer), stdin) == NULL) {
            continue;
        }
        int len = strlen(debtBuffer);
        if (len > 0 && debtBuffer[len - 1] == '\n') {
            debtBuffer[len - 1] = '\0';
            len--;
        }

        if (len == 0) {
            printf(">> Error: Input cannot be empty.\n");
            continue;
        }
        int hasSpace = 0;
        for (int i = 0; i < len; i++) {
            if (isspace((unsigned char)debtBuffer[i])) {
                hasSpace = 1;
                break;
            }
        }
        if (hasSpace) {
            printf(">> Error: Input cannot contain spaces.\n");
            continue;
        }

        char *endPtr;
        double val = strtod(debtBuffer, &endPtr);

        if (*endPtr != '\0') {
            printf(">> Error: Invalid number format (contain letters/symbols).\n");
            continue;
        }

        if (val <= 0) {
            printf(">> Error: Debt must be strictly positive (> 0).\n");
            continue;
        }
        // Nếu vượt qua tất cả cửa ải trên -> Hợp lệ
        p.debt = val;
        break;
    }
        p.visitDays = 0;
        patients[patientCount] = p;
        patientCount++;
        printf("Success!!! New patient added into patient list.\n");
    }

// #2
void updatePatient() {
    char id[20];
    printf("\n---UPDATE PATIENT INFO ---\n");
    printf("Enter Card ID to update: ");
    fflush(stdin);
    fgets(id,sizeof(id),stdin);
    removeNewline(id);
    int index = findIndexPatient(id);
    if (index == -1) {
        printf("Error! Patient not found!\n");
        return;
    }
    char lastStatus[20] = "";
    int hasRecord = 0;
    // Scan records to find the latest status for this patient
    for ( int i = 0 ; i < recordCount; i ++) {
        if (strcmp(records[i].cardId,id) == 0) {
            strcpy(lastStatus,records[i].status);
            hasRecord = 1;
        }
    }
    if (hasRecord == 1 && strcmp(lastStatus,"Discharged") == 0 ) {
        printf("Error: Cannot update. Patient has been already Discharged (Status: Discharged).\n");
        return;
    }
    char newPhoneNumber[15];
    printf("Current Card ID: %s\n",patients[index].cardID);
    printf("Current Patient's Name: %s\n",patients[index].name);
    printf("Current phone number: %s\n ",patients[index].phone);
    printf("------------------------------------\n");
    printf("(Enter '0' to Cancel update)\n");
    while (1) {
        printf("Enter new phone number (min 9 digits, max 12 digits, number only): ");
        fflush(stdin);
        fgets(newPhoneNumber,sizeof(newPhoneNumber),stdin);
        removeNewline(newPhoneNumber);

        if (strlen(newPhoneNumber) == 0) {
            printf("Error! Phone number cannot be empty.\n");
            continue;
        }
        // Check CANCEL
        if (strcmp(newPhoneNumber, "0") == 0) {
            printf(">> Update cancelled by user.\n");
            return;
        }
        if (newPhoneNumber[0] == ' ') {
            printf("Error! Phone number cannot start with a space.\n");
            continue;
        }

        int isNumberValid = 1;
        for ( int i = 0 ; i < strlen(newPhoneNumber) ; i++ ) {
            if (!isdigit((unsigned char)newPhoneNumber[i])) {
                isNumberValid = 0;
                break;
            }
        }
        if (!isNumberValid) {
            printf("Error!Phone number can only contain number digits.\n");
            continue;
        }
        if (strlen(newPhoneNumber) >= 9 && strlen(newPhoneNumber) <=12) {//TRUE
            strcpy(patients[index].phone,newPhoneNumber);
            printf("Success!!! Update sucessful.\n");
            break;
        }
        printf("Error!Invalid phone number ( must be minimum 9 digits, maximum 12 digits and only number ).\n");
    }
}
// #3
void dischargePatient(){
    char id[20];
    printf("\n---DISCHARGE PATIENT ---\n");
    printf("Enter Card ID to discharge: ");
    fflush(stdin);
    fgets(id,sizeof(id),stdin);
    removeNewline(id);
    int index = findIndexPatient(id);
    if (index == -1) {
        printf("Error!Patient not found!\n");
        return;
    }
    char lastStatus[20] = "";
    int hasRecord = 0;
    for ( int i = 0 ; i < recordCount; i ++) {
        if (strcmp(records[i].cardId,id) == 0) {
            strcpy(lastStatus,records[i].status);
            hasRecord = 1;
        }
    }
    if (hasRecord && strcmp(lastStatus,"Discharged") == 0) {
        printf("Error:Patient already discharged previously.\n");
        return;
    }

    //Debt warning
    if (patients[index].debt > 0) {
        printf("WARNING: Patient has an outstanding debt of %.2f.\n", patients[index].debt);
        printf("Are you sure you want to delete this profile?\n ");
        while (1){
            printf("Your choice (y/n):");
            char confirm;
            scanf("%c",&confirm);
            fflush((stdin));
            if (confirm == 'n' || confirm == 'N') {
                printf("Cancelled.\n");
                break;
            }else if (confirm == 'y' || confirm == 'Y') {
                // if (recordCount >= MAX_RECORDS) {
                //     printf("Error: Record memory full. Cannot save discharge history.\n");
                //     return;
                // }
                // char date[20];
                // while (1) {
                //     printf("Enter Discharge Date (dd/mm/yyyy): ");
                //     fgets(date, sizeof(date), stdin);
                //     removeNewline(date);
                //     if (isValidDate(date)) {
                //         break;
                //     }
                //     printf("Error! Invalid date format\n");
                // }
                // Record r;
                // sprintf(r.recID,"%d",recordCount + 1000);//tạo ID
                // strcpy(r.cardId,id);
                // strcpy(r.date,date);
                // strcpy(r.status,"Discharged");
                // records[recordCount] = r;
                // recordCount ++;
                for (int i = index; i <patientCount; i++) {
                    patients[i] = patients[i + 1];
                }
                patientCount--;
                printf("Success!!! Discharged successfully.\n");
                break;
            }
            printf("Error! Invalid choice.(Y/y for Yes, n/N for No)\n");
        }
    }
}

//#4
void listPatients(){
    printf("\n--- LIST OF PATIENTS ---\n");
    if (patientCount == 0) {
        printf("No patients in the list.\n");
        return;
    }
    int currentPage = 1;
    int itemPerPage = 5;
    int totalPages;
    char choice;
    while (1) {
        totalPages = (patientCount + itemPerPage - 1) / itemPerPage;
        if (currentPage < 1) {
            currentPage = 1;
        }
        if (currentPage > totalPages) {
            currentPage = totalPages;
        }
        int start = (currentPage - 1) * itemPerPage;
        int end = start + itemPerPage;
        if ( end > patientCount ) {
            end = patientCount;
        }
        printf("\n--- LIST OF PATIENTS (Page %d/%d) ---\n", currentPage, totalPages);
        printf("-------------------------------------------------------------------------------\n");
        printf("%-10s %-25s %-15s %-15s %-10s\n", "ID", "Name", "Phone", "Debt", "Visits");
        printf("-------------------------------------------------------------------------------\n");
        for (int i = start; i < end; i++) {
            printf("%-10s %-25s %-15s %-15.0f %-10d\n",
                   patients[i].cardID,
                   patients[i].name,
                   patients[i].phone,
                   patients[i].debt,
                   patients[i].visitDays);
        }
        printf("-------------------------------------------------------------------------------\n");
        printf("[Page %d of %d] | Total Patients: %d\n", currentPage, totalPages, patientCount);
        printf("Controls: [n/N] Next Page | [p/P] Previous Page | [e/E] Exit to Menu\n");
        printf("Your choice: ");
        scanf("%c",&choice);
        fflush(stdin);
        if (choice == 'n' || choice == 'N') {
            if ( currentPage < totalPages ) {
                currentPage++;
            }else {
                printf(">> ALERT: You are already on the last page.\n");
            }
        }
        else if ( choice == 'p' || choice == 'P' ) {
            if ( currentPage > 1 ) {
                currentPage--;
            }else {
                printf(">> ALERT: You are already on the first page.\n");
            }
        }
        else if ( choice == 'e' || choice == 'E' ) {
            printf("Exiting patient list view...\n");
            break;// break out loop --- > return Menu
        }
        else {
            printf(">> Invalid choice. Please enter n, p, or e.\n");
        }
    }
}

//#5
void searchPatient() {
    char keyword[50];
    printf("\n--- SEARCH PATIENT ---\n");
    printf("Enter KEYWORD for Name to search: ");
    fflush(stdin);
    fgets(keyword, sizeof(keyword), stdin);
    removeNewline(keyword);

    if (strlen(keyword) == 0) {
        printf("Error: Empty keyword.\n");
        return;
    }
    int found = 0;
    printf("\nSearch Results:\n");
    printf("%-10s %-25s %-15s %-15s\n", "ID", "Name", "Phone", "Debt");
    for (int i = 0; i < patientCount; i++) {
        if (strstr(patients[i].name, keyword) != NULL) {
            printf("%-10s %-25s %-15s %-15.0f\n",
               patients[i].cardID,
               patients[i].name,
               patients[i].phone,
               patients[i].debt);
            found = 1;
        }
    }
    if (!found) {
        printf("No patients found.\n");
    }
}

//#6
void sortPatients() {
    if (patientCount == 0) {
        printf("List is empty. Nothing to sort.\n");
        return;
    }
    int choice = 0;
    char buffer[50];
    printf("\n--- SORT BY DEBT ---\n");
    printf("1. Ascending\n");
    printf("2. Descending\n");
    while (1) {
        printf("Your choice(1 or 2):");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        removeNewline(buffer);
        int len = strlen(buffer);
        if ( len == 0) {
            printf(">> Error: Input cannot be empty.\n");
            continue;
        }
        int isNumber = 1;
        for ( int i = 0; i < len; i++) {
            if (!isdigit((unsigned char)buffer[i])) {
                isNumber = 0;
                break;
            }
        }
        if (!isNumber) {
            printf(">> Error: Input must be number 1 or 2 only.\n");
            continue;
        }
        if (sscanf(buffer,"%d", &choice) == 1) {
            if (choice == 1|| choice == 2) {
                break;//VALID!!!!
            }else {
                printf(">> Error: Please enter number 1 or 2 only.\n");
            }
        }else {
            printf(">> Error: Invalid input.\n");
        }
    }
    if (choice == 1) {
        for ( int i = 0; i < patientCount - 1; i++ ) {
            for ( int j = 0; j < patientCount - 1 - i; j++ ) {
                if (patients[j].debt > patients[j + 1].debt) {
                    Patient temp = patients[j];
                    patients[j] = patients[j + 1];
                    patients[j + 1] = temp;
                }
            }
        }
        printf("Patient list has been Sorted ASCENDING by DEBT.\n");
        listPatients();
    }
    if (choice == 2) {
        for ( int i = 0; i < patientCount - 1; i++ ) {
            for ( int j = 0; j < patientCount - 1 - i; j++ ) {
                if (patients[j].debt < patients[j + 1].debt) {
                    Patient temp = patients[j];
                    patients[j] = patients[j + 1];
                    patients[j + 1] = temp;
                }
            }
        }
        printf("Patient list has been Sorted DESCENDING by DEBT.\n");
        listPatients();
    }
}
int main() {
int choice = 0;
char buffer[100];
while (1){
    printMenu();
    while (1) {
    printf("Your choice: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        break;
    }
    if (strcmp(buffer,"\n") == 0) {
        printf("Input cannot be empty. Please enter a number (1-9).\n");
        continue;
    }
    if (sscanf(buffer,"%d",&choice) == 1) {
        if (choice >= 1 && choice <= 9) {
            break;//VALID
        } else {
            printf("Number must be between 1 and 9.\n");
        }
        // Truong hop nhat ky tu dac biet hoac chu cai
    } else {
        printf("Invalid input! Please enter a digit.\n");
    }
}
        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                updatePatient();
                break;
            case 3:
                dischargePatient();
                break;
            case 4:
                listPatients();
                break;
            case 5:
                searchPatient();
                break;
            case 6:
                sortPatients();
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                printf("Exitting program...Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}