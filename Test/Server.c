#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 256

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;
    char username[BUF_SIZE], topic[BUF_SIZE], msg[BUF_SIZE];
    char buf[BUF_SIZE];
    
    // tạo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Error: Failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    // thiết lập địa chỉ server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // địa chỉ IP của broker
    serv_addr.sin_port = htons(1883); // cổng mặc định của MQTT

    // kết nối tới server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        printf("Error: Failed to connect to server\n");
        exit(EXIT_FAILURE);
    }

    // yêu cầu người dùng nhập tên và chủ đề
    printf("Enter your username: ");
    fgets(username, BUF_SIZE, stdin);
    username[strcspn(username, "\n")] = '\0'; // xóa kí tự xuống dòng
    printf("Enter the topic you want to chat on: ");
    fgets(topic, BUF_SIZE, stdin);
    topic[strcspn(topic, "\n")] = '\0'; // xóa kí tự xuống dòng
    
    // gửi yêu cầu đăng ký tới broker
    sprintf(buf, "SUBSCRIBE %s\n", topic);
    send(sockfd, buf, strlen(buf), 0);

    // vòng lặp chính
    while (1) {
        // yêu cầu người dùng nhập tin nhắn
        printf("[%s]: ", username);
        fgets(msg, BUF_SIZE, stdin);
        msg[strcspn(msg, "\n")] = '\0'; // xóa kí tự xuống dòng

        // kiểm tra xem người dùng có muốn thoát hay không
        if (strcmp(msg, "quit") == 0) {
            break;
        }

        // gửi tin nhắn tới broker
        sprintf(buf, "PUBLISH %s %s\n", topic, msg);
        send(sockfd, buf, strlen(buf), 0);
    }

    // đóng socket
    close(sockfd);
    return 0;
}
