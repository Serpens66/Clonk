/*-- Rauch --*/

#strict 3

global func Smoke(a,b,c,d,e,f,g,h,i,j)
{
if(FindObject(NOSM)) return; // kein rauch

if(!Equal(GetOwner(), -1))
if(GetPlrKnowledge(GetOwner(),_BA2)) d=RGB(255, 255, 255); // jeder Rauch wird weiß, wenn Besitzer des aufrufenden Objekts die Forschung hat 

return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

// da fkt meist in dem obj welches rauchen soll aufgerufen wird, könnte besitzer über this()
// rausgefunden werden und daher ob er filter erforscht hat
