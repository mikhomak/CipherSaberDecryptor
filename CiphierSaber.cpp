#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


// helper function to transform hex string to array of uint8_t
vector<uint8_t> parse_hex(const string& input) {
  vector<uint8_t> output;
  output.resize(input.length() / 2);
  for (int i = 0; i < input.length(); i+=2) {
    output[i / 2] = stoi(input.substr(i, 2), 0, 16);
  }
  return output;
};



string RC4(int itterations, vector<uint8_t>& key_IV, vector<uint8_t>& parsed_message)
{

    string result; 
    // setting s box
    unsigned int s[256];
    for(int i = 0; i < 256; ++i)
    {
        s[i] = i;
    }
    
    // mixing values
    int j = 0;
    for(int i = 0; i < itterations; ++i)
    {
        for(int k = 0; k < 256; k++)
        {
            j = (j + s[k] + key_IV[k % key_IV.size()]) % 256;
            swap(s[k], s[j]);
        }
    }
    
    // xor the values from the message and s box
    j = 0;
    int ip = 0;
    for(int i = 0; i < parsed_message.size(); ++i)
    {
        ip = (i + 1) % 256;
        j = (j + s[ip]) % 256;
        swap(s[ip], s[j]);
        result += ((char)(parsed_message[i] ^ s[(s[ip] + s[j]) % 256]));
    }
    return result;
}


void decrypt(string& message, string& key, int itterations, int iv_length)
{
   // removing spaces in hex message just in case
    message.erase(remove(message.begin(), message.end(), ' '), message.end());

    // parsing the message from hex to bin
    vector<uint8_t> parsed_message = parse_hex(message); 
    // parsing the key to bin
    vector<uint8_t> parsed_key(key.begin(), key.end()); 
    
    // getting IV from the parsed message
    vector<uint8_t> parsed_iv;
    for(int i = 0; i < iv_length; i++)
    {
        parsed_iv.push_back(parsed_message[i]);
    }

   // removing IV from the parsed message
    parsed_message.erase(parsed_message.begin(), parsed_message.begin() + iv_length);

    // concatinating parsed_key + parsed_iv
    vector<uint8_t> key_iv;
    for(int i = 0; i < parsed_key.size(); ++i )
    {
        key_iv.push_back(parsed_key[i]);
    }
    for(int i = 0; i < iv_length; i++)
    {
        key_iv.push_back(parsed_iv[i]);
    }
    
    cout << RC4(itterations, key_iv, parsed_message) << "\n";
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    
    string s;
    string message;
    while(cin >> s)
    {
        if(s == "----")
        {
            break;
        }
        message += s;
    }

    
    int iv_length, itterations;
    cin >> iv_length;
    cin >> itterations;
    
    string key;
    cin >> key;
    decrypt(message, key, itterations, iv_length);
    return 0;
} 
