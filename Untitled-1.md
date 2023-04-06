<br></br>

<div align="center">

# **Projet C++/** *Image : Segments* <a name="projet_intro"></a>

<br></br>

<table>
   <thead>
      <tr>
         <th align="center">Groupe:</th>
      </tr>
   </thead>
   <tbody>
      <tr>
      </tr>
      <tr>
         <td align="center">Alexandre SCHAMBACHER</td>
      </tr>
      <tr>
      </tr>
      <tr>
         <td align="center">Aurelien PARDO</td>
      </tr>
      <tr>
      </tr>
      <tr>
         <td align="center">Semen KORZH</td>
      </tr>
   </tbody>
</table>



<br></br>

[![VersionCPlusPlus](https://img.shields.io/badge/C++-v14-red.svg?style=flat&logo=c%2B%2B)](https://fr.wikipedia.org/wiki/C%2B%2B)

[![GIMP ](https://camo.githubusercontent.com/f522d49dfbd9c5dd3b5a495b13ec8359298eb598a1175478a5a848275b9b9cae/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f67696d702d3543353534333f7374796c653d666f722d7468652d6261646765266c6f676f3d67696d70266c6f676f436f6c6f723d7768697465)](https://www.gimp.org/) 


</div>

<br></br>
<br></br>
<br></br>

# TABLEAU DU CONTENU:


- [Objectif](#objectif)
- [Plan de réalisation](#plan-de-réalisation)
  * [Module de la lecture d'un fichier .ppm](#module-lecture)
  * [Module de la détection des droites sur une image (.ppm)](#module-detection)
  * [Module du dessin d'une droite sur une image (.ppm)](#module-dessin)
- [Travail réalisé](#travail-réalisé)
- [Exploitation du code](#exploitation-du-code)
  * [Lecture des fichiers .ppm](#lecture-des-fichiers)
    + [lecture.hpp qui relie lecture.cpp - des fonctions - et main.cpp - espace d'exécution](#lecture.hpp)
    + [lecture.cpp - des fonctions qui interprètent un fichier .ppm en données exploitables](#lecture.cpp)
  * [Détection des droites sur l'image](#detection-droites)

  * [Dessin des droites  sur l'image](#dessin-droites)
    
  * [Test du code dans main.cpp](#test)

- [Bibliographie](#sources)

<br></br>
<br></br>

# Objectif: <a name="objectif"></a> [↩](#tableau-du-contenu)

<br></br>

Supposons que la position d'un pixel peut s'écrire sous forme des coordonnées *(x,y)* entiers.  

Le but de ce projet est d'écrire un programme dans C++ permettant de détecter des droites (segments) sur une image .ppm (et le compiler avec g++) en utilisant:
<br></br>
- **Transformé de Hough dans l'espace des paramètres (m,p)** avec m - un coefficient directeur et p - une constante sur l'ordonnée (x=0) entre deux points:

*y = m⋅ x + p avec m,p - des réels*

- **Transformé de Hough dans l'espace des paramètres (r,θ)** qui est un transformé de (m,p) en coordonnées polaires:

*r = x⋅ cosθ + y⋅ sinθ avec θ ∈ [0, 2π[ et r-un réel*

- **Choix des meilleures droites (segments)** par suppression des doublons...  

De plus, il fallait créer une manière à *dessiner des droites (segments)* avec des coefficients (m,p) pour tester le fonctionnement de la détection programmée.
<br></br>

# Plan de réalisation: <a name="plan-de-réalisation"></a> [↩](#tableau-du-contenu)
On a divisé le projet en 3 modules à réaliser: *un module de la lecture d'un fichier .ppm*,*un module de la détection des droites sur une image (.ppm)* et *un module du dessin d'une droite sur une image (.ppm)*.  
Chaque module consiste des fichiers .cpp et .hpp et soit référencé dans un fichier d'exécution *main.cpp*

<br></br>
## Module de la lecture d'un fichier .ppm <a name="module-lecture"></a>
<br></br>

Nous travaillons sur des fichiers graphiques du format .ppm - le **portable pixmap file**<a href="#ft-ppm" name="_ppm"><sup>1</sup></a>. Ce type de fichier est surtout utilisé pour convertir les fichiers du type pixmap entre différentes plateformes - donc pour échange.  


Un .ppm est composée sur la base suivante (dont aucune ligne ne doit dépasser 70 caractères):  
<br></br>

$${\color{blue}Format}$$  

- Le numéro de variante (à 2 octets): le type de format (PBM,PGN, *PPM*) et la variante (binaire ou *ASCII*). Dans notre cas c'est *P3*. 

* ${\color{gray} Un  \space caractère \space d'espacement \space ( espace, \space tabulation \space ou \space comme \space dans \space notre \space cas \space}$  ${\textcolor{gray}{\textbf{une nouvelle ligne}}}$ ${\color{gray} \space entre  \space chaque \space composante \space sauf \space}$ ${\color{gray} pour \space}$ ${\color{gray} la \space hauteur \space et \space la  \space largeur)}$
<br></br>

$${\color{blue}Largeur \space et \space hauteur}$$  

- La largeur de l'image en nombre de pixels  
+  _Un caractère d'espacement ( **espace** !) entre largeur et hauteur)_  
- La hauteur de l'image en nombre de pixels  
  * Par exemple, `30 30`  
  
- ${\color{gray}Une  \space nouvelle \space ligne}$  
<br></br>

$${\color{blue}Valeur \space maximale}$$  

- la valeur maximale inférieure à 65536 (dans notre cas *255*) qui a impacte sur la couleur d'un pixel :point_down:

- ${\color{gray}Une  \space nouvelle \space ligne}$  
<br></br>

$${\color{blue} Données \space de \space l'image:}$$  

``` 
255
255
255
255
255
255
...
```
L'image est codée ( en caractères ASCII ) en succession des valeurs associées à chaque pixel :  ligne par ligne en partant du haut et du gauche à droite.
Chaque triplet correspond à la couleur d'un pixel en RVG (RGB en anglais), c.à.d. `255,255,255` est un pixel blanc :white_medium_square: et `0,0,0` est un pixel noir :black_medium_square:

> __Note__
>Il existe des lignes qui commencent par **'#'** - ce sont des commentaires qui sont ignorés par les logiciels d'affichage (ie Gimp, Krita, etc) mais auxquelles la limite de 70 charactères par ligne est imposée.

<br></br>
Dans l'ensemble nous avons un fichier comme ci-dessous:

```
P3
# resolution 
30 30
# avec 255 comme val max
255
# debut de l image
255
255
255
255
255
255
...
```

<br></br>
## Module de la détection des droites sur une image (.ppm) <a name="module-detection"></a>
<br></br>
Ce module est destiné à la detection des droites avec les transformations de Hough et, ensuite, la suppression des doublons.
<br></br>  
$${\color{blue}Principe \space de \space détection \space des \space droites \space potentielles \space avec \space 2 \space points \space (coordonnées \space des \space pixels)}$$  

Au premier temps, on veut savoit tout les droites possible. Par définition, une droite(segment) relie au moins 2 points.
Pour une droite de pente m et de constante à l'ordonnée p, et points A et B on calcule:

$\ avec \space A \space = \space \left( x_{a} \space , y_{a} \right) \space et \space B \space = \space \left( x_{b} \space , y_{b} \right) \$

$\ m =  \frac{y_{b}-y_{a}}{x_{b}-x_{a}} \$

$\ p = y_{a} - m * x_{a}  = y_{a} - \frac{y_{b}-y_{a}}{x_{b}-x_{a}} * x_{a} \$


Donc, on va réunir chaque point(pixel) avec les autres. Et, avec chaque point parcouru, le nombre des liaisons à faire va diminuer car il faut éviter des répétitions.

C'est-à-dire, si on a *n* points(pixels) à relier par des droites, alors nous aurions $\frac{(n-1)n}{2}\$ droites potentielles comme résultat.

$$\ Démonstration: n+(n-1)+(n-2)+...+1+0 = n^{2} - \sum_{i=0}^n i\ = n^{2} -\frac{n(n+1)}{2} = \frac{n^{2}-n}{2} = \frac{(n-1)n}{2} \$$

...
<br></br>

<div align="center">
  
<img src="./images/hough_points.gif"  width="95%" height="95%">

</div>
<div align="center" > <strong>Une démonstration de l'algorithme de la détection des droites </strong> <a href="#myft1" name="a1"><sup>3</sup></a> </div>


<br></br>
## Module du dessin d'une droite sur une image (.ppm) <a name="module-dessin"></a>
<br></br>


<br></br>
# Travail réalisé: <a name="travail-réalisé"></a> [↩](#tableau-du-contenu)
<br></br>

**31/03/2023** - un module "lecture" ( lecture.cpp, lecture.hpp ) pour interpréter un fichier .ppm avec C++ et l'exploiter

<br></br>

# Exploitation du code: <a name="exploitation-du-code"></a> [↩](#tableau-du-contenu)
Le code est à exécuter avec console Ubuntu (dans Visual Studio Code) et le compilateur g++ pour une version 14 de C++:

```PowerShell
g++ -Wall -o test -std=c++14 main.cpp lecture.cpp detection.cpp dessin.cpp

./test
````

<br></br>

## Lecture des fichiers .ppm: <a name="lecture-des-fichiers"></a>
Le code destiné à réaliser une lecture et obtention des données utiles d'un fichier (.ppm)
<br></br>

### lecture.hpp qui relie lecture.cpp - des fonctions - et main.cpp - espace d'exécution: <a name="lecture.hpp"></a>
<br></br>

*Des bibliothèques utilisées:*

```c++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> //getline avec delimiteur
```
<br></br>
*Prototypes des fonctions:*
<br></br>
```c++
std::vector<std::string> slicing(std::vector<std::string>& arr, int X, int Y);
```
Une fonction qui retourne un sous-vecteur coupé depuis le vecteur d'entrée.
<br></br>
<br></br>

```c++
std::vector<std::string> file_to_vector(std::string nomFichier);
```
Une fonction qui converti un fichier .ppm en un vecteur des strings en prenant chaque nouvelle ligne comme le séparateur. 
<br></br>
<br></br>

```c++
std::string standard(std::string nomFichier);
```
Une fonction qui retourne le format d'un .ppm; ie P3 dans notre cas. 
<br></br>
<br></br>

```c++
int largeur(std::string nomFichier);
```
Une fonction qui retourne le largeur de l'image 
<br></br>
<br></br>

```c++
int hauteur(std::string nomFichier);
```
Une fonction qui retourne la hauteur de l'image
<br></br>
<br></br>

```c++
int max_value(std::string nomFichier);
```
Une fonction qui retourne la valeur maximale de la couleur des pixels
<br></br>
<br></br>

```c++
std::vector<int> image(std::string nomFichier);
```
Une fonction qui retourne un vecteur des valeurs entières des couleurs des pixels
<br></br>
<br></br>
### lecture.cpp - des fonctions qui interprètent un fichier .ppm en données exploitables: <a name="lecture.cpp"></a>
<br></br>
D'après ce que nous avons vu dans [Module de la lecture d'un fichier .ppm](#module-lecture) on peut déduire les positions (sans commentaires en *'#'* !) de *format*, *hauteur*, *largeur*, *valeur maximal*, *couleurs des pixel* :

| variable à extraire| sa position dans <br/> un fichier sans commentaires | type du variable dans C++ |
| :---: | :---: | :---: |
| format | 0 | `string` |
| hauteur | 1 (1) | `int` |
| largeur | 1 (0) | `int` |
| max_value | 2 | `int` |
| image | de 3 jusqu'à la fin | `vector<int>` |

<br></br>

* **std::vector\<std::string\>** *slicing*(**std::vector\<std::string\>**& arr, **int** X, **int** Y):
  
  > **Entrée**: un vecteur *arr* des chaînes de caractères à couper, un entier *X* - 1ère position et un entier *Y* - dernière position
  
  > **Fonction:** <br></br>
  > on crée des valeurs *start* et *end* avec arr.begin(), X et Y - des valeurs du type spécifique des position du début et fin dans le vecteur arr <br></br>
  > on crée un containeur *result* pour notre sous-vecteur (des chaînes de caractères) de taille Y-X+1 (un nombre des éléments  souhaité) <br></br>
  > on copie des valeurs de arr dans result avec copy(start, end, result.begin()) <br></br>
  
  > **Sortie**: un sous-vecteur *result* de arr de la position X à la position Y
  
```c++
// fonction depuis le site: https://www.geeksforgeeks.org/slicing-a-vector-in-c/
{
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;

    std::vector<std::string> result(Y-X+1);

    copy(start, end, result.begin());

    return result;
}

```  
<br></br>
* **std::vector\<std::string\>** *file_to_vector*(**std::string** nomFichier): 

  > **Entrée**: une chaînes de caractères *nomFichier* - nom du fichier sous forme "nom.ppm" à convertir en vecteur de données
  
  > **Fonction:** <br></br>

  > créons un containeur temporaire *line* pour chaînes de caractères<br></br>
  > créons un vecteur des chaînes de caractères - *arr* <br></br>
  > on ouvre le fichier du nom <br></br>
  > **si le fichier est ouvert**, on copie temporairement dans *line* des chaînes de caractères ligne par ligne sauf des commentaires (commençants avec '#') <br></br>
  > on insert à la fin de *arr*, des chaînes de caractères de *line* avec .push_back() <br></br>
  > **sinon** on affiche une erreur "Pas lisible" <br></br>
  > **fin de la boucle si**<br></br>
  
  > **Sortie**: un vecteur *arr* (en chaînes de caractères) des données du fichier

```c++
{
    std::vector<std::string> arr;
    std::string line;

    std::ifstream mFile (nomFichier); //ouverture du fichier

    if(mFile.is_open()){
        while(!mFile.eof()){
            getline(mFile, line);
            
            if(line[0]!='#'){ //si c'est pas un commentaire
                arr.push_back(line); //obtentions des données en string
            }
        }
    }else{
        std::cout<<"Pas lisible";
    }

    return arr;
}

```
<br></br>

* **std::string** *standard*(**std::string** nomFichier):

  > **Entrée**: une chaînes de caractères *nomFichier* - nom du fichier sous forme "nom.ppm" à lire
  
  > **Fonction:** <br></br>

  > créons un vecteur *arr* des données du fichier avec *file_to_vector*(**std::string** nomFichier) <br></br>
  > créons un containeur *type* pour chaînes de caractères <br></br>
  > on met la première valeur de arr dans type <br></br>
  
  > **Sortie**: une chaîne de caractères *type* du format du fichier, ie 'P3'

```c++
{
    std::vector<std::string> arr = file_to_vector(nomFichier);

    //donne le standard d'un PPM (ie P3 ou autre)

    std::string type;
    type = arr[0];
    
    return type;
}

```
<br></br>
* **int** *largeur*(**std::string** nomFichier):  

  > **Entrée**: une chaînes de caractères *nomFichier* - nom du fichier sous forme "nom.ppm" à lire
  
  > **Fonction:** <br></br>

  > créons un vecteur *arr* des données du fichier avec *file_to_vector*(**std::string** nomFichier) <br></br>
  > créons une chaînes de caractères *dimensions* <br></br>
  > créons une chaînes de caractères *temp_dim* <br></br>
  > en utilisant *stringstream*, on met la deuxième valeur *dimensions* de arr dans *dims* <br></br>
  > créons un containeur temporaire *s* pour chaînes de caractères <br></br>
  > **tant qu'il y a des séparateurs espaces (' '):** on met s contenant une chaîne de caractères entre séparateurs comme une valeur dans *temp_dims* avec *.push_back()* <br></br>
  > **fin de la boucle tant que**<br></br>
  > on définit un entier *largeur* égal à la première case de temp_dim converti en entier avec *stoi()* <br></br>
  
  > **Sortie**: un entier *largeur* de l'image


```c++
{
    std::vector<std::string> arr = file_to_vector(nomFichier);

    //// dimensions de l'image:

    std::string dimensions = arr[1]; // dans ordre: largeur, hauteur
    
    std::vector<std::string> temp_dim;

    std::stringstream dims(dimensions);
    std::string s;

    while (getline(dims,s,' ')){
        temp_dim.push_back(s);
    }

    //stoi convertis string en entier mais en peu "nulle" pour large opérations

    int largeur = stoi(temp_dim[0]);
    
    return largeur;
}

```
<br></br>
* **int** *hauteur*(**std::string** nomFichier): 

  > **Entrée**: une chaînes de caractères *nomFichier* - nom du fichier sous forme "nom.ppm" à lire
  
  > **Fonction:** <br></br>

  > créons un vecteur *arr* des données du fichier avec *file_to_vector*(**std::string** nomFichier) <br></br>
  > créons une chaînes de caractères *dimensions* <br></br>
  > créons une chaînes de caractères *temp_dim* <br></br>
  > en utilisant *stringstream*, on met la deuxième valeur *dimensions* de arr dans *dims* <br></br>
  > créons un containeur temporaire *s* pour chaînes de caractères <br></br>
  > **tant qu'il y a des séparateurs espaces (' '):** on met s contenant une chaîne de caractères entre séparateurs comme une valeur dans *temp_dims* avec *.push_back()* <br></br>
  > **fin de la boucle tant que**<br></br>
  > on définit un entier *hauteur* égal à la deuxième case de temp_dim converti en entier avec *stoi()* <br></br>
  
  > **Sortie**: un entier *hauteur* de l'image



```c++
{    
    std::vector<std::string> arr = file_to_vector(nomFichier);

    //// dimensions de l'image:

    std::string dimensions = arr[1]; // dans ordre: largeur, hauteur
    
    std::vector<std::string> temp_dim;

    std::stringstream dims(dimensions);
    std::string s;

    while (getline(dims,s,' ')){
        temp_dim.push_back(s);
    }

    //stoi convertis string en entier mais en peu "nulle" pour large opérations

    int hauteur = stoi(temp_dim[1]);

    return hauteur;
}

```
<br></br>
* **int** *max_value*(**std::string** nomFichier):


  > **Entrée**: une chaînes de caractères *nomFichier* - nom du fichier sous forme "nom.ppm" à lire
  
  > **Fonction:** <br></br>

  > créons un vecteur *arr* des données du fichier avec *file_to_vector*(**std::string** nomFichier) <br></br>
  
  > **Sortie**: un entier définit par 3ème case de arr converti en entier avec *stoi()*, correspondant à la valeur maximale permit du couleur des pixels

```c++
{
    std::vector<std::string> arr = file_to_vector(nomFichier);
    return stoi(arr[2]);
}
```
<br></br>
* **std::vector\<int\>** *image*(**std::string** nomFichier)*

  > **Entrée**: une chaînes de caractères *nomFichier* - nom du fichier sous forme "nom.ppm" à lire
  
  > **Fonction:** <br></br>

  > créons un vecteur *arr* des données du fichier avec *file_to_vector*(**std::string** nomFichier) <br></br>
  > créons des entiers *X*, *Y* correspondant au début et à la fin des données des couleurs des pixels dans arr
  > créons un sous-vecteur *str_couleurs* en chaînes de caractères et en mettons des données des couleurs des pixels avec *slicing(arr,X,Y)* <br></br>
  > créons un vecteur des entiers *couleurs* <br></br>
  > créons un entier *len* correspondant à la longueur de *str_couleurs* <br></br>
  > **pour un entier j allant de 0 à len sans sa reste de divison par 3 _(en assurrant qu'un triple par pixel de valeurs RGV est respecté)_:** on crée un containeur temporaire *strm* en stringstream d'une valeur de *str_couleurs* en position j. <br></br>
  > Ensuite, on va créer un entier *temp*. <br></br>
  > Pour la conversion en entiers on va forcer une chaîne de caractères *fluide* de *strm* dans *temp*. <br></br>
  > En plus, des valeurs vides vont être convertis en 0. Donc s'il manque des couleurs pour un pixel, la fonction le remplace par un pixel noir. <br></br>
  > On rajoute l'entier de conversion à la fin du vecteur *couleurs* avec *.push_back()* <br></br>
  > **fin de la boucle pour**<br></br>
 
  > **Sortie**: un vecteur des entiers *couleurs* des valeurs des couleurs de l'image


```c++

    std::vector<std::string> arr = file_to_vector(nomFichier);

    //// Image:

    int X = 3; //debut image
    int Y = arr.size(); //fin image
    
    std::vector<std::string> str_couleurs;
    str_couleurs = slicing(arr,X,Y);


    //conversion en int avec remplacement des données corrumpues:
    std::vector<int> couleurs;
    int len = str_couleurs.size();

    for(int j=0; j < len - len % 3; j++){ //éviter le '' à la fin avec modulo

        std::stringstream strm(str_couleurs[j]);

        int temp;
        strm >> temp;
        
        couleurs.push_back(temp);

        }

    return couleurs;
}

```
<br></br>
 
## Détection des droites sur l'image: <a name="detection-droites"></a>

<br></br>
 
*Des bibliothèques utilisées:*

```c++
#include <iostream>
...

```

<br></br>
 
## Dessin des droites  sur l'image: <a name="dessin-droites"></a>

<br></br>
 
*Des bibliothèques utilisées:*

```c++
#include <iostream>
...
 
```

<br></br>
 
## Test du code dans main.cpp: <a name="test"></a>

<br></br>

 
<br></br>
# Bibliographie <a name="sources"></a> [↩](#tableau-du-contenu)
<br></br>

###### <b name="ft-ppm">1</b>:  <a href="https://fr.wikipedia.org/wiki/Portable_pixmap">Portable Pixmap sur Wikipédia</a>[↩](#_ppm)


###### <b name="myft1">3</b>:  <a href="https://homepages.inf.ed.ac.uk/amos/hough.html">Hough Transform par Amos Storkey</a>[↩](#a1)
 
