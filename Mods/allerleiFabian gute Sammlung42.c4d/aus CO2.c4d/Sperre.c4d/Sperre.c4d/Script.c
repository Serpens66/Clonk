/*-- Neues Objekt --*/

#strict 3
local Enabled;
func Initialize() 
{
	Enable();
	return(1);
}


func Activate(int iPlayer)
{
	iPlayer ??= 0;
	MessageWindow("Erzeugt eine Sperre in der Mitte der landschaft, um zu verhindern, das sich die Gegner vor der Zeit einen Besuch abstatten",iPlayer);
}

func Enable()
{
	Enabled = 1;
	Message("");
}

func DerTimer()
{
	if(!Enabled) return;
	DrawParticleLine("MSpark", -150, nil, -150,LandscapeHeight(), 20, 100, RGBa(255,50,50,50), RGBa(255,255,50,100), -10);
	DrawParticleLine("MSpark", 150, nil, 150,LandscapeHeight(), 20, 100, RGBa(255,50,50,50), RGBa(255,255,50,100), -10);
	var pObj;
	for (pObj in FindObjects(Find_InRect(-150,-500,140,(LandscapeHeight() ?? 0) + 1000),Find_Exclude(this())))
	{
		if(!Equal(GetID(pObj), GetID())) {
		SetXDir((Abs(GetXDir(pObj)) ?? 0) * -2,pObj); // immer nach links schießen
		SetYDir((GetYDir(pObj) ?? 0) * -2,pObj);}
	}
	for (pObj in FindObjects(Find_InRect(10,-500,140,(LandscapeHeight() ?? 0) + 1000),Find_Exclude(this())))
	{
                if(!Equal(GetID(pObj), GetID())) {
		SetXDir((Abs(GetXDir(pObj)) ?? 0) * 2,pObj); // immer nach rechts schießen
		SetYDir((GetYDir(pObj) ?? 0) * -2,pObj);}
	}
	for (pObj in FindObjects(Find_InRect(-120,-500,240,(LandscapeHeight() ?? 0) + 1000),Find_Exclude(this())))
	{
                        if(Or(Equal(GetID(pObj), CLNK), Equal(GetID(pObj), HZCK))) // hier eine Warnmeldung für Clonks, die zu nahe kommen (denn mit graben kommt man z.b durch!)
			Message("Keinen Schritt weiter oder du stirbst!",pObj);
	}
	for (pObj in FindObjects(Find_InRect(-10,-500,20,(LandscapeHeight() ?? 0) + 1000),Find_Exclude(this())))
	{
                        if(!Equal(GetID(pObj), GetID())) // in diesem Bereich alles vernichten, was doch iwie so weit gekommen ist
			RemoveObject(pObj);
	}
}

func Disable()
{
	Enabled = nil;
	Message("<c ff0000> Die Spiele haben begonnen! </c>");
}