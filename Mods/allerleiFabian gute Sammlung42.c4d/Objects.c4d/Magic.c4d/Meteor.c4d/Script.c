#strict 3

local explosion_base;

public func CastMeteor(id idObj,int iX,int iDir,int iRange,int iExplo, int iOwner) { 
  // Standardhagel besteht aus Meteoriten
  iX ??= 0;
  iDir ??= 0;
  iRange ??= 0;
  iExplo ??= 0;
  iOwner ??= 0;
  if(!idObj) idObj=METO;
  // Log("%d is Inside %d,%d",AbsX(iX+RandomX(-iRange/2,+iRange/2)),iX-iRange/2,iX+iRange/2);
  // Einen Meteoriten gen Erdreich schleudern
  var iY=AbsX(nil);
  if (!GetScenTopOpen()) S42MigrationAssign(iY, "+=", (GetDefHeight(idObj) ?? 0));
  var pMeteor = CreateObject(idObj,AbsX((iX ?? 0)+(RandomX(-(iRange ?? 0)/2,(+(iRange ?? 0) ?? 0)/2) ?? 0)),iY, -1);
  SetSpeed((iDir ?? 0)*(RandomX(5,30) ?? 0)+(RandomX(-20,+20) ?? 0),15,pMeteor);
  // Controller korrigieren
  SetController(iOwner,pMeteor);
  // Explosionsradius anpassen
  if(And(iExplo, Equal(idObj, METO))) pMeteor->METO::SetExplosionBase(iExplo);
}

public func Activate(object pCaster, object pRealcaster) {
  // Entfernung des Wirkungsgebietes
  var iDist=RandomX(800,1000);
  // Streuung standard (400 in jede Richtung)
  var iRange=800;

  // Wartezeit für die Zielanpeilung in Frames
  var iDelay=200;

  // Zaubernden Clonk ermitteln
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;

  var iDir=(GetDir(pCaster) ?? 0)*2-1;
  var idC=GetID(Contents(nil,pClonk));
  // wenn gültige Objektkomponente dabei...
  if( Or(Or(Or(Or(Or(Or(Or(Equal(idC, SFLN), Equal(idC, EFLN)), Equal(idC, MEGG)), Equal(idC, FMEG)), Equal(idC, ZAPN)), Equal(idC, CNCR)), Equal(idC, LIQG)), Equal(idC, FRBL)))
    AddEffect("MeteorDelayNSpell",Contents(nil,pClonk),1,iDelay,nil,GetID(),GetOwner(pClonk),pClonk,iDir);

  // ansonsten "normal" casten
  else
    {
     var iEffect=AddEffect("MeteorNSpell",nil,140,15,nil,GetID(),BoundBy((GetX(pCaster) ?? 0)+(iDir ?? 0)*(iDist ?? 0),(iRange ?? 0)/2,(LandscapeWidth() ?? 0)-(iRange ?? 0)/2),iDir,iRange,nil);
     if((iEffect ?? 0) > 0)
       EffectVar(4,nil,iEffect)=GetOwner(pClonk);
    }
  Sound("Magic1");
  RemoveObject();
  return(1);
}

func FxMeteorDelayNSpellStart(pTarget,iEffectNumber,iTemp,iOwner,pCaster,iDir) { 
 EffectVar(nil,pTarget,iEffectNumber)=iOwner; //Besitzer für später speichern
 EffectVar(1,pTarget,iEffectNumber)=pCaster; //Caster für später speichern
 EffectVar(2,pTarget,iEffectNumber)=iDir; //Richtung des Casters für später speichern
 return(+1);
}

func FxMeteorDelayNSpellTimer() { return(-1); }

func FxMeteorDelayNSpellStop(pObject,iEffectNumber,iReason,fTemp) {
  // Entfernung des Wirkungsgebietes
  var iDist=RandomX(800,1000);
  var iRange=500;  // geringere Streuung bei der Zielanpeilung

  // nur temporäre Entfernung: nicht beachten
  if(fTemp) return;

  var idC=GetID(pObject);
  // Wenn der Caster das Objekt noch in der Hand hält, dann Richtung beachten
  var pCaster=EffectVar(1, pObject, iEffectNumber);
  var iDir=EffectVar(2, pObject, iEffectNumber);
  if(Or(!pCaster, !Equal(pObject->Contained(), pCaster))) iDir = nil;
  // pObject auf Objektkomponenten untersuchen
  var iEffect=AddEffect("MeteorNSpell",nil,140,15,nil,GetID(),BoundBy((GetX(pObject) ?? 0)+(iDir ?? 0)*(iDist ?? 0),(iRange ?? 0)/2,(LandscapeWidth() ?? 0)-(iRange ?? 0)/2),iDir,iRange,idC);
  // Da nicht genug Parameter übergeben werden können
  if((iEffect ?? 0) > 0)
    EffectVar(4,nil,iEffect)=EffectVar(nil,pObject,iEffectNumber);
  // Objekt verbrauchen
  RemoveObject(pObject);
}

func FxMeteorNSpellStart(pTarget,iEffectNumber,iTemp,iX,iDir,iRange,idObj,iOwner) {
  // temporäre Aufrufe ignorieren
  if(iTemp) return;

  // X-Position des Hagels
  EffectVar(nil,pTarget,iEffectNumber)=iX;
  // Richtung des Hagels (-1 links, 1 rechts, 0 keine)
  EffectVar(1,pTarget,iEffectNumber)=iDir;
  // Streuung des Hagels
  EffectVar(2,pTarget,iEffectNumber)=iRange; 
  // ID der herunterzufallenden Objekte
  EffectVar(3,pTarget,iEffectNumber)=idObj; 
  //EffectVar 4 ist der Controller des Objektes (die Variable muss ausserhalb dieser Funktion gesetzt werden)
  return(+1);
}

func FxMeteorNSpellTimer(pTarget,iEffectNumber,iEffectTime) {
 // Nach 200 Frames das weitere Meteoriten casten unterlassen
 if((iEffectTime ?? 0)>200) return(-1);

 var iOwner = EffectVar(4,pTarget,iEffectNumber);
 var idObj =  EffectVar(3,pTarget,iEffectNumber);
 var iRange = EffectVar(2,pTarget,iEffectNumber);
 var iDir =   EffectVar(1,pTarget,iEffectNumber);
 var iX =     EffectVar(nil,pTarget,iEffectNumber);
 var iExplo;

 // nur weniger casten bei:
 if(Equal(idObj, ZAPN)) if(Random(3))  return;
 if(Equal(idObj, FMEG)) if(!Random(2)) return;
 if(Equal(idObj, MEGG)) if(!Random(2)) return;
 if(Equal(idObj, FBRL)) if(Random(3)) return;

 // unterschiedliche Verhaltensweisen der Komponenten
 if(Equal(idObj, SFLN))      { idObj=METO; iExplo=14; }
 else if(Equal(idObj, EFLN)) { idObj=METO; iExplo=18; }
 else if(Equal(idObj, MEGG)) if(!Random(10)) idObj=FMEG;
 else if(Equal(idObj, FMEG)) if(!Random(4))  idObj=MEGG;
 //LIQG und CNCR bleiben so
  
 CastMeteor(idObj,iX,iDir,iRange,iExplo,iOwner);
}

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("522"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
