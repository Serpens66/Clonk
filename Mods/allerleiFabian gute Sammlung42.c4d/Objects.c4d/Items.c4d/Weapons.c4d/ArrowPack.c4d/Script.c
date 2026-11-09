/*-- Pfeilpaket --*/

#strict 3

/* Initialisierung */

protected func Initialize()
{
  // Bild anpassen
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

/* Aufschlag */

protected func Hit()
{
  Sound("WoodHit");
}

/* Paketeigenschaften */

public func UnpackTo() { return(ARRW); }
public func IsArrowPack() { return(1); }
public func MaxPackCount() { return(10); }
public func IsAnvilProduct() { return(1); }

/* Packet Zerlegung */
public func Unpack(pTo)
{
  if(!pTo) pTo = Contained(this());
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking", pTo, 1);
  // Pfeile erzeugen
  for(var i = nil; (i ?? 0) < (PackCount() ?? 0); S42MigrationStep(i, 1, true))
    CreateContents(UnpackTo(), pTo);
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", pTo);
  // Fertig
  RemoveObject();
  return(1);
}

/* Manuelle Zerlegung */

protected func Activate(pClonk)
{
  [$TxtUnpack$]
  return (Unpack(pClonk));
}

/* Pfeilverwaltung */
local iUsedItems;

public func PackCount() { return((MaxPackCount() ?? 0)-(LocalN("iUsedItems") ?? 0)); }

public func SetPackCount(iAmount) { iUsedItems = (PackCount() ?? 0)-(iAmount ?? 0); UpdatePicture(); UpdateMass(); }

public func DoPackCount(iChange) 
{
  S42MigrationAssign(iUsedItems, "-=", (iChange ?? 0));
  if((PackCount() ?? 0)<=0) return(RemoveObject());
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

public func GetItem()
{
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking", Contained(), 1);
  // Pfeil ausgeben
  var obj = CreateContents(UnpackTo(), Contained());
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", Contained());
  // Einer weniger
  DoPackCount(-1);
  return(obj);
}

public func SplitPack(iAmount, pClonk)
{
  // Nur wenn überhaut so viele Teile da sind
  if((iAmount ?? 0) >= (MaxPackCount() ?? 0)-(iUsedItems ?? 0)) return(nil);
  // 0 Absplitten? nee
  if(!iAmount) return(nil);
  // Vom Packet werden iAmount Teile abgetrennt und in ein neues Pack gesteckt
  var pSplit = CreateObject(GetID(), nil,(GetDefBottom() ?? 0)-(GetY() ?? 0), GetOwner());
  pSplit->SetPackCount(iAmount);
  DoPackCount(-(iAmount ?? 0));
  return(1);
}

public func IsNotFull() { return((MaxPackCount() ?? 0) > (PackCount() ?? 0)); }

/* Einsammeln */
public func Entrance(pClonk)
{
  // In vorhandene Packte integrieren
  if(JoinPack(pClonk)) return(1);
  // Grafik anpassen
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

public func JoinPack(pClonk)
{
  // In vorhandene Pfeilpakete integrieren
  var obj, iChange, iAmount;
  // Alle gleichen Pakete im Clonk durchsuchen
  for(obj in FindObjects(Find_Exclude(this()), Find_Container(pClonk), Find_ID(GetID()))) 
  {
    iAmount = PackCount();
    // Wie viel Platz ist im neuen Paket?
    iChange = (MaxPackCount() ?? 0) - (obj->PackCount() ?? 0);
    // Wir können aber höchstens so viele abgeben wie wir haben
    if((iChange ?? 0)>(iAmount ?? 0)) iChange = iAmount;
    // Pfeile abgeben
    obj->DoPackCount(iChange);
    // Wenn das Paket leer ist aufhören
    if((iAmount ?? 0)-(iChange ?? 0)<=0) return(RemoveObject());
    DoPackCount(-(iChange ?? 0));
  }
  return;
}

/* Grafik anpassen */
public func UpdatePicture()
{
  if((PackCount() ?? 0)>9)
  {
    SetGraphics(nil,nil,GetNumberID((PackCount() ?? 0)/10),1,GFXOV_MODE_Picture);
    SetObjDrawTransform(400,nil,-12000,nil,400,+10000, this(), 1);
  }
  else SetGraphics(nil,nil,nil,1,nil);

  SetGraphics(nil,nil,GetNumberID(Mod(PackCount(),10)),2,GFXOV_MODE_Picture);
  SetObjDrawTransform(400,nil,-5000,nil,400,+10000, this(), 2);
}

/* Masse anpassen */
public func UpdateMass()
{
  SetMass((GetMass(nil,UnpackTo()) ?? 0)*(Max(PackCount(),1) ?? 0));
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}

/* Wert */

public func CalcValue(object pInBase, int iForPlayer)
{
  // Je nach Anzahl
  iForPlayer ??= 0;
  return((GetValue(nil, GetID()) ?? 0)*(PackCount() ?? 0)/(MaxPackCount() ?? 0));
}

protected func SellTo(int iPlr)
{
  // Nicht volle Pfeilpakete werden zunächst nur zu einem Pfeile Heimatbasismaterial verkauft
  // Im Sale-Callback werden zusätzliche Pfeile angerechnet und ggf. HomebaseMaterial zusammengefasst
  iPlr ??= 0;
  if ((PackCount() ?? 0) < (MaxPackCount() ?? 0)) return UnpackTo();
  // Volle Pakete sind kein Problem
  return GetID();
}
  
protected func Sale(int iPlr)
{
  // Heimatbasismaterial anpassen
  // Paket war voll? Dann stimmt der normale Verkauf
  iPlr ??= 0;
  var cnt = PackCount(), cntpack = MaxPackCount(), idArrow;
  if (Equal(cnt, cntpack)) return;
  // Ansonsten wurde nur zu einem Pfeil dem Heimatbasismaterial angerechnet. Zusätzliche Pfeile ins Heimatbasismaterial
  idArrow = UnpackTo();
  if ((cnt ?? 0) > 1) DoHomebaseMaterial(iPlr, idArrow, (cnt ?? 0)-1);
  // Und Heimatbasispfeile zu Paketen zusammenfassen
  if (cntpack)
    {
    cnt = (GetHomebaseMaterial(iPlr, idArrow) ?? 0) / (cntpack ?? 0);
    if (cnt)
      {
      DoHomebaseMaterial(iPlr, idArrow, -(cnt ?? 0)*(cntpack ?? 0));
      DoHomebaseMaterial(iPlr, GetID(), cnt);
      }
    }
}
