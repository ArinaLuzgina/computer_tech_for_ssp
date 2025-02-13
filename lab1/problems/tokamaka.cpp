#include <set>
#include <cmath>
#include <gmsh.h>

int main(int argc, char **argv)
{
  gmsh::initialize();
  gmsh::model::add("tokamaka");

  double R_e = 2.0;
  double r_e = 0.9;
  double r_i = 0.5;
  double lc = 1e-2;

  int tor_out_tag = gmsh::model::occ::addTorus(0, 0, 0, R_e, r_e, 1);
  int tor_in_tag = gmsh::model::occ::addTorus(0, 0, 0, R_e, r_i, 2);

  std::vector<std::pair<int, int>> objects = {{3, tor_out_tag }}; // Внешний тор
  std::vector<std::pair<int, int>> tools = {{3,  tor_in_tag }};   // Внутренний тор для вычитания

  // Вектор для хранения результатов операции
  std::vector<std::pair<int, int> > outDimTags;
  std::vector<std::vector<std::pair<int, int> > > outDimTagsMap;

  // Выполнение булевой операции вычитания: внешний тор минус внутренний тор
  gmsh::model::occ::cut(objects, tools, outDimTags, outDimTagsMap);
  gmsh::model::mesh::setSize(outDimTags, lc); // Примерный размер элементов

  gmsh::model::occ::synchronize();

  // Генерируем 3D-сетку
  gmsh::model::mesh::generate(3);


  gmsh::write("./../models/tokamaka.msh");

  // Launch the GUI to see the results:
  std::set<std::string> args(argv, argv + argc);
  if(!args.count("-nopopup")) gmsh::fltk::run();

  gmsh::finalize();
  return 0;
}