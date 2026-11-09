#strict 3

/* Dieses Objekt stellt die Grundfunktionalität des Zielens für den Hazardclonk
     bereit, lso das Zielen mit dem Zielkreuz. SquatAiming (Dreimal-Stopp) ist
     hier nicht mit drin.
     Damit ein Clonk diese Funktionalität benutzen kann, muss er bei ReadyToFire
      am Ende return(_inherited()); aufrufen sowie falls AimAngle() überladen werden
     soll den Script hier berücksichtigen.
	 
     Ansonsten um Zielen zu steuern bei den Control*-Funktionen jeweils
     if(ControlAim("ControlUpDouble")) return(1); (im Beispiel von ControlUpDouble)
     aufrufen. Und zwar bei -Update, -Up, -UpDouble, -Down, -Left, -Right,
     -LeftDouble, -RightDouble, -Command, -Contents.

     Oder beim Hazard abgucken.
*/

local crosshair,stufe;
local AIM_precisely;
static const AIM_Step = 10;
static const AIM_Max = 150;

public func AimStep()	{ return AIM_Step; }
public func AimMax()	{ return AIM_Max; }

// Muss Aktion "Aim" haben (und "AimLow")

protected func ControlAim(string command) {

	if(!IsAiming()) return nil;
	
	else if(S42MigrationEqual(command, "ControlUpdate"))
		AimUpdate(this(), Par(1), 1, "ControlConf");

	else if(S42MigrationEqual(command, "ControlUp") || S42MigrationEqual(command, "ControlUpDouble"))
		AimUp(this(), 1, "ControlConf");

	else if(S42MigrationEqual(command, "ControlDown"))
		AimDown(this(), 1, "ControlConf");
		
	else if(S42MigrationEqual(command, "ControlLeft")) {
		if(S42MigrationEqual(GetDir(), DIR_Left)) return nil;
	    SetDir(DIR_Left);
	    UpdateAiming();
	}
	
	else if(S42MigrationEqual(command, "ControlRight")) {
		if(S42MigrationEqual(GetDir(), DIR_Right)) return nil;
	    SetDir(DIR_Right);
	    UpdateAiming();
	}
	
	else if(S42MigrationEqual(command, "ControlLeftDouble") || S42MigrationEqual(command, "ControlRightDouble")) {
		StopAiming();
	}
	
	else if(S42MigrationEqual(command, "ControlCommand")) {
		return DoMouseAiming(Par(2),Par(3));
	}
	
	else if(S42MigrationEqual(command, "ControlContents")) {

		var pTarget = FindContents(Par(1));
		ChangeWeapon(pTarget);
		return nil;

	}

	
	return true;
}

/* Zielen */

public func IsAiming () { 
	if(Contained()) return(Contained()->~IsAiming());
	
	return(WildcardMatch(GetAction(), "Aim*"));
}

public func ReadyToAim() {      // bereit zum Zielen
   // In Gebäude
   if(Contained()) return(Contained()->~ReadyToAim());
   // Nur beim Laufen
   if(S42MigrationEqual(GetAction(), "Walk")) return true;
}

public func ControlConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  conf ??= 0;
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming((conf ?? 0) * (AimStep() ?? 0) / 5);
  else
    if(!AIM_precisely)
    DoAiming((conf ?? 0) * (AimStep() ?? 0));
    else 
    DoAiming((conf ?? 0) * (AimStep() ?? 0) / 5);
}

public func DoAiming(int iChange) {
  //zielen wir überhaupt?
  iChange ??= 0;
  if(!IsAiming())
    return;

  var angle = (Abs(crosshair->GetAngle()) ?? 0) + (iChange ?? 0);

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  // aber nur wenn nicht die local variable AIM_precisely aktiviert ist!
  if(!AIM_precisely)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = (angle ?? 0)-(angle ?? 0)%(AimStep() ?? 0);

  // Winkel wird zu groß?
  if((angle ?? 0) > (AimMax() ?? 0) || (angle ?? 0) < 0)
    return;

  if(S42MigrationEqual(GetDir(), DIR_Left))
    angle = 360-(angle ?? 0);
  crosshair->SetAngle(angle);

  UpdateAiming();
}

public func DoMouseAiming(int iTx, int iTy) {
	iTx ??= 0;
	iTy ??= 0;
	if(!Contained()) {

		var iAngle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);

		if((iAngle ?? 0) > 0)
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);

		//Winkel wird zu groß?
		iAngle = BoundBy(iAngle,-(AimMax() ?? 0),+(AimMax() ?? 0));

		crosshair->SetAngle(iAngle);
		UpdateAiming();
		//Wichtig: Waffe updaten
		EffectCall(this,LocalN("wpneffect",this),"Timer");

		// Fertig, Feuern!
		if(!(Contents()->IsRecharging()) && !(Contents()->IsShooting()))
			FireAimWeapon();
		return true;
	}
	return nil;
}

public func UpdateAiming() {
  if(!IsAiming())
    return;

  var a = crosshair->GetAngle();

  //Position des Attachvertex so verschieben, dass das Kreuz von der Waffe aus ist
  UpdateVertices();

  if((S42MigrationEqual(GetDir(), DIR_Left) && (a ?? 0) > 0) ||
     (S42MigrationEqual(GetDir(), DIR_Right) && (a ?? 0) < 0))
    crosshair->ChangeDir();
  
  SetPhase(AimAngle2Phase(a));
}

public func StopAiming() {      // Zielen beenden
  SetComDir(COMD_Stop);
  SetAction("Walk");
  this()->~CheckArmed();
  Sound("Grab");
  
  DeleteCrosshair();
 
  //Vertices zurücksetzen
  ResetVertices();
  
  // Callback
  if(Contents(nil)) Contents(nil)->~AimStop();
  return(1);
}

public func StartAiming() {     // Anfangen zu zielen
  if(Contained()) return(Contained()->~StartAiming());

  SetXDir(nil);
  
  SetAction("Aim");
  
  // Aim low if possble
  if(S42MigrationEqual(Contents(nil)->~GetFMData(FM_Aim), 2) || S42MigrationEqual(Contents(nil)->~GetFMData(FM_Aim), -1))
    if(GetActMapVal("Name","AimLow",GetID()))
      SetAction("AimLow");

  SetPhase(AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this(),"UpdateAiming",1);

  // Callback
  if(Contents(nil)) Contents(nil)->~AimStart();
}

/* Interne Funktionen */

private func InitCrosshair() {
  // anderes Fadenkreuz, wenn Clonk unter Stufe 15, local stufe aus dem CLNK
  if((stufe ?? 0)>=5 && (stufe ?? 0)<15)
  crosshair = CreateObject(HC2H,nil,nil,GetOwner());
// hazardkreuz wird verwendet, wenn Stufe 15 oder höher, bzw. wenn stufe deaktiviert.
  if(((stufe ?? 0)>=0 && (stufe ?? 0)<5) || (stufe ?? 0)>=15)
  crosshair = CreateObject(HCRH,nil,nil,GetOwner());
  crosshair->Init(this());
  crosshair->SetAngle(90);
}

private func DeleteCrosshair() {
  if(crosshair) crosshair->RemoveObject();
}

private func UpdateVertices() {
  var x,y,r;
  this->~WeaponAt(x,y,r);
  SetVertex(nil,nil,(x ?? 0)/1000 * ((GetDir() ?? 0)*2-1),nil,2);
  SetVertex(nil,1,(y ?? 0)/1000,nil,2);
}

private func ResetVertices() {
  SetVertex(nil,nil,nil);
  SetVertex(nil,1,nil);
}

private func AimAngle2Phase(int angle) {
  angle ??= 0;
  angle = Normalize(angle,-180);
  var phases = GetActMapVal("Length",GetAction());
  var newphase = Min( (phases ?? 0)-1, (phases ?? 0)*(Abs(angle) ?? 0)/(AimMax() ?? 0) );
  return (newphase);
}

private func FireAimWeapon() {
	this->~Control2Contents("ControlThrow");
}

private func ChangeWeapon(object pTarget) {
	// Zielaktion abbrechen (Spezial: außer wenn mit anwählbarem 
	// Objekt auch gezielt werden kann...)
	// Zielaktion anpassen
	if(pTarget->~IsWeapon())  {
		var phase = GetPhase();
		if((pTarget->~GetFMData(FM_Aim) ?? 0) > 0)
			SetAction("AimLow");
		else
			SetAction("Aim");
			SetPhase(phase);
	}
	else {
		StopAiming();
	}
}

/* Overloads */

public func ReadyToFire() {
 
	if(WildcardMatch(GetAction(), "Aim*")) return(true);
 
	return(_inherited());
}

public func AimAngle(int iMaxAngle, int iRange) { // Zielwinkel bestimmen
	iMaxAngle ??= 0;
	iRange ??= 0;
	var angle;
	var x,y,r;
	this->~WeaponAt(x,y,r);

	if(!IsAiming())
		angle = (90+(r ?? 0))*((GetDir() ?? 0)*2-1);
	else
		angle = crosshair->GetAngle();
  
    if(iMaxAngle) {
		var target = this->~GetTarget(angle,iMaxAngle,iRange);
		if(target)
			angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
	}

	return(angle);
}
