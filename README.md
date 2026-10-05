# Bài tập 03 - Phân tích Trade-off Struct Smart Home

## 1. Mục tiêu

Bài tập phân tích sự đánh đổi giữa việc sử dụng các biến rời rạc và việc gom nhóm dữ liệu bằng `struct`.

Nội dung chính:

* Phân tích Memory Alignment.
* Phân tích Padding trong C `struct`.
* Tối ưu thứ tự các thành phần trong `struct`.
* So sánh Flat Struct và Nested Struct.
* Thực hiện kiểm tra an toàn điện.
* Phát hiện sai lệch cảm biến công suất.
* Phát hiện quá tải.
* Phát hiện rò rỉ điện năng.
* Tính điện năng tiêu thụ theo kWh.

---

# 2. Phân tích Input / Output

## 2.1. Input

Chương trình nhận các thông số:

| Dữ liệu            | Kiểu     | Ý nghĩa                                    |
| ------------------ | -------- | ------------------------------------------ |
| `voltage`          | `double` | Điện áp U, đơn vị Volt                     |
| `current`          | `float`  | Dòng điện I, đơn vị Ampere                 |
| `measuredPower`    | `double` | Công suất do cảm biến đo được, đơn vị Watt |
| `operatingMinutes` | `int`    | Thời gian hoạt động, đơn vị phút           |
| `status`           | `char`   | `0`: Tắt, `1`: Bật                         |
| `sensorType`       | `char`   | Loại cảm biến                              |

---

## 2.2. Output

Chương trình xuất:

* Kích thước của struct chưa tối ưu.
* Kích thước của struct đã tối ưu.
* Kích thước của Nested Struct.
* Công suất tính toán `Pcalc`.
* Phần trăm sai lệch công suất.
* Cảnh báo sai lệch cảm biến nếu vượt 5%.
* Cảnh báo quá tải nếu dòng điện > 30A hoặc công suất > 6600W.
* Cảnh báo rò rỉ nếu thiết bị tắt nhưng dòng điện > 0.05A.
* Điện năng tiêu thụ theo kWh.

---

# 3. Phân tích Memory Alignment và Padding

Trong C, compiler có thể chèn các byte đệm giữa các thành phần của `struct` để đảm bảo từng kiểu dữ liệu được đặt tại địa chỉ phù hợp với yêu cầu alignment của hệ thống.

Ví dụ struct chưa tối ưu:

```c
struct SmartDeviceUnoptimized {
    char status;
    double voltage;
    char sensorType;
    float current;
    int operatingMinutes;
    double measuredPower;
};
```

Các kiểu dữ liệu có kích thước/alignment khác nhau được đặt xen kẽ.

Điều này có thể khiến compiler phải thêm padding.

---

# 4. Giải pháp 1 - Flat Struct Optimization

Struct được sắp xếp lại:

```c
struct SmartDeviceOptimized {
    double voltage;
    double measuredPower;
    float current;
    int operatingMinutes;
    char status;
    char sensorType;
};
```

Nguyên tắc:

```text
Kiểu có alignment lớn
        ↓
Kiểu có alignment nhỏ
```

Mục tiêu là giảm số lượng padding không cần thiết.

Ưu điểm:

* Truy xuất đơn giản.
* Không tạo thêm tầng dữ liệu.
* Dễ kiểm soát `sizeof`.
* Phù hợp với thiết bị nhúng cần tiết kiệm RAM.

---

# 5. Giải pháp 2 - Nested Struct

Dữ liệu được chia thành các nhóm chức năng.

## Nhóm dữ liệu điện

```c
struct ElectricalData {
    double voltage;
    double measuredPower;
    float current;
};
```

## Nhóm dữ liệu vận hành

```c
struct OperationData {
    int operatingMinutes;
    char status;
    char sensorType;
};
```

## Struct tổng

```c
struct SmartDeviceNested {
    struct ElectricalData electrical;
    struct OperationData operation;
};
```

Khi truy xuất:

```c
nestedDevice.electrical.voltage
```

hoặc:

```c
nestedDevice.operation.status
```

Ưu điểm:

* Dữ liệu được tổ chức rõ ràng.
* Tăng tính đóng gói.
* Dễ mở rộng khi hệ thống có thêm nhiều nhóm thông tin.

Nhược điểm:

* Truy xuất dài hơn.
* Có thể phát sinh padding giữa các struct con.
* Không đảm bảo `sizeof` nhỏ hơn Flat Struct.

---

# 6. Bảng so sánh hai giải pháp

| Tiêu chí                 | Flat Struct Optimization | Nested Struct  |
| ------------------------ | ------------------------ | -------------- |
| Tối ưu padding           | Tốt                      | Không đảm bảo  |
| Kiểm soát `sizeof`       | Dễ                       | Khó hơn        |
| Truy xuất dữ liệu        | Đơn giản                 | Nhiều tầng     |
| Tính đóng gói            | Trung bình               | Tốt            |
| Dễ mở rộng               | Khá                      | Tốt            |
| Độ phức tạp              | Thấp                     | Cao hơn        |
| Phù hợp RAM hạn chế      | Rất phù hợp              | Tùy trường hợp |
| Khả năng tổ chức dữ liệu | Trung bình               | Tốt            |

Lưu ý: Nested Struct không mặc định giúp tiết kiệm bộ nhớ. Kích thước cuối cùng vẫn phụ thuộc vào alignment và padding của từng struct.

---

# 7. Lựa chọn giải pháp

Đối với IoT Gateway có RAM hạn chế, lựa chọn **Flat Struct Optimization**.

Lý do:

1. Có thể kiểm soát trực tiếp thứ tự các thành phần.
2. Giảm padding trong trường hợp các kiểu dữ liệu có alignment khác nhau.
3. Truy xuất dữ liệu đơn giản.
4. Không phát sinh thêm tầng truy cập.
5. Phù hợp với yêu cầu tối ưu bộ nhớ của hệ thống nhúng.

Nested Struct vẫn hữu ích khi ưu tiên khả năng tổ chức và mở rộng dữ liệu.

---

# 8. Luồng xử lý

```text
Nhập dữ liệu
     |
     v
Kiểm tra điện áp
     |
     v
Kiểm tra dòng điện
     |
     v
Kiểm tra thời gian
     |
     v
Kiểm tra status
     |
     v
Tính Pcalc = U × I
     |
     v
Kiểm tra sai lệch > 5%
     |
     v
Kiểm tra I > 30A
hoặc P > 6600W
     |
     v
Kiểm tra status = 0
và I > 0.05A
     |
     v
Tính kWh
     |
     v
Xuất báo cáo
```

---

# 9. Kiểm tra dữ liệu đầu vào

## Điện áp

Điện áp phải:

```text
U > 0
```

Nếu:

```text
U <= 0
```

thì dữ liệu không hợp lệ.

## Dòng điện

Dòng điện phải:

```text
I >= 0
```

Nếu:

```text
I < 0
```

thì dữ liệu không hợp lệ.

## Thời gian

Thời gian phải:

```text
t >= 0
```

Nếu:

```text
t < 0
```

thì dữ liệu không hợp lệ.

## Trạng thái

Chỉ chấp nhận:

```text
0 = Tắt
1 = Bật
```

Nếu nhập giá trị khác thì chương trình dừng xử lý.

---

# 10. Tính công suất

Công suất thực tế được tính:

```text
Pcalc = U × I
```

Trong C:

```c
calculatedPower = device.voltage * device.current;
```

Các thành phần struct được truy xuất bằng toán tử `.`.

---

# 11. Kiểm tra sai lệch cảm biến

Công thức:

```text
Sai lệch (%) =
|Pmeasured - Pcalc|
------------------- × 100
       Pcalc
```

Nếu:

```text
Sai lệch > 5%
```

chương trình in:

```text
CANH BAO: Cam bien cong suat bao sai lech.
```

Nếu `Pcalc = 0`, chương trình không thực hiện phép chia để tránh chia cho 0.

---

# 12. Kiểm tra quá tải

Hệ thống cảnh báo khi:

```text
I > 30A
```

hoặc:

```text
P > 6600W
```

Điều kiện trong C:

```c
if (device.current > 30.0f ||
    device.measuredPower > 6600.0)
```

Khi xảy ra một trong hai điều kiện, chương trình in:

```text
CANH BAO: QUA TAI NGUY HIEM!
```

---

# 13. Kiểm tra rò rỉ điện

Nếu thiết bị đang tắt:

```text
status = 0
```

nhưng vẫn có dòng điện:

```text
I > 0.05A
```

thì hệ thống cảnh báo:

```text
CANH BAO: RO RI DIEN HOAC TAI AN!
```

Điều kiện:

```c
if (device.status == '0' &&
    device.current > 0.05f)
```

---

# 14. Tính điện năng

Công thức:

```text
kWh = P × (t / 60.0) / 1000
```

Trong đó:

* `P`: Watt.
* `t`: phút.
* `60.0`: chuyển phút sang giờ.
* `1000`: chuyển Watt-hour sang kWh.

Trong C:

```c
energyKWh =
    device.measuredPower
    * (device.operatingMinutes / 60.0)
    / 1000.0;
```

Sử dụng `60.0` thay vì `60` để phép chia được thực hiện trên số thực, tránh lỗi chia nguyên.

---

# 15. Kiểm thử

## Test Case 1 - Dữ liệu bình thường

Input:

```text
U = 220V
I = 10A
P = 2200W
t = 120 phút
status = 1
```

Kết quả:

```text
Pcalc = 220 × 10 = 2200W
Sai lệch = 0%
Không quá tải
Không rò điện
kWh = 2200 × (120 / 60) / 1000
    = 4.4 kWh
```

---

## Test Case 2 - Sai lệch cảm biến

Input:

```text
U = 220V
I = 10A
P = 2000W
t = 120 phút
status = 1
```

Công suất tính toán:

```text
Pcalc = 2200W
```

Sai lệch:

```text
|2000 - 2200| / 2200 × 100
≈ 9.09%
```

Kết quả:

```text
CANH BAO: Cam bien cong suat bao sai lech.
```

---

## Test Case 3 - Quá tải

Input:

```text
U = 220V
I = 35A
P = 7700W
t = 60 phút
status = 1
```

Kết quả:

```text
CANH BAO: QUA TAI NGUY HIEM!
```

Do:

```text
35A > 30A
```

và:

```text
7700W > 6600W
```

---

## Test Case 4 - Rò điện

Input:

```text
U = 220V
I = 0.1A
P = 10W
t = 60 phút
status = 0
```

Kết quả:

```text
CANH BAO: RO RI DIEN HOAC TAI AN!
```

Vì:

```text
status = 0
```

nhưng:

```text
0.1A > 0.05A
```

---

## Test Case 5 - Dữ liệu không hợp lệ

Input:

```text
U = -220V
```

Kết quả:

```text
LOI: Dien ap U phai lon hon 0.
```

Chương trình dừng và không thực hiện các phép tính tiếp theo.

---

# 16. Biên dịch

Sử dụng GCC:

```bash
gcc -Wall -Wextra -std=c11 main.c -o main
```

Nếu môi trường GCC yêu cầu liên kết thư viện toán học:

```bash
gcc -Wall -Wextra -std=c11 main.c -o main -lm
```

Chạy:

```bash
./main
```

Trên Windows:

```cmd
main.exe
```

---

# 17. Kết luận

Bài tập đã triển khai hai hướng tái cấu trúc:

1. Flat Struct Optimization.
2. Nested Struct.

Flat Struct phù hợp hơn với mục tiêu tối ưu RAM trên IoT Gateway vì cho phép kiểm soát trực tiếp thứ tự các thành phần và padding.

Nested Struct có ưu thế về tính tổ chức, đóng gói và khả năng mở rộng nhưng không đảm bảo giảm `sizeof`.

Chương trình đồng thời đáp ứng các yêu cầu nghiệp vụ về:

* Kiểm tra dữ liệu cảm biến.
* Tính công suất.
* Phát hiện sai lệch công suất trên 5%.
* Cảnh báo quá tải.
* Phát hiện rò rỉ điện năng.
* Tính điện năng tiêu thụ kWh.
* Truy xuất dữ liệu bằng toán tử `.`.
* Không sử dụng mảng struct.
* Không sử dụng hàm tự định nghĩa.
