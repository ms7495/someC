#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 10
#define MAX_TRANSACTIONS 100

// Structure for account
typedef struct {
    int id;
    char username[20];
    char password[20];
    double balance;
} Account;

// Structure for transaction
typedef struct {
    int id;
    int account_id;
    char type[20];
    double amount;
    time_t timestamp;
} Transaction;

Account accounts[MAX_ACCOUNTS];
Transaction transactions[MAX_TRANSACTIONS];

int num_accounts = 0;
int num_transactions = 0;

// Function to create dummy accounts
void createDummyAccounts() {
    for (int i = 0; i < MAX_ACCOUNTS; i++) {
        sprintf(accounts[i].username, "user%d", i + 1);
        sprintf(accounts[i].password, "password%d", i + 1);
        accounts[i].balance = (rand() % 10000) + 1000; // Random balance between 1000 and 11000
        accounts[i].id = i;
        num_accounts++;
    }
}

// Function to generate dummy transactions for an account
void generateDummyTransactions(int account_id) {
    for (int i = 0; i < 10; i++) {
        strcpy(transactions[num_transactions].type, (rand() % 2 == 0) ? "Deposit" : "Withdrawal");
        transactions[num_transactions].account_id = account_id;
        transactions[num_transactions].amount = (rand() % 1000) + 100; // Random amount between 100 and 1100
        time(&transactions[num_transactions].timestamp);
        num_transactions++;
    }
}

// Function to write accounts to a file
void writeAccountsToFile() {
    FILE *file = fopen("accounts.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_accounts; i++) {
        fprintf(file, "%d %s %s %.2f\n", accounts[i].id, accounts[i].username, accounts[i].password,
                accounts[i].balance);
    }
    fclose(file);
}

// Function to write transactions to a file
void writeTransactionsToFile() {
    FILE *file = fopen("transactions.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

int main() {
    srand(time(NULL)); // Seed for random number generation
    createDummyAccounts();
    writeAccountsToFile();
    for (int i = 0; i < num_accounts; i++) {
        generateDummyTransactions(i);
    }
    writeTransactionsToFile();
    printf("Files generated successfully\n");
    return 0;
}
