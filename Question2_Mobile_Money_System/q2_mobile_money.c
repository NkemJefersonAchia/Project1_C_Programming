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

/* Prints the menu at the start of every transaction. */
void show_menu(void)
{
    printf("\n\n--------------------------------------------\n");
    printf("       MOBILE MONEY TRANSACTION SYSTEM\n");
    printf("--------------------------------------------\n");
    printf("  1) Deposit              2) Withdraw\n");
    printf("  3) Check balance        4) Summary\n");
    printf("  5) Exit\n");
    printf("--------------------------------------------\n");
    printf(" Select an option [1-5]: ");
}

int main(void)
{
    double balance = 0, amount;

    /* Track only successful transactions. */
    int choice, deposits = 0, withdrawals = 0;

    printf("\nWelcome to Mobile Money Services.\n");

    while (1) {
        show_menu();

        /* Reject letters and other non-numeric choices. */
        if (scanf("%d", &choice) != 1) {
            printf("\n[!] Invalid input. Enter a number from 1 to 5.\n");
            clear_input();
            continue;
        }

        /* Exit before entering the transaction switch. */
        if (choice == 5) {
            printf("\nThank you for using Mobile Money Services.\n");
            break;
        }

        /* Process the selected service. */
        switch (choice) {
        case 1:
            if (!get_amount("Enter deposit amount: ", &amount))
                continue;

            balance += amount;
            deposits++;
            printf("\n[OK] Deposit successful.\n");
            printf("     New balance: %.0f RWF\n", balance);
            break;

        case 2:
            if (!get_amount("Enter withdrawal amount: ", &amount))
                continue;

            if (amount > balance) {
                printf("\n[!] Transaction rejected: insufficient balance.\n");
                continue;
            }

            balance -= amount;
            withdrawals++;
            printf("\n[OK] Withdrawal successful.\n");
            printf("     New balance: %.0f RWF\n", balance);
            break;

        case 3:
            printf("\n[INFO] Current balance: %.0f RWF\n", balance);
            break;

        case 4:
            printf("\n[SUMMARY]\n");
            printf(" Deposits    : %d\n", deposits);
            printf(" Withdrawals : %d\n", withdrawals);
            break;

        default:
            printf("\n[!] Invalid choice. Select an option from 1 to 5.\n");
            break;
        }
    }

    return 0;
}
