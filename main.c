#include <stdio.h>
#include <math.h>

/*
 * Struct chua toi uu:
 * Cac thanh phan co alignment khac nhau duoc sap xep xen ke.
 * Dieu nay co the lam phat sinh padding.
 */
struct SmartDeviceUnoptimized {
    char status;
    double voltage;
    char sensorType;
    float current;
    int operatingMinutes;
    double measuredPower;
};

/*
 * Struct da toi uu:
 * Sap xep cac thanh phan theo alignment giam dan.
 */
struct SmartDeviceOptimized {
    double voltage;
    double measuredPower;
    float current;
    int operatingMinutes;
    char status;
    char sensorType;
};

/*
 * Giai phap 2: Nested Struct
 * Nhom du lieu theo chuc nang.
 */
struct ElectricalData {
    double voltage;
    double measuredPower;
    float current;
};

struct OperationData {
    int operatingMinutes;
    char status;
    char sensorType;
};

struct SmartDeviceNested {
    struct ElectricalData electrical;
    struct OperationData operation;
};

int main() {
    struct SmartDeviceUnoptimized oldDevice;
    struct SmartDeviceOptimized device;

    struct SmartDeviceNested nestedDevice;

    double calculatedPower;
    double differencePercent;
    double energyKWh;

    /* ==========================================
       1. SO SANH KICH THUOC STRUCT
       ========================================== */

    printf("=== PHAN TICH KICH THUOC STRUCT ===\n");

    printf("sizeof(SmartDeviceUnoptimized) = %zu bytes\n",
           sizeof(oldDevice));

    printf("sizeof(SmartDeviceOptimized)   = %zu bytes\n",
           sizeof(device));

    printf("sizeof(SmartDeviceNested)      = %zu bytes\n",
           sizeof(nestedDevice));

    printf("\n");

    /* ==========================================
       2. NHAP DU LIEU
       ========================================== */

    printf("=== NHAP THONG SO THIET BI ===\n");

    printf("Dien ap U (V): ");
    scanf("%lf", &device.voltage);

    printf("Dong dien I (A): ");
    scanf("%f", &device.current);

    printf("Cong suat cam bien do duoc P (W): ");
    scanf("%lf", &device.measuredPower);

    printf("Thoi gian hoat dong (phut): ");
    scanf("%d", &device.operatingMinutes);

    printf("Trang thai (0-TAT, 1-BAT): ");
    scanf(" %c", &device.status);

    printf("Loai cam bien: ");
    scanf(" %c", &device.sensorType);

    /* ==========================================
       3. KIEM TRA DU LIEU DAU VAO
       ========================================== */

    if (device.voltage <= 0) {
        printf("\nLOI: Dien ap U phai lon hon 0.\n");
        return 0;
    }

    if (device.current < 0) {
        printf("\nLOI: Dong dien I khong duoc am.\n");
        return 0;
    }

    if (device.operatingMinutes < 0) {
        printf("\nLOI: Thoi gian hoat dong khong duoc am.\n");
        return 0;
    }

    if (device.status != '0' && device.status != '1') {
        printf("\nLOI: Trang thai chi duoc phep la 0 hoac 1.\n");
        return 0;
    }

    if (device.measuredPower < 0) {
        printf("\nLOI: Cong suat do duoc khong duoc am.\n");
        return 0;
    }

    /* ==========================================
       4. TINH CONG SUAT THUC TE
       ========================================== */

    calculatedPower = device.voltage * device.current;

    printf("\n=== KET QUA TINH CONG SUAT ===\n");

    printf("Cong suat Pcalc = U x I = %.2f W\n",
           calculatedPower);

    printf("Cong suat cam bien = %.2f W\n",
           device.measuredPower);

    /* ==========================================
       5. KIEM TRA SAI LECH CAM BIEN
       ========================================== */

    if (calculatedPower > 0) {

        differencePercent =
            fabs(device.measuredPower - calculatedPower)
            / calculatedPower * 100.0;

        printf("Do sai lech = %.2f%%\n",
               differencePercent);

        if (differencePercent > 5.0) {
            printf("CANH BAO: Cam bien cong suat bao sai lech.\n");
        } else {
            printf("Cam bien cong suat hoat dong trong nguong cho phep.\n");
        }

    } else {
        /*
         * Khi U > 0 va I = 0 thi Pcalc = 0.
         * Khong thuc hien phep chia cho 0.
         */
        printf("Pcalc = 0W, khong tinh phan tram sai lech.\n");
    }

    /* ==========================================
       6. KIEM TRA QUA TAI
       ========================================== */

    if (device.current > 30.0f ||
        device.measuredPower > 6600.0) {

        printf("\nCANH BAO: QUA TAI NGUY HIEM!\n");

        if (device.current > 30.0f) {
            printf("- Dong dien vuot qua 30A.\n");
        }

        if (device.measuredPower > 6600.0) {
            printf("- Cong suat tieu thu vuot qua 6600W.\n");
        }
    } else {
        printf("\nTrang thai tai dien: AN TOAN.\n");
    }

    /* ==========================================
       7. KIEM TRA RO RI DIEN
       ========================================== */

    if (device.status == '0' &&
        device.current > 0.05f) {

        printf("CANH BAO: RO RI DIEN HOAC TAI AN!\n");

    } else {
        printf("Kiem tra ro ri: KHONG PHAT HIEN BAT THUONG.\n");
    }

    /* ==========================================
       8. TINH DIEN NANG kWh
       ========================================== */

    energyKWh =
        device.measuredPower
        * (device.operatingMinutes / 60.0)
        / 1000.0;

    printf("\n=== DIEN NANG TIEU THU ===\n");

    printf("Thoi gian hoat dong: %d phut\n",
           device.operatingMinutes);

    printf("Dien nang tieu thu: %.4f kWh\n",
           energyKWh);

    /* ==========================================
       9. MINH HOA TRUY XUAT NESTED STRUCT
       ========================================== */

    nestedDevice.electrical.voltage = device.voltage;
    nestedDevice.electrical.current = device.current;
    nestedDevice.electrical.measuredPower = device.measuredPower;

    nestedDevice.operation.operatingMinutes =
        device.operatingMinutes;

    nestedDevice.operation.status =
        device.status;

    nestedDevice.operation.sensorType =
        device.sensorType;

    printf("\n=== MINH HOA NESTED STRUCT ===\n");

    printf("Dien ap: %.2f V\n",
           nestedDevice.electrical.voltage);

    printf("Dong dien: %.2f A\n",
           nestedDevice.electrical.current);

    printf("Cong suat: %.2f W\n",
           nestedDevice.electrical.measuredPower);

    printf("Thoi gian: %d phut\n",
           nestedDevice.operation.operatingMinutes);

    printf("Trang thai: %c\n",
           nestedDevice.operation.status);

    printf("\n=== KET THUC CHUONG TRINH ===\n");

    return 0;
}