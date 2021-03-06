#pragma once

// Convenience class for listening on a port and accepting sockets.
struct STCPServer : public STCPManager {

    // Stores information about a port
    struct Port {
        // Attributes
        int s;
        string host;
    };

    // Listens on a given port and accepts sockets, and shuts them down
    STCPServer(const string& host);

    // Destructor
    virtual ~STCPServer();

    // Begins listening on a new port
    Port* openPort(const string& host);

    // Closes all open ports
    void closePorts();

    // Tries to accept a new incoming socket
    Socket* acceptSocket(Port*& port);
    Socket* acceptSocket() {
        Port* ignore;
        return acceptSocket(ignore);
    }

    // Updates all managed ports and sockets
    void prePoll(fd_map& fdm);
    void postPoll(fd_map& fdm);

    // Attributes
    list<Port> portList;
};
