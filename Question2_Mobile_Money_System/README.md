# Question 2 — Mobile-Money Transaction System

Source: [`q2_mobile_money.c`](q2_mobile_money.c)

This program is a transaction processing system for a mobile-money agent. It
shows a menu of five options — deposit, withdraw, check balance, transaction
summary and exit — and loops so the agent can carry out any number of
transactions in one session without restarting. Deposits and withdrawals are
validated before they touch the balance: an amount must be a positive, finite
number with no stray characters after it, and a withdrawal must also fit within
the funds available, with any rejected transaction returning straight to the
menu so it can never alter the balance or be counted in the summary. The
counters therefore report only transactions that actually succeeded. Two
helpers keep the logic tidy — `get_amount()` prompts for and validates an
amount for both transaction types, and `clear_input()` discards the rest of the
line after a bad entry, reporting whether anything other than whitespace was
left over so that input like `50abc` is rejected rather than quietly read as
`50`. Reaching the end of the input closes the session cleanly instead of
leaving the program looping on a prompt it can no longer answer.

Build and run with `gcc -Wall -Wextra -std=c11 -o q2 q2_mobile_money.c` then
`./q2`.
