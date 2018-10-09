#include <iostream>
#include <vector>

#include "get_options.hpp"

using namespace std;

int main() {
    cout << "Testing get_option..." << endl;

    vector<string> cargs = {
        "executable.exe",
        "-flag",
        "-i8", "-127",
        "-u8", "255",
        "-i16", "-32767",
        "-u16", "65535",
        "-i32", "-2147483647",
        "-u32", "4294967295",
        "-i64", "-9223372036854775807",
        "-u64", "18446744073709551615",
        "-f", "-0.2",
        "-d", "-0.2",
        "-ld", "-0.2",
        "-c", "character",
        "-s", "string",
        "-a", "1", "2", "3", "4", "5",
        "-v", "5", "4", "3", "2", "1"
    };

    vector<char*> args(cargs.size());
    for (unsigned i = 0; i < cargs.size(); ++i) {
        args[i] = new char[cargs[i].size() + 1];
        copy(cargs[i].begin(), cargs[i].end(), args[i]);
        args[i][cargs[i].size()] = '\0';
    }

    cout << "Arguments:";
    for (auto && el : args) cout << ' ' << el;
    cout << endl;

    int argc = args.size();
    char **argv = args.data();

    bool flag;
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    float f;
    double d;
    long double ld;
    char* c;
    string s;
    int *a = new int[5];
    vector<int> v(5);

    cout << "Check read... ";
    assert(get_flag(argv, argc, "-flag"));
    assert(get_args(argv, argc, "-i8", &i8));
    assert(i8 == -127);
    assert(get_args(argv, argc, "-u8", &u8));
    assert(u8 == 255);
    assert(get_args(argv, argc, "-i16", &i16));
    assert(i16 == -32767);
    assert(get_args(argv, argc, "-u16", &u16));
    assert(u16 == 65535);
    assert(get_args(argv, argc, "-i32", &i32));
    assert(i32 == -2147483647);
    assert(get_args(argv, argc, "-u32", &u32));
    assert(u32 == 4294967295);
    assert(get_args(argv, argc, "-i64", &i64));
    assert(i64 == -9223372036854775807);
    assert(get_args(argv, argc, "-u64", &u64));
    assert(u64 == 18446744073709551615);
    long double eps = 0.001;
    assert(get_args(argv, argc, "-f", &f));
    assert((f + 0.2) < eps);
    assert(get_args(argv, argc, "-d", &d));
    assert((d + 0.2) < eps);
    assert(get_args(argv, argc, "-ld", &ld));
    assert((ld + 0.2) < eps);
    assert(get_args(argv, argc, "-c", &c));
    assert(string(c) == string("character"));
    assert(get_args(argv, argc, "-s", &s));
    assert(s == string("string"));
    assert(get_args(argv, argc, "-a", a, 5));
    assert(a[0] == 1 && a[1] == 2 && a[2] == 3 && a[3] == 4 && a[4] == 5);
    assert(get_args(argv, argc, "-v", v.begin(), 5));
    assert(v[0] == 5 && v[1] == 4 && v[2] == 3 && v[3] == 2 && v[4] == 1);
    cout << "ok!" << endl;

    cout << "Check pend... ";
    char **pend;
    assert(get_flag(argv, argc, "-flag", &pend));
    assert(std::distance(argv, pend) == 1);
    assert(get_args(argv, argc, "-i8", &i8, 1, &pend));
    assert(std::distance(argv, pend) == 3);
    assert(get_args(argv, argc, "-u8", &u8, 1, &pend));
    assert(std::distance(argv, pend) == 5);
    assert(get_args(argv, argc, "-i16", &i16, 1, &pend));
    assert(std::distance(argv, pend) == 7);
    assert(get_args(argv, argc, "-u16", &u16, 1, &pend));
    assert(std::distance(argv, pend) == 9);
    assert(get_args(argv, argc, "-i32", &i32, 1, &pend));
    assert(std::distance(argv, pend) == 11);
    assert(get_args(argv, argc, "-u32", &u32, 1, &pend));
    assert(std::distance(argv, pend) == 13);
    assert(get_args(argv, argc, "-i64", &i64, 1, &pend));
    assert(std::distance(argv, pend) == 15);
    assert(get_args(argv, argc, "-u64", &u64, 1, &pend));
    assert(std::distance(argv, pend) == 17);
    assert(get_args(argv, argc, "-f", &f, 1, &pend));
    assert(std::distance(argv, pend) == 19);
    assert(get_args(argv, argc, "-d", &d, 1, &pend));
    assert(std::distance(argv, pend) == 21);
    assert(get_args(argv, argc, "-ld", &ld, 1, &pend));
    assert(std::distance(argv, pend) == 23);
    assert(get_args(argv, argc, "-c", &c, 1, &pend));
    assert(std::distance(argv, pend) == 25);
    assert(get_args(argv, argc, "-s", &s, 1, &pend));
    assert(std::distance(argv, pend) == 27);
    assert(get_args(argv, argc, "-a", a, 5, &pend));
    assert(std::distance(argv, pend) == 33);
    assert(get_args(argv, argc, "-v", v.begin(), 5, &pend));
    assert(std::distance(argv, pend) == 39);
    cout << "ok!" << endl;

    cout << "Check fail... ";
    assert(!get_flag(argv, argc, "-noexist", &pend));
    assert(std::distance(argv, pend) == argc);
    assert(!get_args(argv, argc, "-noexist", &i8, 1, &pend));
    assert(std::distance(argv, pend) == argc);
    assert(!get_args(argv, argc, "executable.exe", &i8, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &u8, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &i16, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &u16, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &i32, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &u32, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &i64, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &u64, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &f, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &d, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", &ld, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(get_args(argv, argc, "executable.exe", &c, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(get_args(argv, argc, "executable.exe", &s, 1, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", a, 5, &pend));
    assert(std::distance(argv, pend) == 1);
    assert(!get_args(argv, argc, "executable.exe", v.begin(), 5, &pend));
    assert(std::distance(argv, pend) == 1);
    cout << "ok!" << endl;

    for (auto && c : args) delete[] c;
    delete[] a;
    return 0;
}