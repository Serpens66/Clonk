/*-- Schlackekasten - Müllpaketgrundlage --*/

#strict 3

local iUsedItems;

public func UnpackTo() { return(SHLK); }
public func IsWaste() { return((PackCount() ?? 0)*((UnpackTo()->~IsWaste()) ?? 0)); }
public func MaxPackCount() { return(10); }
public func IsPack() { return(1); }

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




protected func Initialize()
{
  if(FindObject(NOWS)) {RemoveObject();return;} // kein Müll, dann sofort wieder entfernen
  // Bild anpassen
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

/* Aufschlag */

protected func Hit()
{
  Sound("RockHit*");
}

/* Paketverwaltung */

protected func Activate(pClonk)
{
  [Zerlegen]
  return (Unpack(pClonk));
}

public func Unpack(pTo)
{
  if(!pTo) pTo = Contained(this());
  // Effekt zum Deaktivieren des Objektpackens
  AddEffect("IntNoPacking", pTo, 1);
  // Objekte erzeugen
  for(var i = nil; (i ?? 0) < (PackCount() ?? 0); S42MigrationStep(i, 1, true))
    CreateContents(UnpackTo(), pTo);
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", pTo);
  // Fertig
  RemoveObject();
  return(1);
}

public func GetItem()
{
  // Effekt zum Deaktivieren des Objektpackens
  AddEffect("IntNoPacking", Contained(), 1);
  // Objekt ausgeben
  var obj = CreateContents(UnpackTo(), Contained());
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", Contained());
  // Einer weniger
  DoPackCount(-1);
  return(obj);
}

public func SplitPack(iAmount)
{
  // Nur wenn überhaut so viele Teile da sind
  if((iAmount ?? 0) >= (MaxPackCount() ?? 0)-(iUsedItems ?? 0)) return(nil);
  // 0 Absplitten? nee
  if(!iAmount) return(nil);
  // Vom Packet werden iAmount Teile abgetrennt und in ein neues Pack gesteckt
  var pSplit = CreateObject(GetID(), nil, nil, GetOwner());
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
  // In vorhandene Objektpakete integrieren
  var obj, iChange, iAmount;
  // Alle gleichen Pakete im Clonk durchsuchen
  for(obj in FindObjects(Find_Exclude(this()), Find_Container(pClonk), Find_ID(GetID())))
  {
    iAmount = PackCount();
    // Wie viel Platz ist im neuen Paket?
    iChange = (MaxPackCount() ?? 0) - (obj->PackCount() ?? 0);
    // Wir können aber höchstens so viele abgeben wie wir haben
    if((iChange ?? 0)>(iAmount ?? 0)) iChange = iAmount;
    // Objekte abgeben
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
  
  if((PackCount() ?? 0)<2)
    return SetGraphics("0");
  if((PackCount() ?? 0)<4)
    return SetGraphics("25");
  if((PackCount() ?? 0)<7)
    return SetGraphics("50");
  if((PackCount() ?? 0)<9)
    return SetGraphics("75");
  return SetGraphics("");
}

/* Masse anpassen */
public func UpdateMass()
{
  SetMass(Max((GetMass(nil, GetID()) ?? 0) * (PackCount() ?? 0) / (MaxPackCount() ?? 0), 1));
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
  // Nicht volle Objektpakete werden zunächst nur zu einem Objekte Heimatbasismaterial verkauft
  // Im Sale-Callback werden zusätzliche Objekte angerechnet und ggf. HomebaseMaterial zusammengefasst
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
  var cnt = PackCount(), cntpack = MaxPackCount(), idWaste;
  if (Equal(cnt, cntpack)) return;
  // Ansonsten wurde nur zu einem Objekt dem Heimatbasismaterial angerechnet. Zusätzliche Objekte ins Heimatbasismaterial
  idWaste = UnpackTo();
  if ((cnt ?? 0) > 1) DoHomebaseMaterial(iPlr, idWaste, (cnt ?? 0)-1);
  // Und Heimatbasispfeile zu Paketen zusammenfassen
  if (cntpack)
    {
    cnt = (GetHomebaseMaterial(iPlr, idWaste) ?? 0) / (cntpack ?? 0);
    if (cnt)
      {
      DoHomebaseMaterial(iPlr, idWaste, -(cnt ?? 0)*(cntpack ?? 0));
      DoHomebaseMaterial(iPlr, GetID(), cnt);
      }
    }
}
