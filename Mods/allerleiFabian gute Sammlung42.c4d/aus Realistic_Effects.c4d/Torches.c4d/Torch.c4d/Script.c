/*--- Fackel ---*/

#strict 3

public func Activate(...) {
  var gbaeude, obj;
gbaeude=FindObject2(Find_AtRect(-3,-3,6,6), Find_NoContainer(),Find_Func("GetDefCoreVal", "Construction", "DefCore"));

// neuerdings in himmel als "standfackel" aufstellen (andere grafik)
/*
  //Nich im Himmel (außer an Gebäuden)
  if(GetMaterial()!=Material("Tunnel") && !(gbaeude))
      return(Sound("CommandFailure1.wav"),Message("Anbringen hier nicht möglich",this()));
*/

  //Nich im Entrancebereich von Gebäuden
  if((GetOCF(gbaeude) ?? 0)&(OCF_Entrance ?? 0)) if(InEntrance(gbaeude))
      return(S42MigrationFirst(Sound("CommandFailure1.wav"),Message("Anbringen nicht an Eingängen",this())));

  //Nich in Flüssigkeiten
  if(InLiquid(Par()))
      return(S42MigrationFirst(Sound("CommandFailure1.wav"),Message("Anbringen nicht in Flüssigkeiten",this())));
  Sound("Connect");

  //Wenn Himmelhintergrund dann standfackel
  // if(GetMaterial()!=Material("Tunnel") && !(gbaeude))
  ObjectSetAction(obj=CreateObject(TOR3,nil,12,GetOwner()),"Burning",gbaeude);   // überall die standfackel machen, denn die ist beweglich und daher viel praktischer
// else  
// {ObjectSetAction(obj=CreateObject(TORC,0,12,GetOwner()),"Burning",gbaeude);} // fest montierte Fackel
  SetDir(Random(2)); if(gbaeude) SetDir((GetX(obj) ?? 0)>(GetX(gbaeude) ?? 0),obj);

  RemoveObject();
  return(1);
}



private func InEntrance(gbaeude) {
  if(Inside(GetX(),
    (GetX(gbaeude) ?? 0)+(GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),nil) ?? 0),
    (GetX(gbaeude) ?? 0)+(GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),2) ?? 0)+(GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),nil) ?? 0) ))
      if(Inside(GetY(),
        (GetY(gbaeude) ?? 0)+(GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1) ?? 0),
        (GetY(gbaeude) ?? 0)+(GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),3) ?? 0)+(GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1) ?? 0) ))
          return(1);}