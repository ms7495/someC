#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 1000
#define LOCATION_ACC "C:\\Users\\milan\\CLionProjects\\someC\\cmake-build-debug\\accounts.txt"
#define LOCATION_TRANS "C:\\Users\\milan\\CLionProjects\\someC\\cmake-build-debug\\transactions.txt"

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

// Function prototypes
void loadAccountsFromFile();

void saveAccountsToFile();

void loadTransactionsFromFile();

void saveTransactionsToFile();

int authenticateUser(char *username, char *password);

void firstLoop(int account);

int main() {
    loadAccountsFromFile();
    loadTransactionsFromFile();

    char username[20], password[20];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    int account_id = authenticateUser(username, password);
    if (account_id != -1) {
        printf("Authentication successful\n");
        firstLoop(account_id);
    } else {
        printf("Authentication failed\n");
    }

    return 0;
}

// Function to load accounts from file
void loadAccountsFromFile() {
    FILE *file = fopen(LOCATION_ACC, "r");
    if (file == NULL) {
        printf("Error opening accounts file\n");
        exit(EXIT_FAILURE);
    }
    while (fscanf(file, "%d %s %s %lf", &accounts[num_accounts].id, accounts[num_accounts].username, accounts[num_accounts].password, &accounts[num_accounts].balance) == 4) {
        num_accounts++;
    }
    fclose(file);
}

// Function to save accounts to file
void saveAccountsToFile() {
    FILE *file = fopen(LOCATION_ACC, "w");
    if (file == NULL) {
        printf("Error opening accounts file\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_accounts; i++) {
        fprintf(file, "%d %s %s %.2f\n", accounts[i].id, accounts[i].username, accounts[i].password, accounts[i].balance);
    }
    fclose(file);
}

// Function to load transactions from file
void loadTransactionsFromFile() {
    FILE *file = fopen(LOCATION_TRANS, "r");
    if (file == NULL) {
        printf("Error opening transactions file\n");
        exit(EXIT_FAILURE);
    }
    while (fscanf(file, "%d %d %s %lf %ld", &transactions[num_transactions].id, &transactions[num_transactions].account_id, transactions[num_transactions].type, &transactions[num_transactions].amount, &transactions[num_transactions].timestamp) == 5) {
        num_transactions++;
    }
    fclose(file);
}

// Function to save transactions to file
void saveTransactionsToFile() {
    FILE *file = fopen(LOCATION_TRANS, "w");
    if (file == NULL) {
        printf("Error opening transactions file\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_transactions; i++) {
        fprintf(file, "%d %d %s %.2f %ld\n", transactions[i].id, transactions[i].account_id, transactions[i].type, transactions[i].amount, transactions[i].timestamp);
    }
    fclose(file);
}

// Function to authenticate user
int authenticateUser(char *username, char *password) {
    for (int i = 0; i < num_accounts; i++) {
        if (strcmp(accounts[i].username, username) == 0 && strcmp(accounts[i].password, password) == 0) {
            return accounts[i].id;
        }
    }
    return -1;
}

void depositMoney(int account_id, double amount) {
    accounts[account_id].balance += amount;
    transactions[num_transactions].id = num_transactions;
    transactions[num_transactions].account_id = account_id;
    strcpy(transactions[num_transactions].type, "Deposit");
    transactions[num_transactions].amount = amount;
    time(&transactions[num_transactions].timestamp);
    num_transactions++;
    saveAccountsToFile();
    saveTransactionsToFile();
}

void withdrawMoney(int account_id, double amount) {
    if (accounts[account_id].balance >= amount) {
        accounts[account_id].balance -= amount;
        transactions[num_transactions].id = num_transactions;
        transactions[num_transactions].account_id = account_id;
        strcpy(transactions[num_transactions].type, "Withdrawal");
        transactions[num_transactions].amount = amount;
        time(&transactions[num_transactions].timestamp);
        num_transactions++;
        saveAccountsToFile();
        saveTransactionsToFile();
    } else {
        printf("Insufficient balance\n");
    }
}

void transferMoney(int sender_id, int receiver_id, double amount) {
    if (accounts[sender_id].balance >= amount) {
        accounts[sender_id].balance -= amount;
        accounts[receiver_id].balance += amount;
        transactions[num_transactions].id = num_transactions;
        transactions[num_transactions].account_id = sender_id;
        strcpy(transactions[num_transactions].type, "Transfer (Outgoing)");
        transactions[num_transactions].amount = amount;
        time(&transactions[num_transactions].timestamp);
        num_transactions++;
        transactions[num_transactions].id = num_transactions;
        transactions[num_transactions].account_id = receiver_id;
        strcpy(transactions[num_transactions].type, "Transfer (Incoming)");
        transactions[num_transactions].amount = amount;
        time(&transactions[num_transactions].timestamp);
        num_transactions++;
        saveAccountsToFile();
        saveTransactionsToFile();
    } else {
        printf("Insufficient balance\n");
    }
}

void displayAccountDetails(int account_id) {
    printf("Username: %s\n", accounts[account_id].username);
    printf("Balance: %.2f\n", accounts[account_id].balance);
}

void displayTransactionHistory(int account_id) {
    printf("Transaction History:\n");
    for (int i = 0; i < num_transactions; i++) {
        if (transactions[i].account_id == account_id) {
            printf("Type: %s, Amount: %.2f, Timestamp: %s", transactions[i].type, transactions[i].amount, ctime(&transactions[i].timestamp));
        }
    }
}

void firstLoop(int account) {
    int choice;
    do {
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Transfer\n");
        printf("4. Account Details\n");
        printf("5. Transaction History\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Enter amount to deposit: ");
                double amount_deposit;
                scanf("%lf", &amount_deposit);
                depositMoney(account, amount_deposit);
                printf("Deposit successful\n");
                break;
            case 2:
                printf("Enter amount to withdraw: ");
                double amount_withdraw;
                scanf("%lf", &amount_withdraw);
                withdrawMoney(account, amount_withdraw);
                printf("Withdrawal successful\n");
                break;
            case 3:
                printf("Enter recipient's account ID: ");
                int receiver_id;
                scanf("%d", &receiver_id);
                printf("Enter amount to transfer: ");
                double amount_transfer;
                scanf("%lf", &amount_transfer);
                transferMoney(account, receiver_id, amount_transfer);
                printf("Transfer successful\n");
                break;
            case 4:
                displayAccountDetails(account);
                break;
            case 5:
                displayTransactionHistory(account);
                break;
            case 6:
                return;
            default:
                printf("Invalid choice\n");
        }
    } while (1);
}
