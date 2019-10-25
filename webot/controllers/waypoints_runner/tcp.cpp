#include <sys/socket.h>

#include <iostream>

#include "tcp.hpp"
#include "client.hpp"

const std::string PDS_PORT = "4444";
const std::string PDS_ADDR = "127.0.0.1";

Server::Server(std::string id) : server(tcp::Server{0}){
    std::cerr << "Creating Server" << std::endl;
    robot_id = id;
    tcp::Client client{PDS_ADDR, PDS_PORT};
    client.send("addRobot," + id + "," + std::to_string(server.get_port()));
    client.close();
    std::cerr << "Robot " << id << "waiting for connection on port " << server.get_port() << std::endl;
    client_fd = server.accept();
}

Server::~Server(){
    server.close();
    // Deregister robot with the port discovery service.
    tcp::Client client{PDS_ADDR, PDS_PORT};
    client.send("removeRobot," + robot_id);
    client.close();
}

std::vector<Message> Server::get_message(){
    std::vector<std::string> raw_messages = server.receive(client_fd, MSG_DONTWAIT); 
    std::vector<Message> messages;
    for (std::string raw_message : raw_messages){
        MessageType messageType;
        size_t split_pos = raw_message.find(",");
        if (split_pos == std::string::npos){
            send_message(Message{raw_message, MessageType::not_understood});
            continue;
        }
        std::string type = raw_message.substr(0, split_pos);
        if (type == "get_position") {
            messageType = MessageType::get_position;
        } else if (type == "set_destination") {
            messageType = MessageType::set_destination;
        } else {
            send_message(Message{raw_message, MessageType::not_understood});
            continue;
        }
        messages.push_back(Message{raw_message.substr(split_pos + 1, raw_message.length()),
                                   messageType});
    }
    return messages;
}

void Server::send_message(Message message){
    std::string payload;
    switch (message.type) {
    case MessageType::get_position:
        payload = "get_position," + message.payload;
        break;
    case MessageType::set_destination:
        payload = "set_destination," + message.payload;
        break;
    case MessageType::not_understood:
        payload = "not_understood," + message.payload;
    }
    server.send(client_fd, payload);
}

