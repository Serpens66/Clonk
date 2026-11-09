/* Objekte, denen die KI ausweichen sollte */

// weicht außerdem noch einigen anderen dingen aus, die im enemyai skript definiert sind.

#strict 2

// #appendto FIRB // Feuerball
//#appendto AFTF // Luftklaue
//#appendto FRST // Fireshot
#appendto MISS //  Bazookarakete
#appendto HMIS //  Zielrakete
#appendto CGRN // Clustergranate
#appendto GREN // Granate
#appendto LGRN // Lasergranate
#appendto NGRN // Napalmgranate
#appendto SGRN // Schleimgranate


public func IsDangerous4AI() { return true; }
