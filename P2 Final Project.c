/*****************************************************************************
Authors: Diwani Walters - 2303848, Olivia McFarlane - 2301555,
		 Kemone Laws - 2109446, Javone-Anthony Gordon - 2206126,
		 Craig Williamson - 2301516
Tutor's Name: Kenrayan Whittle & Tyrone Edwards
Occurence: UN6 & UN8
Date: April 12, 2024
Description: P2 Final Project
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>

// Constants for constraints
#define MIN_DEPOSIT_AMOUNT 10000.0
#define MIN_CARD_AMOUNT 1000.0
#define MIN_CHEQUE_AMOUNT 5000.0
#define MAX_LITRES 1000.0

//Serve Customer Constants
#define Fuel87 184.90
#define Fuel87Max 75708.23
#define Fuel90 193.69
#define Fuel90Max 113562.35
#define Diesel 183.30
#define DieselMax 94635.29
#define Fw30 2900
#define Fw40 3500
#define FifteenW40 3600
#define Sae40 2100
#define GCT 0.16
#define NumCus 2147483647

//Fuel Constants
#define recMax 1000
#define E87Cap 75708.23
#define E90Cap 113562.35
#define DieselCap 94635.29
#define E87Refuel 165.64
#define E90Refuel 171.74
#define DieselRefuel 159.38

typedef struct {
	int FuelType;
	double FuelAmnt;
	int LubReq;
	int LubAmnt;
	int PaymentType;
	double PaymentAmnt;
} CodCus;

typedef struct {
	int id;
	int CashCuscount;
	int ChargeCuscount;
	double TotalCashAmnt;
	double TotalChargeAmnt;
} SalesRprt;

typedef struct {
	int id;
	char custN[35];
	float initialDeposit;
	float depositBal;
	float initialLitres;
	float litresBal;
	float amountOwed;
	float amountPaid;
	float balance;
} ChargeRprt;

// Structure definition for charge customers
typedef struct {
	int custID;
	char custN[35];
	int numRep;
	char pref[15];
	float money;
	float lit;
} cCustomers;

// Structure definition for fuel
typedef struct {
	char fuelType[8];
	float amount;
	float sales;
	float purchasedLitres;
} Fuel;

int Menu(); //Function prototype for the menu
void Initialize(); // Function prototype to initialize data
void Add_Charge(cCustomers new); // Function prototype to add new charge customers
void Update_Charge(); ////Function prototype to update charge
void del_charge(); //Function prototype to delete charge
void AcceptPayment();
void InitializeSReportFile();
void InitializecReportFile();
void CustomerInformation(CodCus *CashPtr, cCustomers *ChargePtr);
void SalesReport();
void ServeCustomer();
void RefuelTank();
void Generate_Report();
void FuelReport();
void ChargeReport();

// Main function
int main() {
	srand(time(0)); // Seed the random number generator
	
	printf("**WELCOME TO STATELINE GAS STATION**\n\n");
	
	cCustomers record; // Declare a variable of type cCustomers
	int opt;
	Initialize(); // Call the Initialize function to set up data
	InitializeSReportFile();
	InitializecReportFile();

	//ViewRecords();
	opt = Menu(); //Call the Menu function to allow the user to navigate the system

	while (opt!=8) {
		switch (opt) {
			case 1:
				ServeCustomer();
				break;

			case 2:
				// Call the function to add a new charge customer
				Add_Charge(record);
				break;

			case 3:
				Update_Charge();
				break;

			case 4:
				del_charge();
				break;

			case 5:
				AcceptPayment();
				break;

			case 6:
				RefuelTank();
				break;

			case 7:
				Generate_Report();
				break;

			case 8:
				exit (1);
				break;

			default:
				printf("\nINVALID ENTRY");
		}
		opt = Menu();
	}
}

//Function definition for the menu
int Menu() {
	int opt;
	printf("\n\n**SYSTEM MENU**\n");
	printf("1 - Serve Customer\n2 - Add Charge Customer\n3 - Update Charge Customer\n4 - Delete Charge Customer");
	printf("\n5 - Make Payment to Charge Account\n6 - Refuel Tank\n7 - Generate Report\n8 - Exit");
	printf("\nEnter Option: ");
	scanf("%d", &opt);

	return opt;
}

// Function definition to initialize data
void Initialize() {
	// Declare variables and initialize data
	cCustomers zero = {0, "Null", 0, "NULL", 0.0, 0.0}, *zeroPtr= &zero, temp;
	Fuel initialF[3]= {
		{"E10-87", 75708.23 , 0.0, 0.0},
		{"E10-90", 113562.35, 0.0, 0.0},
		{"Diesel", 94635.29, 0.0, 0.0}
	}, Temp;
	cCustomers initial[3]= {
		{1, "TANKWELD_LTD    ", 3, "Deposit       ", 100000.0, 0.0},
		{2, "LEADING_EDGE_LTD", 2, "Maximum_Litres", 0.0, 50.0},
		{3, "DERRIMON_TRADING", 1, "Deposit       ", 50000.0, 0.0}
	};

	int x, id;
	char plateNum[3][5][7]= {{"5786KW", "0578PQ", "7777TY","__","__"},
		{"6588HH", "5436RK","__","__","__"},
		{"3378JQ","__","__","__","__"}
	};
	char tempNum[5][7]= {"__","__","__","__","__"};
	FILE *ptr = NULL, *ptrL= NULL, *fuelPtr= NULL;

	// Checking to see if the file is already created
	ptr = fopen("Charge Customers.bin", "rb");

	if(ptr == NULL) {
		printf("\aCharge Customers' file does not exist\n");

		ptr = fopen("Charge Customers.bin", "wb+");
		for (x = 0; x < recMax; x++) {
			fwrite(zeroPtr, sizeof(cCustomers), 1, ptr);
		}
		printf("\nCharge Customers' file now created\n");

		// Write initial data to the file
		for (x = 0; x < 3; x++) {
			fseek(ptr, (initial[x].custID- 1)* sizeof(cCustomers), SEEK_SET);
			fwrite(&initial[x], sizeof(cCustomers), 1, ptr);
		}
	} else {
		printf("\nCharge customer File loaded\n");
	}

	printf("\nRecords in Charge Customers file:\n");
	printf("Customer ID\tCustomer Name\t   # of Reps\t Preference\t Amount\t\tLitre\n");
	rewind(ptr); // Brings the file pointer to the beginning of the file

	// Display records in the file
	while (fread(&temp, sizeof(cCustomers), 1, ptr) == 1) {
		if (temp.custID != 0) {
			if(temp.lit == 0.0) {
				printf("%d\t      %s\t\t%d\t%s\t%.2f\tN/A\n", temp.custID, temp.custN, temp.numRep, temp.pref, temp.money);
			} else {
				if(temp.money == 0.0) {
					printf("%d\t      %s\t\t%d\t%s\tN/A\t\t%.2f\n", temp.custID, temp.custN, temp.numRep, temp.pref, temp.lit);
				}
			}
		}
	}
	fclose(ptr);

	// Open or create and initialize representatives list file
	ptrL= fopen("Representatives List.txt", "r");
	if(ptrL== NULL) {
		printf("\a\nRepresentatives List file does not exist\n\n");

		ptrL= fopen("Representatives List.txt", "a+");
		if(ptrL== NULL) {
			printf("\aRepresentatives List file not opened\n");
		} else {
			printf("\nRepresentatives List file now created\n\n");
			// Write initial data to the representatives list file
			for(x=0; x< 3; x++) {
				fprintf(ptrL,"%d\t%s\t%s\t%s\t%s\t%s\n",x+1, plateNum[x][0],plateNum[x][1], plateNum[x][2], plateNum[x][3], plateNum[x][4]);
			}
		}
	} else {
		printf("\nRepresentatives file loaded\n");
	}

	rewind(ptrL); // Rewind the file pointer to the beginning of the file
	printf("Representative list now has the following records\nCustomer ID\tRep #1\tRep #2\tRep #3\tRep #4\tRep #5");
	// Display records in the representatives list file
	while(fscanf(ptrL,"%d%s%s%s%s%s\n", &id, tempNum[0],tempNum[1], tempNum[2], tempNum[3], tempNum[4])!= EOF) {
		printf("\n%d\t\t%s\t%s\t%s\t%s\t%s\n", id, tempNum[0], tempNum[1], tempNum[2], tempNum[3], tempNum[4]);
	}
	fclose(ptrL);

	fuelPtr= fopen("Fuel Tanks.txt", "r");
	if(fuelPtr== NULL) {
		printf("\a\nFuel Tanks file does not exist\n\n");

		fuelPtr= fopen("Fuel Tanks.txt", "a+");
		if(fuelPtr== NULL) {
			printf("\aFuel Tanks file not opened\n");
		} else {
			printf("\nFuel Tanks file now created\n\n");
			for(x=0; x< 3; x++) {
				fprintf(fuelPtr, "%s\t%.2f\t%.2f\t%.2f\n", initialF[x].fuelType, initialF[x].amount, initialF[x].sales, initialF[x].purchasedLitres);
			}
		}
	} else {
		printf("\nFuel Tanks file loaded\n");
	}

	rewind(fuelPtr); // Rewind the file pointer to the beginning of the file
	printf("\nFuel Tank Stores\nFuel Type\tCurrent Amount\tSales\tPurchased Litres");
	while(fscanf(fuelPtr, "%s%f%f%f", Temp.fuelType, &Temp.amount, &Temp.sales, &Temp.purchasedLitres)!= EOF) {
		printf("\n%s\t\t%.2f\t%.2f\t%.2f\n", Temp.fuelType, Temp.amount, Temp.sales, Temp.purchasedLitres);
	}
	fclose(fuelPtr);
}

void InitializecReportFile() {
	int count = 0;
	ChargeRprt zeroC= {0, "NULL", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	ChargeRprt initialC[3]= {
		{1, "TANKWELD_LTD    ", 100000.0, 100000.0, 0.0, 0.0, 0.0, 0.0, 0.0},
		{2, "LEADING_EDGE_LTD", 0.0, .0, 50.0, 50.0, 0.0, 0.0, 0.0},
		{3, "DERRIMON_TRADING", 50000.0, 100000.0, 0.0, 0.0, 0.0, 0.0, 0.0}
	};

	FILE *Chargeptr = NULL;

	Chargeptr = fopen("Charge Report.bin", "rb"); //checking to see if the file is already created

	if(Chargeptr == NULL) {
		printf("\nError: File does not exist\n");

		//creating the random access file
		Chargeptr = fopen("Charge Report.bin", "wb");

		//Initializing the amount of records in the file
		for (count = 0; count < recMax; count++) {
			//fwrite(&address of structure, size of datatype, how many u doing at a time, the pointer)
			fwrite(&zeroC, sizeof(ChargeRprt), 1, Chargeptr);
		}

		printf("INFO: File now created with 1000 record\n");
		for (count = 0; count < 3; count++) {
			fseek(Chargeptr, (initialC[count].id- 1)* sizeof(ChargeRprt), SEEK_SET);
			fwrite(&initialC[count], sizeof(ChargeRprt), 1, Chargeptr);
		}
	} else {
		printf("INFO: File loaded\n");
	}

	fclose(Chargeptr);
}

void InitializeSReportFile() {
	SalesRprt First = {1, 0, 0, 0.0, 0.0};
	int count = 0;

	FILE *Salesptr = NULL;

	Salesptr = fopen("Sales.bin", "rb"); //checking to see if the file is already created

	if(Salesptr == NULL) {
		printf("\nError: File does not exist\n");

		//creating the random access file
		Salesptr = fopen("Sales.bin", "wb");

		//Initializing the amount of records in the file
		for (count = 0; count < 1; count++) {
			//fwrite(&address of structure, size of datatype, how many u doing at a time, the pointer)
			fwrite(&First, sizeof(SalesRprt), 1, Salesptr);
		}

		printf("INFO: File now created with 1 record\n");
	} else {
		printf("INFO: File loaded\n");
	}

	fclose(Salesptr);
}

void ServeCustomer() {
	printf("\n\n****************************************");
	printf("\n\n           SERVE CUSTOMER               ");
	printf("\n\n****************************************");
	printf("\n\n");

	CodCus CashCus;
	cCustomers DepositCus;
	SalesRprt Sales;
	CodCus *CashPtr = &CashCus;
	cCustomers *ChargePtr = &DepositCus;
	int LubQues, FuelType, LubReq, LubAmount = 0, Id, RepChoice, FuelRepNum, CusType;;
	double FuelAmount;
	char LisPlate[5][7];
	double ChargeTotal = 0, Change = 0, LubGct = 0, LubResult = 0, FuelCost = 0, LubCost = 0, TotalCash = 0, TotalCharge = 0, GrandTotal = 0;
	FILE *Salesptr = NULL, *filePtr= NULL, *update= NULL, *chargePtr= NULL, *FilePtr = NULL;;
	SalesRprt *SPtr= &Sales;
	ChargeRprt record;
	Fuel change;

	Sales.CashCuscount = 0;
	Sales.ChargeCuscount = 0;

	printf("Customer Type Menu");
	printf("\n\n1 - Cash on Delivery Customer\n2 - Charge Customer\n\nPlease select a number to indicate the type of customer: ");
	scanf("%d", &CusType);

	//Validating if correct customer type is entered
	while(CusType != 1 && CusType != 2) {
		printf("\nInvalid Customer Type!!");
		printf("\n1 - Cash on Delivery Customer\n2 - Charge Customer\n\nPlease select a number to indicate the type of customer: ");
		scanf("%d", &CusType);
	}

	// Get current time
	time_t currentTime;
	time(&currentTime);

	// Convert time to string
	char *dateStr = ctime(&currentTime);

	switch (CusType) {
		case 1:
			printf("\nFuel Type Menu");
			printf("\n1 - E10-87\n2 - E10-90\n3 - Diesel\n\nPlease Enter the number indicating the Customer's Fuel Type: ");
			scanf("%d", &CashPtr->FuelType);

			//Fuel Type Validation Check
			while(CashPtr->FuelType != 1 && CashPtr->FuelType != 2 && CashPtr->FuelType != 3) {
				printf("\nInvalid Fuel Type!!");
				printf("\n1 - E10-87\n2 - E10-90\n3 - Diesel\n\nPlease Enter the number indicating the Customer's Fuel Type: ");
				scanf("%d", &CashPtr->FuelType);
			}

			printf("\nPlease Enter the Customer's desired Fuel Litre Amount: ");
			scanf("%lf", &CashPtr->FuelAmnt);

			printf("\n1 - Yes\n2 - No\n\nDoes the Customer want Lubricant(Please enter the number to indicate yes or no): ");
			scanf("%d", &LubQues);

			//Lubricant Request Validation Check
			while(LubQues != 1 && LubQues != 2) {
				printf("\nInvalid Option!!");
				printf("\n1 - Yes\n2 - No\n\nDoes the Customer want Lubricant(Please enter the number to indicate yes or no): ");
				scanf("%d", &LubQues);
			}

			if (LubQues == 1) {
				printf("\nLubricant Menu");
				printf("\n1 - FW-30\n2 - FW-40\n3 - 15W-40\n4 - SAE-40\n\nPlease Enter the number indicating the Customer's Lubricant Type: ");
				scanf("%d", &CashPtr->LubReq);

				//Lubricant Type Validation Check
				while(CashPtr->LubReq != 1 && CashPtr->LubReq != 2 && CashPtr->LubReq != 3 && CashPtr->LubReq != 4) {
					printf("\nInvalid Lubricant Type!!");
					printf("\n1 - FW-30\n2 - FW-40\n3 - 15W-40\n4 - SAE-40\n\nPlease Enter the number indicating the Customer's Lubricant Type: ");
					scanf("%d", &CashPtr->LubReq);
				}

				printf("\nHow many bottles of Lubricant does the customer desire? ");
				scanf("%d", &CashPtr->LubAmnt);
			}


			//FUEL CALCULATION
			switch (CashPtr->FuelType) {
				case 1:
					FuelCost = CashPtr->FuelAmnt * Fuel87;
					break;
				case 2:
					FuelCost = CashPtr->FuelAmnt * Fuel90;
					break;
				case 3:
					FuelCost = CashPtr->FuelAmnt * Diesel;
					break;
			}

			//LUBRICANT CALCULATION
			switch (CashPtr->LubReq) {
				case 1:
					LubCost = CashPtr->LubAmnt * Fw30;
					break;
				case 2:
					LubCost = CashPtr->LubAmnt * Fw40;
					break;
				case 3:
					LubCost = CashPtr->LubAmnt * FifteenW40;
					break;
				case 4:
					LubCost = CashPtr->LubAmnt * Sae40;
					break;
			}

			LubGct = LubCost * GCT;

			LubResult = LubCost + LubGct;

			TotalCash = FuelCost + LubResult;

			printf("\n1 - Cash\n2 - Card\n\nPlease Enter the number indicating the Customer's Payment Type: ");
			scanf("%d", &CashPtr->PaymentType);

			//Payment Type Validation Check
			while(CashPtr->PaymentType != 1 && CashPtr->PaymentType != 2) {
				printf("\nInvalid Payment Type!!");
				printf("\n1 - Cash\n2 - Card\n\nPlease Enter the number indicating the Customer's Payment Type: ");
				scanf("%d", &CashPtr->PaymentType);
			}

			//Validating that customers who pay by cash dont buy less than 2 litres of fuel
			while(CashPtr->PaymentType == 1 && CashPtr->FuelAmnt < 2) {
				printf("\nThe minimum fuel purchase is two litres for cash payments!!");
				printf("\nPlease re-enter the Customer's desired Fuel Amount: ");
				scanf("%lf", &CashPtr->FuelAmnt);
			}

			printf("\nTotal Amount: \t\t$%.2lf", TotalCash);
			printf("\nPlease Enter the Customer's Payment Amount: ");
			scanf("%lf", &CashPtr->PaymentAmnt);

			//Validating that customers who pay by card payment amount is not less than 1000
			while(CashPtr->PaymentType == 2 && CashPtr->PaymentAmnt < 1000) {
				printf("\nThe minimum amount is $1000 for card transactions!!");
				printf("\nPlease re-enter the Customer's Payment Amount: ");
				scanf("%lf", &CashPtr->PaymentAmnt);
			}

			Change = CashPtr->PaymentAmnt - TotalCash;

			//PRINTING RECEIPT
			printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
			// Print the date
			printf("\nDate: %s\n", dateStr);

			printf("\nType of Customer: \tCash on Delivery");

			printf("\nItems Purchased: ");

			switch (CashPtr->FuelType) {
				case 1:
					printf("\t%.2lf Litres of E10-87 Fuel", CashPtr->FuelAmnt);
					break;
				case 2:
					printf("\t%.2lf Litres of E10-90 Fuel", CashPtr->FuelAmnt);
					break;
				case 3:
					printf("\t%.2lf Litres of Diesel Fuel", CashPtr->FuelAmnt);
					break;
			}

			switch (CashPtr->LubReq) {
				case 1:
					printf("\n\t\t\t%d Bottles of FW-30 Lubricant", CashPtr->LubAmnt);
					break;
				case 2:
					printf("\n\t\t\t%d Bottles of FW-40 Lubricant", CashPtr->LubAmnt);
					break;
				case 3:
					printf("\n\t\t\t%d Bottles of 15W-40 Lubricant", CashPtr->LubAmnt);
					break;
				case 4:
					printf("\n\t\t\t%d Bottles of SAE-40 Lubricant", CashPtr->LubAmnt);
					break;
			}

			printf("\nGCT Amount: \t\t$%.2lf", LubGct);

			printf("\nTotal Amount: \t\t$%.2lf", TotalCash);

			printf("\nCash Tendered: \t\t$%.2lf", CashPtr->PaymentAmnt);

			if(CashPtr->PaymentType == 1) {
				printf("\nChange: \t\t$%.2lf", Change);
			}
			//Updating Sales File
			Salesptr = fopen("Sales.bin", "rb+");

			if(Salesptr == NULL) {
				printf("\nError: File does not opened\n");
			} else {
				SPtr->id = 1;

				//go to the desired position and read position
				fseek(Salesptr, sizeof(SalesRprt), SEEK_SET);
				fread(&Sales, sizeof(SalesRprt), 1, Salesptr);

				//Calculations
				Sales.CashCuscount = Sales.CashCuscount + 1;
				Sales.TotalCashAmnt = Sales.TotalCashAmnt + TotalCash;

				//go to the desired position and write to that position
				fseek(Salesptr, sizeof(SalesRprt), SEEK_SET);
				fwrite(&Sales, sizeof(SalesRprt), 1, Salesptr);

				printf("\nInfo: Sales Record Updated!\n");
			}
			fclose(Salesptr);

			filePtr = fopen("Fuel Tanks.txt", "r+");
			update = fopen("TEMP.txt", "a+");
			if(filePtr== NULL|| update== NULL) {
				printf("Error: Fuel Files Doesn't Open");
			} else {
				printf("Info: Fuel Files Open");
				if(CashPtr->FuelType== 1) {
					while(fscanf(filePtr, "%s%f%f%f", change.fuelType, &change.amount, &change.sales, &change.purchasedLitres)!= EOF) {
						if(strcmp(change.fuelType, "E10-87")== 0) {
							change.sales= change.sales+ FuelCost;
							change.amount= change.amount- CashPtr->FuelAmnt;
							change.purchasedLitres= change.purchasedLitres+ CashPtr->FuelAmnt;
							fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
						} else {
							fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
						}
					}
				} else {
					if(CashPtr->FuelType== 2) {
						while(fscanf(filePtr, "%s%f%f%f", change.fuelType, &change.amount, &change.sales, &change.purchasedLitres)!= EOF) {
							if(strcmp(change.fuelType, "E10-90")== 0) {
								change.sales= change.sales+ FuelCost;
								change.amount= change.amount- CashPtr->FuelAmnt;
								change.purchasedLitres= change.purchasedLitres+ CashPtr->FuelAmnt;
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							} else {
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							}
						}
					} else {
						while(fscanf(filePtr, "%s%f%f%f", change.fuelType, &change.amount, &change.sales, &change.purchasedLitres)!= EOF) {
							if(strcmp(change.fuelType, "Diesel")== 0) {
								change.sales= change.sales+ FuelCost;
								change.amount= change.amount- CashPtr->FuelAmnt;
								change.purchasedLitres= change.purchasedLitres+ CashPtr->FuelAmnt;
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							} else {
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							}
						}
					}
				}
				printf("\nInfo: Tank Record Updated!\n");
			}
			break;
		case 2:
			printf("\nPlease enter the customer's ID Number: ");
			scanf("%d", &ChargePtr->custID);


			FilePtr = fopen("Charge Customers.bin", "rb+");

			if(FilePtr == NULL) {
				printf("\nError: File does not opened\n");
			} else {
				printf("INFO: File loaded\n");

				fseek(FilePtr, (ChargePtr->custID - 1) * sizeof(cCustomers), SEEK_SET);
				fread(ChargePtr, sizeof(cCustomers), 1, FilePtr);

				FILE *FilePtr2 = NULL;

				FilePtr2 = fopen("Representatives List.txt", "r+");
				if(FilePtr2 == NULL) {
					printf("\nError: File does not open\n");
				} else {
					while (fscanf(FilePtr2, "%d%s%s%s%s%s", &Id, LisPlate[0], LisPlate[1], LisPlate[2], LisPlate[3], LisPlate[4]) != EOF) {
						if (Id == ChargePtr->custID) {
							printf("\nCustomer's License Plate(s) are: %s, %s, %s, %s, %s\n", LisPlate[0], LisPlate[1], LisPlate[2], LisPlate[3], LisPlate[4]);
						}
					}
				}
				do {
					printf("\n1 - E10-87\n2 - E10-90\n3 - Diesel\n\nPlease Enter the number indicating the Customer's Fuel Type: ");
					scanf("%d", &FuelType);

					//Fuel Type Validation Check
					while(FuelType != 1 && FuelType != 2 && FuelType != 3) {
						printf("\nInvalid Fuel Type!!");
						printf("\n1 - E10-87\n2 - E10-90\n3 - Diesel\n\nPlease Enter the number indicating the Customer's Fuel Type: ");
						scanf("%d", &FuelType);
					}

					printf("\nPlease Enter the Customer's desired Fuel Litre Amount: ");
					scanf("%lf", &FuelAmount);

					if(strcmp(ChargePtr->pref, "Maximum_Litres") == 0) {
						while (FuelAmount > ChargePtr->lit) {
							printf("\nThe maximum fuel litre amount is %.2f!!", ChargePtr->lit);
							printf("\nPlease Enter the Customer's desired Fuel Litre Amount: ");
							scanf("%lf", &FuelAmount);
						}
						ChargePtr->lit= ChargePtr->lit- FuelAmount;
					}

					printf("\n1 - Yes\n2 - No\n\nDoes the Customer want Lubricant(Please enter the number to indicate yes or no): ");
					scanf("%d", &LubQues);

					//Lubricant Request Validation Check
					while(LubQues != 1 && LubQues != 2) {
						printf("\nInvalid Option!!");
						printf("\n1 - Yes\n2 - No\n\nDoes the Customer want Lubricant(Please enter the number to indicate yes or no): ");
						scanf("%d", &LubQues);
					}

					if (LubQues == 1) {
						printf("\n1 - FW-30\n2 - FW-40\n3 - 15W-40\n4 - SAE-40\n\nPlease Enter the number indicating the Customer's Lubricant Type: ");
						scanf("%d", &LubReq);

						//Lubricant Type Validation Check
						while(LubReq != 1 && LubReq != 2 && LubReq != 3 && LubReq != 4) {
							printf("\nInvalid Lubricant Type!!");
							printf("\n1 - FW-30\n2 - FW-40\n3 - 15W-40\n4 - SAE-40\n\nPlease Enter the number indicating the Customer's Lubricant Type: ");
							scanf("%d", &LubReq);
						}

						printf("\nHow many bottles of Lubricant does the customer desire? ");
						scanf("%d", &LubAmount);
					}

					//FUEL CALCULATION
					switch (FuelType) {
						case 1:
							FuelCost = FuelAmount * Fuel87;
							break;
						case 2:
							FuelCost = FuelAmount * Fuel90;
							break;
						case 3:
							FuelCost = FuelAmount * Diesel;
							break;
					}

					//LUBRICANT CALCULATION
					switch (LubQues) {
						case 1:
							LubCost = LubAmount * Fw30;
							break;
						case 2:
							LubCost = LubAmount * Fw40;
							break;
						case 3:
							LubCost = LubAmount * FifteenW40;
							break;
						case 4:
							LubCost = LubAmount * Sae40;
							break;
					}

					LubGct = LubCost * GCT;
					LubResult = LubCost + LubGct;

					TotalCharge = FuelCost + LubResult;
					Change = CashPtr->PaymentAmnt - TotalCharge;

					//Checking if Total is over Deposit Amount
					ChargeTotal = FuelCost + LubResult;
				} while(ChargeTotal > ChargePtr->money && ChargePtr->money!= 0.0);
			}

			//PRINTING RECEIPT
			printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

			// Print the date
			printf("\nDate: %s\n", dateStr);

			printf("\nType of Customer: \tCharge");

			printf("\nItems Purchased: ");

			switch (FuelType) {
				case 1:
					printf("\t%.2lf Litres of E10-87 Fuel", FuelAmount);
					break;
				case 2:
					printf("\t%.2lf Litres of E10-90 Fuel", FuelAmount);
					break;
				case 3:
					printf("\t%.2lf Litres of Diesel Fuel", FuelAmount);
					break;
			}

			switch (LubReq) {
				case 1:
					printf("\n\t\t\t%d Bottles of FW-30 Lubricant", LubAmount);
					break;
				case 2:
					printf("\n\t\t\t%d Bottles of FW-40 Lubricant", LubAmount);
					break;
				case 3:
					printf("\n\t\t\t%d Bottles of 15W-40 Lubricant", LubAmount);
					break;
				case 4:
					printf("\n\t\t\t%d Bottles of SAE-40 Lubricant", LubAmount);
					break;
			}

			printf("\nGCT Amount: \t\t$%.2lf", LubGct);

			printf("\nTotal Amount: \t\t$%.2lf", TotalCharge);

			FilePtr = fopen("Charge Customers.bin", "rb+");
			chargePtr= fopen("Charge Report.bin", "rb+");

			if(FilePtr == NULL|| chargePtr== NULL) {
				printf("\nError: File does not opened\n");
			} else {
				printf("\n\nINFO: File loaded\n");

				fseek(FilePtr, (ChargePtr->custID - 1) * sizeof(cCustomers), SEEK_SET);
				fread(ChargePtr, sizeof(cCustomers), 1, FilePtr);
				fseek(chargePtr, (ChargePtr->custID - 1) * sizeof(ChargeRprt), SEEK_SET);
				fread(&record, sizeof(ChargeRprt), 1, chargePtr);

				if(ChargePtr->money!= 0.0) {
					ChargePtr->money = ChargePtr->money - TotalCharge;
					record.depositBal= record.depositBal- TotalCharge;
				} else {
					record.litresBal= record.litresBal- FuelAmount;
					record.amountOwed= record.amountOwed+ TotalCharge;
					record.balance= record.amountOwed- record.amountPaid;
				}

				printf("\nCharge Amount: \t$%.2lf", ChargePtr->money);

				fseek(FilePtr, (ChargePtr->custID - 1) * sizeof(cCustomers), SEEK_SET);
				fwrite(ChargePtr, sizeof(cCustomers), 1, FilePtr);
				fseek(chargePtr, (ChargePtr->custID - 1) * sizeof(ChargeRprt), SEEK_SET);
				fwrite(&record, sizeof(ChargeRprt), 1, chargePtr);
			}
			fclose(FilePtr);
			fclose(chargePtr);
			Salesptr = fopen("Sales.bin", "rb+");

			if(Salesptr == NULL) {
				printf("\nError: File does not opened\n");
			} else {
				SPtr->id = 1;

				//go to the desired position and read position
				fseek(Salesptr, sizeof(SalesRprt), SEEK_SET);
				fread(&Sales, sizeof(SalesRprt), 1, Salesptr);

				//Calculations
				Sales.ChargeCuscount = Sales.ChargeCuscount + 1;
				Sales.TotalChargeAmnt = Sales.TotalChargeAmnt + TotalCharge;

				//go to the desired position and write to that position
				fseek(Salesptr, sizeof(SalesRprt), SEEK_SET);
				fwrite(&Sales, sizeof(SalesRprt), 1, Salesptr);

				printf("\nInfo: Sales Record Updated!\n");
			}
			fclose(Salesptr);

			filePtr = fopen("Fuel Tanks.txt", "r+");
			update = fopen("TEMP.txt", "a+");
			if(filePtr== NULL|| update== NULL) {
				printf("Error: Fuel Files Doesn't Open");
			} else {
				printf("Info: Fuel Files Open");
				if(FuelType== 1) {
					while(fscanf(filePtr, "%s%f%f%f", change.fuelType, &change.amount, &change.sales, &change.purchasedLitres)!= EOF) {
						if(strcmp(change.fuelType, "E10-87")== 0) {
							change.sales= change.sales+ FuelCost;
							change.amount= change.amount- FuelAmount;
							change.purchasedLitres= change.purchasedLitres+ FuelAmount;
							fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
						} else {
							fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
						}
					}
				} else {
					if(FuelType== 2) {
						while(fscanf(filePtr, "%s%f%f%f", change.fuelType, &change.amount, &change.sales, &change.purchasedLitres)!= EOF) {
							if(strcmp(change.fuelType, "E10-90")== 0) {
								change.sales= change.sales+ FuelCost;
								change.amount= change.amount- FuelAmount;
								change.purchasedLitres= change.purchasedLitres+ FuelAmount;
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							} else {
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							}
						}
					} else {
						while(fscanf(filePtr, "%s%f%f%f", change.fuelType, &change.amount, &change.sales, &change.purchasedLitres)!= EOF) {
							if(strcmp(change.fuelType, "Diesel")== 0) {
								change.sales= change.sales+ FuelCost;
								change.amount= change.amount- FuelAmount;
								change.purchasedLitres= change.purchasedLitres+ FuelAmount;
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							} else {
								fprintf(update, "%s\t%.2f\t%.2f\t%.2f\n", change.fuelType, change.amount, change.sales, change.purchasedLitres);
							}
						}
					}
				}
				printf("\nInfo: Tank Records Updated!\n");
			}
			break;
	}
	fclose(filePtr);
	fclose(update);

	remove("Fuel Tanks.txt");
	rename("TEMP.txt", "Fuel Tanks.txt");
}

// Function definition to add new charge customers
void Add_Charge(cCustomers new) {
	FILE *ptr= NULL, *ptrL= NULL, *filePtr= NULL;
	int check1= 0, check2= 0, check3= 0, check4=0, x, y, z , pref, id, maxCustID = 0;;
	cCustomers temp;
	cCustomers *tempPtr= &temp, *newPtr= &new;
	ChargeRprt newC;
	char plateNum[5][7]= {"__", "__", "__", "__", "__"}, tempNum[5][7]= {"", "", "", "", ""};

	printf("\n\n*****************************************");
	printf("\n\n           ADD CHARGE CUSTOMER           ");
	printf("\n\n*****************************************");
	printf("\n\n");

	// Open the charge customers file
	ptr= fopen("Charge Customers.bin","rb+");
	if (ptr== NULL) {
		printf("\a\nCharge customers' file not opened!\n");
	} else {
		// Find the maximum custID in the file
		while (fread(tempPtr, sizeof(cCustomers), 1, ptr) != 0) {
			if (temp.custID > maxCustID) {
				maxCustID = temp.custID;
			}
		}

		// Set the new custID to one more than the maximum found
		new.custID = maxCustID + 1;

		printf("\nEnter the Customer's business name (Use underscore '_'for spaces): ");
		scanf("%s", new.custN);

		//Prompts for and input the number of representatives and validate the input
		printf("Enter the number of representatives: ");
		scanf("%d", &new.numRep);
		while(new.numRep< 1 || new.numRep> 5) {
			printf("\aINVALID, The number should be between 1- 5\nRe-enter: ");
			scanf("%d", &new.numRep);
		}

		// Open or create the representatives list file
		ptrL= fopen("Representatives List.txt", "a+");
		if(ptrL== NULL) {
			printf("\a\nRepresentatives file not opened!\n");

		} else {
			// Input and validate license plate numbers for each representative
			for(y= 0; y< new.numRep; y++) {
				do {
					if(check1== 1|| check2== 1 || check3== 1 || check4== 1) {
						printf("\aInvalid\n");
					}
					check1= 0, check2= 0, check3= 0, check4= 0;
					printf("Enter a UNIQUE and VALID License Plate Number (e.g. 5786KW): ");
					scanf("%s", plateNum[y]);
					// Check the format and uniqueness of the license plate number
					if (strlen(plateNum[y])!= 6) {
						check1= 1;
					}
					for(x= 0; x< 4; x++) {
						if(!isdigit(plateNum[y][x])) {
							check2= 1;
						}
					}
					for(x= 4; x< 6; x++) {
						if(!isalpha(plateNum[y][x])) {
							check3= 1;
						}
					}
					rewind(ptrL); // Rewind the file pointer to the beginning of the file
					while(fscanf(ptrL,"%d%s%s%s%s%s",&id, tempNum[0], tempNum[1], tempNum[2], tempNum[3], tempNum[4])!= EOF) {
						for(x= 0; x< 5; x++) {
							if(strcmp(plateNum[y], tempNum[x])== 0) {
								check4= 1;
							}
						}
					}
				} while(check1== 1|| check2== 1 || check3== 1|| check4== 1);
			}
			// Write the customer ID and representative license plate numbers to the representatives list file
			fprintf(ptrL,"%d\t%s\t%s\t%s\t%s\t%s\n", new.custID, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);
		}

		// Generate a random preference and prompt for related information
		pref= 1+ rand() % 2;
		switch(pref) {
			case 1:
				strcpy(new.pref, "Deposit       ");
				printf("Using Deposit\nEnter the amount: $");
				scanf("%f", &new.money);
				while(new.money< 10000) {
					printf("\aINVALID, The minimum is $10000.00\nRe-enter the amount: $");
					scanf("%f", &new.money);
				}
				new.lit= 0.0;
				break;
			case 2:
				strcpy(new.pref, "Maximum_Litres");
				new.money= 0.0;
				printf("Using Maximum Litre\nEnter the litre amount: ");
				scanf("%f", &new.lit);
				while(new.lit> 3785.41) {
					printf("\aINVALID, Maximum is 3785.41\nRe-enter the amount: ");
					scanf("%f", &new.lit);
				}
				break;
		}

		filePtr= fopen("Charge Report.bin", "rb+");
		if(filePtr== NULL) {
			printf("\n\aFile Not Opened!");
		} else {
			newC.id= new.custID;
			strcpy(newC.custN, new.custN);
			newC.initialDeposit= new.money;
			newC.depositBal= new.money;
			newC.initialLitres= new.lit;
			newC.litresBal= new.lit;
			fseek(filePtr, (new.custID- 1)* sizeof(ChargeRprt), SEEK_SET);
			fwrite(&newC, sizeof(cCustomers), 1, filePtr);
		}

		fclose(filePtr);

		// Write the new customer record to the charge customers file
		fseek(ptr, (new.custID- 1)* sizeof(cCustomers), SEEK_SET);
		fwrite(newPtr, sizeof(cCustomers), 1, ptr);
		printf("\nRecord Saved!\n");
	}
	fclose(ptr); // Close the charge customers file
}


void Update_Charge() {
	FILE *ptr= NULL, *ptrL= NULL, *ptrTemp=NULL;
	int check1= 0, check2= 0, check3= 0, check4=0, x, y,z , pref, id, maxCustID = 0;;
	cCustomers update;
	char plateNum[5][7]= {"__", "__", "__", "__", "__"}, tempNum[5][7]= {"", "", "", "", ""};
	int attempt, max_attempt=4, search_ID, opt, found=0;
	char password[6], authorized_password[6]="@DmiN";

	printf("\n\n****************************************");
	printf("\n\n         UPDATE CHARGE CUSTOMER         ");
	printf("\n\n****************************************");
	printf("\n\n");

	printf("\nEnter authorized password:  ");
	scanf("%s",password);

	//Selection to determine if the password is authorised or correct.
	if(strcmp(password, authorized_password)==0) {
		printf("\n\nLogin Complete.\n");
	} else {
		/*Loop that gives the user 4 attempts to enter the password incorrectly.
		After 4 incorrect attempts the program will kick the user out of the
		update option and back to the menu.*/
		for(attempt=1; attempt<max_attempt; attempt++) {
			if(strcmp(password, authorized_password)!=0) {
				if(max_attempt-attempt==1)
					printf("\n\nUnauthorized password! Final attempt.");
				else
					printf("\n\nUnauthorized password! %d attempts remaining.", max_attempt-attempt);

				printf("\n\nEnter authorized passcode:  ");
				scanf("%s", password);
				if(strcmp(password, authorized_password)==0) {
					printf("\n\nLogin Complete\n");
				}
			}
		}
		Menu(); //Menu function call
	}

	// Open the charge customers file
	ptr= fopen("Charge Customers.bin","rb+");
	if (ptr== NULL) {
		printf("\a\nCharge customers' file not opened!\n");
	} else {
		do {
			printf("\nUpdate by ID#: ");
			scanf("%d", &search_ID);

			//go to the desired position
			fseek(ptr, (search_ID - 1) * sizeof(cCustomers), SEEK_SET);

			//write to that position
			fread(&update, sizeof(cCustomers), 1, ptr);

			if(update.custID == 0) {
				printf("Record not found!\n");
			} else {
				printf("\nEdit Menu\n1. Edit Customer Name\n2. Number of Representatives\n3. Edit Preference\n4. Edit All\nOption: ");
				scanf("%d", &opt);

				switch (opt) {
					case 1:
						printf("\nEnter updated Customer business name (Use underscore '_'for spaces): ");
						scanf("%s", update.custN);
						break;
					case 2:
						//Prompts for and input the number of representatives and validate the input
						printf("Enter the updated number of representatives: ");
						scanf("%d", &update.numRep);
						while(update.numRep< 1 || update.numRep> 5) {
							printf("\aINVALID, The number should be between 1- 5\nRe-enter the updated number of representatives: ");
							scanf("%d", &update.numRep);
						}

						// Open or create the representatives list file
						ptrL= fopen("Representatives List.txt", "r");
						if(ptrL== NULL) {
							printf("\a\nRepresentatives file not opened!\n");
						} else {
							ptrTemp = (fopen("temp.txt", "w"));
							if(ptrTemp== NULL) {
								printf("\a\nError: file not opened!\n");
							} else {
								while (fscanf(ptrL, "%d%s%s%s%s%s", &id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]) != EOF) {

									if(id == search_ID) {
										// Update and validate license plate numbers for each representative
										for(y= 0; y< update.numRep; y++) {
											do {
												if(check1== 1|| check2== 1 || check3== 1 || check4== 1) {
													printf("\aInvalid\n");
												}
												check1= 0, check2= 0, check3= 0, check4= 0;
												printf("Enter a UNIQUE and VALID License Plate Number (e.g. 5786KW): ");
												scanf("%s", plateNum[y]);
												// Check the format and uniqueness of the license plate number
												if (strlen(plateNum[y])!= 6) {
													check1= 1;
												}
												for(x= 0; x< 4; x++) {
													if(!isdigit(plateNum[y][x])) {
														check2= 1;
													}
												}
												for(x= 4; x< 6; x++) {
													if(!isalpha(plateNum[y][x])) {
														check3= 1;
													}
												}

												rewind(ptrL); // Rewind the file pointer to the beginning of the file


												while(fscanf(ptrL,"%d%s%s%s%s%s",&id, tempNum[0], tempNum[1], tempNum[2], tempNum[3], tempNum[4])!= EOF) {
													for(x= 0; x< 5; x++) {
														if((strcmp(plateNum[y], tempNum[x])== 0)&&(id!=update.custID)) {
															check4= 1;
														}
													}
												}


												for(x=update.numRep; x<5; x++) {
													strcpy(plateNum[x], "__");
												}
											} while(check1== 1|| check2== 1 || check3== 1|| check4== 1);

										}
										fprintf(ptrTemp,"%d\t%s\t%s\t%s\t%s\t%s\n", id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);

									} else {
										fprintf(ptrTemp,"%d\t%s\t%s\t%s\t%s\t%s\n", id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);
									}
								}

								//closing both files
								fclose(ptrTemp);
								fclose(ptrL);

								remove("Representatives List.txt"); //delete a file
								rename("temp.txt", "Representatives List.txt"); //renaming a file
							}
						}
						break;
					case 3:
						// Generate a random preference and prompt for related information
						pref= 1+ rand() % 2;
						switch(pref) {
							case 1:
								strcpy(update.pref, "Deposit       ");
								printf("Using Deposit\nEnter the amount: $");
								scanf("%f", &update.money);
								while(update.money< 10000) {
									printf("\aINVALID, The minimum is $10000.00\nRe-enter the amount: $");
									scanf("%f", &update.money);
								}
								update.lit= 0.0;
								break;
							case 2:
								strcpy(update.pref, "Maximum_Litres");
								update.money= 0.0;
								printf("Using Maximum Litre\nEnter the litre amount: ");
								scanf("%f", &update.lit);
								while(update.lit> 3785.41) {
									printf("\aINVALID, Maximum is 3785.41\nRe-enter the amount: ");
									scanf("%f", &update.lit);
								}
								break;
						}
						break;
					case 4:
						printf("\nEnter updated Customer business name (Use underscore '_'for spaces): ");
						scanf("%s", update.custN);

						//Prompts for and input the number of representatives and validate the input
						printf("Enter the updated number of representatives: ");
						scanf("%d", &update.numRep);
						while(update.numRep< 1 || update.numRep> 5) {
							printf("\aINVALID, The number should be between 1- 5\nRe-enter the updated number of representatives: ");
							scanf("%d", &update.numRep);
						}

						// Open or create the representatives list file
						ptrL= fopen("Representatives List.txt", "r");
						if(ptrL== NULL) {
							printf("\a\nRepresentatives file not opened!\n");
						} else {
							ptrTemp = (fopen("temp.txt", "w"));
							if(ptrTemp== NULL) {
								printf("\a\nError: file not opened!\n");
							} else {
								//fscanf(ptrL,"%d%s%s%s%s%s", &id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);
								//while(!feof(ptrL))
								while (fscanf(ptrL, "%d%s%s%s%s%s", &id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]) != EOF) {

									if(id == search_ID) {
										// Update and validate license plate numbers for each representative
										for(y= 0; y< update.numRep; y++) {
											do {
												if(check1== 1|| check2== 1 || check3== 1 || check4== 1) {
													printf("\aInvalid\n");
												}
												check1= 0, check2= 0, check3= 0, check4= 0;
												printf("Enter a UNIQUE and VALID License Plate Number (e.g. 5786KW): ");
												scanf("%s", plateNum[y]);
												// Check the format and uniqueness of the license plate number
												if (strlen(plateNum[y])!= 6) {
													check1= 1;
												}
												for(x= 0; x< 4; x++) {
													if(!isdigit(plateNum[y][x])) {
														check2= 1;
													}
												}
												for(x= 4; x< 6; x++) {
													if(!isalpha(plateNum[y][x])) {
														check3= 1;
													}
												}

												rewind(ptrL); // Rewind the file pointer to the beginning of the file

												while(fscanf(ptrL,"%d%s%s%s%s%s",&id, tempNum[0], tempNum[1], tempNum[2], tempNum[3], tempNum[4])!= EOF) {
													for(x= 0; x< 5; x++) {
														if((strcmp(plateNum[y], tempNum[x])== 0)&&(id!=update.custID)) {
															check4= 1;
														}
													}
												}

												for(x=update.numRep; x<5; x++) {
													strcpy(plateNum[x], "__");
												}
											} while(check1== 1|| check2== 1 || check3== 1|| check4== 1);

										}
										fprintf(ptrTemp,"%d\t%s\t%s\t%s\t%s\t%s\n", id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);

									} else {
										fprintf(ptrTemp,"%d\t%s\t%s\t%s\t%s\t%s\n", id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);
									}
								}

								//closing both files
								fclose(ptrTemp);
								fclose(ptrL);

								remove("Representatives List.txt"); //delete a file
								rename("temp.txt", "Representatives List.txt"); //renaming a file
							}
						}

						// Generate a random preference and prompt for related information
						pref= 1+ rand() % 2;
						switch(pref) {
							case 1:
								strcpy(update.pref, "Deposit       ");
								printf("Using Deposit\nEnter the amount: $");
								scanf("%f", &update.money);
								while(update.money< 10000) {
									printf("\aINVALID, The minimum is $10000.00\nRe-enter the amount: $");
									scanf("%f", &update.money);
								}
								update.lit= 0.0;
								break;
							case 2:
								strcpy(update.pref, "Maximum_Litres");
								update.money= 0.0;
								printf("Using Maximum Litre\nEnter the litre amount: ");
								scanf("%f", &update.lit);
								while(update.lit> 3785.41) {
									printf("\aINVALID, Maximum is 3785.41\nRe-enter the amount: ");
									scanf("%f", &update.lit);
								}
								break;
						}

						break;
				}

				//go to the desired position
				fseek(ptr, (search_ID - 1) * sizeof(cCustomers), SEEK_SET);

				//write to that position
				fwrite(&update, sizeof(cCustomers), 1, ptr);

				printf("\nRecord Updated!\n");
			}
		} while(update.custID == 0);

		fclose(ptr); // Close the charge customers file
	}
}

void del_charge() {
	int attempt, max_attempt=4, search_ID;
	char password[6], authorized_password[6]="@DmiN";
	int found = 0, id;
	char plateNum[5][7];
	int continue_var;
	ChargeRprt zeroC= {0, "NULL", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, record;
	FILE *ranptr = NULL, *filePtr= NULL;
	FILE *seqptr = NULL;
	FILE *tempptr = NULL;
	cCustomers rec, del = {0, "Null", 0, "NULL", 0.0, 0.0};

	printf("\n\n****************************************");
	printf("\n\n         DELETE CHARGE CUSTOMER         ");
	printf("\n\n****************************************");
	printf("\n\n");

	printf("\nEnter authorized password: ");
	scanf("%s", password);

	// Loop that gives the user 4 attempts to enter the password correctly
	for (attempt = 1; attempt <= max_attempt; attempt++) {
		if (strcmp(password, authorized_password) == 0) { //compares if the password given is equal to system password
			printf("\nAuthorization successful.\n");


			//// Open charge customer and representative list file
			ranptr = fopen("Charge Customers.bin", "rb+");
			seqptr = fopen("Representatives List.txt", "r+");
			filePtr= fopen("Charge Report.bin", "rb+");

			if (ranptr == NULL || seqptr == NULL || filePtr== NULL) { //checks to see if both files were opened
				printf("Error: Files are not opened\n");
			} else {
				printf("\nEnter the customer ID to delete: ");
				scanf("%d", &search_ID);

				fseek(ranptr, (search_ID - 1) * sizeof(cCustomers), SEEK_SET); //finds and positions cursor at the location of the id entered
				fread(&rec, sizeof(cCustomers), 1, ranptr); //reads the line
				fseek(filePtr, (search_ID - 1) * sizeof(ChargeRprt), SEEK_SET); //finds and positions cursor at the location of the id entered
				fread(&record, sizeof(ChargeRprt), 1, filePtr); //reads the line

				if (rec.custID == search_ID || record.id== search_ID) { //chekcs to see if id given is equal to the id read at the line
					fseek(ranptr, (search_ID - 1) * sizeof(cCustomers), SEEK_SET);
					fwrite(&del, sizeof(cCustomers), 1, ranptr); //deletes the data in the line by placing null values/"empty spaces"
					fseek(filePtr, (search_ID - 1) * sizeof(ChargeRprt), SEEK_SET);
					fwrite(&zeroC, sizeof(ChargeRprt), 1, filePtr); //deletes the data in the line by placing null values/"empty spaces"
					printf("Record deleted\n");
				} else {
					printf("Record not found!!\n");
				}


				// Opens a temporary file to hold modified representatives list
				tempptr = fopen("temp.txt","w"); //opens the tempo
				if (tempptr == NULL) {
					printf("The file did not open\n");
				} else {
					while (fscanf(seqptr,"%d%s%s%s%s%s",&id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4])!= EOF) {
						if (id == search_ID) {
							found = 1;
						} else {
							// Delete the entry associated with the customer ID from the representatives list file
							fprintf(tempptr, "%d\t%s\t%s\t%s\t%s\t%s\n", id, plateNum[0], plateNum[1], plateNum[2], plateNum[3], plateNum[4]);
						}
					}
					if (found == 1) { //simple validation if file was opened or not
						printf("Record found\n");
					} else {
						printf("Record not found!!\n");
					}
					//closes both sequential file and temporary file
					fclose(seqptr);
					fclose(tempptr);

					//deletes the original representative list file and renames the temporary file with the updated data to the same name
					remove("Representatives List.txt");
					rename("temp.txt", "Representatives List.txt");
				}
			}
			fclose(ranptr);
			fclose(filePtr);
			// Exit the loop after successfully deleting the record
			break;
		} else {
			if (attempt < max_attempt) { //allows the user to re-enter the correct password if they have attempts left
				printf("\nUnauthorized password! %d attempts remaining.", max_attempt - attempt);
				printf("\nEnter authorized password: ");
				scanf("%s", password);
			} else { //locks the user out due to too many incorrect attempts
				printf("\nToo many incorrect attempts!!\n");
			}
		}

	}
}

void AcceptPayment() {
	char paymentMethod[10];
	float amount = 0;
	int customerId;
	cCustomers customer;
	FILE *file = NULL, *filePtr= NULL;
	float litresAmount = 0;
	int validLitres = 0;
	int validAmount = 0;
	int validPayment = 0;
	int i;
	ChargeRprt record;

	printf("\n\n****************************************");
	printf("\n\n     MAKE PAYMENT TO CHARGE ACCOUNT     ");
	printf("\n\n****************************************");
	printf("\n\n");

	// Open file in read and write
	file = fopen("Charge Customers.bin","rb+");
	if (file == NULL) {
		printf("\nError opening Charge Customer file.\n");
		return;
	}

	// Validity loop for customer ID
	printf("\nEnter customer ID number: ");
	scanf("%d", &customerId);
	fseek(file,(customerId - 1) * sizeof (cCustomers), SEEK_SET);
	fread(&customer,  sizeof (cCustomers), 1, file);

	while (customerId != customer.custID) {
		printf("\nCustomer with ID %d not found. Please enter a valid customer ID.\n", customerId);
		printf("\nEnter customer ID number: ");
		scanf("%d", &customerId);
		fseek(file,(customerId - 1) * sizeof (cCustomers), SEEK_SET);
		fread(&customer,  sizeof (cCustomers), 1, file);
	}

	if (strcmp(customer.pref, "Deposit       ") == 0) {
		// Validity loop for payment method
		while (!validPayment) {
			printf("\nEnter payment method (cash, card, cheque): ");
			scanf("%s", paymentMethod);

			// Convert payment method to lowercase for easier comparison
			for (i = 0; paymentMethod[i]; i++) {
				paymentMethod[i] = tolower(paymentMethod[i]);
			}

			// Check if payment method is valid
			if (strcmp(paymentMethod, "cash") == 0 || strcmp(paymentMethod, "card") == 0 || strcmp(paymentMethod, "cheque") == 0) {
				validPayment = 1;
			} else {
				printf("\nInvalid payment method. Please enter cash, card, or cheque.\n");
			}
		}
		if (customer.money == 0.0 ) {
			printf("\nDeposit is being renewed. Current balance is $0.00\n");


			// Validity loop for amount
			while (!validAmount) {
				printf("\nEnter payment amount: $");
				scanf("%f", &amount);

				// Check if payment amount meets minimum requirement based on payment method
				if ((strcmp(paymentMethod, "card") == 0 && amount < MIN_CARD_AMOUNT) || (strcmp(paymentMethod, "cheque") == 0 && amount < MIN_CHEQUE_AMOUNT)) {
					printf("\nMinimum payment amount for %s is $%.2f\n", paymentMethod, strcmp(paymentMethod, "card") == 0 ? MIN_CARD_AMOUNT : MIN_CHEQUE_AMOUNT);
				} else {
					validAmount = 1;
				}

				if (amount <= MIN_DEPOSIT_AMOUNT) {
					printf("\n\aINVALID, The minimum is $10000.00\nRe-");
				} else {
					validAmount = 1;
				}
			}

			customer.money = amount;
			printf("\nDeposit renewed to $%.2f \n", customer.money);
		} else {
			printf("\nYou are increasing your deposit\n");

			// Validity loop for amount
			while (!validAmount) {
				printf("\nYour current deposit holdings are $%.2f\n", customer.money);
				printf("\nEnter payment amount: $");
				scanf("%f", &amount);

				// Check if payment amount meets minimum requirement based on payment method
				if ((strcmp(paymentMethod, "card") == 0 && amount < MIN_CARD_AMOUNT) || (strcmp(paymentMethod, "cheque") == 0 && amount < MIN_CHEQUE_AMOUNT)) {
					printf("\nMinimum payment amount for %s is $%.2f\n", paymentMethod, strcmp(paymentMethod, "card") == 0 ? MIN_CARD_AMOUNT : MIN_CHEQUE_AMOUNT);
				} else {
					validAmount = 1;

				}
				if (amount + customer.money <= MIN_DEPOSIT_AMOUNT) {
					printf("\n\aINVALID, The minimum is $10000.00\nRe-enter\n");
				} else {
					validAmount = 1;
				}
			}
			customer.money += amount;
			printf("\nDeposit increased to $%.2f \n", customer.money);
		}
		filePtr= fopen("Charge Report.bin", "rb+");
		if(filePtr== NULL) {
			printf("\n\aFile Not Opened!");
		} else {
			fseek(filePtr,(customerId - 1) * sizeof (ChargeRprt), SEEK_SET);
			fread(&record,  sizeof (ChargeRprt), 1, filePtr);

			record.depositBal= customer.money;
		}
	} else {
		if(customer.lit == 0.0) {
			printf("\nYou are renewing the maximum litres. Current count is 0.0 l\n");
			while (!validLitres) {
				printf("\nEnter litres amount: ");
				scanf("%f", &litresAmount);

				// Check if litres exceed maximum allowed
				if (litresAmount > MAX_LITRES) {
					printf("\nLitres cannot exceed %.2f litres.\n", MAX_LITRES);
				} else {
					customer.lit = litresAmount;
					printf("\nLitres increased to %.2f litres\n", customer.lit);
					validLitres = 1;
				}
			}
		} else {
			printf("\nYou are increasing the  maximum litres. Current count is %.2f l\n", customer.lit);
			while (!validLitres) {
				printf("\nEnter litres amount: ");
				scanf("%f", &litresAmount);

				// Check if litres exceed maximum allowed
				if (litresAmount + customer.lit > MAX_LITRES) {
					printf("\nLitres cannot exceed %.2f litres.\n", MAX_LITRES);
				} else {
					customer.lit += litresAmount;
					printf("\nLitres increased to %.2f litres\n", customer.lit);
					validLitres = 1;
				}
			}
		}
		filePtr= fopen("Charge Report.bin", "rb+");
		if(filePtr== NULL) {
			printf("\n\aFile Not Opened!");
		} else {
			printf("\nNow make your payment\n");
			// Validity loop for payment method
			while (!validPayment) {
				printf("\nEnter payment method (cash, card, cheque): ");
				scanf("%s", paymentMethod);

				// Convert payment method to lowercase for easier comparison
				for (i = 0; paymentMethod[i]; i++) {
					paymentMethod[i] = tolower(paymentMethod[i]);
				}

				// Check if payment method is valid
				if (strcmp(paymentMethod, "cash") == 0 || strcmp(paymentMethod, "card") == 0 || strcmp(paymentMethod, "cheque") == 0) {
					validPayment = 1;
				} else {
					printf("\nInvalid payment method. Please enter cash, card, or cheque.\n");
				}
			}

			// Validity loop for amount
			while (!validAmount) {
				printf("\nEnter payment amount: $");
				scanf("%f", &amount);

				// Check if payment amount meets minimum requirement based on payment method
				if ((strcmp(paymentMethod, "card") == 0 && amount < MIN_CARD_AMOUNT) || (strcmp(paymentMethod, "cheque") == 0 && amount < MIN_CHEQUE_AMOUNT)) {
					printf("\nMinimum payment amount for %s is $%.2f\n", paymentMethod, strcmp(paymentMethod, "card") == 0 ? MIN_CARD_AMOUNT : MIN_CHEQUE_AMOUNT);
				} else {
					validAmount = 1;
				}
			}
			fseek(filePtr,(customerId - 1) * sizeof (ChargeRprt), SEEK_SET);
			fread(&record,  sizeof (ChargeRprt), 1, filePtr);

			record.amountPaid= record.amountPaid + amount;
			record.balance= record.amountOwed- record.amountPaid;
			record.litresBal= customer.lit;
		}
	}
	fseek(file,(customerId - 1) * sizeof (cCustomers), SEEK_SET);
	fwrite(&customer, sizeof (cCustomers), 1, file);
	fseek(filePtr,(customerId - 1) * sizeof (ChargeRprt), SEEK_SET);
	fwrite(&record,  sizeof (ChargeRprt), 1, filePtr);
	fclose(file);
	fclose(filePtr);
}

void RefuelTank() {
	int attempt=0, max_attempt=4;
	char password[6], authorized_password[6]="@DmiN", fuelSearch[8];
	Fuel refuel;
	FILE *seqptr = NULL, *tempptr = NULL;
	float amountRequest, rate;

	printf("\n\n*****************************************");
	printf("\n\n               REFUEL TANK               ");
	printf("\n\n*****************************************");
	printf("\n\n");

	printf("\nEnter authorized password: ");
	scanf("%s", password);

	// Loop that gives the user 4 attempts to enter the password correctly
	for (attempt = 1; attempt <= max_attempt; attempt++) {
		if (strcmp(password, authorized_password) == 0) { //compares if the password given is equal to system password
			printf("\nAuthorization successful.\n");

			// Open charge customer and representative list file
			seqptr = fopen("Fuel Tanks.txt", "r+");
			tempptr = fopen("temp.txt","a+");
			if(seqptr== NULL && tempptr == NULL) {
				printf("\nFiles did not open\n");
			} else {
				printf("\nE10-87\nE10-90\nDiesel\nEnter the fuel tank being refueled: ");
				scanf("%s", fuelSearch);

				while((strcmp(fuelSearch, "E10-87")!=0)&&(strcmp(fuelSearch, "E10-90")!=0)&&(strcmp(fuelSearch, "Diesel")!=0)) {
					printf("\nInvalid fuel type!");
					printf("\nE10-87\nE10-90\nDiesel\nEnter the fuel tank being refueled: ");
					scanf("%s", fuelSearch);
				}

				fscanf(seqptr, "%s%f%f%f", refuel.fuelType, &refuel.amount, &refuel.sales, &refuel.purchasedLitres);
				while(!feof(seqptr)) {
					if(strcmp(fuelSearch, refuel.fuelType)==0) {
						printf("\nFuel Type found");
						printf("\nHow much litres of %s do you want? ", fuelSearch);
						scanf("%f", &amountRequest);

						//if(((refuel.amount!=E87Cap)&&(strcmp(fuelSearch,"E10-87")== 0)) || ((refuel.amount!=E90Cap)&&(strcmp(fuelSearch,"E10-90")== 0)) || ((refuel.amount!=DieselCap)&&(strcmp(fuelSearch,"Diesel")== 0))) {
							if(strcmp(fuelSearch,"E10-87")== 0) {
								if(refuel.amount>=E87Cap){
										printf("\nThe fuel tank is already at maximum cappacity");
										return;
								}else{
									while((amountRequest + refuel.amount)> E87Cap) {
										printf("\nThe maximum capacity of the fuel tank is %f\nThe tank currently holds %.2f", E87Cap, refuel.amount);
										printf("\nHow much litres of %s do you want? ", fuelSearch);
										scanf("%f", &amountRequest);
									}	
								}
								rate= E87Refuel/ 3.785;
							}
								
							 else {
								if(strcmp(fuelSearch,"E10-90")== 0) {
									
									if(refuel.amount>=E90Cap){
											printf("\nThe fuel tank is already at maximum cappacity");
											return;
									}else{
										while((amountRequest + refuel.amount)> E90Cap) {
											printf("\nThe maximum capacity of the fuel tank is %f\nThe tank currently holds %.2f", E90Cap, refuel.amount);
											printf("\nHow much litres of %s do you want? ", fuelSearch);
											scanf("%f", &amountRequest);
										
										
										}
									}
									rate= E90Refuel/ 3.785;
								} else {
									if(refuel.amount>=DieselCap){
										printf("\nThe fuel tank is already at maximum cappacity");
										return;
									}else{
										while((amountRequest + refuel.amount)> DieselCap) {
											printf("\nThe maximum capacity of the fuel tank is %f\nThe tank currently holds %.2f", DieselCap, refuel.amount);
											printf("\nHow much litres of %s do you want? ", fuelSearch);
											scanf("%f", &amountRequest);
										}
									}
									
									rate= DieselRefuel/ 3.785;
								}
							}

						printf("\nThat will be $%.2f", amountRequest* rate);
						refuel.amount= refuel.amount+ amountRequest;
						fprintf(tempptr, "%s\t%.2f\t%.2f\t%.2f\n", refuel.fuelType, refuel.amount, refuel.sales, refuel.purchasedLitres);

					} else {
						fprintf(tempptr, "%s\t%.2f\t%.2f\t%.2f\n", refuel.fuelType, refuel.amount, refuel.sales, refuel.purchasedLitres);
					}
					fscanf(seqptr, "%s%f%f%f", refuel.fuelType, &refuel.amount, &refuel.sales, &refuel.purchasedLitres);
				}
				//closes both files
				fclose(seqptr);
				fclose(tempptr);

				remove("Fuel Tanks.txt");
				rename("temp.txt", "Fuel Tanks.txt");
				break;
			}
		} else {
			if (attempt < max_attempt) { //allows the user to re-enter the correct password if they have attempts left
				printf("\nUnauthorized password! %d attempts remaining.", max_attempt - attempt);
				printf("\nEnter authorized password: ");
				scanf("%s", password);
			} else { //locks the user out due to too many incorrect attempts
				printf("\nUnauthorized password! Final attempt. Access denied.\n");
			}
		}
	}
}


void Generate_Report() {
	int opt;
	int attempt, max_attempt=4, search_ID;
	char password[6], authorized_password[6]="@DmiN";

	printf("\n\n*****************************************");
	printf("\n\n             GENERATE REPORT             ");
	printf("\n\n*****************************************");
	printf("\n\n");

	printf("\nEnter authorized password:  ");
	scanf("%s",password);

	//Selection to determine if the password is authorised or correct.
	if(strcmp(password, authorized_password)==0) {
		printf("\n\nLogin Complete.\n");
	} else {
		/*Loop that gives the user 4 attempts to enter the password incorrectly.
		After 4 incorrect attempts the program will kick the user out of the
		update option and back to the menu.*/
		for(attempt=1; attempt<max_attempt; attempt++) {
			if(strcmp(password, authorized_password)!=0) {
				if(max_attempt-attempt==1)
					printf("\n\nUnauthorized password! Final attempt.");
				else
					printf("\n\nUnauthorized password! %d attempts remaining.", max_attempt-attempt);

				printf("\n\nEnter authorized passcode:  ");
				scanf("%s", password);
				if(strcmp(password, authorized_password)==0) {
					printf("\n\nLogin Complete.\n");
				}
			}
		}
		Menu(); //Menu function call
	}

	printf("\nReport Menu\n1 - Sales Report\n2 - Charge Customers Report\n3 - Fuel Report\n4 - View All\nEnter option: ");
	scanf("%d", &opt);
	switch(opt) {
		case 1:
			SalesReport();
			break;
		case 2:
			ChargeReport();
			break;
		case 3:
			FuelReport();
			break;
		case 4:
			SalesReport();
			ChargeReport();
			FuelReport();
			break;
		default:
			printf("\nInvalid option!");
	}

}

void SalesReport() {
	FILE *Salesptr = NULL;
	SalesRprt Sales;

	double GrandTotal = 0;

	Salesptr = fopen("Sales.bin", "rb+");

	if(Salesptr == NULL) {
		printf("\nError: File does not opened\n");
	} else {
		//go to the desired position and read position
		fseek(Salesptr, sizeof(SalesRprt), SEEK_SET);
		fread(&Sales, sizeof(SalesRprt), 1, Salesptr);

		GrandTotal = Sales.TotalCashAmnt + Sales.TotalChargeAmnt;

		printf("\n~~~~~Sales Report~~~~~");
		printf("\n\nTotal Cash Customers: \t\t%d", Sales.CashCuscount);
		printf("\nTotal Charge Customers: \t%d", Sales.ChargeCuscount);
		printf("\nCash Amount: \t\t\t$%.2lf", Sales.TotalCashAmnt);
		printf("\nCharge Amount: \t\t\t$%.2lf", Sales.TotalChargeAmnt);
		printf("\nGrand Total: \t\t\t$%.2lf", GrandTotal);
	}
	fclose(Salesptr);
}

void ChargeReport() {
	FILE *filePtr= NULL;
	ChargeRprt report;

	filePtr= fopen("Charge Report.bin", "rb+");
	if(filePtr== NULL) {
		printf("\n\aFile Not Opened!");
	} else {
		printf("\n\n~~~~~Charge Customer's Report~~~~~\n\n");
		printf("Customer ID Customer Name\tInitial Deposit\tDeposit Balance\tInitial Litres\tLitre Balance\tOwed\tPaid\tBalance\n");

		rewind(filePtr); // Brings the file pointer to the beginning of the file

		// Display records in the file
		while (fread(&report, sizeof(ChargeRprt), 1, filePtr) == 1) {
			if (report.id != 0) {
				if(report.initialLitres == 0.0) {
					printf("%d\t    %s    %.2f\t%.2f\tN/A\t\tN/A\t\tN/A\tN/A\tN/A\n", report.id, report.custN, report.initialDeposit, report.depositBal);
				} else {
					if(report.initialDeposit == 0.0) {
						printf("%d\t    %s    N/A\t\tN/A\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t%.2f\n", report.id, report.custN, report.initialLitres, report.litresBal, report.amountOwed, report.amountPaid, report.balance);
					}
				}
			}
		}
	}

	fclose(filePtr);
}

void FuelReport() {
	FILE *filePtr= NULL;
	Fuel report;
	float total= 0;

	filePtr = fopen("Fuel Tanks.txt", "r+");
	if(filePtr== NULL) {
		printf("\n\aError: File Not Opened");
	} else {
		printf("\nFile Opened!\n\n~~~~~Fuel Report~~~~~\n");
		while(fscanf(filePtr, "%s%f%f%f", report.fuelType, &report.amount, &report.sales, &report.purchasedLitres)!= EOF) {
			printf("\n%s sales= $%.2f", report.fuelType, report.sales );
			total= total + report.purchasedLitres;
		}
		printf("\nTotal Litres Purchased is %.2f L\n", total);
	}
	fclose(filePtr);
}