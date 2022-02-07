#include <iostream>
#include <math.h> 

using namespace std;

// we asuume that we have small letters.
string letter = "abcdefghijklmnopqrstuvwxyz0123456789A";

// get the position of the letter in letter.
int ASCII(char tmp)
{
    for(int i=0;i<letter.length();i++){
        if(letter[i]==tmp)
            return i+1;
    }
    return 0;
}

// this function converts a string to int.
int convert_to_int(string str)
{
    return stoi(str);
}

// this function give you reverse of a string that you give it as input.
string reverse(string str)
{
    string res = "";
    for (int i = str.length() - 1; i >= 0; i--)
        res += str[i];
    return res;
}

// hash function for converting a timestamp to a long long.
// idea : after every fieldes we will push a zero.
long long timestamp_hash(string str)
{
    long long res = 0;
    int i = 0;
    for (int c = 0; c < 3; c++)
    {
        while (str[i] != '/')
        {
            res *= 10;
            res += str[i] - '0';
            i++;
            if (i >= str.length())
            {
                break;
            }
        }
        if (c != 2)
            res *= 10;
        i++;
    }
    return res;
}

// inverse of timestamp hash function
// if you give me the hashed timestamp i will return the timestamp.
string timestamp_hash_inverse(long long Time)
{
    string time = to_string(Time);
    string year = time.substr(0, 4);
    // cout << "year : " << year << endl;
    string day = "";
    int i = 5;
    while (i < 7)
    {
        if (time[i] != '0')
        {
            day += time[i];
        }
        else if (time[i] == '0' && time[i + 1] == '0')
        {
            day += time[i];
            i++;
            break;
        }
        i++;
    }
    // cout <<" day :" << day << endl;

    i = time.length() - 1;
    string month = "";
    month += time[i];
    i--;
    if (time[i] != '0')
        month += time[i];

    return year + '/' + day + '/' + reverse(month);
}

// base convertor. convert decimal to base b.
void convert10tob(long long N, int b , string& ans)
{
    if (N == 0)
        return;

    long long x = N % b;
    N /= b;

    if (x < 0)
        N += 1;

    convert10tob(N, b, ans);
    
    x= x < 0 ? x + (b * -1) : x;
    ans= ans + to_string(x) + '-';
    return;
};

// this function hash the string that you give me it as input and will retun a long long as hash.
// idea : polynomial hash for strings. 
long long string_hash(string str)
{
    long long res = 0;
    for (int i = 1; i < str.length()-1; i++)
    {
        res += (pow(letter.length(), i) * ASCII(str[i]));
    }
    return res;
}

// if you give me the hashed string i will givr you the first string.
string string_hash_inverse(long long hashed)
{
    string res = "";
    string ans ="";
    convert10tob(hashed , letter.length(), ans);
    // cout << ans << endl;
    int i=0;
    while(i<ans.length()){
        string tmp="";
        while(ans[i]!='-'){
            tmp+=ans[i];
            i++;
        }
        res+=letter[stoi(tmp)-1];
        i++;    
    }
    
    return reverse(res);
}

// for testing these functions.
// int main()
// {

// //     // test timestamp hashing:
// //     string time;
// //     cin >> time;
// //     long long tmp  = timestamp_hash(time);
// //     cout << timestamp_hash(time);
// //     cout << timestamp_hash_inverse(tmp);

//     // test string hashing:
//     string t;
//     cin >> t;
//     cout << string_hash(t);
//     cout << endl;
//     cout << string_hash_inverse(string_hash(t));
//     // convert10tob(string_hash(t), letter.length());
//     cout << endl;
//     cout << letter.length();
// }
