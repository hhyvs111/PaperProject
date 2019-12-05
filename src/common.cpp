//
// Created by 谭文波 on 2019/12/1.
//

#include "common.h"

int Line2D::operator==(Line2D &line)
{
	int temp1,temp2;
	temp1=HasVertex(line.m_x1,line.m_y1);
	temp2=HasVertex(line.m_x2,line.m_y2);
	if(temp1==1 && temp2==2)
	{
		return 1;
	}
	else if(temp1==2 && temp2==1)
	{
		return -1;
	}
	else
		return 0;
}
int Line2D::operator!=(Line2D &line)
{
	if(HasVertex(line.m_x1,line.m_y1)+
	   HasVertex(line.m_x2,line.m_y2)==3)
	{
		return 0;
	}
	else
		return 1;
}

int Line2D::HasVertex(double x,double y)
{
	double temp;
	temp=0;
	temp+=(m_x1-x)*(m_x1-x);
	temp+=(m_y1-y)*(m_y1-y);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 1;
	temp=0;
	temp+=(m_x2-x)*(m_x2-x);
	temp+=(m_y2-y)*(m_y2-y);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 2;
	return 0;
}
int Line2D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1]);
}
int Line2D::HasVertex(Point2D &point)
{
	return HasVertex(point.m_x,point.m_y);
}
int Line2D::Contain(double x,double y)
{
	double temp1,temp2,temp3;                 //临时变量
	double a[2],b[2],p[2];                    //临时变量,表示空间点

	Get(a,b);
	p[0]=x,p[1]=y;

	temp1=0;
	for(int i=0;i<2;i++)
		temp1+=(a[i]-p[i])*(a[i]-p[i]);
	temp1=sqrt(temp1);
	if(temp1<THRESHOLD10)
		return 1;

	temp2=0;
	for(int i=0;i<2;i++)
		temp2+=(b[i]-p[i])*(b[i]-p[i]);
	temp2=sqrt(temp2);
	if(temp2<THRESHOLD10)
		return 2;

	temp3=0;
	for(int i=0;i<2;i++)
		temp3+=(b[i]-a[i])*(b[i]-a[i]);
	temp3=sqrt(temp3);
	if(temp3<THRESHOLD10)
		return 0;

    if(temp1+temp2<temp3+THRESHOLD10)            //|ap|+|bp|<|ab|+THRESHOLD10
		return 3;
	return 0;
}

int Line2D::Contain(double a[])
{
	return Contain(a[0],a[1]);
}
int Line2D::Contain(Point2D &point)
{
	return Contain(point.m_x,point.m_y);
}
int Line3D::operator==(Line2D &line)
{
	return (line==(*this));
}
int Line3D::operator==(Line3D &line)
{
	int temp1,temp2;
	temp1=HasVertex(line.m_x1,line.m_y1,line.m_z1);
	temp2=HasVertex(line.m_x2,line.m_y2,line.m_z2);
	if(temp1==1 && temp2==2)
	{
		return 1;
	}
	else if(temp1==2 && temp2==1)
	{
		return -1;
	}
	else
		return 0;
}
int Line3D::operator!=(Line2D &line)
{
	return (line!=(*this));
}
int Line3D::operator!=(Line3D &line)
{
	if(HasVertex(line.m_x1,line.m_y1,line.m_z1)+
		HasVertex(line.m_x2,line.m_y2,line.m_z2)==3)
	{
		return 0;
	}
	else
		return 1;
}

int Line3D::HasVertex(double x,double y,double z)
{
	double temp;
	temp=0;
	temp+=(m_x1-x)*(m_x1-x);
	temp+=(m_y1-y)*(m_y1-y);
	temp+=(m_z1-z)*(m_z1-z);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 1;
	temp=0;
	temp+=(m_x2-x)*(m_x2-x);
	temp+=(m_y2-y)*(m_y2-y);
	temp+=(m_z2-z)*(m_z2-z);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 2;
	return 0;
}
int Line3D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1],a[2]);
}
int Line3D::HasVertex(Point3D &point)
{
	return HasVertex(point.m_x,point.m_y,point.m_z);
}
int Line3D::Contain(double x,double y,double z)
{
	double temp1,temp2,temp3;                 //临时变量
	double a[3],b[3],p[3];                    //临时变量,表示空间点

	Get(a,b);
	p[0]=x,p[1]=y,p[2]=z;

	temp1=0;
	for(int i=0;i<3;i++)
		temp1+=(a[i]-p[i])*(a[i]-p[i]);
	temp1=sqrt(temp1);
	if(temp1<THRESHOLD10)
		return 1;

	temp2=0;
	for(int i=0;i<3;i++)
		temp2+=(b[i]-p[i])*(b[i]-p[i]);
	temp2=sqrt(temp2);
	if(temp2<THRESHOLD10)
		return 2;

	temp3=0;
	for(int i=0;i<3;i++)
		temp3+=(b[i]-a[i])*(b[i]-a[i]);
	temp3=sqrt(temp3);
	if(temp3<THRESHOLD10)
		return 0;

    if(temp1+temp2<temp3+THRESHOLD10)            //|ap|+|bp|<|ab|+THRESHOLD10
		return 3;
	return 0;
}

int Line3D::Contain(double a[])
{
	return Contain(a[0],a[1],a[2]);
}
int Line3D::Contain(Point3D &point)
{
	return Contain(point.m_x,point.m_y,point.m_z );
}
//int Line4D::operator==(Line3D &line)
//{
//	return (line==(*this));
//}
//int Line4D::operator==(Line4D &line)
//{
//	int temp1,temp2;
//	temp1=HasVertex(line.m_x1,line.m_y1,line.m_z1,line.m_w1 );
//	temp2=HasVertex(line.m_x2,line.m_y2,line.m_z2,line.m_w2 );
//	if(temp1==1 && temp2==2)
//	{
//		return 1;
//	}
//	else if(temp1==2 && temp2==1)
//	{
//		return -1;
//	}
//	else
//		return 0;
//}
//int Line4D::operator!=(Line3D &line)
//{
//	return (line!=(*this));
//}
//int Line4D::operator!=(Line4D &line)
//{
//	if(HasVertex(line.m_x1,line.m_y1,line.m_z1,line.m_w1 )+
//		HasVertex(line.m_x2,line.m_y2,line.m_z2,line.m_w2 )==3)
//	{
//		return 0;
//	}
//	else
//		return 1;
//}
//
//int Line4D::HasVertex(double x,double y,double z,double w)
//{
//	double temp;
//	temp=0;
//	temp+=(m_x1-x)*(m_x1-x);
//	temp+=(m_y1-y)*(m_y1-y);
//	temp+=(m_z1-z)*(m_z1-z);
//	temp+=(m_w1-w)*(m_w1-w);
//	temp=sqrt(temp);
//	if(temp<THRESHOLD10)
//		return 1;
//	temp=0;
//	temp+=(m_x2-x)*(m_x2-x);
//	temp+=(m_y2-y)*(m_y2-y);
//	temp+=(m_z2-z)*(m_z2-z);
//	temp+=(m_w2-w)*(m_w2-w);
//	temp=sqrt(temp);
//	if(temp<THRESHOLD10)
//		return 2;
//	return 0;
//}
//int Line4D::HasVertex(double a[])
//{
//	return HasVertex(a[0],a[1],a[2],a[3]);
//}
//int Line4D::HasVertex(Point4D &point)
//{
//	return HasVertex(point.m_x,point.m_y,point.m_z,point.m_w);
//}
//int Line4D::Contain(double x,double y,double z,double w)
//{
//	double temp1,temp2,temp3;                 //临时变量
//	double a[4],b[4],p[4];                    //临时变量,表示空间点
//
//	Get(a,b);
//	p[0]=x,p[1]=y,p[2]=z,p[3]=w;
//
//	temp1=0;
//	for(int i=0;i<4;i++)
//		temp1+=(a[i]-p[i])*(a[i]-p[i]);
//	temp1=sqrt(temp1);
//	if(temp1<THRESHOLD10)
//		return 1;
//
//	temp2=0;
//	for(int i=0;i<4;i++)
//		temp2+=(b[i]-p[i])*(b[i]-p[i]);
//	temp2=sqrt(temp2);
//	if(temp2<THRESHOLD10)
//		return 2;
//
//	temp3=0;
//	for(int i=0;i<4;i++)
//		temp3+=(b[i]-a[i])*(b[i]-a[i]);
//	temp3=sqrt(temp3);
//	if(temp3<THRESHOLD10)
//		return 0;
//
//    if(temp1+temp2<temp3+THRESHOLD10)            //|ap|+|bp|<|ab|+THRESHOLD10
//		return 3;
//	return 0;
//}
//
//int Line4D::Contain(double a[])
//{
//	return Contain(a[0],a[1],a[2],a[3]);
//}
//int Line4D::Contain(Point4D &point)
//{
//	return Contain(point.m_x,point.m_y,point.m_z,point.m_w);
//}
int Triangle2D::operator==(Triangle2D &triangle)
{
	int Loc[3];
	Loc[0]=HasVertex(triangle.m_x1, triangle.m_y1);
	Loc[1]=HasVertex(triangle.m_x2, triangle.m_y2);
	Loc[2]=HasVertex(triangle.m_x3, triangle.m_y3);
	if(Loc[0]+Loc[1]+Loc[2]==6)
	{
		return 1;
	}
	else
		return 0;
}
int Triangle2D::operator!=(Triangle2D &triangle)
{
	int Loc[3];
	Loc[0]=HasVertex(triangle.m_x1, triangle.m_y1);
	Loc[1]=HasVertex(triangle.m_x2, triangle.m_y2);
	Loc[2]=HasVertex(triangle.m_x3, triangle.m_y3);
	if(Loc[0]+Loc[1]+Loc[2]==6)
	{
		return 0;
	}
	else
		return 1;
}
int Triangle2D::HasVertex(double x,double y)
{
	double temp;

	temp=0;
	temp+=(m_x1-x)*(m_x1-x);
	temp+=(m_y1-y)*(m_y1-y);
	if(sqrt(temp)<THRESHOLD10)
		return 1;

	temp=0;
	temp+=(m_x2-x)*(m_x2-x);
	temp+=(m_y2-y)*(m_y2-y);
	if(sqrt(temp)<THRESHOLD10)
		return 2;

	temp=0;
	temp+=(m_x3-x)*(m_x3-x);
	temp+=(m_y3-y)*(m_y3-y);
	if(sqrt(temp)<THRESHOLD10)
		return 3;

	return 0;
}
int Triangle2D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1]);
}
int Triangle2D::HasVertex(Point2D &point)
{
	return HasVertex(point.m_x,point.m_y);
}
int Triangle2D::Contain(double x,double y)
{
	double a[2],b[2],c[2];
	double p1[2],p2[2],p[2];
	double temp,temp1,temp2;

	Get(a,b,c);
	p[0]=x,p[1]=y;

	if(x<a[0]-THRESHOLD8 && x<b[0]-THRESHOLD8 && x<c[0]-THRESHOLD8)
		return 0;
	if(y<a[1]-THRESHOLD8 && y<b[1]-THRESHOLD8 && y<c[1]-THRESHOLD8)
		return 0;

	if(x>a[0]+THRESHOLD8 && x>b[0]+THRESHOLD8 && x>c[0]+THRESHOLD8)
		return 0;
	if(y>a[1]+THRESHOLD8 && y>b[1]+THRESHOLD8 && y>c[1]+THRESHOLD8)
		return 0;

	temp=0;
	for(int i=0;i<2;i++)
		temp+=(a[i]-p[i])*(a[i]-p[i]);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                              //p为三角形abc的顶点a
		return 1;
	temp=0;
	for(int i=0;i<2;i++)
		temp+=(b[i]-p[i])*(b[i]-p[i]);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                              //p为三角形abc的顶点b
		return 2;
	temp=0;
	for(int i=0;i<2;i++)
		temp+=(c[i]-p[i])*(c[i]-p[i]);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                              //p为三角形abc的顶点c
		return 3;

	Line2D Line;
	Line.Set(a,b);
	if(Line.Contain(p)==3)
		return 4;                                     //p点在ab上
	Line.Set(b,c);
	if(Line.Contain(p)==3)
		return 5;                                     //p点在bc上
	Line.Set(a,c);
	if(Line.Contain(p)==3)
		return 6;                                     //p点在ac上

	/////////////////////////////////////////
	///////开始判断p点是否在三角形内部

	temp=Vector2D::CrossProduct (b[0]-a[0],b[1]-a[1],
								c[0]-a[0],c[1]-a[1]);
	if(abs(temp)<THRESHOLD10)                         //三角形abc的一顶角太小或顶点间距离太小
		return 0;

	temp1=Vector2D::CrossProduct(b[0]-a[0],b[1]-a[1],
								 p[0]-a[0],p[1]-a[1]);//ab与ap的叉积
	if(abs(temp1)<THRESHOLD10)
		return 0;
	temp2=Vector2D::CrossProduct(p[0]-a[0],p[1]-a[1],
								c[0]-a[0],c[1]-a[1]); //ap与ac的叉积
	if(abs(temp2)<THRESHOLD10)
		return 0;

	temp=temp1*temp2;
	if(temp<=0)                                       //p不在ab和ac之间
		return 0;

	temp1=Vector2D::CrossProduct(a[0]-b[0],a[1]-b[1],
		            p[0]-b[0],p[1]-b[1]);             //ba与bp的叉积
	if(abs(temp1)<THRESHOLD10)
		return 0;
	temp2=Vector2D::CrossProduct(p[0]-b[0],p[1]-b[1],
	                c[0]-b[0],c[1]-b[1]);             //bp与bc的叉积
	if(abs(temp2)<THRESHOLD10)
		return 0;

	temp=temp1*temp2;
	if(temp<=0)                                    //p不在ba和bc之间
		return 0;
	return 7;                                      //p点在三角形内部
	//判断p是否在三角形abc内的处理结束
}
int Triangle2D::Contain(double a[])
{
	return Contain(a[0],a[1]);
}
int Triangle2D::Contain(Point2D &point)
{
	return Contain(point.m_x,point.m_y);
}
int Triangle2D::HasEdge(double x1,double y1,
						   double x2,double y2)
{
	double temp;
	int index1,index2;
	temp =(x2-x1)*(x2-x1);
	temp+=(y2-y1)*(y2-y1);
	if(sqrt(temp)<THRESHOLD10)                               //确保两点不同
		return 0;
	index1=HasVertex(x1,y1);
	index2=HasVertex(x2,y2);
	if((index1==1)&&(index2==2))
		return 1;
	if((index1==2)&&(index2==1))
		return -1;
	if((index1==2)&&(index2==3))
		return 2;
	if((index1==3)&&(index2==2))
		return -2;
	if((index1==3)&&(index2==1))
		return 3;
	if((index1==1)&&(index2==3))
		return -3;
	return 0;
}
int Triangle2D::HasEdge(double a[],double b[])
{
	return HasEdge(a[0],a[1],b[0],b[1]);
}
int Triangle2D::HasEdge(Point2D &point1,Point2D &point2)
{
	double a[2],b[2];
	point1.Get(a);
	point2.Get(b);
	return HasEdge(a[0],a[1],b[0],b[1]);
}
int Triangle2D::HasEdge(Line2D &line)
{
	double a[2],b[2];
	line.Get(a,b);
	return HasEdge(a[0],a[1],b[0],b[1]);
}
int Triangle3D::operator==(Triangle2D &triangle)
{
	return triangle==(*this);
}
int Triangle3D::operator==(Triangle3D &triangle)
{
	int Loc[3];
	Loc[0]=HasVertex(triangle.m_x1, triangle.m_y1,triangle.m_z1);
	Loc[1]=HasVertex(triangle.m_x2, triangle.m_y2,triangle.m_z2);
	Loc[2]=HasVertex(triangle.m_x3, triangle.m_y3,triangle.m_z3);
	if(Loc[0]+Loc[1]+Loc[2]==6)
	{
		return 1;
	}
	else
		return 0;
}
int Triangle3D::operator!=(Triangle2D &triangle)
{
	return triangle!=(*this);
}
int Triangle3D::operator!=(Triangle3D &triangle)
{
	int Loc[3];
	Loc[0]=HasVertex(triangle.m_x1, triangle.m_y1,triangle.m_z1);
	Loc[1]=HasVertex(triangle.m_x2, triangle.m_y2,triangle.m_z2);
	Loc[2]=HasVertex(triangle.m_x3, triangle.m_y3,triangle.m_z3);
	if(Loc[0]+Loc[1]+Loc[2]==6)
	{
		return 0;
	}
	else
		return 1;
}
int Triangle3D::HasVertex(double x,double y,double z)
{
	double temp;

	temp=0;
	temp+=(m_x1-x)*(m_x1-x);
	temp+=(m_y1-y)*(m_y1-y);
	temp+=(m_z1-z)*(m_z1-z);
	if(temp<THRESHOLD10)
	//if(sqrt(temp)<THRESHOLD10)
		return 1;

	temp=0;
	temp+=(m_x2-x)*(m_x2-x);
	temp+=(m_y2-y)*(m_y2-y);
	temp+=(m_z2-z)*(m_z2-z);
	if(temp<THRESHOLD10)
	//if(sqrt(temp)<THRESHOLD10)
		return 2;

	temp=0;
	temp+=(m_x3-x)*(m_x3-x);
	temp+=(m_y3-y)*(m_y3-y);
	temp+=(m_z3-z)*(m_z3-z);
	if(temp<THRESHOLD10)
	//if(sqrt(temp)<THRESHOLD10)
		return 3;

	return 0;
}
int Triangle3D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1],a[2]);
}
int Triangle3D::HasVertex(Point3D &point)
{
	return HasVertex(point.m_x,point.m_y,point.m_z);
}
int Triangle3D::Contain(double x,double y,double z)
{
	double a[3],b[3],c[3];
	double p1[3],p2[3],p[3];
	double temp;

	Get(a,b,c);
	p[0]=x,p[1]=y,p[2]=z;

	temp=0;
	for(int i=0;i<3;i++)
		temp+=(a[i]-p[i])*(a[i]-p[i]);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                              //p为三角形abc的顶点a
		return 1;
	temp=0;
	for(int i=0;i<3;i++)
		temp+=(b[i]-p[i])*(b[i]-p[i]);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                              //p为三角形abc的顶点b
		return 2;
	temp=0;
	for(int i=0;i<3;i++)
		temp+=(c[i]-p[i])*(c[i]-p[i]);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                              //p为三角形abc的顶点c
		return 3;

	Line3D Line;
	Line.Set(a,b);
	if(Line.Contain(p)==3)
		return 4;                                  //p点在ab上
	Line.Set(b,c);
	if(Line.Contain(p)==3)
		return 5;                                  //p点在bc上
	Line.Set(a,c);
	if(Line.Contain(p)==3)
		return 6;                                  //p点在ac上

	/////////////////////////////////////////
	///////开始判断p点是否在三角形内部

	Vector3D v;
	v=v.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
		             c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
	temp=v.SquareLength ();
	temp=sqrt(temp);                                //法向量的模
	if(temp<THRESHOLD10)                            //三角形abc的一顶角太小或顶点间距离太小
		return 0;

	v = v.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
		            p[0]-a[0],p[1]-a[1],p[2]-a[2]); //ab与ap的叉积
	v.Get(p1);
	temp=v.SquareLength(p1);
	if(sqrt(temp)<THRESHOLD10)
		return 0;
	v.Normalize(p1);
	v=v.CrossProduct(p[0]-a[0],p[1]-a[1],p[2]-a[2],
	                c[0]-a[0],c[1]-a[1],c[2]-a[2]); //ap与ac的叉积
	v.Get(p2);
	temp=v.SquareLength(p2);
	if(sqrt(temp)<THRESHOLD10)
		return 0;
	v.Normalize(p2);

	temp=0;
	for(int i=0;i<3;i++)
		temp+=p1[i]*p2[i];
	if(temp<THRESHOLD10)                            //p不在ab和ac之间
		return 0;

	//////////////////////////////////////////////////////////
	v=v.CrossProduct(a[0]-b[0],a[1]-b[1],a[2]-b[2],
		            p[0]-b[0],p[1]-b[1],p[2]-b[2]); //ba与bp的叉积
	v.Get(p1);
	temp=v.SquareLength(p1);
	if(sqrt(temp)<THRESHOLD10)
		return 0;
	v.Normalize(p1);
	v=v.CrossProduct(p[0]-b[0],p[1]-b[1],p[2]-b[2],
	                c[0]-b[0],c[1]-b[1],c[2]-b[2]); //bp与bc的叉积
	v.Get(p2);
	temp=v.SquareLength(p2);
	if(sqrt(temp)<THRESHOLD10)
		return 0;
	v.Normalize(p2);

	temp=0;
	for(int i=0;i<3;i++)
		temp+=p1[i]*p2[i];
	if(temp<THRESHOLD10)                            //p不在ba和bc之间
		return 0;
	return 7;                                       //p点在三角形内部
	//判断p是否在三角形abc内的处理结束
}
int Triangle3D::Contain(double a[])
{
	return Contain(a[0],a[1],a[2]);
}
int Triangle3D::Contain(Point3D &point)
{
	return Contain(point.m_x,point.m_y,point.m_z);
}
int Triangle3D::HasEdge(double x1,double y1,double z1,
						   double x2,double y2,double z2)
{
	double temp;
	int index1,index2;
	temp =(x2-x1)*(x2-x1);
	temp+=(y2-y1)*(y2-y1);
	temp+=(z2-z1)*(z2-z1);
	if(temp<THRESHOLD10)
	//if(sqrt(temp)<THRESHOLD10)                               //确保两点不同
		return 0;
	index1=HasVertex(x1,y1,z1);
	index2=HasVertex(x2,y2,z2);
	if((index1==1)&&(index2==2))
		return 1;
	if((index1==2)&&(index2==1))
		return -1;
	if((index1==2)&&(index2==3))
		return 2;
	if((index1==3)&&(index2==2))
		return -2;
	if((index1==3)&&(index2==1))
		return 3;
	if((index1==1)&&(index2==3))
		return -3;
	return 0;
}
int Triangle3D::HasEdge(double a[],double b[])
{
	return HasEdge(a[0],a[1],a[2],b[0],b[1],b[2]);
}
int Triangle3D::HasEdge(Point3D &point1,Point3D &point2)
{
	double a[3],b[3];
	point1.Get(a);
	point2.Get(b);
	return HasEdge(a[0],a[1],a[2],b[0],b[1],b[2]);
}
int Triangle3D::HasEdge(Line3D &line)
{
	double a[3],b[3];
	line.Get(a,b);
	return HasEdge(a[0],a[1],a[2],b[0],b[1],b[2]);
}
int Triangle3D::GetNormal(double a[])
{
	Vector3D v;
	double   p1[3],p2[3];

	p1[0]=m_x2-m_x1,p1[1]=m_y2-m_y1,p1[2]=m_z2-m_z1;
	p2[0]=m_x3-m_x2,p2[1]=m_y3-m_y2,p2[2]=m_z3-m_z2;
	v=v.CrossProduct(p1,p2);
	if(v.Normalize())
	{
	    v.Get(a);
		return 1;
	}
	else
	{
		v.Set (0,0,0);
		v.Get (a);
	}
	return 0;
}


Tetrahedron3D::Tetrahedron3D(double x1,double y1,double z1,
		                     double x2,double y2,double z2,
			                 double x3,double y3,double z3,
							 double x4,double y4,double z4)
{
	m_x1=x1,m_y1=y1,m_z1=z1;
	m_x2=x2,m_y2=y2,m_z2=z2;
	m_x3=x3,m_y3=y3,m_z3=z3;
	m_x4=x4,m_y4=y4,m_z4=z4;
}
Tetrahedron3D::Tetrahedron3D(double a[],double b[],double c[],double d[])
{
	m_x1=a[0],m_y1=a[1],m_z1=a[2];
	m_x2=b[0],m_y2=b[1],m_z2=b[2];
	m_x3=c[0],m_y3=c[1],m_z3=c[2];
	m_x4=d[0],m_y4=d[1],m_z4=d[2];
}
Tetrahedron3D::Tetrahedron3D(Point3D &point1,Point3D &point2,
							 Point3D &point3,Point3D &point4)
{
	double a[3],b[3],c[3],d[3];
	point1.Get(a);
	point2.Get(b);
	point3.Get(c);
	point4.Get(d);
	m_x1=a[0],m_y1=a[1],m_z1=a[2];
	m_x2=b[0],m_y2=b[1],m_z2=b[2];
	m_x3=c[0],m_y3=c[1],m_z3=c[2];
	m_x4=d[0],m_y4=d[1],m_z4=d[2];
}
Tetrahedron3D::Tetrahedron3D(Tetrahedron3D &objTetrahedron)
{
	m_ID=objTetrahedron.m_ID;
	m_x1=objTetrahedron.m_x1;
	m_y1=objTetrahedron.m_y1;
	m_z1=objTetrahedron.m_z1;
	m_x2=objTetrahedron.m_x2;
	m_y2=objTetrahedron.m_y2;
	m_z2=objTetrahedron.m_z2;
	m_x3=objTetrahedron.m_x3;
	m_y3=objTetrahedron.m_y3;
	m_z3=objTetrahedron.m_z3;
	m_x4=objTetrahedron.m_x4;
	m_y4=objTetrahedron.m_y4;
	m_z4=objTetrahedron.m_z4;
}
Tetrahedron3D Tetrahedron3D::operator=(Tetrahedron3D &objTetrahedron)
{
	m_ID=objTetrahedron.m_ID;
	m_x1=objTetrahedron.m_x1;
	m_y1=objTetrahedron.m_y1;
	m_z1=objTetrahedron.m_z1;
	m_x2=objTetrahedron.m_x2;
	m_y2=objTetrahedron.m_y2;
	m_z2=objTetrahedron.m_z2;
	m_x3=objTetrahedron.m_x3;
	m_y3=objTetrahedron.m_y3;
	m_z3=objTetrahedron.m_z3;
	m_x4=objTetrahedron.m_x4;
	m_y4=objTetrahedron.m_y4;
	m_z4=objTetrahedron.m_z4;
	return *this;
}
int Tetrahedron3D::operator==(Tetrahedron3D &objTetrahedron)
{
	double a[3],b[3],c[3],d[3];
	objTetrahedron.Get(a,b,c,d);
	if(HasFacet(a,b,c) && HasVertex(d))
	{
		return 1;
	}
	else
		return 0;
}
int Tetrahedron3D::operator!=(Tetrahedron3D &objTetrahedron)
{
	double a[3],b[3],c[3],d[3];
	objTetrahedron.Get(a,b,c,d);
	if(HasFacet(a,b,c) && HasVertex(d))
	{
		return 0;
	}
	else
		return 1;
}
int Tetrahedron3D::HasVertex(double x1,double y1,double z1)
{
	double temp;
	temp=0;
	temp+=(m_x1-x1)*(m_x1-x1);
	temp+=(m_y1-y1)*(m_y1-y1);
	temp+=(m_z1-z1)*(m_z1-z1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 1;
	temp=0;
	temp+=(m_x2-x1)*(m_x2-x1);
	temp+=(m_y2-y1)*(m_y2-y1);
	temp+=(m_z2-z1)*(m_z2-z1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 2;
	temp=0;
	temp+=(m_x3-x1)*(m_x3-x1);
	temp+=(m_y3-y1)*(m_y3-y1);
	temp+=(m_z3-z1)*(m_z3-z1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 3;
	temp=0;
	temp+=(m_x4-x1)*(m_x4-x1);
	temp+=(m_y4-y1)*(m_y4-y1);
	temp+=(m_z4-z1)*(m_z4-z1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 4;
	return 0;
}
int Tetrahedron3D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1],a[2]);
}
int Tetrahedron3D::HasVertex(Point3D &point)
{
	double a[3];
	point.Get(a);
	return HasVertex(a);
}
int Tetrahedron3D::HasArris(double x1,double y1,double z1,
						    double x2,double y2,double z2)
{
	double temp;
	int index1,index2;
	temp =(x2-x1)*(x2-x1);
	temp+=(y2-y1)*(y2-y1);
	temp+=(z2-z1)*(z2-z1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)               //确保两点不同
		return 0;

	index1=HasVertex(x1,y1,z1);
	index2=HasVertex(x2,y2,z2);
	if((index1==1)&&(index2==2))
		return 1;
	if((index1==2)&&(index2==1))
		return -1;
	if((index1==2)&&(index2==3))
		return 2;
	if((index1==3)&&(index2==2))
		return -2;
	if((index1==3)&&(index2==1))
		return 3;
	if((index1==1)&&(index2==3))
		return -3;
	if((index1==1)&&(index2==4))
		return 4;
	if((index1==4)&&(index2==1))
		return -4;
	if((index1==2)&&(index2==4))
		return 5;
	if((index1==4)&&(index2==2))
		return -5;
	if((index1==3)&&(index2==4))
		return 6;
	if((index1==4)&&(index2==3))
		return -6;
	return 0;
}
int Tetrahedron3D::HasArris(double a[],double b[])
{
	return HasArris(a[0],a[1],a[2],b[0],b[1],b[2]);
}
int Tetrahedron3D::HasArris(Point3D &point1,Point3D &point2)
{
	return HasArris(point1.m_x,point1.m_y,point1.m_z,point2.m_x,point2.m_y,point2.m_z);
}
int Tetrahedron3D::HasFacet(double x1,double y1,double z1,
						    double x2,double y2,double z2,
						    double x3,double y3,double z3)
{
	double a[3],b[3],temp;
	Vector3D vector;

	a[0]=x2-x1,a[1]=y2-y1,a[2]=z2-z1;
	b[0]=x3-x1,b[1]=y3-y1,b[2]=z3-z1;
	vector.CrossProduct(a,b);
	vector.Get(a);
	temp=vector.SquareLength(a);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)                  //三点不能构成三角形
		return 0;

	if(HasVertex(x1,y1,z1) &&
	   HasVertex(x2,y2,z2) &&
	   HasVertex(x3,y3,z3))
	{
		return 1;
	}
	return 0;
}
int Tetrahedron3D::HasFacet(double a[],double b[],double c[])
{
	double x1,y1,z1;
	double x2,y2,z2;
	double x3,y3,z3;
	x1=a[0],y1=a[1],z1=a[2];
	x2=b[0],y2=b[1],z2=b[2];
	x3=c[0],y3=c[1],z3=c[2];
	return HasFacet(x1,y1,z1,x2,y2,z2,x3,y3,z3);
}
int Tetrahedron3D::HasFacet(Point3D &point1,Point3D &point2,Point3D &point3)
{
	return HasFacet(point1.m_x,point1.m_y,point1.m_z,
					point2.m_x,point2.m_y,point2.m_z,
					point3.m_x,point3.m_y,point3.m_z);
}
int Tetrahedron3D::Contain(double x1,double y1,double z1)
{
	return 0;
}
int Tetrahedron3D::Contain(double a[])
{
	double x1,y1,z1;
	x1=a[0],y1=a[1],z1=a[2];
	return Contain(x1,y1,z1);
}
int Tetrahedron3D::Contain(Point3D &point)
{
	return Contain(point.m_x,point.m_y,point.m_z);
}
void Tetrahedron3D::Set(double x1,double y1,double z1,
		                double x2,double y2,double z2,
			            double x3,double y3,double z3,
						double x4,double y4,double z4)
{
	m_x1=x1,m_y1=y1,m_z1=z1;
	m_x2=x2,m_y2=y2,m_z2=z2;
	m_x3=x3,m_y3=y3,m_z3=z3;
	m_x4=x4,m_y4=y4,m_z4=z4;
}
void Tetrahedron3D::Set(double a[],double b[],double c[],double d[])
{
	m_x1=a[0],m_y1=a[1],m_z1=a[2];
	m_x2=b[0],m_y2=b[1],m_z2=b[2];
	m_x3=c[0],m_y3=c[1],m_z3=c[2];
	m_x4=d[0],m_y4=d[1],m_z4=d[2];
}
void Tetrahedron3D::Set(Triangle3D &triangle,Point3D &point)
{
	double a[3],b[3],c[3],d[3];
	triangle.Get(a,b,c);
	point.Get(d);
	Set(a,b,c,d);
}
void Tetrahedron3D::Set(Point3D &point1,Point3D &point2,
						Point3D &point3,Point3D &point4)
{
	double a[3],b[3],c[3],d[3];
	point1.Get(a);
	point2.Get(b);
	point3.Get(c);
	point4.Get(d);
	Set(a,b,c,d);
}
void Tetrahedron3D::Get(double &x1,double &y1,double &z1,
		                double &x2,double &y2,double &z2,
			            double &x3,double &y3,double &z3,
						double &x4,double &y4,double &z4)
{
    x1=m_x1,y1=m_y1,z1=m_z1;
	x2=m_x2,y2=m_y2,z2=m_z2;
	x3=m_x3,y3=m_y3,z3=m_z3;
	x4=m_x4,y4=m_y4,z4=m_z4;
}
void Tetrahedron3D::Get(double a[],double b[],double c[],double d[])
{
		a[0]=m_x1,a[1]=m_y1,a[2]=m_z1;
		b[0]=m_x2,b[1]=m_y2,b[2]=m_z2;
		c[0]=m_x3,c[1]=m_y3,c[2]=m_z3;
		d[0]=m_x4,d[1]=m_y4,d[2]=m_z4;
}
void Tetrahedron3D::Get(Point3D &point1,Point3D &point2,
						Point3D &point3,Point3D &point4)
{
	double a[3],b[3],c[3],d[3];
	Get(a,b,c,d);
	point1.Set(a);
	point2.Set(b);
	point3.Set(c);
	point4.Set(d);
}
void Tetrahedron3D::Translate(double a[])
{
	m_x1+=a[0],m_x2+=a[0],m_x3+=a[0],m_x4+=a[0];
	m_y1+=a[1],m_y2+=a[1],m_y3+=a[1],m_x4+=a[1];
	m_z1+=a[2],m_z2+=a[2],m_z3+=a[2],m_x4+=a[2];
}


////
int Relation2D::PointInLine(double a[],double b[],double p[])
{
	double temp1,temp2,temp3;         //临时变量，表示矢量的模
	double q[3];                      //临时变量,表示空间一点
	int i;

    temp3=0;
	for(i=0;i<2;i++)
		temp3+=(b[i]-a[i])*(b[i]-a[i]);
	if(temp3<THRESHOLD10)             //求ab的模的平方
		return 0;

	temp1=0;
	for(i=0;i<2;i++)
		temp1+=(a[i]-p[i])*(a[i]-p[i]);
	if(temp1<THRESHOLD10)             //p为ab的一个端点a
		return 1;

	temp2=0;
	for(i=0;i<2;i++)
		temp2+=(b[i]-p[i])*(b[i]-p[i]);
	if(temp2<THRESHOLD10)             //p为ab的一个端点b
		return 2;

	if(temp1<temp3)                   //ap的模<ab的模
	{
		temp1=sqrt(temp1);            //求ap的模
		temp3=sqrt(temp3);            //求ab的模
		for(i=0;i<2;i++)
			q[i]=a[i]+(b[i]-a[i])*temp1/temp3;
		//ab上的点q，它使aq的模＝ap的模
		temp3=0;
		for(i=0;i<2;i++)
			temp3+=(q[i]-p[i])*(q[i]-p[i]);
		if(temp3<THRESHOLD10)         //p,q重合，p在ab上
			return 3;
	}
	return 0;
}
int Relation2D::PointInLine(Line2D &line,double p[])
{
	double a[2],b[2];
	line.Get (a,b);
	return PointInLine(a,b,p);
}

int Relation2D::PointInTriangle(double a[],double b[],double c[],double p[])
{
	////////////////////////////////////
	///不能修改a,b,c,p
	////////////////////////////////////

	Vector2D vector;
	double temp,temp1,temp2,temp3;
	double n[3];          //法向量
	double p1[3],p2[3];
	int index;            // 标识点在三角形中的位置
	int i;

	index=0;              //点不在三角形中
	temp=0;
	for(i=0;i<2;i++)
		temp+=(a[i]-p[i])*(a[i]-p[i]);
	if(temp<THRESHOLD10)  //p为三角形abc的一个顶点
		return 1;
	temp=0;
	for(i=0;i<2;i++)
		temp+=(b[i]-p[i])*(b[i]-p[i]);
	if(temp<THRESHOLD10)  //p为三角形abc的一个顶点
		return 2;
	temp=0;
	for(i=0;i<2;i++)
		temp+=(c[i]-p[i])*(c[i]-p[i]);
	if(temp<THRESHOLD10)  //p为三角形abc的一个顶点
		return 3;

	if(PointInLine(a,b,p)==3)
		return 4;         //p点在ab上
	if(PointInLine(b,c,p)==3)
		return 5;         //p点在ab上
	if(PointInLine(c,a,p)==3)
		return 6;         //p点在ab上

	/////////////////////////////////////////
	//判断p点是否在三角形内部开始

	temp=vector.CrossProduct(b[0]-a[0],b[1]-a[1],c[0]-a[0],c[1]-a[1]);
	if(abs(temp)<THRESHOLD10)  //三角形abc的一顶角太小
		return 0;

	temp1=vector.CrossProduct(b[0]-a[0],b[1]-a[1],p[0]-a[0],p[1]-a[1]);//ab与ap的叉积
	if(abs(temp1)<THRESHOLD10) //三角形abc的一顶角太小
		return 0;

	temp2=vector.CrossProduct(p[0]-a[0],p[1]-a[1],c[0]-a[0],c[1]-a[1]);//ac与ap的叉积
	if(abs(temp2)<THRESHOLD10) //三角形abc的一顶角太小
		return 0;

	temp=temp1*temp2;
	if(temp<=0)                //p不在ab和ac之间
		return 0;

	temp1=vector.CrossProduct(a[0]-b[0],a[1]-b[1],p[0]-b[0],p[1]-b[1]);
	if(abs(temp1)<THRESHOLD10) //三角形abc的一顶角太小
		return 0;
	temp2=vector.CrossProduct(p[0]-b[0],p[1]-b[1],c[0]-b[0],c[1]-b[1]);
	if(abs(temp2)<THRESHOLD10) //三角形abc的一顶角太小
		return 0;temp=temp1*temp2;
	if(temp<=0)                //p不在ab和ac之间
		return 0;
	return 7;                  //p点在三角形内部
	//判断p是否在三角形abc内的处理结束
}
int Relation2D::PointInTriangle(Triangle2D &triangle,double p[])
{
	double a[2],b[2],c[2];
	triangle.Get (a,b,c);
	return PointInTriangle(a,b,c,p);
}
int Relation2D::GetIntersectPoint(double a[], double b[],
							  		 double p1[],double p2[],double p[])
{
	///////////////////////////////////////////////////////////////////////
	//a+ab.t1=p1+p1p2.t2      平行的情况不用此公式
	//abXp1p2.t1=(p1-a)Xp1p2  t1=((p1-a)Xp1p2).(abXp1p2)/SquareLength(abXp1p2)
	//abX(a-p1)=abXp1p2t2     t2=(abX(a-p1)  ).(abXp1p2)/SquareLength(abXp1p2)
	///////////////////////////////////////////////////////////////////////
	Vector2D vector;
	Line2D   line;
	double   m[3],n[3];
	double   SquareLengthAB,SquareLengthP1P2;
	double   temp,temp1;
	int      i,index1,index2;

	//////验证两直线段的长度是否为零//////
	SquareLengthAB=vector.SquareLength(b[0]-a[0],b[1]-a[1]);
	SquareLengthP1P2=vector.SquareLength(p2[0]-p1[0],p2[1]-p1[1]);
	if(sqrt(SquareLengthAB)<THRESHOLD10 || sqrt(SquareLengthP1P2)<THRESHOLD10)
		return 0;

	////////////////////////////////////////////////////////
	temp=vector.CrossProduct(b[0]-a[0],  b[1]-a[1] ,
		               p2[0]-p1[0],p2[1]-p1[1]); //n=abXp1p2
	if(abs(temp)<THRESHOLD10)                    //两条线ab,p1p2平行  p=a+t1(b-a), t1=ap.ab/ab.ab
	{
		line.Set(a,b);
		index1=0;
		if(line.Contain(p1)==3 || line.Contain(p2)==3)
			index1=40;
		line.Set(p1,p2);
		if(line.Contain(a)==3 || line.Contain(b)==3)
			index1+=4;
		return index1;
	}
    ///////两直线段不平行时/////////////////////////////
	temp1=vector.CrossProduct(p1[0]-a[0],  p1[1]-a[1],
		               p2[0]-p1[0], p2[1]-p1[1]);//m=ap1Xp1p2
	temp1/=temp;
	for(i=0;i<2;i++)
	{
		p[i]=b[i]-a[i];
		p[i]*=temp1;
		p[i]+=a[i];
	}                                             //p为交点
	if(temp1>-THRESHOLD10 && temp1<THRESHOLD10)   //index/10表示交点在第一条线段ab上的位置
		index1=10;
	else if(temp1>1-THRESHOLD10 && temp1<1+THRESHOLD10)
		index1=20;
	else if(temp1>THRESHOLD10 && temp1<1-THRESHOLD10)
		index1=30;
	else
		return 0;

	temp1=vector.CrossProduct(b[0]-a[0], b[1]-a[1],
		               a[0]-p1[0],a[1]-p1[1]);    //m=abXap1

	temp1/=temp;
	if(temp1>-THRESHOLD10 && temp1<THRESHOLD10)   //index%10表示交点在第二条线段p1p2上的位置
		index2=1;
	else if(temp1>1-THRESHOLD10 && temp1<1+THRESHOLD10)
		index2=2;
	else if(temp1>THRESHOLD10 && temp1<1-THRESHOLD10)
		index2=3;
	else
		return 0;
	return index1+index2;
}
int Relation2D::GetIntersectPoint(Line2D &line1,Line2D &line2,double p[])
{
	double a[2],b[2],p1[2],p2[2];
	line1.Get(a,b);
	line2.Get(p1,p2);
	return GetIntersectPoint(a,b,p1,p2,p);
}
double Relation2D::PToPDistance(double a[],double b[])
{
	double temp;
	Vector2D vector;
	vector.Set (b[0]-a[0],b[1]-a[1]);
	temp=vector.SquareLength();
	temp=sqrt(temp);
	return temp;
}

double Relation2D::PToPDistance(Point2D &point1,Point2D &point2)
{
	double a[3],b[3];
	point1.Get (a);
	point2.Get (b);
	return PToPDistance(a,b);
}

double Relation2D::PToLDistance(double a[],double b[],double p[])
{
	int i;
	double temp;
	double p1[3],p2[3];
	Vector2D vector;
	for(i=0;i<2;i++)
	{
		p1[i]=b[i]-a[i];
		p2[i]=p[i]-a[i];
	}
	temp=vector.SquareLength(p1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
	{
		temp=vector.SquareLength(p2);
		return sqrt(temp);
	}
	for(i=0;i<2;i++)
		p1[i]/=temp;                   //单位化矢量ab
	temp=vector.DotProduct(p1[0],p1[1],p2[0],p2[1]);
	for(i=0;i<2;i++)
		p1[i]*=temp;
	temp=vector.SquareLength(p2[0]-p1[0],p2[1]-p1[1]);
	temp=sqrt(temp);
	return temp;
}

double Relation2D::PToLDistance(Point2D &point1,Point2D &point2,Point2D &point)
{
	double a[2],b[2],c[2];
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	return PToLDistance(a,b,c);
}
double Relation2D::PToLDistance(Line2D &line,Point2D &point)
{
	double a[2],b[2],c[2];
	line.Get(a,b);
	point.Get(c);
	return PToLDistance(a,b,c);
}
double Relation2D::PToSegDistance(double a[],double b[],double p[])
{
	int i;
	double temp,temp1;
	double p1[3],p2[3];
	Vector2D vector;
	for(i=0;i<2;i++)
	{
		p1[i]=b[i]-a[i];
		p2[i]=p[i]-a[i];
	}
	temp=vector.SquareLength(p1);
	temp1=sqrt(temp);
	if(temp1<THRESHOLD10)
	{
		temp=vector.SquareLength (p2);
		return sqrt(temp);
	}
	for(i=0;i<2;i++)
		p1[i]/=temp1;                  //单位化矢量ab
	temp=vector.DotProduct(p1[0],p1[1],p2[0],p2[1]);
	if(temp<-THRESHOLD10 || temp>temp1+THRESHOLD10)
	{
		temp=vector.SquareLength (p2);
		temp=sqrt(temp);
		for(i=0;i<2;i++)
		{
			p2[i]=p[i]-b[i];
		}
		temp1=vector.SquareLength (p2);
		temp1=sqrt(temp1);
		if(temp>temp1+THRESHOLD10)
			return temp;
		else
			return temp1;
	}
	for(i=0;i<2;i++)
		p1[i]*=temp;
	temp=vector.SquareLength(p2[0]-p1[0],p2[1]-p1[1]);
	temp=sqrt(temp);
	return temp;
}

double Relation2D::PToSegDistance(Point2D &point1,Point2D &point2,Point2D &point)
{
	double a[2],b[2],c[2];
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	return PToSegDistance(a,b,c);
}
double Relation2D::PToSegDistance(Line2D &line,Point2D &point)
{
	double a[2],b[2],c[2];
	line.Get(a,b);
	point.Get(c);
	return PToSegDistance(a,b,c);
}///////

//判断点P是否在直线ab上
int Relation3D::PointInLine(double a[],double b[],double p[])
{
	double temp1,temp2,temp3;      //临时变量，表示矢量的模
	double q[3];                   //临时变量,表示空间一点
	int i;

    temp3=0;
	for(i=0;i<3;i++)
		temp3+=(b[i]-a[i])*(b[i]-a[i]);
	if(temp3<THRESHOLD10)          //求ab的模的平方
		return 0;

	temp1=0;
	for(i=0;i<3;i++)
		temp1+=(a[i]-p[i])*(a[i]-p[i]);
	if(temp1<THRESHOLD10)          //p为ab的一个端点a
		return 1;

	temp2=0;
	for(i=0;i<3;i++)
		temp2+=(b[i]-p[i])*(b[i]-p[i]);
	if(temp2<THRESHOLD10)          //p为ab的一个端点b
		return 2;

	if(temp1<temp3)                //ap的模<ab的模
	{
		temp1=sqrt(temp1);         //求ap的模
		temp3=sqrt(temp3);         //求ab的模
		for(i=0;i<3;i++)
			q[i]=a[i]+(b[i]-a[i])*temp1/temp3;
		//ab上的点q，它使aq的模＝ap的模
		temp3=0;
		for(i=0;i<3;i++)
			temp3+=(q[i]-p[i])*(q[i]-p[i]);
		if(temp3<THRESHOLD10)      //p,q重合，p在ab上
			return 3;
	}
	return 0;
}
int Relation3D::PointInLine(Line3D &line,double p[])
{
	double a[3],b[3];
	a[0]=line.m_x1,a[1]=line.m_y1,a[2]=line.m_z1;
	b[0]=line.m_x2,b[1]=line.m_y2,b[2]=line.m_z2;
	return PointInLine(a,b,p);
}

int Relation3D::PointInTriangle(double a[],double b[],double c[],double p[])
{
	////////////////////////////////////
	///不能修改a,b,c,p
	////////////////////////////////////

	Vector3D vector;
	double temp;
	double n[3];          //法向量
	double p1[3],p2[3];
	int index;            //标识点在三角形中的位置
	int i;

	index=0;              //点不在三角形中
	temp=0;
	for(i=0;i<3;i++)
		temp+=(a[i]-p[i])*(a[i]-p[i]);
	if(temp<THRESHOLD10)  //p为三角形abc的一个顶点
		return 1;
	temp=0;
	for(i=0;i<3;i++)
		temp+=(b[i]-p[i])*(b[i]-p[i]);
	if(temp<THRESHOLD10)  //p为三角形abc的一个顶点
		return 2;
	temp=0;
	for(i=0;i<3;i++)
		temp+=(c[i]-p[i])*(c[i]-p[i]);
	if(temp<THRESHOLD10)  //p为三角形abc的一个顶点
		return 3;

	if(PointInLine(a,b,p)==3)
		return 4;        //p点在ab上
	if(PointInLine(b,c,p)==3)
		return 5;        //p点在ab上
	if(PointInLine(c,a,p)==3)
		return 6;        //p点在ab上

	/////////////////////////////////////////
	//判断p点是否在三角形内部开始

	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],c[0]-a[0],c[1]-a[1],c[2]-a[2]);
	vector.Get (n);
	temp=n[0]*n[0]+n[1]*n[1]+n[2]*n[2];
	temp=sqrt(temp);     //法向量的模
	if(temp<THRESHOLD10) //三角形abc的一顶角太小
		return 0;
	else                 //法向量单位化
	{
		n[0]/=temp;
		n[1]/=temp;
		n[2]/=temp;
	}

	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],p[0]-a[0],p[1]-a[1],p[2]-a[2]);//ab与ap的叉积
	vector.Get (p1);
	temp=p1[0]*p1[0]+p1[1]*p1[1]+p1[2]*p1[2];
	temp=sqrt(temp);
	if(temp<THRESHOLD10)  //三角形abc的一顶角太小
		return 0;
	else
	{
		p1[0]/=temp;
		p1[1]/=temp;
		p1[2]/=temp;
	}                     //单位化
	vector.CrossProduct(p[0]-a[0],p[1]-a[1],p[2]-a[2],c[0]-a[0],c[1]-a[1],c[2]-a[2]);//ab与ap的叉积
	vector.Get (p2);
	temp=p2[0]*p2[0]+p2[1]*p2[1]+p2[2]*p2[2];
	temp=sqrt(temp);
	if(temp<THRESHOLD10)  //三角形abc的一顶角太小
		return 0;
	else
	{
		p2[0]/=temp;
		p2[1]/=temp;
		p2[2]/=temp;
	}                     //单位化

	temp=0;
	for(i=0;i<3;i++)
		temp+=p1[i]*p2[i];
	if(temp<THRESHOLD10)  //p不在ab和ac之间
		return 0;

	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	vector.CrossProduct(a[0]-b[0],a[1]-b[1],a[2]-b[2],p[0]-b[0],p[1]-b[1],p[2]-b[2]);
	vector.Get (p1);
	temp=p1[0]*p1[0]+p1[1]*p1[1]+p1[2]*p1[2];
	temp=sqrt(temp);
	if(temp<THRESHOLD10)  //三角形abc的一顶角太小
		return 0;
	else
	{
		p1[0]/=temp;
		p1[1]/=temp;
		p1[2]/=temp;
	}                     //单位化
	vector.CrossProduct(p[0]-b[0],p[1]-b[1],p[2]-b[2],c[0]-b[0],c[1]-b[1],c[2]-b[2]);
	vector.Get (p2);
	temp=p2[0]*p2[0]+p2[1]*p2[1]+p2[2]*p2[2];
	temp=sqrt(temp);
	if(temp<THRESHOLD10)  //三角形abc的一顶角太小
		return 0;
	else
	{
		p2[0]/=temp;
		p2[1]/=temp;
		p2[2]/=temp;
	}                     //单位化
	temp=0;
	for(i=0;i<3;i++)
		temp+=p1[i]*p2[i];
	if(temp<THRESHOLD10)  //p不在ab和ac之间
		return 0;

	return 7;             //p点在三角形内部
	//判断p是否在三角形abc内的处理结束
}
int Relation3D::PointInTriangle(Triangle3D &triangle,double p[])
{
	double a[3],b[3],c[3];
	a[0]=triangle.m_x1,a[1]=triangle.m_y1,a[2]=triangle.m_z1;
	b[0]=triangle.m_x2,b[1]=triangle.m_y2,b[2]=triangle.m_z2;
	c[0]=triangle.m_x3,c[1]=triangle.m_y3,c[2]=triangle.m_z3;
	return PointInTriangle(a,b,c,p);
}
int Relation3D::GetIntersectPoint(double a[], double b[],
								  double p1[],double p2[],double p[])
{
	///////////////////////////////////////////////////////////////////////
	//a+ab.t1=p1+p1p2.t2      平行的情况不用此公式
	//abXp1p2.t1=(p1-a)Xp1p2  t1=((p1-a)Xp1p2).(abXp1p2)/SquareLength(abXp1p2)
	//abX(a-p1)=abXp1p2t2     t2=(abX(a-p1)  ).(abXp1p2)/SquareLength(abXp1p2)
	///////////////////////////////////////////////////////////////////////
	Vector3D vector;
	Line3D   line;
	double   m[3],n[3];
	double   SquareLengthAB,SquareLengthP1P2;
	double   temp;
	int      i,index,index1;
	//////验证两直线段的长度是否为零//////
	SquareLengthAB=vector.SquareLength(b[0]-a[0],b[1]-a[1],b[2]-a[2]);
	SquareLengthP1P2=vector.SquareLength(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
	if(sqrt(SquareLengthAB)<THRESHOLD10 || sqrt(SquareLengthP1P2)<THRESHOLD10)
		return 0;
	//////验证两条直线是否异面  zjg add
	vector.CrossProduct(b[0]-a[0],  b[1]-a[1],  b[2]-a[2],
		p2[0]-p1[0], p2[1]-p1[1],p2[2]-p1[2]);
	vector.Normalize();
	vector.Get(n);
	double   t[3];
	t[0] = b[0]-p2[0];t[1] = b[1] - p2[1]; t[2] = b[2] - p2[2];
	vector.Set(t);
	vector.Normalize();
	vector.Get(t);
	double sign = vector.DotProduct(n,t);
	if(fabs(sign)>THRESHOLD6)
		return 0;
	////////////////////////////////////////////////////////
	vector.CrossProduct(b[0]-a[0],  b[1]-a[1],  b[2]-a[2],
		p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
	vector.Get(n);                             //n=abXp1p2
	temp=vector.SquareLength(n);
	if(sqrt(temp)<THRESHOLD10)                 //两条线ab,p1p2平行  p=a+t1(b-a), t1=ap.ab/ab.ab
	{
		line.Set(a,b);
		index=0;
		if(line.Contain(p1)==3 || line.Contain(p2)==3)
			index=40;
		line.Set(p1,p2);
		if(line.Contain(a)==3 || line.Contain(b)==3)
			index+=4;
		return index;
	}
	///////两直线段不平行时/////////////////////////////
	vector.CrossProduct(p1[0]-a[0],  p1[1]-a[1],  p1[2]-a[2],
		p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]);
	vector.Get(m);                                //m=ap1Xp1p2
	temp=vector.DotProduct(m,n)/temp;
	for(i=0;i<3;i++)
	{
		p[i]=b[i]-a[i];
		p[i]*=temp;
		p[i]+=a[i];
	}                                              //p为交点
	if(temp>-THRESHOLD10 && temp<THRESHOLD10)      //index/10表示交点在第一条线段ab上的位置
		index=10;
	else if(temp>1-THRESHOLD10 && temp<1+THRESHOLD10)
		index=20;
	else if(temp>THRESHOLD10 && temp<1-THRESHOLD10)
		index=30;
	else
		return 0;

	temp=vector.SquareLength(n);
	vector.CrossProduct(b[0]-a[0], b[1]-a[1], b[2]-a[2],
		a[0]-p1[0],a[1]-p1[1],a[2]-p1[2]);

	vector.Get(m);                                   //m=abXap1
	temp=vector.DotProduct(m,n)/temp;
	if(temp>-THRESHOLD10 && temp<THRESHOLD10)        //index%10表示交点在第二条线段p1p2上的位置
		index1=1;
	else if(temp>1-THRESHOLD10 && temp<1+THRESHOLD10)
		index1=2;
	else if(temp>THRESHOLD10 && temp<1-THRESHOLD10)
		index1=3;
	else
		return 0;
	return index+index1;
}
int Relation3D::GetIntersectPoint(Line3D &line1,Line3D &line2,double p[])
{
	double a[3],b[3],p1[3],p2[3];
	line1.Get(a,b);
	line2.Get(p1,p2);
	return GetIntersectPoint(a,b,p1,p2,p);
}
int Relation3D::GetIntersectPoint(double a[], double b[], double c[],
							  		 double p1[],double p2[],double p[])
{
	//////////////////////////////////////////////////////////
	//不能修改a,b,c,p1,p2的值,直线与三角形平行的情况不在此处理
	//////////////////////////////////////////////////////////
	Vector3D vector;
	double n[3];           //三角形所在平面的法向量
	double temp;
	int i,index;

	if(p1[0]>a[0]+THRESHOLD10 && p1[0]>b[0]+THRESHOLD10 && p1[0]>c[0]+THRESHOLD10 &&
		p2[0]>a[0]+THRESHOLD10 && p2[0]>b[0]+THRESHOLD10 && p2[0]>c[0]+THRESHOLD10)
		return 0;
	if(p1[0]<a[0]-THRESHOLD10 && p1[0]<b[0]-THRESHOLD10 && p1[0]<c[0]-THRESHOLD10 &&
		p2[0]<a[0]-THRESHOLD10 && p2[0]<b[0]-THRESHOLD10 && p2[0]<c[0]-THRESHOLD10)
		return 0;

	if(p1[1]>a[1]+THRESHOLD10 && p1[1]>b[1]+THRESHOLD10 && p1[1]>c[1]+THRESHOLD10 &&
		p2[1]>a[1]+THRESHOLD10 && p2[1]>b[1]+THRESHOLD10 && p2[1]>c[1]+THRESHOLD10)
		return 0;
	if(p1[1]<a[1]-THRESHOLD10 && p1[1]<b[1]-THRESHOLD10 && p1[1]<c[1]-THRESHOLD10 &&
		p2[1]<a[1]-THRESHOLD10 && p2[1]<b[1]-THRESHOLD10 && p2[1]<c[1]-THRESHOLD10)
		return 0;

	if(p1[2]>a[2]+THRESHOLD10 && p1[2]>b[2]+THRESHOLD10 && p1[2]>c[2]+THRESHOLD10 &&
		p2[2]>a[2]+THRESHOLD10 && p2[2]>b[2]+THRESHOLD10 && p2[2]>c[2]+THRESHOLD10)
		return 0;
	if(p1[2]<a[2]-THRESHOLD10 && p1[2]<b[2]-THRESHOLD10 && p1[2]<c[2]-THRESHOLD10 &&
		p2[2]<a[2]-THRESHOLD10 && p2[2]<b[2]-THRESHOLD10 && p2[2]<c[2]-THRESHOLD10)
		return 0;

	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
	if(!vector.Normalize ()) return 0;
	vector.Get (n);

	for(i=0;i<3;i++)
		p[i]=p2[i]-p1[i];              //p=p1p2
	temp=n[0]*p[0]+n[1]*p[1]+n[2]*p[2];//p1p2在n上的投影,n.p1p2

	///////////////////////////////////////////////////////////////////
	///// n.(ap1+tp1p2)=0
	///// t=-n.ap1/n.p1p2
	///// ap=ap1+tp1p2
	///// p=ap1+tp1p2+a=p1+(-n.ap1/n.p1p2)p1p2
	///////////////////////////////////////////////////////////////////

	index=0;//初始值设为没有交点
	if(temp<-THRESHOLD10  || temp>THRESHOLD10)         //线段p1p2不平行于三角形abc所在平面
	{
		temp=-(n[0]*(p1[0]-a[0])+n[1]*(p1[1]-a[1])+n[2]*(p1[2]-a[2]))/temp;//(-n.ap1/n.p1p2)
		if((temp>-THRESHOLD10) && (temp<1+THRESHOLD10))//交点在p1和p2之间的处理开始
		{
			for(i=0;i<3;i++)
				p[i]*=temp;                //p1p2与三角形abc所在平面的交点,p=(-n.ap1/n.p1p2)p1p2
			for(i=0;i<3;i++)
				p[i]+=p1[i];               //p=p1+(-n.ap1/n.p1p2)p1p2

			if(temp<THRESHOLD10)           //交点在p1
				index=1;
			else if(temp>1-THRESHOLD10)    //交点在p2
				index=2;
			else                           //交点在p1与p2之间
				index=3;
		}
		else
			return 0;                      //交点不在p1和p2之间,
		index+=10*PointInTriangle(a,b,c,p);//交点在三角形上的位置保存在index的十位数字中
		if(index/10>0)                     //交点在三角内部或边上
			return index;
		else
			return 0;
	}
	else                                   //线段p1p2平行于三角形abc所在平面
	{
		for(i=0;i<3;i++)
			p[i]=p2[i]-a[i];               //p=ap2
		temp=n[0]*p[0]+n[1]*p[1]+n[2]*p[2];//ap2在n上的投影,n.ap2
		if(temp<-THRESHOLD10||temp>THRESHOLD10)//线段不在三角形所在平面内
			return 0;

		//如果线段一个端点在三角形内，求出该端点，否则计算线段与三角某边的一个交点
		index=PointInTriangle(a,b,c,p1);
		if(index>3)                        //端点在三角形内部或边上
		{
			for(i=0;i<3;i++)
				p[i]=p1[i];
			return (10*index+1);
		}
		index=PointInTriangle(a,b,c,p2);
		if(index>3)                        //端点在三角形内部或边上
		{
			for(i=0;i<3;i++)
				p[i]=p2[i];
			return (10*index+2);
		}
		index=GetIntersectPoint(a,b,p1,p2,p);
		if(index/10==3 )
		{
			index=40+index%10;
			return index;
		}
		else if(index%10==3)
		{
			return index;
		}
		index=GetIntersectPoint(b,c,p1,p2,p);
		if(index/10==3)
		{
			index=50+index%10;
			return index;
		}
		else if(index%10==3)
		{
			index+=10;
			return index;
		}

		index=GetIntersectPoint(c,a,p1,p2,p);
		if(index/10==3)
		{
			index=60+index%10;
			return index;
		}
		else if(index%10==3)
		{
			if(index/10==1)
				index=30+index%10;
			else if(index/10==2)
				index=10+index%10;
			return index;
		}
	}
	return 0;
}
int Relation3D::GetIntersectPoint(Triangle3D &triangle,
							  		 Line3D &line,double p[])
{
	double a[3],b[3],c[3];
	double p1[3],p2[3];
	triangle.Get(a,b,c);
	line.Get(p1,p2);
	return GetIntersectPoint(a,b,c,p1,p2,p);
}

double Relation3D::PToPDistance(double a[],double b[])
{
	double temp;
	Vector3D vector;
	vector.Set (b[0]-a[0],b[1]-a[1],b[2]-a[2]);
	temp=vector.SquareLength();
	temp=sqrt(temp);
	return temp;
}

double Relation3D::PToPDistance(Point3D &point1,Point3D &point2)
{
	double a[3],b[3];
	point1.Get (a);
	point2.Get (b);
	return PToPDistance(a,b);
}

double Relation3D::PToLDistance(double a[],double b[],double p[])
{
	int i;
	double temp;
	double p1[3],p2[3];
	Vector3D vector;
	for(i=0;i<3;i++)
	{
		p1[i]=b[i]-a[i];
		p2[i]=p[i]-a[i];
	}
	temp=vector.SquareLength(p1);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return 0;
	for(i=0;i<3;i++)
		p1[i]/=temp;                 //单位化矢量ab
	temp=vector.DotProduct(p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
	for(i=0;i<3;i++)
		p1[i]*=temp;
	temp=vector.SquareLength(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
	temp=sqrt(temp);
	return temp;
}

double Relation3D::PToLDistance(Point3D &point1,Point3D &point2,Point3D &point)
{
	double a[3],b[3],c[3];
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	return PToLDistance(a,b,c);
}
double Relation3D::PToLDistance(Line3D &line,Point3D &point)
{
	double a[3],b[3],c[3];
	line.Get(a,b);
	point.Get(c);
	return PToLDistance(a,b,c);
}
double Relation3D::PToSegDistance(double a[],double b[],double p[])
{
	int i;
	double temp,temp1;
	double p1[3],p2[3];
	Vector3D vector;
	for(i=0;i<3;i++)
	{
		p1[i]=b[i]-a[i];
		p2[i]=p[i]-a[i];
	}
	temp=vector.SquareLength(p1);
	temp1=sqrt(temp);
	if(temp1<THRESHOLD6)
	{
		temp=vector.SquareLength (p2);
		return sqrt(temp);
	}
	for(i=0;i<3;i++)
		p1[i]/=temp1;             //单位化矢量ab
	temp=vector.DotProduct(p1[0],p1[1],p1[2],p2[0],p2[1],p2[2]);
	if(temp<-THRESHOLD10 || temp>temp1+THRESHOLD10)
	{
		temp=vector.SquareLength (p2);
		temp=sqrt(temp);
		for(i=0;i<3;i++)
		{
			p2[i]=p[i]-b[i];
		}
		temp1=vector.SquareLength (p2);
		temp1=sqrt(temp1);
		if(temp>temp1+THRESHOLD10)
			return temp;
		else
			return temp1;
	}
	for(i=0;i<3;i++)
		p1[i]*=temp;
	temp=vector.SquareLength(p2[0]-p1[0],p2[1]-p1[1],p2[2]-p1[2]);
	temp=sqrt(temp);
	return temp;
}

double Relation3D::PToSegDistance(Point3D &point1,Point3D &point2,Point3D &point)
{
	double a[3],b[3],c[3];
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	return PToSegDistance(a,b,c);
}
double Relation3D::PToSegDistance(Line3D &line,Point3D &point)
{
	double a[3],b[3],c[3];
	line.Get(a,b);
	point.Get(c);
	return PToSegDistance(a,b,c);
}
double Relation3D::PToFDistance(double a[],double b[],double c[],double p[])
{
	int i;
	double n[3];
	double temp;
	Vector3D vector;
	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],c[0]-a[0],c[1]-a[1],c[2]-a[2]);
	vector.Get(n);
	temp=vector.SquareLength(n);
	temp=sqrt(temp);
	if(temp<THRESHOLD10)
		return -2;
	else
		for(i=0;i<3;i++)
			n[i]/=temp;
	temp=vector.DotProduct(n[0],n[1],n[2],p[0]-a[0],p[1]-a[1],p[2]-a[2]);
	if(temp<-THRESHOLD10)
		temp=-temp;
	return temp;
}
double Relation3D::PToFDistance(Triangle3D &triangle,Point3D &point)
{
	double a[3],b[3],c[3],p[3];
	triangle.Get(a,b,c);
	point.Get(p);
	return PToFDistance(a,b,c,p);
}
double Relation3D::PToFDistance(Point3D &point1,Point3D &point2,
								 Point3D &point3,Point3D &point)
{
	double a[3],b[3],c[3],p[3];
	point1.Get(a);
	point2.Get(b);
	point3.Get(c);
	point.Get(p);
	return PToFDistance(a,b,c,p);
}
int Relation3D::GetCrossCut(Line3D& line,Triangle3D triangle1,Triangle3D triangle2)
{
	int          i,index,num;
	double       n[3],p1[3],p2[3];
	double       a[3],b[3],c[3],d[3];
	Line3D       tmpline[3];
	Point3D      point[2];
	Triangle3D   triangle;
	Vector3D     vector;

	triangle2.Get(a,b,c);
	for(i=0;i<3;i++)
	{
		b[i]-=a[i];
		c[i]-=a[i];
	}
	vector.CrossProduct (b,c);
	vector.Normalize ();
	vector.Get (n);                            //取三角形triangle2所在平面的法向量
	if(!GetCrossCut(tmpline[0],triangle1,a,n)) //两三角形互不交叉
		return 0;

	triangle1.Get(a,b,c);
	for(i=0;i<3;i++)
	{
		b[i]-=a[i];
		c[i]-=a[i];
	}
	vector.CrossProduct (b,c);
	vector.Normalize ();
	vector.Get (n);                            //取三角形triangle2所在平面的法向量
	if(!GetCrossCut(tmpline[1],triangle2,a,n)) //两三角形互不交叉
		return 0;

	tmpline[0].Get (a,b);
	tmpline[1].Get (c,d);
	for(i=0;i<3;i++)
	{
		p1[i]=b[i]-a[i],p2[i]=d[i]-c[i];
	}
	if(vector.DotProduct (p1,p2)<-THRESHOLD10) //线段的方向相反
	{
		tmpline[1].Set(d,c);
		tmpline[1].Get (c,d);
	}
	for(i=0;i<3;i++)
	{
		p2[i]=c[i]-a[i];
	}
	if(vector.DotProduct (p1,p2)<-THRESHOLD10) //线段tmpline[1]的起点在前
	{
		tmpline[2]=tmpline[0];
		tmpline[0]=tmpline[1];
		tmpline[1]=tmpline[2];
		tmpline[0].Get(a,b);
		tmpline[1].Get(c,d);
	}
	for(i=0;i<3;i++)
		p2[i]=c[i]-b[i];
	if(vector.DotProduct (p1,p2)>-THRESHOLD10) //线段不重叠
		return 0;
	tmpline[0].Set(c,b);
	tmpline[0].Get(a,b);
	for(i=0;i<3;i++)
		p2[i]=d[i]-b[i];
	if(vector.DotProduct (p1,p2)<-THRESHOLD10) //d在b前
	{
		tmpline[0]=tmpline[1];                 //置tmpline[0]为公共线段
	}
	line=tmpline[0];
	return 1;
}
int Relation3D::GetCrossCut(Line3D& line,double a[3],double b[3],double c[3],double p[3],double n[3])
{
	int      i,index=0;
	double   temp,temp1;
	double   p1[3],p2[3],p3[3],r[3];
	Vector3D vector;

	vector.Set (n);
	vector.Normalize ();
	vector.Get (n);
	for( i=0;i<3;i++)
	{
		p1[i]=p[i]-a[i],r[i]=b[i]-a[i];
	}
	temp=vector.DotProduct (n,r);
	if(temp>THRESHOLD10 || temp<-THRESHOLD10)        //直线段ab不平行于由点p和法向量n定义的平面
	{
		temp1=vector.DotProduct (p1,n);
		temp1/=temp;
		if(temp1>-THRESHOLD10 && temp1<1+THRESHOLD10)//交点在线段上
		{
			index+=1;
			for(i=0;i<3;i++)
				p1[i]=a[i]+r[i]*temp1;
		}
	}

	for( i=0;i<3;i++)
	{
		p2[i]=p[i]-b[i],r[i]=c[i]-b[i];
	}
	temp=vector.DotProduct (n,r);
	if(temp>THRESHOLD10 || temp<-THRESHOLD10)        //直线段不平行于平面
	{
		temp1=vector.DotProduct (p2,n);
		temp1/=temp;
		if(temp1>-THRESHOLD10 && temp1<1+THRESHOLD10)//交点在线段上
		{
			index+=10;
			for(i=0;i<3;i++)
				p2[i]=b[i]+r[i]*temp1;
		}
	}

	for( i=0;i<3;i++)
	{
		p3[i]=p[i]-c[i],r[i]=a[i]-c[i];
	}
	temp=vector.DotProduct (n,r);
	if(temp>THRESHOLD10 || temp<-THRESHOLD10)        //直线段不平行于平面
	{
		temp1=vector.DotProduct (p3,n);
		temp1/=temp;
		if(temp1>-THRESHOLD10 && temp1<1+THRESHOLD10)//交点在线段上
		{
			index+=100;
			for(i=0;i<3;i++)
				p3[i]=c[i]+r[i]*temp1;
		}
	}
	if(index%10)                          //p1为有效交点
	{
		if(index/100)                     //p3为有效交点
		{
			temp=0;
			for(i=0;i<3;i++)
				temp+=(p1[i]-p3[i])*(p1[i]-p3[i]);
			if(temp>THRESHOLD10)          //两点不重合
			{
				line.Set(p1,p3);
				return 1;
			}
		}
		if((index%100)/10)                //p2为有效交点
		{
			temp=0;
			for(i=0;i<3;i++)
				temp+=(p1[i]-p2[i])*(p1[i]-p2[i]);
			if(temp>THRESHOLD10)          //两点不重合
			{
				line.Set(p1,p2);
				return 1;
			}
		}
	}
	else                                  //p1为无效交点
	{
		if(index/100)                     //p3为有效交点
		{
			temp=0;
			for(i=0;i<3;i++)
				temp+=(p2[i]-p3[i])*(p2[i]-p3[i]);
			if(temp>THRESHOLD10)          //两点不重合
			{
				line.Set(p2,p3);
				return 1;
			}
		}
	}
	return 0;
}
int Relation3D::GetCrossCut(Line3D& line,Triangle3D triangle,double p[3],double n[3])
{
	double a[3],b[3],c[3];
	triangle.Get (a,b,c);
	return GetCrossCut(line,a,b,c,p,n);
}

int Relation3D::DivideTriangles(Triangle3DArrayEx& objPolyTriangle1,Triangle3DArrayEx& objPolyTriangle2)
{
	int        i,j,k,num1,num2;
	int        index,flag,start=0;
	double     a[3],b[3],c[3];
	double     d[3],e[3],f[3];
	double     p[3][3];
	double     n[2][3];
	Line3D     objLine;
	Triangle3D objTriangle[2];
	Vector3D   objV;

	num1=objPolyTriangle1.size();
	num2=objPolyTriangle2.size();
	if(num1<1 || num2<1) return 0;
	for(i=0;i<num1;i++)
	{
		objTriangle[0] = objPolyTriangle1.at(i);
		objTriangle[0].Get(a,b,c);
		objTriangle[0].GetNormal (n[0]);

		for(j=start;j<num2;j++)
		{
			objTriangle[1]=objPolyTriangle2.at(j);
			objTriangle[1].Get (d,e,f);
			objTriangle[1].GetNormal (n[1]);

			double temp=objV.DotProduct (n[0],n[1]);
			if(temp<-1+THRESHOLD10 || temp>1-THRESHOLD10) continue;

			for(k=0;k<3;k++)
			{
				p[0][k]=d[k];
				p[1][k]=e[k];
			}
			objLine.Set (p[0],p[1]);
			index=GetIntersectPoint(objTriangle[0],objLine,p[2]);
			if(index/10>0)
			{
				if(index/10==4)                        //线段与三角形相交于三角形内部或三角形边上
				{
					objTriangle[0].Set(a,p[2],c);
					objPolyTriangle1.SetAt(i,objTriangle[0]);

					objTriangle[0].Set(p[2],b,c);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==5)
				{
					objTriangle[0].Set(b,p[2],a);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],c,a);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==6)
				{
					objTriangle[0].Set(c,p[2],b);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],a,b);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==7)
				{
					objTriangle[0].Set(a,b,p[2]);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(b,c,p[2]);
					objPolyTriangle1.Add (objTriangle[0]);

					objTriangle[0].Set(c,a,p[2]);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				if(index%10==3)
				{
					objTriangle[1].Set(d,p[2],f);
					objPolyTriangle2.SetAt (j,objTriangle[1]);

					objTriangle[1].Set(p[2],e,f);
					objPolyTriangle2.Add (objTriangle[1]);
				}
				num1=objPolyTriangle1.size ();
				num2=objPolyTriangle2.size ();
				if(index/10>3 || index%10==3)
				{
					i--;
					start=j;
				}
				break;
			}
			for(k=0;k<3;k++)
			{
				p[0][k]=e[k];
				p[1][k]=f[k];
			}
			objLine.Set (p[0],p[1]);
			index=GetIntersectPoint(objTriangle[0],objLine,p[2]);
			if(index/10>0)
			{
				if(index/10==4)                        //线段与三角形相交于三角形内部或三角形边上
				{
					objTriangle[0].Set(a,p[2],c);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],b,c);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==5)
				{
					objTriangle[0].Set(b,p[2],a);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],c,a);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==6)
				{
					objTriangle[0].Set(c,p[2],b);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],a,b);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==7)
				{
					objTriangle[0].Set(a,b,p[2]);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(b,c,p[2]);
					objPolyTriangle1.Add (objTriangle[0]);

					objTriangle[0].Set(c,a,p[2]);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				if(index%10==3)
				{
					objTriangle[1].Set(e,p[2],d);
					objPolyTriangle2.SetAt (j,objTriangle[1]);

					objTriangle[1].Set(p[2],f,d);
					objPolyTriangle2.Add (objTriangle[1]);
				}
				num1=objPolyTriangle1.size ();
				num2=objPolyTriangle2.size ();
				if(index/10>3 || index%10==3)
				{
					i--;
					start=j;
				}
				break;
			}
			for(k=0;k<3;k++)
			{
				p[0][k]=f[k];
				p[1][k]=d[k];
			}
			objLine.Set (p[0],p[1]);
			index=GetIntersectPoint(objTriangle[0],objLine,p[2]);
			if(index/10>3)
			{
				if(index/10==4)                        //线段与三角形相交于三角形内部或三角形边上
				{
					objTriangle[0].Set(a,p[2],c);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],b,c);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==5)
				{
					objTriangle[0].Set(b,p[2],a);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],c,a);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==6)
				{
					objTriangle[0].Set(c,p[2],b);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(p[2],a,b);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				else if(index/10==7)
				{
					objTriangle[0].Set(a,b,p[2]);
					objPolyTriangle1.SetAt (i,objTriangle[0]);

					objTriangle[0].Set(b,c,p[2]);
					objPolyTriangle1.Add (objTriangle[0]);

					objTriangle[0].Set(c,a,p[2]);
					objPolyTriangle1.Add (objTriangle[0]);
				}
				if(index%10==3)
				{
					objTriangle[1].Set(f,p[2],e);
					objPolyTriangle2.SetAt (j,objTriangle[1]);

					objTriangle[1].Set(p[2],d,e);
					objPolyTriangle2.Add (objTriangle[1]);
				}
				num1=objPolyTriangle1.size ();
				num2=objPolyTriangle2.size ();
				if(index/10>3 || index%10==3)
				{
					i--;
					start=j;
				}
				break;
			}
			for(k=0;k<3;k++)
			{
				p[0][k]=a[k];
				p[1][k]=b[k];
			}
			objLine.Set (p[0],p[1]);
			index=GetIntersectPoint(objTriangle[1],objLine,p[2]);
			if(index/10>0)
			{
				if(index/10==4)                        //线段与三角形相交于三角形内部或三角形边上
				{
					objTriangle[0].Set(d,p[2],f);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],e,f);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==5)
				{
					objTriangle[0].Set(e,p[2],d);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],f,d);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==6)
				{
					objTriangle[0].Set(f,p[2],e);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],d,e);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==7)
				{
					objTriangle[0].Set(d,e,p[2]);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(e,f,p[2]);
					objPolyTriangle2.Add (objTriangle[0]);

					objTriangle[0].Set(f,d,p[2]);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				if(index%10==3)
				{
					objTriangle[1].Set(a,p[2],c);
					objPolyTriangle1.SetAt (i,objTriangle[1]);

					objTriangle[1].Set(p[2],b,c);
					objPolyTriangle1.Add (objTriangle[1]);
				}
				num1=objPolyTriangle1.size ();
				num2=objPolyTriangle2.size ();
				if(index/10>3 || index%10==3)
				{
					i--;
					start=j;
				}
				break;
			}
			for(k=0;k<3;k++)
			{
				p[0][k]=b[k];
				p[1][k]=c[k];

			}
			objLine.Set (p[0],p[1]);
			index=GetIntersectPoint(objTriangle[1],objLine,p[2]);
			if(index/10>0)
			{
				if(index/10==4)                        //线段与三角形相交于三角形内部或三角形边上
				{
					objTriangle[0].Set(d,p[2],f);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],e,f);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==5)
				{
					objTriangle[0].Set(e,p[2],d);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],f,d);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==6)
				{
					objTriangle[0].Set(f,p[2],e);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],d,e);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==7)
				{
					objTriangle[0].Set(d,e,p[2]);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(e,f,p[2]);
					objPolyTriangle2.Add (objTriangle[0]);

					objTriangle[0].Set(f,d,p[2]);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				if(index%10==3)
				{
					objTriangle[1].Set(b,p[2],a);
					objPolyTriangle1.SetAt (i,objTriangle[1]);

					objTriangle[1].Set(p[2],c,a);
					objPolyTriangle1.Add (objTriangle[1]);
				}
				num1=objPolyTriangle1.size ();
				num2=objPolyTriangle2.size ();
				if(index/10>3 || index%10==3)
				{
					i--;
					start=j;
				}
				break;
			}
			for(k=0;k<3;k++)
			{
				p[0][k]=c[k];
				p[1][k]=a[k];

			}
			objLine.Set (p[0],p[1]);
			index=GetIntersectPoint(objTriangle[1],objLine,p[2]);
			if(index/10>0)
			{
				if(index/10==4)                        //线段与三角形相交于三角形内部或三角形边上
				{
					objTriangle[0].Set(d,p[2],f);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],e,f);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==5)
				{
					objTriangle[0].Set(e,p[2],d);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],f,d);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==6)
				{
					objTriangle[0].Set(f,p[2],e);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(p[2],d,e);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				else if(index/10==7)
				{
					objTriangle[0].Set(d,e,p[2]);
					objPolyTriangle2.SetAt (j,objTriangle[0]);

					objTriangle[0].Set(e,f,p[2]);
					objPolyTriangle2.Add (objTriangle[0]);

					objTriangle[0].Set(f,d,p[2]);
					objPolyTriangle2.Add (objTriangle[0]);
				}
				if(index%10==3)
				{
					objTriangle[1].Set(c,p[2],b);
					objPolyTriangle1.SetAt (i,objTriangle[1]);

					objTriangle[1].Set(p[2],a,b);
					objPolyTriangle1.Add (objTriangle[1]);
				}
				num1=objPolyTriangle1.size ();
				num2=objPolyTriangle2.size ();
				if(index/10>3 || index%10==3)
				{
					i--;
					start=j;
				}
				break;
			}
		}
		if(j==num2)
			start=0;
	}
	return 1;
}
//int Relation3D::DivideTriangles(Triangle3DArrayEx& objPolyTriangle1,Triangle3DArrayEx& objPolyTriangle2,
//		                         Triangle3D objTriangle1,Triangle3D objTriangle2)
//{
//	int          i,index,num;
//	double       n[3],p1[3],p2[3];
//	double       a[3],b[3],c[3],d[3];
//	Line3D       objLine[3];
//	Point3D      objPoint[2];
//	Triangle3D     objTriangle;
//	PolyTriangle3D objPolyTriangle;
//	Vector3D       objVector;
//
//	objTriangle2.Get(a,b,c);
//	for(i=0;i<3;i++)
//	{
//		b[i]-=a[i];
//		c[i]-=a[i];
//	}
//	objVector.CrossProduct (b,c);
//	objVector.Normalize ();
//	objVector.Get (n);                            //取三角形objTriangle2所在平面的法向量
//	if(!GetCrossCut(objLine[0],objTriangle1,a,n)) //两三角形互不交叉
//		return 0;
//
//	objTriangle1.Get(a,b,c);
//	for(i=0;i<3;i++)
//	{
//		b[i]-=a[i];
//		c[i]-=a[i];
//	}
//	objVector.CrossProduct (b,c);
//	objVector.Normalize ();
//	objVector.Get (n);                            //取三角形objTriangle2所在平面的法向量
//	if(!GetCrossCut(objLine[1],objTriangle2,a,n)) //两三角形互不交叉
//		return 0;
//
//	objLine[0].Get (a,b);
//	objLine[1].Get (c,d);
//	for(i=0;i<3;i++)
//	{
//		p1[i]=b[i]-a[i],p2[i]=d[i]-c[i];
//	}
//	if(objVector.DotProduct (p1,p2)<-THRESHOLD10) //线段的方向相反
//	{
//		objLine[1].Set(d,c);
//		objLine[1].Get (c,d);
//	}
//	for(i=0;i<3;i++)
//	{
//		p2[i]=c[i]-a[i];
//	}
//	if(objVector.DotProduct (p1,p2)<-THRESHOLD10) //线段objLine[1]的起点在前
//	{
//		objLine[2]=objLine[0];
//		objLine[0]=objLine[1];
//		objLine[1]=objLine[2];
//		objLine[0].Get(a,b);
//		objLine[1].Get(c,d);
//	}
//	for(i=0;i<3;i++)
//		p2[i]=c[i]-b[i];
//	if(objVector.DotProduct (p1,p2)>-THRESHOLD10) //线段不重叠
//		return 0;
//	objLine[0].Set(c,b);
//	objLine[0].Get(a,b);
//	for(i=0;i<3;i++)
//		p2[i]=d[i]-b[i];
//	if(objVector.DotProduct (p1,p2)<-THRESHOLD10) //d在b前
//	{
//		objLine[0]=objLine[1];                    //置objLine[0]为公共线段
//	}
//	objLine[0].Get (p1,p2);
//
//	index=PointInTriangle(objTriangle1,p1);
//	objPolyTriangle.clear ();
//	objTriangle1.Get (a,b,c);
//	if(index==7)                                  //交点在三角形内部
//	{
//		objTriangle.Set(a,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(a,c,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else if(index==4)
//	{
//		objTriangle.Set(a,c,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,b,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else if(index==5)
//	{
//		objTriangle.Set(a,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,a,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else if(index==6)
//	{
//		objTriangle.Set(a,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,b,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else
//		objPolyTriangle.Add (objTriangle1);
//	num=objPolyTriangle.size ();
//	objPolyTriangle1.clear ();
//	for(i=0;i<num;i++)
//	{
//		objTriangle1=objPolyTriangle.at (i);
//		index=PointInTriangle(objTriangle1,p2);
//		objTriangle1.Get (a,b,c);
//		if(index==7)                                //交点在三角形内部
//		{
//			objTriangle.Set(a,b,p2);
//			objPolyTriangle1.Add (objTriangle);
//			objTriangle.Set(c,b,p2);
//			objPolyTriangle1.Add (objTriangle);
//			objTriangle.Set(a,c,p2);
//			objPolyTriangle1.Add (objTriangle);
//		}
//		else if(index==4)
//		{
//			objTriangle.Set(a,c,p2);
//			objPolyTriangle1.Add (objTriangle);
//			objTriangle.Set(c,b,p2);
//			objPolyTriangle1.Add (objTriangle);
//		}
//		else if(index==5)
//		{
//			objTriangle.Set(a,b,p2);
//			objPolyTriangle1.Add (objTriangle);
//			objTriangle.Set(c,a,p2);
//			objPolyTriangle1.Add (objTriangle);
//		}
//		else if(index==6)
//		{
//			objTriangle.Set(a,b,p2);
//			objPolyTriangle1.Add (objTriangle);
//			objTriangle.Set(c,b,p2);
//			objPolyTriangle1.Add (objTriangle);
//		}
//		else
//			objPolyTriangle1.Add (objTriangle1);
//	}
//
//	index=PointInTriangle(objTriangle2,p1);
//	objPolyTriangle.clear ();
//	objTriangle2.Get (a,b,c);
//	if(index==7)                                //交点在三角形内部
//	{
//		objTriangle.Set(a,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(a,c,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else if(index==4)
//	{
//		objTriangle.Set(a,c,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,b,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else if(index==5)
//	{
//		objTriangle.Set(a,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,a,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else if(index==6)
//	{
//		objTriangle.Set(a,b,p1);
//		objPolyTriangle.Add (objTriangle);
//		objTriangle.Set(c,b,p1);
//		objPolyTriangle.Add (objTriangle);
//	}
//	else
//		objPolyTriangle.Add (objTriangle2);
//	num=objPolyTriangle.size ();
//	objPolyTriangle2.clear ();
//	for(i=0;i<num;i++)
//	{
//		objTriangle2=objPolyTriangle.at (i);
//		index=PointInTriangle(objTriangle2,p2);
//		objTriangle2.Get (a,b,c);
//		if(index==7)                                //交点在三角形内部
//		{
//			objTriangle.Set(a,b,p2);
//			objPolyTriangle2.Add (objTriangle);
//			objTriangle.Set(c,b,p2);
//			objPolyTriangle2.Add (objTriangle);
//			objTriangle.Set(a,c,p2);
//			objPolyTriangle2.Add (objTriangle);
//		}
//		else if(index==4)
//		{
//			objTriangle.Set(a,c,p2);
//			objPolyTriangle2.Add (objTriangle);
//			objTriangle.Set(c,b,p2);
//			objPolyTriangle2.Add (objTriangle);
//		}
//		else if(index==5)
//		{
//			objTriangle.Set(a,b,p2);
//			objPolyTriangle2.Add (objTriangle);
//			objTriangle.Set(c,a,p2);
//			objPolyTriangle2.Add (objTriangle);
//		}
//		else if(index==6)
//		{
//			objTriangle.Set(a,b,p2);
//			objPolyTriangle2.Add (objTriangle);
//			objTriangle.Set(c,b,p2);
//			objPolyTriangle2.Add (objTriangle);
//		}
//		else
//			objPolyTriangle2.Add (objTriangle2);
//	}
//	return 1;
//}
//Point2DArrayEx::Point2DArrayEx(Point2DArray &pointArray)
//{
//	Point2D point;
//	int index,maxIndex;
//
//	m_minX=m_maxX=m_minY=m_maxY=0.0;
//	maxIndex=pointArray.size();
//	clear();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=pointArray.at(index);
//		v.push_back(point);
//		if(index==0)
//		{
//			m_minX=m_maxX=point.m_x;
//			m_minY=m_maxY=point.m_y;
//		}
//		else
//		{
//			m_minX=(m_minX<point.m_x)?m_minX:point.m_x;
//			m_maxX=(m_maxX>point.m_x)?m_maxX:point.m_x;
//			m_minY=(m_minY<point.m_y)?m_minY:point.m_y;
//			m_maxY=(m_maxY>point.m_y)?m_maxY:point.m_y;
//		}
//	}
//}
Point2DArrayEx::Point2DArrayEx(Point2DArrayEx &pointArray)
{
	Point2D point;
	int index,maxIndex;

	maxIndex=pointArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		point=pointArray.at(index);
		v.push_back(point);
	}
	m_ID=pointArray.m_ID ;
	m_strID=pointArray.m_strID ;
	m_strSubID=pointArray.m_strSubID ;
	m_attr1=pointArray.m_attr1 ;
	m_attr2=pointArray.m_attr2 ;

	m_minX=pointArray.m_minX;
	m_maxX=pointArray.m_maxX;
	m_minY=pointArray.m_minY;
	m_maxY=pointArray.m_maxY;

}
//Point2DArrayEx Point2DArrayEx::operator=(Point2DArray &pointArray)
//{
//	Point2D point;
//	int index,maxIndex;
//
//	m_minX=m_maxX=m_minY=m_maxY=0.0;
//	maxIndex=pointArray.size();
//	clear();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=pointArray.at(index);
//		Add(point);
//		if(index==0)
//		{
//			m_minX=m_maxX=point.m_x;
//			m_minY=m_maxY=point.m_y;
//		}
//		else
//		{
//			m_minX=(m_minX<point.m_x)?m_minX:point.m_x;
//			m_maxX=(m_maxX>point.m_x)?m_maxX:point.m_x;
//			m_minY=(m_minY<point.m_y)?m_minY:point.m_y;
//			m_maxY=(m_maxY>point.m_y)?m_maxY:point.m_y;
//		}
//	}
//	return *this;
//}
Point2DArrayEx Point2DArrayEx::operator=(Point2DArrayEx &pointArray)
{
	Point2D point;
	int index,maxIndex;

	maxIndex=pointArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		point=pointArray.at(index);
		Add(point);
	}
	m_ID=pointArray.m_ID ;
	m_strID=pointArray.m_strID ;
	m_strSubID=pointArray.m_strSubID ;
	m_attr1=pointArray.m_attr1 ;
	m_attr2=pointArray.m_attr2 ;

	m_minX=pointArray.m_minX;
	m_maxX=pointArray.m_maxX;
	m_minY=pointArray.m_minY;
	m_maxY=pointArray.m_maxY;
	return *this;
}
//Point2DArrayEx Point2DArrayEx::operator+=(Point2DArray &pointArray)
//{
//	Point2D point;
//	int index,maxIndex;
//
//	maxIndex=pointArray.size();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=pointArray.at(index);
//		Add(point);
//		if(size()<1 && index==0)
//		{
//			m_minX=m_maxX=point.m_x;
//			m_minY=m_maxY=point.m_y;
//		}
//		else
//		{
//			m_minX=(m_minX<point.m_x)?m_minX:point.m_x;
//			m_maxX=(m_maxX>point.m_x)?m_maxX:point.m_x;
//			m_minY=(m_minY<point.m_y)?m_minY:point.m_y;
//			m_maxY=(m_maxY>point.m_y)?m_maxY:point.m_y;
//		}
//	}
//	return *this;
//}
Point2DArrayEx Point2DArrayEx::operator+=(Point2DArrayEx &pointArray)
{
	Point2D point;
	int index,maxIndex;

	if(size()>0 && pointArray.size ()>0)
	{
		m_minX=(m_minX<pointArray.m_minX)?m_minX:pointArray.m_minX;
		m_maxX=(m_maxX>pointArray.m_maxX)?m_maxX:pointArray.m_maxX;
		m_minY=(m_minY<pointArray.m_minY)?m_minY:pointArray.m_minY;
		m_maxY=(m_maxY>pointArray.m_maxY)?m_maxY:pointArray.m_maxY;
	}
	else if(pointArray.size ()>0)
	{
		m_minX=pointArray.m_minX;
		m_maxX=pointArray.m_maxX;
		m_minY=pointArray.m_minY;
		m_maxY=pointArray.m_maxY;
	}

	maxIndex=pointArray.size();
	for(index=0;index<maxIndex;index++)
	{
		point=pointArray.at(index);
		Add(point);
	}
	return *this;
}

Point2DArrayEx Point2DArrayEx::operator|=(Point2DArrayEx &pointArray)
{
	Point2D point[2];
	Point2DArrayEx pointArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	if(size()>0 && pointArray.size ()>0)
	{
		m_minX=(m_minX<pointArray.m_minX)?m_minX:pointArray.m_minX;
		m_maxX=(m_maxX>pointArray.m_maxX)?m_maxX:pointArray.m_maxX;
		m_minY=(m_minY<pointArray.m_minY)?m_minY:pointArray.m_minY;
		m_maxY=(m_maxY>pointArray.m_maxY)?m_maxY:pointArray.m_maxY;
	}
	else if(pointArray.size ()>0)
	{
		m_minX=pointArray.m_minX;
		m_maxX=pointArray.m_maxX;
		m_minY=pointArray.m_minY;
		m_maxY=pointArray.m_maxY;
	}
	maxIndex=size();
	for(index=0;index<maxIndex;index++)
	{
		point[0]=at(index);
		pointArray1.Add(point[0]);
	}
	maxIndex=pointArray.size();
	for(index=0;index<maxIndex;index++)
	{
		point[0]=pointArray.at(index);
		pointArray1.Add(point[0]);
	}
	clear();
	number1=pointArray1.size();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		point[0]=pointArray1.at(i);
		number2=size();
		for(j=0;j<number2;j++)
		{
        	point[1]=at(j);
			if(point[0]==point[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(point[0]);
		}
	}
	return *this;
}
double Point2DArrayEx::GetDiameter(void)
{
	int      i,j,number;
	int      index1,index2;
	double   temp1,temp2;
	Point2D  point1,point2;
	Vector2D vector;

	temp1=0;
	number=size();
	for(i=0;i<number-1;i++)
	{
		point1=at(i);
		for(j=i+1;j<number;j++)
		{
			point2=at(j);
			temp2=vector.SquareLength(point2.m_x-point1.m_x,
				                       point2.m_y-point1.m_y);
			if(temp1<temp2)
			{
				temp1=temp2;
				index1=i;
				index2=j;           //index1<=index2
			}
		}
	}
	temp1=sqrt(temp1);
	return temp1;
}


int Point2DArrayEx::ExistSharedPoint(Point2DArrayEx pointArray)
{
	int i,j,number1,number2;
	Point2D point[2];

	number1=size();
	number2=pointArray.size ();
	for(i=0;i<number1;i++)
	{
		point[0]=at(i);
		for(j=0;j<number2;j++)
		{
			point[1]=pointArray.at(j);
			if(point[0]==point[1])
				return (i+1)*MaxSize+(j+1);
		}
	}
	return 0;
}
bool Point2DArrayEx::GetNormal(double n[])
{
	int         i,j,number;
	int         index1,index2,index3;
	double      a[2],b[2],c[2];
	double      temp1,temp2,temp;
	Vector2D    vector;
	Point2D     point1,point2,point;

	n[0]=0.0;
	number=size();
	if(number<3)                //不能构成凸包
		return false;
	for(i=0;i<number;i++)
	{
		point=at(i);
		if(i==0)
		{
			index1=0;
			temp=point.m_x;
			continue;
		}
		if(point.m_x <temp)
		{
			temp=point.m_x ;
			index1=i;
		}
	}
	for(i=0;i<number;i++)
	{
		point=at(i);
		if(i==0)
		{
			index2=0;
			temp=point.m_x;
			continue;
		}
		if(point.m_x >temp)
		{
			temp=point.m_x ;
			index2=i;
		}
	}
	point1=at(index1);
	point2=at(index2);
	temp=vector.SquareLength(point2.m_x-point1.m_x,
				            point2.m_y-point1.m_y);
	if(temp<THRESHOLD10)
		return false;

	temp1=0;
	index3=0;
	for(i=0;i<number;i++)        //取到P1P2距离最大的点
	{
		point=at(i);
		temp2=Relation2D::PToLDistance(point1,point2,point);
		if(temp1<temp2)
		{
			temp1=temp2;
			index3=i;
		}
	}
	if(temp1<THRESHOLD6)         //所有点共线
	  	return false;

	if(index1>index2)
	{
		i=index1;
		index1=index2;
		index2=i;
	}
	if(index1>index3)
	{
		i=index1;
		index1=index3;
		index3=i;
	}
	if(index2>index3)
	{
		i=index2;
		index2=index3;
		index3=i;
	}
	point1=at(index1);
	point2=at(index2);
	point =at(index3);
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	n[0]=vector.CrossProduct(b[0]-a[0],b[1]-a[1],
		            c[0]-a[0],c[1]-a[1]);//abXac
	if(abs(n[0])<THRESHOLD6)
	{
		n[0]=0.0;
		return false;
	}
	else
		n[0]/=abs(n[0]);
	return true;
}

bool Point2DArrayEx::GetConvexHull(Point2DArrayEx &pointArray,double n[1])
{

	int    flag;
	int    i,j,number;
	int    index1,index2,index3;
	double a[2],b[2],c[2];
	double p[2];
	double temp1,temp2;

	Vector2D    vector;
	Line2D      line;
	Point2D     point1,point2,point;
	LinkedIndex *vertexIndices;
	LinkedIndex *preCurrent,*current,*rear,*ptemp;

	number=size();              //点集数目
	if(number<3)                   //不能构成凸包
		return false;

	//求出边长最长的边在数组中的位置
	temp1=0;
	for(i=0;i<number-1;i++)
	{
		point1=at(i);
		for(j=i+1;j<number;j++)
		{
			point2=at(j);
			temp2=vector.SquareLength(point2.m_x-point1.m_x,
									  point2.m_y-point1.m_y);
			if(temp1<temp2)
			{
				temp1=temp2;
				index1=i;
				index2=j;           //index1<=index2
			}
		}
	}
	if(temp1<THRESHOLD10)           //所有点重合
		return false;

	temp1=0;
	point1=at(index1);
	point2=at(index2);
	for(i=0;i<number;i++)
	{
		point=at(i);
		temp2=Relation2D::PToLDistance(point1,point2,point);
		if(temp1<temp2)
		{
			temp1=temp2;
			index3=i;
		}
	}
	if(temp1<THRESHOLD10)           //所有点共线
		return false;

	if(index3<index1)
	{
		i=index3;
		index3=index2;
		index2=index1;
		index1=i;                  //index1<index2<index3
	}
	else if(index3>index2)
	{
	}
	else
	{
		i=index3;
		index3=index2;
		index2=i;
	}
	point1=at(index1);
	point2=at(index2);
	point =at(index3);
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	n[0]=vector.CrossProduct(b[0]-a[0],b[1]-a[1],
							 c[0]-a[0],c[1]-a[1]);//abXac
	if(n[0]>THRESHOLD10)
		n[0]=1;
	else if(n[0]<-THRESHOLD10)
		n[0]=-1;
	else
		return false;

	ptemp=new LinkedIndex;
	ptemp->index=index1;
	ptemp->pNext=NULL;
	vertexIndices=ptemp;
	rear=ptemp;

	ptemp=new LinkedIndex;
	ptemp->index=index2;
	ptemp->pNext=NULL;
	rear->pNext=ptemp;
	rear=ptemp;

	ptemp=new LinkedIndex;
	ptemp->index=index3;
	ptemp->pNext=NULL;
	rear->pNext=ptemp;
	rear=ptemp;
	/////////////////凸壳顶点索引表中已经存储有三个索引项
	ptemp=new LinkedIndex;
	ptemp->index=vertexIndices->index;
	ptemp->pNext=NULL;
	rear->pNext=ptemp;
	rear=ptemp;
	/////////////////////闭合点占有两个索引项

	preCurrent=vertexIndices;
	current=preCurrent->pNext;
	while(current!=NULL)
	{
		point1=at(preCurrent->index);
		point2=at(current->index);
		point1.Get(a);
		point2.Get(b);
		temp1=0;
		for(i=0;i<number;i++)
		{
			point=at(i);
			point.Get(c);
			temp2=vector.CrossProduct(b[0]-a[0],b[1]-a[1],
									  c[0]-a[0],c[1]-a[1]);          //abXac
			if(temp2>THRESHOLD10)                          //c在ab右侧
			{
				temp2=Relation2D::PToLDistance(point1,point2,point);
				if(temp1<temp2)
				{
					temp1=temp2;
					index1=i;
				}
			}
		}
		if(temp1<THRESHOLD10)         //当前线段为凸壳的一条边
		{
			preCurrent=current;
			current=current->pNext;
		}
		else                          //在顶点索引表中插入一个索引项
		{
			ptemp=new LinkedIndex;
			ptemp->index=index1;
			preCurrent->pNext=ptemp;
			ptemp->pNext=current;
			//preCurrent=preCurrent;
			current=preCurrent->pNext;
		}
	}//while(current!=NULL)
	preCurrent=vertexIndices;
	current=preCurrent->pNext;
	while(current!=NULL)
	{
		point1=at(preCurrent->index);
		point2=at(current->index);
		point1.Get(a);
		point2.Get(b);
		line.Set(a,b);
		flag=0;
		for(i=0;i<number;i++)
		{
			point=at(i);
			point.Get(c);
			if(line.Contain(c)==3)       //在凸壳的边上有点时，插入该点的索引项
			{
				ptemp=new LinkedIndex;
				ptemp->index=i;
				preCurrent->pNext=ptemp;
				ptemp->pNext=current;
				current=preCurrent->pNext;
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			preCurrent=current;
			current=current->pNext;
		}
	}
	current=vertexIndices;

	pointArray.clear();                      //清空数组
	while(current->pNext!=NULL)
	{
		pointArray.Add(at(current->index));   //闭合点只存储一次
		current=current->pNext;
	}
	return true;
}


//获取凸包？包围盒？
bool Point2DArrayEx::GetConvexHull(IntArray &hullIndices,double n[1])
{
	cout << "convex hull" << endl;
	int      num;
	int      index[2];
	Line2D   line;
	Point2D  point[2];
	vector<int>::iterator start,end,pos;
	IntArray  LeftHullIndices,RightHullIndices;
//	IntArray LeftPointIndices,RightPointIndices;

	num=size();
	if(num<3){
		cout << "getconvex hull < 3" << endl;
		return false;
	}


	IntArray indices,resultIndices;
	for(int i=0;i<num;i++)
		indices.push_back(i);
	//获取最左的
	GetLeftMost(index[0]);
	//获取最右的
	GetRightMost(index[1]);

	point[0]=at(index[0]);
	point[1]=at(index[1]);

	RightHullIndices.push_back(index[0]);
	RightHullIndices.push_back(index[1]);
	start = RightHullIndices.begin();
	end=start;
	//链表获取下一个
//	RightHullIndices.(end);
	end++;
	line.Set(point[0],point[1]);
	GetRightPoints(line,indices,resultIndices);
	cout << "start " << *start << " end " << *end << endl;
	GetRightHull(start,end,resultIndices,RightHullIndices);

	LeftHullIndices.push_back (index[1]);
	LeftHullIndices.push_back (index[0]);
	start=LeftHullIndices.begin ();
	end=start;
//	LeftHullIndices.(end);
	end++;
	line.Set(point[1],point[0]);
	GetRightPoints(line,indices,resultIndices);
	GetRightHull(start,end,resultIndices,LeftHullIndices);
	cout << "last hull"  << hullIndices.size() << endl;
	hullIndices.clear ();
	num=RightHullIndices.size ();
	pos=RightHullIndices.begin ();
	for(int i=0;i<num;i++)
	{
		index[0]= *pos++;
		hullIndices.push_back(index[0]);
	}

	num=LeftHullIndices.size ();
	pos=LeftHullIndices.begin ();
	for(int i=0;i<num;i++)
	{
		//这里算是搞对了，应该是这个pos++后才行，尼玛自己还是太弟弟了。
		index[0]=*pos++;
		if(i==0 || i==num-1)continue;
		hullIndices.push_back (index[0]);
	}

	return true;
}


bool Point2DArrayEx::GetLeftPoints(Line2D &line,IntArray& subsetIndices,IntArray& resultIndices)   //取有向线段右侧点，输出点的序号
{
	int         i,num;
	double      a[2],b[2],c[2];
	Vector2D    v;

	resultIndices.clear ();
	if(line.Length ()<THRESHOLD5)
		return false;
	num=subsetIndices.size();
	if(num<1)return false;

	line.Get (a,b);
	for(i=0;i<num;i++)
	{
		int index=subsetIndices.at (i);
		Point2D pt=at(index);
		pt.Get (c);
		double temp=v.CrossProduct (b[0]-a[0],b[1]-a[1],c[0]-a[0],c[1]-a[1]);
		if(temp>THRESHOLD10)
			resultIndices.push_back  (index);
	}
	if(resultIndices.size ())
		return true;
	return false;
}
bool Point2DArrayEx::GetRightPoints(Line2D &line,IntArray& subsetIndices,IntArray& resultIndices)    //取有向线段左侧点，输出点的序号
{
	int         i,num;
	double      a[2],b[2],c[2];
	Vector2D    v;

	resultIndices.clear();
	if(line.Length ()<THRESHOLD5)
		return false;
	num=subsetIndices.size();
	if(num<1)return false;

	line.Get (a,b);
	for(i=0;i<num;i++)
	{
		int index=subsetIndices.at(i);
		Point2D pt=at(index);
		pt.Get (c);
		double temp=v.CrossProduct (b[0]-a[0],b[1]-a[1],c[0]-a[0],c[1]-a[1]);
		if(temp<-THRESHOLD10)
			resultIndices.push_back  (index);
	}
	if(resultIndices.size ())
		return true;
	return false;
}
bool Point2DArrayEx::GetFarMost(Line2D &line,IntArray& subsetIndices,int &index)              //取最左侧点，输出点的序号
{
	double a[2],b[2],c[2];

	index=-1;
	if(subsetIndices.size ()<1)
		return false;

	Vector2D v;
	line.Get (a,b);
	v.Set(b[0]-a[0],b[1]-a[1]);
	v.Normalize ();
	v.Get (b);

	double temp1=0.0,temp2=0.0;
	int num=subsetIndices.size ();
	for(int i=0;i<num;i++)
	{
		Point2D pt=at(subsetIndices.at (i));
		pt.Get (c);
		for(int j=0;j<2;j++)
			c[j]-=a[j];
		double t=Vector2D::DotProduct (b,c);
		for(int j=0;j<2;j++)
			c[j]-=b[j]*t;
		temp2=v.SquareLength (c);
		if(temp1<temp2-THRESHOLD10)
		{
			temp1=temp2;
			index=subsetIndices.at (i);
		}
	}
	if(index>-1)
		return true;
	return false;
}

bool Point2DArrayEx::GetLeftMost(int& index)
{
	index=-1;
	int num=size();
	if(num<2)return false;

	index=0;
	Point2D point=at(0);
	double minX=point.m_x;
	for(int i=1;i<num;i++)
	{
		point=at(i);
		if(point.m_x <minX)
		{
			minX=point.m_x ;
			index=i;
		}
	}
	return true;
}
bool Point2DArrayEx::GetRightMost(int& index)
{
	index=-1;
	int num=size();
	if(num<2)return false;

	index=0;
	Point2D point=at(0);
	double maxX=point.m_x;
	for(int i=1;i<num;i++)
	{
		point=at(i);
		if(point.m_x >maxX)
		{
			maxX=point.m_x ;
			index=i;
		}
	}
	return true;
}
//bool Point2DArrayEx::GetLeftHull(POSITION start,POSITION end,IntArray &subsetIndices,IntList& vertexIndices)
//{
//	POSITION pos;
//	Point2D  point[3];
//	Line2D   line;
//	int      index;
//
//	if(subsetIndices.size ()<1)
//		return false;
//	point[0]=at(vertexIndices.at (start));
//	point[1]=at(vertexIndices.at (end));
//	line.Set(point[0],point[1]);
//	if(!GetFarMost(line,subsetIndices,index))
//		return false;
//
//	vertexIndices.InsertAfter (start,index);
//	pos=start;
//	vertexIndices.GetNext (pos);
//
//	IntArray indices;
//	point[2]=at(index);
//	line.Set(point[0],point[2]);
//	GetLeftPoints(line,subsetIndices,indices);
//	GetLeftHull(start,pos,indices,vertexIndices);
//
//	line.Set(point[2],point[1]);
//	GetLeftPoints(line,subsetIndices,indices);
//	GetLeftHull(pos,end,indices,vertexIndices);
//	return true;
//}
//


bool Point2DArrayEx::GetRightHull(POSITION& start,POSITION& end,IntArray &subsetIndices,IntArray& vertexIndices)
{
	POSITION pos;
	Point2D  point[3];
	Line2D   line;
	int      index;

	if(subsetIndices.size ()<1)
		return false;
	cout << "getRight hull vertexIndices size " << vertexIndices.size() << " " <<*start << " "  << *end <<  endl;
	cout << "pint 2d size " << size() << endl;
	point[0]=at(*start);
	//这里出的问题，但是end是怎么传入的呢？
	point[1]=at(*end);
	line.Set(point[0],point[1]);
	if(!GetFarMost(line,subsetIndices,index))
		return false;
	cout << "get in success" << endl;

	//这里的list insert和after？
	//在后面插入
	//这里insert后都失效了
	start = vertexIndices.insert(++start,index);
	end = start;
	end++;
	pos=start;
//	vertexIndices.GetNext (pos);
	pos++;
	IntArray indices;
	point[2]=at(index);
	line.Set(point[0],point[2]);
	GetRightPoints(line,subsetIndices,indices);
	GetRightHull(start,pos,indices,vertexIndices);

	line.Set(point[2],point[1]);
	GetRightPoints(line,subsetIndices,indices);
	GetRightHull(pos,end,indices,vertexIndices);
	return true;
}
int Point2DArrayEx::HasElement(double x,double y)
{
	int i,number;
	Point2D point[2];

	point[0].m_x =x,point[0].m_y =y;
	number=size();
	for(i=0;i<number;i++)
	{
		point[1]=at(i);
		if(point[0]==point[1])
			return 1;
	}
	return 0;
}
int Point2DArrayEx::HasElement(double a[])
{
	return HasElement(a[0],a[1]);
}
int Point2DArrayEx::HasElement(Point2D point)
{
	return HasElement(point.m_x ,point.m_y);
}
Point2DArrayEx Point2DArrayEx::ReverseOrder(void)
{
	int        i,num;
	Point2D    point;
	Point2DArrayEx pointArray;
	num=size();
	for(i=num;i>0;)
	{
		i--;
		point=at(i);
		pointArray.Add(point);
	}
	pointArray.m_attr1 =m_attr1;
	pointArray.m_attr2 =m_attr2;
	pointArray.m_ID =m_ID;
	return pointArray;
}
void Point2DArrayEx::Add(double x,double y)
{
	if(size()==0)
	{
		m_minX=m_maxX=x ;
		m_minY=m_maxY=y;
	}
	else
	{
		m_minX=(m_minX<x)?m_minX:x;
		m_minY=(m_minY<y)?m_minY:y;

		m_maxX=(m_maxX>x)?m_maxX:x;
		m_maxY=(m_maxY>y)?m_maxY:y;
	}
	Point2D point(x,y);
	v.push_back(point);
}

void Point2DArrayEx::Add(double a[])
{
	if(size()==0)
	{
		m_minX=m_maxX=a[0];
		m_minY=m_maxY=a[1];
	}
	else
	{
		m_minX=(m_minX<a[0])?m_minX:a[0];
		m_minY=(m_minY<a[1])?m_minY:a[1];

		m_maxX=(m_maxX>a[0])?m_maxX:a[0];
		m_maxY=(m_maxY>a[1])?m_maxY:a[1];
	}
	Point2D point(a);
	v.push_back(point);
}
void Point2DArrayEx::Add(Point2D &point)
{
	if(size()==0)
	{
		m_minX=m_maxX=point.m_x ;
		m_minY=m_maxY=point.m_y;
	}
	else
	{
		m_minX=(m_minX<point.m_x)?m_minX:point.m_x;
		m_minY=(m_minY<point.m_y)?m_minY:point.m_y;

		m_maxX=(m_maxX>point.m_x)?m_maxX:point.m_x;
		m_maxY=(m_maxY>point.m_y)?m_maxY:point.m_y;
	}
	v.push_back (point);
}


//void Point2DArrayEx::InsertAt(int index,Point2D& point)
//{
//	if(size()==0)
//	{
//		m_minX=m_maxX=point.m_x ;
//		m_minY=m_maxY=point.m_y;
//	}
//	else
//	{
//		m_minX=(m_minX<point.m_x)?m_minX:point.m_x;
//		m_minY=(m_minY<point.m_y)?m_minY:point.m_y;
//
//		m_maxX=(m_maxX>point.m_x)?m_maxX:point.m_x;
//		m_maxY=(m_maxY>point.m_y)?m_maxY:point.m_y;
//	}
//	v.insert(v.begin() + index, point);
//}

//Point3DArrayEx::Point3DArrayEx(Point3DArrayEx &pointArray)
//{
//	Point3D point;
//	int index,maxIndex;
//
//	maxIndex=pointArray.size();
//	clear();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=pointArray.at(index);
//		Add(point);
//	}
//	m_ID=pointArray.m_ID ;
//	m_strID=pointArray.m_strID ;
//	m_strSubID=pointArray.m_strSubID ;
//	m_attr1=pointArray.m_attr1 ;
//	m_attr2=pointArray.m_attr2 ;
//
//	m_minX=pointArray.m_minX;
//	m_minY=pointArray.m_minY;
//	m_minZ=pointArray.m_minZ;
//
//	m_maxX=pointArray.m_maxX;
//	m_maxY=pointArray.m_maxY;
//	m_maxZ=pointArray.m_maxZ;
//
//}
//Point3DArrayEx::Point3DArrayEx(Point3DArrayEx &&pointArray)
//{
//	Point3D point;
//	int index,maxIndex;
//
//	maxIndex=pointArray.size();
//	clear();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=pointArray.at(index);
//		Add(point);
//	}
//	m_ID=pointArray.m_ID ;
//	m_strID=pointArray.m_strID ;
//	m_strSubID=pointArray.m_strSubID ;
//	m_attr1=pointArray.m_attr1 ;
//	m_attr2=pointArray.m_attr2 ;
//
//	m_minX=pointArray.m_minX;
//	m_minY=pointArray.m_minY;
//	m_minZ=pointArray.m_minZ;
//
//	m_maxX=pointArray.m_maxX;
//	m_maxY=pointArray.m_maxY;
//	m_maxZ=pointArray.m_maxZ;
//
//}

Point3DArrayEx Point3DArrayEx::operator=(Point3DArrayEx pointArray)
{
	Point3D point;
	int index,maxIndex;

	maxIndex=pointArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		point=pointArray.at(index);
		Add(point);
	}
	m_ID=pointArray.m_ID ;
	m_strID=pointArray.m_strID ;
	m_strSubID=pointArray.m_strSubID ;
	m_attr1=pointArray.m_attr1 ;
	m_attr2=pointArray.m_attr2 ;

	m_minX=pointArray.m_minX;
	m_minY=pointArray.m_minY;
	m_minZ=pointArray.m_minZ;

	m_maxX=pointArray.m_maxX;
	m_maxY=pointArray.m_maxY;
	m_maxZ=pointArray.m_maxZ;

	return *this;
}

Point3DArrayEx Point3DArrayEx::operator+=(Point3DArrayEx &pointArray)
{
	Point3D point;
	int index,maxIndex;

	if(size()>0 && pointArray.size()>0)
	{
		m_minX=(m_minX<pointArray.m_minX)?m_minX:pointArray.m_minX;
		m_maxX=(m_maxX>pointArray.m_maxX)?m_maxX:pointArray.m_maxX;
		m_minY=(m_minY<pointArray.m_minY)?m_minY:pointArray.m_minY;
		m_maxY=(m_maxY>pointArray.m_maxY)?m_maxY:pointArray.m_maxY;
		m_minZ=(m_minZ<pointArray.m_minZ)?m_minZ:pointArray.m_minZ;
		m_maxZ=(m_maxZ>pointArray.m_maxZ)?m_maxZ:pointArray.m_maxZ;
	}
	else if(pointArray.size ()>0)
	{
		m_minX=pointArray.m_minX;
		m_minY=pointArray.m_minY;
		m_minZ=pointArray.m_minZ;

		m_maxX=pointArray.m_maxX;
		m_maxY=pointArray.m_maxY;
		m_maxZ=pointArray.m_maxZ;
	}
	maxIndex=pointArray.size();
	for(index=0;index<maxIndex;index++)
	{
		point=pointArray.at(index);
		Add(point);
	}
	return *this;
}
Point3DArrayEx Point3DArrayEx::operator|=(Point3DArrayEx &pointArray)
{
	Point3D point[2];
	Point3DArrayEx pointArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	if(size()>0 && pointArray.size()>0)
	{
		m_minX=(m_minX<pointArray.m_minX)?m_minX:pointArray.m_minX;
		m_maxX=(m_maxX>pointArray.m_maxX)?m_maxX:pointArray.m_maxX;
		m_minY=(m_minY<pointArray.m_minY)?m_minY:pointArray.m_minY;
		m_maxY=(m_maxY>pointArray.m_maxY)?m_maxY:pointArray.m_maxY;
		m_minZ=(m_minZ<pointArray.m_minZ)?m_minZ:pointArray.m_minZ;
		m_maxZ=(m_maxZ>pointArray.m_maxZ)?m_maxZ:pointArray.m_maxZ;
	}
	else if(pointArray.size ()>0)
	{
		m_minX=pointArray.m_minX;
		m_minY=pointArray.m_minY;
		m_minZ=pointArray.m_minZ;

		m_maxX=pointArray.m_maxX;
		m_maxY=pointArray.m_maxY;
		m_maxZ=pointArray.m_maxZ;
	}
	maxIndex=size();
	for(index=0;index<maxIndex;index++)
	{
		point[0]=at(index);
		pointArray1.Add(point[0]);
	}
	maxIndex=pointArray.size();
	for(index=0;index<maxIndex;index++)
	{
		point[0]=pointArray.at(index);
		pointArray1.Add(point[0]);
	}
	clear();
	number1=pointArray1.size();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		point[0]=pointArray1.at(i);
		number2=size();
		for(j=0;j<number2;j++)
		{
        	point[1]=at(j);
			if(point[0]==point[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(point[0]);
		}
	}
	return *this;
}
//////////////////////////////////////////闭合点不重复存储
bool Point3DArrayEx::GetNormal(double n[])
{
	int         i,j,number;
	int         index1,index2,index3;
	double      a[3],b[3],c[3];
	double      temp1,temp2,temp;
	Vector3D    vector;
	Point3D     point1,point2,point;

	n[0]=n[1]=n[2]=0.0;
	//size为0，我日了。
	number=size();
	if(number<3){//不能构成凸包
		cout << "number < 3 " << number <<  endl;
		return false;
	}

	for(i=0;i<number;i++)
	{
		point=at(i);
		if(i==0)
		{
			index1=0;
			temp=point.m_x;
			continue;
		}
		if(point.m_x <temp)
		{
			temp=point.m_x ;
			index1=i;
		}
	}
	for(i=0;i<number;i++)
	{
		point=at(i);
		if(i==0)
		{
			index2=0;
			temp=point.m_x;
			continue;
		}
		if(point.m_x >temp)
		{
			temp=point.m_x ;
			index2=i;
		}
	}
	point1=at(index1);
	point2=at(index2);
	temp=vector.SquareLength(point2.m_x-point1.m_x,
				            point2.m_y-point1.m_y,
							point2.m_z-point1.m_z);
	if(temp<THRESHOLD10)
	{
		for(i=0;i<number;i++)
		{
			point=at(i);
			if(i==0)
			{
				index1=0;
				temp=point.m_y;
				continue;
			}
			if(point.m_y <temp)
			{
				temp=point.m_y ;
				index1=i;
			}
		}
		for(i=0;i<number;i++)
		{
			point=at(i);
			if(i==0)
			{
				index2=0;
				temp=point.m_y;
				continue;
			}
			if(point.m_y >temp)
			{
				temp=point.m_y ;
				index2=i;
			}
		}
		point1=at(index1);
		point2=at(index2);
		temp=vector.SquareLength(point2.m_x-point1.m_x,
								point2.m_y-point1.m_y,
								point2.m_z-point1.m_z);
	}
	if(temp<THRESHOLD10)
		return false;

	temp1=0;
	index3=0;
	for(i=0;i<number;i++)        //取到P1P2距离最大的点
	{
		point=at(i);
		temp2=Relation3D::PToLDistance(point1,point2,point);
		if(temp1<temp2)
		{
			temp1=temp2;
			index3=i;
		}
	}
	if(temp1<THRESHOLD6)         //所有点共线
	  	return false;

	if(index1>index2)
	{
		i=index1;
		index1=index2;
		index2=i;
	}
	if(index1>index3)
	{
		i=index1;
		index1=index3;
		index3=i;
	}
	if(index2>index3)
	{
		i=index2;
		index2=index3;
		index3=i;
	}
	point1=at(index1);
	point2=at(index2);
	point =at(index3);
	point1.Get(a);
	point2.Get(b);
	point.Get(c);
	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
		            c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
	if(vector.Normalize())
	{
		vector.Get(n);
	}
	else
	{
		n[0]=n[1]=n[2]=0.0;
		return false;
	}
	return true;
}
int Point3DArrayEx::operator==(Point3DArrayEx &pointArray)
{
	int       flag;
	int       i;
	int       number1,number2;
	Point3D   point[2];

	number1=size();
	number2=pointArray.size();
	if(number1!=number2)
		return 0;
	flag=0;
	for(i=0;i<number1;i++)                 //number1==number2
	{
		point[0]=at(i);
		point[1]=pointArray.at(i);
		flag=(point[0]==point[1]);
		if(flag==0)
			break;
	}
	if(i==number1)
		return 1;
	flag=0;
	for(i=0;i<number1;i++)                  //number1==number2
	{
		point[0]=at(i);
		point[1]=pointArray.at(number1-1-i);
		flag=(point[0]==point[1]);
		if(flag==0)
			break;
	}
	if(i==number1)
		return -1;
	return 0;
}
double Point3DArrayEx::GetDiameter(void)
{
	int      i,j,number;
	int      index1,index2;
	double   temp1,temp2;
	Point3D  point1,point2;
	Vector3D vector;

	temp1=0;
	number=size();
	for(i=0;i<number-1;i++)
	{
		point1=at(i);
		for(j=i+1;j<number;j++)
		{
			point2=at(j);
			temp2=vector.SquareLength(point2.m_x-point1.m_x,
				                       point2.m_y-point1.m_y,
									   point2.m_z-point1.m_z);
			if(temp1<temp2)
			{
				temp1=temp2;
				index1=i;
				index2=j;            //index1<=index2
			}
		}
	}
	temp1=sqrt(temp1);
	return temp1;
}
bool Point3DArrayEx::GetPseudoConvexHull(IntArray &hullIndices,double n[3])
{
	//建立平面坐标系
	Vector3D v;
	if(!(v.Normalize(n))){
		cout << "pseudoConvex hull normalize failed" << endl;
		return false;
	}


	double x[3],y[3];          //平面坐标轴单位化方向矢量
	if(abs(n[2])<1-THRESHOLD5)
	{
		x[0]=n[1],x[1]=-n[0],x[2]=0;
		v.Normalize (x);
	}
	else
	{
		x[0]=0,x[1]=1,x[2]=0;
	}
	v.CrossProduct (n,x);
	v.Get (y);

	//空间点向平面投影
	Point2DArrayEx points;
	int num=size();
	for(int i=0;i<num;i++)
	{
		Point3D pt=at (i);
		double p[3],q[2];
		pt.Get (p);
		double t=v.DotProduct(p,n);
		for(int j=0;j<3;j++)    //空间点投影到平面
			p[j]-=n[j]*t;
		q[0]=v.DotProduct (p,x);//投影点在平面上的坐标
		q[1]=v.DotProduct (p,y);//投影点在平面上的坐标
		points.Add (q);
	}

	return points.GetConvexHull (hullIndices,n);
}

//用比较老的方法去生成凸包
bool Point3DArrayEx::GetPlanarConvexHull(Point3DArrayEx &pointArray,double n[])
{
//	cout << "hull" << endl;
//	IntArray indices;
	if(!GetNormal(n)){
		cout << "GetNormal failed" << endl;
		return false; //zjg add
	}


	pointArray  = GetPseudoConvexHull(n);

//	cout << "hull 1" << endl;
//	pointArray.clear();
//	int num=indices.size ();
//	cout << "3d size() " << size() << endl;
//	cout << "indices's size " << num << endl;
//	for(int i = 0; i < num;i++){
//		cout << indices[i] << endl;
//	}
//	for(int i=0;i<num;i++)
//	{
//		cout << indices[i] << endl;
//		Point3D pt= at(indices[i]);
//		pointArray.Add (pt);
//	}
	cout << "planarconvexhull success" << endl;
	return true;
}


bool Point3DArrayEx::GetPlanarConvexHull(Line3DArrayEx &lineArray,double n[])
{
	//注意一边上有多个点的情况
	int        i,number;
	Point3D    point[2];
	Line3D     line;
	Point3DArrayEx pointArray;

	if(!GetPlanarConvexHull(pointArray,n))
		return false;
	number=pointArray.size();
	lineArray.clear();
	for(i=0;i<number;i++)
	{
		point[0]=pointArray.at(i);
		point[1]=pointArray.at((i+1)%number);
		line.Set(point[0],point[1]);
		lineArray.Add(line);
	}
	return true;
}
Point3DArrayEx Point3DArrayEx::GetPseudoConvexHull(double n[])
{
	int         flag;
	int         i,j,k,number;
	int         index1,index2,index3;
	double      a[3],b[3],c[3];
	double      p[3],p1[3];
	double      n1[3];
	double      temp1,temp2,temp;
	Relation3D Relation3D;
	Point3DArrayEx  pointArray;

	Vector3D   vector;
	Line3D     line;
	Point3D    point1,point2,point;
	LinkedIndex *vertexIndices;
	LinkedIndex *preCurrent,*current,*rear,*ptemp;

	vector.Set (n);
	if(vector.Normalize ())
		vector.Get (n1);
	else
		return pointArray;

	number=size();              //点的数目
	if(number<3)                   //不能构成凸包
		return pointArray;

	//求出边长最长的边在数组中的位置
	temp1=0;
	for(i=0;i<number-1;i++)
	{
		point1=at(i);
		point1.Get (a);
		for(j=i+1;j<number;j++)
		{
			point2=at(j);
			point2.Get(b);
			for(k=0;k<3;k++)
				p1[k]=b[k]-a[k];
			temp=vector.DotProduct (n1,p1);
			for(k=0;k<3;k++)
				p1[k]-=n1[k]*temp;  //p1为ab在n1的垂直平面上的分量
			temp2=vector.SquareLength(p1);
			if(temp1<temp2)
			{
				temp1=temp2;
				index1=i;
				index2=j;           //index1<=index2
			}
		}
	}
	if(temp1<THRESHOLD10)           //所有点重合
		return pointArray;
	point1=at(index1);
	point1.Get (p1);
	temp=vector.DotProduct (n1,p1);
	for(k=0;k<3;k++)
		p1[k]-=n1[k]*temp;
	point1.Set(p1);                 //把点投影到n1的垂直平面上

	point2=at(index2);
	point2.Get (p1);
	temp=vector.DotProduct (n1,p1);
	for(k=0;k<3;k++)
		p1[k]-=n1[k]*temp;
	point2.Set(p1);                 //把点投影到n1的垂直平面上
	temp1=0;
	for(i=0;i<number;i++)
	{
		point=at(i);
		point.Get (p1);
		temp=vector.DotProduct (n1,p1);
		for(k=0;k<3;k++)
			p1[k]-=n1[k]*temp;
		point.Set(p1);            //把点投影到n1的垂直平面上
		temp2=Relation3D::PToLDistance(point1,point2,point);
		if(temp1<temp2)
		{
			temp1=temp2;
			index3=i;
		}
	}
	if(temp1<THRESHOLD10)         //所有点共线
	  	return pointArray;

	if(index3<index1)
	{
		i=index3;
		index3=index2;
		index2=index1;
		index1=i;                 //index1<index2<index3
	}
	else if(index3>index2)
	{
	}
	else
	{
		i=index3;
		index3=index2;
		index2=i;
	}
	point1=at(index1);
	point1.Get (p1);
	temp=vector.DotProduct (n1,p1);
	for(k=0;k<3;k++)
		p1[k]-=n1[k]*temp;
	point1.Set(p1);

	point2=at(index2);
	point2.Get (p1);
	temp=vector.DotProduct (n1,p1);
	for(k=0;k<3;k++)
		p1[k]-=n1[k]*temp;
	point2.Set(p1);

	point =at(index3);
	point.Get (p1);
	temp=vector.DotProduct (n1,p1);
	for(k=0;k<3;k++)
		p1[k]-=n1[k]*temp;
	point.Set(p1);

	point1.Get(a);            //a,b,c为n的垂直平面上的点
	point2.Get(b);
	point.Get(c);
	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
		            c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
	if(vector.Normalize())
		vector.Get(n1);       //凸壳的法向量，方向与点的输入顺序有关
	else
		return pointArray;

	if(vector.DotProduct (n,n1)<-THRESHOLD10)
		for(i=0;i<3;i++)
			n1[i]=-n[i];
	////////////建立凸壳顶点初始链表/////////////
	ptemp=new LinkedIndex;
	ptemp->index=index1;
	ptemp->pNext=NULL;
	vertexIndices=ptemp;
	rear=ptemp;

	ptemp=new LinkedIndex;
	ptemp->index=index2;
	ptemp->pNext=NULL;
	rear->pNext=ptemp;
	rear=ptemp;

	ptemp=new LinkedIndex;
	ptemp->index=index3;
	ptemp->pNext=NULL;
	rear->pNext=ptemp;
	rear=ptemp;
	/////////凸壳顶点链表中已经存储有三个项////////
	ptemp=new LinkedIndex;
	ptemp->index=vertexIndices->index;   //加入第一个顶点链尾
	ptemp->pNext=NULL;
	rear->pNext=ptemp;
	rear=ptemp;
	/////////////闭合点占有两个索引项/////////////////

	preCurrent=vertexIndices;
	current=preCurrent->pNext;
	while(current!=NULL)                 //求凸壳
	{
		point1=at(preCurrent->index);
		point1.Get (p1);
		temp=vector.DotProduct (n1,p1);
		for(k=0;k<3;k++)
			p1[k]-=n1[k]*temp;
		point1.Set(p1);

		point2=at(current->index);
		point2.Get (p1);
		temp=vector.DotProduct (n1,p1);
		for(k=0;k<3;k++)
			p1[k]-=n1[k]*temp;
		point2.Set(p1);

		point1.Get(a);
		point2.Get(b);             //a,b为n1的垂直平面上的点
		temp1=0;
		for(i=0;i<number;i++)
		{
			point=at(i);
			point.Get (p1);
			temp=vector.DotProduct (n1,p1);
			for(k=0;k<3;k++)
				p1[k]-=n1[k]*temp;
			point.Set(p1);
			point.Get(c);           //c为n1的垂直平面上的点
			vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
				            c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
	        vector.Get(p);
			temp2=vector.DotProduct(n1,p);
			if(temp2<-THRESHOLD10)         //从n1来看bc在ab右侧
			{
				temp2=Relation3D::PToLDistance(point1,point2,point);
				if(temp1<temp2)
				{
					temp1=temp2;
					index1=i;
				}
			}
		}
		if(temp1<THRESHOLD10)         //当前线段为凸壳的一条边
		{
			preCurrent=current;
			current=current->pNext;
		}
		else                       //在顶点索引表中插入一个索引项
		{
			ptemp=new LinkedIndex;
	        ptemp->index=index1;
			preCurrent->pNext=ptemp;
			ptemp->pNext=current;
			//preCurrent=preCurrent;
			current=preCurrent->pNext;
		}
	}//while(current!=NULL)
	preCurrent=vertexIndices;
	current=preCurrent->pNext;
	while(current!=NULL) //处理凸壳边上的顶点
	{
		point1=at(preCurrent->index);
		point1.Get (p1);
		temp=vector.DotProduct (n1,p1);
		for(k=0;k<3;k++)
			p1[k]-=n1[k]*temp;
		point1.Set(p1);

		point2=at(current->index);
		point2.Get (p1);
		temp=vector.DotProduct (n1,p1);
		for(k=0;k<3;k++)
			p1[k]-=n1[k]*temp;
		point2.Set(p1);
		point1.Get(a);
		point2.Get(b);
		line.Set(a,b);
		flag=0;
		for(i=0;i<number;i++)
		{
			point=at(i);
			point.Get (p1);
			temp=vector.DotProduct (n1,p1);
			for(k=0;k<3;k++)
				p1[k]-=n1[k]*temp;
			point.Set(p1);
			point.Get(c);
			if(line.Contain(c)==3)//在凸壳的边上有点时，插入该点的索引项
			{
				ptemp=new LinkedIndex;
	            ptemp->index=i;
			    preCurrent->pNext=ptemp;
			    ptemp->pNext=current;
				current=preCurrent->pNext;
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			preCurrent=current;
			current=current->pNext;
		}
	}
	current=vertexIndices;
	while(current->pNext!=NULL)
	{
		point1=at(current->index);
		pointArray.Add(point1);        //闭合点只存储一次
		current=current->pNext;
	}
	if(vector.DotProduct (n,n1)<-THRESHOLD10)
		pointArray.ReverseOrder();     //从n看凸壳顶点逆时针排列
	return pointArray;
}
//int Point3DArrayEx::GetSpatialConvexHull(Triangle3DArray &triangleArray)
//{
//	int         flag;
//	int         i,j,k,m,number,num;
//	int         index1,index2,index;
//	double      n[3],p[3];
//	double      temp1,temp2;
//	double      a[3],b[3],c[3],d[3];
//	Point3D     point1,point2,point3,point4,point;
//	Vector3D    vector;
//	Relation3D    Relation3D;
//	Line3D        line;
//	Triangle3D        triangle,objTriangle[4];
//	Triangle3DArrayEx convexTriFacetArray;
//	PolyInt   indices,tempPolyInt;
//	LinkedIndex    *preCurrent,*current,*PreCurrent1,*Current1;
//	LinkedIndex    *FacetIndexArray;
//	LinkedIndex    *rear,*ptemp;
//
//	convexTriFacetArray.clear();
//	number=size();
//	if(number<4)                  //不能构成凸包
//		return 0;
//	for(i=0;i<number;i++)
//	{
//		tempPolyInt.Add (1);
//	}
//	temp1=0;
//	for(i=0;i<number-1;i++)
//	{
//		point1=at(i);
//		for(j=i+1;j<number;j++)
//		{
//			point2=at(j);
//			temp2=vector.SquareLength(point2.m_x-point1.m_x,
//				point2.m_y-point1.m_y,
//				point2.m_z-point1.m_z);
//			if(temp1<temp2)
//			{
//				temp1=temp2;
//				index1=i;
//				index2=j;
//			}
//		}
//	}
//	if(temp1<THRESHOLD10)//所有点重合
//		return 0;
//
//	temp1=0;
//	point1=at(index1);
//	point2=at(index2);
//	for(i=0;i<number;i++)//number=pointArray.size();
//	{
//
//		point=at(i);
//		temp2=Relation3D::PToLDistance(point1,point2,point);
//		if(temp1<temp2)
//		{
//			temp1=temp2;
//			index=i;
//		}
//	}
//	if(temp1<THRESHOLD5)                         //所有点共线
//		return 0;
//	point3= at(index);
//	point1.Get(a);
//	point2.Get(b);
//	point3.Get(c);
//	tempPolyInt.SetAt (index1,0);
//	tempPolyInt.SetAt (index2,0);
//	tempPolyInt.SetAt (index,0);
//	temp1=0;
//	for(i=0;i<number;i++)
//	{
//		point=at(i);
//		point.Get(d);
//		temp2=Relation3D::PToFDistance(a,b,c,d);
//		if(temp1<temp2)
//		{
//			temp1=temp2;
//			index=i;
//		}
//	}
//	if(temp1<THRESHOLD5)                          //所有点共面
//		return 0;
//	point4 =at(index);
//	point4.Get(d);
//	tempPolyInt.SetAt (index,0);
//
//	vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//		c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
//	vector.Normalize();
//	vector.Get(n);
//	for(i=0;i<3;i++)
//		p[i]=d[i]-a[i];
//	vector.Normalize(p);
//	if(vector.DotProduct(n,p)>THRESHOLD5)
//	{
//		convexTriFacetArray.Add(a,c,b);
//		convexTriFacetArray.Add(b,d,a);
//		convexTriFacetArray.Add(c,a,d);
//		convexTriFacetArray.Add(d,b,c);
//
//	}
//	else
//	{
//		convexTriFacetArray.Add(a,b,c);
//		convexTriFacetArray.Add(b,a,d);
//		convexTriFacetArray.Add(c,d,a);
//		convexTriFacetArray.Add(d,c,b);
//	}
//	for(i=0;i<4;i++)
//		indices.Add (1);
//	for(i=0;i<number;i++)
//	{
//		index=tempPolyInt.at (i);
//		if(index==0)
//			continue;
//		point=at(i);
//		point.Get(d);
//		//tempPolyInt.SetAt (i,0);
//		num=convexTriFacetArray.size ();
//		for(j=0;j<num;j++)
//		{
//			index=indices.at (j);
//			if(index==0)
//				continue;
//			triangle=convexTriFacetArray.at(j);
//			triangle.Get(a,b,c);
//			vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//				c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
//			if(vector.Normalize())              //为真正的三角形
//				vector.Get(n);
//			else
//			{
//				indices.SetAt (j,0);
//				continue;
//			}
//			for(k=0;k<3;k++)
//				p[k]=d[k]-a[k];
//			vector.Normalize(p);
//			if(vector.DotProduct(n,p)<THRESHOLD5)//点在面片的反侧
//			{
//				continue;
//			}
//			//triangle[0].Set(a,c,b);
//			indices.SetAt (j,0);
//			objTriangle[0].Set(b,d,a);
//			objTriangle[1].Set(c,a,d);
//			objTriangle[2].Set(d,b,c);
//
//			for(k=0;k<3;k++)
//			{
//				for(m=0;m<num;m++)
//				{
//					index=indices.at (m);
//					if(index==0)
//						continue;
//					triangle=convexTriFacetArray.at(m);
//					if(triangle==objTriangle[k])
//					{
//						indices.SetAt (m,0);
//						break;
//					}
//				}
//				if(m==num && num>0)//
//				{
//					convexTriFacetArray.Add(objTriangle[k]);
//					indices.Add (1);
//				}
//			}
//			num=convexTriFacetArray.size ();
//		}
//	}
//	num=convexTriFacetArray.size ();
//	for(i=0;i<num;i++)
//	{
//		index=indices.at (i);
//		if(index==0)
//			continue;
//		triangle=convexTriFacetArray.at (i);
//		triangleArray.Add (triangle);
//	}
//	return 1;	//add by hw
//
//}
//int Point3DArrayEx::ExistSharedPoint(Point3DArrayEx pointArray)
//{
//	int i,j,number1,number2;
//	Point3D point[2];
//
//	number1=size();
//	number2=pointArray.size ();
//	for(i=0;i<number1;i++)
//	{
//		point[0]=at(i);
//		for(j=0;j<number2;j++)
//		{
//			point[1]=pointArray.at(j);
//			if(point[0]==point[1])
//				return (i+1)*MaxSize+(j+1);
//		}
//	}
//	return 0;
//}
int Point3DArrayEx::HasElement(double x,double y,double z)
{
	int i,number;
	Point3D point[2];

	point[0].m_x =x,point[0].m_y =y,point[0].m_z =z;
	number=size();
	for(i=0;i<number;i++)
	{
		point[1]=at(i);
		if(point[0]==point[1])
			return 1;
	}
	return 0;
}
int Point3DArrayEx::HasElement(double a[])
{
	return HasElement(a[0],a[1],a[2]);
}
int Point3DArrayEx::HasElement(Point3D point)
{
	return HasElement(point.m_x ,point.m_y ,point.m_z );
}
Point3DArrayEx Point3DArrayEx::ReverseOrder(void)
{
	int        i,num;
	Point3D    point;
	Point3DArrayEx pointArray;
	num=size();
	for(i=num;i>0;)
	{
		i--;
		point=at(i);
		pointArray.Add(point);
	}
	pointArray.m_attr1 =m_attr1;
	pointArray.m_attr2 =m_attr2;
	pointArray.m_strID =m_strID;
	pointArray.m_strSubID =m_strSubID;
	pointArray.m_ID =m_ID;
	return pointArray;
}

/////////////////////////////////////////////////////////
void Point3DArrayEx::Add(double x,double y,double z)
{
	if(size()==0)
	{
		m_minX=m_maxX=x ;
		m_minY=m_maxY=y;
		m_minZ=m_maxZ=z;
	}
	else
	{
		m_minX=(m_minX<x)?m_minX:x;
		m_minY=(m_minY<y)?m_minY:y;
		m_minZ=(m_minZ<z)?m_minZ:z;

		m_maxX=(m_maxX>x)?m_maxX:x;
		m_maxY=(m_maxY>y)?m_maxY:y;
		m_maxZ=(m_maxZ>z)?m_maxZ:z;
	}
	Point3D point(x,y,z);
	v.push_back(point);
}

void Point3DArrayEx::Add(double a[])
{
	if(size()==0)
	{
		m_minX=m_maxX=a[0];
		m_minY=m_maxY=a[1];
		m_minZ=m_maxZ=a[2];
	}
	else
	{
		m_minX=(m_minX<a[0])?m_minX:a[0];
		m_minY=(m_minY<a[1])?m_minY:a[1];
		m_minZ=(m_minZ<a[2])?m_minZ:a[2];

		m_maxX=(m_maxX>a[0])?m_maxX:a[0];
		m_maxY=(m_maxY>a[1])?m_maxY:a[1];
		m_maxZ=(m_maxZ>a[2])?m_maxZ:a[2];
	}
	Point3D point(a);
	v.push_back(point);
}
void Point3DArrayEx::Add(Point3D &point)
{
	if(size()==0)
	{
		m_minX=m_maxX=point.m_x ;
		m_minY=m_maxY=point.m_y;
		m_minZ=m_maxZ=point.m_z;
	}
	else
	{
		m_minX=(m_minX<point.m_x)?m_minX:point.m_x;
		m_minY=(m_minY<point.m_y)?m_minY:point.m_y;
		m_minZ=(m_minZ<point.m_z)?m_minZ:point.m_z;

		m_maxX=(m_maxX>point.m_x)?m_maxX:point.m_x;
		m_maxY=(m_maxY>point.m_y)?m_maxY:point.m_y;
		m_maxZ=(m_maxZ>point.m_z)?m_maxZ:point.m_z;
	}
	v.push_back(point);
}
void Point3DArrayEx::InsertAt(int index,Point3D& point)
{
	if(size()==0)
	{
		m_minX=m_maxX=point.m_x ;
		m_minY=m_maxY=point.m_y;
		m_minZ=m_maxZ=point.m_z;
	}
	else {
		m_minX = (m_minX < point.m_x) ? m_minX : point.m_x;
		m_minY = (m_minY < point.m_y) ? m_minY : point.m_y;
		m_minZ = (m_minZ < point.m_z) ? m_minZ : point.m_z;

		m_maxX = (m_maxX > point.m_x) ? m_maxX : point.m_x;
		m_maxY = (m_maxY > point.m_y) ? m_maxY : point.m_y;
		m_maxZ = (m_maxZ > point.m_z) ? m_maxZ : point.m_z;
	}

	v.insert(v.begin() + index,point);
}



Line2DArrayEx::Line2DArrayEx(Line2DArrayEx &lineArray)
{
	int index,maxIndex;
	maxIndex=lineArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
		Add(lineArray.at(index));
	m_ID=lineArray.m_ID ;
	m_strID=lineArray.m_strID ;
	m_strSubID=lineArray.m_strSubID ;
	m_attr1=lineArray.m_attr1 ;
	m_attr2=lineArray.m_attr2 ;
}

Line2DArrayEx Line2DArrayEx::operator=(Line2DArrayEx &lineArray)
{
	int index,maxIndex;
	maxIndex=lineArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
		Add(lineArray.at(index));
	m_ID=lineArray.m_ID ;
	m_strID=lineArray.m_strID ;
	m_strSubID=lineArray.m_strSubID ;
	m_attr1=lineArray.m_attr1 ;
	m_attr2=lineArray.m_attr2 ;
	return *this;
}
Line2DArrayEx Line2DArrayEx::operator+=(Line2DArrayEx &lineArray)
{
	int index,maxIndex;
	maxIndex=lineArray.size();
	for(index=0;index<maxIndex;index++)
		Add(lineArray.at(index));
	return *this;
}
Line2DArrayEx Line2DArrayEx::operator-=(Line2DArrayEx &lineArray)
{
	Line2D line[2];
	Line2DArrayEx lineArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	number1=size();
	for(index=0;index<number1;index++)
	{
		line[0]=at(index);
		lineArray1.Add(line[0]);
	}
	number2=lineArray.size();
	clear();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		line[0]=lineArray1.at(i);
		for(j=0;j<number2;j++)
		{
        	line[1]=lineArray.at(j);
			if(line[0]==line[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(line[0]);
		}
	}
	return *this;
}
Line2DArrayEx Line2DArrayEx::operator|=(Line2DArrayEx &lineArray)
{
	Line2D line[2];
	Line2DArrayEx lineArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	maxIndex=size();
	for(index=0;index<maxIndex;index++)
	{
		line[0]=at(index);
		lineArray1.Add(line[0]);
	}
	maxIndex=lineArray.size();
	for(index=0;index<maxIndex;index++)
	{
		line[0]=lineArray.at(index);
		lineArray1.Add(line[0]);
	}
	clear();
	number1=lineArray1.size();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		line[0]=lineArray1.at(i);
		number2=size();
		for(j=0;j<number2;j++)
		{
        	line[1]=at(j);
			if(line[0]==line[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(line[0]);
		}
	}
	return *this;
}

void Line2DArrayEx::Add(double x1,double y1,
					 	double x2,double y2)
{
	Line2D line(x1,y1, x2,y2);
	v.push_back(line);
}
void Line2DArrayEx::Add(double a[],double b[])
{
	Line2D line(a,b);
	v.push_back(line);
}
void Line2DArrayEx::Add(Line2D &line)
{
	v.push_back(line);
}
void Line2DArrayEx::SetUniDirection()
{
	int           i,j,number,num,nIndex,flag;
	double        a[2],b[2],c[2];
	Line2D        line,tempLine;
	Line2DArrayEx lineArray[3];
	number=size();
	for(i=0;i<number;i++)
	{
		line=at(i);
		lineArray[0].Add (line);
	}
	clear();
	nIndex=0;
	while(number>0)
	{
		lineArray[2].clear ();
		lineArray[(nIndex+1)%2].clear ();
		for(i=0;i<number;i++)
		{
			line=lineArray[nIndex].at (i);
			line.Get (a,b);
			num=size();
			if(num==0)
			{
				Add(line);
				lineArray[2].Add (line);
				continue;
			}

			tempLine=at(num-1);
			flag=tempLine.HasVertex(a);
			if(flag==2)                    //a点在线段的终端
			{
				Add(line);
				lineArray[2].Add (line);
				break;
			}
			flag=tempLine.HasVertex (b);
			if(flag==2)
			{
				lineArray[2].Add (line);
				line.Set (b,a);
				Add(line);
				break;
			}
			tempLine=at(0);
			flag=tempLine.HasVertex(b);
			if(flag==1)                    //a点在线段的终端
			{
				Add(line);
				lineArray[2].Add (line);
				break;
			}
			flag=tempLine.HasVertex (a);
			if(flag==1)
			{
				lineArray[2].Add (line);
				line.Set (b,a);
				Add(line);
				break;
			}
		}
		for(i=0;i<number;i++)
		{
			line=lineArray[nIndex].at (i);
			num=lineArray[2].size ();
			for(j=0;j<num;j++)
			{
				tempLine=lineArray[2].at (j);
				if(line==tempLine)
					break;
			}
			if(j==num && num>0)                  //未处理线段
				lineArray[(nIndex+1)%2].Add (line);
		}
		nIndex=(nIndex+1)%2;
		number=lineArray[nIndex].size ();
	}
}

//Arc2DArrayEx Line2DArrayEx::GetArcs()
//{
//	LinkedIndex *head=NULL,*rear;
//	LinkedIndex *preTemp,*pTemp;      //临时变量，指向索引链表中一个索引项的指针
//    LinkedIndex *pTemp1;
//	int       reverseFlag;            //反向标志
//	int       i,number1,number2,number;
//	int       connectNum;             //一条线段上连接的其它线段的数目
//	int       attachFlag;             //判断一线段是否连接一段曲线
//	int       closeFlag;              //曲线封闭标志
//	double    a[2],b[2],c[2],d[2];
//	double    temp;
//	Line2D    objLine;
//	Line2D    line[2];
//	Point2D   point[4];
//	Vector2D  vector;
//	Arc2D     arc,arc1;
//	Arc2DArrayEx  arcArray;
//
//	number=size();
//	for(i=0;i<number;i++)              //建立未加入到简单曲线的直线段索引表
//	{
//		if(i==0)
//		{
//			head=new LinkedIndex;
//			head->index=0;
//			head->pNext=NULL;
//			//////////////////////////
//			rear=head;
//			continue;
//		}
//		pTemp=new LinkedIndex;
//		pTemp->index=i;
//		pTemp->pNext=NULL;
//		//////////////////
//		rear->pNext=pTemp;
//		rear=pTemp;
//	}
//
//	while(head!=NULL)         //存在有直线段未加入到简单曲线中
//	{
//		objLine=at(head->index);
//		objLine.Get(b,a);
//		arc.clear();
//		arc.Add(a);           //初始化一简单曲线
//		arc.Add(b);
//
//		pTemp=head;           ////////////////////////////////////////
//		head=head->pNext;     ///删除已经处理的线段的索引项，指针后移
//		delete pTemp;         ////////////////////////////////////////
//
//		closeFlag=0;
//		reverseFlag=0;
//		while(1)                           //向简单曲线头部加线段
//		{
//			number=arc.size();
//			point[0]=arc.at(number-1);  //取曲线的尾端点
//
//			attachFlag=0;
//			number1=arcArray.size();
//			for(i=0;i<number1;i++)         //判断是否与已生成曲线相连
//			{
//				arc1=arcArray.at(i);
//				number2=arc1.size();
//				point[2]=arc1.at(0);
//				point[3]=arc1.at(number2-1);
//				if(point[0]==point[2] || point[0]==point[3])
//				{                          //
//					attachFlag=1;
//					break;
//				}
//			}
//			if(attachFlag==1)
//			{
//				reverseFlag++;
//				if(reverseFlag==2)
//				{
//					arcArray.Add(arc);
//					break;                       //while(1)
//				}
//				arc=arc.ReverseOrder();          //将arc反向
//				continue;
//			}
//            //////////////判断与曲线尾端相连的线段数目是否为1/////
//			connectNum=0;
//			pTemp1=NULL;
//			preTemp=NULL;
//			pTemp=head;
//			while(pTemp!=NULL)
//			{
//				///////////////取未加入到简单曲线中的直线段///////
//				objLine=at(pTemp->index);
//				if(objLine.HasVertex(point[0]))
//				{
//			    	connectNum+=1;
//					if(connectNum==1)
//						pTemp1=preTemp;
//					else
//					    break;
//				}
//				preTemp=pTemp;
//				pTemp=pTemp->pNext;
//			}
//		    if(connectNum==1)              //尾端只与一条直线段相连
//			{
//				if(pTemp1==NULL)
//				{
//					objLine=at(head->index);
//					pTemp=head;
//					head=head->pNext;
//					delete pTemp;
//				}
//				else
//				{
//					pTemp=pTemp1->pNext;
//				    objLine=at(pTemp->index);
//					pTemp1->pNext=pTemp->pNext;
//					delete pTemp;
//				}              	            //从直线段索引表中删除索引项
//				objLine.Get(c,d);
//				point[0].Get(a);
//
//				Point2D pt;
//				if(objLine.HasVertex(a)==1) //a和c相连
//				{
//					arc.Add(d);
//					pt.Set(d);
//				}
//				else
//				{
//					arc.Add(c);
//					pt.Set(c);
//				}
//				pt.Get(c);
//				point[0]=arc.at(0);      //取曲线的起点
//				point[0].Get(a);
//				temp=vector.SquareLength(c[0]-a[0],c[1]-a[1]);
//				if(sqrt(temp)<THRESHOLD10)  //判断曲线是否闭合
//				{
//					arcArray.Add(arc);
//					break;                  //while(1)
//				}
//			}                               //if(connectNum==1)
//			else                            //简单曲线已经达到一个真正的端点
//			{
//				reverseFlag++;
//				if(reverseFlag==2)
//				{
//					arcArray.Add(arc);
//					break;                  //while(1)
//				}
//				arc=arc.ReverseOrder();
//			}
//		}//while(1)
//	}    //while(head!=NULL)
//	return arcArray;
//}

Line3DArrayEx::Line3DArrayEx(Line3DArrayEx &lineArray)
{
	Line3D line;
	int index,maxIndex;
	maxIndex=lineArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		line=lineArray.at(index);
		Add(line);
	}
	m_ID=lineArray.m_ID ;
	m_strID=lineArray.m_strID ;
	m_strSubID=lineArray.m_strSubID ;
	m_attr1=lineArray.m_attr1 ;
	m_attr2=lineArray.m_attr2 ;
}


Line3DArrayEx Line3DArrayEx::operator=(Line3DArrayEx lineArray)
{
	Line3D line;
	int index,maxIndex;
	maxIndex=lineArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		line=lineArray.at(index);
		Add(line);
	}
	return *this;
}
Line3DArrayEx Line3DArrayEx::operator=(Line3DArrayEx &lineArray)
{
	Line3D line;
	int index,maxIndex;
	maxIndex=lineArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		line=lineArray.at(index);
		Add(line);
	}
	m_ID=lineArray.m_ID ;
	m_strID=lineArray.m_strID ;
	m_strSubID=lineArray.m_strSubID ;
	m_attr1=lineArray.m_attr1 ;
	m_attr2=lineArray.m_attr2 ;
	return *this;
}
Line3DArrayEx Line3DArrayEx::operator+=(Line3DArrayEx &lineArray)
{
	Line3D line;
	int index,maxIndex;

	maxIndex=lineArray.size();
	for(index=0;index<maxIndex;index++)
	{
		line=lineArray.at(index);
		Add(line);
	}
	return *this;
}
Line3DArrayEx Line3DArrayEx::operator-=(Line3DArrayEx &lineArray)
{
	Line3D line[2];
	Line3DArrayEx lineArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	number1=size();
	for(index=0;index<number1;index++)
	{
		line[0]=at(index);
		lineArray1.Add(line[0]);
	}
	number2=lineArray.size();
	clear();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		line[0]=lineArray1.at(i);
		for(j=0;j<number2;j++)
		{
        	line[1]=lineArray.at(j);
			if(line[0]==line[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(line[0]);
		}
	}
	return *this;
}
Line3DArrayEx Line3DArrayEx::operator|=(Line3DArrayEx &lineArray)
{
	Line3D line[2];
	Line3DArrayEx lineArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	maxIndex=size();
	for(index=0;index<maxIndex;index++)
	{
		line[0]=at(index);
		lineArray1.Add(line[0]);
	}
	maxIndex=lineArray.size();
	for(index=0;index<maxIndex;index++)
	{
		line[0]=lineArray.at(index);
		lineArray1.Add(line[0]);
	}
	clear();
	number1=lineArray1.size();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		line[0]=lineArray1.at(i);
		number2=size();
		for(j=0;j<number2;j++)
		{
        	line[1]=at(j);
			if(line[0]==line[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(line[0]);
		}
	}
	return *this;
}

void Line3DArrayEx::Add(double x1,double y1,double z1,
					 	double x2,double y2,double z2)
{
	//这里估计是要搞一个移动构造函数？
	Line3D line(x1,y1,z1,
		           x2,y2,z2);
	v.push_back(line);
}
void Line3DArrayEx::Add(double a[],double b[])
{
	Line3D line(a,b);
	v.push_back(line);
}
void Line3DArrayEx::Add(Line3D &line)
{
	v.push_back(line);
}

void Line3DArrayEx::SetUniDirection()
{
	int           i,j,number,num,nIndex,flag;
	double        a[3],b[3],c[3];
	Line3D        line,tempLine;
	Line3DArrayEx     lineArray[3];
	number=size();
	for(i=0;i<number;i++)
	{
		line=at(i);
		lineArray[0].Add (line);
	}
	clear();
	nIndex=0;
	while(number>0)
	{
		lineArray[2].clear ();
		lineArray[(nIndex+1)%2].clear ();
		for(i=0;i<number;i++)
		{
			line=lineArray[nIndex].at (i);
			line.Get (a,b);
			num=size();
			if(num==0)
			{
				Add(line);
				lineArray[2].Add (line);
				continue;
			}

			tempLine=at(num-1);
			flag=tempLine.HasVertex(a);
			if(flag==2)                    //a点在线段的终端
			{
				Add(line);
				lineArray[2].Add (line);
				break;
			}
			flag=tempLine.HasVertex (b);
			if(flag==2)
			{
				lineArray[2].Add (line);
				line.Set (b,a);
				Add(line);
				break;
			}
			tempLine=at(0);
			flag=tempLine.HasVertex(b);
			if(flag==1)                    //a点在线段的终端
			{
				Add(line);
				lineArray[2].Add (line);
				break;
			}
			flag=tempLine.HasVertex (a);
			if(flag==1)
			{
				lineArray[2].Add (line);
				line.Set (b,a);
				Add(line);
				break;
			}
		}
		for(i=0;i<number;i++)
		{
			line=lineArray[nIndex].at (i);
			num=lineArray[2].size ();
			for(j=0;j<num;j++)
			{
				tempLine=lineArray[2].at (j);
				if(line==tempLine)
					break;
			}
			if(j==num && num>0)      //未处理线段
				lineArray[(nIndex+1)%2].Add (line);
		}
		nIndex=(nIndex+1)%2;
		number=lineArray[nIndex].size ();
	}

}

Triangle3DArrayEx::Triangle3DArrayEx(Triangle3DArrayEx &triangleArray)
{
	Triangle3D triangle;
	int index,maxIndex;
	maxIndex=triangleArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		triangle=triangleArray.at(index);
		Add(triangle);
	}
}
Triangle3DArrayEx::Triangle3DArrayEx(Triangle3DArrayEx &&triangleArray)
{
	Triangle3D triangle;
	int index,maxIndex;
	maxIndex=triangleArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		triangle=triangleArray.at(index);
		Add(triangle);
	}
	m_ID=triangleArray.m_ID ;
	m_attr1=triangleArray.m_attr1 ;
	m_attr2=triangleArray.m_attr2 ;
	m_strID=triangleArray.m_strID;
	m_strSubID=triangleArray.m_strSubID;

}

Triangle3DArrayEx Triangle3DArrayEx::operator=(Triangle3DArrayEx &triangleArray)

{
	int maxIndex,index;
	Triangle3D triangle;
	maxIndex=triangleArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		triangle=triangleArray.at(index);
		Add(triangle);
	}
	m_ID=triangleArray.m_ID ;
	m_strID=triangleArray.m_strID;
	m_strSubID=triangleArray.m_strSubID;
	m_attr1=triangleArray.m_attr1 ;
	m_attr2=triangleArray.m_attr2 ;
	return *this;
}
Triangle3DArrayEx Triangle3DArrayEx::operator=(Triangle3DArrayEx triangleArray)

{
	int maxIndex,index;
	Triangle3D triangle;
	maxIndex=triangleArray.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		triangle=triangleArray.at(index);
		Add(triangle);
	}
	m_ID=triangleArray.m_ID ;
	m_strID=triangleArray.m_strID;
	m_strSubID=triangleArray.m_strSubID;
	m_attr1=triangleArray.m_attr1 ;
	m_attr2=triangleArray.m_attr2 ;
	return *this;
}
Triangle3DArrayEx Triangle3DArrayEx::operator+=(Triangle3DArrayEx &triangleArray)

{
	int maxIndex,index;
	Triangle3D triangle;

	maxIndex=triangleArray.size();
	for(index=0;index<maxIndex;index++)
	{
		triangle=triangleArray.at(index);
		Add(triangle);
	}
	return *this;
}
Triangle3DArrayEx Triangle3DArrayEx::operator-=(Triangle3DArrayEx &triangleArray)
{
	Triangle3D triangle[2];
	Triangle3DArrayEx triangleArray1;
	int i,j,reFlag;
	int index,maxIndex;
	int number1,number2;

	number1=size();
	for(index=0;index<number1;index++)
	{
		triangle[0]=at(index);
		triangleArray1.Add(triangle[0]);
	}
	number2=triangleArray.size();
	clear();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		triangle[0]=triangleArray1.at(i);
		for(j=0;j<number2;j++)
		{
        	triangle[1]=triangleArray.at(j);
			if(triangle[0]==triangle[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(triangle[0]);
		}
	}
	return *this;
}
Triangle3DArrayEx Triangle3DArrayEx::operator|=(Triangle3DArrayEx &triangleArray)
{
	int maxIndex,index;
	int number1,number2;
	int i,j,reFlag;
	Triangle3D triangle[2];
	Triangle3DArrayEx triangleArray1;

	maxIndex=size();
	for(index=0;index<maxIndex;index++)
	{
		triangle[0]=at(index);
		triangleArray1.Add(triangle[0]);
	}
	maxIndex=triangleArray.size();
	for(index=0;index<maxIndex;index++)
	{
		triangle[0]=triangleArray.at(index);
		triangleArray1.Add(triangle[0]);
	}
	clear();
	number1=triangleArray1.size();
	for(i=0;i<number1;i++)
	{
		reFlag=0;
		triangle[0]=triangleArray1.at(i);
		number2=size();
		for(j=0;j<number2;j++)
		{
        	triangle[1]=at(j);
			if(triangle[0]==triangle[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			Add(triangle[0]);
		}
	}
	return *this;
}
void Triangle3DArrayEx::Add(double x1,double y1,double z1,
							double x2,double y2,double z2,
							double x3,double y3,double z3)
{
	Triangle3D triangle(x1,y1,z1,
		                x2,y2,z2,
				        x3,y3,z3);
	v.push_back(triangle);
}
void Triangle3DArrayEx::Add(double a[],double b[],double c[])
{
	Triangle3D triangle(a,b,c);
	v.push_back(triangle);
}
void Triangle3DArrayEx::Add(Triangle3D &triangle)
{
	v.push_back(triangle);
}
void Triangle3DArrayEx::SetUpDirection()
{
	int           i,num;
	double        a[3]={0,0,1},b[3],c[3];
	Triangle3D    triangle,tempTriangle;
	num=size();
	for(i=0;i<num;i++)
	{
		a[0]=a[1]=0.0,a[2]=1;
		triangle=at(i);
		triangle.GetNormal (b);
		Vector3D vector;
		if(vector.DotProduct(a,b)<0)
		{
			triangle.Get (a,b,c);
			triangle.Set(a,c,b);
			v[i] = triangle;
		}
	}
}
//bool Triangle3DArrayEx::SetUniDirection()
//{
//	POSITION        pos,start;
//	IntList         indices;
//	Triangle3D      triangle[2];
//	Triangle3DArray triangles;
//
//	int number=size();
//	if(number<1)
//		return true;
//	for(int i=1;i<number;i++)
//	{
//		indices.push_back(i);
//	}
//	triangle[0]=at(0);
//	triangles.Add(triangle[0]);
//	number--;
//	if(number<1)
//		return true;
//	pos=indices.begin ();
//
//	while(number)
//	{
//		start=pos;
//		int flag=0;
//		int num=triangles.size();
//
//		do{
//			triangle[0]=at(indices.at(pos));
//			double a[3],b[3],c[3];
//			triangle[0].Get (a,b,c);
//			for(int i=0;i<num;i++)
//			{
//				triangle[1]=triangles.at(i);
//				flag=triangle[1].HasEdge (a,b);
//				if(flag<0)
//				{
//					triangles.Add(triangle[0]);
//					break;
//				}
//				else if(flag>0)
//				{
//					triangle[0].Set (c,b,a);
//					triangles.Add(triangle[0]);
//					break;
//				}
//				flag=triangle[1].HasEdge(b,c);
//				if(flag<0)
//				{
//					triangles.Add(triangle[0]);
//					break;
//				}
//				else if(flag>0)
//				{
//					triangle[0].Set (c,b,a);
//					triangles.Add(triangle[0]);
//					break;
//				}
//				flag=triangle[1].HasEdge (c,a);
//				if(flag<0)
//				{
//					triangles.Add(triangle[0]);
//					break;
//				}
//				else if(flag>0)
//				{
//					triangle[0].Set (c,b,a);
//					triangles.Add(triangle[0]);
//					break;
//				}
//			}
//			if(flag)
//				break;
//			else
//			{
//				indices.GetNext(pos);
//				if(pos==NULL)
//					pos=indices.begin ();
//			}
//		}while(pos!=start);
//		if(flag)
//		{
//			POSITION pos1=pos;
//			indices.GetNext(pos1);
//			indices.RemoveAt (pos);
//			number--;
//			if(number<1)
//				break;
//			pos=pos1;
//			if(pos==NULL)
//				pos=indices.begin ();
//		}
//		else                         //三角形不能构成曲面
//			return false;
//	}
//
//	clear();
//	number = triangles.size();
//	for(int i=0;i<number;i++)
//	{
//		Triangle3D t=triangles.at (i);
//		Add(t);
//	}
//
//	/*
//
//	int               i,j,number,num,nIndex,flag;
//	double            a[3],b[3],c[3];
//	Triangle3D        triangle,tempTriangle;
//	Triangle3DArrayEx triangleArray[3];
//
//	number=size();
//	for(i=0;i<number;i++)
//	{
//		triangle=at(i);
//		triangleArray[0].Add (triangle);
//	}
//	clear();
//	nIndex=0;
//	while(number>0)
//	{
//		triangleArray[2].clear ();
//
//		for(i=0;i<number;i++)
//		{
//			triangle=triangleArray[nIndex].at (i);
//			triangle.Get (a,b,c);
//			num=size();
//			if(num==0)
//			{
//				Add(triangle);
//				triangleArray[2].Add (triangle);
//				continue;
//			}
//			for(j=num-1;j>=0;j--)
//			{
//				tempTriangle=at(j);
//				flag=tempTriangle.HasEdge (a,b);
//				if(flag<0)
//				{
//					Add(triangle);
//					triangleArray[2].Add (triangle);
//					break;
//				}
//				else if(flag>0)
//				{
//					triangleArray[2].Add (triangle);
//					triangle.Set (c,b,a);
//					Add(triangle);
//					break;
//				}
//				flag=tempTriangle.HasEdge (b,c);
//				if(flag<0)
//				{
//					triangleArray[2].Add (triangle);
//					Add(triangle);
//					break;
//				}
//				else if(flag>0)
//				{
//					triangleArray[2].Add (triangle);
//					triangle.Set (c,b,a);
//					Add(triangle);
//					break;
//				}
//				flag=tempTriangle.HasEdge (c,a);
//				if(flag<0)
//				{
//					triangleArray[2].Add (triangle);
//					Add(triangle);
//					break;
//				}
//				else if(flag>0)
//				{
//					triangleArray[2].Add (triangle);
//					triangle.Set (c,b,a);
//					Add(triangle);
//					break;
//				}
//			}
//		}
//
//		triangleArray[(nIndex+1)%2].clear ();
//		for(i=0;i<number;i++)
//		{
//			triangle=triangleArray[nIndex].at (i);
//			num=triangleArray[2].size ();
//			for(j=0;j<num;j++)
//			{
//				tempTriangle=triangleArray[2].at (j);
//				if(triangle==tempTriangle)
//					break;
//			}
//			if(j==num && num>0)
//				triangleArray[(nIndex+1)%2].Add (triangle);
//		}
//		nIndex=(nIndex+1)%2;
//		number=triangleArray[nIndex].size ();
//	}*/
//}
void Triangle3DArrayEx::ReverseDirection()
{
	int           i,number;
	double        a[3],b[3],c[3];
	Triangle3D    triangle;
	number=size();
	for(i=0;i<number;i++)
	{
		triangle=at(i);
		triangle.Get (a,b,c);
		triangle.Set(c,b,a);
		v[i] = triangle;
	}
}
Line3DArrayEx Triangle3DArrayEx::GetBoundaryLines(void)
{
	int i,j,number;
	int reFlag;
	double a[3],b[3],c[3];
	Line3D line[3];
	Triangle3D triangle;
	Line3DArrayEx lineArray,lineArray1;

	number=size();
	for(i=0;i<number;i++)
	{
		triangle=at(i);
		triangle.Get(a,b,c);
		line[0].Set(a,b);
		line[1].Set(b,c);
		line[2].Set(c,a);
		for(j=0;j<3;j++)
		{
			lineArray.Add(line[j]);
		}
	}

	number=lineArray.size();
	if(number<1)
	{
		return lineArray;
	}
	int* flag=new int[number];
	for(int i=0;i<number;i++)
	{
		flag[i]=0;
	}
	for(i=0;i<number;i++)
	{
		if(flag[i]) continue;
		line[0]=lineArray.at(i);
		for(j=i+1;j<number;j++)
		{
			if(flag[j]) continue;
        	line[1]=lineArray.at(j);
			if(line[0]==line[1])
			{
				flag[i]++;
				flag[j]++;
				break;
			}
		}
	}
	/*lineArray1.clear();
	for(i=0;i<number;i++)
	{
		reFlag=0;
		line[0]=lineArray.at(i);
		for(j=0;j<number;j++)
		{
			if(i==j)
				continue;
        	line[1]=lineArray.at(j);
			if(line[0]==line[1])
			{
				reFlag=1;
				break;
			}
		}
		if(reFlag==0)
		{
			lineArray1.Add(line[0]);
		}
	}	*/

	for(i=0;i<number;i++)
	{
		if(!flag[i])
		{
			lineArray1.Add (lineArray.at (i));
		}
	}
	delete[] flag;

	return lineArray1;
}
void Triangle3DArrayEx ::GetSimpleNormals(Point3DArrayEx &normalArray)
{
	Triangle3D    triangle;
	Point3D       point[3],tmppoint[3];
	Vector3D      vector;
	double        a[3][3]={{0,0,0},{0,0,0},{0,0,0}},n[3];
	int           i,j,k,num;

	normalArray.clear ();
	num=size();
	for(i=0;i<num;i++)
	{
		triangle=at(i);
		triangle.GetNormal (n);
		Vector3D v(n);
		normalArray.Add (v);
	}
}
Triangle3DArrayEx Triangle3DArrayEx::GetAverageNormals()
{
	Triangle3DArrayEx triangleArray;
	Triangle3D      objTriangle;
	Point3D         objPoint[3],tmpPoint[3];
	Vector3D        objVector;
	double          a[3][3]={{0,0,0},{0,0,0},{0,0,0}},n[3];
	int             i,j,k,num;

	num=size();
	for(i=0;i<num;i++)
	{
		objTriangle=at(i);
		objVector.Set (0,0,0);
		objTriangle.Get (objPoint[0],objPoint[1],objPoint[2]);
		for(j=0;j<3;j++)
		{
			for(k=0;k<num;k++)
			{
				objTriangle=at(k);
				if(objTriangle.HasVertex (objPoint[j]))
				{
					objTriangle.GetNormal (n);
					objVector+=n;
				}
			}
			if(objVector.Normalize ())
				objVector.Get (a[j]);
			else
				a[j][0]=a[j][1]=a[j][2]=0.0;
		}
		objTriangle.Set(a[0],a[1],a[2]);
		triangleArray.Add(objTriangle);
	}
	return triangleArray;
}
Triangle3DArrayEx Triangle3DArrayEx ::GetSimpleNormals()
{
	Triangle3DArrayEx triangleArray;
	Triangle3D      objTriangle;
	Point3D         objPoint[3],tmpPoint[3];
	Vector3D        objVector;
	double          a[3][3]={{0,0,0},{0,0,0},{0,0,0}},n[3];
	int             i,j,k,num;

	num=size();
	for(i=0;i<num;i++)
	{
		objTriangle=at(i);
		objTriangle.GetNormal (n);
		objTriangle.Set(n,n,n);
		triangleArray.Add(objTriangle);
	}
	return triangleArray;
}
//int Triangle3DArrayEx::IsBody()
//{
//	int num;
//	Line3DArrayEx lineArray;
//	num=size();
//	if(num==0) return 0;
//	lineArray=GetBoundaryLines();
//	num=lineArray.size ();
//	if(num)
//		return 0;
//	return 1;
//}

Polygon2D::Polygon2D(Point2DArrayEx &pointArray)
{
	int i,number;
	Point2D point[2];
	clear();
	if(pointArray.GetNormal (m_n))
	{
		number=pointArray.size();
		point[0]=pointArray.at(0);
		point[1]=pointArray.at(number-1);
		if(point[0]==point[1])//若起点与终点重合，去掉重复点
			number--;

		if(number>2)
			for(i=0;i<number;i++)
			{
				point[0]=pointArray.at(i);
				Add(point[0]);
			}
		m_ID=pointArray.m_ID ;
		m_strID=pointArray.m_strID ;
		m_strSubID=pointArray.m_strSubID ;
		m_attr1=pointArray.m_attr1 ;
		m_attr2=pointArray.m_attr2 ;
	}
}
Polygon2D::Polygon2D(Polygon2D &polygon)
{
	Point2D point;
	int index,maxIndex;
	maxIndex=polygon.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		point=polygon.at(index);
		Add(point);
	}
	m_ID=polygon.m_ID ;
	m_strID=polygon.m_strID ;
	m_strSubID=polygon.m_strSubID ;
	m_attr1=polygon.m_attr1 ;
	m_attr2=polygon.m_attr2 ;
}
Polygon2D Polygon2D::operator=(Polygon2D &polygon)
{
	Point2D point;
	int index,maxIndex;
	maxIndex=polygon.size();
	for(index=0;index<maxIndex;index++)
	{
		point=polygon.at(index);
		Add(point);
	}
	m_ID=polygon.m_ID ;
	m_strID=polygon.m_strID ;
	m_strSubID=polygon.m_strSubID ;
	m_attr1=polygon.m_attr1 ;
	m_attr2=polygon.m_attr2 ;
	return *this;
}
//int Polygon2D::operator==(Polygon2D &polygon)
//{
//	int flag;
//	int i,j;
//	int number1,number2;
//	Point2D point[2];
//	Line2D line[2];
//	Line2DArrayEx lines1,lines2;
//
//	lines1 = GetLines();
//	lines2 = polygon.GetLines();
//	number1=lines1.size();
//	number2=lines2.size();
//	if(number1!=number2)
//		return 0;
//	for(i=0;i<number1;i++)                  //number1==number2
//	{
//		flag=0;
//		line[0]=lines1.at(i);
//		for(j=0;j<number2;j++)
//		{
//			line[1]=lines2.at(j);
//			if(line[0]==line[1])
//			{
//				flag=1;
//				break;
//			}
//		}
//		if(flag==0)                         //lines1中一边不在lines2中
//			return 0;
//	}
//	return 1;
//}
int Polygon2D::HasVertex(double x,double y)
{
	int     i,index,number;
	double  temp;
	Point2D point;

	index=0;
	number=size();
	for(i=0;i<number;i++)
	{
		point=at(i);
		temp=0.0;
		temp+=(x-point.m_x)*(x-point.m_x);
		temp+=(y-point.m_y)*(y-point.m_y);
		temp=sqrt(temp);
		if(temp<THRESHOLD10)
		{
			index=i+1;
			break;
		}
	}
	return index;
}
int Polygon2D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1]);
}
int Polygon2D::HasVertex(Point2D point)
{
	return HasVertex(point.m_x,point.m_y);
}

bool Polygon2D::IsConvexPolygon(void)
{
	int i,number;
	double a[3],b[3],c[3];
	Vector2D vector;
	Point2D point[3];
	number=size();                  //多边形起点与终点不重复存储
	if(number<3)
		return false;
	for(i=0;i<number-1;i++)
	{
		point[0]=at(i);
		point[1]=at(i+1);
		point[2]=at((i+2)%number);

    	point[0].Get(a);
	    point[1].Get(b);
	    point[2].Get(c);
	    double temp=vector.CrossProduct(b[0]-a[0],b[1]-a[1],
		                   c[0]-a[0],c[1]-a[1]);
		if(temp*m_n[0]<-THRESHOLD10)
			return false;
	}
	return true;
}
//bool Polygon2D::IsDelaunayPolygon(void)
//{
//	int i,number;
//	Point2D point[3];
//	Circle2D circle;
//
//	if(!IsConvexPolygon())
//		return false;
//	number=size();                  //number>2
//	for(i=0;i<3;i++)
//		point[i]=at(i);
//	circle.Set(point[0],point[1],point[2]);
//	if(circle.m_radius<THRESHOLD10)
//		return false;
//	for(i=3;i<number;i++)
//	{
//		point[0]=at(i);
//		if(circle.Contain(point[0])!=1)
//			return false;
//	}
//	return true;
//}
bool  Polygon2D::IsCounterclockwise(double n[1])
{
	if(abs(n[0])<THRESHOLD5)
		return false;
	double p[1];
	GetNormal (p);
	if(p[0]*n[0]>THRESHOLD10)
		return true;
	return false;
}
//Triangle2DArrayEx Polygon2D::GetInnerTriangles()
//{
//	int           i,number;
//	double        a[3],b[3],c[3];
//	Point2D       point[3];
//	Triangle2DArrayEx triangleArray;
//
//	number=size();
//	if(number<3)
//		return triangleArray;
//	point[0]=at(0);
//	point[1]=at(number-1);
//	if(point[0]==point[1])                   //闭合点被重复存储时,不重复计算该点
//		number--;
//	point[0].Get(a);
//	for(i=1;i<number-1;i++)                  //忽略最前和最后一条边
//	{
//		point[1]=at(i);
//		point[2]=at(i+1);
//	    point[1].Get(b);
//		point[2].Get(c);
//		triangleArray.Add(a,b,c);
//	}
//	return triangleArray;
//}
Line2DArrayEx Polygon2D::GetLines(void)
{
	int i,number;
	Point2D point1,point2;
	Line2D line;
	Line2DArrayEx lineArray;

	number=size();                  //多边形始点与终点不重复存储
    if(number<2)
		return lineArray;
	point1=at(0);
	point2=at(number-1);
	if(point1==point2)
		number--;
	for(i=0;i<number;i++)
	{
		point1=at(i);
		point2=at((i+1)%number);
		line.Set(point1,point2);
		lineArray.Add(line);
	}
	return lineArray;
}
//int Polygon2D::Contain(Point2D point)
//{
//	int      i,j,number;
//	int      flag[2],Count;
//	int      FirstValid,LastValid;
//	int      PreValid,SucValid;
//	double   v[2];	                //射线方向矢量
//	double   temp;
//	double   a[2],b[2],c[2];
//	Vector2D vector;
//	Point2D  temppoint[2];
//	Line2D   line;
//
//	v[0]=1,v[1]=0;
//	number=size();
//	if(number<3) return 0;           //不能构成多边形
//	for(i=0;i<number;i++)
//	{
//		temppoint[0]=at(i);
//		temppoint[1]=at((i+1)%number);
//		line.Set(temppoint[0],temppoint[1]);
//		if(line.Contain (point))     //点在多边形的边上
//			return 1;
//	}
//	flag[0]=flag[1]=0;
//	FirstValid=-1;                   //多边形第一个不在射线上的顶点的序号
//	LastValid=-1;                    //多边形最后一个不在射线上的顶点的序号
//	for(i=0;i<number;i++)
//	{
//		temppoint[0]=at(i);
//		a[0]=temppoint[0].m_x-point.m_x;
//		a[1]=temppoint[0].m_y-point.m_y;
//		vector.Normalize (a);
//		temp=vector.CrossProduct(v,a);
//		if(abs(temp)>THRESHOLD10)     //多边形顶点不在射线v上
//		{
//			FirstValid=i;             //第一个不在射线上的多边形顶点的序号
//			if(temp>THRESHOLD10)      //所取点在v的逆时针方向
//				flag[0]=1;
//			else
//				flag[0]=-1;
//			break;
//		}
//	}
//	if(FirstValid==-1)                //各顶点都在射线上
//		return 0;
//	Count=0;
//	PreValid=FirstValid;
//	SucValid=FirstValid;
//
//	temppoint[0] = v[FirstValid];
//	for(i=FirstValid+1;i<number+FirstValid+1;i++)
//	{
//		temppoint[1]=at(i%number);
//		a[0]=temppoint[1].m_x-point.m_x;
//		a[1]=temppoint[1].m_y-point.m_y;
//		vector.Normalize (a);
//		temp=vector.CrossProduct(v,a);
//		if(abs(temp)>THRESHOLD10)       //多边形顶点不在射线v上
//		{
//			SucValid=i;
//			if(temp>THRESHOLD10)        //所取点在v的逆时针方向
//				flag[1]=1;
//			else
//				flag[1]=-1;
//			if(flag[0]*flag[1]==-1)     //射线穿越多边形
//			{
//				//if(SucValid-PreValid>1) //不在射线上的两个顶点之间至少有一顶点在射线上
//				//	Count++;
//				//else                    //多边形的边穿过射线所在直线
//				{
//					temppoint[0].Get (a);
//					temppoint[1].Get(b);
//					point.Get (c);
//					for(j=0;j<2;j++)
//					{
//						a[j]-=c[j];
//						b[j]-=c[j];
//					}
//					temp=vector.CrossProduct (a,b);
//					temp*=vector.CrossProduct (a,v);
//					if(temp>THRESHOLD10)//多边形的边穿过射线
//						Count++;
//				}
//			}
//			PreValid=SucValid;
//			flag[0]=flag[1];
//			temppoint[0]=temppoint[1];
//		}
//	}
//	if(Count%2)                         //多边形奇数次穿过射线，点在多边形内部　
//		return 2;
//	else
//		return 0;
//}
double Polygon2D::GetArea()
{
	double area=0.0;
	double normal[3];
	if(abs(m_n[0])<THRESHOLD10)         //多边形不存在或无效
		return 0.0;
	int num=this->size();
	for(int i=0;i<num;i++)
	{
		double v1[2],v2[2],v[2];
		Point2D point1,point2;
		point1=this->at (i);
		point2=this->at ((i+1)%num);
		point1.Get (v1);
		point2.Get (v2);
		Vector2D vector;
		area+=vector.CrossProduct (v1,v2);
	}
	area/=2.0;
	if(area<0.0)
		return -area;
	return area;
}
//构造函数
Polygon3D::Polygon3D(Point3DArrayEx& pointArray)
{
	int i,number;
	double n[3];
	Point3D point[2];

	v.clear();
	if(pointArray.GetNormal(m_n))
	{
		number=pointArray.size();
		point[0]=pointArray.at(0);
		point[1]=pointArray.at(number-1);
		if(point[0]==point[1])//若起点与终点重合，去掉重复点
			number--;

		if(number>2)
			for(i=0;i<number;i++)
			{
				point[0]=pointArray.at(i);
				Add(point[0]);
			}

		m_ID=pointArray.m_ID ;
		m_strID=pointArray.m_strID ;
		m_strSubID=pointArray.m_strSubID ;
		m_attr1=pointArray.m_attr1 ;
		m_attr2=pointArray.m_attr2 ;
	}
}

//将亡值
//Polygon3D::Polygon3D(Polygon3D &&polygon)
//{
//	Point3D point;
//	int index,maxIndex;
//
//	maxIndex=polygon.size();
//	clear();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=polygon.at(index);
//		Add(point);
//	}
//	for(int i=0;i<3;i++)
//	    m_n[i]=polygon.m_n[i];
//	m_ID=polygon.m_ID ;
//	m_strID=polygon.m_strID ;
//	m_strSubID=polygon.m_strSubID ;
//	m_attr1=polygon.m_attr1 ;
//	m_attr2=polygon.m_attr2 ;
//}
//Polygon3D::Polygon3D(Polygon3D &polygon)
//{
//	Point3D point;
//	int index,maxIndex;
//
//	maxIndex=polygon.size();
//	clear();
//	for(index=0;index<maxIndex;index++)
//	{
//		point=polygon.at(index);
//		Add(point);
//	}
//	for(int i=0;i<3;i++)
//		m_n[i]=polygon.m_n[i];
//	m_ID=polygon.m_ID ;
//	m_strID=polygon.m_strID ;
//	m_strSubID=polygon.m_strSubID ;
//	m_attr1=polygon.m_attr1 ;
//	m_attr2=polygon.m_attr2 ;
//}
Polygon3D Polygon3D::operator=(Polygon3D &polygon)
{
	Point3D point;
	int index,maxIndex;
	maxIndex=polygon.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		point=polygon.at(index);
		Add(point);
	}
	for(int i=0;i<3;i++)
	    m_n[i]=polygon.m_n[i];
	m_ID=polygon.m_ID ;
	m_strID=polygon.m_strID ;
	m_strSubID=polygon.m_strSubID ;
	m_attr1=polygon.m_attr1 ;
	m_attr2=polygon.m_attr2 ;
	return *this;
}

Polygon3D Polygon3D::operator=(Polygon3D polygon)
{
	Point3D point;
	int index,maxIndex;
	maxIndex=polygon.size();
	clear();
	for(index=0;index<maxIndex;index++)
	{
		point=polygon.at(index);
		Add(point);
	}
	for(int i=0;i<3;i++)
		m_n[i]=polygon.m_n[i];
	m_ID=polygon.m_ID ;
	m_strID=polygon.m_strID ;
	m_strSubID=polygon.m_strSubID ;
	m_attr1=polygon.m_attr1 ;
	m_attr2=polygon.m_attr2 ;
	return *this;
}
//int Polygon3D::operator==(Polygon3D &polygon)
//{
//	int flag;
//	int i,j;
//	int number1,number2;
//	double p[3],q[3];
//	Vector3D vector;
//	Point3D point[2];
//	Line3D line[2];
//	Line3DArrayEx lines1,lines2;
//
//	lines1 = GetLines();
//	lines2 = polygon.GetLines();
//	number1=lines1.size();
//	number2=lines2.size();
//	if(number1!=number2)
//		return 0;
//	for(i=0;i<number1;i++)                  //number1==number2
//	{
//		flag=0;
//		line[0]=lines1.at(i);
//		for(j=0;j<number2;j++)
//		{
//			line[1]=lines2.at(j);
//			if(line[0]==line[1])
//			{
//				flag=1;
//				break;
//			}
//		}
//		if(flag==0)                         //lines1中一边不在lines2中
//			return 0;
//	}
//	if(vector.DotProduct(m_n,polygon.m_n)>0)
//    	return 1;
//	else
//		return -1;
//}
int Polygon3D::HasVertex(double x,double y,double z)
{
	int     i,index,number;
	double  temp;
	Point3D point;

	index=0;
	number=size();
	for(i=0;i<number;i++)
	{
		point=at(i);
		temp=0.0;
		temp+=(x-point.m_x)*(x-point.m_x);
		temp+=(y-point.m_y)*(y-point.m_y);
		temp+=(z-point.m_z)*(z-point.m_z);
		temp=sqrt(temp);
		if(temp<THRESHOLD10)
		{
			index=i+1;
			break;
		}
	}
	return index;
}
int Polygon3D::HasVertex(double a[])
{
	return HasVertex(a[0],a[1],a[2]);
}
int Polygon3D::HasVertex(Point3D point)
{
	return HasVertex(point.m_x,point.m_y,point.m_z);
}

bool Polygon3D::IsConvexPolygon(void)
{
	int i,number;
	double a[3],b[3],c[3];
	Vector3D vector;
	Point3D point[3];
	number=size();                  //多边形起点与终点不重复存储
	if(number<3)
		return false;
	for(i=0;i<number-1;i++)
	{
		point[0]=at(i);
		point[1]=at(i+1);
		point[2]=at((i+2)%number);

    	point[0].Get(a);
	    point[1].Get(b);
	    point[2].Get(c);
	    vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
		                   c[0]-a[0],c[1]-a[1],c[2]-a[2]);
	    vector.Normalize();
		vector.Get (a);
		if(vector.DotProduct(a,m_n)<-THRESHOLD10)
			return false;
	}
	return true;
}
//bool Polygon3D::IsDelaunayPolygon(void)
//{
//	int i,number;
//	Point3D point[3];
//	Circle3D circle;
//
//	if(!IsConvexPolygon())
//		return false;
//	number=size();                  //number>2
//	for(i=0;i<3;i++)
//		point[i]=at(i);
//	circle.Set(point[0],point[1],point[2]);
//	if(circle.m_radius <THRESHOLD10)
//		return false;
//	for(i=3;i<number;i++)
//	{
//		point[0]=at(i);
//		if(circle.Contain(point[0])!=1)
//			return false;
//	}
//	return true;
//}
bool Polygon3D::IsCounterclockwise(double n[])
{
	double p[3];
	double temp;
	Vector3D vector;

	if(vector.Normalize (n))
	{
		GetNormal (m_n);
		temp=vector.DotProduct (n,m_n);
		if(temp>THRESHOLD10)
			return true;
	}
	return false;
}
Triangle3DArrayEx Polygon3D::GetInnerTriangles()
{
	int           i,number;
	double        a[3],b[3],c[3];
	Point3D       point[3];
	Triangle3DArrayEx triangleArray;

	number=size();
	if(number<3)
		return triangleArray;
	point[0]=at(0);
	point[1]=at(number-1);
	if(point[0]==point[1])                   //闭合点被重复存储时,不重复计算该点
		number--;
	point[0].Get(a);
	for(i=1;i<number-1;i++)                  //忽略最前和最后一条边
	{
		point[1]=at(i);
		point[2]=at(i+1);
	    point[1].Get(b);
		point[2].Get(c);
		triangleArray.Add(a,b,c);
	}
	return triangleArray;
}
Line3DArrayEx& Polygon3D::GetLines(void)
{
	int i,number;
	Point3D point1,point2;
	Line3D line;
	Line3DArrayEx lineArray;

	number=size();                         //多边形始点与终点不重复存储
    if(number<2)
		return lineArray;
	point1=at(0);
	point2=at(number-1);
	if(point1==point2)
		number--;
	for(i=0;i<number;i++)
	{
		point1=at(i);
		point2=at((i+1)%number);
		line.Set(point1,point2);
		lineArray.Add(line);
	}
	return lineArray;
}
int Polygon3D::Contain(Point3D point,double n[])
{
	int      i,j,number;
	int      flag[2],Count;
	int      FirstValid,LastValid;
	int      PreValid,SucValid;
	double   n1[3],n2[3];
	double   temp;
	double   a[3],b[3],c[3];
	Vector3D vector;
	Point3D  temppoint[2];
	Line3D   line;


	vector.Set (n);
	if(!vector.Normalize ())            //不可单位化
		return 0;
	else
		vector.Get (n2);                //取向量n的单位化向量
	temp=sqrt(n2[0]*n2[0]+n2[1]*n2[1]);
	if(temp>THRESHOLD10)                //n的水平分量不为零
	{
		n1[0]=-n2[1]/temp;              //n1为n垂直单位向量
		n1[1]=n2[0]/temp;
		n1[2]=0;
	}
	else                                //n为垂直矢量
	{
		n1[0]=1;
		n1[1]=0;
		n1[2]=0;
	}
	number=size();
	if(number<3) return 0;             //不能构成多边形
	for(i=0;i<number;i++)              //判断过点point且与n平行的直线是否与多边形相交
	{
		temppoint[0]=at(i);
		temppoint[1]=at((i+1)%number);
		line.Set(temppoint[0],temppoint[1]);
		line.Get (a,b);
		point.Get (c);
		for(j=0;j<3;j++)
		{
			b[j]=b[j]-a[j];
			c[j]=c[j]-a[j];
		}
		vector.CrossProduct(b,c);
		if(vector.Normalize ())      //a,b和c三点不共线
		{
			vector.Get (a);
			if(abs(vector.DotProduct (n2,a))>THRESHOLD10) //n与a,b,c所在平面不平行
				continue;
			vector.Set(b);
			vector.Normalize ();
			vector.Get (b);
			temp=vector.DotProduct (b,c);
			if(temp>-THRESHOLD10 && temp*temp<vector.SquareLength (b)+THRESHOLD10)
				return 1;            //投影点在多边形上
		}
		if(line.Contain (point))
			return 1;
	}
	flag[0]=flag[1]=0;
	FirstValid=-1;                   //多边形第一个不在射线上的顶点的序号
	LastValid=-1;                    //多边形最后一个不在射线上的顶点的序号
	for(i=0;i<number;i++)
	{
		temppoint[0]=at(i);
		a[0]=temppoint[0].m_x-point.m_x;
		a[1]=temppoint[0].m_y-point.m_y;
		a[2]=temppoint[0].m_z-point.m_z;
		temp=a[0]*n2[0]+a[1]*n2[1]+a[2]*n2[2];     //a在n上的投影
		for(j=0;j<3;j++)                           //a在n的垂直平面内的分量
			a[j]-=temp*n2[j];
		vector.Normalize (a);
		vector.CrossProduct(n1,a);
		if(vector.SquareLength ()>THRESHOLD10 ||
			vector.DotProduct (a,n1)<-1+THRESHOLD10)//多边形顶点不在射线n1上
		{
			FirstValid=i;             //第一个不在射线上的多边形顶点的序号
			vector.Normalize ();
			vector.Get (c);
			temp=vector.DotProduct (n2,c);
			if(temp>THRESHOLD10)      //从n方向来看，所取点在n1的逆时针方向
				flag[0]=1;
			else
				flag[0]=-1;
			break;
		}
	}
	if(FirstValid==-1)                //各顶点都在射线上
		return 0;
	Count=0;
	PreValid=FirstValid;
	SucValid=FirstValid;
	temppoint[0]=at(FirstValid);
	for(i=FirstValid+1;i<number+FirstValid+1;i++)
	{
		temppoint[1]=at(i%number);
		a[0]=temppoint[1].m_x-point.m_x;
		a[1]=temppoint[1].m_y-point.m_y;
		a[2]=temppoint[1].m_z-point.m_z;
		temp=a[0]*n2[0]+a[1]*n2[1]+a[2]*n2[2];      //a在n上的投影
		for(j=0;j<3;j++)                            //a在n的垂直平面内的分量
			a[j]-=temp*n2[j];
		vector.Normalize (a);
		vector.CrossProduct(n1,a);
		if(vector.SquareLength ()>THRESHOLD10 ||
			vector.DotProduct (a,n1)<-1+THRESHOLD10)//多边形顶点不在射线n1上
		{
			SucValid=i;
			vector.Normalize ();
			vector.Get (c);
			temp=vector.DotProduct (n2,c);
			if(temp>THRESHOLD10)           //从n方向来看，所取点在n1的逆时针方向
				flag[1]=1;
			else
				flag[1]=-1;
			if(flag[0]*flag[1]==-1)
			{
				//if(SucValid-PreValid>1)    //不在射线上的两个顶点之间至少有一顶点在射线上
				//	Count++;
				//else                       //多边形的边穿过射线所在的直线
				{
					temppoint[0].Get (a);
					temppoint[1].Get(b);
					point.Get (c);
					for(j=0;j<3;j++)
					{
						a[j]-=c[j];
						b[j]-=c[j];
					}
					//求从点point到多边形顶点的矢量在垂直于n的平面上的投影矢量
					temp=vector.DotProduct (n2,a);
					for(j=0;j<3;j++)
					{
						a[j]-=n2[j]*temp;
					}
					temp=vector.DotProduct (n2,b);
					for(j=0;j<3;j++)
					{
						b[j]-=n2[j]*temp;
					}
					vector.CrossProduct (a,b);
					vector.Normalize ();
					vector.Get (b);
					vector.CrossProduct (a,n1);
					vector.Normalize ();
					vector.Get (a);

					temp=vector.DotProduct (a,b);
					if(temp>THRESHOLD10)//多边形的边穿过射线
						Count++;
				}
			}
			PreValid=SucValid;
			flag[0]=flag[1];
			temppoint[0]=temppoint[1];
		}
	}
	if(Count%2)                         //多边形奇数次穿过射线，点在多边形内部　
		return 2;
	else
		return 0;
}

double Polygon3D::GetArea()
{
	double area=0.0;
	double normal[3];
	if(!this->GetNormal (normal))
		return 0.0;

	int num=this->size ();
	for(int i=0;i<num;i++)
	{
		double v1[3],v2[3],v[3];
		Point3D point1,point2;
		point1=this->at (i);
		point2=this->at ((i+1)%num);

		v1[0]=point1.m_x ;
		v1[1]=point1.m_y ;
		v1[2]=point1.m_z ;

		v2[0]=point2.m_x ;
		v2[1]=point2.m_y;
		v2[2]=point2.m_z;

		Vector3D vector;
		vector.CrossProduct (v1,v2);
		vector.Get (v);
		area+=vector.DotProduct (normal,v);
	}
	area/=2.0;
	if(area<0.0)
		return -area;
	return area;
}

