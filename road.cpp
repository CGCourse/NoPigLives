#include "road.h"

bool JudgeRange::JudgeInRoad(int posx, int posy)
{
    if (pointInPolygon_outside(posx, posy) && !pointInPolygon_inside(posx, posy))
    {
        return true;
    }
    else
        return false;
}

bool JudgeRange::pointInPolygon_inside(int x, int y)
{

    bool oddNodes = false;
    int i, j = polySides_in - 1;
    for (i = 0; i < polySides_in; i++)
    {
        if (((polyY_inside[i] < y && polyY_inside[j] >= y) || (polyY_inside[j] < y && polyY_inside[i] >= y))
            && (polyX_inside[i] <= x || polyX_inside[j] <= x))
        {
            oddNodes ^= (polyX_inside[i] + (y - polyY_inside[i]) / (polyY_inside[j] - polyY_inside[i]) * (polyX_inside[j] - polyX_inside[i]) < x);
        }
        j = i;
    }
    return oddNodes;
}

bool JudgeRange::pointInPolygon_outside(int x, int y)
{

    bool oddNodes = false;
    int i, j = polySides_out - 1;
    for (i = 0; i < polySides_out; i++)
    {
        if (((polyY_outside[i] < y && polyY_outside[j] >= y) || (polyY_outside[j] < y && polyY_outside[i] >= y))
            && (polyX_outside[i] <= x || polyX_outside[j] <= x))
        {
            oddNodes ^= (polyX_outside[i] + (y - polyY_outside[i]) / (polyY_outside[j] - polyY_outside[i]) * (polyX_outside[j] - polyX_outside[i]) < x);
        }
        j = i;
    }
    return oddNodes;
}

int JudgeRange::InScoreArea(int posx, int posy)
{
    int i;
    for (i = 0; i < 12; i++)
    {
        if (JudgeInArea(posx, posy, i * 4))
            return i;
    }
    return -1;
}

bool JudgeRange::JudgeInArea(int x, int y, int area_num)
{
    bool oddNodes = false;
    int i, j = 3;

    for (i = 0; i < 4; i++)
    {
        if (((score_area[i + area_num][1] < y && score_area[j + area_num][1] >= y)
             || (score_area[j + area_num][1] < y && score_area[i + area_num][1] >= y))
             && (score_area[i + area_num][0] <= x || score_area[j + area_num][0] <= x))
        {
            oddNodes ^= (score_area[i + area_num][0] + (y - score_area[i + area_num][1])
                         / (score_area[j + area_num][1] - score_area[i + area_num][1])
                         * (score_area[j + area_num][0] - score_area[i + area_num][0]) < x);
        }
        j = i;
    }
    return oddNodes;
}
