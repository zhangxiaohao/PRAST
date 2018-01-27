#include "Client/Client.h"
#include "Server/Server.h"

vector<shared_ptr<ClientControl>> Clients;
ServerControl *Server;

void init() {
    Server = ServerControl::GetInstance();
    for(int i=0; i<CLIENTNUM; i++) {
        shared_ptr<ClientControl> client(new ClientControl(i));
        Clients.push_back(client);
    }
}

int main() {
    init();
    for(int i=0; i<10; i++) Clients[0]->ExecuteAnOpertation();
    //for(int i=0; i<10; i++) Clients[1]->ExecuteAnOpertation();
    cout << "Hello, World!" << endl;
    return 0;
}