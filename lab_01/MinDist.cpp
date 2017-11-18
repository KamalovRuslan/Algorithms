#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <time.h>

using namespace std;

class Point;
bool(*Comparator)(const Point &a, const Point &b);
bool cmp_x(const Point &a, const Point &b);
bool cmp_y(const Point &a, const Point &b);
template <class T> void mergeSort(T * mas, int len);

typedef long long Int64;

class Point {
public:
    Int64 x, y;
    int id;
    Point() : x(0), y(0), id(0) {}
    Point(Int64 a, Int64 b, int id) : x(a), y(b), id(id) {}
    bool operator==(const Point& rhs)
    {
        return this->x == rhs.x && this->y == rhs.y;
    }

    bool operator<(const Point& rhs)
    {
        return cmp_x(*this, rhs);
    }
};

bool cmp_x(const Point &a, const Point &b)
{
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool cmp_y(const Point &a, const Point &b)
{
    return a.y < b.y;
}

Int64 mindist = INT64_MAX;
Point ansa, ansb;

Int64 Distance(const Point &a, const Point &b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void upd_ans(const Point &a, const Point &b)
{
    Int64 dist = Distance(a, b);
    if (dist < mindist)
        mindist = dist, ansa = a, ansb = b;
}

std::vector<Point> x;

void merge_inplace(
    vector<Point> a, 
    int first, 
    int mid, 
    int last, 
    bool(*comparator)(const Point &a, const Point &b))
{
    int head = first;
    while (head < last)
    {
        comparator(a[first], a[mid]) ? a[head] = a[first++] : a[head] = a[mid++];
        head++;
    }
}

void min_dist(int l, int r)
{
    int i, j;
    if (r - l < 4) {
        for (i = l; i <= r; ++i)
            for (j = i + 1; j <= r; ++j)
                upd_ans(x[i], x[j]);
        mergeSort(x.data(), r - l);
        return;
    }

    int m = (l + r) / 2;
    Int64 midx = x[m].x;
    min_dist(l, m);
    min_dist(m + 1, r);
    merge_inplace(x, l, m + 1, r + 1, cmp_y);

    vector<Point> t;
    for (i = l; i <= r; ++i)
        if (abs(x[i].x - midx) < mindist) {
            for (j = t.size() - 1; j >= 0 && x[i].y - t[j].y < mindist; --j)
                upd_ans(x[i], t[j]);
            t.push_back(x[i]);
        }
}

template <class T>
T* merge(T *m1, T* m2, int len_1, int len_2)
{
    T* ret = new T[len_1 + len_2];
    int n = 0;

    while (len_1 && len_2) {
        if (*m1 < *m2) {
            ret[n] = *m1;
            m1++;
            --len_1;
        }
        else {
            ret[n] = *m2;
            ++m2;
            --len_2;
        }
        ++n;
    }

    if (len_1 == 0) {
        for (int i = 0; i < len_2; ++i) {
            ret[n++] = *m2++;
        }
    }
    else {
        for (int i = 0; i < len_1; ++i) {
            ret[n++] = *m1++;
        }
    }
    return ret;
}

template <class T>
void mergeSort(T * mas, int len)
{
    int n = 1, k, ost;
    T * mas1;
    while (n < len) {
        k = 0;
        while (k < len) {
            if (k + n >= len) break;
            ost = (k + n * 2 > len) ? (len - (k + n)) : n;
            mas1 = merge(mas + k, mas + k + n, n, ost);
            for (int i = 0; i < n + ost; ++i)
                mas[k + i] = mas1[i];
            delete[] mas1;
            k += n * 2;
        }
        n *= 2;
    }
}


int main(int argc, char** argv) {
    auto comporator = cmp_y;
    if (argc < 2) {
        cout << "Input file is required" << endl;
        cout << "Using: MinDist input" << endl;
        return -1;
    }        
    auto filename = argv[1];
    ifstream file;
    file.open(filename);
    if (file.fail())
    {
        cout << "Can't read from file " << filename << endl;
        return -1;
    }
    int points_number;
    Int64 a, b;
    file >> points_number;
    vector<Point> origin;
    for (int i = 0; i < points_number; ++i) {
        file >> a >> b;
        x.push_back(Point(a, b, i));
    }
    auto start_time = clock();
    mergeSort(x.data(), x.size());
    min_dist(0, points_number - 1);
    auto finish_time = clock();
    cout << "Id of first point " << ansa.id << " id of second point "  << ansb.id << endl;
    cout << "First point " << '(' << ansa.x << ", " << ansa.y << ')' << ' '
        << "second point " << '(' << ansb.x << ", " << ansb.y << ')' << endl;
    cout << "Distance between points " << Distance(ansa, ansb) << endl;
    cout << "Elapsed time " << finish_time - start_time << "ms"<< endl;
}
