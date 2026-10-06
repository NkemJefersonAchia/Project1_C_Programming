# Question 2 — Mobile-Money Transaction System

**Points:** 4

A transaction processing system for a mobile-money agent. The agent keeps
processing deposits, withdrawals and balance checks in one session until they
choose Exit. Every amount is checked before it touches the balance, and
anything that isn't a valid number gets cleared out of the input so the program
doesn't get stuck.

## Deliverable 1 — Source code

[`q2_mobile_money.c`](q2_mobile_money.c)

### How the requirements are met

| Requirement | Where it's done |
|---|---|
| Deposit — add an amount to the balance | `case 1` |
| Withdrawal — only if positive and funds are sufficient | `case 2`, guarded by `amount <= 0` and `amount > balance` |
| Balance inquiry | `case 3` |
| Transaction summary — count of successful deposits and withdrawals | `case 4`, using the `deposits` and `withdrawals` counters |
| Exit — terminate the program | `choice == 5`, checked before the `switch` |
| Appropriate data types for amounts, balances, counts, choices | `double balance`, `double amount`, `int deposits`, `int withdrawals`, `int choice` |
| `switch` or `if-else` to process the operation | `switch (choice)` with a `default` case |
| Loop for multiple transactions without restarting | `while (1)` around the whole menu |
| `continue` to return to the menu on invalid input | Every rejection path — bad number, non-positive amount, insufficient funds |
| `break` to terminate a loop or control-flow structure | Both: ends each `switch` case, and exits the `while` loop on Exit |
| Prevent negative amounts and over-withdrawal | The two `if` guards in cases 1 and 2 |
| Clear messages for success and failure | `"... successful."` and `"Transaction rejected: ..."` |
| Run until the agent explicitly selects Exit | Only `choice == 5` breaks the loop |

### Build and run

```bash
gcc -Wall -Wextra q2_mobile_money.c -o q2
./q2
```

Compiles with zero warnings under `-Wall -Wextra`.

### Menu

```
1. Deposit
2. Withdraw
3. Check Balance
4. Transaction Summary
5. Exit
```

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
Transaction rejected: Insufficient balance.

Enter choice: 2
Enter withdrawal amount: -500
Transaction rejected: Amount must be greater than zero.

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
Total transactions    : 2

Enter choice: 9
Invalid choice. Please select 1 to 5.

Enter choice: 5
System terminated.
```

Notice the summary reports 1 deposit and 1 withdrawal, not 1 and 3 — the three
rejected withdrawals never reached the counter. That's the `continue` doing its
job.

To reproduce this run without typing it:

```bash
printf '1\n50000\n2\n70000\n2\n-500\nabc\n2\n20000\n3\n4\n9\n5\n' | ./q2
```

### Extra validation checks

| Input | Expected | Result |
|-------|----------|--------|
| Withdraw exactly the full balance (5000 from 5000) | Allowed, balance goes to 0 | Pass |
| Deposit of `0` | Rejected — amount must be greater than zero | Pass |
| Withdrawal larger than the balance | Rejected — insufficient balance | Pass |
| Negative amount on deposit or withdrawal | Rejected | Pass |
| Letters (`abc`) at the choice prompt | Rejected, input buffer cleared, menu reappears | Pass |
| Menu choice `0`, `-3` or `9` | Rejected by the `default` case | Pass |

The "exactly the full balance" case matters because the check is
`amount > balance`, not `>=`. Withdrawing your whole balance is a legitimate
transaction and the program correctly allows it.

## Deliverable 3 — How the control flow works

### Data types

`balance` and `amount` are `double` so the program can hold large values and
won't break if someone types `1500.50`. `choice`, `deposits` and `withdrawals`
are plain `int`, since they only ever hold small whole numbers. Balances are
printed with `%.0f` because Rwandan Francs aren't used in fractions in
practice.

### Conditionals

A `switch` picks the operation based on the menu choice, and the `default` case
catches numbers outside 1 to 5. Inside the deposit and withdrawal cases, `if`
statements reject amounts that are zero or negative, and withdrawals also get
checked against the current balance. The balance and counters are only updated
*after* every check passes.

### The loop

A `while (1)` wraps the whole menu so the agent can run as many transactions as
they like without restarting the program. The only way out is the Exit branch,
so the session continues until the agent explicitly chooses it.

### `continue`

Whenever input is bad — letters typed, negative amount, not enough money — the
program prints why and hits `continue`. That skips the rest of the loop body
and jumps straight back to "Enter choice". So a rejected transaction can never
accidentally change the balance or bump the counters.

When `scanf` fails, `clear_input()` empties the leftover characters first.
Without that, `scanf` would leave the bad text sitting in the buffer and read
the same characters again on the next pass, looping forever.

### `break`

It's used in two different ways here:

- **Inside the `switch`**, each `break` just ends that case so execution
  doesn't fall through into the next one.
- **For Exit**, the program checks `choice == 5` *before* the switch and calls
  `break` there, which leaves the `while` loop completely and ends the program.

That placement is deliberate. If the Exit `break` were inside the `switch`, it
would only exit the switch and the loop would keep going — a classic bug, and
the program would never terminate.
