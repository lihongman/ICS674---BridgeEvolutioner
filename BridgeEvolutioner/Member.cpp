#include "Member.h"

Member::Member(const Member& m)
{
    first = m.first;
    second = m.second;
}

double Member::length()
{
    return sqrt(pow(first->x - second->x, 2) + pow(first->y - second->y, 2));
}

void Member::set_force(double f)
{
    if (f < 0)
    {
        force_type = false;
    }
    else
    {
        force_type = true;
    }
    force = abs(f);
    if (first->x == second->x)
    {
        forcey = force;
        forcex = 0;
        return;
    }
    double angle = atan2(abs(first->y - second->y), abs(first->x - second->x));
    forcex = force * cos(angle);
    forcey = force * sin(angle);
}

bool Member::check_intercept(const Member& member)
{
    Joint p1, p2, q1, q2;
    if (first->x > second->x) { p1 = *second; p2 = *first; }
    else { p1 = *first; p2 = *second; }
    if (member.first->x > member.second->x) { q1 = *member.second; q2 = *member.first; }
    else { q1 = *member.first; q2 = *member.second; }

    if (p1 == q1 && !(p2 == q2))
        return false;
    if (p1 == q2 && !(p2 == q1))
        return false;
    if (p2 == q1 && !(p1 == q2))
        return false;
    if (p2 == q2 && !(p1 == q1))
        return false;

    if (p1.x == p2.x && q1.y <= p1.y && q2.y <= p1.y && q1.y <= p2.y && q2.y <= p2.y) { return false; }
    else if (p1.x == p2.x && q1.y > p1.y && q2.y > p1.y && q1.y > p2.y && q2.y > p2.y) { return false; }
    else if (p1.x == p2.x && q2.x < p1.x) { return false; }
    else if (p1.x == p2.x && q1.x > p1.x) { return false; }
    else if (p1.x == p2.x) { 
        return true; }

    if (q1.x == q2.x && p1.y <= q1.y && p2.y <= q1.y && p1.y <= q2.y && p2.y <= q2.y) { return false; }
    else if (q1.x == q2.x && p1.y > q1.y && p2.y > q1.y && p1.y > q2.y && p2.y > q2.y) { return false; }
    else if (q1.x == q2.x && p2.x < q1.x) { return false; }
    else if (q1.x == q2.x && p1.x > q1.x) { return false; }
    else if (q1.x == q2.x) { 
        return true; }

    double slopep = (p1.y - p2.y) / (p1.x - p2.x);
    double constantp = p1.y - (slopep * p1.x);
    double slopeq = (q1.y - q2.y) / (q1.x - q2.x);
    double constantq = q1.y - (slopeq * q1.x);

    if (slopep == slopeq && constantp != constantq) { return false; }
    if (slopep == slopeq && constantp == constantq && ((p2.x > q1.x && p2.x < q2.x) || (q2.x > p1.x && q2.x < p2.x))) { 
        return true; }
    
    double xp, xq, y;
    y = ((slopep * constantq) - (slopeq * constantp)) / (slopep - slopeq);
    xp = (y - constantp) / slopep;
    xq = (y - constantq) / slopeq;

    if (xp >= 0.999999999 * xq && xp <= 1.000000001 * xq)
    {
        if (xp > p1.x && xp < p2.x && xp > q1.x && xp < q2.x)
        {
            return true;
        }
    }
    return false;
}

bool operator==(const Member& m1, const Member& m2)
{
    return (*(m1.first) == *(m2.first)) && (*(m1.second) == *(m2.second));
}
