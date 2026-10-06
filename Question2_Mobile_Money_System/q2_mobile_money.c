#include <stdio.h>

/* throw away whatever is left on the input line (e.g. letters typed by mistake) */
void clear_input(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void)
{
    double balance = 0.0;
    double amount;
    int choice;
    int deposits = 0;
    int withdrawals = 0;

    printf("===== MOBILE MONEY TRANSACTION SYSTEM =====\n\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Check Balance\n");
    printf("4. Transaction Summary\n");
    printf("5. Exit\n");

    while (1) {
        printf("\nEnter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number from 1 to 5.\n");
            clear_input();
            continue;               /* back to the menu */
        }

        if (choice == 5) {
            printf("System terminated.\n");
            break;                  /* leaves the while loop */
        }

        switch (choice) {
        case 1:
            printf("Enter deposit amount: ");
            if (scanf("%lf", &amount) != 1) {
                printf("Invalid amount entered.\n");
                clear_input();
                continue;
            }
            if (amount <= 0) {
                printf("Transaction rejected: Amount must be greater than zero.\n");
                continue;
            }
            balance += amount;
            deposits++;
            printf("Deposit successful.\n");
            printf("Current balance: %.0f RWF\n", balance);
            break;

        case 2:
            printf("Enter withdrawal amount: ");
            if (scanf("%lf", &amount) != 1) {
                printf("Invalid amount entered.\n");
                clear_input();
                continue;
            }
            if (amount <= 0) {
                printf("Transaction rejected: Amount must be greater than zero.\n");
                continue;
            }
            if (amount > balance) {
                printf("Transaction rejected: Insufficient balance.\n");
                continue;
            }
            balance -= amount;
            withdrawals++;
            printf("Withdrawal successful.\n");
            printf("Current balance: %.0f RWF\n", balance);
            break;

        case 3:
            printf("Current balance: %.0f RWF\n", balance);
            break;

        case 4:
            printf("Successful deposits   : %d\n", deposits);
            printf("Successful withdrawals: %d\n", withdrawals);
            printf("Total transactions    : %d\n", deposits + withdrawals);
            break;

        default:
            printf("Invalid choice. Please select 1 to 5.\n");
            break;
        }
    }

    return 0;
}
