#include <stdio.h>
#include <stdlib.h>

struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

int main() {
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 5) {
        switch (choice) {
            case 1: textFile(cfPtr); break;
            case 2: updateRecord(cfPtr); break;
            case 3: newRecord(cfPtr); break;
            case 4: deleteRecord(cfPtr); break;
            default: printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        printf("Cannot create text file.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr)) {
        if (client.acctNum != 0) {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
}

void updateRecord(FILE *fPtr) {
    struct clientData client;
    unsigned int account;
    double transaction;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);
    printf("Enter amount (+ deposit / - withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated Balance: %.2f\n", client.balance);
}

void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter new account number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter last name, first name, balance:\n");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
}

void deleteRecord(FILE *fPtr) {
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number to delete (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

unsigned int enterChoice(void) {
    unsigned int choice;

    printf("\n1. Save to text file\n");
    printf("2. Update account\n");
    printf("3. Add account\n");
    printf("4. Delete account\n");
    printf("5. Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}
