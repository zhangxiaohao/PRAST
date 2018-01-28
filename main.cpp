#include <unistd.h>
#include "Client/Client.h"
#include "Server/Server.h"

vector<shared_ptr<ClientControl>> Clients;
ServerControl *Server;
atomic<bool> done(false);
shared_ptr<std::thread> serverThread;

void init() {
    Server = ServerControl::GetInstance();
    for(int i=0; i<CLIENTNUM; i++) {
        shared_ptr<ClientControl> client(new ClientControl(i));
        Clients.push_back(client);
    }
}

void ServerWork() {
    while(!done) {
        Server->Work();
        usleep(100);
    }
}

void SetServer() {
    done = false;
    serverThread.reset(new thread(ServerWork));
}

void TEST1() {
    for(int i=0; i<10000; i++) Clients[0]->ExecuteAnOpertation();
    //for(int i=0; i<10; i++) Clients[1]->ExecuteAnOpertation();
    Clients[0]->OperationSync();
    Clients[1]->OperationSync();
}

void TEST2() {
    for(int i=0; i<10; i++) Clients[0]->ExecuteAnOpertation();
    for(int i=0; i<10; i++) Clients[1]->ExecuteAnOpertation();
    Clients[0]->OperationSync();
    Clients[1]->OperationSync();
}

void TEST3() {
    for(int i=0; i<10000; i++) Clients[0]->ExecuteAnOpertation();
    for(int i=0; i<10000; i++) Clients[0]->ExecuteAnOpertation();
    Clients[0]->OperationSync();
    Clients[1]->OperationSync();
}

///formal test///

void TEST1_1() {
    long long time_s[1000] = {0};
    for(int i=0; i<100000; i++) {
        int use_time = Clients[0]->ExecuteAnOpertation();
        time_s[i/100] += use_time;
    }
    double time_ss[10] = {0};
    for(int i=0; i<1000; i++) {
        time_ss[i/100] += time_s[i];
    }
    for(int i=0; i<10; i++) {
        time_ss[i] /= 100;
        cout << time_ss[i] << endl;
    }

}

void TEST1_2() {
    long long time_s[1000] = {0};
    for(int i=0; i<100000; i++) {
        int use_time = Clients[0]->ExecuteAnOpertation();
        if(i % 5000 == 0) Clients[0]->ReplicaSync();
        time_s[i/100] += use_time;
    }
    double time_ss[10] = {0};
    for(int i=0; i<1000; i++) {
        time_ss[i/100] += time_s[i];
    }
    for(int i=0; i<10; i++) {
        time_ss[i] /= 100;
        cout << time_ss[i] << endl;
    }
}

void startClient(int id) {
    for(int i=0; i<1000; i++) {
        Clients[id]->ExecuteAnOpertation();
        if(i % 5000 == 0) Clients[id]->ReplicaSync();
    }
    Clients[id]->OperationSync();
}

void TEST2_1() {
    std::thread *clients[CLIENTNUM];
    for(int i=0; i<CLIENTNUM; i++) {
        clients[i] = new thread(std::bind(startClient, i));
    }
    for(int i=0; i<CLIENTNUM; i++) {
        clients[i] -> join();
    }
}

void TEST3_1() {

}

void TEST3_2() {

}

int main() {
    init();
    timeval start, end;
    gettimeofday(&start, NULL);
    SetServer();
    TEST2_1();
    //end server
    sleep(1);
    done = true;
    serverThread->join();
    gettimeofday(&end, NULL);
    int use_time = 1000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000;
    double one_time = (double) use_time / CLIENTNUM / 1000;
    cout << 1000.0 / one_time << endl;
    cout << "All test finish!" << endl;
    return 0;
}