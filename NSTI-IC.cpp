#include <iostream>
#include<fstream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;


double Pus[1800000][24];
double People[1800000][3];   //������ID   ��������ά��
double structp[24][3];
double PQH[24] = { 0.066,0.074,0.111,0.307,0.069,0.091,0.067,0.106,0.381,0.165,0.038,0.186,0.399,0.063,0.619,0.444,0.070,0.420,0.662,0.485 };
double limit = 0.3 ;   ///������ֵ  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int main()
{
    //��ȡPus
    //�û�ID
    //�����ṹ�ĸ���
    ifstream in;///���ļ��ж�ȡ
    ofstream out;///���ļ����
    vector<int>allPeople;  //�������˵�ID����������
    in.open("Pus.txt");
    int id;
    double pp;
    int jsq=0;
    while(!in.eof())
    {
        jsq++;
        if(jsq%10000==0)
            cout<<jsq<<endl;
        in>>id;
        allPeople.push_back(id);
        for(int i=0;i<20;i++)
        {
            in>>pp;
            Pus[id][i]=pp;
        }
    }
    in.close();

    //��ʼ��     ��Ϊֻ����train�е��û����£�δ��train�е��û��������������ֵΪ0

    for(int i=0;i<allPeople.size();i++)
    {
        int tid=allPeople[i];
        for(int j=0;j<3;j++)
        {
            People[tid][j]=rand()/double(RAND_MAX);
        }
    }

    for(int i=0;i<20;i++)
    {
        for(int j=0;j<3;j++)
        {
            structp[i][j]=rand()/double(RAND_MAX);
        }
    }

    //����
    double lamda=0.001;
    int times = 50;

    for(int circle=0;circle<times;circle++)
    {
        cout<<"circle:"<<circle<<endl;
        for(int i=0;i<allPeople.size();i++)
        {
            for(int j=0;j<20;j++)
            {
                int ttid = allPeople[i];
                int tPsu = 0;
                for(int k=0;k<3;k++)
                {
                    tPsu+=People[ttid][k]*structp[j][k];
                }
                double can=lamda*(Pus[ttid][j] - tPsu);
                for(int k=0;k<3;k++)
                {
                    People[ttid][k]-=can * People[ttid][k];
                    structp[j][k]-=can * structp[j][k];
                }
            }
        }
    }

    //�õ�0u,0s

    int TP=0;
    int FP=0;
    int TN=0;
    int FN=0;

    //�ж�׼ȷ��֮��֮���
    //�ȴ�textx�ж���ʵ��
    in.open("ansTestx.txt");
    int testid,testThingNum,testThingID,testStructNum,testStructType,testStructTypeNum;
    double MSEx=0;
    jsq=0;
    while(!in.eof())
    {

        jsq++;
        if(jsq%10000==0)
            cout<<jsq<<endl;

        in>>testid;
        in>>testThingNum;
        if( Pus[testid][0]!=0 || Pus[testid][1]!=0 || Pus[testid][2]!=0 )   //����train�г��ֹ����û���
        {
            for(int i=0;i<testThingNum;i++)
            {
                in>>testThingID;
                in>>testStructNum;
                double Pui=1.0;
                for(int j=0;j<testStructNum;j++)
                {
                    in>>testStructType>>testStructTypeNum;
                    double tes=0;
                    for(int k=0;k<3;k++)
                    {
                       tes += People[testid][k]*structp[testStructType][k];
                    }
                    Pui *= pow( (1-tes) , testStructTypeNum );
                }
                Pui = 1 - Pui;
                MSEx+=(Pui-1)*(Pui-1);
                if(Pui >limit)
                    TP++;
                else
                    FN++;
            }
        }
        else
        {
            for(int i=0;i<testThingNum;i++)
            {
                in>>testThingID;
                in>>testStructNum;
                double Pui=1.0;
                for(int j=0;j<testStructNum;j++)
                {
                    in>>testStructType>>testStructTypeNum;
                    double tes=PQH[testStructType];
                    Pui *= pow( (1-tes) , testStructTypeNum );
                }
                Pui = 1 - Pui;
                MSEx+=(Pui-1)*(Pui-1);
                if(Pui >limit)
                    TP++;
                else
                    FN++;
            }
        }
    }
    in.close();

    //��testy
    in.open("ansTesty.txt");

    double MSEy = 0;

    jsq=0;
    while(!in.eof())
    {

        jsq++;
        if(jsq%10000==0)
            cout<<jsq<<endl;

        in>>testid;
        in>>testThingNum;
        if( Pus[testid][0]!=0 || Pus[testid][1]!=0 || Pus[testid][2]!=0 )   //����train�г�������û���
        {
            for(int i=0;i<testThingNum;i++)
            {
                in>>testThingID;
                in>>testStructNum;
                double Pui=1.0;
                for(int j=0;j<testStructNum;j++)
                {
                    in>>testStructType>>testStructTypeNum;
                    double tes=0;
                    for(int k=0;k<3;k++)
                    {
                       tes += People[testid][k]*structp[testStructType][k];
                    }
                    Pui *= pow( (1-tes) , testStructTypeNum );
                }
                Pui = 1 - Pui;
                MSEy+=Pui*Pui;
                if(Pui >limit)
                    FP++;
                else
                    TN++;
            }
        }
        else
        {
            for(int i=0;i<testThingNum;i++)
            {
                in>>testThingID;
                in>>testStructNum;
                double Pui=1.0;
                for(int j=0;j<testStructNum;j++)
                {
                    in>>testStructType>>testStructTypeNum;
                    double tes=PQH[testStructType];
                    Pui *= pow( (1-tes) , testStructTypeNum );
                }
                Pui = 1 - Pui;
                MSEy+=Pui*Pui;
                if(Pui >limit)
                    FP++;
                else
                    TN++;
            }
        }
    }
    in.close();

    double canshu = 0.015;    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    cout<<"TP:"<<TP<<" FN:"<<FN<<" FP:"<<FP<<" TN:"<<TN<<endl;
    cout<<"׼ȷ�ʣ�"<<(double)(TP+TN)/(double)(TP+TN+FP+FN)<<endl;
    cout<<"FP*0.03���׼ȷ�ʣ�"<<(double)(TP+TN)/(double)(TP+TN+FP*canshu+FN)<<endl;
    cout<<"MSE:"<<(double)(MSEx+MSEy)/(double)(TP+TN+FP+FN)<<endl;
    cout<<"FP*0.03���MSE��"<<(double)(MSEx+MSEy)/(double)(TP+TN+FP*canshu+FN)<<endl;
    cout<<"MSEx:"<<(MSEx)<<endl;
    cout<<"MSEy:"<<(MSEy)<<endl;
    //double P=TP/(TP+FP*0.03);
    double P=TP/(double)(TP+FP);
    cout<<"���ȣ�"<<P<<endl;
    double tPP=TP/(TP+FP*canshu);
    cout<<"FP*0.03��ľ��ȣ�"<<TP/(TP+FP*canshu)<<endl;
    double R=TP/(double)(TP+FN);
    cout<<"�ٻ��ʣ�"<<R<<endl;
    cout<<"FP*0.03����ٻ��ʣ�(û������Ϊ��ʽ�ﲻ�漰��FP)"<<R<<endl;
    cout<<"F1-������"<<(2*P*R)/(double)(P+R)<<endl;
    cout<<"FP*0.03���F1������"<<(2*tPP*R)/(double)(tPP+R)<<endl;
    return 0;
}
