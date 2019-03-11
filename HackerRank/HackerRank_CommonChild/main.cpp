#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

// Complete the commonChild function below.
int commonChild(string s1, string s2) {
    
    vector< vector< int>> lcs;
    
    lcs.resize(s1.size()+1);
    for (auto& row : lcs ){
        row.resize(s2.size()+1);
    }
    
    /*
     lcs(i,j) =
     if ( s(i-1 ) == s(j-1) lcs(i-1,j-1) +1
     max(lcs(i-1,j), lcs(i,j-1)
     */
    
    for ( int i =0 ; i <= s1.size() ; ++i) {
        for ( int j =0 ; j <= s2.size() ; ++j ) {
            if ( i==0 || j==0 ) {
                lcs[i][j] =0;
            }
            else if ( s1[i-1] == s2[j-1])  {
                lcs[i][j] = lcs[i-1][j-1] + 1;
            }
            else {
                lcs[i][j] = max( lcs[i][j-1],lcs[i-1][j] );
            }
        }
    }
    
    return lcs[s1.size()][s2.size()];    
}

int main()
{
    
    string s1;
    getline(cin, s1);
    
    string s2;
    getline(cin, s2);
    
    int result = commonChild(s1, s2);
    
    cout << result << "\n";

    
    return 0;
}
