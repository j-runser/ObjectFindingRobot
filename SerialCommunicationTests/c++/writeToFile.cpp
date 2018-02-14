#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    string str;
    int command = 0;
    ofstream outfile;
    outfile.open("/dev/ttyACM0");

    while(true) {

        cout << "Enter 1 or 0: ";
        cin >> command;

        if(command == 10) {
            break;
        }

        outfile << command;
        usleep(1000000);

    }

    outfile.close();
    return 0;

}
