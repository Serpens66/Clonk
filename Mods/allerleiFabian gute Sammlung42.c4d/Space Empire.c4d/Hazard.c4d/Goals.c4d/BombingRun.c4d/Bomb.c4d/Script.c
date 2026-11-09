/*-- Bombe (Zapper kann nicht coden) --*/

#strict 3

local startX,startY;
local pLight;
local timer, holder;

// Timer zurückliefern
public func GetBombTimer()
{
  return(timer);
}

public func IsAtHome() {
	if((Abs((GetX() ?? 0)-(startX ?? 0)) ?? 0) < 20)
		if((Abs((GetY() ?? 0)-(startY ?? 0)) ?? 0) < 20)
			if(Equal(GetOwner(), -1))
				return(true);
	return(nil);
}

public func Init(iX,iY)
{
	startX=iX;
	startY=iY;
	Exit();
	SetPosition(startX,startY);
	SetOwner(NO_OWNER);
	holder = nil;
	AddEffect("BombFlight",this(),20,1,this(),GetID());
	// eventuelle Bombeneffekte entfernen
	var effect = GetEffect("BombTimer",this());
	if(effect)
		RemoveEffect(nil,this(),effect);
}

// Anzeige in der Hand
public func IsDrawable() { return(true); }
public func HandSize() { return(1050); }
public func HandX() { return(1000); }
public func HandY() { return(1000); }

public func IsBouncy() { return(true); }
public func CanAim() { return(1); }

public func ControlThrow() {
	// bereit zum Schießen
	if(!(Contained()->ReadyToFire())) return(nil);

	LaunchBomb(6,Contained()->~AimAngle(nil));
	return(true);
}

// verschießt die Bombe
// speed: Geschwindigkeit
// angle: Winkel
public func LaunchBomb(int speed, int angle) {
	speed ??= 0;
	angle ??= 0;
	var user = Contained();
	var dir = (GetDir(user) ?? 0)*2-1;

	// Adjust angle
	angle = BoundBy((angle ?? 0)+(GetYDir(user) ?? 0)*(dir ?? 0),-360,360);
	// calculate speed
	var xdir = Sin(angle,speed);
	var ydir = -(Cos(angle,speed) ?? 0);

	var x,y;
	user->WeaponEnd(x,y);

  // effect
  Sound("BombShot");

	for(var cnt=nil;(cnt ?? 0)<5;S42MigrationStep(cnt, 1, true))
	  CreateParticle("Thrust",x,y,(GetXDir(user) ?? 0)+(RandomX(-2,2) ?? 0),(GetYDir(user) ?? 0)+(RandomX(-2,2) ?? 0),100,RGBa(10,10,255,nil),nil,nil);

	for(var i=nil; (i ?? 0)<0; S42MigrationStep(i, 1)) {
		CreateParticle("Smoke2",(x ?? 0)+(RandomX(-5,+5) ?? 0),(y ?? 0)+(RandomX(-5,+5) ?? 0),
						(GetXDir(user) ?? 0)+(RandomX(nil,(xdir ?? 0)/4) ?? 0),(GetYDir(user) ?? 0)+(RandomX(nil,(ydir ?? 0)/4) ?? 0),
						RandomX(80,140),RGBa(100,100,255,nil),nil,nil);
	}

  var no = AddEffect("CollectionException", this(), 1, 5);
  EffectVar(nil, this(), no) = Contained();
	Exit(this(), (x ?? 0)+(xdir ?? 0)/10, (y ?? 0)+(ydir ?? 0)/10,xdir,xdir, ydir,nil);
}

protected func Destruction()
{
	var pBomb=CreateObject(GetID(),nil,nil,-1);
	pBomb->Init(startX,startY);
	GameCallEx("BombReturned");
	Log("$Returned$");
	Sound("BombReset",1);
}

protected func Departure() {
	GameCallEx("BombLoose");
}

protected func ReturnToSpawn()
{
	var eff;
	while(eff = GetEffect("BombFlight",this()))
		RemoveEffect(nil, this(),eff);
	SetSpeed();
	Init(startX, startY);
	GameCallEx("BombScored");
}

protected func Entrance(pTo)
{
  // Wer sie sammelt, besitzt. Quasi.
	if(!Equal(GetOwner(pTo), NO_OWNER))
		SetOwner(GetOwner(pTo));

	// Wenn nicht erstes Objekt und wenn dieses eine Schnellfeuerwaffe ist, abbrechen!
	if(!Equal(Contents(nil,Contained()), this()))
		Contents(nil,Contained())->~Deselection(Contained());
 
	// Noch auswählen // TODO: Get auch nicht mehr, Zapper ist Schuld
	ShiftContents(pTo,nil,GetID(),nil);

  // Timer starten
  if(!((GetOCF(pTo) ?? 0) & (OCF_CrewMember ?? 0))) return(1);
  if(Equal(pTo, holder))
    AddEffect("BombTimer", this(), 1, 1, this(),nil, pTo, timer);
  else
    AddEffect("BombTimer", this(), 1, 1, this(),nil, pTo, 1000);
	
  GameCallEx("BombTaken");
}

protected func Hit(xDir,yDir)
{
	if(Or(Or(Or(GBackSolid(-6,nil), GBackSolid(6,nil)), GBackSolid(-2,4)), GBackSolid(2,4))) SetXDir(-((xDir ?? 0)/20));
	if(Or(Or(Or(GBackSolid(-4,3), GBackSolid(4,3)), GBackSolid(nil,6)), GBackSolid(nil,-6))) SetYDir(-((yDir ?? 0)/20));
	return(_inherited());
}

public func Deselection()
{ 
  // TODO: MICH SOLLTE MAN NICHT DESELEKTIEREN KÖNNEN 
	//Contained()->ShiftContents(0, 0, GetID(this()));
	// die Bombe sofort wieder auszuwählen, geht leider nicht. Dadurch flimmert es kurz :(
	ScheduleCall(this(),"SelectBomb",1); // TODO: Bestimmt auch Zappermurks. :/
	return(1);
}

protected func SelectBomb()
{
	ShiftContents(Contained(),nil,GetID(),nil);
}

/*---- Effekt für den Flug der Bombe ----*/

protected func FxBombFlightStart(pTarget)
{
	// Licht erstellen
	if(!pLight) pLight=AddLight(200,HSL(nil,255,255),pTarget);
	return(true);
}

protected func FxBombFlightTimer(pTarget,iEffectNumber,iEffectTime)
{
  // Licht effektiern
	if(pLight)
		pLight->ChangeColor(HSL(BoundBy(Abs(Sin((iEffectTime ?? 0)*2,120)),nil,120),128,200)); // TODO: Ist bestimmt hässlicher Zapper-Code.
	if(!Contained())
	{
		if((!Equal(GetOwner(), NO_OWNER))) // Irgendwer hat sie irgendwo hingeschmissen.
		{
			// Zeit
			if((S42MigrationStep(EffectVar(nil, pTarget, iEffectNumber), 1)) > 32*30)
			{
				pTarget -> Explode(25); // Draußen-Zeit erhöhen, schauen ob zu lang, evntl. bumm
				return;
			}
			// Effekte
			if( Equal((36*30 % (36*30 - (EffectVar(nil, pTarget, iEffectNumber) ?? 0))), nil))
			{
				CreateParticle("PSpark", nil, nil, nil, nil, 20, RGB(255,nil,nil), pTarget);
				Sound("Bip", nil, pTarget, 20);
			}
		}
		if(!pLight)
		{
			pLight=AddLight(200,HSL(nil,255,255),pTarget);
			pLight->SetPosition(pTarget->GetX(), pTarget->GetY());
		}
/*		// Sammelhilfe
		var pClonk = FindObject2(Find_OCF(OCF_CrewMember), Find_AtPoint());
		if(pClonk)
		{
			pClonk->Collect(pTarget);
			pTarget->Sound("Grab", 0, pTarget);
		}*/
	}
  else
	{
		if(EffectVar(nil, pTarget, iEffectNumber)) // Nicht draußen? Zeit zurücksetzen!
		  EffectVar(nil, pTarget, iEffectNumber) = nil;
		if(pLight) pLight->RemoveObject();
	}

	return(true); 
}

protected func FxBombFlightStop()
{
	if(pLight) RemoveObject(pLight); // Das Licht wegmachen.
}

/*---- Effekt für den Timer der Bombe ----*/

protected func FxBombTimerStart(object pTarget, int iNo, int iTemp, object pHolder, int iTimer)
{
  iNo ??= 0;
  iTemp ??= 0;
  iTimer ??= 0;
  if(iTemp) return;
  if(!Contained()) return(-1);
  holder = pHolder;
  timer = iTimer;
}

protected func FxBombTimerTimer()
{
  if(!Contained()) return(-1);
  S42MigrationStep(timer, -1, true);
  if(!timer) return(Explode(25));
  var angle = 90 * (timer ?? 0) / 1000;
  PlayerMessage(Contained()->GetOwner(), "<c %x>%d</c>", Contained(), RGB(Cos(angle, 255), Sin(angle, 255)), timer);
}
