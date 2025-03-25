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

    // Метод отвечает за перемещение точки
    // Движемся время tau из текущего положения с текущей скоростью
    // void move(double tau) {
    //     x += vx * tau;
    //     y += vy * tau;
    //     z += vz * tau;
    // }
};

// Класс расчётной сетки
class CalcMesh
{
protected:
    // 2D-сетка из расчётных точек
    vector<vector<CalcNode>> points;
    double R0 = 1;
    double I = 1.0;
    double lamb = 555 * 1e-9;
    double k = 2 * M_PI / lamb;


public:
    // Конструктор сетки size x size точек с шагом h по пространству
    CalcMesh(unsigned int size, double h) {
        points.resize(size);
        double centerX = size * h / 2;
        double centerY = size * h / 2;


        for(unsigned int i = 0; i < size; i++) {
            points[i].resize(size);
            for(unsigned int j = 0; j < size; j++) {
                // Начальные координаты зададим равномерно в плоскости OXY
                double pointX = i * h;
                double pointY = j * h;
                double pointZ = 0;
                // Модельная скалярная величина распределена как-то вот так 
                double r_sq = pow((pointX - centerX), 2) + pow(pointY - centerY, 2);
                double intensity = 2 * I * (1 + cos(k * r_sq / (2 * R0)));
                double image = intensity - 2 * I * (1 + cos(k * R0));

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

        // Векторное поле на точках сетки


        // Обходим все точки нашей расчётной сетки
        unsigned int number = (unsigned int)points.size();
        for(unsigned int i = 0; i < number; i++) {
            for(unsigned int j = 0; j < number; j++) {
                // Вставляем новую точку в сетку VTK-снапшота
                dumpPoints->InsertNextPoint(points[i][j].x, points[i][j].y, points[i][j].z);

                // И значение скалярного поля
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
        string fileName = "./output/point_emitter.vts";
        vtkSmartPointer<vtkXMLStructuredGridWriter> writer = vtkSmartPointer<vtkXMLStructuredGridWriter>::New();
        writer->SetFileName(fileName.c_str());
        writer->SetInputData(structuredGrid);
        writer->Write();
    }
};

int main()
{
    // Размер расчётной сетки, точек на сторону
    unsigned int size = 1000;
    // Шаг точек по пространству
    double h = 1e-6;
    // Шаг по времени
    double tau = 0.01;

    // Создаём сетку заданного размера
    CalcMesh mesh(size, h);

    // Пишем её начальное состояние в VTK
    mesh.snapshot(0);



    return 0;
}
