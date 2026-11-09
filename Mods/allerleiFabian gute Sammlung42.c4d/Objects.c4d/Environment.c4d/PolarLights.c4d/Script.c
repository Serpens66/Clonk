/*-- Nordlichter --*/

#strict 3

protected func Initialize() {
  SetAction("NorthernLights");
  SetPosition(50,70);
}
  
private func NorthernLights() {    
  // Nordlichter
  if (IsNight()) 
    if (!Random(20)) 
      PlaceNorthernLight();
}
 
private func PlaceNorthernLight() {
  var iRand;
  CreateObject(NTLT,iRand=Random(LandscapeWidth()),Random((GetHorizonHeight(iRand) ?? 0)-40),-1);
}
  
private func GetHorizonHeight(iX) {
  var i=nil;
  while(And((i ?? 0)<(LandscapeHeight() ?? 0), !GBackSemiSolid(iX,i))) S42MigrationAssign(i, "+=", 10);
  return(i);
}

// braucht kein Destruction, da die Nordlicher von alleine verschwinden und dann einfach keine neuen mehr erstellt werden

// Kann mittels des neuen Spielzielauswählers
public func IsVariabelChooseable2() { return(1); }
