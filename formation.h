#ifndef FORMATION_H
#define FORMATION_H


class Formation
{
private:
    int numUavs;
    int numRows;
    int numColomns;
    int numDecks;
    struct uavData{
        double minimumSafeRadious_m;
        double minimumSafeHeight_m;
        double HorizontalSpeed_m_s;
        double VerticalSpeed_m_s;
    }_UavData;

public:
    Formation();
    Formation(int num, int rows, int colomn, int decks, double minSafeRadious_m, double minSafeHeight_m);
};

#endif // FORMATION_H
