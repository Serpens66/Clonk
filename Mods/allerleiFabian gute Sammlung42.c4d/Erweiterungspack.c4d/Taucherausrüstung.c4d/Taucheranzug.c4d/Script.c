/*-- Taucheranzug --*/

#strict

public func Activate(pClonk)
{ 
[$PutOn$|Image=TAAZ]
// if(GetID(pClonk)== HZCK)
// return(PlayerMessage(GetOwner(pClonk),"Nur von unspezialisierten Clonks verwendbar",pClonk));
  
  SetComDir(COMD_Stop(),pClonk);
  if(pClonk->~IsPinguin()==2)   
   return(PlayerMessage(GetOwner(pClonk),"$AlreadyHasSuit$",pClonk,GetName(pClonk)));
  if(pClonk->~HasGear(GEAR_Armor))
   return(PlayerMessage(GetOwner(pClonk),"Trägt bereits eine Rüstung",pClonk));

  // var old_ID= GetID(pClonk);             // das ganze ohne neues Clonkobjekt machen!
  // if(!ObjectCall(pClonk,"Redefine2",TACL))
    // {
    // ChangeDef(TACL,pClonk);
    // ObjectSetAction(pClonk,"Walk");
    // }
  // LocalN("idClonk",pClonk)= old_ID;
  // LocalN("iOwnerofSuit",pClonk)= GetOwner();
  
  pClonk->Pinguinan(2); // sorgt fuer arbeiten unter wasser. Mit dem Anzug geht das. und mit dem Helm bekommt er dann halt luft ueber ne pumpe
  SetColorDw(RGB(10,10,60),pClonk);
  Sound("PutItOn");
  PlayerMessage(GetOwner(pClonk),"$PutOnSuit$",pClonk);
  RemoveObject();
  return(1);
}

protected func IsAnvilProduct() { return(1); }