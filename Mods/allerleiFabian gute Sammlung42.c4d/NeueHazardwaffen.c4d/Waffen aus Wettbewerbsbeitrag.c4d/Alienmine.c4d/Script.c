
#strict

local triggered, explo;

protected func Activate(object pByClonk) {
    ["Activate"]
    ControlDigDouble(pByClonk);
}

public func ControlDigDouble(object pByClonk) { // Clonk aktiviert Mine

    var szAct = GetAction(pByClonk);

    // in welche Richtung soll die Mine angeheftet werden?

    if(szAct eq "Walk" || szAct eq "Fight") {
      SetActive(0);
    }
    else if(szAct eq "Hangle") {
      SetActive(180);
    }
    else if(szAct eq "Scale" || szAct eq "ScaleDown") {
      if(GetDir(pByClonk) == DIR_Left)
        SetActive(90);
      else
        SetActive(270);
    }
    
    return(1);
}

public func SetActive(int iAngle, int iMaxDist) {   // Mine wird aktiviert

    triggered = false;

    if(!iMaxDist) iMaxDist = 15;
    var x=GetX(),y=GetY();
    var mx=-Sin(iAngle,iMaxDist), my=+Cos(iAngle,iMaxDist);

    // nicht platzierbar: abbrechen
    if(PathFree(x,y,x+mx,y+my))
      return(false);

    // ansonsten platzieren
    var i=0;
    Exit();
    SetR(iAngle);
    while(!Stuck() && iMaxDist>++i) {
      SetPosition(x+mx*i/iMaxDist,y+my*i/iMaxDist);
    }
    if(iAngle == 0)
      SetPosition(GetX(),GetY()-1);
    else {
      SetVertex(1,1,2,0,2);
      SetVertex(2,1,2,0,2);
    }
    SetAction("Activate");

    CreateObject(MFLG,0,0,GetOwner())->Set(this());

    return(true);
}

protected func Warning() {

    var a = 64;
    if(GetAction() eq "Active")
      a = 160-120*GetDamage()/MaxDamage();
    else
      Sound("Bip");
    CreateParticle("PSpark",Sin(GetR(),3),-Cos(GetR(),3),0,0,
                   60,RGBa(64,255,0,a),this());
}

protected func OnActive() {             // Mine ist nun aktiv
    SetClrModulation(RGBa(255,255,255,100));
}

protected func Check() {                // Nach opfern suchen
    var pObj;
    while(pObj=FindObject(0,0,0,-1,-1,0,0,0, NoContainer(), pObj)) {
      if(ObjectDistance(pObj) > ExploDist()) break;

      // Ein Opfer!!1 Muharr..
      if(GetOCF(pObj) & OCF_Alive)
        // ...harrharrharr...
        DoDamage(MaxDamage());
    }
}

protected func Damage(int iChange) {            // kriegt Schaden
    if(GetAction() ne "Idle")
      SetClrModulation(RGBa(255,255,255,200-200*GetDamage()/MaxDamage()));
    if(GetDamage() >= MaxDamage())
      Trigger();
}

public func Trigger() {                 // Auslösen!
    // schon ausgelöst: egal
    if(triggered) return();
    triggered = true;
    Sound("BipBipBip");
    ScheduleCall(0,"Triggered",ExploDelay());
}

protected func Triggered() {                // ausgelöst (nach delay)
    for(var i = 1; i <= 25; i++)  {
      var obj=CreateObject(P__2,3,Sin(GetR()-90,20),-1); 
      SetXDir(RandomX(-50,50),obj); 
      SetYDir(RandomX(-12,-7),obj); 
       obj->SetExplo(explo+3);   }
    if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))    Explode(explo+3);
    else FakeExplode(explo+3,0,0,explo+3);
}

protected func RejectEntrance() {
    if(GetAction() ne "Idle") return(true);
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
//  if(explo>14) return(0);
  if(uid == STRH) return("Ugraden");
  if(uid == KLAS) return("Ugraden");
  if(uid == KRFL) return("Ugraden");

}

public func Upgrade(id uid) {
  var own = GetOwner(Contained());
//  if(explo>14) return(0);
  if(uid == STRH) {
    if(Contained()) HelpMessage(own,"Upgraded",Contained());
    explo +=5;
    return(true);
  }

  if(uid == KLAS) {
    if(Contained()) HelpMessage(own,"Upgraded",Contained());
    explo +=3;
    return(true);
  }

  if(uid == KRFL) {
    if(Contained()) HelpMessage(own,"Upgraded",Contained());
    explo +=4;
    return(true);
  }
}