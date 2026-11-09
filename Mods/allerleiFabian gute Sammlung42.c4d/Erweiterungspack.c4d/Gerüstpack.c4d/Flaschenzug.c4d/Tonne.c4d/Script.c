/*-- Tonne --*/

#strict

protected func Initialize()
{
  // Damit Fahrzeuge vor der Tonne zu sehen sind.
  SetCategory(2);
}

protected func Collection2(pObj,fPut)
{
  Sound("WoodHit*");
  SetEntrance(0);
}

protected func ActivateEntrance(pObj)
{
  // Hinauslassen
  if(Contained(pObj)== this()) return(SetEntrance(1));
  // Maxi. 1 Clonk aufnehmen
  if(GetOCF(pObj) & OCF_CrewMember())
   if(!FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this()))
    return(SetEntrance(1));
   else
    return(Message("$OnlyOneClonk$",this()));
  // Nur Fahrzeuge aufnehmen!
  if(GetCategory(pObj) & C4D_Vehicle())
   if(!RejectCollect(GetID(pObj),pObj))
    return(SetEntrance(1));
}

protected func RejectCollect(idObj,pObj)
{
  // Fahrzeuge nur aufnehmen wenn die Tonne leer ist!
  if(GetCategory(pObj) & C4D_Vehicle())
   if(Contents())
    if(!(GetOCF(Contents()) & OCF_CrewMember()))
     return(1,Message("$MustBeEmpty$",this()));
  // Gegenstände nur aufnehmen wenn weniger als 60 Objekte und kein Fahrzeug im Inhalt
  if(ContentsCount()< 60)
   if(!FindContainedVehicle())
    return(0);
  // Keine Aufnahme!
  if(Abs(GetXDir(pObj))>6) SetYDir(-5,pObj);
  return(1,Message("$Full$",this()));
}

protected func Ejection(pObj) { SetEntrance(0); }

protected func Entrance(pContainer)
{
  // In der Heimatbasis entleeren
  if(GetBase(pContainer)== -1) return(0);
  while(Contents()) Enter(pContainer,Contents());
}

public func FindContainedVehicle()
{
  var i= ContentsCount();
  while(i--)
   if(GetCategory(Contents(i)) & C4D_Vehicle()) 
    return(Contents(i));
  return(0);
}

protected func ControlUp(pClonk)
{
  var obj;
  if(!(obj= FindObject(FHKN,-20,-20,40,40,0,0,0,NoContainer()))) return(0);
  if((obj-> FindAttachedContainer()) && ((obj->FindAttachedContainer()) != this())) return(0);
  CreateMenu(GetID(),pClonk);
  if(obj-> FindAttachedContainer()) return(AddMenuItem("$Unhook$","Haken",FHKN,pClonk,0,obj));
  return(AddMenuItem("$Hook$","Haken",FHKN,pClonk,0,obj));
}

public func Haken(idItem,pHaken)
{
  pHaken->~ UseTool(this());
}

// Position des VertexY für das Tool
public func GetToolVertexY() { return(+27); }

public func IsContainer() { return(1); }

public func IsLorry() { return(1); }