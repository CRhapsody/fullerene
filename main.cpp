#include "graph.hpp"
#include <iostream>
using namespace std;
int f = 59;
int n = 2*f-4;
int num = 0;
int sum = 0;
int** arra;
int release(face* graph[],vertex* vertices[])//释放内存
{
    for(int ii = 0; ii < n; ii++)
    {
        delete vertices[ii];
        vertices[ii] = NULL;
    }
    for(int ii = 0; ii < f; ii++)
    {
        delete graph[ii];
        graph[ii] = NULL;
    }
    delete[] graph;
    delete[] vertices;
}
int compare(int*xx,int*yy)//比较两个序列的顺序
{
    for(int ii = 0; ii < 6; ii++)
    {
        if(xx[ii] > yy[ii])
        {
            return 1;
        }
        else if(xx[ii] < yy[ii])
        {
            return -1;
        }
    }
    return 0;
}
int* find_id(int* xx)//找到序列在序列数组里的位置
{
    int aa = 0, bb = num-1;
    while(1)
    {
        if(compare(xx,arra[bb]) == 0)
        {
            return arra[bb];
        }
        int cc = (aa + bb) / 2;
        int dd = compare(xx,arra[cc]);
        if(dd == 0)
        {
            return arra[cc];
        }
        else if(dd == -1)
        {
            bb = cc;
        }
        else
        {
            aa = cc;
        }
    }
}
int* solve_spriral(int mode,int n,int m,int f,face* graph[])//从第n个面第m条边开始解螺旋，mode为1时是正向，其他为反向
{
    face* start_face = graph[n];
    if(m-1 > start_face->vertex_number)
    {
        return NULL;//错误，起始边编号不存在
    }
    for(int ii = 0; ii < f; ii++)
    {
        graph[ii]->ccount = graph[ii]->counted = 0;//所有面均未被选取，邻接数归零
    }
    face* temp_graph[f] = {NULL};
    temp_graph[0] = start_face;
    face* pre_face;
    for(int ii = 0; ii < 3; ii++)//找到与初始边相邻的第二个面

    {
        if(start_face->vertices[m]->from_vertex[ii] == start_face->vertices[(m+1)%start_face->vertex_number])
        {
            pre_face = start_face->vertices[m]->adjacent_face[ii];
            break;
        }
    }
    temp_graph[1] = pre_face;
    start_face->counted = pre_face->counted = 1;//两个面均已被选取
    for(int ii = 0; ii < start_face->vertex_number; ii++)//所有与已选取的面邻接的面，邻接数+1
    {
        start_face->adj_face[ii]->ccount ++;
    }
    for(int ii = 0; ii < pre_face->vertex_number; ii++)
    {
        pre_face->adj_face[ii]->ccount ++;
    }
    int ii;
    int pp = 0;
    for(ii = 2; ii < f; ii++)
    {
        while(temp_graph[pp]->vertex_number == temp_graph[pp]->ccount)//当第pp个面的所有邻接面均已被选取
        {
            pp ++;//开始包围下一个面
            if(pp == f)//所有面均已被选取
            {
                break;
            }
        }
        vertex* v1 = begin_vertex(mode,temp_graph[pp],temp_graph[ii-1]);//寻找第pp个面与最后选取的一个面的最后一个公共顶点
        if(v1 == NULL)
        {
            return NULL;//没有公共点，结束
        }
        for(int jj = 0; jj < 4; jj++)
        {
            if(jj == 3)//公共点的三个邻接面都已被选取，无法选出新的面
            {
                return NULL;
            }
            face* face1 = v1->adjacent_face[jj];
            if(face1->counted == 0)//找到了公共顶点未被选取的邻接面
            {
                temp_graph[ii] = face1;
                face1->counted = 1;
                pre_face = face1;
                for(int kk = 0; kk < pre_face->vertex_number; kk++)
                {
                    pre_face->adj_face[kk]->ccount ++;
                }
                break;
            }
        }
    }
    if(ii == f)//所有面均已被选取，返回序列
    {
        int* xx = new int[f];
        for(int ii = 0; ii < f; ii++)
        {
            xx[ii] = temp_graph[ii]->vertex_number;
        }
        return xx;
    }
    else
    {
        return NULL;
    }
}
int create_spiral(int n, int f, int* ff)//用序列创造相应的平面图,ff是序列
{
    vertex_id_count = 0;//点编号重置
    face_id_count = 0;//面编号重置
    face* graph[f] = {NULL};//创造用来存储面的数组
    vertex* vertices[n] = {NULL};//创造用来存储点的数组
    graph[0] = new face;//创造初始面
    graph[1] = new face;
    graph[0]->vertex_number = ff[0];
    graph[1]->vertex_number = ff[1];
    graph[0]->array_create();//给面创造顶点数组，邻接面数组
    graph[1]->array_create();
    for(int ii = 0; ii < ff[0]; ii++)//给初始面创造顶点
    {
        vertex* cc = new vertex;
        graph[0]->vertex_append(cc);
        vertices[vertex_id_count-1] = cc;
    }
    graph[1]->vertex_append(graph[0]->vertices[1]);//从第一个多边形的第一条边开始接多边形
    graph[1]->vertex_append(graph[0]->vertices[0]);//边的方向相反
    for(int ii = 0; ii < ff[1]-2; ii++)//给第二个面增加顶点
    {
        vertex* cc = new vertex;
        graph[1]->vertex_append(cc);
        vertices[vertex_id_count-1] = cc;
    }
    connect(graph[0]);
    connect(graph[1]);//给面上的点增加“定向”
    graph[0]->adjface_number = graph[1]->adjface_number = 1;
    graph[0]->face_append(graph[1]);
    graph[1]->face_append(graph[0]);//互为邻接面
    int pp = 0;//第一个未被包围的面
    int ii;
    for(ii = 2; ii < f; ii++)
    {
        //cout << ii << endl;
        while(graph[pp]->vertex_number == graph[pp]->adjface_number)//假如第pp个面已被包围
        {
            pp ++;
            if(pp == f)//所有面的邻面均完成
            {
                break;
            }
            if(pp == ii-1)//只有最后一个面未被包围，错误
            {
                release(graph,vertices);
                return ii;
            }
        }
        if(pp == f)//错误，没有缺边
            break;
        vertex* open = begin_vertex(1,graph[pp],graph[ii-1]);//求第一个未被包围的面与最后一个面的最后一个公共顶点
        if(open == NULL)//错误，初多边形和末多边形没有公共点
        {
            release(graph,vertices);
            return ii;
        }
        face* temp_face = new face;//创造新面
        graph[ii] = temp_face;
        temp_face->vertex_number = ff[ii];
        temp_face->array_create();
        temp_face->vertex_append(open);
        while(temp_face->length < temp_face->vertex_number)//向前寻找可用顶点
        {
            vertex* tt = temp_face->vertex_dop();//由于新面的方向与原来的面相反，因此从最后一个顶点出发，寻求可用顶点
            if(tt->direct >= 3)//错误，存在一个定向大于等于3，无法构成新多边形的顶点
            {
                release(graph,vertices);
                return ii;
            }
            else if(tt->direct == 2)//定向为2，向前寻找可用顶点
            {
                /*通过分析，定向为2的顶点同向的两个方向中，必有一条边是两个面的公共边，因此两条边有且只有一条可用*/
                vertex* rr = tt->from_vertex[0];//考察两个向前方向中的第一条边
                if(rr->direct == 1)//第一条边的只有一个定向，必可用
                {
                    temp_face->vertex_append(rr);
                    rr->adjacent_face[0]->adjface_number ++;
                    rr->adjacent_face[0]->face_append(temp_face);
                    temp_face->adjface_number ++;
                    temp_face->face_append(rr->adjacent_face[0]);
                    break;
                }
                else if(rr->direct == 2)//第一条边有两个定向
                {
                    if(rr->from_vertex[0] == tt)//第一条边就是公共边，只能用第二条边
                    {
                        rr = tt->from_vertex[1];
                        temp_face->vertex_append(rr);
                        tt->adjacent_face[1]->adjface_number ++;
                        tt->adjacent_face[1]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[1]);
                    }
                    else if(rr->from_vertex[1] == tt)//第一条边就是公共边，只能用第二条边
                    {
                        rr = tt->from_vertex[1];
                        temp_face->vertex_append(rr);
                        tt->adjacent_face[1]->adjface_number ++;
                        tt->adjacent_face[1]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[1]);
                    }
                    else//第一条边不是公共边，可用
                    {
                        temp_face->vertex_append(rr);
                        tt->adjacent_face[0]->adjface_number ++;
                        tt->adjacent_face[0]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[0]);
                    }
                }
                else//第一个向前顶点有三个定向，不可用
                {
                    temp_face->vertex_append(tt->from_vertex[1]);
                    tt->adjacent_face[1]->adjface_number ++;
                    tt->adjacent_face[1]->face_append(temp_face);
                    temp_face->adjface_number ++;
                    temp_face->face_append(tt->adjacent_face[1]);
                }
            }
            else if(tt->direct == 1)//顶向为1不能再往外找顶点了
            {
                break;
            }
        }
        if(temp_face->length == temp_face->vertex_number)//若只向一个方向寻找就得到了足够的顶点
        {
            /*判断首尾顶点是否相连，若相连，则为新面加上该边代表的邻接面*/
            vertex* v1 = temp_face->vertices[0];
            vertex* v2 = temp_face->vertices[temp_face->vertex_number-1];
            if(v1->to_vertex[0] == v2)
            {
                v1->adjacent_face[0]->adjface_number ++;
                v1->adjacent_face[0]->face_append(temp_face);
                temp_face->adjface_number ++;
                temp_face->face_append(v1->adjacent_face[0]);
            }
            else if(v1->to_vertex[1] == v2)
            {
                v1->adjacent_face[1]->adjface_number ++;
                v1->adjacent_face[1]->face_append(temp_face);
                temp_face->adjface_number ++;
                temp_face->face_append(v1->adjacent_face[1]);
            }
        }
        while(temp_face->length < temp_face->vertex_number)//向后寻找可用顶点
        {
            vertex* tt = temp_face->vertex_top();
            if(tt->direct >= 3)//异常3，无法构成新多边形
            {
                release(graph,vertices);
                return ii;
            }
            else if(tt->direct == 2)
            {
                vertex* rr = tt->to_vertex[0];
                if(rr->direct == 1)
                {
                    temp_face->vertex_push(rr);
                    tt->adjacent_face[0]->adjface_number ++;
                    tt->adjacent_face[0]->face_append(temp_face);
                    temp_face->adjface_number ++;
                    temp_face->face_append(rr->adjacent_face[0]);
                    break;
                }
                else if(rr->direct == 2)
                {
                    if(rr->to_vertex[0] == tt)
                    {
                        rr = tt->to_vertex[1];
                        temp_face->vertex_push(rr);
                        tt->adjacent_face[1]->adjface_number ++;
                        tt->adjacent_face[1]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[1]);
                    }
                    if(rr->to_vertex[1] == tt)
                    {
                        rr = tt->to_vertex[1];
                        temp_face->vertex_push(rr);
                        tt->adjacent_face[1]->adjface_number ++;
                        tt->adjacent_face[1]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[1]);
                    }
                    else
                    {
                        temp_face->vertex_push(rr);
                        tt->adjacent_face[0]->adjface_number ++;
                        tt->adjacent_face[0]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[0]);
                    }
                }
                else
                {
                    temp_face->vertex_push(tt->to_vertex[1]);
                    tt->adjacent_face[1]->adjface_number ++;
                    tt->adjacent_face[1]->face_append(temp_face);
                    temp_face->adjface_number ++;
                    temp_face->face_append(tt->adjacent_face[1]);
                }
            }
            else if(tt->direct == 1)
            {
                break;
            }
        }
        while(temp_face->length < temp_face->vertex_number)//若无法在原来的图上找到足够顶点，则为其增加新的顶点
        {
            vertex* vv = new vertex;
            temp_face->vertex_append(vv);
            if(vertex_id_count > n)
            {
                release(graph,vertices);
                return ii;
            }
            vertices[vertex_id_count-1] = vv;
        }
        connect(temp_face);//为面上的点增加定向
    }
    //到此，平面图已构造完成
    if(ii < f-1)//错误，面的数量小于给出数量
    {
        release(graph,vertices);
        return ii;
    }
    if(vertex_id_count != n)//错误，点的数量不对
    {
        release(graph,vertices);
        return -1;
    }
    for(ii = 0; ii < n; ii++)//判断是否为3-正则图
    {
        if(vertices[ii]->direct < 3)
        {
            release(graph,vertices);
            return -1;
        }
    }
    /*
    //此部分是输出图的邻接矩阵
    int** vertex_adj_matrix = new int*[n];
    int** face_adj_matrix = new int*[n];
    for(int ii = 0; ii < n; ii++)
    {
        int* xx = new int[n];
        for(int jj = 0; jj < n; jj++)
            xx[jj] = 0;
        vertex_adj_matrix[ii] = xx;
    }
    for(int ii = 0; ii < f; ii++)
    {
        int* xx = new int[f];
        for(int jj = 0; jj < f; jj++)
            xx[jj] = 0;
        face_adj_matrix[ii] = xx;
    }
    for(int ii = 0; ii < f; ii ++)
    {
        for(int jj = 0; jj < graph[ii]->vertex_number; jj++)
        {
            face_adj_matrix[graph[ii]->id][graph[ii]->adj_face[jj]->id] = 1;
        }
    }
    for(int ii = 0; ii < n; ii ++)
    {
        for(int jj = 0; jj < 3; jj++)
        {
            vertex_adj_matrix[vertices[ii]->id][vertices[ii]->to_vertex[jj]->id] = 1;
        }
    }
    for(int ii = 0; ii < n; ii++)
    {
        for(int jj = 0; jj < n; jj++)
        {
            cout << vertex_adj_matrix[ii][jj] << " ";
        }
        cout << endl;
    }
    for(int ii = 0; ii < f; ii++)
    {
        for(int jj = 0; jj < f; jj++)
        {
            cout << face_adj_matrix[ii][jj] << " ";
        }
        cout << endl;
    }
    */
    sum ++;//到此，已通过该序列成功构造出了平面图
    for(int mode = 0; mode < 2; mode++)//解螺旋，mode=1是正向，mode=2是反向
    for(ii = 0; ii < f; ii++)
    {
        for(int jj = 0; jj < ff[ii]; jj++)
        {
            int* xx = solve_spriral(mode,ii,jj,f,graph);//从第ii个面，第jj条边开始解螺旋，得到相应的序列
            if(xx == NULL)
                continue;
            int* yy = new int[6];
            int k1 = 0;
            for(int kk = 0; kk < f; kk++)//转换序列
            {
                if(xx[kk] == 4)
                {
                    yy[k1] = kk;
                    k1++;
                }
            }
            find_id(yy)[6] = sum;//为解出的序列作记号
            delete[] xx;
            delete[] yy;
        }
    }
    release(graph,vertices);
    return 0;
}
int main()
{
    for(int k1 = 0; k1 < f-5; k1++)
    for(int k2 = k1+1; k2 < f-4; k2++)
    for(int k3 = k2+1; k3 < f-3; k3++)
    for(int k4 = k3+1; k4 < f-2; k4++)
    for(int k5 = k4+1; k5 < f-1; k5++)
    for(int k6 = k5+1; k6 < f; k6++)
    {
        num ++;//计算所有序列的个数，这样算是因为直接用算式计算会产生误差
    }
    arra = new int*[num];//分配内存
    int mm = 0;
    for(int k1 = 0; k1 < f-5; k1++)
    {
        for(int k2 = k1+1; k2 < f-4; k2++)
        {
            for(int k3 = k2+1; k3 < f-3; k3++)
            {
                for(int k4 = k3+1; k4 < f-2; k4++)
                {
                    for(int k5 = k4+1; k5 < f-1; k5++)
                    {
                        for(int k6 = k5+1; k6 < f; k6++)
                        {
                            int* xx = new int[7];//为每个序列分配数组
                            xx[0] = k1;
                            xx[1] = k2;
                            xx[2] = k3;
                            xx[3] = k4;
                            xx[4] = k5;
                            xx[5] = k6;
                            xx[6] = 0;
                            arra[mm] = xx;
                            mm++;
                        }
                    }
                }
            }
        }
    }
    int ff[f];//面序列
    int n = 2*f - 4;
    for(int ii = 0; ii < f; ii++)//初始化面序列
    {
        ff[ii] = 6;
    }
    for(int ii = 0; ii < num; ii++)
    {
        if(ii % 1000 == 0)
            cout << ii << endl;
        if(arra[ii][6] != 0)//若序列已被编号，说明要么与某个序列同构，要么无法产生平面图
        {
            continue;
        }
        for(int jj = 0; jj < 6; jj++)//将序列的相应位置变为4
        {
            ff[arra[ii][jj]] = 4;
        }
        int cc = create_spiral(n,f,ff);//构建并解螺旋，返回操作值
        if(cc != 0)//返回值不为0，说明有错误
        {
            arra[ii][6] = -1;//序列无法产生平面图，记号为-1
            if(cc != -1)//若返回值不为-1，说明在序列中的某步就失败了
            {
                int jj;
                for(jj = 0; jj < 6; jj++)//找出是在哪一步失败了
                {
                    if(arra[ii][jj] == cc)
                        break;
                }
                int kk = 1;
                while(ii+kk < num && arra[ii+kk][jj] == cc)//将所有序列中步骤与之相同的都加上记号-1
                {
                    arra[ii+kk][6] = -1;
                    kk++;
                }
            }
        }
        for(int jj = 0; jj < 6; jj++)//恢复序列
        {
            ff[arra[ii][jj]] = 6;
        }
    }
    cout << sum << endl;
    int kk = 1;
    for(int ii = 0; ii < num; ii++)//输出所有可螺旋序列的第一个同构序列
    {
        if(arra[ii][6] == kk)
        {
            for(int jj = 0; jj < 6; jj++)
            {
                ff[arra[ii][jj]] = 4;
            }
            for(int jj = 0; jj < f; jj++)
                cout << ff[jj] << " ";
            cout << endl;
            for(int jj = 0; jj < 6; jj++)
            {
                ff[arra[ii][jj]] = 6;
            }
            kk++;
        }
    }
    return 0;
}
