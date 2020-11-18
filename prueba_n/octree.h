#ifndef OCTREE_H
#define OCTREE_H

// codigo base https://www.geeksforgeeks.org/octree-insertion-and-searching/
#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

#include "point.h"

using namespace std;
using namespace std::chrono;

typedef long long ll;
typedef vector<vector<ll>> matrizll;
typedef vector<ll> vectorll;


template<typename T>
struct Octree
{
    int ch_TopLeftFront         =0;
    int ch_TopRightFront		=1;
    int ch_BottomRightFront     =2;
    int ch_BottomLeftFront		=3;
    int ch_TopLeftBottom		=4;
    int ch_TopRightBottom		=5;
    int ch_BottomRightBack		=6;
    int ch_BottomLeftBack		=7;

    int atributoContador = 0;
    Point<T>* oc_point;
    bool leaf;

    Point<T> *octante_topLeftFront;     //Min
    Point<T> *octante_bottomRightBack;  //Max
    vector<Octree<T>*> children;

    Octree() : oc_point(new Point<T>()) {leaf = true;}

    Octree(T x, T y, T z) : oc_point(new Point<T>(x, y, z)) {leaf = true;}

    Octree(
        T x1, T y1, T z1,
        T x2, T y2, T z2)
    {
        if (x2 < x1
            || y2 < y1
            || z2 < z1)
        {
            cout << "bounday poitns are not valid" << endl;
        }

        oc_point = nullptr;
        octante_topLeftFront = new Point<T>(x1, y1, z1);//Max
        octante_bottomRightBack = new Point<T>(x2, y2, z2);//Min

        children.assign(8, nullptr);
        for (
            size_t i = ch_TopLeftFront;
            i <= ch_BottomLeftBack;
            ++i
            )
            children[i] = new Octree();
    }

    bool insert(T x, T y, T z)
    {
        // If the point already exists in the octree
        if (find(x, y, z))
            return 0;

        // If the point is out of bounds
        if (x < octante_topLeftFront->x
            || x > octante_bottomRightBack->x
            || y < octante_topLeftFront->y
            || y > octante_bottomRightBack->y
            || z < octante_topLeftFront->z
            || z > octante_bottomRightBack->z) {
            return 0;
        }

        // Binary search to insert the point
        T midx = (octante_topLeftFront->x + octante_bottomRightBack->x) / 2;
        T midy = (octante_topLeftFront->y + octante_bottomRightBack->y) / 2;
        T midz = (octante_topLeftFront->z + octante_bottomRightBack->z) / 2;

        int pos = -1;

        // Checking the octant of the point
        if (x <= midx)
        {
            if (y <= midy)
                pos = (z <= midz) ? ch_TopLeftFront : ch_TopLeftBottom;
            else
                pos = (z <= midz) ? ch_BottomLeftFront : ch_BottomLeftBack;
        }
        else
        {
            if (y <= midy)
                pos = (z <= midz) ? ch_TopRightFront : ch_TopRightBottom;
            else
                pos = (z <= midz) ? ch_BottomRightFront : ch_BottomRightBack;
        }

        // If an internal node is encountered
        if (children[pos]->point == nullptr)
        {
            children[pos]->insert(x, y, z);
            return 0;
        }

        // If an empty node is encountered
        else if (children[pos]->point->x == -1)
        {
            delete children[pos];
            children[pos] = new Octree(x, y, z);
            return 0;
        }
        else
        {
            T x_ = children[pos]->point->x;
            T y_ = children[pos]->point->y;
            T z_ = children[pos]->point->z;

            delete children[pos];
            children[pos] = nullptr;

            if (pos == ch_TopLeftFront)
            {
                children[pos] = new Octree(
                    octante_topLeftFront->x,
                    octante_topLeftFront->y,
                    octante_topLeftFront->z,
                    midx,
                    midy,
                    midz
                );
            }

            else if (pos == ch_TopRightFront)
            {
                children[pos] = new Octree(
                    midx + 1,
                    octante_topLeftFront->y,
                    octante_topLeftFront->z,
                    octante_bottomRightBack->x,
                    midy,
                    midz
                );
            }
            else if (pos == ch_BottomRightFront)
            {
                children[pos] = new Octree(
                    midx + 1,
                    midy + 1,
                    octante_topLeftFront->z,
                    octante_bottomRightBack->x,
                    octante_bottomRightBack->y,
                    midz
                );
            }
            else if (pos == ch_BottomLeftFront)
            {
                children[pos] = new Octree(
                    octante_topLeftFront->x,
                    midy + 1,
                    octante_topLeftFront->z,
                    midx,
                    octante_bottomRightBack->y,
                    midz
                );
            }
            else if (pos == ch_TopLeftBottom) {
                children[pos] = new Octree(
                    octante_topLeftFront->x,
                    octante_topLeftFront->y,
                    midz + 1,
                    midx,
                    midy,
                    octante_bottomRightBack->z
                );
            }
            else if (pos == ch_TopRightBottom)
            {
                children[pos] = new Octree(
                    midx + 1,
                    octante_topLeftFront->y,
                    midz + 1,
                    octante_bottomRightBack->x,
                    midy,
                    octante_bottomRightBack->z
                );
            }
            else if (pos == ch_BottomRightBack)
            {
                children[pos] = new Octree(
                    midx + 1,
                    midy + 1,
                    midz + 1,
                    octante_bottomRightBack->x,
                    octante_bottomRightBack->y,
                    octante_bottomRightBack->z
                );
            }
            else if (pos == ch_BottomLeftBack)
            {
                children[pos] = new Octree(
                    octante_topLeftFront->x,
                    midy + 1,
                    midz + 1,
                    midx,
                    octante_bottomRightBack->y,
                    octante_bottomRightBack->z);
            }
            children[pos]->insert(x_, y_, z_);
            children[pos]->insert(x, y, z);

        }
        atributoContador++;
        return 1;
    }

    bool find(T x, T y, T z) {
        // If point is out of bound
        if (x < octante_topLeftFront->x
            || x > octante_bottomRightBack->x
            || y < octante_topLeftFront->y
            || y > octante_bottomRightBack->y
            || z < octante_topLeftFront->z
            || z > octante_bottomRightBack->z)
            return 0;

        // Otherwise perform binary search
        // for each ordinate
        T midx = (octante_topLeftFront->x + octante_bottomRightBack->x) / 2;
        T midy = (octante_topLeftFront->y + octante_bottomRightBack->y) / 2;
        T midz = (octante_topLeftFront->z + octante_bottomRightBack->z) / 2;

        int pos = -1;//empty

        // Deciding the position
        // where to move
        if (x <= midx)
        {
            if (y <= midy)
                pos = (z <= midz) ? ch_TopLeftFront : ch_TopLeftBottom;
            else
                pos = (z <= midz) ? ch_BottomLeftFront : ch_BottomLeftBack;
        }
        else
        {
            if (y <= midy)
                pos = (z <= midz) ? ch_TopRightFront : ch_TopRightBottom;
            else
                pos = (z <= midz) ? ch_BottomRightFront : ch_BottomRightBack;
        }

        // If an internal node is encountered
        if (children[pos]->point == nullptr)
            return children[pos]->find(x, y, z);

        // If an empty node is encountered
        else if (children[pos]->point->x == -1)
            return 0;

        else
        {
            // If node is found with the given value
            if (x == children[pos]->point->x
                && y == children[pos]->point->y
                && z == children[pos]->point->z)
                return 1;
        }
        return 0;
    }
    /*
    int leafCount()
    {
        atributoContador()
    }
    */
};


/*
void writeFiles()
{
    ofstream myfile("test.obj");
    if (myfile.is_open())
    {
        myfile << "1.22222,2.2222,3.111\n";
        myfile << "1.000,2.111,5.333\n";
        myfile.close();
    }
    else cout << "Unable to open file";
}

double str_do(string s)
{
    double f = 0.0;
    stringstream ss;
    ss << s;
    ss >> f;
    return f;
}

template<typename T>
void readFiles(Octree<T> elOctree)
{
    string line;
    ifstream myfile("pokebola.obj");
    vector<string> vec3(3);
    if (myfile.is_open())
    {
        vector<double> vecDo(3);
        while (getline(myfile, line, ','))
        {
            //cout <<  << endl;
            for (size_t i = 0; i < 3; ++i)
            {
                //vec3[i] = (line);
                //vecDo[i] = str_do(vec3[i]);
                //cout << vec3[i] << " ";
            }
            //cout << "HOLI" << endl;
            //cout << endl;
            //elOctree.insert((vecDo[0]), (vecDo[1]), (vecDo[2]));
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}


int main()
{
    Octree<double> elOctree(0, 0, 0, 1, 1, 1);

    readFiles(elOctree);
    cout << (elOctree.find(66.648406, 4.78731, 145.613558)) << endl;
    return 0;
}
*/

#endif // OCTREE_H
