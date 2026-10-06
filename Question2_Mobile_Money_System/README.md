# Question 2 — Mobile-Money Transaction System

Source: [`q2_mobile_money.c`](q2_mobile_money.c)

This program is a transaction processing system for a mobile-money agent. It
shows a menu of five options — deposit, withdraw, check balance, transaction
summary and exit — and loops so the agent can carry out any number of
transactions in one session without restarting. Deposits and withdrawals are
validated before they touch the balance: an amount must be a positive number,
and a withdrawal must also fit within the funds available, with any rejected
transaction returning straight to the menu so it can never alter the balance or
be counted in the summary. The counters therefore report only transactions that
actually succeeded. Two helpers keep the logic tidy — `get_amount()` prompts for
and validates an amount for both transaction types, and `clear_input()` clears
the input buffer after a bad entry so the program cannot get stuck re-reading
it.

Build and run with `gcc -Wall -Wextra -std=c11 -o q2 q2_mobile_money.c` then
`./q2`.
