/*--- Fackel ---*/

#strict

public Activate:
  var gbaeude, obj;
gbaeude=FindObject2(Find_AtRect(-3,-3,6,6), Find_NoContainer(),Find_Func("GetDefCoreVal", "Construction", "DefCore"));

// neuerdings in himmel als "standfackel" aufstellen (andere grafik)
/*
  //Nich im Himmel (außer an Gebäuden)
  if(GetMaterial()!=Material("Tunnel") && !(gbaeude))
      return(Sound("CommandFailure1.wav"),Message("Anbringen hier nicht möglich",this()));
*/

  //Nich im Entrancebereich von Gebäuden
  if(GetOCF(gbaeude)&OCF_Entrance()) if(InEntrance(gbaeude))
      return(Sound("CommandFailure1.wav"),Message("Anbringen nicht an Eingängen",this()));

  //Nich in Flüssigkeiten
  if(InLiquid(Par()))
      return(Sound("CommandFailure1.wav"),Message("Anbringen nicht in Flüssigkeiten",this()));
  Sound("Connect");

  //Wenn Himmelhintergrund dann standfackel
  // if(GetMaterial()!=Material("Tunnel") && !(gbaeude))
  ObjectSetAction(obj=CreateObject(TOR3,0,12,GetOwner()),"Burning",gbaeude);   // überall die standfackel machen, denn die ist beweglich und daher viel praktischer
// else  
// {ObjectSetAction(obj=CreateObject(TORC,0,12,GetOwner()),"Burning",gbaeude);} // fest montierte Fackel
  SetDir(Random(2)); if(gbaeude) SetDir(GetX(obj)>GetX(gbaeude),obj);

  RemoveObject();
  return(1);



private func InEntrance(gbaeude) {
  if(Inside(GetX(),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),2)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0) ))
      if(Inside(GetY(),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),3)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1) ))
          return(1);}