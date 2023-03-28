# Projet-Integrateur

PROJET FINAL 

Mode Initial : 

Bouton Interrupt ou bouton blanc pour détection/interruption. 

 

Mode détection : 

DEL AMBREE 

On indique la direction au robot : Si vers haut : bouton Interrupt, si vers droite : bouton blanc : scrutation 

Del verte si vers le haute et Del rouge si vers la droite pendant 2 secs : scrutation 

Robot tourne pour trouver un poteau : 45 degrés 7 fois et arrête lors d’une détection. 

Robot détermine où est le poteau et sauvegarde sa position : combien de fois tourné? Déplacement en x = +-1, déplacement en y = +-8 

Robot avance vers le poteau et s’arrête à 10-15 cm : Attendre une certaine valeure 

Robot émet trois sons aigus + DEL AMBREE 

Robot attend jusqu’à l’appui du bouton Interrupt. 

Robot retourne à l’étape 2 pour 5-8 poteaux 

Robot détecte l’absence de poteau : A tourné 7 fois sans détection 

Robot arrête de bouger 

Robot émet son grave pendant 2 secs. 

Robot faire clignoter DEL ROUGE à 2Hz 

Dessine la forme : https://www.geeksforgeeks.org/quickhull-algorithm-convex-hull/ 

Calcul l’aire 

Calcul CRC32 

Robot sauvegarde (EEPROM) tout : .ecriture 

Mode transmission: 

Évaluateur appui sur RESET 

Évaluateur appui sur bouton blanc (mode transmission) 

Fait le svg 

Commence la transmission : 

DEL VERTE pendant 2 secs. 

DEL VERTE à 10Hz 

Le robot doit prendre les données dans le EEPROM 

Le robot doit créer un SVG  

Le robot doit transmettre le SVG  

Le robot doit transmettre le calcul CRC32 

Fin transmission = éteindre DEL 

+----------+---------------+
| 0x02     | Debut du text |
+----------+---------------+
| 0x03     | Fin du texte  |
+----------+---------------+ 
| 0x04     |Fin de la trans|
+----------+---------------+

 
