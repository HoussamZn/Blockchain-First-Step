# ATELIER 2
## EXERCICE 2
### 2. Décris le mode de conversion du texte d’entrée en bits

Le texte d’entrée est converti en une représentation binaire à l’aide de la fonction `text_to_bits()`. Chaque caractère du texte est traité comme un octet de 8 bits. Pour chaque caractère :

On parcourt les 8 bits du caractère, du bit le plus significatif (MSB) au bit le moins significatif (LSB).

On ajoute '1' ou '0' à une chaîne de caractères selon que le bit vaut 1 ou 0.

Ensuite, cette chaîne de '0' et '1' peut être convertie en un vecteur d’entiers (0 ou 1) via la fonction `init_state()`, qui initialise l’état du système cellulaire pour le calcul du hash.

__En résumé :__
```
chaque caractère → 8 bits → vecteur binaire pour l’automate cellulaire.
```

### 3. Explique le processus utilisé pour produire un hash final fixe de 256 bits
Le calcul du hash se fait dans la fonction ac_hash(). Le processus est le suivant :

##### Découpage en blocs (chunks) :

La chaîne de bits issue du texte est divisée en blocs de taille fixe `CHUNK_SIZE` (ici 256 bits par exemple).

Si le dernier bloc est incomplet, il est rempli avec des zéros pour atteindre la taille exacte.

##### Initialisation de l’état :

Chaque bloc est converti en vecteur d’entiers 0/1 via `init_state()`.

##### Évolution de l’automate cellulaire :

L’état du bloc évolue pendant un nombre fixe d’itérations (steps).

À chaque itération, la fonction `evolve()` applique la règle du `CA` (rule number) à chaque cellule selon ses voisins gauche, centre, droite (voisinage de 3).

##### Combinaison des blocs :

Après évolution, l’état final de chaque bloc est XORé avec le digest courant (vecteur accumulatif).

Ensuite, le digest lui-même subit une évolution supplémentaire avec la même règle.

##### Conversion finale :

Le digest final, un vecteur de taille `CHUNK_SIZE`, est converti en hexadécimal via `bits_to_hex()`.

Le résultat est une empreinte fixe de 256 bits (ou de la taille de digest choisie) représentant le hash du texte d’entrée.

##### En résumé :
```
Texte → bits → blocs → CA évolutif → combinaison (XOR) → digest final → hexadécimal.
```

Ce processus assure un hash déterministe, sensible aux modifications d’un seul bit, et de taille fixe.

### 4.Vérifie par un test que deux entrées différentes donnent deux sorties différentes.

l'entree : `Houssam` et `HoussaM` donent des Hash completement different :
```
Houssam : 1fb5fb5bb9033896310e5e7434c760442b40863088967bba165588218cc6440d
HoussaM : 366f9e62be4ca235e4d1d33fe156bab13f6ea12d948d151ee3989c27c2dc105d
```

## Exercice 4
_Tableau des résultats :_
|  Method   | Avg Iterations | Total Time (s) | Avg Time/Block (s) |
| :---------: |:--------------:| :--------------: |:------------------:|
|AC         |196             |11.7701         |1.17701             |
|SHA256     |321             |0.03087      |0.00308         |

## Exercice 5
J’ai utilisé `1011110100101010101011110101011` et `1011110100101000101011110101011` qui ne diffèrent que d’un seul bit, au niveau du 15ᵉ bit.
le pourcentage moyen de bits différents entre les hashes de deux messages est `49.61%`

## Exercice 6
La distribution est équilibrée : pour une entrée de _10⁵_ bits, j’ai obtenu `48.05%` de bits à 1.

## Exercice 7
### 2.Compare la stabilité des résultats et les temps d’exécution.
_Tableau des résultats :_

|  Rule   | Time (s) | Ones percentage | Difference percentage in one bit difference inputs |
| :---------: |:--------------:| :--------------: |:------------------:|
|30         |0.697236             |48.44%         |56.25%             |
|90     |0.623147             |0.00%       |0.00%          |
|110     |0.507385             |58.59%        |32.03%          |

### 3.Indique quelle règle te semble la plus adaptée pour le hachage et pourquoi.
__La règle 30__ est la meilleure fonction de hachage, car elle produit des résultats équilibrés et une grande différence entre deux entrées ne différant que d’un seul bit.\
__La règle 90__, en revanche, n’est pas adaptée au hachage, car après quelques itérations, tous les bits deviennent nuls.

