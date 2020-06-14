#include <sstream>
#include <bitset>
#include <chrono>
#include <unordered_set>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>
#include <exception>
#include <ctime>

using namespace std;

// Build individual dictionaries
void webster(string temp, vector<string> &dx, vector<string> &cx)
{
    unordered_set<string> bx{};
    string bc = "";
    for (uint32_t i = 0; i < temp.length();)
    {
        bc = "";

        // If the ">" or the " " (space) is more than 10 away
        // we just take 2 characters from the string
        if (temp.find(" ", i) > 6)
        {
            bc = temp.substr(i, 2);
            i += bc.length();
        }
        /*
        // If the ">" is closer, we take to it's position
        else if (temp.find(">", i) != string::npos && temp.find(">", i) < temp.find(" ", i))
        {
            bc = temp.substr(i, (temp.find(">", i) - i + 1));
            i += bc.length();
        }
        */
        // Elsewise, we use the " " (space) and go until
        // there isn't one within 5 characters.
        // Then we take the next char and we loop again
        else if (temp.find(" ", i) != string::npos)
        {
            bc += temp.substr(i, (temp.find(" ", i) - i + 1));
            i += bc.length();
            uint32_t t_i = i;
            while (temp.find(" ", t_i) - t_i + 1 < 3)
            {
                string xvx = temp.substr(t_i, (temp.find(" ", t_i) - t_i + 1));
                //    cout << "n" << flush;
                bc += xvx;
                t_i += xvx.length();
            }
            i = t_i;
            //cout << ";" << flush;
            bc += temp[i];
            i++;
        }
        if (bc.length() <= 1)
            bc = temp.substr(i, 2);
        cx.push_back(bc);
        uint32_t d = bx.size();
        bx.insert(bc);
        if (bx.size() > d)
            dx.push_back(bc);
    }
}

int main(int argc, char **argv)
{
    locale::classic();

    printf("\t╔════════════════╗");
    printf("\n╟╥╫─────╢  Kcraken v1.0  ║");
    printf("\n╟╜\t╟────────────────╢");
    printf("\n╟╥╫─────╢David Pulse, Jr.║");
    printf("\n\t╚════════════════╝\n\n");

    auto start = std::chrono::system_clock::now();
    // Start Timer
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
    cout << std::ctime(&start_time) << flush;

    if (argc < 4)
    {
        printf("Usage: \n\t%s -c <file input> <output>", argv[0]);
        printf("\n\t%s -d <file input> <output>", argv[0]);
        return 0;
    }

    ifstream  in_file(argv[2], ios::in | ios::binary);
    ofstream out_file(argv[3], ios::out | ios::binary);

    uint32_t sizeofall = 0;
    stringstream file_in;
    uint32_t i = 0, n = 120000;

    // load in_file
    file_in << in_file.rdbuf();

    string temp = file_in.str();

    cout << "\nOriginal File Size: "
         << temp.length() << " " << flush;
    file_in.str("");

    uint32_t loop_cnt = 0;
    // Make sure that we're going to loop
    if (strcmp("-c", argv[1]) == 0)
    {

        while (temp.length() > n * loop_cnt)
        {
            // sets up for dictionaries (dx)
            // and also for list of what is there (cx)
            vector<vector<string>> all_dictionaries{}, all_data{};
            vector<string> cx{}, dx{};
            if (n * loop_cnt + n < temp.length())
            {
                webster(temp.substr(loop_cnt * n, n), dx, cx);
                //temp = temp.substr(n);
                //cout << (n*loop_cnt + n) << " . ." << flush;
                loop_cnt++;
            }
            else // last rollout of a dictionary
            {
                webster(temp, dx, cx);
                temp.clear();
                cout << cx.size() << flush;
                loop_cnt++;
            }
            if (loop_cnt % 25 == 0 && temp.length() > n)
            {
                temp = temp.substr(loop_cnt * n + n);
                loop_cnt = 0;
            }

            cout << "." << flush;
            uint16_t cx_cnt = 0;
            cout << "$" << flush;

            {

                {

                    out_file << "%;%" << flush;
                    sizeofall += 3;
                    // output the dictionary
                    // for this round
                    for (string c : dx)
                    {
                        out_file << c.c_str();
                        sizeofall += sizeof(c.c_str());
                    }

                    out_file << "%;%" << flush;

                    sizeofall += 3;
                    string len = "";

                    // go through all of the combinations
                    // in the file
                    for (string t : cx)
                    {
                        uint32_t y = 0;
                        for (string b : dx) // compare cx to bx to get y
                        {
                            y++;
                            if (t == b)
                            {
                                string vz = "";
                                while (y > 0) // setup to output
                                {
                                    uint8_t v = y % 256;
                                    vz += (char)(v);
                                    y >>= 8;
                                }
                                if (vz.length() == 1)
                                    vz.push_back(';'); // bumpers
                                if (vz.length() == 3)
                                    vz.append(":"); //
                                len.append(vz);
                                break;
                            }
                        }
                    }
                    cout << "." << flush;
                    out_file << len.c_str(); // output

                }

            }
            out_file << "%;%";
        }
    }
    if (strcmp("-d", argv[1]) == 0)
    {
        while (3 < temp.length())
        {
            // read in dictionary
            if (temp.substr(0, 3) == "%;%")
                temp = temp.substr(3);
            string dictionary = temp.substr(0, temp.find("%;%"));
            string piece = "";
            cout << "." << flush;
            // make dictionary
            vector<string> bx = {};
            uint32_t len = 0;

            while (len < dictionary.length())
            {
                if (dictionary.find(">", i) - i + 1 > 6 && dictionary.find(" ", i) - i + 1 > 6)
                    piece = dictionary.substr(i, 2);
                else if (dictionary.find(" ", i) != string::npos)
                {
                    piece = dictionary.substr(i, (dictionary.find(" ", i) - i + 1));
                    while (dictionary.find(" ", i) - i <= 3)
                    {
                        piece += dictionary[i];
                        i++;
                    }
                    piece += dictionary[i];
                    i++;
                }
                bx.push_back(piece);
                len += piece.length();
            }
            temp = temp.substr(dictionary.length());
            cout << "." << flush;

            // read in data
            len = 0;
            if (temp.substr(0, 3) == "%;%")
            {
                temp = temp.substr(3);
                cout << "TT" << flush;
            }
            while (temp.substr(0, 3) != "%;%")
            {
                uint32_t chars = 0;
                if (temp.substr(0, 2) == "%&")
                {
                    out_file << "   ";
                    len = 2;
                    temp = temp.substr(len);
                    continue;
                }
                else if (temp.substr(3, 2) == "::")
                {
                    chars = (uint32_t)temp[2] << 0;
                    chars += (uint32_t)temp[1] << 8;
                    chars += (uint32_t)temp[0] << 16;
                    len = 2 + 3;
                }
                else if (temp.find(';') > 2)
                {
                    chars = (uint32_t)temp[1] << 0;
                    chars += (uint32_t)temp[0] << 8;
                    len = 2;
                }
                else
                {
                    chars = (uint32_t)temp[0];
                    len = 1 + 1;
                }
                out_file << bx.at((chars - 1) % bx.size());
                temp = temp.substr(len);
            }
        }
    }

    cout << "Output Size: " << sizeofall << flush;
    // output last of file
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapscurr_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "\nfinishcurr computation at " << std::ctime(&end_time)
              << "elapscurr time: " << elapscurr_seconds.count() << "s\n";

    return 0;
}
