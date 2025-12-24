


/*------------------------------------------Smart Parking Managment------------------------------*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_SLOTS 50
#define MAX_VEHICLE_NUM_LENGTH 20
#define SECONDS_IN_A_DAY 86400 // 24 hours

// ---- Fixed base charges ----
#define BUS_BASE     70
#define CAR_BASE     30
#define RIKSHA_BASE  15
#define TRUCK_BASE  110

// ---- Declarations ----
void menu();
void bus_auto();
void car_auto();
void riksha_auto();
void truck_auto();
void manual_booking(); 

void detail();
void delet_all();
void deleteSpecificVehicle();

int isValidVehicleNumber(const char* vehicleNumber);
int isVehicleExists(const char* vehicleNumber);
int isSlotAvailable(int slot);
int getNextFreeSlot();
void assignEntry(int type, const char* vehicleNumber, int slot, int charge, time_t when);
void printEntryTime(time_t entryTime);
void checkAndUpdateFees();
void clearScreen();

// ---- Globals ----
int nob = 0, noc = 0, nor = 0, noT = 0, count = 0;
int total_amount = 0;

int bus_slots[MAX_SLOTS], car_slots[MAX_SLOTS], riksha_slots[MAX_SLOTS], truck_slots[MAX_SLOTS];
int bus_pay[MAX_SLOTS], car_pay[MAX_SLOTS], riksha_pay[MAX_SLOTS], truck_pay[MAX_SLOTS];
char bus_numbers[MAX_SLOTS][MAX_VEHICLE_NUM_LENGTH], car_numbers[MAX_SLOTS][MAX_VEHICLE_NUM_LENGTH];
char riksha_numbers[MAX_SLOTS][MAX_VEHICLE_NUM_LENGTH], truck_numbers[MAX_SLOTS][MAX_VEHICLE_NUM_LENGTH];
time_t bus_entry_time[MAX_SLOTS], car_entry_time[MAX_SLOTS], riksha_entry_time[MAX_SLOTS], truck_entry_time[MAX_SLOTS];

// ------------------ Main ---------------------
int main() {
    menu();
    return 0;
}

void menu() {
    int ch;
    while (1) {
        checkAndUpdateFees();
        printf("\n========== SMART PARKING MANAGEMENT SYSTEM ==========\n");
        printf("1. Bus Entry \n2. Car Entry \n3. Riksha Entry \n4. Truck Entry \n");
        printf("5. Show All Details\n6. Delete Specific Vehicle\n7. Delete All Data\n");
        printf("8. Manual Booking (Choose Slot + Extra Payment)\n9. Exit\n");
        printf("=====================================================\n");
        printf("Enter your choice -> ");
        if (scanf("%d", &ch) != 1) { printf("Invalid input.\n"); exit(0); }

        switch (ch) {
            case 1: bus_auto(); break;
            case 2: car_auto(); break;
            case 3: riksha_auto(); break;
            case 4: truck_auto(); break;
            case 5: detail(); break;
            case 6: deleteSpecificVehicle(); break;
            case 7: delet_all(); break;
            case 8: manual_booking(); break;
            case 9: printf(" Exiting... Thank you!\n"); exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

// ------------------ Clear Screen ---------------------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ------------------ Utilities ---------------------
int isSlotAvailable(int slot) {
    for (int i = 0; i < nob; i++) if (bus_slots[i] == slot) return 0;
    for (int i = 0; i < noc; i++) if (car_slots[i] == slot) return 0;
    for (int i = 0; i < nor; i++) if (riksha_slots[i] == slot) return 0;
    for (int i = 0; i < noT; i++) if (truck_slots[i] == slot) return 0;
    return 1;
}

int getNextFreeSlot() {
    for (int s = 1; s <= MAX_SLOTS; s++) if (isSlotAvailable(s)) return s;
    return -1;
}

int isValidVehicleNumber(const char* vehicleNumber) {
    if ((int)strlen(vehicleNumber) != 13) return 0;
    if (isalpha(vehicleNumber[0]) && isalpha(vehicleNumber[1]) &&
        vehicleNumber[2] == ' ' &&
        isdigit(vehicleNumber[3]) && isdigit(vehicleNumber[4]) &&
        vehicleNumber[5] == ' ' &&
        isalpha(vehicleNumber[6]) && isalpha(vehicleNumber[7]) &&
        vehicleNumber[8] == ' ' &&
        isdigit(vehicleNumber[9]) && isdigit(vehicleNumber[10]) &&
        isdigit(vehicleNumber[11]) && isdigit(vehicleNumber[12])) return 1;
    return 0;
}

int isVehicleExists(const char* vehicleNumber) {
    for (int i = 0; i < nob; i++) if (strcmp(bus_numbers[i], vehicleNumber) == 0) return 1;
    for (int i = 0; i < noc; i++) if (strcmp(car_numbers[i], vehicleNumber) == 0) return 1;
    for (int i = 0; i < nor; i++) if (strcmp(riksha_numbers[i], vehicleNumber) == 0) return 1;
    for (int i = 0; i < noT; i++) if (strcmp(truck_numbers[i], vehicleNumber) == 0) return 1;
    return 0;
}

void printEntryTime(time_t entryTime) {
    struct tm *t = localtime(&entryTime);
    const char *days[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    printf("Entry Date: %02d-%02d-%04d | Time: %02d:%02d:%02d | Day: %s\n",
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
           t->tm_hour, t->tm_min, t->tm_sec, days[t->tm_wday]);
}

// Adds another day's base fee after 24 hours for each parked vehicle
void checkAndUpdateFees() {
    time_t now = time(NULL);
    for (int i = 0; i < nob; i++) {
        if (difftime(now, bus_entry_time[i]) >= SECONDS_IN_A_DAY) {
            total_amount += BUS_BASE; bus_entry_time[i] = now;
            printf(" Extra ₹%d added for Bus %s (24+ hrs)\n", BUS_BASE, bus_numbers[i]);
        }
    }
    for (int i = 0; i < noc; i++) {
        if (difftime(now, car_entry_time[i]) >= SECONDS_IN_A_DAY) {
            total_amount += CAR_BASE; car_entry_time[i] = now;
            printf("Extra ₹%d added for Car %s (24+ hrs)\n", CAR_BASE, car_numbers[i]);
        }
    }
    for (int i = 0; i < nor; i++) {
        if (difftime(now, riksha_entry_time[i]) >= SECONDS_IN_A_DAY) {
            total_amount += RIKSHA_BASE; riksha_entry_time[i] = now;
            printf(" Extra ₹%d added for Riksha %s (24+ hrs)\n", RIKSHA_BASE, riksha_numbers[i]);
        }
    }
    for (int i = 0; i < noT; i++) {
        if (difftime(now, truck_entry_time[i]) >= SECONDS_IN_A_DAY) {
            total_amount += TRUCK_BASE; truck_entry_time[i] = now;
            printf(" Extra ₹%d added for Truck %s (24+ hrs)\n", TRUCK_BASE, truck_numbers[i]);
        }
    }
}

// Core assign that records everything for one entry
void assignEntry(int type, const char* vehicleNumber, int slot, int charge, time_t when) {
    switch (type) {
        case 1: // Bus
            bus_slots[nob] = slot;
            strcpy(bus_numbers[nob], vehicleNumber);
            bus_pay[nob] = charge;
            bus_entry_time[nob] = when;
            nob++;
            break;
        case 2: // Car
            car_slots[noc] = slot;
            strcpy(car_numbers[noc], vehicleNumber);
            car_pay[noc] = charge;
            car_entry_time[noc] = when;
            noc++;
            break;
        case 3: // Riksha
            riksha_slots[nor] = slot;
            strcpy(riksha_numbers[nor], vehicleNumber);
            riksha_pay[nor] = charge;
            riksha_entry_time[nor] = when;
            nor++;
            break;
        case 4: // Truck
            truck_slots[noT] = slot;
            strcpy(truck_numbers[noT], vehicleNumber);
            truck_pay[noT] = charge;
            truck_entry_time[noT] = when;
            noT++;
            break;
    }
    total_amount += charge;
    count++;
    printf(" Slot %d assigned successfully! Charge: ₹%d\n", slot, charge);
    printEntryTime(when);
}

// ------------------ AUTO ENTRY FLOWS (Fixed base only) ---------------------
void bus_auto() {
    clearScreen();
    char vno[MAX_VEHICLE_NUM_LENGTH];
    printf("Enter Bus vehicle number (format: MP 45 HY 5698): ");
    scanf(" %[^\n]", vno);
    if (!isValidVehicleNumber(vno)) { printf(" Invalid format! Format: MP 45 HY 5698\n"); return; }
    if (isVehicleExists(vno)) { printf(" This vehicle already exists!\n"); return; }
    int slot = getNextFreeSlot();
    if (slot == -1) { printf(" Parking is full. No slots available.\n"); return; }
    assignEntry(1, vno, slot, BUS_BASE, time(NULL));
}

void car_auto() {
    clearScreen();
    char vno[MAX_VEHICLE_NUM_LENGTH];
    printf("Enter Car vehicle number (format: MP 45 HY 5698): ");
    scanf(" %[^\n]", vno);
    if (!isValidVehicleNumber(vno)) { printf(" Invalid format!\n"); return; }
    if (isVehicleExists(vno)) { printf(" Vehicle already exists!\n"); return; }
    int slot = getNextFreeSlot();
    if (slot == -1) { printf("Parking is full. No slots available.\n"); return; }
    assignEntry(2, vno, slot, CAR_BASE, time(NULL));
}

void riksha_auto() {
    clearScreen();
    char vno[MAX_VEHICLE_NUM_LENGTH];
    printf("Enter Riksha vehicle number (format: MP 45 HY 5698): ");
    scanf(" %[^\n]", vno);
    if (!isValidVehicleNumber(vno)) { printf(" Invalid format!\n"); return; }
    if (isVehicleExists(vno)) { printf(" Vehicle already exists!\n"); return; }
    int slot = getNextFreeSlot();
    if (slot == -1) { printf("Parking is full. No slots available.\n"); return; }
    assignEntry(3, vno, slot, RIKSHA_BASE, time(NULL));
}

void truck_auto() {
    clearScreen();
    char vno[MAX_VEHICLE_NUM_LENGTH];
    printf("Enter Truck vehicle number (format: MP 45 HY 5698): ");
    scanf(" %[^\n]", vno);
    if (!isValidVehicleNumber(vno)) { printf("Invalid format!\n"); return; }
    if (isVehicleExists(vno)) { printf(" Vehicle already exists!\n"); return; }
    int slot = getNextFreeSlot();
    if (slot == -1) { printf(" Parking is full. No slots available.\n"); return; }
    assignEntry(4, vno, slot, TRUCK_BASE, time(NULL));
}

// ------------------ MANUAL BOOKING FLOW ---------------------
void manual_booking() {
    clearScreen();
    int type, slot, extra;
    char vno[MAX_VEHICLE_NUM_LENGTH];

    printf("Vehicle Type? (1=Bus, 2=Car, 3=Riksha, 4=Truck): ");
    if (scanf("%d", &type) != 1 || type < 1 || type > 4) { printf(" Invalid type.\n"); return; }

    printf("Enter vehicle number (format: MP 45 HY 5698): ");
    scanf(" %[^\n]", vno);
    if (!isValidVehicleNumber(vno)) { printf("Invalid format! Format: MP 45 HY 5698\n"); return; }
    if (isVehicleExists(vno)) { printf(" This vehicle already exists!\n"); return; }

    printf("Enter preferred slot number (1–%d): ", MAX_SLOTS);
    if (scanf("%d", &slot) != 1) { printf("Invalid input.\n"); return; }
    if (slot < 1 || slot > MAX_SLOTS) { printf(" Invalid slot number.\n"); return; }
    if (!isSlotAvailable(slot)) { printf("Slot %d is already booked! Please choose another.\n", slot); return; }

    // Base + Extra logic
    int base = (type==1?BUS_BASE:(type==2?CAR_BASE:(type==3?RIKSHA_BASE:TRUCK_BASE)));
    printf("Fixed base charge for this vehicle: ₹%d\n", base);
    printf("Enter EXTRA amount to add (₹, 0 allowed): ");
    if (scanf("%d", &extra) != 1 || extra < 0) { printf("Invalid amount.\n"); return; }
    int total_for_entry = base + extra;

    assignEntry(type, vno, slot, total_for_entry, time(NULL));
}

// ------------------ DETAILS / DELETE ---------------------
void detail() {
    clearScreen();
    printf("\n================== PARKING DETAILS ==================\n");
    printf("| %-10s | %-15s | %-5s | %-20s | %-10s |\n", "Type", "Vehicle No", "Slot", "Entry Date-Time", "Amount");
    printf("-----------------------------------------------------\n");
    struct tm *t;
    for (int i = 0; i < nob; i++) {
        t = localtime(&bus_entry_time[i]);
        printf("| %-10s | %-15s | %-5d | %02d-%02d-%04d %02d:%02d | %-10d |\n",
               "Bus", bus_numbers[i], bus_slots[i],
               t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min, bus_pay[i]);
    }
    for (int i = 0; i < noc; i++) {
        t = localtime(&car_entry_time[i]);
        printf("| %-10s | %-15s | %-5d | %02d-%02d-%04d %02d:%02d | %-10d |\n",
               "Car", car_numbers[i], car_slots[i],
               t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min, car_pay[i]);
    }
    for (int i = 0; i < nor; i++) {
        t = localtime(&riksha_entry_time[i]);
        printf("| %-10s | %-15s | %-5d | %02d-%02d-%04d %02d:%02d | %-10d |\n",
               "Riksha", riksha_numbers[i], riksha_slots[i],
               t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min, riksha_pay[i]);
    }
    for (int i = 0; i < noT; i++) {
        t = localtime(&truck_entry_time[i]);
        printf("| %-10s | %-15s | %-5d | %02d-%02d-%04d %02d:%02d | %-10d |\n",
               "Truck", truck_numbers[i], truck_slots[i],
               t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min, truck_pay[i]);
    }
    printf("-----------------------------------------------------\n");
    printf("Total Vehicles: %d | Total Amount: ₹%d\n", count, total_amount);
    printf("=====================================================\n");

    char c;
    printf("Return to menu? (y/n): "); scanf(" %c", &c);
    if (c=='n'||c=='N'){printf("👋 Exiting... Thank you!\n"); exit(0);}
    clearScreen();
}

void deleteSpecificVehicle() {
    clearScreen();
    char vno[MAX_VEHICLE_NUM_LENGTH];
    printf("Enter vehicle number to delete: ");
    scanf(" %[^\n]", vno);
    int found=0;

    for(int i=0;i<nob;i++){
        if(strcmp(bus_numbers[i],vno)==0){
            total_amount -= bus_pay[i];
            for(int j=i;j<nob-1;j++){
                strcpy(bus_numbers[j],bus_numbers[j+1]);
                bus_slots[j]=bus_slots[j+1];
                bus_entry_time[j]=bus_entry_time[j+1];
                bus_pay[j]=bus_pay[j+1];
            }
            nob--; count--; found=1;
            printf("Bus %s deleted!\n",vno);
            break;
        }
    }
    for(int i=0;i<noc;i++){
        if(strcmp(car_numbers[i],vno)==0){
            total_amount -= car_pay[i];
            for(int j=i;j<noc-1;j++){
                strcpy(car_numbers[j],car_numbers[j+1]);
                car_slots[j]=car_slots[j+1];
                car_entry_time[j]=car_entry_time[j+1];
                car_pay[j]=car_pay[j+1];
            }
            noc--; count--; found=1;
            printf("Car %s deleted!\n",vno);
            break;
        }
    }
    for(int i=0;i<nor;i++){
        if(strcmp(riksha_numbers[i],vno)==0){
            total_amount -= riksha_pay[i];
            for(int j=i;j<nor-1;j++){
                strcpy(riksha_numbers[j],riksha_numbers[j+1]);
                riksha_slots[j]=riksha_slots[j+1];
                riksha_entry_time[j]=riksha_entry_time[j+1];
                riksha_pay[j]=riksha_pay[j+1];
            }
            nor--; count--; found=1;
            printf(" Riksha %s deleted!\n",vno);
            break;
        }
    }
    for(int i=0;i<noT;i++){
        if(strcmp(truck_numbers[i],vno)==0){
            total_amount -= truck_pay[i];
            for(int j=i;j<noT-1;j++){
                strcpy(truck_numbers[j],truck_numbers[j+1]);
                truck_slots[j]=truck_slots[j+1];
                truck_entry_time[j]=truck_entry_time[j+1];
                truck_pay[j]=truck_pay[j+1];
            }
            noT--; count--; found=1;
            printf("Truck %s deleted!\n",vno);
            break;
        }
    }
    if(!found) printf(" Vehicle not found.\n");
}

void delet_all() {
    nob=noc=nor=noT=count=total_amount=0;
    memset(bus_numbers,0,sizeof(bus_numbers)); memset(car_numbers,0,sizeof(car_numbers));
    memset(riksha_numbers,0,sizeof(riksha_numbers)); memset(truck_numbers,0,sizeof(truck_numbers));
    memset(bus_slots,0,sizeof(bus_slots)); memset(car_slots,0,sizeof(car_slots));
    memset(riksha_slots,0,sizeof(riksha_slots)); memset(truck_slots,0,sizeof(truck_slots));
    memset(bus_pay,0,sizeof(bus_pay)); memset(car_pay,0,sizeof(car_pay));
    memset(riksha_pay,0,sizeof(riksha_pay)); memset(truck_pay,0,sizeof(truck_pay));
    printf("🗑️  All records deleted successfully.\n");
}
