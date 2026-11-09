/*-- Blend --*/

#strict 3
local a,target,id;

func Hit(...) {
 SetRDir(nil);
 Local(nil)=1;
 Sound("SGRN_Hit*.ogg");
 Sound("ct_fireinhole");
 return(1);
}

protected func Blend(...) {
 if(Equal(Local(nil), 1))S42MigrationStep(Local(1), 1);
 if((Local(1) ?? 0) > 72)   // erst ca 72 Frames nach aufschlag blenden
 {
  Sound("flashbang_explode2");
  CastParticles("Lichtlein",100,150,nil,nil,80,80,RGBa(20,20,20,nil),RGBa(20,20,20,nil));
 // for(var i = 0; i < GetPlayerCount(); i++)           // bei jedem Spieler "blend" im aktiven Clonk aufrufen.
 // ObjectCall(GetCursor(GetPlayerByIndex(i)),"Blend",GetX(this()),GetY(this()));
for(var i1=nil; (i1 ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i1, 1, true)) {             // bei allen Clonks der Spieler aufrufen
 for(var i2=nil; (i2 ?? 0)<(GetCrewCount(i1) ?? 0); S42MigrationStep(i2, 1, true)) {
  ObjectCall(GetCrew(GetPlayerByIndex(i1),i2),"Blend",GetX(this()),GetY(this()));}}
  RemoveObject(this());
 }
 return(1);
}
 