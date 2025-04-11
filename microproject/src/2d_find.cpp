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
    double width = 25.2;//50.4;//100;//76.67; //100;
    double lenght = 25.2;//50.4; // 100;//76.67; //100;
    double x_st;
    double x_end;
    double y_st;
    double y_end;
    unsigned int number_of_points = 100;
    double sum_intensity = 0;

public:
    CalcMesh(unsigned int size, double h) {
        std::cout << "started calculate mesh" << std::endl;
        calculate_mesh(size, h);
        std::cout << "started mesh intensity" << std::endl;

        calculate_mesh_intensity(size, h);
        //calculate_mesh_image(size, h);
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
        R_0 *= h;
        double centerX = size * h / 2;
        double centerY = size * h / 2;
        x_st = centerX - width * h/ 2;
        y_st = centerY - lenght * h/ 2;
        x_end = centerX + width * h/ 2;
        y_end = centerY + lenght * h/ 2;       
        

        std::thread t1([&](){
            for(unsigned int i = 0; i < size/18; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
            }
        }
        });
        std::thread t2([&](){
            for(unsigned int i = size / 18; i < size/9; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
                }
        });
        std::thread t3([&](){
            for(unsigned int i = size / 9; i < size/6; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t4([&](){
            for(unsigned int i = size / 6; i < size /9 * 2; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t5([&](){
            for(unsigned int i = size / 9 * 2; i < size /18 * 5; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                    }
            }
        });
        std::thread t6([&](){
            for(unsigned int i = size / 18 * 5; i < size /3; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                    }
            }
        });
        std::thread t7([&](){
            for(unsigned int i = size /3; i < size /18 * 7; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                    }
            }
        });
        std::thread t8([&](){
            for(unsigned int i = size / 18 * 7; i < size /9 * 4; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t9([&](){
            for(unsigned int i = size / 9 * 4; i < size / 2; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t10([&](){
            for(unsigned int i = size / 2; i < size / 9 * 5; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t11([&](){
            for(unsigned int i = size / 9 * 5; i < size/18 * 11; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t12([&](){
            for(unsigned int i = size /18 * 11; i < size/3 * 2; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t13([&](){
            for(unsigned int i = size / 3 * 2; i < size / 18 * 13; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t14([&](){
            for(unsigned int i = size / 18 * 13; i < size / 9 * 7; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t15([&](){
            for(unsigned int i = size / 9 * 7; i < size / 6 * 5; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t16([&](){
            for(unsigned int i = size / 6 * 5; i < size / 9 * 8; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t17([&](){
            for(unsigned int i = size / 9 * 8; i < size / 18 * 17; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
                }
            }
        });
        std::thread t18([&](){
            for(unsigned int i = size / 18 * 17; i < size; i++) {
                points[i].resize(size);
                for(unsigned int j = 0; j < size; j++) {
                    // Начальные координаты зададим равномерно в плоскости OXY
                    double pointX = i * h;
                    double pointY = j * h;
                    points[i][j].intensity = calculate_intensity_2d(pointX, pointY);
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
        string fileName = "./output/2d_emitter_s504h4_56_25/step-" + std::to_string(snap_number) + ".vts";
        vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(structuredGrid);
        writer->Write();
    }

    double calculate_intensity_2d(double x, double y){
        double I_res = 0.0;
        for(double i=y_st; i < y_end; i += (y_end - y_st) / number_of_points){
            for(double j=x_st; j < x_end; j += (x_end - x_st) / number_of_points){
                double r_sq = pow(j - x, 2) + pow(i - y, 2);
                double delta = sqrt(r_sq + pow(R_0, 2)) - R_0 / cos_alpha;
                I_res += 2 * I * (1 + cos(k * delta));
            }
        }
        return I_res;
    }
    double calculate_image(double intensity){
        double im = intensity / sum_intensity * I * cos(k * R_0 / cos_alpha);
        return im;
    }
    void move(double time, double h) {
        R_0 = time; //update R_0
        sum_intensity = 0;
    }
    void do_step(double time, double h, double size){
        move(time, h);
        calculate_mesh_intensity(size, h);
        //calculate_mesh_image(size, h);
    }
};

int main()
{
    // unsigned int size = 1530;
    // double h = 1.5e-6;
    unsigned int size = 504;
    double h = 4.56e-6;

    // Создаём сетку заданного размера
    CalcMesh mesh(size, h);

    mesh.snapshot(0);
    int index = 1;
    for(float step = 0; step < 1; step+=0.1) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout << step << std::endl;
    }
    for(float step = 1; step < 10; step+= 1) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout << step << std::endl;

    }
    for(unsigned int step = 10; step < 100; step+=10) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout << step << std::endl;

    }
    for(unsigned int step = 100; step <= 1000; step+=100) {
        mesh.do_step(step, h, size);
        mesh.snapshot(index);
        index ++;
        std::cout << step << std::endl;

    }
    // for(unsigned int step = 500; step < 1000; step+=5) {
    //     mesh.do_step(step, h, size);
    //     mesh.snapshot(index);
    //     index ++;
    //     std::cout << step << std::endl;

    // }


    return 0;
}
