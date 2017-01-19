#ifndef _ROAD_H_
#define _ROAD_H_

#include <stdio.h>
#define MAX_POINTS 10000

class JudgeRange
{
    float polyX_inside[MAX_POINTS];
    float polyY_inside[MAX_POINTS];
    float polyX_outside[MAX_POINTS];
    float polyY_outside[MAX_POINTS];
    int polySides_in;
    int polySides_out;
    double score_area[48][2] =
    {
        {-113, 41.3689}, {-113, 35.6831}, {-90, 41.00}, {-90, 35.348},
        {-113, -0.1}, {-111.478, -8.69}, {-90, -0.1}, {-90, -8.69},
        {-103, -84}, {-99, -91}, {-85, -71}, {-84, -71},
        {-34, -113}, {-25, -114}, {-34, -90}, {-25, -90},
        {9, -113}, {28, -113}, {9, -91}, {27, -91},
        {97, -99}, {106, -90}, {81, -82}, {88, -76},
        {117, -28}, {116, -10.6}, {95, -28}, {93.7, -11},
        {116.42, 15}, {115, 35}, {93, 16}, {93, 34},
        {107, 84}, {95.5, 100}, {87, 75}, {81, 83},
        {41, 115}, {25, 115}, {40.7, 93}, {25, 93},
        {-7, 117}, {-40.0826, 117}, {-30, 94}, {-30, 94},
        {-88.5, 104.3}, {-100, 95}, {-74.4, 6.68}, {-81.7, 79.9}
    };
    bool pointInPolygon_inside(int x, int y);
    bool pointInPolygon_outside(int x, int y);

public:
    int InScoreArea(int posx, int posy);
    bool JudgeInArea(int posx, int posy, int area_num);
    bool JudgeInRoad(int posx, int posy);
    JudgeRange()
    {
        FILE *points_in = fopen("record_inside.txt", "r");
        FILE *points_out = fopen("record_outside.txt", "r");
        polySides_in = 0;
        polySides_out = 0;
        while (!feof(points_in))
        {
            fscanf(points_in, "%f,%f\n", polyX_inside + polySides_in, polyY_inside + polySides_in);
            polySides_in++;
        }
        while (!feof(points_out))
        {
            fscanf(points_out, "%f,%f\n", polyX_outside + polySides_out, polyY_outside + polySides_out);
            polySides_out++;
        }
        fclose(points_out);
        fclose(points_in);

    }
};

#endif
