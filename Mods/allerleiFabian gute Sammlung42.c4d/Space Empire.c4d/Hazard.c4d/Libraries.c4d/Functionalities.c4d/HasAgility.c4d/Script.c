/* Has agility functionality */

#strict 3

/* Dieses Script bietet die Grundfunktionalität der Beweglichkeit des
    Hazardclonks. Um diese Funktionalität zu bedienen, im inkludierenden
    Script in folgenden Control*-Funktionen eine Zeile an der geeigneten
    Stelle hinzufügen. Und zwar bei

	-Up, -Left, -Right, -LeftDouble, -RightDouble, -UpDouble.
	
    Im Beispiel von ControlLeftSingle:
	
	if(ControlAgility("ControlLeftSingle")) return(1);

    Außerdem muss im StartCall von Jump-Aktionen die Funktion JumpStart();
    aufgerufen werden bzw. wenn diese schon im Script vorhanden ist am Ende
   _inherited aufgerufen werden.
   
   Agility kann mit der Leiterklettern-Funktionalität zusammenarbeiten.
*/
	
/* Control from the clonk */

private func ControlAgility(string strControl) {
	// return 0 if the rest of the clonks script should be executed
    
    var pCrew,i=1; // i bei 1 starten, weil der befehl nicht nochmal an den hauptclonk geschickt werden soll
    if(Equal(this(), GetCursor(GetOwner()))) // wenn dies der Hauptclonk ausführt, dann den Befehl an alle angewählten nicht-hauptclonks weitergeben. (damit befehl nur einmal ausgeführt wird)
    while (pCrew=GetCursor(GetOwner(),S42MigrationStep(i, 1, true))) // gib den Befehl an alle anderen angewählten Clonks weiter
    {
        pCrew->~ControlAgility(strControl);
    }
    
	// left
	if(!!SEqual(strControl, "ControlLeft")) {
		if(this()->IsJumping()) {
			SetDir(DIR_Left);
			SetComDir(COMD_Left); 
		}
		
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(nil, this, iEff) = COMD_Left;
		return nil;
	}
  
	// right
	if(!!SEqual(strControl, "ControlRight")) {
		if(this()->IsJumping()) {
			SetDir(DIR_Right);
			SetComDir(COMD_Right); 
		}
		
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(nil, this, iEff) = COMD_Right;
		return nil;
	}
  
	// left double
	if(!!SEqual(strControl, "ControlLeftDouble")) {
		if(MayFlip())
		{
			if(Equal(GetDir(), DIR_Left))
				ForwardFlip(-30);
			else
				BackFlip(-30);

			return 1;
		}
	}

	// right double
	if(!!SEqual(strControl, "ControlRightDouble")) {
		if(MayFlip())
		{
			if(Equal(GetDir(), DIR_Right))
				ForwardFlip(-30);
			else
				BackFlip(-30);

			return 1;
		}
	}
	
	// up
	if(!!SEqual(strControl, "ControlUp")) {
		if(And(WildcardMatch(GetAction(), "Scale*"), (GetActTime() ?? 0) < 5))
			EffectVar(2, this, AddEffect("ControlStack", this, 110, 5, this)) = true;

		if(GetEffect("ControlStack", this))
			EffectVar(1, this, GetEffect("ControlStack", this)) = COMD_Up;

		return nil;
	}
	
	if(!!SEqual(strControl, "ControlUpDouble")) {
		if(And((Or(this()->IsJumping(), !!SEqual(GetAction(), "JetpackFlight"))), !GetEffect("ExtraJump", this)))
			if((GetYDir() ?? 0) <= 5)
				return ExtraJump();
			
		return nil;
	}
	
}

private func MayFlip() {
	if(Or(this()->IsJumping(), !!SEqual(GetAction(), "JetpackFlight")))
	  if(!GetEffect("ExtraJump", this))
	    return(true);
}

/* JumpStart */

// Jump-StartCall: Spezialsprung?
// overloaded by Clonk
public func JumpStart(bool bBackflip)
{
	// Clonk auf festem Boden?
	bBackflip ??= false;
	if (!GetEffect("ExtraJump", this)) {
		var iEff, iComd, iStr = nil, iYDir = nil;
		if(iEff = GetEffect("ControlStack", this))
		{
			if(And(!Equal(EffectVar(1, this, iEff), COMD_Up), !bBackflip)) return;
			if(EffectVar(2, this, iEff)) { iYDir = -60; iStr = -40; }
			iComd = EffectVar(nil, this, iEff);

			RemoveEffect(nil, this, iEff);
			if(Equal(iComd, COMD_Left))
			{
				if(Equal(GetDir(), DIR_Left))
					ForwardFlip(iStr, nil, iYDir);
				else
					BackFlip(iStr, nil, iYDir);
			}
			if(Equal(iComd, COMD_Right))
			{
				if(Equal(GetDir(), DIR_Left))
					BackFlip(iStr, nil, iYDir);
				else
					ForwardFlip(iStr, nil, iYDir);
			}
		}
	}
	// für ladder
	_inherited(bBackflip);
}

// Leiter-Funktionalität benutzend

public func ReleaseLadderStop() {
	//Backflip! :D
	if(this()->ReleaseLadder(-20*((GetDir() ?? 0)*2-1))) {
	  SetAction("BackFlip");
	  ScheduleCall(nil, "BackFlipBoost", 1, 1);
	  AddEffect("ExtraJump", this, 111, 5, this);
	}
}
	
/* Jump functions */

protected func ExtraJump()
{
	var jump = (GetPhysical("Jump", nil) ?? 0) * 100 / (GetPhysical("Jump", nil,nil, GetID()) ?? 0);
	SetYDir(-40 * (jump ?? 0) / 100);
	SetXDir((GetXDir() ?? 0) / 2);
	AddEffect("ExtraJump", this, 111, 5, this);
}

protected func FxExtraJumpTimer()
{
	if(And(And(And(!WildcardMatch(GetAction(), "Jump*")
	, !WildcardMatch(GetAction(), "*Flip"))
	, !WildcardMatch(GetAction(), "*Flight"))
	, !SEqual(GetAction(), "Dive")))
			return -1;
}

protected func BackFlip(int iStr, bool fNoJumpCalc, int iYDir)
{
	iStr ??= 0;
	fNoJumpCalc ??= false;
	iYDir ??= 0;
	SetAction("BackFlip");
	ScheduleCall(nil, "FlipBoost", 1, 1, !GetDir(), iStr, fNoJumpCalc, iYDir);
        AddEffect("ExtraJump", this, 111, 5, this);
}

protected func ForwardFlip(int iStr, bool fNoJumpCalc, int iYDir)
{
	iStr ??= 0;
	fNoJumpCalc ??= false;
	iYDir ??= 0;
	if(And(!iStr, !iYDir)) { iStr = -20; iYDir = -45; }
	SetAction("ForwardFlip");
	ScheduleCall(nil, "FlipBoost", 1, 1, GetDir(), iStr, fNoJumpCalc, iYDir);
        AddEffect("ExtraJump", this, 111, 5, this);
}

protected func FlipBoost(int iDir, int iStr, bool fNoJumpCalc, int iYDir)
{
	iDir ??= 0;
	iStr ??= 0;
	fNoJumpCalc ??= false;
	iYDir ??= 0;
	if(!iYDir) iYDir = -24;
	iDir = (iDir ?? 0) *2 -1;
	var jump = (GetPhysical("Jump", nil) ?? 0) * 100 / (GetPhysical("Jump", nil,nil, GetID()) ?? 0);
	jump = BoundBy((jump ?? 0) + (iStr ?? 0), nil, 300);
	if(fNoJumpCalc) jump = iStr;
	SetXDir(40 * (iDir ?? 0) * (jump ?? 0) / 100);
	SetYDir((iYDir ?? 0) * (jump ?? 0) / 100);
}

//  Alte Funktion wird noch von ReleaseLadderStop benutzt
//   |
//  v

protected func BackFlipBoost()
{
  // Wenn die Sprungkraft gerade verändert ist, gilt das auch für den Backflip
  var jump = (GetPhysical("Jump", nil) ?? 0) * 100 / (GetPhysical("Jump", nil,nil, GetID()) ?? 0);
  SetXDir(40*((((GetXDir() ?? 0)>0) ?? 0)*2-1) * (jump ?? 0) / 100); // SetXDir(GetXDir()/*<-20*/* 3/2);
  SetYDir(-24 * (jump ?? 0) / 100);
}

// ^
// |

public func FxControlStackEffect(string newEffect, object pTarget, int iNo)
{
	iNo ??= 0;
	if(!SEqual(newEffect, "ControlStack")) return;
	ChangeEffect(nil, pTarget, iNo, nil, -1);
	return -2;
}
