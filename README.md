# Stateline Gas Station System

## University of Technology, Jamaica

### Faculty of Engineering and Computing  
**School of Computing and Information Technology**  
**Programming 2 (CMP1025) Group Project**

**Contributors:**  
- Diwani Walters  
- Kemone Laws  
- Olivia McFarlane  
- Javone - Anthony Gordon  
- Craig Williamson  

**Date:** April 12, 2024  

---

## Table of Contents
- [Introduction](#introduction)
- [System Overview](#system-overview)
- [Features](#features)
  - [Administrator Functions](#administrator-functions)
  - [Customer Transactions](#customer-transactions)
  - [Reports](#reports)
- [Technical Details](#technical-details)
- [How to Run](#how-to-run)
- [Contact](#contact)

---

## Introduction
This README provides an overview of the **Stateline Gas Station System**, detailing its functionality, user interactions, and system processes.

---

## System Overview
The **Stateline Gas Station System** is designed to manage fuel sales, customer accounts, and administrative tasks. It includes:
- Customer fuel purchasing (cash and charge accounts)
- Fuel inventory management
- Administrative access for account updates and reports

---

## Features

### Administrator Functions
Administrators can:
- Manage charge customer accounts (add, update, delete customers)
- Refuel fuel tanks
- Generate system reports (sales, charge accounts, fuel inventory)
- View and process payments

### Customer Transactions
Customers can:
- Purchase fuel using cash or charge accounts
- View available fuel types and prices
- Make payments to charge accounts

### Reports
System generates:
- **Sales Reports** (total revenue from cash and charge sales)
- **Charge Customer Reports** (account balances, outstanding payments)
- **Fuel Reports** (fuel stock, purchases, and usage trends)

---

## Technical Details
- **Programming Language:** C
- **Data Storage:** Binary and text files for customer records and fuel inventory
- **Security:** Admin authentication required for account modifications
- **File Handling:** Fuel and customer records stored in structured files

---

## How to Run
1. Compile the program using a C compiler (e.g., GCC):
   ```
   gcc -o stateline_gas_station main.c
   ```
2. Run the compiled executable:
   ```
   ./stateline_gas_station
   ```
3. Follow the on-screen menu to interact with the system.

---

## Contact
For more information, contact the project contributors.

---

*This README summarizes the system functionalities based on the user manual and project documentation.*

