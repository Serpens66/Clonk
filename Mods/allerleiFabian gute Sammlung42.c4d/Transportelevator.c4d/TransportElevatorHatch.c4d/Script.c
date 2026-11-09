/*-- Neues Objekt --*/

#strict 3

local seg1,seg2;

func Initialize() {
  SetAction("Closed");
  SetPosition(GetX(), (GetY() ?? 0)+6);
  DigFreeRect((GetX() ?? 0)-66, (GetY() ?? 0)-26, 132, 18);
//seg1=CreateObject(BAS1, -53, 0);
//seg2=CreateObject(BAS1, 53, 0);
  return(1);
}

func BeginMaskr()	{
	SetSolidMask(nil,nil,132,8);
}

func EndMaskr()	{
	SetSolidMask(nil,nil,nil,nil);
}

private func UpdateSolidMaskr()
{
  if (!!SEqual(GetAction(), "Open"))
    SetSolidMask((GetPhase() ?? 0) * 132, nil, 132, 8, nil, nil);
  if (!!SEqual(GetAction(), "Close"))
    SetSolidMask((GetDefCoreVal("Width", "DefCore", GetID()) ?? 0) - (GetPhase() ?? 0) * 132, nil, 132, 8, nil, nil);
}  

func Timerr()	{
	if(!!SEqual(GetAction(), "Opened"))
		{
		EndMaskr();
		}
	if(!!SEqual(GetAction(), "Closed"))
		{
		BeginMaskr();
		}

	if(Or(Or(FindObject(BELC, -40, -45, 80, 85), FindObject(SLP1, -40, -45, 80, 85)), FindObject(CHLP, -40, -45, 80, 85)))
		{
		if(!!SEqual(GetAction(), "Closed"))
			{
			SetAction("Open");
			Sound("SteelGate2");
			}
		}
	else
		{
		if(!!SEqual(GetAction(), "Opened"))
			{
			SetAction("Close");
			Sound("SteelGate2");
			}
		}
}

func Destruction()
{
if(seg1)RemoveObject(seg1);
if(seg2)RemoveObject(seg2);
_inherited();
}
