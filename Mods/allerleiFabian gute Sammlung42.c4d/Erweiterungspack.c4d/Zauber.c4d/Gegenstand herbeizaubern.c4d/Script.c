/*-- Gegenstand herbeizaubern --*/

#strict 3

public func Activate(pCaster,pRealcaster)
{
  var Typ,idObject,i,pMagic;
  Sound("Magic1");
  while(Typ=GetDefinition(S42MigrationStep(i, 1, true),C4D_Object))
   if(GetDefCoreVal("Collectible","DefCore",Typ))
    if(!Random(i)) idObject= Typ;
  if(!idObject) return(Message("$Nothing$",pCaster));
  SetPosition(GetX(),(GetY() ?? 0)+ 8,CreateObject(idObject,nil,nil,GetOwner()));
  Message(GetName(nil,idObject),pCaster);
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("4"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)