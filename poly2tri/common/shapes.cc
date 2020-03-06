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
#include "shapes.h"
#include <iostream>

namespace p2t {

Triangle::Triangle(Point& a, Point& b, Point& c)
{
  points_[0] = &a; points_[1] = &b; points_[2] = &c;
  neighbors_[0] = NULL; neighbors_[1] = NULL; neighbors_[2] = NULL;
  constrained_edge[0] = constrained_edge[1] = constrained_edge[2] = false;
  delaunay_edge[0] = delaunay_edge[1] = delaunay_edge[2] = false;
  interior_ = false;
}

void Triangle::HidePoints()
{
  points_[0]->isHide = true;
  points_[1]->isHide = true;
  points_[2]->isHide = true;
}

// Upoints_ate neighbor pointers
void Triangle::MarkNeighbor(Point* p1, Point* p2, Triangle* t)
{
  if ((p1 == points_[2] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[2]))
    neighbors_[0] = t;
  else if ((p1 == points_[0] && p2 == points_[2]) || (p1 == points_[2] && p2 == points_[0]))
    neighbors_[1] = t;
  else if ((p1 == points_[0] && p2 == points_[1]) || (p1 == points_[1] && p2 == points_[0]))
    neighbors_[2] = t;
  else
    assert(0);
}

// Exhaustive search to upoints_ate neighbor pointers
void Triangle::MarkNeighbor(Triangle& t)
{
  if (t.Contains(points_[1], points_[2])) {
    neighbors_[0] = &t;
    t.MarkNeighbor(points_[1], points_[2], this);
  } else if (t.Contains(points_[0], points_[2])) {
    neighbors_[1] = &t;
    t.MarkNeighbor(points_[0], points_[2], this);
  } else if (t.Contains(points_[0], points_[1])) {
    neighbors_[2] = &t;
    t.MarkNeighbor(points_[0], points_[1], this);
  }
}

/**
 * Clears all references to all other triangles and points
 */
void Triangle::Clear()
{
    Triangle *t;
    for( int i=0; i<3; i++ )
    {
        t = neighbors_[i];
        if( t != NULL )
        {
            t->ClearNeighbor( this );
        }
    }
    ClearNeighbors();
    points_[0]=points_[1]=points_[2] = NULL;
}

void Triangle::ClearNeighbor(Triangle *triangle )
{
    if( neighbors_[0] == triangle )
    {
        neighbors_[0] = NULL;
    }
    else if( neighbors_[1] == triangle )
    {
        neighbors_[1] = NULL;            
    }
    else
    {
        neighbors_[2] = NULL;
    }
}
    
void Triangle::ClearNeighbors()
{
  neighbors_[0] = NULL;
  neighbors_[1] = NULL;
  neighbors_[2] = NULL;
}

void Triangle::ClearDelunayEdges()
{
  delaunay_edge[0] = delaunay_edge[1] = delaunay_edge[2] = false;
}

Point* Triangle::OppositePoint(Triangle& t, Point& p)
{
  Point *cw = t.PointCW(p);
  double x = cw->x;
  double y = cw->y;
  x = p.x;
  y = p.y;
  return PointCW(*cw);
}

// Legalized triangle by rotating clockwise around point(0)
void Triangle::Legalize(Point& point)
{
  points_[1] = points_[0];
  points_[0] = points_[2];
  points_[2] = &point;
}

// Legalize triagnle by rotating clockwise around oPoint
void Triangle::Legalize(Point& opoint, Point& npoint)
{
  if (&opoint == points_[0]) {
    points_[1] = points_[0];
    points_[0] = points_[2];
    points_[2] = &npoint;
  } else if (&opoint == points_[1]) {
    points_[2] = points_[1];
    points_[1] = points_[0];
    points_[0] = &npoint;
  } else if (&opoint == points_[2]) {
    points_[0] = points_[2];
    points_[2] = points_[1];
    points_[1] = &npoint;
  } else {
    assert(0);
  }
}

int Triangle::Index(const Point* p)
{
  if (p == points_[0]) {
    return 0;
  } else if (p == points_[1]) {
    return 1;
  } else if (p == points_[2]) {
    return 2;
  }
  assert(0);
}

int Triangle::EdgeIndex(const Point* p1, const Point* p2)
{
  if (points_[0] == p1) {
    if (points_[1] == p2) {
      return 2;
    } else if (points_[2] == p2) {
      return 1;
    }
  } else if (points_[1] == p1) {
    if (points_[2] == p2) {
      return 0;
    } else if (points_[0] == p2) {
      return 2;
    }
  } else if (points_[2] == p1) {
    if (points_[0] == p2) {
      return 1;
    } else if (points_[1] == p2) {
      return 0;
    }
  }
  return -1;
}

void Triangle::MarkConstrainedEdge(const int index)
{
  constrained_edge[index] = true;
}

void Triangle::MarkConstrainedEdge(Edge& edge)
{
  MarkConstrainedEdge(edge.p, edge.q);
}

// Mark edge as constrained
void Triangle::MarkConstrainedEdge(Point* p, Point* q)
{
  if ((q == points_[0] && p == points_[1]) || (q == points_[1] && p == points_[0])) {
    constrained_edge[2] = true;
  } else if ((q == points_[0] && p == points_[2]) || (q == points_[2] && p == points_[0])) {
    constrained_edge[1] = true;
  } else if ((q == points_[1] && p == points_[2]) || (q == points_[2] && p == points_[1])) {
    constrained_edge[0] = true;
  }
}
//顺时针给下一个点
// The point clockwise to given point
Point* Triangle::PointCW(Point& point)
{
  if (&point == points_[0]) {
    return points_[2];
  } else if (&point == points_[1]) {
    return points_[0];
  } else if (&point == points_[2]) {
    return points_[1];
  }
  assert(0);
}
//逆时针给出下一个点
// The point counter-clockwise to given point
Point* Triangle::PointCCW(Point& point)
{
  if (&point == points_[0]) {
    return points_[1];
  } else if (&point == points_[1]) {
    return points_[2];
  } else if (&point == points_[2]) {
    return points_[0];
  }
  assert(0);
}

// The neighbor clockwise to given point
Triangle* Triangle::NeighborCW(Point& point)
{
  if (&point == points_[0]) {
    return neighbors_[1];
  } else if (&point == points_[1]) {
    return neighbors_[2];
  }
  return neighbors_[0];
}

//反方向计算相邻点
// The neighbor counter-clockwise to given point
Triangle* Triangle::NeighborCCW(Point& point)
{
  if (&point == points_[0]) {
    return neighbors_[2];
  } else if (&point == points_[1]) {
    return neighbors_[0];
  }
  return neighbors_[1];
}

bool Triangle::GetConstrainedEdgeCCW(Point& p)
{
  if (&p == points_[0]) {
    return constrained_edge[2];
  } else if (&p == points_[1]) {
    return constrained_edge[0];
  }
  return constrained_edge[1];
}

bool Triangle::GetConstrainedEdgeCW(Point& p)
{
  if (&p == points_[0]) {
    return constrained_edge[1];
  } else if (&p == points_[1]) {
    return constrained_edge[2];
  }
  return constrained_edge[0];
}

void Triangle::SetConstrainedEdgeCCW(Point& p, bool ce)
{
  if (&p == points_[0]) {
    constrained_edge[2] = ce;
  } else if (&p == points_[1]) {
    constrained_edge[0] = ce;
  } else {
    constrained_edge[1] = ce;
  }
}

void Triangle::SetConstrainedEdgeCW(Point& p, bool ce)
{
  if (&p == points_[0]) {
    constrained_edge[1] = ce;
  } else if (&p == points_[1]) {
    constrained_edge[2] = ce;
  } else {
    constrained_edge[0] = ce;
  }
}

bool Triangle::GetDelunayEdgeCCW(Point& p)
{
  if (&p == points_[0]) {
    return delaunay_edge[2];
  } else if (&p == points_[1]) {
    return delaunay_edge[0];
  }
  return delaunay_edge[1];
}

bool Triangle::GetDelunayEdgeCW(Point& p)
{
  if (&p == points_[0]) {
    return delaunay_edge[1];
  } else if (&p == points_[1]) {
    return delaunay_edge[2];
  }
  return delaunay_edge[0];
}

void Triangle::SetDelunayEdgeCCW(Point& p, bool e)
{
  if (&p == points_[0]) {
    delaunay_edge[2] = e;
  } else if (&p == points_[1]) {
    delaunay_edge[0] = e;
  } else {
    delaunay_edge[1] = e;
  }
}

void Triangle::SetDelunayEdgeCW(Point& p, bool e)
{
  if (&p == points_[0]) {
    delaunay_edge[1] = e;
  } else if (&p == points_[1]) {
    delaunay_edge[2] = e;
  } else {
    delaunay_edge[0] = e;
  }
}

// The neighbor across to given point
Triangle& Triangle::NeighborAcross(Point& opoint)
{
  if (&opoint == points_[0]) {
    return *neighbors_[0];
  } else if (&opoint == points_[1]) {
    return *neighbors_[1];
  }
  return *neighbors_[2];
}

void Triangle::DebugPrint()
{
  using namespace std;
  cout << points_[0]->x << "," << points_[0]->y << "," << points_[0]->z << " ";
  cout << points_[1]->x << "," << points_[1]->y << "," << points_[1]->z << " ";
  cout << points_[2]->x << "," << points_[2]->y << "," << points_[2]->z << endl;
}


//获取三角形的外接圆，这里可能是求得这个圆中心了，不太对的样子

//这里没写
void Triangle::GetCircleCenter(Point* center){
    if(center == nullptr)
        return;
    double a1, b1, c1, d1;
    double a2, b2, c2, d2;
    double a3, b3, c3, d3;
  
    double x1 = points_[0]->x, y1 = points_[0]->y, z1 = points_[0]->z;
    double x2 = points_[1]->x, y2 = points_[1]->y, z2 = points_[1]->z;
    double x3 = points_[2]->x, y3 = points_[2]->y, z3 = points_[2]->z;
  
    a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
    b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
    c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
    d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);
  
    a2 = 2 * (x2 - x1);
    b2 = 2 * (y2 - y1);
    c2 = 2 * (z2 - z1);
    d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;
  
    a3 = 2 * (x3 - x1);
    b3 = 2 * (y3 - y1);
    c3 = 2 * (z3 - z1);
    d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

    center->x = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)
                     /(a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
    center->y =  (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)
                      /(a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
    center->z = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)
                     /(a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);

}


//这个是求三角形的重心
void Triangle::GetCenter(p2t::Point *center) {
    if(center == nullptr)
      return;
    center->x = (points_[0]->x + points_[1]->x + points_[2]->x) / 3.0;
    center->y = (points_[0]->y + points_[1]->y + points_[2]->y) / 3.0;
    center->z = (points_[0]->z + points_[1]->z + points_[2]->z) / 3.0;
}

void Triangle::GetInnerCenter(Point* center){
    double a = points_[0]->Distance(points_[1]);
    double b = points_[1]->Distance(points_[2]);
    double c = points_[2]->Distance(points_[0]);

//    cout << a << " " << b << " " << c << endl;
    double s = a + b + c;
    //难道这个z也是这样算？我日了，好像尼玛真的是可以这样算啊，我怎么感觉是可以这样算的，应该是没毛病？要不自己试一下看看。
    center->x = (a * points_[0]->x + b * points_[1]->x + c * points_[2]->x) / s;
    center->y = (a * points_[0]->y + b * points_[1]->y + c * points_[2]->y) / s;
    center->z = (a * points_[0]->z + b * points_[1]->z + c * points_[2]->z) / s;



//    var AB = Vector3.Normalize(B - A);
//    var AC = Vector3.Normalize(C - A);
//    var bisectADir = AB + AC;
//    var BA = Vector3.Normalize(A - B);
//    var BC = Vector3.Normalize(C - B);
//    var bisectBDir =BA + BC;
//    // find intersection => center point of incirclevar
//    center = LineLineIntersection(A, bisectADir, B, bisectBDir);
//    //这里看起来是有点难顶啊，尼玛还这样实现了
//    // find distance to any side => radiusvar
//    radius = PointLineDistance(center, A, B - A);




}


//设计一种状态机？好像也不太行，这个就是感觉算是自己的核心公式了？也是难顶，就这几行代码也好意思说是三棱柱法，太难了老铁。
int Triangle::IsFalseTri() {

  if (0 == points_[0]->isHole && 0 == points_[1]->isHole && 0 == points_[2]->isHole)
    return IsBaseTri;
  //这里会出现那种情况，就是有两个0？
  else if((0 == points_[0]->isHole && (points_[1]->isHole != points_[2]->isHole))
       || (0 == points_[1]->isHole && (points_[0]->isHole != points_[2]->isHole))
       || (0 == points_[2]->isHole && (points_[0]->isHole != points_[1]->isHole))){
      int cnt = 0;
      if(0 == points_[0]->isHole)
          cnt++;
      if(0 == points_[1]->isHole)
          cnt++;
      if(0 == points_[2]->isHole)
          cnt++;
      if(cnt == 1)
          return IsMidTri;
  }
  else if(points_[0]->isHole > 0 && points_[1]->isHole > 0 && points_[2]->isHole > 0)
    return IsTopTri;

  return 0;
}


//三维空间点到线段的距离。
float shortDistance(Point line_point1, Point line_point2,
                        Point point)
    {
        Point AB = line_point2 - line_point1;
        Point AC = point - line_point1;
        double area = Point(AB * AC).magnitude();
        double CD = area / AB.magnitude();
        return CD;
    }

}

