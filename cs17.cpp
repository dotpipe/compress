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

uint64_t analyze(ofstream &out, string file_in, uint64_t iterated);
void compressThis(ofstream &out, uint64_t int_, vector<int8_t> file_in);

uint8_t MAX_CHARS_USED = 255;

// SIZE OF BYTE SEGMENTS
uint16_t interval = 0;
uint64_t output_length = 0;
bool CURRENT_MARK = false;

void nop() {}

uint64_t analyze(ofstream &out, string file_in, uint64_t iterated)
{
    stringstream decx;
    int8_t dsa = 127;
    bool dgh = false;
    vector<int8_t> fgh{};
    int64_t gfh = 0;
    fgh.push_back(file_in[0]);
    for (int8_t x : file_in)
    {
        dsa = dsa - x;
        fgh.push_back(dsa);
        gfh += (dsa);
    }

    bool hgd = false;
    uint8_t dx = ((float)(gfh) / (float)(file_in.size()));
    decx << (char)(dx);
    int8_t dy = 0;
    uint64_t totals = 0;
    uint8_t hdx = 0;
    dsa = 255;
    uint8_t z = 0;
    while (pow(2, ++z) < dx)
        ;

    for (int8_t x : fgh)
    {
        uint8_t c = 1;
        dsa = dsa - x;
        if (dsa > dx)
            totals <<= 1;
        else if (dsa < dx)
        {
            totals = (totals << 2) + 2;
            c = 2;
        }
        else if (dsa == dx)
        {
            totals = (totals << 3) + 1;
            c = 3;
        }
        if (dsa != dx) {
            totals = (dsa > dx) ? (totals << z) + dsa : (totals << z) + dsa;
            hdx += z;
        }
        else
        {
            hdx += c + 1;
        }
        
        if (hdx >= 64 - z - 2)
        {
            while (totals > 0)
            {
                decx << (char)(uint8_t)(totals % 256);
                totals >>= 8;
            }
            hdx = 0;
            dsa = 255;
        }
    }
    while (totals > 0)
    {
        decx << (char)(uint8_t)(totals % 256);
        totals >>= 8;
    }

    out.write(decx.str().c_str(), decx.str().length());
}

void decompString(ofstream &out, uint8_t INSTRUCTION, string file_in)
{
    stringstream xma;
    uint64_t str_len = 0;
    INSTRUCTION = 0;
    //cout << file_in.substr(0,20) << flush;
    string school = file_in.substr(3, file_in.find("XMA") - 3);
    uint64_t file_len = strtoull(school.c_str(), NULL, 10);

    string unhash = "", reduced = "";
    uint64_t c = school.length() + 6, sezi = 0, total = 0;

    while (total < file_len)
    {
        if (file_in.substr(c + 1, 3) == "{)}")
        {
            unhash = unhash + file_in.substr(c, 1);
            c += 4;
            total++;
        }
        else if (file_in.substr(c - 3, 3) == "{)}" && file_in.substr(c, 3) == "hGf")
        {
            cout << "." << flush;
            unhash = unhash + file_in.substr(c + 3, 1);
            total++;
            c += 4;
            stringstream x;
            x << unhash;
            out.write(x.str().c_str(), x.str().length());
            unhash.clear();
        }
        else if (file_in.substr(c + 2, 3) == "FHg")
        {
            unhash = unhash + file_in.substr(c, 1) + file_in.substr(c + 1, 1);
            total += 2;
            c += 5;
        }
        else
        {
            unhash = unhash + file_in.substr(c, 1) + file_in.substr(c, 1);
            total += 2;
        }
    }
    stringstream x;
    x << unhash;
    out.write(x.str().c_str(), x.str().length());
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
        stringstream xma;
        xma << (const char *)("MXA") << to_string(file_pass.length()).c_str() << (const char *)("XMA");
        out.write(xma.str().c_str(), xma.str().length());
        xma.str("");
        uint64_t fx = 250 * 100, xf = 0;
        //while ((fx * xf) < file_pass.length())
        {
            analyze(out, file_pass, 0);
        }
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
