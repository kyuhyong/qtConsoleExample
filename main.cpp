#include <iostream>
#include <QCoreApplication>
#include "latlongconv.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    LATLONGCONV latlongConv;
    LATLONGCONV::Coordinate coord;
    LATLONGCONV::Coordinate outCoord;
    LATLONGCONV::UTM utm;

    coord.lat = 37.404833;
    coord.lon = 127.110111;
    int RefEllipsoid = 23;//WGS-84. See list with file "LatLong- UTM conversion.cpp" for id numbers
    std::cout.precision(10);
    std::cout << "Starting position(Lat, Long):  " << coord.lat << "   " << coord.lon <<std::endl;
    latlongConv.SetRefEllipsoid(RefEllipsoid);
    latlongConv.LLtoUTM(coord, &utm);
    std::cout << "Calculated UTM position(Northing, Easting, Zone):  ";
    std::cout << utm.Northing << "   " << utm.Easting;
    std::cout << "   " << latlongConv.GetUTMZone() <<std::endl;
    latlongConv.UTMtoLL(utm, &outCoord);
    std::cout << "Calculated Lat, Long position(Lat, Long):  " << outCoord.lat << "   " << outCoord.lon <<std::endl;
}
