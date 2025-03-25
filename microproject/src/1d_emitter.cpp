#include <iostream>
#include <cmath>
#include <vector>

#include <vtkDoubleArray.h>
#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>

using namespace std;

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
    // Некая величина, в попугаях
    double intensity;
    double image;
    // Скорость

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
    double R_0 = 100;
    double I = 1.0;
    double lamb = 555 * 1e-9;
    double k = 2 * M_PI / lamb;
    double alpha = 50 / 180 * M_PI;
    double width = 0;
    double lenght = 100;
    double x_st;
    double x_end;
    double y_st;
    double y_end;
    unsigned int number_of_points = 50;


public:
    // Конструктор сетки size x size точек с шагом h по пространству
    CalcMesh(unsigned int size, double h) {
        points.resize(size);
        R_0 *= h;
        double centerX = size * h / 2;
        double centerY = size * h / 2;
        x_st = centerX - width * h/ 2;
        y_st = centerY - lenght * h/ 2;
        x_end = centerX + width * h/ 2;
        y_end = centerY + lenght * h/ 2;

        for(unsigned int i = 0; i < size; i++) {
            points[i].resize(size);
            for(unsigned int j = 0; j < size; j++) {
                // Начальные координаты зададим равномерно в плоскости OXY
                double pointX = i * h;
                double pointY = j * h;
                double pointZ = 0;
                // Модельная скалярная величина распределена как-то вот так 
                double intensity = calculate_intensity_1d(pointX, pointY);

                double image = calculate_image(pointX, pointY, intensity);
                // Профиль скорости по Z тоже взят какой-нибудь с потолка
                points[i][j] = CalcNode(pointX, pointY, pointZ, intensity, image);
            }
        }
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
        structuredGrid->GetPointData()->AddArray(intensity);
        structuredGrid->GetPointData()->AddArray(image);

        // Создаём снапшот в файле с заданным именем
        string fileName = "./output/1d_emitter.vts";
        vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(structuredGrid);
        writer->Write();
    }

    double calculate_angle(double x, double y, double x_from, double y_from){
        double r = sqrt(pow((x - x_from), 2) + pow((y - y_from), 2));
        double psi = atan(r / R_0);
        return psi;
    }
    double calculate_intensity_1d(double x, double y){
        double I_res = 0.0;
        for(double i=y_st; i <= y_end; i += (y_end - y_st) / number_of_points){
            for(double j=x_st; j <= x_end; j += (x_end - x_st) / number_of_points){
            //std::cout << i << " " << y << " " << x <<  std::endl;
            double r_sq = pow(j - x, 2) + pow(i - y, 2);
            double delta = sqrt(r_sq + pow(R_0, 2)) - R_0 * cos(alpha);
            I_res += 2 * I * (1 + cos(k * delta));
        }
    }
        return I_res;
    }
    double calculate_image(double x, double y, double intensity){
        double im = intensity - 2 * I * (1 + cos(k * R_0 * cos(alpha)));
        return im;
    }
};

int main()
{
    // Размер расчётной сетки, точек на сторону
    unsigned int size = 1000;
    // Шаг точек по пространству
    double h = 1e-6;
    // Шаг по времени

    // Создаём сетку заданного размера
    CalcMesh mesh(size, h);

    // Пишем её начальное состояние в VTK
    mesh.snapshot(0);

    return 0;
}
