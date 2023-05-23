# Lập trình mạng INT3304_20
# Báo cáo mô tả chương trình của nhóm 11

## Thành viên và phần trăm đóng góp
* Nông Ngọc Sơn – 20021430 - 30%
* Nguyễn Đức Chính – 20021306 - 17.5%
* Phan Anh Quân -  20020325 - 17.5%
* Đặng Việt Linh – 20021383 - 17.5%
* Đỗ Minh Quân - 20021414 - 17.5%

## Đề tài của nhóm
Tên đề tài: chương trình quản lý và theo dõi nhiệt độ phòng kho

## Hệ điều hành sử dụng
Linux (cụ thể hơn là trên Ubuntu)

## Ngôn ngữ mà chương trình sử dụng
C++

## Các tài nguyên cần cài đặt thêm
Người dùng chương trình phải cài thư viện gnuplot-iostream sử dụng terminal.
Ta làm điều đó bằng cách chạy câu lệnh sau để tải thư viện về Folder chứa chương trình: 
```bash
sudo apt-get install libgnuplot-iostream-dev
```

## Hướng dẫn cài đặt và sử dụng
Hướng dẫn cài đặt chi tiết sẽ có trong báo cáo ở trên. 
Dưới đây là tóm tắt sơ qua quá trình cài đặt:
* Cài đặt thư viện gnuplot-iostream bằng terminal
* Clone project về máy
* Biên dịch các file 
+ Với File Gateway:
```bash
g++ -o Gateway Gateway.cpp -pthread
```
+ Với File Node:
 ```bash
g++ -o Node Node.cpp -pthread
```
+ Với File Linechar
 ```bash
g++ -o LineChar LineChar.cpp -lboost_iostreams -pthread
```
* Chạy file đã biên dịch theo thứ tự Gateway -> Node -> Linechar
+ Cách chạy file đầu tiên: Dùng lệnh trong command line:
 ```bash
./Gateway 
```
 ```bash
./Node
```
 ```bash
./Linechar
```
+ Cách chạy file thứ 2: Double click vào phần biên dịch đã được in ra (Xem chi tiết ở trong Docs)

* Có thể tồn tại nhiều Node cũng như nhiều Linechar, nhưng chỉ có một Gateway!
