#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <chrono>
#include <random>
#include <thread>

#define MESSAGE_LENGTH 1024

using namespace std;

int create_data() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(20, 30);
    
    int randomNumber = dis(gen);

    return randomNumber;
}

void *receive_messages(void *arg) {
    int client_socket = *(int *)arg;
    char message[MESSAGE_LENGTH];

    while (1) {
        memset(message, 0, sizeof(message));
        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received > 0) {
            cout << "Received message: " << message << endl; //
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
    cout << "Enter your topic: ";
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
    
    while (1) {
    	
        
        cout << "Enter commander (auto -- maxium, quit): ";
        cin.getline(message, sizeof(message));

        // Check if user wants to quit
        
        if (strcmp(message, "auto") == 0){
        	int i = 0;
        	send(client_socket, message, strlen(message), 0);
        	while (i < 40){
			i++;
	    	 	int random_number = create_data();
			string message = to_string(random_number);
		 	cout << "Enter :" << random_number << endl;
			send(client_socket, message.c_str(), message.length(), 0);
			this_thread::sleep_for(chrono::seconds(5));
        	}
        }
        
        if (strcmp(message, "quit") == 0 || strcmp(message, "exit") == 0) {
            // Send termination signal to the server
            send(client_socket, message, strlen(message), 0);
            break;
        }

        // Send chat message to server
        send(client_socket, message, strlen(message), 0);
        
    }

    // Close the client socket
    close(client_socket);

    return 0;
}
