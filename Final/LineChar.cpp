#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <gnuplot-iostream.h>
#include <vector>

#define MESSAGE_LENGTH 1024

using namespace std;

double x_ = 0.0;
// Tạo dữ liệu
  vector<double> x;
  vector<double> y;


void *receive_messages(void *arg) {
    int client_socket = *(int *)arg;
    char message[MESSAGE_LENGTH];

Gnuplot gp;

    
    
    // Đặt tiêu đề cho trục x và y
  //gp << "set xlabel 'X'\n";
  //gp << "set ylabel 'Y'\n";
    while (1) {
        memset(message, 0, sizeof(message));
        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received > 0) {
            
            
            
// Dữ liệu điểm trên đồ thị
    int yy = stoi(message);
    /*
    for (char c : message) {
    	if (c >= '0' && c <= '9') {
    		yy*= 10;
    		yy += (int)c;
    	}
    }*/
    
    double y_ = yy + 0.0;
    x.push_back(x_);
    y.push_back(y_);
   cout << x_ << " " << y_ << endl;
    x_ += 1.0;
  // Vẽ sơ đồ line chart
  gp << "plot '-' with lines title 'Data'\n";
  gp.send1d(boost::make_tuple(x, y));
    
    
        } else if (bytes_received == 0) {
            cout << "Server disconnected." << endl;
            break;
        } else {
            cout << "Error receiving message." << endl;
            break;
        }
    }


    return nullptr;
}

int main() {
    int client_socket;
    struct sockaddr_in server;
    char message[MESSAGE_LENGTH];
    char server_ip[16];
    int server_port = 8888;

    cout << "Enter Server IP: ";
    cin.getline(server_ip, sizeof(server_ip));

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection error");
        return 1;
    }
    cout << "Connected successfully to server." << endl;

    // Send client ID and topic to server
    int client_id;
    string topic;
    cout << "Enter your ID: ";
    cin >> client_id;
    cin.ignore(); // Consume newline character
    cout << "Enter your topic (Room): ";
    cin.ignore(); // Consume newline character
    getline(cin, topic);
    send(client_socket, (char *)&client_id, sizeof(client_id), 0);
    send(client_socket, topic.c_str(), topic.length(), 0);

    // Create a separate thread to receive messages from the server
    pthread_t thread_id;
    if (pthread_create(&thread_id, nullptr, receive_messages, (void *)&client_socket) != 0) {
        perror("Failed to create thread");
        close(client_socket);
        return 1;
    }

    // Send chat messages to the server
    while(1){}
// Dừng chương trình để xem kết quả
  cout << "Press enter to exit." << endl;
  cin.get();
    // Close the client socket
    close(client_socket);

    return 0;
}
