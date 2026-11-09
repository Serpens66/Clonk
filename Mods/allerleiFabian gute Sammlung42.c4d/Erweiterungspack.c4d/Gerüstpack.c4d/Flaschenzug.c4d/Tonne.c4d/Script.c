/*-- Tonne --*/

#strict 3

protected func Initialize()
{
  // Damit Fahrzeuge vor der Tonne zu sehen sind.
  SetCategory(2);
}

protected func Collection2(pObj,fPut)
{
  Sound("WoodHit*");
  SetEntrance(nil);
}

protected func ActivateEntrance(pObj)
{
  // Hinauslassen
  if(Equal(Contained(pObj), this())) return(SetEntrance(1));
  // Maxi. 1 Clonk aufnehmen
  if((GetOCF(pObj) ?? 0) & (OCF_CrewMember ?? 0))
   if(!FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this()))
    return(SetEntrance(1));
   else
    return(Message("$OnlyOneClonk$",this()));
  // Nur Fahrzeuge aufnehmen!
  if((GetCategory(pObj) ?? 0) & (C4D_Vehicle ?? 0))
   if(!RejectCollect(GetID(pObj),pObj))
    return(SetEntrance(1));
}

protected func RejectCollect(idObj,pObj)
{
  // Fahrzeuge nur aufnehmen wenn die Tonne leer ist!
  if((GetCategory(pObj) ?? 0) & (C4D_Vehicle ?? 0))
   if(Contents())
    if(!((GetOCF(Contents()) ?? 0) & (OCF_CrewMember ?? 0)))
     return(S42MigrationFirst(1,Message("$MustBeEmpty$",this())));
  // Gegenstände nur aufnehmen wenn weniger als 60 Objekte und kein Fahrzeug im Inhalt
  if((ContentsCount() ?? 0)< 60)
   if(!FindContainedVehicle())
    return(nil);
  // Keine Aufnahme!
  if((Abs(GetXDir(pObj)) ?? 0)>6) SetYDir(-5,pObj);
  return(S42MigrationFirst(1,Message("$Full$",this())));
}

protected func Ejection(pObj) { SetEntrance(nil); }

protected func Entrance(pContainer)
{
  // In der Heimatbasis entleeren
  if(Equal(GetBase(pContainer), -1)) return(nil);
  while(Contents()) Enter(pContainer,Contents());
}

public func FindContainedVehicle()
{
  var i= ContentsCount();
  while(S42MigrationStep(i, -1, true))
   if((GetCategory(Contents(i)) ?? 0) & (C4D_Vehicle ?? 0))
    return(Contents(i));
  return(nil);
}

protected func ControlUp(pClonk)
{
  var obj;
  if(!(obj= FindObject(FHKN,-20,-20,40,40,nil,nil,nil,NoContainer()))) return(nil);
  if(And((obj-> FindAttachedContainer()), (!Equal((obj->FindAttachedContainer()), this())))) return(nil);
  CreateMenu(GetID(),pClonk);
  if(obj-> FindAttachedContainer()) return(AddMenuItem("$Unhook$","Haken",FHKN,pClonk,nil,obj));
  return(AddMenuItem("$Hook$","Haken",FHKN,pClonk,nil,obj));
}

public func Haken(idItem,pHaken)
{
  pHaken->~ UseTool(this());
}

// Position des VertexY für das Tool
public func GetToolVertexY() { return(+27); }

public func IsContainer() { return(1); }

public func IsLorry() { return(1); }