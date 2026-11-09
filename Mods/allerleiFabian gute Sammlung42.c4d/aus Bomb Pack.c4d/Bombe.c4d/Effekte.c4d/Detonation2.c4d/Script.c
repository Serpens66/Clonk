#strict

Launch:
  SetClrModulation(RGBa(255,255,255,80));
  DoCon(Sub(Div(Mul(Par(0),100),32),100));
  SetVar(1,Par(1));
  if(LessThan(Var(1),1)) SetVar(1,1);
  SetLocal(0,Par(0));
  SetVar(0,Mul(Div(Sub(32,Div(Mul(32,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(0)));
  while(Not(LessThan(Var(1),Var(2))))  And(SearchTarget(),And(BlastObjects(GetX(),GetY(),Div(Local(0),3)),SetVar(2,Sum(Var(2),1))));
  DigFree(GetX(),GetY(),Div(Local(0),3));
  Sound("explo*");
  return(1);
SearchTarget:
  SetVar(1,FindObject(0,Mul(-1,Sum(Div(Local(0),2),5)),Mul(-1,Sum(Div(Local(0),2),5)),Sum(Local(0),10),Sum(Local(0),10),OCF_Living()));
  SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  while(SetVar(1,FindObject(0,Mul(-1,Sum(Div(Local(0),2),5)),Mul(-1,Sum(Div(Local(0),2),5)),Sum(Local(0),10),Sum(Local(0),10),OCF_Living(),0,0,0,Var(1)))) SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  return();
Initialize:
  SetAction("Blast");
  SetLocal(0,32);
  return(1);  
Remove:
  RemoveObject();
  return(1);
Launch2:
//Sprengsatz
//Par(0)...Größe
//Par(1)...Stärke
//Par(2)...Sprengsatz
//Par(3)...Winkel zur letzten Explosion/Sprengsatz
  SetLocal(9,Par(2));
  SetLocal(8,Par(3));
//Größe anpassen
  SetClrModulation(RGBa(255,255,255,80));
  DoCon(Sub(Div(Mul(Par(0),100),32),100));
  SetVar(1,Par(1));
  if(LessThan(Var(1),1)) SetVar(1,1);
  SetLocal(0,Par(0));
//Position nach Größenveränderung anpassen, damit der Mittelpunkt der Größenveränderten Explosion am alten Ort bleibt
  SetVar(0,Mul(Div(Sub(32,Div(Mul(32,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(0)));
//Je nach Stärke öfter Explosionsschaden zufügen
  while(Not(LessThan(Var(1),Var(2))))  And(SearchTarget(),And(BlastObjects(GetX(),GetY(),Div(Local(0),3)),SetVar(2,Sum(Var(2),1))));
  DigFree(GetX(),GetY(),Div(Local(0),3));
  SetAction("Blasting");
  Sound("explo*");
  return(1);
NextExplode:
//EndCall von "Blasting"
//Explosion weiter ausbreiten
//prüfen ob noch Explosion plaziert werden kann, d.h. ob die Druckwelle noch stark genug ist
  if(LessThan(GetComponent(BOMB,0,Local(9)),1)) return(ObjectCall(Local(9),"Finished"));
//Neue Explosionen werden in Winkeln platziert, in dem diese Explosion von der letzten platziert wurde +- 45 Grad
//Am geplanten Ort darf keine Massivfläche sein und die Explosion darf sich nicht mit einer anderen überlagern
//Das zu prüfen ist schwer, FindObject findet alle platzieren Explosionen außer denen, die zeitgleich platziert werden
//Deshalb werden die Koordinaten aller Explosionen im Dataobjekt (DT3V) mit der Aktion "Sprenger" und dem Sprengsatz
//als Aktionsziel als Komponenten "X0.." und "Y0.." mit der Nummer der Explosion dahinter gespeichert.
//So werden im Dataobject vor Platzierung alle Explosionen gecheckt, damit keine Überlagerung zustande kommt
//Dann Explosion platzieren, "Launch2" aufrufen, Stärke ändern (Div(GetComponent(BOMB,0,Local(9)),12)),
//Winkel übergeben (zwecks Beugung bei Wellen) und Reichweite im Sprengsatz verringern
//Und die Koordinaten im Dataobjekt eintragen
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(Sub(Local(8),45),25)),Sum(GetY(),Mul(-1,Cos(Sub(Local(8),45),25)))))
  if(Not(FindObject(EY3V,Sub(Sum(GetX(),Sin(Sub(Local(8),45),25)),30),Sub(Sum(GetY(),Mul(-1,Cos(Sub(Local(8),45),25))),30),60,60)))
  if(Not(ObjectCall(FindObject(DT3V,0,0,0,0,0,"Sprenger",Local(9)),"CheckExplosion",Sum(GetX(),Sin(Sub(Local(8),45),25)),Sum(GetY(),Mul(-1,Cos(Sub(Local(8),45),25))))))
  And(ObjectCall(FindObject(DT3V,0,0,0,0,0,"Sprenger",Local(9)),"NewPosSet",Sum(GetX(),Sin(Sub(Local(8),45),25)),Sum(GetY(),Mul(-1,Cos(Sub(Local(8),45),25)))),And(ObjectCall(CreateObject(EY3V,Sin(Sub(Local(8),45),25),Sum(Mul(-1,Cos(Sub(Local(8),45),25)),20),GetOwner()),"Launch2",50,Div(GetComponent(BOMB,0,Local(9)),12),Local(9),Sub(Local(8),45)),SetComponent(BOMB,Sub(GetComponent(BOMB,0,Local(9)),1),Local(9))));
  if(LessThan(GetComponent(BOMB,0,Local(9)),1)) return(ObjectCall(Local(9),"Finished"));
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(Local(8),25)),Sum(GetY(),Mul(-1,Cos(Local(8),25)))))
  if(Not(FindObject(EY3V,Sub(Sum(GetX(),Sin(Local(8),25)),30),Sub(Sum(GetY(),Mul(-1,Cos(Local(8),25))),30),60,60)))
  if(Not(ObjectCall(FindObject(DT3V,0,0,0,0,0,"Sprenger",Local(9)),"CheckExplosion",Sum(GetX(),Sin(Local(8),25)),Sum(GetY(),Mul(-1,Cos(Local(8),25))))))
  And(ObjectCall(FindObject(DT3V,0,0,0,0,0,"Sprenger",Local(9)),"NewPosSet",Sum(GetX(),Sin(Local(8),25)),Sum(GetY(),Mul(-1,Cos(Local(8),25)))),And(ObjectCall(CreateObject(EY3V,Sin(Local(8),25),Sum(Mul(-1,Cos(Local(8),25)),20),GetOwner()),"Launch2",50,Div(GetComponent(BOMB,0,Local(9)),12),Local(9),Local(8)),SetComponent(BOMB,Sub(GetComponent(BOMB,0,Local(9)),1),Local(9))));
  if(LessThan(GetComponent(BOMB,0,Local(9)),1)) return(ObjectCall(Local(9),"Finished"));
  if(PathFree(GetX(),GetY(),Sum(GetX(),Sin(Sum(Local(8),45),25)),Sum(GetY(),Mul(-1,Cos(Sum(Local(8),45),25)))))
  if(Not(FindObject(EY3V,Sub(Sum(GetX(),Sin(Sum(Local(8),45),25)),30),Sub(Sum(GetY(),Mul(-1,Cos(Sum(Local(8),45),25))),30),60,60)))
  if(Not(ObjectCall(FindObject(DT3V,0,0,0,0,0,"Sprenger",Local(9)),"CheckExplosion",Sum(GetX(),Sin(Sum(Local(8),45),25)),Sum(GetY(),Mul(-1,Cos(Sum(Local(8),45),25))))))
  And(ObjectCall(FindObject(DT3V,0,0,0,0,0,"Sprenger",Local(9)),"NewPosSet",Sum(GetX(),Sin(Sum(Local(8),45),25)),Sum(GetY(),Mul(-1,Cos(Sum(Local(8),45),25)))),And(ObjectCall(CreateObject(EY3V,Sin(Sum(Local(8),45),25),Sum(Mul(-1,Cos(Sum(Local(8),45),25)),20),GetOwner()),"Launch2",50,Div(GetComponent(BOMB,0,Local(9)),12),Local(9),Sum(Local(8),45)),SetComponent(BOMB,Sub(GetComponent(BOMB,0,Local(9)),1),Local(9))));
  return(1);
