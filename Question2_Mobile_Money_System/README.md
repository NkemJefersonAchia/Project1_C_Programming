# Question 2 — Transaction Processing and Control Flow

**Points:** 4

A transaction processing system for a mobile-money agent. The agent keeps
processing deposits, withdrawals and balance checks in one session until they
choose Exit. Every amount is validated before it touches the balance.

## Deliverable 1 — Complete C source code

[`q2_mobile_money.c`](q2_mobile_money.c)

Two small helpers keep `main()` readable: `clear_input()` empties leftover
characters after a bad entry, and `get_amount()` prompts for an amount and
rejects anything that is not a positive number — so the deposit and withdrawal
cases don't repeat the same checks.

| Requirement | Where it's done |
|---|---|
| Deposit — add an amount to the balance | `case 1` |
| Withdrawal — only if positive and funds are sufficient | `case 2`, guarded by `get_amount()` and `amount > balance` |
| Balance inquiry | `case 3` |
| Transaction summary — successful deposits and withdrawals | `case 4`, using the `deposits` and `withdrawals` counters |
| Exit — terminate the program | `choice == 5`, checked before the `switch` |
| Appropriate data types | `double balance`, `double amount`, `int deposits`, `int withdrawals`, `int choice` |
| `switch` or `if-else` to process the operation | `switch (choice)` with a `default` case |
| Loop for multiple transactions without restarting | `while (1)` around the whole menu |
| `continue` to return to the menu on invalid input | Every rejection path — bad number, non-positive amount, insufficient funds |
| `break` to terminate a loop or control-flow structure | Both: ends each `switch` case, and exits the `while` loop on Exit |
| Prevent negative amounts and over-withdrawal | `get_amount()` plus the balance check in `case 2` |
| Clear messages for success and failure | `"... successful."` and `"Transaction rejected: ..."` |
| Run until the agent explicitly selects Exit | Only `choice == 5` breaks the loop |

### Build and run

```bash
gcc -Wall -Wextra -std=c11 -o q2 q2_mobile_money.c
./q2
```

Compiles with **zero warnings** under `-Wall -Wextra`.

## Deliverable 2 — Sample input/output

This run covers all five menu options plus four invalid cases: a withdrawal
larger than the balance, a negative amount, letters instead of a number, and a
menu choice that doesn't exist.

```
===== MOBILE MONEY TRANSACTION SYSTEM =====

1. Deposit
2. Withdraw
3. Check Balance
4. Transaction Summary
5. Exit

Enter choice: 1
Enter deposit amount: 50000
Deposit successful.
Current balance: 50000 RWF

Enter choice: 2
Enter withdrawal amount: 70000
Transaction rejected: insufficient balance.

Enter choice: 2
Enter withdrawal amount: -500
Transaction rejected: amount must be greater than zero.

Enter choice: abc
Invalid input. Please enter a number from 1 to 5.

Enter choice: 2
Enter withdrawal amount: 20000
Withdrawal successful.
Current balance: 30000 RWF

Enter choice: 3
Current balance: 30000 RWF

Enter choice: 4
Successful deposits   : 1
Successful withdrawals: 1

Enter choice: 9
Invalid choice. Please select 1 to 5.

Enter choice: 5
System terminated.
```

The summary reports 1 deposit and 1 withdrawal, not 1 and 3 — the two rejected
withdrawals never reached the counter. That is the `continue` doing its job.

To reproduce the run without typing:

```bash
printf '1\n50000\n2\n70000\n2\n-500\nabc\n2\n20000\n3\n4\n9\n5\n' | ./q2
```

### Extra validation checks

| Input | Expected | Result |
|-------|----------|--------|
| Withdraw exactly the full balance (5000 from 5000) | Allowed, balance goes to 0 | Pass |
| Deposit of `0` | Rejected | Pass |
| Withdrawal larger than the balance | Rejected | Pass |
| Negative amount on deposit or withdrawal | Rejected | Pass |
| Letters at the choice prompt | Rejected, buffer cleared, menu reappears | Pass |
| Letters at the amount prompt | Rejected, buffer cleared, no infinite loop | Pass |
| Menu choice `0`, `-3` or `9` | Rejected by the `default` case | Pass |

The "exactly the full balance" case matters because the check is
`amount > balance`, not `>=`. Withdrawing your whole balance is legitimate and
the program allows it.

## Deliverable 3 — How the program uses conditionals, loops, break and continue

### Data types

`balance` and `amount` are `double` so the program can hold large values and
accept amounts like `1500.50`. `choice`, `deposits` and `withdrawals` are
`int`, since they only hold small whole numbers. Balances print with `%.0f`
because Rwandan Francs aren't used in fractions.

### Conditionals

A `switch` picks the operation from the menu choice, and its `default` case
catches anything outside 1 to 5. Inside `get_amount()` an `if` rejects amounts
that are zero or negative, and `case 2` adds a second `if` comparing the amount
against the current balance. The balance and counters are only updated *after*
every check passes.

### The loop

A `while (1)` wraps the whole menu so the agent can run as many transactions as
they like without restarting. The only way out is the Exit branch, so the
session continues until the agent explicitly chooses it.

### `continue`

Whenever input is bad — letters typed, a non-positive amount, insufficient
funds — the program explains why and hits `continue`. That skips the rest of
the loop body and jumps straight back to "Enter choice", so a rejected
transaction can never change the balance or bump a counter.

When `scanf` fails, `clear_input()` empties the leftover characters first.
Without it, `scanf` would leave the bad text in the buffer and read the same
characters again on the next pass, looping forever.

### `break`

It appears in two different roles:

- **Inside the `switch`**, each `break` ends that case so execution doesn't
  fall through into the next one.
- **For Exit**, `choice == 5` is checked *before* the switch and `break` there
  leaves the `while` loop entirely, ending the program.

That placement is deliberate. If the Exit `break` sat inside the `switch` it
would only exit the switch, the loop would keep going, and the program would
never terminate.
