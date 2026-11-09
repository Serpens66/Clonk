/*-- Rahmen --*/
 
#strict 3
 
local smashed;
local width, height;
 
global func AddFrame(object pObj) {
	if(!pObj)
		if(!(pObj = this()))
			return(nil);
	var tmp = CreateObject(FRME,nil,25,GetOwner(pObj));
	ScheduleCall(tmp,"Set",1,nil,pObj);
	return(tmp);
}
 
global func RemoveFrame(object pObj) {
	if(!pObj)
		if(!(pObj = this()))
			return(nil);
	var pFrame = pObj->FindObject(FRME, 1,1);
	if(pFrame) RemoveObject(pFrame);
}
 
protected func Initialize() {
	SetClrModulation(RGBa(255,255,255,200));
	SetObjectBlitMode(GFX_BLIT_Additive);
	SetGraphics("Frame", this(),GetID(),4,1);
}
 
public func Set(object pTarget) {
	// *1000/(51-2-3) => Normiert auf 1000 und Verhältnis Zielobjekt <-> Fenster ohne Rahmen
	width = (GetObjWidth(pTarget) ?? 0)*1000/56;
	height = (GetObjHeight(pTarget) ?? 0)*1000/56;
	var width2 = (GetObjWidth(pTarget) ?? 0)*1000/61;
	var height2 = (GetObjHeight(pTarget) ?? 0)*1000/61;
 
	SetShape((-32*(width ?? 0))/1000,(-32*(height ?? 0))/1000,(64*(width ?? 0))/1000,(64*(height ?? 0))/1000);
	SetPosition(GetX(pTarget),GetY(pTarget));
 
	SetObjDrawTransform(width, nil, nil, nil, height, nil ,nil, nil);
	SetObjDrawTransform(width2, nil, nil, nil, height2, nil, nil, 4);
}
 
/* Farbmodulationen */
 
public func PanelColor(int rgba) {  // Frontscheibenmodulation
	rgba ??= 0;
	SetClrModulation(rgba);
}
 
public func FrameColor(int rgba) {  // Rahmenmodulation
	rgba ??= 0;
	SetClrModulation(rgba, nil, 4);
}
 
// Schaden
protected func Damage() {
	if(Or(!smashed, (And((smashed ?? 0) < 3, !Random(4)))))
	{
		S42MigrationStep(smashed, 1, true);
		CastObjects(SPRK, 7, 50);
		Sparks(15, RGB(120, 150, 135));
		Sound("CrystalHit*");
 
		var graphic = (Random(4) ?? 0)+1;
 
		SetGraphics(Format("%s%d","Broken",graphic),this(),GetID(),smashed,1,nil, 1);
		SetObjDrawTransform(width, nil,nil,nil, height,nil,nil,smashed);
	}
	if((GetDamage() ?? 0) >= 75)
	{
		smashed = 4;
		CastParticles("SplinterGlass", 1, 35, RandomX(-20,20), -5, 20, 20, GetClrModulation(), GetClrModulation());
		Sound("CrystalHit*");
 
		SetGraphics("BrokenGlass", this(),GetID(), nil,1,nil, 1);
		SetGraphics("BrokenEnd", this(),GetID(), 1,1,nil, 1);
		SetGraphics(nil,nil,nil,2);
		SetGraphics(nil,nil,nil,3);
	}
}

/* Serialisierung */

public func NoSerialize() { return !nil; }
