import meshio

mesh = meshio.read("cup2.msh")

print(mesh.cells)
mesh.cells = [mesh.cells[-1]]
print(mesh.cells)

meshio.write("cup2.xdmf", mesh)