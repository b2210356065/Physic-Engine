//
// Created by Lenovo on 27.06.2024.
//

#include <fstream>
#include <iostream>
#include <vector>
#include "physics.cpp"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class file {
private:
    string unicodes[21] = {
            "\u2588", // Full Block
            "\u2587", // Lower Seven Eighths Block
            "\u2586", // Lower Three Quarters Block
            "\u2585", // Lower Five Eighths Block
            "\u2584", // Lower Half Block
            "\u2583", // Lower Three Eighths Block
            "\u2582", // Lower One Quarter Block
            "\u2581", // Lower One Eighth Block
            "\u2580", // Upper Half Block
            "\u2593", // Dark Shade
            "\u2592", // Medium Shade
            "\u2591", // Light Shade
            "\u25A0", // Black Square
            "\u25A1", // White Square
            "\u25A3", // White Square Containing Black Small Square
            "\u25A4", // Square With Horizontal Fill
            "\u25A5", // Square With Vertical Fill
            "\u25A6", // Square With Orthogonal Crosshatch Fill
            "\u25A7", // Square With Upper Left to Lower Right Fill
            "\u25A8", // Square With Upper Right to Lower Left Fill
            "\u25A9", // Square With Diagonal Crosshatch Fill
    };
public:
    void readfile(basic_string<char> filepath) {
        ifstream file(filepath);
        if (!file.is_open()) {
            cout << "Dosya açılamadı: " << filepath << endl;
            return;
        }

        string line;
        physics *phys = new physics;
        vector<vector<int>> simulation_values;
        getline(file, line);
        int time = stoi(line);
        getline(file, line);
        phys->gravityx = stoi(line);
        getline(file, line);
        phys->gravityy = stoi(line);
        while (getline(file, line)) {
            string val;
            vector<int> linevalues;
            for (int i = 0; i < line.length(); ++i) {
                if (line[i] == ' ') {
                    int integer_val = stoi(val);
                    val = "";
                    linevalues.push_back(integer_val);
                } else {
                    val += line[i];
                }
            }
            int integer_val = stoi(val);
            linevalues.push_back(integer_val);
            simulation_values.push_back(linevalues);
        }
        file.close();
        phys->assignandrun(time, simulation_values);
        write(phys->space, simulation_values.size());
        delete phys;
    }

    void write(vector<vector<int>>& space, int l) {
        string currentPath = fs::current_path().string();
        string outputPath = currentPath + "/output.txt";

        ofstream outfile(outputPath);
        if (!outfile.is_open()) {
            cout << "Dosya açılamadı: " << outputPath << endl;
            return;
        }

        for (int i = 0; i < space.size(); i++) {
            for (int j = 0; j < space[i].size(); ++j) {
                outfile << unicodes[space[space.size()-1-i][j] % 21]<<unicodes[space[space.size()-1-i][j] % 21] << "";
            }
            outfile << "\n";
        }
        outfile << "\n\n";
        outfile.close();
    }
};