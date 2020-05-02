#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <bitset>
#include <sstream>
#include <chrono>
#include <locale>
#include <set>
#include <bits/stdc++.h>

using namespace std;

uint8_t MAX_CHARS_USED = 255;

// SIZE OF BYTE SEGMENTS
uint16_t interval = 0;
vector<bitset<8>> oxen = {};
uint64_t output_length = 0;
bool CURRENT_MARK = false;
string nextFold(string file_in, uint64_t &pos)
{

    vector<uint64_t> arr = {};
    arr.push_back(file_in.find((char)(128), pos));
    arr.push_back(file_in.find((char)(129), pos));
    arr.push_back(file_in.find((char)(130), pos));
    arr.push_back(file_in.find((char)(131), pos));
    arr.push_back(file_in.find((char)(132), pos));
    arr.push_back(file_in.find((char)(133), pos));
    arr.push_back(file_in.find((char)(134), pos));
    arr.push_back(file_in.find((char)(135), pos));
    arr.push_back(file_in.find((char)(136), pos));
    arr.push_back(file_in.find((char)(137), pos));

    uint8_t longest = 255;
    uint8_t order = 0;
    uint8_t checked = 0;
    for (uint8_t x : arr)
    {
        if (longest >= x)
        {
            longest = x;
            checked = order;
        }
        order++;
    }

    uint8_t tmp = pos;
    pos += longest + 1;
    return file_in.substr(tmp, arr[checked]);
}

uint64_t binaryTree(ofstream &out, string file_in, uint64_t iterated)
{

    if (iterated == 0)
    {
        output_length += 1;
        if (file_in.front() < 127)
        {

            if (CURRENT_MARK == false)
                out << (char)(140);
            out << file_in.front();
            CURRENT_MARK = true;
        }
        else
            out << file_in.front() % 128;
        
        return binaryTree(out, file_in, iterated + 1);
    }
    if (file_in.length() - iterated == 1)
    {
        output_length += 1;
        if (file_in.back() < 127)
        {
            if (CURRENT_MARK == false)
                out << (char)(160);
            out << file_in.back();
            CURRENT_MARK = true;
        }
        else {
            out << (char)(160);
            out << file_in.back() % 128;
            CURRENT_MARK = false;
        }
        return iterated + 1;
    }

    // RETURN IF FINISHED RECURSING
    if (iterated > file_in.length())
        return iterated - 1;
    if (iterated == file_in.length())
        return iterated;

    // COMPARING BYTES
    bitset<8> str_char0 = file_in[iterated];
    bitset<8> str_char1 = file_in[(iterated + 1)];

    // ASSUMING CHARS ARE EQUAL
    if (str_char0 == str_char1)
    {
        if (str_char0.to_ulong() < 127)
        {
            if (CURRENT_MARK == false)
                out << (char)(140);
            out << (char)(str_char0.to_ulong() % 128);
            output_length += 2;
            CURRENT_MARK = true;
        }
        else
        {
            CURRENT_MARK = false;
            out << (char)(140);
            //out << (char)(str_char0.to_ulong() % 128);
            output_length += 1;
        }
        return binaryTree(out, file_in, iterated + 2);
    }
    // ASSUMING LOWER NIBBLE IS SAME
    else if ((str_char0 ^ str_char1).to_ullong() << 4 > 0)
    {
        if (str_char0.to_ulong() < 127)
        {
            if (CURRENT_MARK == false)
                out << (char)(150);
            CURRENT_MARK = true;
            out << (char)(str_char0.to_ulong() % 128);
            output_length += 2;
        }
        else
        {
            CURRENT_MARK = false;
            out << (char)(str_char0.to_ulong() % 128);
            output_length += 1;
        }
        return binaryTree(out, file_in, iterated + 2);
    }
    // ASSUMING UPPER NIBBLE IS SAME
    else if ((str_char0 ^ str_char1).to_ullong() >> 4 == 0)
    {
        if (str_char0.to_ulong() < 127)
        {
            if (CURRENT_MARK == false)
                out << (char)(150);
            out << (char)(str_char0.to_ulong() % 128);
            output_length += 2;
            CURRENT_MARK = true;
        }
        else
        {
            CURRENT_MARK = false;
            out << (char)(str_char0.to_ulong() % 128);
            output_length += 1;
        }
        return binaryTree(out, file_in, iterated + 2);
    }
    // ASSUMING CHARS ARE gt/lt EACH OTHER
    // OR NOT EQUAL IN ANY SUCH WAY
    else
    {
        out << (char)(160);
        output_length += 1;
        return iterated;
    }
    return iterated;
}

void compressThis(ofstream &out, string file_in)
{

    auto y = file_in.begin();

    while (y != file_in.end())
    {
        string y_str = {};
        // PROBABLY NOT OPTIMZED
        while (y_str.length() < 1250 && y != file_in.end())
        {
            y_str.push_back(*y);
            y++;
        }
        // TRY TO CREATE BINARY TREE

        // FOR EACH UNEXAMINED BYTE, INSERT BYTE
        uint64_t total = 0;
        while (total < y_str.length())
        {
            total = binaryTree(out, y_str, total);
            int spa = 0;
            while (spa < 1 && total < y_str.length())
            {
                output_length++;
                out << (char)(y_str[total]);
                total++;
                spa++;
            }
        }
        out << (char)(255);
        output_length++;
    }
    cout << " Output File Size: " << output_length << flush;
}

void decompString(ofstream &out, uint8_t INSTRUCTION, string file_in)
{
    cout << " " << file_in << " " << flush;
    uint64_t len = 0;
    INSTRUCTION = 0;
    while (file_in.length() > len)
    {
        INSTRUCTION = file_in[len];
        len++;
        // START BYTE
        if (INSTRUCTION == 140)
        {
            out << file_in.substr(len, 1) << file_in.substr(len, 1);
            CURRENT_MARK = 0; 
            len++;
        }
        // END BYTE
        else if (INSTRUCTION == 150)
        {
            out << file_in.substr(len, 1) << file_in.substr(len, 1);
            len++;
        }
        // EQUAL
        else if (INSTRUCTION == 160)
        {
            out << file_in.substr(len, 1);
            len++;
        }
        else {
            out << (file_in[len] + 128);
            out << (file_in[len+1] + 128);
            len += 2;
        }
        if (file_in[len] == 255) {
            len++;
        }
    }
}

void decompLayers(string file_in, ofstream &out)
{

    while (file_in.length() > 0)
    {
        int n = 2;
        unsigned char xyz = file_in[0];
        string f_in = "";
        switch ((int)(file_in[0]))
        {
        case 255:
            f_in = to_string((char)(file_in[1] + 128) + (char)(file_in[2] + 128));
            decompString(out, xyz, f_in);
            n = f_in.length() + 1;
            break;
        case 254:
            f_in = to_string((char)(file_in[1] + 128) + (char)(file_in[2]));
            decompString(out, xyz, f_in);
            n = f_in.length() + 1;
            break;
        case 253:
            f_in = to_string((char)(file_in[1]) + (char)(file_in[2] + 128));
            decompString(out, xyz, f_in);
            n = f_in.length() + 1;
            break;
        case 252:
            f_in = to_string((char)(file_in[1]) + (char)(file_in[2]));
            decompString(out, xyz, f_in);
            n = f_in.length() + 1;
            break;
        case 251:
            decompString(out, xyz, file_in.substr(1, 1));
            n = 1;
            break;
        case 250:
            n = 1;
        }
        cout << (int)(uint8_t)file_in[0] << " ";
        file_in = file_in.substr(n);
    }
}

int main(int c, char *argv[])
{

    locale::classic();

    printf("\t╔════════════════╗");
    printf("\n╟╥╫─────╢  Kcraken v1.0  ║");
    printf("\n╟╜\t╟────────────────╢");
    printf("\n╟╥╫─────╢David Pulse, Jr.║");
    printf("\n\t╚════════════════╝\n\n");
    ifstream in;
    in.open(argv[2], ios::in | ios::binary);
    ofstream out;
    out.open(argv[3], ios::out | ios::binary);

    auto start = std::chrono::system_clock::now();
    // Start Timer
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    cout << std::ctime(&start_time) << flush;

    in.seekg(0, in.end);

    long long unsigned int length;
    length = in.tellg();
    in.seekg(0, in.beg);
    stringstream in_file;
    in_file << in.rdbuf();
    string file_pass = in_file.str();
    in_file.str("");

    cout << "\nOriginal File Size: "
         << file_pass.length() << " " << flush;
    //cout << endl << file_pass << endl << flush;
    if (strcmp("-c", argv[1]) == 0)
    {
        compressThis(out, file_pass); // String(out, file_pass);
    }
    else if (strcmp("-d", argv[1]) == 0)
        decompString(out, 'x', file_pass);

    // output last of file
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "\nfinished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}
