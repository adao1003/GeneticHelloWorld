#include <iostream>
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

struct object
{
    string data;
    double rating;
};
bool compare(const object& a, const object& b)
{
    return (a.rating>b.rating);
}
void generate(default_random_engine& gen,vector<object>& population,  int count)
{
    string temp = "............";
    uniform_int_distribution<char> ran(32,126);
    for(int i=0; i<count; i++)
    {
        for(int j=0; j<temp.size(); j++)
            temp[j]=ran(gen);
        population.push_back({temp,0});
    }
}
double rate(const string& temp)
{
    const string hello = "Hello world!";
    double rate = 0;
    for(int i=0; i<12; i++)
    {
        auto pos=temp.find(hello[i]);
        if(temp[i]==hello[i])
            rate+=1;
        else if(pos!=string::npos)
            rate+=0.3;
    }
    return rate;
}
void select(vector<object>& population)
{
    for(int i=0; i<population.size(); i++)
    {
        population[i].rating=rate(population[i].data);
    }
    sort(population.begin(),population.end(),compare);
}

object cross(default_random_engine& gen,const object& a, const object& b)
{
    uniform_real_distribution<double> r(0,1);
    double rand;
    string temp = "............";
    for(int i=0; i<temp.size(); i++)
    {
        rand=r(gen);
        if(rand<0.5)
            temp[i]=a.data[i];
        else
            temp[i]=b.data[i];
    }
    return {temp,0};

}
void crossover(default_random_engine& gen,vector<object>& population, int count)
{
    int k=count;
    for(int i=0; i<count; i++)
        for(int j=i+1; j<count; j++)
        {
            population[k]=cross(gen, population[i],population[j]);
            k++;
            if(k==population.size())
                return;
        }
}
void mutate(default_random_engine& gen,vector<object>& population, double probability )
{
    uniform_real_distribution<double> r(0,1);
    uniform_int_distribution<char> ran(32,126);
    for(int i=0; i<population.size(); i++)
    {
        for(int j=0; j<population[i].data.size(); j++)
        {
            double p=r(gen);
            if(p<=probability)
                population[i].data[j]=ran(gen);
        }
    }
}
int main()
{
    int LO, LK ;
    double LM;
    int generation = 0;
    vector<object> population;
    auto seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gen(seed);
    ofstream out("out.txt");
    cout << "Ilosc osobnikow: ";
    cin >> LO;
    cout << endl << "Ilosc osobnikow do krzyzowania: ";
    cin >> LK;
    cout << endl << "Prawdopodobienstwo mutacji: ";
    cin >> LM;
    generate(gen,population, LO);
    for(;;)
    {
        select(population);
        crossover(gen,population,LK);
        mutate(gen,population,LM);
        //cout << generation << endl;
        out << generation << endl;
        for(int i=0; i<population.size(); i++)
        {
            //cout << population[i].data << " ";
            out << population[i].data << "\t";
        }
        //cout << endl;
        out << endl;
        generation++;
        if(population[0].rating==12)
        {
            cout << "Liczba pokolen: " << generation << endl;
            system("pause");
            return 0;
        }
    }
    return 0;
}