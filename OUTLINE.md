# Group Project


## Topic/Summary

The project is an object-oriented client/server messaging application. \
A client is able to connect to a server via a socket and send messages to other clients. \
The server is able to accept multiple clients and send messages to all connected clients. \
The server stores messages in a linked list and sends them to clients when they connect. \
A client is able to search for messages by keyword (linear search).

## Requirement Fulfillment
- [x] Algorithms
  - [x] Linear Search
- [x] Data Structures
  - [x] Linked List
- [x] Object-Oriented Programming
  - [x] Classes
  - [x] Encapsulation

## Stages of Development
- [x] PoC
  - [x] Server setup
    - [x] Open a socket to allow clients to connect
    - [x] Send a response message to the client
  - [x] Client setup
    - [x] Connect to server via socket
    - [x] Send a message to the server
    - [x] User able to send a message via stdin
- [ ] Phase 1
  - [ ] Server
    - [ ] Accept multiple clients
    - [ ] Store messages in a linked list
    - [ ] Send message history to clients upon connection
    - [ ] Send messages to all connected clients
- [ ] Phase 2
  - [ ] Client
    - [ ] Search for messages by keyword (Linear Search)

## UML Diagram

~~~mermaid
classDiagram
    class Server {
        -unsigned int port
        -int server_fd
        -LinkedList* list
        +Server()
        +Server(unsigned int)
        +int run()
        -void setupSocket()
        -void handleClient(int)
    }

    class LinkedList {
        -Node* head
        -Node* tail
        -int size
        +LinkedList()
        ~LinkedList()
    }

    class Node {
        -Node* next
        -Node* prev
        +Node()
    }

    class Message {
        -std::string timestamp
        -std::string content
        +Message(std::string, std::string)
        +std::string serialize() const
        +static Message deserialize(std::string)
    }

    class Client {
        -unsigned int port
        -int socket_fd
        +Client(unsigned int)
        +int run()
        -void setupConnection()
    }

    class Student {
    }
    class Staff {
    }

    class User {
      - std::string username
    }
    
    Client <|-- User
    User <|-- Student
    User <|-- Staff
  ~~~