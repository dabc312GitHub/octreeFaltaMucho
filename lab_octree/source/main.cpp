// codigo base https://www.geeksforgeeks.org/octree-insertion-and-searching/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

typedef long long ll;
typedef vector<vector<ll>> matrizll;
typedef vector<ll> vectorll;

#define TopLeftFront 0 
#define TopRightFront 1 
#define BottomRightFront 2 
#define BottomLeftFront 3 
#define TopLeftBottom 4 
#define TopRightBottom 5 
#define BottomRightBack 6 
#define BottomLeftBack 7 

template<typename T>
struct Point {
	T x, y, z;
	Point() : x(-1), y(-1), z(-1) {}
	Point(T a, T b, T c) : x(a), y(b), z(c) {}
};

template<typename T>
struct Octree {

	Point<T>* point;

	Point<T> *topLeftFront, *bottomRightBack;
	vector<Octree<T>*> children;

	Octree() : point(new Point<T>()) {}

	Octree(T x, T y, T z) : point(new Point<T>(x, y, z)) {}

	Octree(
		T x1, T y1, T z1,
		T x2, T y2, T z2)
	{
		if (x2 < x1
			|| y2 < y1
			|| z2 < z1) {
			cout << "bounday poitns are not valid" << endl;
		}

		point = nullptr;
		topLeftFront = new Point<T>(x1, y1, z1);
		bottomRightBack = new Point<T>(x2, y2, z2);

		children.assign(8, nullptr);
		for (
			size_t i = TopLeftFront;
			i <= BottomLeftBack;
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
		if (x < topLeftFront->x
			|| x > bottomRightBack->x
			|| y < topLeftFront->y
			|| y > bottomRightBack->y
			|| z < topLeftFront->z
			|| z > bottomRightBack->z) {
			return 0;
		}

		// Binary search to insert the point 
		T midx = (topLeftFront->x + bottomRightBack->x) / 2;
		T midy = (topLeftFront->y + bottomRightBack->y) / 2;
		T midz = (topLeftFront->z + bottomRightBack->z) / 2;

		int pos = -1;

		// Checking the octant of 
		// the point 
		if (x <= midx) {
			if (y <= midy)
				pos = (z <= midz) ? TopLeftFront : TopLeftBottom;
			else
				pos = (z <= midz) ? BottomLeftFront : BottomLeftBack;
		}
		else {
			if (y <= midy)
				pos = (z <= midz) ? TopRightFront : TopRightBottom;
			else
				pos = (z <= midz) ? BottomRightFront : BottomRightBack;
		}

		// If an internal node is encountered 
		if (children[pos]->point == nullptr) {
			children[pos]->insert(x, y, z);
			return 0;
		}

		// If an empty node is encountered 
		else if (children[pos]->point->x == -1) {
			delete children[pos];
			children[pos] = new Octree(x, y, z);
			return 0;
		}
		else {
			T x_ = children[pos]->point->x;
			T y_ = children[pos]->point->y;
			T z_ = children[pos]->point->z;

			delete children[pos];
			children[pos] = nullptr;

			if (pos == TopLeftFront) {
				children[pos] = new Octree(
					topLeftFront->x,
					topLeftFront->y,
					topLeftFront->z,
					midx,
					midy,
					midz
				);
			}

			else if (pos == TopRightFront) {
				children[pos] = new Octree(
					midx + 1,
					topLeftFront->y,
					topLeftFront->z,
					bottomRightBack->x,
					midy,
					midz
				);
			}
			else if (pos == BottomRightFront) {
				children[pos] = new Octree(
					midx + 1,
					midy + 1,
					topLeftFront->z,
					bottomRightBack->x,
					bottomRightBack->y,
					midz
				);
			}
			else if (pos == BottomLeftFront) {
				children[pos] = new Octree(
					topLeftFront->x,
					midy + 1,
					topLeftFront->z,
					midx,
					bottomRightBack->y,
					midz
				);
			}
			else if (pos == TopLeftBottom) {
				children[pos] = new Octree(
					topLeftFront->x,
					topLeftFront->y,
					midz + 1,
					midx,
					midy,
					bottomRightBack->z
				);
			}
			else if (pos == TopRightBottom) {
				children[pos] = new Octree(
					midx + 1,
					topLeftFront->y,
					midz + 1,
					bottomRightBack->x,
					midy,
					bottomRightBack->z
				);
			}
			else if (pos == BottomRightBack) {
				children[pos] = new Octree(
					midx + 1,
					midy + 1,
					midz + 1,
					bottomRightBack->x,
					bottomRightBack->y,
					bottomRightBack->z
				);
			}
			else if (pos == BottomLeftBack) {
				children[pos] = new Octree(
					topLeftFront->x,
					midy + 1,
					midz + 1,
					midx,
					bottomRightBack->y,
					bottomRightBack->z);
			}
			children[pos]->insert(x_, y_, z_);
			children[pos]->insert(x, y, z);
		}
		return 1;
	}

	bool find(T x, T y, T z) {
		// If point is out of bound 
		if (x < topLeftFront->x
			|| x > bottomRightBack->x
			|| y < topLeftFront->y
			|| y > bottomRightBack->y
			|| z < topLeftFront->z
			|| z > bottomRightBack->z)
			return 0;

		// Otherwise perform binary search 
		// for each ordinate 
		T midx = (topLeftFront->x + bottomRightBack->x) / 2;
		T midy = (topLeftFront->y + bottomRightBack->y) / 2;
		T midz = (topLeftFront->z + bottomRightBack->z) / 2;

		int pos = -1;

		// Deciding the position 
		// where to move 
		if (x <= midx) {
			if (y <= midy)
				pos = (z <= midz) ? TopLeftFront : TopLeftBottom;
			else
				pos = (z <= midz) ? BottomLeftFront : BottomLeftBack;
		}
		else {
			if (y <= midy) 
				pos = (z <= midz) ? TopRightFront : TopRightBottom;
			else 
				pos = (z <= midz) ? BottomRightFront : BottomRightBack;
		}

		// If an internal node is encountered 
		if (children[pos]->point == nullptr)
			return children[pos]->find(x, y, z);

		// If an empty node is encountered 
		else if (children[pos]->point->x == -1)
			return 0;

		else {	
			// If node is found with 
			// the given value 
			if (x == children[pos]->point->x
				&& y == children[pos]->point->y
				&& z == children[pos]->point->z)
				return 1;
		}
		return 0;
	}
};



template<typename T>
void readFiles(Octree<T> elOctree)
{
	string line;
	ifstream myfile("pokebola.obj");
	vector<string> vec3(3);
	if (myfile.is_open())
	{
		while (getline(myfile, line, ','))
		{
			for (size_t i = 0; i < 3; ++i)
				vec3[i] = (line);
			elOctree.insert(stod(vec3[0]), stod(vec3[1]), stod(vec3[2]));
		}
		myfile.close();
	}

	else cout << "Unable to open file";

}

int main()
{
	Octree<double> elOctree(0, 0, 0, 1, 1, 1);

	readFiles(elOctree);

	return 0;
}
