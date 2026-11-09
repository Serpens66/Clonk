/*-- Blend --*/

#strict
local a,target,id;

Hit:
 SetRDir(0);
 Local(0)=1;
 Sound("SGRN_Hit*.ogg");
 Sound("ct_fireinhole");
 return(1);

protected Blend:
 if(Local(0)==1)++Local(1);
 if(Local(1) > 72)   // erst ca 72 Frames nach aufschlag blenden
 {
  Sound("flashbang_explode2");
  CastParticles("Lichtlein",100,150,0,0,80,80,RGBa(20,20,20,0),RGBa(20,20,20,0)); 
 // for(var i = 0; i < GetPlayerCount(); i++)           // bei jedem Spieler "blend" im aktiven Clonk aufrufen.
 // ObjectCall(GetCursor(GetPlayerByIndex(i)),"Blend",GetX(this()),GetY(this()));
for(var i1=0; i1<GetPlayerCount(); i1++) {             // bei allen Clonks der Spieler aufrufen
 for(var i2=0; i2<GetCrewCount(i1); i2++) {
  ObjectCall(GetCrew(GetPlayerByIndex(i1),i2),"Blend",GetX(this()),GetY(this()));}}
  RemoveObject(this());
 }
 return(1);
 