#include "robot.h"
int main() {
const Point2D posicion(7,3);
const Direccion orientacion(Direccion::Norte);
std::string nombre1 = "c3p0";
Robot c3po(nombre1 , posicion, orientacion, 0);
c3po.EjecutaSecuencia("DAAIAI");
std::cout << c3po << std::endl;
Robot r2d2 ( "r2d2", posicion, Direccion::Sur, 0);
std::cout << r2d2 << std::endl;
return 0;
}