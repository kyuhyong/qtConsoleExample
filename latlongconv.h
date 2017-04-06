#ifndef LATLONGCONV_H
#define LATLONGCONV_H

class LATLONGCONV
{
private:
    double EquatorialRadius;
    double EccentricitySquared;
    bool isEllipsoidSet;
    char _UTMZone[4];

public:
    struct UTM{
        double Northing;
        double Easting;
    };
    struct Coordinate{
        double lat;
        double lon;
    };

    LATLONGCONV();
    void SetRefEllipsoid(int ref);
    void LLtoUTM(Coordinate coord, UTM *utm);
    void UTMtoLL(UTM utm, Coordinate *coord );
    char UTMLetterDesignator(double Lat);
    char* GetUTMZone();

};

class Ellipsoid
{
public:
    Ellipsoid(){};
    Ellipsoid(int Id, char const* name, double radius, double ecc)
    {
        id = Id;
        ellipsoidName = name;
        EquatorialRadius = radius;
        eccentricitySquared = ecc;
    }

    int id;
    char const* ellipsoidName;
    double EquatorialRadius;
    double eccentricitySquared;

};


#endif // LATLONGCONV_H
