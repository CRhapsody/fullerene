#include <iostream>
using namespace std;
int vertex_id_count = 0;//顶点编号
int face_id_count = 0;//面编号
class face;
class vertex
{
public:
    int direct;//顶点的定向数
    int id;//顶点的编号
    vertex()
    {
        direct = 0;
        id = vertex_id_count;
        vertex_id_count ++;
    }
    vertex* from_vertex[3];//向前顶点
    vertex* to_vertex[3];//向后顶点
    face* adjacent_face[3];//定向所在的面
    ~vertex()
    {
        for(int ii = 0; ii < 3; ii++)
        {
            from_vertex[ii] = NULL;
            to_vertex[ii] = NULL;
            adjacent_face[ii] = NULL;
        }
        delete from_vertex;
        delete to_vertex;
        delete adjacent_face;
    }
};
class face
{
public:
    int vertex_number;//面的顶点数
    int id;//面的编号
    int adjface_number;//构建平面图时，已经有的邻接面个数
    int ccount;//解螺旋时，已经被选取的邻接面数
    int counted;//解螺旋时，该面是否已被选取
    int length;//构建平面图时，已经有的顶点数
    face()
    {
        adjface_number = 0;
        length = 0;
        id = face_id_count;
        face_id_count ++;
    }
    bool face_append(face* aa);//将面aa加入邻接面数组
    bool vertex_push(vertex* aa);//将点aa加入顶点数组的最前方
    bool vertex_append(vertex* aa);//将点aa加入顶点数组的最后方
    bool array_create();//给顶点数组和邻接面数组分配空间
    vertex* vertex_dop();//获取最后方的顶点
    vertex* vertex_top();//获取最前方的顶点
    vertex** vertices;//顶点数组
    face** adj_face;//邻接面数组
    ~face()
    {
        for(int ii = 0; ii < vertex_number; ii++)
        {
            adj_face[ii] = NULL;
            vertices[ii] = NULL;
        }
        delete adj_face;
        delete vertices;
    }
};
bool face :: array_create()
{
    adj_face = new face*[vertex_number];
    vertices = new vertex*[vertex_number];
    for(int ii = 0;ii < vertex_number; ii++)
    {
        adj_face[ii] = NULL;
        vertices[ii] = NULL;
    }
}
bool face :: face_append(face* aa)
{
    for(int ii = 0; ii < vertex_number; ii++)
    {
        if(adj_face[ii] == NULL)
        {
            adj_face[ii] = aa;
            return 1;
        }
    }
}
bool face :: vertex_push(vertex* aa)
{
    for(int ii = 0; ii < vertex_number; ii++)
    {
        if(vertices[ii] == NULL)
        {
            int jj;
            for(jj = ii+1; jj < vertex_number; jj++)
            {
                if(vertices[jj] != NULL)
                {
                    vertices[jj-1] = aa;
                    length ++;
                    return 1;
                }
            }
            vertices[vertex_number-1] = aa;
            length ++;
            return 1;
        }
    }
}
bool face :: vertex_append(vertex* aa)
{
    for(int ii = 0; ii < vertex_number; ii++)
    {
        if(vertices[ii] == NULL)
        {
            vertices[ii] = aa;
            length ++;
            break;
        }
    }
}
vertex* face :: vertex_top()
{
    for(int ii = 0; ii < vertex_number; ii++)
    {
        if(vertices[ii] == NULL)
        {
            int jj;
            for(jj = ii+1; jj < vertex_number; jj++)
            {
                if(vertices[jj] != NULL)
                {
                    return vertices[jj];
                }
            }
            return vertices[0];
        }
    }
    return vertices[0];
}
vertex* face :: vertex_dop()
{
    for(int ii = 1; ii < vertex_number; ii++)
    {
        if(vertices[ii] == NULL)
        {
            return vertices[ii-1];
        }
    }
    return vertices[0];
}
vertex* begin_vertex(int mode,face* aa,face* bb)//寻找初始面aa和最后一个面bb的最后一个公共顶点，mode=1时正向，其他为反向
{
    for(int ii = 0; ii < aa->vertex_number; ii++)
    {
        for(int jj = 0; jj < bb->vertex_number; jj++)
        {
            if(aa->vertices[ii] == bb->vertices[jj])
            {
                if(mode == 1)//正向
                {
                    int cc = ii+1;
                    int dd = jj-1;
                    if(cc >= aa->vertex_number)
                        cc -= aa->vertex_number;
                    if(dd < 0)
                        dd += bb->vertex_number;
                    if(aa->vertices[cc] == bb->vertices[dd])
                        return aa->vertices[cc];
                    else
                        return aa->vertices[ii];
                }
                else//反向
                {
                    int cc = ii-1;
                    int dd = jj+1;
                    if(dd >= bb->vertex_number)
                        dd -= bb->vertex_number;
                    if(cc < 0)
                        cc += aa->vertex_number;
                    if(aa->vertices[cc] == bb->vertices[dd])
                        return aa->vertices[cc];
                    else
                        return aa->vertices[ii];
                }
            }
        }
    }
    return NULL;
}
bool connect(face* aa)//给面的顶点增加定向
{
    int ii;
    for(ii = 0; ii < aa->vertex_number-1; ii++)
    {
        aa->vertices[ii]->to_vertex[aa->vertices[ii]->direct] = aa->vertices[ii+1];
        aa->vertices[ii+1]->from_vertex[aa->vertices[ii+1]->direct] = aa->vertices[ii];
        aa->vertices[ii]->adjacent_face[aa->vertices[ii]->direct] = aa;
    }
    aa->vertices[ii]->to_vertex[aa->vertices[ii]->direct] = aa->vertices[0];
    aa->vertices[0]->from_vertex[aa->vertices[0]->direct] = aa->vertices[ii];
    aa->vertices[ii]->adjacent_face[aa->vertices[ii]->direct] = aa;
    for(ii = 0; ii < aa->vertex_number; ii++)
    {
        aa->vertices[ii]->direct ++;
    }
}
