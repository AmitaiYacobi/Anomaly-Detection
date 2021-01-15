#include "minCircle.h"

/*
  algorithm welzl:
    input: Finite sets P and R of points in the plane |R|<= 3.
    output: Minimal disk enclosing P with R on the boundary.

    if P is empty or |R| = 3 then
        return trivial(R)
    choose p in P (randomly and uniformly)
    D := welzl(P - { p }, R)
    if p is in D then
        return D

    return welzl(P - { p }, R U { p })
 */
float dist(Point a, Point b)
{
    float x2 = (a.x - b.x) * (a.x - b.x);
    float y2 = (a.y - b.y) * (a.y - b.y);
    return sqrt(x2 + y2);
}

Circle from2points(Point a, Point b)
{
    float x = (a.x + b.x) / 2;
    float y = (a.y + b.y) / 2;
    float r = dist(a, b) / 2;
    return Circle(Point(x, y), r);
}

Circle from3Points(Point a, Point b, Point c)
{
    // find the circumcenter of the triangle a,b,c
    // find the circumcenter of the triangle a,b,c

    Point mAB((a.x + b.x) / 2, (a.y + b.y) / 2); // mid point of line AB
    float slopAB = (b.y - a.y) / (b.x - a.x); // the slop of AB
    float pSlopAB = -1 / slopAB; // the perpendicular slop of AB
    // pSlop equation is:
    // y - mAB.y = pSlopAB * (x - mAB.x) ==> y = pSlopAB * (x - mAB.x) + mAB.y

    Point mBC((b.x + c.x) / 2, (b.y + c.y) / 2); // mid point of line BC
    float slopBC = (c.y - b.y) / (c.x - b.x); // the slop of BC
    float pSlopBC = -1 / slopBC; // the perpendicular slop of BC

    //pSlopAB * (x - mAB.x) + mAB.y = pSlopBC * (x - mBC.x) + mBC.y
    //pSlopAB*x - pSlopAB*mAB.x + mAB.y = pSlopBC*x - pSlopBC*mBC.x + mBC.y
    //x*(pSlopAB - pSlopBC) = - pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y
    //x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);

    float x = (-pSlopBC * mBC.x + mBC.y + pSlopAB * mAB.x - mAB.y) / (pSlopAB - pSlopBC);
    float y = pSlopAB * (x - mAB.x) + mAB.y;
    Point center(x, y);
    float R = dist(center, a);

    return Circle(center, R);
}

Circle trivial(vector<Point>& P)
{
    if (P.size() == 0)
        return Circle(Point(0, 0), 0);
    else if (P.size() == 1)
        return Circle(P[0], 0);
    else if (P.size() == 2)
        return from2points(P[0], P[1]);
    // maybe 2 of the points define a small circle that contains the 3ed point
    Circle c = from2points(P[0], P[1]);
    if (dist(P[2], c.center) <= c.radius)
        return c;
    c = from2points(P[0], P[2]);
    if (dist(P[1], c.center) <= c.radius)
        return c;
    c = from2points(P[1], P[2]);
    if (dist(P[0], c.center) <= c.radius)
        return c;
    // else find the unique circle from 3 points
    return from3Points(P[0], P[1], P[2]);
}

Circle welzl(Point** P, vector<Point> R, size_t n)
{
    if (n == 0 || R.size() == 3) {
        return trivial(R);
    }

    int i = rand() % n;
    Point p(P[i]->x, P[i]->y);
    swap(P[i], P[n - 1]);
    Circle c = welzl(P, R, n - 1);

    if (dist(p, c.center) <= c.radius)
        return c;

    R.push_back(p);
    return welzl(P, R, n - 1);
}

Circle findMinCircle(Point** points, size_t size)
{
    return welzl(points, {}, size);
}
