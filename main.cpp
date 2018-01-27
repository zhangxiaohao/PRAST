#include "Client/Client.h"
#include "Server/Server.h"

int main() {
    shared_ptr<ClientControl> Client(new ClientControl(1));
    ServerControl* Server = ServerControl::GetInstance();
    cout << "Hello, World!" << endl;
    return 0;
}