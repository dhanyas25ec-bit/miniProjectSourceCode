#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
    int pin;
};

unsigned int enterChoice(void);
void createAccount(FILE *fPtr);
void deposit(FILE *fPtr);
void transfer(FILE *fPtr);
int verifyPIN(struct clientData client);

int main()
{
    FILE *cfPtr;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+"); // create file if not exists

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            exit(1);
        }
    }

    unsigned int choice;

    while ((choice = enterChoice()) != 4)
    {
        switch (choice)
        {
        case 1:
            createAccount(cfPtr);
            break;

        case 2:
            deposit(cfPtr);
            break;

        case 3:
            transfer(cfPtr);
            break;

        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Create Account\n");
    printf("2 - Deposit Money\n");
    printf("3 - Transfer Money\n");
    printf("4 - Exit\n");

    printf("Enter choice: ");
    scanf("%u", &choice);

    return choice;
}

int verifyPIN(struct clientData client)
{
    int enteredPin;

    printf("Enter PIN: ");
    scanf("%d", &enteredPin);

    if (enteredPin == client.pin)
        return 1;
    else
    {
        printf("Incorrect PIN!\n");
        return 0;
    }
}

void createAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0, 0};

    unsigned int acc;

    printf("Enter account number (1-100): ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter Last Name, First Name, Balance, PIN: ");

    scanf("%14s %9s %lf %d",
          client.lastName,
          client.firstName,
          &client.balance,
          &client.pin);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully!\n");
}

void deposit(FILE *fPtr)
{
    struct clientData client = {0};

    unsigned int acc;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    if (!verifyPIN(client))
        return;

    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Deposit successful! New balance: %.2f\n", client.balance);
}

void transfer(FILE *fPtr)
{
    struct clientData sender = {0}, receiver = {0};

    unsigned int fromAcc, toAcc;
    double amount;

    printf("Enter sender account number: ");
    scanf("%u", &fromAcc);

    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&sender, sizeof(struct clientData), 1, fPtr);

    if (sender.acctNum == 0)
    {
        printf("Sender account not found!\n");
        return;
    }

    if (!verifyPIN(sender))
        return;

    printf("Enter receiver account number: ");
    scanf("%u", &toAcc);

    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&receiver, sizeof(struct clientData), 1, fPtr);

    if (receiver.acctNum == 0)
    {
        printf("Receiver account not found!\n");
        return;
    }

    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    if (amount > sender.balance)
    {
        printf("Insufficient balance!\n");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    fseek(fPtr, (fromAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&sender, sizeof(struct clientData), 1, fPtr);

    fseek(fPtr, (toAcc - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&receiver, sizeof(struct clientData), 1, fPtr);

    printf("Transfer successful!\n");
}
