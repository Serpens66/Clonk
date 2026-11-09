/* Schwarmmine */

#strict 3

protected func Activate(object pByClonk) {
    [$Activate$]
    ControlDigDouble(pByClonk);
}

protected func Hit()
{
Sound("RockHit*");
if(!Random(10)) {if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode((Random(20) ?? 0)+15); else FakeExplode((Random(20) ?? 0)+15,nil,nil,(Random(20) ?? 0)+15);}
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
    if(!iMaxDist) iMaxDist = 15;
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

    return(true);
}

protected func Warning() {

    var a = 64;
    if(!!SEqual(GetAction(), "Active"))
      a = 160-120*(GetDamage() ?? 0)/(MaxDamage() ?? 0);
    else
      Sound("Bip");
    CreateParticle("PSpark",Sin(GetR(),3),-(Cos(GetR(),3) ?? 0),nil,nil,
                   60,RGBa(nil,64,255,a),this());
}

protected func OnActive() {             // Mine ist nun aktiv
    SetVertex(1,1,nil,nil,2);
    SetVertex(2,1,nil,nil,2);
    var xdir, ydir,
    xdir = Sin(GetR(), 30);
    ydir = -(Cos(GetR(), 30) ?? 0);

    SetAction("Fly");           // Hüpfen
    SetSpeed(xdir, ydir);
    SetRDir(RandomX(-50,50));
    ScheduleCall(this(), "Swarm", 10);
}

protected func Swarm() {            // Ordnungsgemäß aktiviert, gesprungen; jetzt schwärmen
    for(var i = nil,swarmy ; (i ?? 0) < 10 ; S42MigrationStep(i, 1, true))
      {
      swarmy = CreateObject(SWRM, nil, nil, GetOwner());
      swarmy -> Activate();
      }
    Sparks(10, RGB(255, 125, nil));           // Sound!!!
    RemoveObject();
}

protected func Damage(int iChange) {            // kriegt Schaden
    iChange ??= 0;
    if(!SEqual(GetAction(), "Idle"))
      SetClrModulation(RGBa(255,255,255,200-200*(GetDamage() ?? 0)/(MaxDamage() ?? 0)));
    if((GetDamage() ?? 0) >= (MaxDamage() ?? 0))
      Trigger();
}

public func Trigger() {                 // Auslösen!
    ScheduleCall(nil,"Triggered",ExploDelay());
}

protected func Triggered() {                // ausgelöst (nach delay)
   if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(10);
   else FakeExplode(10,nil,nil,10);
}

protected func RejectEntrance() {
    if(!SEqual(GetAction(), "Idle")) return(true);
}

private func ExploDelay() { return(10); }
private func MaxDamage() { return(30); }

public func IsBulletTarget(pObj) { return(true); }
// Status
public func IsEquipment() { return(1); }
public func IsMine() { return(1); }
