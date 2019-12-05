////
//// Created by 谭文波 on 2019/12/1.
////
//
//#include "PseudoDelaunay.h"
//#include "common.h"
//#include <math.h>
//PseudoDelaunay::PseudoDelaunay(Line3DArrayEx  &lineArray)
//{
//    int           i,number;
//    Line3D        line;
//    Point3D       point[2];
//    Point3DArray  polygonNodes;
//
//    number=lineArray.GetSize();
//    for(i=0;i<number;i++)
//    {
//        line=lineArray.GetAt(i);
//        line.Get(point[0],point[1]);
//        polygonNodes.Add(point[0]);
//    }
//    m_pointArray=polygonNodes;
//    m_lineArray= lineArray;
//    if(!HasNormalVector(polygonNodes,m_n))
//        for(i=0;i<3;i++)
//            m_n[i]=1;
//}
//PseudoDelaunay::PseudoDelaunay(Point3DArrayEx &pointArray,
//                               Line3DArrayEx  &lineArray)
//{
//    int           i,number;
//    Line3D        line;
//    Point3D       point[2];
//    Point3DArray  polygonNodes;
//
//    number=lineArray.GetSize();
//    for(i=0;i<number;i++)
//    {
//        line=lineArray.GetAt(i);
//        line.Get(point[0],point[1]);
//        polygonNodes.Add(point[0]);
//    }
//    pointArray|=Point3DArrayEx(polygonNodes);
//    m_pointArray=pointArray;
//    m_lineArray= lineArray;
//    if(!HasNormalVector(polygonNodes,m_n))
//        for(i=0;i<3;i++)
//            m_n[i]=1;
//}
//PseudoDelaunay::PseudoDelaunay(PseudoDelaunay &pseudoDelaunay)
//{
//    int i;
//    m_pointArray=pseudoDelaunay.m_pointArray;
//    m_lineArray =pseudoDelaunay.m_lineArray;
//    for(i=0;i<3;i++)
//        m_n[i]=pseudoDelaunay.m_n[i];
//}
//PseudoDelaunay PseudoDelaunay::operator=(PseudoDelaunay &pseudoDelaunay)
//{
//    int i;
//    m_pointArray=pseudoDelaunay.m_pointArray;
//    m_lineArray =pseudoDelaunay.m_lineArray;
//    for(i=0;i<3;i++)
//        m_n[i]=pseudoDelaunay.m_n[i];
//    return *this;
//}
//int PseudoDelaunay::HasNormalVector(Point3DArray &pointArray,double n[])
//{
//    int         i,j,number;
//    int         index1,index2,index3;
//    double      a[3],b[3],c[3];
//    double      temp1,temp2;
//    Vector3D    vector;
//    Point3D     point1,point2,point;
//    Relation3D  posRel;
//
//    number=pointArray.GetSize();
//    if(number<3)                //���ܹ���͹��
//        return 0;
//    temp1=0;
//    for(i=0;i<number-1;i++)     //ȡ��������������
//    {
//        point1=pointArray.GetAt(i);
//        for(j=i+1;j<number;j++)
//        {
//            point2=pointArray.GetAt(j);
//            temp2=vector.SquareLength(point2.m_x-point1.m_x,
//                                      point2.m_y-point1.m_y,
//                                      point2.m_z-point1.m_z);
//            if(temp1<temp2)
//            {
//                temp1=temp2;
//                index1=i;
//                index2=j;
//            }
//        }
//    }
//    if(temp1<THRESHOLD12)           //���е��غ�
//        return 0;
//    temp1=0;
//    point1=pointArray.GetAt(index1);
//    point2=pointArray.GetAt(index2);
//    for(i=0;i<number;i++)        //ȡ��P1P2�������ĵ�
//    {
//        point=pointArray.GetAt(i);
//        temp2=posRel.PToLDistance(point1,point2,point);
//        if(temp1<temp2)
//        {
//            temp1=temp2;
//            index3=i;
//        }
//    }
//    if(temp1<THRESHOLD12)            //���е㹲��
//        return 0;
//
//    if(index3<index1)             //���������������ϵ��Ⱥ�������ȡ�ĵ�
//    {
//        i=index3;
//        index3=index2;
//        index2=index1;
//        index1=i;
//    }
//    else if(index3>index2)
//    {
//    }
//    else
//    {
//        i=index3;
//        index3=index2;
//        index2=i;
//    }
//    point1=pointArray.GetAt(index1);
//    point2=pointArray.GetAt(index2);
//    point =pointArray.GetAt(index3);
//    point1.Get(a);
//    point2.Get(b);
//    point.Get(c);
//    vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//                        c[0]-a[0],c[1]-a[1],c[2]-a[2]);//abXac
//    if(vector.Normalize())
//        vector.Get(n);
//    else
//        return 0;
//    return 1;
//}
//int PseudoDelaunay::IsAInnerTriangle(double a[],double b[],double c[])
//{
//    int            i,flag;
//    int            number;
//    double         p[3],n[3];
//    Relation3D     relation;
//    Vector3D       vector;
//    Point3D        point1,point2,point;
//    Line3D         line1,line2,line;
//    Triangle3D     triangle;
//    //Point3DArray   pointArray;
//
//    number=m_lineArray.GetSize();
//    if(number<3)
//        return 0;
//
//    vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//                        c[0]-a[0],c[1]-a[1],c[2]-a[2]);
//    vector.Normalize();
//    vector.Get(n);
//    vector.Normalize(m_n);
//    if(vector.DotProduct(n,m_n)<THRESHOLD12)  //c�㲻��ab���
//        return 0;
//    line1.Set(a,c);
//    line2.Set(c,b);
//    for(i=0;i<number;i++)
//    {
//        line=m_lineArray.GetAt(i);
//        flag=relation.GetIntersectPoint(line1,line,p);
//        if(flag/10>=3)                         //������ac֮��
//            return 0;
//        flag=relation.GetIntersectPoint(line2,line,p);
//        if(flag/10>=3)                         //������cb֮��
//            return 0;
//    }
//    triangle.Set(a,b,c);
//    number=m_lineArray.GetSize();
//    for(i=0;i<number;i++)
//    {
//        line=m_lineArray.GetAt(i);
//        line.Get(point1,point2);
//        //pointArray.Add(point1);
//        if(triangle.Contain(point1)==7)        //�������ڲ�����Լ�����ߵĽ��
//            return 0;
//    }
//    return 1;
//}
//int PseudoDelaunay::IsAInnerTriangle(Point3D &point1,Point3D &point2,Point3D &point3)
//{
//    double a[3],b[3],c[3];
//    point1.Get(a);
//    point2.Get(b);
//    point3.Get(c);
//    return IsAInnerTriangle(a,b,c);
//}
//int PseudoDelaunay::IsAInnerTriangle(Line3D &line,Point3D &point)
//{
//    double a[3],b[3],c[3];
//    line.Get(a,b);
//    point.Get(c);
//    return IsAInnerTriangle(a,b,c);
//}
//int PseudoDelaunay::IsAPDTriangle(double a[],double b[],double c[])
//{
//    int          i,j,flag;
//    int          number1,number2;
//    double       p[3],p1[3],p2[3],q[3];
//    Vector3D     vector;
//    Point3D      point;
//    Line3D       line;
//    Line3D       Line[3];
//    Circle3D     circle;
//    Relation3D   posRel;
//    //PointArray   pointArray;
//
//    circle.Set(a,b,c);
//    if(circle.m_radius<THRESHOLD12)
//        return 0;
//    if(!IsAInnerTriangle(a,b,c))
//        return 0;
//
//    number1=m_pointArray.GetSize();
//    for(i=0;i<number1;i++)
//    {
//        point=m_pointArray.GetAt(i);
//        point.Get(p);
//
//        //vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//        //					p[0]-a[0],p[1]-a[1],p[2]-a[2]);
//        //vector.Normalize();
//        //vector.Get(p);
//        //if(!vector.DotProduct(m_n,p)>THRESHOLD12)//p����ab���
//        //{
//        //	continue;
//        //}
//
//        if(circle.Contain(p)==2)                 //�������������Բ�ڲ�
//        {
//            Line[0].Set(a,p);
//            Line[1].Set(b,p);
//            Line[2].Set(c,p);
//            flag=0;
//            number2=m_lineArray.GetSize();
//            for(j=0;j<number2;j++)
//            {
//                line=m_lineArray.GetAt(j);
//                int index1=posRel.GetIntersectPoint(line,Line[0],q);
//                int index2=posRel.GetIntersectPoint(line,Line[1],q);
//                int index3=posRel.GetIntersectPoint(line,Line[2],q);
//                if( index1%10>=3 || index2%10>=3 || index3%10>=3)
//                {                                  //��p���������ε�ĳ�����㲻�ɼ�
//                    flag=1;
//                    break;
//                }
//            }
//            if(flag==0)                            //��p���������ε��������㶼�ɼ�
//                return 0;
//        }
//    }
//    ///*Line[0].Set(a,c);
//    //Line[1].Set(b,c);
//    //
//    //flag=0;
//    //number2=m_lineArray.GetSize();
//    //for(j=0;j<number2;j++)
//    //{
//    //	line=m_lineArray.GetAt(j);
//    //	if(line==Line[0])
//    //	{
//    //		flag++;
//    //		break;
//    //	}
//    //}
//    //for(j=0;j<number2;j++)
//    //{
//    //	line=m_lineArray.GetAt(j);
//    //	if(line==Line[1])
//    //	{
//    //		flag++;
//    //		break;
//    //	}
//    //}*/
//    return 1;
//}
//int PseudoDelaunay::IsAPDTriangle(Point3D &point1,Point3D &point2,Point3D &point3)
//{
//    double a[3],b[3],c[3];
//    point1.Get(a);
//    point2.Get(b);
//    point3.Get(c);
//    return IsAPDTriangle(a,b,c);
//}
//int PseudoDelaunay::IsAPDTriangle(Line3D &line,Point3D &point)
//{
//    double a[3],b[3],c[3];
//    line.Get(a,b);
//    point.Get(c);
//    return IsAPDTriangle(a,b,c);
//}
///////////����һ������Σ�����εķ�������֪�����εķ����෴
//Polygon3D PseudoDelaunay::GetConstrainPolygon(double a[],double b[],double c[])
//{
//    int                i,j,flag;
//    int                number1,number2;
//    double             p[3],q[3],n[3];
//    Circle3D           circle;
//    Point3D            tempPoint;
//    Point3D            point[2];
//    Line3D             line;
//    Line3D             Line[3];
//    Triangle3D         triangle;
//    Vector3D           vector;
//    Polygon3D          polygon;
//    Polygon3D          polygon1;
//    Relation3D         posRel;
//    Point3DArrayEx     pointArray;
//
//    circle.Set(a,b,c);                     //ȡ�����ε����Բ
//    triangle.Set(a,b,c);
//    pointArray.Add(a);
//    pointArray.Add(b);
//    pointArray.Add(c);
//    number1=m_pointArray.GetSize();
//    for(i=0;i<number1;i++)                    //���������ε����Բ�ϵ�ɢ�ҵ�
//    {
//        tempPoint=m_pointArray.GetAt(i);
//        tempPoint.Get(p);
//        if((circle.Contain(p)==1) && !triangle.HasVertex(p))
//        {
//            Line[0].Set(a,p);
//            Line[1].Set(b,p);
//            Line[2].Set(c,p);
//            flag=0;
//            number2=m_lineArray.GetSize();
//            for(j=0;j<number2;j++)
//            {
//                line=m_lineArray.GetAt(j);
//                if((posRel.GetIntersectPoint(line,Line[0],q)%10)>=3 ||
//                   (posRel.GetIntersectPoint(line,Line[1],q)%10)>=3 ||
//                   (posRel.GetIntersectPoint(line,Line[2],q)%10)>=3)
//                {                                  //��p���������ε�ĳ�����㲻�ɼ�
//                    flag=1;
//                    break;
//                }
//            }
//            if(flag==0)                            //��p���������ε��������㶼�ɼ�
//                pointArray.Add(tempPoint);
//        }
//    }
//    pointArray.GetPlanarConvexHull(polygon,p);     //����αպϵ㲻�ظ��洢,������Ϊp
//    vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//                        c[0]-a[0],c[1]-a[1],c[2]-a[2]);
//    vector.Normalize();
//    vector.Get(n);
//    if(vector.DotProduct(n,p)>THRESHOLD12)         //����εķ�������֪�����εķ�����ͬ
//    {
//        number1=polygon.GetSize();
//        for(i=number1;i>0;i--)
//        {
//            tempPoint=polygon.GetAt(i-1);
//            polygon1.Add(tempPoint);
//        }
//        return polygon1;
//    }
//    return polygon;
//}
//
//Polygon3D PseudoDelaunay::GetConstrainPolygon(Point3D &point1,Point3D &point2,Point3D &point3)
//{
//    double a[3],b[3],c[3];
//    point1.Get(a);
//    point2.Get(b);
//    point3.Get(c);
//    return GetConstrainPolygon(a,b,c);
//}
//
//Polygon3D PseudoDelaunay::GetConstrainPolygon(Triangle3D &triangle)
//{
//    double a[3],b[3],c[3];
//    triangle.Get(a,b,c);
//    return GetConstrainPolygon(a,b,c);
//}
//
//Polygon3DArrayEx  PseudoDelaunay::CreatePolygonArray(void)
//{
//    int          i,j,flag;
//    int          number1,number2;
//    double       a[3],b[3],c[3],p[3];
//    Vector3D     vector;
//    Point3D      point1,point2,point;
//    Line3D       line1,line2,line;
//    Polygon3D    polygon;
//
//    Point3DArrayEx   pointArray;
//    Line3DArrayEx    constrainLateralArray;
//    Line3DArrayEx    constrainLateralArray1;
//    Line3DArrayEx    lineArray;
//    Line3DArrayEx    polygonEdges;
//    Polygon3DArrayEx polygonArray;
//    /////Լ��������һ����ʱ���⻷����κͶ��˳ʱ���ڻ���������//////
//    //constrainLateralArray=m_lineArray;
//    number1=m_lineArray.GetSize();
//    if(number1<3)
//        return polygonArray;
//    /////���ж�����и����///////////////
//    while(m_lineArray.GetSize()>0)
//    {
//        line=m_lineArray.GetAt(0);
//        line.Get(a,b);
//        number2=m_pointArray.GetSize();
//        flag=0;
//        for(i=0;i<number2;i++)
//        {
//            point=m_pointArray.GetAt(i);
//            point.Get(c);
//            vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//                                c[0]-a[0],c[1]-a[1],c[2]-a[2]);
//            vector.Normalize();
//            vector.Get(p);
//            if(vector.DotProduct(m_n,p)>THRESHOLD12)//c��ab���
//            {
//                if(IsAPDTriangle(a,b,c))
//                {
//                    flag=1;
//                    break;
//                }
//            }
//        }
//        if(flag==0)
//            return polygonArray;
//        //polygon=GetConstrainPolygon(a,b,c);   //�������η����෴��Լ�������
//        polygon.RemoveAll ();
//        polygon.Add (a);
//        polygon.Add (c);
//        polygon.Add (b);
//        polygonArray.Add(polygon);
//        //////////////����һ������κ����¼���Լ������///////////
//        constrainLateralArray1.RemoveAll();
//        lineArray.RemoveAll();
//        number1=m_lineArray.GetSize();
//        ////afxDump<<"1 m_lineArray num="<<number1<<"\n";
//        for(i=0;i<number1;i++)
//        {
//            line=m_lineArray.GetAt(i);
//            polygonEdges=polygon.GetLines();
//            number2=polygonEdges.GetSize();
//            flag=0;
//            for(j=0;j<number2;j++)
//            {
//                line1=polygonEdges.GetAt(j);   //ȡ������ϵ�һ����
//                if((line==line1)==-1)
//                {
//                    lineArray.Add(line);     //�ڶ�����ϵ�Լ����
//                    flag=1;
//                    break;
//                }
//            }
//            if(flag==0)                                //���ڶ�����ϵ�Լ����
//                constrainLateralArray1.Add(line);
//        }
//        m_lineArray=constrainLateralArray1;
//
//        number1=polygonEdges.GetSize();
//        number2=lineArray.GetSize();            //�ڶ�����ϵ�Լ����
//        ////afxDump<<"2 m_lineArray num="<<m_lineArray.GetSize()<<"\n";
//        for(i=0;i<number1;i++)
//        {
//            line=polygonEdges.GetAt(i);
//            flag=0;
//            for(j=0;j<number2;j++)
//            {
//                line1=lineArray.GetAt(j);
//                if((line==line1))
//                {
//                    flag=1;
//                    break;
//                }
//            }
//            if(flag==0)
//                m_lineArray.Add(line);      //������ϵ��±�
//        }
//    }
//    return polygonArray;
//}
///*
//PolygonArray  PseudoDelaunay::CreatePolygonArray(void)
//{
//	int          i,j,flag;
//	int          Pnumber,Lnumber,number;
//	double       a[3],b[3],c[3],p[3];
//	Vector3D     vector;
//	Point3D      point1,point2,point;
//	Line3D       line1,line2,line;
//	Circle3D     circle;
//	Polygon3D    polygon;
//
//	PointArray   pointArray;
//	LineArray    constrainLateralArray;
//	PolygonArray polygonArray;
//
//	TDObjindex   *LateralindexList;
//	TDObjindex   *Current,*Rear;
//	TDObjindex   *Pretemp,*ptemp;
//	/////Լ��������һ����ʱ���⻷����κͶ��˳ʱ���ڻ���������//////
//	constrainLateralArray=m_lineArray;
//	Lnumber=constrainLateralArray.GetSize();
//	if(Lnumber<3)
//		return polygonArray;
//	LateralindexList=NULL;
//	for(i=0;i<Lnumber;i++)               //����Լ��������ֱ�߶ε�������
//	{
//		if(LateralindexList==NULL)
//		{
//	        ptemp=new TDObjindex;
//	        ptemp->index=i;
//	        ptemp->pNext=NULL;
//	        LateralindexList=ptemp;
//			Rear=ptemp;
//		}
//		else
//		{
//			ptemp=new TDObjindex;
//	        ptemp->index=i;
//	        ptemp->pNext=NULL;
//	        Rear->pNext=ptemp;
//			Rear=ptemp;
//		}
//	}
//    /////���ж�����и����///////////////
//	Pnumber=m_pointArray.GetSize();
//	Current=LateralindexList;
//	while(Current!=NULL)
//	{
//		line=constrainLateralArray.GetAt(Current->index);
//		line.Get(a,b);
//		for(i=0;i<Pnumber;i++)
//		{
//			point=m_pointArray.GetAt(i);
//			point.Get(c);
//			vector.CrossProduct(b[0]-a[0],b[1]-a[1],b[2]-a[2],
//				            c[0]-a[0],c[1]-a[1],c[2]-a[2]);
//			vector.Get(p);
//			if(vector.DotProduct(m_n,p)>THRESHOLD12)//c��ab���
//			    if(IsAPDTriangle(a,b,c))
//					break;
//		}
//		polygon=GetConstrainPolygon(a,b,c);   //�������η����෴��Լ�������
//		polygonArray.Add(polygon);
//		//����һ�Ӷ���κ����¼���Լ������
//		number=polygon.GetSize();
//		for(i=0;i<number;i++)
//		{
//			point1=polygon.GetAt(i);
//			point2=polygon.GetAt((i+1)%number);
//			line1.Set(point1,point2);   //ȡ������ϵ�һ����
//
//			flag=0;
//			Pretemp=NULL;
//			ptemp=Current;                       //ָ��Լ������εĵ�һ����
//			while(ptemp!=NULL)
//			{
//				line2=constrainLateralArray.GetAt(ptemp->index);
//				if(line1==line2)           //����ε�һ������Լ�����ߵ�һ����ͬ
//				{
//					if(Pretemp==NULL)
//					{
//						Current=ptemp->pNext;
//						delete ptemp;
//					}
//					else
//					{
//						Pretemp->pNext=ptemp->pNext;
//						delete ptemp;
//					}                    //ɾ����ͬ�ı�
//					flag=1;
//					break;
//				}
//				else
//				{
//					Pretemp=ptemp;
//					ptemp=ptemp->pNext;
//				}
//			}
//			if(flag==0)                  //����ε�ĳ����Լ�����ߵı߲���ͬ
//			{
//				constrainLateralArray.Add(line1);
//				ptemp=new TDObjindex;
//				ptemp->index=constrainLateralArray.GetSize()-1;
//				ptemp->pNext=Current;
//				Current=ptemp;
//			}
//		}//for(i=number-1;i>0;i--)
//	}//while(Current!=NULL)//Current=LateralindexList;
//	return polygonArray;
//}*/
//Triangle3DArrayEx PseudoDelaunay::CreateTriangleArray(void)
//{
//    int               i,j,number1,number2;
//    Polygon3D         polygon;
//    Triangle3D        triangle;
//    Triangle3DArrayEx triangleArray1,triangleArray2;
//    Polygon3DArrayEx  polygonArray;
//
//    //test code lxy add------------------------------------------
//    //   number1 = m_pointArray.GetSize();
//    //Point3D tpoint;
//    //for (int i = 0; i < number1; i++) {
//    //	tpoint = m_pointArray.GetAt(i);
//    //	//afxDump<<"tpoint="<<tpoint.m_x<<"--"<<tpoint.m_y<<"--"<<tpoint.m_z<<"\n";
//    //}
//
//    //number1 = m_lineArray.GetSize();
//    //Line3D tline;
//    //Point3D tpoint1,tpoint2;
//    //for (int i = 0; i < number1; i++) {
//    //	tline = m_lineArray.GetAt(i);
//    //	tline.Get(tpoint1,tpoint2);
//    //	//afxDump<<"tpoint1="<<tpoint1.m_x<<"--"<<tpoint1.m_y<<"--"<<tpoint1.m_z<<"\n";
//    //	//afxDump<<"tpoint2="<<tpoint2.m_x<<"--"<<tpoint2.m_y<<"--"<<tpoint2.m_z<<"\n";
//    //}
//    //test code lxy add------------------------------------------
//    polygonArray=CreatePolygonArray();
//    number1=polygonArray.GetSize();
//    for(i=0;i<number1;i++)
//    {
//        polygon=polygonArray.GetAt(i);
//        triangleArray2=polygon.GetInnerTriangles();
//
//        number2=triangleArray2.GetSize();
//        for(j=0;j<number2;j++)
//        {
//            triangle=triangleArray2.GetAt(j);
//            triangleArray1.Add(triangle);
//        }
//    }
//    return triangleArray1;
//}
///*----------------------------------------------------------------
//��������n�Ĵ�ֱƽ����Լ��Delaunay�����ʷֹ��򣬽��ռ����ʾΪTIN
//*///--------------------------------------------------------------
//Triangle3DArrayEx PseudoDelaunay::CreateTriangleArray(double n[])
//{
//    int            i,j,k,Num,Num1,number;
//    double         a[3],b[3],c[3],d[3],p[3];
//    double         temp;
//    Vector3D       vector;
//    Point3D        point[4];
//    Line3D         line,line1;
//    Point3DArrayEx     pointArray;
//    Line3DArrayEx      lineArray;
//    Triangle3D         triangle;
//    PseudoDelaunay     pseudoDelaunay;
//    Triangle3DArrayEx  triangleArray;
//
//    vector.Set (n);
//    if(vector.Normalize ())
//        vector.Get (n);
//    else
//        return triangleArray;
//
//    number=m_pointArray.GetSize ();
//    for(i=0;i<number;i++)
//    {
//        point[0]=m_pointArray.GetAt (i);
//        point[0].Get (a);
//        temp=vector.DotProduct (n,a);
//        for(j=0;j<3;j++)
//            a[j]-=n[j]*temp;
//        pointArray.Add (a);   //���ռ��ͶӰ���ռ�һ������
//    }
//    number=m_lineArray.GetSize ();
//    for(i=0;i<number;i++)
//    {
//        line=m_lineArray.GetAt (i);
//        line.Get (a,b);
//        temp=vector.DotProduct (n,a);
//        for(j=0;j<3;j++)
//            a[j]-=n[j]*temp;
//        temp=vector.DotProduct (n,b);
//        for(j=0;j<3;j++)
//            b[j]-=n[j]*temp;
//        line.Set(a,b);
//        lineArray.Add (line);//���ռ���ͶӰ���ռ�һ������
//    }
//    pseudoDelaunay=PseudoDelaunay(pointArray,lineArray);
//    triangleArray=pseudoDelaunay.CreateTriangleArray ();
//    number=triangleArray.GetSize ();
//    Num=pointArray.GetSize ();
//    Num1=lineArray.GetSize ();
//    for(i=0;i<number;i++)
//    {
//        triangle=triangleArray.GetAt (i);
//        triangle.Get (point[0],point[1],point[2]);
//        for(j=0;j<3;j++)
//        {
//            for(k=0;k<Num;k++)
//            {
//                point[3]=pointArray.GetAt (k);
//                if(point[j]==point[3])
//                {
//                    point[j]=m_pointArray.GetAt (k);
//                    break;
//                }
//            }
//            if(k==Num)
//            {
//                for(k=0;k<Num1;k++)
//                {
//                    line=lineArray.GetAt (k);
//                    line.Get (a,b);
//                    point[3].Set(a);
//                    if(point[j]==point[3])
//                    {
//                        line=m_lineArray.GetAt (k);
//                        line.Get(c,d);
//                        point[j].Set(c);
//                        break;
//                    }
//                    point[3].Set(b);
//                    if(point[j]==point[3])
//                    {
//                        line=m_lineArray.GetAt (k);
//                        line.Get(c,d);
//                        point[j].Set(d);
//                        break;
//                    }
//                }
//            }
//        }
//        triangle.Set(point[0],point[1],point[2]);
//        triangleArray.SetAt (i,triangle);//
//    }
//    return triangleArray;
//}
///*
//int i,flag,number;
//double a[3],b[3],c[3];
//Point3D point1,point2;
//Line3D line,line1,line2,line3;
//Triangle3D triangle;
//PointArray pointArray;
//LineArray lineArray,lineArray1;
//TriangleArray triangleArray;
//
//lineArray=m_lineArray;
//number=lineArray.GetSize();
//if(number<2)
//    return triangleArray;
//
//while(number>0)
//{
//    pointArray.RemoveAll();
//    for(i=0;i<number;i++)
//    {
//        line=lineArray.GetAt(i);
//        line.Get(a,b);
//        pointArray.Add(a);
//    }
//    for(i=0;i<number;i++)
//    {
//        point1=pointArray.GetAt(i);
//        point1.Get(c);
//        if(IsAPDTriangle(lineArray,a,b,c))
//        {
//            triangleArray.Add(a,b,c);
//            break;
//        }
//    }
//    line1.Set(a,b);
//    line2.Set(a,c);
//    line3.Set(c,b);
//    lineArray1.RemoveAll();
//    for(i=0;i<number;i++)
//    {
//        line=lineArray.GetAt(i);
//        if(line!=line1 && line!=line2 && line!=line3)
//            lineArray1.Add(line);
//    }
//    flag=0;
//    for(i=0;i<number;i++)
//    {
//        line=lineArray.GetAt(i);
//        if(line==line2)
//        {
//            flag=1;
//            break;
//        }
//    }
//    if(flag==0)
//        lineArray1.Add(line2);
//    flag=0;
//    for(i=0;i<number;i++)
//    {
//        line=lineArray.GetAt(i);
//        if(line==line3)
//        {
//            flag=1;
//            break;
//        }
//    }
//    if(flag==0)
//        lineArray1.Add(line3);
//    lineArray=lineArray1;
//    number=lineArray.GetSize();
//}
//return triangleArray;*/
