# Distributed Systems Chat Tutorial

A simple C++ client-server chat application demonstrating basic message passing with enum-based message classification for a college assignment.

**Academic Project Details:**
- **Degree**: Software Engineering
- **University**: U-tad
- **Subject**: Distributed Systems Programming

Explores straightforward TCP message passing between clients and server, using enums to classify different message types.

## Key Features
- TCP socket-based client-server architecture.
- Simple chat message broadcasting to all connected clients.
- Enum-based message classification (chat, join, leave, etc.).
- Multi-client server handling.
- Command-line interface for sending/receiving messages.

## Tech Stack
- **Language**: C++17
- **Networking**: POSIX sockets
- **Build**: CMake & Makefile
- **Libraries**: Standard C++ libraries (threads, iostream, etc.)

## Setup & Run

```sh
# 1. Clone repo
git clone https://github.com/itzi97/DS_tutorial1_chat.git
cd DS_tutorial1_chat

# 2. Build with cmake
cmake .
make

# 3. Run server and client
./server
./client
```
