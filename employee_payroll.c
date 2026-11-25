/*
 * Employee Payroll Management System
 * Skills used : Structures, Arrays, Functions
 * Description : Stores employee details and calculates salary,
 *               allowances and deductions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMP 100

/* Allowance and deduction rates */
#define HRA_RATE   0.20   /* House Rent Allowance : 20% of basic */
#define DA_RATE    0.10   /* Dearness Allowance   : 10% of basic */
#define TA_AMOUNT  1500.0 /* Travel Allowance     : fixed        */
#define PF_RATE    0.12   /* Provident Fund       : 12% of basic */
#define PROF_TAX   200.0  /* Professional Tax     : fixed        */

/* Structure to store employee details and salary components */
struct Employee
{
    int    id;
    char   name[50];
    char   designation[30];
    double basic;
    double hra, da, ta;      /* allowances */
    double pf, tax, profTax; /* deductions */
    double gross, totalDed, net;
};

struct Employee emp[MAX_EMP];  /* array of employees */
int count = 0;

/* ---------- Helper input functions ---------- */

void readString(const char *prompt, char *dest, int size)
{
    printf("%s", prompt);
    fgets(dest, size, stdin);
    dest[strcspn(dest, "\n")] = '\0';
}

double readDouble(const char *prompt, double min)
{
    char buf[100];
    double v;

    while (1)
    {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) != NULL &&
            sscanf(buf, "%lf", &v) == 1 && v >= min)
            return v;
        printf("  Invalid input! Enter a number >= %.0f.\n", min);
    }
}

int readInt(const char *prompt, int min, int max)
{
    char buf[100];
    int v;

    while (1)
    {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) != NULL &&
            sscanf(buf, "%d", &v) == 1 && v >= min && v <= max)
            return v;
        printf("  Invalid input! Enter a number between %d and %d.\n", min, max);
    }
}

/* ---------- Core functions ---------- */

/* Returns index of employee with given ID, or -1 if not found */
int findEmployee(int id)
{
    int i;
    for (i = 0; i < count; i++)
        if (emp[i].id == id)
            return i;
    return -1;
}

/* Income tax based on simple monthly gross slabs */
double calculateTax(double gross)
{
    if (gross <= 25000)  return 0.0;
    if (gross <= 50000)  return (gross - 25000) * 0.05;
    if (gross <= 100000) return 1250 + (gross - 50000) * 0.10;
    return 6250 + (gross - 100000) * 0.20;
}

/* Calculates allowances, deductions and net salary */
void calculateSalary(struct Employee *e)
{
    e->hra = e->basic * HRA_RATE;
    e->da  = e->basic * DA_RATE;
    e->ta  = TA_AMOUNT;
    e->gross = e->basic + e->hra + e->da + e->ta;

    e->pf      = e->basic * PF_RATE;
    e->profTax = PROF_TAX;
    e->tax     = calculateTax(e->gross);
    e->totalDed = e->pf + e->profTax + e->tax;

    e->net = e->gross - e->totalDed;
}

void addEmployee(void)
{
    struct Employee e;
    int id;

    if (count >= MAX_EMP)
    {
        printf("\nEmployee list is full!\n");
        return;
    }

    id = readInt("\nEnter Employee ID: ", 1, 999999);
    if (findEmployee(id) != -1)
    {
        printf("Employee ID already exists!\n");
        return;
    }

    e.id = id;
    readString("Enter Name: ", e.name, sizeof(e.name));
    readString("Enter Designation: ", e.designation, sizeof(e.designation));
    e.basic = readDouble("Enter Basic Salary: ", 1);

    calculateSalary(&e);
    emp[count++] = e;

    printf("\nEmployee added successfully!\n");
}

void displayPayslip(const struct Employee *e)
{
    printf("\n=========================================\n");
    printf("               PAY SLIP\n");
    printf("=========================================\n");
    printf("Employee ID  : %d\n", e->id);
    printf("Name         : %s\n", e->name);
    printf("Designation  : %s\n", e->designation);
    printf("-----------------------------------------\n");
    printf("Basic Salary        : %10.2f\n", e->basic);
    printf("  + HRA (20%%)       : %10.2f\n", e->hra);
    printf("  + DA  (10%%)       : %10.2f\n", e->da);
    printf("  + Travel Allowance: %10.2f\n", e->ta);
    printf("Gross Salary        : %10.2f\n", e->gross);
    printf("-----------------------------------------\n");
    printf("  - Provident Fund  : %10.2f\n", e->pf);
    printf("  - Professional Tax: %10.2f\n", e->profTax);
    printf("  - Income Tax      : %10.2f\n", e->tax);
    printf("Total Deductions    : %10.2f\n", e->totalDed);
    printf("=========================================\n");
    printf("NET SALARY          : %10.2f\n", e->net);
    printf("=========================================\n");
}

void displayAll(void)
{
    int i;

    if (count == 0)
    {
        printf("\nNo employee records found.\n");
        return;
    }

    printf("\n%-6s %-20s %-15s %10s %10s %10s\n",
           "ID", "Name", "Designation", "Gross", "Deduct.", "Net");
    printf("---------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++)
        printf("%-6d %-20s %-15s %10.2f %10.2f %10.2f\n",
               emp[i].id, emp[i].name, emp[i].designation,
               emp[i].gross, emp[i].totalDed, emp[i].net);
}

void searchEmployee(void)
{
    int id, idx;

    if (count == 0)
    {
        printf("\nNo employee records found.\n");
        return;
    }

    id = readInt("\nEnter Employee ID to search: ", 1, 999999);
    idx = findEmployee(id);

    if (idx == -1)
        printf("Employee not found!\n");
    else
        displayPayslip(&emp[idx]);
}

void updateSalary(void)
{
    int id, idx;

    if (count == 0)
    {
        printf("\nNo employee records found.\n");
        return;
    }

    id = readInt("\nEnter Employee ID to update: ", 1, 999999);
    idx = findEmployee(id);

    if (idx == -1)
    {
        printf("Employee not found!\n");
        return;
    }

    emp[idx].basic = readDouble("Enter new Basic Salary: ", 1);
    calculateSalary(&emp[idx]);
    printf("Salary updated successfully!\n");
}

void deleteEmployee(void)
{
    int id, idx, i;

    if (count == 0)
    {
        printf("\nNo employee records found.\n");
        return;
    }

    id = readInt("\nEnter Employee ID to delete: ", 1, 999999);
    idx = findEmployee(id);

    if (idx == -1)
    {
        printf("Employee not found!\n");
        return;
    }

    for (i = idx; i < count - 1; i++)   /* shift records left */
        emp[i] = emp[i + 1];
    count--;

    printf("Employee deleted successfully!\n");
}

void showTotals(void)
{
    int i;
    double totalNet = 0, totalGross = 0;

    if (count == 0)
    {
        printf("\nNo employee records found.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        totalGross += emp[i].gross;
        totalNet   += emp[i].net;
    }

    printf("\nTotal Employees     : %d\n", count);
    printf("Total Gross Payout  : %.2f\n", totalGross);
    printf("Total Net Payout    : %.2f\n", totalNet);
}

int main(void)
{
    int choice;

    do
    {
        printf("\n=========================================\n");
        printf("   EMPLOYEE PAYROLL MANAGEMENT SYSTEM\n");
        printf("=========================================\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee / Show Pay Slip\n");
        printf("4. Update Basic Salary\n");
        printf("5. Delete Employee\n");
        printf("6. Payroll Summary\n");
        printf("7. Exit\n");

        choice = readInt("Enter your choice (1-7): ", 1, 7);

        switch (choice)
        {
            case 1: addEmployee();    break;
            case 2: displayAll();     break;
            case 3: searchEmployee(); break;
            case 4: updateSalary();   break;
            case 5: deleteEmployee(); break;
            case 6: showTotals();     break;
            case 7: printf("\nThank you for using the Payroll System!\n"); break;
        }
    } while (choice != 7);

    return 0;
}