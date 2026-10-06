/* Mobile Money transaction system for an agent  */
#include <stdio.h>

/* Discards invalid characters left on the input line. */
void clear_input(void)
{
    int c;

    /* Remove the rest of the invalid line. */
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* Reads and validates a positive transaction amount. */
int get_amount(const char *prompt, double *amount)
{
    printf("%s", prompt);

    if (scanf("%lf", amount) != 1) {
        clear_input();
        printf("Invalid amount entered.\n");
        return 0;
    }

    if (*amount <= 0) {
        printf("Transaction rejected: amount must be greater than zero.\n");
        return 0;
    }

    return 1;
}

/* Prints the available transaction options once at startup. */
void show_menu(void)
{
    printf("\n\n--------------------------------------------\n");
    printf("       MOBILE MONEY TRANSACTION SYSTEM\n");
    printf("--------------------------------------------\n");
    printf("  1) Deposit              2) Withdraw\n");
    printf("  3) Check balance        4) Summary\n");
    printf("  5) Exit\n");
    printf("--------------------------------------------\n");
}

int main(void)
{
    double balance = 0, amount;

    /* Track only successful transactions. */
    int choice, deposits = 0, withdrawals = 0;

    printf("\nWelcome to Mobile Money Services.\n");
    show_menu();

    while (1) {
        /* Reject letters and other non-numeric choices. */
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number from 1 to 5.\n");
            clear_input();
            continue;
        }

        /* Exit before entering the transaction switch. */
        if (choice == 5) {
            printf("System terminated.\n");
            break;
        }

        /* Process the selected service. */
        switch (choice) {
        case 1:
            if (!get_amount("Enter deposit amount: ", &amount))
                continue;

            balance += amount;
            deposits++;
            printf("Deposit successful.\n");
            printf("Current balance: %.0f RWF\n", balance);
            break;

        case 2:
            if (!get_amount("Enter withdrawal amount: ", &amount))
                continue;

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
            break;

        default:
            printf("Invalid choice. Please select 1 to 5.\n");
            break;
        }
    }

    return 0;
}
