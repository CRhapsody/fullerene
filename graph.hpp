#include <iostream>
using namespace std;
int vertex_id_count = 0;//������
int face_id_count = 0;//����
class face;
class vertex
{
public:
    int direct;//����Ķ�����
    int id;//����ı��
    vertex()
    {
        direct = 0;
        id = vertex_id_count;
        vertex_id_count ++;
    }
    vertex* from_vertex[3];//��ǰ����
    vertex* to_vertex[3];//��󶥵�
    face* adjacent_face[3];//�������ڵ���
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
    int vertex_number;//��Ķ�����
    int id;//��ı��
    int adjface_number;//����ƽ��ͼʱ���Ѿ��е��ڽ������
    int ccount;//������ʱ���Ѿ���ѡȡ���ڽ�����
    int counted;//������ʱ�������Ƿ��ѱ�ѡȡ
    int length;//����ƽ��ͼʱ���Ѿ��еĶ�����
    face()
    {
        adjface_number = 0;
        length = 0;
        id = face_id_count;
        face_id_count ++;
    }
    bool face_append(face* aa);//����aa�����ڽ�������
    bool vertex_push(vertex* aa);//����aa���붥���������ǰ��
    bool vertex_append(vertex* aa);//����aa���붥����������
    bool array_create();//������������ڽ����������ռ�
    vertex* vertex_dop();//��ȡ��󷽵Ķ���
    vertex* vertex_top();//��ȡ��ǰ���Ķ���
    vertex** vertices;//��������
    face** adj_face;//�ڽ�������
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
vertex* begin_vertex(int mode,face* aa,face* bb)//Ѱ�ҳ�ʼ��aa�����һ����bb�����һ���������㣬mode=1ʱ��������Ϊ����
{
    for(int ii = 0; ii < aa->vertex_number; ii++)
    {
        for(int jj = 0; jj < bb->vertex_number; jj++)
        {
            if(aa->vertices[ii] == bb->vertices[jj])
            {
                if(mode == 1)//����
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
                else//����
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
bool connect(face* aa)//����Ķ������Ӷ���
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
