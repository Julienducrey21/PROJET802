#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main() {
  const int largeur = 30;
  const int hauteur = 30;
  const int maxColor = 255;
  const char* FILENAME = "image.ppm";
  float m = -1; // Pente de la droite
  float p = 5.0; // Ordonnée à l'origine de la droite

  ofstream image(FILENAME); // Ouverture du fichier image.ppm en mode écriture

  // Ecriture de l'entête du fichier PPM
  image << "P3\n" << largeur << " " << hauteur << "\n" << maxColor << "\n";

  // Parcours de chaque pixel de l'image
  for (int y = 0; y < hauteur; y++) {
    for (int x = 0; x < largeur; x++) {
      // Calcul de la valeur de y pour la droite en fonction de la valeur de x
      float y_val = m * x + p;

      // Calcul de la distance entre le point et la droite
      float distance = abs(y - y_val) / sqrt(1 + m * m);
    
    // on gére l’anti-aliasing c’est à dire que si le point à colorier ne tombe pas exactement au milieu d’un pixel,
    // on partager la couleur avec les pixels voisins.
      
      // Si le point est proche de la droite, colorier en rouge
      if (distance < 0.5) {
        image << maxColor << " 0 0 ";
      } 
      // Sinon, partager la couleur avec les pixels voisins
      else if (distance < 1.0) {
        float frac = distance - 0.5;
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

  return 0; 
}
