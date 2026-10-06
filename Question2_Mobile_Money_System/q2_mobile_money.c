#include <stdio.h>

/* discard leftover characters so a bad entry isn't read again */
void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* prompt for an amount and reject anything that isn't a positive number */
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

int main(void)
{
    double balance = 0, amount;
    int choice, deposits = 0, withdrawals = 0;

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
            continue;
        }

        if (choice == 5) {
            printf("System terminated.\n");
            break;
        }

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
                printf("Transaction rejected: insufficient balance.\n");
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
