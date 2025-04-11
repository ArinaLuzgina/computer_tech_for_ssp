#include <iostream>
#include <cmath>
#include <vector>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>

#include <thread>
#include <mutex>

using namespace std;

//Считал час 10

// Класс расчётной точки
class CalcNode
{
// Класс сетки будет friend-ом точки
friend class CalcMesh;

protected:
    // Координаты
    double x;
    double y;
    double z;
    double intensity;
    double image;

public:
    // Конструктор по умолчанию
    CalcNode() : x(0.0), y(0.0), z(0.0), intensity(0.0), image(0.0)
    {
    }

    // Конструктор с указанием всех параметров
    CalcNode(double x, double y, double z, double intensity, double image) 
            : x(x), y(y), z(z), intensity(intensity), image(image)
    {
    }
};

// Класс расчётной сетки
class CalcMesh
{
protected:
    // 2D-сетка из расчётных точек
    vector<vector<CalcNode>> points;
    double R_0 = 0;
    double I = 1;
    double lamb = 555 * 1e-9;
    double k = 2 * M_PI / lamb;
    double alpha = 0 / 180 * M_PI;
    double cos_alpha = cos(alpha);
    std::vector<std::vector<double>> choords = {{500, 450, -R_0}, {501, 550, -R_0}, {600, 450, -R_0 - 100}};
    unsigned int number_of_points = 100;
    double sum_intensity = 0;
    std::mutex mtx;

public:
    CalcMesh(unsigned int size, double h) {
        //std::cout"started calculate mesh" << std::endl;
        calculate_mesh(size, h);
        //std::cout"started mesh intensity" << std::endl;

        calculate_mesh_intensity(size, h);
        //calculate_mesh_image(size, h);
        choords = {{400, 450, -R_0 - 100}, {401, 550, -R_0 - 100}, {500, 450, -R_0}};
        calculate_mesh_intensity(size, h);

    }
    void calculate_mesh(unsigned int size, double h){
        points.resize(size);
        for(unsigned int i = 0; i < size; i++) {
            points[i].resize(size);
            for(unsigned int j = 0; j < size; j++) {
                points[i][j] = CalcNode(i * h, j * h, 0, 0, 0);
            }
        }
    }
    
    void calculate_mesh_intensity(unsigned int size, double h){
        points.resize(size);
        std::thread t1([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[0][0] << std::endl;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = 0; i < size/18; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t2([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[0][1] << std::endl;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 18; i < size/9; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
                }
            }
        });
        std::thread t3([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[0][2] << std::endl;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 9; i < size/6; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t4([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[1][0] << std::endl;

            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 6; i < size/9 * 2; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t5([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[1][1] << std::endl;

            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 9 * 2; i < size/18 * 5; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t6([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[1][2] << std::endl;

            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 18 * 5; i < size/3; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t7([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[2][0] << std::endl;

            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 3; i < size/18 * 7; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t8([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[2][1] << std::endl;

            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 18 * 7; i < size / 9 * 4; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t9([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            //std::coutstarting_points[2][2] << std::endl;

            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 9 * 4; i < size/ 2; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t10([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 2; i < size/9 * 5; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t11([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 9 * 5; i < size/18 * 11; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t12([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 18 * 11; i < size/3 * 2; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t13([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 3 * 2; i < size/18 * 13; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t14([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 18 * 13; i < size/9 * 7; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t15([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 9 * 7; i < size/6 * 5; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t16([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size /6 * 5; i < size/9 * 8; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t17([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 9 * 8; i < size / 18 * 17; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        std::thread t18([&](){
            mtx.lock();
            std::vector<std::vector<double>> starting_points = choords;
            int points_n = number_of_points;
            int R = R_0 * h;
            mtx.unlock();
            for(size_t p = 0; p != 3; p++){for(size_t s = 0; s != 3; s++){starting_points[p][s] *= h;}}
            for(unsigned int i = size / 18 * 17; i < size; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity += calculate_intensity_3d(pointX, pointY, starting_points, points_n, R);
            }
        }
        });
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();
        t10.join(); 
        t11.join(); 
        t12.join(); 
        t13.join(); 
        t14.join(); 
        t15.join(); 
        t16.join(); 
        t17.join(); 
        t18.join(); 
    }

    void calc_image(double start, double end, unsigned int size, double h){
        //mtx.lock();
        double sc = I * cos(k * R_0 / cos_alpha) / sum_intensity;
        //mtx.unlock();
        for(unsigned int i = size * start; i < size * end; i++) {
            points[i].resize(size);
            for(unsigned int j = 0; j < size; j++) {
                points[i][j].image = points[i][j].intensity * sc;
            }
        }
    }
    void calculate_mesh_image(unsigned int size, double h){
        points.resize(size);
        std::thread t1(&CalcMesh::calc_image, this, 0.0, 1/18, size, h);
        std::thread t2(&CalcMesh::calc_image, this,1/18, 2/18, size, h);
        std::thread t3(&CalcMesh::calc_image, this,2/18, 3/18, size, h);
        std::thread t4(&CalcMesh::calc_image, this, 3/18, 4/18, size, h);
        std::thread t5(&CalcMesh::calc_image, this, 4/18, 5/18, size, h);
        std::thread t6(&CalcMesh::calc_image, this, 5/18, 6/18, size, h);
        std::thread t7(&CalcMesh::calc_image, this, 6/18, 7/18, size, h);
        std::thread t8(&CalcMesh::calc_image, this, 7/18, 8/18, size, h);
        std::thread t9(&CalcMesh::calc_image, this, 8/18, 9/18, size, h);
        std::thread t10(&CalcMesh::calc_image, this, 9/18, 10/18, size, h);
        std::thread t11(&CalcMesh::calc_image, this, 10/18, 11/18, size, h);
        std::thread t12(&CalcMesh::calc_image, this, 11/18, 12/18, size, h);
        std::thread t13(&CalcMesh::calc_image, this, 12/18, 13/18, size, h);
        std::thread t14(&CalcMesh::calc_image, this, 13/18, 14/18, size, h);
        std::thread t15(&CalcMesh::calc_image, this, 14/18, 15/18, size, h);
        std::thread t16(&CalcMesh::calc_image, this, 15/18, 16/18, size, h);
        std::thread t17(&CalcMesh::calc_image, this, 16/18, 17/18, size, h);
        std::thread t18(&CalcMesh::calc_image, this, 17/18, 1, size, h);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();
        t10.join(); 
        t11.join(); 
        t12.join(); 
        t13.join(); 
        t14.join(); 
        t15.join(); 
        t16.join(); 
        t17.join(); 
        t18.join();
        
    }

    // Метод отвечает за запись текущего состояния сетки в снапшот в формате VTK
    void snapshot(unsigned int snap_number) {
        // Сетка в терминах VTK
        vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkSmartPointer<vtkStructuredGrid>::New();
        // Точки сетки в терминах VTK
        vtkSmartPointer<vtkPoints> dumpPoints = vtkSmartPointer<vtkPoints>::New();

        // Скалярное поле на точках сетки
        auto intensity = vtkSmartPointer<vtkDoubleArray>::New();
        intensity->SetName("intensity");

        auto image = vtkSmartPointer<vtkDoubleArray>::New();
        image->SetName("image");

        // Обходим все точки нашей расчётной сетки
        unsigned int number = (unsigned int)points.size();
        for(unsigned int i = 0; i < number; i++) {
            for(unsigned int j = 0; j < number; j++) {
                // Вставляем новую точку в сетку VTK-снапшота
                dumpPoints->InsertNextPoint(points[i][j].x, points[i][j].y, points[i][j].z);
                // И значение скалярного поля тоже
                intensity->InsertNextValue(points[i][j].intensity);
                image->InsertNextValue(points[i][j].image);
            }
        }

        // Задаём размеры VTK-сетки (в точках, по трём осям)
        structuredGrid->SetDimensions(number, number, 1);
        // Грузим точки в сетку
        structuredGrid->SetPoints(dumpPoints);

        // Присоединяем векторное и скалярное поля к точкам
        //structuredGrid->GetPointData()->AddArray(vel);
        structuredGrid->GetPointData()->AddArray(intensity);
        structuredGrid->GetPointData()->AddArray(image);

        // Создаём снапшот в файле с заданным именем
        string fileName = "./output/2d_em_in3d_upgrade/step-" + std::to_string(snap_number) + ".vts";
        vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(structuredGrid);
        writer->Write();
    }


    double calculate_intensity_3d(double x, double y, std::vector<std::vector<double>> starting_points, int points_n, double R_0){
        double I_res = 0.0;
        k = 2 * M_PI / (555 * 1e-9);
        //std::cout(starting_points[1][0] - starting_points[0][0]) / number_of_points << std::endl;
        for(double i=starting_points[0][0]; i <= starting_points[1][0]; i += (starting_points[1][0] - starting_points[0][0]) / points_n){
            for(double j=starting_points[0][0]; j <= starting_points[2][0]; j += (starting_points[2][0] - starting_points[0][0]) / points_n){
                double y_l = (starting_points[0][1] * (i - starting_points[1][0]) - starting_points[1][1] * (i - starting_points[0][0])) / (starting_points[0][0] - starting_points[1][0]);
                double y_r = (starting_points[0][1] * (j - starting_points[2][0]) - starting_points[2][1] * (j - starting_points[0][0])) / (starting_points[0][0] - starting_points[2][0]);
                double z_l = (starting_points[0][2] * (i - starting_points[1][0]) - starting_points[1][2] * (i - starting_points[0][0])) / (starting_points[0][0] - starting_points[1][0]);
                double z_r = (starting_points[0][2] * (j - starting_points[2][0]) - starting_points[2][2] * (j - starting_points[0][0])) / (starting_points[0][0] - starting_points[2][0]);
                double r_sq = pow(i + j - starting_points[0][0] - x, 2) + pow(y_l + y_r - starting_points[0][1] - y, 2) + pow(z_l + z_r - starting_points[0][2], 2);
                double delta = sqrt(r_sq) - R_0 / 1;
                I_res += 2 * I * (1 + cos(k * delta));
                // mtx.lock();
                // std::cout << i << " " << y_l + y_r - starting_points[0][1] << " " << y << std::endl;
                // mtx.unlock();
            }

        }
        return I_res;
    }
    double calculate_image(double intensity){
        double im = intensity / sum_intensity * I * cos(k * R_0 / cos_alpha);
        return im;
    }
    void zero_intense(unsigned int size, double h){
        points.resize(size);
        for(unsigned int i = 0; i < size; i++) {
            points[i].resize(size);
            for(unsigned int j = 0; j < size; j++) {
                points[i][j].intensity = 0.0;
            }
        }
    }
    void move(double time, double h) {
        R_0 = time; //update R_0
        choords[0][2] = -R_0;
        choords[1][2] = -R_0;
        choords[2][2] = -R_0 - 100;
        sum_intensity = 0;
        
    }
    void do_step(double time, double h, double size){
        move(time, h);
        zero_intense(size, h);
        choords = {{500, 450, -R_0}, {501, 550, -R_0}, {600, 450, -R_0 - 100}};
        calculate_mesh_intensity(size, h);
        choords = {{400, 450, -R_0 - 100}, {401, 550, -R_0 - 100}, {500, 450, -R_0}};
        calculate_mesh_intensity(size, h);
        //calculate_mesh_image(size, h);
    }
};

int main()
{
    unsigned int size = 1008;
    double h = 1e-6;

    // Создаём сетку заданного размера
    CalcMesh mesh(size, h);

    mesh.snapshot(0);
    int index = 1;
    for(float step = 0.1; step < 1; step+=0.1) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout<<step << std::endl;
    }
    for(float step = 1; step < 10; step+= 1) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout<<step << std::endl;

    }
    for(unsigned int step = 10; step < 2000; step+=10) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout<<step << std::endl;

    }
    // for(unsigned int step = 100; step < 500; step+=100) {
    //     mesh.do_step(step, h, size);
    //     mesh.snapshot(index);
    //     index ++;
    //     std::cout<<step << std::endl;

    // }
    // for(unsigned int step = 100; step < 1000; step+=10) {
    //     mesh.do_step(step, h, size);
    //     mesh.snapshot(index);
    //     index ++;
    //     std::cout << step << std::endl;

    // }


    return 0;
}
