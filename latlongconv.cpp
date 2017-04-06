#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utmconstants.h"
#include "latlongconv.h"

LATLONGCONV::LATLONGCONV()
{

}



void LATLONGCONV::SetRefEllipsoid(int ref)
{
    if(ref > 23 || ref <-1) {
        isEllipsoidSet = false;
        return;
    }
    EquatorialRadius = ellipsoid[ref].EquatorialRadius;
    EccentricitySquared = ellipsoid[ref].eccentricitySquared;
    isEllipsoidSet = true;
}

void LATLONGCONV::LLtoUTM(Coordinate coord, UTM *utm)
{
//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees
    //Written by Chuck Gantz- chuck.gantz@globalstar.com
    double k0 = 0.9996;
    double LongOrigin;
    double eccPrimeSquared;
    double N, T, C, A, M;
//Make sure the longitude is between -180.00 .. 179.9
    double LongTemp = (coord.lon+180)-int((coord.lon+180)/360)*360-180; // -180.00 .. 179.9;
    double LatRad = coord.lat*deg2rad;
    double LongRad = LongTemp*deg2rad;
    double LongOriginRad;
    int    ZoneNumber;

    ZoneNumber = int((LongTemp + 180)/6) + 1;

    if( coord.lat >= 56.0 && coord.lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
        ZoneNumber = 32;

  // Special zones for Svalbard
    if( coord.lat >= 72.0 && coord.lat < 84.0 )
    {
      if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
      else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
      else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
      else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
     }
    LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
    LongOriginRad = LongOrigin * deg2rad;

    //compute the UTM Zone from the latitude and longitude
    sprintf(LATLONGCONV::_UTMZone, "%d%c", ZoneNumber, UTMLetterDesignator(coord.lat));

    eccPrimeSquared = (EccentricitySquared)/(1-EccentricitySquared);

    N = EquatorialRadius/sqrt(1-EccentricitySquared*sin(LatRad)*sin(LatRad));
    T = tan(LatRad)*tan(LatRad);
    C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
    A = cos(LatRad)*(LongRad-LongOriginRad);

    M = EquatorialRadius*((1	- EccentricitySquared/4		- 3*EccentricitySquared*EccentricitySquared/64	- 5*EccentricitySquared*EccentricitySquared*EccentricitySquared/256)*LatRad
                - (3*EccentricitySquared/8	+ 3*EccentricitySquared*EccentricitySquared/32	+ 45*EccentricitySquared*EccentricitySquared*EccentricitySquared/1024)*sin(2*LatRad)
                                    + (15*EccentricitySquared*EccentricitySquared/256 + 45*EccentricitySquared*EccentricitySquared*EccentricitySquared/1024)*sin(4*LatRad)
                                    - (35*EccentricitySquared*EccentricitySquared*EccentricitySquared/3072)*sin(6*LatRad));

    utm->Easting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
                    + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
                    + 500000.0);

    utm->Northing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));
    if(coord.lat < 0)
        utm->Northing += 10000000.0; //10000000 meter offset for southern hemisphere
}

char LATLONGCONV::UTMLetterDesignator(double Lat)
{
//This routine determines the correct UTM letter designator for the given latitude
//returns 'Z' if latitude is outside the UTM limits of 84N to 80S
    //Written by Chuck Gantz- chuck.gantz@globalstar.com
    char LetterDesignator;
    if((84 >= Lat) && (Lat >= 72)) LetterDesignator = 'X';
    else if((72 > Lat) && (Lat >= 64)) LetterDesignator = 'W';
    else if((64 > Lat) && (Lat >= 56)) LetterDesignator = 'V';
    else if((56 > Lat) && (Lat >= 48)) LetterDesignator = 'U';
    else if((48 > Lat) && (Lat >= 40)) LetterDesignator = 'T';
    else if((40 > Lat) && (Lat >= 32)) LetterDesignator = 'S';
    else if((32 > Lat) && (Lat >= 24)) LetterDesignator = 'R';
    else if((24 > Lat) && (Lat >= 16)) LetterDesignator = 'Q';
    else if((16 > Lat) && (Lat >= 8)) LetterDesignator = 'P';
    else if(( 8 > Lat) && (Lat >= 0)) LetterDesignator = 'N';
    else if(( 0 > Lat) && (Lat >= -8)) LetterDesignator = 'M';
    else if((-8> Lat) && (Lat >= -16)) LetterDesignator = 'L';
    else if((-16 > Lat) && (Lat >= -24)) LetterDesignator = 'K';
    else if((-24 > Lat) && (Lat >= -32)) LetterDesignator = 'J';
    else if((-32 > Lat) && (Lat >= -40)) LetterDesignator = 'H';
    else if((-40 > Lat) && (Lat >= -48)) LetterDesignator = 'G';
    else if((-48 > Lat) && (Lat >= -56)) LetterDesignator = 'F';
    else if((-56 > Lat) && (Lat >= -64)) LetterDesignator = 'E';
    else if((-64 > Lat) && (Lat >= -72)) LetterDesignator = 'D';
    else if((-72 > Lat) && (Lat >= -80)) LetterDesignator = 'C';
    else LetterDesignator = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits
    return LetterDesignator;
}

void LATLONGCONV::UTMtoLL(UTM utm, Coordinate *coord )
{
    //converts UTM coords to lat/long.  Equations from USGS Bulletin 1532
    //East Longitudes are positive, West longitudes are negative.
    //North latitudes are positive, South latitudes are negative
    //Lat and Long are in decimal degrees.
    //Written by Chuck Gantz- chuck.gantz@globalstar.com
    double k0 = 0.9996;
    double eccPrimeSquared;
    double e1 = (1-sqrt(1-EccentricitySquared))/(1+sqrt(1-EccentricitySquared));
    double N1, T1, C1, R1, D, M;
    double LongOrigin;
    double mu, phi1, phi1Rad;
    double x, y;
    int ZoneNumber;
    char* ZoneLetter;
    int NorthernHemisphere; //1 for northern hemispher, 0 for southern

    x = utm.Easting - 500000.0; //remove 500,000 meter offset for longitude
    y = utm.Northing;

    ZoneNumber = strtoul(_UTMZone, &ZoneLetter, 10);
    if((*ZoneLetter - 'N') >= 0)
        NorthernHemisphere = 1;//point is in northern hemisphere
    else
    {
        NorthernHemisphere = 0;//point is in southern hemisphere
        y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
    }

    LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone

    eccPrimeSquared = (EccentricitySquared)/(1-EccentricitySquared);

    M = y / k0;
    mu = M/(EquatorialRadius*(1-EccentricitySquared/4-3*EccentricitySquared*EccentricitySquared/64-5*EccentricitySquared*EccentricitySquared*EccentricitySquared/256));

    phi1Rad = mu	+ (3*e1/2-27*e1*e1*e1/32)*sin(2*mu)
                + (21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*mu)
                +(151*e1*e1*e1/96)*sin(6*mu);
    phi1 = phi1Rad*rad2deg;
    N1 = EquatorialRadius/sqrt(1-EccentricitySquared*sin(phi1Rad)*sin(phi1Rad));
    T1 = tan(phi1Rad)*tan(phi1Rad);
    C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
    R1 = EquatorialRadius*(1-EccentricitySquared)/pow(1-EccentricitySquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
    D = x/(N1*k0);
    coord->lat = phi1Rad - (N1*tan(phi1Rad)/R1)*(D*D/2-(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
                    +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
    coord->lat = coord->lat * rad2deg;
    coord->lon = (D-(1+2*T1+C1)*D*D*D/6+(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
                    *D*D*D*D*D/120)/cos(phi1Rad);
    coord->lon = LongOrigin + coord->lon * rad2deg;
}

char* LATLONGCONV::GetUTMZone()
{
    char* str = new char [4];
    strcpy(str, LATLONGCONV::_UTMZone);
    return str;
}
