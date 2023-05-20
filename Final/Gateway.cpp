#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <algorithm>

#define MESSAGE_LENGTH 1024

using namespace std;

struct ClientInfo {
    int id;
    int socket;
    string topic;
};

    vector<ClientInfo> clients;
    map<string, vector<int>> topicSubscribers;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(const string& message, const string& topic, int sender_id) {
    pthread_mutex_lock(&mutex);

    // Iterate through the subscribers of the topic and send the message to each subscriber
    vector<int> subscribers = topicSubscribers[topic];
    for (int subscriber : subscribers) {
        if (subscriber != sender_id) {
            send(clients[subscriber].socket, message.c_str(), message.length(), 0);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *handle_client(void *arg) {
    int client_index = *(int *)arg;
    ClientInfo client = clients[client_index];
    char buffer[MESSAGE_LENGTH];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int recv_size = recv(client.socket, buffer, MESSAGE_LENGTH, 0);
        if (recv_size > 0) {
            // std::string message = "Room " + std::to_string(client.id) + ": " + buffer;
            string message =  buffer;
            // std::cout << "Received message: " << message << std::endl;

            // Broadcast the message to subscribers of the client's topic
            broadcast_message(message, client.topic, client_index);
        } else if (recv_size == 0) {
            cout << "Client " << client.id << " disconnected." << endl;

            // Remove the client from subscribers of the topic
            if (!client.topic.empty()) {
                pthread_mutex_lock(&mutex);
                vector<int>& subscribers = topicSubscribers[client.topic];
                subscribers.erase(remove_if(subscribers.begin(), subscribers.end(), [client_index](int index) {
    return index == client_index;
}), subscribers.end());
                pthread_mutex_unlock(&mutex);
            }

            break;
        } else {
            cout << "Error receiving message from client " << client.id << endl;
            break;
        }
    }

    // Remove the client from the clients vector
    pthread_mutex_lock(&mutex);
    clients.erase(clients.begin() + client_index);
    pthread_mutex_unlock(&mutex);

    free(arg);
    close(client.socket);

    return nullptr;
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in server, client;
    char buffer[MESSAGE_LENGTH];
    int opt = 1;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        return 1;
    }

    // Set socket options to reuse address and port
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Bind failed");
        return 1;
    }

    // Listen
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        return 1;
    }

    cout << "Server started. Waiting for connections..." << endl;

    int client_index = 0;
    while ((new_socket = accept(server_socket, (struct sockaddr *)&client, (socklen_t *)&client)) != -1) {
        

        // Receive client ID and topic
        int client_id;
        string topic;
        recv(new_socket, &client_id, sizeof(client_id), 0);
        recv(new_socket, buffer, MESSAGE_LENGTH, 0);
        topic = buffer;
	
	cout << "New client connected. Socket ID: " << client_id << endl;
	
        // Create a new client info
        ClientInfo new_client;
        new_client.id = client_id;
        new_client.socket = new_socket;
        new_client.topic = topic;

        // Add the new client to the clients vector
        pthread_mutex_lock(&mutex);
        clients.push_back(new_client);
        pthread_mutex_unlock(&mutex);

        // Add the new client to subscribers of the topic
        if (!topic.empty()) {
            pthread_mutex_lock(&mutex);
            topicSubscribers[topic].push_back(client_index);
            pthread_mutex_unlock(&mutex);
        }

        // Create a new thread to handle the client
        pthread_t thread_id;
        int *client_index_ptr = (int *)malloc(sizeof(int));
        *client_index_ptr = client_index;
        if (pthread_create(&thread_id, nullptr, handle_client, (void *)client_index_ptr) != 0) {
            perror("Failed to create thread");
            close(new_socket);
            return 1;
        }

        client_index++;
    }

    if (new_socket == -1) {
        perror("Accept failed");
        return 1;
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
