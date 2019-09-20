#include<iostream>

using namespace std;

string logo ="\n          \x1b[35;1mMMMMMMMMMMMMMMMMMMMMM\n          MMMMMMMMMMMMMMMMMMMMM\n          MMMMMMMMMMMMMMMMMMMMM\n          MMMMMMMMMMMMMMMMMMMMM\n\x1b[36;1mNNNNNNNN  \x1b[35mMMMMMMMMMMMMMMMMMMMMM\n\x1b[36;1mNNNNNNNN  \x1b[35mMMMMMMMMMMMMMMMMMMMMM\n\x1b[36;1mNNNNNNNN  \x1b[35mMMMMMMMMMMMMMMMMMMMMM\n\x1b[36;1mNNNNNNNN  \x1b[35mMMMMMMMMMMMMMMMMMMMMM\n\x1b[36;1mNNNNNNNN               \x1b[35mMMMM     \n\x1b[36;1mNNNNNNNNNNNNNNNNNNNNN    \x1b[35mMM      \n\x1b[36;1mNNNNNNNNNNNNNNNNNNNNN     \x1b[35mM      \n\x1b[36;1mNNNNNNNNNNNNNNNNNNNNN               \n\x1b[36;1mNNNNNNNNNNNNNNNNNNNNN           \n    \x1b[36;1mNNNN  \x1b[33;1m      _           _                                  \n    \x1b[36;1mNN    \x1b[33;1m  ___| |__   __ _| |_                               \n    \x1b[36;1mN     \x1b[33;1m / __| '_ \\ / _` | __|\n          \x1b[33;1m| (__| | | | (_| | |_ \n          \x1b[33;1m \\___|_| |_|\\__,_|\\__|\n\x1b[0m";

void init(){
    std::ios_base::sync_with_stdio(true);
    cout << logo;
}
