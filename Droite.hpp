#ifndef DROITEH
#define DROITEH

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;

struct Droite{
public:
    double a;
    double b;
public:
    Droite();
    Droite(const double direc, const double ord);

    void tracerMP(const double m, const double p, const int hauteur, const int largeur, const int maxColor, const char* FILENAME);
    vector<double> transformeeHoughMP(const char* FILENAME);
    void tracerRTheta(const double r, const double theta, const int hauteur, const int largeur, const int maxColor, const char* FILENAME);
    vector<double> transformeeHoughRTheta(const char* FILENAME);
    void affiche();
};

#endif 