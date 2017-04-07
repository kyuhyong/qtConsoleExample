#include "formation.h"

Formation::Formation()
{
    Formation::_UavData.HorizontalSpeed_m_s = 1.0;
    Formation::_UavData.minimumSafeHeight_m = 3.0;
    Formation::_UavData.minimumSafeRadious_m = 5.0;
    Formation::numUavs = 18;
    Formation::numDecks = 2;
    Formation::numRows = 3;
    Formation::numColomns = 3;
}

Formation::Formation(int num, int rows, int colomn, int decks, double minSafeRadious_m, double minSafeHeight_m)
{
    Formation::numUavs = num;
    Formation::numRows = rows;
    Formation::numColomns = colomn;
    Formation::numDecks = decks;
    Formation::_UavData.minimumSafeRadious_m = minSafeRadious_m;
    Formation::_UavData.minimumSafeHeight_m = minSafeHeight_m;

}
