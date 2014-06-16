# Movie-Project

Movies organizer. Qt/C++/Sqlite

*Explain here what it is about*

1. Rules of the coding
2. Presentation of the project

## 1. Rules of the coding

 - English is used for all names (class, function, variable…)
 - Abreviations are not tolerated anywhere in the code. All names must be as explicit as possible
 - class names start with a capital letter
 - members start with `m_`
 - local variables start with `l_`
 - function parameter names do not have a specific prefix
 - global variables are prohibited
 - function names do not have a prefix, start with a lowercase character and use upper case characters for each new word (no underscore)
 - curly brackets always stand alone on new lines
 - if / else blocks always have curly brackets, even if only one call is to be done
 - blank line before calling return
 - blank line before two logical blocs

I've seen people who additionally do this (INRIA), is it relevant ? To be discussed
 - Class names start with `C`
 - template names start with `T`
 - interface names start with `I` (as with pure abstract classes of an API)
 - variable names are prefixed with their type : `b` for boolean, `s` for string, `r` for references, `o` for objects, `i16` for signed 16 bits integers, `uint64` for unsigned 64 bits integers or `float80` for 80 bits floats…

## 2. Presentation of the project

[TO BE TRANSLATED:]

Mail de mon pere selon ce qu'il faudrait (synthétise bien le CDC)

L’idéal serait d’être en capacité de récupérer les infos directement dans un truc genre IMDB et l’injecter dans ton outil (comme c’est fait avec gracenote pour Itunes).

http://fr.wikipedia.org/wiki/Liste_de_bases_de_données_cinématographiques_de_l'Internet (plein de choix….)

—> voir ici pour une base gratuite avec des apis http://www.bdfci.info/apis/

### Ainsi, on aurait tout plein d’infos super intéressante :
* Titre original (I)
* Titre Francais (I)
* Date de sortie (I)
* Typr fr film (Drame, Romance, Science Fiction, etc (choix multiples) (I)
* Director (I)
* Pays
* Description, résumé
* Chemin vers les fichers film et sous titre (plusieurs fichiers dans le cas d’un truc avec partie 1 et partie 2)
* Durée (a récupérer sur le fichier)
* Langues disponibles sur le fichier
* Couleur/NB
* Format (16/9, 4/3, etc) (a récupérer sur le fichier)
* Taille de l’image (620x420, 1920x1080 ou autre) (a récupérer sur le fichier)
* Affiche du film.
* Writer
* Star 1, star 2, star 3 , star 4, etc (I)
* Producteur (I)
* Musique par
* Site officiel
* Box office
* budget
* Gains lors de la sortie
* Nb de spectateurs lors de la mise ne exploitation
* Prix : Oscar, lion, etc (plusieurs possibilités) et pour qui (acteur, actrice, producteur, réalisateur, writer, etc) (I)
* lieux de tournage : Paris, New york, plusieurs saisies possibles.

### Complément pour les videos TV
* Saison,
* Episode,
* Identifiant d’épisode


La où j’ai mis (I) c’est que ce la doit être un index de tri ou de recherche."
