#include "graph.hpp"
#include <iostream>
using namespace std;
int f = 59;
int n = 2*f-4;
int num = 0;
int sum = 0;
int** arra;
int release(face* graph[],vertex* vertices[])//�ͷ��ڴ�
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
int compare(int*xx,int*yy)//�Ƚ��������е�˳��
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
int* find_id(int* xx)//�ҵ������������������λ��
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
int* solve_spriral(int mode,int n,int m,int f,face* graph[])//�ӵ�n�����m���߿�ʼ��������modeΪ1ʱ����������Ϊ����
{
    face* start_face = graph[n];
    if(m-1 > start_face->vertex_number)
    {
        return NULL;//������ʼ�߱�Ų�����
    }
    for(int ii = 0; ii < f; ii++)
    {
        graph[ii]->ccount = graph[ii]->counted = 0;//�������δ��ѡȡ���ڽ�������
    }
    face* temp_graph[f] = {NULL};
    temp_graph[0] = start_face;
    face* pre_face;
    for(int ii = 0; ii < 3; ii++)//�ҵ����ʼ�����ڵĵڶ�����

    {
        if(start_face->vertices[m]->from_vertex[ii] == start_face->vertices[(m+1)%start_face->vertex_number])
        {
            pre_face = start_face->vertices[m]->adjacent_face[ii];
            break;
        }
    }
    temp_graph[1] = pre_face;
    start_face->counted = pre_face->counted = 1;//��������ѱ�ѡȡ
    for(int ii = 0; ii < start_face->vertex_number; ii++)//��������ѡȡ�����ڽӵ��棬�ڽ���+1
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
        while(temp_graph[pp]->vertex_number == temp_graph[pp]->ccount)//����pp����������ڽ�����ѱ�ѡȡ
        {
            pp ++;//��ʼ��Χ��һ����
            if(pp == f)//��������ѱ�ѡȡ
            {
                break;
            }
        }
        vertex* v1 = begin_vertex(mode,temp_graph[pp],temp_graph[ii-1]);//Ѱ�ҵ�pp���������ѡȡ��һ��������һ����������
        if(v1 == NULL)
        {
            return NULL;//û�й����㣬����
        }
        for(int jj = 0; jj < 4; jj++)
        {
            if(jj == 3)//������������ڽ��涼�ѱ�ѡȡ���޷�ѡ���µ���
            {
                return NULL;
            }
            face* face1 = v1->adjacent_face[jj];
            if(face1->counted == 0)//�ҵ��˹�������δ��ѡȡ���ڽ���
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
    if(ii == f)//��������ѱ�ѡȡ����������
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
int create_spiral(int n, int f, int* ff)//�����д�����Ӧ��ƽ��ͼ,ff������
{
    vertex_id_count = 0;//��������
    face_id_count = 0;//��������
    face* graph[f] = {NULL};//���������洢�������
    vertex* vertices[n] = {NULL};//���������洢�������
    graph[0] = new face;//�����ʼ��
    graph[1] = new face;
    graph[0]->vertex_number = ff[0];
    graph[1]->vertex_number = ff[1];
    graph[0]->array_create();//���洴�춥�����飬�ڽ�������
    graph[1]->array_create();
    for(int ii = 0; ii < ff[0]; ii++)//����ʼ�洴�춥��
    {
        vertex* cc = new vertex;
        graph[0]->vertex_append(cc);
        vertices[vertex_id_count-1] = cc;
    }
    graph[1]->vertex_append(graph[0]->vertices[1]);//�ӵ�һ������εĵ�һ���߿�ʼ�Ӷ����
    graph[1]->vertex_append(graph[0]->vertices[0]);//�ߵķ����෴
    for(int ii = 0; ii < ff[1]-2; ii++)//���ڶ��������Ӷ���
    {
        vertex* cc = new vertex;
        graph[1]->vertex_append(cc);
        vertices[vertex_id_count-1] = cc;
    }
    connect(graph[0]);
    connect(graph[1]);//�����ϵĵ����ӡ�����
    graph[0]->adjface_number = graph[1]->adjface_number = 1;
    graph[0]->face_append(graph[1]);
    graph[1]->face_append(graph[0]);//��Ϊ�ڽ���
    int pp = 0;//��һ��δ����Χ����
    int ii;
    for(ii = 2; ii < f; ii++)
    {
        //cout << ii << endl;
        while(graph[pp]->vertex_number == graph[pp]->adjface_number)//�����pp�����ѱ���Χ
        {
            pp ++;
            if(pp == f)//���������������
            {
                break;
            }
            if(pp == ii-1)//ֻ�����һ����δ����Χ������
            {
                release(graph,vertices);
                return ii;
            }
        }
        if(pp == f)//����û��ȱ��
            break;
        vertex* open = begin_vertex(1,graph[pp],graph[ii-1]);//���һ��δ����Χ���������һ��������һ����������
        if(open == NULL)//���󣬳�����κ�ĩ�����û�й�����
        {
            release(graph,vertices);
            return ii;
        }
        face* temp_face = new face;//��������
        graph[ii] = temp_face;
        temp_face->vertex_number = ff[ii];
        temp_face->array_create();
        temp_face->vertex_append(open);
        while(temp_face->length < temp_face->vertex_number)//��ǰѰ�ҿ��ö���
        {
            vertex* tt = temp_face->vertex_dop();//��������ķ�����ԭ�������෴����˴����һ�����������Ѱ����ö���
            if(tt->direct >= 3)//���󣬴���һ��������ڵ���3���޷������¶���εĶ���
            {
                release(graph,vertices);
                return ii;
            }
            else if(tt->direct == 2)//����Ϊ2����ǰѰ�ҿ��ö���
            {
                /*ͨ������������Ϊ2�Ķ���ͬ������������У�����һ������������Ĺ����ߣ��������������ֻ��һ������*/
                vertex* rr = tt->from_vertex[0];//����������ǰ�����еĵ�һ����
                if(rr->direct == 1)//��һ���ߵ�ֻ��һ�����򣬱ؿ���
                {
                    temp_face->vertex_append(rr);
                    rr->adjacent_face[0]->adjface_number ++;
                    rr->adjacent_face[0]->face_append(temp_face);
                    temp_face->adjface_number ++;
                    temp_face->face_append(rr->adjacent_face[0]);
                    break;
                }
                else if(rr->direct == 2)//��һ��������������
                {
                    if(rr->from_vertex[0] == tt)//��һ���߾��ǹ����ߣ�ֻ���õڶ�����
                    {
                        rr = tt->from_vertex[1];
                        temp_face->vertex_append(rr);
                        tt->adjacent_face[1]->adjface_number ++;
                        tt->adjacent_face[1]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[1]);
                    }
                    else if(rr->from_vertex[1] == tt)//��һ���߾��ǹ����ߣ�ֻ���õڶ�����
                    {
                        rr = tt->from_vertex[1];
                        temp_face->vertex_append(rr);
                        tt->adjacent_face[1]->adjface_number ++;
                        tt->adjacent_face[1]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[1]);
                    }
                    else//��һ���߲��ǹ����ߣ�����
                    {
                        temp_face->vertex_append(rr);
                        tt->adjacent_face[0]->adjface_number ++;
                        tt->adjacent_face[0]->face_append(temp_face);
                        temp_face->adjface_number ++;
                        temp_face->face_append(tt->adjacent_face[0]);
                    }
                }
                else//��һ����ǰ�������������򣬲�����
                {
                    temp_face->vertex_append(tt->from_vertex[1]);
                    tt->adjacent_face[1]->adjface_number ++;
                    tt->adjacent_face[1]->face_append(temp_face);
                    temp_face->adjface_number ++;
                    temp_face->face_append(tt->adjacent_face[1]);
                }
            }
            else if(tt->direct == 1)//����Ϊ1�����������Ҷ�����
            {
                break;
            }
        }
        if(temp_face->length == temp_face->vertex_number)//��ֻ��һ������Ѱ�Ҿ͵õ����㹻�Ķ���
        {
            /*�ж���β�����Ƿ�����������������Ϊ������ϸñߴ�����ڽ���*/
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
        while(temp_face->length < temp_face->vertex_number)//���Ѱ�ҿ��ö���
        {
            vertex* tt = temp_face->vertex_top();
            if(tt->direct >= 3)//�쳣3���޷������¶����
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
        while(temp_face->length < temp_face->vertex_number)//���޷���ԭ����ͼ���ҵ��㹻���㣬��Ϊ�������µĶ���
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
        connect(temp_face);//Ϊ���ϵĵ����Ӷ���
    }
    //���ˣ�ƽ��ͼ�ѹ������
    if(ii < f-1)//�����������С�ڸ�������
    {
        release(graph,vertices);
        return ii;
    }
    if(vertex_id_count != n)//���󣬵����������
    {
        release(graph,vertices);
        return -1;
    }
    for(ii = 0; ii < n; ii++)//�ж��Ƿ�Ϊ3-����ͼ
    {
        if(vertices[ii]->direct < 3)
        {
            release(graph,vertices);
            return -1;
        }
    }
    /*
    //�˲��������ͼ���ڽӾ���
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
    sum ++;//���ˣ���ͨ�������гɹ��������ƽ��ͼ
    for(int mode = 0; mode < 2; mode++)//��������mode=1������mode=2�Ƿ���
    for(ii = 0; ii < f; ii++)
    {
        for(int jj = 0; jj < ff[ii]; jj++)
        {
            int* xx = solve_spriral(mode,ii,jj,f,graph);//�ӵ�ii���棬��jj���߿�ʼ���������õ���Ӧ������
            if(xx == NULL)
                continue;
            int* yy = new int[6];
            int k1 = 0;
            for(int kk = 0; kk < f; kk++)//ת������
            {
                if(xx[kk] == 4)
                {
                    yy[k1] = kk;
                    k1++;
                }
            }
            find_id(yy)[6] = sum;//Ϊ������������Ǻ�
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
        num ++;//�����������еĸ���������������Ϊֱ������ʽ�����������
    }
    arra = new int*[num];//�����ڴ�
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
                            int* xx = new int[7];//Ϊÿ�����з�������
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
    int ff[f];//������
    int n = 2*f - 4;
    for(int ii = 0; ii < f; ii++)//��ʼ��������
    {
        ff[ii] = 6;
    }
    for(int ii = 0; ii < num; ii++)
    {
        if(ii % 1000 == 0)
            cout << ii << endl;
        if(arra[ii][6] != 0)//�������ѱ���ţ�˵��Ҫô��ĳ������ͬ����Ҫô�޷�����ƽ��ͼ
        {
            continue;
        }
        for(int jj = 0; jj < 6; jj++)//�����е���Ӧλ�ñ�Ϊ4
        {
            ff[arra[ii][jj]] = 4;
        }
        int cc = create_spiral(n,f,ff);//�����������������ز���ֵ
        if(cc != 0)//����ֵ��Ϊ0��˵���д���
        {
            arra[ii][6] = -1;//�����޷�����ƽ��ͼ���Ǻ�Ϊ-1
            if(cc != -1)//������ֵ��Ϊ-1��˵���������е�ĳ����ʧ����
            {
                int jj;
                for(jj = 0; jj < 6; jj++)//�ҳ�������һ��ʧ����
                {
                    if(arra[ii][jj] == cc)
                        break;
                }
                int kk = 1;
                while(ii+kk < num && arra[ii+kk][jj] == cc)//�����������в�����֮��ͬ�Ķ����ϼǺ�-1
                {
                    arra[ii+kk][6] = -1;
                    kk++;
                }
            }
        }
        for(int jj = 0; jj < 6; jj++)//�ָ�����
        {
            ff[arra[ii][jj]] = 6;
        }
    }
    cout << sum << endl;
    int kk = 1;
    for(int ii = 0; ii < num; ii++)//������п��������еĵ�һ��ͬ������
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
