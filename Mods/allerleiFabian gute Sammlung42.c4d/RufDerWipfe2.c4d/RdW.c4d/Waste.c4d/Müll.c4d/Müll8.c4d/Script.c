/*-- Schlackekasten - Müllpaketgrundlage --*/

#strict

local iUsedItems;

public func UnpackTo() { return(SHLK); }
public func IsWaste() { return(PackCount()*(UnpackTo()->~IsWaste())); }
public func MaxPackCount() { return(10); }
public func IsPack() { return(1); }

public func PackCount() { return(MaxPackCount()-LocalN("iUsedItems")); }

public func SetPackCount(iAmount) { iUsedItems = PackCount()-iAmount; UpdatePicture(); UpdateMass(); }

public func DoPackCount(iChange)
{
  iUsedItems-=iChange;
  if(PackCount()<=0) return(RemoveObject());
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}




protected func Initialize()
{
  if(FindObject(NOWS)) {RemoveObject();return();} // kein Müll, dann sofort wieder entfernen
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
  for(var i = 0; i < PackCount(); i++)
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
  if(iAmount >= MaxPackCount()-iUsedItems) return(0);
  // 0 Absplitten? nee
  if(!iAmount) return(0);
  // Vom Packet werden iAmount Teile abgetrennt und in ein neues Pack gesteckt
  var pSplit = CreateObject(GetID(), 0, 0, GetOwner());
  pSplit->SetPackCount(iAmount);
  DoPackCount(-iAmount);
  return(1);
}

public func IsNotFull() { return(MaxPackCount() > PackCount()); }

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
    iChange = MaxPackCount() - obj->PackCount();
    // Wir können aber höchstens so viele abgeben wie wir haben
    if(iChange>iAmount) iChange = iAmount;
    // Objekte abgeben
    obj->DoPackCount(iChange);
    // Wenn das Paket leer ist aufhören
    if(iAmount-iChange<=0) return(RemoveObject());
    DoPackCount(-iChange);
  }
  return();
}

/* Grafik anpassen */
public func UpdatePicture()
{
  if(PackCount()>9)
  {
    SetGraphics(0,0,GetNumberID(PackCount()/10),1,GFXOV_MODE_Picture);
    SetObjDrawTransform(400,0,-12000,0,400,+10000, this(), 1);
  }
  else SetGraphics(0,0,0,1,0);

  SetGraphics(0,0,GetNumberID(Mod(PackCount(),10)),2,GFXOV_MODE_Picture);
  SetObjDrawTransform(400,0,-5000,0,400,+10000, this(), 2);
  
  if(PackCount()<2)
    return SetGraphics("0");
  if(PackCount()<4)
    return SetGraphics("25");
  if(PackCount()<7)
    return SetGraphics("50");
  if(PackCount()<9)
    return SetGraphics("75");
  return SetGraphics("");
}

/* Masse anpassen */
public func UpdateMass()
{
  SetMass(Max(GetMass(0, GetID()) * PackCount() / MaxPackCount(), 1));
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
  return(GetValue(0, GetID())*PackCount()/MaxPackCount());
}

protected func SellTo(int iPlr)
{
  // Nicht volle Objektpakete werden zunächst nur zu einem Objekte Heimatbasismaterial verkauft
  // Im Sale-Callback werden zusätzliche Objekte angerechnet und ggf. HomebaseMaterial zusammengefasst
  if (PackCount() < MaxPackCount()) return UnpackTo();
  // Volle Pakete sind kein Problem
  return GetID();
}

protected func Sale(int iPlr)
{
  // Heimatbasismaterial anpassen
  // Paket war voll? Dann stimmt der normale Verkauf
  var cnt = PackCount(), cntpack = MaxPackCount(), idWaste;
  if (cnt == cntpack) return();
  // Ansonsten wurde nur zu einem Objekt dem Heimatbasismaterial angerechnet. Zusätzliche Objekte ins Heimatbasismaterial
  idWaste = UnpackTo();
  if (cnt > 1) DoHomebaseMaterial(iPlr, idWaste, cnt-1);
  // Und Heimatbasispfeile zu Paketen zusammenfassen
  if (cntpack)
    {
    cnt = GetHomebaseMaterial(iPlr, idWaste) / cntpack;
    if (cnt)
      {
      DoHomebaseMaterial(iPlr, idWaste, -cnt*cntpack);
      DoHomebaseMaterial(iPlr, GetID(), cnt);
      }
    }
}
