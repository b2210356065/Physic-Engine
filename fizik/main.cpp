#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include "file.cpp"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;



int main() {
    file *f=new file;
    string path="";
    cout<<"Enter the input txt path:\n";
    cin>>path;
     f->readfile(path);

    string currentPath = fs::current_path().string();
    string outputPath = currentPath + "/output.txt";
    delete f;
    system(("start " + outputPath).c_str());
    return 0;
}
//"C:\\Users\\Lenovo\\Desktop\\fizik\\input.txt"

