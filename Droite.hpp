#ifndef DROITEH
#define DROITEH

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <math.h>
using namespace std;

struct Droite{
public:
    // Deux attributs:
    double a; // Coefficient directeur.
    double b; // Ordonnée à l'origine.
public:
    // 2 Constructeurs:
    Droite();
    Droite(const double direc, const double ord);
    // 1 Méthode affiche:
    void affiche();
    // 2 Méthodes membres, pour la transformation de Hough en espace paramètrique (m,p).
    void tracerMP(const double m, const double p, const int hauteur, const int largeur, const int maxColor, const char* FILENAME);
    vector<double> transformeeHoughMP(const char* FILENAME);
    // 2 Méthodes membres, pour la transformation de Hough en espace paramètrique (r,theta).
    void tracerRTheta(const double r, const double theta, const int hauteur, const int largeur, const int maxColor, const char* FILENAME);
    vector<double> transformeeHoughRTheta(const char* FILENAME);
};

#endif 