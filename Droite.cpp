#include "Droite.hpp"

Droite::Droite(){
        a = 0;
        b = 0;
    }

Droite::Droite(const double direc, const double ord){
    a = direc;
    b = ord;
}

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
            // La distance calculée permet de déterminer si le point est proche de la droite ou non, ce qui permet de décider quelle couleur lui attribuer

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



vector<vector<double> > incremente(vector<vector<double> > buffer, int x, int y, int largeur, int hauteur) {
    int m = -x; // coefficient directeur 
    int p = y; // ordonnée à l'origine de la droite y=mx+p
    
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            float d = abs(j - m*i - p) / sqrt(m*m + 1); // distance entre le pixel (i,j) et la droite y=mx+p
            if (d < 0.1) { // on incrémente les cases du buffer proches de la droite
                buffer[i][j] += 1;
            }
            else if(d<0.5){
                buffer[i][j] += 0.5;
            }
            else if(d<0.7){
                buffer[i][j] += 0.1;
            }
        }
    }
    return buffer;
}



int affiche_buffer(vector<vector<double> >buffer,int largeur,int hauteur){
    for(int i=0;i<hauteur;i++){
        for(int j=0; j<largeur;j++){
            cout<< buffer[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;

}



vector<double> Droite::transformeeHoughMP(const char* FILENAME){
    ifstream infile("image.ppm");

    string format;
    int largeur, hauteur, maxColor; // déclarer les variables entières pour stocker les dimensions
    infile >> format >> hauteur >> largeur >> maxColor;

    // à partir des données récupérées dans le fichier .ppm, on crée le buffer d'accumulation, avec les dimensions indiquées.
    vector<vector<double> > buffer(hauteur, vector<double> (largeur));

    // Parcours de chaque pixel de l'image.
    for (int x=0; x<hauteur; x++){
        for (int y=0; y < largeur; y++){
            int r,g,b; // utiliser des entiers pour stocker les valeurs de couleur
            infile >> r >> g >> b;
            // On teste si le pixel appartient à la droite y=mx+p en regardant sa couleur.
            if (r>192 && b==0 && g==0){
                // On appel la fonction incrémente, avec le buffer actuel et les coordonnées du pixel (x,y) courant:
                buffer=incremente(buffer,x,y,largeur,hauteur); // utiliser les crochets pour accéder à un élément spécifique du tableau
            }
        }
    }

    infile.close(); // Fermeture du fichier

    // Parcourir le buffer pour trouver les coordonnées de la case ayant la valeur maximale
    int max_score = 0;
    int max_x = 0;
    int max_y = 0;
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            if (buffer[x][y] > max_score) {
                max_score = buffer[x][y];
                max_x = x;
                max_y = y;
            }
        }
    }

    // Afficher les coordonnées de la case ayant la valeur maximale
    cout << "La case avec le score maximal est : (" << max_x << ", " << max_y << ")" << endl;

    affiche_buffer(buffer,largeur,hauteur);

    vector<double> droite_reperer;
    droite_reperer.push_back(max_x); // coefficient m trouver.
    droite_reperer.push_back(max_y); // coefficient p trouver.
    return droite_reperer;
}

void Droite::affiche(){
    cout << "Droite ("<<a<<","<<b<<")"<<endl;
}

int main(){
    Droite D=Droite(2,1);
    D.affiche();
    D.tracerMP(D.a,D.b,30,30,255,"image.ppm");
    vector<double> approx;
    approx=D.transformeeHoughMP("image.ppm");
    Droite D2=Droite(approx[0],approx[1]);
    D2.tracerMP(D2.a,D2.b,30,30,255,"image2.ppm");
    return 0;
}
