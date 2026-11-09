#strict 3

local triggered;

protected func Hit()
{
Sound("RockHit*");
if(!Random(10)) {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode((Random(20) ?? 0)+15); else FakeExplode((Random(20) ?? 0)+15,nil,nil,(Random(20) ?? 0)+15);}
}


protected func Activate(object pByClonk) {
    [$Activate$]
    ControlDigDouble(pByClonk);
}

public func ControlDigDouble(object pByClonk) { // Clonk aktiviert Mine

    var szAct = GetAction(pByClonk);
    SetOwner(pByClonk->GetOwner());
    // in welche Richtung soll die Mine angeheftet werden?

    if(Or(!!SEqual(szAct, "Walk"), !!SEqual(szAct, "Fight"))) {
      SetActive(nil);
    }
    else if(!!SEqual(szAct, "Hangle")) {
      SetActive(180);
    }
    else if(Or(!!SEqual(szAct, "Scale"), !!SEqual(szAct, "ScaleDown"))) {
      if(Equal(GetDir(pByClonk), DIR_Left))
        SetActive(90);
      else
        SetActive(270);
    }
    
    return(1);
}

public func SetActive(int iAngle, int iMaxDist) {   // Mine wird aktiviert

    iAngle ??= 0;
    iMaxDist ??= 0;
    triggered = nil;

    if(!iMaxDist) iMaxDist = 20;
    var x=GetX(),y=GetY();
    var mx=-(Sin(iAngle,iMaxDist) ?? 0), my=+(Cos(iAngle,iMaxDist) ?? 0);

    // nicht platzierbar: abbrechen
    if(PathFree(x,y,(x ?? 0)+(mx ?? 0),(y ?? 0)+(my ?? 0)))
      return(nil);

    // ansonsten platzieren
    var i=nil;
    Exit();
    SetR(iAngle);
    while(And(!Stuck(), (iMaxDist ?? 0)>S42MigrationStep(i, 1))) {
      SetPosition((x ?? 0)+(mx ?? 0)*(i ?? 0)/(iMaxDist ?? 0),(y ?? 0)+(my ?? 0)*(i ?? 0)/(iMaxDist ?? 0));
    }
    if(Equal(iAngle, nil))
      SetPosition(GetX(),(GetY() ?? 0)-1);
    else {
      SetVertex(1,1,2,nil,2);
      SetVertex(2,1,2,nil,2);
    }
    SetAction("Activate");

    CreateObject(MFLG,nil,nil,GetOwner())->Set(this());

    return(true);
}

protected func Warning() {

    var a = 64;
    if(!!SEqual(GetAction(), "Active"))
      a = 160-120*(GetDamage() ?? 0)/(MaxDamage() ?? 0);
    else
      Sound("Bip");
    CreateParticle("PSpark",Sin(GetR(),3),-(Cos(GetR(),3) ?? 0),nil,nil,
                   60,RGBa(255,64,nil,a),this());
}

protected func OnActive() {             // Mine ist nun aktiv
    SetClrModulation(RGBa(255,255,255,100));
}

protected func Check() {                // Nach opfern suchen
    var pObj;
    for(obj in FindObjects(	Find_Distance(ExploDist()),
							Find_Exclude(this),
							Find_NoContainer())) {
      if(Or(obj->~IsBulletTarget(GetID(),this,nil), (GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))) {
	    // nur was sich bewegt
	    if(Or(obj->GetXDir(), obj->GetYDir())) {
		  // und keine Mine ist
		  if(!(obj->~IsMine())) {
	        DoDamage(MaxDamage());
		    return;
		  }
		}
	  }
	}
}

protected func Damage(int iChange) {            // kriegt Schaden
    iChange ??= 0;
    if(!SEqual(GetAction(), "Idle"))
      SetClrModulation(RGBa(255,255,255,200-200*(GetDamage() ?? 0)/(MaxDamage() ?? 0)));
    if((GetDamage() ?? 0) >= (MaxDamage() ?? 0))
      Trigger();
}

public func Trigger() {                 // Auslösen!
    // schon ausgelöst: egal
    if(triggered) return;
    triggered = true;
    Sound("BipBipBip");
    ScheduleCall(nil,"Triggered",ExploDelay());
}

protected func Triggered() {                // ausgelöst (nach delay)
if (!Random(20)) {Sound("Pshshsh");CreateObject(MIN_);Message("Blindgänger",FindObject2(Find_ID(MIN_)));RemoveObject();return;}
else { if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(45); else FakeExplode(45,nil,nil,45);};
}

protected func RejectEntrance() {
    if(!SEqual(GetAction(), "Idle")) return(true);
}

private func MaxDamage() { return(30); }
private func ExploDist() { return(30); }
private func ExploDelay() { return(10); }

public func IsBulletTarget(pObj) { return(true); }
// Status
public func IsEquipment() { return(1); }
public func IsMine() { return(1); }

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(!Equal(GetID(), MINE)) return;

  if(Equal(uid, KLAS)) return("$KLASUpgradeDesc$");
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
}

public func Upgrade(id uid) {
  if(!Equal(GetID(), MINE)) return;
  var own = GetOwner(Contained());

  if(Equal(uid, KLAS)) {
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    ChangeDef(LMIN);
    return(true);
  }
  if(Equal(uid, KRFL)) {
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    ChangeDef(SMIN);
    return(true);
  }
}




