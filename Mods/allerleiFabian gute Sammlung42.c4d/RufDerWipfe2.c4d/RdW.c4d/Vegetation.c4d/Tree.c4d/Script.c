/*-- Baumsteuerung --*/

#strict 3

local MotionThreshold;
local MotionOffset;
local axeddown;

/* Überladbare Konstanten */
public  func TreeStrength() {return(150);} // Sozusagen Lebenspunkte. Je höher, desto schwerer zu fällen.
private func ReproductionAreaSize() { return(400); } // Größe des Bereichs, in dem neue Bäume dieser Art entstehen können
private func ReproductionRate()     { return(500); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxTreeCount()         { return(12); } // Maximale Baumanzahl im Umkreis
private func ForestChance()         { return(2);}  // Chance und Grösse eines Waldes
private func AllowForeground()      { return(1);}  // Erlaubt Vordergrund-Kategorie

func AxedDown(pClonk) // wird aufgerufen, wenn von Clonk mit axt zerkleinert wird
{
    if(axeddown)
        axeddown=nil;
    else
        axeddown=pClonk;
}

func IsAxingDown() // info, ob er bereits zerkleinert wird
{
    return(axeddown);
}

/* Kontext */

public func ContextChop(pClonk)
{
  [$TxtChop$|Image=TRMS|Condition=IsStanding]
  AddCommand(pClonk, "Chop", this());
  return(1);
}

public func StartChop(pClonk)
{
  ObjectSetAction(pClonk,"Chop",this());
  return(1);
}

func Construction()
{
  // Bewegungsschwelle
  MotionThreshold = Random(10);
  // Aktionszeitoffset
  MotionOffset = Random(GetActMapVal("Length", "Breeze"));
  // Verzögerte Initialisierung: Schlägt SetAction fehl, ist es ein alter Baum ohne Animation. Motion-Variablen werden ignoriert.
  if(SetAction("Initialize"))
  {
    // Ausrichtung
    SetDir(Random(2));
  }
  ScheduleCall(nil,"Constructed",1);
  SetLayerTransform(nil,RandomX(-5,5),RandomX(95,105),RandomX(95,105));
  return(1);
}

func Constructed()
{
  var pReproduct;
  /* Wüste? */
  if(Equal(GetMaterial(nil,80), Material("Sand")))
    if(Random(10))
    {
    	if(!Random(2)) return RemoveObject();
      ChangeDef(GetDefBurnTo(GetID()));
    	return(1);
    };
  //SetCon(100);
  var category;
  category = GetCategory();
  if(AllowForeground())
    if(!Random(5))
      category = (category ?? 0) | (C4D_Foreground ?? 0);	// Vordergrund
  SetCategory(category);
  // Waldbildung
  if((FrameCounter() ?? 0)<5)
   if(!IsDeadTree())
    if(Random(ForestChance()))
	   if(pReproduct=Reproduction())
	    pReproduct->~SetCon(BoundBy(RandomX(10,150),10,100));
  return(1);
}

func Damage()
{
  Laub();
  var worker;
  // Wenn der Baum schon zu viel Schaden genommen hat...
  if ((GetDamage() ?? 0) > (TreeStrength() ?? 0) * (Max(GetCon(), 30) ?? 0)/100)
    {
    // ...und er noch steht...
    if (IsStanding())
      // fällen
      ChopDown(); 
    // Beschädigte, tote Bäume sollten außerdem verfallen
    if (IsDeadTree()) if (!GetEffect("IntTreeDecay", this()))
      if ((GetCon() ?? 0) < 90) if ((GetCategory() ?? 0) & (C4D_Vehicle ?? 0))
        AddEffect("IntTreeDecay", this(), 1, 51, this());
    }
  return(1);
}

public func Laub()
{
	if(And((GetTemperature() ?? 0)>0, !IsDeadTree()))
	{
    var id=GetID();
    var clr=RGB(64,255,64);
// krank oder herbst, dann braune blätter
    if(Or(!!SEqual(GetGraphics(), "Krank"), (And(And(And((GetSeason() ?? 0)>=62, (GetSeason() ?? 0)<87), (GetTemperature() ?? 0)>0), !IsDeadTree()))))
    {
      if(!Random(2))
        Laub();
      clr=RGB(127,107,45);
    }
        for(var i=nil; (i ?? 0)<(GetCon() ?? 0)/20; S42MigrationStep(i, 1, true))
      CreateParticle("Laub",(GetDefOffset(id) ?? 0)+(Random((GetDefWidth(id) ?? 0)-20) ?? 0)+10,(GetDefOffset(id, 1) ?? 0)+(Random((GetDefHeight(id) ?? 0)-(GetDefFireTop(id) ?? 0)-20) ?? 0)+10,nil,1,25,clr);
	}

	if(And(!!SEqual(GetGraphics(), "Winter"), !IsDeadTree()))
	{
    var id=GetID();
    var clr=RGB(255,255,255);

        for(var i=nil; (i ?? 0)<(GetCon() ?? 0)/20; S42MigrationStep(i, 1, true))
      CreateParticle("Laub",(GetDefOffset(id) ?? 0)+(Random((GetDefWidth(id) ?? 0)-20) ?? 0)+10,(GetDefOffset(id, 1) ?? 0)+(Random((GetDefHeight(id) ?? 0)-(GetDefFireTop(id) ?? 0)-20) ?? 0)+10,nil,1,25,clr);
	}
	return(1);
}

/*private func Shrink(worker)
{
  //DoCon(-Abs(120/GetComponentCount(WOOD,1)));
  DoCon(-20);
  CreateObject(WOOD,GetX(worker)-GetX(),GetY(worker)-GetY()+8);
  DoDamage(-100);
  CheckFree(); 
  if(GetCon()<=20) RemoveObject();
  return(1);
}*/

public func CheckFree()
{
  while(GBackSolid(nil,(GetVertex(nil,1) ?? 0)-3)) SetPosition(GetX(),(GetY() ?? 0)-1);
  return(1);
}

/* Fortpflanzung */

private func Seed()
{
 if(axeddown) // wenn baum zerkleinert wird, ab und zu checken, ob zerkleinerung noch läuft. 
 {
     var axe;
     for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember),Find_Distance(30)))
        if(!!SEqual(GetAction(pClonk), "AxeDown"))
            axe = 1;
     if(!axe) // wenn kein Clonk in der Nähe zerkleinert, dann baum wieder freigeben.
        axeddown=nil;
 }
 
    
//Pollute(-1);
  var pReproduct;
  if((GetCon() ?? 0)<80)
   return(1);
  if(IsDeadTree())
   return(1);
  if((GetTemperature() ?? 0)<0)
   return(1);
  // Je nach Fortpflanzungsrate mehr oder weniger oft...
  if(!Random((ReproductionRate() ?? 0)*4))//RdW-Bäume pflanzen sich langsamer fort
    // ...reproduzieren
    if(pReproduct=Reproduction())
	     pReproduct->~NewTree();
}

public func Reproduction()
{
//tote Bäume reproduzieren sich nicht, für umweltstärken-forschung
if (IsDeadTree()) return(nil);
  // Ist noch Platz für einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = (iSize ?? 0) / -2;
  if ((ObjectCount(nil, iOffset, iOffset, iSize, iSize, OCF_Chop) ?? 0)<(MaxTreeCount() ?? 0))
    // OK, hin damit
//Log("Baum geplanzt");
    return(PlaceVegetation(GetID(this()), iOffset, iOffset, iSize, iSize, 10) );
  // Kein Platz ;'(
  return(nil);
}

/* Status */

public func IsTree() { return(1); }
public func IsStanding() { return(And((~(GetCategory() ?? 0) & (C4D_Vehicle ?? 0)), IsTree())); }
public func IsDeadTree() { return(nil); } // Überladen von toten Bäumen

///
public func ChopDown()
{
//  var worker;
  //Dem Typ den Befehl zum kleinhacken geben
//  if(worker=FindObject2(Find_Action("Chop"),Find_ActionTarget(this())))
//  {
//    AppendCommand(worker,"MoveTo",this());
//    AppendCommand(worker,"Wait",0,0,0,0,0,5);
//    AppendCommand(worker,"Call",this(),0,0,0,0,"ControlDigDouble2");
//  }
  // Bereits gefällt
  if (!IsStanding()) return(nil);
  // Kategorie ändern
  SetAction("Idle");
  SetCategory(C4D_Vehicle);
  // Aus der Erde lösen
  while (And(Stuck(), (S42MigrationStep(Var(nil), 1) < 12)))
    SetPosition(GetX(), (GetY() ?? 0) - 1, this());
  // Umfallen
  SetRDir(+10); if (Random(2)) SetRDir(-10);
  // Geräusch
  if ((GetCon() ?? 0) > 50) Sound("TreeDown*");
  // Fertig
  return(1);
}





/*
public func Decompose(pClonk)
{
  AddEffect("IntDecompose",pClonk,1,1,0,GetID(),this());
}

public func ControlDigDouble2(pClonk)
{
  if(!IsStanding())
    if(IsTree())
      return AddEffect("IntDecompose",pClonk,1,1,0,GetID(),this());
}
/*

protected func FxIntTreeDecayTimer(object target, int num, int time)
{
  // Zur Sicherheit
  if (!target->~IsDeadTree() || GetCon(target) >= 90 || (GetCategory(target) & C4D_StaticBack))
    return FX_Execute_Kill;
  // Verfall
  DoCon(-1, target);
  return FX_OK;
}

/*
func FxIntDecomposeStart(object pClonk, int iNumber, int iTemp, object pDecompose)
{
  var szAct;
  EffectVar(0,pClonk,iNumber) = pDecompose;
  // Hackle vorhanden? toll. Nicht vorhanden? nich so toll.
  // ...aber möglich!! :)
  if(GetActMapVal("Name","Hackle",GetID(pClonk)))
    szAct = "Hackle";
  else if(GetActMapVal("Name","Process",GetID(pClonk)))
    szAct = "Process";
  else
    szAct = "FlatUp";
  pClonk->SetAction(szAct);
  EffectVar(1,pClonk,iNumber) = szAct;
}

func FxIntDecomposeTimer(object pClonk, int iNumber, int iEffectTime)
{
  var pDecompose = EffectVar(0,pClonk,iNumber);
  var szAction   = EffectVar(1,pClonk,iNumber);

  if(!pDecompose)
    return -1;
  
  // Objekt weg
  if(ObjectDistance(pDecompose,pClonk)>25)
      return(-1);
      
  // Zeit abgelaufen
  if(iEffectTime >= pDecompose->~TreeStrength()*2)
  {
    var i,id,num;
    // Richtung ermitteln 
    var iOutDir; if (!GetDir(pClonk)) iOutDir=-1; else iOutDir=1; 
    while (id = GetComponent(0, i++, pDecompose))
	   if(num = GetComponent(id,0,pDecompose))
		  while (num--) CreateObject(id, GetX(pClonk)+10*iOutDir, GetY(pClonk)-(num*5)+5, -1)->SetR(RandomX(125,145));
		RemoveObject(pDecompose);
    return(-1);
  }
  // Aktion
  if(szAction eq "FlatUp")
    if(GetPhase(pClonk)!=1) SetPhase(1,pClonk);

  if(GetAction(pClonk) ne szAction) return(-1);
}

func FxIntDecomposeStop(object pClonk, int iNumber, int iReason, bool fTemp)
{
  if (fTemp) return();
  pClonk->SetAction("Walk");
  return (FX_OK);
}
/*

/* Bewegung (Wind) */  
  
private func Still()
{
  if ((Abs(GetWind()) ?? 0) > 49 + (MotionThreshold ?? 0)) SetAction("Breeze");
}
    
private func Breeze()
{
  if ((Abs(GetWind()) ?? 0) < 50 + (MotionThreshold ?? 0)) SetAction("Still");
}

private func Initializing()
{
  // Bis zum Aktionszeitoffset gewartet: los geht's
  if (Equal(GetPhase(), MotionOffset))
    SetAction("Still");
  // Noch warten
  return(1);  
}

/* KI (Holzbeschaffung) 

public func IsProducerOf (clonk, def)
{
  if(!FindObject2(Find_OCF(OCF_CrewMember),Find_ActionTarget(this)))
    if(!OnFire())
      if(GetComponent(WOOD))
        if(def == WOOD) return true;
}

public func HowToProduce (worker, def)
{
  if(IsStanding())
    AddCommand("Chop",worker,this);
  else
  {
    AppendCommand(worker,"MoveTo",this);
    AppendCommand(worker,"Wait",0,0,0,0,0,5);
    AppendCommand(worker,"Call",this,0,0,0,0,"ControlDigDouble2");
  }
  return (1);
}
*/