#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

using namespace std;

vector<string> split_string(string);

// Complete the arrayManipulation function below.
long arrayManipulation(int n, vector<vector<int>> queries) {
    using namespace std;
    typedef std::map< long,int> Operations;
    typedef Operations::iterator Iterator;
    Operations operations;

    for ( auto q : queries ){
        Iterator it = operations.find( q[0] );

        if ( q[0] > n ) continue;

        if ( it != operations.end() ) {
            it->second = it->second + q[2];
        } else {
            operations.insert(std::make_pair(q[0],q[2]));
        }

        if ( q[1] + 1 > n ) continue;

        it = operations.find( q[1] + 1 );
        if ( it != operations.end() ) {
            it->second = it->second - q[2];
        } else {
            operations.insert(std::make_pair(q[1] + 1,0 - q[2]));
        }

    }

    int max =0;
    int running_sum=0;
    for ( long i = 1; i <= n ; ++i ){
        Iterator it = operations.find(i);
        if ( it != operations.end() ) {
            running_sum = running_sum + it->second;
            if ( running_sum > max ) max = running_sum;
        }
    }

    return max;

}

int main()
{

    string nm_temp;
    getline(cin, nm_temp);

    vector<string> nm = split_string(nm_temp);

    int n = stoi(nm[0]);
    int m = stoi(nm[1]);

    vector<vector<int>> queries(m);
    for (int i = 0; i < m; i++) {
        queries[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> queries[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    long result = arrayManipulation(n, queries);

    cout << result << "\n";

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y && x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
