import numpy as np
import matplotlib.pyplot as plt

# Parameters
wavelength = 500e-9  # meters (green light)
k = 2 * np.pi / wavelength
pixel_size = 1e-6  # 1 micron per pixel
grid_size = 1024  # Grid size (adjust for resolution)
z = 0.1  # Distance from object to hologram plane (10 cm)

# Tilt angle for reference wave (10 degrees in radians)
theta = np.pi / 18

# Object coordinates (single point at center)
obj_x, obj_y, obj_z = 0, 0, z

# Create grid
x = np.arange(-grid_size//2, grid_size//2) * pixel_size
y = np.arange(-grid_size//2, grid_size//2) * pixel_size
X, Y = np.meshgrid(x, y)

# Distance from object to each grid point
r = np.sqrt((X - obj_x)**2 + (Y - obj_y)**2 + obj_z**2)

# Object wave (spherical wave, phase only)
object_wave = np.exp(1j * k * r)

# Reference wave (tilted plane wave)
reference_wave = np.exp(1j * k * np.sin(theta) * X)

# Compute interference intensity
hologram = np.abs(object_wave + reference_wave)**2

# Normalize and display
hologram_normalized = (hologram - hologram.min()) / (hologram.max() - hologram.min())
plt.figure(figsize=(10, 10))
plt.imshow(hologram_normalized, cmap='gray')
plt.title('Simulated Hologram of a Single Point')
plt.axis('off')
plt.show()