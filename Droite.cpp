#include "Droite.hpp"

// Définition des constantes:
const double M_PI=3.14159;//265358979323846;

// Définition des constructeurs de la classe Droite:
Droite::Droite(){
        a = 0; // coefficient directeur
        b = 0; // ordonnée à l'origine
    }

Droite::Droite(const double direc, const double ord){
    a = direc; // coefficient directeur
    b = ord;   // ordonnée à l'origine
}

// Méthode affiche:
void Droite::affiche(){
    cout << "Droite ("<<a<<","<<b<<")"<<endl;
}

// Méthodes non membres de la classe Droite, mais méthodes outils des méthodes membres:
vector<double> vers_polaire(double m, double p){
    vector<double> vect;
    double r,theta;
    r = abs(p) / sqrt(1 + m * m);
    vect.push_back(r);
    theta = atan(m);
    vect.push_back(theta);
    return vect;
}

vector<double> vers_MP(double r, double theta){
    vector<double> vect;
    double m,p;
    m = -cos(theta) / sin(theta);
    vect.push_back(m);
    p = r / sin(theta);
    vect.push_back(p);
    return vect;
}

// Pour visualiser si les résultats sont correctes et si les méthodes membres fonctionnent bien.
int affiche_buffer(vector<vector<double> >buffer,int largeur,int hauteur){
    for(int i=0;i<hauteur;i++){
        for(int j=0; j<largeur;j++){
            cout<< buffer[i][j]<<" "; // On affiche le buffer case par case.
        }
        cout<<endl;
    }
    return 0;

}

//Partie III) Recherche les coordonnées du barycentre 
vector<int> trouve_barycentre(vector<vector<int> > tableau){
    vector<int> barycentre;
    double x=0;
    double y=0;
    for(unsigned int i=0;i<tableau.size();i++){ // Pour la première coordonnée (en x):
        x=x+(1.0/tableau.size())*tableau[i][0]; // Car on suppose que chaque case à le même poids.
    }
    barycentre.push_back((int)x);
    for(unsigned int i=0;i<tableau.size();i++){ // Pour la seconde coordonnée (en y):
        y=y+(1.0/tableau.size())*tableau[i][1]; // Car on suppose que chaque case à le même poids.
    }
    barycentre.push_back((int)y);
    return barycentre;
}



// Partie I) Transformation de Hough en espace de paramètres (m,p):
 void Droite::tracerMP(const double m, const double p, const int hauteur, const int largeur, const int maxColor, const char* FILENAME){
    ofstream image(FILENAME); // Ouverture du fichier image.ppm en mode écriture

    // Ecriture de l'entête du fichier PPM
    image << "P3\n" << hauteur << " " << largeur << "\n" << maxColor << "\n";

    // Parcours de chaque pixel de l'image
    for (int x = 0; x < hauteur; x++) {
        for (int y = 0; y < largeur; y++) {
            // Calcul de la valeur de y pour la droite en fonction de la valeur de x
            float y_val = m * x + p;

            // Calcul de la distance entre le point et la droite
            //une instruction qui calcule la distance entre un point et une droite en utilisant la formule de la distance d'un point à une droite.
            // La distance calculée permet de déterminer si le point est proche de la droite ou non, ce qui permet de décider quelle couleur lui attribuer.

            float distance = abs(y - y_val) / sqrt(1 + m * m);
                
            // on gére l’anti-aliasing c’est à dire que si le point à colorier ne tombe pas exactement au milieu d’un pixel,
            // on partager la couleur avec les pixels voisins.
                
            // Si le point est proche de la droite, colorier en rouge
            // cette ligne permet de partager la couleur du pixel courant avec ses voisins si celui-ci n'est pas exactement situé au centre d'un pixel.
            if (distance < 0.5) {
                image << maxColor << " 0 0 ";
            } 
            // Sinon, partager la couleur avec les pixels voisins
            else if (distance < 1.0) {
                // on calcule une fraction "frac" qui mesure la distance entre le centre du pixel et le point à colorier
                float frac = distance - 0.5; // on calcule la fraction "frac" comme étant la différence entre la distance et 0,5
                float r = maxColor * (1 - frac);
                float g = 0;
                float b = 0;
                image << (int)r << " " << (int)g << " " << (int)b << " ";
            } 
            // Si le point est loin de la droite, colorier en blanc
            else {
                image << "255 255 255 ";
            }
        }
        image << "\n"; // Nouvelle ligne pour passer à la ligne suivante de l'image
    }
    image.close(); // Fermeture du fichier
}

// Méthode outils de tranformeeHoughMP, qui incrémente le buffer, sur les cases prochent de la droite, définit par les coordonnées du pixel identifié comme appartenant à la droite dans l'image .ppm correspondante.
vector<vector<double> > incrementeMP(vector<vector<double> > buffer, int x, int y, int largeur, int hauteur,vector<double> disc_M,vector<double> disc_P){
    int m = -x; // coefficient directeur 
    int p = y; // ordonnée à l'origine de la droite y=mx+p
    
    for (int i = 0; i < hauteur; i++) {
        double abscisse=disc_M[i];
        for (int j = 0; j < largeur; j++) {
            double ordonnee=disc_P[j];
            double d = abs(ordonnee - m*abscisse - p) / sqrt(m*m + 1); // distance entre le pixel (i,j) et la droite y=mx+p
            if (d < 0.5) { // on incrémente les cases du buffer proches de la droite
                buffer[i][j] += 1;
            }
            else if(d<1.0){
                buffer[i][j] += 0.5;
            }
            else if(d<1.5){
                buffer[i][j] += 0.1;
            }
        }
    }
    return buffer;
}

vector<double> Droite::transformeeHoughMP(const char* FILENAME){
    ifstream infile("image.ppm"); // Ouverture du fichier .ppm en mode lecture.

    string format;
    int largeur, hauteur, maxColor; // déclarer les variables entières pour stocker les dimensions
    infile >> format >> hauteur >> largeur >> maxColor;

    // Définition des vecteurs de discrétisation des paramètres.
    const int n_valM = 60;
    const int n_valP = 60;
    const int M_max = 15;
    const int P_max = 15;
    vector<double> disc_M;
    for(int i=0;i<n_valM; i++){
        double value = -15.0 + 2*(double)M_max / n_valM * i;
        disc_M.push_back(value);
    }
    vector<double> disc_P;
    for(int j=0;j<n_valP; j++){
        double value = -15.0 + 2*(double)P_max / n_valP * j;
        disc_P.push_back(value);
    }

    // à partir des données récupérées dans le fichier .ppm, on crée le buffer d'accumulation, avec les dimensions indiquées.
    vector<vector<double> > buffer(n_valM, vector<double> (n_valP));

    // Parcours de chaque pixel de l'image.
    for (int x=0; x<hauteur; x++){
        for (int y=0; y < largeur; y++){
            int r,g,b; // utiliser des entiers pour stocker les valeurs de couleur
            infile >> r >> g >> b;
            // On teste si le pixel appartient à la droite y=mx+p en regardant sa couleur.
            if (r>192 && b==0 && g==0){
                // On appel la fonction incrémente, avec le buffer actuel et les coordonnées du pixel (x,y) courant:
                buffer=incrementeMP(buffer,x,y,n_valP,n_valP,disc_M,disc_P); // utiliser les crochets pour accéder à un élément spécifique du tableau
            }
        }
    }

    infile.close(); // Fermeture du fichier

    // Parcourir le buffer pour trouver les coordonnées de la case ayant la valeur maximale
    double max_score = 0;
    int max_x;
    int max_y;
    vector<vector<int> > resultat;  // Contiendra une liste de vector de 2 doubles, i et j.
    // Première boucle pour récupérer la valeur de score maximal du buffer.
    for (int y = 0; y < n_valP; y++) {
        for (int x = 0; x < n_valM; x++) {
            if (buffer[x][y] > max_score) {
                max_score = buffer[x][y];
            }
        }
    }
    // Seconde boucle pour récpurer les coordonnées, de toutes les cases ayant ce score maximal.
    for (int y = 0; y < n_valP; y++) {
        for (int x = 0; x < n_valM; x++) {
            if (buffer[x][y] == max_score) {
                vector<int> case_courante;
                case_courante.push_back(x);
                case_courante.push_back(y);
                resultat.push_back(case_courante);
            }
        }
    }
    // Calcul du barycentre des cases du buffer, ayant le score maximal:
    vector<int> barycentre; // 2 double
    barycentre=trouve_barycentre(resultat);
    max_x=barycentre[0];
    max_y=barycentre[1];
    // Afficher les coordonnées de la case ayant la valeur maximale
    cout << "La case avec le score maximal est : (" << max_x << ", " << max_y << ")" << endl;

    cout << "Cela correspond au couple (m,p)=(" << disc_M[max_x] << "," << disc_P[max_y] << ")" << endl;

    affiche_buffer(buffer,n_valP,n_valM);

    vector<double> droite_reperer;
    droite_reperer.push_back(disc_M[max_x]); // coefficient m trouver.
    droite_reperer.push_back(disc_P[max_y]); // coefficient p trouver.
    return droite_reperer;
}



// Partie II) Transformation de Hough en espace de paramètres (r,theta):
void Droite::tracerRTheta(const double r, const double theta, const int hauteur, const int largeur, const int maxColor, const char* FILENAME){
    ofstream image(FILENAME); // Ouverture du fichier image.ppm en mode écriture

    // Ecriture de l'entête du fichier PPM
    image << "P3\n" << largeur << " " << hauteur << "\n" << maxColor << "\n";

    // Parcours de chaque pixel de l'image
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            double xp = x - largeur/2;
            double yp = y - hauteur/2;
            double rp = xp*cos(theta) + yp*sin(theta); // Projection sur la droite
            double d = fabs(rp - r); // Distance entre le point et la droite en polair (fabs valeur absolue pour les flottants)
            if (d <= 0.5) {
                image << maxColor << " 0 0 "; // Rouge
            } else if (d <= 1.0) {
                double frac = d - 0.5; // Fraction de la couleur à partager avec les pixels voisins (antialiasing)
                double rouge = maxColor * (1 - frac);
                image << (int)rouge << " " << "0" << " " << "0" << " "; // 
            } else {
                image << "255 255 255 "; // Blanc
            }
        }
        image << "\n"; // Nouvelle ligne pour passer à la ligne suivante de l'image
    }
    image.close(); // Fermeture du fichier
}

// Méthode outils de tranformeeHoughRTheta, qui incrémente le buffer, sur les cases prochent de la droite, définit par les coordonnées du pixel identifié comme appartenant à la droite dans l'image .ppm correspondante.
vector<vector<double> > incrementeRTheta(vector<vector<double> > buffer, int x, int y, int n_valR, int n_valTheta,vector<double> disc_R,vector<double> disc_Theta,int R) {
    
    for (int i = 0; i < n_valR; i++) {
        double r = disc_R[i]; // Récupère la valeur de r correspondante à la ligne d'indice i.
        for (int j = 0; j < n_valTheta; j++) {
            double theta = disc_Theta[j]; // Récupère la valeur de theta correspondante à la colonne d'indice j.
            double d = abs(r - x*cos(theta) - y*sin(theta)); // Calcule la distance entre le pixel (x,y) et la droite définie par r et theta
            if (d < 5*R/n_valR) { // Incrémente les cases du buffer proches de la droite
                buffer[i][j] += 1;
            }
            else if(d<10*R/n_valR){
                buffer[i][j] += 0.5;
            }
            else if(d<20*R/n_valR){
                buffer[i][j] += 0.1;
            }
        }
    }
    return buffer;
}


vector<double> Droite::transformeeHoughRTheta(const char* FILENAME){
    ifstream infile("image.ppm"); //Ouverture du fichier .ppm en mode lecture.
    
    // Définition des vecteurs de discrétisation des paramètres.
    const int n_valR = 50;
    const int n_valTheta = 100;
    const int R = 10;
    vector<double> disc_R;
    for(int i=0;i<n_valR; i++){
        disc_R.push_back((double)-(double)R+(2*(double)R/(double)n_valR)*i);
    }
    vector<double> disc_Theta;
    for(int j=0;j<n_valTheta; j++){
        disc_Theta.push_back((double)-(M_PI/2)+(M_PI/(double)n_valTheta)*j);
    }

    
    string format;
    int largeur, hauteur, maxColor; // déclarer les variables entières pour stocker les dimensions
    infile >> format >> hauteur >> largeur >> maxColor;

    // à partir des données récupérées dans le fichier .ppm, on crée le buffer d'accumulation, avec les dimensions indiquées.
    vector<vector<double> > buffer(n_valR, vector<double> (n_valTheta));

    // Parcours de chaque pixel de l'image.
    for (int x=0; x<hauteur; x++){
        for (int y=0; y < largeur; y++){
            int r,g,b; // utiliser des entiers pour stocker les valeurs de couleur
            infile >> r >> g >> b;
            // On teste si le pixel appartient à la droite y=mx+p en regardant sa couleur.
            if (r>192 && b==0 && g==0){
                // On appel la fonction incrémente, avec le buffer actuel et les coordonnées du pixel (x,y) courant:
                buffer=incrementeRTheta(buffer,x,y,n_valR,n_valTheta,disc_R,disc_Theta,R); // utiliser les crochets pour accéder à un élément spécifique du tableau
            }
        }
    }

    infile.close(); // Fermeture du fichier

    // Parcourir le buffer pour trouver les coordonnées de la case ayant la valeur maximale
    double max_score = 0;
    int max_x;
    int max_y;
    vector<vector<int> > resultat;  // Contiendra une liste de vector de 2 doubles, i et j.
    // Première boucle pour récupérer la valeur de score maximal du buffer.
    for (int j = 0; j < n_valTheta; j++) {
        for (int i = 0; i < n_valR; i++) {
            if (buffer[i][j] > max_score) {
                max_score = buffer[i][j];
            }
        }
    }
    // Seconde boucle pour récupérer les coordonnées, de toutes les cases ayant ce score maximal.
    for (int j = 0; j < n_valTheta; j++) {
        for (int i = 0; i < n_valR; i++) {
            if (buffer[i][j] == max_score) {
                vector<int> case_courante;
                case_courante.push_back(i);
                case_courante.push_back(j);
                resultat.push_back(case_courante);
            }
        }
    }
    // Calcul du barycentre des cases du buffer, ayant le score maximal:
    vector<int> barycentre; // 2 double
    barycentre=trouve_barycentre(resultat);
    max_x=barycentre[0];
    max_y=barycentre[1];
    double approxR=disc_R[max_x];
    double approxTheta=disc_Theta[max_y];
    // Afficher les coordonnées de la case ayant la valeur maximale
    cout << "La case avec le score maximal est : (" << max_x << ", " << max_y << ")" << endl;
    cout << "Cela correspond au couple (r,theta)=(" << approxR << "," << approxTheta << ")" << endl;
    vector<double> conversion;
    conversion=vers_MP(approxR,approxTheta);
    cout << "Cela correspond au couple (m,p)=(" << conversion[0] << "," << conversion[1] << ")" << endl;

    affiche_buffer(buffer,n_valTheta,n_valR);

    vector<double> droite_reperer;
    droite_reperer.push_back(approxR); // coefficient r trouver.
    droite_reperer.push_back(approxTheta); // coefficient Theta trouver.
    return droite_reperer;
}

// Tests des méthodes de la classe Droite, précédemment définies:
int main(){
    // Partie I espace (m,p):
    Droite D=Droite(3.5,8);
    D.affiche();
    D.tracerMP(D.a,D.b,30,30,255,"image.ppm");
    // test transformee de hough en (m,p).
    vector<double> approx1;
    approx1=D.transformeeHoughMP("image.ppm");
    Droite D2=Droite(approx1[0],approx1[1]);
    D2.tracerMP(D2.a,D2.b,30,30,255,"image2.ppm");

    // Partie II espace (r,theta):
    Droite D3 = Droite(2,0);
    D3.affiche();
    vector<double> convert;
    convert=vers_polaire(D3.a,D3.b);
    D3.tracerRTheta(convert[0],convert[1],30,30,255,"image3.ppm");
    // test transformee de hough en (r,theta).
    vector<double> approx2;
    approx2=D3.transformeeHoughRTheta("image3.ppm");
    approx2=vers_MP(approx2[0],approx2[1]);
    Droite D4=Droite(approx2[0],approx2[1]);
    D4.tracerRTheta(D4.a,D4.b,30,30,255,"image4.ppm");

    // Partie III Suppression des doublons et choix de la meilleure droite.
    return 0;
}
