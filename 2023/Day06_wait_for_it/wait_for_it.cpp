#include<iostream>
#include<fstream>

using namespace std;

long long int race_win_nr(long long int, long long int);

int main() {

    int total = 1;

    // int n = 3;
    // int time[3] = {7, 15, 30};
    // int dist[3] = {9, 40, 200};

    int n = 4;
    int time[4] = {42, 68, 69, 85};
    int dist[4] = {284, 1005, 1122, 1341};

    for (int i = 0; i < n; i++){
        total *= race_win_nr(time[i], dist[i]);
    }

    // cout << total;

    // cout << race_win_nr(71530, 940200);

    cout << race_win_nr(42686985, 284100511221341);

    return 0;
}

long long int race_win_nr(long long int time,long long int distance) {

    long long int total = 0, speed;

    for (int i = 0; i < time; i++) {
        speed = i;
        if((time - i) * speed > distance) {
            total++;
        }
    }

        return total;
}