/*
 * Mobile-money transaction system for an agent.
 *
 * Shows a menu and keeps processing deposits, withdrawals, balance
 * checks and summaries until the agent chooses Exit. Every amount is
 * validated before it is allowed to change the balance.
 */

#include <stdio.h>

/*
 * Throws away whatever is left on the input line.
 *
 * Needed because a failed scanf() leaves the bad text sitting in the
 * buffer. Without clearing it, the next scanf() would read the same
 * characters again and the program would loop forever.
 */
void clear_input(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/*
 * Prompts for an amount and rejects anything that is not a positive
 * number. Returns 1 if the amount is usable, 0 if it was rejected.
 *
 * Both the deposit and withdrawal cases call this, so neither has to
 * repeat the same two checks.
 */
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

    /* counters for the transaction summary */
    int choice, deposits = 0, withdrawals = 0;

    printf("===== MOBILE MONEY TRANSACTION SYSTEM =====\n\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Check Balance\n");
    printf("4. Transaction Summary\n");
    printf("5. Exit\n");

    /* keeps the agent in the menu until they choose Exit */
    while (1) {
        printf("\nEnter choice: ");

        /* letters instead of a number: clear them and ask again */
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number from 1 to 5.\n");
            clear_input();
            continue;
        }

        /*
         * Exit is handled here rather than inside the switch. A break
         * inside the switch would only leave the switch, and the loop
         * would carry on running.
         */
        if (choice == 5) {
            printf("System terminated.\n");
            break;
        }

        switch (choice) {
        case 1:
            /* continue sends a rejected deposit back to the menu
               before the balance or the counter can change */
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

            /* a withdrawal also has to fit inside the balance */
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

        /* any number outside 1 to 5 lands here */
        default:
            printf("Invalid choice. Please select 1 to 5.\n");
            break;
        }
    }

    return 0;
}
