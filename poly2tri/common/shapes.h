/*
 * Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
 * http://code.google.com/p/poly2tri/
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of Poly2Tri nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Include guard
#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <cstddef>
#include <assert.h>
#include <vertex.h>
#include <cmath>




namespace p2t {
struct Point;
    struct Normal{
        double x, y, z;
        Normal(){
          x = 0;
          y = 0;
          z = 0;
        }
        Normal(double num){
          x = num;
          y = num;
          z = num;
        }
        Normal(double x_, double y_, double z_){
          x = x_;
          y = y_;
          z = z_;
        }

        void operator +=(const p2t::Point& v);
        /// Convert this point into a unit point. Returns the Length. 坐标归一化？变成0到1的范围？
        Normal Normalize(Normal no);

    };

struct Edge;

struct Point {

  double x, y, z;

  int index;
  int lenIndex;
  bool isMove = false;
  bool isHide = false;
  Normal normal;
  double inDistance, outDistance;

  // 1 2 3 则是不同的hole，-1是插值点
  int isHole = 0;

  /// Default constructor does nothing (for performance).
  Point()
  {
    x = 0.0;
    y = 0.0;
  }

  void print(){
    cout << "index: " << index << " " <<  x << " " << y << " " << z << endl;
  }

  VERTEX PointToVertex()
  {
    VERTEX vertex;
    vertex.x = this->x;
    vertex.y = this->y;
    vertex.z = this->z;
//    vertex.index = this->lenIndex;
    return vertex;
  }

  /// The edges this point constitutes an upper ending point
  std::vector<Edge*> edge_list;

  /// Construct using coordinates.
  Point(double x, double y) : x(x), y(y) {}
  Point(double x, double y, double z) : x(x), y(y), z(z) {}

  /// Set this point to all zeros.
  void set_zero()
  {
    x = 0.0;
    y = 0.0;
  }


  float magnitude()
  {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
  }

    /// Set this point to some specified coordinates.
  void set(double x_, double y_)
  {
    x = x_;
    y = y_;
  }

  /// Negate this point.
  Point operator -() const
  {
    Point v;
    v.set(-x, -y);
    return v;
  }

  /// Add a point to this point.
  void operator +=(const Point& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
  }

  /// Subtract a point from this point.
  void operator -=(const Point& v)
  {
    x -= v.x;
    y -= v.y;
  }

  /// Multiply this point by a scalar.
  void operator *=(double a)
  {
    x *= a;
    y *= a;
  }

  /// Get the length of this point (the norm).
  double Length() const
  {
    return sqrt(x * x + y * y + z * z);
  }

  /// Convert this point into a unit point. Returns the Length. 坐标归一化？变成0到1的范围？
  double Normalize()
  {
    double len = Length();
    x /= len;
    y /= len;
    return len;
  }


    Point operator+(Point v)
    {
      double x1, y1, z1;
      x1 = x + v.x;
      y1 = y + v.y;
      z1 = z + v.z;
      return Point(x1, y1, z1);
    }

// Subtract 2 Points 
    Point operator-(Point v)
    {
      double x1, y1, z1;
      x1 = x - v.x;
      y1 = y - v.y;
      z1 = z - v.z;
      return Point(x1, y1, z1);
    }

// Dot product of 2 Points
  //点积
    double operator^(Point v)
    {
      double x1, y1, z1;
      x1 = x * v.x;
      y1 = y * v.y;
      z1 = z * v.z;
      return (x1 + y1 + z1);
    }

    //叉集
// Cross product of 2 Points 
    Point operator*(Point v)
    {
      double x1, y1, z1;
      x1 = y * v.z - z * v.y;
      y1 = z * v.x - x * v.z;
      z1 = x * v.y - y * v.x;
      return Point(x1, y1, z1);
    }


    double Distance(Point *point){
      return sqrt(pow(x - point->x, 2) + pow(y - point->y, 2) + pow(y - point->y, 2));
  }



};



float shortDistance(Point line_point1, Point line_point2,
                        Point point);
//三角的边
// Represents a simple polygon's edge
struct Edge {
    //由下面的构造函数可以得知，p是在左，q在右
  Point* p, *q;

  /// Constructor，p一定要比q要低，但是x和y就不好确定了。
  Edge(Point& p1, Point& p2) : p(&p1), q(&p2)
  {
    if (p1.y > p2.y) {
      q = &p1;
      p = &p2;
    } else if (p1.y == p2.y) {

      if (p1.x > p2.x) {
        q = &p1;
        p = &p2;
        //出现重复的点会出问题
      } else if (p1.x == p2.x) {
        // Repeat points
        cout << "repeat :";
        p1.print();
        p2.print();
        assert(false);
      }
    }

    //每个点都有一堆的边？
    q->edge_list.push_back(this);
  }
};

// Triangle-based data structures are know to have better performance than quad-edge structures
// See: J. Shewchuk, "Triangle: Engineering a 2D Quality Mesh Generator and Delaunay Triangulator"
//      "Triangulations in CGAL"
//三角形的类
class Triangle {
public:
//函数还蛮多，主要是有这个限制边，限制边的话应该就是不会剖分？还是说限制边围绕起来的空洞内层不会剖分？还是需要研究研究。
/// Constructor
Triangle(Point& a, Point& b, Point& c);

//将三角里的点置为隐藏
void HidePoints();

//判断是否要隐藏，入股
bool isHide = false;

//两种边，一种约束边，一种德内诺边。
/// Flags to determine if an edge is a Constrained edge
bool constrained_edge[3];
/// Flags to determine if an edge is a Delauney edge
bool delaunay_edge[3];

Point* GetPoint(const int& index);
Point* PointCW(Point& point);
Point* PointCCW(Point& point);
Point* OppositePoint(Triangle& t, Point& p);

Triangle* GetNeighbor(const int& index);
void MarkNeighbor(Point* p1, Point* p2, Triangle* t);
void MarkNeighbor(Triangle& t);

void MarkConstrainedEdge(const int index);
void MarkConstrainedEdge(Edge& edge);
void MarkConstrainedEdge(Point* p, Point* q);

int Index(const Point* p);
int EdgeIndex(const Point* p1, const Point* p2);

Triangle* NeighborCW(Point& point);
Triangle* NeighborCCW(Point& point);
bool GetConstrainedEdgeCCW(Point& p);
bool GetConstrainedEdgeCW(Point& p);
void SetConstrainedEdgeCCW(Point& p, bool ce);
void SetConstrainedEdgeCW(Point& p, bool ce);
bool GetDelunayEdgeCCW(Point& p);
bool GetDelunayEdgeCW(Point& p);
void SetDelunayEdgeCCW(Point& p, bool e);
void SetDelunayEdgeCW(Point& p, bool e);

bool Contains(Point* p);
bool Contains(const Edge& e);
bool Contains(Point* p, Point* q);
void Legalize(Point& point);
void Legalize(Point& opoint, Point& npoint);
/**
 * Clears all references to all other triangles and points
 */
void Clear();
void ClearNeighbor(Triangle *triangle );
void ClearNeighbors();
void ClearDelunayEdges();

inline bool IsInterior();
inline void IsInterior(bool b);

Triangle& NeighborAcross(Point& opoint);

void DebugPrint();

//获得三角形的圆心，用于插值
void GetCircleCenter(Point*);

void GetCenter(Point*);


void GetInnerCenter(Point*);



//判断是否是需要处理的三角形
int IsFalseTri();


/// Triangle points
Point* points_[3];
/// Neighbor list
Triangle* neighbors_[3];



/// Has this triangle been marked as an interior triangle?
bool interior_;
};



//定义一个三棱柱类，其是有三角形组成，三角形又是由这个三个点组成
class TriPrism {
    Triangle *t;

    bool pointStatus[6];

    TriPrism(Triangle* m_t){
        t = m_t;
    }

    //判断是哪一层？如何生成等值面？其实我都不用生成等值面了，生成尼玛p哦。就假装生成体素吧，难受的一批。
    int TriPrismType(){
        Point *a, *b, *c;
        a = t->GetPoint(0);
        b = t->GetPoint(1);
        c = t->GetPoint(2);
    }
};

inline bool cmp(const Point* a, const Point* b)
{
  if (a->y < b->y) {
    return true;
  } else if (a->y == b->y) {
    // Make sure q is point with greater x value
    if (a->x < b->x) {
      return true;
    }
  }
  return false;
}

/// Add two points_ component-wise.
inline Point operator +(const Point& a, const Point& b)
{
  return Point(a.x + b.x, a.y + b.y);
}

/// Subtract two points_ component-wise.
inline Point operator -(const Point& a, const Point& b)
{
  return Point(a.x - b.x, a.y - b.y);
}

/// Multiply point by scalar
inline Point operator *(double s, const Point& a)
{
  return Point(s * a.x, s * a.y);
}

inline bool operator ==(const Point& a, const Point& b)
{
  return a.x == b.x && a.y == b.y;
}

inline bool operator !=(const Point& a, const Point& b)
{
  return !(a.x == b.x) && !(a.y == b.y);
}

/// Peform the dot product on two vectors.
inline double Dot(const Point& a, const Point& b)
{
  return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline double Cross(const Point& a, const Point& b)
{
  return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a point and a scalar. In 2D this produces
/// a point.
inline Point Cross(const Point& a, double s)
{
  return Point(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a point. In 2D this produces
/// a point.
inline Point Cross(const double s, const Point& a)
{
  return Point(-s * a.y, s * a.x);
}

inline Point* Triangle::GetPoint(const int& index)
{
  return points_[index];
}

inline Triangle* Triangle::GetNeighbor(const int& index)
{
  return neighbors_[index];
}

inline bool Triangle::Contains(Point* p)
{
  return p == points_[0] || p == points_[1] || p == points_[2];
}

inline bool Triangle::Contains(const Edge& e)
{
  return Contains(e.p) && Contains(e.q);
}

inline bool Triangle::Contains(Point* p, Point* q)
{
  return Contains(p) && Contains(q);
}

inline bool Triangle::IsInterior()
{
  return interior_;
}

inline void Triangle::IsInterior(bool b)
{
  interior_ = b;
}

}

#endif


