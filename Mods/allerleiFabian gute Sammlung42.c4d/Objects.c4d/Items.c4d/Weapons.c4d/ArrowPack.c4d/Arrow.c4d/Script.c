/*-- Pfeil --*/

#strict 3

local lastX,lastY,estX,estY;

/* Pfeileigenschaften */
public func PackTo() { return(ARWP); }
public func IsArrow() { return(1); }
public func ArrowStrength() { return(10); }
public func ArrowTumbleRandom() {return(2);}
public func ArrowStopsOnHit() { return(1); }

/* Produkteigenschaften */

// Abwärtskompatibilität. Neuerdings ist der Pfeil eh kein Bauplan mehr
// und kann daher so oder so nicht mehr hergestellt werden.
public func IsAnvilProduct() { return(1); }

/* Wird abgefeuert */
public func Launch()
{
  SetAction("Shot");

  // Es gibt jetzt eine extra Trefferpruefung
  SetCategory(C4D_Vehicle,this());
}

/* Aufschlag */
protected func Hit(int oldX, int oldY)
{
  oldX ??= 0;
  oldY ??= 0;
  if(!!SEqual(GetAction(), "Shot"))
  {
   // Damit es zu keiner Endlosrekursion kommt wenn InFlight nochmal Hit()
   // aufruft (z.B. Golem, siehe auch #2104) wird die Action hier zurückgesetzt
   // bevor InFlight() aufgerufen wird.
   SetAction("Idle");
   
   // Wenn geschossen noch einmal die Ueberpruefung durchfuehren
   InFlight();
   
   // Kurzzeitiges Stecken in festen Dingen
   var angle=(Angle(GetX(),GetY(),(GetX() ?? 0)+(oldX ?? 0),(GetY() ?? 0)+(oldY ?? 0)) ?? 0)-90;
   var iX=Cos(angle,5);
   var iY=Sin(angle,5);
   if(GBackSolid(iX,iY))
   {    
    SetVertex(2,nil,Cos((angle ?? 0)-(GetR() ?? 0),7),this(),1);
    SetVertex(2,1,Sin((angle ?? 0)-(GetR() ?? 0),7),this(),1);
   
    SetSpeed();
    SetRDir();
    AddEffect("IntStickInMaterial",this(),1,10,nil,GetID());
   }
  }
  
  // Wieder zu einem Objekt machen
  if(!((GetCategory() ?? 0) & (C4D_Object ?? 0)))
   SetCategory(C4D_Object,this());
}

/* Effekt fuers Stecken in Material */

public func FxIntStickInMaterialStart()
{
 return(1);
}

public func FxIntStickInMaterialTimer(pTarget,iEffectNumber,iEffectTime)
{
 if(Or((iEffectTime ?? 0) > 36*5, Contained(pTarget)))
 {
  SetVertex(2,nil,GetDefCoreVal("VertexX","DefCore",GetID(pTarget),2),pTarget,1);
  SetVertex(2,1,GetDefCoreVal("VertexY","DefCore",GetID(pTarget),2),pTarget,1);
  return(-1);
 }
 
 return(1);
}

/* Effekt fuer Stillstand */

func FxPendingToRemoveTimer(pTarget,iEffectNumber,iEffectTime)
{
 // Pfeil bewegt sich wieder?
 if(Or(GetXDir(pTarget), GetYDir(pTarget)))return -1;
 
 // Pfeiltrefferueberpruefung entfernen (nach knapp einer Sekunde)?
 if((iEffectTime ?? 0) > 36*1)
 {
  pTarget->SetSpeed(nil,nil);
  if(!!SEqual(GetAction(pTarget), "Shot"))
   pTarget->SetAction("Idle");
  return -1;
 }
}

/* Treffer */
public func HitTarget(pObject)
{
  Sound("ArrowHit");
  
  // Ruestungsritter zB wollen nicht immer getroffen werden
  if(pObject->~QueryCatchBlow(this()))return(nil);
  
  // Ziel wird getroffen
  pObject->~OnArrowHit(this());
  
  if(GetAlive(pObject))
  {
   var iDmg=((ArrowStrength() ?? 0)*((Max(Abs(GetXDir()),Abs(GetYDir())) ?? 0)+1))/50;
   DoEnergy(-(iDmg ?? 0),pObject);

   // Objekt wurde getroffen. Damit Schild des Ritters noch vor Pfeilen schuetzt
   if(And(pObject, GetAlive(pObject)))
    pObject->~CatchBlow(-(iDmg ?? 0),this());

   // Ziel koennte irgendwas mit dem Pfeil gemacht haben
   if(!this())
    return;

   // Schleudern nur einmal / Sekunde erlaubt
   var bTumble=true;
   var iEffect;
   if(pObject)
   for(var i=nil;iEffect=GetEffect("IntNoArrowTumble",pObject,i);S42MigrationStep(i, 1, true))
   {
    if(!Equal(EffectVar(nil,pObject,GetEffect("IntNoArrowTumble",pObject,i)), this()))continue;
    bTumble=nil;
    break;
   }

   // Zielobjekt auch sichtbar treffen
   if(pObject)
   if(bTumble)
   if(!Random(ArrowTumbleRandom()))
   {
    var xDir=((GetXDir(this()) ?? 0)/3);
    var yDir=((GetYDir(this()) ?? 0)/3);
    
    if(GetAlive(pObject))
    {
     pObject->SetAction("Tumble");
     AddEffect("IntNoArrowTumble",pObject,1,30,nil,ARRW,this());
    } 
    pObject->SetSpeed((GetXDir(pObject) ?? 0)+(xDir ?? 0),(GetYDir(pObject) ?? 0)+(yDir ?? 0)-1);
   }
  }

  // Wenn sich das Objekt nicht entfernt hat noch festhaengen (Zielscheibe zB)
  if(pObject)
  if(pObject->~IsArrowTarget(this()))
  {
   SetXDir(nil); SetYDir(nil); SetRDir(nil);
   Local(2) = pObject;
   Local(3) = (GetX() ?? 0) - (GetX(pObject) ?? 0);
   Local(4) = (GetY() ?? 0) - (GetY(pObject) ?? 0);
   SetAction("Stick");
  }

  // Pfeil stoppen?
  if(ArrowStopsOnHit())
  {
   SetSpeed(GetXDir(pObject),GetYDir(pObject));
   if(!!SEqual(GetAction(), "Shot"))
    SetAction("Idle");
   // Nicht sofort einsammeln
   AddEffect("CannotBeCollected",this(),1,36*2,nil,ARRW);
  }

 
 return(1);
}

/* Direkt nach dem Treffer nicht einsammelbar */

public func FxCannotBeCollectedStart()
{
 return(1);
}

public func FxCannotBeCollectedTimer()
{
 return(-1);
}

/* Effekt gegen mehrfach wegschleudern */

public func FxIntNoArrowTumbleStart(pTarget,iEffectNumber,temp,par1)
{
 if(temp)return;
 EffectVar(nil,pTarget,iEffectNumber)=par1;
 return(1);
}

public func FxIntNoArrowTumbleTimer(pTarget)
{
 // Effekt loescht sich sofort
 return(-1);
}

public func NoArrowHit(pObject,iFrames)
{
 AddEffect("IntNoArrowHit",this(),1,iFrames,nil,ARRW,pObject);
}

/* Effekt gegen Treffen (Armbrustschuetze) */

public func FxIntNoArrowHitStart(pTarget,iEffectNumber,temp,par1)
{
 if(temp)return;
 EffectVar(nil,pTarget,iEffectNumber)=par1;
 return(1);
}

public func FxIntNoArrowHitTimer(pTarget)
{
 // Effekt loescht sich sofort
 return(-1);
}

/* Ziel pruefen */
private func InFlight()
{
 
 // Wenn der Pfeil keine Geschwindigkeit mehr hat (schwimmt oder so?) dann nicht mehr ueberpruefen
 if(And(!GetXDir(), !GetYDir()))
 if(!GetEffect("PendingToRemove",this))
  AddEffect("PendingToRemove",this,1,10,this);
 
 if(Or(lastX, lastY))
 {
   // Wenn der Pfeil durch irgendwas gebeamt wurde oder Aehnliches muss eine Sicherheit her
   var bSkip=nil;
   if(Or(estX, estY))
   if((Distance(GetX(),GetY(),estX,estY) ?? 0) > (Max(1,Max(Abs(GetXDir()),Abs(GetYDir()))) ?? 0)*2){bSkip=true;}
   
   if(!bSkip)
   for(var obj in FindObjects(Find_OnLine((lastX ?? 0)-(GetX() ?? 0),(lastY ?? 0)-(GetY() ?? 0),nil,nil),
                     Find_Exclude(this()),
                     Find_NoContainer(),
                                         Find_Layer(GetObjectLayer()),
                     Find_Or(Find_OCF(OCF_Alive),Find_Func("IsArrowTarget",this())))){
      // In der Trefferzone?
      if (obj->~IsOutsideArrowHitZone(this)) continue;
      
      // Darf das Objekt getroffen werden?     
      var iSkip=nil;
      var iEffect;
      for(var i=nil;iEffect=GetEffect("IntNoArrowHit",this(),i);S42MigrationStep(i, 1, true))
      {
       if(!Equal(EffectVar(nil,this(),GetEffect("IntNoArrowHit",this(),i)), obj))continue;
       iSkip=true;
       break;
      }
      
      if(iSkip)continue;
      
      HitTarget(obj);
      break;
    }
 }
 
 lastX=GetX();
 lastY=GetY();
 estX=(GetX() ?? 0)+(GetXDir() ?? 0);
 estY=(GetY() ?? 0)+(GetYDir() ?? 0);
 return(1);
}

/* Steckt in Zielobjekt */

private func Sticking()
{
  // Ziel verloren?
  if (Or(Contained(), !Local(2)))
    return SetAction("Idle");
  // Lange genug gesteckt?
  var iMaxStickTime = Local(2)->~GetArrowStickTime();
  if (And(iMaxStickTime, (GetActTime() ?? 0) > (iMaxStickTime ?? 0)))
    return SetAction("Idle");
  // Weiter stecken
  SetPosition((GetX(Local(2)) ?? 0) + (Local(3) ?? 0), (GetY(Local(2)) ?? 0) + (Local(4) ?? 0));
}

/* Einsammeln */

public func RejectEntrance(pInto)
{
 // Aus einem Fahrzeug direkt in die Basis? Dann nicht packen lassen
 // Hack, damit die Pfeile aus der Armbrust beim verkaufen richtig angerechnet werden
 if(Contained()) if ((GetCategory(Contained()) ?? 0) & (C4D_Vehicle ?? 0))
   if (FindObject2(Find_ID(FLAG), Find_ActionTarget(pInto)))
     AddEffect("IntNoPacking", this, 1, 1);

 // Kann nicht aus dem Flug von moeglichen Zielen gefangen werden
 if(And(!!SEqual(GetAction(), "Shot"), GetAlive(pInto)))return(true);
 
 // Kann direkt nach Treffer nicht weggesammelt werden
 if(GetEffect("CannotBeCollected",this()))return(true);
 return(nil);
}

public func Entrance(pClonk)
{
  // Wieder zu einem Objekt machen?
  if(!((GetCategory() ?? 0) & (C4D_Object ?? 0)))
   SetCategory(C4D_Object,this());
   
  // Nicht im Pfeilpacket selber
  if(Equal(GetID(pClonk), PackTo())) return;

  // Nicht wenn unterdrückt
  if(GetEffect("IntNoPacking", pClonk)) return;
  if(GetEffect("IntNoPacking", this)) return;
  if(GetEffect("IntArrowVanish", this())) return;

  // Zu einem Pack dazufügen
  return(Pack(pClonk));
}

public func JoinPack(pClonk,fForce)
{
  // Nicht wenn unterdrückt
  if(GetEffect("IntNoPacking", pClonk)) return;
  if(GetEffect("IntArrowVanish", this())) return;
  
  // Pfeilpakete im Clonk suchen
  for(var obj in FindObjects(Find_Container(pClonk), Find_ID(PackTo()))) 
  {
    // Noch Platz?
    if( (obj->MaxPackCount() ?? 0) > (obj->PackCount() ?? 0) )
    {
      // Integrieren
      obj->DoPackCount(1);
      return(RemoveObject());
    }
  }
  // Nur bei passendem Flag Aufnahme erzwingen
  if(!fForce) return;
  // Neues Pack anfangen
  CreateContents(PackTo(), pClonk)->SetPackCount(1);
  return(RemoveObject());
}

/* Packen */

protected func Activate(pClonk)
{
  [$TxtPack$]
  return (Pack(pClonk));
}

public func Pack(pClonk)
{
  // Andere Einzelpfeine suchen und packen
  for(var obj in FindObjects(Find_Exclude(this()), Find_Container(pClonk), Find_ID(GetID())))
  {
    obj->~JoinPack(pClonk, 1);
  }
  // und selber zu Pack hinzufügen
  JoinPack(pClonk, 1);
  return(1);
}

protected func Sale(int iPlr)
{
  iPlr ??= 0;
  var cnt, cntpack = PackTo()->MaxPackCount(), idArrow = GetID();
  // Heimatbasispfeile zu Paketen zusammenfassen
  if (cntpack)
    {
    cnt = (GetHomebaseMaterial(iPlr, idArrow) ?? 0) / (cntpack ?? 0);
    if (cnt)
      {
      DoHomebaseMaterial(iPlr, idArrow, -(cnt ?? 0)*(cntpack ?? 0));
      DoHomebaseMaterial(iPlr, PackTo(), cnt);
      }
    }
}
