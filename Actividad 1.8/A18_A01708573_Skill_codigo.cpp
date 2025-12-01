#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <string>

// --- Estructuras de Datos ---

struct Point {
    int id;
    double x;
    double y;
    bool delivered; 
};

struct Vehicle {
    int id;
    int capacity;
    double distanceTraveled;
};

// --- Funciones Auxiliares ---

// Calcular distancia Euclidiana
double calculateDistance(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// --- Lógica Principal (Encapsulada) ---

void resolverEscenario(std::string nombreCaso, std::vector<Point> packages, int numVehicles, int vehicleCapacity) {
    std::cout << "\n==========================================" << std::endl;
    std::cout << "EJECUTANDO: " << nombreCaso << std::endl;
    std::cout << "Datos: " << numVehicles << " vehiculos (Capacidad: " << vehicleCapacity << "), " 
              << packages.size() << " paquetes." << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    double totalFleetDistance = 0.0;
    int packagesDeliveredCount = 0;
    int totalPackages = packages.size();
    
    // Coordenadas del almacén
    double warehouseX = 0.0;
    double warehouseY = 0.0;

    for (int v = 0; v < numVehicles; v++) {
        // Si ya no hay paquetes pendientes, rompemos el ciclo de vehículos
        if (packagesDeliveredCount >= totalPackages) break;

        Vehicle currentVehicle;
        currentVehicle.id = v + 1;
        currentVehicle.capacity = vehicleCapacity;
        currentVehicle.distanceTraveled = 0.0;

        // Posición actual del vehículo (inicia en almacén)
        double currentX = warehouseX;
        double currentY = warehouseY;

        std::cout << "[Vehiculo " << currentVehicle.id << "] Ruta: Almacen";

        // Bucle de entrega del vehículo
        while (currentVehicle.capacity > 0 && packagesDeliveredCount < totalPackages) {
            int nearestIndex = -1;
            double minDistance = std::numeric_limits<double>::max();

            // Buscar el vecino más cercano (Greedy)
            for (size_t i = 0; i < packages.size(); i++) {
                if (!packages[i].delivered) {
                    double d = calculateDistance(currentX, currentY, packages[i].x, packages[i].y);
                    if (d < minDistance) {
                        minDistance = d;
                        nearestIndex = i;
                    }
                }
            }

            // Moverse al paquete encontrado
            if (nearestIndex != -1) {
                currentVehicle.distanceTraveled += minDistance;
                currentX = packages[nearestIndex].x;
                currentY = packages[nearestIndex].y;
                
                packages[nearestIndex].delivered = true;
                currentVehicle.capacity--;
                packagesDeliveredCount++;

                std::cout << " -> P" << packages[nearestIndex].id;
            }
        }

        // Regresar al almacén
        double distBack = calculateDistance(currentX, currentY, warehouseX, warehouseY);
        currentVehicle.distanceTraveled += distBack;
        totalFleetDistance += currentVehicle.distanceTraveled;

        std::cout << " -> Almacen. (Distancia: " << std::fixed << std::setprecision(2) << currentVehicle.distanceTraveled << ")" << std::endl;
    }

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "RESULTADO " << nombreCaso << ":" << std::endl;
    std::cout << "Distancia Total Flota: " << totalFleetDistance << std::endl;
    std::cout << "Paquetes Entregados: " << packagesDeliveredCount << "/" << totalPackages << std::endl;
    
    if (packagesDeliveredCount < totalPackages) {
        std::cout << "(!) ALERTA: Capacidad insuficiente para entregar todo." << std::endl;
    }
    std::cout << "==========================================\n" << std::endl;
}

// --- Main con los 4 Casos de Prueba ---

int main() {
    // --- CASO 1: Básico (Ejemplo del enunciado reducido) ---
    // 3 vehículos, capacidad 2, pocos paquetes cercanos.
    std::vector<Point> caso1 = {
        {1, 2.0, 2.0, false}, {2, -2.0, 2.0, false}, 
        {3, 2.0, -2.0, false}, {4, -2.0, -2.0, false}
    };
    resolverEscenario("CASO 1 (Basico)", caso1, 3, 2);


    // --- CASO 2: Sobrecarga (Insuficientes vehículos) ---
    // 1 vehículo, capacidad 2, pero hay 5 paquetes. Debería dejar 3 sin entregar.
    std::vector<Point> caso2 = {
        {1, 1.0, 1.0, false}, {2, 2.0, 2.0, false}, {3, 3.0, 3.0, false},
        {4, 4.0, 4.0, false}, {5, 5.0, 5.0, false}
    };
    resolverEscenario("CASO 2 (Sobrecarga)", caso2, 1, 2);


    // --- CASO 3: Larga Distancia ---
    // Paquetes muy lejos del centro. Las distancias serán grandes.
    std::vector<Point> caso3 = {
        {1, 100.0, 100.0, false}, {2, -100.0, -100.0, false}
    };
    resolverEscenario("CASO 3 (Larga Distancia)", caso3, 2, 5);


    // --- CASO 4: Clusters (Agrupaciones) ---
    // Paquetes agrupados en dos zonas distintas. El algoritmo debería 
    // preferir terminar una zona antes de ir a la otra.
    std::vector<Point> caso4 = {
        // Grupo Norte (Cercanos entre sí)
        {1, 0.0, 10.0, false}, {2, 1.0, 11.0, false}, {3, -1.0, 10.0, false},
        // Grupo Sur (Lejanos del norte)
        {4, 0.0, -10.0, false}, {5, 1.0, -11.0, false}, {6, -1.0, -10.0, false}
    };
    // 2 vehículos con capacidad 3 (Idealmente uno va al Norte y otro al Sur)
    resolverEscenario("CASO 4 (Clusters)", caso4, 2, 3);

    return 0;
}