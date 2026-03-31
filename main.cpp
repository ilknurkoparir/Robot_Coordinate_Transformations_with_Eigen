#include "transformations.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace Eigen;

// Helper: create 4x4 homogeneous transform from rotation and translation (meters)
Matrix4d makeTransform(const Matrix3d& R, const Vector3d& t) {
    Matrix4d T = Matrix4d::Identity();
    T.block<3,3>(0,0) = R;
    T.block<3,1>(0,3) = t;
    return T;
}

int main() {

    Vector3d t_S_B(0.026650, -0.148670, 0.159750);
    Matrix4d T_S_B = makeTransform(Matrix3d::Identity(), t_S_B);

    Vector3d t_O_P(-0.038689, 0.259150, -0.032485);
    Matrix4d T_O_P = makeTransform(Matrix3d::Identity(), t_O_P);


    Vector3d t_P_B(0.039000, 0.139800, 0.105650);
    Matrix4d T_P_B = makeTransform(Matrix3d::Identity(), t_P_B);


    Matrix4d inv_T_O_P = makeTransform(Matrix3d::Identity(), -t_O_P);
    Matrix4d inv_T_P_B = makeTransform(Matrix3d::Identity(), -t_P_B);
    Matrix4d T_S_O = transform(inv_T_O_P, transform(inv_T_P_B, T_S_B));

    std::cout << "T_S_O (sensor to marker pattern frame) in meters:\n" << T_S_O << std::endl;


    struct Entry {
        double time;
        std::string filename;
        Vector3d pos_m;   // metre
        Vector4d quat;    // (x, y, z, w)
    };

    std::vector<Entry> entries = {
        { 9.8,  "4527-pointcloud.xyz",
          Vector3d(3.25307, 1.32123, 2.02809),
          Vector4d(0.00864255, -0.970598, 0.00295932, 0.240533) },
        { 22.8, "4723-pointcloud.xyz",
          Vector3d(3.01444, 1.35715, 1.48149),
          Vector4d(-0.00204355, -0.999556, 0.00285325, 0.0295867) },
        { 25.8, "4768-pointcloud.xyz",
          Vector3d(2.32046, 1.37835, 1.58003),
          Vector4d(0.0142343, -0.999592, 0.00627709, 0.0239432) },
        { 27.8, "4798-pointcloud.xyz",
          Vector3d(1.53895, 1.37098, 1.60283),
          Vector4d(0.0185319, -0.999556, 0.00589044, 0.0225724) }
    };


    for (const auto& entry : entries) {
        // 5. T_O_W dönüşümünü oluştur
        Matrix3d R_O_W = getRotation(entry.quat);   
        Matrix4d T_O_W = makeTransform(R_O_W, entry.pos_m);

        // 6. T_S_W = T_O_W * T_S_O
        Matrix4d T_S_W = transform(T_O_W, T_S_O);

        std::string infile = entry.filename;
        std::string outfile = infile.substr(0, infile.find('.')) + "_w.xyz";

        std::ifstream fin(infile);
        if (!fin.is_open()) {
            std::cerr << "Hata: " << infile << " dosyasi acilamadi!" << std::endl;
            continue;
        }

        std::ofstream fout(outfile);
        if (!fout.is_open()) {
            std::cerr << "Hata: " << outfile << " dosyasi olusturulamadi!" << std::endl;
            continue;
        }

        double x, y, z;
        int pointCount = 0;
        while (fin >> x >> y >> z) {
            Vector3d p_s(x, y, z);
            Vector3d p_w = transform(T_S_W, p_s);
            fout << p_w(0) << " " << p_w(1) << " " << p_w(2) << "\n";
            pointCount++;
        }
        fin.close();
        fout.close();

        std::cout << "Islendi: " << infile << " -> " << outfile
                  << " (" << pointCount << " nokta)" << std::endl;
    }

    std::cout << "\nTum donusumler tamamlandi." << std::endl;
    return 0;
}